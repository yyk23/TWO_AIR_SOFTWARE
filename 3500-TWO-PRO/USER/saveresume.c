
#include "saveresume.h"
#include "24c64.h"
#include "sample.h"
#include "pump.h"
#include "print.h"
#include "process.h"
#include "TASK.h"
#include "bat.h"
void SavePara(u16 offset)
{
	u8 flow_rate_count;
	savefloat(ADDR_barometric_zero + offset,barometric.zero);
	savefloat(ADDR_barometric_rate + offset,barometric.rate);	
	savefloat(ADDR_A_SAMPLE_dy_rate + offset,A_SAMPLE.dy_rate);
	savefloat(ADDR_B_SAMPLE_dy_rate + offset,B_SAMPLE.dy_rate);
	savefloat(ADDR_A_SAMPLE_jy_rate + offset,A_SAMPLE.jy_rate);
    savefloat(ADDR_B_SAMPLE_jy_rate + offset,B_SAMPLE.jy_rate);
	savefloat(ADDR_bat_voltage_rate + offset,bat_voltage_rate);
	savefloat(ADDR_sample_temperature_ENVIRONMENT_zero + offset,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].zero);
	savefloat(ADDR_sample_temperature_CTR_zero + offset,sample_temperature[DF_CTR_TEMPERATURE].zero);
	savefloat(ADDR_sample_temperature_A_zero + offset,sample_temperature[DF_A_TEMPERATURE].zero);
	savefloat(ADDR_sample_temperature_B_zero + offset,sample_temperature[DF_B_TEMPERATURE].zero);
	saveintdata(ADDR_password + offset,password);
	savelongdata(ADDR_bianhao + offset,bianhao_new);
    W_24(ADDR_RTC_select + offset,RTC_select);
	W_24(ADDR_err_dis + offset,err_dis);
	for(flow_rate_count = 1;flow_rate_count < 11;flow_rate_count++)
	{
		savefloat(ADDR_A_flow_rate_rate_array + 4 * flow_rate_count + offset,A_flow_rate.rate_array[flow_rate_count]);
		savefloat(ADDR_B_flow_rate_rate_array + 4 * flow_rate_count + offset,B_flow_rate.rate_array[flow_rate_count]);
	}
	saveintdata(ADDR_A_PWM_01L + offset,A_PWM_01L);
	saveintdata(ADDR_A_PWM_10L + offset,A_PWM_10L);
	saveintdata(ADDR_B_PWM_01L + offset,B_PWM_01L);
	saveintdata(ADDR_B_PWM_10L + offset,B_PWM_10L);
	savefloat(ADDR_pid_para_pgain + offset,wk_pid_para.pgain);
	savefloat(ADDR_pid_para_igain + offset,wk_pid_para.igain);
	savefloat(ADDR_pid_para_dgain + offset,wk_pid_para.dgain);
	savefloat(ADDR_pid_para_deadband + offset,wk_pid_para.deadband);
	W_24(ADDR_pid_integral_limit + offset,pid_integral_limit);
	savefloat(ADDR_wk_start_heat_err + offset,wk_start_heat_err);
}

