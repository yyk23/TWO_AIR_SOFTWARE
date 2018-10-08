
#include "parameter.h"
#include "task.h"
#include "delay.h"
#include "rtc.h"
#include "sample.h"
#include "file.h"
#include "24c64.h"
#include "print.h"
#include "DMT.h"
const u8 sf30[]={'+','3','0'};
const u8 sf31[]={'+','3','1'};
const u8 sf40[]={'+','4','0'};
const u8 sf41[]={'+','4','1'};
const u8 sf42[]={'+','4','2'};
const u8 sf51[]={'+','5','1'};

const u8 f20[]={'2','0'};
const u8 f21[]={'2','1'};
const u8 f30[]={'3','0'};
const u8 f31[]={'3','1'};
const u8 f32[]={'3','2'};
const u8 f40[]={'4','0'};
const u8 f41[]={'4','1'};
const u8 f42[]={'4','2'};
const u8 f43[]={'4','3'};
const u8 f50[]={'5','0'};
const u8 f51[]={'5','1'};
const u8 f52[]={'5','2'};
const u8 f54[]={'5','4'};
const u8 f60[]={'6','0'};
const u8 f62[]={'6','2'};
//const u8 f71[]={'7','1'};
const u8 f81[]={'8','1'};

struct POWER_TIME A_power_lose,A_sample_start;
struct POWER_TIME B_power_lose,B_sample_start;

u32 A_LostPowerTime;					//A·����ʱ��
u32 B_LostPowerTime;					//B·����ʱ��
u32 A_avg_count = 0;
u32 B_avg_count = 0;
u8 bianhao[9];							//�������

u16 print_file_step;

u32 bianhao_new;

struct FILE_INDEX AB_sample_file,AB_power_lose_file;
struct SAPMLE_FILE A_sample_file,B_sample_file,sample_file_chaxun;
struct POWER_FILE A_power_file,B_power_file,power_file_chaxun;

struct LCD_LIGHT lcd_light_ctr; 
u16 password = 0;

u8 err_dis;
u8 RTC_select = DF_RTC_MAIN_BOARD;

u16 printer_password;
float A_max_jy;
float B_max_jy;
u16 A_PWM_01L;
u16 B_PWM_01L;
u16 A_PWM_10L;
u16 B_PWM_10L;
u8 A_sample_protect_time;
u8 B_sample_protect_time;
float wk_temperature;
u16 wk_temperature_set;
struct SAMPLE_CHANNEL sample_select_channel;

u8 file_format = 1;	//0:txt��ʽ��1:csv��ʽ

u8 bak_flag = 0;  //�Ƿ񱸷ݱ�־
u8 FMc64_Flag = 0;//����24c64�Ƿ����



void power_lose_test(void)
{
	A_power_lose_resume();
	B_power_lose_resume();
	delay_ms(1000);
    if(RTC_select == DF_RTC_MAIN_BOARD)
    {
        my_RTC_GetTime();
    }
    else
    {
        DMT_GetRTC();
        sys_year = DMT_year;
        sys_month = DMT_month;
        sys_day = DMT_day;
        sys_hour = DMT_hour;
        sys_min = DMT_min;
        sys_sec = DMT_sec;
    }
	if(A_power_lose.status != DF_SAMPLE_TASK_IDLE)		//���ڵ������
	{
		A_LostPowerTime = A_GetLostTime();						//��ȡ����ʱ��
		if((A_LostPowerTime != 0) && (A_LostPowerTime < 360000))//��������ļ�
		{
			SavePowerFile_A();
		}
		A_power_test();
	}
	if(B_power_lose.status != DF_SAMPLE_TASK_IDLE)		//���ڵ������
	{
		B_LostPowerTime = B_GetLostTime();						//��ȡ����ʱ��
		if((B_LostPowerTime != 0) && (B_LostPowerTime < 360000))//��������ļ�
		{
			SavePowerFile_B();
		}
		B_power_test();
	}
	power_on_to_sample();	
}

/*-------------------------------------------------
�������ܣ������⺯��
�ж�ϵͳ������һ���׶ε���
---------------------------------------------------*/
u8 A_power_low_sample_status;			
u8 B_power_low_sample_status;

