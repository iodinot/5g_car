/***********************************************
��˾����Ȥ�Ƽ�����ݸ�����޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com 
����ͨ: https://minibalance.aliexpress.com/store/4455017
�汾��5.7
�޸�ʱ�䣺2021-04-29

 
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update��2021-04-29

All rights reserved
***********************************************/
#ifndef __LED_H
#define __LED_H
#include "main.h"
//LED �˿ڶ���
#define LED(value) (HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, (value) ? GPIO_PIN_SET : GPIO_PIN_RESET))

extern uint8_t Load_flag;
void LED_Init(void);  //��ʼ��
void Led_Flash(uint16_t time);

extern void LoadOrNot(void);
void Load_Init(void);
#endif
