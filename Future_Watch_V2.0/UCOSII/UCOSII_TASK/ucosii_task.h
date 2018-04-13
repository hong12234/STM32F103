#ifndef _UCOSII_H_
#define _UCOSII_H_
#include "main.h"
#include "display.h"
/**************************�ⲿ��������************************************/
extern TouchPointRefTypeDef TPR_Structure;
extern  Clock My_Clock;
extern SYSTEM_STA System_Sta;	
extern u8  read_flag;  
extern SYSTEM_DATA System_Data;
extern u8 H_Average_Buf[12];
extern u8 T_Average_Buf[12];
extern u8 SEND_AT_FLAG;
extern u16 DATA_RX_STA;
extern SYSTEM_SET System_Set;
extern u8 Data_All[];
extern u8 Data_Ok[];
extern u8 Help_hg[];
extern u8 Help_ht[];
extern u8 Help_hh[];
extern u8 Help_ho[];
extern u16 AdcDma_Buf;
/**************************�ⲿ��������************************************/
extern u16 Get_Adc_Average(u16 adc[],u8 times);
extern WM_HWIN CreateWindow(void);
/**************************ȫ�ֱ���****************************************/

/****************************UCOSII�ź���********************************/


//OS_EVENT * sem_smartlink;		//WIFI�ź���ָ��
//OS_EVENT * sem_getdata;		//get�������ݵ��ź���ָ��
//OS_EVENT * sem_weather;		//��ʼ����������ʾ
//OS_EVENT * sem_wifiover;
//OS_EVENT * sem_wifilink;
//OS_EVENT * sem_dataover;
/**************************UCOSII�����ź���******************************/
//OS_EVENT * Data_SendMute;
/*************************UCOSII��������********************************/
//START����
//������������ȼ�
#define START_TASK_PRIO				3
//�����ջ��С 
#define START_STK_SIZE			  64
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//Wifi_Task����
//�����������ȼ�
#define WIFI_TASK_PRIO			7
//�����ջ��С
#define WIFI_STK_SIZE			128
//������ƿ�
OS_TCB WifiTaskTCB;
//�����ջ
OS_STK WIFI_TASK_STK[WIFI_STK_SIZE];
//Display_Task����
void Wifi_Task(void *pdata);

//�������
//�����������ȼ�
#define BATTERY_TASK_PRIO 				10
//�����ջ��С
#define BATTERY_STK_SIZE				128
//������ƿ�
OS_TCB BATTERYTaskTCB;
//�����ջ
OS_STK BATTERY_TASK_STK[BATTERY_STK_SIZE];
//�������
void Battery_Task(void *pdata);

//Display_Task����
//�����������ȼ�
#define DISPLAY_TASK_PRIO			9
//�����ջ��С
#define DISPLAY_STK_SIZE			512
//������ƿ�
OS_TCB DisplayTaskTCB;
//�����ջ
OS_STK DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//Display_Task����
void Display_Task(void *pdata);

//Touch_Task
//�����������ȼ�
#define TOUCH_TASK_PRIO 	4	
//�����ջ��С
#define TOUCH_STK_SIZE		128
//������ƿ�
OS_TCB TouchTaskTCB;
//�����ջ
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//Touch_Task����
void Touch_Task(void *pdata);

//UpDate_Task
//�����������ȼ�
#define UPDATE_TASK_PRIO 	8
//�����ջ��С
#define UPDATE_STK_SIZE		128
//������ƿ�
OS_TCB UpDateTaskTCB;
//�����ջ
OS_STK UPDATE_TASK_STK[UPDATE_STK_SIZE];
//Touch_Task����
void UpDate_Task(void *pdata);

//Warning_Task
//�����������ȼ�
#define WARNING_TASK_PRIO 	5
//�����ջ��С
#define WARNING_STK_SIZE		128
//������ƿ�
OS_TCB Warning_TaskTCB;
//�����ջ
OS_STK WARNING_TASK_STK[WARNING_STK_SIZE];
//Touch_Task����
void Warning_Task(void *pdata);

