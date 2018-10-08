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

u8 CHECKFILE = 0;				/*У�鲻����1ʱд�룬У��Ϊ1ʱ��ȡ���ж�д���Ƿ���ȷ*/
u8 LoadFileBuffer(u8 * pFile,u8 flag);
u32 DataSector = 0;										//���ݵ�ǰ����
u8  cancelflag = 0;										/*ȡ�������ļ�����ֹδ����U��һֱ�ȴ�����*/
u8 usb_out_file_step;
u32 NewSize;
u32 SectorNumber;
u16 total;
void write_test(u8 * p);
//----------------------------------------------

//------������ռ���ַ������Ƶ��ڲ�RAM��,�����ַ�������
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


//------���ַ������Ƶ��ڲ�RAM��,�����ַ�������
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
// force = 0 ���Զ�ˢ��(��黺�����е����ݳ���,����д��,��������ʱ���ڻ�������), force != 0 ��ǿ��ˢ��(���ܻ������е������ж��ٶ�д��,ͨ����ϵͳ�ػ�ǰӦ��ǿ��д��)
{
	if(force != 0) 
	{  // ǿ��ˢ��
		f_write(file,FILE_DATA_BUF,total,&bw);
		total = 0;
		f_sync(file);
	}
	else if(total >= DEF_SECTOR_SIZE) 
	{  // �������е����ݿ�Ҫ����,����Ӧ���Ƚ��������е�ԭ������д��U��
		f_write(file,FILE_DATA_BUF,total,&bw);
		total = 0;
		f_sync(file);
	}
}

//��pFileת�ص��ļ�������FILE_DATA_BUF��ֱ��512�ֽ�
//װ������1�����򷵻�0
	u16 Load_datacount=0;			//��װ����������
	u16 pFile_datacount=0;			//��װ�����
u8 FILE_DATA_BUF[712];		//500+200 (200Ϊһ��������ݸ���)

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
        *(array + i) = '\xd';	   //�س�
        *(array + i + 1) = '\xa';	   //����
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
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"ZR-3500�ʹ��������� ",0);
		mFlushBufferToDisk( 0 );
#ifdef ZR3500TWO
        strcpy(strbuf,(char*)ZR3500TWO_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�������  ",0);
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
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�������  ",0);
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
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�������  ",0);
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
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"A ·����",1);
		}
		else
		{
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"B ·����",1);		
		}
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�� �� ��  ",0);
		mFlushBufferToDisk( 0 );
		SPuti(4,testbuf,usb_out_start_file);		//�ļ���
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"��������  20",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.year);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.month);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.day);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.hour);						//ʱ
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.min);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.sec);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"����ʱ��  ",0);
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

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�����¶�  ",0);
		mFlushBufferToDisk( 0 );
		SPutf(sf31,testbuf,sample_file_chaxun.environment_temperature);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"��",1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�� �� ѹ  ",0);
		mFlushBufferToDisk( 0 );
		SPutf(f52,testbuf,sample_file_chaxun.barometric);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"kPa",1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�趨����  ",0);
		mFlushBufferToDisk( 0 );
		SPutf(f21,testbuf,sample_file_chaxun.flow_set);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"L/min",1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"��ǰѹ��  ",0);
		mFlushBufferToDisk( 0 );
		SPutf(sf42,testbuf,sample_file_chaxun.jy);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"kPa",1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"��ǰ�¶�  ",0);
		mFlushBufferToDisk( 0 );
		SPutf(sf31,testbuf,sample_file_chaxun.temperature);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"��",1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�ۻ����  ",0);
		mFlushBufferToDisk( 0 );
		SPutf(f62,(u8 *)testbuf,sample_file_chaxun.volume);
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"L",1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"������  ",0);
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
				total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"��·����",1);
				mFlushBufferToDisk( 0 );
			}
			else if(sample_file_chaxun.success == DF_FAN_ERR)
			{
				total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�����ﲻ��",1);
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
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"ZR-3500�ʹ��������� ",0);
		mFlushBufferToDisk( 0 );  // �Զ�ˢ�»�����,��黺�����Ƿ�����,����д��
