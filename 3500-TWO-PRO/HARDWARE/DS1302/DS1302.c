
//------------include------------
#include "delay.h"
#include "DS1302.h"
//-------define--------------
#define uint u16
#define uchar u8

//--------------------------

#define Set_RST()	GPIO_SetBits(GPIOD,GPIO_Pin_5)
#define Clr_RST()	GPIO_ResetBits(GPIOD,GPIO_Pin_5)
#define Set_IO()	GPIO_SetBits(GPIOD,GPIO_Pin_4)
#define Clr_IO()	GPIO_ResetBits(GPIOD,GPIO_Pin_4)
#define Set_SCLK()	GPIO_SetBits(GPIOD,GPIO_Pin_3)
#define Clr_SCLK()	GPIO_ResetBits(GPIOD,GPIO_Pin_3)
#define T_IO_IN()	PDin(4)

uchar year,month,day,hour,min,sec;

//-------------------------
void DS1302_GPIOInit(void)
{

 GPIO_InitTypeDef  GPIO_InitStructure;
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PD端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;	 //PD3,PD4,PD5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //漏极开路输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD.3,GPIOD.4,GPIOD.5
 GPIO_SetBits(GPIOD,GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);//PD.3,PD.4,PD.5 输出高

}

void DS1302IO_IN(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PD端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	 			//PD4端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	 //浮空输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD.4
}
void DS1302IO_OUT(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PD端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	 			//PD4端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //漏极开路输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD.4
}
//-------------------------
//功能：往1302写一个字节数据
//输入：uchar d
//
//返回值：无
//-------------------------
void RT_Input(uchar d)
{
	uchar i;
	uchar dtemp;
	dtemp = d;
	DS1302IO_OUT();	//置为输出
	for(i = 8; i > 0; i--)
	{
		if(dtemp & 0x01 == 1)
		{
			Set_IO();
		}
		else
		{
			Clr_IO();
		}
		__nop();
		__nop();
				
		Set_SCLK();
		__nop();
		__nop();					
		__nop();
		__nop();
		Clr_SCLK();
		dtemp = dtemp>>1;
	}
}


//--------------------------
//功能：从1302读一个字节数据
//输入：uchar d
//
//返回值：ACC
//--------------------------
uchar RT_Output(void)
{
	uchar i;
	uchar dtemp;
	DS1302IO_IN();	//置为输入
	for(i=8;i>0;i--)
	{
		dtemp = dtemp>>1;
		if(T_IO_IN() == 1)
		{
			dtemp |= 0x80;
		}
		else
		{
			dtemp &= 0x7F;
		}
		__nop();
		__nop();					
		__nop();
		__nop();
		Set_SCLK();
		__nop();
		__nop();					
		__nop();
		__nop();
		Clr_SCLK();
	}
	return (dtemp);
}
//--------BCD --------转换-----------
uchar changeBCD(uchar d)
{
	uchar t;
	t=d&0xf0;
	t>>=4;
	d&=0x0f;
	d=10*t+d;
	return d;
}

//---------h to BCD------------------

uchar toBCD(uchar d)
{
	uchar t;
	t = d/10;
	t<<=4;
	d %= 10;
	d += t;
	return d;
}

//----------------------------
//功能：先写地址，再命令/数据
//调用：RT_Input()
//输入：addr,d
//
//返回值：无
//----------------------------
void W1302(uchar addr,uchar d)
{
	uchar t;
	t=toBCD(d);
	Clr_RST();
	Clr_SCLK();
	Set_RST();
	delay_us(1);
	RT_Input(addr);
	RT_Input(t);
	Set_SCLK();
	Clr_RST();
}

//------------------------------
//功能：先写地址，再读命令/数据
//调用：RT_Output()
//输入：addr
//
//返回值：d
//------------------------------
uchar R1302(uchar addr)
{
	uchar d;
	Clr_RST();
	Clr_SCLK();
	Set_RST();
	delay_us(1);
	RT_Input(addr);
	d=RT_Output();
	Set_SCLK();
	Clr_RST();
	d=changeBCD(d);
	return(d);
}


//---------ReadDate-------ReadTime---

void ReadDate(void)
{
	month=R1302(0x89);
	day=R1302(0x87);
	year=R1302(0x8d);
}


void ReadTime(void)
{
	hour=R1302(0x85);
	min=R1302(0x83);
	sec=R1302(0x81);	//显示时转换为BCD码
}

void DS1302_SetTime(uchar set_hour,uchar set_min,uchar set_sec)
{
	W1302(0x8e,0x00);		
	W1302(0x80,set_sec);	//写入ds1302
	W1302(0x82,set_min);
	W1302(0x84,set_hour);	
	W1302(0x8e,0x80);	//写保护
}
void DS1302_SetDate(uchar set_year,uchar set_month,uchar set_day)
{
	W1302(0x8e,0x00);		
	W1302(0x8c,set_year);	//写入ds1302
	W1302(0x88,set_month);
	W1302(0x86,set_day);	
	W1302(0x8e,0x80);	//写保护
}
void DS1302_Charge(void)
{
	//1302充电
	W1302(0x8e,0x00);
	W1302(0x90,0xa6);		//写入ds1302,one diode & 4k 
	W1302(0x8e,0x80);
}
void DS1302_GetRTC(void)
{
    ReadDate();
	ReadTime();
}
//----------------------------
//功能：先写地址，再命令/数据
//调用：RT_Input()
//输入：addr,d
//
//返回值：无
//----------------------------
/*
void W1302_A(uchar addr,uchar d)
{
	T_RST=0;
	T_SCLK=0;
	T_RST=1;
				Delay5us(1);
	RT_Input(addr);
	RT_Input(d);
	T_SCLK=1;
	T_RST=0;
}
*/
//------------------------------
//功能：先写地址，再读命令/数据
//调用：RT_Output()
//输入：addr
//
//返回值：d
//------------------------------
/*
uchar R1302_A(uchar addr)
{
	uchar d;
	T_RST=0;
	T_SCLK=0;
	T_RST=1;
				Delay5us(1);
	RT_Input(addr);
	d=RT_Output();
	T_SCLK=1;
	T_RST=0;
	return(d);
}
*/


