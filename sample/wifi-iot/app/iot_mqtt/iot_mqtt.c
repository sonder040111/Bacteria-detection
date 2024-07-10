/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - clarifications and/or documentation extension
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmsis_os2.h"
#include "ohos_init.h"

#include "MQTTClient.h"
#include "wifi_connect.h"

#include "iot_gpio.h"
#include "iot_errno.h"
#include "hi_io.h"

#include "hi_time.h"

#define MQTT_SERVERIP "192.168.0.179"
#define MQTT_SERVERPORT 1883
#define MQTT_CMD_TIMEOUT_MS 2000
#define MQTT_KEEP_ALIVE_MS 2000
#define MQTT_DELAY_2S 200
#define MQTT_DELAY_500_MS 50
#define MQTT_VERSION 3
#define MQTT_QOS 2
#define MQTT_TASK_STACK_SIZE (1024 * 10)

static unsigned char sendBuf[1000];
static unsigned char readBuf[1000];


#include <memory.h>
#include <signal.h>
#include <sys/time.h>
#include <cJSON.h>

#define EXAMPLE_PRODUCT_KEY			"k0k4xubB26S"
#define EXAMPLE_DEVICE_NAME			"Joanvin_XJBDJCQ"
#define EXAMPLE_DEVICE_SECRET       "87e8ec511e50fdb485bec350af8e3643"

/* declare the external function aiotMqttSign() */
extern int aiotMqttSign(const char *productKey, const char *deviceName, const char *deviceSecret, 
                     	char clientId[150], char username[65], char password[65]);

volatile int toStop = 0;


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
        oc_set_angle(2500);
    }
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


void cfinish(int sig)
{
	signal(SIGINT, NULL);
	toStop = 1;
}

void messageArrived(MessageData* md)
{
	MQTTMessage* message = md->message;

	printf("%.*s\t", md->topicName->lenstring.len, md->topicName->lenstring.data);
	printf("%.*s\n", (int)message->payloadlen, (char*)message->payload);

    cJSON *obj_root, *obj_cmdname, *obj_paras, *obj_para;

    // 使用 cJSON 解析 JSON 格式的指令
    obj_root = cJSON_Parse((const char*)message->payload);
    if (obj_root == NULL) {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }
    obj_cmdname = cJSON_GetObjectItem(obj_root, "method");
    if (obj_cmdname == NULL) {
        cJSON_Delete(obj_root);
    }
    if (strcmp(cJSON_GetStringValue(obj_cmdname), "thing.service.XJBDJCQ_Control") == 0) {
        obj_paras = cJSON_GetObjectItem(obj_root, "params");
        if (obj_paras == NULL) {
            cJSON_Delete(obj_root);
        }
        obj_para = cJSON_GetObjectItem(obj_paras, "Control");
        if (obj_paras == NULL) {
        cJSON_Delete(obj_root);
        }
        if (strcmp(cJSON_GetStringValue(obj_para), "D1ON") == 0) {
			IoTGpioSetOutputVal(2, 1);
			printf("D1 On!\r\n");
			sleep(1);
			IoTGpioSetOutputVal(2, 0);
			printf("D1 Off!\r\n");
        }
		 //泵2
		if (strcmp(cJSON_GetStringValue(obj_para), "D2ON") == 0) {
			IoTGpioSetOutputVal(8, 1);
			printf("D2 On!\r\n");
			sleep(1);
			IoTGpioSetOutputVal(8, 0);
			printf("D2 Off!\r\n");
		}
		//泵3
		if (strcmp(cJSON_GetStringValue(obj_para), "D3ON") == 0) {
			IoTGpioSetOutputVal(1, 1);
			printf("D3 On!\r\n");
			sleep(1);
			IoTGpioSetOutputVal(1, 0);
			printf("D3 Off!\r\n");
		}
		//泵4
		if (strcmp(cJSON_GetStringValue(obj_para), "D4ON") == 0) {
			IoTGpioSetOutputVal(0, 1);
			printf("D4 On!\r\n");
			sleep(1);
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
			oc_engine_turn_level();
		}
		if (strcmp(cJSON_GetStringValue(obj_para), "LEFT") == 0) {
			oc_engine_turn_right();
		}
		if (strcmp(cJSON_GetStringValue(obj_para), "RIGHT") == 0) {
			
			oc_engine_turn_left();
		}
		if (strcmp(cJSON_GetStringValue(obj_para), "LEVEL") == 0) {
			
			oc_engine_turn_zero();
		}
    }
    cJSON_Delete(obj_root);
}

