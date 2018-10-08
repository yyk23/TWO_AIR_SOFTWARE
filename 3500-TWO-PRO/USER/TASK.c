#include "TASK.h"
#include "DMT.h"
#include "adc.h"
#include "sample.h"
#include "pump.h"
#include "parameter.h"
#include "DMT_ADDRESS.h"
#include "24c64.h"
#include "process.h"
#include "usb.h"
#include "print.h"
#include "delay.h"
#include "w25qxx.h"
#include "file.h"
#include "saveresume.h"
#include "18b20.h"
#include "rtc.h"
#include "timer.h"
#include "led.h"
#include "bat.h"
#include "malloc.h" 
#include "ff.h"  
#include "usbh_usr.h" 
#include "exfuns.h" 
#include "usart.h"
#include "ZR3500_CONFIG.h"
#include "SSDL.h"
#include <string.h>
struct OWNTIMER REFRESH_DELAY,REFRESH_SHARING_DELAY,LED_DELAY;
struct OWNTIMER ADC_1000ms;
struct OWNTIMER DS18B20_1000ms;
struct OWNTIMER PUMP_A_1000ms,PUMP_B_1000ms;
struct OWNTIMER SAMPLE_A_1000ms,SAMPLE_B_1000ms;
struct OWNTIMER USB_1000ms,PRINT_DELAY;
struct OWNTIMER EMPLOY_DELAY;
struct OWNTIMER DS1302_DELAY;
struct OWNTIMER BACKLIGHT_DELAY;
struct OWNTIMER BLUETOOTH_DELAY,BLUETOOTH_50ms_DELAY;
struct OWNTIMER WK_DELAY;
struct OWNTIMER SSDL_CONVERT_DELAY,SSDL_1000ms;
struct OWNTIMER DMT_TEST_DELAY;
u16 timer_count_tmp;
u16 last_timer_count;
u16 delta_timer_count;
struct WND_STRUCT wnd_struct;
struct PUMP_CTR_TASK PUMP_CTR_TASK_A,PUMP_CTR_TASK_B;
struct SAMPLE_TASK SAMPLE_TASK_A,SAMPLE_TASK_B;
struct USB_TASK USB_OUT_SAMPLE,USB_OUT_POWER;
struct PRINT_TASK PRINT_SAMPLE,PRINT_POWER;
struct BLUETOOTH_TASK BLUETOOTH;
struct FLOW_DIS A_flow_dis,B_flow_dis;
struct LED_TASK_STRUCT LED_TASK;
struct STRUCT_SAMPLE_MODE SAMPLE_MODE;
u16 key = 0;
u16 iii;
u16 AB_sample_file_num_tmp;
u16 AB_power_lose_file_num_tmp;
u16 usb_out_start_file;
u16 usb_out_end_file;
u16 print_start_file;
u16 print_end_file;
u16 sample_file_start_num;
u16 sample_file_end_num;
u16 power_file_start_num;
u16 power_file_end_num;
u16 last_wnd;
u8 last_wnd_locked = 0;
u16 bluetooth_last_wnd;
u8 supper_password = 0;
u8 auto_exit_flag = 0;
u16 back_light_min = 0;
u16 display_current_pic = 0;
u16 display_last_pic = 0;
u16 back_light_min_set = 20;
float kedu_temperature_set = 20.0;
u16 select_location[]={56,10,96,45};
extern u16 main_i;
void set_refresh_wnd_num(u8 new)		//显示当前页面
{
	if(new != wnd_struct.current)		//
	{
		wnd_struct.last = wnd_struct.current;
	}
	wnd_struct.current = new;
	wnd_struct.change_pic = 1;
}
void set_refresh_wnd_num_tmp(u8 new)		//显示当前页面
{
	wnd_struct.current = new;
	wnd_struct.change_pic = 1;
}
void set_refresh_last_wnd_num(void)		//返回到上个页面
{
	u8 temp;
	temp = wnd_struct.current;
	wnd_struct.current = wnd_struct.last;
	wnd_struct.last = temp;
	wnd_struct.change_pic = 1;
}

void timer_init(struct OWNTIMER * s,u16 set_value)
{
	(*s).flag = 0;
	(*s).reload = set_value;
	(*s).set = (*s).reload;
}

void timer_task_init(void)
{
	last_timer_count = timer1_count;

	timer_init(&REFRESH_DELAY,20);
	timer_init(&REFRESH_SHARING_DELAY,20);
	timer_init(&ADC_1000ms,20);
	timer_init(&DS18B20_1000ms,20);
	timer_init(&PUMP_A_1000ms,20);
	timer_init(&PUMP_B_1000ms,20);
	timer_init(&SAMPLE_A_1000ms,20);
	timer_init(&SAMPLE_B_1000ms,20);
	timer_init(&USB_1000ms,20);
	timer_init(&PRINT_DELAY,20);
	timer_init(&EMPLOY_DELAY,20);
	timer_init(&DS1302_DELAY,10);	//0.5s定时
	timer_init(&BACKLIGHT_DELAY,1200);
	timer_init(&BLUETOOTH_DELAY,20);
	timer_init(&BLUETOOTH_50ms_DELAY,1);
	timer_init(&WK_DELAY,20);
	timer_init(&LED_DELAY,10);
    timer_init(&SSDL_CONVERT_DELAY,1);
	timer_init(&SSDL_1000ms,20);
    timer_init(&DMT_TEST_DELAY,20);
}
void creat_timer_flag(struct OWNTIMER * s,u16 delta)
{
	u16 temp;
	if((*s).reload != 0)
	{
		if((*s).set > delta)
		{
			(*s).set -= delta;
		}
		else
		{
			temp = (*s).reload + (*s).set;
			if(temp >= delta)		//刚好或错过delta-s.set个1ms
			{
				(*s).set = temp - delta;
			}
			else				//已经错过设定定时时间，之间重装初值
			{	
				(*s).set = (*s).reload;
			}
			(*s).flag = 1;
		}
	}
	else
	{
		(*s).flag = 1;
	}
}
void timer_task(void)
{
	timer_count_tmp = timer1_count;
	if(timer_count_tmp >= last_timer_count)
	{
		delta_timer_count = timer_count_tmp - last_timer_count;
	}
	else
	{
		delta_timer_count = 65536 + timer_count_tmp - last_timer_count;
	}
	last_timer_count = timer_count_tmp;

	creat_timer_flag(&REFRESH_DELAY,delta_timer_count);
	creat_timer_flag(&REFRESH_SHARING_DELAY,delta_timer_count);
	creat_timer_flag(&ADC_1000ms,delta_timer_count);
	creat_timer_flag(&DS18B20_1000ms,delta_timer_count);
	creat_timer_flag(&PUMP_A_1000ms,delta_timer_count);
	creat_timer_flag(&PUMP_B_1000ms,delta_timer_count);
	creat_timer_flag(&SAMPLE_A_1000ms,delta_timer_count);
	creat_timer_flag(&SAMPLE_B_1000ms,delta_timer_count);
	creat_timer_flag(&USB_1000ms,delta_timer_count);
	creat_timer_flag(&PRINT_DELAY,delta_timer_count);
	creat_timer_flag(&EMPLOY_DELAY,delta_timer_count);
	creat_timer_flag(&DS1302_DELAY,delta_timer_count);
	creat_timer_flag(&BACKLIGHT_DELAY,delta_timer_count);
	creat_timer_flag(&BLUETOOTH_DELAY,delta_timer_count);
	creat_timer_flag(&BLUETOOTH_50ms_DELAY,delta_timer_count);
	creat_timer_flag(&WK_DELAY,delta_timer_count);
	creat_timer_flag(&LED_DELAY,delta_timer_count);
    creat_timer_flag(&SSDL_CONVERT_DELAY,delta_timer_count);
	creat_timer_flag(&SSDL_1000ms,delta_timer_count);
    creat_timer_flag(&DMT_TEST_DELAY,delta_timer_count);
}
u8 DMT_error = 0;
void uart_task(void)
{
	u8 locale_flag = 0;
	u16 i;	
	u16 ibuf;
	u8 A_flow_rate_count = 0;
	u8 B_flow_rate_count = 0;
	u32 lbuf;
	locale_flag = DMT_GetMessage(FRAME_BUF);
	if(locale_flag == DEF_READ_VARIALBE_RESPONSE)
	{
		display_sleep_init();
		i = DMT_GetVariableAddr(FRAME_BUF);
		if(i == DF_KEY_ADDRESS)								//收到了按键
		{
			i = DMT_GetVariableIntValue(FRAME_BUF);//获取按键值
			key = i;
			return;
		}
		else
		{
			ibuf = DMT_GetVariableIntValue(FRAME_BUF);
			switch(i)
			{
				case DF_INPUT_ENVIRONMENT_TEMPERATURE_ADDR:
					sample_temperature[DF_ENVIRONMENT_TEMPERATURE].input = (s16)ibuf / 10.0;
					savefloat(ADDR_environment_temperature_input,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].input);
					sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode = DF_INPUT_MODE;
					W_24(ADDR_environment_temperature_mode,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode);//温度选择模式
					set_refresh_wnd_num(DF_PIC_SET);
					break;
				case DF_INPUT_BAR_ADDR:
					barometric.input = ibuf / 100.0;
					savefloat(ADDR_barometric_input,barometric.input);
					barometric.mode = DF_INPUT_MODE;
					W_24(ADDR_barometric_mode,barometric.mode);
					set_refresh_wnd_num(DF_PIC_SET);
					break;
				case DF_INPUT_TEMPERATURE_CTR_1_SET_ADDR:
					wk_temperature_set = ibuf;
					if(wk_temperature_set > 30)
					{
						wk_temperature_set = 30;
					}
					if(wk_temperature_set < 15)
					{
						wk_temperature_set = 15;
					}
					saveintdata(ADDR_wk_temperature_set,wk_temperature_set);
					set_refresh_wnd_num(DF_PIC_SET);
					break;
				case DF_BACK_LIGHT_SET_IN:
					back_light_min_set = ibuf;
					saveintdata(ADDR_back_light_min_set,back_light_min_set);
					display_sleep_init();
					set_refresh_wnd_num(DF_PIC_SET);
					break;
				case DF_INPUT_SYS_YEAR_ADDR:
					sys_year = ibuf;
					if(RTC_select == DF_RTC_MAIN_BOARD)
					{
						Write_Date(sys_year,sys_month,sys_day);
					}
					else
					{
						DMT_year = toBCD(sys_year);
						DMT_month = toBCD(DMT_month);
						DMT_day = toBCD(DMT_day);
						DMT_hour = toBCD(DMT_hour);
						DMT_min = toBCD(DMT_min);
						DMT_sec = toBCD(DMT_sec);
						DMT_WriteRTC();
					}
					set_refresh_wnd_num(DF_PIC_SET);
					break;
				case DF_INPUT_SYS_MONTH_ADDR:
					if(ibuf > 12)ibuf = 12;
					sys_month = ibuf;
					if(RTC_select == DF_RTC_MAIN_BOARD)
					{
						Write_Date(sys_year,sys_month,sys_day);
					}
					else
					{
						DMT_year = toBCD(DMT_year);
						DMT_month = toBCD(sys_month);
						DMT_day = toBCD(DMT_day);
						DMT_hour = toBCD(DMT_hour);
						DMT_min = toBCD(DMT_min);
						DMT_sec = toBCD(DMT_sec);
						DMT_WriteRTC();
					}
					set_refresh_wnd_num(DF_PIC_SET);
					break;
				case DF_INPUT_SYS_DAY_ADDR:
					if(ibuf > 31)ibuf = 31;
					sys_day = ibuf;
					if(RTC_select == DF_RTC_MAIN_BOARD)
					{
						Write_Date(sys_year,sys_month,sys_day);
					}
					else
					{
						DMT_year = toBCD(DMT_year);
						DMT_month = toBCD(DMT_month);
						DMT_day = toBCD(sys_day);
						DMT_hour = toBCD(DMT_hour);
						DMT_min = toBCD(DMT_min);
						DMT_sec = toBCD(DMT_sec);
						DMT_WriteRTC();
					}
					set_refresh_wnd_num(DF_PIC_SET);
					break;
				case DF_INPUT_SYS_HOUR_ADDR:
					if(ibuf > 24)ibuf = 24;
					sys_hour = ibuf;
					if(RTC_select == DF_RTC_MAIN_BOARD)
					{
						Write_time(sys_hour,sys_min,sys_sec);  //写入时间函数
					}
					else
					{
						DMT_year = toBCD(DMT_year);
						DMT_month = toBCD(DMT_month);
						DMT_day = toBCD(DMT_day);
						DMT_hour = toBCD(sys_hour);
						DMT_min = toBCD(DMT_min);
						DMT_sec = toBCD(DMT_sec);
						DMT_WriteRTC();
					}
					set_refresh_wnd_num(DF_PIC_SET);
					break;
				case DF_INPUT_SYS_MIN_ADDR:
					if(ibuf > 59)ibuf = 59;
					sys_min = ibuf;
					if(RTC_select == DF_RTC_MAIN_BOARD)
					{
						Write_time(sys_hour,sys_min,sys_sec);  //写入时间函数
					}
					else
					{
						DMT_year = toBCD(DMT_year);
						DMT_month = toBCD(DMT_month);
						DMT_day = toBCD(DMT_day);
						DMT_hour = toBCD(DMT_hour);
						DMT_min = toBCD(sys_min);
						DMT_sec = toBCD(DMT_sec);
						DMT_WriteRTC();
					}
					set_refresh_wnd_num(DF_PIC_SET);
					break;
				case DF_INPUT_SYS_SEC_ADDR:
					if(ibuf > 59)ibuf = 59;
					sys_sec = ibuf;
					if(RTC_select == DF_RTC_MAIN_BOARD)
					{
						Write_time(sys_hour,sys_min,sys_sec);  //写入时间函数
					}
					else
					{
						DMT_year = toBCD(DMT_year);
						DMT_month = toBCD(DMT_month);
						DMT_day = toBCD(DMT_day);
						DMT_hour = toBCD(DMT_hour);
						DMT_min = toBCD(DMT_min);
						DMT_sec = toBCD(sys_sec);
						DMT_WriteRTC();
					}
					set_refresh_wnd_num(DF_PIC_SET);
					break;

				case DF_A_DY_RATE_IN_ADDR:
					A_SAMPLE.dy_rate = (float)ibuf / (float)10000.0;
					savefloat(ADDR_A_SAMPLE_dy_rate,A_SAMPLE.dy_rate);
					set_refresh_wnd_num(DF_PIC_PRESSURE_BIAODING);
					break;
				case DF_A_DY_ZERO_IN_ADDR:
					A_SAMPLE.dy_zero = (float)ibuf;
					savefloat(ADDR_A_SAMPLE_dy_zero,A_SAMPLE.dy_zero);
					set_refresh_wnd_num(DF_PIC_PRESSURE_BIAODING);
					break;					
				case DF_A_JY_RATE_IN_ADDR:
					A_SAMPLE.jy_rate = (float)ibuf / (float)10000.0;
					savefloat(ADDR_A_SAMPLE_jy_rate,A_SAMPLE.jy_rate);
					set_refresh_wnd_num(DF_PIC_PRESSURE_BIAODING);
					break;
				case DF_A_JY_ZERO_IN_ADDR:
					A_SAMPLE.jy_zero = (float)ibuf / (float)100.0;
					savefloat(ADDR_A_SAMPLE_jy_zero,A_SAMPLE.jy_zero);
					set_refresh_wnd_num(DF_PIC_PRESSURE_BIAODING);
					break;				
				case DF_B_DY_RATE_IN_ADDR:
					B_SAMPLE.dy_rate = (float)ibuf / (float)10000.0;
					savefloat(ADDR_B_SAMPLE_dy_rate,B_SAMPLE.dy_rate);
					set_refresh_wnd_num(DF_PIC_PRESSURE_BIAODING);
					break;
				case DF_B_DY_ZERO_IN_ADDR:
					B_SAMPLE.dy_zero = (float)ibuf;
					savefloat(ADDR_B_SAMPLE_dy_zero,B_SAMPLE.dy_zero);
					set_refresh_wnd_num(DF_PIC_PRESSURE_BIAODING);
					break;					
				case DF_B_JY_RATE_IN_ADDR:
					B_SAMPLE.jy_rate = (float)ibuf / (float)10000.0;
					savefloat(ADDR_B_SAMPLE_jy_rate,B_SAMPLE.jy_rate);
					set_refresh_wnd_num(DF_PIC_PRESSURE_BIAODING);
					break;
				case DF_B_JY_ZERO_IN_ADDR:
					B_SAMPLE.jy_zero = (float)ibuf / (float)100.0;
					savefloat(ADDR_B_SAMPLE_jy_zero,B_SAMPLE.jy_zero);
					set_refresh_wnd_num(DF_PIC_PRESSURE_BIAODING);
					break;						
				case DF_BAR_RATE_IN_ADDR:
					barometric.rate = (float)ibuf / (float)10000.0;
					savefloat(ADDR_barometric_rate,barometric.rate);
					set_refresh_wnd_num(DF_PIC_PRESSURE_BIAODING);
					break;
				case DF_BAR_ZERO_IN_ADDR:
					barometric.zero = (float)ibuf / (float)100.0;
					savefloat(ADDR_barometric_zero,barometric.zero);
					set_refresh_wnd_num(DF_PIC_PRESSURE_BIAODING);
					break;
				case DF_TEMPERATURE_ZERO_IN_ADDR:
					sample_temperature[DF_ENVIRONMENT_TEMPERATURE].zero = (float)ibuf / (float)10.0;
					if(sample_temperature[DF_ENVIRONMENT_TEMPERATURE].zero > 120)
					{
						sample_temperature[DF_ENVIRONMENT_TEMPERATURE].zero = 120;
					}
					else if(sample_temperature[DF_ENVIRONMENT_TEMPERATURE].zero < 100)
					{
						sample_temperature[DF_ENVIRONMENT_TEMPERATURE].zero = 100;
					}
					savefloat(ADDR_sample_temperature_ENVIRONMENT_zero,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].zero);
					set_refresh_wnd_num(DF_PIC_TEMPERATURE_BIAODING);
					break;
				case DF_A_TEMPERATURE_ZERO_IN_ADDR:
					sample_temperature[DF_A_TEMPERATURE].zero = (float)ibuf / (float)10.0;
					if(sample_temperature[DF_A_TEMPERATURE].zero > 120)
					{
						sample_temperature[DF_A_TEMPERATURE].zero = 120;
					}
					else if(sample_temperature[DF_A_TEMPERATURE].zero < 100)
					{
						sample_temperature[DF_A_TEMPERATURE].zero = 100;
					}
					savefloat(ADDR_sample_temperature_A_zero,sample_temperature[DF_A_TEMPERATURE].zero);
					set_refresh_wnd_num(DF_PIC_TEMPERATURE_BIAODING);
					break;
				case DF_B_TEMPERATURE_ZERO_IN_ADDR:
					sample_temperature[DF_B_TEMPERATURE].zero = (float)ibuf / (float)10.0;
					if(sample_temperature[DF_B_TEMPERATURE].zero > 120)
					{
						sample_temperature[DF_B_TEMPERATURE].zero = 120;
					}
					else if(sample_temperature[DF_B_TEMPERATURE].zero < 100)
					{
						sample_temperature[DF_B_TEMPERATURE].zero = 100;
					}
					savefloat(ADDR_sample_temperature_B_zero,sample_temperature[DF_B_TEMPERATURE].zero);
					set_refresh_wnd_num(DF_PIC_TEMPERATURE_BIAODING);
					break;
				case DF_CTR_TEMPERATURE_ZERO_IN_ADDR:
					sample_temperature[DF_CTR_TEMPERATURE].zero = (float)ibuf / (float)10.0;
					if(sample_temperature[DF_CTR_TEMPERATURE].zero > 120)
					{
						sample_temperature[DF_CTR_TEMPERATURE].zero = 120;
					}
					else if(sample_temperature[DF_CTR_TEMPERATURE].zero < 100)
					{
						sample_temperature[DF_CTR_TEMPERATURE].zero = 100;
					}
					savefloat(ADDR_sample_temperature_CTR_zero,sample_temperature[DF_CTR_TEMPERATURE].zero);
					set_refresh_wnd_num(DF_PIC_TEMPERATURE_BIAODING);
					break;
				case DF_BIAODING_A_FLOW_SET_IN_ADDR:
					A_SAMPLE.flow_set = (float)ibuf / (float)10.0;
					if(A_SAMPLE.flow_set < (float)DF_A_FLOW_SET_MIN)
					{
						A_SAMPLE.flow_set = (float)DF_A_FLOW_SET_MIN;
					}
					else if(A_SAMPLE.flow_set > (float)DF_A_FLOW_SET_MAX)
					{
						A_SAMPLE.flow_set = (float)DF_A_FLOW_SET_MAX;
					}
					set_refresh_wnd_num(DF_PIC_FLOW_BIAODING);
					break;
				case DF_BIAODING_B_FLOW_SET_IN_ADDR:
					B_SAMPLE.flow_set = (float)ibuf / (float)10.0;
					if(B_SAMPLE.flow_set < (float)DF_B_FLOW_SET_MIN)
					{
						B_SAMPLE.flow_set = (float)DF_B_FLOW_SET_MIN;
					}
					else if(B_SAMPLE.flow_set > (float)DF_B_FLOW_SET_MAX)
					{
						B_SAMPLE.flow_set = (float)DF_B_FLOW_SET_MAX;
					}
					set_refresh_wnd_num(DF_PIC_FLOW_BIAODING);
					break;
				case DF_BIAODING_A_FLOW_RATE_IN_ADDR:
					A_flow_rate_count = A_SAMPLE.flow_set * (float)10.0 + (float)0.5;
					A_flow_rate.rate_array[A_flow_rate_count] = (float)ibuf / (float)1000.0;
					savefloat(ADDR_A_flow_rate_rate_array + 4 * A_flow_rate_count,A_flow_rate.rate_array[A_flow_rate_count]);
					set_refresh_wnd_num(DF_PIC_FLOW_BIAODING);
					break;
				case DF_BIAODING_B_FLOW_RATE_IN_ADDR:
					B_flow_rate_count = B_SAMPLE.flow_set * (float)10.0 + (float)0.5;
					B_flow_rate.rate_array[B_flow_rate_count] = (float)ibuf / (float)1000.0;
					savefloat(ADDR_B_flow_rate_rate_array + 4 * B_flow_rate_count,B_flow_rate.rate_array[B_flow_rate_count]);
					set_refresh_wnd_num(DF_PIC_FLOW_BIAODING);
					break;
				case DF_DIS_PASSWORD_IN_ADDR:
					password = ibuf;
					saveintdata(ADDR_password,password);
					set_refresh_wnd_num(DF_PIC_PASSWORD_SET);
					break;
				case DF_KEDU_FLOW_TEMPERATURE_SET_IN:
					kedu_temperature_set = (float)ibuf;
					savefloat(ADDR_kedu_temperature_set,kedu_temperature_set);
					set_refresh_wnd_num(DF_PIC_FLOW_BIAODING);
					break;
				case DF_A_FLOW_SET_IN_ADDR:
					A_SAMPLE.flow_set = (float)ibuf / (float)10.0;
					if(A_SAMPLE.flow_set < (float)DF_A_FLOW_SET_MIN)
					{
						A_SAMPLE.flow_set = (float)DF_A_FLOW_SET_MIN;
					}
					else if(A_SAMPLE.flow_set > (float)DF_A_FLOW_SET_MAX)
					{
						A_SAMPLE.flow_set = (float)DF_A_FLOW_SET_MAX;
					}
					savefloat(ADDR_A_SAMPLE_flow_set,A_SAMPLE.flow_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_SET);
					break;
				case DF_A_VOLUME_SET_IN_ADDR:
					A_SAMPLE.sample_volume_set = (float)ibuf / (float)10.0;
					if(A_SAMPLE.sample_volume_set < (float)0.1)
					{
						A_SAMPLE.sample_volume_set = (float)0.1;
					}
					else if(A_SAMPLE.sample_volume_set > (float)6000.0)
					{
						A_SAMPLE.sample_volume_set = (float)6000.0;
					}
					save_sample_para();
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_SET);
					break;
				case DF_A_HOUR_SET_IN_ADDR:
					A_SAMPLE.running_time_set = (u32)ibuf * 3600 + (A_SAMPLE.running_time_set % 3600);
					if(A_SAMPLE.running_time_set < 60)
					{
						A_SAMPLE.running_time_set = 60;
					}
					savelongdata(ADDR_A_SAMPLE_running_time_set,A_SAMPLE.running_time_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_SET);
					break;	
				case DF_A_MIN_SET_IN_ADDR:
					if(ibuf > 59)
					{
						ibuf = 59;
					}
					A_SAMPLE.running_time_set = (A_SAMPLE.running_time_set / 3600) * 3600 + (u32)ibuf * 60;
					if(A_SAMPLE.running_time_set < 60)
					{
						A_SAMPLE.running_time_set = 60;
					}
					savelongdata(ADDR_A_SAMPLE_running_time_set,A_SAMPLE.running_time_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_SET);
					break;
				case DF_A_START_HOUR_IN_ADDR:
					A_SAMPLE.start_hour_set = ibuf;
					if(A_SAMPLE.start_hour_set > 23)
					{
						A_SAMPLE.start_hour_set = 23;
					}
					W_24(ADDR_A_SAMPLE_start_hour_set,A_SAMPLE.start_hour_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_SET);
					break;	
				case DF_A_START_MIN_IN_ADDR:
					A_SAMPLE.start_min_set = ibuf;
					if(A_SAMPLE.start_min_set > 59)
					{
						A_SAMPLE.start_min_set = 59;
					}
					W_24(ADDR_A_SAMPLE_start_min_set,A_SAMPLE.start_min_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_SET);
					break;	
				case DF_A_INTERVAL_HOUR_IN_ADDR:
					A_SAMPLE.jiange_time_set = (u32)ibuf * 3600 + (A_SAMPLE.jiange_time_set % 3600);
					savelongdata(ADDR_A_SAMPLE_jiange_time_set,A_SAMPLE.jiange_time_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_SET);
					break;	
				case DF_A_INTERVAL_MIN_IN_ADDR:
					if(ibuf > 59)
					{
						ibuf = 59;
					}
					A_SAMPLE.jiange_time_set = (A_SAMPLE.jiange_time_set / 3600) * 3600 + (u32)ibuf * 60;
					savelongdata(ADDR_A_SAMPLE_jiange_time_set,A_SAMPLE.jiange_time_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_SET);
					break;	
				case DF_A_COUNT_IN_ADDR:
					A_SAMPLE.sample_times_set = ibuf;
					if(A_SAMPLE.sample_times_set < 1)
					{
						A_SAMPLE.sample_times_set = 1;
					}
					W_24(ADDR_A_SAMPLE_sample_times_set,A_SAMPLE.sample_times_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_SET);
					break;
				case DF_B_VOLUME_SET_IN_ADDR:
					B_SAMPLE.sample_volume_set = (float)ibuf / (float)10.0;
					if(B_SAMPLE.sample_volume_set < (float)0.1)
					{
						B_SAMPLE.sample_volume_set = (float)0.1;
					}
					else if(B_SAMPLE.sample_volume_set > (float)6000.0)
					{
						B_SAMPLE.sample_volume_set = (float)6000.0;
					}
					save_sample_para();
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_SET);
					break;
				case DF_B_FLOW_SET_IN_ADDR:
					B_SAMPLE.flow_set = (float)ibuf / (float)10.0;
					if(B_SAMPLE.flow_set < (float)DF_B_FLOW_SET_MIN)
					{
						B_SAMPLE.flow_set = (float)DF_B_FLOW_SET_MIN;
					}
					else if(B_SAMPLE.flow_set > (float)DF_B_FLOW_SET_MAX)
					{
						B_SAMPLE.flow_set = (float)DF_B_FLOW_SET_MAX;
					}
					savefloat(ADDR_B_SAMPLE_flow_set,B_SAMPLE.flow_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_SET);
					break;	
				case DF_B_HOUR_SET_IN_ADDR:
					B_SAMPLE.running_time_set = (u32)ibuf * 3600 + (B_SAMPLE.running_time_set % 3600);
					if(B_SAMPLE.running_time_set < 60)
					{
						B_SAMPLE.running_time_set = 60;
					}
					savelongdata(ADDR_B_SAMPLE_running_time_set,B_SAMPLE.running_time_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_SET);
					break;	
				case DF_B_MIN_SET_IN_ADDR:
					if(ibuf > 59)
					{
						ibuf = 59;
					}
					B_SAMPLE.running_time_set = (B_SAMPLE.running_time_set / 3600) * 3600 + (u32)ibuf * 60;
					if(B_SAMPLE.running_time_set < 60)
					{
						B_SAMPLE.running_time_set = 60;
					}
					savelongdata(ADDR_B_SAMPLE_running_time_set,B_SAMPLE.running_time_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_SET);
					break;
				case DF_B_START_HOUR_IN_ADDR:
					B_SAMPLE.start_hour_set = ibuf;
					if(B_SAMPLE.start_hour_set > 23)
					{
						B_SAMPLE.start_hour_set = 23;
					}
					W_24(ADDR_B_SAMPLE_start_hour_set,B_SAMPLE.start_hour_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_SET);
					break;	
				case DF_B_START_MIN_IN_ADDR:
					B_SAMPLE.start_min_set = ibuf;
					if(B_SAMPLE.start_min_set > 59)
					{
						B_SAMPLE.start_min_set = 59;
					}
					W_24(ADDR_B_SAMPLE_start_min_set,B_SAMPLE.start_min_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_SET);
					break;	
				case DF_B_INTERVAL_HOUR_IN_ADDR:
					B_SAMPLE.jiange_time_set = (u32)ibuf * 3600 + (B_SAMPLE.jiange_time_set % 3600);
					savelongdata(ADDR_B_SAMPLE_jiange_time_set,B_SAMPLE.jiange_time_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_SET);
					break;	
				case DF_B_INTERVAL_MIN_IN_ADDR:
					if(ibuf > 59)
					{
						ibuf = 59;
					}
					B_SAMPLE.jiange_time_set = (B_SAMPLE.jiange_time_set / 3600) * 3600 + (u32)ibuf * 60;
					savelongdata(ADDR_B_SAMPLE_jiange_time_set,B_SAMPLE.jiange_time_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_SET);
					break;	
				case DF_B_COUNT_IN_ADDR:
					B_SAMPLE.sample_times_set = ibuf;
					if(B_SAMPLE.sample_times_set < 1)
					{
						B_SAMPLE.sample_times_set = 1;
					}
					W_24(ADDR_B_SAMPLE_sample_times_set,B_SAMPLE.sample_times_set);
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_SET);
					break;
				case DF_SAMPLE_FILE_IN_ADDR:
					i = AB_sample_file_num_tmp;
					AB_sample_file_num_tmp = ibuf;
					if(AB_sample_file_num_tmp == 0)
					{
						AB_sample_file_num_tmp = i;
					}
					else if(AB_sample_file_num_tmp > DEF_SAMPLE_FILE_MAX_W25)
					{
						AB_sample_file_num_tmp = i;	
					}
					if((AB_sample_file.fg_flag == 0) && (AB_sample_file_num_tmp > AB_sample_file.num))
					{
						AB_sample_file_num_tmp = AB_sample_file.num;
						set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_NO_FILE_USB_PRINT);
					}
					else
					{
						set_refresh_wnd_num(DF_PIC_SAMPLE_DATA);
					}
					break;
				case DF_SAMPLE_FILE_IN_2_ADDR:
					i = AB_sample_file_num_tmp;
					AB_sample_file_num_tmp = ibuf;
					if(AB_sample_file_num_tmp == 0)
					{
						AB_sample_file_num_tmp = i;
					}
					else if(AB_sample_file_num_tmp > DEF_SAMPLE_FILE_MAX_W25)
					{
						AB_sample_file_num_tmp = i;	
					}
					if((AB_sample_file.fg_flag == 0) && (AB_sample_file_num_tmp > AB_sample_file.num))
					{
						AB_sample_file_num_tmp = AB_sample_file.num;
						set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_NO_FILE_USB_PRINT);
					}
					else
					{
						set_refresh_wnd_num(DF_PIC_QUERY);
					}
					break;
				case DF_POWER_FILE_IN_ADDR:
					i = AB_power_lose_file_num_tmp;
					AB_power_lose_file_num_tmp = ibuf;
					if(AB_power_lose_file_num_tmp == 0)
					{
						AB_power_lose_file_num_tmp = i;
					}
					else if(AB_power_lose_file_num_tmp > DEF_POWER_FILE_MAX_W25)
					{
						AB_power_lose_file_num_tmp = i;	
					}
					if((AB_power_lose_file.fg_flag == 0) && (AB_power_lose_file_num_tmp > AB_power_lose_file.num))
					{
						AB_power_lose_file_num_tmp = AB_power_lose_file.num;
						set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_POWER_NO_FILE_USB_PRINT);
					}
					else
					{
						set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_POWER);
					}
					break;
				case DF_POWER_FILE_IN_2_ADDR:
					i = AB_power_lose_file_num_tmp;
					AB_power_lose_file_num_tmp = ibuf;
					if(AB_power_lose_file_num_tmp == 0)
					{
						AB_power_lose_file_num_tmp = i;
					}
					else if(AB_power_lose_file_num_tmp > DEF_POWER_FILE_MAX_W25)
					{
						AB_power_lose_file_num_tmp = i;	
					}
					if((AB_power_lose_file.fg_flag == 0) && (AB_power_lose_file_num_tmp > AB_power_lose_file.num))
					{
						AB_power_lose_file_num_tmp = AB_power_lose_file.num;
						set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_POWER_NO_FILE_USB_PRINT);
					}
					else
					{
						set_refresh_wnd_num(DF_PIC_QUERY_POWER);
					}
					break;

				case DF_SAMPLE_FILE_USB_START_IN_ADDR:
				case DF_SAMPLE_FILE_PRINT_START_IN_ADDR:
					sample_file_start_num = ibuf;
					if(AB_sample_file.fg_flag == 0)
					{
						if(sample_file_start_num > AB_sample_file.num)
						{
							sample_file_start_num = AB_sample_file.num;
						}
					}
					if(sample_file_start_num > DEF_SAMPLE_FILE_MAX_W25)
					{
						sample_file_start_num = DEF_SAMPLE_FILE_MAX_W25;
					}
					if(AB_sample_file.num != 0)
					{
						if(sample_file_start_num == 0)
						{
							sample_file_start_num = 1;
						}
					}
					DMT_ScreenIbuf(DF_SAMPLE_FILE_USB_START_ADDR,4,sample_file_start_num);//显示文件号
					DMT_ScreenIbuf(DF_SAMPLE_FILE_PRINT_START_ADDR,4,sample_file_start_num);
					break;
				case DF_SAMPLE_FILE_USB_END_IN_ADDR:
				case DF_SAMPLE_FILE_PRINT_END_IN_ADDR:
					sample_file_end_num = ibuf;
					if(AB_sample_file.fg_flag == 0)
					{
						if(sample_file_end_num > AB_sample_file.num)
						{
							sample_file_end_num = AB_sample_file.num;
						}
					}
					if(sample_file_end_num > DEF_SAMPLE_FILE_MAX_W25)
					{
						sample_file_end_num = DEF_SAMPLE_FILE_MAX_W25;
					}
					if(AB_sample_file.num != 0)
					{
						if(sample_file_end_num == 0)
						{
							sample_file_end_num = 1;
						}
					}
					DMT_ScreenIbuf(DF_SAMPLE_FILE_USB_END_ADDR,4,sample_file_end_num);//显示文件号
					DMT_ScreenIbuf(DF_SAMPLE_FILE_PRINT_END_ADDR,4,sample_file_end_num);
					break;
				case DF_POWER_FILE_USB_START_IN_ADDR:
				case DF_POWER_FILE_PRINT_START_IN_ADDR:
					power_file_start_num = ibuf;
					if(AB_power_lose_file.fg_flag == 0)
					{
						if(power_file_start_num > AB_power_lose_file.num)
						{
							power_file_start_num = AB_power_lose_file.num;
						}
					}
					if(power_file_start_num > DEF_SAMPLE_FILE_MAX_W25)
					{
						power_file_start_num = DEF_SAMPLE_FILE_MAX_W25;
					}
					if(AB_power_lose_file.num != 0)
					{
						if(power_file_start_num == 0)
						{
							power_file_start_num = 1;
						}
					}
					DMT_ScreenIbuf(DF_POWER_FILE_USB_START_ADDR,4,power_file_start_num);//显示文件号
					DMT_ScreenIbuf(DF_POWER_FILE_PRINT_START_ADDR,4,power_file_start_num);
					break;
				case DF_POWER_FILE_USB_END_IN_ADDR:
				case DF_POWER_FILE_PRINT_END_IN_ADDR:
					power_file_end_num = ibuf;
					if(AB_power_lose_file.fg_flag == 0)
					{
						if(power_file_end_num > AB_power_lose_file.num)
						{
							power_file_end_num = AB_power_lose_file.num;
						}
					}
					if(power_file_end_num > DEF_SAMPLE_FILE_MAX_W25)
					{
						power_file_end_num = DEF_SAMPLE_FILE_MAX_W25;
					}
					if(AB_power_lose_file.num != 0)
					{
						if(power_file_end_num == 0)
						{
							power_file_end_num = 1;
						}
					}
					DMT_ScreenIbuf(DF_POWER_FILE_USB_END_ADDR,4,power_file_end_num);//显示文件号
					DMT_ScreenIbuf(DF_POWER_FILE_PRINT_END_ADDR,4,power_file_end_num);
					break;

				case DF_PASSWORD_IN_ADDR:
					if((ibuf == 807) || (ibuf == password))
					{
						set_refresh_wnd_num(DF_PIC_PRESSURE_BIAODING);
					}
					else if(ibuf == 87)
					{
						supper_password = 1;
					}
					else if(ibuf == 2012)
					{
						if(supper_password == 1)
						{
							set_refresh_wnd_num(DF_PIC_SUPPER_SYS_SET);
						}
						else
						{
							supper_password = 0;
						}
					}
					else
					{
						supper_password = 0;
					}
					break;
				case DF_PRINTER_PASSWORD_IN_ADDR:
					DMT_ScreenIbuf(DF_PRINTER_PASSWORD_ADDR,4,ibuf);
					DMT_PutStr(DF_PRINTER_PASSWORD_MSG_ADDR,(u8 *)"WAIT ");
					if(bluetooth_set_printer_password(ibuf) == 1)
					{
						printer_password = ibuf;
						DMT_PutStr(DF_PRINTER_PASSWORD_MSG_ADDR,(u8 *)"OK!  ");
						saveintdata(ADDR_printer_password,printer_password);
					}
					else
					{
						DMT_PutStr(DF_PRINTER_PASSWORD_MSG_ADDR,(u8 *)"ERROR");
					}
					break;
				case DF_A_START_PWM_IN_ADDR:
					if(ibuf > 60000)
					{
						ibuf = 60000;
					}
					A_PWM_01L = ibuf;
					saveintdata(ADDR_A_PWM_01L,A_PWM_01L);
					set_refresh_wnd_num(DF_PIC_SUPPER_SYS_SET);
					break;
				case DF_B_START_PWM_IN_ADDR:
					if(ibuf > 60000)
					{
						ibuf = 60000;
					}
					B_PWM_01L = ibuf;
					saveintdata(ADDR_B_PWM_01L,B_PWM_01L);
					set_refresh_wnd_num(DF_PIC_SUPPER_SYS_SET);
					break;
				case DF_BAT_VOLTAGE_RATE_IN_ADDR:
					bat_voltage_rate = ibuf / 1000.0;
					savefloat(ADDR_bat_voltage_rate,bat_voltage_rate);
					set_refresh_wnd_num(DF_PIC_SUPPER_SYS_SET);
					break;	
				case DF_WK_PID_P_IN_ADDR:
					wk_pid_para.pgain = (float)ibuf / (float)10.0;
					savefloat(ADDR_pid_para_pgain,wk_pid_para.pgain);
					set_refresh_wnd_num(DF_PIC_SUPPER_PID_SET);
					break;
				case DF_WK_PID_I_IN_ADDR:
					wk_pid_para.igain = (float)ibuf / (float)1000.0;
					savefloat(ADDR_pid_para_igain,wk_pid_para.igain);
					set_refresh_wnd_num(DF_PIC_SUPPER_PID_SET);
					break;
				case DF_WK_PID_I_LIMIT_IN_ADDR:
					pid_integral_limit = ibuf;
					W_24(ADDR_pid_integral_limit,pid_integral_limit);
					set_refresh_wnd_num(DF_PIC_SUPPER_PID_SET);
					break;	
				case DF_WK_PID_D_IN_ADDR:
					wk_pid_para.dgain = (float)ibuf / (float)10.0;
					savefloat(ADDR_pid_para_dgain,wk_pid_para.dgain);
					set_refresh_wnd_num(DF_PIC_SUPPER_PID_SET);
					break;		
				case DF_WK_PID_DEADBAND_IN_ADDR:
					wk_pid_para.deadband = (float)ibuf / (float)10.0;
					savefloat(ADDR_pid_para_deadband,wk_pid_para.deadband);
					set_refresh_wnd_num(DF_PIC_SUPPER_PID_SET);
					break;
				case DF_WK_PID_START_HEATING_IN_ADDR:
					wk_start_heat_err = (float)ibuf / (float)10.0;
					savefloat(ADDR_wk_start_heat_err,wk_start_heat_err);
					set_refresh_wnd_num(DF_PIC_SUPPER_PID_SET);
					break;			
				case DF_BIANHAO_IN_ADDR:
					lbuf = DMT_GetVariableLongValue(FRAME_BUF);
					bianhao_new = lbuf;
					savelongdata(ADDR_bianhao,lbuf);
					set_refresh_wnd_num(DF_PIC_SUPPER_BIANHAO);
					break;
				case DF_BLUETOOTH_CLOSE_TIME_IN_ADDR:
					BLUETOOTH.close_time_set = ibuf;
					saveintdata(ADDR_BLUETOOTH_close_time_set,BLUETOOTH.close_time_set);
					DMT_ScreenIbuf(DF_DIS_BLUETOOTH_CLOSE_TIME_ADDR,3,BLUETOOTH.close_time_set);
					break;
			}
		}
	}
