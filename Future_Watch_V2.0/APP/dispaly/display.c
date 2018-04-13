#include "display.h"
#include "main.h"
extern Clock My_Clock;
extern SYSTEM_STA System_Sta;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontmyfont;
extern SYSTEM_DATA System_Data;
extern u8 H_Average_Buf[12];
extern u8 T_Average_Buf[12];
extern GUI_CONST_STORAGE GUI_BITMAP bmwifiok;
extern GUI_CONST_STORAGE GUI_BITMAP bmwifino;

void Main_Window(WM_HWIN *head)
{
	if((System_Sta.Disp_Sta&(1<<6))!= 0)	//����Ǵ�WIFI���ý���ת������ɾ��WIFI����
	{
		System_Sta.Disp_Sta &= ~(1<<6);
		WM_DeleteWindow(*head);
		GUI_Clear();
	}
	if((System_Sta.Disp_Sta&(1<<4))!= 0)
	{
		System_Sta.Disp_Sta &= ~(1<<4);
		WM_DeleteWindow(*head);
		GUI_Clear();
	}
	if((System_Sta.Disp_Sta&(1<<0))!= 0)
	{
		System_Sta.Disp_Sta &= ~(1<<0);//��־λ��0
		WM_DeleteWindow(*head);
		GUI_Clear();
	}
	Disp_Watch();
}

void Wifi_Window(WM_HWIN *head)
{
	*head = Create_WifiWindow();
	GUI_Exec();
	WM_SelectWindow(*head);
	WM_EnableMemdev(*head);
	System_Sta.Disp_Sta &= ~(1<<7);			//�����־λ���������ظ���ʾ
	System_Sta.Disp_Sta |= (1<<6);			//��־����ʾ��־λ
}

void Data_Window(WM_HWIN *head)
{
	GUI_SetBkColor(GUI_RED);
	*head = Create_DataWindow();
	WM_SelectWindow(*head);
	Disp_Data(head);
	GUI_Exec();
	WM_EnableMemdev(*head);
	System_Sta.Disp_Sta &= ~(1<<7);	//�����־λ���������ظ���ʾ
	System_Sta.Disp_Sta |= (1<<4);	//��־����ʾ��־λ
}

void Help_Window(WM_HWIN *head)
{
	if((System_Sta.Disp_Sta&(1<<1)) == 0)		//��ǰ����������
	{
		System_Sta.Disp_Sta &= ~(1<<6);		//��������־λ���������ظ�ɾ����ˢ��
		System_Sta.Disp_Sta &= ~(1<<4);
		System_Sta.Disp_Sta &= ~(14<<1);	//�������ʾ��־λ
		WM_DeleteWindow(*head);					//�����������ɾ��֮ǰ��ʾ�Ĵ���
		*head = Create_HelpWindow();			
		WM_EnableMemdev(*head);				//����ʹ�����Ľ��治������
		System_Sta.Disp_Sta |= (1<<0);		//��λ��־λ������Ϊ�˷�ֹ�ظ�ˢ�����Լ��ر�����		
	}
	else	//��������
	{
		*head = Create_HelpWindow();	//ֱ����ʾ����ɾ��
		GUI_Exec();
		WM_EnableMemdev(*head);	
		System_Sta.Disp_Sta |= (1<<0);		//��λ��־λ
		System_Sta.Disp_Sta &= ~(1<<1);		//����������־λ	
	}
}

void Remind_Window(WM_HWIN *head)
{
	if((System_Sta.Disp_Sta&(1<<1)) == 0)		//��ǰ����������
	{
		System_Sta.Disp_Sta &= ~(1<<6);		//��������־λ���������ظ�ɾ����ˢ��
		System_Sta.Disp_Sta &= ~(1<<4);
		System_Sta.Disp_Sta &= ~(14<<1);	//�������ʾ��־λ
		WM_DeleteWindow(*head);					//�����������ɾ��֮ǰ��ʾ�Ĵ���
		*head = Create_MedicineWindow();			
		WM_EnableMemdev(*head);				//����ʹ�����Ľ��治������
		System_Sta.Disp_Sta |= (1<<0);		//��λ��־λ������Ϊ�˷�ֹ�ظ�ˢ�����Լ��ر�����		
	}	
	else	//��������
	{
		*head =  Create_MedicineWindow();	//ֱ����ʾ����ɾ��
		GUI_Exec();
		WM_EnableMemdev(*head);	
		System_Sta.Disp_Sta |= (1<<0);		//��λ��־λ
		System_Sta.Disp_Sta &= ~(1<<1);		//����������־λ	
	}
}

