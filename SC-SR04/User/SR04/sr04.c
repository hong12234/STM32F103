#include "sr04.h"
#include "bsp_TIMbase.h"
#include "delay.h"

extern volatile uint16_t msHcCount;

// tips:static函数的作用域仅限于定义它的源文件
static void HCSR04_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(HCSR04_TRIG_CLK|HCSR04_ECHO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = HCSR04_TRIG_PIN;// 触发信号引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;// 推挽输出
	GPIO_Init(HCSR04_TRIG_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(HCSR04_TRIG_PORT,HCSR04_TRIG_PIN);// 初始化后输出低
	
	GPIO_InitStructure.GPIO_Pin = HCSR04_ECHO_PIN;// 反馈信号引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;// 浮空输入
	GPIO_Init(HCSR04_ECHO_PORT, &GPIO_InitStructure);
}


static void OpenTimerForHc()        // 打开定时器
{
	TIM_SetCounter(TIM6,0);// 清除计数
	msHcCount = 0;
	TIM_Cmd(TIM6, ENABLE);  // 使能计数
}
 
static void CloseTimerForHc()        // 
{
	TIM_Cmd(TIM6, DISABLE);  // 
}

// 获取定时器时间
static uint32_t GetEchoTimer(void)
{
	uint32_t TimeUs = 0;
	TimeUs = msHcCount*1000;
	TimeUs += TIM_GetCounter(TIM6);
	TIM_SetCounter(TIM6,0);
	return TimeUs;
}


// 五次求平均 每次之间需要延时
float Hcsr04GetLength(void)
{
	uint32_t TimeUs = 0;
	uint8_t i = 0;
	float lengthTemp = 0;
	float sum = 0;
	while(i!=5)
	{
		GPIO_SetBits(HCSR04_TRIG_PORT,HCSR04_TRIG_PIN);
		Delay_us(20);
		GPIO_ResetBits(HCSR04_TRIG_PORT,HCSR04_TRIG_PIN);
		while(GPIO_ReadInputDataBit(HCSR04_ECHO_PORT,HCSR04_ECHO_PIN)==RESET);
		OpenTimerForHc();        // 开始计数
		i++;
		while(GPIO_ReadInputDataBit(HCSR04_ECHO_PORT,HCSR04_ECHO_PIN)==SET);
		CloseTimerForHc();        // 停止计数
		TimeUs = GetEchoTimer();        // 获取时间，单位us
		lengthTemp = TimeUs/58.0;//cm
		sum = lengthTemp + sum;
		Delay_ms(10);
	}
	lengthTemp = sum/5.0;
	return lengthTemp;
}


void HCSR04_Config(void)
{
	BASIC_TIM_Init();
	HCSR04_GPIO_Config();
}
