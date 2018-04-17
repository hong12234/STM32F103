#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32f10x.h"


/*SPI接口定义-开头****************************/
#define				SPIx													SPI1
#define				SPI_APBxClock_FUN							RCC_APB2PeriphClockCmd
#define				SPI_CLK												RCC_APB2Periph_SPI1

//CS(NSS)引脚 片选选普通GPIO即可
#define				SPI_LCD_CS_CLK								RCC_APB2Periph_GPIOC
#define				SPI_LCD_CS_PORT								GPIOC
#define				SPI_LCD_CS_PIN								GPIO_Pin_0

//SCK引脚
#define				SPI_SCK_CLK										RCC_APB2Periph_GPIOA   
#define				SPI_SCK_PORT									GPIOA   
#define				SPI_SCK_PIN										GPIO_Pin_5
//MISO引脚
#define				SPI_MISO_CLK									RCC_APB2Periph_GPIOA    
#define				SPI_MISO_PORT									GPIOA 
#define				SPI_MISO_PIN									GPIO_Pin_6
//MOSI引脚
#define				SPI_MOSI_CLK									RCC_APB2Periph_GPIOA    
#define				SPI_MOSI_PORT									GPIOA 
#define				SPI_MOSI_PIN									GPIO_Pin_7

#define				SPI_LCD_CS_LOW()							GPIO_ResetBits( SPI_LCD_CS_PORT, SPI_LCD_CS_PIN )
#define				SPI_LCD_CS_HIGH()							GPIO_SetBits( SPI_LCD_CS_PORT, SPI_LCD_CS_PIN )

/*SPI接口定义-结尾****************************/


void SPIx_Init(void);
uint8_t SPIx_SendByte(uint8_t byte);
uint16_t SPIx_SendHalfWord(uint16_t HalfWord);

#endif
