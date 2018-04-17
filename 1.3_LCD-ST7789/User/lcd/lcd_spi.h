#ifndef __BSP_LCD_H
#define __BSP_LCD_H

#include "stm32f10x.h"

#define LCD_ROW						240								//行数
#define	LCD_COL						240								//列数

#define     RED          0xF800	  //红色
#define     GREEN        0x07E0	  //绿色
#define     BLUE         0x001F	  //蓝色
#define     WHITE        0xFFFF	  //白色
#define     BLACK        0x0000	  //黑色
#define     YELLOW       0xFFE0	  //黄色
#define     ORANGE       0xFC08	  //橙色
#define     GRAY  	     0x8430   //灰色
#define     LGRAY        0xC618	  //浅灰色
#define     DARKGRAY     0x8410	  //深灰色
#define     PORPO        0x801F	  //紫色
#define     PINK         0xF81F	  //粉红色
#define     GRAYBLUE     0x5458   //灰蓝色
#define     LGRAYBLUE    0xA651   //浅灰蓝色
#define     DARKBLUE     0x01CF	  //深灰蓝色
#define 		LIGHTBLUE    0x7D7C	  //浅蓝色


//背光引脚
#define				SPI_LCD_BL_CLK									RCC_APB2Periph_GPIOA
#define				SPI_LCD_BL_PORT									GPIOA
#define				SPI_LCD_BL_PIN									GPIO_Pin_2

//RESET引脚
#define				SPI_LCD_RST_CLK									RCC_APB2Periph_GPIOA
#define				SPI_LCD_RST_PORT								GPIOA
#define				SPI_LCD_RST_PIN									GPIO_Pin_3

//D/C引脚
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
