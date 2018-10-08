#include "exti.h"
#include "led.h"
#include "DMT64480T056.h" 
#include "delay.h"
//外部中断0服务程序
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

    //GPIOE.4 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);

  	EXTI_InitStructure.EXTI_Line = EXTI_Line4;	//
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;				//使能外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);

}


void EXTI4_IRQHandler(void)
{		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位
	delay_ms(10);
	if(SOFT_KEY == 0)
	{
		BUZZER = 0;
		DMT64480T056_ChangePic(160);
		delay_ms(200);
		BUZZER = 1;
		delay_ms(1000);
		DMT64480T056_ChangePic(1);
		POWER_LED = 1;
		while(1)
		{
			if(SOFT_KEY == 1)
			{
				break;
			}
		}
		NVIC_SystemReset();  
	}
}
 
