
//------------include------------
#include "sys.h"
#include "delay.h"
#include "DMT.h"
#include "usart.h"
#include "process.h"
#include "parameter.h"
#include "task.h"
extern u8 R_flag;
u8 DMT_year = 0;
u8 DMT_month = 0;
u8 DMT_day = 0;
u8 DMT_week = 0;
u8 DMT_hour = 0;
u8 DMT_min = 0;
u8 DMT_sec = 0;


//--------BCD --------??-----------
u8 changeBCD(u8 d)
{
	u8 t;
	t=d&0xf0;
	t>>=4;
	d&=0x0f;
	d=10*t+d;
	return d;
}

//---------h to BCD------------------

u8 toBCD(u8 d)
{
	u8 t;
	t = d/10;
	t<<=4;
	d %= 10;
	d += t;
	return d;
}

void DMT_Init(void)
{
	uart5_init(115200);
}


void DMT_ChangePic(u16 PicID)
{
    u8 i;
	u8 command_buf[7] = {DF_FRAME_START1,DF_FRAME_START2,0x04,0x80,0x03,0x00,0x00};
	if(display_current_pic != PicID)
	{
		display_last_pic = display_current_pic;
	}
	display_current_pic = PicID;
	command_buf[5] = (u8)(PicID >> 8);
	command_buf[6] = (u8)(PicID);
	for(i = 0; i < 7; i++)
	{
		UART5_SendByte(command_buf[i]);
	}    
}



u8 AnalysisDataBuf(u8 *pstr)
{
	if(R_flag != 1)
	{
		return DEF_NO_RESPONSE;
	}
	R_flag = 0;

	if(pstr[1] == CMD_READ_REGISTER_RESPONSE)	//
	{
	    
		return DEF_READ_REGISTER_RESPONSE;
	}
	if(pstr[1] == CMD_READ_VARIALBE_RESPONSE)	//
	{

		return DEF_READ_VARIALBE_RESPONSE;
	}

	return DEF_INVALID_RESPONSE;
}

u8 RTC_AnalysisDataBuf(u8 *pstr)
{
	if(R_flag_FRAME_BUF2 != 1)
	{
		return DEF_NO_RESPONSE;
	}
	R_flag_FRAME_BUF2 = 0;

	if(pstr[1] == CMD_READ_REGISTER_RESPONSE)	//
	{
	    
		return DEF_READ_REGISTER_RESPONSE;
	}
	if(pstr[1] == CMD_READ_VARIALBE_RESPONSE)	//
	{

		return DEF_READ_VARIALBE_RESPONSE;
	}

	return DEF_INVALID_RESPONSE;
}

u8 DMT_GetMessage(u8 *pstr)
{
	u8 cbuf;
	cbuf = AnalysisDataBuf(pstr);
	return cbuf;
}

u16 DMT_GetVariableAddr(u8 * pstr)
{
	u16 tmp;
	tmp = (pstr[2] << 8) + pstr[3];
	return tmp;
}

u16 DMT_GetVariableIntValue(u8 * pstr)
{
	u16 tmp;
	tmp = (pstr[5] << 8) + pstr[6];
	return tmp;
}

u32 DMT_GetVariableLongValue(u8 * pstr)
{
	u32 tmp;
	tmp = ((u32)pstr[5] << 24) + ((u32)pstr[6] << 16) + ((u32)pstr[7] << 8) + ((u32)pstr[8] << 0);
	return tmp;
}

u8 DMT_GetRegisterAddr(u8 * pstr)
{
	u8 tmp;
	tmp = pstr[2];
	return tmp;
}
void DMT_GetRTCValue(u8 * pstr)
{
	DMT_year = changeBCD(pstr[4]);
	DMT_month = changeBCD(pstr[5]);
	DMT_day = changeBCD(pstr[6]);
	DMT_week = changeBCD(pstr[7]);
	DMT_hour = changeBCD(pstr[8]);
	DMT_min = changeBCD(pstr[9]);	
	DMT_sec = changeBCD(pstr[10]);
} 


