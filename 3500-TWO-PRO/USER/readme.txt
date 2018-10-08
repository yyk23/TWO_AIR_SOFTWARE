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
这样在初始化时，TIM1定时50ms，实际运行时间要慢很多，大约在200ms左右，不知道为什么？
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
增加delay_ms(1);再初始化时，TIM1定时50ms就正常了

把delay_ms(1);放到TIM1_Int_Init(500-1,16800-1);函数内部的开始处，同样不行，不知道为什么了？

原因找到了

void TIM1_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  ///使能TIM1时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化TIM1
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //允许定时器1更新中断
	TIM_Cmd(TIM1,ENABLE); //使能定时器1
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; //定时器1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
以上是TIM1的初始化函数，是在TIM3的初始化函数上改的，但STM的单片机的TIM1和TIM3有些区别，TIM1
额外增加了RCR寄存器，也需要初始化，所以要将TIM1的初始化函数改为

void TIM1_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  ///使能TIM1时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化TIM1
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //允许定时器1更新中断
	TIM_Cmd(TIM1,ENABLE); //使能定时器1
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; //定时器1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
即增加TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0; 

2、蓝牙模块供电电压不稳定，要求电压3.1V~4.2V，波动后一直不能初始化蓝牙模块，但后来
通过上电后延时2秒发送指令，则通信正常，是否需要改电路?若改，可以控制5V电压，然后使
用3.3V稳压芯片给模块供电，有SOT-23封装的3.3V稳压芯片

3、流量达不到的保护时间改为60秒，原来为15秒
//20151030
程序版本：V1.01
硬件版本：ZR-3500-FOUR-MB-V1.00
改动内容：
1、C路的流量倍率和B路的流量倍率在存储时存在重叠，改了B、C路流量倍率地址，之前版本的程序需要
重新标定B、C两路

//20151106
程序版本：ZR-3500-TWO-PRO-V1.00
硬件版本：ZR-3500-MB-V6.0-151029
改动内容：
1、在四路程序的基础上去掉了CD两路相关内容，改为了双路的程序
2、去掉了ParaBackup函数，因为功能与SavePara一样，每次都需要同时改动，容易漏东西(已经发现PID参数被漏掉)
3、注意
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;							//12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;									//扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;								//连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;		//禁止触发检测，使用软件触发
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 7;										//7个转换在规则序列中
	ADC_Init(ADC1, &ADC_InitStructure);	
	中，ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;这句是不能省略的，否则这个可能是任意值，
	将冲掉与它“或”运算的任意其他设置项，包括ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	这项设置，导致不能正确触发或转换
	
//20160118
程序版本：ZR-3500-TWO-PRO-V4.00
硬件版本：ZR-3500-MB-V6.1-151231
改动内容：
1、版本号改为V4.00，原来未Ver 1.00，因为之前的双路版本号已经到了3.0，所以这次以4.0开始
2、产品编号增加了5位型号名称3500E/3500F
3、换数字压力传感器，流压、计压、大气压零点统一为500，倍率固定为1.0
4、触摸屏工程版本改为H1.1,去掉了压力传感器的倍率，计压的零点改为3位整数2位小数，编号地址改为2000，长度改为20
5、打印和导出增加仪器编号，导出文件夹改为中文，编号增加型号，打印和导出数据增加版本号
//20160126
程序版本：ZR-3500-TWO-PRO-V4.00未改
硬件版本：ZR-3500-MB-V6.1-151231
改动内容：
1、打印仪器名改为ZR-3500型大气采样器，去掉型后面的空格
2、导出文件名、导出文件内容也去掉型后面的空格
//20160218
程序版本：3500E:ZR-3500-TWO-PRO-V4.01   3500F:ZR-3500-TWO-PRO-V4.00
硬件版本：ZR-3500-MB-V6.1-151231
改动内容：
1、大气压偶尔出现最大值，并且不能恢复，需要重新上电，解决方法是读到最大值时使用上次的有效值作为大气压值临时代替
2、18B20增加初始化函数，设置转换精度为12位，防止某些传感器不是默认12位精度的情况

//20160315
程序版本：3500E:ZR-3500-TWO-PRO-V4.01   3500F:ZR-3500-TWO-PRO-V4.00(未改版本号，因未发货)
硬件版本：ZR-3500-MB-V6.1-151231   ZR-3500-MB-V6.2
改动内容：
1、时钟选择显示屏时，相应的数据里面的采样文件改为了使用主板时钟
2、温控温度经常发现两次显示xx.x，怀疑是	
	DQIn();
	delay_us(10);						//10us
	if(DQ2In == 0)
	{
		wk_temperature_err = 1;
	}
	这几句引起的问题，在以后的版本中，去掉了这几句，同时四路和3920AIR型也应该去掉
//20160316
补充修改（昨天修改的版本号已经发货，因此以下的问题只能放到下次修改程序时使用了）
3、迪文DGUS屏读寄存器时数据被保存在第二个缓冲区中，在task.c中else if(locale_flag == DEF_READ_REGISTER_RESPONSE)
改为了else{}，里面内容也改为了第二缓冲区，否则在数据录入状态屏保后不能恢复
//20160321
程序版本：3500E:ZR-3500-TWO-PRO-V4.02   3500F:ZR-3500-TWO-PRO-V4.01
硬件版本：ZR-3500-MB-V6.1-151231  ZR-3500-MB-V6.2
改动内容：
1、
USBH_MSC_Handle() 

{......

{ switch(USBH_MSC_BOTXferParam.MSCState)



 { case USBH_MSC_BOT_INIT_STATE:

 USBH_MSC_Init(pdev); 

//USBH_MSC_BOTXferParam.MSCState = USBH_MSC_BOT_RESET; 



USBH_MSC_BOTXferParam.MSCState = USBH_MSC_GET_MAX_LUN;

 break; 

和
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
这样就支持了读卡器和一些别的U盘了，之前发现一个闪迪16G的盘可以读，导出文件后发现文件系统损坏只能重新
格式化的现象也得到了解决

//20161014
程序版本：3500E:ZR-3500-TWO-PRO-V4.10   3500F:ZR-3500-TWO-PRO-V4.10(这个程序没有下发生产，所以以后再改时在版本号不必改动)
硬件版本：ZR-3500-MB-V6.1-151231   ZR-3500-MB-V6.2
改动内容：
1、改了间隔时间为0时B路不启动的问题，间隔时间设为0后A、B路不校零，直接启动采样

//20161125
程序版本：3500E:ZR-3500-TWO-PRO-V4.10   3500F:ZR-3500-TWO-PRO-V4.10
硬件版本：ZR-3500-MB-V6.1-151231   ZR-3500-MB-V6.2
改动内容：
1、流量标定改为三点标定，其余点拟合与单点倍率相乘的方式（标定方法，先标定0.3，0.6和1.0三个点倍率，再验证其他点倍率）
2、增加显示屏长时间被按下的信息提示
3、将压力传感器零点初始化为默认值并保存