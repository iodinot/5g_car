/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define open 0 
#define close 1 
typedef enum
{
	left_90,
	right_90,
	back_180,
	straight,
	fianl,
	ojbk
}spin_dir_t;	//转弯函数的输入参数

uint8_t Spin_start_flag, Spin_succeed_flag, Stop_Flag, Flag_Stop = 1;//旋转开始标志位  旋转成功标志位  路口停止标志位 默认停止标志位
uint8_t Do_count = 0, Do2_count = 0;	//任务计数
__IO uint16_t SendTime = 0;						//电机PID计算时间调整
float Velocity,Turn;									//速度  转向状态
uint8_t car_speed_l=0,car_speed_r=0;	//设置电机的速度
char show[10];												//储存OLED需要显示的字符串
extern uint8_t LED;
extern uint8_t intersection;					//路口识别标志位
extern uint8_t LoadCount,NotLoadCount;//装载药品时间计数  拿走药品时间计数
extern uint16_t lukou_straight; 
void Find(void);											//巡线函数
void spin_Turn(spin_dir_t zhuanxiang);//转弯函数
char Load_mode;												//药品装载标志位
uint8_t key_num=0;										//按键扫描标志位

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM10_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */

	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);	
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);	
	TIM1->CCR1  =  200;
	HAL_UART_Receive_IT(&huart1, &Openmv_Receive_Buffer[0], 1);
	
	HAL_Delay(1000);	//等待系统初始化
	OLED_Init();
	HAL_Delay(500);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
