#include "sample.h"
#include "adc.h"
#include "18b20.h"
#include "TASK.h"
#include "DMT.h"
#include <math.h>
struct FLOW_STRUCT A_SAMPLE,B_SAMPLE;
struct BAR barometric;
struct TEMPERATURE sample_temperature[8];
struct AB_FLOW_RATE A_flow_rate,B_flow_rate;

/*
float InvSqrt(float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x; // get bits for floating VALUE 
	i = 0x5f375a86- (i>>1); // gives initial guess y0
	x = *(float*)&i; // convert bits BACK to float
	x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
	return x;
}
*/

//void GetPressure(void)
//{

//}

float calc_flow_kedu(float bar_buf,float pj_buf,float t_buf)
{
	float fbuf;
	fbuf = ((float)273.15+kedu_temperature_set) / ((float)273.15+t_buf) / (float)101.325 / (bar_buf + pj_buf);
	if(fbuf < 0)
	{
		fbuf = 0;
	}
	fbuf = bar_buf * sqrt(fbuf);
	return fbuf;
}

void ComputeFlow(void)
{
	float fbuf;
	A_calc_flow_rate(A_SAMPLE.flow_set);
	B_calc_flow_rate(B_SAMPLE.flow_set);
//	GetTemperature();			//计算流量时必须得到温度和大气压值
	if(sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode == DF_INPUT_MODE)
	{
		A_SAMPLE.environment_temperature_input = sample_temperature[DF_ENVIRONMENT_TEMPERATURE].input;
		B_SAMPLE.environment_temperature_input = sample_temperature[DF_ENVIRONMENT_TEMPERATURE].input;
	}
	else
	{
		A_SAMPLE.environment_temperature_input = sample_temperature[DF_ENVIRONMENT_TEMPERATURE].value;
		B_SAMPLE.environment_temperature_input = sample_temperature[DF_ENVIRONMENT_TEMPERATURE].value;
	}
	if(barometric.mode == DF_INPUT_MODE)
	{
		A_SAMPLE.barometric_input = barometric.input;
		B_SAMPLE.barometric_input = barometric.input;
	}
	else
	{
		A_SAMPLE.barometric_input = barometric.value;
		B_SAMPLE.barometric_input = barometric.value;
	}

	fbuf = A_SAMPLE.dy;
	if(fbuf < 5)																//小于2Pa则忽略，防止漂移
	{
		fbuf = 0;
	}	
    fbuf = fbuf / (float)1.34 * ((float)273.15 + sample_temperature[DF_A_TEMPERATURE].value) / (float)273.15 * (float)101.325 / (A_SAMPLE.barometric_input + A_SAMPLE.jy);
	if(fbuf < 0)
	{
		fbuf = 0;
		A_SAMPLE.flow = 0;
	}
	else
	{
		A_SAMPLE.flow = sqrt(fbuf)*DF_A_FLOW_RATE * A_flow_rate.rate;								//计算流量计处流量(计前流量)
	}
	A_SAMPLE.flow_rukou = A_SAMPLE.flow * (A_SAMPLE.barometric_input + A_SAMPLE.jy) / A_SAMPLE.barometric_input * ((float)273.15 + A_SAMPLE.environment_temperature_input) / ((float)273.15 + sample_temperature[DF_A_TEMPERATURE].value);//入口流量
	A_SAMPLE.flow_kedu = A_SAMPLE.flow_rukou * calc_flow_kedu(A_SAMPLE.barometric_input,A_SAMPLE.jy,A_SAMPLE.environment_temperature_input);								//刻度流量
	A_SAMPLE.flow_biaokuang = A_SAMPLE.flow * (A_SAMPLE.barometric_input + A_SAMPLE.jy) / (float)101.325 * ((float)273.15 + (float)0.0) / ((float)273.15 + sample_temperature[DF_A_TEMPERATURE].value);//0℃标况流量
	if(DF_FLOW_CTR_KEDU == A_SAMPLE.flow_ctr_select)
	{
		A_SAMPLE.flow_ctr = A_SAMPLE.flow_kedu;
	}
	else if(DF_FLOW_CTR_RUKOU == A_SAMPLE.flow_ctr_select)
	{
		A_SAMPLE.flow_ctr = A_SAMPLE.flow_rukou;
	}
	else if(DF_FLOW_CTR_BIAOKUANG == A_SAMPLE.flow_ctr_select)
	{
		A_SAMPLE.flow_ctr = A_SAMPLE.flow_biaokuang;
	}
	else
	{
		A_SAMPLE.flow_ctr = A_SAMPLE.flow_kedu;	
	}
	fbuf = B_SAMPLE.dy;
	if(fbuf < 5)																//小于2Pa则忽略，防止漂移
	{
		fbuf = 0;
	}	
    fbuf = fbuf / (float)1.34 * ((float)273.15 + sample_temperature[DF_B_TEMPERATURE].value) / (float)273.15 * (float)101.325 / (B_SAMPLE.barometric_input + B_SAMPLE.jy);
	if(fbuf < 0)
	{
		fbuf = 0;
		B_SAMPLE.flow = 0;
	}
	else
	{
		B_SAMPLE.flow = sqrt(fbuf)*DF_B_FLOW_RATE * B_flow_rate.rate;				//计算流量计处流量(计前流量)
	}
	B_SAMPLE.flow_rukou = B_SAMPLE.flow * (B_SAMPLE.barometric_input + B_SAMPLE.jy) / B_SAMPLE.barometric_input * ((float)273.15 + B_SAMPLE.environment_temperature_input) / ((float)273.15 + sample_temperature[DF_B_TEMPERATURE].value);//入口流量
	B_SAMPLE.flow_kedu = B_SAMPLE.flow_rukou * calc_flow_kedu(B_SAMPLE.barometric_input,B_SAMPLE.jy,B_SAMPLE.environment_temperature_input);								//刻度流量
 	B_SAMPLE.flow_biaokuang = B_SAMPLE.flow * (B_SAMPLE.barometric_input + B_SAMPLE.jy) / (float)101.325 * ((float)273.15 + (float)0.0) / ((float)273.15 + sample_temperature[DF_B_TEMPERATURE].value);//0℃标况流量
	if(DF_FLOW_CTR_KEDU == B_SAMPLE.flow_ctr_select)
	{
		B_SAMPLE.flow_ctr = B_SAMPLE.flow_kedu;
	}
	else if(DF_FLOW_CTR_RUKOU == B_SAMPLE.flow_ctr_select)
	{
		B_SAMPLE.flow_ctr = B_SAMPLE.flow_rukou;
	}
	else if(DF_FLOW_CTR_BIAOKUANG == B_SAMPLE.flow_ctr_select)
	{
		B_SAMPLE.flow_ctr = B_SAMPLE.flow_biaokuang;
	}
	else
	{
		B_SAMPLE.flow_ctr = B_SAMPLE.flow_kedu;	
	}
}

void GetTemperature(void)
{
	float farray[8];
	Temperature_read(farray);
	sample_temperature[DF_ENVIRONMENT_TEMPERATURE].value = farray[DF_ENVIRONMENT_TEMPERATURE] + 110 - sample_temperature[DF_ENVIRONMENT_TEMPERATURE].zero;
	sample_temperature[DF_CTR_TEMPERATURE].value = farray[DF_CTR_TEMPERATURE] + 110 - sample_temperature[DF_CTR_TEMPERATURE].zero;
	sample_temperature[DF_A_TEMPERATURE].value = farray[DF_A_TEMPERATURE] + 110 - sample_temperature[DF_A_TEMPERATURE].zero;
	sample_temperature[DF_B_TEMPERATURE].value = farray[DF_B_TEMPERATURE] + 110 - sample_temperature[DF_B_TEMPERATURE].zero;
	wk_temperature = sample_temperature[DF_CTR_TEMPERATURE].value;		//
}





















