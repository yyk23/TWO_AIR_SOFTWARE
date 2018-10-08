#include "PUMP.h" 
float PUMP_fbuf_A;									//
float PUMP_fbuf_B;									//

void TIM4_PWM_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIMx����

	TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����Ԥ��Ƶֵ ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx
}
void PUMP_AB_PWM_Init(void)
{
	TIM4_PWM_Init(60000-1,9-1);  //155Hz
}
//����A·��
void PUMP_A_Start(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIMx����
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //ʹ��GPIOB����ʱ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4); //GPIOB9����Ϊ��ʱ��4
	
   //����PB8Ϊ�����������,���TIM4 CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOB9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//��ʼ��PB9

	//��ʼ��TIM4 Channe4 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //���û������������
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//��ʹ�ܻ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //���������״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//�����󻥲������״̬
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4OC4
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 
}
//ֹͣA·��
void PUMP_A_Stop(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIMx����
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //ʹ��GPIOB����ʱ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4); //GPIOB8����Ϊ��ʱ��4

   //����PB8Ϊ©������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM4_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //©�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//��ʼ��PB9
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH3 PWM2ģʽ	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�رձȽ����
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC4 �͵�ƽ��Ч 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //���û������������
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//��ʹ�ܻ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //���������״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//�����󻥲������״̬

	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIMx
}

//����B·��
void PUMP_B_Start(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIMx����
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //ʹ��GPIOB����ʱ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4); //GPIOB8����Ϊ��ʱ��4
	
   //����PB8Ϊ�����������,���TIM4 CH3��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //GPIOB8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//��ʼ��PB8

	//��ʼ��TIM4 Channe3 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //���û������������
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//��ʹ�ܻ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //���������״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//�����󻥲������״̬
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4OC3
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 
}
//ֹͣB·��
void PUMP_B_Stop(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIMx����
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //ʹ��GPIOB����ʱ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4); //GPIOB8����Ϊ��ʱ��4

   //����PB9Ϊ©������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM4_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //©�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//��ʼ��PB8
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH4 PWM2ģʽ	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�رձȽ����
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC4 �͵�ƽ��Ч 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //���û������������
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//��ʹ�ܻ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //���������״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//�����󻥲������״̬

	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIMx
}
//����C·��
void PUMP_C_Start(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIMx����
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //ʹ��GPIOB����ʱ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4); //GPIOB7����Ϊ��ʱ��4
	
   //����PB7Ϊ�����������,���TIM4 CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//��ʼ��PB7

	//��ʼ��TIM4 Channe2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //���û������������
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//��ʹ�ܻ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //���������״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//�����󻥲������״̬
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 
}
//ֹͣC·��
void PUMP_C_Stop(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIMx����
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //ʹ��GPIOB����ʱ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4); //GPIOB8����Ϊ��ʱ��4

   //����PB7Ϊ©������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM4_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //©�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//��ʼ��PB7
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH2 PWM2ģʽ	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�رձȽ����
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC2 �͵�ƽ��Ч 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //���û������������
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//��ʹ�ܻ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //���������״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//�����󻥲������״̬

	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIMx
}

//����D·��
void PUMP_D_Start(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIMx����
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //ʹ��GPIOB����ʱ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOB8����Ϊ��ʱ��4
	
   //����PB6Ϊ�����������,���TIM4 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOB6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//��ʼ��PB6

	//��ʼ��TIM4 Channe1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //���û������������
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//��ʹ�ܻ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //���������״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//�����󻥲������״̬
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 
}
//ֹͣB·��
void PUMP_D_Stop(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIMx����
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //ʹ��GPIOB����ʱ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOB6����Ϊ��ʱ��4

   //����PB6Ϊ©������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM4_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //©�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//��ʼ��PB6
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH4 PWM2ģʽ	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�رձȽ����
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 �͵�ƽ��Ч 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //���û������������
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//��ʹ�ܻ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //���������״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//�����󻥲������״̬

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIMx
}