void A_power_test(void)
{
	unsigned long int LosTime = 0; 			//����ʱ�䣬24СʱΪ�ڵ�,��ȷ����
	A_power_low_sample_status = DF_SAMPLE_STATUS_INVALID;
	switch(A_power_lose.status)
	{
		case DF_SAMPLE_TASK_WAIT: 			//�ȴ�״̬����
	
			LosTime = A_LostPowerTime;					//��ȡ����ʱ��,��ȷ����
			if(LosTime <= 86400)						//����ʱ����24Сʱ֮��
			{//С��24Сʱ
				if((A_SAMPLE.start_hour_set * 60 + A_SAMPLE.start_min_set) >= (A_power_lose.hour * 60 + A_power_lose.min)) //�趨����ʱ���ڵ���
				{  
					if(A_power_lose.day == sys_day)			//�����ٴ���������ͬ
					{
						if((sys_hour * 60 + sys_min) < (A_SAMPLE.start_hour_set * 60 + A_SAMPLE.start_min_set))	//����ʱ��δ���������ȴ�����
						{ 
							A_power_low_sample_status = DF_SAMPLE_STATUS_WAIT_RUN;
							SAMPLE_TASK_A.step = DF_SAMPLE_TASK_WAIT;
						}
						else if(( sys_hour * 60 + sys_min) >= (A_SAMPLE.start_hour_set * 60 + A_SAMPLE.start_min_set))//����ʱ������������
						{
							A_power_low_sample_status = DF_SAMPLE_STATUS_TO_RUN;
						}//��������
					}//����ͬ
					else if(A_power_lose.day != sys_day)//�ڶ�����������,�Ѿ���������ʱ�䣬�����̲���
					{
						A_power_low_sample_status = DF_SAMPLE_STATUS_TO_RUN;
					}
				}
				/*-----------------------------------------------------------------------------*/
				else				//�趨��������ʱ���ڵڶ���
				{
					if(A_power_lose.day == sys_day)	//�����ٴ�����������ͬ��˵����δ������ʱ��
					{
						A_power_low_sample_status = DF_SAMPLE_STATUS_WAIT_RUN;
						SAMPLE_TASK_A.step = DF_SAMPLE_TASK_WAIT;
					}
					else if(A_power_lose.day != sys_day)	//�ٴ����紦����һ��
					{
						if((sys_hour * 60 + sys_min) < (A_SAMPLE.start_hour_set * 60 + A_SAMPLE.start_min_set))//����ʱ��δ���������ȴ�����
						{
							A_power_low_sample_status = DF_SAMPLE_STATUS_WAIT_RUN;
							SAMPLE_TASK_A.step = DF_SAMPLE_TASK_WAIT;
						}
						else if((sys_hour * 60 + sys_min) >= (A_SAMPLE.start_hour_set * 60 + A_SAMPLE.start_min_set))//���õȵ�����������
						{
							A_power_low_sample_status = DF_SAMPLE_STATUS_TO_RUN;
							
						}//��������
					}
				}//�趨�����ڵڶ���
			}
			else if((LosTime > 86400) && (LosTime <= 360000))//����ʱ����24Сʱ��100Сʱ֮�䣬���̲���
			{
				A_power_low_sample_status = DF_SAMPLE_STATUS_TO_RUN;
			}
			else												//����ʱ�����100Сʱ�����ٲ���
			{ //����100Сʱ
				A_power_low_sample_status = DF_SAMPLE_STATUS_INVALID;
			}
			break;//�ȴ�״̬����
		/*-------------------------------------------------------------------------------------*/
		case DF_SAMPLE_TASK_SAMPLING: //����״̬����
		case DF_SAMPLE_TASK_AUTOZERO:
		case DF_SAMPLE_TASK_ERR:
			LosTime = A_LostPowerTime;	//��ȡ����ʱ��,��ȷ����
			if(LosTime < 360000)		//����ʱ��С��100Сʱ,��������
			{
				A_power_low_sample_status = DF_SAMPLE_STATUS_CONTINUE_RUN;
				if(A_SAMPLE.sample_times >= A_SAMPLE.sample_times_set)	//����ָ����ִ��󣬲��ټ�������
				{
					A_power_low_sample_status = DF_SAMPLE_STATUS_INVALID;
					break;
				}
			}
			else//����100Сʱ�����ټ��������������Ѳ�����Ϣ
			{
				A_power_low_sample_status = DF_SAMPLE_STATUS_INVALID; 
				//�����ۼƲ�����ʱ�����������Ϣ
				SaveSampleFile_A();
			}
			break;
		case DF_SAMPLE_TASK_INTERVAL:	//����ȴ�״̬����
			LosTime = A_LostPowerTime;	//��ȡ����ʱ��,��ȷ����
			if(LosTime < 360000)		//100Сʱ֮��
			{
				//��ȡ������������
				if(LosTime >= A_SAMPLE.jiange_time)//����ʱ�����ʣ����ʱ��,�����̽��в���
				{
					A_power_low_sample_status = DF_SAMPLE_STATUS_TO_RUN;
				}//����ʱ�����ʣ����ʱ��
				else				//����ʱ��С��ʣ����ʱ��,�����ʣ������ռ��ʱ��
				{
					A_power_low_sample_status = DF_SAMPLE_STATUS_WAIT_RUN;
					A_SAMPLE.jiange_time = A_SAMPLE.jiange_time - LosTime;//��ȡʣ����,��λΪ��
					SAMPLE_TASK_A.step = DF_SAMPLE_TASK_INTERVAL;
				}
			}
			else			//����100Сʱ
			{
				A_power_low_sample_status = DF_SAMPLE_STATUS_INVALID;
			}
			break;
		default:			//�����г��ֶ�����Ϊδ���棬����������ʱ�Խ���������
			A_power_low_sample_status = DF_SAMPLE_STATUS_INVALID;
			break;
	}
}
void B_power_test(void)
{
	unsigned long int LosTime = 0; 			//����ʱ�䣬24СʱΪ�ڵ�,��ȷ����
	B_power_low_sample_status = DF_SAMPLE_STATUS_INVALID;
	switch(B_power_lose.status)
	{
		case DF_SAMPLE_TASK_WAIT: 			//�ȴ�״̬����
	
			LosTime = B_LostPowerTime;					//��ȡ����ʱ��,��ȷ����
			if(LosTime <= 86400)						//����ʱ����24Сʱ֮��
			{//С��24Сʱ
				if((B_SAMPLE.start_hour_set * 60 + B_SAMPLE.start_min_set) >= (B_power_lose.hour * 60 + B_power_lose.min)) //�趨����ʱ���ڵ���
				{  
					if(B_power_lose.day == sys_day)			//�����ٴ���������ͬ
					{
						if((sys_hour * 60 + sys_min) < (B_SAMPLE.start_hour_set * 60 + B_SAMPLE.start_min_set))	//����ʱ��δ���������ȴ�����
						{ 
							B_power_low_sample_status = DF_SAMPLE_STATUS_WAIT_RUN;
							SAMPLE_TASK_B.step = DF_SAMPLE_TASK_WAIT;
						}
						else if(( sys_hour * 60 + sys_min) >= (B_SAMPLE.start_hour_set * 60 + B_SAMPLE.start_min_set))//����ʱ������������
						{
							B_power_low_sample_status = DF_SAMPLE_STATUS_TO_RUN;
						}//��������
					}//����ͬ
					else if(B_power_lose.day != sys_day)//�ڶ�����������,�Ѿ���������ʱ�䣬�����̲���
					{
						B_power_low_sample_status = DF_SAMPLE_STATUS_TO_RUN;
					}
				}
				/*-----------------------------------------------------------------------------*/
				else				//�趨��������ʱ���ڵڶ���
				{
					if(B_power_lose.day == sys_day)	//�����ٴ�����������ͬ��˵����δ������ʱ��
					{
						B_power_low_sample_status = DF_SAMPLE_STATUS_WAIT_RUN;
						SAMPLE_TASK_B.step = DF_SAMPLE_TASK_WAIT;
					}
					else if(B_power_lose.day != sys_day)	//�ٴ����紦����һ��
					{
						if((sys_hour * 60 + sys_min) < (B_SAMPLE.start_hour_set * 60 + B_SAMPLE.start_min_set))//����ʱ��δ���������ȴ�����
						{
							B_power_low_sample_status = DF_SAMPLE_STATUS_WAIT_RUN;
							SAMPLE_TASK_B.step = DF_SAMPLE_TASK_WAIT;
						}
						else if((sys_hour * 60 + sys_min) >= (B_SAMPLE.start_hour_set * 60 + B_SAMPLE.start_min_set))//���õȵ�����������
						{
							B_power_low_sample_status = DF_SAMPLE_STATUS_TO_RUN;	
						}//��������
					}
				}//�趨�����ڵڶ���
			}
			else if((LosTime > 86400) && (LosTime <= 360000))//����ʱ����24Сʱ��100Сʱ֮�䣬���̲���
			{
				B_power_low_sample_status = DF_SAMPLE_STATUS_TO_RUN;
			}
			else												//����ʱ�����100Сʱ�����ٲ���
			{ //����100Сʱ
				B_power_low_sample_status = DF_SAMPLE_STATUS_INVALID;
			}
			break;//�ȴ�״̬����
		/*-------------------------------------------------------------------------------------*/
		case DF_SAMPLE_TASK_SAMPLING: 	//����״̬����
		case DF_SAMPLE_TASK_AUTOZERO:
		case DF_SAMPLE_TASK_ERR:
			LosTime = B_LostPowerTime;	//��ȡ����ʱ��,��ȷ����
			if(LosTime < 360000)		//����ʱ��С��100Сʱ,��������
			{
				B_power_low_sample_status = DF_SAMPLE_STATUS_CONTINUE_RUN;
				//�Զ�У��
				//��ʾˢ�±�־��1
				//����PWM
				//��ʼ�����ٿ��Ʊ�־��
				if(B_SAMPLE.sample_times >= B_SAMPLE.sample_times_set)	//����ָ����ִ��󣬲��ټ�������
				{
					B_power_low_sample_status = DF_SAMPLE_STATUS_INVALID;
					break;
				}
			}
			else//����100Сʱ�����ټ��������������Ѳ�����Ϣ
			{
				B_power_low_sample_status = DF_SAMPLE_STATUS_INVALID; 
				//�����ۼƲ�����ʱ�����������Ϣ
				SaveSampleFile_B();
			}
			break;
		case DF_SAMPLE_TASK_INTERVAL://����ȴ�״̬����
			LosTime = B_LostPowerTime;	//��ȡ����ʱ��,��ȷ����
			if(LosTime < 360000)		//100Сʱ֮��
			{
				//��ȡ������������
				if(LosTime >= B_SAMPLE.jiange_time)//����ʱ�����ʣ����ʱ��,�����̽��в���
				{
					B_power_low_sample_status = DF_SAMPLE_STATUS_TO_RUN;
				}//����ʱ�����ʣ����ʱ��
				else				//����ʱ��С��ʣ����ʱ��,�����ʣ������ռ��ʱ��
				{
					B_power_low_sample_status = DF_SAMPLE_STATUS_WAIT_RUN;
					B_SAMPLE.jiange_time = B_SAMPLE.jiange_time - LosTime;//��ȡʣ����,��λΪ��
					SAMPLE_TASK_B.step = DF_SAMPLE_TASK_INTERVAL;
				}
			}
			else			//����100Сʱ
			{
				B_power_low_sample_status = DF_SAMPLE_STATUS_INVALID;
			}
			break;
		default:
			B_power_low_sample_status = DF_SAMPLE_STATUS_INVALID;
			break;
	}
}

