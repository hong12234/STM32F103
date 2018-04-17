#include "stm32f10x.h"
#include "delay.h"
#include "bsp_spi.h"
#include "lcd_spi.h"

int main(void)
{	
	Delay_Init();
	SPIx_Init();
	LCD_Init();
	
	
	while(1)
	{
		LCD_Clear(RED);
		LCD_Clear(GREEN);
		LCD_Clear(BLUE);
		LCD_Clear(WHITE);
		LCD_Clear(BLACK);
		LCD_Clear(YELLOW);
		LCD_Clear(ORANGE);
		LCD_Clear(GRAY);
		LCD_Clear(LGRAY);
		LCD_Clear(DARKGRAY);
		LCD_Clear(PORPO);
		LCD_Clear(PINK);
		LCD_Clear(GRAYBLUE);
		LCD_Clear(LGRAYBLUE);
		LCD_Clear(DARKBLUE);
		LCD_Clear(LIGHTBLUE);
	}
}

