#include "key.h"
#include "stdio.h"

/**�����ɼ��ļ�**/




static void SW1_Short_fun();
static void SW1_Long_fun();


typedef struct
{
	GPIO_TypeDef* port; //GPIO
	uint16_t pin; //GPIO PIN
	void(*keyshortpress_fun)(); //�̰��ص�����
	void(*keylongpress_fun)(); //�����ص�����
}keyPortStructdef;


typedef struct
{
	uint8_t state; //����IO״̬
	uint8_t mask; //����λ����������
	uint8_t press; //�������»����ͷ�״̬
	uint8_t elapse; //��������ʱ��
	uint8_t iscontinuousLongpress; //�Ƿ�������Ӧ����
	keyPortStructdef* keyPortPdu; //����io����

}keystateStructdef;

enum//��Ӧ״̬ö�ٱ������������ߵ��Σ�
{
	SINGLE_RESPONSE,
	CONTINUOUS_RESPONSE,
};

//�������IO�ͻص�������ʼ��
static keyPortStructdef keyPortlist1=
{
	SW1_GPIO_Port,SW1_Pin,SW1_Short_fun,SW1_Long_fun,
};

//��������״̬��ʼ�����������������������ֻ��������������ݼ���
static keystateStructdef keystatelist[]=
{
	{
		0,0,0,0,SINGLE_RESPONSE,
		&keyPortlist1,
	}

};

extern uint8_t key_num;
//SW1�̰��ص�����
static void SW1_Short_fun()
{
	printf("short press\n");
	key_num++;
	if(key_num > 3)
		key_num = 1;
	return;

}
//SW1�����ص�����
static void SW1_Long_fun()
{
	printf("long press\n");
	key_num++;
	if(key_num > 3)
		key_num = 0;
	
	return;

}

/** NONE:�ް���  
	PRESS���������� 
	RELEASE�������ͷ�  
	SHORTPRESS���̰�
	LONGPRESS������
**/
#define NONE  0
#define PRESS 1
#define RELEASE 2
#define SHORTPRESS  3
#define LONGPRESS  4

#define LONGPRESSCYCLE  200



void key_scan()//����ɨ�裬ͨ��ɨ�谴�����ȡ����״̬
{
	
	for(uint8_t i = 0;i< sizeof(keystatelist)/sizeof(keystatelist[0]);i++)
	{
		keystatelist[i].state = HAL_GPIO_ReadPin(keystatelist[i].keyPortPdu->port,keystatelist[i].keyPortPdu->pin);
		
		//״̬������ͨ��λ��ʵ���ӳ�����
		if(0 == keystatelist[i].state)//����ɨ��7�ζ��ǰ���״̬����Ϊ����
		{
			
			keystatelist[i].mask |= 0x01;	
		
			if(0x7f==keystatelist[i].mask) 
			{
//				printf("key press\n");
				keystatelist[i].press = PRESS;
			}	
			keystatelist[i].mask <<= 1;						
		}
		else//����ɨ��7�ζ����ͷ�״̬����Ϊ�ͷ�
		{		
			
			keystatelist[i].mask >>= 1;		
			
			if(0x01==keystatelist[i].mask) 
			{
//				printf("key release\n");
				keystatelist[i].press = RELEASE;
			}	
			
			
		}		
		//״̬��ɨ�谴��״̬
		switch(keystatelist[i].press)
		{
			
			case PRESS:
					keystatelist[i].press = SHORTPRESS;
				break;
			
			case SHORTPRESS: //�̰�
				keystatelist[i].elapse++;			
				if(0 == keystatelist[i].state) //ɨ���ڼ�δ�ͷŰ�������100��ɨ�����ڣ�����Ϊ�ǳ���
				{
					if(keystatelist[i].elapse>LONGPRESSCYCLE)
						keystatelist[i].press = LONGPRESS;
				}
				else
				{
					if(NULL != keystatelist[i].keyPortPdu->keyshortpress_fun) //ɨ���ڼ��ͷ��˰���������Ϊ�Ƕ̰�
					{
						keystatelist[i].keyPortPdu->keyshortpress_fun();//�̰�������Ӧ
						keystatelist[i].press = NONE;
						keystatelist[i].elapse=0;
						
					}				
				}
				break;
				
			case LONGPRESS: //����
					if(NULL != keystatelist[i].keyPortPdu->keyshortpress_fun)
					{
						if(keystatelist[i].iscontinuousLongpress)//������Ӧ����
						{
							
						}
						else//��������Ӧ����
						{
							keystatelist[i].press = NONE;
							keystatelist[i].elapse=0;
						}
						keystatelist[i].keyPortPdu->keylongpress_fun();//����������Ӧ
					}
					
				break;
					
			case RELEASE: //�����ͷ�
				    keystatelist[i].press = NONE;
					keystatelist[i].elapse=0;
				break;
		
		}	
		
	}
	

}