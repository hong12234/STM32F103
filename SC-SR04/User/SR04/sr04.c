#include "sr04.h"
#include "bsp_TIMbase.h"
#include "delay.h"

extern volatile uint16_t msHcCount;

// tips:static����������������ڶ�������Դ�ļ�
static void HCSR04_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(HCSR04_TRIG_CLK|HCSR04_ECHO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = HCSR04_TRIG_PIN;// �����ź�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;// �������
	GPIO_Init(HCSR04_TRIG_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(HCSR04_TRIG_PORT,HCSR04_TRIG_PIN);// ��ʼ���������
	
	GPIO_InitStructure.GPIO_Pin = HCSR04_ECHO_PIN;// �����ź�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;// ��������
	GPIO_Init(HCSR04_ECHO_PORT, &GPIO_InitStructure);
}


static void OpenTimerForHc()        // �򿪶�ʱ��
{
	TIM_SetCounter(TIM6,0);// �������
	msHcCount = 0;
	TIM_Cmd(TIM6, ENABLE);  // ʹ�ܼ���
}
 
static void CloseTimerForHc()        // 
{
	TIM_Cmd(TIM6, DISABLE);  // 
}

// ��ȡ��ʱ��ʱ��
static uint32_t GetEchoTimer(void)
{
	uint32_t TimeUs = 0;
	TimeUs = msHcCount*1000;
	TimeUs += TIM_GetCounter(TIM6);
	TIM_SetCounter(TIM6,0);
	return TimeUs;
}


// �����ƽ�� ÿ��֮����Ҫ��ʱ
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
		OpenTimerForHc();        // ��ʼ����
		i++;
		while(GPIO_ReadInputDataBit(HCSR04_ECHO_PORT,HCSR04_ECHO_PIN)==SET);
		CloseTimerForHc();        // ֹͣ����
		TimeUs = GetEchoTimer();        // ��ȡʱ�䣬��λus
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
