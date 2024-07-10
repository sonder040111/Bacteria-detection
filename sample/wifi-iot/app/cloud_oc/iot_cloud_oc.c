/*
 * Copyright (c) 2020 Nanjing Xiaoxiongpai Intelligent Technology Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cmsis_os2.h"
#include "ohos_init.h"

#include "iot_gpio.h"
#include "iot_errno.h"
#include "hi_io.h"

#include "hi_time.h"

#include <dtls_al.h>
#include <mqtt_al.h>
#include <oc_mqtt_al.h>
#include <oc_mqtt_profile.h>
#include "E53_IA1.h"
#include "wifi_connect.h"

extern float voltage;

#define CONFIG_WIFI_SSID "joanvin" // 修改为自己的WiFi 热点账号

#define CONFIG_WIFI_PWD "3218821505" // 修改为自己的WiFi 热点密码

#define CONFIG_APP_SERVERIP "117.78.5.125"

#define CONFIG_APP_SERVERPORT "1883"

#define CONFIG_APP_DEVICEID "647d222dec46a256bca57674_XJBDJCQ_01" // 替换为注册设备后生成的deviceid

#define CONFIG_APP_DEVICEPWD "3218821505" // 替换为注册设备后生成的密钥

#define CONFIG_APP_LIFETIME 60 // seconds

#define CONFIG_QUEUE_TIMEOUT (5 * 1000)

#define MSGQUEUE_COUNT 16
#define MSGQUEUE_SIZE 10
#define CLOUD_TASK_STACK_SIZE (1024 * 10)
#define CLOUD_TASK_PRIO 24
#define SENSOR_TASK_STACK_SIZE (1024 * 4)
#define SENSOR_TASK_PRIO 25
#define TASK_DELAY 3

osMessageQueueId_t mid_MsgQueue; // message queue id
typedef enum {
    en_msg_cmd = 0,
    en_msg_report,
    en_msg_conn,
    en_msg_disconn,
} en_msg_type_t;

typedef struct {
    char *request_id;
    char *payload;
} cmd_t;

typedef struct {
    int lum;
    int temp;
    int hum;
} report_t;

typedef struct {
    en_msg_type_t msg_type;
    union {
        cmd_t cmd;
        report_t report;
    } msg;
} app_msg_t;

typedef struct {
    osMessageQueueId_t app_msg;
    int connected;
    int led;
    int motor;
} app_cb_t;
static app_cb_t g_app_cb;

void oc_set_angle( unsigned int duty) {
    IoTGpioSetDir(9, IOT_GPIO_DIR_OUT);//设置GPIO2为输出模式

    //GPIO2输出x微秒高电平
    IoTGpioSetOutputVal(9, 1);
    hi_udelay(duty);

    //GPIO2输出20000-x微秒低电平
    IoTGpioSetOutputVal(9, 0);
    hi_udelay(20000 - duty);
}

/*Steering gear turn left
1、依据角度与脉冲的关系，设置高电平时间为1000微秒
2、发送10次脉冲信号，控制舵机向左旋转0度
*/
void oc_engine_turn_zero(void)
{
    for (int i = 0; i <10; i++) {
        oc_set_angle(500);
    }
}


void oc_engine_turn_left(void)
{
    for (int i = 0; i <10; i++) {
        oc_set_angle(1000);
    }
}

/*Steering gear turn right
1、依据角度与脉冲的关系，设置高电平时间为2000微秒
2、发送10次脉冲信号，控制舵机向右旋转45度
*/
void oc_engine_turn_right(void)
{
    for (int i = 0; i <10; i++) {
        oc_set_angle(2000);
    }
}

/*Steering gear return to middle
1、依据角度与脉冲的关系，设置高电平时间为1500微秒
2、发送10次脉冲信号，控制舵机居中
*/


void oc_engine_turn_level(void)
{
    for (int i = 0; i <10; i++) {
        oc_set_angle(2000);
    }
}


static void deal_report_msg(report_t *report)
{
    oc_mqtt_profile_service_t service;
    oc_mqtt_profile_kv_t temperature;
    oc_mqtt_profile_kv_t humidity;
    oc_mqtt_profile_kv_t luminance;
    oc_mqtt_profile_kv_t led;
    oc_mqtt_profile_kv_t motor;

    if (g_app_cb.connected != 1) {
        return;
    }

    service.event_time = NULL;
    service.service_id = "Agriculture";
    service.service_property = &temperature;
    service.nxt = NULL;

    temperature.key = "Temperature";
    temperature.value = &report->temp;
    temperature.type = EN_OC_MQTT_PROFILE_VALUE_INT;
    temperature.nxt = &humidity;

    humidity.key = "Humidity";
    humidity.value = &report->hum;
    humidity.type = EN_OC_MQTT_PROFILE_VALUE_INT;
    humidity.nxt = &luminance;

    luminance.key = "Luminance";
    luminance.value = &report->lum;
    luminance.type = EN_OC_MQTT_PROFILE_VALUE_INT;
    luminance.nxt = &led;

    led.key = "LightStatus";
    led.value = g_app_cb.led ? "ON" : "OFF";
    led.type = EN_OC_MQTT_PROFILE_VALUE_STRING;
    led.nxt = &motor;

    motor.key = "MotorStatus";
    motor.value = g_app_cb.motor ? "ON" : "OFF";
    motor.type = EN_OC_MQTT_PROFILE_VALUE_STRING;
    motor.nxt = NULL;

    oc_mqtt_profile_propertyreport(NULL, &service);
    return;
}

