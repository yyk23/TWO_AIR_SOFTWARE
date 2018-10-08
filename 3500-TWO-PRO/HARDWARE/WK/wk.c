
/* Includes --------------------------------*/

#include "math.h"
#include "wk.h"
#include "parameter.h"
#include "18b20.h"
#include "TASK.h"
#include "ZR3500_CONFIG.h"
#define DF_HEAT					1
#define DF_COOL					0
#define DEF_PWM_CTR_PERIOD 	  	10						   	//10s的控制周期
u8 wk_enable = 1;
#define WK_HEAT_ON();				PAout(6) = 1;
#define WK_HEAT_OFF();				PAout(6) = 0;
#define WK_COOL_ON();				PAout(7) = 1;
#define WK_COOL_OFF();				PAout(7) = 0;
#define WK_FAN_ON();				PCout(4) = 1;
#define WK_FAN_OFF();              	PCout(4) = 0;
u8 wk_re_cooling_flag = 0;									//温度达到设定温度时才启动制冷
u8 wk_heating_flag = 0;										//加热标志位（仅用作了加热指示）
u8 wk_cooling_flag = 0;										//制冷标志位（仅用作了制冷指示）
u8 wk_heat_or_cool_flag = 0;								//0:制冷，1：加热
u8 wk_cool_fan_delay = 0;									//风扇延时时间
struct s_pid wk_pid_para;
u8 pid_integral_limit = 0;
float wk_start_heat_err = 0.5;								//由制冷切换到加热时需要超过下偏差0.5℃，超级维护中可设
void wk_GPIOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
}

void start_wk_ctr(void)
{
	wk_enable = 1;
}
void stop_wk_ctr(void)
{
	wk_enable = 0;
}
//温度控制用PID方式
float wk_pid_calc(struct s_pid *pid)
{
    float err;
	float pterm,dterm,result,ferror;
	pterm = 0;
	dterm = 0;
	//计算偏差
	err = (pid->sp) - (pid->pv);
	
	//判断是否大于死区
	if(fabs(err) > pid->deadband)
	{
	    ferror = err;
		//比例项
		pterm = pid->pgain * ferror;					//比例项
		//积分项
		if(pterm > 10 || pterm < -10)
		{
		     pid->integral = 0;	  	   					//不再计算积分项
		}
		else
		{
		    pid->integral += pid->igain * ferror;		//计算积分项
			if(pid->integral > 10)
			{
			    pid->integral = 10;
			}
			else if(pid->integral < -10)
			{
			    pid->integral = -10;
			}
		}
		//微分项
		dterm = ((err - pid->last_error)) * pid->dgain;			//微分项
		//PID控制
		result = pterm + pid->integral + dterm;	   				//PID值
		//PD控制
		//result = pterm + dterm;	   							//PD值
		//P控制
		//result = pterm;
		
	}
	else
	{
		if(pid->integral > pid_integral_limit)
		{
			pid->integral = pid_integral_limit;
		}
		else if(pid->integral < -pid_integral_limit)
		{
			pid->integral = -pid_integral_limit;
		}
		else
		{
		
		}
		result = pid->integral;		//在死区范围内，保持现有输出
	}
	//保存上次偏差
	pid->last_error = err;
	//输出PID值
	return (result);
}

//制冷温度控制用PID方式

float wk_pid_calc_cool(struct s_pid *pid)
{
    float err;
	float pterm,dterm,result,ferror;
	pterm = 0;
	dterm = 0;
	//计算偏差
	err = (pid->sp) - (pid->pv);
	if(err > (pid->deadband))	//如果设定温度大于当前温度，重新制冷标志被清零
	{
		wk_re_cooling_flag = 0;
	}	
	else if(err <= 0)			//如果设定温度小于当前温度，重新制冷标志被置1
	{
		wk_re_cooling_flag = 1;
	}
	else
	{

	}
	//判断是否达到下偏差
	if(err >= (pid->deadband))	//已经达到下偏差，此时停止制冷
	{
		result = 0;				//停止制冷
		pid->integral = 0;		//积分项清零
		return (result);
	}
	else						//假如设定20度，实际温度大于19度，此时将设置温度程序调整到18度，即20-2*deadband
	{
		pid->sp = pid->sp - pid->deadband - pid->deadband;
	}
	err = (pid->sp) - (pid->pv);
	if(wk_re_cooling_flag == 0)	//如果重新制冷标志被清零，则不启动制冷
	{
		result = 0;				//停止制冷
		pid->integral = 0;		//积分项清零
		return (result);		
	}	
	if(fabs(err) > pid->deadband)
	{
	    ferror = err;
		//比例项
		pterm = pid->pgain * ferror;					//比例项
		//积分项
		if(pterm > 10 || pterm < -10)
		{
		     pid->integral = 0;	  	   					//不再计算积分项
		}
		else
		{
		    pid->integral += pid->igain * ferror;		//计算积分项
			if(pid->integral > 10)
			{
			    pid->integral = 10;
			}
			else if(pid->integral < -10)
			{
			    pid->integral = -10;
			}
		}
		//微分项
		dterm = ((err - pid->last_error)) * pid->dgain;			//微分项
		//PID控制
		result = pterm + pid->integral + dterm;	   				//PID值
		//PD控制
		//result = pterm + dterm;	   							//PD值
		//P控制
		//result = pterm;
		
	}
	else
	{
		if(pid->integral > pid_integral_limit)
		{
			pid->integral = pid_integral_limit;
		}
		else if(pid->integral < -pid_integral_limit)
		{
			pid->integral = -pid_integral_limit;
		}
		else
		{
		
		}
		result = pid->integral;		//在死区范围内，保持现有输出
	}
	//保存上次偏差
	pid->last_error = err;
	//输出PID值
	return (result);
}

