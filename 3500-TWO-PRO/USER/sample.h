#ifndef __SAMPLE_H__
#define	__SAMPLE_H__	1

#include "parameter.h"

#define DF_INPUT_MODE				0

#define	DF_A_TEMPERATURE			0
#define DF_B_TEMPERATURE			1
#define DF_CTR_TEMPERATURE			2
#define	DF_ENVIRONMENT_TEMPERATURE	3

#define DF_FLOW_SET_MAX				120
#define DF_FLOW_SET_MIN				80
#define DF_A_FLOW_SET_MAX			1.0
#define DF_A_FLOW_SET_MIN			0.1
#define DF_B_FLOW_SET_MAX			1.0
#define DF_B_FLOW_SET_MIN			0.1

#define DF_A_DY_RATE		0.95166
#define DF_A_JY_RATE		0.00818
#define DF_A_FLOW_RATE		0.03028
#define DF_B_DY_RATE		0.95166
#define DF_B_JY_RATE		0.00818
#define DF_B_FLOW_RATE		0.03028
#define DF_BARAMETRIC_RATE	0.03507

#define DF_FLOW_CTR_KEDU		1
#define DF_FLOW_CTR_RUKOU		2
#define DF_FLOW_CTR_BIAOKUANG	3
struct FLOW_STRUCT
{
	float dy_rate;				//动压倍率
	float dy_zero;				//动压零点
	float jy_rate;				//计压倍率
	float jy_zero;				//计压零点
	float flow_set;				//设定流量
//	float flow_rate;			//流量倍率
	float sample_volume_set;	//采样体积设置
	u32 running_time_set;		//设定时间：秒
	u8 start_hour_set;		//开始时间：时
	u8 start_min_set;		//开始时间：分
	u32 jiange_time_set;		//设定间隔时间：秒
	u8 sample_times_set;		//设定采样次数
	float dy;					//动压
	float jy;					//计压
//	float temperature;			//计温
	float Sqrt_dy;				//动压开根
	u8 flow_ctr_select;		//选择流量控制方式（刻度、入口or标况）
	float flow;					//流量
	float flow_biaokuang;		//标况流量
	float flow_rukou;			//入口流量
	float flow_kedu;			//刻度流量
	float flow_ctr;				//控制流量(刻度，入口或标况中的一种)
	float volume;				//体积
	float volume_biaokuang;		//标况体积
	u32 running_time;			//已运行时间：秒
	u32 running_time_by_volume;	//按体积采样时间计时：秒
	u32 jiange_time;			//间隔时间：秒
	u8 sample_times;			//采样次数
	float environment_temperature_input;
	float barometric_input;
};
struct BAR
{
	float rate;
	float zero;
	u8 mode;
	float input;
	float value;	
};

struct TEMPERATURE
{
	u8 mode;
	float zero;
	float input;
	float value;
};

struct AB_FLOW_RATE
{
	float rate;
	float rate_array[11];
};
extern struct AB_FLOW_RATE A_flow_rate,B_flow_rate;
extern struct TEMPERATURE sample_temperature[8];
extern struct FLOW_STRUCT A_SAMPLE,B_SAMPLE;
extern struct BAR barometric;
//void GetPressure(void);
void ComputeFlow(void);
void GetTemperature(void);
float flow_amend_correct(float flow_amend_jy);
//float InvSqrt(float x);
#endif

