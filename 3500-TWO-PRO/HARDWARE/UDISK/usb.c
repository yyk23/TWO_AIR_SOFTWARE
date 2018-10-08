//--------include------------
#include "sys.h"  
#include "sample.h"
#include "process.h"
#include "usb.h"
#include "file.h"
#include "delay.h"
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "usbh_usr.h" 
#include "exfuns.h" 
#include <string.h>
#include <stdio.h>
#include "ZR3500_CONFIG.h"
#define DEF_SECTOR_SIZE		512
extern u16 usb_out_start_file;
extern u16 usb_out_end_file;

u8 CHECKFILE = 0;				/*校验不等于1时写入，校验为1时读取以判断写入是否正确*/
u8 LoadFileBuffer(u8 * pFile,u8 flag);
u32 DataSector = 0;										//数据当前扇区
u8  cancelflag = 0;										/*取消导出文件，防止未插入U盘一直等待问题*/
u8 usb_out_file_step;
u32 NewSize;
u32 SectorNumber;
u16 total;
void write_test(u8 * p);
//----------------------------------------------

//------将程序空间的字符串复制到内部RAM中,返回字符串长度
u8	mCopyCodeStringToIRAM(u8 *iDestination, const u8 *iSource )
{
	u8	i=0;
	while(*iDestination == *iSource )
	{
		iDestination++;
		iSource++;
		i++;
	}
	return(i);
}


//------将字符串复制到内部RAM中,返回字符串长度
u8	mCopyStringToIRAM(u8 *iDestination, u8 *iSource )
{
	u8	i=0;
	while(*iDestination == *iSource ) 
	{
		iDestination++;
		iSource++;
		i++;
	}
	return(i);
}
void mFlushBufferToDisk(u8 force)
// force = 0 则自动刷新(检查缓冲区中的数据长度,满则写盘,不满则暂时放在缓冲区中), force != 0 则强制刷新(不管缓冲区中的数据有多少都写盘,通常在系统关机前应该强制写盘)
{
	if(force != 0) 
	{  // 强制刷新
		f_write(file,FILE_DATA_BUF,total,&bw);
		total = 0;
		f_sync(file);
	}
	else if(total >= DEF_SECTOR_SIZE) 
	{  // 缓冲区中的数据快要满了,所以应该先将缓冲区中的原有数据写入U盘
		f_write(file,FILE_DATA_BUF,total,&bw);
		total = 0;
		f_sync(file);
	}
}

//将pFile转载到文件缓冲区FILE_DATA_BUF，直到512字节
//装满返回1，否则返回0
	u16 Load_datacount=0;			//已装入扇区计数
	u16 pFile_datacount=0;			//已装入计数
u8 FILE_DATA_BUF[712];		//500+200 (200为一次最大数据个数)

u16 my_sprintf(u8 *array,u8 *pstr,u16 flag)
{
    u16 i = 0;
    while(*pstr)
    {
        *(array + i) = (*pstr);
        i++;
        pstr += 1;
    }
    if(flag == 1)
    {
        *(array + i) = '\xd';	   //回车
        *(array + i + 1) = '\xa';	   //换行
        i += 2;
    }
    else
    {
    }
    return i;
}

u8 	testbuf[200];
void SampleFileOut(u16 start_file,u16 end_file)
{
	u8 hour_tmp;
	u8 min_tmp;
	u8 sec_tmp;
    u8 strbuf[20];
    u8 bianhao_buf[10];
	if(usb_out_file_step == DF_FILE_HEAD_LILE)
	{
		usb_out_file_step = DF_FILE_CONTENT;
		return;
	}	
	if(usb_out_file_step == DF_FILE_CONTENT)
	{
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"",1);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"",1);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"ZR-3500型大气采样器 ",0);
		mFlushBufferToDisk( 0 );
#ifdef ZR3500TWO
        strcpy(strbuf,(char*)ZR3500TWO_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"仪器编号  ",0);
		mFlushBufferToDisk( 0 );
		SPutl(8,bianhao_buf,bianhao_new);
        strcpy(strbuf,(char*)ZR3500TWO_NAME);
        strcat(strbuf,bianhao_buf);
		total += my_sprintf(FILE_DATA_BUF+total,strbuf,1);
		mFlushBufferToDisk( 0 );       
