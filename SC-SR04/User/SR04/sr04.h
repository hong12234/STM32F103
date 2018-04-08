#ifndef __SR04_H
#define __SR04_H

#include "stm32f10x.h"

#define				HCSR04_TRIG_CLK				RCC_APB2Periph_GPIOA
#define				HCSR04_TRIG_PORT			GPIOA
#define				HCSR04_TRIG_PIN				GPIO_Pin_12

#define				HCSR04_ECHO_CLK				RCC_APB2Periph_GPIOA
#define				HCSR04_ECHO_PORT			GPIOA
#define				HCSR04_ECHO_PIN				GPIO_Pin_11


void HCSR04_Config(void);
float Hcsr04GetLength(void);

#endif
