
/* Includes --------------------------------*/

#include "math.h"
#include "wk.h"
#include "parameter.h"
#include "18b20.h"
#include "TASK.h"
#include "ZR3500_CONFIG.h"
#define DF_HEAT					1
#define DF_COOL					0
#define DEF_PWM_CTR_PERIOD 	  	10						   	//10s�Ŀ�������
u8 wk_enable = 1;
#define WK_HEAT_ON();				PAout(6) = 1;
#define WK_HEAT_OFF();				PAout(6) = 0;
#define WK_COOL_ON();				PAout(7) = 1;
#define WK_COOL_OFF();				PAout(7) = 0;
#define WK_FAN_ON();				PCout(4) = 1;
#define WK_FAN_OFF();              	PCout(4) = 0;
u8 wk_re_cooling_flag = 0;									//�¶ȴﵽ�趨�¶�ʱ����������
u8 wk_heating_flag = 0;										//���ȱ�־λ���������˼���ָʾ��
u8 wk_cooling_flag = 0;										//�����־λ��������������ָʾ��
u8 wk_heat_or_cool_flag = 0;								//0:���䣬1������
u8 wk_cool_fan_delay = 0;									//������ʱʱ��
struct s_pid wk_pid_para;
u8 pid_integral_limit = 0;
float wk_start_heat_err = 0.5;								//�������л�������ʱ��Ҫ������ƫ��0.5�棬����ά���п���
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
//�¶ȿ�����PID��ʽ
float wk_pid_calc(struct s_pid *pid)
{
    float err;
	float pterm,dterm,result,ferror;
	pterm = 0;
	dterm = 0;
	//����ƫ��
	err = (pid->sp) - (pid->pv);
	
	//�ж��Ƿ��������
	if(fabs(err) > pid->deadband)
	{
	    ferror = err;
		//������
		pterm = pid->pgain * ferror;					//������
		//������
		if(pterm > 10 || pterm < -10)
		{
		     pid->integral = 0;	  	   					//���ټ��������
		}
		else
		{
		    pid->integral += pid->igain * ferror;		//���������
			if(pid->integral > 10)
			{
			    pid->integral = 10;
			}
			else if(pid->integral < -10)
			{
			    pid->integral = -10;
			}
		}
		//΢����
		dterm = ((err - pid->last_error)) * pid->dgain;			//΢����
		//PID����
		result = pterm + pid->integral + dterm;	   				//PIDֵ
		//PD����
		//result = pterm + dterm;	   							//PDֵ
		//P����
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
		result = pid->integral;		//��������Χ�ڣ������������
	}
	//�����ϴ�ƫ��
	pid->last_error = err;
	//���PIDֵ
	return (result);
}

//�����¶ȿ�����PID��ʽ

float wk_pid_calc_cool(struct s_pid *pid)
{
    float err;
	float pterm,dterm,result,ferror;
	pterm = 0;
	dterm = 0;
	//����ƫ��
	err = (pid->sp) - (pid->pv);
	if(err > (pid->deadband))	//����趨�¶ȴ��ڵ�ǰ�¶ȣ����������־������
	{
		wk_re_cooling_flag = 0;
	}	
	else if(err <= 0)			//����趨�¶�С�ڵ�ǰ�¶ȣ����������־����1
	{
		wk_re_cooling_flag = 1;
	}
	else
	{

	}
	//�ж��Ƿ�ﵽ��ƫ��
	if(err >= (pid->deadband))	//�Ѿ��ﵽ��ƫ���ʱֹͣ����
	{
		result = 0;				//ֹͣ����
		pid->integral = 0;		//����������
		return (result);
	}
	else						//�����趨20�ȣ�ʵ���¶ȴ���19�ȣ���ʱ�������¶ȳ��������18�ȣ���20-2*deadband
	{
		pid->sp = pid->sp - pid->deadband - pid->deadband;
	}
	err = (pid->sp) - (pid->pv);
	if(wk_re_cooling_flag == 0)	//������������־�����㣬����������
	{
		result = 0;				//ֹͣ����
		pid->integral = 0;		//����������
		return (result);		
	}	
	if(fabs(err) > pid->deadband)
	{
	    ferror = err;
		//������
		pterm = pid->pgain * ferror;					//������
		//������
		if(pterm > 10 || pterm < -10)
		{
		     pid->integral = 0;	  	   					//���ټ��������
		}
		else
		{
		    pid->integral += pid->igain * ferror;		//���������
			if(pid->integral > 10)
			{
			    pid->integral = 10;
			}
			else if(pid->integral < -10)
			{
			    pid->integral = -10;
			}
		}
		//΢����
		dterm = ((err - pid->last_error)) * pid->dgain;			//΢����
		//PID����
		result = pterm + pid->integral + dterm;	   				//PIDֵ
		//PD����
		//result = pterm + dterm;	   							//PDֵ
		//P����
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
		result = pid->integral;		//��������Χ�ڣ������������
	}
	//�����ϴ�ƫ��
	pid->last_error = err;
	//���PIDֵ
	return (result);
}

