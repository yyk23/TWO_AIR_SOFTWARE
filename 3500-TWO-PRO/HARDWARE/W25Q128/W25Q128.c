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
//****************W25Q128指令定义****************/
#define W25Q_WR_ENABLE          0x06 // 写使能指令
#define W25Q_WR_DISABLE         0x04 // 写关断指令
#define W25Q_RD_STATUS_REG      0x05 // 读状态寄存器BIT0~7
#define W25Q_RD_STATUS_REG_2    0x35 // 读状态寄存器BIT8~15
#define W25Q_WR_STATUS_REG      0x01 // 写状态寄存器
#define W25Q_RD_DATA            0x03 // 读寄存器
#define W25Q_FAST_RD_DATA       0x0B // 快速读寄存器
#define W25Q_PAGE_PROGRAM       0x02 // 页编程
#define W25Q_SECTORERASE        0x20 // 4KB 　扇区擦除指令
#define W25Q_BLOCKERASE1        0x52 // 32KB　32KB块擦除指令　
#define W25Q_BLOCKERASE2        0xD8 // 64KB　64KB块擦除指令
#define W25Q_CHIPERASE          0xC7 // 擦除一整片指令
#define W25Q_POWERDOWN          0xB9 // 芯片掉电指令
#define W25Q_RELEASEPOWERDOWN   0xAB // 唤醒芯片指令



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
		W25_CLK = 0;				//若接下来执行SPI_RD_Byte(),则最后这个W25_CLK=0时第一个数据已经被读出，在SPI_RD_Byte()中直接读取端口就行
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
** 读取SPI_FLASH的状态寄存器              ***
** BIT  6  5  4   3   2   1   0           ***
** SPR  RV TB BP2 BP1 BP0 WEL BUSY        ***
** SPR: 默认0,状态寄存器保护位,配合WP使用 ***
** TB,BP2,BP1,BP0:FLASH区域写保护设置     ***
** WEL:写使能锁定                         ***
** BUSY:忙标记位(1,忙;0,空闲)             ***
** 默认:0x00                              ***
*********************************************/
u8 SPI_RD_Reg(void)
{
	u8 byte = 0;
	W25_CS = 0;
	SPI_WR_Byte(W25Q_RD_STATUS_REG); //发送读取状态寄存器
	byte = SPI_RD_Byte();        //读取一个字节
	W25_CS = 1;
	return byte;
}


//写SPI_FLASH状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void SPI_WR_SR(u8 sr)
{
	W25_CS = 0;
	SPI_WR_Byte(W25Q_WR_STATUS_REG);  //发送写取状态寄存器
	SPI_WR_Byte(sr);                  //写入一个字节
	W25_CS = 1;
}


//SPI FLASH写使能
//将WEL置位
void SPI_WR_EN(void)
{
	W25_CS = 0;
	SPI_WR_Byte(W25Q_WR_ENABLE);     //发送写使能
	W25_CS = 1;
}

//SPI FLASH写禁止
//将WEL清零
void SPI_WR_DIS(void)
{
	W25_CS = 0;
	SPI_WR_Byte(W25Q_WR_DISABLE);   //发送写禁止指令
	W25_CS = 1;
}

//读取芯片ID 
u16 SPI_ReadID(void)
{
	u16 Temp = 0;
	W25_CS = 0;
	SPI_WR_Byte(0x90);   //发送读取ID命令
	SPI_WR_Byte(0x00);
	SPI_WR_Byte(0x00);
	SPI_WR_Byte(0x00);
	Temp |= (u16)SPI_RD_Byte()<<8;
	Temp |= SPI_RD_Byte();
	W25_CS = 1;
	return Temp;
}

//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void SPI_Data_Read(u8 *pBuffer,u32 ReadAddr,u16 NumByteToRead)
{
	u16 i;
	W25_CS = 0;
	SPI_WR_Byte(W25Q_RD_DATA);    //发送读取命令
	SPI_WR_Byte((u8)((ReadAddr)>>16));
	SPI_WR_Byte((u8)((ReadAddr)>>8 ));
	SPI_WR_Byte((u8)(ReadAddr));
	for(i=0;i<NumByteToRead;i++)
	{
		pBuffer[i] = SPI_RD_Byte();//循环读数
	}
	W25_CS = 1;
}

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址
//NumByteToWrite:要写如的字节数(最大256),该数不能超过该页的剩余字节数
void SPI_DATA_WR_Page(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u16 i;
	SPI_WR_EN();//写使能
	W25_CS = 0;
	SPI_WR_Byte(W25Q_PAGE_PROGRAM);      //发送页编程指令
	SPI_WR_Byte((u8)((WriteAddr)>>16));  //发送24bit地址
	SPI_WR_Byte((u8)((WriteAddr)>>8 ));  
	SPI_WR_Byte((u8)(WriteAddr));
	for(i=0;i<NumByteToWrite;i++)
	{
		SPI_WR_Byte(pBuffer[i]);//循环写入数据
	}
	W25_CS = 1;
	SPI_Wait_Busy();           //等待写入结束
}

