#include "usr_wifi.h"
#include "GUI.h"
#include "get_data.h"
extern u8 SEND_AT_FLAG;
extern u16 DATA_RX_STA;
extern u8 Rxd_DataBuf[70];
/******************USR_WIFI����*********************/
//WIFI�����й�����
const u8 Wifi_ENTM[]={"AT+ENTM"};		//����͸��ģʽ
const u8 Wifi_OutTM[]={"+++"};		//�˳�͸��ģʽ
const u8 Wifi_SendA[]={"a"};
const u8 Wifi_SendZ[]={"AT+Z"};		//����
const u8 Wifi_KeyCheck[]={"AT+WSKEY"};//��ȡ�˺�
const u8 Wifi_IDCheck[]={"AT+WSSSID"};//��ȡ����
const u8 Wifi_SMTLK[]={"AT+SMTLK"};//smartlink
const u8 Wifi_KeySet[]={"AT+WSKEY="};//�����˺�
const u8 Wifi_IdSet[]={"AT+WSSSID="};//��������
const u8 Wifi_Enter[]={"\r\n"};
//�������й�����
const u8 Wifi_WSSSID[]={"BloodTest"};		//Ѫѹ���˺�
const u8 Wifi_WSKEY[]={"WPA2PSK,AES,12345678"};		//Ѫѹ����
const u8 Wifi_BPTCP[]={"AT+NETP=TCP,Client,5000,192.168.4.1"};	//����Э�飬ģʽ���˿ڣ�IP��ַ
const u8 Wifi_ServerTCP[]={"AT+NETP=TCP,Client,80,183.61.166.98"};	
const u8 Wifi_CheckLink[]={"AT+TCPLK"};				//�������������Ƿ���
const u8 Wifi_SetTcpLink[]={"AT+TCPDIS=ON"};		//����TCP����
//�������йص�����
const u8 Wifi_HTPChcek[]={"AT+HTPEN"};
const u8 Wifi_HTPOFF[]={"AT+HTPEN=OFF"};			//�ر�HTP
//websocket����
const u8 Wifi_Get[]={"GET /websocket?xx2 HTTP/1.1"};
const u8 Wifi_Host[]={"Host: myxtc910839.java.jspee.cn"};
const u8 Wifi_Upgrade[]={"Upgrade: websocket"};
const u8 Wifi_Connection[]={"Connection: keep-alive"};
const u8 Wifi_WBKey[]={"Sec-WebSocket-Key: CMZruIgSKwXQgWonViF6Fg=="};
const u8 Wifi_WBPro[]={"Sec-WebSocket-Protocol: char,superchat"};
const u8 Wifi_WBVer[]={"Sec-WebSocket-Version: 13"};

/**********ȫ�ֱ�������*******************/
_Wifi_Information My_Wifi;
//u8 Key_Test[]={"WPA2PSK,AES,123456789"};
//u8 Id_Test[]={"Future"};

/*
* �������ƣ�Wifi_SendAt
* ����˵��������WifiAtָ��
* ��ڲ�����At���� 
* ����ֵ  ��0:�ɹ�
* ��ע    ���ޣ�
*/

int Wifi_SendAt(const u8 Commond[]){
	Wifi_Send_Str(Commond);
	return 0;
}

/*
* �������ƣ�Wifi_Send_Str��
* ����˵���������ַ�����
* ��ڲ������ޣ� 
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
int Wifi_Send_Str(const u8* p){
	while(*p!='\0')
	{
		Usart_1_Send(*p);
		p++;
	}
	return 0;
}

/*
* �������ƣ�Wait_AtRxd
* ����˵�����ȴ�����OK
* ��ڲ�������ʱ�ȴ�ʱ�� time*100ms  time���ֵ65536
* ����ֵ  ��1:��ʱδ���� 0�����ճɹ�
* ��ע    ���ޣ�
*/
int Wait_AtRxd(u16 time){
	u16 i = 0;
	while(i<time)
	{
		if((AT_RX_STA&0x8000) != 0)		//���ճɹ�
		{
			AT_RX_STA = 0 ;		//��־λ��0
			return 0;
		}
		i++;
		delay_ms(10);
	}
	return 1;
}

