/***********************************************
公司：轮趣科技（东莞）有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：5.7
修改时间：2021-04-29

 
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update：2021-04-29

All rights reserved
***********************************************/
#ifndef __LED_H
#define __LED_H
#include "main.h"
//LED 端口定义
#define LED(value) (HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, (value) ? GPIO_PIN_SET : GPIO_PIN_RESET))

extern uint8_t Load_flag;
void LED_Init(void);  //初始化
void Led_Flash(uint16_t time);

extern void LoadOrNot(void);
void Load_Init(void);
#endif
