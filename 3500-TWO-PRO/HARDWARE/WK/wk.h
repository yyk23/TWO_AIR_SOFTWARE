#ifndef __WK_H__
#define __WK_H__ 1
#include "sys.h"

extern u8 wk_re_cooling_flag;									//温度达到设定温度时才启动制冷
extern u8 wk_heating_flag;										//加热标志位（仅用作了加热指示）
extern u8 wk_cooling_flag;										//制冷标志位（仅用作了制冷指示）
extern u8 wk_heat_or_cool_flag;									//0:制冷，1：加热
extern u8 wk_cool_fan_delay;									//风扇延时时间
extern u8 pid_integral_limit;
extern float wk_start_heat_err;									//由制冷切换到加热时需要超过下偏差0.5℃，超级维护中可设

struct s_pid
{
    float pv;			  	//过程量
	float sp;			  	//设定值
	float integral;	  		//积分值
	float pgain;	  		//比例增益
	float igain;	  		//积分增益
	float dgain;	  		//微分增益
	float deadband;	  		//死区
	float last_error;	  	//
};
extern struct s_pid wk_pid_para;
void wk_GPIOInit(void);
float wk_pid_calc(struct s_pid *pid);
void wk_ctr(void);
void start_wk_ctr(void);
void stop_wk_ctr(void);
#endif /**/