/*
* �������ƣ�Wifi_Init
* ����˵����WIFI��ʼ��
* ��ڲ�������;
* ����ֵ  ����;
* ��ע    ����ʼ��������Ĭ�Ͻ�HTTP��
*/
int Wifi_Init(void){
	
	//u8 AT1231[]={"AT+CHKTIME=60\r\n"};
	u8  i =0;
	GPIO_Clock_Set(IOPCEN);
	GPIO_Init(GPIOC,6,IO_MODE_OUT,IO_SPEED_100M,IO_OTYPER_PP,IO_KEEP,IO_H);		//REST
	GPIO_Init(GPIOC,7,IO_MODE_IN,IO_SPEED_50M,IO_OTYPER_PP,IO_KEEP,IO_H);		//LINK
	GPIO_Init(GPIOC,8,IO_MODE_IN,IO_SPEED_50M,IO_OTYPER_PP,IO_KEEP,IO_H);		//READY
	
	My_Wifi.Wifi_Sta = 0;
	
	while(!((GPIOC->IDR&(1<<8)) == 0))		//������WIFI�Ƿ�����
	{
		delay_ms(100);
		if(i>100){break;}		//�ȴ�10s
		i++;
	}
	i = 0;
	while(!((GPIOC->IDR&(1<<7)) == 0))		//�ȴ�WIFI��������
	{
		delay_ms(100);
		if(i>50){break;}		//�ȴ�5s
		i++;
	}
	if(Wifi_OutUDP())	//����ָ��ģʽ,���ﱣ֤һ���ɹ�
	{
		Wifi_OutUDP();
	}
//	Wifi_SendAt(AT1231);
	if((GPIOC->IDR&(1<<7)) == 0)		//�Ѿ��ɹ�����
	{
		//���ȼ���ǲ�������Ѫѹ��
		Wifi_GetId();
		if(!((My_Wifi.Wifi_Id[0] == 'B')&(My_Wifi.Wifi_Id[5] == 'T')))		
		{
			My_Wifi.Wifi_Sta |= (1<<7);		//WIFI������
			Wifi_GetKey();					//��ȡ�����ӵ�����
			delay_ms(1000);
			Wifi_GetId();					//��ȡ�����ӵ��˺�
			delay_ms(1000);
			if(Wifi_LinkServer())			//����WEBSOCKET����
			{
				Wifi_LinkServer();			
			}
		}
	}
	Wifi_InUDP();		//ֻ��������ģʽ���ܴӷ�����������
	
	return 0;
}

/*
* �������ƣ�Wifi_DataWait
* ����˵�����ȴ����ݽ������
* ��ڲ������ȴ���ʱ��times*100ms
* ����ֵ  ��0 �ɹ� 1ʧ��
* ��ע    ���ޣ�
*/
int Wifi_DataWait(u16 times)
{
	u16 i= 0;
	while((DATA_RX_STA&0x4000) == 0)		//�ȴ����ݽ��ճɹ�
	{
		delay_ms(10);
		i++;
		if(i>times)				//�ȴ�times*100ms
		{
			return 1;			//����
		}
	}
	DATA_RX_STA = 0;			//��־λ��0
	return 0;
}

/*
* �������ƣ�Wifi_CheckSta
* ����˵�����жϽ��յ�����ON����OFF
* ��ڲ�������
* ����ֵ  ��1 ON 2 OFF
* ��ע    ���ޣ�
*/
int Wifi_CheckSta(u16 times)
{
	u8 i=0;
	while(i<times)
	{
		if((DATA_RX_STA&0x4000) != 0)		//���ճɹ�
		{
			if(((Rxd_DataBuf[0]=='O')&(Rxd_DataBuf[1]=='N'))||((Rxd_DataBuf[0]=='o')&(Rxd_DataBuf[1]=='n')))	//��Сд����
			{
				DATA_RX_STA = 0 ;	
				return 0;		//��ȡ��ON
			}
			else
			{
				DATA_RX_STA = 0 ;	
				return 1;		//��ȡ��OFF
			}	
		}
		i++;
		delay_ms(100);
	}
		AT_RX_STA = 0 ;	
		return 2;	
}