//����ʱ�Ľ��洦��
int Disp_lockWindow(WM_HWIN *head)
{
	if((System_Sta.Warning&0xe0)!= 0)	//��������
	{
		System_Sta.Get_backlit &= ~(1<<7);		//�����������
		TIM3->CCR3 = 500;
		delay_ms(20);
		System_Sta.Disp_Sta &= ~(1<<5);		//���������־λ 
	}
	else
	{
		if((((System_Sta.Remind&(1<<7))!=0) &&((System_Sta.Remind&(1<<6))==0)) || (((System_Sta.Remind1&(1<<7))!=0) &&((System_Sta.Remind1&(1<<6))==0))||(((System_Sta.Remind2&(1<<7))!=0) &&((System_Sta.Remind2&(1<<6))==0)))
		{
			System_Sta.Get_backlit &= ~(1<<7);		//�����������
			TIM3->CCR3 = 500;
			delay_ms(20);
			System_Sta.Disp_Sta &= ~(1<<5);		//���������־λ 
		}
	}
	return 0 ;
}
//�������ݽ���ĸ���
void Disp_Update(WM_HWIN *head)
{
	//���������ڲ������������
	if(((System_Sta.Get_Heart&(1<<15))!=0)||((System_Sta.Get_Temp&(1<<15))!=0)||((System_Sta.Measure&(1<<7))!=0))
	{
		Disp_Circle(head);//���ڲ�����ʾ��־
	}
	else 
	{
		if(((System_Sta.Get_Heart&(1<<14))!=0)||((System_Sta.Get_Temp&(1<<14))!=0)||((System_Sta.Measure&(1<<6))!=0))		//�����ɹ�
		{
			Disp_Data(head); //����ҳ������
			System_Sta.Get_Heart &= ~(1<<14);	//����ɹ���־λ
			System_Sta.Get_Temp &= ~(1<<14);
			if((System_Sta.Measure&(1<<6))!=0)
			{
				System_Sta.Measure &= ~(1<<6);		
			}
		}
		else
		{
			if(((System_Sta.Get_Heart&(1<<13))!=0)||((System_Sta.Get_Temp&(1<<13))!=0))
			{
					GUI_SetColor(GUI_WHITE);
					GUI_FillCircle(5, 5, 5);
					GUI_FillCircle(3, 86, 5);
				//GUI_FillCircle(5, 170, 5);   
				//GUI_DispStringAt("")
				/*����ʧ��������*/
			}
		}
	}
}


//��ʾ��������
void Disp_Data(WM_HWIN *head)
{
	u16 buf;
//	u8 i = 0;
	//GUI_Clear();
	GUI_SetBkColor(GUI_WHITE);
	GUI_ClearRect(137,83,240,158);		//������ֵ����
	GUI_ClearRect(137,3,240,78);		//������ֵ����
	GUI_ClearRect(164,163,240,240);		//Ѫѹ����
	GUI_SetColor(GUI_WHITE);
	GUI_FillCircle(5, 5, 5);
	GUI_FillCircle(3, 86, 5);
	GUI_FillCircle(5, 170, 5);         
	GUI_SetColor(GUI_RED);
	GUI_SetFont(&GUI_Font32_ASCII);
	//WM_SelectWindow(*head);
	//����
	buf = ((u16)(System_Data.temperature*10))%10;//��ȡС�����һλ
	GUI_DispDecAt((u8)System_Data.temperature,137,3,2);	
	GUI_DispStringAt(".",166,3);
	GUI_DispDecAt(buf,172,3,1);
	//��ʾ����ƽ��ֵ
	buf = ((u16)(System_Data.temp_average*10))%10;//��ȡС�����һλ
	GUI_DispDecAt((u8)System_Data.temp_average,137,43,2);	
	GUI_DispStringAt(".",166,43);
	GUI_DispDecAt(buf,172,43,1);
		
	//����
	//��ʾ����ƽ��ֵ
	if((System_Data.heart_average/100) != 0)		//��ֵ3λ��
	{
		GUI_DispDecAt(System_Data.heart_average,137,123,3);		//��ʾ����
	}
	else
	{
		GUI_DispDecAt(System_Data.heart_average,137,123,2);
	}
	//��ʾ����ֵ
	if((System_Data.heart/100) != 0)	//��λ��
	{
		
		GUI_DispDecAt(System_Data.heart,137,83,3);		//��ʾ����
	}
	else
	{
		GUI_ClearRect(137,83,175,113);
		GUI_DispDecAt(System_Data.heart,137,83,2);		//��ʾ����
	}
	//Ѫѹ
	GUI_DispDecAt((System_Data.boolpressure_up),164,163,3);		//����ѹ
	GUI_DispDecAt((System_Data.boolpressure_down),164,203,2);	//����ѹ
	/*��ʾ����*/
	
	//��������
	GUI_SetFont(&GUI_Fontmyfont);
	GUI_DispStringAt("\xe5\xba\xa6",187,3);		//��
	if(System_Data.temperature>37)		
	{
		GUI_DispStringAt("\xe9\xab\x98",215,20);//��ʾ��
	}
	else
	{
		if(System_Data.temperature<36)
		{
			GUI_DispStringAt("\xe4\xbd\x8e",215,20);//��ʾ��
		}
	}
	
	//��������
	if(System_Data.heart>101)		
	{
		GUI_DispStringAt("\xe9\xab\x98",215,105);//��ʾ��
	}
	else
	{
		if(System_Data.heart<55)
		{
			GUI_DispStringAt("\xe4\xbd\x8e",215,108);//��ʾ��
		}
	}
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_DispStringAt("/bpm",185,83);
	GUI_DispStringAt("/bpm",185,137);
	GUI_Exec();
}