void ReadPara(u16 offset)
{
	u8 flow_rate_count;
	barometric.zero = readfloat(ADDR_barometric_zero + offset);
	barometric.rate = readfloat(ADDR_barometric_rate + offset);	
	A_SAMPLE.dy_rate = readfloat(ADDR_A_SAMPLE_dy_rate + offset);
	B_SAMPLE.dy_rate = readfloat(ADDR_B_SAMPLE_dy_rate + offset);
	A_SAMPLE.jy_rate = readfloat(ADDR_A_SAMPLE_jy_rate + offset);
    B_SAMPLE.jy_rate = readfloat(ADDR_B_SAMPLE_jy_rate + offset);
	bat_voltage_rate = readfloat(ADDR_bat_voltage_rate + offset);
	sample_temperature[DF_ENVIRONMENT_TEMPERATURE].zero = readfloat(ADDR_sample_temperature_ENVIRONMENT_zero + offset);
	sample_temperature[DF_CTR_TEMPERATURE].zero = readfloat(ADDR_sample_temperature_CTR_zero + offset);
	sample_temperature[DF_A_TEMPERATURE].zero = readfloat(ADDR_sample_temperature_A_zero + offset);
	sample_temperature[DF_B_TEMPERATURE].zero = readfloat(ADDR_sample_temperature_B_zero + offset);
	password = readintdata(ADDR_password + offset);
	bianhao_new = readlongdata(ADDR_bianhao + offset);
	SPutl(8,bianhao,bianhao_new);
    RTC_select = R_24(ADDR_RTC_select + offset);
	err_dis = R_24(ADDR_err_dis + offset);
	for(flow_rate_count = 1;flow_rate_count < 11;flow_rate_count++)
	{
		A_flow_rate.rate_array[flow_rate_count] = readfloat(ADDR_A_flow_rate_rate_array + 4 * flow_rate_count + offset);
		B_flow_rate.rate_array[flow_rate_count] = readfloat(ADDR_B_flow_rate_rate_array + 4 * flow_rate_count + offset);
	}
	A_PWM_01L = readintdata(ADDR_A_PWM_01L + offset);
	A_PWM_10L = readintdata(ADDR_A_PWM_10L + offset);
	B_PWM_01L = readintdata(ADDR_B_PWM_01L + offset);
	B_PWM_10L = readintdata(ADDR_B_PWM_10L + offset);
	wk_pid_para.pgain = readfloat(ADDR_pid_para_pgain + offset);
	wk_pid_para.igain = readfloat(ADDR_pid_para_igain + offset);
	wk_pid_para.dgain = readfloat(ADDR_pid_para_dgain + offset);
	wk_pid_para.deadband = readfloat(ADDR_pid_para_deadband + offset);
	pid_integral_limit = R_24(ADDR_pid_integral_limit + offset);
	wk_start_heat_err = readfloat(ADDR_wk_start_heat_err + offset);
}

