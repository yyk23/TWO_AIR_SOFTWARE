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
#include "DMT_ADDRESS.h"
#include "DMT80480T050.h"
#define FIRST_BANK	0
#define SECOND_BANK	1

u8 sample_file_bank;
u8 power_file_bank;
u8 run_file_bank;

void SaveSampleFile_TSP(void)
{
	u8 *p;
	u8 i;
	u8 * file_ptr;
	u8 file_overflow_flag = 0;
	u32 addr;
	u16 sample_file_max;
    sample_file_max = DEF_SAMPLE_FILE_MAX_W25;
	tsp_sample_file.num++;
	if(tsp_sample_file.num > sample_file_max)
	{
		file_overflow_flag = 1;
		tsp_sample_file.num = 1;
		tsp_sample_file.fg_flag = 1;
		
		if(sample_file_bank == FIRST_BANK)
		{
			sample_file_bank = SECOND_BANK;
		}
		else
		{
			sample_file_bank = FIRST_BANK;		
		}
	}
	
 	if(sample_file_bank == FIRST_BANK)
	{
		addr = (u32)DEF_SAMPLE_FILE_FIRST_START_PAGE_W25 * 256 + (u32)(tsp_sample_file.num - 1) * 128;	
	}
	else
	{
		addr = (u32)DEF_SAMPLE_FILE_SECOND_START_PAGE_W25 * 256 + (u32)(tsp_sample_file.num - 1) * 128;
	}

	p = (uchar *)&TSP_sample_start;
	for(i = 0;i < 7; i++)
	{
		*(p++) = R_24(ADDR_TSP_sample_start+i);	//注：从FM24C64中读取临时文件内容
	}
	sample_file.channel = DF_CHANNEL_TSP;
	sample_file.year  = TSP_sample_start.year;
	sample_file.month = TSP_sample_start.month;
	sample_file.day   = TSP_sample_start.day;
	sample_file.hour  = TSP_sample_start.hour;
	sample_file.min   = TSP_sample_start.min;
	sample_file.sec   = TSP_sample_start.sec;
	sample_file.running_time = TSP_SAMPLE.running_time_set - TSP_SAMPLE.running_time;//已经采样时间
	sample_file.flow_set = TSP_SAMPLE.flow_set;
	sample_file.volume = TSP_SAMPLE.volume;
	sample_file.volume_biaokuang = TSP_SAMPLE.volume_biaokuang;
	file_ptr = (uchar *)(& sample_file);

	SPI_DATA_WR_Page(file_ptr,addr,DEF_SAMPLE_FILE_LENGTH_W25);

	if(sample_file_bank == FIRST_BANK)
	{
		if((tsp_sample_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_SAMPLE_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_sample_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_sample_file.num == sample_file_max)
		{
			addr = (u32)DEF_SAMPLE_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_sample_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}
	}
	else
	{
		if((tsp_sample_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_SAMPLE_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_sample_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_sample_file.num == sample_file_max)
		{
			addr = (u32)DEF_SAMPLE_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_sample_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}	
	}
	if(file_overflow_flag == 1)
	{
		W_24(ADDR_tsp_sample_file_fg_flag,tsp_sample_file.fg_flag);
		W_24(ADDR_sample_file_bank,sample_file_bank);
	}
	saveintdata(ADDR_tsp_sample_file_num,tsp_sample_file.num);//保存当前文件号地址
}

void ReadSampleFile_TSP(uint NumFile)
{
    u32 addr;
	if(sample_file_bank == FIRST_BANK)
	{
		if(NumFile <= tsp_sample_file.num)
		{
			addr = (u32)DEF_SAMPLE_FILE_FIRST_START_PAGE_W25 * 256 + (u32)(NumFile - 1) * 128;	
		}
		else
		{
			addr = (u32)DEF_SAMPLE_FILE_SECOND_START_PAGE_W25 * 256 + (u32)(NumFile - 1) * 128;
		}
	}
	else
	{
		if(NumFile <= tsp_sample_file.num)
		{
			addr = (u32)DEF_SAMPLE_FILE_SECOND_START_PAGE_W25 * 256 + (u32)(NumFile - 1) * 128;	
		}
		else
		{
			addr = (u32)DEF_SAMPLE_FILE_FIRST_START_PAGE_W25 * 256 + (u32)(NumFile - 1) * 128;
		}	
	}

	SPI_Data_Read((u8 *)(&sample_file_chaxun),addr,DEF_SAMPLE_FILE_LENGTH_W25);
}



void SaveSampleFile_A(void)
{
	u8 *p;
	u8 i;
	u8 * file_ptr;
	u8 file_overflow_flag = 0;
	u32 addr;
	u16 sample_file_max;
    sample_file_max = DEF_SAMPLE_FILE_MAX_W25;
	tsp_sample_file.num++;
	if(tsp_sample_file.num > sample_file_max)
	{
		file_overflow_flag = 1;
		tsp_sample_file.num = 1;
		tsp_sample_file.fg_flag = 1;
		
		if(sample_file_bank == FIRST_BANK)
		{
			sample_file_bank = SECOND_BANK;
		}
		else
		{
			sample_file_bank = FIRST_BANK;		
		}
	}
	
 	if(sample_file_bank == FIRST_BANK)
	{
		addr = (u32)DEF_SAMPLE_FILE_FIRST_START_PAGE_W25 * 256 + (u32)(tsp_sample_file.num - 1) * 128;	
	}
	else
	{
		addr = (u32)DEF_SAMPLE_FILE_SECOND_START_PAGE_W25 * 256 + (u32)(tsp_sample_file.num - 1) * 128;
	}

	p = (uchar *)&A_sample_start;
	for(i = 0;i < 7; i++)
	{
		*(p++) = R_24(ADDR_A_sample_start+i);	//注：从FM24C64中读取临时文件内容
	}
	A_sample_file.channel = DF_CHANNEL_A;
	A_sample_file.year  = A_sample_start.year;
	A_sample_file.month = A_sample_start.month;
	A_sample_file.day   = A_sample_start.day;
	A_sample_file.hour  = A_sample_start.hour;
	A_sample_file.min   = A_sample_start.min;
	A_sample_file.sec   = A_sample_start.sec;
	A_sample_file.running_time = A_SAMPLE.running_time_set - A_SAMPLE.running_time;//已经采样时间
	A_sample_file.flow_set = A_SAMPLE.flow_set;
	A_sample_file.volume = A_SAMPLE.volume;
	A_sample_file.volume_biaokuang = A_SAMPLE.volume_biaokuang;
	file_ptr = (uchar *)(& A_sample_file);

	SPI_DATA_WR_Page(file_ptr,addr,DEF_SAMPLE_FILE_LENGTH_W25);

	if(sample_file_bank == FIRST_BANK)
	{
		if((tsp_sample_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_SAMPLE_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_sample_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_sample_file.num == sample_file_max)
		{
			addr = (u32)DEF_SAMPLE_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_sample_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}
	}
	else
	{
		if((tsp_sample_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_SAMPLE_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_sample_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_sample_file.num == sample_file_max)
		{
			addr = (u32)DEF_SAMPLE_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_sample_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}	
	}
	if(file_overflow_flag == 1)
	{
		W_24(ADDR_tsp_sample_file_fg_flag,tsp_sample_file.fg_flag);
		W_24(ADDR_sample_file_bank,sample_file_bank);
	}
	saveintdata(ADDR_tsp_sample_file_num,tsp_sample_file.num);//保存当前文件号地址
}

void ReadSampleFile_A(uint NumFile)
{
    ReadSampleFile_TSP(NumFile);
}


void SaveSampleFile_B(void)
{
	u8 *p;
	u8 i;
	u8 * file_ptr;
	u8 file_overflow_flag = 0;
	u32 addr;
	u16 sample_file_max;
    sample_file_max = DEF_SAMPLE_FILE_MAX_W25;
	tsp_sample_file.num++;
	if(tsp_sample_file.num > sample_file_max)
	{
		file_overflow_flag = 1;
		tsp_sample_file.num = 1;
		tsp_sample_file.fg_flag = 1;
		
		if(sample_file_bank == FIRST_BANK)
		{
			sample_file_bank = SECOND_BANK;
		}
		else
		{
			sample_file_bank = FIRST_BANK;		
		}
	}
	
 	if(sample_file_bank == FIRST_BANK)
	{
		addr = (u32)DEF_SAMPLE_FILE_FIRST_START_PAGE_W25 * 256 + (u32)(tsp_sample_file.num - 1) * 128;	
	}
	else
	{
		addr = (u32)DEF_SAMPLE_FILE_SECOND_START_PAGE_W25 * 256 + (u32)(tsp_sample_file.num - 1) * 128;
	}

	p = (uchar *)&B_sample_start;
	for(i = 0;i < 7; i++)
	{
		*(p++) = R_24(ADDR_B_sample_start+i);	//注：从FM24C64中读取临时文件内容
	}
	B_sample_file.channel = DF_CHANNEL_B;
	B_sample_file.year  = B_sample_start.year;
	B_sample_file.month = B_sample_start.month;
	B_sample_file.day   = B_sample_start.day;
	B_sample_file.hour  = B_sample_start.hour;
	B_sample_file.min   = B_sample_start.min;
	B_sample_file.sec   = B_sample_start.sec;
	B_sample_file.running_time = B_SAMPLE.running_time_set - B_SAMPLE.running_time;//已经采样时间
	B_sample_file.flow_set = B_SAMPLE.flow_set;
	B_sample_file.volume = B_SAMPLE.volume;
	B_sample_file.volume_biaokuang = B_SAMPLE.volume_biaokuang;
	file_ptr = (uchar *)(& B_sample_file);
	SPI_DATA_WR_Page(file_ptr,addr,DEF_SAMPLE_FILE_LENGTH_W25);

	if(sample_file_bank == FIRST_BANK)
	{
		if((tsp_sample_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_SAMPLE_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_sample_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_sample_file.num == sample_file_max)
		{
			addr = (u32)DEF_SAMPLE_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_sample_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}
	}
	else
	{
		if((tsp_sample_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_SAMPLE_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_sample_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_sample_file.num == sample_file_max)
		{
			addr = (u32)DEF_SAMPLE_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_sample_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}	
	}
	if(file_overflow_flag == 1)
	{
		W_24(ADDR_tsp_sample_file_fg_flag,tsp_sample_file.fg_flag);
		W_24(ADDR_sample_file_bank,sample_file_bank);
	}
	saveintdata(ADDR_tsp_sample_file_num,tsp_sample_file.num);//保存当前文件号地址
}

void ReadSampleFile_B(uint NumFile)
{
    ReadSampleFile_TSP(NumFile);
}



void SavePowerFile_TSP(void)
{
	u8 i;
	u8 *p;
	u8 * file_ptr;
	u8 file_overflow_flag = 0;
	u32 addr;
	u16 power_file_max;
    power_file_max = DEF_POWER_FILE_MAX_W25;
	tsp_power_lose_file.num++;
	if(tsp_power_lose_file.num > power_file_max)
	{
		file_overflow_flag = 1;
		tsp_power_lose_file.num = 1;
		tsp_power_lose_file.fg_flag = 1;
		
		if(power_file_bank == FIRST_BANK)
		{
			power_file_bank = SECOND_BANK;
		}
		else
		{
			power_file_bank = FIRST_BANK;		
		}
	}
	
 	if(power_file_bank == FIRST_BANK)
	{
		addr = (u32)DEF_POWER_FILE_FIRST_START_PAGE_W25 * 256 + (u32)(tsp_power_lose_file.num - 1) * 128;	
	}
	else
	{
		addr = (u32)DEF_POWER_FILE_SECOND_START_PAGE_W25 * 256 + (u32)(tsp_power_lose_file.num - 1) * 128;	
	}

	p = (uchar *)&TSP_power_lose;
	for(i = 0;i < 7; i++)
	{
		*(p++) = R_24(ADDR_TSP_power_lose+i);
	}
	power_file.channel = DF_CHANNEL_TSP;
	power_file.year = TSP_power_lose.year;
	power_file.month = TSP_power_lose.month;
	power_file.day = TSP_power_lose.day;
	power_file.hour = TSP_power_lose.hour;
	power_file.min = TSP_power_lose.min;
	power_file.sec = TSP_power_lose.sec;

	power_file.power_year = Year;
	power_file.power_month = Month;
	power_file.power_day = Day;
	power_file.power_hour = Hour;
	power_file.power_min = Min;
	power_file.power_sec = Sec;

	power_file.lose_hour = LostPowerTime/3600;
	power_file.lose_min = (LostPowerTime%3600)/60;
	power_file.lose_sec = LostPowerTime%60;
	
	file_ptr = (uchar *)(& power_file);

	SPI_DATA_WR_Page(file_ptr,addr,DEF_POWER_FILE_LENGTH_W25);

	if(power_file_bank == FIRST_BANK)
	{
		if((tsp_power_lose_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_POWER_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_power_lose_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_power_lose_file.num == power_file_max)
		{
			addr = (u32)DEF_POWER_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_power_lose_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}
	}
	else
	{
		if((tsp_power_lose_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_POWER_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_power_lose_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_power_lose_file.num == power_file_max)
		{
			addr = (u32)DEF_POWER_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_power_lose_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}	
	}
	if(file_overflow_flag == 1)
	{
		W_24(ADDR_tsp_power_lose_file_fg_flag,tsp_power_lose_file.fg_flag);
		W_24(ADDR_power_file_bank,power_file_bank);
	}
	saveintdata(ADDR_tsp_power_lose_file_num,tsp_power_lose_file.num);//保存当前文件号地址
}

void ReadPowerFile_TSP(uint NumFile)
{
    u32 addr;
	if(power_file_bank == FIRST_BANK)
	{
		if(NumFile <= tsp_power_lose_file.num)
		{
			addr = (u32)DEF_POWER_FILE_FIRST_START_PAGE_W25 * 256 + (u32)(NumFile - 1) * 128;	
		}
		else
		{
			addr = (u32)DEF_POWER_FILE_SECOND_START_PAGE_W25 * 256 + (u32)(NumFile - 1) * 128;
		}
	}
	else
	{
		if(NumFile <= tsp_power_lose_file.num)
		{
			addr = (u32)DEF_POWER_FILE_SECOND_START_PAGE_W25 * 256 + (u32)(NumFile - 1) * 128;	
		}
		else
		{
			addr = (u32)DEF_POWER_FILE_FIRST_START_PAGE_W25 * 256 + (u32)(NumFile - 1) * 128;
		}	
	}

	SPI_Data_Read((u8 *)(&power_file_chaxun),addr,DEF_POWER_FILE_LENGTH_W25);
}





void SavePowerFile_A(void)
{
	u8 i;
	u8 *p;
	u8 * file_ptr;
	u8 file_overflow_flag = 0;
	u32 addr;
	u16 power_file_max;
    power_file_max = DEF_POWER_FILE_MAX_W25;
	tsp_power_lose_file.num++;
	if(tsp_power_lose_file.num > power_file_max)
	{
		file_overflow_flag = 1;
		tsp_power_lose_file.num = 1;
		tsp_power_lose_file.fg_flag = 1;
		
		if(power_file_bank == FIRST_BANK)
		{
			power_file_bank = SECOND_BANK;
		}
		else
		{
			power_file_bank = FIRST_BANK;		
		}
	}
	
 	if(power_file_bank == FIRST_BANK)
	{
		addr = (u32)DEF_POWER_FILE_FIRST_START_PAGE_W25 * 256 + (u32)(tsp_power_lose_file.num - 1) * 128;	
	}
	else
	{
		addr = (u32)DEF_POWER_FILE_SECOND_START_PAGE_W25 * 256 + (u32)(tsp_power_lose_file.num - 1) * 128;	
	}

	p = (uchar *)&A_power_lose;
	for(i = 0;i < 7; i++)
	{
		*(p++) = R_24(ADDR_A_power_lose+i);
	}
	A_power_file.channel = DF_CHANNEL_A;
	A_power_file.year = A_power_lose.year;
	A_power_file.month = A_power_lose.month;
	A_power_file.day = A_power_lose.day;
	A_power_file.hour = A_power_lose.hour;
	A_power_file.min = A_power_lose.min;
	A_power_file.sec = A_power_lose.sec;

	A_power_file.power_year = Year;
	A_power_file.power_month = Month;
	A_power_file.power_day = Day;
	A_power_file.power_hour = Hour;
	A_power_file.power_min = Min;
	A_power_file.power_sec = Sec;

	A_power_file.lose_hour = A_LostPowerTime/3600;
	A_power_file.lose_min = (A_LostPowerTime%3600)/60;
	A_power_file.lose_sec = A_LostPowerTime%60;
	
	file_ptr = (uchar *)(& A_power_file);

	SPI_DATA_WR_Page(file_ptr,addr,DEF_POWER_FILE_LENGTH_W25);

	if(power_file_bank == FIRST_BANK)
	{
		if((tsp_power_lose_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_POWER_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_power_lose_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_power_lose_file.num == power_file_max)
		{
			addr = (u32)DEF_POWER_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_power_lose_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}
	}
	else
	{
		if((tsp_power_lose_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_POWER_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_power_lose_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_power_lose_file.num == power_file_max)
		{
			addr = (u32)DEF_POWER_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_power_lose_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}	
	}
	if(file_overflow_flag == 1)
	{
		W_24(ADDR_tsp_power_lose_file_fg_flag,tsp_power_lose_file.fg_flag);
		W_24(ADDR_power_file_bank,power_file_bank);
	}
	saveintdata(ADDR_tsp_power_lose_file_num,tsp_power_lose_file.num);//保存当前文件号地址
}

void ReadPowerFile_A(uint NumFile)
{
    ReadPowerFile_TSP(NumFile);
}






void SavePowerFile_B(void)
{
	u8 i;
	u8 *p;
	u8 * file_ptr;
	u8 file_overflow_flag = 0;
	u32 addr;
	u16 power_file_max;
    power_file_max = DEF_POWER_FILE_MAX_W25;
	tsp_power_lose_file.num++;
	if(tsp_power_lose_file.num > power_file_max)
	{
		file_overflow_flag = 1;
		tsp_power_lose_file.num = 1;
		tsp_power_lose_file.fg_flag = 1;
		
		if(power_file_bank == FIRST_BANK)
		{
			power_file_bank = SECOND_BANK;
		}
		else
		{
			power_file_bank = FIRST_BANK;		
		}
	}
	
 	if(power_file_bank == FIRST_BANK)
	{
		addr = (u32)DEF_POWER_FILE_FIRST_START_PAGE_W25 * 256 + (u32)(tsp_power_lose_file.num - 1) * 128;	
	}
	else
	{
		addr = (u32)DEF_POWER_FILE_SECOND_START_PAGE_W25 * 256 + (u32)(tsp_power_lose_file.num - 1) * 128;	
	}

	p = (uchar *)&B_power_lose;
	for(i = 0;i < 7; i++)
	{
		*(p++) = R_24(ADDR_B_power_lose+i);
	}
	B_power_file.channel = DF_CHANNEL_B;
	B_power_file.year = B_power_lose.year;
	B_power_file.month = B_power_lose.month;
	B_power_file.day = B_power_lose.day;
	B_power_file.hour = B_power_lose.hour;
	B_power_file.min = B_power_lose.min;
	B_power_file.sec = B_power_lose.sec;

	B_power_file.power_year = Year;
	B_power_file.power_month = Month;
	B_power_file.power_day = Day;
	B_power_file.power_hour = Hour;
	B_power_file.power_min = Min;
	B_power_file.power_sec = Sec;

	B_power_file.lose_hour = B_LostPowerTime/3600;
	B_power_file.lose_min = (B_LostPowerTime%3600)/60;
	B_power_file.lose_sec = B_LostPowerTime%60;
	
	file_ptr = (uchar *)(& B_power_file);

	SPI_DATA_WR_Page(file_ptr,addr,DEF_POWER_FILE_LENGTH_W25);

	if(power_file_bank == FIRST_BANK)
	{
		if((tsp_power_lose_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_POWER_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_power_lose_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_power_lose_file.num == power_file_max)
		{
			addr = (u32)DEF_POWER_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_power_lose_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}
	}
	else
	{
		if((tsp_power_lose_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_POWER_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_power_lose_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_power_lose_file.num == power_file_max)
		{
			addr = (u32)DEF_POWER_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_power_lose_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}	
	}
	if(file_overflow_flag == 1)
	{
		W_24(ADDR_tsp_power_lose_file_fg_flag,tsp_power_lose_file.fg_flag);
		W_24(ADDR_power_file_bank,power_file_bank);
	}
	saveintdata(ADDR_tsp_power_lose_file_num,tsp_power_lose_file.num);//保存当前文件号地址
}

void ReadPowerFile_B(uint NumFile)
{
    ReadPowerFile_TSP(NumFile);
}




void SaveRunFile_TSP(void)
{
	u8 * file_ptr;
	u8 file_overflow_flag = 0;
	u32 addr;
	u16 run_file_max;
    run_file_max = DEF_RUN_FILE_MAX_W25;
	tsp_run_file.num++;
	if(tsp_run_file.num > run_file_max)
	{
		file_overflow_flag = 1;
		tsp_run_file.num = 1;
		tsp_run_file.fg_flag = 1;
		
		if(run_file_bank == FIRST_BANK)
		{
			run_file_bank = SECOND_BANK;
		}
		else
		{
			run_file_bank = FIRST_BANK;		
		}
	}
	
 	if(run_file_bank == FIRST_BANK)
	{
		addr = (u32)DEF_RUN_FILE_FIRST_START_PAGE_W25 * 256 + (u32)(tsp_run_file.num - 1) * 128;	
	}
	else
	{
		addr = (u32)DEF_RUN_FILE_SECOND_START_PAGE_W25 * 256 + (u32)(tsp_run_file.num - 1) * 128;	
	}
	
	
	run_file.year = Year;
	run_file.month = Month;
	run_file.day = Day;
	run_file.hour = Hour;
	run_file.min = Min;
	run_file.sec = Sec;

	run_file.flow = TSP_SAMPLE.flow;
	run_file.environment_temperature = TSP_SAMPLE.environment_temperature_input;
	run_file.environment_barometric = TSP_SAMPLE.barometric_input;
	run_file.volume_biaokuang = TSP_SAMPLE.volume_biaokuang;

	file_ptr = (uchar *)(& run_file);

	SPI_DATA_WR_Page(file_ptr,addr,DEF_RUN_FILE_LENGTH_W25);

	if(run_file_bank == FIRST_BANK)
	{
		if((tsp_run_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_RUN_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_run_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_run_file.num == run_file_max)
		{
			addr = (u32)DEF_RUN_FILE_SECOND_START_PAGE_W25 * 256 + (u32)((tsp_run_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}
	}
	else
	{
		if((tsp_run_file.num % 32) == 0)				//写满一个扇区
		{
			addr = (u32)DEF_RUN_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_run_file.num -1) / 2) * 256;		
			SPI_Flash_Erase_Sector(addr);
		}
		else if(tsp_run_file.num == run_file_max)
		{
			addr = (u32)DEF_RUN_FILE_FIRST_START_PAGE_W25 * 256 + (u32)((tsp_run_file.num -1) / 2) * 256;
			SPI_Flash_Erase_Sector(addr);
		}	
	}
	if(file_overflow_flag == 1)
	{
		W_24(ADDR_tsp_run_file_fg_flag,tsp_run_file.fg_flag);
		W_24(ADDR_run_file_bank,run_file_bank);
	}
	saveintdata(ADDR_tsp_run_file_num,tsp_run_file.num);//保存当前文件号地址
}


void ReadRunFile_TSP(uint NumFile)
{
    u32 addr;
	if(run_file_bank == FIRST_BANK)
	{
		if(NumFile <= tsp_run_file.num)
		{
			addr = (u32)DEF_RUN_FILE_FIRST_START_PAGE_W25 * 256 + (u32)(NumFile - 1) * 128;	
		}
		else
		{
			addr = (u32)DEF_RUN_FILE_SECOND_START_PAGE_W25 * 256 + (u32)(NumFile - 1) * 128;
		}
	}
	else
	{
		if(NumFile <= tsp_run_file.num)
		{
			addr = (u32)DEF_RUN_FILE_SECOND_START_PAGE_W25 * 256 + (u32)(NumFile - 1) * 128;	
		}
		else
		{
			addr = (u32)DEF_RUN_FILE_FIRST_START_PAGE_W25 * 256 + (u32)(NumFile - 1) * 128;
		}	
	}

	SPI_Data_Read((u8 *)(&run_file_chaxun),addr,DEF_RUN_FILE_LENGTH_W25);
}




uint W25_erase_sector = 0;
//删除所有文件 从0号文件开始
void DeleteAllSampleFiles(void)
{
	float percent;
	u32 sector_tmp,sector_tmp2;
	sector_tmp = DEF_SAMPLE_FILE_FIRST_START_PAGE_W25 / 16;
	sector_tmp2 = DEF_SAMPLE_FILE_FIRST_START_PAGE_W25 / 16 + ((DEF_SAMPLE_FILE_MAX_W25 / 2) / 16 + 1);
	while(sector_tmp <= sector_tmp2)
	{
		SPI_Flash_Erase_Sector(sector_tmp*4096);
		sector_tmp++;
		W25_erase_sector++;
		percent = (float)W25_erase_sector/2004.0*100.0;
		if(percent > 99.9)
		{
			percent = 99.9;
		}
//		Putf(10,140,(Uchar *)"31",percent,24,0);
	}
	sector_tmp = DEF_SAMPLE_FILE_SECOND_START_PAGE_W25 / 16;
	sector_tmp2 = DEF_SAMPLE_FILE_SECOND_START_PAGE_W25 / 16 + ((DEF_SAMPLE_FILE_MAX_W25 / 2) / 16 + 1);
	while(sector_tmp <= sector_tmp2)
	{
		SPI_Flash_Erase_Sector(sector_tmp*4096);
		sector_tmp++;
		W25_erase_sector++;
		percent = (float)W25_erase_sector/2004.0*100.0;
		if(percent > 99.9)
		{
			percent = 99.9;
		}
//		Putf(10,140,(Uchar *)"31",percent,24,0);
	}
}

//删除所有实时存储数据文件
void DeleteAllRunFile(void)
{
	float percent;
	u32 sector_tmp,sector_tmp2;
	sector_tmp = DEF_RUN_FILE_FIRST_START_PAGE_W25 / 16;
	sector_tmp2 = DEF_RUN_FILE_FIRST_START_PAGE_W25 / 16 + ((DEF_RUN_FILE_MAX_W25 / 2) / 16 + 1);
	while(sector_tmp <= sector_tmp2)
	{
		SPI_Flash_Erase_Sector(sector_tmp*4096);
		sector_tmp++;
		W25_erase_sector++;
		percent = (float)W25_erase_sector/2004.0*100.0;
		if(percent > 99.9)
		{
			percent = 99.9;
		}
//		Putf(10,140,(Uchar *)"31",percent,24,0);
	}
	sector_tmp = DEF_RUN_FILE_SECOND_START_PAGE_W25 / 16;
	sector_tmp2 = DEF_RUN_FILE_SECOND_START_PAGE_W25 / 16 + ((DEF_RUN_FILE_MAX_W25 / 2) / 16 + 1);
	while(sector_tmp <= sector_tmp2)
	{
		SPI_Flash_Erase_Sector(sector_tmp*4096);
		sector_tmp++;
		W25_erase_sector++;
		percent = (float)W25_erase_sector/2004.0*100.0;
		if(percent > 99.9)
		{
			percent = 99.9;
		}
//		Putf(10,140,(Uchar *)"31",percent,24,0);
	}
}

//删除所有掉电文件
void DeletAllPowerFile(void)
{
	float percent;
	u32 sector_tmp,sector_tmp2;
	sector_tmp = DEF_POWER_FILE_FIRST_START_PAGE_W25 / 16;
	sector_tmp2 = DEF_POWER_FILE_FIRST_START_PAGE_W25 / 16 + ((DEF_POWER_FILE_MAX_W25 / 2) / 16 + 1);
	while(sector_tmp <= sector_tmp2)
	{
		SPI_Flash_Erase_Sector(sector_tmp*4096);
		sector_tmp++;
		W25_erase_sector++;
		percent = (float)W25_erase_sector/2004.0*100.0;
		if(percent > 99.9)
		{
			percent = 99.9;
		}
//		Putf(10,140,(Uchar *)"31",percent,24,0);
	}
	sector_tmp = DEF_POWER_FILE_SECOND_START_PAGE_W25 / 16;
	sector_tmp2 = DEF_POWER_FILE_SECOND_START_PAGE_W25 / 16 + ((DEF_POWER_FILE_MAX_W25 / 2) / 16 + 1);
	while(sector_tmp <= sector_tmp2)
	{
		SPI_Flash_Erase_Sector(sector_tmp*4096);
		sector_tmp++;
		W25_erase_sector++;
		percent = (float)W25_erase_sector/2004.0*100.0;
		if(percent > 99.9)
		{
			percent = 99.9;
		}
//		Putf(10,140,(Uchar *)"31",percent,24,0);
	}
}

void DeleteAllFiles(void)
{
	uchar cbuf[10];
	SPI_Flash_Erase_Chip();
	cbuf[0] = 0x55;
	cbuf[1] = 0xAA;
	SPI_DATA_WR_Page(cbuf,DEF_FIRST_START_FLAG_W25,2);
}

void FileInit(void)
{
	uchar cbuf[10];
	SPI_Data_Read(cbuf,DEF_FIRST_START_FLAG_W25,2);
	if((cbuf[0] != 0x55) || (cbuf[1] != 0xAA))
	{
		DMT_ChangePic(DF_PIC_FLASH_INIT);
		SPI_Flash_Erase_Chip();
		cbuf[0] = 0x55;
		cbuf[1] = 0xAA;
		SPI_DATA_WR_Page(cbuf,DEF_FIRST_START_FLAG_W25,2);
		tsp_sample_file.fg_flag = 0;
		tsp_sample_file.num = 0;
		tsp_power_lose_file.fg_flag = 0;
		tsp_power_lose_file.num = 0;
		tsp_run_file.fg_flag = 0;
		tsp_run_file.num = 0;
		W_24(ADDR_tsp_sample_file_fg_flag,tsp_sample_file.fg_flag);
		saveintdata(ADDR_tsp_sample_file_num,tsp_sample_file.num);
		W_24(ADDR_tsp_power_lose_file_fg_flag,tsp_power_lose_file.fg_flag);
		saveintdata(ADDR_tsp_power_lose_file_num,tsp_power_lose_file.num);
		W_24(ADDR_tsp_run_file_fg_flag,tsp_run_file.fg_flag);
		saveintdata(ADDR_tsp_run_file_num,tsp_run_file.num);
	}
	else
	{
		tsp_sample_file.fg_flag = R_24(ADDR_tsp_sample_file_fg_flag);
		tsp_sample_file.num = readintdata(ADDR_tsp_sample_file_num);
		tsp_power_lose_file.fg_flag = R_24(ADDR_tsp_power_lose_file_fg_flag);
		tsp_power_lose_file.num = readintdata(ADDR_tsp_power_lose_file_num);
		tsp_run_file.fg_flag = R_24(ADDR_tsp_run_file_fg_flag);
		tsp_run_file.num = readintdata(ADDR_tsp_run_file_num);
		if(tsp_sample_file.num > DF_sample_file_max)
		{
			tsp_sample_file.num = DF_sample_file_max;
			saveintdata(ADDR_tsp_sample_file_num,tsp_sample_file.num);
		}
		if(tsp_power_lose_file.num > DF_power_lose_file_max)
		{
			tsp_power_lose_file.num = DF_power_lose_file_max;
			saveintdata(ADDR_tsp_power_lose_file_num,tsp_power_lose_file.num);
		}
		if(tsp_run_file.num > DF_run_file_max)
		{
			tsp_run_file.num = DF_run_file_max;
			saveintdata(ADDR_tsp_run_file_num,tsp_run_file.num);
		}
	}
}
/*
void test_file_write(void)
{
	uint iii;
	for(iii = 0; iii < 1996; iii++)
	{
		SampleFile.file_sample_volume[0] = iii;
		SaveSampleFile();
	}
	for(iii = 0; iii < 1996; iii++)
	{
		PowerFile.power_file_dsec = iii;
		SavePowerFile();
	}
	for(iii = 0; iii < 59998; iii++)
	{
		RunFile.volume_biaokuang = iii;
		SaveRunFile();
	}
}
*/
#ifdef fielfief
u16 Serch_SamFile()
{
   u16 FileNo;
   EEPROMtoRAM(CURSAMFILE_ADD_1,(u8 *)(&SamCurFile),CURSAMFILE_SIZE);
   FileNo = SamCurFile.File;
   if(SamCurFile.OvFlag == 1)
   {
      //已存满
   }
   else
   {
      while(1)
	   {
	      ReadSamCurFile(FileNo);

		   if(g.SerTimeYear>FileSam.StartTimeYear + 2000)
		   {
		      return FileNo;
		   }
		   if((g.SerTimeYear==FileSam.StartTimeYear + 2000)&&(g.SerTimeMonth > FileSam.StartTimeMonth))
		   {
		      return FileNo;
		   }
		   if((g.SerTimeYear == FileSam.StartTimeYear + 2000)&&(g.SerTimeMonth == FileSam.StartTimeMonth)&&(g.SerTimeDay > FileSam.StartTimeDay))
	      {
		      return FileNo;
		   }
		   if((g.SerTimeYear == FileSam.StartTimeYear + 2000)&&(g.SerTimeMonth == FileSam.StartTimeMonth)&&(g.SerTimeDay == FileSam.StartTimeDay) &&(g.SerTimeHour > FileSam.StartTimeHour))
		   {
		      return FileNo;
		   }
		   if((g.SerTimeYear == FileSam.StartTimeYear + 2000)&&(g.SerTimeMonth == FileSam.StartTimeMonth)&&(g.SerTimeDay == FileSam.StartTimeDay)&&(g.SerTimeHour == FileSam.StartTimeHour)&&(g.SerTimeMin >= FileSam.StartTimeMin))
		   {
		      return FileNo;
		   }
		   if(FileNo>0)
		   {
		      FileNo--;
		   }
		   if(FileNo == 0)
		   {
		      break;
		   }
	   }
   }
   return FileNo;
}
u16 Serch_RealDataFile()
{
   u16 FileNo;
   EEPROMtoRAM(CURREALDATAFILE_ADD_1,(u8 *)(&RealDataCurFile),CURREALDATAFILE_SIZE);
   FileNo = RealDataCurFile.File;
   /*if(RealDataCurFile.OvFlag == 1)
   {
      //已存满()
   }*/
   //else
   {
		while(1)
		{
		    ReadRealDataCurFile(FileNo); 
			 if(g.SerTimeYear > RealDataFile.RealTimeYear+2000)
			 {
			    return FileNo;
			    //return 1;
			 }
			 if((g.SerTimeYear == RealDataFile.RealTimeYear + 2000)&&(g.SerTimeMonth > RealDataFile.RealTimeMonth))
			 {
			    return FileNo;
			    //return 2;
			 }
			
			 if((g.SerTimeYear == RealDataFile.RealTimeYear + 2000)&&(g.SerTimeMonth == RealDataFile.RealTimeMonth)&&(g.SerTimeDay > RealDataFile.RealTimeDay))
			 {
			    return FileNo;
			    //return 3;
			 }
			
			 if((g.SerTimeYear == RealDataFile.RealTimeYear + 2000)&&(g.SerTimeMonth == RealDataFile.RealTimeMonth)&&(g.SerTimeDay == RealDataFile.RealTimeDay) &&(g.SerTimeHour > RealDataFile.RealTimeHour))
			 {
			    return FileNo;
			    //return 4;
			 }
			
			 if((g.SerTimeYear == RealDataFile.RealTimeYear + 2000)&&(g.SerTimeMonth == RealDataFile.RealTimeMonth)&&(g.SerTimeDay == RealDataFile.RealTimeDay)&&(g.SerTimeHour == RealDataFile.RealTimeHour)&&(g.SerTimeMin >= RealDataFile.RealTimeMin))
			 {
			    return FileNo;
			    //return 5;
			 }
		    FileNo--;
			 if(FileNo == 0)
			 {
			    break;
			 }
		}
	}
	return FileNo;
	//return 6;
}

u16 Serch_AbNormalFile()
{
   u16 FileNo;
   EEPROMtoRAM(CURABNORMALFILE_ADD_1,(u8 *)(&AbNormalCurFile),CURABNORMALFILE_SIZE);
   FileNo = AbNormalCurFile.File;
   if(AbNormalCurFile.OvFlag == 1)
   {
      //已存满()
   }
   else
   {
		while(1)
		{
		   ReadAbNormalCurFile(FileNo);
			if(g.SerTimeYear > AbNormalFile.PowerTimeYear+2000)
			{
			   return FileNo;
			}
			if((g.SerTimeYear == AbNormalFile.PowerTimeYear + 2000)&&(g.SerTimeMonth > AbNormalFile.PowerTimeMonth))
			{
			   return FileNo;
			}
			
			if((g.SerTimeYear == AbNormalFile.PowerTimeYear + 2000)&&(g.SerTimeMonth == AbNormalFile.PowerTimeMonth)&&(g.SerTimeDay > AbNormalFile.PowerTimeDay))
			{
			   return FileNo;
			}
			
			if((g.SerTimeYear == AbNormalFile.PowerTimeYear + 2000)&&(g.SerTimeMonth == AbNormalFile.PowerTimeMonth)&&(g.SerTimeDay == AbNormalFile.PowerTimeDay) &&(g.SerTimeHour > AbNormalFile.PowerTimeHour))
			{
			   return FileNo;
			}
			
			if((g.SerTimeYear == AbNormalFile.PowerTimeYear + 2000)&&(g.SerTimeMonth == AbNormalFile.PowerTimeMonth)&&(g.SerTimeDay == AbNormalFile.PowerTimeDay)&&(g.SerTimeHour == AbNormalFile.PowerTimeHour)&&(g.SerTimeMin >= AbNormalFile.PowerTimeMin))
			{
			   return FileNo;
			}
		    FileNo--;
			if(FileNo == 0)
			{
			   break;
			}
		}
	}
	return FileNo;
}

u16 Serch_QiXiangFile(void)
{
   u16 FileNo;
   GetSaveQiXiangFileNo();									 
   FileNo = QiXiangCurFile.File-1;
   if(QiXiangCurFile.OvFlag == 1)
   {
      //已存满()
   }
   else
   {
		while(1)
		{
		   ReadQiXiangCurFile(FileNo); 
			if(g.SerTimeYear > QiXiangFile.QiXiangYear+2000)
			{
			   return FileNo;
			}
			if((g.SerTimeYear == QiXiangFile.QiXiangYear + 2000)&&(g.SerTimeMonth > QiXiangFile.QiXiangMonth))
			{
			   return FileNo;
			}
			
			if((g.SerTimeYear == QiXiangFile.QiXiangYear + 2000)&&(g.SerTimeMonth == QiXiangFile.QiXiangMonth)&&(g.SerTimeDay > QiXiangFile.QiXiangDay))
			{
			   return FileNo;
			}
			
			if((g.SerTimeYear == QiXiangFile.QiXiangYear + 2000)&&(g.SerTimeMonth == QiXiangFile.QiXiangMonth)&&(g.SerTimeDay == QiXiangFile.QiXiangDay) &&(g.SerTimeHour > QiXiangFile.QiXiangHour))
			{
			   return FileNo;
			}
			
			if((g.SerTimeYear == QiXiangFile.QiXiangYear + 2000)&&(g.SerTimeMonth == QiXiangFile.QiXiangMonth)&&(g.SerTimeDay == QiXiangFile.QiXiangDay)&&(g.SerTimeHour == QiXiangFile.QiXiangHour)&&(g.SerTimeMin >= QiXiangFile.QiXiangMin))
			{
			   return FileNo;
			}
		   FileNo--;
			if(FileNo == 0)
			{
			   break;
			}
		}
	}
	return FileNo;
	//return 6;
}
#endif