/********************************ϵͳ������****************************/
//����&��������
void Warning_Task(void *pdata)
{
	while(1)
	{	
		Check_Fall();		//ɨ��ˤ��		
		Action_Task();		//����ɨ��
		if(System_Set.warning_fall == 1)
		{
			if(System_Sta.Tumble_Flag != 0)		//ˤ������
			{
				if((System_Sta.Warning&0x60) !=0)	//��������������ڷ�����������ֹ��ˤ��ӵ��������ȼ�
				{
					System_Sta.Help_Time = 0;		//�����ȴ���0
					System_Sta.Warning |= (1<<3);
					System_Sta.Tumble_Flag = 0;													
					System_Sta.Disp_Sta |= (1<<1);//��ʾ������												
					System_Sta.Disp_Sta &= ~(1<<2);
					System_Sta.Disp_Sta &= ~(1<<3);
					System_Sta.Motor |= (1<<5);	//���ֹͣ����
				}
				System_Sta.Warning |= (1<<7) ;		//����״̬
				System_Sta.Help_Time = 0;		//�����ȴ���0
			}
		}
		if((System_Set.warning_eat&0xe0)!=0)
		{
			if((System_Sta.Warning&(0xe0))==0)		//δ�б������
			{
				if((System_Set.warning_eat&0x80) != 0)
				{
					if((System_Set.eat_time1 == My_Clock.hour) &&(System_Set.eat_min1 == My_Clock.min))
					{
							System_Sta.Remind |= (1<<7);		//���ѳ�ҩ����
					}
					else
					{
						System_Sta.Remind &= ~(1<<6);	//������ѹ���־λ
						System_Sta.Remind &= ~(1<<7);
					}
				}
				if((System_Set.warning_eat&0x40) != 0)
				{
					if((System_Set.eat_time2 == My_Clock.hour) &&(System_Set.eat_min2 == My_Clock.min))
					{
						System_Sta.Remind1 |= (1<<7);		//���ѳ�ҩ����
					}
					else
					{
						System_Sta.Remind1 &= ~(1<<6);	//������ѹ���־λ
						System_Sta.Remind1 &= ~(1<<7);
					}
				}
				if((System_Set.warning_eat&0x20) != 0)
				{	
					if((System_Set.eat_time3 == My_Clock.hour) &&(System_Set.eat_min3 == My_Clock.min))
					{
						System_Sta.Remind2 |= (1<<7);		//���ѳ�ҩ����
					}
					else
					{
						System_Sta.Remind2 &= ~(1<<6);	//������ѹ���־λ
						System_Sta.Remind2 &= ~(1<<7);
					}
				}
			}
		}
		if(System_Sta.Check_Health > 600)	//1���Ӽ��һ��
		{
			if(System_Set.warning_heart == 1)		//���ʱ���
			{
				if(((System_Sta.Warning&(1<<7))==0)&&((System_Sta.Warning&(1<<5))==0))//��������δ��
				{
					if((System_Data.heart_average >100) || (System_Data.heart_average<50))
					{
						System_Sta.Warning |= (1<<6) ;		//����״̬
						System_Sta.Help_Time = 0;		//�����ȴ���0
					}
				}
			}
			if(System_Set.warning_temp == 1)		//���±���
			{
				if(((System_Sta.Warning&(1<<7))==0)&&((System_Sta.Warning&(1<<6))==0))//��������δ��
				{
					if((System_Data.temp_average>(float)37.5) || (System_Data.temp_average<36))
					{
						System_Sta.Warning |= (1<<5) ;		//����״̬
						System_Sta.Help_Time = 0;		//�����ȴ���0
					}
				}
			}
			System_Sta.Check_Health = 0;
		}
		if(System_Sta.unlock == 1)
		{
			System_Sta.Disp_Sta &= ~(1<<5);		//����
			System_Sta.Get_backlit &= ~(1<<7);		//�����������
			TIM3->CCR3 = 500;
			System_Sta.unlock = 0;
			System_Sta.Motor |= (1<<7);
		}		

		if((System_Sta.Motor&(1<<7))!=0)		//��
		{
			Motor_Work();
		}
		delay_ms(10);
	}
}