void ParaLimit(void)
{
	
}
//重置用户设置参数并保存到存储器
void reset_para(void)
{
	sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode = !DF_INPUT_MODE;//温度选择模式
    sample_temperature[DF_ENVIRONMENT_TEMPERATURE].input = 20;//温度输入值
	barometric.mode = !DF_INPUT_MODE;
	barometric.input = 101;
	A_SAMPLE.flow_set = 0.5;
	A_SAMPLE.running_time_set = 3600;
	A_SAMPLE.start_hour_set = 9;
	A_SAMPLE.start_min_set = 0;
	A_SAMPLE.jiange_time_set = 3600;
	A_SAMPLE.sample_times_set = 2;
	A_power_lose.status = 0;
	B_SAMPLE.flow_set = 0.5;
	B_SAMPLE.running_time_set = 3600;
	B_SAMPLE.start_hour_set = 9;
	B_SAMPLE.start_min_set = 0;
	B_SAMPLE.jiange_time_set = 3600;
	B_SAMPLE.sample_times_set = 2;
	B_power_lose.status = 0;
	lcd_light_ctr.mode = !DF_CTR_MODE;
	lcd_light_ctr.time_set = 100;
	file_format = 1;
	wk_temperature_set = 20;
	wk_enable = 1;
	back_light_min_set = 20;//背光时间20分钟
	BLUETOOTH.close_time_set = 300;

	sample_select_channel.A_select_flag = 0;
	sample_select_channel.B_select_flag = 0;	
	
	A_SAMPLE.flow_ctr_select = DF_FLOW_CTR_KEDU;
	B_SAMPLE.flow_ctr_select = DF_FLOW_CTR_KEDU;
	B_SAMPLE.flow_ctr_select = A_SAMPLE.flow_ctr_select;
	kedu_temperature_set = 20.0;
	
	W_24(ADDR_environment_temperature_mode,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode);//温度选择模式
    savefloat(ADDR_environment_temperature_input,sample_temperature[DF_ENVIRONMENT_TEMPERATURE].input);//温度输入值
	W_24(ADDR_barometric_mode,barometric.mode);
	savefloat(ADDR_barometric_input,barometric.input);

	savefloat(ADDR_A_SAMPLE_flow_set,A_SAMPLE.flow_set);
	savelongdata(ADDR_A_SAMPLE_running_time_set,A_SAMPLE.running_time_set);
	W_24(ADDR_A_SAMPLE_start_hour_set,A_SAMPLE.start_hour_set);
	W_24(ADDR_A_SAMPLE_start_min_set,A_SAMPLE.start_min_set);
	savelongdata(ADDR_A_SAMPLE_jiange_time_set,A_SAMPLE.jiange_time_set);
	W_24(ADDR_A_SAMPLE_sample_times_set,A_SAMPLE.sample_times_set);
	W_24(ADDR_A_power_lose_status,A_power_lose.status);

	savefloat(ADDR_B_SAMPLE_flow_set,B_SAMPLE.flow_set);
	savelongdata(ADDR_B_SAMPLE_running_time_set,B_SAMPLE.running_time_set);
	W_24(ADDR_B_SAMPLE_start_hour_set,B_SAMPLE.start_hour_set);
	W_24(ADDR_B_SAMPLE_start_min_set,B_SAMPLE.start_min_set);
	savelongdata(ADDR_B_SAMPLE_jiange_time_set,B_SAMPLE.jiange_time_set);
	W_24(ADDR_B_SAMPLE_sample_times_set,B_SAMPLE.sample_times_set);
	W_24(ADDR_B_power_lose_status,B_power_lose.status);
	
	saveintdata(ADDR_BLUETOOTH_close_time_set,BLUETOOTH.close_time_set);
	W_24(ADDR_lcd_light_ctr_mode,lcd_light_ctr.mode);
	saveintdata(ADDR_lcd_light_ctr_time_set,lcd_light_ctr.time_set);
	W_24(ADDR_file_format,file_format);
	saveintdata(ADDR_wk_temperature_set,wk_temperature_set);
	W_24(ADDR_wk_enable,wk_enable);
	saveintdata(ADDR_back_light_min_set,back_light_min_set);

	W_24(ADDR_sample_select_channel_A_select_flag,sample_select_channel.A_select_flag);
	W_24(ADDR_sample_select_channel_B_select_flag,sample_select_channel.B_select_flag);
	
	savefloat(ADDR_kedu_temperature_set,kedu_temperature_set);
	
	W_24(ADDR_A_sample_flow_ctr_select,A_SAMPLE.flow_ctr_select);
	W_24(ADDR_B_sample_flow_ctr_select,B_SAMPLE.flow_ctr_select);
	
	SAMPLE_MODE.channel_A = DF_SAMPLE_MODE_BY_TIME;
	SAMPLE_MODE.channel_B = DF_SAMPLE_MODE_BY_TIME;
	
	W_24(ADDR_A_SAMPLE_MODE,SAMPLE_MODE.channel_A);
	W_24(ADDR_B_SAMPLE_MODE,SAMPLE_MODE.channel_B);
	
	A_SAMPLE.sample_volume_set = 30.0;
	B_SAMPLE.sample_volume_set = 30.0;
	savefloat(ADDR_A_SAMPLE_sample_volume_set,A_SAMPLE.sample_volume_set);
	savefloat(ADDR_B_SAMPLE_sample_volume_set,B_SAMPLE.sample_volume_set);
}


