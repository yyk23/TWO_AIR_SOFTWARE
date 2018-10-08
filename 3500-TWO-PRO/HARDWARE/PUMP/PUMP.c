#include "PUMP.h" 
float PUMP_fbuf_A;									//
float PUMP_fbuf_B;									//

void TIM4_PWM_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIMx外设

	TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载周期值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置预分频值 不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx
}
void PUMP_AB_PWM_Init(void)
{
	TIM4_PWM_Init(60000-1,9-1);  //155Hz
}
//启动A路泵
void PUMP_A_Start(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIMx外设
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //使能GPIOB外设时钟
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4); //GPIOB9复用为定时器4
	
   //设置PB8为复用输出功能,输出TIM4 CH4的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOB9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//初始化PB9

	//初始化TIM4 Channe4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //设置互补端输出极性
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//不使能互补端输出
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //死区后输出状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//死区后互补端输出状态
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4OC4
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 
}
//停止A路泵
void PUMP_A_Stop(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIMx外设
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //使能GPIOB外设时钟
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4); //GPIOB8复用为定时器4

   //设置PB8为漏开功能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM4_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //漏开输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//初始化PB9
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH3 PWM2模式	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //关闭比较输出
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC4 低电平有效 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //设置互补端输出极性
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//不使能互补端输出
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //死区后输出状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//死区后互补端输出状态

	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIMx
}

//启动B路泵
void PUMP_B_Start(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIMx外设
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //使能GPIOB外设时钟
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4); //GPIOB8复用为定时器4
	
   //设置PB8为复用输出功能,输出TIM4 CH3的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //GPIOB8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//初始化PB8

	//初始化TIM4 Channe3 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //设置互补端输出极性
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//不使能互补端输出
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //死区后输出状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//死区后互补端输出状态
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4OC3
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 
}
//停止B路泵
void PUMP_B_Stop(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIMx外设
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //使能GPIOB外设时钟
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4); //GPIOB8复用为定时器4

   //设置PB9为漏开功能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM4_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //漏开输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//初始化PB8
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH4 PWM2模式	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //关闭比较输出
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC4 低电平有效 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //设置互补端输出极性
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//不使能互补端输出
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //死区后输出状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//死区后互补端输出状态

	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIMx
}
//启动C路泵
void PUMP_C_Start(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIMx外设
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //使能GPIOB外设时钟
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4); //GPIOB7复用为定时器4
	
   //设置PB7为复用输出功能,输出TIM4 CH2的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//初始化PB7

	//初始化TIM4 Channe2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //设置互补端输出极性
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//不使能互补端输出
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //死区后输出状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//死区后互补端输出状态
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 
}
//停止C路泵
void PUMP_C_Stop(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIMx外设
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //使能GPIOB外设时钟
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4); //GPIOB8复用为定时器4

   //设置PB7为漏开功能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM4_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //漏开输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//初始化PB7
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH2 PWM2模式	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //关闭比较输出
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC2 低电平有效 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //设置互补端输出极性
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//不使能互补端输出
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //死区后输出状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//死区后互补端输出状态

	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIMx
}

//启动D路泵
void PUMP_D_Start(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIMx外设
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //使能GPIOB外设时钟
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOB8复用为定时器4
	
   //设置PB6为复用输出功能,输出TIM4 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOB6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//初始化PB6

	//初始化TIM4 Channe1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //设置互补端输出极性
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//不使能互补端输出
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //死区后输出状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//死区后互补端输出状态
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 
}
//停止B路泵
void PUMP_D_Stop(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIMx外设
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //使能GPIOB外设时钟
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOB6复用为定时器4

   //设置PB6为漏开功能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM4_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //漏开输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              	//初始化PB6
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH4 PWM2模式	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //关闭比较输出
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 低电平有效 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;        //设置互补端输出极性
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//不使能互补端输出
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        //死区后输出状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//死区后互补端输出状态

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIMx
}

