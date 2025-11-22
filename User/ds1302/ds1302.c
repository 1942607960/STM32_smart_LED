#include "ds1302.h"
#include "sys.h"
#include "delay.h"
#include "bsp_oled.h"
	
struct TIMEData TimeData;
u8 read_time[7];
volatile u8 time_buf[8] = {0};//初始时间2010年6月1号23点59分55秒 星期二
 
void ds1302_gpio_init(void)//CE,SCLK端口初始化
{
GPIO_InitTypeDef GPIO_InitStructure;
	
RCC_APB2PeriphClockCmd(DS1302_CE, ENABLE);
GPIO_InitStructure.GPIO_Pin = DS1302_CE_PIN; //  RST
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
GPIO_Init(DS1302_CE_PORT, &GPIO_InitStructure);
GPIO_ResetBits(DS1302_CE_PORT,DS1302_CE_PIN); 
 
RCC_APB2PeriphClockCmd(DS1302_CLK, ENABLE);
GPIO_InitStructure.GPIO_Pin = DS1302_SCLK_PIN; //  CLK
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
GPIO_Init(DS1302_SCLK_PORT, &GPIO_InitStructure);
GPIO_ResetBits(DS1302_SCLK_PORT,DS1302_SCLK_PIN); 
}
 
void ds1032_DATAOUT_init(void)//配置双向I/O端口为输出态
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(DS1302_DATA, ENABLE);
 
GPIO_InitStructure.GPIO_Pin = DS1302_DATA_PIN; //  DATA
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(DS1302_DATA_PORT, &GPIO_InitStructure);
GPIO_ResetBits(DS1302_DATA_PORT,DS1302_DATA_PIN);
}
 
void ds1032_DATAINPUT_init(void)//配置双向I/O端口为输入态
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(DS1302_DATA, ENABLE);
GPIO_InitStructure.GPIO_Pin = DS1302_DATA_PIN; // DATA
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(DS1302_DATA_PORT, &GPIO_InitStructure);
}
 
void ds1302_write_onebyte(u8 data)//向DS1302发送一字节数据
{
u8 count=0;
ds1032_DATAOUT_init();

SCLK_L;
for(count=0;count<8;count++)
	{	SCLK_L;
		if(data&0x01)
		{DATA_H;}
		else{DATA_L;}//先准备好数据再发送
		SCLK_H;//拉高时钟线，发送数据
		data>>=1;
	}
}
 
void ds1302_wirte_rig(u8 address,u8 data)//向指定寄存器地址发送数据
{
u8 temp1=address;
u8 temp2=data;
CE_L;SCLK_L;delay_us(1);
CE_H;delay_us(2);
ds1302_write_onebyte(temp1);
ds1302_write_onebyte(temp2);
CE_L;SCLK_L;delay_us(2);
}
 
u8 ds1302_read_rig(u8 address)//从指定地址读取一字节数据
{
u8 temp3=address;
u8 count=0;
u8 return_data=0x00;
CE_L;SCLK_L;delay_us(3);
CE_H;delay_us(3);
ds1302_write_onebyte(temp3);
ds1032_DATAINPUT_init();//配置I/O口为输入
delay_us(2);
for(count=0;count<8;count++)
	{
	delay_us(2);//使电平持续一段时间
	return_data>>=1;
	SCLK_H;delay_us(4);//使高电平持续一段时间
	SCLK_L;delay_us(14);//延时14us后再去读取电压，更加准确
	if(GPIO_ReadInputDataBit(DS1302_DATA_PORT,DS1302_DATA_PIN))//此处有埋伏
	{return_data=return_data|0x80;}
	
	}
delay_us(2);
CE_L;DATA_L;
return return_data;
}

 
void ds1032_init(void)
{
ds1302_wirte_rig(0x8e,0x00);//关闭写保护
ds1302_wirte_rig(0x80,0x37);//seconds37秒
ds1302_wirte_rig(0x82,0x20);//minutes58分
ds1302_wirte_rig(0x84,0x23);//hours23时
ds1302_wirte_rig(0x86,0x19);//date30日
ds1302_wirte_rig(0x88,0x08);//months9月
ds1302_wirte_rig(0x8a,0x05);//days星期日
ds1302_wirte_rig(0x8c,0x22);//year2020年
ds1302_wirte_rig(0x8e,0x80);//关闭写保护
}

