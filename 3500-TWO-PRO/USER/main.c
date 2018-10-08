#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "process.h"
#include "adc.h"
#include "timer.h"
#include "pwm.h"
#include "pump.h"
#include "rtc.h"
#include "malloc.h" 
#include "w25qxx.h"    
#include "24c64.h"
#include "sdio_sdcard.h"
#include "ff.h"  
#include "usbh_usr.h" 
#include "exfuns.h" 
#include "parameter.h"
#include "PUMP.h"
#include "TASK.h"
#include "led.h"
#include "bat.h"
#include "saveresume.h"
#include "DMT.h"
#include "print.h"
#include "file.h"
#include "SSDL.h"
#include "18b20.h"
USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

u16 main_i = 0;
int main(void)
{
	SCB->VTOR = FLASH_BASE | 0x20000; /* Vector Table Relocation in Internal FLASH. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);
	LED_Init();
	POWER_TEST_Init();
	wk_GPIOInit();
	TIM1_Int_Init(500-1,16800-1);
	PUMP_AB_PWM_Init();
	FM24C64_GPIOInit();
	DMT_Init();
	My_RTC_Init();		 							//初始化RTC
	W25QXX_Init();									//SPI FLASH初始化 
	my_mem_init(SRAMIN);							//初始化内部内存池	
 	exfuns_init();									//为fatfs相关变量申请内存   
  	f_mount(fs[2],"2:",1); 							//挂载U盘
	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks); 
	ParaInit();
	timer_task_init();
	bluetooth_init();
    DS18b20_init();
	FileInit();
	sys_check();
	DMT_ChangePic(DF_PIC_SYS_CHECK);
    SSDL_port_init();
	ADC_task_init();
	sample_A_task_init();
	sample_B_task_init();
	DMT_refresh_para();	
	power_lose_test();
	err_dis = 1;	
//    DMT_SetTPSensitivity(0x01);
//test_file_write();	
	while(1)
	{
		USH_User_App_flag = DF_USH_USER_APP_INVALID;
		USBH_Process(&USB_OTG_Core, &USB_Host);
		if(main_i < 65535)
		{
			main_i++;
		}
		timer_task();
		uart_task();
		key_task();
		LED_task();
		display_task();
		usb_out_sample_file_task();
		usb_out_power_file_task();
		read_temperature_task();
        SSDL_task();
		ADC_task();
		ctr_A_pump_task();
		ctr_B_pump_task();
		sample_A_task();
		sample_B_task();
		print_sample_file_task();
		print_power_file_task();
		wk_ctr();
		DS1302_task();
		bluetooth_connect_task();
	}
}

FRESULT set_modify_time(char * obj,int year,int month,int day,int hour,int min,int sec)
{
	FILINFO fno;
	fno.fdate = (WORD)(((u32)(2000 + year - 1980) * 512U) | (u32)month * 32U | day);
	fno.ftime = (WORD)((hour * 2048U) | min * 32U | sec / 2U);
	return f_utime(obj,&fno);
}

//用户测试主程序
//返回值:0,正常
//       1,有问题

u8 USH_User_App(void)
{ 
	u8 res = 0;
	USH_User_App_flag = DF_USH_USER_APP_OK;
	return res;
} 
