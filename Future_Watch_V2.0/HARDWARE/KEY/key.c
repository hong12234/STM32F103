#include "key.h"

//������ʼ��
void Key_Init(void)
{
	GPIO_Clock_Set(IOPBEN);		//ʹ��PBʱ��
	GPIO_Init(GPIOB,P13,IO_MODE_OUT,IO_SPEED_50M,IO_OTYPER_PP,IO_KEEP,IO_L);	//PB13 ���������
	GPIO_Init(GPIOB,P12,IO_MODE_IN,IO_SPEED_50M,IO_OTYPER_PP,IO_DOWN,IO_L);		//PB12 ���������
}