// use this function to push all the message to the buffer
static int msg_rcv_callback(oc_mqtt_profile_msgrcv_t *msg)
{
    int ret = 0;
    char *buf;
    int buf_len;
    app_msg_t *app_msg;

    if ((msg == NULL) || (msg->request_id == NULL) || (msg->type != EN_OC_MQTT_PROFILE_MSG_TYPE_DOWN_COMMANDS)) {
        return ret;
    }

    buf_len = sizeof(app_msg_t) + strlen(msg->request_id) + 1 + msg->msg_len + 1;
    buf = malloc(buf_len);
    if (buf == NULL) {
        return ret;
    }
    app_msg = (app_msg_t *)buf;
    buf += sizeof(app_msg_t);

    app_msg->msg_type = en_msg_cmd;
    app_msg->msg.cmd.request_id = buf;
    buf_len = strlen(msg->request_id);
    buf += buf_len + 1;
    memcpy_s(app_msg->msg.cmd.request_id, buf_len, msg->request_id, buf_len);
    app_msg->msg.cmd.request_id[buf_len] = '\0';

    buf_len = msg->msg_len;
    app_msg->msg.cmd.payload = buf;
    memcpy_s(app_msg->msg.cmd.payload, buf_len, msg->msg, buf_len);
    app_msg->msg.cmd.payload[buf_len] = '\0';

    ret = osMessageQueuePut(g_app_cb.app_msg, &app_msg, 0U, CONFIG_QUEUE_TIMEOUT);
    if (ret != 0) {
        free(app_msg);
    }

    return ret;
}

static void oc_cmdresp(cmd_t *cmd, int cmdret)
{
    oc_mqtt_profile_cmdresp_t cmdresp;
    ///< do the response
    cmdresp.paras = NULL;
    cmdresp.request_id = cmd->request_id;
    cmdresp.ret_code = cmdret;
    cmdresp.ret_name = NULL;
    (void)oc_mqtt_profile_cmdresp(NULL, &cmdresp);
}

///< COMMAND DEAL
#include <cJSON.h>
static void deal_light_cmd(cmd_t *cmd, cJSON *obj_root)
{
    cJSON *obj_paras;
    cJSON *obj_para;
    int cmdret;

    obj_paras = cJSON_GetObjectItem(obj_root, "paras");
    if (obj_paras == NULL) {
        cJSON_Delete(obj_root);
    }
    obj_para = cJSON_GetObjectItem(obj_paras, "D12");
    if (obj_paras == NULL) {
        cJSON_Delete(obj_root);
    }
    //泵1
    if (strcmp(cJSON_GetStringValue(obj_para), "D1ON") == 0) {
        IoTGpioSetOutputVal(2, 1);
        printf("D1 On!\r\n");
        sleep(0.5);
        IoTGpioSetOutputVal(2, 0);
        printf("D1 Off!\r\n");
    }
    //泵2
    if (strcmp(cJSON_GetStringValue(obj_para), "D2ON") == 0) {
        IoTGpioSetOutputVal(8, 1);
        printf("D2 On!\r\n");
        sleep(0.5);
        IoTGpioSetOutputVal(8, 0);
        printf("D2 Off!\r\n");
    }
    //泵3
    if (strcmp(cJSON_GetStringValue(obj_para), "D3ON") == 0) {
        IoTGpioSetOutputVal(1, 1);
        printf("D3 On!\r\n");
        sleep(0.5);
        IoTGpioSetOutputVal(1, 0);
        printf("D3 Off!\r\n");
    }
    //泵4
    if (strcmp(cJSON_GetStringValue(obj_para), "D4ON") == 0) {
        IoTGpioSetOutputVal(0, 1);
        printf("D4 On!\r\n");
        sleep(0.5);
        IoTGpioSetOutputVal(0, 0);
        printf("D4 Off!\r\n");
    }
    //气泵
    if (strcmp(cJSON_GetStringValue(obj_para), "DBON") == 0) {
        IoTGpioSetOutputVal(10, 1);
        printf("DB On!\r\n");
    }
    if (strcmp(cJSON_GetStringValue(obj_para), "DBOFF") == 0) {
        IoTGpioSetOutputVal(10, 0);
        printf("DB Off!\r\n");
    }
    //废液泵
    if (strcmp(cJSON_GetStringValue(obj_para), "SD12ON") == 0) {
        IoTGpioSetOutputVal(7, 1);
        printf("SD12 On!\r\n");
    }
    if (strcmp(cJSON_GetStringValue(obj_para), "SD12OFF") == 0) {
        IoTGpioSetOutputVal(7, 0);
        printf("SD12 Off!\r\n");
    }
    //阀
    if (strcmp(cJSON_GetStringValue(obj_para), "ISO9ON") == 0) {
        IoTGpioSetOutputVal(14, 1);
        printf("ISO9 On!\r\n");
    }
    if (strcmp(cJSON_GetStringValue(obj_para), "ISO9OFF") == 0) {
        IoTGpioSetOutputVal(14, 0);
        printf("ISO9 Off!\r\n");
    }
    if (strcmp(cJSON_GetStringValue(obj_para), "ISO9OFF") == 0) {
        IoTGpioSetOutputVal(14, 0);
        printf("ISO9 Off!\r\n");
    }
    if (strcmp(cJSON_GetStringValue(obj_para), "ZERO") == 0) {
        oc_engine_turn_zero();
    }
    if (strcmp(cJSON_GetStringValue(obj_para), "LEFT") == 0) {
        oc_engine_turn_left();
    }
    if (strcmp(cJSON_GetStringValue(obj_para), "RIGHT") == 0) {
        oc_engine_turn_right();
    }
    if (strcmp(cJSON_GetStringValue(obj_para), "LEVEL") == 0) {
        oc_engine_turn_level();
    }
    

    cmdret = 0;
    oc_cmdresp(cmd, cmdret);

    cJSON_Delete(obj_root);
    return;
}