//��ʾ���ڲ����ı�ʶλ
void Disp_Circle(WM_HWIN *head)
{
	static u8 times ;
	GUI_SetColor(GUI_RED);
	times++;
	if((System_Sta.Get_Temp&(1<<15))!=0)		//�����ڲ���
	{
		if(times < 25)
		{
			GUI_SetColor(GUI_RED);
			GUI_FillCircle(5, 5, 5);
		}
		else
		{
			if(times <50)
			{
				GUI_SetColor(GUI_WHITE);
				GUI_FillCircle(5, 5, 5);
			}
			else
			{
				times = 0;
			}
		}
	}
	if((System_Sta.Get_Heart&(1<<15))!=0)		//�����ڲ���
	{	
		if(times < 25)
		{
			GUI_SetColor(GUI_RED);
			GUI_FillCircle(3, 86, 5);
		}
		else
		{
			if(times < 50)
			{
				GUI_SetColor(GUI_WHITE);
				GUI_FillCircle(3, 86, 5);	
			}
			else
			{
				times = 0;
			}
		}
	}
	if((System_Sta.Measure&(1<<7))!=0 )		//Ѫѹ�ڲ���
	{
		if(times < 25)
		{
			GUI_SetColor(GUI_RED);
			GUI_FillCircle(5, 170, 5);
		}
		else
		{
			if(times < 50)
			{
				GUI_SetColor(GUI_WHITE);
				GUI_FillCircle(5, 170, 5);
			}
			else
			{
				times = 0;
			}
		}
	}
	GUI_Exec();
}



void Disp_Connection(WM_HWIN *head)
{
	static u8 times = 0;
	WM_HWIN hItem;
	if((System_Sta.Wifi_Connection&(1<<4))==0)	//��һ�ν���
	{
		times = 0;
		System_Sta.Wifi_Connection |= (1<<4);		//�����ʾ�Ѿ���ʾ������
		GUI_SetFont(&GUI_Fontmyfont);
		GUI_SetColor(GUI_RED);
		GUI_DispStringAt("Wifi\xe9\x93\xbe\xe6\x8e\xa5\xe4\xb8\xad",50,200);
	}
	GUI_SetFont(&GUI_Font32_ASCII);
	switch(times/50)
	{
		case 0: GUI_DispStringAt(".",180,203);break;
		case 1:	GUI_DispStringAt(".",190,203);break;
		case 2: GUI_DispStringAt(".",200,203);break;
		case 3:	GUI_DispStringAt(".",210,203);break;
		case 4: GUI_DispStringAt(".",220,203);break;
		default:times=1;
				GUI_SetColor(GUI_WHITE);
				GUI_FillRect(180,190,240,240);
				GUI_SetColor(GUI_RED);
				break;
	}
	times++;
	if((System_Sta.Wifi_Connection&(1<<6)) != 0)
	{
		System_Sta.Wifi_Connection = 0;
		hItem = WM_GetDialogItem(*head, 0x800);
		BUTTON_SetBitmapEx(hItem,0,&bmwifino,0,0);
		Disp_No();
	}
	else
	{
		if((System_Sta.Wifi_Connection&(1<<5)) != 0)
		{
			System_Sta.Wifi_Connection = 0;
			hItem = WM_GetDialogItem(*head, 0x800);
			BUTTON_SetBitmapEx(hItem,0,&bmwifiok,0,0);
			Disp_Ok();
		}
	}
		
}

void Disp_Ok(void)
{
	GUI_SetColor(GUI_WHITE);
	GUI_FillRect(50,200,240,240);
	GUI_SetFont(&GUI_Fontmyfont);
	GUI_SetColor(GUI_RED);
	GUI_DispStringAt("\xe9\x93\xbe\xe6\x8e\xa5\xe6\x88\x90\xe5\x8a\x9f!",60,200);
}

void Disp_No(void)
{
	GUI_SetColor(GUI_WHITE);
	GUI_FillRect(50,200,240,240);
	GUI_SetFont(&GUI_Fontmyfont);
	GUI_SetColor(GUI_RED);
	GUI_DispStringAt("\xe9\x93\xbe\xe6\x8e\xa5\xe5\xa4\xb1\xe8\xb4\xa5!",60,200);
}

void Motor_Work(void)
{
	static u8 times;
	if((System_Sta.Motor&(1<<6))!= 0)//������ʱ����
	{
		if(times<80)
		{
			MOTOR_START;
		}
		else
		{
			if(times<120)
			{
				MOTOR_STOP;
			}
			else
			{
				times = 0;
			}
			if((System_Sta.Motor&(1<<5))!=0)
			{
				System_Sta.Motor = 0;
				MOTOR_STOP;
				times = 0;
			}
		}
	}
	else
	{		
		if(times<10)
		{
			MOTOR_START;
		}
		else 
		{
			MOTOR_STOP;
			times = 0;
			System_Sta.Motor = 0;
		}
	}
	times++;
}
