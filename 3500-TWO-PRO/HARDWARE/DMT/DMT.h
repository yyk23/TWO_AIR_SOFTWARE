#ifndef __DMT_H
#define __DMT_H			    
#define DF_FRAME_START1	0x5A
#define DF_FRAME_START2	0xA5

#define DEF_NO_RESPONSE				0
#define DEF_READ_REGISTER_RESPONSE	1
#define DEF_READ_VARIALBE_RESPONSE	2
#define DEF_INVALID_RESPONSE		3
//DMT64480T056????
#define CMD_READ_REGISTER_RESPONSE	0x81
#define CMD_READ_VARIALBE_RESPONSE	0x83

#define DEF_REGISTER_RTC			0x20

extern u8 DMT_year;
extern u8 DMT_month;
extern u8 DMT_day;
extern u8 DMT_week;
extern u8 DMT_hour;
extern u8 DMT_min;
extern u8 DMT_sec;

void DMT_Init(void);
void UART2_SendByte(u8 TxData);
void DMT_ChangePic(u16 PicID);
u8 changeBCD(u8 d);
u8 toBCD(u8 d);
void DMT_PutStr(u16 addr,u8 * str);
void DMT_ScreenCbuf(u16 addr,u8 wei,u8 cbuf);
void DMT_ScreenIbuf(u16 addr,u8 wei,u16 ibuf);
void DMT_ScreenLbuf(u16 addr,u8 wei,u32 lbuf);
void DMT_ScreenFbuf(u16 addr,const u8 * format,float fbuf);
void DMT_ScreenChar(u16 addr,s8 cbuf);
void DMT_ScreenInt(u16 addr,s16 ibuf);
void DMT_ScreenLongInt(u16 addr,s32 ibuf);
void DMT_ScreenFloat(u16 addr,uc8 * format,float fbuf);
void DMT_ScreenLongFloat(u16 addr,uc8 * format,float fbuf);

u8 DMT_GetMessage(u8 *pstr);
u16 DMT_GetVariableAddr(u8 * pstr);
u16 DMT_GetVariableIntValue(u8 * pstr);
u32 DMT_GetVariableLongValue(u8 * pstr);
u8 DMT_GetRegisterAddr(u8 * pstr);
void DMT_GetRTC(void);
void DMT_GetRTCValue(u8 * pstr);
void DMT_CmdReadRTC(void);
void DMT_WriteRTC(void);
void DMT_CutPicture(u16 x, u16 y, u16 add, u16 pic, u16 *p );
void DMT_SetBackLight(u8 light);
void DMT_Buz(u8 buz10ms);
void UART2_init(void);
void UART2_SendByte(u8 TxData);
void DMT_CmdReadTPStatus(void);
u8 RTC_AnalysisDataBuf(u8 *pstr);
void DMT_SetTPSensitivity(u8 sensitivity);
#endif
