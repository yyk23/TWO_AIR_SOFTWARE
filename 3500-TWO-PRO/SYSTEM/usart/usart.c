#include "sys.h"
#include "usart.h"	
#include "DMT.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F4探索者开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/6/10
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
//	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
//	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
} 
#endif	

 
void uart2_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

//	USART_Cmd(USART2, DISABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 	//使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//使能USART2时钟

	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2

	//USART2端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 	//GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 			//开漏输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 			//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 					//初始化PA2，PA3

	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;										//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init(USART2, &USART_InitStructure); 										//初始化串口2

	USART_Cmd(USART2, ENABLE);  													//使能串口2 

	USART_ClearFlag(USART2, USART_FLAG_TC);
	
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//开启相关中断
//	//Usart2 NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//串口2中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;	//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器、
}

void UART2_SendByte(u8 TxData)
{
	while((USART2->SR & 0X40)==0);//循环发送,直到发送完毕   
	USART2->DR = (u8) TxData;  
}


void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
		USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据	 
	}
} 



u8 frame_buf_select = 0; 	//2个缓冲区选择
u8 FRAME_BUF[50];			//
//u8 FRAME_BUF1[50];
u8 FRAME_BUF2[50];
u8 R_x = 0;
u8 Receive[60];
u8 Receive_index = 0;
u8 R_y = 0;
u8 R_flag = 0;
//u8 R_flag_FRAME_BUF1 = 0;
u8 R_flag_FRAME_BUF2 = 0; 
void uart5_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOC\D时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);						//使能USART5时钟

	//串口5对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); 					//GPIOC12复用为UART5
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); 						//GPIOD2复用为UART5

	//UART5端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 									//GPIOC12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;								//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 								//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 								//上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); 										//初始化PC12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 									//GPIOD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;								//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 								//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 								//上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); 										//初始化PC12

	//UART5 初始化设置
	USART_InitStructure.USART_BaudRate = bound;									//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;					//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;						//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;							//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				//收发模式
	USART_Init(UART5, &USART_InitStructure); 									//初始化串口5

	USART_ClearFlag(UART5, USART_FLAG_TC);
	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);								//开启相关中断
	//Usart5 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;							//串口5中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;						//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;							//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);												//根据指定的参数初始化VIC寄存器
	
	USART_Cmd(UART5, ENABLE);  													//使能串口5
}

void UART5_SendByte(u8 TxData)
{
	while((UART5->SR & 0X40)==0);												//循环发送,直到发送完毕   
	UART5->DR = (u8) TxData;  
}

void UART5_IRQHandler(void)                									//串口5中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  						//接收中断
	{
		Res =USART_ReceiveData(UART5);											//读取接收到的数据
		Receive[R_x] = Res;                   //
		R_x++;
		if(Receive[0]!=0x5a)
		{
			R_x=0;
		}
		if((Receive[1]!=0xa5)&&(R_x==2))
		{
			R_x=0;
		}
		if(R_x==3)
		{
			R_y=Receive[2];
		}
		if(R_x==(R_y+3))
		{
			R_x=0;
			if(Receive[3] != 0x81)
			{
				for(Receive_index = 0; Receive_index <= R_y; Receive_index++)
				{
					if(Receive_index >= 50)
					{
						Receive_index = 0;
					} 
					FRAME_BUF[Receive_index] = Receive[Receive_index + 2];
					R_flag=1;
				}
			}
			else
			{
				for(Receive_index = 0; Receive_index <= R_y; Receive_index++)
				{
					
					if(Receive_index >= 50)
					{
						Receive_index = 0;
					} 
					FRAME_BUF2[Receive_index] = Receive[Receive_index + 2];
					R_flag_FRAME_BUF2 = 1;
				}
			}
		} 
	} 
} 