void ParaInit(void)
{
	if((R_24(ADDR_first_start) != 0x12) && (R_24(ADDR_first_start+1) != 0x12))
	{
		barometric.zero = 500;
		barometric.rate = 1;	
		A_SAMPLE.dy_rate = 1;
		A_SAMPLE.jy_rate = 1;
		A_SAMPLE.flow_set = 1;
		B_SAMPLE.dy_rate = 1;
		B_SAMPLE.jy_rate = 1;
		B_SAMPLE.flow_set = 1;
        A_SAMPLE.dy_zero = 500;
		B_SAMPLE.dy_zero = 500;
		A_SAMPLE.jy_zero = 500;
		B_SAMPLE.jy_zero = 500;
		bat_voltage_rate = 1.0;
		charge_current_rate = 1.0;
		sample_temperature[DF_ENVIRONMENT_TEMPERATURE].zero = 110;
		sample_temperature[DF_CTR_TEMPERATURE].zero = 110;
		sample_temperature[DF_A_TEMPERATURE].zero = 110;
		sample_temperature[DF_B_TEMPERATURE].zero = 110;
		password = 2007;
		err_dis = 1;     
		RTC_select = DF_RTC_MAIN_BOARD;
		A_flow_rate.rate_array[1] = 1.0;
		A_flow_rate.rate_array[2] = 1.0;
		A_flow_rate.rate_array[3] = 1.0;
		A_flow_rate.rate_array[4] = 1.0;
		A_flow_rate.rate_array[5] = 1.0;
		A_flow_rate.rate_array[6] = 1.0;
		A_flow_rate.rate_array[7] = 1.0;
		A_flow_rate.rate_array[8] = 1.0;
		A_flow_rate.rate_array[9] = 1.0;
		A_flow_rate.rate_array[10] = 1.0;
		B_flow_rate.rate_array[1] = 1.0;
		B_flow_rate.rate_array[2] = 1.0;
		B_flow_rate.rate_array[3] = 1.0;
		B_flow_rate.rate_array[4] = 1.0;
		B_flow_rate.rate_array[5] = 1.0;
		B_flow_rate.rate_array[6] = 1.0;
		B_flow_rate.rate_array[7] = 1.0;
		B_flow_rate.rate_array[8] = 1.0;
		B_flow_rate.rate_array[9] = 1.0;
		B_flow_rate.rate_array[10] = 1.0;
		A_PWM_01L = 5000;
		A_PWM_10L = 25000;
		B_PWM_01L = 5000;
		B_PWM_10L = 25000;
		wk_pid_para.pgain = 1.0;
		wk_pid_para.igain = 0.03;
		wk_pid_para.dgain = 10.0;
		wk_pid_para.deadband = 0.5;		   //死区0.5度
		pid_integral_limit = 2;
		wk_start_heat_err = 0.5;
		SPutl(8,bianhao,bianhao_new);
		printer_password = 0;
		saveintdata(ADDR_printer_password,printer_password);
        savefloat(ADDR_A_SAMPLE_dy_zero,A_SAMPLE.dy_zero);
		savefloat(ADDR_B_SAMPLE_dy_zero,B_SAMPLE.dy_zero);
		savefloat(ADDR_A_SAMPLE_jy_zero,A_SAMPLE.jy_zero);
		savefloat(ADDR_B_SAMPLE_jy_zero,B_SAMPLE.jy_zero);
		SavePara(0);		//保存标定参数到存储器
		reset_para();		//重置用户设置参数并保存到存储器

		W_24(ADDR_first_start,0x12);
		W_24(ADDR_first_start+1,0x12);
	}	
	else
	{
		ReadPara(0);			//读取标定参数
		//读取用户设置参数
		sample_temperature[DF_ENVIRONMENT_TEMPERATURE].mode = R_24(ADDR_environment_temperature_mode);//温度选择模式
	    sample_temperature[DF_ENVIRONMENT_TEMPERATURE].input = readfloat(ADDR_environment_temperature_input);//温度输入值
		barometric.mode = R_24(ADDR_barometric_mode);
		barometric.input = readfloat(ADDR_barometric_input);

		BLUETOOTH.close_time_set = readintdata(ADDR_BLUETOOTH_close_time_set);
		A_SAMPLE.flow_set = readfloat(ADDR_A_SAMPLE_flow_set);
		A_SAMPLE.running_time_set = readlongdata(ADDR_A_SAMPLE_running_time_set);
		A_SAMPLE.start_hour_set = R_24(ADDR_A_SAMPLE_start_hour_set);
		A_SAMPLE.start_min_set = R_24(ADDR_A_SAMPLE_start_min_set);
		A_SAMPLE.jiange_time_set = readlongdata(ADDR_A_SAMPLE_jiange_time_set);
		A_SAMPLE.sample_times_set = R_24(ADDR_A_SAMPLE_sample_times_set);

		B_SAMPLE.flow_set = readfloat(ADDR_B_SAMPLE_flow_set);
		B_SAMPLE.running_time_set = readlongdata(ADDR_B_SAMPLE_running_time_set);
		B_SAMPLE.start_hour_set = R_24(ADDR_B_SAMPLE_start_hour_set);
		B_SAMPLE.start_min_set = R_24(ADDR_B_SAMPLE_start_min_set);
		B_SAMPLE.jiange_time_set = readlongdata(ADDR_B_SAMPLE_jiange_time_set);
		B_SAMPLE.sample_times_set = R_24(ADDR_B_SAMPLE_sample_times_set);

		lcd_light_ctr.mode = R_24(ADDR_lcd_light_ctr_mode);
		lcd_light_ctr.time_set = readintdata(ADDR_lcd_light_ctr_time_set);
		file_format = R_24(ADDR_file_format);
		if(file_format > 1)
		{
			file_format = 1;
		}
		wk_temperature_set = readintdata(ADDR_wk_temperature_set);
		wk_enable = R_24(ADDR_wk_enable);
		back_light_min_set = readintdata(ADDR_back_light_min_set);

		sample_select_channel.A_select_flag = R_24(ADDR_sample_select_channel_A_select_flag);
		sample_select_channel.B_select_flag = R_24(ADDR_sample_select_channel_B_select_flag);
		A_SAMPLE.flow_ctr_select = R_24(ADDR_A_sample_flow_ctr_select);
		B_SAMPLE.flow_ctr_select = R_24(ADDR_B_sample_flow_ctr_select);
		B_SAMPLE.flow_ctr_select = A_SAMPLE.flow_ctr_select;
		
		kedu_temperature_set = readfloat(ADDR_kedu_temperature_set);
		A_SAMPLE.dy_zero = readfloat(ADDR_A_SAMPLE_dy_zero);
		B_SAMPLE.dy_zero = readfloat(ADDR_B_SAMPLE_dy_zero);
		A_SAMPLE.jy_zero = readfloat(ADDR_A_SAMPLE_jy_zero);
		B_SAMPLE.jy_zero = readfloat(ADDR_B_SAMPLE_jy_zero);
		
		printer_password = readintdata(ADDR_printer_password);
		
		SAMPLE_MODE.channel_A = R_24(ADDR_A_SAMPLE_MODE);
		SAMPLE_MODE.channel_B = R_24(ADDR_B_SAMPLE_MODE);
		
		A_SAMPLE.sample_volume_set = readfloat(ADDR_A_SAMPLE_sample_volume_set);
		B_SAMPLE.sample_volume_set = readfloat(ADDR_B_SAMPLE_sample_volume_set);
	}

	//PID初始化 温度控制采用PID控制方式
	wk_pid_para.integral = 0;
	wk_pid_para.last_error = 0;

	A_max_jy = -30.0; 
	A_sample_protect_time = 60;
	B_max_jy = -30.0;
	B_sample_protect_time = 60;
}

