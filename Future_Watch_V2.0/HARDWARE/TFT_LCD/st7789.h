/*
**************************************************************
*
* �ļ����ƣ�st7789.h;
*
* �汾˵����V1.1��
*
* ���л�����(1) STM32F103RCT;
*           (2) �ⲿ����8M9��Ƶ��
*           (3) MDK 5.0��
*
* �޸�˵�����ޣ�
*           
* ������ڣ�2016/06/25;
*
* ����������֣��˸;
*
* ������ע��1.54��Һ������������ 80���ڣ�����оƬst7789��
*          
* �޸�˵����2016/6/25 ���Ӷ��㺯����
*
**************************************************************
*/


#ifndef __ST7789_H__
#define __ST7789_H__


#include "sys.h"
#include "delay.h"
#include "gpio.h"


//#define CS1  PCout(6)  
//#define RS   PCout(7)  
//#define WRD  PCout(8)  
//#define RDB  PCout(9)  

#define CS1  PBout(9)  
#define RS   PBout(8)  
#define WRD  PBout(7)  
#define RDB  PCout(3) 


//#define DATA_OUT(x) GPIOF->ODR =x
//#define DATA_READ() (GPIOF->IDR)//&0x000000ff
#define DATA_OUT(x) GPIOA->ODR =x
#define DATA_READ() (GPIOA->IDR)//&0x000000ff

///*******ֱ�Ӳ���GPIO����Ĵ���*******/
//#define LCD_CS_SET GPIOC->BSRR |=1<<6                              //Ƭѡ�ˣ�
//#define LCD_RS_SET GPIOC->BSRR |=1<<7                              //����\���ݣ� (0/1)
//#define LCD_WR_SET GPIOC->BSRR |=1<<8                              //д�룻      (0->1��Ч����/1��Ч)
//#define LCD_RD_SET GPIOC->BSRR |=1<<9                              //������      (0->1��Ч����/1��Ч)
//////#define LCD_RESET_SET                                           //Һ����λ������н��룻
///*******ֱ�Ӳ���GPIO����Ĵ���*******/
#define LCD_CS_SET GPIOB->BSRRL |=1<<9                              //Ƭѡ�ˣ�
#define LCD_RS_SET GPIOB->BSRRL |=1<<8                              //����\���ݣ� (0/1)
#define LCD_WR_SET GPIOB->BSRRL |=1<<7                              //д�룻      (0->1��Ч����/1��Ч)
#define LCD_RD_SET GPIOC->BSRRL |=1<<3                              //������      (0->1��Ч����/1��Ч)


//#define LCD_CS_CLR GPIOC->BSRR  |=1<<22                              //Ƭѡ�ˣ�
//#define LCD_RS_CLR GPIOC->BSRR  |=1<<23                              //����\���ݣ�
//#define LCD_WR_CLR GPIOC->BSRR  |=1<<24                              //д�룻
//#define LCD_RD_CLR GPIOC->BSRR  |=1<<25                              //������
#define LCD_CS_CLR GPIOB->BSRRH |=1<<9                              //Ƭѡ�ˣ�
#define LCD_RS_CLR GPIOB->BSRRH |=1<<8                             //����\���ݣ�
#define LCD_WR_CLR GPIOB->BSRRH |=1<<7                             //д�룻
#define LCD_RD_CLR GPIOC->BSRRH |=1<<3                             //������
#define LCD_RESET_CLR                                           //Һ����λ������н��룻


/****LCD����������****/
typedef struct
{
	u8 with;         //Һ������ȣ�
	u8 height;       //Һ�������ȣ�
	u8 wramcmd;      //��ʼдgram���
	u8 setxcmd;      //����x���ꣻ
	u8 setycmd;      //����y���ꣻ
	u16 color;       //������ɫ��
}_lcd_dev;

extern _lcd_dev lcd;


/**********��������***********/

/*
* �������ƣ�Init_ST7789��
* ����˵����st7789��ʼ����
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void Init_ST7789(void);


/*
* �������ƣ�Write_ST7789_Reg��
* ����˵����д���
* ��ڲ�����hibyte->���
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void Write_ST7789_Reg(u8 hibyte);
 

/*
* �������ƣ�Write_ST7789_Data��
* ����˵����д���ݣ�
* ��ڲ�����hibyte->���ݣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void Write_ST7789_Data(u8 hibyte);


/*
* �������ƣ�Read_ST7789_Data��
* ����˵���������ݣ�
* ��ڲ������ޣ�
* ����ֵ  ��8λ���ݣ�
* ��ע    ���ޣ�
*/
u8 Read_ST7789_Data(void);


/*
* �������ƣ�Address_Set��
* ����˵�����������ã�
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void  Address_Set(u16 x,u16 y);


/*
* �������ƣ�Write_ST7789_16Bit��
* ����˵����д16λ��ɫ���ݣ�
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void Write_ST7789_16Bit(u8 hibyte,u8 lowbyte);


/*
* �������ƣ�Write_ST7789_Point��
* ����˵����д�����ɫ���ݣ�
* ��ڲ�����Ŀ�����ꣻ
* ����ֵ  ��16λ��ɫֵ��
* ��ע    ���ޣ�
*/
void Write_ST7789_Point(u16 x,u16 y,u16 color);


/*
* �������ƣ�Read_ST7789_Point��
* ����˵�����������ɫ���ݣ�
* ��ڲ�����Ŀ�����ꣻ
* ����ֵ  ��16λ��ɫֵ��
* ��ע    ���ޣ�
*/
u16 Read_ST7789_Point(u16 x,u16 y);

void Draw_Background(u8 x_s,u8 y_s,u8 length,u8 wide,u16 color);
u8 Read_LCD_Reg(u8 reg);

#endif
