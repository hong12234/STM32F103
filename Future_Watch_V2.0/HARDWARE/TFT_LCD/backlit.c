#include "backlit.h"


void TIM3_PWM_Init(u32 arr,u32 psc)
{
	RCC->APB1ENR|=1<<1; 	//TIM3ʱ��ʹ��    
	RCC->AHB1ENR|=1<<1;   	//ʹ��PORTBʱ��	
	GPIO_Set(GPIOB,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//���ù���,�������
	GPIO_AF_Set(GPIOB,0,2);	//PB0,AF2 
	
	TIM3->ARR=arr;			//�趨�������Զ���װֵ 
	TIM3->PSC=psc;			//Ԥ��Ƶ������Ƶ 
	TIM3->CCMR2|=6<<4;  	//CH3 PWM1ģʽ		 
	TIM3->CCMR2|=1<<3; 		//CH3 Ԥװ��ʹ��	   
	TIM3->CCER|=1<<8;   	//OC1 ���ʹ��	
	TIM3->CCER &= ~(1<<9);   	//OC1 �͵�ƽ��Ч	   
	TIM3->CR1|=1<<7;   		//ARPEʹ�� 
	TIM3->CR1|=1<<0;    	//ʹ�ܶ�ʱ��3
}



//��ʼ��ADC3
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ADC3_CH5																	   
void  Adc3_Init(void)
{    
	//�ȳ�ʼ��IO��
 	RCC->APB2ENR|=1<<10;   	//ʹ��ADC3ʱ��  
	RCC->APB2RSTR|=1<<8;   	//ADCs��λ(ע��,����ADC���ᱻ��λ.)
	RCC->APB2RSTR&=~(1<<8);	//��λ����	 
	ADC->CCR=1<<16;			//ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz
 	 
	ADC3->CR1=0;   			//CR1��������
	ADC3->CR2=0;   			//CR2��������
	ADC3->CR1|=0<<24;      	//12λģʽ
	ADC3->CR1|=0<<8;    	//��ɨ��ģʽ	
	
	ADC3->CR2&=~(1<<1);    	//����ת��ģʽ
 	ADC3->CR2&=~(1<<11);   	//�Ҷ���	
	ADC3->CR2|=0<<28;    	//�������
	
	ADC3->SQR1&=~(0XF<<20);
	ADC3->SQR1|=0<<20;     	//1��ת���ڹ��������� Ҳ����ֻת����������1 			   
	//����ͨ��5�Ĳ���ʱ��
	ADC3->SMPR1&=~(7<<(3*1));//ͨ��11����ʱ�����	  
 	ADC3->SMPR1|=7<<(3*1); 	//ͨ��11  480������,��߲���ʱ�������߾�ȷ��	
 	ADC3->CR2|=1<<0;	   	//����ADת����	  
}	 
//���ADCֵ
//ch:ͨ��ֵ 0~16
//����ֵ:ת�����
u16 Get_Adc3(u8 ch)   
{
	//����ת������	  		 
	ADC3->SQR3&=0XFFFFFFE0;//��������1 ͨ��ch
	ADC3->SQR3|=ch;		  			    
	ADC3->CR2|=1<<30;       //��������ת��ͨ�� 
	while(!(ADC3->SR&1<<1));//�ȴ�ת������	 	   
	return ADC3->DR;		//����adcֵ	
} 
 
 
//��ʼ������������
void Lsens_Init(void)
{
	RCC->AHB1ENR|=1<<2;    	//ʹ��PORTCʱ��	  
	GPIO_Set(GPIOC,PIN1,GPIO_MODE_AIN,0,0,GPIO_PUPD_PU);//PF7,ģ������,����   
	Adc3_Init();
}
//��ȡLight Sens��ֵ
//0~100:0,�;100,���� 
u8 Lsens_Get_Val(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_Adc3(LSENS_ADC_CHX);	//��ȡADCֵ
		delay_ms(5);
	}
	temp_val/=LSENS_READ_TIMES;//�õ�ƽ��ֵ 
	if(temp_val>4000)temp_val=4000;
	return (u8)(100-(temp_val/40));
}