//ϵͳ���ݸ�������
void UpDate_Task(void *pdata)
{
	u8 i;
	while(1)
	{
		if(System_Sta.Get_Time > 5)		//500ms����һ��
		{
			Data_GetTime();				//ʱ���ȡ
		}		
		if((System_Sta.Get_Heart&0x0fff)>300)		//30s����һ������
		{	
			Data_GetHeart();			//���ʻ�ȡ
		}
		if((System_Sta.Get_Temp&0x0fff)>300)		//30s����һ������
		{	
			Data_GetTemp();		//�¶Ȼ�ȡ
		}
		if(((System_Sta.Get_backlit&0x7f)>10)&&((System_Sta.Get_backlit&(1<<7))==0))		//1s��һ�±���
		{
			i = Lsens_Get_Val();
			TIM3->CCR3 = (i*10);
			System_Sta.Get_backlit &= 0x80;	//�������λ
		}
		else
		{
			if((System_Sta.Get_backlit&0x7f)>0x0f)
			{
				System_Sta.Get_backlit &= 0x80;	//�������λ
			}
		}
		if(((System_Sta.Warning&0xe0)!=0)&&(System_Sta.Help_Time>300))		//�б����������
		{
			if(System_Sta.App_Sta == 0)
			{
				System_Sta.Tumble_Flag = 0;
				System_Sta.Warning = 0;
				System_Sta.Disp_Sta |= (1<<1);//��ʾ������												
				System_Sta.Disp_Sta &= ~(1<<2);
				System_Sta.Disp_Sta &= ~(1<<3);
				System_Sta.Motor |= (1<<5);//���ֹͣ����
			}
			else
			{
				System_Sta.Warning |= (1<<3);
				System_Sta.Tumble_Flag = 0;													
				System_Sta.Disp_Sta |= (1<<1);//��ʾ������												
				System_Sta.Disp_Sta &= ~(1<<2);
				System_Sta.Disp_Sta &= ~(1<<3);
				System_Sta.Motor |= (1<<5);	//���ֹͣ����
			}
		}
		if(System_Sta.Help_Time>30000)
		{
			System_Sta.Help_Time = 0;		//������0
		}
		delay_ms(10);
	}
}