void wk_off_ctr(void)
{
    if(WK_DELAY.flag == 1)
	{
		WK_DELAY.flag = 0;
        wk_enable = 0;
        WK_HEAT_OFF();										//关加热
        wk_heating_flag = 0;								//加热标志位清零（仅用作了不加热指示）	
        WK_COOL_OFF();										//关制冷
        wk_cooling_flag = 0;								//制冷标志位清零
        wk_cool_fan_delay = 0;
        WK_FAN_OFF();
    }
}


void wk_heat_ctr(void)
{
	static u8 pwm_ctr_period = 0;							//每次执行函数时加1，当大于10时，重新计数
	float pid_ctr_result;
	if(WK_DELAY.flag == 1)
	{
		WK_DELAY.flag = 0;
		if(wk_enable != 0)								//使能了温度控制
		{
			if(wk_temperature_err != 0)					//温控传感器故障时停止加热
			{
				WK_HEAT_OFF();								//关加热
				wk_heating_flag = 0;						//加热标志位清零（仅用作了不加热指示）
				WK_COOL_OFF();
                WK_FAN_OFF();
				wk_cooling_flag = 0;						//制冷标志位清零
				return;
			}
			pwm_ctr_period++;							//10s作为一个周期
			if(pwm_ctr_period > DEF_PWM_CTR_PERIOD)
			{
				pwm_ctr_period = 0;
			}
			wk_pid_para.sp = wk_temperature_set;
			wk_pid_para.pv = wk_temperature;
            pid_ctr_result = wk_pid_calc(&wk_pid_para);
            if(pid_ctr_result < 0)				  //不需要加热
            {

            }
            else
            {
                if(pid_ctr_result > DEF_PWM_CTR_PERIOD)
                {
                    pid_ctr_result = DEF_PWM_CTR_PERIOD;
                }
            }
            if(pid_ctr_result == DEF_PWM_CTR_PERIOD)			//输出100%功率
            {
                WK_HEAT_ON();										//开加热
                wk_heating_flag = 1;								//加热标志位置1（仅用作了加热指示）
            }
            else
            {
                if(pid_ctr_result > pwm_ctr_period)
                {
                    WK_HEAT_ON();										//开加热
                    wk_heating_flag = 1;								//加热标志位置1（仅用作了加热指示）
                }
                else
                {
                    WK_HEAT_OFF();										//关加热
                    wk_heating_flag = 0;								//加热标志位清零（仅用作了不加热指示）
                }
            }
		}
		else					//不使能温度控制
		{
			WK_HEAT_OFF();										//关加热
			wk_heating_flag = 0;								//加热标志位清零（仅用作了不加热指示）	
		}	
        WK_COOL_OFF();										//关制冷
        wk_cooling_flag = 0;								//制冷标志位清零
        wk_cool_fan_delay = 0;
        WK_FAN_OFF();
	}
}

