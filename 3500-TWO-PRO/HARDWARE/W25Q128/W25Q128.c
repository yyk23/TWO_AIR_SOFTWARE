#include <p33FJ256GP510A.h>
#include "datatype.h"
#include "wnd.h"
#include "key.h"
#include "menu.h"
#include "LM2068E.h"
#include "adc.h"
#include "sample.h"
#include "ziku.h"
#include "24c64.h"
#include "ds1302.h"
#include "pump.h"
#include "usb.h"
#include "print.h"
#include "18b20.h"
#include "saveresume.h"
#include "x9c104.h"
#include <string.h>
#include "TASK.h"
#include "W25Q128.h"
//****************W25Q128ָ���****************/
#define W25Q_WR_ENABLE          0x06 // дʹ��ָ��
#define W25Q_WR_DISABLE         0x04 // д�ض�ָ��
#define W25Q_RD_STATUS_REG      0x05 // ��״̬�Ĵ���BIT0~7
#define W25Q_RD_STATUS_REG_2    0x35 // ��״̬�Ĵ���BIT8~15
#define W25Q_WR_STATUS_REG      0x01 // д״̬�Ĵ���
#define W25Q_RD_DATA            0x03 // ���Ĵ���
#define W25Q_FAST_RD_DATA       0x0B // ���ٶ��Ĵ���
#define W25Q_PAGE_PROGRAM       0x02 // ҳ���
#define W25Q_SECTORERASE        0x20 // 4KB ����������ָ��
#define W25Q_BLOCKERASE1        0x52 // 32KB��32KB�����ָ�
#define W25Q_BLOCKERASE2        0xD8 // 64KB��64KB�����ָ��
#define W25Q_CHIPERASE          0xC7 // ����һ��Ƭָ��
#define W25Q_POWERDOWN          0xB9 // оƬ����ָ��
#define W25Q_RELEASEPOWERDOWN   0xAB // ����оƬָ��



#define W25_CS 		LATAbits.LATA15
#define W25_CLK		LATDbits.LATD9
#define W25_DO		PORTAbits.RA14
#define W25_DIO		LATDbits.LATD8

void W25_Init(void)
{
	TRISAbits.TRISA15 = 0;
	TRISDbits.TRISD9 = 0;
	TRISAbits.TRISA14 = 1;
	TRISDbits.TRISD8 = 0;
	W25_CS = 0;
	W25_CLK = 0;
	LATAbits.LATA14 = 1;
	W25_DIO = 1;
	W25_CS = 1;
}


void SPI_WR_Byte(u8 dat)
{
	u8 i = 0;
	W25_CS = 0;
	for(i = 0;i < 8;i++)
	{
		if((dat & 0x80) == 0x80)	/* check if MSB is high */
		{
			W25_DIO = 1;
		}
		else
		{
			W25_DIO = 0;			/* if not set to low */
		}
		W25_CLK = 1;
		dat = (dat << 1);
		W25_CLK = 0;				//��������ִ��SPI_RD_Byte(),��������W25_CLK=0ʱ��һ�������Ѿ�����������SPI_RD_Byte()��ֱ�Ӷ�ȡ�˿ھ���
	}
}


u8 SPI_RD_Byte(void)
{
	u8 i    = 0;
	u8 in   = 0;
	u8 temp = 0;
	W25_CS = 0;
	for(i = 0;i < 8;i++)
	{
		in = (in << 1);
		temp = W25_DO;
		W25_CLK = 1;
		if(temp != 0)
		{
			in |= 0x01;
		}
		W25_CLK = 0;	
	}
	return in;
}

