#ifndef _BACKLIT_H_
#define _BACKLIT_H_

#include "delay.h"
#include "sys.h"


#define LSENS_READ_TIMES	10		//���������������ȡ����,����ô���,Ȼ��ȡƽ��ֵ
#define LSENS_ADC_CHX		11		//����������������ڵ�ADCͨ�����


void Lsens_Init(void); 				//��ʼ������������
u8 Lsens_Get_Val(void);				//��ȡ������������ֵ
void TIM3_PWM_Init(u32 arr,u32 psc);

#endif
