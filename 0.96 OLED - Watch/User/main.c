#include "stm32f10x.h"
#include "delay.h"
#include "bsp_i2c_gpio.h"
#include "OLED_I2C.h"
#include "bsp_TiMbase.h"
#include "bsp_usart.h"

volatile uint8_t x=0,y=0,z=0;
volatile uint16_t ms=0;

int main(void)
{
	uint8_t i=4,x1=1,y1=1,z1=1,x2=1,y2=1,z2=1;
	
	Delay_Init();
	i2c_GPIO_Init();
	OLED_Init();
	BASIC_TIM_Init();
	USART_Config();
	
	while(1)
	{
		OLED_ShowStr(28,0,"2018/3/30",2);
		if(z%10 != z2)
		{
			OLED_F10x32(i+110,2,z%10);
			z2 = z%10;
		}
		if(z/10 != z1)
		{
			OLED_F10x32(i+100,2,z/10);
			z1 = z/10;
		}
		OLED_F10x48(i+90,2);
		if(y%10 != y2)
		{
			OLED_F20x48(i+70,2,y%10);
			y2 = y%10;
		}
		if(y/10 != y1)
		{
			OLED_F20x48(i+50,2,y/10);
			y1 = y/10;
		}
		OLED_F10x48(i+40,2);
		if(x%10 != x2)
		{
			OLED_F20x48(i+20,2,x%10);
			x2 = x%10;
		}if(x/10 != x1)
		{
			OLED_F20x48(i,2,x/10);
			x1 = x/10;
		}
	}
}