void power_on_to_sample(void)
{
	if((A_power_low_sample_status == DF_SAMPLE_STATUS_INVALID) && (B_power_low_sample_status == DF_SAMPLE_STATUS_INVALID))
	{
		set_refresh_wnd_num(DF_PIC_HOME);
	}
	else
	{
		set_refresh_wnd_num(DF_PIC_POWER_LOSE);
	}
}

/*--------------------------------------------------
�������ܣ��õ����ۼƵ���ʱ��,ѭ��ģʽ
---------------------------------------------------*/
u32 A_GetLostTime(void)
{
	u32 lbuf;
    lbuf = JiCai_Own(A_power_lose.year,A_power_lose.month,A_power_lose.day,A_power_lose.hour,A_power_lose.min,A_power_lose.sec);//��ȡ�ȴ�״̬����ʱ��
	return lbuf; 
}
/*--------------------------------------------------
�������ܣ��õ����ۼƵ���ʱ��,ѭ��ģʽ
---------------------------------------------------*/
u32 B_GetLostTime(void)
{
	u32 lbuf;
    lbuf = JiCai_Own(B_power_lose.year,B_power_lose.month,B_power_lose.day,B_power_lose.hour,B_power_lose.min,B_power_lose.sec);//��ȡ�ȴ�״̬����ʱ��
	return lbuf; 
}