//	else if(locale_flag == DEF_READ_REGISTER_RESPONSE)
    else
	{
        locale_flag = RTC_AnalysisDataBuf(FRAME_BUF2);
        if(locale_flag  == DEF_READ_REGISTER_RESPONSE)
        {
            i = DMT_GetRegisterAddr(FRAME_BUF2);
            if(i == 0x06)
            {
                if((FRAME_BUF2[4] == 0x01) || (FRAME_BUF2[4] == 0x03))
                {
                    display_sleep_init();
                    if(DMT_TEST_DELAY.flag == 1)
                    {
                        DMT_TEST_DELAY.flag = 0;
                        if(DMT_error < 60)
                        {
                            DMT_error++;
                        }
                    }
                }
                else
                {
                    DMT_error = 0;
                }
            }
            else if(i == DEF_REGISTER_RTC)
            {
                if(RTC_select == DF_RTC_MAIN_BOARD)
                {
                
                }
                else
                {
                    DMT_GetRTCValue(FRAME_BUF2);
                }
                if(DMT_TEST_DELAY.flag == 1)
                {
                    DMT_TEST_DELAY.flag = 0;
                    DMT_error = 0;
                }
            }
            else
            {
                if(DMT_TEST_DELAY.flag == 1)
                {
                    DMT_TEST_DELAY.flag = 0;
                    DMT_error = 0;
                }
            }
        }
	}
}
float A_flow_maintenance_tmp;
float B_flow_maintenance_tmp;
void key_task_start_set(void)
{
	switch (key)
	{
		case DF_KEY_VALUE_A2B:
			if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_IDLE)
			{
				B_SAMPLE.flow_set = A_SAMPLE.flow_set;
				B_SAMPLE.running_time_set = A_SAMPLE.running_time_set;
				B_SAMPLE.sample_volume_set = A_SAMPLE.sample_volume_set;
				B_SAMPLE.start_hour_set = A_SAMPLE.start_hour_set;
				B_SAMPLE.start_min_set = A_SAMPLE.start_min_set;
				B_SAMPLE.jiange_time_set = A_SAMPLE.jiange_time_set;
				B_SAMPLE.sample_times_set = A_SAMPLE.sample_times_set;
				SAMPLE_MODE.channel_B = SAMPLE_MODE.channel_A;
				save_sample_para();
			}
			else			//提示B路正在运行，不能完成操作
			{
				set_refresh_wnd_num(DF_PIC_COPY_TO_B_ERR);
			}
			break;
		case DF_KEY_VALUE_ALL_START:
			if(SAMPLE_MODE.channel_A_selected == DF_SAMPLE_CHANNEL_SELECTED)
			{
				SAMPLE_TASK_A.step = DF_SAMPLE_TASK_WAIT;
				A_SAMPLE.sample_times = 0;
			}
            if(SAMPLE_MODE.channel_B_selected == DF_SAMPLE_CHANNEL_SELECTED)
			{
				SAMPLE_TASK_B.step = DF_SAMPLE_TASK_WAIT;
				B_SAMPLE.sample_times = 0;
			}
			sample_pic_select = DF_SAMPLE_SELECT_SAMPLE;
			sample_pic_refresh_flag = 1;
			break;
		case DF_KEY_VALUE_ALL_START_NOW:
			if(SAMPLE_MODE.channel_A_selected == DF_SAMPLE_CHANNEL_SELECTED)
			{
				SAMPLE_TASK_A.step = DF_SAMPLE_TASK_AUTOZERO;
				A_SAMPLE.sample_times = 0;
			}
            if(SAMPLE_MODE.channel_B_selected == DF_SAMPLE_CHANNEL_SELECTED)
			{
				SAMPLE_TASK_B.step = DF_SAMPLE_TASK_AUTOZERO;
				B_SAMPLE.sample_times = 0;
			}
			sample_pic_select = DF_SAMPLE_SELECT_SAMPLE;
			sample_pic_refresh_flag = 1;
			break;
	}
}

