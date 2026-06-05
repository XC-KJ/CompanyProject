#ifndef __PWM__H__
#define __PWM__H__

#include "stm32f4xx.h"


void PWM_Init(void);

void PWM_SetCompare1(uint16_t Compare);
void PWM_SetCompare2(uint16_t Compare);



#endif
