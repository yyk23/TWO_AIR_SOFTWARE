#ifndef __PUMP_H
#define __PUMP_H			    
#include "sys.h" 

#define AB_PUMP_MAX		60000									//
#define AB_PUMP_LOW		1000									//
#define PUMP_SPEED			100										//

extern float PUMP_fbuf_A;									//
extern float PUMP_fbuf_B;									//

void TIM4_PWM_Init(u16 arr,u16 psc);
void PUMP_AB_PWM_Init(void);
void PUMP_A_Start(void);
void PUMP_A_Stop(void);
void PUMP_B_Start(void);
void PUMP_B_Stop(void);

#define BEEP_ON() DMT_Buz(50)
#define BEEP_OFF() DMT_Buz(0)
#endif