void key_task(void)
{
	switch (key)
	{
		case DF_KEY_VALUE_HOME:
			if((SAMPLE_TASK_A.step != DF_SAMPLE_TASK_IDLE) || (SAMPLE_TASK_B.step != DF_SAMPLE_TASK_IDLE))
			{
				set_refresh_wnd_num(DF_PIC_SAMPLE_AB_STOP_OR_NOT);
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_HOME);
			}
			break;
		case DF_KEY_VALUE_MAINTENANCE_HOME:
			set_refresh_wnd_num(DF_PIC_EXIT_MAINTENANCE);
			break;
		case DF_KEY_VALUE_EXIT_MAINTENANCE_YES:
			A_SAMPLE.flow_set = A_flow_maintenance_tmp;
			B_SAMPLE.flow_set = B_flow_maintenance_tmp;
			set_refresh_wnd_num(DF_PIC_HOME);
			break;
		case DF_KEY_VALUE_EXIT_MAINTENANCE_NO:
			set_refresh_last_wnd_num();
			break;
		case DF_KEY_VALUE_SET:
			set_refresh_wnd_num(DF_PIC_SET);
			break;
		case DF_KEY_VALUE_ENVIRONMENT_TEMPERATURE_MEASURE:
			sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode = !DF_INPUT_MODE;
			W_24(ADDR_environment_temperature_mode,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode);//温度选择模式
			set_refresh_wnd_num(DF_PIC_SET);
			break;
		case DF_KEY_VALUE_ENVIRONMENT_TEMPERATURE_INPUT:
			sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode = DF_INPUT_MODE;
			W_24(ADDR_environment_temperature_mode,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode);//温度选择模式
			set_refresh_wnd_num(DF_PIC_SET);
			break;
		case DF_KEY_VALUE_BAR_MEASURE:
			barometric.mode = !DF_INPUT_MODE;
			W_24(ADDR_barometric_mode,barometric.mode);
			set_refresh_wnd_num(DF_PIC_SET);
			break;
		case DF_KEY_VALUE_BAR_INPUT:
			barometric.mode = DF_INPUT_MODE;
			W_24(ADDR_barometric_mode,barometric.mode);
			set_refresh_wnd_num(DF_PIC_SET);
			break;
		case DF_KEY_VALUE_TEMPERATURE_CTR_ON:
			start_wk_ctr();
			W_24(ADDR_wk_enable,wk_enable);
			set_refresh_wnd_num(DF_PIC_SET);
			break;
		case DF_KEY_VALUE_TEMPERATURE_CTR_OFF:
			stop_wk_ctr();
			W_24(ADDR_wk_enable,wk_enable);
			set_refresh_wnd_num(DF_PIC_SET);
			break;
	
		case DF_KEY_VALUE_MAINTENANCE:
			A_flow_maintenance_tmp = A_SAMPLE.flow_set;
			B_flow_maintenance_tmp = B_SAMPLE.flow_set;
			set_refresh_wnd_num(DF_PIC_MAINTENANCE);
			break;
		case DF_KEY_VALUE_PRINT:
			set_refresh_wnd_num(DF_PIC_PRINT);
			break;
		case DF_KEY_VALUE_USB:
			set_refresh_wnd_num(DF_PIC_USB);
			break;
		case DF_KEY_VALUE_SAMPLE:
			//可以根据channel和select选择进入界面，或直接定位到A路采样设置
			SAMPLE_TASK_A.step = DF_SAMPLE_TASK_IDLE;
			SAMPLE_TASK_B.step = DF_SAMPLE_TASK_IDLE;
			sample_pic_select = DF_SAMPLE_SELECT_SAMPLE;
			sample_pic_channel = DF_SAMPLE_CHANNEL_A;	
			sample_pic_refresh_flag = 1;	
			break;
		case DF_KEY_VALUE_A_CHANNEL:
			sample_pic_refresh_flag = 1;
			sample_pic_channel = DF_SAMPLE_CHANNEL_A;
			break;
		case DF_KEY_VALUE_B_CHANNEL:
			sample_pic_refresh_flag = 1;
			sample_pic_channel = DF_SAMPLE_CHANNEL_B;
			break;
        case DF_KEY_VALUE_A2B:
			if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_IDLE)
			{
				B_SAMPLE.flow_set = A_SAMPLE.flow_set;
				B_SAMPLE.running_time_set = A_SAMPLE.running_time_set;
				B_SAMPLE.sample_volume_set = A_SAMPLE.sample_volume_set;
				B_SAMPLE.start_hour_set = A_SAMPLE.start_hour_set;
				B_SAMPLE.start_min_set = A_SAMPLE.start_min_set;
				B_SAMPLE.jiange_time_set = A_SAMPLE.jiange_time_set;
				B_SAMPLE.sample_times_set = A_SAMPLE.sample_times_set;
				SAMPLE_MODE.channel_B = SAMPLE_MODE.channel_A;
				save_sample_para();
			}
			else			//提示B路正在运行，不能完成操作
			{
				set_refresh_wnd_num(DF_PIC_COPY_TO_B_ERR);
			}
			break;
		case DF_KEY_VALUE_A_BY_TIME:
			SAMPLE_MODE.channel_A = DF_SAMPLE_MODE_BY_TIME;
			save_sample_mode();
			set_refresh_wnd_num(DF_PIC_SAMPLE_A_SET);
			break;
		case DF_KEY_VALUE_A_BY_VOLUME:
			SAMPLE_MODE.channel_A = DF_SAMPLE_MODE_BY_VOLUME;
			save_sample_mode();
			set_refresh_wnd_num(DF_PIC_SAMPLE_A_SET_VOLUME);
			break;
		case DF_KEY_VALUE_A_START:
			SAMPLE_TASK_A.step = DF_SAMPLE_TASK_WAIT;
			A_SAMPLE.sample_times = 0;
			sample_pic_refresh_flag = 1;
			break;
		case DF_KEY_VALUE_A_START_NOW:
			SAMPLE_TASK_A.step = DF_SAMPLE_TASK_AUTOZERO;
			A_SAMPLE.sample_times = 0;
			sample_pic_refresh_flag = 1;
			break;
		case DF_KEY_VALUE_STOP_A_SAMPLE_OR_NOT:
			set_refresh_wnd_num(DF_PIC_SAMPLE_A_STOP_OR_NOT);
			break;	
		case DF_KEY_VALUE_STOP_A_SAMPLE:
			if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_SAMPLING)	//若处于采样状态则保存文件，否则不保存，因为在每次采样结束时已经保存了文件
			{
				SaveSampleFile_A();
			}
			else if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_ERR)
			{
				SaveSampleFile_A();
			}
			A_sample_over();
			stop_sample_A_task();
			sample_pic_refresh_flag = 1;
			break;
		case DF_KEY_VALUE_CONTINUE_A_SAMPLE:
			sample_pic_refresh_flag = 1;
			break;
		case DF_KEY_VALUE_ERR_CONTINUE_A_SAMPLE:
			SAMPLE_TASK_A.step = DF_SAMPLE_TASK_SAMPLING;
			start_ctr_A_pump_task();
			sample_pic_refresh_flag = 1;
			break;	
		case DF_KEY_VALUE_B_BY_TIME:
			SAMPLE_MODE.channel_B = DF_SAMPLE_MODE_BY_TIME;
			save_sample_mode();
			set_refresh_wnd_num(DF_PIC_SAMPLE_B_SET);
			break;
		case DF_KEY_VALUE_B_BY_VOLUME:
			SAMPLE_MODE.channel_B = DF_SAMPLE_MODE_BY_VOLUME;
			save_sample_mode();
			set_refresh_wnd_num(DF_PIC_SAMPLE_B_SET_VOLUME);
			break;
		case DF_KEY_VALUE_B_START:
			SAMPLE_TASK_B.step = DF_SAMPLE_TASK_WAIT;
			sample_pic_refresh_flag = 1;
			break;
		case DF_KEY_VALUE_B_START_NOW:
			SAMPLE_TASK_B.step = DF_SAMPLE_TASK_AUTOZERO;
			B_SAMPLE.sample_times = 0;
			sample_pic_refresh_flag = 1;
			break;
		case DF_KEY_VALUE_STOP_B_SAMPLE_OR_NOT:
			set_refresh_wnd_num(DF_PIC_SAMPLE_B_STOP_OR_NOT);
			break;			
		case DF_KEY_VALUE_STOP_B_SAMPLE:
			if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_SAMPLING)	//若处于采样状态则保存文件，否则不保存，因为在每次采样结束时已经保存了文件
			{
				SaveSampleFile_B();
			}
			else if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_ERR)
			{
				SaveSampleFile_B();
			}
			B_sample_over();
			stop_sample_B_task();
			sample_pic_refresh_flag = 1;
			break;
		case DF_KEY_VALUE_CONTINUE_B_SAMPLE:
			sample_pic_refresh_flag = 1;
			break;
		case DF_KEY_VALUE_ERR_CONTINUE_B_SAMPLE:
			SAMPLE_TASK_B.step = DF_SAMPLE_TASK_SAMPLING;
			start_ctr_B_pump_task();
			sample_pic_refresh_flag = 1;
			break;
        case DF_KEY_VALUE_ALL_START:
			if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_IDLE)
			{
				SAMPLE_TASK_A.step = DF_SAMPLE_TASK_WAIT;
				SAMPLE_TASK_B.step = DF_SAMPLE_TASK_WAIT;
				sample_pic_refresh_flag = 1;
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_A_TO_START_ERR);
			}
			break;    
		case DF_KEY_VALUE_ALL_START_NOW:
			if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_IDLE)
			{
				SAMPLE_TASK_A.step = DF_SAMPLE_TASK_AUTOZERO;
				SAMPLE_TASK_B.step = DF_SAMPLE_TASK_AUTOZERO;
				A_SAMPLE.sample_times = 0;
				B_SAMPLE.sample_times = 0;
				sample_pic_refresh_flag = 1;
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_A_TO_START_ERR);
			}
			break;            
		case DF_KEY_VALUE_STOP_AB_SAMPLE:
			if((SAMPLE_TASK_A.step == DF_SAMPLE_TASK_SAMPLING) || (SAMPLE_TASK_A.step == DF_SAMPLE_TASK_ERR))
			{
				SaveSampleFile_A();
				
			}
			if((SAMPLE_TASK_B.step == DF_SAMPLE_TASK_SAMPLING) || (SAMPLE_TASK_B.step == DF_SAMPLE_TASK_ERR))
			{
				SaveSampleFile_B();	
			}
			A_sample_over();
			stop_sample_A_task();
			B_sample_over();
			stop_sample_B_task();
			set_refresh_wnd_num(DF_PIC_HOME);
			break;
		case DF_KEY_VALUE_CONTINUE_AB_SAMPLE:
			sample_pic_refresh_flag = 1;
			break;	
		case DF_KEY_VALUE_SAMPLE_SAMPLE:
			sample_pic_refresh_flag = 1;
			sample_pic_select = DF_SAMPLE_SELECT_SAMPLE;
			break;
			
		case DF_KEY_VALUE_SAMPLE_DATA:
			sample_pic_refresh_flag = 1;
			sample_pic_select = DF_SAMPLE_SELECT_DATA;
			break;
		case DF_KEY_VALUE_QUERY_SAMPLE_FILE:
			sample_pic_refresh_flag = 1;
			sample_pic_select = DF_SAMPLE_SELECT_DATA;
			break;
		case DF_KEY_VALUE_SAMPLE_FILE_PAGEUP:
//		case DF_KEY_VALUE_SAMPLE_FILE_BUTTON_PAGEUP:
			if(AB_sample_file_num_tmp > 1)
			{
				AB_sample_file_num_tmp--;
			}
			else
			{
				if(AB_sample_file.fg_flag != 0)
				{
					AB_sample_file_num_tmp = DEF_SAMPLE_FILE_MAX_W25;
				}
				else
				{
					AB_sample_file_num_tmp = AB_sample_file.num;
				}
			}
			set_refresh_wnd_num(wnd_struct.current);
			break;
		case DF_KEY_VALUE_SAMPLE_FILE_PAGEDOWN:
//		case DF_KEY_VALUE_SAMPLE_FILE_BUTTON_PAGEDOWN:
			if(AB_sample_file_num_tmp < DEF_SAMPLE_FILE_MAX_W25)
			{
				if(AB_sample_file.fg_flag != 0)
				{
					AB_sample_file_num_tmp++;
				}
				else
				{
					if(AB_sample_file_num_tmp < AB_sample_file.num)
					{
						AB_sample_file_num_tmp++;
					}
					else
					{
						AB_sample_file_num_tmp = 1;
					}
				}
			}
			else
			{
				AB_sample_file_num_tmp = 1;
			}
			set_refresh_wnd_num(wnd_struct.current);
			break;
		case DF_KEY_VALUE_SAMPLE_FILE_USB:
			if(USB_OUT_SAMPLE.step != DF_USB_IDLE)
			{
				break;
			}
			if(AB_sample_file_num_tmp != 0)
			{
				usb_out_start_file = AB_sample_file_num_tmp;
				usb_out_end_file = usb_out_start_file;
				USB_OUT_SAMPLE.step = DF_USB_INIT;			//启动U盘导出任务
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_NO_FILE_USB_PRINT);
			}
			break;
		case DF_KEY_VALUE_SAMPLE_FILE_PRINT:
			if(AB_sample_file_num_tmp != 0)
			{
				print_start_file = AB_sample_file_num_tmp;
				print_end_file = print_start_file;
				PRINT_SAMPLE.step = DF_PRINT_INIT;			//启动打印任务	
			}	
			else
			{
				set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_NO_FILE_USB_PRINT);
			}	
			break;	
//		case DF_KEY_VALUE_SAMPLE_FILE_BUTTON_PRINT:
//			if(AB_sample_file_num_tmp != 0)
//			{
//				print_start_file = AB_sample_file_num_tmp;
//				print_end_file = print_start_file;
//				PRINT_SAMPLE.step = DF_PRINT_INIT;			//启动打印任务	
//			}	
//			else
//			{
//				set_refresh_wnd_num(DF_PIC_QUERY_NO_SAMPLE_FILE);
//			}	
//			break;			
		case DF_KEY_VALUE_QUERY_POWER_FILE:
			sample_pic_refresh_flag = 1;
			sample_pic_select = DF_SAMPLE_SELECT_DATA_POWER;
			break;
		case DF_KEY_VALUE_POWER_FILE_PAGEUP:
			if(AB_power_lose_file_num_tmp > 1)
			{
				AB_power_lose_file_num_tmp--;
			}
			else
			{
				if(AB_power_lose_file.fg_flag != 0)
				{
					AB_power_lose_file_num_tmp = DEF_POWER_FILE_MAX_W25;
				}
				else
				{
					AB_power_lose_file_num_tmp = AB_power_lose_file.num;
				}
			}
			set_refresh_wnd_num(wnd_struct.current);
			break;
		case DF_KEY_VALUE_POWER_FILE_PAGEDOWN:
			if(AB_power_lose_file_num_tmp < DEF_POWER_FILE_MAX_W25)
			{
				if(AB_power_lose_file.fg_flag != 0)
				{
					AB_power_lose_file_num_tmp++;
				}
				else
				{
					if(AB_power_lose_file_num_tmp < AB_power_lose_file.num)
					{
						AB_power_lose_file_num_tmp++;
					}
					else
					{
						AB_power_lose_file_num_tmp = 1;
					}
				}
			}
			else
			{
				AB_power_lose_file_num_tmp = 1;
			}
			set_refresh_wnd_num(wnd_struct.current);
			break;
		case DF_KEY_VALUE_POWER_FILE_USB:
			if(USB_OUT_POWER.step != DF_USB_IDLE)
			{
				break;
			}
			if(AB_power_lose_file_num_tmp != 0)
			{
				usb_out_start_file = AB_power_lose_file_num_tmp;
				usb_out_end_file = usb_out_start_file;
				USB_OUT_POWER.step = DF_USB_INIT;			//启动U盘导出任务
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_POWER_NO_FILE_USB_PRINT);
			}
			break;		
		case DF_KEY_VALUE_POWER_FILE_PRINT:
			if(AB_power_lose_file_num_tmp != 0)
			{
				print_start_file = AB_power_lose_file_num_tmp;
				print_end_file = print_start_file;
				PRINT_POWER.step = DF_PRINT_INIT;			//启动打印任务	
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_POWER_NO_FILE_USB_PRINT);
			}		
			break;			

		case DF_KEY_VALUE_SAMPLE_USB:
			sample_pic_refresh_flag = 1;
			sample_pic_select = DF_SAMPLE_SELECT_USB;
			break;
		case DF_KEY_VALUE_SAMPLE_USB_ALL:
			if(USB_OUT_SAMPLE.step != DF_USB_IDLE)
			{
				break;
			}
			if(sample_file_start_num != 0)
			{
				usb_out_start_file = sample_file_start_num ;
				usb_out_end_file = sample_file_end_num ;
				USB_OUT_SAMPLE.step = DF_USB_INIT;			//启动U盘导出任务
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_NO_FILE_USB_PRINT);
			}
			break;
		case DF_KEY_VALUE_POWER_USB_ALL:
			if(USB_OUT_POWER.step != DF_USB_IDLE)
			{
				break;
			}
			if(power_file_start_num != 0)
			{
				usb_out_start_file = power_file_start_num;
				usb_out_end_file = power_file_end_num;
				USB_OUT_POWER.step = DF_USB_INIT;			//启动U盘导出任务
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_POWER_NO_FILE_USB_PRINT);
			}
			break;				
			
		case DF_KEY_VALUE_SAMPLE_PRINT:
			sample_pic_refresh_flag = 1;
			sample_pic_select = DF_SAMPLE_SELECT_PRINT;
			break;
		case DF_KEY_VALUE_SAMPLE_PRINT_ALL:
			if(sample_file_start_num != 0)
			{
				print_start_file = sample_file_start_num;
				print_end_file = sample_file_end_num;
				PRINT_SAMPLE.step = DF_PRINT_INIT;			//启动打印任务	
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_NO_FILE_USB_PRINT);
			}
			break;
		case DF_KEY_VALUE_POWER_PRINT_ALL:
			if(power_file_start_num != 0)
			{
				print_start_file = power_file_start_num;
				print_end_file = power_file_end_num;
				PRINT_POWER.step = DF_PRINT_INIT;			//启动打印任务	
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_POWER_NO_FILE_USB_PRINT);
			}
			break;			

		case DF_KEY_VALUE_QUERY:
		case DF_KEY_VALUE_SAMPLE_FILE_BUTTON:
			AB_sample_file.num = readintdata(ADDR_AB_sample_file_num);
			AB_sample_file_num_tmp = AB_sample_file.num;
			AB_sample_file.fg_flag = R_24(ADDR_AB_sample_file_fg_flag);
			if((AB_sample_file.fg_flag == 0) && (AB_sample_file.num == 0))
			{
				set_refresh_wnd_num(DF_PIC_QUERY_NO_SAMPLE_FILE);
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_QUERY);
			}
			break;
		
		case DF_KEY_VALUE_POWER_FILE_BUTTON:
			AB_power_lose_file.num = readintdata(ADDR_AB_power_lose_file_num);
			AB_power_lose_file_num_tmp = AB_power_lose_file.num;
			AB_power_lose_file.fg_flag = R_24(ADDR_AB_power_lose_file_fg_flag);
			if((AB_power_lose_file.fg_flag == 0) && (AB_power_lose_file.num == 0))
			{
				set_refresh_wnd_num(DF_PIC_QUERY_NO_POWER_FILE);
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_QUERY_POWER);
			}
			break;


		case DF_KEY_VALUE_POWER_LOSE_YES:
			sample_pic_refresh_flag = 1;					//显示刷新
			sample_pic_select = DF_SAMPLE_SELECT_SAMPLE;	//采样界面
			sample_pic_channel = DF_SAMPLE_CHANNEL_A;
			//采样状态、参数初始化

			if(A_power_low_sample_status == DF_SAMPLE_STATUS_TO_RUN)
			{
				SAMPLE_TASK_A.step = DF_SAMPLE_TASK_AUTOZERO;
				sample_pic_channel = DF_SAMPLE_CHANNEL_A;
			}
			if(A_power_low_sample_status == DF_SAMPLE_STATUS_CONTINUE_RUN)
			{
				SAMPLE_TASK_A.step = DF_SAMPLE_TASK_CONTINUE_SAMPLING;
				sample_pic_channel = DF_SAMPLE_CHANNEL_A;
			}
			if(A_power_low_sample_status == DF_SAMPLE_STATUS_WAIT_RUN)
			{
				sample_pic_channel = DF_SAMPLE_CHANNEL_A;
			}
			if(A_power_low_sample_status == DF_SAMPLE_STATUS_INVALID)
			{
				SAMPLE_TASK_A.step = DF_SAMPLE_TASK_IDLE;
				A_sample_over();
			}
			if(B_power_low_sample_status == DF_SAMPLE_STATUS_TO_RUN)
			{
				SAMPLE_TASK_B.step = DF_SAMPLE_TASK_AUTOZERO;
				if(sample_pic_channel != DF_SAMPLE_CHANNEL_A)
				{
					sample_pic_channel = DF_SAMPLE_CHANNEL_B;
				}
			}
			if(B_power_low_sample_status == DF_SAMPLE_STATUS_CONTINUE_RUN)
			{
				SAMPLE_TASK_B.step = DF_SAMPLE_TASK_CONTINUE_SAMPLING;
				if(sample_pic_channel != DF_SAMPLE_CHANNEL_A)
				{
					sample_pic_channel = DF_SAMPLE_CHANNEL_B;
				}
			}
			if(B_power_low_sample_status == DF_SAMPLE_STATUS_WAIT_RUN)
			{
				if(sample_pic_channel != DF_SAMPLE_CHANNEL_A)
				{
					sample_pic_channel = DF_SAMPLE_CHANNEL_B;
				}
			}
			if(B_power_low_sample_status == DF_SAMPLE_STATUS_INVALID)
			{
				SAMPLE_TASK_B.step = DF_SAMPLE_TASK_IDLE;
				B_sample_over();
			}
			break;
		case DF_KEY_VALUE_POWER_LOSE_NO:
			if(A_power_low_sample_status == DF_SAMPLE_STATUS_CONTINUE_RUN)
			{
				SaveSampleFile_A();
				
			}
			if(B_power_low_sample_status == DF_SAMPLE_STATUS_CONTINUE_RUN)
			{
				SaveSampleFile_B();	
			}
			A_sample_over();
			stop_sample_A_task();
			B_sample_over();
			stop_sample_B_task();
			set_refresh_wnd_num(DF_PIC_HOME);
			break;
		case DF_KEY_VALUE_PRESSURE_BIAODING:
			set_refresh_wnd_num(DF_PIC_PRESSURE_BIAODING);
			break;
		case DF_KEY_VALUE_TEMPERATURE_BIAODING:
			set_refresh_wnd_num(DF_PIC_TEMPERATURE_BIAODING);
			break;
		case DF_KEY_VALUE_FLOW_BIAODING:
			A_SAMPLE.flow_set = 0.1;
			B_SAMPLE.flow_set = 0.1;
			set_refresh_wnd_num(DF_PIC_FLOW_BIAODING);
			break;
		case DF_KEY_VALUE_START_BIAODING_A:
			if(PUMP_CTR_TASK_A.enable != 1)
			{
				start_ctr_A_pump_task();
			}
			else
			{
				stop_ctr_A_pump_task();
			}
			set_refresh_wnd_num(DF_PIC_FLOW_BIAODING);
			break;
		case DF_KEY_VALUE_AUTOZERO_BIAODING_A:
			if(PUMP_CTR_TASK_A.enable != 1)
			{
				A_auto_zero();
			}
			set_refresh_wnd_num(DF_PIC_FLOW_BIAODING);
			break;
		case DF_KEY_VALUE_START_BIAODING_B:
			if(PUMP_CTR_TASK_B.enable != 1)
			{
				start_ctr_B_pump_task();
			}
			else
			{
				stop_ctr_B_pump_task();
			}
			set_refresh_wnd_num(DF_PIC_FLOW_BIAODING);
			break;
		case DF_KEY_VALUE_AUTOZERO_BIAODING_B:
			if(PUMP_CTR_TASK_B.enable != 1)
			{
				B_auto_zero();
			}
			set_refresh_wnd_num(DF_PIC_FLOW_BIAODING);
			break;
		case DF_KEY_VALUE_DELETE_ALL_FILES:
			set_refresh_wnd_num(DF_PIC_DELETE_ALL_FILES);
			break;
		case DF_KEY_VALUE_DELETE_ALL_FILES_YES:
			set_refresh_wnd_num(DF_PIC_WAIT_DELETE_FILES);
			break;
		case DF_KEY_VALUE_DELETE_ALL_FILES_NO:
			set_refresh_wnd_num(DF_PIC_OTHER_SET);
			break;	
		case DF_KEY_VALUE_RESET_SYS:
			set_refresh_wnd_num(DF_PIC_RESET_SYS);
			break;
		case DF_KEY_VALUE_RESET_SYS_YES:
			set_refresh_wnd_num(DF_PIC_WAIT_RESET);
			break;	
		case DF_KEY_VALUE_RESET_SYS_NO:
			set_refresh_wnd_num(DF_PIC_OTHER_SET);
			break;	
		case DF_KEY_VALUE_PASSWORD_SET:
			set_refresh_wnd_num(DF_PIC_PASSWORD_SET);
			break;
		case DF_KEY_VALUE_FILE_FORMAT:
			set_refresh_wnd_num(DF_PIC_FILE_FORMAT);
			break;
		case DF_KEY_VALUE_FILE_FORMAT_TXT:
			file_format = 0;
			W_24(ADDR_file_format,file_format);
			set_refresh_wnd_num(DF_PIC_FILE_FORMAT);
			break;
		case DF_KEY_VALUE_FILE_FORMAT_CSV:
			file_format = 1;
			W_24(ADDR_file_format,file_format);
			set_refresh_wnd_num(DF_PIC_FILE_FORMAT);
			break;
		case DF_KEY_VALUE_RTC_SELECT:
			set_refresh_wnd_num(DF_PIC_RTC_SELECT);
			break;
		case DF_KEY_VALUE_RTC_BOARD:
			RTC_select = DF_RTC_MAIN_BOARD;
			W_24(ADDR_RTC_select,RTC_select);
			set_refresh_wnd_num(DF_PIC_RTC_SELECT);
			break;
		case DF_KEY_VALUE_RTC_DMT:
			RTC_select = DF_RTC_DMT;
			W_24(ADDR_RTC_select,RTC_select);
			set_refresh_wnd_num(DF_PIC_RTC_SELECT);
			break;			
		case DF_KEY_VALUE_FLOW_KEDU:
			A_SAMPLE.flow_ctr_select = DF_FLOW_CTR_KEDU;
			B_SAMPLE.flow_ctr_select = DF_FLOW_CTR_KEDU;
			W_24(ADDR_A_sample_flow_ctr_select,A_SAMPLE.flow_ctr_select);
			W_24(ADDR_B_sample_flow_ctr_select,B_SAMPLE.flow_ctr_select);
			set_refresh_wnd_num(DF_PIC_FLOW_CTR);
			break;	
		case DF_KEY_VALUE_FLOW_RUKOU:
			A_SAMPLE.flow_ctr_select = DF_FLOW_CTR_RUKOU;
			B_SAMPLE.flow_ctr_select = DF_FLOW_CTR_RUKOU;
			W_24(ADDR_A_sample_flow_ctr_select,A_SAMPLE.flow_ctr_select);
			W_24(ADDR_B_sample_flow_ctr_select,B_SAMPLE.flow_ctr_select);
			set_refresh_wnd_num(DF_PIC_FLOW_CTR);
			break;	
		case DF_KEY_VALUE_FLOW_BIAOKUANG:
			A_SAMPLE.flow_ctr_select = DF_FLOW_CTR_BIAOKUANG;
			B_SAMPLE.flow_ctr_select = DF_FLOW_CTR_BIAOKUANG;
			W_24(ADDR_A_sample_flow_ctr_select,A_SAMPLE.flow_ctr_select);
			W_24(ADDR_B_sample_flow_ctr_select,B_SAMPLE.flow_ctr_select);
			set_refresh_wnd_num(DF_PIC_FLOW_CTR);
			break;	
			
		case DF_KEY_VALUE_PRINT_SET:
			set_refresh_wnd_num(DF_PIC_PRINT_SET);
			break;	
		case DF_KEY_VALUE_FLOW_CTR:
			set_refresh_wnd_num(DF_PIC_FLOW_CTR);
			break;
		case DF_KEY_VALUE_PRESSURE_ZERO:
			A_auto_zero();
			B_auto_zero();
			savefloat(ADDR_A_SAMPLE_dy_zero,A_SAMPLE.dy_zero);
			savefloat(ADDR_A_SAMPLE_jy_zero,A_SAMPLE.jy_zero);
			savefloat(ADDR_B_SAMPLE_dy_zero,B_SAMPLE.dy_zero);
			savefloat(ADDR_B_SAMPLE_jy_zero,B_SAMPLE.jy_zero);
