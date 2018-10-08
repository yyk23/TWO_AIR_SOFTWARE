
#ifndef __DS1302_H_
#define __DS1302_H_ 1
//-------define--------------
#define uint u16
#define uchar u8

extern uchar year,month,day,hour,min,sec;

void DS1302_GPIOInit(void);
void W1302(uchar addr,uchar d);
uchar R1302(uchar addr);
void ReadDate(void);
void ReadTime(void);
void DS1302_Charge(void);
void DS1302_SetTime(uchar set_hour,uchar set_min,uchar set_sec);
void DS1302_SetDate(uchar set_year,uchar set_month,uchar set_day);
void DS1302_GetRTC(void);
uchar changeBCD(uchar d);
uchar toBCD(uchar d);
#endif

