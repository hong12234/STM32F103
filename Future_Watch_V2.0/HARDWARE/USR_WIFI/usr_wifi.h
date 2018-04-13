#ifndef _USRWIFI_H_
#define _USRWIFI_H_

#include "sys.h"
#include "delay.h"
#include "usart_2.h"
#include "stdio.h"
#include "gpio.h"

typedef struct 
{
	u8 Wifi_Id[16];		//WIFI���˺�
	u8 Wifi_Key[30];	//WIFI������
	u8 Wifi_Sta;		//WIFI��״̬
						//��7λ ��ʾ����״̬	    1����  0û������
						//��6λ ��ʾ�����Ƿ񱣴� 1����  0û�б���
						//��5λ ��ʾ�˺��Ƿ񱣴� 1����  0û�б���
						//��4λ ��ʾ��ǰ����ģʽ 1����  0����
						//��3λ WEBSOCKET �Ƿ�������
						//��2λ ��ʾBPTCP�����Ƿ�����	1���� 0�ر�
						//��1λ ��ʾWEBTCP�����Ƿ�����	1���� 0�ر�
						//��0λ ��ʾWEBTCP�Ƿ����ɹ�
				
}_Wifi_Information;

int Wifi_SendAt(const u8 Commond[]);//wifi����ATָ��
int Wifi_Send_Str(const u8* p);//WIFI�����ַ���
int Wait_AtRxd(u16 time);//WIFI�ȴ�����AT��OK
int Wifi_Init(void);//WIFI��ʼ��
int Wifi_DataWait(u16 times);//WIFI�ȴ���������{}
int Wifi_CheckSta(u16 times);//WIFI�����������
int Wifi_DataGet(u8 *pstr);//��ȡ��ȡ��������
int Wifi_Send2Sever(u8 *pstr);//�������ݵ�������
int Wifi_OutUDP(void);//�˳�͸��ģʽ
int Wifi_InUDP(void);//����͸��ģʽ
int Wifi_Link(u8 *IdStr,u8 *KeyStr);//WIFI����WIFI
int Wifi_ReStart(void);//WIFI����
int Wifi_SmartLink(void);//WIFI������������
int Wifi_GetKey(void);//WIFI��ȡ����
int Wifi_GetId(void);//WIFI��ȡ�˺�
int Wifi_SendCheck(const u8 *Str,u8 times);	//WIFI���ͼ������
int Wifi_LinkBP(void);//WIFI������Ѫѹģ��
int Wifi_StartBp(u8 *Bpdata);//WIFI���Ϳ�ʼ��������ȴ�����
int Wifi_LinkServer(void);//WIFI���ӷ�����
int Set_WebSocket(void);//WIFI����WEBSOCKET����

#endif