void wk_off_ctr(void)
{
    if(WK_DELAY.flag == 1)
	{
		WK_DELAY.flag = 0;
        wk_enable = 0;
        WK_HEAT_OFF();										//�ؼ���
        wk_heating_flag = 0;								//���ȱ�־λ���㣨�������˲�����ָʾ��	
        WK_COOL_OFF();										//������
        wk_cooling_flag = 0;								//�����־λ����
        wk_cool_fan_delay = 0;
        WK_FAN_OFF();
    }
}


void wk_heat_ctr(void)
{
	static u8 pwm_ctr_period = 0;							//ÿ��ִ�к���ʱ��1��������10ʱ�����¼���
	float pid_ctr_result;
	if(WK_DELAY.flag == 1)
	{
		WK_DELAY.flag = 0;
		if(wk_enable != 0)								//ʹ�����¶ȿ���
		{
			if(wk_temperature_err != 0)					//�¿ش���������ʱֹͣ����
			{
				WK_HEAT_OFF();								//�ؼ���
				wk_heating_flag = 0;						//���ȱ�־λ���㣨�������˲�����ָʾ��
				WK_COOL_OFF();
                WK_FAN_OFF();
				wk_cooling_flag = 0;						//�����־λ����
				return;
			}
			pwm_ctr_period++;							//10s��Ϊһ������
			if(pwm_ctr_period > DEF_PWM_CTR_PERIOD)
			{
				pwm_ctr_period = 0;
			}
			wk_pid_para.sp = wk_temperature_set;
			wk_pid_para.pv = wk_temperature;
            pid_ctr_result = wk_pid_calc(&wk_pid_para);
            if(pid_ctr_result < 0)				  //����Ҫ����
            {

            }
            else
            {
                if(pid_ctr_result > DEF_PWM_CTR_PERIOD)
                {
                    pid_ctr_result = DEF_PWM_CTR_PERIOD;
                }
            }
            if(pid_ctr_result == DEF_PWM_CTR_PERIOD)			//���100%����
            {
                WK_HEAT_ON();										//������
                wk_heating_flag = 1;								//���ȱ�־λ��1���������˼���ָʾ��
            }
            else
            {
                if(pid_ctr_result > pwm_ctr_period)
                {
                    WK_HEAT_ON();										//������
                    wk_heating_flag = 1;								//���ȱ�־λ��1���������˼���ָʾ��
                }
                else
                {
                    WK_HEAT_OFF();										//�ؼ���
                    wk_heating_flag = 0;								//���ȱ�־λ���㣨�������˲�����ָʾ��
                }
            }
		}
		else					//��ʹ���¶ȿ���
		{
			WK_HEAT_OFF();										//�ؼ���
			wk_heating_flag = 0;								//���ȱ�־λ���㣨�������˲�����ָʾ��	
		}	
        WK_COOL_OFF();										//������
        wk_cooling_flag = 0;								//�����־λ����
        wk_cool_fan_delay = 0;
        WK_FAN_OFF();
	}
}

