#include "bsp_Ultrasonic.h"
#include "bsp_led.h"
#include "bsp_Timbase.h"
#include "delay.h"
float Senor_Using(void)
{
float length=0,sum=0;
u16 tim;   //高电平总时间
int i=0;
/*测5次数据计算一次平均值*/
while(i!=5)
{
	
	  GPIO_SetBits(GPIOA,GPIO_Pin_8); //拉高信号，作为触发信号
		delay_us(20); //高电平信号超过10us
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	
while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0);
TIM_Cmd(TIM2,ENABLE);//回响信号到来，开启定时器计数

i+=1; //每收到一次回响信号+1，收到5次就计算均值
while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1);//回响信号消失
TIM_Cmd(TIM2,DISABLE);//关闭定时器

tim=TIM_GetCounter(TIM2);//获取计TIM2数寄存器中的计数值，一边计算回响信号时间

length=(tim+overcount*1000)/58.0;//通过回响信号计算距离

sum=length+sum;  //5次测量的总距离
TIM2->CNT=0;  //将TIM2计数寄存器的计数值清零
overcount=0;  //中断溢出次数清零
delay_ms(10);
}
length=sum/5; //平均距离
return length;//距离作为函数返回值
}
