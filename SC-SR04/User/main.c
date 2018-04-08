#include "stm32f10x.h"
#include "delay.h"
#include "bsp_usart.h"
#include "sr04.h"
#include "bsp_i2c_gpio.h"
#include "OLED_I2C.h"

volatile uint16_t msHcCount = 0;
extern void Delay_ms(uint16_t nms);

int main(void)
{
	float length = 0;
	
	uint8_t i = 0;
	
	Delay_Init();
	
	USART_Config();
	
	printf("串口初始化完成！\n");	
	
	i2c_GPIO_Init();
	OLED_Init();
	
	printf("OLED初始化完成！\n");
	
	HCSR04_Config();
	
	printf("超声波模块初始化完成！\n");
	
	while(1)
	{
		length = Hcsr04GetLength();
		
		if((uint16_t)length>=1000)
		i = (uint16_t)length/1000 + 0x30;
		else
		i = 32;
		OLED_ShowStr(20,3,&i,2);
		
		if((uint16_t)length>=100)
		i = (uint16_t)length%1000/100 + 0x30;
		else
		i = 32;
		OLED_ShowStr(28,3,&i,2);
		
		if((uint16_t)length>=10)
		i = (uint16_t)length%100/10 + 0x30;
		else
		i = 32;
		OLED_ShowStr(36,3,&i,2);

		i = (uint16_t)length%10 + 0x30;
		OLED_ShowStr(44,3,&i,2);
		OLED_ShowStr(52,3,".",2);
		
		i = (uint16_t)(length*10)%10 + 0x30;
		OLED_ShowStr(60,3,&i,2);
		
		i = (uint16_t)(length*100)%10 + 0x30;
		OLED_ShowStr(68,3,&i,2);
		
		i = (uint16_t)(length*1000)%10 + 0x30;
		OLED_ShowStr(76,3,&i,2);
		OLED_ShowStr(84,3,"cm",2);
		
		printf("距离为：%.3fcm\n",length);
	}
}
