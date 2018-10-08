#ifndef __BAT_H
#define __BAT_H	 
#include "sys.h"									  
//////////////////////////////////////////////////////////////////////////////////   	 
#define POWER	PCin(3)
extern float charge_current;
extern float bat_voltage;
extern float bat_voltage_rate;
extern float charge_current_rate;
extern float heat_cool_current;
extern float heat_cool_current_rate;
void POWER_TEST_Init(void);	
		 					    
#endif