void wk_heat_and_cool_ctr(void)
{
	static u8 pwm_ctr_period = 0;							//每次执行函数时加1，当大于10时，重新计数
	float pid_ctr_result;
	if(WK_DELAY.flag == 1)
	{
		WK_DELAY.flag = 0;
		wk_cool_fan_delay ++;
		if(wk_cool_fan_delay >= 60)
		{
			WK_FAN_OFF();
		}
		if(wk_enable != 0)								//使能了温度控制
		{
			if(wk_temperature_err != 0)					//温控传感器故障时停止加热
			{
				WK_HEAT_OFF();								//关加热
				wk_heating_flag = 0;						//加热标志位清零（仅用作了不加热指示）
				WK_COOL_OFF();
				wk_cooling_flag = 0;						//制冷标志位清零
				return;
			}
			pwm_ctr_period++;							//10s作为一个周期
			if(pwm_ctr_period > DEF_PWM_CTR_PERIOD)
			{
				pwm_ctr_period = 0;
			}
			wk_pid_para.sp = wk_temperature_set;
			wk_pid_para.pv = wk_temperature;
			if(wk_temperature > (wk_temperature_set + wk_pid_para.deadband))
			{
				if(wk_heat_or_cool_flag != DF_COOL)
				{
					wk_heat_or_cool_flag = DF_COOL;
					wk_pid_para.integral = 0;
					WK_HEAT_OFF();										//关加热
					wk_heating_flag = 0;								//加热标志位清零	
					WK_COOL_OFF();										//关制冷
					wk_cooling_flag = 0;								//制冷标志位清零
				}
				else
				{
				
				}
			}
			else if(wk_temperature < (wk_temperature_set - wk_pid_para.deadband - wk_start_heat_err))	//假如设置20度，死区1度，则温度降低到20-1-wk_start_heat_err时才启动加热
			{
				if(wk_heat_or_cool_flag != DF_HEAT)
				{
					wk_heat_or_cool_flag = DF_HEAT;
					wk_pid_para.integral = 0;
					WK_HEAT_OFF();										//关加热
					wk_heating_flag = 0;								//加热标志位清零	
					WK_COOL_OFF();										//关制冷
					wk_cooling_flag = 0;								//制冷标志位清零
				}
				else
				{
				
				}
			}
			else
			{

			}
			if(wk_heat_or_cool_flag == DF_HEAT)
			{
				pid_ctr_result = wk_pid_calc(&wk_pid_para);
				if(pid_ctr_result < 0)				  //不需要加热
				{
					//pid_ctr_result = 0;
					//wk_pid_para.integral = 0;
				}
				else
				{
					if(pid_ctr_result > DEF_PWM_CTR_PERIOD)
					{
						pid_ctr_result = DEF_PWM_CTR_PERIOD;
					}
				}
				if(pid_ctr_result == DEF_PWM_CTR_PERIOD)			//输出100%功率
				{
					WK_HEAT_ON();										//开加热
					wk_heating_flag = 1;								//加热标志位置1（仅用作了加热指示）
				}
				else
				{
					if(pid_ctr_result > pwm_ctr_period)
					{
						WK_HEAT_ON();										//开加热
						wk_heating_flag = 1;								//加热标志位置1（仅用作了加热指示）
					}
					else
					{
						WK_HEAT_OFF();										//关加热
						wk_heating_flag = 0;								//加热标志位清零（仅用作了不加热指示）
					}
				}
			}
			else
			{
				pid_ctr_result = wk_pid_calc_cool(&wk_pid_para);
				if(pid_ctr_result > 0)				  //不需要制冷
				{
					//pid_ctr_result = 0;
					//wk_pid_para.integral = 0;
				}
				else
				{
					if(pid_ctr_result < -DEF_PWM_CTR_PERIOD)
					{
						pid_ctr_result = -DEF_PWM_CTR_PERIOD;
					}
				}
				if(pid_ctr_result == -DEF_PWM_CTR_PERIOD)			//输出100%功率
				{
					WK_COOL_ON();										//开制冷
					wk_cooling_flag = 1;								//制冷标志位置1
					WK_FAN_ON();										//开风扇
					wk_cool_fan_delay = 0;								//重新计时
				}
				else
				{
					if(pid_ctr_result < -pwm_ctr_period)
					{
						WK_COOL_ON();										//开制冷
						wk_cooling_flag = 1;								//制冷标志位置1
						WK_FAN_ON();										//开风扇
						wk_cool_fan_delay = 0;								//重新计时
					}
					else
					{
						WK_COOL_OFF();										//关制冷
						wk_cooling_flag = 0;								//制冷标志位清零
					}
				}			
			}
		}
		else					//不使能温度控制
		{
			WK_HEAT_OFF();										//关加热
			wk_heating_flag = 0;								//加热标志位清零（仅用作了不加热指示）	
			WK_COOL_OFF();										//关制冷
			wk_cooling_flag = 0;								//制冷标志位清零
			wk_cool_fan_delay = 0;
			WK_FAN_OFF();
		}	
	}
}

void wk_ctr(void)
{
#ifdef ZR3500TWO
    wk_off_ctr();
#endif
#ifdef ZR3500TWO_HEAT
    wk_heat_ctr();
#endif
#ifdef ZR3500TWO_HEATandCOOL
    wk_heat_and_cool_ctr();
#endif
}

