#include "stm32f10x.h"
#include "delay.h"
#include "bsp_i2c_gpio.h"
#include "OLED_I2C.h"


int main(void)
{
//	int8_t i,j;
	uint16_t i;
	uint8_t j;
	extern const unsigned char BMP1[];
	extern const unsigned char BMP2[];
	
	Delay_Init();
	i2c_GPIO_Init();
	OLED_Init();
	
	while(1)
	{
//		OLED_ShowStr(24,3,"WenZhongYi",2);
//		OLED_CLS();
		
		OLED_ShowStr(24,3,"Temp:",2);
		OLED_ShowStr(80,3,".",2);
		OLED_ShowStr(96,3,"C",2);
		for(i=999; i>1; i--)
		{
			j = i/100 + 0x30;
			OLED_ShowStr(64,3,&j,2);
			j = i%100/10 + 0x30;
			OLED_ShowStr(72,3,&j,2);
			j = i%10 + 0x30;
			OLED_ShowStr(88,3,&j,2);
			Delay_ms(100);
		}

		/*	
		for(j=7;j>=0;j--)
		{
			for(i=j; i<8; i++)//通过点阵显示汉字--i表示字库数组的位置
			{
				OLED_ShowCN(i*16,0,i-j,1);
				OLED_ShowCN(i*16,2,i-j+8,1);
				OLED_ShowCN(i*16,4,i-j+16,1);
				OLED_ShowCN(i*16,6,i-j+24,1);
			}
			Delay_ms(100);
		}
		
		for(i=10; i>0; i--)
		{
			if(i%2 == 0)
			{
				for(j=0; j<8; j++)//通过点阵显示汉字--i表示字库数组的位置
				{
					OLED_ShowCN(j*16,0,j,0);
					OLED_ShowCN(j*16,2,j+8,0);
					OLED_ShowCN(j*16,4,j+16,0);
					OLED_ShowCN(j*16,6,j+24,0);
				}
			
			}
			else
			{
				for(j=0; j<8; j++)//通过点阵显示汉字--i表示字库数组的位置
				{
						OLED_ShowCN(j*16,0,j,1);
						OLED_ShowCN(j*16,2,j+8,1);
						OLED_ShowCN(j*16,4,j+16,1);
						OLED_ShowCN(j*16,6,j+24,1);
				}			
			}
			Delay_ms(100);			
		}
		
		Delay_s(2);
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);
		Delay_s(2);
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP2);
		Delay_s(2);
		OLED_CLS();//清屏
		*/
	}
}