/*
* �������ƣ�Wifi_DataGet
* ����˵������ȡWIFI���յ�������
* ��ڲ�����������ݵ�����
* ����ֵ  ����
* ��ע    ���ޣ�
*/
int Wifi_DataGet(u8 *pstr)
{
	u8 i;
	for(i=0;i<200;i++)
	{
		if((Rxd_DataBuf[i] == '\r')||(Rxd_DataBuf[i] == '}'))		//��ȡID
		{
			DATA_RX_STA = 0;			//��־λ��0
			*pstr = '\0';
			return 0;
		}
		*pstr = Rxd_DataBuf[i];
		pstr++;
	}
	return 1;
}

/*
* �������ƣ�Wifi_Send2Sever
* ����˵����WIFI�����ݵ�������
* ��ڲ�������ŵ��ַ������ַ
* ����ֵ  ��0�ɹ���1��������������ʧ��
* ��ע    ���ޣ�
*/
int Wifi_Send2Sever(u8 *pstr)
{
	
	if(Wifi_LinkServer())
	{
		return 1;
	}

	if((My_Wifi.Wifi_Sta&(1<<4)) != 0)			//�ж�������ģʽ���������ݴ���ģʽ
	{
		Wifi_InUDP();
	}
	SEND_AT_FLAG = 0;
	Wifi_SendAt(pstr);			//��������
	if(Wait_AtRxd(1000))	//��������ʧ��
	{
		Wifi_ReStart();
		delay_ms(5000);
		Set_WebSocket();			//���½���WEB����
		delay_ms(1000);
		Wifi_SendAt(pstr);			//�ٳ��Է�һ��
		if(Wait_AtRxd(1000)){SEND_AT_FLAG = 1;return 2;}		//�ڶ���ʧ��ֱ���˳�
	}
	SEND_AT_FLAG = 1;
	return 0;
}

/*
* �������ƣ�Wifi_ReStart
* ����˵����Wifi����
* ��ڲ�������
* ����ֵ  ����
* ��ע    ���ޣ�
*/
//void Wifi_Reload(void)
//{
//	u8 i = 0;
//	GPIOD->ODR &= ~(1<<4);		//PD4���� �����ָ���������
//	delay_ms(5000);				//����3s������
//	GPIOD->ODR |= (1<<4);		//PD4����
//	delay_ms(2000);				//�ȴ��ȶ�
//	while((GPIOD->IDR&(1<<5)) != 0)		//P5Ϊ�ߣ�ģ��Ϊ����
//	{
//		delay_ms(1000);
//		if(i>50){break;}		//�ȴ�5s
//		i++;
//	}
//	delay_ms(2000);				//�ȴ��ȶ�
//	while((GPIOD->IDR&(1<<5)) != 0)		//P5Ϊ�ߣ�ģ��Ϊ����
//	{
//		delay_ms(1000);
//		if(i>50){break;}		//�ȴ�5s
//		i++;
//	}
//	My_Wifi.Wifi_Sta &= ((1<<5)) |((1<<6))|((1<<3))|((1<<2));		//�˺�����λ����0
//}

/*
* �������ƣ�Wifi_OutTM
* ����˵����Wifi����ָ��ģʽ
* ��ڲ�������
* ����ֵ  ����
* ��ע    ���ޣ�
*/
int Wifi_OutUDP(void)
{
	if((My_Wifi.Wifi_Sta&(1<<4)) == 0)			//��ǰΪ͸��ģʽ
	{
		SEND_AT_FLAG = 0;
		Wifi_SendAt(Wifi_OutTM);	//Wifi��������ģʽ
		delay_ms(1500);				//3s����Ҫ��Ӧa
		Wifi_SendAt(Wifi_SendA);		
		if(Wait_AtRxd(500))
		{
			Wifi_ReStart();
			SEND_AT_FLAG = 1;
			return 1;
		}//ʧ������
		else{My_Wifi.Wifi_Sta |= (1<<4);}	//�ȴ�5s,�����λ�ɹ�����ģʽ
		SEND_AT_FLAG = 1;
		return 0;
	}
	return 0;
}

/*
* �������ƣ�Wifi_InUDP
* ����˵����Wifi����͸��
* ��ڲ�������
* ����ֵ  ����
* ��ע    ���ޣ�
*/
int Wifi_InUDP(void)			
{	
	if((My_Wifi.Wifi_Sta&(1<<4)) != 0)			//��ǰΪ����ģʽ
	{		
		SEND_AT_FLAG = 0;	
		Wifi_SendAt(Wifi_ENTM);			//����͸��ģʽ
		Wifi_SendAt(Wifi_Enter);		
		if(Wait_AtRxd(500)){Wifi_ReStart();SEND_AT_FLAG = 1;return 1;}//�ȴ�����ͳɹ�
		else{My_Wifi.Wifi_Sta &= ~(1<<4);}		//��λ͸��ģʽ��־λ
		SEND_AT_FLAG = 1;
		return 0;
	}
	return 0;
}

