#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "print.h"
#include "process.h"
#include "parameter.h"
#include <string.h>
#include "ZR3500_CONFIG.h"
#define LF 0x0a //打印换行符
u8   HZtemp[32];          //汉字缓冲
u8   ASCtemp[16];         //ASCII缓冲
u8 PrintErr = 0;                 //打印错误标志
extern u16 print_start_file;
extern u16 print_end_file;


void PrintSetUp(void)
{
//	uart2_init(9600);
	PrintErr = 0;
}
/* -------------------------------------------*/
/*   打 印 汉 字 模 式 字 符 串 函 数         */
/* 说明: 自动分辩汉字,ASCII,以及回车控制字符  */
/* 入口: u8 *pstr 字符串                   */
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
	PrintC(LF);//换行
	return;
}
/* --------------------------------- */
/*     打印一个字符函数              */
/*    字符在num中                    */
/* ----------------------------------*/

void PrintC(u8 num)
{
	UART2_SendByte(num);
}
/*-----------------------------------*/
/*  打印机检测函数                   */
/*-----------------------------------*/
u8 PrinterTest(void)
{
	u8 i;
	i = printer_connect();
	return i;
}
/*------------------------------*/
/*  初始化打印机函数            */
/*------------------------------*/
void InitPrinter(void)
{
    PrintC(0x1b);        /* 设置  */
	PrintC(0x40);
}
/*------------------------------------------------
函数功能：打印文件信息
-------------------------------------------------*/
void PrintStart(void)
{
    char strbuf[40];
	nDot = 1;
	PrintHZStr((u8 *)"----------------------------");
    strcpy(strbuf,(char*)"ZR-3500型大气采样器 ");
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
    strcpy(strbuf,(char*)"仪器编号  ");  
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
		PrintHZStr((u8 *)"A 路采样");
	}
	else
	{
		PrintHZStr((u8 *)"B 路采样");
	}

	strcpy(strbuf,(char*)"文 件 号  ");
	SPuti(4,(u8 *)cbuf,FileNo);//
	strcat(strbuf,cbuf);
	PrintHZStr((u8 *)strbuf);
	strcpy(strbuf,(char*)"采样日期  20");
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
	strcpy(strbuf,(char*)"采样时长  ");
	CharToStr(2,(u8 *)cbuf,hour_tmp);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)":");
	CharToStr(2,(u8 *)cbuf,min_tmp);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)":");
	CharToStr(2,(u8 *)cbuf,sec_tmp);
	strcat(strbuf,cbuf);
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char*)"环境温度  ");
	SPutf(sf31,(u8 *)cbuf,sample_file_chaxun.environment_temperature);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"℃");
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char*)"大 气 压  ");
	SPutf(f52,(u8 *)cbuf,sample_file_chaxun.barometric);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"kPa");
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char*)"设定流量  ");
	SPutf(f21,(u8 *)cbuf,sample_file_chaxun.flow_set);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"L/min");
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char*)"计前压力  ");
	SPutf(sf42,(u8 *)cbuf,sample_file_chaxun.jy);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"kPa");
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char*)"计前温度  ");
	SPutf(sf31,(u8 *)cbuf,sample_file_chaxun.temperature);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"℃");
	PrintHZStr((u8 *)strbuf);

	strcpy(strbuf,(char*)"累积体积  ");
	SPutf(f62,(u8 *)cbuf,sample_file_chaxun.volume);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"L");
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char*)"标况体积  ");
	SPutf(f62,(u8 *)cbuf,sample_file_chaxun.volume_biaokuang);
	strcat(strbuf,cbuf);
	strcat(strbuf,(char*)"L");
	PrintHZStr((u8 *)strbuf);

	if(err_dis == 1)
	{
		if(sample_file_chaxun.success == DF_JY_OUT)
		{
			PrintHZStr((u8 *)"管路堵塞");
		}
		else if(sample_file_chaxun.success == DF_FAN_ERR)
		{
			PrintHZStr((u8 *)"流量达不到");
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
函数功能：打印掉电信息
-------------------------------------------------*/
void PrintPower(u16 FileNo)
{
	char strbuf[40];
	char cbuf[40];
    u8 bianhao_buf[10];
	nDot = 2;
    strcpy(strbuf,(char*)"仪器编号  ");  
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
		PrintHZStr((u8 *)"A 路掉电");
	}
	else
	{
		PrintHZStr((u8 *)"B 路掉电");
	}

	strcpy(strbuf,(char *)"记 录 号  ");
	SPuti(4,(u8 *)cbuf,FileNo);//
	strcat(strbuf,cbuf);
	PrintHZStr((u8 *)strbuf);
	
	strcpy(strbuf,(char *)"掉电时间  20");
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
	
	strcpy(strbuf,(char *)"来电时间  20");
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
	
	strcpy(strbuf,(char *)"累计掉电  ");
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
函数：void UART2_ReceiveStr(u8 num)
说明：查询方式接收串口数据函数，待接收的数据个数为num
	  支持的波特率范围为2400~115200
	  1秒(5us×20000)未收到数据则超时退出
返回：1：收到数据；0：超时未收到指定个数
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
			delay_us(50);			//5us延时
			j++;
			if(j >= 20000)
			{
				rx_buf[i] = 0;
				return 0;			//1秒超时退出
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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//初始化
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOE时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//初始化	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;			//漏开输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//初始化

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
			bluetooth_ok_flag = 1;//蓝牙模块正常
		}
		else
		{
			bluetooth_ok_flag = 0;//蓝牙模块故障
		}
	}
	else
	{
		bluetooth_ok_flag = 0;//蓝牙模块故障
	}
	BLUETOOTH_POWER_OFF();
}
/*设置蓝牙模块为主模式*/
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

/*设置蓝牙模块密码*/
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
			if(BLUETOOTH_LED == 0)	//未连接上
			{
				j = 0;
			}
			else
			{
				j++;
			}
			if(j > 1000)	//连接上
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

















