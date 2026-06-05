#include "stm32f4xx.h"
#include <Delay.h>
#include <stdio.h>
#include "usart.h"
#include "Timer.h"
#include "PID.h"
#include "Key.h"
#include <math.h>  // 添加fabs函数支持
#include "L298N.h"
#include "OLED.h"
#include "TB6612.h"
#include "Encoder.h"
#include "LED.h"
#include "freertos.h"
#include "task.h"


/*动作类型定义*/
#define ACTION_NONE 0
#define ACTION_FORWARD 1
#define ACTION_STOP 2
#define ACTION_LEFT 3
#define ACTION_RIGHT 4

/* 全局动作状态变量*/
volatile uint8_t current_action = ACTION_NONE;
volatile uint32_t action_remaining_time = 0;
volatile uint8_t action_finished_flag = 0;

int16_t Speed_L,Speed_R;		//速度，位置

// 位置到达信号定义
#define POSITION_REACHED_SIGNAL 0xFF



PID_t InnerR={
	.Kp=0,
	.Ki=0,
	.Kd=0,
	.OutMax=900,
	.OutMin=-900,
	.Target=100,	//速度最高600
};
PID_t InnerL={
	.Kp=0,
	.Ki=0,
	.Kd=0,
	.OutMax=100,
	.OutMin=-100,
//	.Target=0,  //速度最高600
};

uint8_t KeyNum=0;	//按键变量获取




/*RTOS变量及函数声明*/
//Task priority    //任务优先级
#define START_TASK_PRIO	1

//Task stack size //任务堆栈大小	
#define START_STK_SIZE 	256  

//Task handle     //任务句柄
TaskHandle_t StartTask_Handler;

//Task function   //任务函数
void start_task(void *pvParameters);

/*LED任务相关*/

//Task priority    //任务优先级
#define LED_TASK_PRIO	4

//Task stack size //任务堆栈大小	
#define LED_STK_SIZE 	256  

//Task handle     //任务句柄
//TaskHandle_t StartTask_Handler;

//Task function   //任务函数
void LED_task(void *pvParameters);
void Log_task(void *pvParameters);

int main()
{
	
	Delay_init();
	
	Usart1_Init();
//	Timer1_Init();
	Encoder_Init();
	TB6612_Init();
	L298N_Init();
	
	LED_Init();
	Key_Init();	
	
	
		xTaskCreate((TaskFunction_t )start_task,            //Task function   //任务函数
							(const char*    )"start_task",          //Task name       //任务名称
							(uint16_t       )START_STK_SIZE,        //Task stack size //任务堆栈大小
							(void*          )NULL,                  //Arguments passed to the task function //传递给任务函数的参数
							(UBaseType_t    )START_TASK_PRIO,       //Task priority   //任务优先级
							(TaskHandle_t*  )&StartTask_Handler);   //Task handle     //任务句柄    					
	vTaskStartScheduler();  //Enables task scheduling //开启任务调度	
	
	while (1)
	{
		// ==================== 新增：动作执行和计时 ====================
        if(current_action != ACTION_NONE && action_remaining_time > 0)
        {
            // 1ms延时，精确计时
            Delay_ms(1);
            action_remaining_time--;
            
            // 时间到，停止动作并标记完成
            if(action_remaining_time == 0)
            {
                Motor_SetSpeed1(0);
                Motor_SetSpeed2(0);
                current_action = ACTION_NONE;
                action_finished_flag = 1;
            }
        }

        // ==================== 新增：动作完成反馈 ====================
        if(action_finished_flag)
        {
            action_finished_flag = 0;
            
            // 向上位机发送动作完成信号（使用新的CRC16协议）
            Serial_TxPacket[0] = 0x08; // 指令码：动作完成
            Serial_TxPacket[1] = 0x00;
            Serial_TxPacket[2] = 0x00;
            Serial_TxPacket[3] = 0x00;
            Serial_Sendpacket();
        }
		
//		printf("%d,%d\n",Speed_R,Speed_L);		
				
		KeyNum=Key_GetNum();
		
		if(KeyNum==1)
		{ 
			LED1_Turn();
			// 发送测试指令（使用新的CRC16协议）
			Serial_TxPacket[0] = 0x01;
			Serial_TxPacket[1] = 0x02;
			Serial_TxPacket[2] = 0x03;
			Serial_TxPacket[3] = 0x04;
			Serial_Sendpacket();
			
			/*shangjianceshi */
//			L298N_ONmode(0);
//			Delay_s(1);
//			L298N_ONmode(1);
//			Delay_s(2);
//			L298N_OFF();
			
				/*
				Usart2_Printf("#000P0500T3000!");	//p500-2500 t0-9999
				Delay_ms(500);
				Usart2_Printf("#000P0700T3000!");
				Delay_ms(500);
				Usart2_Printf("#000P1000T3000!");
				Delay_ms(500);
				Usart2_Printf("#000P1500T3000!");
				Delay_ms(500);
				Usart2_Printf("#000P2000T3000!");
				Delay_ms(500);
				Usart2_Printf("#000P2500T3000!");
				Delay_ms(500);
			*/
		} 
	}
}
///