//saverate_zero();
			set_refresh_wnd_num(DF_PIC_PRESSURE_BIAODING);
			break;
		
		case DF_KEY_VALUE_RESET_BLUETOOTH:
			DMT_PutStr(DF_BLUETOOTH_RESET_MSG_ADDR,(u8 *)"WAIT ");
			if(bluetooth_set_master() ==  1)
			{
				DMT_PutStr(DF_BLUETOOTH_RESET_MSG_ADDR,(u8 *)"OK!  ");
			}
			else
			{
				DMT_PutStr(DF_BLUETOOTH_RESET_MSG_ADDR,(u8 *)"ERROR");
			}
			break;

		case DF_KEY_VALUE_SYS_SET:
			set_refresh_wnd_num(DF_PIC_SUPPER_SYS_SET);
			break;
		case DF_KEY_VALUE_WK_PID:
			set_refresh_wnd_num(DF_PIC_SUPPER_PID_SET);
			break;
		case DF_KEY_VALUE_SYS_BACKUP:
			set_refresh_wnd_num(DF_PIC_SUPPER_SYS_BACKUP);
			break;
		case DF_KEY_VALUE_SYS_BACKUP_YES:
			SavePara(DF_OFFSET);
			W_24(ADDR_bak_flag,1);		//备份标志
			DMT_PutStr(DF_BACK_UP_MSG_ADDR,(u8 *)"OK!");
			break;
		case DF_KEY_VALUE_SYS_BIANHAO:
			set_refresh_wnd_num(DF_PIC_SUPPER_BIANHAO);
			break;
		case DF_KEY_VALUE_OTHER_SET:
		case DF_KEY_VALUE_TO_OTHER_SET:
			set_refresh_wnd_num(DF_PIC_OTHER_SET);
			break;
		case DF_KEY_VALUE_BLUETOOTH_SET:
			if(wnd_struct.current != DF_PIC_BLUETOOTH_SET)
			{
				bluetooth_last_wnd = wnd_struct.current;
				set_refresh_wnd_num(DF_PIC_BLUETOOTH_SET);
			}
			break;
		case DF_KEY_VALUE_BLUETOOTH_CONNECT:
			bluetooth_task_init();
			DMT_PutStr(DF_DIS_BLUETOOTH_TEXT_ADDR,(u8 *)"连接中...");
			set_refresh_wnd_num(DF_PIC_BLUETOOTH_SET);
			break;
		case DF_KEY_VALUE_BLUETOOTH_DISCONNECT:
			BLUETOOTH.step = DF_BLUETOOTH_IDLE;
			BLUETOOTH_POWER_OFF();
			clear_bluetooth_timeout();
			DMT_PutStr(DF_DIS_BLUETOOTH_TEXT_ADDR,(u8 *)"已断开!  ");
			set_refresh_wnd_num(DF_PIC_BLUETOOTH_SET);
			break;
		case DF_KEY_VALUE_BLUETOOTH_SET_CLOSE:
			set_refresh_wnd_num(bluetooth_last_wnd);
			break;
	}
	key_task_start_set();	
	
	if(sample_pic_refresh_flag != 0)
	{
		sample_pic_refresh_flag = 0;	
		if(sample_pic_select == DF_SAMPLE_SELECT_SAMPLE)
		{
			if(sample_pic_channel == DF_SAMPLE_CHANNEL_A)
			{
				if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_IDLE)
				{
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_SET);
				}
				else if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_SAMPLING)
				{
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_RUN);
				}
				else if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_AUTOZERO)
				{
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_AUTOZERO);
				}
				else if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_CONTINUE_SAMPLING)
				{
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_AUTOZERO);
				}
				else if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_WAIT)
				{
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_WAIT);
				}
				else if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_INTERVAL)
				{
					set_refresh_wnd_num(DF_PIC_SAMPLE_A_INTERVAL);
				}
				else if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_ERR)
				{
					//增加继续采样菜单
					if(A_sample_file.success == DF_FAN_ERR)
					{
						set_refresh_wnd_num(DF_PIC_SAMPLE_A_ERR_FLOW);
					}
					if(A_sample_file.success == DF_JY_OUT)
					{
						set_refresh_wnd_num(DF_PIC_SAMPLE_A_ERR_JY);
					}
				}
				
			}
			if(sample_pic_channel == DF_SAMPLE_CHANNEL_B)
			{
				if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_IDLE)
				{
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_SET);
				}
				else if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_SAMPLING)
				{
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_RUN);
				}
				else if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_AUTOZERO)
				{
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_AUTOZERO);
				}
				else if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_CONTINUE_SAMPLING)
				{
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_AUTOZERO);
				}
				else if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_WAIT)
				{
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_WAIT);
				}
				else if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_INTERVAL)
				{
					set_refresh_wnd_num(DF_PIC_SAMPLE_B_INTERVAL);
				}
				else if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_ERR)
				{
					//增加继续采样菜单
					if(B_sample_file.success == DF_FAN_ERR)
					{
						set_refresh_wnd_num(DF_PIC_SAMPLE_B_ERR_FLOW);
					}
					if(B_sample_file.success == DF_JY_OUT)
					{
						set_refresh_wnd_num(DF_PIC_SAMPLE_B_ERR_JY);
					}
				}
			}
		}
		if(sample_pic_select == DF_SAMPLE_SELECT_DATA)
		{
			AB_sample_file.num = readintdata(ADDR_AB_sample_file_num);
			AB_sample_file_num_tmp = AB_sample_file.num;
			AB_sample_file.fg_flag = R_24(ADDR_AB_sample_file_fg_flag);
			if((AB_sample_file.fg_flag == 0) && (AB_sample_file.num == 0))
			{
				set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_NO_FILE);
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_SAMPLE_DATA);
			}
		}
		if(sample_pic_select == DF_SAMPLE_SELECT_DATA_POWER)
		{
			AB_power_lose_file.num = readintdata(ADDR_AB_power_lose_file_num);
			AB_power_lose_file_num_tmp = AB_power_lose_file.num;
			AB_power_lose_file.fg_flag = R_24(ADDR_AB_power_lose_file_fg_flag);
			if((AB_power_lose_file.fg_flag == 0) && (AB_power_lose_file.num == 0))
			{
				set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_POWER_NO_FILE);
			}
			else
			{
				set_refresh_wnd_num(DF_PIC_SAMPLE_DATA_POWER);
			}
		}
		if(sample_pic_select == DF_SAMPLE_SELECT_USB)
		{
			set_refresh_wnd_num(DF_PIC_SAMPLE_USB);
		}
		if(sample_pic_select == DF_SAMPLE_SELECT_PRINT)
		{
			set_refresh_wnd_num(DF_PIC_SAMPLE_PRINT);
		}
	}
	
	if(key != DF_KEY_IDLE)
	{
		sample_channel_and_select_refresh();
	}
	key = DF_KEY_IDLE;
}

void parameter_input_task(void)
{
	
}
u8 DMT_error_flash = 0;
void DS1302_task(void)
{
	char cbuf[10];
	char strbuf[30];
	static u8 next = 0;
	if(DS1302_DELAY.flag == 1)
	{
		DS1302_DELAY.flag = 0;
		if(RTC_select == DF_RTC_MAIN_BOARD)
		{
			my_RTC_GetTime();
		}
		else
		{
			DMT_GetRTC();
			sys_year = DMT_year;
			sys_month = DMT_month;
			sys_day = DMT_day;
			sys_hour = DMT_hour;
			sys_min = DMT_min;
			sys_sec = DMT_sec;
		}
		
		DMT_ScreenCbuf(DF_DIS_SYS_YEAR_ADDR,2,sys_year);
		DMT_ScreenCbuf(DF_DIS_SYS_MONTH_ADDR,2,sys_month);
		DMT_ScreenCbuf(DF_DIS_SYS_DAY_ADDR,2,sys_day);
		DMT_ScreenCbuf(DF_DIS_SYS_HOUR_ADDR,2,sys_hour);
		DMT_ScreenCbuf(DF_DIS_SYS_MIN_ADDR,2,sys_min);
		DMT_ScreenCbuf(DF_DIS_SYS_SEC_ADDR,2,sys_sec);	
		strcpy(strbuf,(char *)"20");
		CharToStr(2,(u8 *)cbuf,sys_year);
		strcat(strbuf,cbuf);
		strcat(strbuf,(char *)"年");
		CharToStr(2,(u8 *)cbuf,sys_month);
		strcat(strbuf,cbuf);
		strcat(strbuf,(char *)"月");
		CharToStr(2,(u8 *)cbuf,sys_day);
		strcat(strbuf,cbuf);
		strcat(strbuf,(char *)"日");
		DMT_PutStr(DF_SYS_DATE_MSG_ADDR,(u8 *)strbuf);
        if(DMT_error >= 10)
        {
            DMT_error_flash++;
            DMT_error_flash %= 2;
            if(DMT_error_flash == 0)
            {
                DMT_PutStr(DF_SYS_DATE_MSG_ADDR,(u8 *)"显示屏挤压!   ");
            }
            else
            {
            
            }
        }		
		CharToStr(2,(u8 *)strbuf,sys_hour);
		strcat(strbuf,(char *)"时");
		CharToStr(2,(u8 *)cbuf,sys_min);
		strcat(strbuf,cbuf);
		strcat(strbuf,(char *)"分");
		CharToStr(2,(u8 *)cbuf,sys_sec);
		strcat(strbuf,cbuf);
		strcat(strbuf,(char *)"秒");
		DMT_PutStr(DF_SYS_TIME_MSG_ADDR,(u8 *)strbuf);

#ifdef ZR3500TWO
		next++;
		if(next > 10){next = 0;}
        strcpy(strbuf,(char *)"      ");
        DMT_PutStr(DF_SYS_WK_MSG_ADDR,(u8 *)strbuf);
        strcpy(strbuf,(char *)"      ");
        DMT_PutStr(DF_SYS_ENVIRONMENT_TEMPERATURE_ADDR,(u8 *)strbuf);
#endif
#ifdef ZR3500TWO_HEAT
		next++;
		if(next > 10){next = 0;}
		if(next < 5)
		{
			if(wk_temperature_err != 1)
			{
				strcpy(strbuf,(char *)"保温箱 =");
				SPutf(sf31,(u8 *)cbuf,wk_temperature);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char *)"℃");
			}
			else
			{
				strcpy(strbuf,(char *)"保温箱 = xx.x℃");
			}
			DMT_PutStr(DF_SYS_WK_MSG_ADDR,(u8 *)strbuf);
		}
		else
		{
			if(POWER != 0)
			{
				if(wk_enable != 0)
				{
					DMT_PutStr(DF_SYS_WK_MSG_ADDR,(u8 *)"保温箱未供电!  ");
				}
				else
				{
					next = 0;
				}
			}
			else
			{
				next = 0;
			}
		}
		
		strcpy(strbuf,(char *)"环温=");
		SPutf(sf31,(u8 *)cbuf,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].value);
		strcat(strbuf,cbuf);
		strcat(strbuf,(char *)"℃");
		DMT_PutStr(DF_SYS_ENVIRONMENT_TEMPERATURE_ADDR,(u8 *)strbuf);
#endif
#ifdef ZR3500TWO_HEATandCOOL
		next++;
		if(next > 10){next = 0;}
		if(next < 5)
		{
			if(wk_temperature_err != 1)
			{
				strcpy(strbuf,(char *)"保温箱 =");
				SPutf(sf31,(u8 *)cbuf,wk_temperature);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char *)"℃");
			}
			else
			{
				strcpy(strbuf,(char *)"保温箱 = xx.x℃");
			}
			DMT_PutStr(DF_SYS_WK_MSG_ADDR,(u8 *)strbuf);
		}
		else
		{
			if(POWER != 0)
			{
				if(wk_enable != 0)
				{
					DMT_PutStr(DF_SYS_WK_MSG_ADDR,(u8 *)"保温箱未供电!  ");
				}
				else
				{
					next = 0;
				}
			}
			else
			{
				next = 0;
			}
		}
		
		strcpy(strbuf,(char *)"环温=");
		SPutf(sf31,(u8 *)cbuf,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].value);
		strcat(strbuf,cbuf);
		strcat(strbuf,(char *)"℃");
		DMT_PutStr(DF_SYS_ENVIRONMENT_TEMPERATURE_ADDR,(u8 *)strbuf);
#endif
	}
}

void calc_flow_filter(struct FLOW_DIS * flow_struct,float flow_buf)
{
	u8 i;
	float fbuf;
	(* flow_struct).i++;
	if((* flow_struct).i > 9)
	{
		(* flow_struct).i = 0;
	}
	(* flow_struct).buf[(* flow_struct).i] = flow_buf;
	fbuf = 0;
	for(i = 0 ; i <= 9 ; i++)
	{
		fbuf += (* flow_struct).buf[i];
	}
	(* flow_struct).value = fbuf / (float)10.0;
}
void flow_filter_init(struct FLOW_DIS * flow_struct)
{
	u8 i;
	(* flow_struct).i = 0;
	for(i = 0; i <= 9; i++)
	{
		(* flow_struct).buf[i] = 0;
	}
}
u16 bluetooth_select_location[4] = {688,170,704,194};
u16 bluetooth_select_location2[4] = {716,170,732,194};
u16 sec_test = 0;
void display_sharing(void)
{
	if(REFRESH_SHARING_DELAY.flag == 1)
	{
		REFRESH_SHARING_DELAY.flag = 0;
DMT_CmdReadTPStatus();
/*
DMT_ScreenIbuf(DF_BIAODING_B_FLOW_RATE_ADDR,5,main_i);
main_i = 0;

DMT_ScreenIbuf(DF_TEST_2_ADDR,5,sec_test);
sec_test++;
*/
//流量显示滑动滤波值(仅在采样时计算，也可以实现在泵运行时计算，仅需改成泵启动标志即可可能更好)
		if(PUMP_CTR_TASK_A.enable != 0)
		{
			calc_flow_filter(&A_flow_dis,A_SAMPLE.flow_ctr);
			DMT_ScreenFbuf(DF_DIS_A_FLOW_ADDR,f32,A_flow_dis.value);
		}
		else
		{
			flow_filter_init(&A_flow_dis);
			calc_flow_filter(&A_flow_dis,0);
			DMT_ScreenFbuf(DF_DIS_A_FLOW_ADDR,f32,A_flow_dis.value);
		}
		if(PUMP_CTR_TASK_B.enable != 0)
		{
			calc_flow_filter(&B_flow_dis,B_SAMPLE.flow_ctr);
			DMT_ScreenFbuf(DF_DIS_B_FLOW_ADDR,f32,B_flow_dis.value);
		}
		else
		{
			flow_filter_init(&B_flow_dis);
			calc_flow_filter(&B_flow_dis,0);
			DMT_ScreenFbuf(DF_DIS_B_FLOW_ADDR,f32,B_flow_dis.value);
		}	
		
		DMT_ScreenFbuf(DF_DIS_A_DY_ADDR,sf40,A_SAMPLE.dy);
		DMT_ScreenFbuf(DF_DIS_B_DY_ADDR,sf40,B_SAMPLE.dy);
		DMT_ScreenFbuf(DF_DIS_A_JY_ADDR,sf42,A_SAMPLE.jy);
		DMT_ScreenFbuf(DF_DIS_B_JY_ADDR,sf42,B_SAMPLE.jy);

		DMT_ScreenFbuf(DF_DIS_A_VOLUME_ADDR,f62,A_SAMPLE.volume);
		DMT_ScreenFbuf(DF_DIS_A_NORMAL_VOLUME_ADDR,f62,A_SAMPLE.volume_biaokuang);
		DMT_ScreenCbuf(DF_DIS_A_HOUR_LEAVE_ADDR,2,A_SAMPLE.running_time / 3600);
		DMT_ScreenCbuf(DF_DIS_A_MIN_LEAVE_ADDR,2,(A_SAMPLE.running_time % 3600) / 60);
		DMT_ScreenCbuf(DF_DIS_A_SEC_LEAVE_ADDR,2,A_SAMPLE.running_time % 60);
		DMT_ScreenCbuf(DF_DIS_A_COUNT_ADDR,2,A_SAMPLE.sample_times + 1);
		DMT_ScreenCbuf(DF_DIS_A_INTERVAL_LEAVING_HOUR,2,A_SAMPLE.jiange_time / 3600);
		DMT_ScreenCbuf(DF_DIS_A_INTERVAL_LEAVING_MIN,2,(A_SAMPLE.jiange_time % 3600) / 60);
		DMT_ScreenCbuf(DF_DIS_A_INTERVAL_LEAVING_SEC,2,A_SAMPLE.jiange_time % 60);
		DMT_ScreenCbuf(DF_DIS_A_INTERVAL_SAMPLING_COUNT,2,A_SAMPLE.sample_times);
		
		DMT_ScreenFbuf(DF_DIS_B_VOLUME_ADDR,f62,B_SAMPLE.volume);
		DMT_ScreenFbuf(DF_DIS_B_NORMAL_VOLUME_ADDR,f62,B_SAMPLE.volume_biaokuang);
		DMT_ScreenCbuf(DF_DIS_B_HOUR_LEAVE_ADDR,2,B_SAMPLE.running_time / 3600);
		DMT_ScreenCbuf(DF_DIS_B_MIN_LEAVE_ADDR,2,(B_SAMPLE.running_time % 3600) / 60);
		DMT_ScreenCbuf(DF_DIS_B_SEC_LEAVE_ADDR,2,B_SAMPLE.running_time % 60);
		DMT_ScreenCbuf(DF_DIS_B_COUNT_ADDR,2,B_SAMPLE.sample_times + 1);
		DMT_ScreenCbuf(DF_DIS_B_INTERVAL_LEAVING_HOUR,2,B_SAMPLE.jiange_time / 3600);
		DMT_ScreenCbuf(DF_DIS_B_INTERVAL_LEAVING_MIN,2,(B_SAMPLE.jiange_time % 3600) / 60);
		DMT_ScreenCbuf(DF_DIS_B_INTERVAL_LEAVING_SEC,2,B_SAMPLE.jiange_time % 60);
		DMT_ScreenCbuf(DF_DIS_B_INTERVAL_SAMPLING_COUNT,2,B_SAMPLE.sample_times);

		if(sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode == DF_INPUT_MODE)
		{
			DMT_ScreenFbuf(DF_DIS_ENVIRONMENT_TEMPERATURE_ADDR,sf31,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].input);
		}
		else
		{
			DMT_ScreenFbuf(DF_DIS_ENVIRONMENT_TEMPERATURE_ADDR,sf31,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].value);
		}
		if(barometric.mode == DF_INPUT_MODE)
		{
			DMT_ScreenFbuf(DF_DIS_BAR_ADDR,f52,barometric.input);
		}
		else
		{
			DMT_ScreenFbuf(DF_DIS_BAR_ADDR,f52,barometric.value);
		}	
		DMT_ScreenFbuf(DF_DIS_REAL_BAR_ADDR,f52,barometric.value);
		DMT_ScreenFbuf(DF_DIS_REAL_ENVIRONMENT_TEMPERATURE_ADDR,sf31,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].value);
		DMT_ScreenFbuf(DF_DIS_A_TEMPERATURE_ADDR,sf31,sample_temperature[DF_A_TEMPERATURE].value);
		DMT_ScreenFbuf(DF_DIS_B_TEMPERATURE_ADDR,sf31,sample_temperature[DF_B_TEMPERATURE].value);
		DMT_ScreenFbuf(DF_DIS_WK_TEMPERATURE_ADDR,sf31,wk_temperature);
		
		if(BLUETOOTH.connected_flag == DF_STATUS_BLUETOOTH_CONNECTED)
		{
			DMT_CutPicture(8,452,DF_DIS_BLUETOOTH_STATUS_ADDR,DF_PIC_ICON_3,bluetooth_select_location2);
		}
		else
		{
			DMT_CutPicture(8,452,DF_DIS_BLUETOOTH_STATUS_ADDR,DF_PIC_ICON_3,bluetooth_select_location);
		}
        bat_symbol_display();
		chatou_symbol_display();
	}
}
//开机时初始化所有的静态变量，不再每秒都刷新
void DMT_refresh_para(void)
{
	DMT_ScreenFbuf(DF_A_FLOW_SET_ADDR,f21,A_SAMPLE.flow_set);
	DMT_ScreenFbuf(DF_A_VOLUME_SET_ADDR,f51,A_SAMPLE.sample_volume_set);
	DMT_ScreenCbuf(DF_A_HOUR_SET_ADDR,2,A_SAMPLE.running_time_set / 3600);
	DMT_ScreenCbuf(DF_A_MIN_SET_ADDR,2,(A_SAMPLE.running_time_set % 3600)/60);
	DMT_ScreenCbuf(DF_A_START_HOUR_SET_ADDR,2,A_SAMPLE.start_hour_set);
	DMT_ScreenCbuf(DF_A_START_MIN_SET_ADDR,2,A_SAMPLE.start_min_set);
	DMT_ScreenCbuf(DF_AB_START_SEC_ADDR,2,0);
	DMT_ScreenCbuf(DF_A_INTERVAL_HOUR_SET_ADDR,2,A_SAMPLE.jiange_time_set / 3600);
	DMT_ScreenCbuf(DF_A_INTERVAL_MIN_SET_ADDR,2,(A_SAMPLE.jiange_time_set % 3600)/60);
	DMT_ScreenCbuf(DF_DIS_A_COUNT_SET_ADDR,2,A_SAMPLE.sample_times_set);
	DMT_ScreenFbuf(DF_B_FLOW_SET_ADDR,f21,B_SAMPLE.flow_set);
	DMT_ScreenFbuf(DF_B_VOLUME_SET_ADDR,f51,B_SAMPLE.sample_volume_set);
	DMT_ScreenCbuf(DF_B_HOUR_SET_ADDR,2,B_SAMPLE.running_time_set / 3600);
	DMT_ScreenCbuf(DF_B_MIN_SET_ADDR,2,(B_SAMPLE.running_time_set % 3600)/60);
	DMT_ScreenCbuf(DF_B_START_HOUR_SET_ADDR,2,B_SAMPLE.start_hour_set);
	DMT_ScreenCbuf(DF_B_START_MIN_SET_ADDR,2,B_SAMPLE.start_min_set);
	DMT_ScreenCbuf(DF_B_INTERVAL_HOUR_SET_ADDR,2,B_SAMPLE.jiange_time_set / 3600);
	DMT_ScreenCbuf(DF_B_INTERVAL_MIN_SET_ADDR,2,(B_SAMPLE.jiange_time_set % 3600)/60);
	DMT_ScreenCbuf(DF_DIS_B_COUNT_SET_ADDR,2,B_SAMPLE.sample_times_set);
}

u8 power_lose_yes_timeout = 10;
void display_power_lose(void)
{
	switch (wnd_struct.current)
	{
		case DF_PIC_POWER_LOSE:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
				DMT_ScreenCbuf(DF_POWER_LOSE_SEC_COUNT_ADDR,2,power_lose_yes_timeout);
				timer_init(&REFRESH_DELAY,20);
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
				if(power_lose_yes_timeout == 0)
				{
					power_lose_yes_timeout = 10;
					key = DF_KEY_VALUE_POWER_LOSE_YES;				//超时则自动产生key，默认选中继续采样
					break;
				}
				if(power_lose_yes_timeout > 0)
				{
					power_lose_yes_timeout--;
				}
				DMT_ScreenCbuf(DF_POWER_LOSE_SEC_COUNT_ADDR,2,power_lose_yes_timeout);
			}
			break;
	}
}



