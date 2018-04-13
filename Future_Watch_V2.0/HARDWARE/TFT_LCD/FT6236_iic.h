/*
**************************************************************
*
* �ļ����ƣ�touch.h;
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
* ������ע������оƬFT6236�ײ�iic��
*          
* �޸�˵�����ޣ�
*
**************************************************************
*/


#ifndef __TOUCH_H__
#define __TOUCH_H__


#include "sys.h"
#include "delay.h"
#include "gpio.h"

#define FT6236_SDA_OUT() GPIO_Init(GPIOC,P12,IO_MODE_OUT,IO_SPEED_50M,GPIO_OTYPE_PP,IO_NOT,IO_H)             //����SDAΪ���ģʽ��
#define FT6236_SDA_IN()  GPIO_Init(GPIOC,P12,IO_MODE_IN,IO_SPEED_50M,GPIO_OTYPE_PP,IO_PULL,IO_H)            //����SDAΪ����ģʽ��

#define FT6236_SDA PCout(12)                                                   //дSDA
#define FT6236_SCL PBout(3)                                                   //ʱ�ӿ��ƣ�
#define FT6236_RST PCout(11)                                                   //��λ���ƣ�

#define FT6236_SDA_Read PCin(12)                                               //��SDA��
#define FT6236_INT      PBin(4)                                               //��INT��





/*
* �������ƣ�FT6236_IIC_Start��
* ����˵����FT6236��ʼ�źţ�
* ��ڲ�������
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void FT6236_IIC_Start(void);


/*
* �������ƣ�FT6236_IIC_Stop��
* ����˵����FT6236�����źţ�
* ��ڲ�������
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void FT6236_IIC_Stop(void);


/*
* �������ƣ�FT6236_Send_ACK��
* ����˵������Ƭ������Ӧ���źţ�
* ��ڲ�������
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void FT6236_Send_ACK(void);


/*
* �������ƣ�FT6236_Send_NACK��
* ����˵������Ƭ�����ͷ�Ӧ���źţ�
* ��ڲ�������
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void FT6236_Send_NACK(void);


/*
* �������ƣ�FT6236_Wait_Ack��
* ����˵�����ȴ�Ӧ���źţ�
* ��ڲ�������
* ����ֵ  ��0-->����Ӧ��ɹ���1-->����Ӧ��ʧ�ܣ�
* ��ע    ���ޣ�
*/
u8 FT6236_Wait_Ack(void);


/*
* �������ƣ�FT6236_Send_Byte��
* ����˵������FT6236����һ���ֽ����ݣ�
* ��ڲ�����data-->���͵����ݣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void FT6236_Send_Byte(u8 data);

/*
* �������ƣ�FT6236_Read_Byte��
* ����˵������FT6236��һ���ֽ����ݣ�
* ��ڲ�������
* ����ֵ  �����������ݣ�
* ��ע    ���ޣ�
*/
u8 FT6236_Read_Byte(unsigned char ack);


#endif

