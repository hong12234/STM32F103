#include "get_data.h"
#include "usr_wifi.h"
/***************�ⲿ��������****************/
extern  Clock My_Clock;		//ʱ����Ϣ
extern u8  read_flag;  	//���ʶ���־λ
extern u8 H_Average_Buf[12];	
extern u8 T_Average_Buf[12];
extern SYSTEM_STA System_Sta;			//ϵͳ��־λ
extern SYSTEM_DATA	System_Data;		//ϵͳ����
extern SYSTEM_SET	System_Set;			//ϵͳ����

//�������������
const u8  Data_Num[]={0x31,0x30,0x33,0x32,0x35,0x34,0x37,0x36,0x39,0x38};	//0~9
u8 Data_Ok[]={0x81,0x88,0x01,0x01,0x01,0x01,0x41,0x60,0x71,0x71,0x7A,0x4e,0x4a,0x7c,'\0'};	//ok
u8 Data_All[]={0x81,0xA4,0x01,0x01,0x01,0x01,0x41,0x60,0x71,0x71,0x72,0x64,0x6f,0x65,0x45,0x60,0x75,0x60,0x75,0x31,0x31,0x31,0x27,0x69,0x31,0x31,0x31,0x27,0x63,0x71,0x31,0x31,0x31,0x27,0x63,0x65,0x31,0x31,0x31,0x27,0x6f,0x74,'\0'};
const u8 Help_hg[]={0x69,0x66};//ˤ�����
const u8 Help_ht[]={0x69,0x75};//�������
const u8 Help_hh[]={0x69,0x69,};//�������
const u8 Help_ho[]={0x69,0x6e};//�������
const u8 Help_nu[]={0x6f,0x74};//�����
u8 	Time_AdJust[]={0x81,0x8B,0x01,0x01,0x01,0x01,0x41,0x60,0x71,0x71,0x66,0x64,0x75,0x55,0x68,0x6c,0x64,'\0'};//gettime

//��ȡ��������
//������Ҳ��������ƽ��ֵ
void Data_GetHeart(void)
{
	u8 i=0;
	u16 buf=0;
	System_Sta.Get_Heart |= (1<<15);	//���ݿ�ʼ������־λ
	if((System_Sta.Get_Heart&0x7fff)<320)		//�ȴ�2s
	{
		read_flag &= ~(1<<0);		//������ݶ�ȡ��ѡ��λ
	}
	else
	{
		i = Get_Haer_Rate();
		if((read_flag&(1<<1)) !=0 )		//�������
		{
			System_Data.heart = i;
			if(H_Average_Buf[10]==10)		//ƽ��ֵ����λ
			{
				H_Average_Buf[10] = 0;
				H_Average_Buf[11] = 1;
			}
			H_Average_Buf[H_Average_Buf[10]]=i;//����ƽ��ֵ
			H_Average_Buf[10] = (H_Average_Buf[10])+1;//ƽ��ֵ��1
			//����ƽ��ֵ
			if(H_Average_Buf[11] == 1)		//�Ѿ�����10������
			{
				for(i=0;i<10;i++)
				{
					buf = buf+H_Average_Buf[i];
				}
				System_Data.heart_average = buf/10;		//����ƽ������

			}
			else		//��δ�õ�10������
			{
				for(i=0;i<H_Average_Buf[10];i++)
				{
					buf = buf+H_Average_Buf[i];
				}
				System_Data.heart_average = buf/H_Average_Buf[10];		//����ƽ������
			}	
			System_Sta.Get_Heart = 0;		//����λ��0
			System_Sta.Get_Heart |= (1<<14);	//��������ɹ���־λ
		}
		else
		{
			if((System_Sta.Get_Heart&0x7fff)>600)	//30s�ڶ���û�в����ɹ�
			{								
				System_Sta.Get_Heart = 0;		//����λ��0
				System_Sta.Get_Heart |= (1<<13);		//����ʧ��
			}
		}
	}
}
//��ȡ��������
//������Ҳ��������ƽ��ֵ
void Data_GetTemp(void)
{
	u16 buf=0;
	u8 i=0;
	System_Sta.Get_Temp |= (1<<15);	//���ݿ�ʼ������־λ
	if((System_Sta.Get_Temp&0x7fff)<400)		//�ȴ�5s
	{
		System_Data.temperature=SMBus_ReadTemp();		//��ȡ����							
	}
	else
	{	
		//����ƽ��ֵ
		if(T_Average_Buf[10]==10)		//ƽ��ֵ����λ�����0
		{
			T_Average_Buf[10] = 0;
			T_Average_Buf[11] = 1;			//�����Ѿ�ȡ��10��־λ
		}
		T_Average_Buf[T_Average_Buf[10]]=System_Data.temperature;//����ƽ��ֵ
		T_Average_Buf[10] = (T_Average_Buf[10])+1;//ƽ��ֵ��1
		if(T_Average_Buf[11] == 1)		//�Ѿ�����10������
		{
			for(i=0;i<10;i++)
			{
				buf = buf+T_Average_Buf[i];
			}
			System_Data.temp_average = (buf+0.1)/10;		//����ƽ������
		}
		else		//��δ�õ�10������
		{
			for(i=0;i<T_Average_Buf[10];i++)
			{
				buf = buf+T_Average_Buf[i];
			}
			System_Data.temp_average = (buf+0.1)/(float)(T_Average_Buf[10]);		//����ƽ������
		}						
		System_Sta.Get_Temp = 0;		//����λ��0
		System_Sta.Get_Temp |= (1<<14);	//��������ɹ���־λ
	}	
}
//��ȡRTCʱ������
void Data_GetTime(void)
{
	RTC_Get_Time(&My_Clock.hour,&My_Clock.min,&My_Clock.sec,&My_Clock.week);
	RTC_Get_Date(&My_Clock.year,&My_Clock.month,&My_Clock.date,&My_Clock.week);
	System_Sta.Get_Time = 0;		//��0
}
//��ȡѪѹ����
u8 Data_GetBP(void)
{
	u8 i=0,j=0;
	u8 bp_buf[11];
	if(Wifi_LinkBP())
	{

		System_Sta.Measure &= ~(1<<7);	//Ѫѹ������־λ���
		return 1;
	}					
	else
	{
		delay_ms(1000);
		if(Wifi_StartBp(bp_buf))
		{
			System_Sta.Measure &= ~(1<<7);	//Ѫѹ������־λ���
			return 2;
		}
		else		//�����ɹ������ݿ�ʼ����
		{					
			for(i=0;i<7;i++)	//Ѱ����Ϣ�еĶ���
			{
				if(bp_buf[i] == ',')
				{
					j = i;		//��¼���ŵ�λ��
					break;
				}	 
			}
			if(j == 0)		//���û�ж���˵�����ݴ���
			{
				GUI_DispStringAt("data err",60,120);
				GUI_Exec();
				System_Sta.Measure &= ~(1<<7);	//Ѫѹ������־λ���
			}
			else			//���Ų�0����ʼ��ȡ����
			{
				//����ѹ��λ
				System_Data.boolpressure_up = ((bp_buf[2]-'0')*100)+((bp_buf[3]-'0')*10)+((bp_buf[4]-'0'));
				//��չѹ��λ
				System_Data.boolpressure_down = ((bp_buf[6]-'0')*100)+((bp_buf[7]-'0')*10)+((bp_buf[8]-'0'));
				System_Sta.Measure &= ~(1<<7);	//Ѫѹ������־λ���
				System_Sta.Measure |= (1<<6);	//��λ�ɹ���־λ
			}
		}
	}
	return 0;
}