//void ParaBackup(void)
//{
//	SavePara(DF_OFFSET);
//}

void save_sample_mode(void)
{
	W_24(ADDR_A_SAMPLE_MODE,SAMPLE_MODE.channel_A);
	W_24(ADDR_B_SAMPLE_MODE,SAMPLE_MODE.channel_B);
}
void save_sample_para(void)
{
	save_sample_mode();
	savefloat(ADDR_A_SAMPLE_sample_volume_set,A_SAMPLE.sample_volume_set);
	savefloat(ADDR_A_SAMPLE_flow_set,A_SAMPLE.flow_set);
	savelongdata(ADDR_A_SAMPLE_running_time_set,A_SAMPLE.running_time_set);
	W_24(ADDR_A_SAMPLE_start_hour_set,A_SAMPLE.start_hour_set);
	W_24(ADDR_A_SAMPLE_start_min_set,A_SAMPLE.start_min_set);
	savelongdata(ADDR_A_SAMPLE_jiange_time_set,A_SAMPLE.jiange_time_set);
	W_24(ADDR_A_SAMPLE_sample_times_set,A_SAMPLE.sample_times_set);
	
	savefloat(ADDR_B_SAMPLE_sample_volume_set,B_SAMPLE.sample_volume_set);
	savefloat(ADDR_B_SAMPLE_flow_set,B_SAMPLE.flow_set);
	savelongdata(ADDR_B_SAMPLE_running_time_set,B_SAMPLE.running_time_set);
	W_24(ADDR_B_SAMPLE_start_hour_set,B_SAMPLE.start_hour_set);
	W_24(ADDR_B_SAMPLE_start_min_set,B_SAMPLE.start_min_set);
	savelongdata(ADDR_B_SAMPLE_jiange_time_set,B_SAMPLE.jiange_time_set);
	W_24(ADDR_B_SAMPLE_sample_times_set,B_SAMPLE.sample_times_set);  
}



