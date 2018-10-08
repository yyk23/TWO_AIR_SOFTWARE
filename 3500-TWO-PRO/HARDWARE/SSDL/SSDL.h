#ifndef __SSDL_H
#define __SSDL_H
#include "sys.h"
void SSDL_port_init(void);
float Read_Pressure_LY_A(u8 PressChannel);	    
float Read_Pressure_JY_A(u8 PressChannel);
float Read_Pressure_LY_B(u8 PressChannel);
float Read_Pressure_JY_B(u8 PressChannel);
float Read_Pressure_QY(u8 PressChannel);
#endif