void start_task(void *pvParameters)
{
	taskENTER_CRITICAL(); //Enter the critical area //进入临界区
	
	//Create the task //创建任务
	xTaskCreate(LED_task,  "LED_task",  LED_STK_SIZE,  NULL, LED_TASK_PRIO,  NULL);	//LED闪烁任务
	xTaskCreate(Log_task,  "Log_task",  256,  NULL, 4,  NULL);	//LED闪烁任务
	xTaskCreate(Log_task,  "Log_task",  256,  NULL, 4,  NULL);	//LED闪烁任务

	vTaskDelete(StartTask_Handler); //Delete the start task //删除开始任务

  taskEXIT_CRITICAL();            //Exit the critical section//退出临界区
}


void LED_task(void *pvParameters)
{
	while(1){
		GPIO_WriteBit(GPIOB,GPIO_Pin_2,Bit_RESET);
		vTaskDelay(200);
		GPIO_WriteBit(GPIOB,GPIO_Pin_2,Bit_SET);
		vTaskDelay(200);
	}
	
}

void Log_task(void *pvParameters)
{
	uint16_t Counst=0;
	while(1)
	{
		Counst++;
		printf("这是Log打印任务%d\r\n",Counst);
		vTaskDelay(500);
	}
}




///
void Data_Processing(void)	//数据处理函数
{
	if(Serial_RxFlag==1)
	{
		switch(Serial_RxPacket[0])
		{
			case 0x06:
			{
				if(Serial_RxPacket[1]==0x01)	// 升降台-升
				{
					L298N_ONmode(1);
				}
				else if(Serial_RxPacket[1]==0x02)  // 升降台-降
				{
					L298N_ONmode(0);
				}
				else if(Serial_RxPacket[1]==0x03) // 升降台停止
				{
					L298N_OFF();
				}
			}
			break;
			
			case 0x07:	// 运动控制指令
			{
				if(Serial_RxPacket[1]==0x01)
				{
					// 前进：设置动作类型和执行时间（单位：毫秒）
					current_action = ACTION_FORWARD;
					action_remaining_time = 0; // 前进没有固定时间，由上位机控制停止
					Motor_SetSpeed1(150);
					Motor_SetSpeed2(150);
				}
				else if(Serial_RxPacket[1]==0x02)
				{
					// 后退（预留）
				}
				else if(Serial_RxPacket[1]==0x03)
				{
					current_action = ACTION_STOP;
					action_remaining_time = 0;
					Motor_SetSpeed1(0);
					Motor_SetSpeed2(0);
					// 停止动作立即完成
					action_finished_flag = 1;
				}
				else if(Serial_RxPacket[1]==0x04)	// 左转
				{
					// 只设置动作标记和延时时间，不在中断里执行
					current_action = ACTION_LEFT;
					action_remaining_time = 265; // 保留你校准好的265ms
					// 立即启动电机
					Motor_SetSpeed1(-150);
					Motor_SetSpeed2(150);
				}
				else if(Serial_RxPacket[1]==0x05)  // 右转
				{
					current_action = ACTION_RIGHT;
					action_remaining_time = 200; // 保留你校准好的235ms
					Motor_SetSpeed1(150);
					Motor_SetSpeed2(-150);
				}
			}
			break;
		}//switch结束	
		Serial_RxFlag=0;
	}
}



void TIM1_UP_TIM10_IRQHandler(void)
{
	static uint16_t Count1;
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
			Count1++;
			if(Count1>=10)	// 速度环更新周期
			{
				Count1=0;
				
				// 读取编码器速度
				Speed_R=Encoder_Get_R();	//M3接口电机
				Speed_L=Encoder_Get_L();	//M4接口电机
				
				// 更新速度环PID
				InnerR.Actual=Speed_R;		//Actual实际值，将Speed值赋值给Inner.Actual结构体
				InnerL.Actual=Speed_L;		//Actual实际值，将Speed值赋值给Inner.Actual结构体
				
//				PID_Update(&InnerR);
//				PID_Update(&InnerL);
				
				// 设置电机速度
//				Motor_SetSpeed2(InnerL.Out);
//				Motor_SetSpeed1(InnerR.Out);
			}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}


