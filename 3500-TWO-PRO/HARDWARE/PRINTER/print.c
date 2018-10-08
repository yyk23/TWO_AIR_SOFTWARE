#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "print.h"
#include "process.h"
#include "parameter.h"
#include <string.h>
#include "ZR3500_CONFIG.h"
#define LF 0x0a //��ӡ���з�
u8   HZtemp[32];          //���ֻ���
u8   ASCtemp[16];         //ASCII����
u8 PrintErr = 0;                 //��ӡ�����־
extern u16 print_start_file;
extern u16 print_end_file;


void PrintSetUp(void)
{
//	uart2_init(9600);
	PrintErr = 0;
}
/* -------------------------------------------*/
/*   �� ӡ �� �� ģ ʽ �� �� �� �� ��         */
/* ˵��: �Զ��ֱ纺��,ASCII,�Լ��س������ַ�  */
/* ���: u8 *pstr �ַ���                   */
/* -------------------------------------------*/
u8 nDot = 0;
void PrintHZStr(u8 *pstr)
{
	PrintC(0x1b);
	PrintC(0x31);
	PrintC(nDot);
	while(*pstr)
	{
		PrintC(*pstr);
		pstr++;
	}
	PrintC(LF);//����
	return;
}
/* --------------------------------- */
/*     ��ӡһ���ַ�����              */
/*    �ַ���num��                    */
/* ----------------------------------*/

void PrintC(u8 num)
{
	UART2_SendByte(num);
}
/*-----------------------------------*/
/*  ��ӡ����⺯��                   */
/*-----------------------------------*/
u8 PrinterTest(void)
{
	u8 i;
	i = printer_connect();
	return i;
}
/*------------------------------*/
/*  ��ʼ����ӡ������            */
/*------------------------------*/
void InitPrinter(void)
{
    PrintC(0x1b);        /* ����  */
	PrintC(0x40);
}
/*------------------------------------------------
�������ܣ���ӡ�ļ���Ϣ
-------------------------------------------------*/
void PrintStart(void)
{
    char strbuf[40];
	nDot = 1;
	PrintHZStr((u8 *)"----------------------------");
    strcpy(strbuf,(char*)"ZR-3500�ʹ��������� ");
#ifdef ZR3500TWO
    strcat(strbuf,(char*)ZR3500TWO_VER);  
#endif
#ifdef ZR3500TWO_HEAT
    strcat(strbuf,(char*)ZR3500TWO_HEAT_VER);
    
#endif
#ifdef ZR3500TWO_HEATandCOOL
    strcat(strbuf,(char*)ZR3500TWO_HEATandCOOL_VER);
#endif
	PrintHZStr((u8 *)strbuf);
	return;
}
void PrintData(u16 FileNo)
{
	u8 hour_tmp,min_tmp,sec_tmp;
	char strbuf[40];
	char cbuf[40];
    u8 bianhao_buf[10];
	nDot = 2;
    strcpy(strbuf,(char*)"�������  ");  
#ifdef ZR3500TWO
    SPutl(8,bianhao_buf,bianhao_new);
    strcat(strbuf,(char*)ZR3500TWO_NAME);
    strcat(strbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEAT  
    SPutl(8,bianhao_buf,bianhao_new);
    strcat(strbuf,(char*)ZR3500TWO_HEAT_NAME);
    strcat(strbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEATandCOOL
    SPutl(8,bianhao_buf,bianhao_new);
    strcat(strbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
    strcat(strbuf,bianhao_buf);
#endif
    PrintHZStr((u8 *)strbuf);
	if(sample_file_chaxun.channel == DF_CHANNEL_A)
	{
		PrintHZStr((u8 *)"A ·����");
	}
	else
	{
		PrintHZStr((u8 *)"B ·����");
	}

	strcpy(strbuf,(char*)"�� �� ��  ");
	SPuti(4,(u8 *)cbuf,FileNo);//
	strcat(strbuf,cbuf);
	PrintHZStr((u8 *)strbuf);
	strcpy(strbuf,(char*)"��������  20");
	CharToStr(2,(u8 *)cbuf,sample_file_chaxun.year);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"-");
	CharToStr(2,(u8 *)cbuf,sample_file_chaxun.month);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"-");
	CharToStr(2,(u8 *)cbuf,sample_file_chaxun.day);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"  ");
	CharToStr(2,(u8 *)cbuf,sample_file_chaxun.hour);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)":");
	CharToStr(2,(u8 *)cbuf,sample_file_chaxun.min);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)":");
	CharToStr(2,(u8 *)cbuf,sample_file_chaxun.sec);
	strcat(strbuf,cbuf);
	PrintHZStr((u8 *)strbuf);
	hour_tmp = sample_file_chaxun.running_time / 3600;
	min_tmp = (sample_file_chaxun.running_time % 3600) / 60;
	sec_tmp = sample_file_chaxun.running_time % 60;
	strcpy(strbuf,(char*)"����ʱ��  ");
	CharToStr(2,(u8 *)cbuf,hour_tmp);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)":");
	CharToStr(2,(u8 *)cbuf,min_tmp);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)":");
	CharToStr(2,(u8 *)cbuf,sec_tmp);
	strcat(strbuf,cbuf);
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char*)"�����¶�  ");
	SPutf(sf31,(u8 *)cbuf,sample_file_chaxun.environment_temperature);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"��");
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char*)"�� �� ѹ  ");
	SPutf(f52,(u8 *)cbuf,sample_file_chaxun.barometric);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"kPa");
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char*)"�趨����  ");
	SPutf(f21,(u8 *)cbuf,sample_file_chaxun.flow_set);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"L/min");
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char*)"��ǰѹ��  ");
	SPutf(sf42,(u8 *)cbuf,sample_file_chaxun.jy);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"kPa");
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char*)"��ǰ�¶�  ");
	SPutf(sf31,(u8 *)cbuf,sample_file_chaxun.temperature);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"��");
	PrintHZStr((u8 *)strbuf);

	strcpy(strbuf,(char*)"�ۻ����  ");
	SPutf(f62,(u8 *)cbuf,sample_file_chaxun.volume);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"L");
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char*)"������  ");
	SPutf(f62,(u8 *)cbuf,sample_file_chaxun.volume_biaokuang);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"L");
	PrintHZStr((u8 *)strbuf);

	if(err_dis == 1)
	{
		if(sample_file_chaxun.success == DF_JY_OUT)
		{
			PrintHZStr((u8 *)"��·����");
		}
		else if(sample_file_chaxun.success == DF_FAN_ERR)
		{
			PrintHZStr((u8 *)"�����ﲻ��");
		}
		else
		{
			
		}
	}
	else
	{
		
	}
	nDot = 1;
	PrintHZStr((u8 *)"----------------------------");
	PrintC(LF);
}

