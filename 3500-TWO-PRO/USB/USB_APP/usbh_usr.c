#include "usbh_usr.h" 
#include "ff.h" 
#include "usart.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//USBH-USR 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/7/22
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
u8 UDISK_state = DF_UDISK_STATE_OUT;
u8 USH_User_App_flag = 0;

static u8 AppState;
extern USB_OTG_CORE_HANDLE  USB_OTG_Core;

//USB OTG 中断服务函数
//处理所有USB中断
void OTG_FS_IRQHandler(void)
{
  	USBH_OTG_ISR_Handler(&USB_OTG_Core);
} 
//USB HOST 用户回调函数.
USBH_Usr_cb_TypeDef USR_Callbacks=
{
	USBH_USR_Init,
	USBH_USR_DeInit,
	USBH_USR_DeviceAttached,
	USBH_USR_ResetDevice,
	USBH_USR_DeviceDisconnected,
	USBH_USR_OverCurrentDetected,
	USBH_USR_DeviceSpeedDetected,
	USBH_USR_Device_DescAvailable,
	USBH_USR_DeviceAddressAssigned,
	USBH_USR_Configuration_DescAvailable,
	USBH_USR_Manufacturer_String,
	USBH_USR_Product_String,
	USBH_USR_SerialNum_String,
	USBH_USR_EnumerationDone,
	USBH_USR_UserInput,
	USBH_USR_MSC_Application,
	USBH_USR_DeviceNotSupported,
	USBH_USR_UnrecoveredError
};
/////////////////////////////////////////////////////////////////////////////////
//以下为各回调函数实现.

//USB HOST 初始化 
void USBH_USR_Init(void)
{
//	UDISK_state = DF_UDISK_STATE_INIT;
//	printf("USB OTG HS MSC Host\r\n");
//	printf("> USB Host library started.\r\n");
//	printf("  USB Host Library v2.1.0\r\n\r\n");
	
}
//检测到U盘插入
void USBH_USR_DeviceAttached(void)//U盘插入
{
	UDISK_state = DF_UDISK_STATE_INSERT;
	//printf("检测到USB设备插入!\r\n");
//	LCD_ShowStr(200,80,16,"U_DISK INSERT!");
}
//检测到U盘拔出
void USBH_USR_DeviceDisconnected (void)//U盘移除
{
	UDISK_state = DF_UDISK_STATE_OUT;
	//printf("USB设备拔出!\r\n");
//	LCD_Clear(BLACK);
//	LCD_ShowStr(200,80,16,"U_DISK OUT!   ");
}  
//复位从机
void USBH_USR_ResetDevice(void)
{
//	UDISK_state = DF_UDISK_STATE_RESET;
//	//printf("复位设备...\r\n");
//	LCD_ShowStr(200,100,16,"U_DISK RESET!");
}
//检测到从机速度
//DeviceSpeed:从机速度(0,1,2 / 其他)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
	if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED)
	{
//		UDISK_state = DF_UDISK_STATE_HIGH_SPEED;
//		//printf("高速(HS)USB设备!\r\n");
//		LCD_ShowStr(200,120,16,"HS_U_DISK!");
 	}  
	else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED)
	{
//		UDISK_state = DF_UDISK_STATE_FULL_SPEED;
//		//printf("全速(FS)USB设备!\r\n"); 
//		LCD_ShowStr(200,120,16,"FS_U_DISK!");
	}
	else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED)
	{
//		UDISK_state = DF_UDISK_STATE_LOW_SPEED;
//		//printf("低速(LS)USB设备!\r\n");  
//		LCD_ShowStr(200,120,16,"LS_U_DISK!");
	}
	else
	{
//		UDISK_state = DF_UDISK_STATE_ERROR;
//		//printf("设备错误!\r\n");  
//		LCD_ShowStr(200,120,16,"ERROR_!   ");
	}
}
//检测到从机的描述符
//DeviceDesc:设备描述符指针
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
//	USBH_DevDesc_TypeDef *hs;
//	hs=DeviceDesc;   
//	printf("VID: %04Xh\r\n" , (uint32_t)(*hs).idVendor); 
//	printf("PID: %04Xh\r\n" , (uint32_t)(*hs).idProduct); 
}
//从机地址分配成功
void USBH_USR_DeviceAddressAssigned(void)
{
//	UDISK_state = DF_UDISK_STATE_ADDRESS_OK;
//	LCD_ShowStr(200,140,16,"U_DISK_ADRESS_OK!");
//	printf("从机地址分配成功!\r\n");   
}
//配置描述符获有效
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
	USBH_InterfaceDesc_TypeDef *id; 
	id = itfDesc;   
	if((*id).bInterfaceClass==0x08)
	{
//		printf("可移动存储器设备!\r\n"); 
//		LCD_ShowStr(200,160,16,"U_DISK!");
	}else if((*id).bInterfaceClass==0x03)
	{
//		printf("HID 设备!\r\n"); 
//		LCD_ShowStr(200,160,16,"HID_DISK!");
	}    
}
//获取到设备Manufacturer String
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
//	printf("Manufacturer: %s\r\n",(char *)ManufacturerString);
}
//获取到设备Product String 
void USBH_USR_Product_String(void *ProductString)
{
//	printf("Product: %s\r\n",(char *)ProductString);  
}
//获取到设备SerialNum String 
void USBH_USR_SerialNum_String(void *SerialNumString)
{
//	printf("Serial Number: %s\r\n",(char *)SerialNumString);    
} 
//设备USB枚举完成
void USBH_USR_EnumerationDone(void)
{
//	UDISK_state = DF_UDISK_STATE_ENUMERATION_OK;
//	//printf("设备枚举完成!\r\n\r\n");
//	LCD_ShowStr(200,180,16,"U_DISK_ENUM_OK!");
} 
//无法识别的USB设备
void USBH_USR_DeviceNotSupported(void)
{
//	UDISK_state = DF_UDISK_STATE_NOT_SUPPORTED;
//	LCD_ShowStr(200,200,16,"UNKNOWN_U_DISK!");
//	printf("无法识别的USB设备!\r\n\r\n");    
}  
//等待用户输入按键,执行下一步操作
USBH_USR_Status USBH_USR_UserInput(void)
{ 
//	printf("跳过用户确认步骤!\r\n");
	return USBH_USR_RESP_OK;
} 
//USB接口电流过载
void USBH_USR_OverCurrentDetected (void)
{
//	printf("端口电流过大!!!\r\n");
} 

