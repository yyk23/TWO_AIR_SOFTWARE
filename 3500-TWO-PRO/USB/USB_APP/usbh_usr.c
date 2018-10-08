#include "usbh_usr.h" 
#include "ff.h" 
#include "usart.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//USBH-USR ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/7/22
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
u8 UDISK_state = DF_UDISK_STATE_OUT;
u8 USH_User_App_flag = 0;

static u8 AppState;
extern USB_OTG_CORE_HANDLE  USB_OTG_Core;

//USB OTG �жϷ�����
//��������USB�ж�
void OTG_FS_IRQHandler(void)
{
  	USBH_OTG_ISR_Handler(&USB_OTG_Core);
} 
//USB HOST �û��ص�����.
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
//����Ϊ���ص�����ʵ��.

//USB HOST ��ʼ�� 
void USBH_USR_Init(void)
{
//	UDISK_state = DF_UDISK_STATE_INIT;
//	printf("USB OTG HS MSC Host\r\n");
//	printf("> USB Host library started.\r\n");
//	printf("  USB Host Library v2.1.0\r\n\r\n");
	
}
//��⵽U�̲���
void USBH_USR_DeviceAttached(void)//U�̲���
{
	UDISK_state = DF_UDISK_STATE_INSERT;
	//printf("��⵽USB�豸����!\r\n");
//	LCD_ShowStr(200,80,16,"U_DISK INSERT!");
}
//��⵽U�̰γ�
void USBH_USR_DeviceDisconnected (void)//U���Ƴ�
{
	UDISK_state = DF_UDISK_STATE_OUT;
	//printf("USB�豸�γ�!\r\n");
//	LCD_Clear(BLACK);
//	LCD_ShowStr(200,80,16,"U_DISK OUT!   ");
}  
//��λ�ӻ�
void USBH_USR_ResetDevice(void)
{
//	UDISK_state = DF_UDISK_STATE_RESET;
//	//printf("��λ�豸...\r\n");
//	LCD_ShowStr(200,100,16,"U_DISK RESET!");
}
//��⵽�ӻ��ٶ�
//DeviceSpeed:�ӻ��ٶ�(0,1,2 / ����)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
	if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED)
	{
//		UDISK_state = DF_UDISK_STATE_HIGH_SPEED;
//		//printf("����(HS)USB�豸!\r\n");
//		LCD_ShowStr(200,120,16,"HS_U_DISK!");
 	}  
	else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED)
	{
//		UDISK_state = DF_UDISK_STATE_FULL_SPEED;
//		//printf("ȫ��(FS)USB�豸!\r\n"); 
//		LCD_ShowStr(200,120,16,"FS_U_DISK!");
	}
	else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED)
	{
//		UDISK_state = DF_UDISK_STATE_LOW_SPEED;
//		//printf("����(LS)USB�豸!\r\n");  
//		LCD_ShowStr(200,120,16,"LS_U_DISK!");
	}
	else
	{
//		UDISK_state = DF_UDISK_STATE_ERROR;
//		//printf("�豸����!\r\n");  
//		LCD_ShowStr(200,120,16,"ERROR_!   ");
	}
}
//��⵽�ӻ���������
//DeviceDesc:�豸������ָ��
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
//	USBH_DevDesc_TypeDef *hs;
//	hs=DeviceDesc;   
//	printf("VID: %04Xh\r\n" , (uint32_t)(*hs).idVendor); 
//	printf("PID: %04Xh\r\n" , (uint32_t)(*hs).idProduct); 
}
//�ӻ���ַ����ɹ�
void USBH_USR_DeviceAddressAssigned(void)
{
//	UDISK_state = DF_UDISK_STATE_ADDRESS_OK;
//	LCD_ShowStr(200,140,16,"U_DISK_ADRESS_OK!");
//	printf("�ӻ���ַ����ɹ�!\r\n");   
}
//��������������Ч
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
	USBH_InterfaceDesc_TypeDef *id; 
	id = itfDesc;   
	if((*id).bInterfaceClass==0x08)
	{
//		printf("���ƶ��洢���豸!\r\n"); 
//		LCD_ShowStr(200,160,16,"U_DISK!");
	}else if((*id).bInterfaceClass==0x03)
	{
//		printf("HID �豸!\r\n"); 
//		LCD_ShowStr(200,160,16,"HID_DISK!");
	}    
}
//��ȡ���豸Manufacturer String
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
//	printf("Manufacturer: %s\r\n",(char *)ManufacturerString);
}
//��ȡ���豸Product String 
void USBH_USR_Product_String(void *ProductString)
{
//	printf("Product: %s\r\n",(char *)ProductString);  
}
//��ȡ���豸SerialNum String 
void USBH_USR_SerialNum_String(void *SerialNumString)
{
//	printf("Serial Number: %s\r\n",(char *)SerialNumString);    
} 
//�豸USBö�����
void USBH_USR_EnumerationDone(void)
{
//	UDISK_state = DF_UDISK_STATE_ENUMERATION_OK;
//	//printf("�豸ö�����!\r\n\r\n");
//	LCD_ShowStr(200,180,16,"U_DISK_ENUM_OK!");
} 
//�޷�ʶ���USB�豸
void USBH_USR_DeviceNotSupported(void)
{
//	UDISK_state = DF_UDISK_STATE_NOT_SUPPORTED;
//	LCD_ShowStr(200,200,16,"UNKNOWN_U_DISK!");
//	printf("�޷�ʶ���USB�豸!\r\n\r\n");    
}  
//�ȴ��û����밴��,ִ����һ������
USBH_USR_Status USBH_USR_UserInput(void)
{ 
//	printf("�����û�ȷ�ϲ���!\r\n");
	return USBH_USR_RESP_OK;
} 
//USB�ӿڵ�������
void USBH_USR_OverCurrentDetected (void)
{
//	printf("�˿ڵ�������!!!\r\n");
} 