u16 Get_EatTime(u8 i,u8 j,u8 k,u8 l)
{
	u8 buf1=0;
	u8 buf2=0;
	u16 buf3 = 0;
	if(i!=0)
	{
		buf1 = ((i-'0')*10)+(j-'0');
	}
	else
	{
		buf1 = (j-'0');
	}
	if(k!=0)
	{
		buf2 = ((k-'0')*10)+(l-'0');
	}
	else
	{
		buf2 = (l-'0');
	}
	
	buf3 = ((buf1<<8)|(buf2));
	return  buf3;
 }
//�����ж����Է�����������
int Data_GetServer(void)
{
	u8 buf[35];
	u16 timebuf;
	if(Wifi_DataGet(buf)){return 1;}//��ȡ����
	if(buf[0] == 'S')	//����
	{
		//��������
		if(buf[2] == 'O'){System_Set.warning_temp = 1;}		//�������ѿ�
		else
		{
			if(buf[2] == 'C')
			{
				System_Set.warning_temp = 0;//�������ѹ�
			}
			else{return 1;}
		}
		//ˤ������
		if(buf[6] == 'O'){System_Set.warning_fall  = 1;}	//ˤ��������
		else
		{
			if(buf[6] == 'C')
			{
				System_Set.warning_fall  = 0;	//ˤ��������
			}
			else{return 1;}
		}
		//��������
		if(buf[10] == 'O')	{System_Set.warning_heart = 1;}//�������ѿ�
		else
		{
			if(buf[10] == 'C')
			{
				System_Set.warning_heart  = 0;		//�������ѹ�
			}
			else{return 1;}
		}
		//��ҩ����ʱ�����������	
		//��һ��ʱ���
		if(buf[14] != '6')			
		{
			System_Set.warning_eat |= (1<<7);	//��һ��ʱ��㿪
		}
		else
		{
			if(buf[14] == '6')
			{
				System_Set.warning_eat &= ~(1<<7);	//��һ��ʱ����
			}
			else{return 1;}
		}
		//�ڶ���ʱ���
		if(buf[20] != '6')			
		{
			System_Set.warning_eat |= (1<<6);	//��һ��ʱ��㿪
		}
		else
		{
			if(buf[20] == '6')
			{
				System_Set.warning_eat &= ~(1<<6);	//��һ��ʱ��㿪
			}
			else{return 1;}
		}
		//������ʱ���
		if(buf[26] != '6')			
		{
			System_Set.warning_eat |= (1<<5);	//��һ��ʱ��㿪
		}
		else
		{
			if(buf[26] == '6')
			{
				System_Set.warning_eat &= ~(1<<5);	//��һ��ʱ��㿪
			}
			else{return 1;}
		}
		//��ʼ��ȡʱ������
		if((System_Set.warning_eat&(1<<7))!=0)		//��һ��ʱ��
		{
			timebuf = Get_EatTime(buf[14],buf[15],buf[17],buf[18]);
			System_Set.eat_time1 = (u8)(timebuf>>8);
			System_Set.eat_min1 = (u8)(timebuf&0x00ff);
		}
		if((System_Set.warning_eat&(1<<6))!=0)		//�ڶ���ʱ��
		{
			timebuf = Get_EatTime(buf[20],buf[21],buf[23],buf[24]);
			System_Set.eat_time2 = (u8)(timebuf>>8);
			System_Set.eat_min2 = (u8)(timebuf&0x00ff);
		}
		if((System_Set.warning_eat&(1<<5))!=0)		//������ʱ��
		{
			timebuf = Get_EatTime(buf[26],buf[27],buf[29],buf[30]);
			System_Set.eat_time3 = (u8)(timebuf>>8);
			System_Set.eat_min3 = (u8)(timebuf&0x00ff);
		}
		Wifi_Send2Sever(Data_Ok);
		return 0;	
	}
	else 
	{
		if((buf[0] == 'g')&&(buf[3]=='D'))		//��ȡ���ݣ��������������GET�ֱ������
		{
			System_Sta.Send_Start = 1;
			return 0;
		}
		else 
		{
			if((buf[0] == 'c')&&(buf[1]=='o'))
			{
				System_Sta.App_Sta = 1 ;		//APP�Ѿ�����
				return 0;
			}
			else
			{
				if((buf[0] == 'u')&&(buf[1]=='n'))
				{
					System_Sta.App_Sta = 0 ;		//APP�Ͽ�����
					return 0;
				}
			}
		}
	}
	return 1;//��Ч������
}


