#include "usart_2.h"
#include "includes.h"
u16 AT_RX_STA=0;                   //ATģʽ����״̬��־,B15:������ɱ�־;B14:���յ�'O',B13��
u16 DATA_RX_STA=0;                   //͸��ģʽ����ת̬��־��
u8 Rxd_DataBuf[70];
u8 data_len = 0;			//���ݳ���
u16	OK_Sta = 0;
u8 SEND_AT_FLAG = 1;
/*
* �������ƣ�Usart_2_Init��
* ����˵����Usart2��ʼ����
* ��ڲ�����pclk1->PCLK1ʱ��Ƶ�����36��bound->�����ʣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void Usart_1_Init(u32 pclk1,u32 bound)
{
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);   //�õ�USARTDIV
	mantissa=temp;                            //�õ���������
	fraction=(temp-mantissa)*16;              //�õ�С������	 
	mantissa<<=4;
	mantissa+=fraction; 

	RCC->AHB1ENR|=1<<0;   	//ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<4;  	//ʹ�ܴ���1ʱ�� 

	GPIO_Set(GPIOA,PIN9|PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
 	GPIO_AF_Set(GPIOA,9,7);	//PA9,AF7
	GPIO_AF_Set(GPIOA,10,7);//PA10,AF7  	   
	RCC->APB2RSTR|=1<<4;                     //��λ��
	RCC->APB2RSTR&=~(1<<4);                  //ֹͣ��λ��
	//����������
 	USART1->BRR=mantissa; 	//����������	 
	USART1->CR1&=~(1<<15); 	//����OVER8=0 
	USART1->CR1|=1<<3;  	//���ڷ���ʹ�� 
	//ʹ�ܽ����ж� 
	USART1->CR1|=1<<2;  	//���ڽ���ʹ��
	USART1->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 
	USART1->CR1|=1<<13;  	//����ʹ��
	
}
/*
* �������ƣ�Usart_2_Send��
* ����˵����Usart2���ڷ��ͣ�
* ��ڲ�����data->���͵����ݣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void Usart_1_Send(u8 data)
{
	while((USART1->SR&0X40)==0);      //ѭ������,ֱ���������   
	USART1->DR = (u8)data;
}


void Set_Next(u8 x,u8 y)
{
//	UT_RX_STA |=  (1<<y);	//��һλ��1
//	UT_RX_STA &= ~(1<<x);	//��λ��0
}
/*
* �������ƣ�USART2_IRQHandler��
* ����˵����Usart2�����жϷ�������
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ��ATģʽ����"OK"Ϊ������־��δ�����鵽�����־��
*           ͸��ģʽ
*/
void USART1_IRQHandler(void)
{
	u8 rxd;	
	OSIntEnter();   
	if(USART1->SR&(1<<5))                                //���յ�����,�����־λ��
	{	
		rxd = USART1->DR;  						 //��ȡ����
		if(SEND_AT_FLAG == 0) 					//ATģʽ��ȡOK����
		{                              
			AT_RX_STA++;						//ͨ�������ж�O��K�ļ��
			if((rxd == 'o')||(rxd == 'O'))
			{
				AT_RX_STA |= (1<<12);			//��ʮ��λΪ ���յ�O�ı�־λ
				OK_Sta = AT_RX_STA;				//�ӵ�0
			}		
			else
			{
				if((rxd == 'k')||(rxd == 'K'))					//�ж��Ƿ�ӵ�K
				{
					if(AT_RX_STA == (OK_Sta+1))	//OK���������յ���
					{
						AT_RX_STA |= 0x8000;	//�ɹ���־	
						AT_RX_STA &= 0x8000;	//���˳ɹ�λ��������0
						OK_Sta = 0;
					}
					else						//OK�����������յ���
					{
						AT_RX_STA = 0;			//����λ��0
					}
				}
			}
		}
		else
		{
			if((DATA_RX_STA & 0x8000) != 0)			//���ݽ��յ���
			{
				Rxd_DataBuf[data_len]=rxd;			
				data_len++;
				if((rxd == '\r') || (rxd == '}'))		//���ݽ��ս���
				{
					DATA_RX_STA = 0;
					DATA_RX_STA |=  0x4000;	//���ݽ��ճɹ���־λ
					data_len = 0;
				}
			}
			else
			{
				if((rxd == '=')|| (rxd == '{'))	//=��Ķ�������
				{
					DATA_RX_STA = 0; 
					DATA_RX_STA |= 0x8000 ;		//��ʼ�������� 
				}
			}
		}
		USART1->SR&= ~(1<<5);
	}
	OSIntExit();  
}
	
