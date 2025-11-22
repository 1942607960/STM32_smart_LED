/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "delay.h"


extern u16    Run_Indicate_num;          //运行指示灯辅助定时

extern u8 Alarm_Clock_flag;           //闹钟响标志位
extern u16 Alarm_Clock_num;           //闹钟响标志位计数器

extern u16 DHT11_Read_6s_num;   //间隔6s读取一次DHT11传感器，手册上说间隔大于5s为准确的值
extern u8  DHT11_Data_Read_flag;//DHT11读取标志位

extern u8 temperature;          //温度
extern u8 humidity;             //湿度

extern u16 Over_Limit_num;  //温湿度超限计数器
extern u8  Over_Limit_flag; //温湿度超限标志位
extern u16 Over_Limit_Alarm_num;//温湿度超限报警辅助计数器

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}



void TIM4_IRQHandler(void)//10ms
{  
//	int i;
  
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//检查指定的 TIM 中断发生与否 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清除更新中断标志位
	}

	__nop();

	
		DHT11_Read_6s_num++;                            //DHT11 6s计数器
	if(DHT11_Read_6s_num>=200)                      //定时2s
	{
	    DHT11_Read_6s_num=0;                        //定时器复位
		  DHT11_Data_Read_flag=1;                     //读取一次DHT11
	}
	
//	if(Alarm_Clock_flag)			 //闹钟报警标志位
//	  {
//		   Alarm_Clock_num++;
//			 if(Alarm_Clock_num>=300)   //3s一个周期
//			 {
//				    Alarm_Clock_num=0;
//			 } 
//			    	if(Alarm_Clock_num<=10||(Alarm_Clock_num>=30&&Alarm_Clock_num<=40)||(Alarm_Clock_num>=60
//					 &&Alarm_Clock_num<=70)||(Alarm_Clock_num>=90&&Alarm_Clock_num<=100))
//				     {            
//							   BEEP_ENABLE;    
//                 Alarm_ENABLE;		
//								// BEEP_UNABLE;
//             }
//						 				 
//				else
//						{
//                 BEEP_UNABLE;
//							   Alarm_UNABLE;
//            }
//		
//		}
//		else
//		{
//		    Alarm_Clock_num=0;
//			  Alarm_UNABLE;
//		
//		}

//	
//	
//  Run_Indicate_num++;                              //主程序运行指示灯计数
//	
//	if(Run_Indicate_num>=100)                        //确定计时周期
//	{
//	   Run_Indicate_num=0;                           //计数时间到复位
//		
//	}
//	
//	
////***************************判断温湿度超限报警*******************************************************//
//	if(temperature>=30||temperature<=20||humidity>=70||humidity<=10)
//	{
//	    Over_Limit_num++;
//		  if(Over_Limit_num>=500)
//			{
//			   Over_Limit_num=0;
//				 Over_Limit_flag=1;                   //温湿度超限标志位
//			  
//			}

//	}
//	
//	else
//	{
//	   Over_Limit_num=0;
//		 Over_Limit_flag=0;                      //温湿度超限标志位
//	
//	}
//	


//	
//	if(Over_Limit_flag)			                  //温湿度报警标志位
//	  {
//		   Over_Limit_Alarm_num++;
//			 if(Over_Limit_Alarm_num>=300)       //10s一个循环报警
//			 {
//				    Over_Limit_Alarm_num=0;
//			 } 
//			    	if(Over_Limit_Alarm_num<=10||(Over_Limit_Alarm_num>=30&&Over_Limit_Alarm_num<=40)||(Over_Limit_Alarm_num>=60&&Over_Limit_Alarm_num<=70))
//				     {            
//							   //BEEP_ENABLE;    
//                 Limit_ENABLE;							 
//             }
//						 				 
//				else
//						{
//                 //BEEP_UNABLE;
//							   Limit_UNABLE;
//            }
//		
//		}
//		else
//		{
//		    Over_Limit_Alarm_num=0;
//			  Limit_UNABLE;
//		
//		}

//	
//		if(!Alarm_Clock_flag&&!Over_Limit_flag)    //无报警时，关掉蜂鸣器，否则两个报警有一种无蜂鸣器响
//		{
//		     BEEP_UNABLE;
//		}

}

// /******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/