#endif
#ifdef ZR3500TWO_HEAT
        strcpy(strbuf,(char*)ZR3500TWO_HEAT_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"仪器编号  ",0);
		mFlushBufferToDisk( 0 );
		SPutl(8,bianhao_buf,bianhao_new);
        strcpy(strbuf,(char*)ZR3500TWO_HEAT_NAME);
        strcat(strbuf,bianhao_buf);
		total += my_sprintf(FILE_DATA_BUF+total,strbuf,1);
		mFlushBufferToDisk( 0 );   
        
#endif
#ifdef ZR3500TWO_HEATandCOOL
        strcpy(strbuf,(char*)ZR3500TWO_HEATandCOOL_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"仪器编号  ",0);
		mFlushBufferToDisk( 0 );
		SPutl(8,bianhao_buf,bianhao_new);
        strcpy(strbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
        strcat(strbuf,bianhao_buf);
		total += my_sprintf(FILE_DATA_BUF+total,strbuf,1);
		mFlushBufferToDisk( 0 );   
#endif
		ReadSampleFile_AB(usb_out_start_file);
		
		if(sample_file_chaxun.channel == DF_CHANNEL_A)
		{
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"A 路采样",1);
		}
		else
		{
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"B 路采样",1);		
		}
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"文 件 号  ",0);
		mFlushBufferToDisk( 0 );
		SPuti(4,testbuf,usb_out_start_file);		//文件号
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"采样日期  20",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.year);						//年
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.month);						//月
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.day);						//日
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.hour);						//时
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.min);						//分
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.sec);						//秒
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"采样时长  ",0);
		mFlushBufferToDisk( 0 );
		hour_tmp = sample_file_chaxun.running_time / 3600;
		min_tmp = (sample_file_chaxun.running_time % 3600) / 60;
		sec_tmp = sample_file_chaxun.running_time % 60;
		CharToStr(2,testbuf,hour_tmp);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,min_tmp);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sec_tmp);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"环境温度  ",0);
		mFlushBufferToDisk( 0 );
		SPutf(sf31,testbuf,sample_file_chaxun.environment_temperature);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"℃",1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"大 气 压  ",0);
		mFlushBufferToDisk( 0 );
		SPutf(f52,testbuf,sample_file_chaxun.barometric);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"kPa",1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"设定流量  ",0);
		mFlushBufferToDisk( 0 );
		SPutf(f21,testbuf,sample_file_chaxun.flow_set);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"L/min",1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"计前压力  ",0);
		mFlushBufferToDisk( 0 );
		SPutf(sf42,testbuf,sample_file_chaxun.jy);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"kPa",1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"计前温度  ",0);
		mFlushBufferToDisk( 0 );
		SPutf(sf31,testbuf,sample_file_chaxun.temperature);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"℃",1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"累积体积  ",0);
		mFlushBufferToDisk( 0 );
		SPutf(f62,(u8 *)testbuf,sample_file_chaxun.volume);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"L",1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"标况体积  ",0);
		mFlushBufferToDisk( 0 );
		SPutf(f62,(u8 *)testbuf,sample_file_chaxun.volume_biaokuang);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"L",1);
		mFlushBufferToDisk( 0 );
		if(err_dis == 1)
		{
			if(sample_file_chaxun.success == DF_JY_OUT)
			{
				total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"管路堵塞",1);
				mFlushBufferToDisk( 0 );
			}
			else if(sample_file_chaxun.success == DF_FAN_ERR)
			{
				total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"流量达不到",1);
				mFlushBufferToDisk( 0 );
			}
			else
			{
				
			}
		}
		else
		{
			
		}
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"",1);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"",0);
		mFlushBufferToDisk( 0 );
		usb_out_start_file++;
		if(usb_out_start_file > usb_out_end_file)
		{
			mFlushBufferToDisk( 1 );
			f_close(file);
			usb_out_file_step = DF_FILE_CLOSED;	
		}
		return;
	}

}



