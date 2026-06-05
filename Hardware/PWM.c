#include "PWM.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		//开启AFIO时钟
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);	//端口重映射函数
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//解除PA15.PB3.PB4引脚复用
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM2);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		
	
	TIM_InternalClockConfig(TIM2);
		
	TIM_TimeBaseInitTypeDef TIM_TimeBaselnitStructure;
	TIM_TimeBaselnitStructure.TIM_ClockDivision=TIM_CKD_DIV1;	//选择滤波分频
	TIM_TimeBaselnitStructure.TIM_CounterMode=TIM_CounterMode_Up;		//选择计算模式
	TIM_TimeBaselnitStructure.TIM_Period=1000-1;		//ARR自动重装器的值
	TIM_TimeBaselnitStructure.TIM_Prescaler=4-1;		//PSC预分频器的值
	TIM_TimeBaselnitStructure.TIM_RepetitionCounter=0;	//重复计数器的值
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaselnitStructure);
		
	TIM_OCInitTypeDef TIM_OClnitStructure;
	TIM_OCStructInit(&TIM_OClnitStructure);
	TIM_OClnitStructure.TIM_OCMode=TIM_OCMode_PWM1;	//设置输出比较的模式
	TIM_OClnitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;	//设置输出比较的极性
	TIM_OClnitStructure.TIM_OutputState=TIM_OutputState_Enable;	//设置输出使能
	TIM_OClnitStructure.TIM_Pulse=0;	//设置CCR
	
	TIM_OC3Init(TIM2,&TIM_OClnitStructure);
	TIM_OC4Init(TIM2,&TIM_OClnitStructure);
		  
	TIM_Cmd(TIM2,ENABLE);	//启动定时器
}


void PWM_SetCompare1(uint16_t Compare)
{
//	TIM_SetCompare3(TIM2,Compare);	//TIM_SetCompare1（）；设置CCR值
	TIM2->CCR3=Compare;
}
void PWM_SetCompare2(uint16_t Compare)
{
//	TIM_SetCompare4(TIM2,Compare);	//TIM_SetCompare2（）；设置CCR值
	TIM2->CCR4=Compare;
}
