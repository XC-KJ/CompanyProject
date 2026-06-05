#include "usart.h"

//extern int8_t RX_Speed_L,RX_Speed_R;

uint8_t Serial_TxPacket[4];	//串口发送数据数组（主指令+3个数据）
uint8_t Serial_RxPacket[4];	//串口接收数据数组（主指令+3个数据）
uint8_t Serial_RxFlag;

#define PKT_HEADER 0xAA		//包头宏定义

// CRC16-MODBUS参数
#define CRC16_POLY 0x8005
#define CRC16_INIT 0xFFFF

/**
 * @brief  CRC16-MODBUS校验计算函数
 * @param  data: 待校验数据指针
 * @param  length: 数据长度
 * @retval 16位CRC校验值
 */
uint16_t CRC16_Calculate(uint8_t *data, uint16_t length)
{
    uint16_t crc = CRC16_INIT;
    uint16_t i, j;
    
    for(i = 0; i < length; i++)
    {
        crc ^= data[i];
        for(j = 0; j < 8; j++)
        {
            if(crc & 0x0001)
            {
                crc >>= 1;
                crc ^= CRC16_POLY;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    
    return crc;
}

void Usart1_Init(void)
{
	/*打开时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	//*选择复用功能*//
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	/*开启GPIO*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*配置串口*/
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=115200;	//设置波特率  蓝牙模块  9600   AX650是115200
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//流控选择
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;	//串口模式
	USART_InitStructure.USART_Parity=USART_Parity_No;	//校验位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;	//停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;	//字长(8位/9位)
	USART_Init(USART1,&USART_InitStructure);
	
	/*开启中断*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//开启RXNE标志位到NVIC的输出
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//NVIC分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;	//中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//中断使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	/*开启USART1*/
	USART_Cmd(USART1,ENABLE);
}

//void Usart2_Init(void)
//{
//	/*打开时钟*/
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	
//	/*开启GPIO*/
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	/*配置串口*/
//	USART_InitTypeDef USART_InitStructure;
//	USART_InitStructure.USART_BaudRate=115200;	//设置波特率
//	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//流控选择
//	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;	//串口模式
//	USART_InitStructure.USART_Parity=USART_Parity_No;	//校验位
//	USART_InitStructure.USART_StopBits=USART_StopBits_1;	//停止位
//	USART_InitStructure.USART_WordLength=USART_WordLength_8b;	//字长(8位/9位)
//	USART_Init(USART2,&USART_InitStructure);
//	
//	/*开启中断*/
//	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	//开启RXNE标志位到NVIC的输出
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//NVIC分组
//	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;	//中断通道
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//中断使能
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	/*开启USART1*/
//	USART_Cmd(USART2,ENABLE);
//}

/*
	发送函数串口1
*/
void Serial_SendByte(uint8_t Byte)	//发送一个字节
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	//检测标志位
}

void Serial_SendArray(uint8_t *Array,uint16_t Length)	//发送一个数组
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)	//发送一个字符串
{
	uint8_t i;
	for(i=0;String[i]!=0;i++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result=1;
	while(Y--)
	{
		Result*=X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number,uint8_t Length)	//发送字符形式的数字
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Number/Serial_Pow(10,Length-i-1)%10+'0');
	}
}

int fputc(int ch,FILE *f)		//调用stdio.h进行打印
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format,...)	//可以进行多端显示打印
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	Serial_SendString(String);
}

/*数据包发送 - 新CRC16格式（无包尾）*/
void Serial_Sendpacket(void)	
{
	uint16_t crc;
	
	// 计算CRC16校验值
	crc = CRC16_Calculate(Serial_TxPacket, 4);
	
	// 发送数据包格式：包头(0xAA) + 4字节数据 + CRC16高字节 + CRC16低字节（无包尾）
	Serial_SendByte(PKT_HEADER);
	Serial_SendArray(Serial_TxPacket, 4);
	Serial_SendByte((uint8_t)(crc >> 8));   // 先发送高字节
	Serial_SendByte((uint8_t)(crc & 0xFF)); // 再发送低字节
}


/*串口2发送相关*/

void Usart2_SendByte(uint8_t Byte)	//发送一个字节
{
	USART_SendData(USART2,Byte);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);	//检测标志位
}

void Usart2_SendString(char *String)	//发送一个字符串
{
	uint8_t i;
	for(i=0;String[i]!=0;i++)
	{
		Usart2_SendByte(String[i]);
	}
}

void Usart2_Printf(char *format,...)	//可以进行多端显示打印
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	Usart2_SendString(String);
}
/*
	接收函数
*/
uint8_t Serial_GetRxDaTa(void)		//查询方法接收数据
{
	uint8_t RxTada;
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
	{
		RxTada=USART_ReceiveData(USART1);
	}
	return RxTada;
}

uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag==1)
	{
		Serial_RxFlag=0;
		return 1;
	}
	return 0;
}

/*数据校验 - 新CRC16校验*/
void Data_Verification(uint8_t crc_high, uint8_t crc_low)
{
	uint16_t calculated_crc;
	uint16_t received_crc;
	
	// 计算接收到的数据的CRC16值
	calculated_crc = CRC16_Calculate(Serial_RxPacket, 4);
	
	// 组合接收到的CRC16值
	received_crc = (uint16_t)crc_high << 8 | crc_low;
	
	// 校验通过
	if(calculated_crc == received_crc)
	{
		Serial_RxFlag=1;
		Data_Processing();
	}
	else 
	{
		// 校验失败，丢弃数据包
		Serial_RxFlag=0;
	}
}

/*接收数据包 - 新CRC16格式（无包尾）*/
void USART1_IRQHandler(void)	//中断接收
{
	static uint8_t RxState=0;
	static uint8_t pRxPacket=0;
	static uint8_t crc_high, crc_low;
	
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
	{
		uint8_t RxData=USART_ReceiveData(USART1);
		
		switch(RxState)
		{
			case 0: // 等待包头
				if(RxData==PKT_HEADER)
				{
					RxState=1;
					pRxPacket=0;
				}
				break;
				
			case 1: // 接收4字节数据
				Serial_RxPacket[pRxPacket]=RxData;
				pRxPacket ++;
				
				if(pRxPacket>=4)
				{
					RxState=2;
				}
				break;
				
			case 2: // 接收CRC16高字节
				crc_high = RxData;
				RxState=3;
				break;
				
			case 3: // 接收CRC16低字节
				crc_low = RxData;
				// 接收到完整数据包，直接进行CRC校验（无包尾）
				Data_Verification(crc_high, crc_low);
				// 回到初始状态等待下一个包
				RxState=0;
				break;
		}			
		
		USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);
	}
}
