#ifndef __WK_H__
#define __WK_H__ 1
#include "sys.h"

extern u8 wk_re_cooling_flag;									//�¶ȴﵽ�趨�¶�ʱ����������
extern u8 wk_heating_flag;										//���ȱ�־λ���������˼���ָʾ��
extern u8 wk_cooling_flag;										//�����־λ��������������ָʾ��
extern u8 wk_heat_or_cool_flag;									//0:���䣬1������
extern u8 wk_cool_fan_delay;									//������ʱʱ��
extern u8 pid_integral_limit;
extern float wk_start_heat_err;									//�������л�������ʱ��Ҫ������ƫ��0.5�棬����ά���п���

struct s_pid
{
    float pv;			  	//������
	float sp;			  	//�趨ֵ
	float integral;	  		//����ֵ
	float pgain;	  		//��������
	float igain;	  		//��������
	float dgain;	  		//΢������
	float deadband;	  		//����
	float last_error;	  	//
};
extern struct s_pid wk_pid_para;
void wk_GPIOInit(void);
float wk_pid_calc(struct s_pid *pid);
void wk_ctr(void);
void start_wk_ctr(void);
void stop_wk_ctr(void);
#endif /**/
