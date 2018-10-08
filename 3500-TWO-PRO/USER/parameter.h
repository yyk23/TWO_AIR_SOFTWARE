

#ifndef __PARAMETER_H_
#define __PARAMETER_H_ 1
#include "sys.h"
#include "wk.h"

#define DF_CTR_MODE					1

#define DF_RTC_MAIN_BOARD 			0
#define DF_RTC_DMT					1
extern u8 RTC_select;
extern u8 bianhao[];						//仪器编号
struct POWER_TIME
{
	u8 status;							//掉电时所处的状态
	u8 year;
	u8 month;
	u8 day;
	u8 hour;
	u8 min;
	u8 sec;
};

extern struct POWER_TIME A_power_lose,A_sample_start;
extern struct POWER_TIME B_power_lose,B_sample_start;
struct FILE_INDEX
{
	u8 fg_flag;
	u16 num;
};
extern struct FILE_INDEX AB_sample_file,AB_power_lose_file;

#define DF_CHANNEL_A		0
#define DF_CHANNEL_B		1

#define DF_SUCCESS	1					//采样成功
#define DF_JY_OUT	2					//计压过大保护
#define DF_FAN_ERR	3					//流量达不到保护
struct SAPMLE_FILE                      //共38个字节
{
	u8 channel;						//A\B\C\D采样
    u8 year;               			//采样日期时间
    u8 month;
	u8 day;
	u8 hour;
	u8 min;
	u8 sec;
	u8 success;						//采样成功，负压过大时此值为2,风机流量达不到时为3,其他忽略
	u32 running_time;              	//采样时长
    float flow_set;                 	//流量
    float volume;            			//实际体积
	float volume_biaokuang;				//标况体积
    float jy;              				//采样负压
    float temperature;          		//温度
	float environment_temperature;		//环境温度
    float barometric;           		//大气压
};
extern struct SAPMLE_FILE A_sample_file,B_sample_file,sample_file_chaxun;

struct POWER_FILE
{
	u8 channel;						//A\B\C\D采样
    u8 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 min;
    u8 sec;
    u8 power_year;
    u8 power_month;
    u8 power_day;
    u8 power_hour;
    u8 power_min;
    u8 power_sec;
    u8 lose_hour;
    u8 lose_min;
    u8 lose_sec;
};
extern struct POWER_FILE A_power_file,B_power_file,power_file_chaxun;

struct LCD_LIGHT
{
	u8 mode;
	u16 time_set;
	volatile u16 time;
};
extern struct LCD_LIGHT lcd_light_ctr; 

struct SAMPLE_CHANNEL
{
	u8 A_select_flag;
	u8 B_select_flag;
};
extern struct SAMPLE_CHANNEL sample_select_channel;
extern u8 bak_flag;  //是否备份标志
extern u8 FMc64_Flag;//测试24c64是否出错

extern u16 print_file_step;
extern u32 bianhao_new;
extern u16 password;
extern u16 print_start_file,print_end_file;
extern u8 err_dis;

extern u8 file_format;
extern u8 A_sample_protect_time;
extern u8 B_sample_protect_time;
extern float A_max_jy;
extern float B_max_jy;

extern u16 A_PWM_01L;
extern u16 B_PWM_01L;
extern u16 A_PWM_10L;
extern u16 B_PWM_10L;

extern u8 wk_enable;	

extern float wk_temperature;
extern u16 wk_temperature_set;
extern u16 printer_password;

#define DF_FILE_HEAD_LILE	1
#define DF_FILE_CONTENT		2
#define DF_FILE_CLOSED		3

//实际状态定义方法，需要在对应处理上再整合
#define DF_SAMPLE_STATUS_WAIT_CONTINUE		0		//需要继续等待，等待
#define DF_SAMPLE_STATUS_WAIT_OK			1		//等待时间到，采样
#define DF_SAMPLE_STATUS_WAIT_INVALID		2		//等待超时，无效
#define DF_SAMPLE_STATUS_RUN_CONTINUE		3		//继续采样，采样
#define DF_SAMPLE_STATUS_RUN_INVALID		4		//掉电超时，无效
#define DF_SAMPLE_STATUS_JIANGE_CONTINUE	5		//继续间隔等待状态，间隔等待
#define DF_SAMPLE_STATUS_JIANGE_OK			6		//间隔时间到，采样
#define DF_SAMPLE_STATUS_JIANGE_INVALID		7		//间隔超时，无效
//按照对应处理定义方法
#define DF_SAMPLE_STATUS_INVALID			0		//超时，无效
#define DF_SAMPLE_STATUS_WAIT_RUN			1		//等待采样状态
#define DF_SAMPLE_STATUS_TO_RUN				2		//立即采样状态
#define DF_SAMPLE_STATUS_CONTINUE_RUN		3		//继续未完成的采样状态
extern u8 power_low_sample_status;						//掉电再来电应有的状态
extern u8 A_power_low_sample_status;			
extern u8 B_power_low_sample_status;
extern const u8 sf30[];
extern const u8 sf31[];
extern const u8 sf40[];
extern const u8 sf41[];
extern const u8 sf42[];
extern const u8 sf51[];

extern const u8 f20[];
extern const u8 f21[];
extern const u8 f30[];
extern const u8 f31[];
extern const u8 f32[];
extern const u8 f40[];
extern const u8 f41[];
extern const u8 f42[];
extern const u8 f43[];
extern const u8 f50[];
extern const u8 f51[];
extern const u8 f52[];
extern const u8 f54[];
extern const u8 f60[];
extern const u8 f62[];
//extern const u8 f71[];
extern const u8 f81[];

extern u32 A_LostPowerTime;					//A路掉电时间
extern u32 B_LostPowerTime;					//B路掉电时间
extern u32 A_avg_count;
extern u32 B_avg_count;

extern u8 A_sample_over_flag;
extern u8 B_sample_over_flag;
extern u8 A_Qcount;
extern u8 B_Qcount;
extern u8 A_PWM_count;
extern u8 B_PWM_count;


u32 GetLostTime(void);
u32 A_GetLostTime(void);
u32 B_GetLostTime(void);
void power_lose_test(void);
void A_power_test(void);
void B_power_test(void);
u32 JiCai_Own(u8 year,u8 month,u8 day,u8 hour,u8 min,u8 sec);

void A_power_lose_protect(u8 sample_status);
void A_power_lose_resume(void);
void A_save_sample_start(void);
void A_sample_over(void);
void B_power_lose_protect(u8 sample_status);
void B_power_lose_resume(void);
void B_save_sample_start(void);
void B_sample_over(void);

void PrintSampleFile(u16 start_file,u16 end_file);
void PrintPowerFile(u16 start_file,u16 end_file);
void power_lose_resume(void);
void A_calc_flow_rate(float fbuf);
void B_calc_flow_rate(float fbuf);
void power_lose_protect(u8 sample_status);

float A_stabilize_flow_dis(float A_flow);
float B_stabilize_flow_dis(float B_flow);
void power_on_to_sample(void);
float point_rate_A_flow(float fbuf);
float point_rate_B_flow(float fbuf);
#endif

