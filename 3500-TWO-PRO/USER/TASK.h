#ifndef __TASK_H_
#define __TASK_H_ 1

#include "sys.h"
#include "parameter.h"
#include "DMT_ADDRESS.h"


struct WND_STRUCT
{
	u8 current;		//当前页面
	u8 last;		//上个页面
	u8 change_pic;	//切换图片
};
extern struct WND_STRUCT wnd_struct;



struct OWNTIMER
{
    u8 flag;
	u16 set;
	u16 reload;
};
extern struct OWNTIMER REFRESH_DELAY,REFRESH_SHARING_DELAY,LED_DELAY;
extern struct OWNTIMER ADC_1000ms;
extern struct OWNTIMER DS18B20_1000ms;
extern struct OWNTIMER PUMP_A_1000ms,PUMP_B_1000ms;
extern struct OWNTIMER SAMPLE_A_1000ms,SAMPLE_B_1000ms;
extern struct OWNTIMER USB_1000ms,PRINT_DELAY;
extern struct OWNTIMER EMPLOY_DELAY;
extern struct OWNTIMER DS1302_DELAY;
extern struct OWNTIMER BACKLIGHT_DELAY;
extern struct OWNTIMER BLUETOOTH_DELAY;
extern struct OWNTIMER WK_DELAY;
extern struct OWNTIMER SSDL_CONVERT_DELAY,SSDL_1000ms;
extern struct OWNTIMER DMT_TEST_DELAY;
#define DF_SAMPLE_MODE_BY_TIME			0
#define DF_SAMPLE_MODE_BY_VOLUME		1
#define DF_SAMPLE_CHANNEL_NOT_SELECTED	0
#define DF_SAMPLE_CHANNEL_SELECTED		1
struct STRUCT_SAMPLE_MODE
	{
	u8 channel_A;
	u8 channel_B;
	u8 channel_A_selected;
	u8 channel_B_selected;
	};
extern struct STRUCT_SAMPLE_MODE SAMPLE_MODE;
struct PUMP_CTR_TASK
	{
	u8 enable;
	u8 step;
	u8 count;
	};
extern struct PUMP_CTR_TASK PUMP_CTR_TASK_A,PUMP_CTR_TASK_B;

#define DF_SAMPLE_TASK_IDLE					0
#define DF_SAMPLE_TASK_WAIT					1
#define DF_SAMPLE_TASK_INTERVAL				2
#define DF_SAMPLE_TASK_SAMPLING				3
#define DF_SAMPLE_TASK_AUTOZERO				4
#define DF_SAMPLE_TASK_ERR					5
#define DF_SAMPLE_TASK_CONTINUE_SAMPLING	6
struct SAMPLE_TASK
	{
	u8 enable;
	u8 step;
	u8 count;
	u8 timeout;
	};
extern struct SAMPLE_TASK SAMPLE_TASK_A,SAMPLE_TASK_B;

#define DF_SAMPLE_CHANNEL_A			1
#define DF_SAMPLE_CHANNEL_B			2
#define DF_SAMPLE_SELECT_SAMPLE		1
#define DF_SAMPLE_SELECT_DATA		2
#define DF_SAMPLE_SELECT_DATA_POWER	3
#define DF_SAMPLE_SELECT_USB		4
#define DF_SAMPLE_SELECT_PRINT		5
#define DF_SAMPLE_SELECT_DATA_RUN	6
#define DF_SAMPLE_SET_ALL			7
extern u8 sample_pic_channel;
extern u8 sample_pic_select;
extern u8 sample_pic_refresh_flag;



#define DF_USB_IDLE					0
#define DF_USB_INIT					1
#define DF_UDISK_ERR				2
#define DF_USB_CONNECT				3
#define DF_USB_CONNECT_ERR			4
#define DF_USB_DISK_STATUS			5
#define DF_USB_DISK_OK				6
#define DF_USB_FREE_CAP				7
#define DF_USB_CREAT_DIR			8
#define DF_USB_CREAT_FILE			9
#define DF_USB_ONE_FILE				10
#define DF_USB_CLOSE_FILE			11
#define DF_USB_OUT_SUCCESS			12

