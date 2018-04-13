#include "timer_4.h"
#include "led.h"
#include "delay.h"
#include "wdg.h"
#include "rtc.h"
#include "main.h"

extern SYSTEM_STA System_Sta;
//��ʱ��3�жϷ������	 
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//����ж�
	{
		IWDG_Feed();		//ι��
		System_Sta.Get_Time++;		//ʱ��
		System_Sta.Get_Heart++;		//����
		System_Sta.Get_Temp++;		//����
		System_Sta.Get_Tumble++;	//ˤ��
		System_Sta.Get_backlit++;	//����
		System_Sta.Check_Health++;		//���£����ʣ���鱨��ʱ��
		System_Sta.Help_Time++;
		if((KEY_CHECK) != 0)		//�ߵ�ƽ����������
		{
			System_Sta.Check_Key++;		//ÿ+1��ʾ��ʱ��100ms
			if(System_Sta.Check_Key>60)	//i����60 ˵���ߵ�ƽ�Ѿ�������6sִ�йػ�
			{
				System_Sta.Motor |= (1<<7);	//����һ����
				OSIntEnter();  
				GUI_SetBkColor(GUI_WHITE);	//���ñ���ɫ
				GUI_Clear();
				GUI_SetFont(&GUI_Font32_ASCII);
				GUI_SetColor(GUI_BLACK);
				GUI_DispStringAt("turn off..",60,120);
				GUI_Exec();
				delay_ms(500);
				Save_Data();	//��������
				STOP_MCU;		//�ػ�
			}
		}
		else			//�����ɿ�
		{
			if(System_Sta.Check_Key>30)//����3s ��������
			{
				System_Sta.Motor |= (1<<7);	//����һ����
				System_Sta.Warning |= (1<<4);
				System_Sta.Check_Key=0;
			} 
			else
			{
				if(System_Sta.Check_Key>2)		//����������
				{
					System_Sta.Motor |= (1<<7);	//����һ����
					if((System_Sta.Disp_Sta&(1<<5))==0)
					{
						System_Sta.Get_backlit |= (1<<7);	//�رձ������
						TIM3->CCR3 = 0;
						System_Sta.Disp_Sta |= (1<<5);		//����
					}
					else
					{
							System_Sta.Get_backlit &= ~(1<<7);		//�����������
							TIM3->CCR3 = 500;
							System_Sta.Disp_Sta &= ~(1<<5);		//����
					}
						System_Sta.Check_Key = 0;
				}
				else
				{
					System_Sta.Check_Key = 0;//��0
				}																			 					
			}
		}
	}				   
	TIM4->SR&=~(1<<0);//����жϱ�־λ 	    
}
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void Time_4_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;	//TIM4ʱ��ʹ��    
 	TIM4->ARR=arr;  	//�趨�������Զ���װֵ 
	TIM4->PSC=psc;  	//Ԥ��Ƶ��	  
	TIM4->DIER|=1<<0;   //��������ж�	  
	TIM4->CR1|=0x01;    //ʹ�ܶ�ʱ��3								 
}














