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
	float dy_rate;				//��ѹ����
	float dy_zero;				//��ѹ���
	float jy_rate;				//��ѹ����
	float jy_zero;				//��ѹ���
	float flow_set;				//�趨����
//	float flow_rate;			//��������
	float sample_volume_set;	//�����������
	u32 running_time_set;		//�趨ʱ�䣺��
	u8 start_hour_set;		//��ʼʱ�䣺ʱ
	u8 start_min_set;		//��ʼʱ�䣺��
	u32 jiange_time_set;		//�趨���ʱ�䣺��
	u8 sample_times_set;		//�趨��������
	float dy;					//��ѹ
	float jy;					//��ѹ
//	float temperature;			//����
	float Sqrt_dy;				//��ѹ����
	u8 flow_ctr_select;		//ѡ���������Ʒ�ʽ���̶ȡ����or�����
	float flow;					//����
	float flow_biaokuang;		//�������
	float flow_rukou;			//�������
	float flow_kedu;			//�̶�����
	float flow_ctr;				//��������(�̶ȣ���ڻ����е�һ��)
	float volume;				//���
	float volume_biaokuang;		//������
	u32 running_time;			//������ʱ�䣺��
	u32 running_time_by_volume;	//���������ʱ���ʱ����
	u32 jiange_time;			//���ʱ�䣺��
	u8 sample_times;			//��������
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