void display_home(void)
{
	if(wnd_struct.current == DF_PIC_HOME)
	{
		if(wnd_struct.change_pic == 1)
		{
			wnd_struct.change_pic = 0;
			DMT_ChangePic(wnd_struct.current);
		}
		if(REFRESH_DELAY.flag == 1)
		{
			REFRESH_DELAY.flag = 0;	//每秒刷新一次
			
		}
	}
	else
	{

	}
}

void display_sleep_init(void)
{
	if(display_current_pic == DF_PIC_SLEEP)
	{
		DMT_ChangePic(display_last_pic);
		DMT_SetBackLight(0x40);
	}
	back_light_min = 0;
	timer_init(&BACKLIGHT_DELAY,1200);
}

void display_sleep(void)
{
	if(back_light_min_set < 1)
	{
		return;						//常开
	}
	if(back_light_min < back_light_min_set)
	{
		if(BACKLIGHT_DELAY.flag == 1)
		{
			BACKLIGHT_DELAY.flag = 0;
			if(back_light_min < back_light_min_set)
			{
				back_light_min ++;
			}
			if(back_light_min >= back_light_min_set)
			{
				DMT_ChangePic(DF_PIC_SLEEP);
				DMT_SetBackLight(0);
				timer_init(&BACKLIGHT_DELAY,2);
			}
		}
	}
	else
	{
		if(BACKLIGHT_DELAY.flag == 1)
		{
			BACKLIGHT_DELAY.flag = 0;
		}
		if(display_current_pic != DF_PIC_SLEEP)
		{
			DMT_ChangePic(DF_PIC_SLEEP);
		}
	}
}
void bat_symbol_display(void)
{
    static u8 bat_disp = 0;
	if((charge_current > (float)30)||((POWER == 0)&&(bat_voltage < (float)15.7)))//正在充电中，动态显示电池充电
	{
	    bat_disp++;
		bat_disp %= 5;
	}
	else	   	 								  //静态显示电池电量
	{
	    if(bat_voltage > (float)15.6)
		{
			bat_disp = 4;
		}
		else if(bat_voltage > (float)14.9)
		{
			bat_disp = 3;
		}
		else if(bat_voltage > (float)14.4)
		{
			bat_disp = 2;
		}
		else if(bat_voltage > (float)14.0)
		{
			bat_disp = 1;
		}
		else
		{
			bat_disp = 0;
		}
//		else if(bat_voltage > 13.6)
//		{
//			bat_disp = 1;
//		}
//		else
//		{
//		    bat_disp++;
//			bat_disp %= 2;
//		}
	}
	DMT_ScreenChar(DF_BAT_SYMBOL_ADDR,bat_disp);
}
//---------插头---------------

void chatou_symbol_display(void)
{
	if(POWER == 0)
	{
		//显示交流电图标
		DMT_ScreenChar(DF_POWER_SYMBOL_ADDR,1);
	}
	else
	{
		//清除交流电图标
		DMT_ScreenChar(DF_POWER_SYMBOL_ADDR,0);
	}
}

void display_set(void)
{
	if(wnd_struct.current == DF_PIC_SET)
	{
		if(wnd_struct.change_pic == 1)
		{
			wnd_struct.change_pic = 0;
			DMT_ChangePic(wnd_struct.current);			
//LOOP:
#ifdef ZR3500TWO
            if(sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode == DF_INPUT_MODE)
			{
				DMT_CutPicture(673,102,DF_ENVIRONMENT_TEMPERATURE_MEASURE_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenFbuf(DF_DIS_ENVIRONMENT_TEMPERATURE_ADDR,sf31,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].input);
			}
			else
			{
				DMT_CutPicture(560,102,DF_ENVIRONMENT_TEMPERATURE_MEASURE_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenFbuf(DF_DIS_ENVIRONMENT_TEMPERATURE_ADDR,sf31,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].value);
			}
			if(barometric.mode == DF_INPUT_MODE)
			{
				DMT_CutPicture(673,154,DF_BAR_MEASURE_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenFbuf(DF_DIS_BAR_ADDR,f52,barometric.input);
			}
			else
			{
				DMT_CutPicture(560,154,DF_BAR_MEASURE_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenFbuf(DF_DIS_BAR_ADDR,f52,barometric.value);
			}
#endif
#ifdef ZR3500TWO_HEAT
            if(sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode == DF_INPUT_MODE)
			{
				DMT_CutPicture(673,81,DF_ENVIRONMENT_TEMPERATURE_MEASURE_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenFbuf(DF_DIS_ENVIRONMENT_TEMPERATURE_ADDR,sf31,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].input);
			}
			else
			{
				DMT_CutPicture(560,81,DF_ENVIRONMENT_TEMPERATURE_MEASURE_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenFbuf(DF_DIS_ENVIRONMENT_TEMPERATURE_ADDR,sf31,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].value);
			}
			if(barometric.mode == DF_INPUT_MODE)
			{
				DMT_CutPicture(673,134,DF_BAR_MEASURE_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenFbuf(DF_DIS_BAR_ADDR,f52,barometric.input);
			}
			else
			{
				DMT_CutPicture(560,134,DF_BAR_MEASURE_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenFbuf(DF_DIS_BAR_ADDR,f52,barometric.value);
			}
			if(wk_enable != 1)
			{
				DMT_CutPicture(673,192,DF_TEMPERATURE_CTR_ON_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenIbuf(DF_DIS_TEMPERATURE_CTR_1_SET_ADDR,2,wk_temperature_set);
			}
			else
			{
				DMT_CutPicture(560,192,DF_TEMPERATURE_CTR_ON_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenIbuf(DF_DIS_TEMPERATURE_CTR_1_SET_ADDR,2,wk_temperature_set);
			}
#endif
#ifdef ZR3500TWO_HEATandCOOL
			if(sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode == DF_INPUT_MODE)
			{
				DMT_CutPicture(673,81,DF_ENVIRONMENT_TEMPERATURE_MEASURE_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenFbuf(DF_DIS_ENVIRONMENT_TEMPERATURE_ADDR,sf31,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].input);
			}
			else
			{
				DMT_CutPicture(560,81,DF_ENVIRONMENT_TEMPERATURE_MEASURE_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenFbuf(DF_DIS_ENVIRONMENT_TEMPERATURE_ADDR,sf31,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].value);
			}
			if(barometric.mode == DF_INPUT_MODE)
			{
				DMT_CutPicture(673,134,DF_BAR_MEASURE_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenFbuf(DF_DIS_BAR_ADDR,f52,barometric.input);
			}
			else
			{
				DMT_CutPicture(560,134,DF_BAR_MEASURE_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenFbuf(DF_DIS_BAR_ADDR,f52,barometric.value);
			}
			if(wk_enable != 1)
			{
				DMT_CutPicture(673,192,DF_TEMPERATURE_CTR_ON_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenIbuf(DF_DIS_TEMPERATURE_CTR_1_SET_ADDR,2,wk_temperature_set);
			}
			else
			{
				DMT_CutPicture(560,192,DF_TEMPERATURE_CTR_ON_CUT,DF_PIC_ICON_3,select_location);
				DMT_ScreenIbuf(DF_DIS_TEMPERATURE_CTR_1_SET_ADDR,2,wk_temperature_set);
			}
#endif            
//			if(sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode == DF_INPUT_MODE)
//			{
//				DMT_CutPicture(673,81,DF_ENVIRONMENT_TEMPERATURE_MEASURE_CUT,DF_PIC_ICON_3,select_location);
//				DMT_ScreenFbuf(DF_DIS_ENVIRONMENT_TEMPERATURE_ADDR,sf31,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].input);
//			}
//			else
//			{
//				DMT_CutPicture(560,81,DF_ENVIRONMENT_TEMPERATURE_MEASURE_CUT,DF_PIC_ICON_3,select_location);
//				DMT_ScreenFbuf(DF_DIS_ENVIRONMENT_TEMPERATURE_ADDR,sf31,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].value);
//			}
//			if(barometric.mode == DF_INPUT_MODE)
//			{
//				DMT_CutPicture(673,134,DF_BAR_MEASURE_CUT,DF_PIC_ICON_3,select_location);
//				DMT_ScreenFbuf(DF_DIS_BAR_ADDR,f52,barometric.input);
//			}
//			else
//			{
//				DMT_CutPicture(560,134,DF_BAR_MEASURE_CUT,DF_PIC_ICON_3,select_location);
//				DMT_ScreenFbuf(DF_DIS_BAR_ADDR,f52,barometric.value);
//			}
//			if(wk_enable != 1)
//			{
//				DMT_CutPicture(673,192,DF_TEMPERATURE_CTR_ON_CUT,DF_PIC_ICON_3,select_location);
//				DMT_ScreenIbuf(DF_DIS_TEMPERATURE_CTR_1_SET_ADDR,2,wk_temperature_set);
//			}
//			else
//			{
//				DMT_CutPicture(560,192,DF_TEMPERATURE_CTR_ON_CUT,DF_PIC_ICON_3,select_location);
//				DMT_ScreenIbuf(DF_DIS_TEMPERATURE_CTR_1_SET_ADDR,2,wk_temperature_set);
//			}
			DMT_ScreenIbuf(DF_BACK_LIGHT_SET_ADDR,3,back_light_min_set);
		}
		if(REFRESH_DELAY.flag == 1)
		{
			REFRESH_DELAY.flag = 0;	//每秒刷新一次
		}
	}
	else
	{

	}
}
void display_bluetooth_set(void)
{
	switch (wnd_struct.current)
	{
		case DF_PIC_BLUETOOTH_SET:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
				DMT_ScreenIbuf(DF_DIS_BLUETOOTH_CLOSE_TIME_ADDR,3,BLUETOOTH.close_time_set);
				DMT_ScreenIbuf(DF_DIS_BLUETOOTH_CLOSE_TIME_COUNT_ADDR,3,BLUETOOTH.close_time_set - BLUETOOTH.timeout);
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次
				DMT_ScreenIbuf(DF_DIS_BLUETOOTH_CLOSE_TIME_ADDR,3,BLUETOOTH.close_time_set);
				if(BLUETOOTH.close_time_set >= BLUETOOTH.timeout)
				{
					DMT_ScreenIbuf(DF_DIS_BLUETOOTH_CLOSE_TIME_COUNT_ADDR,3,BLUETOOTH.close_time_set - BLUETOOTH.timeout);	
				}
				else
				{
					DMT_ScreenIbuf(DF_DIS_BLUETOOTH_CLOSE_TIME_COUNT_ADDR,3,0);
				}
				if(BLUETOOTH.connected_flag == DF_STATUS_BLUETOOTH_CONNECTED)
				{
					DMT_PutStr(DF_DIS_BLUETOOTH_TEXT_ADDR,(u8 *)"已连接!  ");
				}
				else
				{
					if(BLUETOOTH.step != DF_BLUETOOTH_IDLE)
					{
						DMT_PutStr(DF_DIS_BLUETOOTH_TEXT_ADDR,(u8 *)"连接中...");
					}
					else
					{
						DMT_PutStr(DF_DIS_BLUETOOTH_TEXT_ADDR,(u8 *)"已断开!  ");
					}
				}
			}
			break;
	}
}

u16 select_location_to_start[]={292,14,326,47};
u16 not_select_location_to_start[]={0,0,1,1};
void display_sample(void)
{
	switch (wnd_struct.current)
	{
		case DF_PIC_SAMPLE_A_SET:
		case DF_PIC_SAMPLE_A_SET_VOLUME:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				if(SAMPLE_MODE.channel_A == DF_SAMPLE_MODE_BY_TIME)
				{
					DMT_ChangePic(DF_PIC_SAMPLE_A_SET);
				}
				else
				{
					DMT_ChangePic(DF_PIC_SAMPLE_A_SET_VOLUME);
				}
				DMT_refresh_para();
			}
			break;
		case DF_PIC_SAMPLE_B_SET:
        case DF_PIC_SAMPLE_B_SET_VOLUME:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				if(SAMPLE_MODE.channel_B == DF_SAMPLE_MODE_BY_TIME)
				{
					DMT_ChangePic(DF_PIC_SAMPLE_B_SET);
				}
				else
				{
					DMT_ChangePic(DF_PIC_SAMPLE_B_SET_VOLUME);
				}
				DMT_refresh_para();
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
			}
			break;
		case DF_PIC_SAMPLE_A_RUN:
		case DF_PIC_SAMPLE_B_RUN:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
			}
			break;

		case DF_PIC_SAMPLE_A_WAIT:	
        case DF_PIC_SAMPLE_A_WAIT_VOLUME:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				if(SAMPLE_MODE.channel_A == DF_SAMPLE_MODE_BY_TIME)
				{
					DMT_ChangePic(DF_PIC_SAMPLE_A_WAIT);
				}
				else
				{
					DMT_ChangePic(DF_PIC_SAMPLE_A_WAIT_VOLUME);
				}
				DMT_refresh_para();
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
			}
			break;
		case DF_PIC_SAMPLE_B_WAIT:			
		case DF_PIC_SAMPLE_B_WAIT_VOLUME:
		    if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				if(SAMPLE_MODE.channel_B == DF_SAMPLE_MODE_BY_TIME)
				{
					DMT_ChangePic(DF_PIC_SAMPLE_B_WAIT);
				}
				else
				{
					DMT_ChangePic(DF_PIC_SAMPLE_B_WAIT_VOLUME);
				}
				DMT_refresh_para();
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
			}
			break;
		
		case DF_PIC_SAMPLE_A_INTERVAL:
		case DF_PIC_SAMPLE_A_INTERVAL_VOLUME:
            if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				if(SAMPLE_MODE.channel_A == DF_SAMPLE_MODE_BY_TIME)
				{
					DMT_ChangePic(DF_PIC_SAMPLE_A_INTERVAL);
				}
				else
				{
					DMT_ChangePic(DF_PIC_SAMPLE_A_INTERVAL_VOLUME);
				}
				DMT_refresh_para();
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
			}
			break;
		
		case DF_PIC_SAMPLE_B_INTERVAL:	
		case DF_PIC_SAMPLE_B_INTERVAL_VOLUME:
		    if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				if(SAMPLE_MODE.channel_B == DF_SAMPLE_MODE_BY_TIME)
				{
					DMT_ChangePic(DF_PIC_SAMPLE_B_INTERVAL);
				}
				else
				{
					DMT_ChangePic(DF_PIC_SAMPLE_B_INTERVAL_VOLUME);
				}
				DMT_refresh_para();
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
			}
			break;
		
		case DF_PIC_SAMPLE_A_AUTOZERO:	
		case DF_PIC_SAMPLE_B_AUTOZERO:
		case DF_PIC_SAMPLE_A_STOP_OR_NOT:
		case DF_PIC_SAMPLE_B_STOP_OR_NOT:
        case DF_PIC_SAMPLE_AB_STOP_OR_NOT:
		case DF_PIC_SAMPLE_A_ERR_FLOW:
		case DF_PIC_SAMPLE_A_ERR_JY:
		case DF_PIC_SAMPLE_B_ERR_FLOW:
		case DF_PIC_SAMPLE_B_ERR_JY:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
			}
			break;			
		case DF_PIC_COPY_TO_B_ERR:
		case DF_PIC_A_TO_START_ERR:
		case DF_PIC_B_TO_START_ERR:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
				timer_init(&EMPLOY_DELAY,20);
			}
			if(EMPLOY_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//延时1秒返回
				set_refresh_last_wnd_num();
			}
			break;
	}
}
u16 select_location_AC[4] = {167,13,203,45};
u16 select_location_BD[4] = {290,13,326,45};
u16 select_no[4] = {0,0,1,1};
u16 select_location_format[4] = {292,15,324,44};
u16 select_location_flow_ctr[4] = {60,15,95,44};
u8 reset_sys_count = 0;
void display_maintenance(void)
{
	switch (wnd_struct.current)
	{
		case DF_PIC_MAINTENANCE:
		case DF_PIC_PRESSURE_BIAODING:
		case DF_PIC_TEMPERATURE_BIAODING:
		case DF_PIC_FLOW_BIAODING:
		case DF_PIC_DELETE_ALL_FILES:
		case DF_PIC_RESET_SYS:
		case DF_PIC_PASSWORD_SET:
		case DF_PIC_FILE_FORMAT:
		case DF_PIC_PRINT_SET:
		case DF_PIC_EXIT_MAINTENANCE:
		case DF_PIC_OTHER_SET:
		case DF_PIC_FLOW_CTR:
		case DF_PIC_RTC_SELECT:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
				supper_password = 0;				//超级密码输入
				if(wnd_struct.current != DF_PIC_FLOW_BIAODING)
				{
					stop_ctr_A_pump_task();
					stop_ctr_B_pump_task();
				}
				DMT_ScreenFbuf(DF_A_DY_RATE_ADDR,f54,A_SAMPLE.dy_rate);
				DMT_ScreenFbuf(DF_A_DY_ZERO_ADDR,f40,A_SAMPLE.dy_zero);
				DMT_ScreenFbuf(DF_A_JY_RATE_ADDR,f54,A_SAMPLE.jy_rate);
				DMT_ScreenFbuf(DF_A_JY_ZERO_ADDR,f52,A_SAMPLE.jy_zero);
				DMT_ScreenFbuf(DF_B_DY_RATE_ADDR,f54,B_SAMPLE.dy_rate);
				DMT_ScreenFbuf(DF_B_DY_ZERO_ADDR,f40,B_SAMPLE.dy_zero);
				DMT_ScreenFbuf(DF_B_JY_RATE_ADDR,f54,B_SAMPLE.jy_rate);
				DMT_ScreenFbuf(DF_B_JY_ZERO_ADDR,f52,B_SAMPLE.jy_zero);	
				DMT_ScreenFbuf(DF_BAR_RATE_ADDR,f54,barometric.rate);
				DMT_ScreenFbuf(DF_BAR_ZERO_ADDR,f52,barometric.zero);
				DMT_ScreenFbuf(DF_TEMPERATURE_ZERO_ADDR,f41,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].zero);
				DMT_ScreenFbuf(DF_A_TEMPERATURE_ZERO_ADDR,f41,sample_temperature[DF_A_TEMPERATURE].zero);
				DMT_ScreenFbuf(DF_B_TEMPERATURE_ZERO_ADDR,f41,sample_temperature[DF_B_TEMPERATURE].zero);
				DMT_ScreenFbuf(DF_CTR_TEMPERATURE_ZERO_ADDR,f41,sample_temperature[DF_CTR_TEMPERATURE].zero);
				
				if(PUMP_CTR_TASK_A.enable != 1)
				{
					DMT_CutPicture(0,0,DF_BIAODING_START_A_CUT,DF_PIC_FLOW_BIAODING,select_no);
				}
				else
				{
					DMT_CutPicture(94,109,DF_BIAODING_START_A_CUT,DF_PIC_ICON_3,select_location_AC);
				}
				if(PUMP_CTR_TASK_B.enable != 1)
				{
					DMT_CutPicture(0,0,DF_BIAODING_START_B_CUT,DF_PIC_FLOW_BIAODING,select_no);
				}
				else
				{
					DMT_CutPicture(94,230,DF_BIAODING_START_B_CUT,DF_PIC_ICON_3,select_location_BD);
				}

				DMT_ScreenFbuf(DF_BIAODING_A_FLOW_SET_ADDR,f21,A_SAMPLE.flow_set);
				DMT_ScreenFbuf(DF_BIAODING_B_FLOW_SET_ADDR,f21,B_SAMPLE.flow_set);

				DMT_ScreenFbuf(DF_BIAODING_A_FLOW_RATE_ADDR,f43,point_rate_A_flow(A_SAMPLE.flow_set));
				DMT_ScreenFbuf(DF_BIAODING_B_FLOW_RATE_ADDR,f43,point_rate_B_flow(B_SAMPLE.flow_set));
                
				DMT_ScreenFbuf(DF_DIS_KEDU_FLOW_TEMPERATURE_SET,f20,kedu_temperature_set);
				DMT_PutStr(DF_BLUETOOTH_RESET_MSG_ADDR,(u8 *)"     ");		//清空显示区
				DMT_PutStr(DF_PRINTER_PASSWORD_MSG_ADDR,(u8 *)"     ");		//清空显示区
				DMT_ScreenIbuf(DF_PRINTER_PASSWORD_ADDR,4,printer_password);
				DMT_ScreenIbuf(DF_DIS_PASSWORD_ADDR,4,password);
				if(file_format == 0)
				{
					DMT_CutPicture(497,215,DF_FILE_FORMAT_CUT,DF_PIC_ICON_3,select_location_format);
				}
				else
				{
					DMT_CutPicture(497,292,DF_FILE_FORMAT_CUT,DF_PIC_ICON_3,select_location_format);
				}
				if(A_SAMPLE.flow_ctr_select == DF_FLOW_CTR_KEDU)
				{
					DMT_CutPicture(466,201,DF_FLOW_CTR_CUT_ADDR,DF_PIC_ICON_3,select_location_flow_ctr);
				}
				else if(A_SAMPLE.flow_ctr_select == DF_FLOW_CTR_RUKOU)
				{
					DMT_CutPicture(466,255,DF_FLOW_CTR_CUT_ADDR,DF_PIC_ICON_3,select_location_flow_ctr);
				}
				else if(A_SAMPLE.flow_ctr_select == DF_FLOW_CTR_BIAOKUANG)
				{
					DMT_CutPicture(466,310,DF_FLOW_CTR_CUT_ADDR,DF_PIC_ICON_3,select_location_flow_ctr);
				}
				else
				{
					DMT_CutPicture(466,201,DF_FLOW_CTR_CUT_ADDR,DF_PIC_ICON_3,select_location_flow_ctr);
				}
				if(RTC_select == DF_RTC_MAIN_BOARD)
				{
					DMT_CutPicture(440,178,DF_RTC_SELECT_CUT_ADDR,DF_PIC_ICON_3,select_location_format);
				}
				else
				{
					DMT_CutPicture(440,253,DF_RTC_SELECT_CUT_ADDR,DF_PIC_ICON_3,select_location_format);
				}
			
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
				DMT_ScreenIbuf(DF_BIAODING_A_PWM_ADDR,5,TIM_GetCapture4(TIM4));
				DMT_ScreenIbuf(DF_BIAODING_B_PWM_ADDR,5,TIM_GetCapture3(TIM4));
			}
			break;
		case DF_PIC_WAIT_DELETE_FILES:
		case DF_PIC_WAIT_RESET:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
				if(wnd_struct.current == DF_PIC_WAIT_DELETE_FILES)
				{
					DeleteAllFiles();
					AB_sample_file.fg_flag = 0;
					AB_sample_file.num = 0;
					AB_power_lose_file.fg_flag = 0;
					AB_power_lose_file.num = 0;
					W_24(ADDR_AB_sample_file_fg_flag,AB_sample_file.fg_flag);
					saveintdata(ADDR_AB_sample_file_num,AB_sample_file.num);
					W_24(ADDR_AB_power_lose_file_fg_flag,AB_power_lose_file.fg_flag);
					saveintdata(ADDR_AB_power_lose_file_num,AB_power_lose_file.num);
					DMT_ChangePic(DF_PIC_DELETE_FILES_OK);
					timer_init(&REFRESH_DELAY,30);			//1.5秒延时
					
				}
				else if(wnd_struct.current == DF_PIC_WAIT_RESET)
				{
					if(R_24(ADDR_bak_flag) == 1)
					{
FORCE_RESET:
						ReadPara(DF_OFFSET);
						SavePara(0);
						reset_para();
						DMT_ChangePic(DF_PIC_RESET_SYS_OK);
						timer_init(&REFRESH_DELAY,30);			//1.5秒延时
					}
					else
					{
						reset_sys_count++;
						if(reset_sys_count > 10)
						{
							reset_sys_count = 0;
							goto FORCE_RESET;
						}
						DMT_ChangePic(DF_PIC_RESET_SYS_ERR);
						timer_init(&REFRESH_DELAY,30);			//1.5秒延时
					}
				}
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//延时时间到
				set_refresh_wnd_num(DF_PIC_OTHER_SET);
			}
			break;
	}
}

