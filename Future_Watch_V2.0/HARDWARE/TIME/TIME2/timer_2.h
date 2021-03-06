/*
**************************************************************
*
* 文件名称：time_6.h;
*
* 版本说明：V.10；
*
* 运行环境：(1) STM32F103RCT;
*           (2) 外部晶振8M9倍频；
*           (3) MDK 5.0；
*
* 修改说明：无；
*           
* 完成日期：2016/02/15;
*
* 作者姓名：郑子烁;
*
* 其他备注：基本定时器6使用；
*
**************************************************************
*/


#ifndef __TIME_2_H__
#define __TIME_2_H__

#include "delay.h"
#include "sys.h"
#include "led.h"

/*
* 函数名称：TIM6_IRQHandler；
* 功能说明：基本定时器6中断服务程序；
* 入口参数：无；
* 返回值  ：无；
* 备注    ：UIF位在更新中断时硬件设置该位，它由软件清除，
*           在函数里查询标志位,进行软件清除；
*/
void TIM2_IRQHandler(void);

/*
* 函数名称：Time_6_Init；
* 功能说明：基本定时器6初始化；
* 入口参数：arr->自动重装载值,psc->分频值；
* 返回值  ：无；
* 备注    ：定时器的时钟频率CK_CNT等于fCK_PSC/(PSC[15:0]+1)；
*           计数器是从0开始计数的；
*/
void Time_2_Init(u16 arr,u16 psc);




#endif