/*-------------------------------------------------
��������:�������״̬����ʱ�������
--------------------------------------------------*/
u32  JiCai_Own(u8 year,u8 month,u8 day,u8 hour,u8 min,u8 sec)
{
 u8 i =0;
 u16 ibuf1 = 0;
 u16 ibuf2 = 0;

 u32 Lost = 0;

    if(sys_year == year)
    {
        if(sys_month > month) //�ٴ��ϵ�ʱ�����ͬ���·ݲ�ͬ
        {
            for(i = month;i < sys_month;i++)    //������ٴ��ϵ�ʱ�Ѿ�������������
            {
                if(i==1 || i==3 || i==5 || i==7 || i==8 || i==10 || i==12)
                {
                    Lost = Lost + 31;
                }//31 days
                else if(i == 2)
                {
                    if((year + 2000) % 4 == 0)
                    {
                        Lost = Lost + 29;//����
                    }
                    else
                    {
                        Lost = Lost + 28;
                    }
                }
                else
                {
                    Lost = Lost + 30;
                }//30 days
            }//for
        }//month>=Mon1
        Lost = Lost + day - sys_day;        //������ٴ��ϵ�ʱ�Ѿ���������

        Lost = (Lost * 24 + sys_hour - hour) * 60 + sys_min - min;

        Lost = Lost * 60 + sys_sec - sec; //ת��Ϊ��
		if(Lost == 0)
		{
			if(sys_sec < sec)
			{
				Lost = 0;
			}
		}
        return(Lost);

    }//year==year

    else if(sys_year>year)                  //�ٴ��ϵ�ʱ��ݲ�ͬ
    {
        for(i=year;i<sys_year;i++)
        {
            if((year+2000)%4==0)
            {
                Lost=Lost+366;//����
            }
            else
            {
                Lost=Lost+365;
            }
        }//for

        for(i=1;i<month;i++)            //���������ʱ�����Ѿ��߹�����������
        {
            if(i==1 || i==3 || i==5 || i==7 || i==8 || i==10 || i==12)
            {
                ibuf1 = ibuf1 + 31;
            }//31 days
            else if(i==2)
            {
                if((year+2000)%4==0)
                {
                    ibuf1=ibuf1+29;//����
                }
                else
                {
                    ibuf1=ibuf1+28;
                }
            }
            else
            {
                ibuf1=ibuf1+30;
            }//30 days
        }//for

        ibuf1 = (ibuf1 + day);    	//���������ʱ�����Ѿ��߹�����

        for(i = 1;i < sys_month;i++)    //������ϵ�ʱ�Ѿ��߹���������
        {
            if(i==1 || i==3 || i==5 || i==7 || i==8 || i==10 || i==12)
            {
                ibuf2=ibuf2+31;
            }//31 days
            else if(i==2)
            {
                if((sys_year+2000)%4==0)
                {
                    ibuf2=ibuf2+29;//����
                }
                else
                {
                    ibuf2=ibuf2+28;
                }
            }
            else
            {
                ibuf2 = ibuf2+30;
            }//30 days
        }//for
        ibuf2 = (ibuf2 + sys_day);			//������ϵ�ʱ�Ѿ��߹�����
        Lost = Lost + ibuf2 - ibuf1;    //������ٴ��ϵ�ʱ��ݲ�ͬʱ���ܵ�������

        Lost = (Lost * 24 + sys_hour - hour) * 60 + sys_min - min;//������ٴ��ϵ�ʱ��ݲ�ͬʱ���ܵ��������

        Lost = Lost * 60 + sys_sec - sec; 	//������ٴ��ϵ�ʱ��ݲ�ͬʱ���ܵ�������

        return(Lost);
    }//year>year
    return 0;
}