static void deal_motor_cmd(cmd_t *cmd, cJSON *obj_root)
{
    cJSON *obj_paras;
    cJSON *obj_para;
    int cmdret;

    obj_paras = cJSON_GetObjectItem(obj_root, "Paras");
    if (obj_paras == NULL) {
        cJSON_Delete(obj_root);
    }
    obj_para = cJSON_GetObjectItem(obj_paras, "Motor");
    if (obj_para == NULL) {
        cJSON_Delete(obj_root);
    }
    ///< operate the Motor here
    if (strcmp(cJSON_GetStringValue(obj_para), "ON") == 0) {
        g_app_cb.motor = 1;
        MotorStatusSet(ON);
        printf("Motor On!\r\n");
    } else {
        g_app_cb.motor = 0;
        MotorStatusSet(OFF);
        printf("Motor Off!\r\n");
    }
    cmdret = 0;
    oc_cmdresp(cmd, cmdret);

    cJSON_Delete(obj_root);
    return;
}

static void deal_cmd_msg(cmd_t *cmd)
{
    cJSON *obj_root;
    cJSON *obj_cmdname;

    int cmdret = 1;
    obj_root = cJSON_Parse(cmd->payload);
    if (obj_root == NULL) {
        oc_cmdresp(cmd, cmdret);
    }
    obj_cmdname = cJSON_GetObjectItem(obj_root, "command_name");
    if (obj_cmdname == NULL) {
        cJSON_Delete(obj_root);
    }
    if (strcmp(cJSON_GetStringValue(obj_cmdname), "XJbdjcq_D12") == 0) {
        deal_light_cmd(cmd, obj_root);
    } else if (strcmp(cJSON_GetStringValue(obj_cmdname), "Agriculture_Control_Motor") == 0) {
        deal_motor_cmd(cmd, obj_root);
    }

    return;
}

static void goip_init(void)
{
    IoTGpioInit(2);
    IoTGpioInit(7);
    IoTGpioInit(8);
    IoTGpioInit(1);
    IoTGpioInit(0);
    IoTGpioInit(10);
    IoTGpioInit(14);
    IoTGpioInit(9);
    hi_io_set_func(2,HI_IO_FUNC_GPIO_2_GPIO);
    hi_io_set_func(7,HI_IO_FUNC_GPIO_7_GPIO);
    hi_io_set_func(8,HI_IO_FUNC_GPIO_8_GPIO);
    hi_io_set_func(1,HI_IO_FUNC_GPIO_1_GPIO);
    hi_io_set_func(0,HI_IO_FUNC_GPIO_0_GPIO);
    hi_io_set_func(10,HI_IO_FUNC_GPIO_10_GPIO);
    hi_io_set_func(14,HI_IO_FUNC_GPIO_14_GPIO);
    hi_io_set_func(9, HI_IO_FUNC_GPIO_9_GPIO);
    IoTGpioSetDir(2, IOT_GPIO_DIR_OUT);
    IoTGpioSetDir(7, IOT_GPIO_DIR_OUT);
    IoTGpioSetDir(8, IOT_GPIO_DIR_OUT);
    IoTGpioSetDir(1, IOT_GPIO_DIR_OUT);
    IoTGpioSetDir(0, IOT_GPIO_DIR_OUT);
    IoTGpioSetDir(10, IOT_GPIO_DIR_OUT);
    IoTGpioSetDir(14, IOT_GPIO_DIR_OUT);    
}

