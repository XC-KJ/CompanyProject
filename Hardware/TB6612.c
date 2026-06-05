#include "TB6612.h"
#include "PWM.h"

void TB6612_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	PWM_Init();	//兜兵晒PWM
}

void Motor_SetSpeed1(int16_t Speed)
{
	if(Speed>=0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_15);	//性互pin4哈重
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);	//性詰pin5哈重
		PWM_SetCompare2(Speed);
	}
	else if(Speed<=0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_14);	//性互pin4哈重
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);	//性詰pin5哈重
		PWM_SetCompare2(-Speed);
	}
}

void Motor_SetSpeed2(int16_t Speed)
{
	if(Speed>=0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_13);	//性互pin4哈重
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);	//性詰pin5哈重
		PWM_SetCompare1(Speed);
	}
	else if(Speed<=0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);	//性互pin4哈重
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);	//性詰pin5哈重
		PWM_SetCompare1(-Speed);
	}
}