void SampleFileOut_CSV(u16 start_file,u16 end_file)
{
	u8 hour_tmp;
	u8 min_tmp;
	u8 sec_tmp;
    u8 strbuf[20];
    u8 bianhao_buf[10];
	if(usb_out_file_step == DF_FILE_HEAD_LILE)
	{
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"ZR-3500型大气采样器 ",0);
		mFlushBufferToDisk( 0 );  // 自动刷新缓冲区,检查缓冲区是否已满,满则写盘
#ifdef ZR3500TWO
        strcpy(strbuf,(char*)ZR3500TWO_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"仪器编号,",0);
		mFlushBufferToDisk( 0 );    
#endif
#ifdef ZR3500TWO_HEAT
        strcpy(strbuf,(char*)ZR3500TWO_HEAT_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"仪器编号,",0);
		mFlushBufferToDisk( 0 );
        
#endif
#ifdef ZR3500TWO_HEATandCOOL
        strcpy(strbuf,(char*)ZR3500TWO_HEATandCOOL_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"仪器编号,",0);
		mFlushBufferToDisk( 0 );
#endif
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ,文件号,采样日期,采样时长,",0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"环境温度(℃),大气压(kPa),设定流量(L/min),",0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"计前压力(kPa),计前温度(℃),累积体积(L),",0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"标况体积(L),",0);
		mFlushBufferToDisk( 0 );
		usb_out_file_step = DF_FILE_CONTENT;
		return;
	}
	if(usb_out_file_step == DF_FILE_CONTENT)
	{
		ReadSampleFile_AB(usb_out_start_file);
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"",1);
		mFlushBufferToDisk( 0 );
#ifdef ZR3500TWO
		total += my_sprintf(FILE_DATA_BUF+total,"No.",0);
		mFlushBufferToDisk( 0 );     
        SPutl(8,bianhao_buf,bianhao_new);
        strcpy(strbuf,(char*)ZR3500TWO_NAME);
        strcat(strbuf,bianhao_buf);
		total += my_sprintf(FILE_DATA_BUF+total,strbuf,0);
		mFlushBufferToDisk( 0 );       
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );
#endif
#ifdef ZR3500TWO_HEAT
        total += my_sprintf(FILE_DATA_BUF+total,"No.",0);
		mFlushBufferToDisk( 0 );     
		SPutl(8,bianhao_buf,bianhao_new);
        strcpy(strbuf,(char*)ZR3500TWO_HEAT_NAME);
        strcat(strbuf,bianhao_buf);
		total += my_sprintf(FILE_DATA_BUF+total,strbuf,0);
		mFlushBufferToDisk( 0 );       
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 ); 
        
#endif
#ifdef ZR3500TWO_HEATandCOOL
        total += my_sprintf(FILE_DATA_BUF+total,"No.",0);
		mFlushBufferToDisk( 0 );     
		SPutl(8,bianhao_buf,bianhao_new);
        strcpy(strbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
        strcat(strbuf,bianhao_buf);
		total += my_sprintf(FILE_DATA_BUF+total,strbuf,0);
		mFlushBufferToDisk( 0 );       
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );  
#endif
		if(sample_file_chaxun.channel == DF_CHANNEL_A)
		{
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"A路采样",0);
		}
		else
		{
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"B路采样",0);		
		}
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		SPuti(4,testbuf,usb_out_start_file);		//文件号
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"20",0);		  			   		    //文本方式(CSV格式要求)
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.year);						//年
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.month);						//月
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.day);						//日
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.hour);						//时
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.min);						//分
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.sec);						//秒
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		hour_tmp = sample_file_chaxun.running_time / 3600;
		min_tmp = (sample_file_chaxun.running_time % 3600) / 60;
		sec_tmp = sample_file_chaxun.running_time % 60;
		CharToStr(2,testbuf,hour_tmp);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,min_tmp);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sec_tmp);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		SPutf(sf31,testbuf,sample_file_chaxun.environment_temperature);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		SPutf(f52,testbuf,sample_file_chaxun.barometric);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		SPutf(f21,testbuf,sample_file_chaxun.flow_set);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		SPutf(sf42,testbuf,sample_file_chaxun.jy);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		SPutf(sf31,testbuf,sample_file_chaxun.temperature);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		SPutf(f62,(u8 *)testbuf,sample_file_chaxun.volume);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		SPutf(f62,(u8 *)testbuf,sample_file_chaxun.volume_biaokuang);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );
		
		if(err_dis == 1)
		{
			if(sample_file_chaxun.success == DF_JY_OUT)
			{
				total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"管路堵塞",0);
				mFlushBufferToDisk( 0 );
			}
			else if(sample_file_chaxun.success == DF_FAN_ERR)
			{
				total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"流量达不到",0);
				mFlushBufferToDisk( 0 );
			}
			else
			{
				total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"",0);
				mFlushBufferToDisk( 0 );
			}
		}
		else
		{
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"",0);
			mFlushBufferToDisk( 0 );
		}	
		usb_out_start_file++;
		if(usb_out_start_file > usb_out_end_file)
		{
			mFlushBufferToDisk( 1 );
			f_close(file);
			usb_out_file_step = DF_FILE_CLOSED;
		}
		return;
	}
}

