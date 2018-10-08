 #include "adc.h"
 #include "delay.h"	   

float ADBuf[16];					//ad缓冲区
u32 adcount = 0;					//ad次数
	   																   
void  ADC1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);	//使能GPIOA\C时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 	//使能ADC1时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;			//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PA5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;			//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化 
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PC2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;			//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//不带上下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//初始化
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  	//ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;						//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 		//DMA失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;						//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);										//初始化

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;							//12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;									//扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;								//连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;		//禁止触发检测，使用软件触发
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 3;										//7个转换在规则序列中
	ADC_Init(ADC1, &ADC_InitStructure);												//ADC初始化
}				

u16 ADC_Table[500];
#define ADC1_DR_Address    ((u32)0x4001204C)
/* 函数名：ADC1_Mode_Config
 * 描述  ：配置ADC1的工作模式为DMA模式
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */

void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	//启动DMA时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_DeInit(DMA2_Stream0);
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_BASE + 0x4C;
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&ADC_Table[0];					//(u32)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;						//外设作为数据传输的来源
	DMA_InitStructure.DMA_BufferSize = 90;										//3*30次
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//用来设定外设地址寄存器递增与否
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//用来设定内存地址寄存器递增与否。
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//设定了外设数据宽度
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//设定了存储器数据宽度
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							//设定DMA通道x的软件优先级
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, DISABLE);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_112Cycles);   		//保温箱电流
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2,ADC_SampleTime_112Cycles);			//电池电压
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4,  3,ADC_SampleTime_112Cycles);			//充电电流
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);													//
	ADC_Cmd(ADC1, ENABLE);														//开启AD转换器	
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
* 函数名：    Average_AD_Value
* 函数原型：  U16 Average_AD_Value(U16 AD_Value_Tab[],U8 channel)
* 功能描述：  求channle通道的AD转换值的平均数
* 输入参数：  AD_Value_Tab：存放AD转换结果的数组。数组的内容是由DMA填充的
			  channel：待求转换结果的通道
* 返回值：    某个通道的平均值
* 被调用函数：无
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
	ADC_Cmd(ADC1, DISABLE);	//停止指定的ADC1
}



 