void wk_heat_and_cool_ctr(void)
{
	static u8 pwm_ctr_period = 0;							//ÿ��ִ�к���ʱ��1��������10ʱ�����¼���
	float pid_ctr_result;
	if(WK_DELAY.flag == 1)
	{
		WK_DELAY.flag = 0;
		wk_cool_fan_delay ++;
		if(wk_cool_fan_delay >= 60)
		{
			WK_FAN_OFF();
		}
		if(wk_enable != 0)								//ʹ�����¶ȿ���
		{
			if(wk_temperature_err != 0)					//�¿ش���������ʱֹͣ����
			{
				WK_HEAT_OFF();								//�ؼ���
				wk_heating_flag = 0;						//���ȱ�־λ���㣨�������˲�����ָʾ��
				WK_COOL_OFF();
				wk_cooling_flag = 0;						//�����־λ����
				return;
			}
			pwm_ctr_period++;							//10s��Ϊһ������
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
					WK_HEAT_OFF();										//�ؼ���
					wk_heating_flag = 0;								//���ȱ�־λ����	
					WK_COOL_OFF();										//������
					wk_cooling_flag = 0;								//�����־λ����
				}
				else
				{
				
				}
			}
			else if(wk_temperature < (wk_temperature_set - wk_pid_para.deadband - wk_start_heat_err))	//��������20�ȣ�����1�ȣ����¶Ƚ��͵�20-1-wk_start_heat_errʱ����������
			{
				if(wk_heat_or_cool_flag != DF_HEAT)
				{
					wk_heat_or_cool_flag = DF_HEAT;
					wk_pid_para.integral = 0;
					WK_HEAT_OFF();										//�ؼ���
					wk_heating_flag = 0;								//���ȱ�־λ����	
					WK_COOL_OFF();										//������
					wk_cooling_flag = 0;								//�����־λ����
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
				if(pid_ctr_result < 0)				  //����Ҫ����
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
				if(pid_ctr_result == DEF_PWM_CTR_PERIOD)			//���100%����
				{
					WK_HEAT_ON();										//������
					wk_heating_flag = 1;								//���ȱ�־λ��1���������˼���ָʾ��
				}
				else
				{
					if(pid_ctr_result > pwm_ctr_period)
					{
						WK_HEAT_ON();										//������
						wk_heating_flag = 1;								//���ȱ�־λ��1���������˼���ָʾ��
					}
					else
					{
						WK_HEAT_OFF();										//�ؼ���
						wk_heating_flag = 0;								//���ȱ�־λ���㣨�������˲�����ָʾ��
					}
				}
			}
			else
			{
				pid_ctr_result = wk_pid_calc_cool(&wk_pid_para);
				if(pid_ctr_result > 0)				  //����Ҫ����
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
				if(pid_ctr_result == -DEF_PWM_CTR_PERIOD)			//���100%����
				{
					WK_COOL_ON();										//������
					wk_cooling_flag = 1;								//�����־λ��1
					WK_FAN_ON();										//������
					wk_cool_fan_delay = 0;								//���¼�ʱ
				}
				else
				{
					if(pid_ctr_result < -pwm_ctr_period)
					{
						WK_COOL_ON();										//������
						wk_cooling_flag = 1;								//�����־λ��1
						WK_FAN_ON();										//������
						wk_cool_fan_delay = 0;								//���¼�ʱ
					}
					else
					{
						WK_COOL_OFF();										//������
						wk_cooling_flag = 0;								//�����־λ����
					}
				}			
			}
		}
		else					//��ʹ���¶ȿ���
		{
			WK_HEAT_OFF();										//�ؼ���
			wk_heating_flag = 0;								//���ȱ�־λ���㣨�������˲�����ָʾ��	
			WK_COOL_OFF();										//������
			wk_cooling_flag = 0;								//�����־λ����
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

