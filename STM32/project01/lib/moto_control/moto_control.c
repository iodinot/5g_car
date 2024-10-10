#include "moto_control.h"
#include "stdio.h"

/**��������ļ�**/

#define LENGTH_WHEEL								0.068f	//�����ܳ�����λm
#define LENGTH_FROM_WHEEL_TO_CENTER 10.0f		//���ӵ������м�ľ���
#define Hall_11											11 			//����������

//PID������
typedef struct
{
	float kp; 				//����ϵ��
	float ki; 				//����ϵ��
	float kd; 				//΢��ϵ��
	float error; 			//��ǰ���
	float L_error; 		//��һ�����
	float L_L_error;	//�ϴ���� - ��ǰ���
	float integral;		//������ֵ
	float out; 				//pid�������
}pidStructdef_l;
typedef struct
{
	float kp; 				//����ϵ��
	float ki; 				//����ϵ��
	float kd; 				//΢��ϵ��
	float error; 			//��ǰ���
	float L_error; 		//��һ�����
	float L_L_error;	//�ϴ���� - ��ǰ���
	float integral;		//������ֵ
	float out; 				//pid�������
}pidStructdef_r;

//����λ�û�����
static pidStructdef_l position_pid_list_l[]=
{
	4.3,2.1,1.5,0,0,0,0,
};
//����λ�û�����
static pidStructdef_r position_pid_list_r[]=
{
	4.3,2.2,1.7,0,0,0,0,
};

//�����ٶȻ�����
pidStructdef_l speed_pid_list_l[]=
{
	3,1,0,0,0,0,0,
};
//�����ٶȻ�����
pidStructdef_r speed_pid_list_r[]=
{
	10,5,0,0,0,0,0,
};

#if WHEELS==FOUR_WHEELS 
//static pidStructdef pid_list[]=
//{
//	1,0,0.1,0,0,0,0,
//	1,0,0.1,0,0,0,0,
//	1,0,0.1,0,0,0,0,
//	1,0,0.1,0,0,0,0,
//};


//static motoStructdef moto_list[]=
//{
//	0,&(TIM4->CCR1),
//	0,&(TIM4->CCR2),
//	0,&(TIM4->CCR3),
//	0,&(TIM4->CCR4),
//};


//float speed_destination[]= 
//{
//	0,
//	0,
//	0,
//	0,
//};

#elif WHEELS==THREE_WHEELS 

#endif

void Moto_forward_run()	//�������������ת
{
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_SET);
}

void Moto_back_run()
{
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_RESET);
}

void Moto_stop()
{
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_RESET);
}

void Moto_turn_left()
{
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_SET);
}


void Moto_turn_right()
{
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_RESET);
}

//���PID����

static float pid_cal_l(pidStructdef_l pid,float destination_l,float current_l,uint8_t mode)
{
	static float Integral_error_l;
	if(mode == SPEED)
	{
		pid.error = destination_l - current_l;
		pid.L_L_error = pid.error - pid.L_error;
				
		pid.out = pid.kp * pid.L_L_error + pid.ki * pid.error;//����ʽ
					
		pid.L_error = pid.error;
//		printf("pid.out=%f\r\n",pid.out);

		if(pid.out<0) pid.out=0;
		if(pid.out>1000) pid.out=1000;
		return pid.out;
	}
	else//POSITION
	{
		pid.error = destination_l - current_l;
		pid.L_L_error = pid.error - pid.L_error;
		Integral_error_l+=pid.error;
		
		pid.out = pid.kp * pid.error + pid.ki*Integral_error_l + pid.kd * pid.L_L_error;// λ��ʽ
			
		pid.L_error = pid.error;
//		printf("pid.out=%f\r\n",pid.out);
		
		if(pid.out<0) pid.out=0;
		if(pid.out>1000) pid.out=1000;
		return pid.out;
	}
}

