#ifndef __24C64_H__
#define	__24C64_H__	1


#define ADDR_TEST										10
#define ADDR_bianhao									20
#define ADDR_password									30
#define ADDR_bak_flag									40
#define ADDR_first_start								50

#define ADDR_APP_VER									60		//Ö÷³ÌÐò°æ±¾ºÅ

#define ADDR_A_SAMPLE_MODE                              100
#define ADDR_B_SAMPLE_MODE                              101

#define ADDR_environment_temperature_mode				190
#define ADDR_environment_temperature_input				191
#define ADDR_sample_temperature_ENVIRONMENT_zero		250
#define ADDR_err_dis									260
#define ADDR_RTC_select									603

#define ADDR_barometric									320
#define ADDR_barometric_rate							320
#define ADDR_barometric_zero							324
#define ADDR_barometric_mode 							328
#define ADDR_barometric_input							329

#define ADDR_lcd_light_ctr_mode							370
#define ADDR_lcd_light_ctr_time_set						371

#define ADDR_A_max_jy									480
#define ADDR_A_min_PWM									484
#define ADDR_A_sample_protect_time						486
#define ADDR_A_PWM_01L									487
#define ADDR_A_PWM_10L									489
#define ADDR_B_max_jy									491
#define ADDR_B_min_PWM									495
#define ADDR_B_sample_protect_time						497
#define ADDR_B_PWM_01L									498
#define ADDR_B_PWM_10L									500

#define ADDR_A_power_lose								510
#define ADDR_A_power_lose_status						510
#define ADDR_A_power_lose_year							511
#define ADDR_A_power_lose_month							512
#define ADDR_A_power_lose_day							513
#define ADDR_A_power_lose_hour							514
#define ADDR_A_power_lose_min							515
#define ADDR_A_power_lose_sec							516
#define ADDR_A_SAMPLE									520
#define ADDR_A_SAMPLE_dy_rate							520
#define ADDR_A_SAMPLE_dy_zero							524
#define ADDR_A_SAMPLE_jy_rate							528
#define ADDR_A_SAMPLE_jy_zero							532
#define ADDR_A_SAMPLE_flow_set							536
#define ADDR_A_SAMPLE_flow_rate							540
#define ADDR_A_SAMPLE_running_time_set					544
#define ADDR_A_SAMPLE_start_hour_set					548
#define ADDR_A_SAMPLE_start_min_set						549
#define ADDR_A_SAMPLE_jiange_time_set					553
#define ADDR_A_SAMPLE_sample_times_set					557
#define ADDR_A_SAMPLE_jiange_time						558
#define ADDR_A_SAMPLE_sample_times						562
#define ADDR_A_SAMPLE_running_time						563
#define ADDR_A_SAMPLE_volume							567
#define ADDR_A_SAMPLE_volume_biaokuang					571
#define ADDR_sample_temperature_A_mode					580
#define ADDR_sample_temperature_A_input					581
#define ADDR_A_sample_file_running_time					600
#define ADDR_A_sample_file_flow_set						604
#define ADDR_A_sample_file_volume						608
#define ADDR_A_sample_file_volume_biaokuang				612
#define ADDR_A_sample_file_jy							616
#define ADDR_A_sample_file_temperature					620
#define ADDR_A_sample_file_environment_temperature		624
#define ADDR_A_sample_file_barometric					628
#define ADDR_A_avg_count								632
#define ADDR_sample_temperature_A_zero					654
#define ADDR_A_sample_start								660
#define ADDR_A_sample_start_status						660
#define ADDR_A_sample_start_year						661
#define ADDR_A_sample_start_month						662
#define ADDR_A_sample_start_day							663
#define ADDR_A_sample_start_hour						664
#define ADDR_A_sample_start_min							665
#define ADDR_A_sample_start_sec							666
#define ADDR_A_SAMPLE_sample_volume_set					667
#define ADDR_A_SAMPLE_running_time_by_volume			671