//WIFI������
void Wifi_Task(void *pdata)
{
	while(1)
	{
		if((System_Sta.Wifi_Connection&(1<<7))!=0)		//����smartlink;
		{
			System_Sta.Wifi_Connection |= (1<<3);
			if(Wifi_SmartLink()){System_Sta.App_Sta = 0;System_Sta.Wifi_Connection |= (1<<6);}//ʧ��
			else//�ɹ�
			{	
				Wifi_LinkServer();//���������
				delay_ms(200);
				if(Watch_Get_Time()){/*У׼ʧ��*/}//ʱ��У׼
				System_Sta.Wifi_Connection |= (1<<5);
			}
			System_Sta.Wifi_Connection &= ~(1<<7);	//���smartlink��־λ
			Wifi_InUDP();		//��������͸��ģʽ
		}
		if((DATA_RX_STA&0x4000)!=0)		//���յ�����
		{
			if(Data_GetServer()){GUI_DispStringAt("server err",120,120);GUI_Exec();}//���ݴ���
			else{/*Wifi_Send2Sever(Data_Ok);*/}//����OK��������
			Wifi_InUDP();		//��������͸��ģʽ
		}
		if(System_Sta.Send_Start == 1)
		{
			//���ݱ���
			Data_Convert(System_Data.temp_average,System_Data.heart_average,System_Data.boolpressure_up,System_Data.boolpressure_down,0);
			//��������
			if(Wifi_Send2Sever(Data_All)){GUI_DispStringAt("server err",60,120);GUI_Exec();}	
			System_Sta.Send_Start = 0;		//������ݷ��͵���������־λ
			DATA_RX_STA = 0;
			Wifi_InUDP();		//��������͸��ģʽ
		}
		if((System_Sta.Measure&(1<<7))!=0)		//��ʼ����
		{							
			if(Data_GetBP()){Wifi_LinkServer();/*��ȡʧ��*/}
			Wifi_InUDP();		//��������͸��ģʽ
		}
		if((System_Sta.Warning&(1<<3)) != 0 )
		{
			
			switch(System_Sta.Warning&0xf0)
			{
				//ˤ�����
				case 0x80:System_Sta.Warning = 0;
						  Data_Convert(System_Data.temp_average,System_Data.heart_average,System_Data.boolpressure_up,System_Data.boolpressure_down,1);
						  Wifi_Send2Sever(Data_All);
						  break;
				//�������
				case 0x40:System_Sta.Warning = 0;
						  Data_Convert(System_Data.temp_average,System_Data.heart_average,System_Data.boolpressure_up,System_Data.boolpressure_down,2);
						  Wifi_Send2Sever(Data_All);
						  break;	
				//�¶����
				case 0x20:System_Sta.Warning = 0;
						  Data_Convert(System_Data.temp_average,System_Data.heart_average,System_Data.boolpressure_up,System_Data.boolpressure_down,3);
						  Wifi_Send2Sever(Data_All);
						  break;
				//�������
				case 0x10:System_Sta.Warning = 0;
						  Data_Convert(System_Data.temp_average,System_Data.heart_average,System_Data.boolpressure_up,System_Data.boolpressure_down,4);
						  Wifi_Send2Sever(Data_All);
						  break;
				default:break;
			}
			Wifi_InUDP();		//��������͸��ģʽ
		}
		delay_ms(50);
	}
	
}


