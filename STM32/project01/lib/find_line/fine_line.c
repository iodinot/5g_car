#include "fine_line.h"

#include "../../lib/oled/oled.h"
#include "stdio.h"

char have_line =0;

uint8_t find_line_state = 1;
uint16_t find_time=0;	
uint8_t intersection=0;
uint8_t final_intersection=0;
uint8_t low_pin_count = 0;

uint8_t get_LedFind_Scan(void)
{
	low_pin_count = 0;
	if (READ_FIND_LINE_OUT1 == GPIO_PIN_RESET) low_pin_count++;
	if (READ_FIND_LINE_OUT2 == GPIO_PIN_RESET) low_pin_count++;
	if (READ_FIND_LINE_OUT3 == GPIO_PIN_RESET) low_pin_count++;
	if (READ_FIND_LINE_OUT4 == GPIO_PIN_RESET) low_pin_count++;
	if (READ_FIND_LINE_OUT5 == GPIO_PIN_RESET) low_pin_count++;
	if (READ_FIND_LINE_OUT6 == GPIO_PIN_RESET) low_pin_count++;
	if (READ_FIND_LINE_OUT7 == GPIO_PIN_RESET) low_pin_count++;
	if (READ_FIND_LINE_OUT8 == GPIO_PIN_RESET) low_pin_count++;
	
	if (low_pin_count>=4) final_intersection++;
  else if	(low_pin_count==0) final_intersection ++;
	else final_intersection=0;
	if	(final_intersection>10){ intersection = 1; final_intersection=0;}

	if(READ_FIND_LINE_OUT4 == 0)
	{
		return 3;
	}
	else if(READ_FIND_LINE_OUT5 == 0)
	{
		return 4;
	}
	else if(READ_FIND_LINE_OUT6 == 0)
	{
		return 5;
	}
	else if(READ_FIND_LINE_OUT3 == 0)
	{
		return 2;
	}
	else if(READ_FIND_LINE_OUT2 == 0)
	{
		return 1;
	}
	else if(READ_FIND_LINE_OUT7 == 0)
	{
		return 6;
	}
	else if(READ_FIND_LINE_OUT1 == 0)
	{
		return 0;
	}
	else if(READ_FIND_LINE_OUT8 == 0)
	{
		return 7;
	}
	else
	{
		return 3;
	}	
//	else
//	{
//		find_time++;
//		if(find_time>0 && find_time<100)
//			find_line_state = 1; //find line is lost
//		if(find_time>=100)
//		{
//			find_time=1;
//			find_line_state = 0;	//find line is end
//			if(find_time>10000)
//				find_time=100;
//		}
//	}
//	
//	return 2;
}

static int Linechange_list[8]=
{
	-15,
	-10,
	-7,
	-3,
	3,
	7,
	10,
	15,
};
//extern void set_speed(uint16_t left,uint16_t right);
char value_led[20];
char show_intersection[20];
extern uint8_t car_speed_l,car_speed_r;
int Find_Line_Begins(void)
{
	static int find_value;
	find_value = Linechange_list[get_LedFind_Scan()];
	
//  sprintf(value_led,"value_led = %d   ",get_LedFind_Scan());
//  OLED_ShowString(0,1,(unsigned char *)value_led);
//	
//	sprintf(show_intersection,"intersection = %d   ",intersection);
//  OLED_ShowString(0,3,(unsigned char *)show_intersection);
	
	Moto_forward_run();
//	set_speed(35 + find_value,35 - find_value);
	car_speed_l = 20 + find_value;
	car_speed_r = 20 - find_value;
	
}

