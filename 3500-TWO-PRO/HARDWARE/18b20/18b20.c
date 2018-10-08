
//------------include------------
#include "sys.h"
#include "delay.h"
#include "18b20.h"
//---------λ����-----------------
#define DQIn();     DS18b20_InputMode();delay_us(2);
#define DQOut();    DS18b20_OutputMode();
#define DQ0In		PEin(8)							    //A·����
#define DQ1In		PCin(0)								//B·����
#define DQ2In		PEin(7)								//�¿����¶�
#define DQ3In		PCin(1)                             //�����¶�

u8 wk_temperature_err = 0;

void DS18b20_OutputMode(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1);
}

void DS18b20_InputMode(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1);
}
/*********************************************************************
 * @fn      DS18b20_reset
 *
 * @brief
 *
 *   ��λ����18b20
 *
 * @param   ��λЧ���洢��ַ
 *
 * @return  ��
 */
void DS18b20_reset(u8 * pre)		//4·�¶�ͬʱת��
{
	DQOut();
    GPIO_ResetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8);
	GPIO_ResetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1);
	delay_us(600);         	 		//600us		
    GPIO_SetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8);
	GPIO_SetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1);
	delay_us(60);           		//60us
	DQIn();
	*(pre+0)=DQ0In;
	*(pre+1)=DQ1In;
	*(pre+2)=DQ2In;
	*(pre+3)=DQ3In;
	DQOut();
	delay_us(500);					//500us
}


/*********************************************************************
 * @fn      DS18b20_write
 *
 * @brief
 *
 *   д����
 *
 * @param   ��д������
 *
 * @return  ��
 */
void DS18b20_write(u8 d)
{
	u8 i;
	DQOut();
	for(i=8;i>0;i--)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8);
		GPIO_ResetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1);
		delay_us(2);				//>1us
		if(d&0x01 != 0)
		{
			GPIO_SetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8);
			GPIO_SetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1);
		}
		else
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8);
			GPIO_ResetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1);
		}
		delay_us(60);				//>60us
		GPIO_SetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8);
		GPIO_SetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1);
		d>>=1;
		delay_us(6);
	}
	delay_us(60);						//60us 	???????
}

/*********************************************************************
 * @fn      DS18b20_read
 *
 * @brief
 *
 *   ��18b20������
 *
 * @param   ���ݴ洢��ַ
 *
 * @return  ��
 */
void DS18b20_read(u8 * darray)
{
	u8 i;
	DQOut();
	for(i=8;i>0;i--)
	{
		*(darray+0)>>=1;
		*(darray+1)>>=1;
		*(darray+2)>>=1;
		*(darray+3)>>=1;
		GPIO_ResetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8);
		GPIO_ResetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1);
		delay_us(2);
		GPIO_SetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8);
		GPIO_SetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1);
		DQIn();
		delay_us(10);				//10us
		if(DQ0In != 0)(*(darray+0))|=0x80;
		if(DQ1In != 0)(*(darray+1))|=0x80;
		if(DQ2In != 0)(*(darray+2))|=0x80;
		if(DQ3In != 0)(*(darray+3))|=0x80;
		DQOut();
		delay_us(60);				//>45us	(60)
	}
}


void DS18b20_init(void)
{
	u8 darray2[4];
	DS18b20_reset(darray2);				//????darray2???
	DS18b20_write(0xcc);				//??ROM
	DS18b20_write(0x4e);
	DS18b20_write(0x0);
	DS18b20_write(0x0);
	DS18b20_write(0x7f);
	DS18b20_reset(darray2);	
}
/*********************************************************************
 * @fn      Temperature_read
 *
 * @brief
 *
 *   ת��Ϊ�¶�ֵ
 *
 * @param   ���ݴ洢��ַ
 *
 * @return  ��
 */
void Temperature_read(float * tarray)	//���ж�ȡ7·�¶ȣ��Խ�ʡʱ�䣬�¶ȴ�����tarrayΪ��ʼ��ַ��7����������
{
	union WenDu
		{
			int_least16_t temp;
			u8 a[2];
		}wd;
	u8 i;
	u8 darray2[8];
	u8 darray1[8];
	u8 darray0[8];
	wk_temperature_err = 0;
	
//	DQIn();
//	delay_us(10);						//10us
//	if(DQ2In == 0)
//	{
//		wk_temperature_err = 1;
//	}
	
	DS18b20_reset(darray2);				//��ʱ����darray2������
	DS18b20_write(0xcc);				//����ROM
	DS18b20_write(0xbe);				//���ɲ�доƬ
	DS18b20_read(darray1);
	DS18b20_read(darray0);
		
	for(i = 0; i < 4 ;i++)
	{
        wd.a[0] = darray1[i];
        wd.a[1] = darray0[i];
        tarray[i] = wd.temp * 0.0625;
	}	
	
	DS18b20_reset(darray1);				//��ʱ����darray1������
	DS18b20_write(0xcc);				//����ROM
	DS18b20_write(0x44);	
	
    for(i = 0; i < 4 ;i++)
	{
        if(darray1[i] == 1)
        {
            tarray[i] = 20;
        }
	}
	if(darray1[2] == 1)
	{
		wk_temperature_err = 1;
	}
}