extern u8 USH_User_App(void);		//用户测试主程序
//USB HOST MSC类用户应用程序
int USBH_USR_MSC_Application(void)
{
	u8 res=0;
  	switch(AppState)
  	{
    	case USH_USR_FS_INIT://初始化文件系统 
//			printf("开始执行用户程序!!!\r\n");
//			LCD_ShowStr(200,220,16,"U_DISK_READY!");
			AppState=USH_USR_FS_TEST;
      		break;
    	case USH_USR_FS_TEST:	//执行USB OTG 测试主程序
//			LCD_ShowStr(200,240,16,"U_DISK_GOGOGO!");
			res=USH_User_App(); //用户主程序
     		res=0;
			if(res)AppState=USH_USR_FS_INIT;
      		break;
    	default:break;
  	} 
	return res;
}
//用户要求重新初始化设备
void USBH_USR_DeInit(void)
{
//  	AppState=USH_USR_FS_INIT;
}
//无法恢复的错误!!  
void USBH_USR_UnrecoveredError (void)
{
//	printf("无法恢复的错误!!!\r\n\r\n");	
}
////////////////////////////////////////////////////////////////////////////////////////
//用户定义函数,实现fatfs diskio的接口函数 
extern USBH_HOST              USB_Host;

//获取U盘状态
//返回值:0,U盘未就绪
//      1,就绪
u8 USBH_UDISK_Status(void)
{
	return HCD_IsDeviceConnected(&USB_OTG_Core);//返回U盘状态
}

//读U盘
//buf:读数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;		 
u8 USBH_UDISK_Read(u8* buf,u32 sector,u32 cnt)
{
	u32 rty_timeout = 0;
	u8 res=1;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//连接还存在,且是APP测试状态
	{  		    
		do
		{
			res=USBH_MSC_Read10(&USB_OTG_Core,buf,sector,512*cnt);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);		      
			if(!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res=1;//读写错误
				break;
			};
			rty_timeout++;
			if(rty_timeout > 5000)
			{
				res=1;//读写错误
				break;
			}
		}while(res==USBH_MSC_BUSY);
	}else res=1;		  
	if(res==USBH_MSC_OK)res=0;	
	return res;
}

//写U盘
//buf:写数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;		 
u8 USBH_UDISK_Write(u8* buf,u32 sector,u32 cnt)
{
	u8 res=1;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//连接还存在,且是APP测试状态
	{  		    
		do
		{
			res=USBH_MSC_Write10(&USB_OTG_Core,buf,sector,512*cnt); 
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);		      
			if(!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res=1;//读写错误
				break;
			};   
		}while(res==USBH_MSC_BUSY);
	}else res=1;		  
	if(res==USBH_MSC_OK)res=0;	
	return res;
}












