/*
* �������ƣ�Wifi_Link
* ����˵����WIFI���˺ź�����
* ��ڲ�������
* ����ֵ  ����
* ��ע    ���ޣ�
*/
int Wifi_Link(u8 *IdStr,u8 *KeyStr)
{
	if((My_Wifi.Wifi_Sta&(1<<4)) == 0)		//�����͸��ģʽ
	{
		if((Wifi_OutUDP()))		//����ָ��ģʽ
		{
			Wifi_OutUDP();
		}
	}
	SEND_AT_FLAG = 0;
	delay_ms(1000);
	Wifi_SendAt(Wifi_KeySet);//��������
	Wifi_SendAt(KeyStr);
	Wifi_SendAt(Wifi_Enter);
	if(Wait_AtRxd(500)){SEND_AT_FLAG = 1;return 1;}//�ȴ�����OK
	delay_ms(2000);
	Wifi_SendAt(Wifi_IdSet);//�����˺�
	Wifi_SendAt(IdStr);
	Wifi_SendAt(Wifi_Enter);
	if(Wait_AtRxd(500)){SEND_AT_FLAG = 1;return 2;}		//�ȴ�����OK
	delay_ms(500);	
	if(Wifi_ReStart()){Wifi_ReStart();SEND_AT_FLAG = 1;return 3;} //Wifi����
	SEND_AT_FLAG = 1;
	return 0;
}

/*
* �������ƣ�Wifi_ReStart
* ����˵��������
* ��ڲ�������
* ����ֵ  ����
* ��ע    ���ޣ�
*/
int Wifi_ReStart(void)
{
	u8 i=0;
	u8 i_am_test[]={"123\r\n"};
	Wifi_InUDP();
	Wifi_SendAt(i_am_test);
	GPIOC->ODR &= ~(1<<6); //����
	delay_ms(100);
	GPIOC->ODR |= (1<<6);//��������
	My_Wifi.Wifi_Sta &= ((1<<5))|((1<<6))|((1<<2))|((1<<7))|((1<<1));	/*�����7λ��Ҫȥ��*/	//�˺������־λ HTTP��־λ
	delay_ms(2000); //2ms�ȴ�ģ���ȶ�
	while(!((GPIOC->IDR&(1<<8)) == 0))			//�ȴ�ģ���������
	{
		delay_ms(100);
		if(i>100){return 1;}		//�ȴ�10s
		i++;
	}
	delay_ms(2000);			//�ȴ�ģ���ȶ�
	return 0;
}