#define ADDR_B_power_lose								710
#define ADDR_B_power_lose_status						710
#define ADDR_B_power_lose_year							711
#define ADDR_B_power_lose_month							712
#define ADDR_B_power_lose_day							713
#define ADDR_B_power_lose_hour							714
#define ADDR_B_power_lose_min							715
#define ADDR_B_power_lose_sec							716
#define ADDR_B_SAMPLE									720
#define ADDR_B_SAMPLE_dy_rate							720
#define ADDR_B_SAMPLE_dy_zero							724
#define ADDR_B_SAMPLE_jy_rate							728
#define ADDR_B_SAMPLE_jy_zero							732
#define ADDR_B_SAMPLE_flow_set							736
#define ADDR_B_SAMPLE_flow_rate							740
#define ADDR_B_SAMPLE_running_time_set					744
#define ADDR_B_SAMPLE_start_hour_set					748
#define ADDR_B_SAMPLE_start_min_set						749
#define ADDR_B_SAMPLE_jiange_time_set					753
#define ADDR_B_SAMPLE_sample_times_set					757
#define ADDR_B_SAMPLE_jiange_time						758
#define ADDR_B_SAMPLE_sample_times						762
#define ADDR_B_SAMPLE_running_time						763
#define ADDR_B_SAMPLE_volume							767
#define ADDR_B_SAMPLE_volume_biaokuang					771
#define ADDR_sample_temperature_B_mode					780
#define ADDR_sample_temperature_B_input					781
#define ADDR_B_sample_file_running_time					800
#define ADDR_B_sample_file_flow_set						804
#define ADDR_B_sample_file_volume						808
#define ADDR_B_sample_file_volume_biaokuang				812
#define ADDR_B_sample_file_jy							816
#define ADDR_B_sample_file_temperature					820
#define ADDR_B_sample_file_environment_temperature		824
#define ADDR_B_sample_file_barometric					828
#define ADDR_B_avg_count								832
#define ADDR_sample_temperature_B_zero					854
#define ADDR_B_sample_start								860
#define ADDR_B_sample_start_status						860
#define ADDR_B_sample_start_year						861
#define ADDR_B_sample_start_month						862
#define ADDR_B_sample_start_day							863
#define ADDR_B_sample_start_hour						864
#define ADDR_B_sample_start_min							865
#define ADDR_B_sample_start_sec							866
#define ADDR_B_SAMPLE_sample_volume_set					867
#define ADDR_B_SAMPLE_running_time_by_volume			871

#define ADDR_sample_temperature_CTR_zero				1300
#define ADDR_bat_voltage_rate							1310

#define ADDR_sample_select_channel_A_select_flag		1400
#define ADDR_sample_select_channel_B_select_flag		1401
#define ADDR_file_format								1410
#define ADDR_wk_temperature_set							1420
#define ADDR_wk_enable									1422
#define ADDR_pid_para_pgain								1424
#define ADDR_pid_para_igain								1428
#define ADDR_pid_para_dgain								1432
#define ADDR_pid_para_deadband							1436
#define ADDR_pid_integral_limit							1440
#define ADDR_wk_start_heat_err							1442

#define ADDR_sample_file_bank							1450
#define ADDR_power_file_bank							1451

#define ADDR_printer_password							1460
#define ADDR_back_light_min_set							1462
#define ADDR_A_sample_flow_ctr_select					1464
#define ADDR_B_sample_flow_ctr_select					1465
#define ADDR_kedu_temperature_set						1466
#define ADDR_BLUETOOTH_close_time_set					1470
#define ADDR_A_flow_rate_rate_array						1500
#define ADDR_B_flow_rate_rate_array						1560

#define DF_OFFSET							2000			//??????

#define ADDR_AB_sample_file_fg_flag					4010
#define ADDR_AB_sample_file_num						4011
#define ADDR_AB_power_lose_file_fg_flag				4015
#define ADDR_AB_power_lose_file_num					4016


void FM24C64_GPIOInit(void);
u8 W_24(u16 addr,u8 d);		//70us
u8 R_24(u16 addr);				//70us
void saveintdata(u16 addr,u16 d);	//140us
u16 readintdata(u16 addr);		//140us
void saveint(u16 addr,u16 d);	//140us
u16 readint(u16 addr);		//140us
void savelongdata(u16 addr,unsigned long int d);		//280us
unsigned long int readlongdata(u16 addr);				//280us
void savefloat(u16 addr,float savevalue);//280us
float readfloat(u16 addr);				//280us

u8 at24c1024_W_24(u32 addr,u8 d);
u8 at24c1024_R_24(u32 addr);
void at24c1024_saveint(u32 addr,u16 d);
u16 at24c1024_readint(u32 addr);
void at24c1024_savefloat(u32 addr,float savevalue);		//280us
float at24c1024_readfloat(u32 addr);
#endif	/*__24C64_H__*/