//��ʾ����
void Display_Task(void *pdata)
{	
	WM_HWIN head;		//��ǰ��ʾ�ľ��
	while(1)	
	{			
		if((System_Sta.Disp_Sta&(1<<5)) == 0)		//��������
		{			
			if(((System_Sta.Warning&0xf0)!= 0) && ((System_Sta.Disp_Sta&(1<<0))==0))//��������
			{
					System_Sta.Motor = 0 ;
					System_Sta.Motor |= ((1<<6)|(1<<7));		//����￪��
					System_Sta.Help_Time = 0;		//�����ȴ���0
					Help_Window(&head);		//��������
			}
			else
			{
				if((((System_Sta.Remind&(1<<7))!=0) &&((System_Sta.Remind&(1<<6))==0)) || (((System_Sta.Remind1&(1<<7))!=0) &&((System_Sta.Remind1&(1<<6))==0))||(((System_Sta.Remind2&(1<<7))!=0) &&((System_Sta.Remind2&(1<<6))==0)))//���ѽ���
				{
					if((System_Sta.Disp_Sta&(1<<0))==0)
					{
						System_Sta.Motor = 0 ;
						System_Sta.Motor |= ((1<<6)|(1<<7));	//����￪��
						Remind_Window(&head);		//���ѽ���
					}
				}
				else
				{ 
					if((System_Sta.Disp_Sta&(1<<1))!=0)	 //������
					{	
						System_Sta.Wifi_Connection = 0;
						Main_Window(&head);
					}
					else
					{
						if(((System_Sta.Disp_Sta&(1<<2))!=0) &&((System_Sta.Disp_Sta&(1<<7))!=0))//Wifi���ý���
						{
							Wifi_Window(&head);		//WIFI����
						}
						else
						{
							if(((System_Sta.Disp_Sta&(1<<3))!=0) &&((System_Sta.Disp_Sta&(1<<7))!=0))//������ʾ����
							{		
								Data_Window(&head);	//���ݽ���
							}
						}
					}
				}
			}
		}
		else		//����״̬
		{		
			Disp_lockWindow(&head);		//�������洦��
		}		
		if(((System_Sta.Disp_Sta&(1<<3))!=0)&&((System_Sta.Disp_Sta&(1<<7))==0))//��ǰ���ݽ���
		{	
			Disp_Update(&head);		//���ݽ�����Ϣ��ʾ
		}
		else
		{
			if(((System_Sta.Disp_Sta&(1<<2))!=0) &&((System_Sta.Disp_Sta&(1<<7))==0))	//��ǰΪWifi����
			{
				if((System_Sta.Wifi_Connection&(1<<3))!=0)		//��ʼ����
				{
					Disp_Connection(&head);
				}
			}
		}
		GUI_Exec();		
		delay_ms(10);
	}
	
}
//��ص����������
void Battery_Task(void *pdata)
{
	u8 i,j;
	u16 adc_buf;
	u16 adc[20];
	u16 adc_new[10];
	float temp;
	while(1)
	{	
		for(j=0;j<6;j++)	//10sȡһ��ADֵ
		{
			for(i=0;i<20;i++)		
			{
				adc[i] = AdcDma_Buf;	//��DMA��ȡ����
			}
			adc_new[j]=Get_Adc_Average(adc,20);		//ð�������˲�
			delay_ms(1000);		//1sȡ��ʱ��
		}
		adc_buf=Get_Adc_Average(adc_new,6);		//ð�������˲�
		temp=((float)adc_buf)*(3.300/4096); 	//ת����ĵ�ѹ
		temp -= (float)0.05;		//��� ������Ҫ�ٲ���
		temp *= 2;			//���������ѹ��*2=��ʵ��ѹ 		
		if(temp>(float)3.9)		//����
		{
			System_Sta.battery &= ~(1<<6);
			System_Sta.battery &= ~(1<<5);
			System_Sta.battery |= (1<<7);		//�����־λ
		}
		else
		{
			if(temp<(float)3.9 && temp>(float)3.7)		//�м����
			{
				System_Sta.battery &= ~(1<<7);
				System_Sta.battery &= ~(1<<5);
				System_Sta.battery |= (1<<6);			//�м����
				
			}
			else				//û��
			{
				System_Sta.battery &= ~(1<<7);
				System_Sta.battery &= ~(1<<6);
				System_Sta.battery |= (1<<5);			//û����
			}
		}
	}

}

