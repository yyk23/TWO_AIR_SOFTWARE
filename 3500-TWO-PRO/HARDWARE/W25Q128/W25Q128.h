
#ifndef __W25Q128_H__
#define __W25Q128_H__
#include "datatype.h"

#define         DEF_FIRST_START_FLAG_W25               				0         	//首次上电标志
#define         DEF_POWER_FILE_FIRST_START_PAGE_W25               	160         //掉电文件起始页(页)
#define         DEF_POWER_FILE_SECOND_START_PAGE_W25              	1600        //掉电文件起始页(页)
#define         DEF_POWER_FILE_LENGTH_W25           				16          //掉电文件长度(字节)每页256字节存储2个文件
#define         DEF_POWER_FILE_MAX_W25              				1000        //掉电文件数(个)
#define         DEF_SAMPLE_FILE_FIRST_START_PAGE_W25              	3200       	//采样文件起始页(页)
#define         DEF_SAMPLE_FILE_SECOND_START_PAGE_W25             	4800       	//采样文件起始页(页)
#define         DEF_SAMPLE_FILE_LENGTH_W25         					40          //采样文件长度(字节)每页256字节存储2个文件
#define         DEF_SAMPLE_FILE_MAX_W25             				1000        //采样文件数(个)
#define 		DEF_RUN_FILE_FIRST_START_PAGE_W25					8000		//瞬时文件起始页(页)
#define 		DEF_RUN_FILE_SECOND_START_PAGE_W25					32000		//瞬时文件起始页(页)
#define         DEF_RUN_FILE_LENGTH_W25          					22          //瞬时文件长度(字节)每页256字节存储2个文件
#define         DEF_RUN_FILE_MAX_W25             					30000       //瞬时文件数(个)



void W25_Init(void);
void SPI_WR_Byte(u8 dat);

u8 SPI_RD_Byte(void);

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
u8 SPI_RD_Reg(void);

//写SPI_FLASH状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void SPI_WR_SR(u8 sr);

//SPI FLASH写使能
//将WEL置位
void SPI_WR_EN(void);

//SPI FLASH写禁止
//将WEL清零
void SPI_WR_DIS(void);

//读取芯片ID 
u16 SPI_ReadID(void);

//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void SPI_Data_Read(u8 *pBuffer,u32 ReadAddr,u16 NumByteToRead);

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址
//NumByteToWrite:要写如的字节数(最大256),该数不能超过该页的剩余字节数
void SPI_DATA_WR_Page(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite);

//无需检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0xFF,否则在非0xFF处写入的数据将失败
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void SPI_Write_NoCheck(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite);

//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
void SPI_Flash_Write(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite);

//擦除整个芯片
//整片擦除时间:
//W25x16:25s
//W25x32:40s
//W25x64:40s
//等待时间超长
void SPI_Flash_Erase_Chip(void);

//擦除一个扇区
//Dst Addr:扇区地址 0~511 for w25x16 此处需更改
//擦除一个扇区最少时间:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr);

/* 等待芯片空闲 */
void SPI_Wait_Busy(void);

//进入掉电模式
void SPI_Flash_PowerDown(void);

//唤醒芯片
void SPI_Flash_WakeUp(void);


#endif
