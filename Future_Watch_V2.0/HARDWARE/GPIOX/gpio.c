#include "gpio.h"
/*
* �������ƣ�GPIO_Init��
* ����˵����GPIO��ʼ����
* ��ڲ�����IO���ĸ��˿�
*           PORT_NUM���˿�λ��
*           MODE���˿�ģʽ
*           SPEED���˿��ٶ�
*			OTY	 :�����©
*			PUPD��������������
*           IO_DR���˿ڵ�ƽ
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void GPIO_Init(GPIO_TypeDef * IO,u8 PORT_NUM,u8 MODE,u8 SPEED,u8 OTY,u8 PUPD,u8 IO_DR)
{
	//�˿�ģʽ����
	IO->MODER &= (~(1<<(PORT_NUM*2))) & (~(1<<(PORT_NUM*2+1)));		//�����Ӧλ
	IO->MODER |= (MODE<<(PORT_NUM*2));							//����or�������
	//�˿��ٶ�����
	IO->OSPEEDR &= (~(1<<(PORT_NUM*2))) & (~(1<<(PORT_NUM*2+1)));		//�����Ӧλ
	IO->OSPEEDR |= (SPEED<<(PORT_NUM*2));					//�����ٶ�
	//����������߿�©
	if(MODE == GPIO_MODE_OUT)
	{
		if(OTY)
		{
			IO->OTYPER |= (1<<PORT_NUM);		//��©
		}
		else
		{
			IO->OTYPER &= ~(1<<PORT_NUM);		//����
		}
		if(IO_DR)
		{
			IO->ODR |= (1<<PORT_NUM);	//��λ
		}
		else
		{
			IO->ODR &= ~(1<<PORT_NUM);//��0
		}
	}
	else
	{
		if(IO_DR)
		{
			IO->IDR |= (1<<PORT_NUM);	//��λ
		}
		else
		{
			IO->IDR &= ~(1<<PORT_NUM);//��0
		}
	}
	//����������������
	IO->PUPDR &= (~(1<<(PORT_NUM*2))) & (~(1<<(PORT_NUM*2+1)));		//�����Ӧλ
	IO->PUPDR |= (PUPD<<(PORT_NUM*2));

}

/*
* �������ƣ�GPIO_Clock_Set��
* ����˵����GPIOʱ��ʹ�ܣ�
* ��ڲ�����IOPxEN->IOPAEN--IOPEEN,�˿�ʱ�ӣ�
*           EN_PULL_OR_OUT-->ODR���ã�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void GPIO_Clock_Set(u8 IOPxEN)
{
	RCC->AHB1ENR|=(1<<IOPxEN);                       //�����˿�ʱ�ӣ�
}



