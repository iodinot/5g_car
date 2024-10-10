#ifndef USART_OPENMV_STM32_H
#define USART_OPENMV_STM32_H
#include "../../Core/Inc/main.h"


#define USART_SIZE	8
typedef struct
{
	uint8_t in;//�����±�
	uint8_t out;//����±�
	uint8_t buf[USART_SIZE];//���ݻ�����
	

}usartstructdef;



extern usartstructdef Usart1_rec;
extern usartstructdef Usart2_rec;



#endif