void DMT_PutStr(u16 addr,u8 * str)
{
    u8 i = 0;
	u8 length = 0;
	u8 command_buf[7];
	while(*(str + i) != 0)
	{
		i++;
		length++;
	}
	command_buf[0] = DF_FRAME_START1;
	command_buf[1] = DF_FRAME_START2;
	command_buf[2] = 0x03 + length; 
	command_buf[3] = 0x82;
	command_buf[4] = (u8)(addr >> 8);
	command_buf[5] = (u8)(addr);
	for(i = 0; i < 6; i++)
	{
		UART5_SendByte(command_buf[i]);
	}
	while(*str != 0)				
	{
		UART5_SendByte(*(str++));
	} 	   
}

void DMT_ScreenCbuf(u16 addr,u8 wei,u8 cbuf)
{
    u8 i = 0;
	u8 c[10];
	u8 command_buf[7];
	
	command_buf[0] = DF_FRAME_START1;
	command_buf[1] = DF_FRAME_START2;
	command_buf[2] = 0x03 + wei; 
	command_buf[3] = 0x82;
	command_buf[4] = (u8)(addr >> 8);
	command_buf[5] = (u8)(addr);
	for(i = 0; i < 6; i++)
	{
		UART5_SendByte(command_buf[i]);
	}
	CharToStr(wei,c,cbuf);
	for(i = 0; i < wei; i++)
	{
		UART5_SendByte(c[i]);
	} 	   
}

void DMT_ScreenIbuf(u16 addr,u8 wei,u16 ibuf)
{
    u8 i = 0;
	u8 c[10];
	u8 command_buf[7];
	
	command_buf[0] = DF_FRAME_START1;
	command_buf[1] = DF_FRAME_START2;
	command_buf[2] = 0x03 + wei; 
	command_buf[3] = 0x82;
	command_buf[4] = (u8)(addr >> 8);
	command_buf[5] = (u8)(addr);
	for(i = 0; i < 6; i++)
	{
		UART5_SendByte(command_buf[i]);
	}
	SPuti(wei,c,ibuf);
	for(i = 0; i < wei; i++)
	{
		UART5_SendByte(c[i]);
	} 	   
}

void DMT_ScreenLbuf(u16 addr,u8 wei,u32 lbuf)
{
    u8 i = 0;
	u8 c[10];
	u8 command_buf[7];
	
	command_buf[0] = DF_FRAME_START1;
	command_buf[1] = DF_FRAME_START2;
	command_buf[2] = 0x03 + wei; 
	command_buf[3] = 0x82;
	command_buf[4] = (u8)(addr >> 8);
	command_buf[5] = (u8)(addr);
	for(i = 0; i < 6; i++)
	{
		UART5_SendByte(command_buf[i]);
	}
	SPutl(wei,c,lbuf);
	for(i = 0; i < wei; i++)
	{
		UART5_SendByte(c[i]);
	} 	   
}

void DMT_ScreenFbuf(u16 addr,const u8 * format,float fbuf)
{
    u8 i = 0;
	u8 c[10];
	u8 command_buf[7];
	u8 wei = 0;

	SPutf(format,c,fbuf);
	while(c[wei] != 0)
	{
		wei++;
	}
	command_buf[0] = DF_FRAME_START1;
	command_buf[1] = DF_FRAME_START2;
	command_buf[2] = 0x03 + wei; 
	command_buf[3] = 0x82;
	command_buf[4] = (u8)(addr >> 8);
	command_buf[5] = (u8)(addr);
	for(i = 0; i < 6; i++)
	{
		UART5_SendByte(command_buf[i]);
	}
	for(i = 0; i < wei; i++)
	{
		UART5_SendByte(c[i]);
	} 	   
}


void DMT_ScreenChar(u16 addr,s8 cbuf)
{
    u8 i = 0;
	u8 command_buf[7];
	u8 wei = 2;
	s16 tmp;
	tmp = cbuf;
	command_buf[0] = DF_FRAME_START1;
	command_buf[1] = DF_FRAME_START2;
	command_buf[2] = 0x03 + wei; 
	command_buf[3] = 0x82;
	command_buf[4] = (u8)(addr >> 8);
	command_buf[5] = (u8)(addr);
	for(i = 0; i < 6; i++)
	{
		UART5_SendByte(command_buf[i]);
	}
	UART5_SendByte(tmp >> 8);
	UART5_SendByte(tmp);	   
}

