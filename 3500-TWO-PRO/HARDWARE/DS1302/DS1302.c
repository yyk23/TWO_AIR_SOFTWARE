
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
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PD�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;	 //PD3,PD4,PD5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //©����·���
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOD.3,GPIOD.4,GPIOD.5
 GPIO_SetBits(GPIOD,GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);//PD.3,PD.4,PD.5 �����

}

void DS1302IO_IN(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PD�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	 			//PD4�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	 //��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOD.4
}
void DS1302IO_OUT(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PD�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	 			//PD4�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //©����·���
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOD.4
}
//-------------------------
//���ܣ���1302дһ���ֽ�����
//���룺uchar d
//
//����ֵ����
//-------------------------
void RT_Input(uchar d)
{
	uchar i;
	uchar dtemp;
	dtemp = d;
	DS1302IO_OUT();	//��Ϊ���
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
//���ܣ���1302��һ���ֽ�����
//���룺uchar d
//
//����ֵ��ACC
//--------------------------
uchar RT_Output(void)
{
	uchar i;
	uchar dtemp;
	DS1302IO_IN();	//��Ϊ����
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
//--------BCD --------ת��-----------
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
//���ܣ���д��ַ��������/����
//���ã�RT_Input()
//���룺addr,d
//
//����ֵ����
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
//���ܣ���д��ַ���ٶ�����/����
//���ã�RT_Output()
//���룺addr
//
//����ֵ��d
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
	sec=R1302(0x81);	//��ʾʱת��ΪBCD��
}

void DS1302_SetTime(uchar set_hour,uchar set_min,uchar set_sec)
{
	W1302(0x8e,0x00);		
	W1302(0x80,set_sec);	//д��ds1302
	W1302(0x82,set_min);
	W1302(0x84,set_hour);	
	W1302(0x8e,0x80);	//д����
}
void DS1302_SetDate(uchar set_year,uchar set_month,uchar set_day)
{
	W1302(0x8e,0x00);		
	W1302(0x8c,set_year);	//д��ds1302
	W1302(0x88,set_month);
	W1302(0x86,set_day);	
	W1302(0x8e,0x80);	//д����
}
void DS1302_Charge(void)
{
	//1302���
	W1302(0x8e,0x00);
	W1302(0x90,0xa6);		//д��ds1302,one diode & 4k 
	W1302(0x8e,0x80);
}
void DS1302_GetRTC(void)
{
    ReadDate();
	ReadTime();
}
//----------------------------
//���ܣ���д��ַ��������/����
//���ã�RT_Input()
//���룺addr,d
//
//����ֵ����
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
//���ܣ���д��ַ���ٶ�����/����
//���ã�RT_Output()
//���룺addr
//
//����ֵ��d
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