/*
* �������ƣ�Wifi_SmartLink
* ����˵����������������ģʽ
* ��ڲ�������
* ����ֵ  ����
* ��ע    ���ޣ�
*/
int Wifi_SmartLink(void)
{
	u8 times=0;
	u8 i_am_test[]={"123"};
	Wifi_InUDP();
	Wifi_SendAt(i_am_test);
	//Wifi_ReStart();
	if((My_Wifi.Wifi_Sta&(1<<4)) == 0)			//�ж�������ģʽ
	{
		if(Wifi_OutUDP())
		{
			Wifi_OutUDP();
		}
	}
	
	SEND_AT_FLAG = 0;
//	Wifi_SendAt(Wifi_BPTCP);			//TCP����
//	Wifi_SendAt(Wifi_Enter);
//	Wait_AtRxd(300);
//	

	Wifi_SendAt(Wifi_SMTLK);//����smart link
	Wifi_SendAt(Wifi_Enter);
	SEND_AT_FLAG = 1;
	delay_ms(2000);		//�ȴ��ȶ�
	while((GPIOC->IDR&(1<<8)) != 0)			//�ȴ�ģ���������
	{
		delay_ms(1000);
		times ++;
		if(times>90)
		{
			return 1;
		}
	}
	delay_ms(10000);			//10s�ȴ�WIFI�ȶ�
	My_Wifi.Wifi_Sta &= ((1<<5))|((1<<6))|((1<<2))|((1<<7))|((1<<1)); //WIFI������������
	if((GPIOC->IDR&(1<<7)) == 0)		//���ӳɹ�
	{
		My_Wifi.Wifi_Sta |= (1<<7);		//WIFI������
		My_Wifi.Wifi_Sta &= ~(1<<4);
		Wifi_GetKey();
		delay_ms(2000);
		Wifi_GetId();
		delay_ms(1000);
		return 0;
	}
	else
	{
		return 1;
	}
}
/*
* �������ƣ�Wifi_GetKey
* ����˵������ȡWIFI������
* ��ڲ�������
* ����ֵ  ����
* ��ע    ���ޣ�
*/
int Wifi_GetKey(void)
{	
	if((My_Wifi.Wifi_Sta&(1<<4)) == 0)			//�ж�������ģʽ
	{
		if(Wifi_OutUDP())
		{
			Wifi_OutUDP();
		}
	}
	delay_ms(1000);
	Wifi_SendAt(Wifi_KeyCheck);		//WIFI�����ѯ
	Wifi_SendAt(Wifi_Enter);		
	if(!(Wifi_DataWait(1000)))//�ȴ����ݽ��ճɹ�
	{			
		Wifi_DataGet(My_Wifi.Wifi_Key);//��������
		My_Wifi.Wifi_Sta |= (1<<6);
	}
	else{return 2;}
	return 0;
}

/*
* �������ƣ�Wifi_GetId
* ����˵������ȡWIFI���˺�
* ��ڲ�������
* ����ֵ  ����
* ��ע    ���ޣ�
*/
int Wifi_GetId(void)
{	
	if((My_Wifi.Wifi_Sta&(1<<4)) == 0)			//�ж�������ģʽ
	{
		if(Wifi_OutUDP())
		{
			Wifi_OutUDP();
		}
	}
	delay_ms(1000);
	Wifi_SendAt(Wifi_IDCheck);			//WIFI�˺ż��
	Wifi_SendAt(Wifi_Enter);		
	if(!(Wifi_DataWait(1000)))			//�ȴ����ݽ��ճɹ�
	{	
		Wifi_DataGet(My_Wifi.Wifi_Id);			//�����˺�
		My_Wifi.Wifi_Sta |= (1<<5);
	}		
	else{return 2;}
	return 0;
}



int Wifi_SendCheck(const u8 *Str,u8 times)
{	
	u8 i=0;
	if((My_Wifi.Wifi_Sta&(1<<4)) == 0)			//�ж�������ģʽ
	{
		if(Wifi_OutUDP())
		{
			Wifi_OutUDP();
		}
	}
	delay_ms(1000);
	Wifi_SendAt(Str);			//�������
	Wifi_SendAt(Wifi_Enter);		
	i = Wifi_CheckSta(times);
	if(i == 1)
	{
		return 1;		//�ر�
	}
	else
	{
		if(i == 0){return 0;}//��
		 else{return 2;}	//��ȡʧ��
	}
	
//	return 0;	
}