struct USB_TASK
	{
	u8 enable;
	u8 step;
	u8 count;
	};
extern struct USB_TASK USB_OUT_SAMPLE,USB_OUT_POWER;

#define DF_PRINT_IDLE					0
#define DF_PRINT_INIT					1
#define DF_PRINT_CONNECT				2
#define DF_PRINT_CONNECT_ERR			3
#define DF_PRINT_FILE					4
#define DF_PRINT_FILE_OK				5
struct PRINT_TASK
	{
	u8 enable;
	u8 step;
	u8 count;
	u16 i;
	};
extern struct PRINT_TASK PRINT_SAMPLE,PRINT_POWER;

//step
#define DF_BLUETOOTH_IDLE				0
#define DF_BLUETOOTH_INIT				1
#define DF_BLUETOOTH_POWER_ON			2
#define DF_BLUETOOTH_RESET				3
#define DF_BLUETOOTH_CONNECT			4
#define DF_BLUETOOTH_CONNECT_TIMEOUT	5
//status
#define DF_STATUS_BLUETOOTH_NOT_CONNECTED		0
#define DF_STATUS_BLUETOOTH_CONNECTED			1
#define DF_STATUS_BLUETOOTH_CONNECTED_TIMEOUT	2
struct BLUETOOTH_TASK
	{
	u8 enable;
	u8 step;
	u8 connected_flag;
	u8 count;
	u16 i;
	u16 timeout;
	u16 close_time_set;
	
	};
extern struct BLUETOOTH_TASK BLUETOOTH;


struct FLOW_DIS
	{
		float value;
		float buf[11];
		u8 i;
	};
extern struct FLOW_DIS A_flow_dis,B_flow_dis;

#define DF_LED_ON						0
#define DF_LED_OFF						1
struct LED_TASK_STRUCT
{
	u8 step;
	u8 count;
};
extern struct LED_TASK_STRUCT LED_TASK;

extern u16 display_current_pic;
extern u16 display_last_pic;
extern u16 back_light_min_set;
extern float kedu_temperature_set;

void set_refresh_wnd_num(u8 new);		//显示当前页面
void set_refresh_last_wnd_num(void);
void timer_init(struct OWNTIMER * s,u16 set_value);
void timer_task_init(void);
void creat_timer_flag(struct OWNTIMER * s,u16 delta);
void timer_task(void);
void uart_task(void);
void key_task(void);
void display_task(void);
void usb_out_sample_file_task(void);

void bluetooth_task_init(void);
void bluetooth_connect_task(void);
void clear_bluetooth_timeout(void);

void ADC_task_init(void);
void ADC_task(void);
void calc_flow_task(void);
void read_temperature_task(void);

void ctr_pump_task_init(void);
void start_ctr_A_pump_task(void);
void stop_ctr_A_pump_task(void);
void ctr_A_pump_task(void);
void start_ctr_B_pump_task(void);
void stop_ctr_B_pump_task(void);
void ctr_B_pump_task(void);

void A_auto_zero(void);
void B_auto_zero(void);

void sample_A_task_init(void);
void stop_sample_A_task(void);
void sample_A_task(void);
void sample_B_task_init(void);
void stop_sample_B_task(void);
void sample_B_task(void);

void sample_channel_and_select_refresh(void);
void usb_out_power_file_task(void);
void print_sample_file_task(void);
void print_power_file_task(void);

void sample_channel_and_select_refresh(void);
void sys_check(void);
void DS1302_task(void);
void display_sleep_init(void);
void DMT_refresh_para(void);

void LED_task_init(void);
void LED_task(void);
void bat_symbol_display(void);
void chatou_symbol_display(void);

void SSDL_task(void);
#endif

