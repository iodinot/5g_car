#ifndef __ENCODER_H
#define __ENCODER_H

#include "../../Core/Inc/main.h"



#if WHEELS==FOUR_WHEELS //���ַ�λ����


/** LEFT_REAR:�����  
	RIGHT_REAR���Һ���
	LEFT_FRONT����ǰ���
	RIGHT_FRONT����ǰ���
**/
typedef enum//�������ö�ٱ���
{
	LEFT_REAR,
	RIGHT_REAR,
	LEFT_FRONT,
	RIGHT_FRONT,
	
}ewheel;

#elif WHEELS==THREE_WHEELS //���ַ�λ����


/** LEFT_REAR:����  
	RIGHT_REAR���ҵ��
**/
typedef enum //�������ö�ٱ���
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
	uint16_t wheel; //����ֺ�
  uint16_t count; //��ǰ����ֵ
	uint16_t last_count; //�ϴμ���ֵ
	float cycle; //��������
	float speed; //�ٶ� = ����ǰ����ֵ - �ϴμ���ֵ��/��������
	float lenght;//�������߹��ľ���
	
}encoderdef;


extern volatile encoderdef encoder_mlist[];





#endif