/********************************************
** ��ȡSPI_FLASH��״̬�Ĵ���              ***
** BIT  6  5  4   3   2   1   0           ***
** SPR  RV TB BP2 BP1 BP0 WEL BUSY        ***
** SPR: Ĭ��0,״̬�Ĵ�������λ,���WPʹ�� ***
** TB,BP2,BP1,BP0:FLASH����д��������     ***
** WEL:дʹ������                         ***
** BUSY:æ���λ(1,æ;0,����)             ***
** Ĭ��:0x00                              ***
*********************************************/
u8 SPI_RD_Reg(void)
{
	u8 byte = 0;
	W25_CS = 0;
	SPI_WR_Byte(W25Q_RD_STATUS_REG); //���Ͷ�ȡ״̬�Ĵ���
	byte = SPI_RD_Byte();        //��ȡһ���ֽ�
	W25_CS = 1;
	return byte;
}


//дSPI_FLASH״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void SPI_WR_SR(u8 sr)
{
	W25_CS = 0;
	SPI_WR_Byte(W25Q_WR_STATUS_REG);  //����дȡ״̬�Ĵ���
	SPI_WR_Byte(sr);                  //д��һ���ֽ�
	W25_CS = 1;
}


//SPI FLASHдʹ��
//��WEL��λ
void SPI_WR_EN(void)
{
	W25_CS = 0;
	SPI_WR_Byte(W25Q_WR_ENABLE);     //����дʹ��
	W25_CS = 1;
}

//SPI FLASHд��ֹ
//��WEL����
void SPI_WR_DIS(void)
{
	W25_CS = 0;
	SPI_WR_Byte(W25Q_WR_DISABLE);   //����д��ָֹ��
	W25_CS = 1;
}

//��ȡоƬID 
u16 SPI_ReadID(void)
{
	u16 Temp = 0;
	W25_CS = 0;
	SPI_WR_Byte(0x90);   //���Ͷ�ȡID����
	SPI_WR_Byte(0x00);
	SPI_WR_Byte(0x00);
	SPI_WR_Byte(0x00);
	Temp |= (u16)SPI_RD_Byte()<<8;
	Temp |= SPI_RD_Byte();
	W25_CS = 1;
	return Temp;
}

//��ȡSPI FLASH
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void SPI_Data_Read(u8 *pBuffer,u32 ReadAddr,u16 NumByteToRead)
{
	u16 i;
	W25_CS = 0;
	SPI_WR_Byte(W25Q_RD_DATA);    //���Ͷ�ȡ����
	SPI_WR_Byte((u8)((ReadAddr)>>16));
	SPI_WR_Byte((u8)((ReadAddr)>>8 ));
	SPI_WR_Byte((u8)(ReadAddr));
	for(i=0;i<NumByteToRead;i++)
	{
		pBuffer[i] = SPI_RD_Byte();//ѭ������
	}
	W25_CS = 1;
}

//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ
//NumByteToWrite:Ҫд����ֽ���(���256),�������ܳ�����ҳ��ʣ���ֽ���
void SPI_DATA_WR_Page(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u16 i;
	SPI_WR_EN();//дʹ��
	W25_CS = 0;
	SPI_WR_Byte(W25Q_PAGE_PROGRAM);      //����ҳ���ָ��
	SPI_WR_Byte((u8)((WriteAddr)>>16));  //����24bit��ַ
	SPI_WR_Byte((u8)((WriteAddr)>>8 ));  
	SPI_WR_Byte((u8)(WriteAddr));
	for(i=0;i<NumByteToWrite;i++)
	{
		SPI_WR_Byte(pBuffer[i]);//ѭ��д������
	}
	W25_CS = 1;
	SPI_Wait_Busy();           //�ȴ�д�����
}

//�������дSPI FLASH
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0xFF,�����ڷ�0xFF��д������ݽ�ʧ��
//�����Զ���ҳ����
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void SPI_Write_NoCheck(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u16 pageremain;
	pageremain = 256 - WriteAddr % 256;//��ҳʣ����ֽ���
	if(NumByteToWrite <= pageremain)
	{
		pageremain = NumByteToWrite; //������256���ֽ�
	}
	while(1)
	{
		SPI_DATA_WR_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite == pageremain)
		{
			break; //д�������
		}
		else
		{
			pBuffer   += pageremain;
			WriteAddr += pageremain;
		
			NumByteToWrite -= pageremain;   //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite > 256)
			{
				NumByteToWrite = 256;        //һ�ο���д��256���ֽ�
			}
			else
			{
				pageremain = NumByteToWrite; //����256���ֽ���
			}
		}
	}
}

