#ifndef _MAIN_H_
#define _MAIN_H_

#include "sys.h"
#include "delay.h"
#include "includes.h"
#include "GUI.h"
#include "FT6236.h"
#include "st7789.h"
#include "led.h"
#include "DMA.h"
#include "battery.h"
#include "usr_wifi.h"
#include "rtc.h"
#include "usart_2.h"
#include "stdlib.h"
#include "math.h"
#include "watch.h"
#include "mlx90615.h"
#include "backlit.h"
#include "timer_4.h"
#include "timer_2.h"
#include "mpu6050.h"
#include "motor.h"
#include "key.h"
#include "wdg.h"
#include "dispwifi.h"
#include "stmflash.h"
#include "Interrupt.h"
#include "dispdata.h"
#include "heart.h"
#include "inv_mpu.h"                         //DMP��
#include "inv_mpu_dmp_motion_driver.h"  
#include "dispwifi.h"
#include "watch.h"
#include "dispdata.h"
#include "help.h"
#include "medicine.h"
#include "arm_math.h"
#include "get_data.h"
//ϵͳ״̬��־λ
typedef struct{
	u8 Mcu_Power;		//���ػ���־λ
	volatile u8 Get_Time;		//ʱ����¼������
	volatile u8 Check_Key;		//ɨ�谴���������
	volatile u16 Get_Heart;		//��ȡ���ʼ������
	volatile u16 Get_Temp;		//��ȡ�¶ȼ������
	volatile u8 Get_Tumble;		//ˤ���жϼ������
	volatile u8 Get_backlit;		//�����ж�ʱ������
	volatile u16 Check_Health;		//�������ʱ��
	volatile u8 Tumble_Flag;		//ˤ����־λ
	
	volatile u16 Disp_Sta;		//������ʾ��־λ
							//��7λ����ʾ�������־λ�������У���ҳ��ֻˢ��һ�Σ��Ͳ���ˢ��
							//��6λ��WIFI�����Ѿ���ʾ��־λ����������ʾ������ʱɾ��WIFI����ľ��
							//��4λ�����ݽ�������ʾ��־λ
							//��5λ��������־λ��Ϊ1��ʾ��ǰΪ����״̬
							//��3λ����ǰ��ʾ��������ҳ��
							//��2λ����ǰ��ʾ����WIFIҳ��
							//��1λ����ǰ��ʾ����������
							//��0λ����ǰ��ʾ���Ǳ�������
	
	volatile u8 unlock;		//������������Ļ��0->����û�з�����1->��������
	
	volatile u8 Measure;		//Ѫѹ���ڲ�����־λ
							//��7λ:��ʼ����
							//��6λ:��ʾ�����ɹ�
	
	volatile u8 Warning;		//�������ѹ��ܱ�־λ
							//��7λ:ˤ��
							//��6λ:����
							//��5λ:����
							//��4λ:����
							//��3λ:ȷ�����������Ϣ
									
	volatile u8 Remind;			//��ҩ���ѽ���
	volatile u8 Remind1;		//��ҩ���ѽ���
	volatile u8 Remind2;		//��ҩ���ѽ���
	volatile u8 Send_Start;	//�յ���������GET����ʼ��������
	
	volatile u8 battery;		//��ص�����־λ
							//��7λ:����
							//��6λ:�м�
							//��5λ:û��
							
	volatile u8 Wifi_Connection;		//Wifi������ر�־λ
									//��7λ:��ʼ����smartlink
									//��6λ:����ʧ��
									//��5λ:���ӳɹ�
									//��4λ:�Ѿ���ʾ"Wifi�������ӱ�־λ"
									//��3λ:��ʼ��ʾ,����ʱ�����������ʾ
	volatile u8 Motor;		//������־λ
							//��7λ:����һ���𶯱�־λ
							//��6λ:��ǰΪ�𶯱�־λ
							//����λ��������
	u8 App_Sta;			//APP�Ƿ����ӱ�־λ
	u16 Help_Time;		//�������棬�ȴ�ʱ��
}SYSTEM_STA;


//ϵͳ����
typedef struct{
	u8 heart;			//����
	float temperature;		//����
	u8 heart_average;		//���ʾ�ֵ
	float temp_average;		//���¾�ֵ
	u8 boolpressure_up;	//Ѫѹ����ѹ
	u8 boolpressure_down;//Ѫѹ����ѹ
}SYSTEM_DATA;


//ϵͳ����(�˲�����Ҫ���㱣��)
typedef struct{
	u8 warning_heart;	//���ʱ������� 0:�� 1:��
	u8 warning_fall;	//ˤ����������
	u8 warning_temp;	//���±�������
	u8 warning_eat;		//��ҩ���ѿ���
							//��7λ ��һ��ʱ������ÿ�
							//��6λ �ڶ���ʱ������ÿ�
							//��5λ ������ʱ������ÿ�
	u8 eat_time1;		//��һ��ʱ���ĳ�ҩ��ʱ
	u8 eat_min1;		//��һ��ʱ���ĳ�ҩ�ķ�
	u8 eat_time2;		//��һ��ʱ���ĳ�ҩ��ʱ
	u8 eat_min2;		//��һ��ʱ���ĳ�ҩ�ķ�
	u8 eat_time3;		//��һ��ʱ���ĳ�ҩ��ʱ
	u8 eat_min3;		//��һ��ʱ���ĳ�ҩ�ķ�
}SYSTEM_SET;


//��FLASH������
int Read_Data(void);

//�������ݵ�FLASH
int Save_Data(void);
#endif