/*------------------------------------------------
�������ܣ���ӡ������Ϣ
-------------------------------------------------*/
void PrintPower(u16 FileNo)
{
	char strbuf[40];
	char cbuf[40];
    u8 bianhao_buf[10];
	nDot = 2;
    strcpy(strbuf,(char*)"�������  ");  
#ifdef ZR3500TWO
    SPutl(8,bianhao_buf,bianhao_new);
    strcat(strbuf,(char*)ZR3500TWO_NAME);
    strcat(strbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEAT  
    SPutl(8,bianhao_buf,bianhao_new);
    strcat(strbuf,(char*)ZR3500TWO_HEAT_NAME);
    strcat(strbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEATandCOOL
    SPutl(8,bianhao_buf,bianhao_new);
    strcat(strbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
    strcat(strbuf,bianhao_buf);
#endif
    PrintHZStr((u8 *)strbuf);
	if(power_file_chaxun.channel == DF_CHANNEL_A)
	{
		PrintHZStr((u8 *)"A ·����");
	}
	else
	{
		PrintHZStr((u8 *)"B ·����");
	}

	strcpy(strbuf,(char *)"�� ¼ ��  ");
	SPuti(4,(u8 *)cbuf,FileNo);//
	strcat(strbuf,cbuf);
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char *)"����ʱ��  20");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.year);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char *)"-");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.month);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char *)"-");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.day);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char *)"  ");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.hour);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char *)":");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.min);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char *)":");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.sec);
	strcat(strbuf,cbuf);
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char *)"����ʱ��  20");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.power_year);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char *)"-");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.power_month);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char *)"-");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.power_day);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char *)"  ");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.power_hour);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char *)":");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.power_min);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char *)":");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.power_sec);
	strcat(strbuf,cbuf);
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char *)"�ۼƵ���  ");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.lose_hour);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char *)":");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.lose_min);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char *)":");
	CharToStr(2,(u8 *)cbuf,power_file_chaxun.lose_sec);
	strcat(strbuf,cbuf);
	PrintHZStr((u8 *)strbuf);
	nDot = 1;
	PrintHZStr((u8 *)"----------------------------");
	PrintC(LF);
}



