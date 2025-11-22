#include "bsp_Timbase.h"
#include "gizwits_product.h"

/*******************************************************************************
* Function Name  : TIM4_Configuare
* Description    : 定时器4模式初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GENERAL4_TIM_Init(void)   //定时器4初始化
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;						//声明一个定时器初始化结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		 //使能定时器2 新增的程序
	TIM_DeInit(TIM4);                                                   //将TIM2定时器初始化为复位值
	TIM_InternalClockConfig(TIM4); 			                            //选择时钟源 内部时钟
	TIM_TimeBaseStructure.TIM_Prescaler = 71; 						    //分频为1/72，即1MHz 1秒 = 1 000 000
	TIM_TimeBaseStructure.TIM_Period =9999;  							//初装值 10ms = 10 000
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    			//设置时钟分割:0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;			//TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 					//写入
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);								//清除TIM的待处理标志位
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); 			//使能更新中断 
	TIM_Cmd(TIM4, ENABLE);												//使能
}



/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : 初始化NVIC
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)  
{         
 		NVIC_InitTypeDef NVIC_InitStructure;   					    //声明一个中断初始化结
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		 	//设成优先级组2：先占优先级2位 从优先级2位    

		NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;               //设置TIM4中断优先级
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//0    //中断占先等级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	   	//1 	//中断响应优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;				//使能
		NVIC_Init(&NVIC_InitStructure);						   	
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;	// 设置中断来源	
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 // 设置主优先级为 0  
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	    // 设置抢占优先级为3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
} 			



/*
 * 函数名：TIM3_Int_Init
 * 描述  ：配置TIM3
 * 输入  ：arr, psc
 * 输出  ：无
 * 调用  ：外部调用
 */

void TIM3_Int_Init(u16 arr,u16 psc)
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
		// 开启定时器时钟,即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=arr;
	  // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= psc;	
		// 时钟分频因子 ，没用到不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
		// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // 初始化定时器
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM3, ENABLE);
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断		20ms中断
{
		if ( TIM_GetITStatus( TIM3, TIM_IT_Update) != RESET ) 
		{	
				TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);  	
				gizTimerMs();
		}		 	
}