#if 0
		OLED_ShowString(0,3,(unsigned char *)"111111"); 
	while(1)
	{
		Moto_forward_run();
		car_speed_l = car_speed_r = 15;
		OLED_ShowString(0,1,(unsigned char *)"test"); 
		HAL_Delay(100);
	}
	#endif
	
	OLED_Clear();
  while (1)
	{
		if(SendTime >= 10)	//SendTime每10ms +1
		{
			printf("test");
			SendTime = 0;
			SendDataToOpenmv();						//不能太快，否则会超过openmv的串口接收数据缓冲区
			if(Load_mode==0) LoadOrNot();	//判断药物是否装载
			oled_show();									//显示屏打开
		}
		if(Load_flag == 1&&TASK == 1) 	//选择任务一 || 药物已经放上  
		{
			SetTargetRoom();							//设置识别到的房间号
		}
		if(TASK == 2)    
	 	{
			OLED_Clear();
			oled_show();							//OLED刷新
			if(Load_flag == 1)    		//运送药物
			{
				if(TargetRoom == 'A')   //这个位置理解为A房，数字为1
				{
					switch(Do_count)
					{
						case 0:
						{
							Stop_Flag = 0;							//巡线停车标志位
							Spin_succeed_flag = 0;			//转向成功标志位
							while(Stop_Flag==0)Find();	//巡线(此时 Stop_Flag ==0  完成后自动置1)
							Do_count++;	
						}
						break;
						
						case 1:
						{
							if(Stop_Flag == 1)		//确保此时巡线关闭
							{
								spin_Turn(left_90);	//左转  此时Spin_succeed_flag== 0 ,完成后自动置1
								Do_count++;
							}
						}
						break;
						
						case 2:
						{
							if(Spin_succeed_flag == 1)	//上一步旋转完毕Spin_succeed_flag会置1
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag==0) Find();
								Do_count++;
							}
						}
						break;
							
						case 3:
						{
							if(Stop_Flag == 1)	//到终点
							{							
								spin_Turn(fianl);	//前进一段距离
							}
						}
						break;
					}
				}
				else if(TargetRoom == 'B')	//B号房
				{
					switch(Do_count)
					{
						case 0:
						{
							Spin_succeed_flag = 0;
							Stop_Flag = 0;
							while(Stop_Flag == 0) Find();  						
							Do_count++;	
						}
						break;
						
						case 1:
						{
							if(Stop_Flag ==1)
								spin_Turn(right_90);
							
							if(Spin_succeed_flag == 1)	
								Do_count++;
						}
						break;
							
						case 2:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								spin_Turn(fianl);
								Do_count++;
							}
						}
						break;
						
						case 3:
						{
							if(Spin_succeed_flag ==1)
							{
								car_speed_l = car_speed_r = 0;
								Flag_Stop=1;
							}
						}
						break;
					}			 
				}
				//中端病房和远端病房需要发送特定标志位					
				else      //TargetRoom == 'C' || TargetRoom == 'D' || TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H'
				{
					OLED_ShowNumber(10,3, Do_count,3,4);
					switch(Do_count)	//直走到第一个路口
					{
						case 0: 
						{
							Spin_succeed_flag = 0;
							Stop_Flag = 0;
							while(Stop_Flag == 0) Find();
							
							Do_count++;
							SendDataToOpenmv();  //打开数字识别
						}
						break;
						
						case 1:
						{
							if(Stop_Flag ==1)				//遇到第一个路口
							 {
								 spin_Turn(straight);	//此时Spin_succeed_flag== 0 ,完成后自动置1
								 Do_count++;
								 SendDataToOpenmv();	//打开模板匹配数字
							 }
						 }
						
						case 2:
						{
							if(Spin_succeed_flag == 1)	//第一个路口往第二个路口直走
							{
								Spin_succeed_flag = 0;
								lukou_straight=1;
								Stop_Flag = 0;
								while(lukou_straight <= 120) Find();//lukou_straight 每10ms +1
								
								//巡线1.2s后停下1.5s识别数字,再继续巡线到下一个路口
								lukou_straight=0;
								Turn=0;
								Moto_stop();
								car_speed_l=car_speed_r=0;
								HAL_Delay(1000);
								HAL_Delay(500);
								Moto_forward_run();
								
								intersection = 0;
								Stop_Flag = 0;
								Spin_succeed_flag = 0;
								while(Stop_Flag == 0) Find();
								Do_count++;
							 }
						 }
						
						case 3:	//第二个路口		判定是否识别到数字，且该数字的位置偏左还是偏右
						{
							if(Stop_Flag == 1)
							{
								if(RoomNum == TargetNum && LoR == 1)	//识别到了目标数字, 且其位置偏左  
								{
									Do_count = 8; 											//后续直至左转并直行到病房门口的函数
									TargetRoom = 'C';
								}
								else if(RoomNum == TargetNum && LoR == 2)	//识别到目标数字, 且其位置偏右
								{
									Do_count = 8;			//后续直至右转并直行到病房门口的函数
									TargetRoom = 'D';
								}	
								if(LoR == 0)		//没识别到就直走到第三个路口停止
								{
									spin_Turn(straight);
									lukou_straight=1;
									while(lukou_straight <= 150) Find();
									
									lukou_straight=0;
									Moto_stop();
									car_speed_l=car_speed_r=0;
									HAL_Delay(1000);
									HAL_Delay(500);
									Moto_forward_run();
									
									intersection = 0;
									Stop_Flag = 0;
									Spin_succeed_flag = 0;
									while(Stop_Flag == 0) Find();
									Do_count++;
								}	
							}
						}
						break;
								
						case 4:	//第三个路口准备转弯
						{
							if(Stop_Flag == 1)
							{
								if(RoomNum == TargetNum && LoR == 1)  //识别到了目标数字, 且其位置偏左  
								{
									Do_count++; 
									TargetRoom = 'E';		//先假定是E
									SendDataToOpenmv(); //关闭模板匹配数字
								}
								else if(RoomNum == TargetNum && LoR == 2 )  //识别到目标数字, 且其位置偏右
								{
									Do_count++;
									TargetRoom = 'G';		//先假定是G
									SendDataToOpenmv(); //关闭模板匹配数字	
								}
							}
						}
						break;
								
						case 5://转弯
						{
							if(Stop_Flag == 1)
							{
								Do_count++;
								if(TargetRoom == 'E')
								{
									spin_Turn(left_90);
									LoR = 0;						//清空预转弯方向
									SendDataToOpenmv(); //打开模板匹配数字
								}
								else if(TargetRoom == 'G')
								{
									spin_Turn(right_90);
									LoR = 0;
									SendDataToOpenmv();
								}
							}
						}
						break;
						
						case 6: //直走到第四个路口
						{
							if(Spin_succeed_flag == 1)
							{
								Do_count++;
								Stop_Flag = 0;	
								lukou_straight=1;
								while(lukou_straight <= 200) Find(); 
								
								lukou_straight=0;
								Moto_stop();
								car_speed_l=car_speed_r=0;
								HAL_Delay(1000);
								HAL_Delay(500);
								Moto_forward_run();
								
								intersection = 0;
								Stop_Flag = 0;
								Spin_succeed_flag = 0;
								while(Stop_Flag == 0) Find();
							}
						}
						break;
						
						case 7: //第四个路口准备转弯
						{
							if(Stop_Flag == 1)
							{
								if(TargetRoom == 'E')
								{
									if(RoomNum == TargetNum && LoR == 1)    //识别到RoomNum, 且其位置偏左    
									{
										Do_count ++;  
										TargetRoom = 'E';
										SendDataToOpenmv(); //关闭模板匹配数字	
									}										 
									else if(RoomNum == TargetNum && LoR == 2)   //一定时间内识别到RoomNum, 且其位置偏右
									{
										Do_count++;  
										TargetRoom = 'F';
										SendDataToOpenmv(); //关闭模板匹配数字	
									}
									else
									{
										Velocity=0;
										Turn=0;
										Flag_Stop=1;
									}
								}
								else if(TargetRoom == 'G')
								{
									if(RoomNum == TargetNum && LoR == 1)    //一定时间内识别到RoomNum, 且其位置偏左    
									{
										Do_count++; 
										TargetRoom = 'G';
										SendDataToOpenmv(); //关闭模板匹配数字	
									}										 											 
									else if(RoomNum == TargetNum && LoR == 2)   //一定时间内识别到RoomNum, 且其位置偏右
									{
										Do_count++;  
										TargetRoom = 'H';
										SendDataToOpenmv(); //关闭模板匹配数字	
									}
									else
									{
										Velocity=0;
										Turn=0;
										Flag_Stop=1;
									}
								}	
							}
						}
						break;
								
						case 8://最后路口转弯
						{
							if(Stop_Flag == 1)
							{
								if(TargetRoom == 'C' || TargetRoom == 'E' || TargetRoom == 'G')
								{
									spin_Turn(left_90);
									Do_count++;
								}
								else if(TargetRoom == 'D' || TargetRoom == 'F' || TargetRoom == 'H')
								{
									spin_Turn(right_90);
									Do_count++;
								}
							}
						}
						break;
								
						case 9: //直走到终点
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								
								spin_Turn(fianl);
								Do_count++;
							}
						}
						break;
						
						case 10:
						{
							if(Spin_succeed_flag == 1) //送完药
							{
								Velocity=0;
								Turn=0;
								Flag_Stop=1;
							}
						}
						break;								
					}
				}
			}
			/**********************************送药完毕返回**********************************/
			else if(Load_flag == 2)		// 全部房间都可返回了，就是远端的三叉路口那可能有时候会出现问题。
			{
				Flag_Stop=0;
				if(TargetRoom == 'A')
				{
					switch(Do2_count)
					{
						case 0:
						{
							spin_Turn(back_180);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1) //转弯成功
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								Do2_count++;	
							}
						}
						break;
							
						case 2:
						{
							if(Stop_Flag ==1)
							{
								spin_Turn(right_90);   //此时Spin_succeed_flag== 0 ,完成后自动置1
								Do2_count++;
							}
						}
						break;
							 
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								spin_Turn(ojbk);
								Do2_count++;
							}
						}
						break;
						
						case 4:
						{
							if(Spin_succeed_flag == 1)
							{
								//清空标志位,停下小车
								Velocity=0;
								Turn=0;
								Flag_Stop=1;
								Moto_stop();
								car_speed_l=car_speed_r=0;
							}
						}
						break;
					}
				}
				else if(TargetRoom == 'B')
				{
					switch(Do2_count)
					{
						case 0:
						{
							spin_Turn(back_180);
							Do2_count++;	
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{ 
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								HAL_Delay(100);
								while(Stop_Flag == 0) Find();
								Do2_count++;
							}
						}
						break;
						
						case 2:
						{
							 if(Stop_Flag ==1)
							 {
								 spin_Turn(left_90); 	
								 Do2_count++;
							 }
						}
						break;
						
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								spin_Turn(ojbk);
								Do2_count++;
							}
						}
						break;
						
						case 4:
						{
							if(Spin_succeed_flag == 1)
							{
								Velocity=0;
								Turn=0;
								Flag_Stop=1;
								Moto_stop();
								car_speed_l=car_speed_r=0;
								LED(open);
							}
						}
						break;
					}
				}
				else if(TargetRoom == 'C')   //中端病房   这两个与近端病房的差别只是在最后直走的时候走多了几十cm而已
				{
					switch(Do2_count)
					{
						case 0:
						{
							spin_Turn(back_180);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{		 
								Stop_Flag = 0;	
								while(Stop_Flag==0)Find();
								Do2_count++;
							}
						}
						break;	
						
						case 2:
						{
							if(Stop_Flag ==1)
							{
								spin_Turn(right_90 );   //此时Spin_succeed_flag== 0 ,完成后自动置1
								Spin_succeed_flag=1;
								Do2_count++;
							}
						}
						break;
						
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								
								spin_Turn(straight);
								Do2_count++;
								Stop_Flag = 1;	
							}
						}
						break;	
						 
						case 4:
						{
							if(Stop_Flag ==1)
							{
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								Do2_count++;
							}
						}
						break;

						case 5:
						{
							Stop_Flag = 0;	
							while	(Stop_Flag == 0) Find();
								
							Stop_Flag = 0;	
							while	(Stop_Flag == 0) Find();
								
							spin_Turn(ojbk);
							Do2_count++;
						}
						break;
							
						case 6:
						{
							Velocity=0;
							Turn=0;
							Flag_Stop=1;
							Moto_stop();
							car_speed_l = car_speed_r = 0;
						}
						break;
					}
				}
				else if(TargetRoom == 'D')
				{
					switch(Do2_count)
					{
						case 0:
						{
							spin_Turn(back_180);
							Do2_count++;	
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								HAL_Delay(100);
								while(Stop_Flag == 0) Find();    
								Do2_count++;
							}
						}
						break;
							
						case 2:
						{
							if(Stop_Flag ==1)
							{
								spin_Turn(left_90);   //此时Spin_succeed_flag== 0 ,完成后自动置1
								Do2_count++;
							}
						}
						break;

						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								HAL_Delay(100);
								while(Stop_Flag == 0) Find();
								Do2_count++;
							}
						}
						break;
							
						case 4:
						{
							if(Stop_Flag ==1)
							{
								spin_Turn(straight);   //此时Spin_succeed_flag== 0 ,完成后自动置1
								Do2_count++;
							}
						}
						break;

						case 5:
						{
							if(Spin_succeed_flag ==1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								HAL_Delay(100);
								while(Stop_Flag == 0) Find();
								Do2_count++;
							}
						}
						break;
							
						case 6:
						{
							if(Stop_Flag ==1)
							{
								spin_Turn(ojbk);   //此时Spin_succeed_flag== 0 ,完成后自动置1
								Flag_Stop=1;
								Moto_stop();
								car_speed_l=car_speed_r=0;
							}
						}
						break;	
					}
				}
				else if(TargetRoom == 'E')   //远端病房   四个只是在两个路口的转向不同而已
				{
					switch(Do2_count)
					{
						case 0:
						{
							spin_Turn(back_180);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								Do2_count++;	
							}
						}
						break;
						
						case 2:
						{
							if(Stop_Flag ==1)
							{
								spin_Turn(right_90);   //此时Spin_succeed_flag== 0 ,完成后自动置1
								Do2_count++;
							}
						}
						break;
						
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								Do2_count++;
							}
						}
						break;
						
						case 4:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(right_90);
								Do2_count++;
							}
						}
						break;
							
						case 5:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								
								spin_Turn(straight);
								Stop_Flag = 1;
								Do2_count++;
							}
						}
						break;
							
						case 6:
						{
							if(Stop_Flag ==1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								
								spin_Turn(straight);
								Do2_count++;
							}
						}
						break;
						
						case 7:
						{
							Spin_succeed_flag = 0;
							Stop_Flag = 0;	
							while(Stop_Flag == 0) Find();
							Do2_count++;
						}
						break;
							
						case 8:
						{
							Spin_succeed_flag = 0;
							Stop_Flag = 0;
							while(Stop_Flag == 0) Find();
							
							spin_Turn(ojbk);
							Do2_count++;
						}
						break;

						case 9:
						{
							Velocity=0;
							Turn=0;
							Flag_Stop=1;
							Moto_stop();
							car_speed_l=car_speed_r=0;
						}
						break;	
					}
				}
				
				else if(TargetRoom == 'F')
				{
					switch(Do2_count)
					{
						case 0:
						{
							spin_Turn(back_180);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								Do2_count++;   
							}
						}
						break;
							
						case 2:
						{
							if(Stop_Flag ==1)
							{
								spin_Turn(left_90);   //此时Spin_succeed_flag== 0 ,完成后自动置1
								Do2_count++;
							}
						}
						break;
							
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								Do2_count++;
							}
						}
						break;
							
						case 4:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(right_90);
								Do2_count++;
							}
						}
						break;
							
						case 5:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								
								spin_Turn(straight);
								Stop_Flag = 1;
								Do2_count++;
							}
						}
						break;
						
						case 6:
						{
							if(Stop_Flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								
								spin_Turn(straight);
								Do2_count++;
							}
						}
						break;
							
						case 7:
						{
							Spin_succeed_flag = 0;
							Stop_Flag = 0;
							while(Stop_Flag == 0) Find();
							Do2_count++;
						}
						break;
						
						case 8:
						{
							Spin_succeed_flag = 0;
							Stop_Flag = 0;
							while(Stop_Flag == 0) Find();
							
							spin_Turn(ojbk);
							Do2_count++;
						}
						break;
						
						case 9:
						{
							Velocity=0;
							Turn=0;
							Flag_Stop=1;
							Moto_stop();
							car_speed_l=car_speed_r=0;
						}
						break;
					}
				}
				
				else if(TargetRoom == 'G')
				{
					switch(Do2_count)
					{
						case 0:
						{
							spin_Turn(back_180);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								Do2_count++;	  
							}
						}
						break;
							
						case 2:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(right_90);   //此时Spin_succeed_flag== 0 ,完成后自动置1
								Do2_count++;
							}
						}
						break;
							 
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								Do2_count++;
							}
						}
						break;
							
						case 4:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(left_90);
								Do2_count++;
							}
						}
						break;
							
						case 5:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								
								spin_Turn(straight);
								Stop_Flag = 1;
								Do2_count++;
							}
						}
						break;
							
						case 6:
						{
							if(Stop_Flag == 1)
							{
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								spin_Turn(straight);
								Do2_count++;
							}
						}
						break;
							
						case 7:
						{
							Spin_succeed_flag = 0;
							Stop_Flag = 0;
							while(Stop_Flag == 0) Find();
							Do2_count++;
						}
						break;
							
						case 8:
						{
							Spin_succeed_flag = 0;
							Stop_Flag = 0;
							while(Stop_Flag == 0) Find();
							
							spin_Turn(ojbk);
							Do2_count++;
						}
						break;
							
						case 9:
						{
							Velocity=0;
							Turn=0;
							Flag_Stop=1;
							Moto_stop();
							car_speed_l=car_speed_r=0;
						}
						break;
					}
				}
				
				else if(TargetRoom == 'H')
				{
					switch(Do2_count)
					{
						case 0:
						{
							spin_Turn(back_180);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								Do2_count++;
							}
						}
						break;
							
						case 2:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(left_90);   //此时Spin_succeed_flag== 0 ,完成后自动置1
								Do2_count++;
							}
						}
						break;
							 
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								Do2_count++;
							}
						}
						break;
							
						case 4:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(left_90);
								Do2_count++;
							}
						}
						break;
							
						case 5:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								
								spin_Turn(straight);
								Do2_count++;
								Stop_Flag = 1;	
							}
						}
						break;
						
						case 6:
						if(Stop_Flag == 1)
						{
							Spin_succeed_flag = 0;
							Stop_Flag = 0;	
							while(Stop_Flag == 0) Find();
							spin_Turn(straight);
							Do2_count++;
						}
						break;
						
						case 7:
						{
							Spin_succeed_flag = 0;
							Stop_Flag = 0;
							while(Stop_Flag == 0) Find();
							Do2_count++;
						}
						break;
						
						case 8:
						{
							Spin_succeed_flag = 0;
							Stop_Flag = 0;
							while(Stop_Flag == 0) Find();
							
							spin_Turn(ojbk);
							Do2_count++;
						}
						break;
							
						case 9:
						{
							Velocity=0;
							Turn=0;
							Flag_Stop=1;
							Moto_stop();
							car_speed_l=car_speed_r=0;
						}
						break;	
					}
				}
			}
		}
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
extern uint8_t intersection;
void Find(void)
{
	Spin_start_flag = 0;			//旋转开始标志位
	Spin_succeed_flag = 0;		//旋转成功标志位
	Find_Line_Begins();				//开始巡线
	if (intersection == 1)
	{
		Stop_Flag = 1;					//遇到路口停止
		intersection = 0;
		car_speed_l = car_speed_r = 0;
	}
}