void A_power_lose_protect(u8 sample_status)
{
    if(RTC_select == DF_RTC_MAIN_BOARD)
    {
        my_RTC_GetTime();
    }
    else
    {
        DMT_GetRTC();
        sys_year = DMT_year;
        sys_month = DMT_month;
        sys_day = DMT_day;
        sys_hour = DMT_hour;
        sys_min = DMT_min;
        sys_sec = DMT_sec;
    }
	W_24(ADDR_A_power_lose_status,sample_status);
	W_24(ADDR_A_power_lose_year,sys_year);
	W_24(ADDR_A_power_lose_month,sys_month);
	W_24(ADDR_A_power_lose_day,sys_day);
	W_24(ADDR_A_power_lose_hour,sys_hour);
	W_24(ADDR_A_power_lose_min,sys_min);
	W_24(ADDR_A_power_lose_sec,sys_sec);
	if(sample_status == DF_SAMPLE_TASK_SAMPLING)
	{
		//����ƽ��ֵ������������ѹ����ѹ����ǰ�¶ȣ������¶�
		savelongdata(ADDR_A_avg_count,A_avg_count);
		savefloat(ADDR_A_sample_file_jy,A_sample_file.jy);
		savefloat(ADDR_A_sample_file_barometric,A_sample_file.barometric);
		savefloat(ADDR_A_sample_file_temperature,A_sample_file.temperature);
		savefloat(ADDR_A_sample_file_environment_temperature,A_sample_file.environment_temperature);
		//���������Ҫ�����������Ѳ����������Ѳ���ʱ�䣨����ʱ�����Ѳ���������Ѳ�������
		W_24(ADDR_A_SAMPLE_sample_times,A_SAMPLE.sample_times);
		savelongdata(ADDR_A_SAMPLE_running_time,A_SAMPLE.running_time);
		savelongdata(ADDR_A_SAMPLE_running_time_by_volume,A_SAMPLE.running_time_by_volume);
		savefloat(ADDR_A_SAMPLE_volume,A_SAMPLE.volume);
		savefloat(ADDR_A_SAMPLE_volume_biaokuang,A_SAMPLE.volume_biaokuang);
	}
	else if(sample_status == DF_SAMPLE_TASK_INTERVAL)
	{
		savelongdata(ADDR_A_SAMPLE_jiange_time,A_SAMPLE.jiange_time);
		W_24(ADDR_A_SAMPLE_sample_times,A_SAMPLE.sample_times);
	}
	else if(sample_status == DF_SAMPLE_TASK_AUTOZERO)
	{
		savelongdata(ADDR_A_SAMPLE_running_time,A_SAMPLE.running_time);
		savelongdata(ADDR_A_SAMPLE_running_time_by_volume,A_SAMPLE.running_time_by_volume);
	}
}

