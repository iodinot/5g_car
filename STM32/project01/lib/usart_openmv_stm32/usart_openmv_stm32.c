#include "usart_openmv_stm32.h"
#include "usart.h"

/**����ͨѶ�ļ�**/


usartstructdef Usart1_rec={0};

usartstructdef Usart2_rec={0};


//�������ݽ���
void usart_anl()
{
	/**���������������  bufs=Usart1_rec.buf[Usart1_rec.out++] ��ȡ���ݣ�����ֱ�ӽ�������**/
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
//		if(++Usart1_rec.in>=USART_SIZE)//���û��λ����ȡ����
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
