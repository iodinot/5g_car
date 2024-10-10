#ifndef __ENCODER_H
#define __ENCODER_H

#include "../../Core/Inc/main.h"



#if WHEELS==FOUR_WHEELS //四轮方位定义


/** LEFT_REAR:左后电机  
	RIGHT_REAR：右后电机
	LEFT_FRONT：左前电机
	RIGHT_FRONT：右前电机
**/
typedef enum//电机轮子枚举变量
{
	LEFT_REAR,
	RIGHT_REAR,
	LEFT_FRONT,
	RIGHT_FRONT,
	
}ewheel;

#elif WHEELS==THREE_WHEELS //三轮方位定义


/** LEFT_REAR:左电机  
	RIGHT_REAR：右电机
**/
typedef enum //电机轮子枚举变量
{
	LEFT_REAR = 0,
	RIGHT_REAR = 1,
}EWHEEL;
#else

typedef enum
{
	LEFT_REAR,
	RIGHT_REAR,
}ewheel;

#endif

typedef struct
{
	uint16_t wheel; //电机轮号
  uint16_t count; //当前计数值
	uint16_t last_count; //上次计数值
	float cycle; //计数周期
	float speed; //速度 = （当前计数值 - 上次计数值）/计数周期
	float lenght;//步长，走过的距离
	
}encoderdef;


extern volatile encoderdef encoder_mlist[];





#endif

