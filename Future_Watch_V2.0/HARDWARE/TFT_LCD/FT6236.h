/*
**************************************************************
*
* �ļ����ƣ�FT6236.h;
*
* �汾˵����V1.0��
*
* ���л�����(1) STM32F103RCT;
*           (2) �ⲿ����8M9��Ƶ��
*           (3) MDK 5.0��
*
* �޸�˵�����ޣ�
*           
* ������ڣ�2016/06/26;
*
* ����������֣��˸;
*
* ������ע������оƬFT6236��������
*           ���Գ������̣��ο�����ԭ��FT5206��
*          
* �޸�˵�����ޣ�
*
**************************************************************
*/


#ifndef __FT6236_H__
#define __FT6236_H__

#include "FT6236_iic.h"
#include "GUI.h"
#include "led.h"

//I2C��д����	
#define FT_CMD_WR 				0X70    	//д����
#define FT_CMD_RD 				0X71		//������

//FT6236 ���ּĴ������� 
#define FT_DEVIDE_MODE 			0x00   		//FT6236ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER       0x02		//����״̬�Ĵ���

#define FT_TP1_REG 				0X03	  	//��һ�����������ݵ�ַ
#define FT_TP2_REG 				0X09		//�ڶ������������ݵ�ַ
#define FT_TP3_REG 				0X0F		//���������������ݵ�ַ
#define FT_TP4_REG 				0X15		//���ĸ����������ݵ�ַ
#define FT_TP5_REG 				0X1B		//��������������ݵ�ַ  
 

#define	FT_ID_G_LIB_VERSION		0xA1        //�汾		
#define FT_ID_G_MODE 			0xA4   		//FT6236�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP			0x80   		//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE	0x88   		//����״̬�������üĴ���  
#define Chip_Vendor_ID          0xA3        //оƬID(0x36)
#define ID_G_FT6236ID			0xA8		//0x11

#define TP_PRES_DOWN            0x80        //����������	
#define TP_COORD_UD             0x40        //����������±��


//������������ݽṹ�嶨��
//���������b7:����1/�ɿ�0; b6:0û�а�������/1�а�������;
//bit5:������bit4-bit0�����㰴����Ч��־����ЧΪ1���ֱ��Ӧ������5-1��
//֧��5�㴥������Ҫʹ��5������洢����������
typedef struct			
{
	u8 TouchSta;	
	u16 x[2];		
	u16 y[2];
	u16 Point_X[30];
	u16 Point_Y[30];
	u16 LastX_Point;
	u16 LastY_Point;
	u8 Point_Num;
	u8 Gesture_Sta;
}TouchPointRefTypeDef;

extern TouchPointRefTypeDef TPR_Structure;


/*
* �������ƣ�FT6236_Init��
* ����˵����FT6236��ʼ����
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/		
void FT6236_Init(void);


/*
* �������ƣ�FT6236_Write_Reg��
* ����˵����FT6236����д�Ĵ�����
* ��ڲ�����reg-->�Ĵ�����ַ��buf-->���ͻ���ռ䣬len-->�������ȣ�
* ����ֵ  ��0-->�ɹ���1-->ʧ�ܣ�
* ��ע    ���ޣ�
*/
u8 FT6236_Write_Reg(u16 reg,u8 *buf,u8 len);


/*
* �������ƣ�FT6236_Read_Reg��
* ����˵����FT6236�������Ĵ�����
* ��ڲ�����reg-->�Ĵ�����ַ��buf-->���ջ���ռ䣬len-->�������ȣ�
* ����ֵ  ��0-->�ɹ���1-->ʧ�ܣ�
* ��ע    ���ޣ�
*/		  
u8 FT6236_Read_Reg(u16 reg,u8 *buf,u8 len); 



/*
* �������ƣ�FT6236_Scan��
* ����˵��������ɨ�裻
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/	
void FT6236_Scan(void);
int Gesture_Scan(void);

#endif
