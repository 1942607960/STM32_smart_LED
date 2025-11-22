#ifndef __ds1302_H 
#define __ds1302_H  
 
#include "sys.h" 
#include "stm32f10x.h"

extern unsigned char BMP19[];
extern volatile u8 time_buf[8];//初始时间2010年6月1号23点59分55秒 星期二
extern u8 RealTime_Display_flag;       //时间显示标志位
extern u8 temperature;  //温度
extern u8 humidity;     //湿度
 
 //---------------DS1302端口定义----------------- 
/***************根据自己需求更改****************/
#define DS1302_CLK  	RCC_APB2Periph_GPIOB
#define DS1302_CE  		RCC_APB2Periph_GPIOB
#define DS1302_DATA  	RCC_APB2Periph_GPIOB


#define DS1302_CE_PORT  	GPIOB
#define DS1302_CE_PIN		GPIO_Pin_7
#define DS1302_DATA_PORT	GPIOB
#define DS1302_DATA_PIN   	GPIO_Pin_8
#define DS1302_SCLK_PORT   	GPIOB
#define DS1302_SCLK_PIN  	GPIO_Pin_9 
 
 
 
 
 
 
#define CE_L	GPIO_ResetBits(DS1302_CE_PORT,DS1302_CE_PIN)//CE
#define CE_H	GPIO_SetBits(DS1302_CE_PORT,DS1302_CE_PIN)

#define DATA_L	GPIO_ResetBits(DS1302_SCLK_PORT,DS1302_DATA_PIN)//DATA
#define DATA_H	GPIO_SetBits(DS1302_SCLK_PORT,DS1302_DATA_PIN)

#define SCLK_L	GPIO_ResetBits(DS1302_DATA_PORT,DS1302_SCLK_PIN)//SCLK
#define SCLK_H	GPIO_SetBits(DS1302_DATA_PORT,DS1302_SCLK_PIN)
 
struct TIMEData
{
	u16 year;
	u8  month;
	u8  day;
	u8  hour;
	u8  minute;
	u8  second;
	u8  week;
};//创建TIMEData结构体方便存储时间日期数据

extern struct TIMEData TimeData;//全局变量
void ds1302_gpio_init(void);//ds1302端口初始化
void ds1302_write_onebyte(u8 data);//向ds1302发送一字节数据
void ds1302_wirte_rig(u8 address,u8 data);//向指定寄存器写一字节数据
u8 ds1302_read_rig(u8 address);//从指定寄存器读一字节数据
void ds1032_init(void);//ds1302初始化函数
void ds1032_DATAOUT_init(void);//IO端口配置为输出
void ds1032_DATAINPUT_init(void);//IO端口配置为输入
void ds1032_read_time(void);//从ds1302读取实时时间（BCD码）
void ds1032_read_realTime(void);//将BCD码转化为十进制数据
void ds1302_write_time(void); //设定时间日期信息 写入芯片
void RealTime_Display_Init(void);//上电初始化时间显示
void RealTime_Display(void);     //主程序实时时间日期实时刷新显示	
void Modify_RealTime_Display(void); //修改后时间日期刷新一次显示
 
#endif