void display_supper_maintenance(void)
{
	switch (wnd_struct.current)
	{
		case DF_PIC_SUPPER_SYS_SET:
		case DF_PIC_SUPPER_SYS_BACKUP:
		case DF_PIC_SUPPER_BIANHAO:

		case DF_PIC_SUPPER_PID_SET:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
				
				DMT_ScreenIbuf(DF_A_START_PWM_ADDR,5,A_PWM_01L);
				DMT_ScreenIbuf(DF_B_START_PWM_ADDR,5,B_PWM_01L);
				DMT_ScreenFbuf(DF_BAT_VOLTAGE_RATE_ADDR,f43,bat_voltage_rate);
				DMT_ScreenLbuf(DF_WEIHU_BIANHAO_ADDR,8,bianhao_new);		
				DMT_ScreenFbuf(DF_WK_PID_P_ADDR,f21,wk_pid_para.pgain);
				DMT_ScreenFbuf(DF_WK_PID_I_ADDR,f43,wk_pid_para.igain);
				DMT_ScreenCbuf(DF_WK_PID_I_LIMIT_ADDR,1,pid_integral_limit);
				DMT_ScreenFbuf(DF_WK_PID_D_ADDR,f41,wk_pid_para.dgain);
				DMT_ScreenFbuf(DF_WK_PID_DEADBAND_ADDR,f21,wk_pid_para.deadband);
				DMT_ScreenFbuf(DF_WK_PID_START_HEATING_ADDR,f21,wk_start_heat_err);
				
				DMT_PutStr(DF_BACK_UP_MSG_ADDR,(u8 *)"   ");
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
                DMT_ScreenFbuf(DF_BAT_VOLTAGE_ADDR,f42,bat_voltage);
				DMT_ScreenFbuf(DF_BAT_CHARGE_CURRENT_ADDR,f40,charge_current);
			}
			break;
	}
}

void display_query(void)
{
	char strbuf[30];
	char cbuf[30];
	u8 hour_tmp,min_tmp,sec_tmp;
	switch (wnd_struct.current)
	{
		case DF_PIC_SAMPLE_DATA:
		case DF_PIC_QUERY:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;

				DMT_ChangePic(wnd_struct.current);
				ReadSampleFile_AB(AB_sample_file_num_tmp);
				if(sample_file_chaxun.channel == DF_CHANNEL_A)
				{
					DMT_PutStr(DF_SAMPLE_FILE_CHANNEL_ADDR,(u8 *)"A路采样");
				}
				else
				{
					DMT_PutStr(DF_SAMPLE_FILE_CHANNEL_ADDR,(u8 *)"B路采样");
				}
				DMT_ScreenIbuf(DF_SAMPLE_FILE_NO_ADDR,4,AB_sample_file_num_tmp);
			    strcpy(strbuf,(char*)"20");
				CharToStr(2,(u8 *)cbuf,sample_file_chaxun.year);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char*)"-");
				CharToStr(2,(u8 *)cbuf,sample_file_chaxun.month);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char*)"-");
				CharToStr(2,(u8 *)cbuf,sample_file_chaxun.day);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char*)"  ");
				CharToStr(2,(u8 *)cbuf,sample_file_chaxun.hour);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char*)":");
				CharToStr(2,(u8 *)cbuf,sample_file_chaxun.min);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char*)":");
				CharToStr(2,(u8 *)cbuf,sample_file_chaxun.sec);
				strcat(strbuf,cbuf);
				DMT_PutStr(DF_SAMPLE_FILE_DATE_ADDR,(u8 *)strbuf);
				hour_tmp = sample_file_chaxun.running_time / 3600;
				min_tmp = (sample_file_chaxun.running_time % 3600) / 60;
				sec_tmp = sample_file_chaxun.running_time % 60;
				CharToStr(2,(u8 *)cbuf,hour_tmp);
				strcpy(strbuf,cbuf);
				strcat(strbuf,(char*)":");
				CharToStr(2,(u8 *)cbuf,min_tmp);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char*)":");
				CharToStr(2,(u8 *)cbuf,sec_tmp);
				strcat(strbuf,cbuf);
				DMT_PutStr(DF_SAMPLE_FILE_TIME_ADDR,(u8 *)strbuf);				
				DMT_ScreenFbuf(DF_SAMPLE_FILE_ENVIRONMENT_TEMPERATURE_ADDR,sf31,sample_file_chaxun.environment_temperature);
				DMT_ScreenFbuf(DF_SAMPLE_FILE_BAR_ADDR,f52,sample_file_chaxun.barometric);
				DMT_ScreenFbuf(DF_SAMPLE_FILE_FLOW_SET_ADDR,f21,sample_file_chaxun.flow_set);
				DMT_ScreenFbuf(DF_SAMPLE_FILE_JY_ADDR,sf42,sample_file_chaxun.jy);
				DMT_ScreenFbuf(DF_SAMPLE_FILE_JW_ADDR,sf31,sample_file_chaxun.temperature);
				DMT_ScreenFbuf(DF_SAMPLE_FILE_VOLUME_ADDR,f62,sample_file_chaxun.volume);
			    DMT_ScreenFbuf(DF_SAMPLE_FILE_NORMAL_VOLUME_ADDR,f62,sample_file_chaxun.volume_biaokuang);
				if(err_dis == 1)
				{
					if(sample_file_chaxun.success == DF_JY_OUT)
					{
						DMT_PutStr(DF_SAMPLE_FILE_MSG_ADDR,(u8 *)"管路堵塞  ");
					}
					else if(sample_file_chaxun.success == DF_FAN_ERR)
					{
						DMT_PutStr(DF_SAMPLE_FILE_MSG_ADDR,(u8 *)"流量达不到");
					}
					else
					{
						DMT_PutStr(DF_SAMPLE_FILE_MSG_ADDR,(u8 *)"          ");
					}
				}
				else
				{
					DMT_PutStr(DF_SAMPLE_FILE_MSG_ADDR,(u8 *)"          ");
				}			
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
			}
			break;
		case DF_PIC_SAMPLE_DATA_NO_FILE:
		case DF_PIC_SAMPLE_DATA_POWER_NO_FILE:

		case DF_PIC_QUERY_NO_SAMPLE_FILE:
		case DF_PIC_QUERY_NO_POWER_FILE:

			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
			}
			break;
		case DF_PIC_SAMPLE_DATA_NO_FILE_USB_PRINT:
		case DF_PIC_SAMPLE_DATA_POWER_NO_FILE_USB_PRINT:

		
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
				timer_init(&EMPLOY_DELAY,20);
			}
			if(EMPLOY_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//延时1秒返回
				set_refresh_last_wnd_num();
			}
			break;
		case DF_PIC_SAMPLE_DATA_POWER:
		case DF_PIC_QUERY_POWER:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);	
				ReadPowerFile_AB(AB_power_lose_file_num_tmp);		
				if(power_file_chaxun.channel == DF_CHANNEL_A)
				{
					DMT_PutStr(DF_POWER_FILE_CHANNEL_ADDR,(u8 *)"A路掉电");
				}
				else
				{
					DMT_PutStr(DF_POWER_FILE_CHANNEL_ADDR,(u8 *)"B路掉电");
				}
				DMT_ScreenIbuf(DF_POWER_FILE_NO_ADDR,4,AB_power_lose_file_num_tmp);

				strcpy(strbuf,(char *)"20");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.year);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char *)"-");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.month);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char *)"-");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.day);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char *)"  ");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.hour);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char *)":");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.min);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char *)":");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.sec);
				strcat(strbuf,cbuf);
				DMT_PutStr(DF_POWER_FILE_POWERLOSE_ADDR,(u8 *)strbuf);
	
				strcpy(strbuf,(char *)"20");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.power_year);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char *)"-");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.power_month);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char *)"-");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.power_day);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char *)"  ");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.power_hour);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char *)":");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.power_min);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char *)":");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.power_sec);
				strcat(strbuf,cbuf);
				DMT_PutStr(DF_POWER_FILE_POWERON_ADDR,(u8 *)strbuf);
				
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.lose_hour);
				strcpy(strbuf,cbuf);
				strcat(strbuf,(char *)":");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.lose_min);
				strcat(strbuf,cbuf);
				strcat(strbuf,(char *)":");
				CharToStr(2,(u8 *)cbuf,power_file_chaxun.lose_sec);
				strcat(strbuf,cbuf);
				DMT_PutStr(DF_POWER_FILE_LOSETIME_ADDR,(u8 *)strbuf);
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
			}
			break;
	}
}
void display_print(void)
{
	switch (wnd_struct.current)
	{
		case DF_PIC_NO_PRINTER:
		case DF_PIC_PRINTING:
		case DF_PIC_FIND_PRINTER:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次
			}
			break;
		case DF_PIC_SAMPLE_PRINT:	
		case DF_PIC_PRINT:	
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
			    sample_file_start_num = AB_sample_file.num;
			    sample_file_end_num = AB_sample_file.num;
			    power_file_start_num = AB_power_lose_file.num;
			    power_file_end_num = AB_power_lose_file.num;
				DMT_ScreenIbuf(DF_SAMPLE_FILE_PRINT_START_ADDR,4,sample_file_start_num);
				DMT_ScreenIbuf(DF_SAMPLE_FILE_PRINT_END_ADDR,4,sample_file_end_num);
				DMT_ScreenIbuf(DF_POWER_FILE_PRINT_START_ADDR,4,power_file_start_num);
				DMT_ScreenIbuf(DF_POWER_FILE_PRINT_END_ADDR,4,power_file_end_num);
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
			}
			break;
	}
}
void display_usb(void)
{
	switch (wnd_struct.current)
	{
		case DF_PIC_UDISK_ERR:
		case DF_PIC_INSERT_DISK:
		case DF_PIC_FILE_OUT:
		case DF_PIC_FILE_OUT_SUCCESS:
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次
			}
			break;
		case DF_PIC_SAMPLE_USB:	
		case DF_PIC_USB:	
			if(wnd_struct.change_pic == 1)
			{
				wnd_struct.change_pic = 0;
				DMT_ChangePic(wnd_struct.current);
			    sample_file_start_num = AB_sample_file.num;
			    sample_file_end_num = AB_sample_file.num;
			    power_file_start_num = AB_power_lose_file.num;
			    power_file_end_num = AB_power_lose_file.num;
				DMT_ScreenIbuf(DF_SAMPLE_FILE_USB_START_ADDR,4,sample_file_start_num);//显示文件号
				DMT_ScreenIbuf(DF_SAMPLE_FILE_USB_END_ADDR,4,sample_file_end_num);//显示文件号
				DMT_ScreenIbuf(DF_POWER_FILE_USB_START_ADDR,4,power_file_start_num);//显示文件号								
				DMT_ScreenIbuf(DF_POWER_FILE_USB_END_ADDR,4,power_file_end_num);//显示文件号											
			}
			if(REFRESH_DELAY.flag == 1)
			{
				REFRESH_DELAY.flag = 0;	//每秒刷新一次	
			}
			break;
	}
}


void display_task(void)
{
	display_sleep();
	display_power_lose();
	display_sharing();
	display_home();
	display_set();
	display_sample();
	display_maintenance();
	display_query();
	display_print();
	display_usb();
	display_supper_maintenance();
	display_bluetooth_set();
}

