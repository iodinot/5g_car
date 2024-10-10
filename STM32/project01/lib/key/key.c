#include "key.h"
#include "stdio.h"

/**按键采集文件**/




static void SW1_Short_fun();
static void SW1_Long_fun();


typedef struct
{
	GPIO_TypeDef* port; //GPIO
	uint16_t pin; //GPIO PIN
	void(*keyshortpress_fun)(); //短按回调函数
	void(*keylongpress_fun)(); //长按回调函数
}keyPortStructdef;


typedef struct
{
	uint8_t state; //按键IO状态
	uint8_t mask; //按键位移消抖变量
	uint8_t press; //按键按下或者释放状态
	uint8_t elapse; //按键按下时间
	uint8_t iscontinuousLongpress; //是否连续响应长按
	keyPortStructdef* keyPortPdu; //按键io对象

}keystateStructdef;

enum//响应状态枚举变量（连续或者单次）
{
	SINGLE_RESPONSE,
	CONTINUOUS_RESPONSE,
};

//按键相关IO和回调函数初始化
static keyPortStructdef keyPortlist1=
{
	SW1_GPIO_Port,SW1_Pin,SW1_Short_fun,SW1_Long_fun,
};

//按键对象状态初始化，可自行添加其他按键，只需往表中添加内容即可
static keystateStructdef keystatelist[]=
{
	{
		0,0,0,0,SINGLE_RESPONSE,
		&keyPortlist1,
	}

};

extern uint8_t key_num;
//SW1短按回调函数
static void SW1_Short_fun()
{
	printf("short press\n");
	key_num++;
	if(key_num > 3)
		key_num = 1;
	return;

}
//SW1长按回调函数
static void SW1_Long_fun()
{
	printf("long press\n");
	key_num++;
	if(key_num > 3)
		key_num = 0;
	
	return;

}

/** NONE:无按键  
	PRESS：按键按下 
	RELEASE：按键释放  
	SHORTPRESS：短按
	LONGPRESS：长按
**/
#define NONE  0
#define PRESS 1
#define RELEASE 2
#define SHORTPRESS  3
#define LONGPRESS  4

#define LONGPRESSCYCLE  200



void key_scan()//按键扫描，通过扫描按键表获取按键状态
{
	
	for(uint8_t i = 0;i< sizeof(keystatelist)/sizeof(keystatelist[0]);i++)
	{
		keystatelist[i].state = HAL_GPIO_ReadPin(keystatelist[i].keyPortPdu->port,keystatelist[i].keyPortPdu->pin);
		
		//状态消抖，通过位移实现延迟消抖
		if(0 == keystatelist[i].state)//周期扫描7次都是按下状态则认为按下
		{
			
			keystatelist[i].mask |= 0x01;	
		
			if(0x7f==keystatelist[i].mask) 
			{
//				printf("key press\n");
				keystatelist[i].press = PRESS;
			}	
			keystatelist[i].mask <<= 1;						
		}
		else//周期扫描7次都是释放状态则认为释放
		{		
			
			keystatelist[i].mask >>= 1;		
			
			if(0x01==keystatelist[i].mask) 
			{
//				printf("key release\n");
				keystatelist[i].press = RELEASE;
			}	
			
			
		}		
		//状态机扫描按键状态
		switch(keystatelist[i].press)
		{
			
			case PRESS:
					keystatelist[i].press = SHORTPRESS;
				break;
			
			case SHORTPRESS: //短按
				keystatelist[i].elapse++;			
				if(0 == keystatelist[i].state) //扫描期间未释放按键持续100个扫描周期，则认为是长按
				{
					if(keystatelist[i].elapse>LONGPRESSCYCLE)
						keystatelist[i].press = LONGPRESS;
				}
				else
				{
					if(NULL != keystatelist[i].keyPortPdu->keyshortpress_fun) //扫描期间释放了按键，则认为是短按
					{
						keystatelist[i].keyPortPdu->keyshortpress_fun();//短按函数响应
						keystatelist[i].press = NONE;
						keystatelist[i].elapse=0;
						
					}				
				}
				break;
				
			case LONGPRESS: //长按
					if(NULL != keystatelist[i].keyPortPdu->keyshortpress_fun)
					{
						if(keystatelist[i].iscontinuousLongpress)//连续响应长按
						{
							
						}
						else//连单次响应长按
						{
							keystatelist[i].press = NONE;
							keystatelist[i].elapse=0;
						}
						keystatelist[i].keyPortPdu->keylongpress_fun();//长按函数响应
					}
					
				break;
					
			case RELEASE: //按键释放
				    keystatelist[i].press = NONE;
					keystatelist[i].elapse=0;
				break;
		
		}	
		
	}
	

}