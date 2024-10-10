#ifndef __OLED_H
#define __OLED_H	
#include "main.h"
#include "usart.h"

#define  u8 unsigned char 
#define  u32 unsigned int 

#define OLED_MODE 0
#define SIZE 8
#define XLevelL     0x00
#define XLevelH     0x10
#define Max_Column  128
#define Max_Row     64
#define Brightness  0xFF
#define X_WIDTH     128
#define Y_WIDTH     64

#define OLED_SCLK_Clr()   	HAL_GPIO_WritePin(IIC1_SCL_GPIO_Port,IIC1_SCL_Pin,GPIO_PIN_RESET)//CLK
#define OLED_SCLK_Set() 	HAL_GPIO_WritePin(IIC1_SCL_GPIO_Port,IIC1_SCL_Pin,GPIO_PIN_SET)

#define OLED_SDIN_Clr()   	HAL_GPIO_WritePin(IIC1_SDA_GPIO_Port,IIC1_SDA_Pin,GPIO_PIN_RESET)//DIN
#define OLED_SDIN_Set() 	HAL_GPIO_WritePin(IIC1_SDA_GPIO_Port,IIC1_SDA_Pin,GPIO_PIN_SET)

#define OLED_CMD  0
#define OLED_DATA 1

extern int GetOpenmvDataCount;
extern uint8_t  Num, LoR, Finded_flag, FindTask;
extern uint8_t LastNum;
extern uint8_t NumCount;
extern uint8_t uart6_rxbuff;
extern uint8_t RoomNum, TargetNum;
extern uint8_t TASK;    //这个TASK可以传输给openmv，赋值openmv上的FindTask来控制openmv模板匹配的不同模式
extern char TargetRoom;  //A, B, C, D, E, F, G, H;    //这八个字符对应着地图实际房间，里面的数字3—8会随机对应C-H
extern float Velocity,Velocity_Set,Angle_Set,Turn;
extern uint8_t Flag_Stop;       //默认停止标志位

// Functions to control OLED are as follows
void OLED_WR_Byte(unsigned dat,unsigned cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
extern void OLED_ShowString(u8 x,u8 y, u8 *p);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void fill_picture(unsigned char fill_Data);
void Picture();
void IIC_Start();
void IIC_Stop();
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack();

void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size);
uint32_t oled_pow(uint8_t m,uint8_t n);
void oled_show(void);

#endif

