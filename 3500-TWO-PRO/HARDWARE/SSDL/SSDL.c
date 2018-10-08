#include "SSDL.h" 
#include "delay.h"
#define DF_Command 4
#define ReadTwo (DF_Command-2)/2
#define SSDL_SCLK PEout(10)
#define SSDL_MSIO PEin(9)
#define SSDL_SS_LY_A   PBout(10)
#define SSDL_SS_LY_B   PEout(13)
#define SSDL_SS_JY_A   PEout(11)
#define SSDL_SS_JY_B   PEout(12)
#define SSDL_SS_QY     PEout(2)
u8 Buf_Pressure[4];
float Temperature_LY_A,Temperature_JY_A,Temperature_QY,Temperature_LY_B,Temperature_JY_B;
void SSDL_port_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_SetBits(GPIOE,GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_10);
}

u8 GetOneByte(void)
{   
	u8 Data=0; 
	u8 i; 
	for (i=0; i<8; i++) 
	{ 
		SSDL_SCLK = 1; 
		delay_us(2); 
		Data = Data << 1; 
		if (SSDL_MSIO) 
		Data = Data | 1; 
		SSDL_SCLK = 0; 
		delay_us(2); 
	} 
	return (Data);  
} 

void ReadSSDL_JY_A(u8 Comm)
{
	u8 i;
	SSDL_SCLK = 0; 
	SSDL_SS_JY_A = 0; 
	delay_us(2); 
	for (i = 0; i < Comm; i++) 
	{ 
		Buf_Pressure[Comm-1 - i] = GetOneByte ();       /* 1 byte of read sequence */ 
	} 
	SSDL_SS_JY_A = 1; 
	delay_us(2); 
}


float Read_Pressure_JY_A(u8 PressChannel)
{
	float Pressure = 0.0,Temperature=0; 
	u16 Dpressure = 0,Dtemperature=0; 
	float P1=1638.3;     /* P1= 10% * 16383 ñ B type*/ 
	float P2=13106.4;    /* P2= 80% * 16383 ñ B type*/ 
	float Pmax=40.0; 
	float Pmin=-40.0; 
	
	ReadSSDL_JY_A(DF_Command);      /*Read_DF4 command ñ data fetch 4 bytes */ 

	Dpressure = ((u16) (Buf_Pressure[DF_Command-1] & 0x3f) <<8) + (Buf_Pressure[DF_Command-2]); 
	#ifdef	ReadTwo	
		Dtemperature = Buf_Pressure [DF_Command-3];
        Dtemperature = (Dtemperature * 256 + Buf_Pressure [DF_Command-4]) / 32;
	#endif

	Pressure = (((float)Dpressure) - P1) * (Pmax - Pmin) / P2 + Pmin; 
	Temperature_JY_A = ((float) Dtemperature) * 200.0 / 2047.0 -50.0; 
	return (Pressure);
}
void ReadSSDL_LY_A(u8 Comm)
{
	u8 i;
	SSDL_SCLK = 0; 
	SSDL_SS_LY_A = 0; 
	delay_us(2); 
	for (i = 0; i < Comm; i++) 
	{ 
		Buf_Pressure[Comm-1 - i] = GetOneByte ();       /* 1 byte of read sequence */ 
	} 
	SSDL_SS_LY_A = 1; 
	delay_us(2); 
}


float Read_Pressure_LY_A(u8 PressChannel)
{
	float Pressure = 0.0,Temperature=0; 
	u16 Dpressure = 0,Dtemperature=0; 
	float P1=1638.3;     /* P1= 10% * 16383 ñ B type*/ 
	float P2=13106.4;    /* P2= 80% * 16383 ñ B type*/ 
	float Pmax=2500; 
	float Pmin=0; 
	
	ReadSSDL_LY_A(DF_Command);      /*Read_DF4 command ñ data fetch 4 bytes */ 

	Dpressure = ((u16) (Buf_Pressure[DF_Command-1] & 0x3f) <<8) + (Buf_Pressure[DF_Command-2]); 
//    return (Dpressure);
	#ifdef	ReadTwo	
        Dtemperature = Buf_Pressure [DF_Command-3];
        Dtemperature = (Dtemperature * 256 + Buf_Pressure [DF_Command-4]) / 32;
	#endif

	Pressure = (((float)Dpressure) - P1) * (Pmax - Pmin) / P2 + Pmin; 
	Temperature_LY_A = ((float) Dtemperature) * 200 / 2047 -50; 
	return (Pressure);
}


void ReadSSDL_JY_B(u8 Comm)
{
	u8 i;
	SSDL_SCLK = 0; 
	SSDL_SS_JY_B = 0; 
	delay_us(2); 
	for (i = 0; i < Comm; i++) 
	{ 
		Buf_Pressure[Comm-1 - i] = GetOneByte ();       /* 1 byte of read sequence */ 
	} 
	SSDL_SS_JY_B = 1; 
	delay_us(2); 
}