void usb_out_sample_file_task(void)
{
	u8 Err;
	u8 cbuf[100];
    u8 bianhao_buf[10];
    u16 file_tmp;
	if(USB_OUT_SAMPLE.step == DF_USB_IDLE)
	{
		return;
	}
	if(USB_OUT_SAMPLE.step == DF_USB_INIT)
	{
		USB_OUT_SAMPLE.step = DF_USB_CONNECT;
	}
	if(USB_OUT_SAMPLE.step == DF_USB_CONNECT)
	{
		last_wnd = wnd_struct.current;
		if(USH_User_App_flag != DF_USH_USER_APP_OK)
		{
			USB_OUT_SAMPLE.step = DF_USB_CONNECT_ERR;
			//切换显示插入U盘图片，2秒
			set_refresh_wnd_num(DF_PIC_INSERT_DISK);
			timer_init(&USB_1000ms,40);//定时2秒
		}
		else
		{
			USB_OUT_SAMPLE.step = DF_USB_CREAT_FILE;
			set_refresh_wnd_num(DF_PIC_FILE_OUT);
		}
        return;
	}
	if(USB_OUT_SAMPLE.step == DF_USB_CONNECT_ERR || USB_OUT_SAMPLE.step == DF_UDISK_ERR)
	{
		if(USB_1000ms.flag == 1)
		{
			USB_1000ms.flag = 0;
			//切换到原始界面
			set_refresh_wnd_num(last_wnd);
			USB_OUT_SAMPLE.step = DF_USB_IDLE;
		}
        return;
	}
	if(USB_OUT_SAMPLE.step == DF_USB_CREAT_FILE)
	{
		if(file_format == 0)
		{
			f_mount(fs[2],"2:",1);
			Err = f_mkdir("2:ZR-3500型大气采样器");
			delay_ms(10);
			bianhao[8] = 0;
			strcpy((char *)cbuf,(const char *)"2:ZR-3500型大气采样器/");
#ifdef ZR3500TWO 
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEAT
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEAT_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEATandCOOL
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
            strcat(cbuf,bianhao_buf);
#endif
//			strcat((char *)cbuf,(const char *)bianhao);
			Err = f_mkdir(cbuf);
			delay_ms(10);
			strcat((char *)cbuf,(const char *)"/");
			strcat((char *)cbuf,(const char *)"采样文件.txt");
			
			Err = f_open(file,cbuf,FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
			if(Err == FR_OK)
			{
				if((*file).fsize == 0)
				{
					usb_out_file_step = DF_FILE_HEAD_LILE;	
				}
				else
				{
					usb_out_file_step = DF_FILE_CONTENT;             	//定位到文件的结尾，继续追加数据
				}
				f_lseek(file,(*file).fsize);							//新文件
				total = 0;				
			}
			else													//导出错误
			{
				USB_OUT_SAMPLE.step = DF_UDISK_ERR;
				//切换导出错误图片，2秒
				set_refresh_wnd_num(DF_PIC_UDISK_ERR);
				timer_init(&USB_1000ms,40);//定时2秒
				return;
			}
		}
		else
		{
			f_mount(fs[2],"2:",1);
			Err = f_mkdir("2:ZR-3500型大气采样器");
			delay_ms(10);
			bianhao[8] = 0;
			strcpy((char *)cbuf,(const char *)"2:ZR-3500型大气采样器/");
#ifdef ZR3500TWO 
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEAT
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEAT_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEATandCOOL
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
            strcat(cbuf,bianhao_buf);
#endif
//			strcat((char *)cbuf,(const char *)bianhao);
			Err = f_mkdir(cbuf);
			delay_ms(10);
			strcat((char *)cbuf,(const char *)"/");
			strcat((char *)cbuf,(const char *)"采样文件.csv");
			Err = f_open(file,cbuf,FA_OPEN_ALWAYS|FA_WRITE|FA_READ);		
			if(Err == FR_OK)
			{
				if((*file).fsize == 0)
				{
					usb_out_file_step = DF_FILE_HEAD_LILE;
				}
				else
				{
					usb_out_file_step = DF_FILE_CONTENT;             	//定位到文件的结尾，继续追加数据
				}
				f_lseek(file,(*file).fsize);							//新文件
				total = 0;
			}
			else
			{
				USB_OUT_SAMPLE.step = DF_UDISK_ERR;
				//切换导出错误图片，2秒
				set_refresh_wnd_num(DF_PIC_UDISK_ERR);
				timer_init(&USB_1000ms,40);//定时2秒
				return;
			}
		}
		if(usb_out_start_file > usb_out_end_file)//调换文件号
		{
			file_tmp = usb_out_end_file;
			usb_out_end_file = usb_out_start_file;
			usb_out_start_file = file_tmp;
		}
		USB_OUT_SAMPLE.step = DF_USB_ONE_FILE;
		return;
	}
	if(USB_OUT_SAMPLE.step == DF_USB_ONE_FILE)
	{
		//显示正在导出的文件号
		DMT_ScreenIbuf(DF_DIS_SAMPLE_FILE_USB_PRINT_ADDR,4,usb_out_start_file);
		if(file_format == 0)
		{
			f_mount(fs[2],"2:",1);
			bianhao[8] = 0;
			strcpy((char *)cbuf,(const char *)"2:ZR-3500型大气采样器/");
#ifdef ZR3500TWO 
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEAT
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEAT_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEATandCOOL
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
            strcat(cbuf,bianhao_buf);
#endif
//			strcat((char *)cbuf,(const char *)bianhao);
			strcat((char *)cbuf,(const char *)"/");
			strcat((char *)cbuf,(const char *)"采样文件.txt");
			Err = f_open(file,cbuf,FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
			if(Err != FR_OK)
			{
				USB_OUT_SAMPLE.step = DF_UDISK_ERR;
				//切换导出错误图片，2秒
				set_refresh_wnd_num(DF_PIC_UDISK_ERR);
				timer_init(&USB_1000ms,40);//定时2秒
				return;
			}
			f_lseek(file,(*file).fsize);		
			SampleFileOut(usb_out_start_file,usb_out_end_file);
		}
		else
		{
			f_mount(fs[2],"2:",1);
			Err = f_mkdir("2:ZR-3500型大气采样器");
			bianhao[8] = 0;
			strcpy((char *)cbuf,(const char *)"2:ZR-3500型大气采样器/");
#ifdef ZR3500TWO 
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEAT
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEAT_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEATandCOOL
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
            strcat(cbuf,bianhao_buf);
#endif
//			strcat((char *)cbuf,(const char *)bianhao);
			strcat((char *)cbuf,(const char *)"/");
			strcat((char *)cbuf,(const char *)"采样文件.csv");
			Err = f_open(file,cbuf,FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
			if(Err != FR_OK)
			{
				USB_OUT_SAMPLE.step = DF_UDISK_ERR;
				//切换导出错误图片，2秒
				set_refresh_wnd_num(DF_PIC_UDISK_ERR);
				timer_init(&USB_1000ms,40);//定时2秒
				return;
			}
			f_lseek(file,(*file).fsize);		
			SampleFileOut_CSV(usb_out_start_file,usb_out_end_file);
		}
		if(usb_out_file_step == DF_FILE_CLOSED)	//导出完毕或拔出U盘或导出错误
		{
			USB_OUT_SAMPLE.step = DF_USB_OUT_SUCCESS;
			set_refresh_wnd_num(DF_PIC_FILE_OUT_SUCCESS);
			timer_init(&USB_1000ms,40);//定时2秒
			return;
		}
		return;
	}
	if(USB_OUT_SAMPLE.step == DF_USB_OUT_SUCCESS)
	{
		if(USB_1000ms.flag == 1)
		{
			USB_1000ms.flag = 0;
			USB_OUT_SAMPLE.step = DF_USB_IDLE;
			//切换到原始界面
			set_refresh_wnd_num(last_wnd);
		}
	}
}


void usb_out_power_file_task(void)
{
	u8 Err;
	u8 cbuf[100];
    u8 bianhao_buf[10];
    u16 file_tmp;
	if(USB_OUT_POWER.step == DF_USB_IDLE)
	{
		return;
	}
	if(USB_OUT_POWER.step == DF_USB_INIT)
	{
		last_wnd = wnd_struct.current;
		USB_OUT_POWER.step = DF_USB_CONNECT;
	}
	if(USB_OUT_POWER.step == DF_USB_CONNECT)
	{
		if(USH_User_App_flag != DF_USH_USER_APP_OK)
		{
			USB_OUT_POWER.step = DF_USB_CONNECT_ERR;
			//切换显示插入U盘图片，2秒
			set_refresh_wnd_num(DF_PIC_INSERT_DISK);
			timer_init(&USB_1000ms,40);//定时2秒
		}
		else
		{
			USB_OUT_POWER.step = DF_USB_CREAT_FILE;
			set_refresh_wnd_num(DF_PIC_FILE_OUT);
		}
        return;
	}
	if(USB_OUT_POWER.step == DF_USB_CONNECT_ERR || USB_OUT_POWER.step == DF_UDISK_ERR)
	{
		if(USB_1000ms.flag == 1)
		{
			USB_1000ms.flag = 0;
			//切换到原始界面
			set_refresh_wnd_num(last_wnd);
			USB_OUT_POWER.step = DF_USB_IDLE;
		}
        return;
	}
	if(USB_OUT_POWER.step == DF_USB_CREAT_FILE)
	{
		if(file_format == 0)
		{
			f_mount(fs[2],"2:",1);
			Err = f_mkdir("2:ZR-3500型大气采样器");
			delay_ms(10);
			bianhao[8] = 0;
			strcpy((char *)cbuf,(const char *)"2:ZR-3500型大气采样器/");
#ifdef ZR3500TWO 
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEAT
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEAT_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEATandCOOL
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
            strcat(cbuf,bianhao_buf);
#endif
//			strcat((char *)cbuf,(const char *)bianhao);
			Err = f_mkdir(cbuf);
			delay_ms(10);
			strcat((char *)cbuf,(const char *)"/");
			strcat((char *)cbuf,(const char *)"掉电记录.txt");
			Err = f_open(file,cbuf,FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
			if(Err == FR_OK)
			{
				if((*file).fsize == 0)
				{
					usb_out_file_step = DF_FILE_HEAD_LILE;	
				}
				else
				{
					usb_out_file_step = DF_FILE_CONTENT;             	//定位到文件的结尾，继续追加数据
				}
				f_lseek(file,(*file).fsize);							//新文件
				total = 0;				
			}
			else													//导出错误
			{
				USB_OUT_POWER.step = DF_UDISK_ERR;
				//切换导出错误图片，2秒
				set_refresh_wnd_num(DF_PIC_UDISK_ERR);
				timer_init(&USB_1000ms,40);//定时2秒
				return;
			}
		}
		else
		{
			f_mount(fs[2],"2:",1);
			Err = f_mkdir("2:ZR-3500型大气采样器");
			delay_ms(10);
			bianhao[8] = 0;
			strcpy((char *)cbuf,(const char *)"2:ZR-3500型大气采样器/");
#ifdef ZR3500TWO 
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEAT
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEAT_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEATandCOOL
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
            strcat(cbuf,bianhao_buf);
#endif
//			strcat((char *)cbuf,(const char *)bianhao);
			Err = f_mkdir(cbuf);
			delay_ms(10);
			strcat((char *)cbuf,(const char *)"/");
			strcat((char *)cbuf,(const char *)"掉电记录.csv");
			Err = f_open(file,cbuf,FA_OPEN_ALWAYS|FA_WRITE|FA_READ);		
			if(Err == FR_OK)
			{
				if((*file).fsize == 0)
				{
					usb_out_file_step = DF_FILE_HEAD_LILE;
				}
				else
				{
					usb_out_file_step = DF_FILE_CONTENT;             	//定位到文件的结尾，继续追加数据
				}
				f_lseek(file,(*file).fsize);							//新文件
				total = 0;
			}
			else
			{
				USB_OUT_POWER.step = DF_UDISK_ERR;
				//切换导出错误图片，2秒
				set_refresh_wnd_num(DF_PIC_UDISK_ERR);
				timer_init(&USB_1000ms,40);//定时2秒
				return;
			}
		}
		if(usb_out_start_file > usb_out_end_file)//调换文件号
		{
			file_tmp = usb_out_end_file;
			usb_out_end_file = usb_out_start_file;
			usb_out_start_file = file_tmp;
		}
		USB_OUT_POWER.step = DF_USB_ONE_FILE;
		return;
	}
	if(USB_OUT_POWER.step == DF_USB_ONE_FILE)
	{
		//显示正在导出的文件号
		DMT_ScreenIbuf(DF_DIS_SAMPLE_FILE_USB_PRINT_ADDR,4,usb_out_start_file);
		if(file_format == 0)
		{
			f_mount(fs[2],"2:",1);
			bianhao[8] = 0;
			strcpy((char *)cbuf,(const char *)"2:ZR-3500型大气采样器/");
#ifdef ZR3500TWO 
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEAT
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEAT_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEATandCOOL
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
            strcat(cbuf,bianhao_buf);
#endif
//			strcat((char *)cbuf,(const char *)bianhao);
			strcat((char *)cbuf,(const char *)"/");
			strcat((char *)cbuf,(const char *)"掉电记录.txt");
			Err = f_open(file,cbuf,FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
			if(Err != FR_OK)
			{
				USB_OUT_POWER.step = DF_UDISK_ERR;
				//切换导出错误图片，2秒
				set_refresh_wnd_num(DF_PIC_UDISK_ERR);
				timer_init(&USB_1000ms,40);//定时2秒
				return;
			}
			f_lseek(file,(*file).fsize);		
			PowerFileOut(usb_out_start_file,usb_out_end_file);
		}
		else
		{
			f_mount(fs[2],"2:",1);
			Err = f_mkdir("2:ZR-3500型大气采样器");
			bianhao[8] = 0;
			strcpy((char *)cbuf,(const char *)"2:ZR-3500型大气采样器/");
#ifdef ZR3500TWO 
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEAT
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEAT_NAME);
            strcat(cbuf,bianhao_buf);
#endif
#ifdef ZR3500TWO_HEATandCOOL
            SPutl(8,bianhao_buf,bianhao_new);
            strcat(cbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
            strcat(cbuf,bianhao_buf);
#endif
//			strcat((char *)cbuf,(const char *)bianhao);
			strcat((char *)cbuf,(const char *)"/");
			strcat((char *)cbuf,(const char *)"掉电记录.csv");
			Err = f_open(file,cbuf,FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
			if(Err != FR_OK)
			{
				USB_OUT_POWER.step = DF_UDISK_ERR;
				//切换导出错误图片，2秒
				set_refresh_wnd_num(DF_PIC_UDISK_ERR);
				timer_init(&USB_1000ms,40);//定时2秒
				return;
			}
			f_lseek(file,(*file).fsize);		
			PowerFileOut_CSV(usb_out_start_file,usb_out_end_file);
		}
		if(usb_out_file_step == DF_FILE_CLOSED)	//导出完毕
		{
			USB_OUT_POWER.step = DF_USB_OUT_SUCCESS;
            set_refresh_wnd_num(DF_PIC_FILE_OUT_SUCCESS);
			timer_init(&USB_1000ms,40);//定时2秒
			return;
		}
		return;
	}
	if(USB_OUT_POWER.step == DF_USB_OUT_SUCCESS)
	{
		if(USB_1000ms.flag == 1)
		{
			USB_1000ms.flag = 0;
			USB_OUT_POWER.step = DF_USB_IDLE;
			//切换到原始界面
			set_refresh_wnd_num(last_wnd);
		}
	}
}



void bluetooth_task_init(void)
{
	BLUETOOTH.count = 0;
	BLUETOOTH.i = 0;
	BLUETOOTH.connected_flag = DF_STATUS_BLUETOOTH_NOT_CONNECTED;
	BLUETOOTH.enable = 1;
	clear_bluetooth_timeout();
	BLUETOOTH.step = DF_BLUETOOTH_INIT;
}
void clear_bluetooth_timeout(void)
{
	BLUETOOTH.timeout = 0;
}

void bluetooth_connect_task(void)
{
	if(BLUETOOTH.step == DF_BLUETOOTH_IDLE)
	{
		BLUETOOTH.connected_flag = DF_STATUS_BLUETOOTH_NOT_CONNECTED;
		return;
	}
	else
	{
		if((PRINT_SAMPLE.step != DF_PRINT_IDLE) ||(PRINT_POWER.step != DF_PRINT_IDLE))
		{
			clear_bluetooth_timeout();
		}
		else
		{
			if(BLUETOOTH.connected_flag == DF_STATUS_BLUETOOTH_NOT_CONNECTED)
			{
				clear_bluetooth_timeout();
			}
			else
			{
			
			}
		}
		if(BLUETOOTH_DELAY.flag == 1)
		{
			BLUETOOTH_DELAY.flag = 0;
			if(BLUETOOTH.timeout > BLUETOOTH.close_time_set)
			{
				BLUETOOTH.step = DF_BLUETOOTH_IDLE;
				BLUETOOTH_POWER_OFF();
				clear_bluetooth_timeout();
				return;
			}
			BLUETOOTH.timeout++;	
		}	
		if(BLUETOOTH.step == DF_BLUETOOTH_INIT)
		{
			BLUETOOTH_POWER_ON();
			timer_init(&BLUETOOTH_50ms_DELAY,10);//定时500ms
			BLUETOOTH.step = DF_BLUETOOTH_POWER_ON;
		}
		if(BLUETOOTH.step == DF_BLUETOOTH_POWER_ON)
		{
			if(BLUETOOTH_50ms_DELAY.flag == 1)	//500ms时间到
			{
				BLUETOOTH_50ms_DELAY.flag = 0;
				BLUETOOTH.step = DF_BLUETOOTH_RESET;
			}
		}
		if(BLUETOOTH.step == DF_BLUETOOTH_RESET)
		{
			BLUETOOTH_KEY = 0;
			delay_ms(10);
			BLUETOOTH_KEY = 1;
			delay_ms(10);		
			BLUETOOTH_KEY = 0;
			BLUETOOTH.step = DF_BLUETOOTH_CONNECT;
			timer_init(&BLUETOOTH_50ms_DELAY,1);//定时50ms
			BLUETOOTH.count = 0;
			BLUETOOTH.i = 0;
		}
		if(BLUETOOTH.step == DF_BLUETOOTH_CONNECT)
		{
			if(BLUETOOTH_50ms_DELAY.flag == 1)
			{
				BLUETOOTH_50ms_DELAY.flag = 0;
				if(BLUETOOTH_LED == 0)		//未连接上
				{
					BLUETOOTH.count = 0;
					if(BLUETOOTH.connected_flag != DF_STATUS_BLUETOOTH_CONNECTED_TIMEOUT)
					{
						BLUETOOTH.connected_flag = DF_STATUS_BLUETOOTH_NOT_CONNECTED;
					}
				}
				else
				{
					BLUETOOTH.count++;
					if(BLUETOOTH.count > 20)	//连接上
					{
						BLUETOOTH.count = 0;
						BLUETOOTH.connected_flag = DF_STATUS_BLUETOOTH_CONNECTED;
					}
				}
				if(BLUETOOTH.connected_flag == DF_STATUS_BLUETOOTH_NOT_CONNECTED)
				{
					BLUETOOTH.i++;
					if(BLUETOOTH.i > 300)		//15秒
					{
						BLUETOOTH.step = DF_BLUETOOTH_RESET;
						BLUETOOTH.connected_flag = DF_STATUS_BLUETOOTH_CONNECTED_TIMEOUT;			//超时未连接上
					}
				}
				else
				{
					BLUETOOTH.i = 0;
				}
			}
		}	
	}
}


void print_sample_file_task(void)
{
    u16 file_tmp;
	if(PRINT_SAMPLE.step == DF_PRINT_IDLE)
	{
		return;
	}
	if(PRINT_SAMPLE.step == DF_PRINT_INIT)
	{
		if(last_wnd_locked == 0)
		{
			last_wnd_locked = 1;
			last_wnd = wnd_struct.current;
		}
		//初始化串口
		PrintSetUp();
		if(BLUETOOTH.step == DF_BLUETOOTH_IDLE)
		{
			bluetooth_task_init();
		}
        PRINT_SAMPLE.i = 0;
		timer_init(&PRINT_DELAY,20);//定时1s
        PRINT_SAMPLE.step = DF_PRINT_CONNECT;
        set_refresh_wnd_num(DF_PIC_FIND_PRINTER); 
        DMT_ScreenIbuf(DF_DIS_SAMPLE_FILE_USB_PRINT_ADDR,4,print_start_file);
	}
	if(PRINT_SAMPLE.step == DF_PRINT_CONNECT)
	{
		if(BLUETOOTH.connected_flag == DF_STATUS_BLUETOOTH_CONNECTED)
		{
			PRINT_SAMPLE.step = DF_PRINT_FILE;
			set_refresh_wnd_num(DF_PIC_PRINTING);
			InitPrinter();
			print_file_step = DF_FILE_HEAD_LILE;
			if(print_start_file > print_end_file)//调换文件号
			{
				file_tmp = print_end_file;
				print_end_file = print_start_file;
				print_start_file = file_tmp;
			}
		}
		else
		{
			if(PRINT_DELAY.flag == 1)
			{
				PRINT_DELAY.flag = 0;
				PRINT_SAMPLE.i++;
				if(PRINT_SAMPLE.i > 15)
				{
					PRINT_SAMPLE.step = DF_PRINT_CONNECT_ERR;
					//切换显示未搜索到打印机1.5秒
					set_refresh_wnd_num(DF_PIC_NO_PRINTER);
					timer_init(&PRINT_DELAY,30);//定时1.5s
				}
			}
		}
	}
	if(PRINT_SAMPLE.step == DF_PRINT_CONNECT_ERR)
	{
		if(PRINT_DELAY.flag == 1)
		{
			PRINT_DELAY.flag = 0;
			//切换到原始界面
			set_refresh_wnd_num(last_wnd);
			PRINT_SAMPLE.step = DF_PRINT_IDLE;
			last_wnd_locked = 0;
		}
        return;
	}
	if(PRINT_SAMPLE.step == DF_PRINT_FILE)
	{
		//显示正在打印的文件号
		DMT_ScreenIbuf(DF_DIS_SAMPLE_FILE_USB_PRINT_ADDR,4,print_start_file);
		PrintSampleFile(print_start_file,print_end_file);
		if(print_file_step == DF_FILE_CLOSED)	//打印完毕,延时关闭蓝牙模块
		{
			timer_init(&PRINT_DELAY,10);//定时500ms
			PRINT_SAMPLE.step = DF_PRINT_FILE_OK;
		}
		return;
	}
	if(PRINT_SAMPLE.step == DF_PRINT_FILE_OK)
	{
		if(PRINT_DELAY.flag == 1)
		{
			PRINT_DELAY.flag = 0;
			PRINT_SAMPLE.step = DF_PRINT_IDLE;
			//切换到原始界面
			set_refresh_wnd_num(last_wnd);
			last_wnd_locked = 0;
		}
	}
}


void print_power_file_task(void)
{
    u16 file_tmp;
	if(PRINT_POWER.step == DF_PRINT_IDLE)
	{
		return;
	}
	if(PRINT_POWER.step == DF_PRINT_INIT)
	{
		if(last_wnd_locked == 0)
		{
			last_wnd_locked = 1;
			last_wnd = wnd_struct.current;
		}
		//初始化串口
		PrintSetUp();
		if(BLUETOOTH.step == DF_BLUETOOTH_IDLE)
		{
			bluetooth_task_init();
		}
		PRINT_POWER.i = 0;
		timer_init(&PRINT_DELAY,20);//定时1s
        PRINT_POWER.step = DF_PRINT_CONNECT;
        set_refresh_wnd_num(DF_PIC_FIND_PRINTER); 
        DMT_ScreenIbuf(DF_DIS_SAMPLE_FILE_USB_PRINT_ADDR,4,print_start_file);
	}
	if(PRINT_POWER.step == DF_PRINT_CONNECT)
	{
		if(BLUETOOTH.connected_flag == DF_STATUS_BLUETOOTH_CONNECTED)
		{
			PRINT_POWER.step = DF_PRINT_FILE;
			set_refresh_wnd_num(DF_PIC_PRINTING);
			InitPrinter();
			print_file_step = DF_FILE_HEAD_LILE;
			if(print_start_file > print_end_file)//调换文件号
			{
				file_tmp = print_end_file;
				print_end_file = print_start_file;
				print_start_file = file_tmp;
			}
		}
		else
		{
			if(PRINT_DELAY.flag == 1)
			{
				PRINT_DELAY.flag = 0;
				PRINT_POWER.i++;
				if(PRINT_POWER.i > 15)
				{
					PRINT_POWER.step = DF_PRINT_CONNECT_ERR;
					//切换显示未搜索到打印机5秒
					set_refresh_wnd_num(DF_PIC_NO_PRINTER);
					timer_init(&PRINT_DELAY,30);//定时1.5s
				}
			}
		}
	}
	if(PRINT_POWER.step == DF_PRINT_CONNECT_ERR)
	{
		if(PRINT_DELAY.flag == 1)
		{
			PRINT_DELAY.flag = 0;
			//切换到原始界面
			set_refresh_wnd_num(last_wnd);
			PRINT_POWER.step = DF_PRINT_IDLE;
			last_wnd_locked = 0;
		}
        return;
	}
	if(PRINT_POWER.step == DF_PRINT_FILE)
	{
		//显示正在打印的文件号
		DMT_ScreenIbuf(DF_DIS_SAMPLE_FILE_USB_PRINT_ADDR,4,print_start_file);
		PrintPowerFile(print_start_file,print_end_file);
		
		if(print_file_step == DF_FILE_CLOSED)	//打印完毕
		{
			timer_init(&PRINT_DELAY,10);//定时500ms
			PRINT_POWER.step = DF_PRINT_FILE_OK;
		}
		return;
	}
	if(PRINT_POWER.step == DF_PRINT_FILE_OK)
	{
		if(PRINT_DELAY.flag == 1)
		{
			PRINT_DELAY.flag = 0;
			PRINT_POWER.step = DF_PRINT_IDLE;
			//切换到原始界面
			set_refresh_wnd_num(last_wnd);
			last_wnd_locked = 0;
		}
	}	
}
extern float Temperature_LY_A,Temperature_JY_A,Temperature_QY,Temperature_LY_B,Temperature_JY_B;
u16 SSDL_count = 0;
void SSDL_task(void)
{
    float fbuf;
    if(SSDL_CONVERT_DELAY.flag == 1)
    {
        SSDL_CONVERT_DELAY.flag = 0;
        
        ADBuf[DF_A_LIUYA] += Read_Pressure_LY_A(0);
        ADBuf[DF_A_JIYA] += Read_Pressure_JY_A(0);
        ADBuf[DF_B_LIUYA] += Read_Pressure_LY_B(0);
        ADBuf[DF_B_JIYA] += Read_Pressure_JY_B(0);
        ADBuf[DF_QIYA] += Read_Pressure_QY(0);
        SSDL_count++;
    }
    if(SSDL_1000ms.flag == 1)
	{
		SSDL_1000ms.flag = 0;
        if(SSDL_count != 0)
        {
//            fbuf = (ADBuf[DF_A_LIUYA]/(float)SSDL_count);
//			A_SAMPLE.dy = fbuf * A_SAMPLE.dy_rate + 500.0 - A_SAMPLE.dy_zero;
//			fbuf = (ADBuf[DF_A_JIYA]/(float)SSDL_count);
//			A_SAMPLE.jy = fbuf * A_SAMPLE.jy_rate + 500.0 - A_SAMPLE.jy_zero;
//			fbuf = (ADBuf[DF_B_LIUYA]/(float)SSDL_count);
//			B_SAMPLE.dy = fbuf * B_SAMPLE.dy_rate + 500.0 - B_SAMPLE.dy_zero;
//			fbuf = (ADBuf[DF_B_JIYA]/(float)SSDL_count);
//			B_SAMPLE.jy = fbuf * B_SAMPLE.jy_rate + 500.0 - B_SAMPLE.jy_zero;
//			fbuf = (ADBuf[DF_QIYA]/(float)SSDL_count);
//			barometric.value = fbuf * barometric.rate + 500.0 - barometric.zero;
            fbuf = (ADBuf[DF_A_LIUYA]/(float)SSDL_count);
			A_SAMPLE.dy = fbuf + (float)500.0 - A_SAMPLE.dy_zero;
			fbuf = (ADBuf[DF_A_JIYA]/(float)SSDL_count);
			A_SAMPLE.jy = fbuf + (float)500.0 - A_SAMPLE.jy_zero;
			fbuf = (ADBuf[DF_B_LIUYA]/(float)SSDL_count);
			B_SAMPLE.dy = fbuf + (float)500.0 - B_SAMPLE.dy_zero;
			fbuf = (ADBuf[DF_B_JIYA]/(float)SSDL_count);
			B_SAMPLE.jy = fbuf + (float)500.0 - B_SAMPLE.jy_zero;
			fbuf = (ADBuf[DF_QIYA]/(float)SSDL_count);
			barometric.value = fbuf + (float)500.0 - barometric.zero;            
            ADBuf[DF_A_LIUYA] = 0;
			ADBuf[DF_B_LIUYA] = 0;
			ADBuf[DF_A_JIYA] = 0;
			ADBuf[DF_B_JIYA] = 0;
			ADBuf[DF_QIYA] = 0;
            SSDL_count = 0;
        }
        calc_flow_task();
    }
}



void ADC_task_init(void)
{
	ADC1_Start();
}
void ADC_task(void)
{
	float fbuf;
	Process_ADC();
	if(ADC_1000ms.flag == 1)
	{
		ADC_1000ms.flag = 0;
		if(adcount != 0)
		{
			fbuf = (ADBuf[DF_BAT_VOLTAGE]/(float)adcount);
			bat_voltage = fbuf * (float)((float)750.0 + (float)100.0) /(float)100.0 * (float)3.3 / (float)4096.0 * bat_voltage_rate;

			fbuf = (ADBuf[DF_CHARGE_CURRENT]/(float)adcount);
			charge_current = fbuf * (float)0.5 * (float)3.3 / (float)4096.0 * charge_current_rate * (float)1000.0;													//????

            fbuf = (ADBuf[DF_HEAT_COOL_CURRENT]/(float)adcount);
			heat_cool_current = fbuf * (float)3.3 / (float)4096.0 / (float)0.01 * heat_cool_current_rate * (float)1000.0;	
//charge_current = heat_cool_current;            
			ADBuf[DF_BAT_VOLTAGE] = 0;
			ADBuf[DF_CHARGE_CURRENT] = 0;
            ADBuf[DF_HEAT_COOL_CURRENT] = 0;
//DMT_ScreenIbuf(DF_BIAODING_A_FLOW_RATE_ADDR,5,adcount);
			adcount = 0;
		}
	}
	return;
}

void calc_flow_task(void)
{
	ComputeFlow();
}

void read_temperature_task(void)
{
	if(DS18B20_1000ms.flag == 1)
	{
		DS18B20_1000ms.flag = 0;
		GetTemperature();
	}
}
void ctr_pump_task_init(void)
{
	PUMP_CTR_TASK_A.enable = 0;
	PUMP_CTR_TASK_B.enable = 0;
}

void start_ctr_A_pump_task(void)
{
	PUMP_CTR_TASK_A.enable = 1;
	PUMP_A_Start();
	PUMP_fbuf_A = A_PWM_01L;
	A_SAMPLE.flow_kedu = 0;
	A_SAMPLE.flow_rukou = 0;
	A_SAMPLE.flow_biaokuang = 0;
}
void stop_ctr_A_pump_task(void)
{
	PUMP_A_Stop();
	PUMP_CTR_TASK_A.enable = 0;
}
void ctr_A_pump_task(void)
{
	float ibuf;
	float result;
	float fbuf;	
	if(PUMP_A_1000ms.flag == 1)
	{
		PUMP_A_1000ms.flag = 0;
		if(PUMP_CTR_TASK_A.enable == 1)
		{
			result = PUMP_fbuf_A;
			
			fbuf = (A_SAMPLE.flow_set - A_SAMPLE.flow_ctr) * 100 * PUMP_SPEED;
			ibuf = fbuf;
			if(ibuf < -20000)
			{
				ibuf = -20000;
			}
			if(ibuf > 20000)
			{
				ibuf = 20000;
			}
			result += ibuf;
			if(result > AB_PUMP_MAX)
			{
				result = AB_PUMP_MAX;
			}
			if(result < AB_PUMP_LOW)
			{
				result = AB_PUMP_LOW;
			}
			PUMP_fbuf_A = result;
			TIM_SetCompare4(TIM4,PUMP_fbuf_A);
		}
	}
}



void start_ctr_B_pump_task(void)
{
	PUMP_CTR_TASK_B.enable = 1;
	PUMP_B_Start();
	PUMP_fbuf_B = B_PWM_01L;
	B_SAMPLE.flow_kedu = 0;
	B_SAMPLE.flow_rukou = 0;
	B_SAMPLE.flow_biaokuang = 0;
}
void stop_ctr_B_pump_task(void)
{
	PUMP_B_Stop();												//关闭PWM模块
	PUMP_CTR_TASK_B.enable = 0;
}
void ctr_B_pump_task(void)
{
	float ibuf;
	float result;
	float fbuf;
	if(PUMP_CTR_TASK_B.enable != 1)
	{
		return;
	}
	if(PUMP_B_1000ms.flag == 1)
	{
		PUMP_B_1000ms.flag = 0;
		result = PUMP_fbuf_B;
		fbuf = (B_SAMPLE.flow_set - B_SAMPLE.flow_ctr) * 100 * PUMP_SPEED;
		ibuf = fbuf;
		if(ibuf < -20000)
		{
			ibuf = -20000;
		}
		if(ibuf > 20000)
		{
			ibuf = 20000;
		}
		result += ibuf;
		if(result > AB_PUMP_MAX)
		{
			result = AB_PUMP_MAX;
		}
		if(result < AB_PUMP_LOW)
		{
			result = AB_PUMP_LOW;
		}
		PUMP_fbuf_B = result;
		TIM_SetCompare3(TIM4,PUMP_fbuf_B);
	}	
}	

void A_auto_zero(void)
{
	A_SAMPLE.dy_zero += A_SAMPLE.dy;
	A_SAMPLE.jy_zero += A_SAMPLE.jy; 
}
void B_auto_zero(void)
{
	B_SAMPLE.dy_zero += B_SAMPLE.dy;
	B_SAMPLE.jy_zero += B_SAMPLE.jy; 
}


u8 sample_pic_refresh_flag = 0;
void sample_A_task_init(void)
{
	SAMPLE_TASK_A.step = DF_SAMPLE_TASK_IDLE;
	SAMPLE_TASK_A.count = 5;
	SAMPLE_TASK_A.timeout = 0;
}
void stop_sample_A_task(void)
{
	sample_A_task_init();
	stop_ctr_A_pump_task();
}
void sample_A_task(void)
{
	u32 ibuf;
	if(SAMPLE_A_1000ms.flag == 1)
	{
		SAMPLE_A_1000ms.flag = 0;
		if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_IDLE)
		{
			return;
		}
		if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_CONTINUE_SAMPLING)
		{
			A_SAMPLE.dy_zero += A_SAMPLE.dy;
			A_SAMPLE.jy_zero += A_SAMPLE.jy; 
			if(SAMPLE_TASK_A.count > 0)
			{
	        	SAMPLE_TASK_A.count--;
	  		}      
			if(SAMPLE_TASK_A.count == 0)
	        {
				if((A_SAMPLE.dy > 1) || (A_SAMPLE.dy < -1))	//防止采样泵由于惯性运行的过程中执行了校零导致零点不准
				{
					SAMPLE_TASK_A.timeout++;
					if(SAMPLE_TASK_A.timeout > 60)
					{
						SAMPLE_TASK_A.step = DF_SAMPLE_TASK_SAMPLING;
						SAMPLE_TASK_A.count = 5;
						SAMPLE_TASK_A.timeout = 0;
					}
				}
	            else
				{
	               SAMPLE_TASK_A.step = DF_SAMPLE_TASK_SAMPLING;
	               SAMPLE_TASK_A.count = 5;
	               SAMPLE_TASK_A.timeout = 0;
				}
			}
			if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_SAMPLING)
			{
				start_ctr_A_pump_task();
				sample_pic_refresh_flag = 1;
			}
		}
		if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_AUTOZERO)
		{
			A_SAMPLE.dy_zero += A_SAMPLE.dy;
			A_SAMPLE.jy_zero += A_SAMPLE.jy; 
			if(SAMPLE_TASK_A.count > 0)
			{
	        	SAMPLE_TASK_A.count--;
	  		}      
			if(SAMPLE_TASK_A.count == 0)
	        {
				if((A_SAMPLE.dy > 1) || (A_SAMPLE.dy < -1))	//防止采样泵由于惯性运行的过程中执行了校零导致零点不准
				{
					SAMPLE_TASK_A.timeout++;
					if(SAMPLE_TASK_A.timeout > 60)
					{
						SAMPLE_TASK_A.step = DF_SAMPLE_TASK_SAMPLING;
						SAMPLE_TASK_A.count = 5;
						SAMPLE_TASK_A.timeout = 0;
					}
				}
	            else
				{
	               SAMPLE_TASK_A.step = DF_SAMPLE_TASK_SAMPLING;
	               SAMPLE_TASK_A.count = 5;
	               SAMPLE_TASK_A.timeout = 0;
				}
			}
			if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_SAMPLING)
			{
				A_SAMPLE.volume = 0;
				A_SAMPLE.volume_biaokuang = 0;
				A_SAMPLE.running_time = A_SAMPLE.running_time_set;	
				A_SAMPLE.running_time_by_volume = 0;
				A_SAMPLE.jiange_time = A_SAMPLE.jiange_time_set;
				//A_SAMPLE.sample_times保持原来值
				A_sample_file.jy = 0;
				A_sample_file.barometric = 0;
				A_sample_file.temperature = 0;
				A_sample_file.environment_temperature = 0;
				start_ctr_A_pump_task();
				A_save_sample_start();
				//刷新页面标志
				sample_pic_refresh_flag = 1;
			}
			A_SAMPLE.running_time = A_SAMPLE.running_time_set;	
			A_SAMPLE.running_time_by_volume = 0;
			A_SAMPLE.jiange_time = A_SAMPLE.jiange_time_set;
		}
		if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_SAMPLING)
		{
			//累计体积
			if(A_SAMPLE.flow_ctr_select == DF_FLOW_CTR_BIAOKUANG)
			{
				A_SAMPLE.volume += A_SAMPLE.flow / (float)60.0;
			}
			else
			{
				A_SAMPLE.volume += A_SAMPLE.flow_ctr / (float)60.0;
			}
			//标况体积
			A_SAMPLE.volume_biaokuang += A_SAMPLE.flow_biaokuang / (float)60.0;
			//5秒的滑动滤波显示流量值(显示函数中)
			
			//计算平均值，见suanfa.h
			A_avg_count += 1;
			A_sample_file.jy += (A_SAMPLE.jy - A_sample_file.jy) / A_avg_count;
			A_sample_file.temperature += (sample_temperature[DF_A_TEMPERATURE].value - A_sample_file.temperature) / A_avg_count;
			A_sample_file.environment_temperature += (A_SAMPLE.environment_temperature_input - A_sample_file.environment_temperature) / A_avg_count;
			A_sample_file.barometric += (A_SAMPLE.barometric_input - A_sample_file.barometric) / A_avg_count;	
			A_SAMPLE.running_time_by_volume++;
			if(SAMPLE_MODE.channel_A == DF_SAMPLE_MODE_BY_TIME)
			{
				if(A_SAMPLE.running_time > 0)
				{
					A_SAMPLE.running_time--;
				}
				if(A_SAMPLE.running_time == 0)		//本次采样结束
				{
					//停止PWM
					stop_ctr_A_pump_task();
					//保存采样文件
					SaveSampleFile_A();
					A_SAMPLE.sample_times++;
					if(A_SAMPLE.sample_times >= A_SAMPLE.sample_times_set)
					{
						A_sample_over_flag = 1;
						SAMPLE_TASK_A.step = DF_SAMPLE_TASK_IDLE;
						A_sample_over();
						sample_pic_refresh_flag = 1;
						return;
					}
					else
					{
						SAMPLE_TASK_A.step = DF_SAMPLE_TASK_INTERVAL;
						A_SAMPLE.jiange_time = A_SAMPLE.jiange_time_set;
						sample_pic_refresh_flag = 1;
					}
				}
			}
			else
			{
				if(A_SAMPLE.sample_volume_set > A_SAMPLE.volume)
				{
					if(A_flow_dis.value > 0)
					{
						A_SAMPLE.running_time = (A_SAMPLE.sample_volume_set - A_SAMPLE.volume) / A_flow_dis.value * (float)60.0;
				    }
					else
					{
						A_SAMPLE.running_time = 360000 - 1;
					}
				}
				else
				{
					A_SAMPLE.running_time = 0;
				}
				if(A_SAMPLE.volume >= A_SAMPLE.sample_volume_set)
				{
					//停止PWM
					stop_ctr_A_pump_task();
					//保存采样文件
					SaveSampleFile_A();
					A_SAMPLE.sample_times++;
					if(A_SAMPLE.sample_times >= A_SAMPLE.sample_times_set)
					{
						A_sample_over_flag = 1;
						SAMPLE_TASK_A.step = DF_SAMPLE_TASK_IDLE;
						A_sample_over();
						sample_pic_refresh_flag = 1;
						return;
					}
					else
					{
						SAMPLE_TASK_A.step = DF_SAMPLE_TASK_INTERVAL;
						A_SAMPLE.jiange_time = A_SAMPLE.jiange_time_set;
						sample_pic_refresh_flag = 1;
					}
				}
			}
			ibuf = TIM_GetCapture4(TIM4);
			if(ibuf >= AB_PUMP_MAX)
			{
				if(A_SAMPLE.flow_biaokuang < (A_SAMPLE.flow_set * (float)0.9))
				{
					A_PWM_count++;
				}
				else
				{
					A_PWM_count = 0;
				}
			}
			else
			{
				A_PWM_count = 0;
			}
			if(A_PWM_count > A_sample_protect_time)
			{
				A_sample_file.success = DF_FAN_ERR;
				goto GOTO_SAMPLE_ERR;
			}
			if(A_SAMPLE.jy < A_max_jy)
			{ 
				if(++A_Qcount >= A_sample_protect_time)
				{
					A_sample_file.success = DF_JY_OUT;
	GOTO_SAMPLE_ERR:
					//停止PWM
					stop_ctr_A_pump_task();
					A_PWM_count = 0;
					A_Qcount = 0;
					//提示出错信息
					SAMPLE_TASK_A.step = DF_SAMPLE_TASK_ERR;
					sample_pic_refresh_flag = 1;
				}//5秒超压，说明管路有问题
			}
			else
			{
				A_Qcount = 0;
			}
		}
		if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_WAIT)
		{
			//判断当前时间是否为设定时间
			if((A_SAMPLE.start_hour_set == sys_hour) && (A_SAMPLE.start_min_set == sys_min))
			{
				A_SAMPLE.sample_times = 0;
				SAMPLE_TASK_A.step = DF_SAMPLE_TASK_AUTOZERO;
				sample_pic_refresh_flag = 1;
			}
		}
		if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_INTERVAL)
		{
			//间隔时间计数（递减）
			//若时间为0，则启动采样
			if(A_SAMPLE.jiange_time > 0)
			{
				A_SAMPLE.jiange_time--;
			}
			if(A_SAMPLE.jiange_time == 0)
			{
				if(A_SAMPLE.jiange_time_set != 0)	//若间隔时间设置为0，则不必校零
				{
					SAMPLE_TASK_A.step = DF_SAMPLE_TASK_AUTOZERO;
				}
				else
				{
					SAMPLE_TASK_A.step = DF_SAMPLE_TASK_SAMPLING;
				}
				sample_pic_refresh_flag = 1;
			}
		}
		if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_ERR)
		{
		
		}
		//掉电保护
		A_power_lose_protect(SAMPLE_TASK_A.step);
	}
}




