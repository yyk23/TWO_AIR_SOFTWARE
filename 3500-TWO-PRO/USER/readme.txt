//20151015
main()
{
	.
	.
	.
	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks); 
	TIM1_Int_Init(500-1,16800-1);
	.
	.
	.
}
�����ڳ�ʼ��ʱ��TIM1��ʱ50ms��ʵ������ʱ��Ҫ���ܶ࣬��Լ��200ms���ң���֪��Ϊʲô��
main()
{
	.
	.
	.
	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks); 
	delay_ms(1);
	TIM1_Int_Init(500-1,16800-1);
	.
	.
	.
}
����delay_ms(1);�ٳ�ʼ��ʱ��TIM1��ʱ50ms��������

��delay_ms(1);�ŵ�TIM1_Int_Init(500-1,16800-1);�����ڲ��Ŀ�ʼ����ͬ�����У���֪��Ϊʲô�ˣ�

ԭ���ҵ���

void TIM1_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  ///ʹ��TIM1ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//��ʼ��TIM1
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //����ʱ��1�����ж�
	TIM_Cmd(TIM1,ENABLE); //ʹ�ܶ�ʱ��1
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; //��ʱ��1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
������TIM1�ĳ�ʼ������������TIM3�ĳ�ʼ�������ϸĵģ���STM�ĵ�Ƭ����TIM1��TIM3��Щ����TIM1
����������RCR�Ĵ�����Ҳ��Ҫ��ʼ��������Ҫ��TIM1�ĳ�ʼ��������Ϊ

void TIM1_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  ///ʹ��TIM1ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//��ʼ��TIM1
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //����ʱ��1�����ж�
	TIM_Cmd(TIM1,ENABLE); //ʹ�ܶ�ʱ��1
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; //��ʱ��1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
������TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0; 

2������ģ�鹩���ѹ���ȶ���Ҫ���ѹ3.1V~4.2V��������һֱ���ܳ�ʼ������ģ�飬������
ͨ���ϵ����ʱ2�뷢��ָ���ͨ���������Ƿ���Ҫ�ĵ�·?���ģ����Կ���5V��ѹ��Ȼ��ʹ
��3.3V��ѹоƬ��ģ�鹩�磬��SOT-23��װ��3.3V��ѹоƬ

3�������ﲻ���ı���ʱ���Ϊ60�룬ԭ��Ϊ15��
//20151030
����汾��V1.01
Ӳ���汾��ZR-3500-FOUR-MB-V1.00
�Ķ����ݣ�
1��C·���������ʺ�B·�����������ڴ洢ʱ�����ص�������B��C·�������ʵ�ַ��֮ǰ�汾�ĳ�����Ҫ
���±궨B��C��·

//20151106
����汾��ZR-3500-TWO-PRO-V1.00
Ӳ���汾��ZR-3500-MB-V6.0-151029
�Ķ����ݣ�
1������·����Ļ�����ȥ����CD��·������ݣ���Ϊ��˫·�ĳ���
2��ȥ����ParaBackup��������Ϊ������SaveParaһ����ÿ�ζ���Ҫͬʱ�Ķ�������©����(�Ѿ�����PID������©��)
3��ע��
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;							//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;									//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;								//����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;		//��ֹ������⣬ʹ���������
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 7;										//7��ת���ڹ���������
	ADC_Init(ADC1, &ADC_InitStructure);	
	�У�ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;����ǲ���ʡ�Եģ������������������ֵ��
	�����������������������������������ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	�������ã����²�����ȷ������ת��
	
//20160118
����汾��ZR-3500-TWO-PRO-V4.00
Ӳ���汾��ZR-3500-MB-V6.1-151231
�Ķ����ݣ�
1���汾�Ÿ�ΪV4.00��ԭ��δVer 1.00����Ϊ֮ǰ��˫·�汾���Ѿ�����3.0�����������4.0��ʼ
2����Ʒ���������5λ�ͺ�����3500E/3500F
3��������ѹ������������ѹ����ѹ������ѹ���ͳһΪ500�����ʹ̶�Ϊ1.0
4�����������̰汾��ΪH1.1,ȥ����ѹ���������ı��ʣ���ѹ������Ϊ3λ����2λС������ŵ�ַ��Ϊ2000�����ȸ�Ϊ20
5����ӡ�͵�������������ţ������ļ��и�Ϊ���ģ���������ͺţ���ӡ�͵����������Ӱ汾��
//20160126
����汾��ZR-3500-TWO-PRO-V4.00δ��
Ӳ���汾��ZR-3500-MB-V6.1-151231
�Ķ����ݣ�
1����ӡ��������ΪZR-3500�ʹ�����������ȥ���ͺ���Ŀո�
2�������ļ����������ļ�����Ҳȥ���ͺ���Ŀո�
//20160218
����汾��3500E:ZR-3500-TWO-PRO-V4.01   3500F:ZR-3500-TWO-PRO-V4.00
Ӳ���汾��ZR-3500-MB-V6.1-151231
�Ķ����ݣ�
1������ѹż���������ֵ�����Ҳ��ָܻ�����Ҫ�����ϵ磬��������Ƕ������ֵʱʹ���ϴε���Чֵ��Ϊ����ѹֵ��ʱ����
2��18B20���ӳ�ʼ������������ת������Ϊ12λ����ֹĳЩ����������Ĭ��12λ���ȵ����

//20160315
����汾��3500E:ZR-3500-TWO-PRO-V4.01   3500F:ZR-3500-TWO-PRO-V4.00(δ�İ汾�ţ���δ����)
Ӳ���汾��ZR-3500-MB-V6.1-151231   ZR-3500-MB-V6.2
�Ķ����ݣ�
1��ʱ��ѡ����ʾ��ʱ����Ӧ����������Ĳ����ļ���Ϊ��ʹ������ʱ��
2���¿��¶Ⱦ�������������ʾxx.x��������	
	DQIn();
	delay_us(10);						//10us
	if(DQ2In == 0)
	{
		wk_temperature_err = 1;
	}
	�⼸����������⣬���Ժ�İ汾�У�ȥ�����⼸�䣬ͬʱ��·��3920AIR��ҲӦ��ȥ��
//20160316
�����޸ģ������޸ĵİ汾���Ѿ�������������µ�����ֻ�ܷŵ��´��޸ĳ���ʱʹ���ˣ�
3������DGUS�����Ĵ���ʱ���ݱ������ڵڶ����������У���task.c��else if(locale_flag == DEF_READ_REGISTER_RESPONSE)
��Ϊ��else{}����������Ҳ��Ϊ�˵ڶ�������������������¼��״̬�������ָܻ�
//20160321
����汾��3500E:ZR-3500-TWO-PRO-V4.02   3500F:ZR-3500-TWO-PRO-V4.01
Ӳ���汾��ZR-3500-MB-V6.1-151231  ZR-3500-MB-V6.2
�Ķ����ݣ�
1��
USBH_MSC_Handle() 

{......

{ switch(USBH_MSC_BOTXferParam.MSCState)



 { case USBH_MSC_BOT_INIT_STATE:

 USBH_MSC_Init(pdev); 

//USBH_MSC_BOTXferParam.MSCState = USBH_MSC_BOT_RESET; 



USBH_MSC_BOTXferParam.MSCState = USBH_MSC_GET_MAX_LUN;

 break; 

��
USBH_HandleControl( )

{

......

switch (phost->Control.state)

{

case CTRL_DATA_IN_WAIT:



 URB_Status = HCD_GetURB_State(pdev , phost->Control.hc_num_in); 



 /* check is DATA packet transfered successfully */

 if (URB_Status == URB_DONE)

 { 

 phost->Control.state = CTRL_STATUS_OUT;

 }



 /* manage error cases*/

 if (URB_Status == URB_STALL) 

 { 

 /* In stall case, return to previous machine state*/

 phost->gState = phost->gStateBkp;

 phost->Control.state = CTRL_STALLED;  //

 } 
������֧���˶�������һЩ���U���ˣ�֮ǰ����һ������16G���̿��Զ��������ļ������ļ�ϵͳ��ֻ������
��ʽ��������Ҳ�õ��˽��

//20161014
����汾��3500E:ZR-3500-TWO-PRO-V4.10   3500F:ZR-3500-TWO-PRO-V4.10(�������û���·������������Ժ��ٸ�ʱ�ڰ汾�Ų��ظĶ�)
Ӳ���汾��ZR-3500-MB-V6.1-151231   ZR-3500-MB-V6.2
�Ķ����ݣ�
1�����˼��ʱ��Ϊ0ʱB·�����������⣬���ʱ����Ϊ0��A��B·��У�㣬ֱ����������

//20161125
����汾��3500E:ZR-3500-TWO-PRO-V4.10   3500F:ZR-3500-TWO-PRO-V4.10
Ӳ���汾��ZR-3500-MB-V6.1-151231   ZR-3500-MB-V6.2
�Ķ����ݣ�
1�������궨��Ϊ����궨�����������뵥�㱶����˵ķ�ʽ���궨�������ȱ궨0.3��0.6��1.0�����㱶�ʣ�����֤�����㱶�ʣ�
2��������ʾ����ʱ�䱻���µ���Ϣ��ʾ
3����ѹ������������ʼ��ΪĬ��ֵ������