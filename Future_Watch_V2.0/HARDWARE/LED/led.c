#include "led.h" 
#include "gpio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{    	 
	GPIO_Clock_Set(IOPFEN);
	GPIO_Init(GPIOF,P9,IO_MODE_OUT,GPIO_SPEED_50M,IO_OTYPER_PP,IO_NOT,IO_H);
	GPIO_Init(GPIOF,P10,IO_MODE_OUT,GPIO_SPEED_50M,IO_OTYPER_PP,IO_NOT,IO_H);
	//RCC->AHB1ENR|=1<<5;//ʹ��PORTFʱ�� 
	//GPIO_Set(GPIOF,PIN9|PIN10,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PF9,PF10����
	LED0=1;//LED0�ر�
	LED1=1;//LED1�ر�
}