/* main function */
int mqtt_main(void)
{
	int rc = 0;

	/* setup the buffer, it must big enough for aliyun IoT platform */
	unsigned char buf[1000];
	unsigned char readbuf[1000];

	Network n;
	MQTTClient c;
	char *host = EXAMPLE_PRODUCT_KEY".iot-as-mqtt.cn-shanghai.aliyuncs.com";
	short port = 443;

	const char *subTopic = "/"EXAMPLE_PRODUCT_KEY"/"EXAMPLE_DEVICE_NAME"/user/get";
	const char *pubTopic = "/"EXAMPLE_PRODUCT_KEY"/"EXAMPLE_DEVICE_NAME"/user/update";

	/* invoke aiotMqttSign to generate mqtt connect parameters */
	char clientId[150] = {0};
	char username[65] = {0};
	char password[65] = {0};

	if ((rc = aiotMqttSign(EXAMPLE_PRODUCT_KEY, EXAMPLE_DEVICE_NAME, EXAMPLE_DEVICE_SECRET, clientId, username, password) < 0)) {
		printf("aiotMqttSign -%0x4x\n", -rc);
		return -1;
	}
	printf("clientid: %s\n", clientId);
	printf("username: %s\n", username);
	printf("password: %s\n", password);

	signal(SIGINT, cfinish);
	signal(SIGTERM, cfinish);

	/* network init and establish network to aliyun IoT platform */
	NetworkInit(&n);
	rc = NetworkConnect(&n, host, port);
	printf("NetworkConnect %d\n", rc);

	/* init mqtt client */
	MQTTClientInit(&c, &n, 1000, buf, sizeof(buf), readbuf, sizeof(readbuf));
 
	/* set the default message handler */
	c.defaultMessageHandler = messageArrived;

	/* set mqtt connect parameter */
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
	data.willFlag = 0;
	data.MQTTVersion = 3;
	data.clientID.cstring = clientId;
	data.username.cstring = username;
	data.password.cstring = password;
	data.keepAliveInterval = 60;
	data.cleansession = 1;
	printf("Connecting to %s %d\n", host, port);

	rc = MQTTConnect(&c, &data);
	printf("MQTTConnect %d, Connect aliyun IoT Cloud Success!\n", rc);
    
    printf("Subscribing to %s\n", subTopic);
	rc = MQTTSubscribe(&c, subTopic, 1, messageArrived);
	printf("MQTTSubscribe %d\n", rc);

	int cnt = 0;
    unsigned int msgid = 0;
	while (!toStop)
	{
		MQTTYield(&c, 1000);	

		if (++cnt % 5 == 0) {
			MQTTMessage msg = {
				QOS1, 
				0,
				0,
				0,
				"Hello world",
				strlen("Hello world"),
			};
            msg.id = ++msgid;
			rc = MQTTPublish(&c, pubTopic, &msg);
			printf("MQTTPublish %d, msgid %d\n", rc, msgid);
		}
	}

	printf("Stopping\n");

	MQTTDisconnect(&c);
	NetworkDisconnect(&n);

	return 0;
}
//*************************************************************************************************


static void MQTTDemoTask(void)
{
    WifiConnect("joanvin", "3218821505");
	goip_init();
    printf("Starting ...\n");
    mqtt_main();

}
static void MQTTDemo(void)
{
    osThreadAttr_t attr;

    attr.name = "MQTTDemoTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = MQTT_TASK_STACK_SIZE;
    attr.priority = 24;

    if (osThreadNew((osThreadFunc_t)MQTTDemoTask, NULL, &attr) == NULL) {
        printf("[MQTT_Demo] Failed to create MQTTDemoTask!\n");
    }
}

APP_FEATURE_INIT(MQTTDemo);