#include "LED.h"

/**
  * 函    数：LED初始化
  * 参    数：无
  * 返 回 值：无
  */
void LED_Init(void)
{
	/*开启时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
																													//使用各个外设前必须开启时钟，否则对外设的操作无效
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;										//定义结构体变量				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;						//GPIO模式，赋值为输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//GPIO模式输出，赋值为推挽输出模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;				//GPIO模式上拉模式，赋值为无上无下  输出模式不需要
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//GPIO速度，赋值为100MHz
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;							//GPIO引脚，赋值为2号引脚	
	GPIO_Init(GPIOB, &GPIO_InitStructure);									//将赋值后的构体变量传递给GPIO_Init函数
	                                                        //函数内部会自动根据结构体的参数配置相应寄存器
	                                                        //实现GPIOB的初始化
	
	/*设置GPIO引脚的高低电平*/
	/*若不设置GPIO引脚的电平，则在GPIO初始化为推挽输出后，指定引脚默认输出低电平*/
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);											
}

	
	
	

/**
  * 函    数：LED1开启
  * 参    数：无
  * 返 回 值：无
  */
void LED1_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);		//设置PA1引脚为低电平
}

/**
  * 函    数：LED1关闭
  * 参    数：无
  * 返 回 值：无
  */
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_2);		//设置PA1引脚为高电平
}

/**
  * 函    数：LED1状态翻转
  * 参    数：无
  * 返 回 值：无
  */
void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_2) == 0)		//获取输出寄存器的状态，如果当前引脚输出低电平
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_2);					//则设置PA1引脚为高电平
	}
	else													//否则，即当前引脚输出高电平
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_2);					//则设置PA1引脚为低电平
	}
}
