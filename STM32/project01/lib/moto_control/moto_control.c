#include "moto_control.h"
#include "stdio.h"

/**电机控制文件**/

#define LENGTH_WHEEL								0.068f	//轮子周长，单位m
#define LENGTH_FROM_WHEEL_TO_CENTER 10.0f		//轮子到两轮中间的距离
#define Hall_11											11 			//编码器精度

//PID参数表
typedef struct
{
	float kp; 				//比例系数
	float ki; 				//积分系数
	float kd; 				//微分系数
	float error; 			//当前误差
	float L_error; 		//上一次误差
	float L_L_error;	//上次误差 - 当前误差
	float integral;		//误差积分值
	float out; 				//pid计算输出
}pidStructdef_l;
typedef struct
{
	float kp; 				//比例系数
	float ki; 				//积分系数
	float kd; 				//微分系数
	float error; 			//当前误差
	float L_error; 		//上一次误差
	float L_L_error;	//上次误差 - 当前误差
	float integral;		//误差积分值
	float out; 				//pid计算输出
}pidStructdef_r;

//左轮位置环控制
static pidStructdef_l position_pid_list_l[]=
{
	4.3,2.1,1.5,0,0,0,0,
};
//右轮位置环控制
static pidStructdef_r position_pid_list_r[]=
{
	4.3,2.2,1.7,0,0,0,0,
};

//左轮速度环控制
pidStructdef_l speed_pid_list_l[]=
{
	3,1,0,0,0,0,0,
};
//右轮速度环控制
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

void Moto_forward_run()	//控制两个电机正转
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

//电机PID计算

static float pid_cal_l(pidStructdef_l pid,float destination_l,float current_l,uint8_t mode)
{
	static float Integral_error_l;
	if(mode == SPEED)
	{
		pid.error = destination_l - current_l;
		pid.L_L_error = pid.error - pid.L_error;
				
		pid.out = pid.kp * pid.L_L_error + pid.ki * pid.error;//增量式
					
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
		
		pid.out = pid.kp * pid.error + pid.ki*Integral_error_l + pid.kd * pid.L_L_error;// 位置式
			
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
				
		pid.out = pid.kp * pid.L_L_error + pid.ki * pid.error;//增量式
					
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
		
		pid.out = pid.kp * pid.error + pid.ki*Integral_error + pid.kd * pid.L_L_error;// 位置式

		pid.L_error = pid.error;
//		printf("pid.out=%f\r\n",pid.out);
		
		if(pid.out<0) pid.out=0;
		if(pid.out>1000) pid.out=1000;
		return pid.out;
	}
}

//设置电机相应的控制PWM函数
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

//速度控制，单速度环
static void speed_control_l(pidStructdef_l pid,float set_speed_l,float now_speed_l,EWHEEL mwheel)
{
	static float pid_cal_val_Left;
	pid_cal_val_Left = pid_cal_l(pid,set_speed_l,now_speed_l,POSITION);
	//		printf("set_speed=%f\r\n",set_speed);
	//		printf("now_speed=%f\r\n",now_speed);
	//printf("pid_cal_l=%f\r\n",pid_cal_val_Left);
	Set_TIM_PWM(mwheel,(uint16_t)pid_cal_val_Left);	
}

//速度控制，单速度环
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
//位置控制，串级pid，内环速度，外环位置
static void position_control(pidStructdef pid,float set_position,float now_position,float now_speed,EWHEEL mwheel)
{
	static float pid_cal_in_Left,pid_cal_out_Left,pid_cal_in_Right,pid_cal_out_Right;
	
	if(mwheel == LEFT_REAR)
	{
		pid_cal_in_Left = pid_cal(pid,set_position,now_position,POSITION);//外环计算位置
		
		pid_cal_out_Left += pid_cal(pid,pid_cal_in_Left,now_speed,POSITION);//内环计算速度
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
		pid_cal_in_Right = pid_cal(pid,set_position,now_position,POSITION);//外环计算位置
		
		pid_cal_out_Right += pid_cal(pid,pid_cal_in_Right,now_speed,POSITION);//内环计算速度
		
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

extern volatile encoderdef encoder_mlist[];//引用编码器对象


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

/**让车子旋转固定角度***/
/**
angle : 角度
dire  :转动方向
turn_speed : 转动速度(0~70)
返回值：1：已经到达，0：未到达

**/
/****/
float Nturns;//轮子需要转动的距离
float Length_left,Length_right;//轮子需要转动的距离

uint8_t set_turn_angle(float angle,float turn_speed,TURN_DIRE dire)
{
//	Nturns = ((LENGTH_FROM_WHEEL_TO_CENTER*angle*3.14)/(180*LENGTH_WHEEL))*3;//根据轮子到圆心的半径，以及设置角度，计算车轮需要滚动的距离（cm） 弧长=半径*角度
	
	Nturns = ((angle*3.14*LENGTH_FROM_WHEEL_TO_CENTER*2)/180);  //弧长公式: L=N*Π*R/180  N是角度 R是半径
	
	//获取实时走过的距离
	Length_left += (encoder_mlist[LEFT_REAR].speed/10*0.05);//转速乘以时间就是走过的距离
	Length_right += (encoder_mlist[RIGHT_REAR].speed/10*0.05);//0.05=50ms  10是系数 使电机speed的单位为m/s  所以 全速运行1m/s
	printf("Length_left = %.3f\n", Length_left);
	if(Length_left>Nturns && Length_right>Nturns)//若实时走过的距离达到了计算的距离则满足条件停下
	{
		Length_left =0;
		Length_right =0;
		return 1;
	}
	else//否则则pid控制电机速度
	{
		car_speed_l=car_speed_r=turn_speed;	//更新速度
		
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