void A_power_lose_resume(void)
{
	//����ʱ״̬
	A_power_lose.status = R_24(ADDR_A_power_lose_status);
	//����ʱ��
	A_power_lose.year = R_24(ADDR_A_power_lose_year);
	A_power_lose.month = R_24(ADDR_A_power_lose_month);
	A_power_lose.day = R_24(ADDR_A_power_lose_day);
	A_power_lose.hour = R_24(ADDR_A_power_lose_hour);
	A_power_lose.min = R_24(ADDR_A_power_lose_min);
	A_power_lose.sec = R_24(ADDR_A_power_lose_sec);
	//����ʱ��ƽ��ֵ����
	A_avg_count = readlongdata(ADDR_A_avg_count);
	A_sample_file.jy = readfloat(ADDR_A_sample_file_jy);
	A_sample_file.barometric = readfloat(ADDR_A_sample_file_barometric);
	A_sample_file.temperature = readfloat(ADDR_A_sample_file_temperature);
	A_sample_file.environment_temperature = readfloat(ADDR_A_sample_file_environment_temperature);
	//����ʱ�Ĳ�������������ά�ּ��������Ĳ�����
	A_SAMPLE.sample_times = R_24(ADDR_A_SAMPLE_sample_times);
	A_SAMPLE.running_time = readlongdata(ADDR_A_SAMPLE_running_time);
	A_SAMPLE.running_time_by_volume = readlongdata(ADDR_A_SAMPLE_running_time_by_volume);
	A_SAMPLE.volume = readfloat(ADDR_A_SAMPLE_volume);
	A_SAMPLE.volume_biaokuang = readfloat(ADDR_A_SAMPLE_volume_biaokuang);

	A_SAMPLE.jiange_time = readlongdata(ADDR_A_SAMPLE_jiange_time);
}
void A_save_sample_start(void)
{
	A_sample_file.success = DF_SUCCESS;
	A_avg_count = 0;						//����ƽ��ֵ����
    if(RTC_select == DF_RTC_MAIN_BOARD)
    {
        my_RTC_GetTime();
    }
    else
    {
        DMT_GetRTC();
        sys_year = DMT_year;
        sys_month = DMT_month;
        sys_day = DMT_day;
        sys_hour = DMT_hour;
        sys_min = DMT_min;
        sys_sec = DMT_sec;
    }
	W_24(ADDR_A_sample_start_year,sys_year);
	W_24(ADDR_A_sample_start_month,sys_month);
	W_24(ADDR_A_sample_start_day,sys_day);
	W_24(ADDR_A_sample_start_hour,sys_hour);
	W_24(ADDR_A_sample_start_min,sys_min);
	W_24(ADDR_A_sample_start_sec,sys_sec);
}
void A_sample_over(void)
{
	W_24(ADDR_A_power_lose_status,DF_SAMPLE_TASK_IDLE);
}

void B_power_lose_protect(u8 sample_status)
{
	if(RTC_select == DF_RTC_MAIN_BOARD)
    {
        my_RTC_GetTime();
    }
    else
    {
        DMT_GetRTC();
        sys_year = DMT_year;
        sys_month = DMT_month;
        sys_day = DMT_day;
        sys_hour = DMT_hour;
        sys_min = DMT_min;
        sys_sec = DMT_sec;
    }
	W_24(ADDR_B_power_lose_status,sample_status);
	W_24(ADDR_B_power_lose_year,sys_year);
	W_24(ADDR_B_power_lose_month,sys_month);
	W_24(ADDR_B_power_lose_day,sys_day);
	W_24(ADDR_B_power_lose_hour,sys_hour);
	W_24(ADDR_B_power_lose_min,sys_min);
	W_24(ADDR_B_power_lose_sec,sys_sec);
	if(sample_status == DF_SAMPLE_TASK_SAMPLING)
	{
		//����ƽ��ֵ������������ѹ����ѹ����ǰ�¶ȣ������¶�
		savelongdata(ADDR_B_avg_count,B_avg_count);
		savefloat(ADDR_B_sample_file_jy,B_sample_file.jy);
		savefloat(ADDR_B_sample_file_barometric,B_sample_file.barometric);
		savefloat(ADDR_B_sample_file_temperature,B_sample_file.temperature);
		savefloat(ADDR_B_sample_file_environment_temperature,B_sample_file.environment_temperature);
		//���������Ҫ�����������Ѳ����������Ѳ���ʱ�䣨����ʱ�����Ѳ���������Ѳ�������
		W_24(ADDR_B_SAMPLE_sample_times,B_SAMPLE.sample_times);
		savelongdata(ADDR_B_SAMPLE_running_time,B_SAMPLE.running_time);
		savelongdata(ADDR_B_SAMPLE_running_time_by_volume,B_SAMPLE.running_time_by_volume);
		savefloat(ADDR_B_SAMPLE_volume,B_SAMPLE.volume);
		savefloat(ADDR_B_SAMPLE_volume_biaokuang,B_SAMPLE.volume_biaokuang);
	}
	else if(sample_status == DF_SAMPLE_TASK_INTERVAL)
	{
		savelongdata(ADDR_B_SAMPLE_jiange_time,B_SAMPLE.jiange_time);
		W_24(ADDR_B_SAMPLE_sample_times,B_SAMPLE.sample_times);
	}
	else if(sample_status == DF_SAMPLE_TASK_AUTOZERO)
	{
		savelongdata(ADDR_B_SAMPLE_running_time,B_SAMPLE.running_time);
		savelongdata(ADDR_B_SAMPLE_running_time_by_volume,B_SAMPLE.running_time_by_volume);
	}
}