//向DS302写入时钟数据
void ds1302_write_time(void) 
{
ds1302_wirte_rig(0x8e,0x00);//关闭写保护
ds1302_wirte_rig(0x82,time_buf[5]);//minutes58分
ds1302_wirte_rig(0x84,time_buf[4]);//hours23时
ds1302_wirte_rig(0x86,time_buf[3]);//date30日
ds1302_wirte_rig(0x88,time_buf[2]);//months9月
ds1302_wirte_rig(0x8a,time_buf[6]);//days星期日
ds1302_wirte_rig(0x8c,time_buf[1]);//year2020年
ds1302_wirte_rig(0x8e,0x80);//关闭写保护
}



void ds1032_read_time(void)
{
read_time[0]=ds1302_read_rig(0x81);//读秒
read_time[1]=ds1302_read_rig(0x83);//读分
read_time[2]=ds1302_read_rig(0x85);//读时
read_time[3]=ds1302_read_rig(0x87);//读日
read_time[4]=ds1302_read_rig(0x89);//读月
read_time[5]=ds1302_read_rig(0x8B);//读星期
read_time[6]=ds1302_read_rig(0x8D);//读年
}
 
void ds1032_read_realTime(void)
{
ds1032_read_time();  //BCD码转换为10进制
TimeData.second=(read_time[0]>>4)*10+(read_time[0]&0x0f);
TimeData.minute=((read_time[1]>>4)&(0x07))*10+(read_time[1]&0x0f);
TimeData.hour=(read_time[2]>>4)*10+(read_time[2]&0x0f);
TimeData.day=(read_time[3]>>4)*10+(read_time[3]&0x0f);
TimeData.month=(read_time[4]>>4)*10+(read_time[4]&0x0f);
TimeData.week=read_time[5];
TimeData.year=(read_time[6]>>4)*10+(read_time[6]&0x0f)+2000;
}


void RealTime_Display_Init(void)         //上电初始化显示实时时间
{
		OLED_ShowChinese(84,0,12);//星
		OLED_ShowChinese(98,0,13);//期
		OLED_ShowChinese(112,0,TimeData.week+13);                    //用oled实时显示秒钟数

		OLED_ShowNum(0,0,TimeData.year,4,16);
		OLED_ShowString(32,0,"-",16);                      
		OLED_ShowNum(42,0,TimeData.month,2,16);                  
		OLED_ShowString(58,0,"-",16);                      
		OLED_ShowNum(68,0,TimeData.day,2,16);                    
			if(TimeData.month<10)                                     //防止显示1位数，不整齐
			{  
			  	OLED_ShowString(42,0,"0",16);                 //0位补偿
			}
			if(TimeData.day<10)                                     //防止显示1位数，不整齐
			{
			  	OLED_ShowString(68,0,"0",16);                 //0位补偿
			}			
			
		
			
		 OLED_ShowNum(34,3,TimeData.hour,2,16);                    //用oled实时显示分钟数
		 OLED_ShowString(50,3,":",16);                      //用oled实时显示倒计时分号
		OLED_ShowNum(58,3,TimeData.minute,2,16);                    //用oled实时显示分钟数
		 OLED_ShowString(74,3,":",16);                      //用oled实时显示倒计时分号
	   OLED_ShowNum(82,3,TimeData.second,2,16);                    //用oled实时显示秒钟数
			if(TimeData.minute<10)                                     //防止显示1位数，不整齐
			{  
			  	OLED_ShowString(58,3,"0",16);                 //0位补偿
			}
			if(TimeData.second<10)                                     //防止显示1位数，不整齐
			{
			  	OLED_ShowString(82,3,"0",16);                 //0位补偿
			}		
			if(TimeData.hour<10)                                     //防止显示1位数，不整齐
			{  
			  	OLED_ShowString(34,3,"0",16);                 //0位补偿
			}
			 OLED_ShowChinese(0,6,42);//温
			 OLED_ShowNum(16,6,temperature,2,16);                   
		   OLED_ShowChinese(32,6,39);//℃	
		

			
			 OLED_ShowChinese(48,6,43);//湿
			 OLED_ShowNum(64,6,humidity,2,16);  
				OLED_ShowString(80,6,"%",16); 
			
			
			 
		}

		
		
		