//无需检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0xFF,否则在非0xFF处写入的数据将失败
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void SPI_Write_NoCheck(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u16 pageremain;
	pageremain = 256 - WriteAddr % 256;//单页剩余的字节数
	if(NumByteToWrite <= pageremain)
	{
		pageremain = NumByteToWrite; //不大于256个字节
	}
	while(1)
	{
		SPI_DATA_WR_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite == pageremain)
		{
			break; //写入结束了
		}
		else
		{
			pBuffer   += pageremain;
			WriteAddr += pageremain;
		
			NumByteToWrite -= pageremain;   //减去已经写入了的字节数
			if(NumByteToWrite > 256)
			{
				NumByteToWrite = 256;        //一次可以写入256个字节
			}
			else
			{
				pageremain = NumByteToWrite; //不够256个字节了
			}
		}
	}
}

//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
u8 SPI_FLASH_BUF[4096];
void SPI_Flash_Write(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u32 secpos;
	u16 secoff;
	u16 secremain;
	u16 i;
	
	secpos = WriteAddr / 4096; //扇区地址:0~511 for w25X16 这部分需要更改
	secoff = WriteAddr % 4096; //在扇区内的偏移
	secremain = 4096 - secoff; //扇区剩余空间大小
	
	if(NumByteToWrite <= secremain)
	{
		secremain = NumByteToWrite;//不大于4096个字节
	}
	while(1)
	{
		SPI_Data_Read(SPI_FLASH_BUF,secpos * 4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(SPI_FLASH_BUF[secoff+i] != 0xFF)
			{
				break;//需要擦除
			}
			if(i<secremain)//需要擦除
			{
				SPI_Flash_Erase_Sector(secpos);//擦除这个扇区
				for(i=0;i<secremain;i++)       //复制
				{
					SPI_FLASH_BUF[i+secoff] = pBuffer[i];
				}
				SPI_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096); //写入整个扇区
			}
			else
			{
				SPI_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间
			}
			if(NumByteToWrite==secremain)
			{
				break;//写入结束了
			}
			else
			{//写入未结束
				secpos++; //偏移地址增1
				secoff=0; //偏移地址为0
				
				pBuffer += secremain; //指针偏移
				WriteAddr += secremain; //写地址偏移
				NumByteToWrite -= secremain;   //字节数递减
				if(NumByteToWrite > 4096)
				{
					secremain = 4096;           //下一个扇区还是写不完
				}
				else
				{
					secremain = NumByteToWrite; //下一个扇区可以写完了
				}
			}
		}
	}		
}	

//擦除整个芯片
//整片擦除时间:
//W25x16:25s
//W25x32:40s
//W25x64:40s
//等待时间超长
void SPI_Flash_Erase_Chip(void)
{
	SPI_WR_EN();             //芯片写使能
	SPI_Wait_Busy();
	W25_CS = 0; 
	SPI_WR_Byte(W25Q_CHIPERASE); //发送片擦除命令
	W25_CS = 1;
	SPI_Wait_Busy();             //等待芯片擦除结束
}

//擦除一个扇区
//Dst Addr:扇区地址 0~511 for w25x16 此处需更改
//擦除一个扇区最少时间:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr)
{
	//Dst_Addr *= 4096;
	SPI_WR_EN();   //写使能
	SPI_Wait_Busy();            //等待空闲  
	W25_CS = 0;             
	SPI_WR_Byte(W25Q_SECTORERASE);  //发送扇区擦除指令
	SPI_WR_Byte((u8)((Dst_Addr)>>16)); //发送24bit地址
	SPI_WR_Byte((u8)((Dst_Addr)>>8 ));
	SPI_WR_Byte((u8)(Dst_Addr));
	W25_CS = 1;
	SPI_Wait_Busy();
}

/* 等待芯片空闲 */
void SPI_Wait_Busy(void)
{
/*  waste time until not busy WEL & Busy bit all be 1 (0x03). */
	while (SPI_RD_Reg() == 0x03)
	{
		SPI_RD_Reg();
	}
}

//进入掉电模式
void SPI_Flash_PowerDown(void)
{
	SPI_WR_Byte(W25Q_POWERDOWN);  //发送掉电命令
}

//
void SPI_Flash_WakeUp(void)
{
	SPI_WR_Byte(W25Q_RELEASEPOWERDOWN);
}














