u8 rx_buf[15];
u8 tx_buf[15];
//u16 printer_password;
u8 bluetooth_ok_flag = 0;

void UART2_SendStr(u8 * TxStr)
{
	while(*TxStr)
	{
		UART2_SendByte(*TxStr);
		TxStr++;
	}	
}

/*
������void UART2_ReceiveStr(u8 num)
˵������ѯ��ʽ���մ������ݺ����������յ����ݸ���Ϊnum
	  ֧�ֵĲ����ʷ�ΧΪ2400~115200
	  1��(5us��20000)δ�յ�������ʱ�˳�
���أ�1���յ����ݣ�0����ʱδ�յ�ָ������
*/
u8 UART2_ReceiveStr(u8 num)
{
	u8 i;
	u16 j = 0;
	for(i = 0; i < num; i++)
	{
		j = 0;
		while(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == 0)
		{
			delay_us(50);			//5us��ʱ
			j++;
			if(j >= 20000)
			{
				rx_buf[i] = 0;
				return 0;			//1�볬ʱ�˳�
			}
		}
		rx_buf[i] = USART_ReceiveData(USART2);
	}
	rx_buf[i] = 0;
	return 1;
}
void bluetooth_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��ʼ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//��ʼ��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;			//©�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//��ʼ��

	BLUETOOTH_POWER_ON();
	uart2_init(9600);
	BLUETOOTH_KEY = 1;
	delay_ms(1);
	BLUETOOTH_KEY = 0;
	delay_ms(1);
	UART2_SendStr((u8 *)"AT");
	if(UART2_ReceiveStr(2) == 1)
	{
		if((rx_buf[0] == 'O') && (rx_buf[1] == 'K'))
		{
			bluetooth_ok_flag = 1;//����ģ������
		}
		else
		{
			bluetooth_ok_flag = 0;//����ģ�����
		}
	}
	else
	{
		bluetooth_ok_flag = 0;//����ģ�����
	}
	BLUETOOTH_POWER_OFF();
}
/*��������ģ��Ϊ��ģʽ*/
u8 bluetooth_set_master(void)
{
	u8 retflag;
	BLUETOOTH_POWER_ON();
	delay_ms(2000);
	UART2_SendStr((u8 *)"AT+ROLE=M");
	if(UART2_ReceiveStr(9) == 1)
	{
		if(strcmp((const char *)rx_buf,"OK+ROLE:M") == 0)
		{
			retflag = 1;
		}
		else
		{
			retflag = 0;
		}
	}
	else
	{
		retflag = 0;
	}
	delay_ms(5000);
	BLUETOOTH_POWER_OFF();
	return retflag;
}

/*��������ģ������*/
u8 bluetooth_set_printer_password(u16 password_tmp)
{
	u8 retflag;
	u8 cbuf[10];
	BLUETOOTH_POWER_ON();
	delay_ms(2000);
	strcpy((char *)tx_buf,(const char *)"AT+PIN");
	SPuti(4,(u8 *)cbuf,password_tmp);
	strcat((char *)tx_buf,(char *)(cbuf));
	UART2_SendStr(tx_buf);
	if(UART2_ReceiveStr(8) == 1)
	{
		if(strcmp((const char *)rx_buf,"OKsetPIN") == 0)
		{
			retflag = 1;
		}
		else
		{
			retflag = 0;
		}
	}
	else
	{
		retflag = 0;
	}
	delay_ms(5000);
	BLUETOOTH_POWER_OFF();
	return retflag;
}

u8 printer_connect(void)
{
	u16 i = 0;
	u16 j = 0;
	while(1)
	{
		for(i = 0; i < 1500; i++)
		{
			delay_ms(1);
			if(BLUETOOTH_LED == 0)	//δ������
			{
				j = 0;
			}
			else
			{
				j++;
			}
			if(j > 1000)	//������
			{
				return 0;
			}
		}
		BLUETOOTH_KEY = 1;
		delay_ms(1);
		BLUETOOTH_KEY = 0;
		return 1;
	}
	return 1;
}

