/******转向完成时  Spin_start_flag == 0 && Spin_succeed_flag == 1  ********/
/*转角有左转90，右转90，和转180三种情况。*/
void spin_Turn(spin_dir_t zhuanxiang) //其实转向环并不需要很精准，转弯后直接用直走时的巡线函数回正车身就好
{
	Stop_Flag = 0;  //执行转弯时，将直走完成的标志位清零. 即如果上一次是直行，这次是转弯，则不用在业务代码里手动置位

	Spin_start_flag = 1;
	Spin_succeed_flag = 0;

	/**************要是转弯后不能回到线上，可以改这里的转向系数**************/
	if (zhuanxiang == left_90)  //openmv识别到需要往左边病房走
	{
		Moto_forward_run();
		car_speed_l=car_speed_r=15;
		HAL_Delay(780);		//越长时间就前进越多
		Moto_turn_left();
		HAL_Delay(740);		//越长时间就转越多
		car_speed_l=car_speed_r=0;
	}
	else if (zhuanxiang == right_90)  //openmv识别到需要往右边病房走
	{
		Moto_forward_run();
		car_speed_l=car_speed_r=15;
		HAL_Delay(900);
		Moto_turn_right();
		HAL_Delay(820);
		car_speed_l=car_speed_r=0;
	}
	else if (zhuanxiang == back_180)
	{
		Moto_forward_run();
		car_speed_l=car_speed_r=15;
		HAL_Delay(400);
		Moto_turn_right();
		HAL_Delay(1000);HAL_Delay(680);
	}

	else if (zhuanxiang == straight)
	{
		Moto_forward_run();
		car_speed_l=car_speed_r=15;
		HAL_Delay(500);
	}
	else if (zhuanxiang == fianl)
	{
		Moto_forward_run();
		car_speed_l=car_speed_r=15;
		HAL_Delay(800);
		car_speed_l=car_speed_r=0;
		Flag_Stop=1;
		Load_mode=0;
	}
	else if (zhuanxiang == ojbk)
	{
		Moto_forward_run();
		car_speed_l=car_speed_r=15;
		HAL_Delay(1000);HAL_Delay(300);
	}
	Spin_succeed_flag = 1;
}

uint8_t LoadCount,NotLoadCount;
uint8_t Load_flag = 0;  //0表示还没转载药品，1表示转载完毕，2表示药品送完
extern char Load_mode;
void LoadOrNot()
{
	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==0)
	{
		NotLoadCount = 0;
		if(Load_flag==0)
		{
			LoadCount++;
			if(LoadCount > 3) //3次程序执行到这之后再次判断
			{
				Load_flag = 1;
				Load_mode=1;
			}
		}
	}
	else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==1)
	{
		LoadCount = 0;
		
		if(Load_flag == 1)			//必须先装载过药品的情况下才能判断是否拿走药品
		{
			NotLoadCount++;
			if(NotLoadCount > 3)	//3次程序执行到这之后再次判断
			{
				Load_flag = 2;
			}
		}
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
