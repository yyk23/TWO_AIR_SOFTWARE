 #include "adc.h"
 #include "delay.h"	   

float ADBuf[16];					//ad������
u32 adcount = 0;					//ad����
	   																   
void  ADC1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);	//ʹ��GPIOA\Cʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 	//ʹ��ADC1ʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;			//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ�� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PA5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;			//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ�� 
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PC2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;			//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//��ʼ��
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  	//ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;						//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 		//DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;						//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);										//��ʼ��

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;							//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;									//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;								//����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;		//��ֹ������⣬ʹ���������
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 3;										//7��ת���ڹ���������
	ADC_Init(ADC1, &ADC_InitStructure);												//ADC��ʼ��
}				

u16 ADC_Table[500];
#define ADC1_DR_Address    ((u32)0x4001204C)
/* ��������ADC1_Mode_Config
 * ����  ������ADC1�Ĺ���ģʽΪDMAģʽ
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */

void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	//����DMAʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_DeInit(DMA2_Stream0);
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_BASE + 0x4C;
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&ADC_Table[0];					//(u32)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;						//������Ϊ���ݴ������Դ
	DMA_InitStructure.DMA_BufferSize = 90;										//3*30��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//�����趨�����ַ�Ĵ����������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�����趨�ڴ��ַ�Ĵ����������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//�趨���������ݿ��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//�趨�˴洢�����ݿ��
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							//�趨DMAͨ��x��������ȼ�
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, DISABLE);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_112Cycles);   		//���������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2,ADC_SampleTime_112Cycles);			//��ص�ѹ
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4,  3,ADC_SampleTime_112Cycles);			//������
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);													//
	ADC_Cmd(ADC1, ENABLE);														//����ADת����	
}
//#define		DF_A_LIUYA			0
//#define 		DF_A_JIYA			1
//#define		DF_B_LIUYA			2
//#define		DF_B_JIYA			3
//#define		DF_QIYA				4
//#define 		DF_BAT_VOLTAGE		5
//#define 		DF_CHARGE_CURRENT	6      
//#define 	DF_HEAT_COOL_CURRENT	7
void DMA2_Stream0_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
	}
}

/***********************************************************************
* ��������    Average_AD_Value
* ����ԭ�ͣ�  U16 Average_AD_Value(U16 AD_Value_Tab[],U8 channel)
* ����������  ��channleͨ����ADת��ֵ��ƽ����
* ���������  AD_Value_Tab�����ADת����������顣�������������DMA����
			  channel������ת�������ͨ��
* ����ֵ��    ĳ��ͨ����ƽ��ֵ
* �����ú�������
***********************************************************************/
float Get_AD_Value(u16 * AD_Value_Tab,u8 channel)
{
	u16 i = 0;
	float sum = 0;

	for(i = 0; i < 30; i++)
	{
	    sum += *(AD_Value_Tab + (u16)channel + 3*i - 1);
	}
	return (float)sum/(float)30.0;
}

void Process_ADC(void)
{
	u8 j = 0;
	if(RESET != DMA_GetFlagStatus(DMA2_Stream0,DMA_FLAG_TCIF0))
	{
//		for(j = 0; j < 3; j++)
//		{
//			ADBuf[j]+=((Get_AD_Value(ADC_Table,j + 1)));
//		}
        ADBuf[DF_HEAT_COOL_CURRENT] += Get_AD_Value(ADC_Table,1);
        ADBuf[DF_BAT_VOLTAGE] += Get_AD_Value(ADC_Table,2);
        ADBuf[DF_CHARGE_CURRENT] += Get_AD_Value(ADC_Table,3);
		adcount++;
		DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
//		ADC_SoftwareStartConv(ADC1);
	}
}

void ADC1_Start(void)
{
	ADC1_Init();
	ADC1_Mode_Config();
	ADC_SoftwareStartConv(ADC1);
}

void ADC1_Stop(void)
{
	ADC_Cmd(ADC1, DISABLE);	//ָֹͣ����ADC1
}



 







