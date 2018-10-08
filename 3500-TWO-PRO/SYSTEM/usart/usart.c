#include "sys.h"
#include "usart.h"	
#include "DMT.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F4̽���߿�����
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/6/10
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
//	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
//	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}


void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
  } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 
#endif	

 
void uart2_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

//	USART_Cmd(USART2, DISABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 	//ʹ��GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//ʹ��USART2ʱ��

	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2

	//USART2�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 	//GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 			//��©���
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 			//����
	GPIO_Init(GPIOA,&GPIO_InitStructure); 					//��ʼ��PA2��PA3

	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); 										//��ʼ������2

	USART_Cmd(USART2, ENABLE);  													//ʹ�ܴ���2 

	USART_ClearFlag(USART2, USART_FLAG_TC);
	
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//��������ж�
//	//Usart2 NVIC ����
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//����2�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;	//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

void UART2_SendByte(u8 TxData)
{
	while((USART2->SR & 0X40)==0);//ѭ������,ֱ���������   
	USART2->DR = (u8) TxData;  
}


void USART2_IRQHandler(void)                	//����2�жϷ������
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
	{
		USART_ReceiveData(USART2);//(USART2->DR);	//��ȡ���յ�������	 
	}
} 



u8 frame_buf_select = 0; 	//2��������ѡ��
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
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIOC\Dʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);						//ʹ��USART5ʱ��

	//����5��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); 					//GPIOC12����ΪUART5
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); 						//GPIOD2����ΪUART5

	//UART5�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 									//GPIOC12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;								//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 								//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 								//����
	GPIO_Init(GPIOC,&GPIO_InitStructure); 										//��ʼ��PC12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 									//GPIOD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;								//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 								//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 								//����
	GPIO_Init(GPIOD,&GPIO_InitStructure); 										//��ʼ��PC12

	//UART5 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;									//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;					//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;						//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;							//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				//�շ�ģʽ
	USART_Init(UART5, &USART_InitStructure); 									//��ʼ������5

	USART_ClearFlag(UART5, USART_FLAG_TC);
	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);								//��������ж�
	//Usart5 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;							//����5�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;						//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;							//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);												//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART_Cmd(UART5, ENABLE);  													//ʹ�ܴ���5
}

void UART5_SendByte(u8 TxData)
{
	while((UART5->SR & 0X40)==0);												//ѭ������,ֱ���������   
	UART5->DR = (u8) TxData;  
}

void UART5_IRQHandler(void)                									//����5�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  						//�����ж�
	{
		Res =USART_ReceiveData(UART5);											//��ȡ���յ�������
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


