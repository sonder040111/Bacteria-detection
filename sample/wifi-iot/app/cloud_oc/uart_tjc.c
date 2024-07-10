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
#include <string.h>
#include <unistd.h>

#include <stdlib.h>

#include "iot_gpio.h"
#include "iot_errno.h"
#include "hi_io.h"

#include "hi_time.h"

#include "cmsis_os2.h"
#include "iot_errno.h"
#include "iot_uart.h"
#include "ohos_init.h"

extern float voltage;


#define UART_TASK_STACK_SIZE (1024 * 8)
#define UART_TASK_PRIO 25
#define UART_BUFF_SIZE 4
#define WIFI_IOT_UART_IDX_2 2
#define TASK_DELAY_1S 1000000

//static const char *data = "Hello, BearPi!\r\n";

/**
 * @brief uart task send data through uart1 and receive data through uart1
 *
 */


//输出20000微秒的脉冲信号(x微秒高电平,20000-x微秒低电平)
void set_angle( unsigned int duty) {
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
void engine_turn_zero(void)
{
    for (int i = 0; i <10; i++) {
        set_angle(500);
    }
}


void engine_turn_left(void)
{
    for (int i = 0; i <10; i++) {
        set_angle(1000);
    }
}

/*Steering gear turn right
1、依据角度与脉冲的关系，设置高电平时间为2000微秒
2、发送10次脉冲信号，控制舵机向右旋转45度
*/
void engine_turn_right(void)
{
    for (int i = 0; i <10; i++) {
        set_angle(2000);
    }
}

/*Steering gear return to middle
1、依据角度与脉冲的关系，设置高电平时间为1500微秒
2、发送10次脉冲信号，控制舵机居中
*/
void regress_middle(void)
{
    for (int i = 0; i <10; i++) {
        set_angle(1500);
    }
}

void engine_turn_level(void)
{
    for (int i = 0; i <10; i++) {
        set_angle(2500);
    }
}




static void UartTask(void)
{
    uint8_t uart_buff[UART_BUFF_SIZE] = { 0 };
    uint8_t *uart_buff_ptr = uart_buff;
    uint8_t ret;

    IotUartAttribute uart_attr = {

        // baud_rate: 9600
        .baudRate = 115200,

        // data_bits: 8bits
        .dataBits = 8,
        .stopBits = 1,
        .parity = 0,
    };

    // Initialize uart driver
    ret = IoTUartInit(WIFI_IOT_UART_IDX_2, &uart_attr);
    if (ret != IOT_SUCCESS) {
        printf("Failed to init uart! Err code = %d\n", ret);
        return;
    }

    while (1) {
        printf("=======================================\r\n");
        printf("*************UART_example**************\r\n");
        printf("=======================================\r\n");

        

        char str[20];
        char *data =str;
        sprintf(data,"MAIN.t0.txt=\"%.1f\"\xff\xff\xff",voltage*10);
        IoTUartWrite(WIFI_IOT_UART_IDX_2, (unsigned char *)data, strlen(data));
        sprintf(data,"add MAIN.s0.id,0,%.0f\xff\xff\xff",voltage*10);
        IoTUartWrite(WIFI_IOT_UART_IDX_2, (unsigned char *)data, strlen(data));

        // receive data through uart1
        IoTUartRead(WIFI_IOT_UART_IDX_2, uart_buff_ptr, UART_BUFF_SIZE);
        if (uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X02)
        {
            IoTGpioSetOutputVal(2, 1);
            printf("D1 On!\r\n");
            sleep(1);
            IoTGpioSetOutputVal(2, 0);
            printf("D1 Off!\r\n");
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X08)
        {
            IoTGpioSetOutputVal(8, 1);
            printf("D2 On!\r\n");
            sleep(1);
            IoTGpioSetOutputVal(8, 0);
            printf("D2 Off!\r\n");
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X01)
        {
            IoTGpioSetOutputVal(1, 1);
            printf("D3 On!\r\n");
            sleep(1);
            IoTGpioSetOutputVal(1, 0);
            printf("D3 Off!\r\n");
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X00)
        {
            IoTGpioSetOutputVal(0, 1);
            printf("D4 On!\r\n");
            sleep(1);
            IoTGpioSetOutputVal(0, 0);
            printf("D4 Off!\r\n");
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X10)
        {
            IoTGpioSetOutputVal(10, 1);
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X30)
        {
            IoTGpioSetOutputVal(10, 0);
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X14)
        {
            IoTGpioSetOutputVal(14, 1);
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X34)
        {
            IoTGpioSetOutputVal(14, 0);
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X07)
        {
            IoTGpioSetOutputVal(7, 1);
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X37)
        {
            IoTGpioSetOutputVal(7, 0);
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X10)
        {
            IoTGpioSetOutputVal(10, 1);
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X30)
        {
            IoTGpioSetOutputVal(10, 0);
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X09)
        {
            engine_turn_level();
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X19)
        {
            engine_turn_right();
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X29)
        {
            engine_turn_left();
        }
        if(uart_buff[UART_BUFF_SIZE-1]==0XFF&&uart_buff[UART_BUFF_SIZE-2]==0XFF&&uart_buff[UART_BUFF_SIZE-3]==0XFF&&uart_buff[UART_BUFF_SIZE-4]==0X39)
        {
            
            engine_turn_zero();
        }

        printf("Uart1 read data:%s\n", uart_buff_ptr);
        usleep(TASK_DELAY_1S);
        for(int i=0;i<4;i++)
        {
            uart_buff[i]=0x00;
        }

    }
}

/**
 * @brief Main Entry of the UART Example
 *
 */
static void UartExampleEntry(void)
{
    osThreadAttr_t attr;

    attr.name = "UartTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = UART_TASK_STACK_SIZE;
    attr.priority = UART_TASK_PRIO;

    if (osThreadNew((osThreadFunc_t)UartTask, NULL, &attr) == NULL) {
        printf("Failed to create UartTask!\n");
    }
}

APP_FEATURE_INIT(UartExampleEntry);