#ifdef ZR3500TWO
        strcpy(strbuf,(char*)ZR3500TWO_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�������,",0);
		mFlushBufferToDisk( 0 );    
#endif
#ifdef ZR3500TWO_HEAT
        strcpy(strbuf,(char*)ZR3500TWO_HEAT_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�������,",0);
		mFlushBufferToDisk( 0 );
        
#endif
#ifdef ZR3500TWO_HEATandCOOL
        strcpy(strbuf,(char*)ZR3500TWO_HEATandCOOL_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�������,",0);
		mFlushBufferToDisk( 0 );
#endif
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ,�ļ���,��������,����ʱ��,",0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�����¶�(��),����ѹ(kPa),�趨����(L/min),",0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"��ǰѹ��(kPa),��ǰ�¶�(��),�ۻ����(L),",0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"������(L),",0);
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
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"A·����",0);
		}
		else
		{
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"B·����",0);		
		}
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		SPuti(4,testbuf,usb_out_start_file);		//�ļ���
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"20",0);		  			   		    //�ı���ʽ(CSV��ʽҪ��)
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.year);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.month);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.day);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.hour);						//ʱ
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.min);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,sample_file_chaxun.sec);						//��
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
				total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"��·����",0);
				mFlushBufferToDisk( 0 );
			}
			else if(sample_file_chaxun.success == DF_FAN_ERR)
			{
				total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�����ﲻ��",0);
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
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"ZR-3500�ʹ��������� ",0);
		mFlushBufferToDisk( 0 );
#ifdef ZR3500TWO
        strcpy(strbuf,(char*)ZR3500TWO_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�������  ",0);
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
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�������  ",0);
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
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�������  ",0);
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
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"A ·����",1);
		}
		else
		{
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"B ·����",1);		
		}
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�� ¼ ��  ",0);
		mFlushBufferToDisk( 0 );
		SPuti(4,testbuf,usb_out_start_file);		//�ļ���
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,1);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"����ʱ��  ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.year);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.month);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.day);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.hour);						//ʱ
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.min);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.sec);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,1);
		mFlushBufferToDisk( 0 );
		
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"����ʱ��  ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_year);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_month);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_day);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_hour);						//ʱ
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_min);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_sec);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,1);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�ۼƵ���  ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.lose_hour);						//ʱ
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.lose_min);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.lose_sec);						//��
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
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"ZR-3500�ʹ��������� ",0);
		mFlushBufferToDisk( 0 );
#ifdef ZR3500TWO
        strcpy(strbuf,(char*)ZR3500TWO_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�������,",0);
		mFlushBufferToDisk( 0 );    
#endif
#ifdef ZR3500TWO_HEAT
        strcpy(strbuf,(char*)ZR3500TWO_HEAT_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�������,",0);
		mFlushBufferToDisk( 0 );
        
#endif
#ifdef ZR3500TWO_HEATandCOOL
        strcpy(strbuf,(char*)ZR3500TWO_HEATandCOOL_VER);
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)strbuf,1);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"�������,",0);
		mFlushBufferToDisk( 0 );
#endif
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",�ļ���,����ʱ��,����ʱ��,�ۼƵ���",0);
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
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"A·����",0);
		}
		else
		{
			total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"B·����",0);		
		}
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );
		
		SPuti(4,testbuf,usb_out_start_file);		//�ļ���
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"20",0);		  			   		//(CSV��ʽҪ��)
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.year);					//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.month);					//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.day);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.hour);					//ʱ
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.min);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.sec);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		CharToStr(2,testbuf,power_file_chaxun.power_year);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_month);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)"-",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_day);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)" ",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_hour);						//ʱ
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_min);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.power_sec);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)",",0);
		mFlushBufferToDisk( 0 );

		CharToStr(2,testbuf,power_file_chaxun.lose_hour);						//ʱ
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
		total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.lose_min);						//��
		total += my_sprintf(FILE_DATA_BUF+total,testbuf,0);
		mFlushBufferToDisk( 0 );
        total += my_sprintf(FILE_DATA_BUF+total,(u8 *)":",0);
		mFlushBufferToDisk( 0 );
		CharToStr(2,testbuf,power_file_chaxun.lose_sec);						//��
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