void RealTime_Display(void)            //实时时间日期实时刷新显示	
{
	OLED_ShowNum(82,3,TimeData.second,2,16);                    //用oled实时显示秒钟数
	if(TimeData.second<10)                                     //防止显示1位数，不整齐
	{
		OLED_ShowString(82,3,"0",16);                 //0位补偿
	}		
	
	
	if(TimeData.second==0)
	{

		OLED_ShowNum(0,0,TimeData.year,4,16);                    //用oled实时显示分钟数
		OLED_ShowString(32,0,"-",16);                      //用oled实时显示倒计时分号
		OLED_ShowNum(42,0,TimeData.month,2,16);                    //用oled实时显示分钟数
		OLED_ShowString(58,0,"-",16);                      //用oled实时显示倒计时分号
		OLED_ShowNum(68,0,TimeData.day,2,16);                    //用oled实时显示秒钟数
  
		if(TimeData.month<10)                                     //防止显示1位数，不整齐
		{  
		  	OLED_ShowString(42,0,"0",16);                 //0位补偿
		}
		if(TimeData.day<10)                                     //防止显示1位数，不整齐
		{
		  	OLED_ShowString(68,0,"0",16);                 //0位补偿
		}			
		
	
		
		OLED_ShowNum(34,3,TimeData.hour,2,16);                    //用oled实时显示分钟数
		OLED_ShowString(50,3,":",16);                      //用oled实时显示倒计时分号
		OLED_ShowNum(58,3,TimeData.minute,2,16);                    //用oled实时显示分钟数
		OLED_ShowString(74,3,":",16);                      //用oled实时显示倒计时分号
	 
		if(TimeData.minute<10)                                     //防止显示1位数，不整齐
		{  
		  	OLED_ShowString(58,3,"0",16);                 //0位补偿
		}
		
		if(TimeData.hour<10)                                     //防止显示1位数，不整齐
		{  
		  	OLED_ShowString(34,3,"0",16);                 //0位补偿
		}
		
	}
		
	OLED_ShowChinese(0,6,42);//温
	OLED_ShowNum(16,6,temperature,2,16);                   
	OLED_ShowChinese(32,6,39);//℃	
	
	OLED_ShowChinese(48,6,43);//湿
	OLED_ShowNum(64,6,humidity,2,16);  
	OLED_ShowString(80,6,"%",16); 
	
	
	 
}

void Modify_RealTime_Display(void)            //修改后时间日期刷新一次显示
{
	OLED_ShowChinese(84,0,12);//星
	OLED_ShowChinese(98,0,13);//期
	OLED_ShowChinese(112,0,TimeData.week+13);                    //显示星期几
	
	OLED_ShowNum(82,3,TimeData.second,2,16);                    //实时显示秒钟数
	
	if(TimeData.second<10)                                     //防止显示1位数，不整齐
	{
	  	OLED_ShowString(82,3,"0",16);                 			//0位补偿
	}		

	OLED_ShowNum(0,0,TimeData.year,4,16);              //用oled实时显示分钟数
	OLED_ShowString(32,0,"-",16);                      //用oled实时显示倒计时分号
	OLED_ShowNum(42,0,TimeData.month,2,16);            //用oled实时显示分钟数
	OLED_ShowString(58,0,"-",16);                      //用oled实时显示倒计时分号
	OLED_ShowNum(68,0,TimeData.day,2,16);              //用oled实时显示秒钟数
	
	if(TimeData.month<10)                             //防止显示1位数，不整齐
	{  
	  	OLED_ShowString(42,0,"0",16);                 //0位补偿
	}
	
	if(TimeData.day<10)                               //防止显示1位数，不整齐
	{
	  	OLED_ShowString(68,0,"0",16);                 //0位补偿
	}			
	
	
	
	OLED_ShowNum(34,3,TimeData.hour,2,16);                    //用oled实时显示分钟数
	OLED_ShowString(50,3,":",16);                      //用oled实时显示倒计时分号
	OLED_ShowNum(58,3,TimeData.minute,2,16);                    //用oled实时显示分钟数
	OLED_ShowString(74,3,":",16);                      //用oled实时显示倒计时分号
	 
	if(TimeData.minute<10)                                     //防止显示1位数，不整齐
	{  
	  	OLED_ShowString(58,3,"0",16);                 //0位补偿
	}
	if(TimeData.hour<10)                                     //防止显示1位数，不整齐
	{  
	  	OLED_ShowString(34,3,"0",16);                 //0位补偿
	}
	OLED_ShowChinese(0,6,42);//温
	OLED_ShowNum(16,6,temperature,2,16);                   
	OLED_ShowChinese(32,6,39);//℃	
	

	
	OLED_ShowChinese(48,6,43);//湿
	OLED_ShowNum(64,6,humidity,2,16);  
	OLED_ShowString(80,6,"%",16); 
	
	
	
}