//�������ݱ���
//sta 0:�������Ϣ 1:ˤ�� 2:���� 3:���� 4:����
int Data_Convert(float Temp,u8 Heart,u8 BP,u8 BD,u8 sta)
{
	u16 buf = (u16)(Temp*10);
	Data_All[19]= Data_Num[buf/100];		//Ƕ���¶�����
	Data_All[20]= Data_Num[buf%100/10];
	Data_All[21]= Data_Num[buf%10];
	
	Data_All[24]= Data_Num[Heart/100];		//Ƕ����������
	Data_All[25]= Data_Num[Heart%100/10];
	Data_All[26]= Data_Num[Heart%10];
	
	Data_All[30]= Data_Num[BP/100];			//Ƕ������ѹ
	Data_All[31]= Data_Num[BP%100/10];
	Data_All[32]= Data_Num[BP%10];
	
	Data_All[36]= Data_Num[BD/100];			//Ƕ��������ѹ
	Data_All[37]= Data_Num[BD%100/10];
	Data_All[38]= Data_Num[BD%10];
	
	switch(sta)
	{
		case 0:Data_All[40]=Help_nu[0];		//û�����
			   Data_All[41]=Help_nu[1];
			   break;	
		case 1:Data_All[40]=Help_hg[0];		//ˤ�����
			   Data_All[41]=Help_hg[1];
			   break;		
		case 2:Data_All[40]=Help_hh[0];		//�������
			   Data_All[41]=Help_hh[1];
			   break;		
		case 3:Data_All[40]=Help_ht[0];		//�������
			   Data_All[41]=Help_ht[1];
				break;		
		case 4:Data_All[40]=Help_ho[0];		//�������
			   Data_All[41]=Help_ho[1];
			   break;
		default:break;
	}
	return 0;
}

//����ʱ��У׼����
//���������ص����ݸ�ʽΪYXXRXXHXXMXX
int Watch_Get_Time(void)
{
	u8 buf[15],i=0;
	u8 mon=0,date=0,hour = 0,min = 0;
	if(Wifi_Send2Sever(Time_AdJust)){return 1;}
	if(Wifi_DataWait(1000)){return 1;}//10s
	if(Wifi_DataGet(buf)){return 1;}
	if(buf[0]=='Y')		//�·�
	{
		mon = (buf[1]-'0')*10+(buf[2]-'0');
	}
	if(buf[3]=='R')		//����
	{
		date = (buf[4]-'0')*10+(buf[5]-'0');
	}
	if(buf[6]=='H')		//Сʱ
	{
		hour = (buf[7]-'0')*10+(buf[8]-'0');
	}
	if(buf[9]=='M')		//��
	{
		min = (buf[10]-'0')*10+(buf[11]-'0');
	}
	if(hour>12)
	{
		i = 1;
	}
	RTC_Set_Time(hour,min,0,1);		//����ʱ��
	i =RTC_Get_Week(2016,mon,date);	//��ȡ������Ϣ
	RTC_Set_Date(16,mon,date,i);	//�������·�
	return 0;
}