void DMT_ScreenInt(u16 addr,s16 ibuf)
{
    u8 i = 0;
	u8 command_buf[7];
	u8 wei = 2;
	s16 tmp;
	tmp = ibuf;
	command_buf[0] = DF_FRAME_START1;
	command_buf[1] = DF_FRAME_START2;
	command_buf[2] = 0x03 + wei; 
	command_buf[3] = 0x82;
	command_buf[4] = (u8)(addr >> 8);
	command_buf[5] = (u8)(addr);
	for(i = 0; i < 6; i++)
	{
		UART5_SendByte(command_buf[i]);
	}
	UART5_SendByte(tmp >> 8);
	UART5_SendByte(tmp);	   
}

void DMT_ScreenLongInt(u16 addr,s32 ibuf)
{
    u8 i = 0;
	u8 command_buf[7];
	u8 wei = 4;
	s32 tmp;
	tmp = ibuf;
	command_buf[0] = DF_FRAME_START1;
	command_buf[1] = DF_FRAME_START2;
	command_buf[2] = 0x03 + wei; 
	command_buf[3] = 0x82;
	command_buf[4] = (u8)(addr >> 8);
	command_buf[5] = (u8)(addr);
	for(i = 0; i < 6; i++)
	{
		UART5_SendByte(command_buf[i]);
	}
	UART5_SendByte(tmp >> 24);
	UART5_SendByte(tmp >> 16);
	UART5_SendByte(tmp >> 8);
	UART5_SendByte(tmp);	   
}

void DMT_ScreenFloat(u16 addr,uc8 * format,float fbuf)
{
	s16 ibuf;
	u8 cval;
	u8 i = 0;
	u8 minus_flag = 0;
	if(fbuf < 0)
	{
		fbuf = -fbuf;			
		minus_flag = 1;
	}
	if(*format == '+')
	{
		cval = *(format + 2) - 0x30;
	}
	else
	{
		cval = *(format + 1) - 0x30;
	}
	for(i = 0; i < cval; i++)
	{
		fbuf *= 10;
	}
	fbuf = fbuf + (float)0.5;
	ibuf = fbuf;
	if(minus_flag != 0)
	{
		ibuf = -ibuf;
	}
	DMT_ScreenInt(addr,ibuf);	   
}

void DMT_ScreenLongFloat(u16 addr,uc8 * format,float fbuf)
{
	s32 libuf = 1;
	u8 cval;
	u8 i = 0;
	u8 minus_flag = 0;
	if(fbuf < 0)
	{
		fbuf = -fbuf;			
		minus_flag = 1;
	}
	if(*format == '+')
	{
		cval = *(format + 2) - 0x30;
	}
	else
	{
		cval = *(format + 1) - 0x30;
	}
	for(i = 0; i < cval; i++)
	{
		fbuf *= 10;
	}
	fbuf = fbuf + (float)0.5;
	libuf = fbuf;
	if(minus_flag != 0)
	{
		libuf = -libuf;
	}
	DMT_ScreenLongInt(addr,libuf);	   
}

void DMT_CmdReadRTC(void)
{
    u8 i = 0;
	u8 command_buf[7];
	
	command_buf[0] = DF_FRAME_START1;
	command_buf[1] = DF_FRAME_START2;
	command_buf[2] = 0x03; 
	command_buf[3] = 0x81;
	command_buf[4] = 0x20;
	command_buf[5] = 0x07;
	for(i = 0; i < 6; i++)
	{
		UART5_SendByte(command_buf[i]);
	} 	   
}

void DMT_GetRTC(void)
{
    u8 l_rtc_flag;
	u8 i = 0;
	DMT_CmdReadRTC();
	while(1)
	{
		delay_ms(1);
		l_rtc_flag = RTC_AnalysisDataBuf(FRAME_BUF2);
		if(l_rtc_flag == DEF_READ_REGISTER_RESPONSE)
		{
			l_rtc_flag = DMT_GetRegisterAddr(FRAME_BUF2);
			if(l_rtc_flag == DEF_REGISTER_RTC)
			{
				DMT_GetRTCValue(FRAME_BUF2);
				return;
			}
		}
//		else if(l_rtc_flag == DEF_READ_VARIALBE_RESPONSE)	//
//		{													//
//			FRAME_BUF[0] = 0xFF;
//			return;
//		}
		i++;
		if(i >= 30) 	//
		{
			return;
		}
	}	   
}

