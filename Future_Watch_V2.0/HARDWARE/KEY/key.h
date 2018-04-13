#ifndef _KEY_H_
#define _KEY_H_

#include "sys.h"
#include "delay.h"
#include "gpio.h"

#define START_MCU		GPIOB->ODR|=(1<<13)		//PB13 = 1����
#define STOP_MCU		GPIOB->ODR&=~(1<<13)		//PB13 = 0�ػ�
#define KEY_CHECK		GPIOB->IDR &= (1<<12)

//������ʼ��
void Key_Init(void);


#endif