void PowerFileOut(u16 start_file,u16 end_file)
{
    u8 strbuf[20];
    u8 bianhao_buf[10];
	if(usb_out_file_step == DF_FILE_HEAD_LILE)
	{
		usb_out_file_step = DF_FILE_CONTENT;
		return;
	}	
	if(usb_out_file_step == DF_FILE_CONTENT)
	{
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"",1);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"",1);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"ZR-3500型大气采样器 ",0);
		mFlushBufferToDisk( 0 );
#ifdef ZR3500TWO
        strcpy(strbuf,(char*)ZR3500TWO_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"仪器编号  ",0);
		mFlushBufferToDisk( 0 );
		SPutl(8,bianhao_buf,bianhao_new);
        strcpy(strbuf,(char*)ZR3500TWO_NAME);
        strcat(strbuf,bianhao_buf);
		total += my_sprintf(FILE_DATA_BUF+total,strbuf,1);
		mFlushBufferToDisk( 0 );       
#endif
#ifdef ZR3500TWO_HEAT
        strcpy(strbuf,(char*)ZR3500TWO_HEAT_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"仪器编号  ",0);
		mFlushBufferToDisk( 0 );
		SPutl(8,bianhao_buf,bianhao_new);
        strcpy(strbuf,(char*)ZR3500TWO_HEAT_NAME);
        strcat(strbuf,bianhao_buf);
		total += my_sprintf(FILE_DATA_BUF+total,strbuf,1);
		mFlushBufferToDisk( 0 );   
        
#endif
#ifdef ZR3500TWO_HEATandCOOL
        strcpy(strbuf,(char*)ZR3500TWO_HEATandCOOL_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"仪器编号  ",0);
		mFlushBufferToDisk( 0 );
		SPutl(8,bianhao_buf,bianhao_new);
        strcpy(strbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
        strcat(strbuf,bianhao_buf);
		total += my_sprintf(FILE_DATA_BUF+total,strbuf,1);
		mFlushBufferToDisk( 0 );   
#endif
		ReadPowerFile_AB(usb_out_start_file);

		if(power_file_chaxun.channel == DF_CHANNEL_A)
		{
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"A 路掉电",1);
		}
		else
		{
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"B 路掉电",1);		
		}
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"记 录 号  ",0);
		mFlushBufferToDisk( 0 );
		SPuti(4,testbuf,usb_out_start_file);		//文件号
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"掉电时间  ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.year);						//年
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.month);						//月
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.day);						//日
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.hour);						//时
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.min);						//分
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.sec);						//秒
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,1);
		mFlushBufferToDisk( 0 );
		
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"来电时间  ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_year);						//年
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_month);						//月
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_day);						//日
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_hour);						//时
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_min);						//分
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_sec);						//秒
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,1);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"累计掉电  ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.lose_hour);						//时
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.lose_min);						//分
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.lose_sec);						//秒
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,1);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"",1);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"",0);
		mFlushBufferToDisk( 0 );
		usb_out_start_file++;
		if(usb_out_start_file > usb_out_end_file)
		{
			mFlushBufferToDisk( 1 );
			f_close(file);
			usb_out_file_step = DF_FILE_CLOSED;
		}
		return;
	}
}


