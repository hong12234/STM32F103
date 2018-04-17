#ifndef __BSP_LCD_H
#define __BSP_LCD_H

#include "stm32f10x.h"

#define LCD_ROW						240								//����
#define	LCD_COL						240								//����

#define     RED          0xF800	  //��ɫ
#define     GREEN        0x07E0	  //��ɫ
#define     BLUE         0x001F	  //��ɫ
#define     WHITE        0xFFFF	  //��ɫ
#define     BLACK        0x0000	  //��ɫ
#define     YELLOW       0xFFE0	  //��ɫ
#define     ORANGE       0xFC08	  //��ɫ
#define     GRAY  	     0x8430   //��ɫ
#define     LGRAY        0xC618	  //ǳ��ɫ
#define     DARKGRAY     0x8410	  //���ɫ
#define     PORPO        0x801F	  //��ɫ
#define     PINK         0xF81F	  //�ۺ�ɫ
#define     GRAYBLUE     0x5458   //����ɫ
#define     LGRAYBLUE    0xA651   //ǳ����ɫ
#define     DARKBLUE     0x01CF	  //�����ɫ
#define 		LIGHTBLUE    0x7D7C	  //ǳ��ɫ


//��������
#define				SPI_LCD_BL_CLK									RCC_APB2Periph_GPIOA
#define				SPI_LCD_BL_PORT									GPIOA
#define				SPI_LCD_BL_PIN									GPIO_Pin_2

//RESET����
#define				SPI_LCD_RST_CLK									RCC_APB2Periph_GPIOA
#define				SPI_LCD_RST_PORT								GPIOA
#define				SPI_LCD_RST_PIN									GPIO_Pin_3

//D/C����
#define				SPI_LCD_DC_CLK									RCC_APB2Periph_GPIOC
#define				SPI_LCD_DC_PORT									GPIOA
#define				SPI_LCD_DC_PIN									GPIO_Pin_8


#define				SPI_LCD_DC_CMD()								GPIO_ResetBits( SPI_LCD_DC_PORT, SPI_LCD_DC_PIN )
#define				SPI_LCD_DC_DATA()								GPIO_SetBits( SPI_LCD_DC_PORT, SPI_LCD_DC_PIN )

#define				SPI_LCD_BL_ON()									GPIO_ResetBits( SPI_LCD_BL_PORT, SPI_LCD_BL_PIN )
#define				SPI_LCD_BL_OFF()								GPIO_SetBits( SPI_LCD_BL_PORT, SPI_LCD_BL_PIN )

#define				SPI_LCD_RST_LOW()								GPIO_ResetBits( SPI_LCD_RST_PORT, SPI_LCD_RST_PIN )
#define				SPI_LCD_RST_HIGH()							GPIO_SetBits( SPI_LCD_RST_PORT, SPI_LCD_RST_PIN )


void LCD_Init(void);
void LCD_Clear(uint16_t color);
void LCD_Fill(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend, uint16_t color);

#endif