float Read_Pressure_JY_B(u8 PressChannel)
{
	float Pressure = 0.0,Temperature=0; 
	u16 Dpressure = 0,Dtemperature=0; 
	float P1=1638.3;     /* P1= 10% * 16383 ñ B type*/ 
	float P2=13106.4;    /* P2= 80% * 16383 ñ B type*/ 
	float Pmax=40.0; 
	float Pmin=-40.0; 
	
	ReadSSDL_JY_B(DF_Command);      /*Read_DF4 command ñ data fetch 4 bytes */ 

	Dpressure = ((u16) (Buf_Pressure[DF_Command-1] & 0x3f) <<8) + (Buf_Pressure[DF_Command-2]); 
	#ifdef	ReadTwo	
		Dtemperature = Buf_Pressure [DF_Command-3];
        Dtemperature = (Dtemperature * 256 + Buf_Pressure [DF_Command-4]) / 32;
	#endif

	Pressure = (((float)Dpressure) - P1) * (Pmax - Pmin) / P2 + Pmin; 
	Temperature_JY_B = ((float) Dtemperature) * 200.0 / 2047.0 -50.0; 
	return (Pressure);
}
void ReadSSDL_LY_B(u8 Comm)
{
	u8 i;
	SSDL_SCLK = 0; 
	SSDL_SS_LY_B = 0; 
	delay_us(2); 
	for (i = 0; i < Comm; i++) 
	{ 
		Buf_Pressure[Comm-1 - i] = GetOneByte ();       /* 1 byte of read sequence */ 
	} 
	SSDL_SS_LY_B = 1; 
	delay_us(2); 
}


float Read_Pressure_LY_B(u8 PressChannel)
{
	float Pressure = 0.0,Temperature=0; 
	u16 Dpressure = 0,Dtemperature=0; 
	float P1=1638.3;     /* P1= 10% * 16383 ñ B type*/ 
	float P2=13106.4;    /* P2= 80% * 16383 ñ B type*/ 
	float Pmax=2500; 
	float Pmin=0; 
	
	ReadSSDL_LY_B(DF_Command);      /*Read_DF4 command ñ data fetch 4 bytes */ 

	Dpressure = ((u16) (Buf_Pressure[DF_Command-1] & 0x3f) <<8) + (Buf_Pressure[DF_Command-2]); 
//    return (Dpressure);
	#ifdef	ReadTwo	
        Dtemperature = Buf_Pressure [DF_Command-3];
        Dtemperature = (Dtemperature * 256 + Buf_Pressure [DF_Command-4]) / 32;
	#endif

	Pressure = (((float)Dpressure) - P1) * (Pmax - Pmin) / P2 + Pmin; 
	Temperature_LY_B = ((float) Dtemperature) * 200 / 2047 -50; 
	return (Pressure);
}
void ReadSSDL_QY(u8 Comm)
{
	u8 i;
	SSDL_SCLK = 0; 
	SSDL_SS_QY = 0; 
	delay_us(2); 
	for (i = 0; i < Comm; i++) 
	{ 
		Buf_Pressure[Comm-1 - i] = GetOneByte ();       /* 1 byte of read sequence */ 
	} 
	SSDL_SS_QY = 1; 
	delay_us(2); 
}


float Read_Pressure_QY(u8 PressChannel)
{
	float Pressure = 0.0,Temperature=0; 
	u16 Dpressure = 0,Dtemperature=0; 
	float P1=1638.3;     /* P1= 10% * 16383 ñ B type*/ 
	float P2=13106.6;    /* P2= 80% * 16383 ñ B type*/ 
	float Pmax=120.0; 
	float Pmin=0.0; 
	static float Pressure_tmp = 101.325;
	ReadSSDL_QY(DF_Command);      /*Read_DF4 command ñ data fetch 4 bytes */ 
    if(Buf_Pressure[DF_Command-1] > 0x3f) // ˝æ›Œﬁ–ß£¨ π”√…œ“ª¥Œ”––ß÷µ
    {
        Pressure = Pressure_tmp;
        return (Pressure); 
    }
	Dpressure = ((u16) (Buf_Pressure[DF_Command-1] & 0x3f) <<8) + (Buf_Pressure[DF_Command-2]); 
	#ifdef	ReadTwo	
		Dtemperature = Buf_Pressure [DF_Command-3];
        Dtemperature = (Dtemperature * 256 + Buf_Pressure [DF_Command-4]) / 32;
	#endif

	Pressure = (((float)Dpressure) - P1) * (Pmax - Pmin) / P2 + Pmin; 
	Temperature_QY = ((float) Dtemperature) * 200 / 2047 -50; 
    Pressure_tmp = Pressure;
	return (Pressure);
}
