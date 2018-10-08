#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

#define		DF_A_LIUYA			0
#define 	DF_A_JIYA			1
#define		DF_B_LIUYA			2
#define		DF_B_JIYA			3
#define		DF_QIYA				4
#define 	DF_BAT_VOLTAGE		5
#define 	DF_CHARGE_CURRENT	6   
#define 	DF_HEAT_COOL_CURRENT	7
//extern u16 adc_test_count;
extern u16 ADC_Table[500];

extern float ADBuf[16];					//ad»º³åÇø
extern u32 adcount;					//ad´ÎÊý
//void  Adc_Init(void);
void ADC1_Init(void);
void ADC1_Mode_Config(void);
void ADC1_Start(void);
void ADC1_Stop(void);
void Process_ADC(void);
u16  Get_Adc(u8 ch); 
float Get_Adc_Average(u8 ch,u8 times); 
u16 Get_Adc3(u8 ch);
void  Adc3_Init(void); 
void ADC_SUM(void);
#endif 