static float pid_cal_r(pidStructdef_r pid,float destination,float current,uint8_t mode)
{
	static float Integral_error;
	if(mode == SPEED)
	{
		pid.error = destination - current;
		pid.L_L_error = pid.error - pid.L_error;
				
		pid.out = pid.kp * pid.L_L_error + pid.ki * pid.error;//����ʽ
					
		pid.L_error = pid.error;
//		printf("pid.out=%f\r\n",pid.out);

		if(pid.out<0) pid.out=0;
		if(pid.out>1000) pid.out=1000;
		return pid.out;
	}
	else//POSITION
	{
		pid.error = destination - current;
		pid.L_L_error = pid.error - pid.L_error;
		Integral_error+=pid.error;
		
		pid.out = pid.kp * pid.error + pid.ki*Integral_error + pid.kd * pid.L_L_error;// λ��ʽ

		pid.L_error = pid.error;
//		printf("pid.out=%f\r\n",pid.out);
		
		if(pid.out<0) pid.out=0;
		if(pid.out>1000) pid.out=1000;
		return pid.out;
	}
}

//���õ����Ӧ�Ŀ���PWM����
static void Set_TIM_PWM(EWHEEL wheel,uint16_t ccr_val)
{
	if(ccr_val>1000)
	{
		ccr_val=1000;
	}
	switch(wheel)
	{
		case LEFT_REAR:
//				printf("ccr_val_LEFT_REAR:%d \n",ccr_val);
				TIM4->CCR4 = ccr_val;
			break;
		
		case RIGHT_REAR:
//				printf("ccr_val_LEFT_REAR:%d \n",ccr_val);
				TIM4->CCR3 = ccr_val;
			break;
	
	}
	
}

//�ٶȿ��ƣ����ٶȻ�
static void speed_control_l(pidStructdef_l pid,float set_speed_l,float now_speed_l,EWHEEL mwheel)
{
	static float pid_cal_val_Left;
	pid_cal_val_Left = pid_cal_l(pid,set_speed_l,now_speed_l,POSITION);
	//		printf("set_speed=%f\r\n",set_speed);
	//		printf("now_speed=%f\r\n",now_speed);
	//printf("pid_cal_l=%f\r\n",pid_cal_val_Left);
	Set_TIM_PWM(mwheel,(uint16_t)pid_cal_val_Left);	
}

//�ٶȿ��ƣ����ٶȻ�
static void speed_control_r(pidStructdef_r pid,float set_speed_r,float now_speed_r,EWHEEL mwheel)
{
	static float pid_cal_val_Right;
	pid_cal_val_Right = pid_cal_r(pid,set_speed_r,now_speed_r,POSITION);
	//		printf("set_speed=%f\r\n",set_speed);
	//		printf("now_speed=%f\r\n",now_speed);
	//printf("pid_cal_r=%f\r\n",pid_cal_val_Right);
	Set_TIM_PWM(mwheel,(uint16_t)pid_cal_val_Right);	
}
#if USE_POSITION
//λ�ÿ��ƣ�����pid���ڻ��ٶȣ��⻷λ��
static void position_control(pidStructdef pid,float set_position,float now_position,float now_speed,EWHEEL mwheel)
{
	static float pid_cal_in_Left,pid_cal_out_Left,pid_cal_in_Right,pid_cal_out_Right;
	
	if(mwheel == LEFT_REAR)
	{
		pid_cal_in_Left = pid_cal(pid,set_position,now_position,POSITION);//�⻷����λ��
		
		pid_cal_out_Left += pid_cal(pid,pid_cal_in_Left,now_speed,POSITION);//�ڻ������ٶ�
		if(pid_cal_out_Left>0)
		{
			HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,SET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,RESET);
		}
		else
		{
			HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,RESET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,RESET);
		}
		Set_TIM_PWM(LEFT_REAR,(uint16_t)pid_cal_out_Left);	
	}
	else
	{
		pid_cal_in_Right = pid_cal(pid,set_position,now_position,POSITION);//�⻷����λ��
		
		pid_cal_out_Right += pid_cal(pid,pid_cal_in_Right,now_speed,POSITION);//�ڻ������ٶ�
		
		if(pid_cal_out_Left>0)
		{
			HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,RESET);
			HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,SET);
		}
		else
		{
			HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,RESET);
			HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,RESET);		
		
		}
		Set_TIM_PWM(RIGHT_REAR,(uint16_t)pid_cal_out_Right);		
	}


}
#endif

