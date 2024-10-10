/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
#include <stdarg.h>


int fputc(int ch, FILE *f)  //串口重定向
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF); 	
  	return ch;
}


/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}
/* USART6 init function */

void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */
	__HAL_UART_DISABLE_IT(&huart6, UART_IT_IDLE);
    __HAL_UART_CLEAR_IDLEFLAG(&huart6);
    __HAL_UART_CLEAR_FLAG(&huart6, UART_FLAG_TC);
    __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);
  /* USER CODE END USART6_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspInit 0 */

  /* USER CODE END USART6_MspInit 0 */
    /* USART6 clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART6 interrupt Init */
    HAL_NVIC_SetPriority(USART6_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspInit 1 */

  /* USER CODE END USART6_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspDeInit 0 */

  /* USER CODE END USART6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART6_CLK_DISABLE();

    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);

    /* USART6 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspDeInit 1 */

  /* USER CODE END USART6_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

int GetOpenmvDataCount  = 0;															//OpenMV数据读取位数
uint8_t  Num = 0, LoR = 0, Finded_flag = 0, FindTask = 0;	//识别的数字  识别数字的左右方向  是否识别到的数字  识别到的任务
uint8_t LastNum;
uint8_t NumCount = 0;
uint8_t sendBuf[5];					//串口发送的数据
uint8_t uart6_rxbuff;
uint8_t RoomNum, TargetNum;	//房间编号  任务编号
uint8_t TASK=1;    					//这个TASK可以传输给openmv，赋值openmv上的FindTask来控制openmv模板匹配的不同模式

char TargetRoom ='0';  //A, B, C, D, E, F, G, H;    //这八个字符对应着地图实际房间，里面的数字3—8会随机对应C-H

void SendDataToOpenmv(void)
{
	uint8_t i;
	if(LoR==0)
	//加上发送给openmv 的数据的代码 (帧头， 模板匹配模式选择标志位，模式2所需要匹配的数字，帧尾)   //不需要很高的发送频率			
		{
			for(i = 0; i <= 4; i++)   //将TASK和TargetNum打包一次性发送给openmv
			{ 
				sprintf((char *)sendBuf, "*%d%d&", TASK, TargetNum);
				HAL_UART_Transmit(&huart1, (uint8_t*)sendBuf, 4, 4);
			}
		}
		else
		{
			for(i = 0; i <= 4; i++)   //将TASK和TargetNum打包一次性发送给openmv
			{ 
				sprintf((char *)sendBuf, "*%d%d&", 0, TargetNum);
				HAL_UART_Transmit(&huart1, (uint8_t*)sendBuf, 4, 4);
			}
		}
}

//等待openmv识别指定数字,并设置目标房间。 只有在复位后
void SetTargetRoom(void)
{
		//查寻目标病房号的缓冲区数据，跳转任务开始函数   //重启后第一次从openmv传来的数字即为目标房号
		/*一开始识别目标房间号*/  
		if(Finded_flag == 1)
		{
			 RoomNum = Num;
		}
		
		
		else if(Finded_flag == 0)
		{
			RoomNum = 0;   
			LastNum = 0;
			NumCount = 0;
		}
	
		
	 if(RoomNum ==  1) 
	 {
		 TargetRoom = 'A';
		 TASK = 2;   
		 Flag_Stop=0;
	 }
	 else if(RoomNum == 2)
	 {
		 TargetRoom = 'B';
		 TASK = 2;  
		 Flag_Stop=0;
			
	 }
	 else if(RoomNum >= 3)  //不能else if(3 <= Num <= 8)
	 {
		 TargetRoom = 'G';
		 TASK = 2;  
		 Flag_Stop=0;
	 }

   switch(RoomNum)
		{
			case 1:
				TargetNum = 1;//保存房间号,在识别到数字时才会更新
			break;
			case 2:
				TargetNum = 2;
			break;
			case 3:
				TargetNum = 3;
			break;
			case 4:
				TargetNum = 4;
			break;
			case 5:
				TargetNum = 5;
			break;
			case 6:
				TargetNum = 6;
			break;
			case 7:
				TargetNum = 7;
			break;
			case 8:
				TargetNum = 8;
			break;	 	 		
		}
	 //识别到的数字是3-8， 默认先给  TargetRoom = RoomH
	 //根据openmv识别到的数据,在送药的函数里面进行目标值的实时更改
}



void Openmv_Receive_Data(uint8_t com_data)
{
		uint8_t i;
		static uint8_t RxCounter1=0;//计数
		static uint16_t RxBuffer1[10]={0};
		static uint8_t RxState = 0;	
		if(RxState==0&&com_data==0x2C)  //0x2c帧头
		{			
			RxState=1;
			RxBuffer1[RxCounter1++]=com_data;  
		}
		else if(RxState==1&&com_data==0x12)  //0x12帧头
		{
			RxState=2;
			RxBuffer1[RxCounter1++]=com_data;
		}		
		else if(RxState==2)
		{		 
			RxBuffer1[RxCounter1++]=com_data;
			if(RxCounter1>=10||com_data == 0x5B)       //RxBuffer1接受满了,接收数据结束
			{
				RxState=3;
				 //正常情况下,运行到这RxCounter1 == 7？  7-5 = 2    openmv发送过来的一个数据包有8个
				Num =          RxBuffer1[RxCounter1-5]; 
				
				OLED_ShowNumber(35,6, Num,3,4); 
				
				LoR =          RxBuffer1[RxCounter1-4];	//-1是左， 1是右，0表示还没有识别到任何数字
				Finded_flag =  RxBuffer1[RxCounter1-3];	//是否识别到数字的标志位
				FindTask =      RxBuffer1[RxCounter1-2];		
				//RxCounter1-1是帧尾				
		  	GetOpenmvDataCount++;      
				//用来看1秒内成功解码多少个数据包的 需要在1s钟的延时中清除，帧率越高越准确，个位数的话偏差就大了
				//不如改一下解码代码，将openmv那里的帧率直接传过来
				if(have==0)have=1;				
			}
		}
		else if(RxState==3)		//检测是否接受到结束标志
		{
				if(RxBuffer1[RxCounter1-1] == 0x5B)
				{
							RxCounter1 = 0;
							RxState = 0;
						
				}
				else   //接收错误
				{
							RxState = 0;
							RxCounter1=0;
							for(i=0;i<10;i++)
							{
									RxBuffer1[i]=0x00;      //将存放数据数组清零
							}
				}
		} 
		else   //接收异常
		{
				RxState = 0;
				RxCounter1=0;
				for(i=0;i<10;i++)
				{
						RxBuffer1[i]=0x00;      //将存放数据数组清零
				}
		}
}
uint8_t Openmv_Receive_Buffer[1];
uint8_t have;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)		//????
	{
		Openmv_Receive_Data(Openmv_Receive_Buffer[0]);
		HAL_UART_Receive_IT(&huart1, &Openmv_Receive_Buffer[0], 1);
	}
}

/* USER CODE END 1 */
