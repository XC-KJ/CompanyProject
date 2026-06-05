#include "L298N.h"


void L298N_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);	//开启GPIOC的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
}

void L298N_ONmode(uint8_t BitValue)
{
//	GPIO_WriteBit(GPIOC,GPIO_Pin_0,(BitAction)(BitValue==0?0:1));
//	GPIO_WriteBit(GPIOC,GPIO_Pin_1,(BitAction)(BitValue==0?1:0));
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_0, BitValue ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_1, BitValue ? Bit_RESET : Bit_SET);
}
			
void L298N_OFF(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
}