void B_power_lose_resume(void)
{
	//����ʱ״̬
	B_power_lose.status = R_24(ADDR_B_power_lose_status);
	//����ʱ��
	B_power_lose.year = R_24(ADDR_B_power_lose_year);
	B_power_lose.month = R_24(ADDR_B_power_lose_month);
	B_power_lose.day = R_24(ADDR_B_power_lose_day);
	B_power_lose.hour = R_24(ADDR_B_power_lose_hour);
	B_power_lose.min = R_24(ADDR_B_power_lose_min);
	B_power_lose.sec = R_24(ADDR_B_power_lose_sec);
	//����ʱ��ƽ��ֵ����
	B_avg_count = readlongdata(ADDR_B_avg_count);
	B_sample_file.jy = readfloat(ADDR_B_sample_file_jy);
	B_sample_file.barometric = readfloat(ADDR_B_sample_file_barometric);
	B_sample_file.temperature = readfloat(ADDR_B_sample_file_temperature);
	B_sample_file.environment_temperature = readfloat(ADDR_B_sample_file_environment_temperature);
	//����ʱ�Ĳ�������������ά�ּ��������Ĳ�����
	B_SAMPLE.sample_times = R_24(ADDR_B_SAMPLE_sample_times);
	B_SAMPLE.running_time = readlongdata(ADDR_B_SAMPLE_running_time);
	B_SAMPLE.running_time_by_volume = readlongdata(ADDR_B_SAMPLE_running_time_by_volume);
	B_SAMPLE.volume = readfloat(ADDR_B_SAMPLE_volume);
	B_SAMPLE.volume_biaokuang = readfloat(ADDR_B_SAMPLE_volume_biaokuang);

	B_SAMPLE.jiange_time = readlongdata(ADDR_B_SAMPLE_jiange_time);
}
void B_save_sample_start(void)
{
	B_sample_file.success = DF_SUCCESS;
	B_avg_count = 0;						//����ƽ��ֵ����
    if(RTC_select == DF_RTC_MAIN_BOARD)
    {
        my_RTC_GetTime();
    }
    else
    {
        DMT_GetRTC();
        sys_year = DMT_year;
        sys_month = DMT_month;
        sys_day = DMT_day;
        sys_hour = DMT_hour;
        sys_min = DMT_min;
        sys_sec = DMT_sec;
    }
	W_24(ADDR_B_sample_start_year,sys_year);
	W_24(ADDR_B_sample_start_month,sys_month);
	W_24(ADDR_B_sample_start_day,sys_day);
	W_24(ADDR_B_sample_start_hour,sys_hour);
	W_24(ADDR_B_sample_start_min,sys_min);
	W_24(ADDR_B_sample_start_sec,sys_sec);
}
void B_sample_over(void)
{
//	B_sample_status = DF_SAMPLE_STATUS_IDLE;
	W_24(ADDR_B_power_lose_status,DF_SAMPLE_TASK_IDLE);
}

u8 A_sample_over_flag = 0; 
u8 B_sample_over_flag = 0;
u8 A_Qcount = 0;
u8 B_Qcount = 0;
u8 A_PWM_count = 0;
u8 B_PWM_count = 0;

u8 user_flag = 0;


