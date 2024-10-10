#include "usart_openmv_stm32.h"
#include "usart.h"

/**串口通讯文件**/


usartstructdef Usart1_rec={0};

usartstructdef Usart2_rec={0};


//串口数据解析
void usart_anl()
{
	/**可以自行添加数据  bufs=Usart1_rec.buf[Usart1_rec.out++] 获取数据，或者直接解析数据**/
	if(Usart1_rec.in!=Usart1_rec.out)
	{
		Usart1_rec.buf[Usart1_rec.out++];
		
	}
	else
	{
		Usart1_rec.out=0;
	}
	
	if(Usart2_rec.in!=Usart2_rec.out)
	{
		Usart2_rec.buf[Usart2_rec.out++];
	}
	else
	{
	
		Usart2_rec.out=0;
	}
}



//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance==USART1)
//	{	
//	
//		HAL_UART_Receive_IT(&huart1,(uint8_t*)(&Usart1_rec.buf[Usart1_rec.in]),1);		
//	
//		if(++Usart1_rec.in>=USART_SIZE)//采用环形缓冲读取数据
//		{
//			Usart1_rec.in=0;
//		
//		}
//		
//	}
//	if(huart->Instance==USART2)
//	{
//	
//		HAL_UART_Receive_IT(&huart2,(uint8_t*)(&Usart2_rec.buf[Usart2_rec.in]),1);		
//	
//		if(++Usart2_rec.in>=USART_SIZE)
//		{
//			Usart2_rec.in=0;
//		
//		}	
//	
//	}
//}
