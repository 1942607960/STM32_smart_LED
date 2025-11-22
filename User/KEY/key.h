#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f10x.h"

//#define KEY_DEC     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) //读取按键1 数据减少
//#define KEY_ADJ     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) //读取按键2 校准时间
//#define KEY_ADD     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) //读取按键3 数据增加
//#define KEY_ALM     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) //读取按键4 闹钟设定


#define KEY0    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//读取按键0
#define KEY1    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//读取按键1
#define KEY2    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//读取按键2
#define KEY3    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//读取按键3

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define KEY2_PRES	3		//KEY2
#define KEY3_PRES	4		//KEY2

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8 mode);

#endif