extern volatile encoderdef encoder_mlist[];//���ñ���������


void moto_control_l(uint8_t moto_id,float destination_l,float control_mode_l)
{
	if(control_mode_l == SPEED)
	{
		speed_control_l(position_pid_list_l[LEFT_REAR],destination_l,encoder_mlist[LEFT_REAR].speed,LEFT_REAR);
	printf("encoder_mlist[LEFT_REAR].speed:%f \n",encoder_mlist[LEFT_REAR].speed);
	}
	else
	{
		#ifdef USE_POSITION
	position_control(position_pid_list_l[LEFT_REAR],destination,encoder_mlist[LEFT_REAR].lenght,encoder_mlist[LEFT_REAR].speed,LEFT_REAR);
//	printf("encoder_mlist[LEFT_REAR].speed:%f \n",encoder_mlist[LEFT_REAR].speed);
		}	
		#endif
	}
}

void moto_control_r(uint8_t moto_id,float destination_r,float control_mode_r)
{
	if(control_mode_r == SPEED)
	{
		speed_control_r(position_pid_list_r[0],destination_r,encoder_mlist[RIGHT_REAR].speed,RIGHT_REAR);
//	printf("encoder_mlist[RIGHT_REAR].speed:%f \n",encoder_mlist[RIGHT_REAR].speed);
	}
	else
	{
		#ifdef USE_POSITION
		position_control(position_pid_list_r[0],destination,encoder_mlist[RIGHT_REAR].lenght,encoder_mlist[RIGHT_REAR].speed,RIGHT_REAR);
//	printf("encoder_mlist[RIGHT_REAR].speed%f \n",encoder_mlist[RIGHT_REAR].speed);
		}	
		#endif
	}
}

/**�ó�����ת�̶��Ƕ�***/
/**
angle : �Ƕ�
dire  :ת������
turn_speed : ת���ٶ�(0~70)
����ֵ��1���Ѿ����0��δ����

**/
/****/
float Nturns;//������Ҫת���ľ���
float Length_left,Length_right;//������Ҫת���ľ���

uint8_t set_turn_angle(float angle,float turn_speed,TURN_DIRE dire)
{
//	Nturns = ((LENGTH_FROM_WHEEL_TO_CENTER*angle*3.14)/(180*LENGTH_WHEEL))*3;//�������ӵ�Բ�ĵİ뾶���Լ����ýǶȣ����㳵����Ҫ�����ľ��루cm�� ����=�뾶*�Ƕ�
	
	Nturns = ((angle*3.14*LENGTH_FROM_WHEEL_TO_CENTER*2)/180);  //������ʽ: L=N*��*R/180  N�ǽǶ� R�ǰ뾶
	
	//��ȡʵʱ�߹��ľ���
	Length_left += (encoder_mlist[LEFT_REAR].speed/10*0.05);//ת�ٳ���ʱ������߹��ľ���
	Length_right += (encoder_mlist[RIGHT_REAR].speed/10*0.05);//0.05=50ms  10��ϵ�� ʹ���speed�ĵ�λΪm/s  ���� ȫ������1m/s
	printf("Length_left = %.3f\n", Length_left);
	if(Length_left>Nturns && Length_right>Nturns)//��ʵʱ�߹��ľ���ﵽ�˼���ľ�������������ͣ��
	{
		Length_left =0;
		Length_right =0;
		return 1;
	}
	else//������pid���Ƶ���ٶ�
	{
		car_speed_l=car_speed_r=turn_speed;	//�����ٶ�
		
		if(TURN_LEFT == dire)
		{
			Moto_turn_left();
		}
		else if(TURN_RIGHT == dire)
		{
			Moto_turn_right();
		}
		else
		{
			Moto_forward_run();
		}
		return 0;
	}
	HAL_Delay(50);
}
