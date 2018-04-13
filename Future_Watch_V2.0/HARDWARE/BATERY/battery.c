#include "battery.h"


//�������Ĵ�Adc��ֵ
u16 AdcDma_Buf;//�������DMA��ֵ

//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ADC1_CH5																	   
void  Adc_Init(void)
{    
	//�ȳ�ʼ��IO��
 	RCC->APB2ENR|=1<<8;    	//ʹ��ADC1ʱ�� 
	RCC->AHB1ENR|=1<<2;    	//ʹ��PORTAʱ��	  
	GPIO_Set(GPIOC,PIN0,GPIO_MODE_AIN,0,0,GPIO_PUPD_PU);	//PA5,ģ������,����   

	RCC->APB2RSTR|=1<<8;   	//ADCs��λ
	RCC->APB2RSTR&=~(1<<8);	//��λ����	 
	ADC->CCR=3<<16;			//ADCCLK ʱ����ԴΪPLL 168M��8��Ƶ21MHz
							//ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz
 	
	ADC1->CR1=0;   			//CR1��������
	ADC1->CR2=0;   			//CR2��������
	
	ADC1->CR1|=0<<24;      	//12λģʽ
	ADC1->CR1|=0<<8;    	//��ɨ��ģʽ	
	
	ADC1->CR2|=1<<8;		//ʹ��DMAת��ʹ��
	ADC1->CR2|=1<<9;		//DDS��ÿ��ת����ɺ�DMA=1ʱ������DMA����
	
	ADC1->CR2|=(1<<1);    	//����ת��ģʽ 23.4usһ��
 	ADC1->CR2&=~(1<<11);   	//�Ҷ���	
	ADC1->CR2|=0<<28;    	//������� ʹ�ܺ�����������ת��ģʽ��������ת��
	
	
	
//	ADC1->SQR1&=~(0XF<<20);
//	ADC1->SQR1|=0<<20;     		//1��ת���ڹ��������� Ҳ����ֻת����������1 			   
	 
	//����ͨ��5�Ĳ���ʱ��  23.4usһ�� Time = (����ʱ��+12)*(1/ADCCLK) 
	ADC1->SMPR1&=~(7<<(3*0));	//ͨ��10����ʱ�����	  
 	ADC1->SMPR1|=7<<(3*0); 		//ͨ��10  480������,��߲���ʱ�������߾�ȷ��	 
 	ADC1->CR2|=1<<0;	   		//����ADת����	
	
	ADC1->SQR3&=0XFFFFFFE0;	//��������1 ͨ��ch
	ADC1->SQR3|=10;		  			    
	ADC1->CR2|=1<<30;       //��������ת��ͨ�� 

}				  
//���ADCֵ
//ch:ͨ��ֵ 0~16
//����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	//����ת������	  		 
	ADC1->SQR3&=0XFFFFFFE0;//��������1 ͨ��ch
	ADC1->SQR3|=10;		  			    
	ADC1->CR2|=1<<30;       //��������ת��ͨ�� 
	while(!(ADC1->SR&1<<1));//�ȴ�ת������	 	   
	return ADC1->DR;		//����adcֵ	
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u16 adc[],u8 times)
{
	u8 i,j;
	u16 adc_buf;
	for(i=0;i<(times-1);i++)
	{
		for(j=0;j<times-i;j++)
		{
			if(adc[j]>adc[j+1])
			{
				adc_buf = adc[j+1];
				adc[j+1]= adc[j];
				adc[j] = adc_buf;
			}
		}
	}
	adc_buf=0;
	for(i=1;(i<times-1);i++)
	{
		adc_buf += adc[i];
	}
	adc_buf = adc_buf/(times-2);
	return adc_buf;
}  