void sample_B_task_init(void)
{
	SAMPLE_TASK_B.step = DF_SAMPLE_TASK_IDLE;
	SAMPLE_TASK_B.count = 5;
	SAMPLE_TASK_B.timeout = 0;
}
void stop_sample_B_task(void)
{
	sample_B_task_init();
	stop_ctr_B_pump_task();
}
void sample_B_task(void)
{
	u32 ibuf;
	if(SAMPLE_B_1000ms.flag == 1)
	{
		SAMPLE_B_1000ms.flag = 0;
		if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_IDLE)
		{
			return;
		}
		if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_CONTINUE_SAMPLING)
		{
			B_SAMPLE.dy_zero += B_SAMPLE.dy;
			B_SAMPLE.jy_zero += B_SAMPLE.jy; 
			if(SAMPLE_TASK_B.count > 0)
			{
	        	SAMPLE_TASK_B.count--;
	  		}	      
			if(SAMPLE_TASK_B.count == 0)
	        {
		        if((B_SAMPLE.dy > 1) || (B_SAMPLE.dy < -1))
		        {
		        	SAMPLE_TASK_B.timeout++;
		        	if(SAMPLE_TASK_B.timeout > 60)
		        	{
		        		SAMPLE_TASK_B.step = DF_SAMPLE_TASK_SAMPLING;
		        		SAMPLE_TASK_B.count = 5;
		        		SAMPLE_TASK_B.timeout = 0;
		        	}
		        }
		        else
		        {
		        	SAMPLE_TASK_B.step = DF_SAMPLE_TASK_SAMPLING;
	        		SAMPLE_TASK_B.count = 5;
	        		SAMPLE_TASK_B.timeout = 0;
		        }				
			}
			if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_SAMPLING)
			{
				start_ctr_B_pump_task();
				sample_pic_refresh_flag = 1;
			}
		}
		if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_AUTOZERO)
		{
			B_SAMPLE.dy_zero += B_SAMPLE.dy;
			B_SAMPLE.jy_zero += B_SAMPLE.jy; 
	        if(SAMPLE_TASK_B.count > 0)
			{
	        	SAMPLE_TASK_B.count--;
	  		}	      
			if(SAMPLE_TASK_B.count == 0)
	        {
		        if((B_SAMPLE.dy > 1) || (B_SAMPLE.dy < -1))
		        {
		        	SAMPLE_TASK_B.timeout++;
		        	if(SAMPLE_TASK_B.timeout > 60)
		        	{
		        		SAMPLE_TASK_B.step = DF_SAMPLE_TASK_SAMPLING;
		        		SAMPLE_TASK_B.count = 5;
		        		SAMPLE_TASK_B.timeout = 0;
		        	}
		        }
		        else
		        {
		        	SAMPLE_TASK_B.step = DF_SAMPLE_TASK_SAMPLING;
	        		SAMPLE_TASK_B.count = 5;
	        		SAMPLE_TASK_B.timeout = 0;
		        }				
			}
			if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_SAMPLING)
	        {
				B_SAMPLE.volume = 0;
				B_SAMPLE.volume_biaokuang = 0;
				B_SAMPLE.running_time = B_SAMPLE.running_time_set;
				B_SAMPLE.running_time_by_volume	= 0;			
				B_SAMPLE.jiange_time = B_SAMPLE.jiange_time_set;
				//B_SAMPLE.sample_times保持原来值
				B_sample_file.jy = 0;
				B_sample_file.barometric = 0;
				B_sample_file.temperature = 0;
				B_sample_file.environment_temperature = 0;
				start_ctr_B_pump_task();
				B_save_sample_start();
				sample_pic_refresh_flag = 1;
			}
			B_SAMPLE.running_time = B_SAMPLE.running_time_set;	
			B_SAMPLE.running_time_by_volume	= 0;
			B_SAMPLE.jiange_time = B_SAMPLE.jiange_time_set;
		}
		if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_SAMPLING)
		{
			//累计体积
			if(B_SAMPLE.flow_ctr_select == DF_FLOW_CTR_BIAOKUANG)
			{
				B_SAMPLE.volume += B_SAMPLE.flow / (float)60.0;
			}
			else
			{
				B_SAMPLE.volume += B_SAMPLE.flow_ctr / (float)60.0;
			}
			//标况体积
			B_SAMPLE.volume_biaokuang += B_SAMPLE.flow_biaokuang / (float)60.0;
			//计算平均值，见suanfa.h
			B_avg_count += 1;
			B_sample_file.jy += (B_SAMPLE.jy - B_sample_file.jy) / B_avg_count;
			B_sample_file.temperature += (sample_temperature[DF_B_TEMPERATURE].value - B_sample_file.temperature) / B_avg_count;
			B_sample_file.environment_temperature += (B_SAMPLE.environment_temperature_input - B_sample_file.environment_temperature) / B_avg_count;
			B_sample_file.barometric += (B_SAMPLE.barometric_input - B_sample_file.barometric) / B_avg_count;
			B_SAMPLE.running_time_by_volume++;
			if(SAMPLE_MODE.channel_B == DF_SAMPLE_MODE_BY_TIME)
			{
				if(B_SAMPLE.running_time > 0)
				{
					B_SAMPLE.running_time--;
				}
				if(B_SAMPLE.running_time == 0)		//本次采样结束
				{
					//停止PWM
					stop_ctr_B_pump_task();
					//保存采样文件
					SaveSampleFile_B();
					B_SAMPLE.sample_times++;
					if(B_SAMPLE.sample_times >= B_SAMPLE.sample_times_set)
					{
						B_sample_over_flag = 1;
						SAMPLE_TASK_B.step = DF_SAMPLE_TASK_IDLE;
						B_sample_over();
						sample_pic_refresh_flag = 1;
						return;
					}
					else
					{
						SAMPLE_TASK_B.step = DF_SAMPLE_TASK_INTERVAL;
						B_SAMPLE.jiange_time = B_SAMPLE.jiange_time_set;
						sample_pic_refresh_flag = 1;
					}
				}
			}
			else
			{
				if(B_SAMPLE.sample_volume_set > B_SAMPLE.volume)
				{
					if(B_flow_dis.value > 0)
					{
						B_SAMPLE.running_time = (B_SAMPLE.sample_volume_set - B_SAMPLE.volume) / B_flow_dis.value * (float)60.0;
				    }
					else
					{
						B_SAMPLE.running_time = 360000 - 1;
					}
				}
				else
				{
					B_SAMPLE.running_time = 0;
				}
				if(B_SAMPLE.volume >= B_SAMPLE.sample_volume_set)
				{
					//停止PWM
					stop_ctr_B_pump_task();
					//保存采样文件
					SaveSampleFile_B();
					B_SAMPLE.sample_times++;
					if(B_SAMPLE.sample_times >= B_SAMPLE.sample_times_set)
					{
						B_sample_over_flag = 1;
						SAMPLE_TASK_B.step = DF_SAMPLE_TASK_IDLE;
						B_sample_over();
						sample_pic_refresh_flag = 1;
						return;
					}
					else
					{
						SAMPLE_TASK_B.step = DF_SAMPLE_TASK_INTERVAL;
						B_SAMPLE.jiange_time = B_SAMPLE.jiange_time_set;
						sample_pic_refresh_flag = 1;
					}
				}
			}
			ibuf = TIM_GetCapture3(TIM4);
			if(ibuf >= AB_PUMP_MAX)
			{
				if(B_SAMPLE.flow_biaokuang < (B_SAMPLE.flow_set * (float)0.9))
				{
					B_PWM_count++;
				}
				else
				{
					B_PWM_count = 0;
				}
			}
			else
			{
				B_PWM_count = 0;
			}
			if(B_PWM_count > B_sample_protect_time)
			{
				B_sample_file.success = DF_FAN_ERR;
				goto GOTO_SAMPLE_ERR;
			}
			if(B_SAMPLE.jy < B_max_jy)//
			{ 
				if(++B_Qcount >= B_sample_protect_time)
				{
					B_sample_file.success = DF_JY_OUT;
	GOTO_SAMPLE_ERR:
					//停止PWM
					stop_ctr_B_pump_task();
					B_PWM_count = 0;
					B_Qcount = 0;
					//提示出错信息
					SAMPLE_TASK_B.step = DF_SAMPLE_TASK_ERR;
					sample_pic_refresh_flag = 1;
				}//5秒超压，说明管路有问题
			}
			else
			{
				B_Qcount = 0;
			}
		}
		if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_WAIT)
		{
			//判断当前时间是否为设定时间
			if((B_SAMPLE.start_hour_set == sys_hour) && (B_SAMPLE.start_min_set == sys_min))
			{
				B_SAMPLE.sample_times = 0;
				SAMPLE_TASK_B.step = DF_SAMPLE_TASK_AUTOZERO;
				sample_pic_refresh_flag = 1;
			}
		}
		if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_INTERVAL)
		{
			//间隔时间计数（递减）
			//若时间为0，则启动采样
			if(B_SAMPLE.jiange_time > 0)
			{
				B_SAMPLE.jiange_time--;
			}
			if(B_SAMPLE.jiange_time == 0)
			{
				if(B_SAMPLE.jiange_time_set != 0)	//若间隔时间设置为0，则不必校零
				{
					SAMPLE_TASK_B.step = DF_SAMPLE_TASK_AUTOZERO;
				}
                else
				{
					SAMPLE_TASK_B.step = DF_SAMPLE_TASK_SAMPLING;
				}
				sample_pic_refresh_flag = 1;
			}
		}
		if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_ERR)
		{
		
		}
		//掉电保护
		B_power_lose_protect(SAMPLE_TASK_B.step);
	}
}


u8 sample_pic_channel;
u8 sample_pic_select;
//u16 cutpic_channel_A[4] = {210,72,250,92};
//u16 cutpic_channel_B[4] = {210,172,250,192};
u16 not_cutpic[4] = {0,0,1,1};
u16 cutpic_sampling[4] = {210,72,250,92};
void sample_channel_and_select_refresh(void)
{
	if(SAMPLE_TASK_A.step == DF_SAMPLE_TASK_IDLE)
	{
		DMT_CutPicture(0,0,DF_SAMPLE_PIC_CHANNEL_A_ADDR,DF_PIC_SAMPLE_A_SET,not_cutpic);
	}
	else
	{
		if(sample_pic_channel == DF_SAMPLE_CHANNEL_A)
		{
			DMT_CutPicture(0,0,DF_SAMPLE_PIC_CHANNEL_A_ADDR,DF_PIC_SAMPLE_A_SET,not_cutpic);
		}
		else
		{
			DMT_CutPicture(3,55,DF_SAMPLE_PIC_CHANNEL_A_ADDR,DF_PIC_ICON_3,cutpic_sampling);
		}
		
	}
	if(SAMPLE_TASK_B.step == DF_SAMPLE_TASK_IDLE)
	{
		DMT_CutPicture(0,0,DF_SAMPLE_PIC_CHANNEL_B_ADDR,DF_PIC_SAMPLE_A_SET,not_cutpic);
	}
	else
	{
		if(sample_pic_channel == DF_SAMPLE_CHANNEL_B)
		{
			DMT_CutPicture(0,0,DF_SAMPLE_PIC_CHANNEL_B_ADDR,DF_PIC_SAMPLE_A_SET,not_cutpic);
		}
		else
		{
			DMT_CutPicture(3,255,DF_SAMPLE_PIC_CHANNEL_B_ADDR,DF_PIC_ICON_3,cutpic_sampling);
		}
	}
	if(sample_pic_select == DF_SAMPLE_SELECT_SAMPLE)
	{

	}
	if(sample_pic_select == DF_SAMPLE_SELECT_DATA)
	{

	}
	if(sample_pic_select == DF_SAMPLE_SELECT_USB)
	{

	}
	if(sample_pic_select == DF_SAMPLE_SELECT_PRINT)
	{

	}
	if(sample_pic_select == DF_SAMPLE_SET_ALL)
	{
	
	}
}


void sys_check(void)
{
	u16 m;
    u8 i;
	u8 TemperatureTest[8];
	u8 DS1302_EXIST =0;
	u16 jindutiao[4];
    u8 strbuf[20];
    u8 bianhao_buf[10];
	if(RTC_select == DF_RTC_MAIN_BOARD)
	{
		my_RTC_GetTime();
	}
	else
	{
		DMT_GetRTC();
		sys_year = DMT_year;
		sys_month = DMT_month;
		sys_day = DMT_day;
		sys_hour = DMT_hour;
		sys_min = DMT_min;
		sys_sec = DMT_sec;
	}
	DS1302_EXIST = sys_sec;
	DMT_SetBackLight(0x40);
	DMT_ChangePic(DF_PIC_SYS_CHECK);
//	DMT_ScreenLbuf(DF_BIANHAO_ADDR,8,bianhao_new);
#ifdef ZR3500TWO
    SPutl(8,bianhao_buf,bianhao_new);
    strcpy(strbuf,(char*)ZR3500TWO_NAME);
    strcat(strbuf,bianhao_buf);
    DMT_PutStr(DF_BIANHAO_ADDR,(u8 *)strbuf);
    strcpy(strbuf,(char*)ZR3500TWO_VER);
#endif
#ifdef ZR3500TWO_HEAT
    SPutl(8,bianhao_buf,bianhao_new);
    strcpy(strbuf,(char*)ZR3500TWO_HEAT_NAME);
    strcat(strbuf,bianhao_buf);
    DMT_PutStr(DF_BIANHAO_ADDR,(u8 *)strbuf);
    strcpy(strbuf,(char*)ZR3500TWO_HEAT_VER);
#endif
#ifdef ZR3500TWO_HEATandCOOL
    SPutl(8,bianhao_buf,bianhao_new);
    strcpy(strbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
    strcat(strbuf,bianhao_buf);
    DMT_PutStr(DF_BIANHAO_ADDR,(u8 *)strbuf);
    strcpy(strbuf,(char*)ZR3500TWO_HEATandCOOL_VER);
#endif
	DMT_PutStr(DF_PROGRAME_VER_ADDR,(u8 *)strbuf);
	W_24(ADDR_APP_VER+0,strbuf[0]);
	W_24(ADDR_APP_VER+1,strbuf[1]);
	W_24(ADDR_APP_VER+2,strbuf[2]);
	W_24(ADDR_APP_VER+3,strbuf[3]);
	W_24(ADDR_APP_VER+4,strbuf[4]);
	W_24(ADDR_APP_VER+5,0);
	for(m = 0;m<20;m++)
	{
		jindutiao[0] = 32;
		jindutiao[1] = 377;
		jindutiao[2] = m * 28 + 32;
		jindutiao[3] = 394;
		DMT_CutPicture(186,339,DF_SYS_CHECK_CUT,DF_PIC_ICON_2,jindutiao);
		delay_ms(100);
		if(m == 2)
		{
			W_24(ADDR_TEST,0x12);
			if(R_24(ADDR_TEST) == 0x12)
			{
				FMc64_Flag = 1;
			}
			if(FMc64_Flag == 0)
			{
				DMT_PutStr(DF_SYS_CHECK_MSG_ADDR,(u8 *)"存储器故障!     ");
				BEEP_ON();
				delay_ms(1000);
				BEEP_OFF();
				delay_ms(1000);
			}
			DS18b20_reset(TemperatureTest);
		}//m=2
		if(m == 3)
		{
			DMT_ChangePic(DF_PIC_SYS_CHECK);
		}
		if(m == 6)
		{
			DS18b20_reset(TemperatureTest);
			if(TemperatureTest[DF_ENVIRONMENT_TEMPERATURE] == 1)
			{
				DMT_PutStr(DF_SYS_CHECK_MSG_ADDR,(u8 *)"环境温度故障!   ");
				BEEP_ON();
				delay_ms(1000);
				BEEP_OFF();
				delay_ms(1000);
			}
		}

		if(m == 8)
		{    
			DS18b20_reset(TemperatureTest);
			if(TemperatureTest[DF_A_TEMPERATURE] == 1)
			{
				DMT_PutStr(DF_SYS_CHECK_MSG_ADDR,(u8 *)"计前温度A故障!  ");
				BEEP_ON();
				delay_ms(1000);
				BEEP_OFF();
				delay_ms(1000);
			}
		}
		if(m == 9)
		{    
			DS18b20_reset(TemperatureTest);
			if(TemperatureTest[DF_B_TEMPERATURE] == 1)
			{
				DMT_PutStr(DF_SYS_CHECK_MSG_ADDR,(u8 *)"计前温度B故障!  ");
				BEEP_ON();
				delay_ms(1000);
				BEEP_OFF();
				delay_ms(1000);
			}
		}
#ifdef ZR3500TWO
        if(m == 12)
		{    
		
		}  
#endif
#ifdef ZR3500TWO_HEAT
		if(m == 12)
		{    
			DS18b20_reset(TemperatureTest);
			if(TemperatureTest[DF_CTR_TEMPERATURE] == 1)
			{
				DMT_PutStr(DF_SYS_CHECK_MSG_ADDR,(u8 *)"保温箱温度故障! ");
				BEEP_ON();
				delay_ms(1000);
				BEEP_OFF();
				delay_ms(1000);
			}
		}
#endif
#ifdef ZR3500TWO_HEATandCOOL
		if(m == 12)
		{    
			DS18b20_reset(TemperatureTest);
			if(TemperatureTest[DF_CTR_TEMPERATURE] == 1)
			{
				DMT_PutStr(DF_SYS_CHECK_MSG_ADDR,(u8 *)"保温箱温度故障! ");
				BEEP_ON();
				delay_ms(1000);
				BEEP_OFF();
				delay_ms(1000);
			}
		}
#endif

		/*//蓝牙不用检测，若出现不能打印情况可以到维护中查看蓝牙故障原因
		if(m == 13)
		{
			if(bluetooth_ok_flag != 1)
			{
				DMT_PutStr(DF_SYS_CHECK_MSG_ADDR,(u8 *)"蓝牙故障!      ");
				BEEP_ON();
				delay_ms(1000);
				BEEP_OFF();
				delay_ms(1000);
			}
		}*/
		if(m == 15)
		{
			if(RTC_select == DF_RTC_MAIN_BOARD)
			{
				my_RTC_GetTime();
			}
			else
			{
				DMT_GetRTC();
				sys_year = DMT_year;
				sys_month = DMT_month;
				sys_day = DMT_day;
				sys_hour = DMT_hour;
				sys_min = DMT_min;
				sys_sec = DMT_sec;
			}
			if(DS1302_EXIST == sys_sec)
			{
				DS1302_EXIST = 0;
				DMT_PutStr(DF_SYS_CHECK_MSG_ADDR,(u8 *)"时钟故障!       ");
				BEEP_ON();
				delay_ms(1000);
				BEEP_OFF();
				delay_ms(1000);
			}
			GetTemperature();
		}
		if(m == 16)
		{
			bak_flag = R_24(ADDR_bak_flag);
			if(bak_flag != 1)
			{
				DMT_PutStr(DF_SYS_CHECK_MSG_ADDR,(u8 *)"系统无备份!     ");
				BEEP_ON();
                delay_ms(1000);
				BEEP_OFF();
				delay_ms(1000);
			}
		}
		if(m == 18)
		{

		}
	}
}


void LED_task_init(void)
{
	LED_TASK.step = DF_LED_ON;
}
void LED_task(void)
{
	if(LED_TASK.step == DF_LED_ON)
	{
		if(LED_DELAY.flag == 1)				//亮0.5秒，灭0.5秒
		{
			LED_DELAY.flag = 0;
			LED = 0;
			timer_init(&LED_DELAY,10);	
			LED_TASK.step = DF_LED_OFF;
		}	
	}
	else
	{
		if(LED_DELAY.flag == 1)	
		{
			LED_DELAY.flag = 0;
			LED = 1;
			timer_init(&LED_DELAY,10);	
			LED_TASK.step = DF_LED_ON;
		}
	}
}


