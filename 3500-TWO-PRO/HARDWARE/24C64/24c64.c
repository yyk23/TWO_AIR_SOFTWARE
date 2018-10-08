
//------------include------------
#include "sys.h"
#include "delay.h"
#include "24c64.h"

#define somenop();  delay_us(1);
//---------------------------------

#define SetSCL();	GPIO_SetBits(GPIOC,GPIO_Pin_7);
#define ClrSCL();	GPIO_ResetBits(GPIOC,GPIO_Pin_7);

#define SetSDA();	GPIO_SetBits(GPIOC,GPIO_Pin_6);
#define ClrSDA();	GPIO_ResetBits(GPIOC,GPIO_Pin_6);

#define SDA_Read()	PCin(6)

//       模拟I2C          可以写入和读出数据
void FM24C64_GPIOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_6 | GPIO_Pin_7);
}

void SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
}

void SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}



void start(void)				//1.3us
{
	SDA_OUT();
	SetSCL();
	SetSDA();
	somenop();
	ClrSDA();
	somenop();
	ClrSCL();
}

void stop(void)					//1.3us
{
	ClrSCL();
	ClrSDA();
	somenop();
	SetSCL();
	somenop();
	SetSDA();
}

u8 ack(void)					//3us
{
	u8 errtime=255;
	SetSDA();
	somenop();
	SetSCL();
	somenop();
	SDA_IN();
	while(SDA_Read() == 1)
	{
		errtime--;
		if(!errtime)
		{
			stop();
			SDA_OUT();
			ClrSCL();
			return 0;
		}
	}
	SDA_OUT();
	ClrSCL();
	return 1;
}
/*
void sendack(void)
{
	SDA=0;
	somenop();
	SCL=1;
	somenop();
	SCL=0;
}
*/
void sendnotack(void)			//2us
{
	SetSDA();
	somenop();
	SetSCL();
	somenop();
	ClrSCL();
}

void sendbyte(u8 d)			//16us
{
	u8 i=8;
	while(i--)
	{
		ClrSCL();
		if(d&0x80)
		{
		   SetSDA();
		}
		else
		{
		   ClrSDA();
		}
		d<<=1;
		somenop();
		SetSCL();
		somenop();
	}
	ClrSCL();
}

u8 receive(void)				//16us
{
	u8 i=8;
	u8 cbuf=0;
	SetSDA();
	SDA_IN();
	while(i--)
	{
		cbuf<<=1;
		ClrSCL();
		somenop();
		SetSCL();
		somenop();
		if(SDA_Read() == 1)
		{
		   cbuf|=1;
		}
		else
		{
		   cbuf|=0;
		}
	}
	SDA_OUT();
	ClrSCL();
	return (cbuf);
}

//指定地址写一个字节函数
u8 W_24(u16 addr,u8 d)		//70us
{
	u8 High,Low;
	u8 a;	
	High=(addr>>8)&0x00ff;
	Low=addr&0x00ff;	
	start();
	sendbyte(0xa0);
	a=ack();
	if(a==0)return 0;
	sendbyte(High);
	a=ack();
	if(a==0)return 0;
	sendbyte(Low);
	a=ack();
	if(a==0)return 0;
	sendbyte(d);
	a=ack();
	if(a==0)return 0;
	stop();	
	return 1;
}


u8 R_24(u16 addr)			//70us
{
	u8 High,Low,d;
	u8 a;
	High=(addr>>8)&0x00ff;
	Low=addr&0x00ff;
	start();
	sendbyte(0xa0);
	a=ack();
	if(a==0)return 0;
	sendbyte(High);
	a=ack();
	if(a==0)return 0;
	sendbyte(Low);
	a=ack();
	if(a==0)return 0;
	start();
	sendbyte(0xa1);
	a=ack();
	if(a==0)return 0;
	d=receive();
	sendnotack();
	stop();
	return d;
}

void saveintdata(u16 addr,u16 d)		//140us
{
	u8 i;
	union
		{
			u16 ibuf;
			u8 c[2];
		}s;
	s.ibuf=d;
	for(i=0;i<2;i++)
	{
		W_24(addr,s.c[i]);
		addr++;
	}
}
void saveint(u16 addr,u16 d)
{
	saveintdata(addr,d);
}
u16 readintdata(u16 addr)				//140us
{
	u8 i;
	union
		{
			u16 ibuf;
			u8 c[2];
		}s;
	for(i=0;i<2;i++)
	{
		s.c[i]=R_24(addr);
		addr++;
	}
	return (s.ibuf);
}
u16 readint(u16 addr)
{
	return readintdata(addr);
}
void savelongdata(u16 addr,unsigned long int d)		//140us
{
	u8 i;
	union
		{
			unsigned long int ibuf;
			u8 c[4];
		}s;
	s.ibuf=d;
	for(i=0;i<4;i++)
	{
		W_24(addr,s.c[i]);
		addr++;
	}
}

