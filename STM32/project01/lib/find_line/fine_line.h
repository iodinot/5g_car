#ifndef __FINE_LINE_H
#define __FINE_LINE_H

#include "main.h"

#define  u8 unsigned char 
#define  u32 unsigned int 


#define READ_FIND_LINE_OUT1 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)
#define READ_FIND_LINE_OUT2 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)
#define READ_FIND_LINE_OUT3 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)
#define READ_FIND_LINE_OUT4 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3)
#define READ_FIND_LINE_OUT5 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)
#define READ_FIND_LINE_OUT6 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)
#define READ_FIND_LINE_OUT7 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10)
#define READ_FIND_LINE_OUT8 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)


static uint8_t mask_line=0;

uint8_t get_LedFind_Scan(void);
int Find_Line_Begins(void);
#endif

