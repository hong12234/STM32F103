/*
**************************************************************
*
* �ļ����ƣ�time_1.h;
*
* �汾˵����V.13��
*
* ���л�����(1) STM32F103RCT;
*           (2) �ⲿ����8M9��Ƶ��
*           (3) MDK 5.0��
*
* �޸�˵�����������벶��ʵ�飻
*           
* ������ڣ�2016/03/04;
*
* ����������֣��˸;
*
* ������ע���߼���ʱ������ʵ�飻
*           PWM����ʵ�飻
*           ���벶��ʵ�飻
*           �������ӿ�ģʽʵ�飻
*      
**************************************************************
*/

#ifndef __TIME_1_H__
#define __TIME_1_H__

#include "sys.h"
#include "delay.h"
//#include "gpio.h"
//#include "led.h"

#define CNT_FREQUENCY   600000                         //����Ƶ��*60��


#define PWM_VAL TIM1->CCR1                           //���ò���/�ȽϼĴ���1��
#define TIM1_COUNT TIM1->CNT                         //��ȡ��ʱ��1�ļ���ֵ��
#define GET_DIRECTION ((TIM1->CR1 & 1<<4)?1:0)       //��ȡ��������




/*
* �������ƣ�Time_1_Init��
* ����˵�����߼���ʱ����ʼ����
* ��ڲ�����arr->�Զ���װֵ��psc->Ԥ��Ƶ����
* ����ֵ  ���ޣ�
* ��ע    ��ע������ģʽ��ʱ��Դ��
*           Ĭ�����ϱ��ؼ�������ֹ�жϺ�DMA��
*/
void Time_1_Init(u16 arr,u16 psc);


/*
* �������ƣ�Get_Haer_Rate��
* ����˵�������������ݣ�
* ��ڲ������ޣ�
* ����ֵ  �����ʣ�
* ��ע    ���ޣ�
*/
u8 Get_Haer_Rate(void);

#endif