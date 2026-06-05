#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f4xx.h"

void Encoder_Init(void);
int16_t Encoder_Get_R(void);
int16_t Encoder_Get_L(void);

#endif