//дSPI FLASH
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
u8 SPI_FLASH_BUF[4096];
void SPI_Flash_Write(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u32 secpos;
	u16 secoff;
	u16 secremain;
	u16 i;
	
	secpos = WriteAddr / 4096; //������ַ:0~511 for w25X16 �ⲿ����Ҫ����
	secoff = WriteAddr % 4096; //�������ڵ�ƫ��
	secremain = 4096 - secoff; //����ʣ��ռ��С
	
	if(NumByteToWrite <= secremain)
	{
		secremain = NumByteToWrite;//������4096���ֽ�
	}
	while(1)
	{
		SPI_Data_Read(SPI_FLASH_BUF,secpos * 4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(SPI_FLASH_BUF[secoff+i] != 0xFF)
			{
				break;//��Ҫ����
			}
			if(i<secremain)//��Ҫ����
			{
				SPI_Flash_Erase_Sector(secpos);//�����������
				for(i=0;i<secremain;i++)       //����
				{
					SPI_FLASH_BUF[i+secoff] = pBuffer[i];
				}
				SPI_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096); //д����������
			}
			else
			{
				SPI_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������
			}
			if(NumByteToWrite==secremain)
			{
				break;//д�������
			}
			else
			{//д��δ����
				secpos++; //ƫ�Ƶ�ַ��1
				secoff=0; //ƫ�Ƶ�ַΪ0
				
				pBuffer += secremain; //ָ��ƫ��
				WriteAddr += secremain; //д��ַƫ��
				NumByteToWrite -= secremain;   //�ֽ����ݼ�
				if(NumByteToWrite > 4096)
				{
					secremain = 4096;           //��һ����������д����
				}
				else
				{
					secremain = NumByteToWrite; //��һ����������д����
				}
			}
		}
	}		
}	

//��������оƬ
//��Ƭ����ʱ��:
//W25x16:25s
//W25x32:40s
//W25x64:40s
//�ȴ�ʱ�䳬��
void SPI_Flash_Erase_Chip(void)
{
	SPI_WR_EN();             //оƬдʹ��
	SPI_Wait_Busy();
	W25_CS = 0; 
	SPI_WR_Byte(W25Q_CHIPERASE); //����Ƭ��������
	W25_CS = 1;
	SPI_Wait_Busy();             //�ȴ�оƬ��������
}

//����һ������
//Dst Addr:������ַ 0~511 for w25x16 �˴������
//����һ����������ʱ��:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr)
{
	//Dst_Addr *= 4096;
	SPI_WR_EN();   //дʹ��
	SPI_Wait_Busy();            //�ȴ�����  
	W25_CS = 0;             
	SPI_WR_Byte(W25Q_SECTORERASE);  //������������ָ��
	SPI_WR_Byte((u8)((Dst_Addr)>>16)); //����24bit��ַ
	SPI_WR_Byte((u8)((Dst_Addr)>>8 ));
	SPI_WR_Byte((u8)(Dst_Addr));
	W25_CS = 1;
	SPI_Wait_Busy();
}

/* �ȴ�оƬ���� */
void SPI_Wait_Busy(void)
{
/*  waste time until not busy WEL & Busy bit all be 1 (0x03). */
	while (SPI_RD_Reg() == 0x03)
	{
		SPI_RD_Reg();
	}
}

//�������ģʽ
void SPI_Flash_PowerDown(void)
{
	SPI_WR_Byte(W25Q_POWERDOWN);  //���͵�������
}

//
void SPI_Flash_WakeUp(void)
{
	SPI_WR_Byte(W25Q_RELEASEPOWERDOWN);
}














