void PowerFileOut_CSV(u16 start_file,u16 end_file)
{
    u8 strbuf[20];
    u8 bianhao_buf[10];
	if(usb_out_file_step == DF_FILE_HEAD_LILE)
	{
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"ZR-3500型大气采样器 ",0);
		mFlushBufferToDisk( 0 );
#ifdef ZR3500TWO
        strcpy(strbuf,(char*)ZR3500TWO_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"仪器编号,",0);
		mFlushBufferToDisk( 0 );    
#endif
#ifdef ZR3500TWO_HEAT
        strcpy(strbuf,(char*)ZR3500TWO_HEAT_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"仪器编号,",0);
		mFlushBufferToDisk( 0 );
        
#endif
#ifdef ZR3500TWO_HEATandCOOL
        strcpy(strbuf,(char*)ZR3500TWO_HEATandCOOL_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"仪器编号,",0);
		mFlushBufferToDisk( 0 );
#endif
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",文件号,掉电时间,来电时间,累计掉电",0);
		mFlushBufferToDisk( 0 );
		usb_out_file_step = DF_FILE_CONTENT;
		return;
	}
	if(usb_out_file_step == DF_FILE_CONTENT)
	{
		ReadPowerFile_AB(usb_out_start_file);
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"",1);
		mFlushBufferToDisk( 0 );
#ifdef ZR3500TWO
        total += my_sprintf(FILE_DATA_BUF+total,"No.",0);
		mFlushBufferToDisk( 0 );     
		SPutl(8,bianhao_buf,bianhao_new);
        strcpy(strbuf,(char*)ZR3500TWO_NAME);
        strcat(strbuf,bianhao_buf);
		total += my_sprintf(FILE_DATA_BUF+total,strbuf,0);
		mFlushBufferToDisk( 0 );       
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );
#endif
#ifdef ZR3500TWO_HEAT
        total += my_sprintf(FILE_DATA_BUF+total,"No.",0);
		mFlushBufferToDisk( 0 );     
		SPutl(8,bianhao_buf,bianhao_new);
        strcpy(strbuf,(char*)ZR3500TWO_HEAT_NAME);
        strcat(strbuf,bianhao_buf);
		total += my_sprintf(FILE_DATA_BUF+total,strbuf,0);
		mFlushBufferToDisk( 0 );       
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 ); 
        
#endif
#ifdef ZR3500TWO_HEATandCOOL
		total += my_sprintf(FILE_DATA_BUF+total,"No.",0);
		mFlushBufferToDisk( 0 );     
        SPutl(8,bianhao_buf,bianhao_new);
        strcpy(strbuf,(char*)ZR3500TWO_HEATandCOOL_NAME);
        strcat(strbuf,bianhao_buf);
		total += my_sprintf(FILE_DATA_BUF+total,strbuf,0);
		mFlushBufferToDisk( 0 );       
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );  
#endif
		if(power_file_chaxun.channel == DF_CHANNEL_A)
		{
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"A路掉电",0);
		}
		else
		{
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"B路掉电",0);		
		}
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );
		
		SPuti(4,testbuf,usb_out_start_file);		//文件号
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"20",0);		  			   		//(CSV格式要求)
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.year);					//年
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.month);					//月
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.day);						//日
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.hour);					//时
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.min);						//分
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.sec);						//秒
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		CharToStr(2,testbuf,power_file_chaxun.power_year);						//年
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_month);						//月
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_day);						//日
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_hour);						//时
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_min);						//分
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_sec);						//秒
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		CharToStr(2,testbuf,power_file_chaxun.lose_hour);						//时
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.lose_min);						//分
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.lose_sec);						//秒
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		usb_out_start_file++;
		if(usb_out_start_file > usb_out_end_file)
		{
			mFlushBufferToDisk( 1 );
			f_close(file);
			usb_out_file_step = DF_FILE_CLOSED;
		}
		return;
	}
}








