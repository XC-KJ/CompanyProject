#ifndef __USART__H__
#define __USART__H__

#include "stm32f4xx.h"
#include "stdio.h"
#include "stdarg.h"
#include "stdint.h"

extern uint8_t Serial_TxPacket[];
extern uint8_t Serial_RxPacket[];
extern uint8_t Serial_RxFlag;  //串口标志位声明

void Usart1_Init(void);	//串口1初始化
void Usart2_Init(void);	//串口2初始化
/*串口1发送*/
void Serial_SendByte(uint8_t Byte);	//发送一个字节
void Serial_SendArray(uint8_t *Array,uint16_t Length);	//发送一个数组
void Serial_SendString(char *String);	//发送一个字符串
void Serial_SendNumber(uint32_t Number,uint8_t Length);	//发送字符形式的数字
void Serial_Printf(char *format,...);	//可以进行多端显示打印

void Serial_Sendpacket(void);	//发送数据包

/*串口2发送函数*/

void Usart2_SendByte(uint8_t Byte);	//发送一个字节
void Usart2_SendString(char *String);  //发送一个字符串
void Usart2_Printf(char *format,...);  //可以进行多端显示打印

/*串口1接收*/
uint8_t Serial_GetRxDaTa(void);		//查询方法接收数据
uint8_t Serial_GetRxFlag(void);

uint16_t CRC16_Calculate(uint8_t *data, uint16_t length); // CRC16计算函数
void Data_Verification(uint8_t crc_high, uint8_t crc_low);	//数据校验
void Data_Processing(void); // 数据处理





#endif