/*
* �������ƣ�Wifi_LinkBP
* ����˵������Ѫѹ�˽�������
* ��ڲ�������
* ����ֵ  ����
* ��ע    ���ޣ�
*/
int Wifi_LinkBP(void)
{
	u8 Bp_LK[]={"LK"};
	u8 i;
	if((My_Wifi.Wifi_Sta&(1<<4)) == 0)			//�ж�������ģʽ
	{
		if(Wifi_OutUDP())			//���ﱣ֤һ���ɹ�
		{
			Wifi_OutUDP();				
		}
	}
	SEND_AT_FLAG = 0;
	delay_ms(1000);
	Wifi_SendAt(Wifi_KeySet);//��������
	Wifi_SendAt(Wifi_WSKEY);
	Wifi_SendAt(Wifi_Enter);
	if(Wait_AtRxd(500)){SEND_AT_FLAG = 1;
	return 1;}//�ȴ�����OK
	delay_ms(2000);
	Wifi_SendAt(Wifi_IdSet);//�����˺�
	Wifi_SendAt(Wifi_WSSSID);
	Wifi_SendAt(Wifi_Enter);
	if(Wait_AtRxd(500)){SEND_AT_FLAG = 1;
	return 2;}		//�ȴ�����OK
	delay_ms(500);
	Wifi_SendAt(Wifi_BPTCP);			//TCP����
	Wifi_SendAt(Wifi_Enter);
	if(Wait_AtRxd(500)){SEND_AT_FLAG = 1;
	return 2;}
	My_Wifi.Wifi_Sta |= (1<<2);			//TCP��־λ
	My_Wifi.Wifi_Sta &= ~(1<<1);
	delay_ms(2000);
	Wifi_SendAt(Wifi_SetTcpLink);//����TCP����
	Wifi_SendAt(Wifi_Enter);
	if(Wait_AtRxd(500)){SEND_AT_FLAG = 1;
	return 3;}	
	delay_ms(500);
	SEND_AT_FLAG = 1;
	My_Wifi.Wifi_Sta &= ~(1<<7);		//���������ѶϿ�
	Wifi_ReStart();//�������������
	delay_ms(1000);			//1s
	while(1)
	{
		if(Wifi_SendCheck(Wifi_CheckLink,30))	//���TCP���ӷ��ǽ���
		{
				
			if(i==6)	//
			{
				return 1;
			}
			if(i == 3)
			{
				Wifi_ReStart();		//��������
			}					
			i++;
			delay_ms(5000);		//5s
		}
		else
		{
				My_Wifi.Wifi_Sta |= (1<<2);	//BPTCP�����ɹ���־λ
				break;
		}
	}
	Wifi_InUDP();
	SEND_AT_FLAG = 0;
	Wifi_SendAt(Bp_LK);
	if(Wait_AtRxd(6000)){SEND_AT_FLAG = 1;return 5;}
	SEND_AT_FLAG = 1;
	Wifi_InUDP();
	return 0;
}
/*
* �������ƣ�Wifi_StartBp
* ����˵������ʼ����
* ��ڲ�������
* ����ֵ  ����
* ��ע    ���ޣ�
*/
int Wifi_StartBp(u8 *Bpdata)
{
	u8 Bp_ST[]={"ST"};
	u8 Bp_NL[]={"NL"};
	Wifi_InUDP();
	SEND_AT_FLAG = 0;
	Wifi_SendAt(Bp_ST);
	if(Wait_AtRxd(2000)){SEND_AT_FLAG = 1;return 1;}	//�ȴ���Ӧok
	SEND_AT_FLAG = 1;
	//delay_ms(60000);
	if(!(Wifi_DataWait(12000)))
	{
		Wifi_DataGet(Bpdata);			//��ȡѪѹ����
	}
	else{return 2;}
	SEND_AT_FLAG = 0;
	Wifi_SendAt(Bp_NL);
	if(Wait_AtRxd(500)){SEND_AT_FLAG = 1;}
	SEND_AT_FLAG =1;
	Wifi_LinkServer();
	return 0;
}
/*
* �������ƣ�Wifi_LinkServer
* ����˵����������������
* ��ڲ�������
* ����ֵ  ����
* ��ע    ���ޣ�
*/
int Wifi_LinkServer(void)
{	
	u8 i = 0;
	if((My_Wifi.Wifi_Sta&(1<<4)) == 0)			//�ж�������ģʽ
	{
		if(Wifi_OutUDP())			//���ﱣ֤һ���ɹ�
		{
			Wifi_OutUDP();				
		}
	}
	//TCP����
	if(!(((My_Wifi.Wifi_Sta & (1<<1)) !=0)&&((My_Wifi.Wifi_Sta & (1<<2)) ==0)))
	{
		SEND_AT_FLAG = 0;
		delay_ms(500);
		Wifi_SendAt(Wifi_ServerTCP);		//����TCP����ָ��
		Wifi_SendAt(Wifi_Enter);
		if(Wait_AtRxd(300)){SEND_AT_FLAG = 1;return 1;}
		delay_ms(1000);
		Wifi_SendAt(Wifi_SetTcpLink);//����TCP����
		Wifi_SendAt(Wifi_Enter);
		My_Wifi.Wifi_Sta |= (1<<1);		//webTCP����
		My_Wifi.Wifi_Sta &= ~(1<<2);	//BPTCP�声
		delay_ms(500);
		SEND_AT_FLAG = 1;
		delay_ms(1000);
		i = 1;
	}
	//WIFI����
	if((My_Wifi.Wifi_Sta &(1<<7)) == 0)
	{
		if(((My_Wifi.Wifi_Sta &(1<<6)) != 0)&((My_Wifi.Wifi_Sta &(1<<5)) != 0))		//�˺������ѱ���
		{
			if(Wifi_Link(My_Wifi.Wifi_Id,My_Wifi.Wifi_Key)){return 3;}		//����WIFI,ʧ�ܷ���3
			My_Wifi.Wifi_Sta |= (1<<7);			//WIFI���ӱ�־λ
		}
		else	//BUG,û���˺������ʱ��
		{
			if(i == 1){Wifi_ReStart();}	
		}
	}
	else
	{
		if(i == 1){Wifi_ReStart();}		//������ЧTCP����
	}
	
	//TCP���Ӽ��
	if((My_Wifi.Wifi_Sta & (1<<0)) == 0)
	{	
		if((Wifi_OutUDP()))		//����ָ��ģʽ�����ﱣ֤�ɹ�
		{
			Wifi_OutUDP();		
		}
		i = 0;
		delay_ms(1000);
		while(1)
		{
			if(Wifi_SendCheck(Wifi_CheckLink,30))	//���TCP���ӷ��ǽ���
			{
				
				if(i==6)	//
				{
					return 1;
				}
				if(i == 3)
				{
					Wifi_ReStart();		//��������
				}					
				i++;
				delay_ms(5000);		//5s
			}
			else
			{
				My_Wifi.Wifi_Sta |= (1<<0);		//TCP�����ɹ���־λ
				break;
			}
		}
	}		
	
	delay_ms(1000);
	if((My_Wifi.Wifi_Sta & (1<<3)) == 0)
	{
		if(Set_WebSocket())		//���ﳢ�Խ�����������
		{
			Wifi_ReStart();		//��������������һ��
			delay_ms(15000);		//�ȴ�15s
			if(Set_WebSocket()){return 1;}
		}
	}
	if(!(Wifi_DataWait(100)))
	{
		if(Data_GetServer()){GUI_DispStringAt("server err",120,120);GUI_Exec();}//���ݴ���
		else{/*Wifi_Send2Sever(Data_Ok);*/}//����OK��������
		Wifi_InUDP();		//��������͸��ģʽ
	}
//	else
//	{
//		GUI_DispStringAt("GG",60,120);
//		GUI_Exec();
//	}
	return 0;
}