//ʹ��0.3��0.6��1.0�����㱶��������������������
float flow_rate_creat_by_point_03_06_10(float * pstr,u8 flow_point)
{
    float fbuf;
    //(y-y2)/(x-x2) = (y2-y1)/(x2-x1)  ==>   
    //��x1=0.3,x2=0.6,��(y-y2)/(x-0.6) = (y2-y1)/(0.6-0.3)   ==>   
    //x=0.2,y=(4y1-y2)/3
    //x=0.4,y=(y2+2y1)/3   
    //x=0.5,y=(2y2+y1)/3
    //��x1=0.6,x2=1.0,��(y-y2)/(x-1.0) = (y2-y1)/(1.0-0.6)   ==>   
    //x=0.7,y=(y2+3y1)/4   
    //x=0.8,y=(y2+y1)/2
    //x=0.9,y=(3y2+y1)/4
    switch (flow_point)
    {
        case 1:
            fbuf = pstr[1];                                                         //0.1L/minʹ�����뱶��
            break;
        case 2:
            fbuf = (pstr[3]+pstr[3]+pstr[3]+pstr[3]-pstr[6])/(float)3.0*pstr[2];    //0.2L/min���Ե�������
            break;
        case 3:
            fbuf = pstr[3];                                                         //0.3L/minʹ�����뱶��
            break;
        case 4:
            fbuf = (pstr[6]+pstr[3]+pstr[3])/(float)3.0*pstr[4];                    //0.4L/min���Ե�������
            break;
        case 5:
            fbuf = (pstr[6]+pstr[6]+pstr[3])/(float)3.0*pstr[5];                    //0.5L/min���Ե�������
            break;
        case 6:
            fbuf = pstr[6];                                                         //0.6L/minʹ�����뱶��
            break;
        case 7:
            fbuf = (pstr[10]+pstr[6]+pstr[6]+pstr[6])/(float)4.0*pstr[7];           //0.7L/min���Ե�������
            break;
        case 8:
            fbuf = (pstr[10]+pstr[6])/(float)2.0*pstr[8];                           //0.8L/min���Ե�������
            break;
        case 9:
            fbuf = (pstr[10]+pstr[10]+pstr[10]+pstr[6])/(float)4.0*pstr[9];         //0.9L/min���Ե�������
            break;
        case 10:
            fbuf = pstr[10];                                                        //1.0L/minʹ�����뱶��
            break;
    }
    return fbuf;
}
float point_rate_A_flow(float fbuf)
{
	u8 A_flow_rate_count = 0;
	A_flow_rate_count = fbuf * (float)10.0 + (float)0.5;
    return(A_flow_rate.rate_array[A_flow_rate_count]);
}
float point_rate_B_flow(float fbuf)
{
	u8 B_flow_rate_count = 0;
	B_flow_rate_count = fbuf * (float)10.0 + (float)0.5;
    return(B_flow_rate.rate_array[B_flow_rate_count]);
}
void A_calc_flow_rate(float fbuf)
{
	u8 A_flow_rate_count = 0;
	A_flow_rate_count = fbuf * (float)10.0 + (float)0.5;
//    A_flow_rate.rate = A_flow_rate.rate_array[A_flow_rate_count];
    A_flow_rate.rate = flow_rate_creat_by_point_03_06_10(&A_flow_rate.rate_array[0],A_flow_rate_count);
}
void B_calc_flow_rate(float fbuf)
{
	u8 B_flow_rate_count = 0;
	B_flow_rate_count = fbuf * (float)10.0 + (float)0.5;
//    B_flow_rate.rate = B_flow_rate.rate_array[B_flow_rate_count];
    B_flow_rate.rate = flow_rate_creat_by_point_03_06_10(&B_flow_rate.rate_array[0],B_flow_rate_count);
}
void PrintSampleFile(u16 start_file,u16 end_file)
{
	if(print_file_step == DF_FILE_HEAD_LILE)
	{
		PrintSetUp(); //??????
		print_file_step = DF_FILE_CONTENT;
	}
	if(print_file_step == DF_FILE_CONTENT)
	{
        PrintStart();
        ReadSampleFile_AB(print_start_file);
        PrintData(print_start_file);
		print_start_file++;
		if(print_start_file > print_end_file)
		{
			print_file_step = DF_FILE_CLOSED;
		}
		return;
	}
}

void PrintPowerFile(u16 start_file,u16 end_file)
{
	if(print_file_step == DF_FILE_HEAD_LILE)
	{
		PrintSetUp(); //??????
		print_file_step = DF_FILE_CONTENT;
	}
	if(print_file_step == DF_FILE_CONTENT)
	{
        PrintStart();
        ReadPowerFile_AB(print_start_file);
        PrintPower(print_start_file);
		print_start_file++;
		if(print_start_file > print_end_file)
		{
			print_file_step = DF_FILE_CLOSED;
		}
		return;
	}
}











