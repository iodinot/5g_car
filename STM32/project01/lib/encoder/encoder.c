#include "encoder.h"
#include "stdio.h"
/**����������ɼ��ļ�**/


#define CYCLE_10MS 0.01f //�������ھ���
#define Hall_11   	11 //����������


#if WHEELS==FOUR_WHEELS 

volatile encoderdef encoder_mlist[]=
{
	SPEED_IN1_Pin,0,0,CYCLE_1MS,0,
	SPEED_IN2_Pin,0,0,CYCLE_1MS,0,
	SPEED_IN3_Pin,0,0,CYCLE_1MS,0,
	SPEED_IN4_Pin,0,0,CYCLE_1MS,0,
};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		switch(GPIO_Pin)
		{
			case SPEED_IN1_Pin:	
				encoder_mlist[LEFT_REAR].count++;	
				break;
			case SPEED_IN2_Pin:	
				encoder_mlist[RIGHT_REAR].count++;	
				break;
			case SPEED_IN3_Pin:	
				encoder_mlist[LEFT_FRONT].count++;	
				break;
			case SPEED_IN4_Pin:	
				encoder_mlist[RIGHT_FRONT].count++;	
				break;	
			default:
				break;
		}
}

#elif WHEELS==THREE_WHEELS 


//���������ݱ�
volatile encoderdef encoder_mlist[]=
{
	SPEED_IN1_Pin,0,0,CYCLE_10MS,0,0,
	SPEED_IN2_Pin,0,0,CYCLE_10MS,0,0,
};


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//���ټ����жϣ������������α仯����
{
	for(uint8_t i=0;i<sizeof(encoder_mlist)/sizeof(encoder_mlist[0]);i++)
	{
		if(GPIO_Pin == encoder_mlist[i].wheel  && (HAL_GPIO_ReadPin(GPIOB, GPIO_Pin)==1))
		{
			encoder_mlist[i].count++;	
//			printf("Interrupt triggered on pin: %d\r\n", GPIO_Pin);
		}	
	}
}

#endif
extern __IO uint16_t SendTime;
extern uint8_t car_speed_l,car_speed_r;
uint8_t count=0;
uint16_t lukou_straight = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//��ʱ�����ڣ�10ms����ȡ����ֵ�������㵱ǰ�ٶ�
{
	
	if(htim==&htim10)
	{
		for(uint8_t i=0;i<sizeof(encoder_mlist)/sizeof(encoder_mlist[0]);i++)
		{			
			encoder_mlist[i].speed = (float)(encoder_mlist[i].count/(encoder_mlist[i].cycle*Hall_11));//����ʵʱ�ٶ�  speed������ת��  speed:0-60 
			encoder_mlist[i].count = 0;
		}
	}
	if(htim==&htim9)
	{
		count++;
		SendTime++;
		if(lukou_straight>=1) lukou_straight++;
		if(count>10)	//100ms����һ��
		{	
			count=0;
			moto_control_l(LEFT_REAR,car_speed_l,SPEED);	//���������ٶ�
			moto_control_r(RIGHT_REAR,car_speed_r,SPEED);
		}
	}
}