//����ɨ������
void Touch_Task(void *pdata)
{	
	while(1)
	{
		GUI_TOUCH_Exec();
		if(((TPR_Structure.Gesture_Sta &(1<<6)) != 0)&((TPR_Structure.Gesture_Sta &(1<<7)) != 0))	//���Ʒ��� ���������ж�
		{		
			if((System_Sta.Warning == 0)&&((System_Sta.Remind&(1<<7)) == 0))
			{
				TPR_Structure.Gesture_Sta |= (1<<(Gesture_Scan()));		//��־����λ
				switch(TPR_Structure.Gesture_Sta&0x0f)
				{	
					//����
					case 0x01:	if((System_Sta.Disp_Sta&(1<<5)) == 0)
								{
									if((System_Sta.Disp_Sta&(1<<1)) != 0)		//������/*�����ټ�һ����־λ�ж��Ƿ�������״̬*/
									{
										System_Sta.Disp_Sta &= ~(1<<1);
										System_Sta.Disp_Sta |= (1<<2);		//���󻬶�������WIfi���ӽ���
										System_Sta.Disp_Sta |= (1<<7);
									}
									else
									{
										if((System_Sta.Disp_Sta&(1<<3)) != 0)	//������ʾ����
										{
											System_Sta.Disp_Sta &= ~(1<<3);
											System_Sta.Disp_Sta |= 1<<1;		//����������
											System_Sta.Disp_Sta |= (1<<7);
										}
									}
								}
								TPR_Structure.Gesture_Sta &= ~(1<<0);
								break;
					//���Ͻ���		
					case 0x02:	if((System_Sta.Disp_Sta&(1<<5)) != 0)
								{
									System_Sta.Motor |= (1<<7);//����һ����
									System_Sta.Disp_Sta &= ~(1<<5);		//���������־λ
//									System_Sta.Disp_Sta |= (1<<1);
//									System_Sta.Disp_Sta &= ~((1<<2)|(1<<3));
									System_Sta.Get_backlit &= ~(1<<7);	//�����������
									TIM3->CCR3 = 500;
									//GPIOB->BSRRL |=(1<<0);	
								}
								TPR_Structure.Gesture_Sta &= ~(1<<1);
								break;
					//����
					case 0x04:	if((System_Sta.Disp_Sta&(1<<5)) == 0)
								{
									if((System_Sta.Disp_Sta&(1<<1))!=0)		//������
									{
										System_Sta.Disp_Sta &= ~(1<<1);
										System_Sta.Disp_Sta |= (1<<3);		//������ʾ����
										System_Sta.Disp_Sta |= (1<<7);
									}
									else
									{
										if((System_Sta.Disp_Sta&(1<<2))!=0)		//WIFI���ý���
										{
											System_Sta.Disp_Sta &= ~(1<<2);
											System_Sta.Disp_Sta |= (1<<1);		//����������
											System_Sta.Disp_Sta |= (1<<7);
										}
									}
								}
								TPR_Structure.Gesture_Sta &= ~(1<<2);
								break;
					//��������
					case 0x08:	if((System_Sta.Disp_Sta &(1<<5)) == 0)
								{
									System_Sta.Motor |= (1<<7);//����һ����
									//System_Sta.Disp_Sta &= 0xe0;
									System_Sta.Disp_Sta |= (1<<5);
									System_Sta.Get_backlit |= (1<<7);		//�رձ������
									TIM3->CCR3 = 0;
									//GPIOB->BSRRH |=(1<<0);
								}
								TPR_Structure.Gesture_Sta &= ~(1<<3);
								break;
					default : break;	
				}
					TPR_Structure.Gesture_Sta &= ~(1<<7);			//������Ʒ�����־λ
					TPR_Structure.Gesture_Sta &= ~(1<<6);			//������Ʒ�����־λ
			}
			else
			{
				TPR_Structure.Gesture_Sta &= ~(1<<7);			//������Ʒ�����־λ
				TPR_Structure.Gesture_Sta &= ~(1<<6);			//������Ʒ�����־λ
			}
		
		}
		//GUI_Exec();	
		delay_ms(5);
	}										 
}	  


//��ʼ����
void start_task(void *pdata)
{	
//	u8 err;
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 	
//	sem_smartlink = OSSemCreate(0);		//smart link �ź���
	OSStatInit();					//��ʼ��ͳ������.�������ʱ1��������	
 	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
	OSTaskCreate(Display_Task,(void *)0,(OS_STK *)&DISPLAY_TASK_STK[DISPLAY_STK_SIZE-1],DISPLAY_TASK_PRIO);
	OSTaskCreate(Wifi_Task,(void *)0,(OS_STK *)&WIFI_TASK_STK[WIFI_STK_SIZE-1],WIFI_TASK_PRIO);	
	OSTaskCreate(Touch_Task,(void *)0,(OS_STK *)&TOUCH_TASK_STK[TOUCH_STK_SIZE-1],TOUCH_TASK_PRIO);		
	OSTaskCreate(UpDate_Task,(void *)0,(OS_STK *)&UPDATE_TASK_STK[UPDATE_STK_SIZE-1],UPDATE_TASK_PRIO);	
 	OSTaskCreate(Battery_Task,(void *)0,(OS_STK *)&BATTERY_TASK_STK[BATTERY_STK_SIZE-1],BATTERY_TASK_PRIO);
	OSTaskCreate(Warning_Task,(void *)0,(OS_STK *)&WARNING_TASK_STK[WARNING_STK_SIZE-1],WARNING_TASK_PRIO);	
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}

#endif