unsigned long int readlongdata(u16 addr)				//140us
{
	u8 i;
	union
		{
			unsigned long int ibuf;
			u8 c[4];
		}s;
	for(i=0;i<4;i++)
	{
		s.c[i]=R_24(addr);
		addr++;
	}
	return (s.ibuf);
}
void savefloat(u16 addr,float savevalue)		//280us
{
	u8 i;
	union
		{
			float fbuf;
			u8 c[4];
		}s;
	s.fbuf=savevalue;
	for(i=0;i<4;i++)
	{
	   W_24(addr+i,*(s.c+i));
	}
}
float readfloat(u16 addr)				//280us
{
	u8 i;
	union
		{
			float fbuf;
			u8 c[4];
		}s;
	for(i=0;i<4;i++)
	{
	   *(s.c+i)=R_24(addr+i);
	}
	return (s.fbuf);
}




//指定地址写一个字节函数
u8 at24c1024_W_24(u32 addr,u8 d)		//70us
{
	u8 High,Low,device_address;
	u8 a;

	if(addr >= 196608)
	{
		device_address = 0xaa;
		addr -= 131072;	
	}
	else if(addr >= 131072)
	{
		device_address = 0xa8;
		addr -= 131072;
	}	
	else if(addr >= 65536)
	{
		device_address = 0xa6;
	}
	else
	{
		device_address = 0xa4;
	}
	High=(addr>>8)&0x00ff;
	Low=addr&0x00ff;	
	start();
	sendbyte(device_address);
	a=ack();
	if(a==0)return 0;
	sendbyte(High);
	a=ack();
	if(a==0)return 0;
	sendbyte(Low);
	a=ack();
	if(a==0)return 0;
	sendbyte(d);
	a=ack();
	if(a==0)return 0;
	stop();
	delay_ms(5);
	return 1;
}


u8 at24c1024_R_24(u32 addr)			//70us
{
	u8 High,Low,d,device_address;
	u8 a;
	if(addr >= 196608)
	{
		device_address = 0xaa;
		addr -= 131072;	
	}
	else if(addr >= 131072)
	{
		device_address = 0xa8;
		addr -= 131072;
	}	
	else if(addr >= 65536)
	{
		device_address = 0xa6;
	}
	else
	{
		device_address = 0xa4;
	}

	High=(addr>>8)&0x00ff;
	Low=addr&0x00ff;
	start();
	sendbyte(device_address);
	a=ack();
	if(a==0)return 0;
	sendbyte(High);
	a=ack();
	if(a==0)return 0;
	sendbyte(Low);
	a=ack();
	if(a==0)return 0;
	start();
	sendbyte(device_address+1);
	a=ack();
	if(a==0)return 0;
	d=receive();
	sendnotack();
	stop();
	return d;

}

void at24c1024_saveint(u32 addr,u16 d)		//140us
{
	u8 i;
	union
		{
			u16 ibuf;
			u8 c[2];
		}s;
	s.ibuf=d;
	for(i=0;i<2;i++)
	{
		at24c1024_W_24(addr,s.c[i]);
		addr++;
	}
}

u16 at24c1024_readint(u32 addr)				//140us
{
	u8 i;
	union
		{
			u16 ibuf;
			u8 c[2];
		}s;
	for(i=0;i<2;i++)
	{
		s.c[i]=at24c1024_R_24(addr);
		addr++;
	}
	return (s.ibuf);
}

void at24c1024_savefloat(u32 addr,float savevalue)		//280us
{
	u8 i;
	union
		{
			float fbuf;
			u8 c[4];
		}s;
	s.fbuf=savevalue;
	for(i=0;i<4;i++)
	{
	   at24c1024_W_24(addr+i,*(s.c+i));
	}
}
float at24c1024_readfloat(u32 addr)				//280us
{
	u8 i;
	union
		{
			float fbuf;
			u8 c[4];
		}s;
	for(i=0;i<4;i++)
	{
	   *(s.c+i)=at24c1024_R_24(addr+i);
	}
	return (s.fbuf);
}