extern u8 USH_User_App(void);		//�û�����������
//USB HOST MSC���û�Ӧ�ó���
int USBH_USR_MSC_Application(void)
{
	u8 res=0;
  	switch(AppState)
  	{
    	case USH_USR_FS_INIT://��ʼ���ļ�ϵͳ 
//			printf("��ʼִ���û�����!!!\r\n");
//			LCD_ShowStr(200,220,16,"U_DISK_READY!");
			AppState=USH_USR_FS_TEST;
      		break;
    	case USH_USR_FS_TEST:	//ִ��USB OTG ����������
//			LCD_ShowStr(200,240,16,"U_DISK_GOGOGO!");
			res=USH_User_App(); //�û�������
     		res=0;
			if(res)AppState=USH_USR_FS_INIT;
      		break;
    	default:break;
  	} 
	return res;
}
//�û�Ҫ�����³�ʼ���豸
void USBH_USR_DeInit(void)
{
//  	AppState=USH_USR_FS_INIT;
}
//�޷��ָ��Ĵ���!!  
void USBH_USR_UnrecoveredError (void)
{
//	printf("�޷��ָ��Ĵ���!!!\r\n\r\n");	
}
////////////////////////////////////////////////////////////////////////////////////////
//�û����庯��,ʵ��fatfs diskio�Ľӿں��� 
extern USBH_HOST              USB_Host;

//��ȡU��״̬
//����ֵ:0,U��δ����
//      1,����
u8 USBH_UDISK_Status(void)
{
	return HCD_IsDeviceConnected(&USB_OTG_Core);//����U��״̬
}

//��U��
//buf:�����ݻ�����
//sector:������ַ
//cnt:��������	
//����ֵ:����״̬;0,����;����,�������;		 
u8 USBH_UDISK_Read(u8* buf,u32 sector,u32 cnt)
{
	u32 rty_timeout = 0;
	u8 res=1;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//���ӻ�����,����APP����״̬
	{  		    
		do
		{
			res=USBH_MSC_Read10(&USB_OTG_Core,buf,sector,512*cnt);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);		      
			if(!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res=1;//��д����
				break;
			};
			rty_timeout++;
			if(rty_timeout > 5000)
			{
				res=1;//��д����
				break;
			}
		}while(res==USBH_MSC_BUSY);
	}else res=1;		  
	if(res==USBH_MSC_OK)res=0;	
	return res;
}

//дU��
//buf:д���ݻ�����
//sector:������ַ
//cnt:��������	
//����ֵ:����״̬;0,����;����,�������;		 
u8 USBH_UDISK_Write(u8* buf,u32 sector,u32 cnt)
{
	u8 res=1;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//���ӻ�����,����APP����״̬
	{  		    
		do
		{
			res=USBH_MSC_Write10(&USB_OTG_Core,buf,sector,512*cnt); 
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);		      
			if(!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res=1;//��д����
				break;
			};   
		}while(res==USBH_MSC_BUSY);
	}else res=1;		  
	if(res==USBH_MSC_OK)res=0;	
	return res;
}












