void DMT_WriteRTC(void)
{
    u8 i = 0;
	u8 command_buf[14];	
	command_buf[0] = DF_FRAME_START1;
	command_buf[1] = DF_FRAME_START2;
	command_buf[2] = 0x0A; 
	command_buf[3] = 0x80;
	command_buf[4] = 0x1F;
	command_buf[5] = 0x5A;
	command_buf[6] = DMT_year;
	command_buf[7] = DMT_month;
	command_buf[8] = DMT_day;
	command_buf[9] = 0;	   				//
	command_buf[10] = DMT_hour;
	command_buf[11] = DMT_min;
	command_buf[12] = DMT_sec;
	for(i = 0; i < 13; i++)
	{
		UART5_SendByte(command_buf[i]);
	} 	   
}



void DMT_CutPicture(u16 x, u16 y, u16 add, u16 pic, u16 *p )
{
    u8 buf[26];
	u8 i;
	buf[0] = DF_FRAME_START1;
	buf[1] = DF_FRAME_START2;

	buf[2] = 23;     //len
	buf[3] = 0x82;     //

	buf[4] = add/256; //
	buf[5] = add%256; //

	buf[6] = 0x00; //
	buf[7] = 0x06; //

	buf[8] = 0x00; //
	buf[9] = 0x05; //

	buf[10] = pic/256; //
	buf[11] = pic%256; //

	buf[12] = (*p)/256; //Xs_H
	buf[13] = (*p)%256; //Xs_L
    p++;

	buf[14] = (*p)/256; //Ys_H
	buf[15] = (*p)%256; //Ys_L
    p++;

	buf[16] = (*p)/256; //Xe_H
	buf[17] = (*p)%256; //Xe_L
    p++;

	buf[18] = (*p)/256; //Ye_H
	buf[19] = (*p)%256; //Ye_L

	buf[20] = x/256;  //X_H
	buf[21] = x%256;  //X_L

	buf[22] = y/256;  //Y_H
	buf[23] = y%256;  //Y_L

	buf[24] = 0xFF;  //
	buf[25] = 0x00;  //

	for(i = 0; i < 26; i++)
	{
		UART5_SendByte(buf[i]);
	} 
}


void DMT_SetBackLight(u8 light)
{
    u8 i = 0;
	u8 command_buf[14];	
	command_buf[0] = DF_FRAME_START1;
	command_buf[1] = DF_FRAME_START2;
	command_buf[2] = 0x03; 	//?????????
	command_buf[3] = 0x80;	//??:????
	command_buf[4] = 0x01;	//?????,?????????
	command_buf[5] = light;
	for(i = 0; i < 6; i++)
	{
		UART5_SendByte(command_buf[i]);
	} 	 
}


void DMT_SetKey(u8 InputNum)
{
	u8 i;
	u8 LcdOrder[6] = {0x5A,0xA5,0x03,0x80,0x4F,0x01};
	LcdOrder[5] = InputNum;
	for(i = 0; i < 6; i++)
	{
		UART5_SendByte(LcdOrder[i]);
	} 
}

void DMT_Buz(u8 buz10ms)
{
	u8 i;
	u8 LcdOrder[6] = {0x5A,0xA5,0x03,0x80,0x02,0x01};
	LcdOrder[5] = buz10ms;
	for(i = 0; i < 6; i++)
	{
		UART5_SendByte(LcdOrder[i]);
	} 
}

void DMT_CmdReadTPStatus(void)
{
    u8 i = 0;
	u8 command_buf[7];
	
	command_buf[0] = DF_FRAME_START1;
	command_buf[1] = DF_FRAME_START2;
	command_buf[2] = 0x03; 
	command_buf[3] = 0x81;
	command_buf[4] = 0x06;
	command_buf[5] = 0x01;
	for(i = 0; i < 6; i++)
	{
		UART5_SendByte(command_buf[i]);
	} 	   
}

void DMT_SetTPSensitivity(u8 sensitivity)
{
    u8 i = 0;
	u8 command_buf[12];
	
	command_buf[0] = DF_FRAME_START1;
	command_buf[1] = DF_FRAME_START2;
	command_buf[2] = 0x08; 
	command_buf[3] = 0x80;
	command_buf[4] = 0xE0;
	command_buf[5] = 0x5A;
    command_buf[6] = 0x5A;
    command_buf[7] = sensitivity;
    command_buf[8] = 0x0C;
    command_buf[9] = 0x1E;
    command_buf[10] = 0x08;
	for(i = 0; i < 11; i++)
	{
		UART5_SendByte(command_buf[i]);
	} 	   
}

