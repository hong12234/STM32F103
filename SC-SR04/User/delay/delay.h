#ifndef __DELAY_H
#define __DELAY_H 			   

#include "stm32f10x.h"

void Delay_Init(void);
void Delay_us(uint16_t nus);
void Delay_ms(uint16_t nms);
void Delay_s(uint16_t ns);

#endif
