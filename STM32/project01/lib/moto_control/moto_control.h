#ifndef __MOTO_CONTROL_H
#define __MOTO_CONTROL_H

#include "../encoder/encoder.h"




typedef enum
{
	POSITION,
	SPEED,

}CONTROL_MODE;

typedef enum
{
	TURN_LEFT,
	TURN_RIGHT,
  FORWARD,
}TURN_DIRE;


extern float speed_destination[];

extern void moto_control_l(uint8_t moto_id,float destination,float control_mode);
extern void moto_control_r(uint8_t moto_id,float destination,float control_mode);
extern void Moto_forward_run();
extern void Moto_back_run();
extern void Moto_turn_left();
extern void Moto_turn_right();
extern void Moto_stop();
extern uint8_t set_turn_angle(float angle,float turn_speed,uint8_t dire);
extern void set_speed(int speed_left,int speed_right);
#endif