static int CloudMainTaskEntry(void)
{
    app_msg_t *app_msg;
    uint32_t ret;

    WifiConnect(CONFIG_WIFI_SSID, CONFIG_WIFI_PWD);
    dtls_al_init();
    mqtt_al_init();
    oc_mqtt_init();
    goip_init();

    g_app_cb.app_msg = osMessageQueueNew(MSGQUEUE_COUNT, MSGQUEUE_SIZE, NULL);
    if (g_app_cb.app_msg == NULL) {
        printf("Create receive msg queue failed");
    }
    oc_mqtt_profile_connect_t connect_para;
    (void)memset_s(&connect_para, sizeof(connect_para), 0, sizeof(connect_para));

    connect_para.boostrap = 0;
    connect_para.device_id = CONFIG_APP_DEVICEID;
    connect_para.device_passwd = CONFIG_APP_DEVICEPWD;
    connect_para.server_addr = CONFIG_APP_SERVERIP;
    connect_para.server_port = CONFIG_APP_SERVERPORT;
    connect_para.life_time = CONFIG_APP_LIFETIME;
    connect_para.rcvfunc = msg_rcv_callback;
    connect_para.security.type = EN_DTLS_AL_SECURITY_TYPE_NONE;
    ret = oc_mqtt_profile_connect(&connect_para);
    if ((ret == (int)en_oc_mqtt_err_ok)) {
        g_app_cb.connected = 1;
        printf("oc_mqtt_profile_connect succed!\r\n");
    } else {
        printf("oc_mqtt_profile_connect faild!\r\n");
    }

    while (1) {
        app_msg = NULL;
        (void)osMessageQueueGet(g_app_cb.app_msg, (void **)&app_msg, NULL, 0xFFFFFFFF);
        if (app_msg != NULL) {
            switch (app_msg->msg_type) {
                case en_msg_cmd:
                    deal_cmd_msg(&app_msg->msg.cmd);
                    break;
                case en_msg_report:
                    deal_report_msg(&app_msg->msg.report);
                    break;
                default:
                    break;
            }
            free(app_msg);
        }
    }
    return 0;
}

static int SensorTaskEntry(void)
{
    app_msg_t *app_msg;
    int ret;
    E53IA1Data data;

    ret = E53IA1Init();
    if (ret != 0) {
        printf("E53_IA1 Init failed!\r\n");
        return;
    }
    while (1) {
        ret = E53IA1ReadData(&data);
        if (ret != 0) {
            printf("E53_IA1 Read Data failed!\r\n");
            return;
        }
        app_msg = malloc(sizeof(app_msg_t));
        printf("SENSOR:lum:%.2f temp:%.2f hum:%.2f\r\n", data.Lux, data.Temperature, data.Humidity);
        if (app_msg != NULL) {
            app_msg->msg_type = en_msg_report;
            app_msg->msg.report.hum = (int)data.Humidity;
            app_msg->msg.report.lum = (int)data.Lux;
            app_msg->msg.report.temp = (int)data.Temperature;
            if (osMessageQueuePut(g_app_cb.app_msg, &app_msg, 0U, CONFIG_QUEUE_TIMEOUT != 0)) {
                free(app_msg);
            }
        }
        sleep(TASK_DELAY);
    }
    return 0;
}

static void IotMainTaskEntry(void)
{
    osThreadAttr_t attr;

    attr.name = "CloudMainTaskEntry";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = CLOUD_TASK_STACK_SIZE;
    attr.priority = CLOUD_TASK_PRIO;

    if (osThreadNew((osThreadFunc_t)CloudMainTaskEntry, NULL, &attr) == NULL) {
        printf("Failed to create CloudMainTaskEntry!\n");
    }
    attr.stack_size = SENSOR_TASK_STACK_SIZE;
    attr.priority = SENSOR_TASK_PRIO;
    attr.name = "SensorTaskEntry";
    if (osThreadNew((osThreadFunc_t)SensorTaskEntry, NULL, &attr) == NULL) {
        printf("Failed to create SensorTaskEntry!\n");
    }
}

APP_FEATURE_INIT(IotMainTaskEntry);