int Set_WebSocket(void)
{
	if((My_Wifi.Wifi_Sta&(1<<4)) != 0)			//�ж�������ģʽ
	{
		Wifi_InUDP();				
	}
	SEND_AT_FLAG = 0;
	delay_ms(1000);
	Wifi_SendAt(Wifi_Get);			//Get
	Wifi_SendAt(Wifi_Enter);
	delay_ms(1000);
	Wifi_SendAt(Wifi_Host);			//Host
	Wifi_SendAt(Wifi_Enter);
	delay_ms(1000);
	Wifi_SendAt(Wifi_Upgrade);		//Upgrade
	Wifi_SendAt(Wifi_Enter);
	delay_ms(1000);
	Wifi_SendAt(Wifi_Connection);	//Connection
	Wifi_SendAt(Wifi_Enter);
	delay_ms(1000);
	Wifi_SendAt(Wifi_WBKey);		//Sec-WebSocket-Key
	Wifi_SendAt(Wifi_Enter);
	delay_ms(1000);
	Wifi_SendAt(Wifi_WBPro);		//Sec-WebSocket-Protocol
	Wifi_SendAt(Wifi_Enter);
	delay_ms(1000);
	Wifi_SendAt(Wifi_WBVer);		//Version
	Wifi_SendAt(Wifi_Enter);
	delay_ms(1000);
	Wifi_SendAt(Wifi_Enter);		//���������\r\n����
//	delay_ms(1000);
	if(Wait_AtRxd(3000)){My_Wifi.Wifi_Sta &=~(1<<3);SEND_AT_FLAG = 1;return 1;}		//�ȴ�����������OK
	else{My_Wifi.Wifi_Sta |= (1<<3);}			//WEBSOCKET�����ɹ�
	SEND_AT_FLAG = 1;
	//GUI_DispStringAt("Server OK",120,120);
	return 0;
}



