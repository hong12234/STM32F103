/*
**************************************************************
*
* �ļ����ƣ�mpu_iic.h;
*
* �汾˵����V.10��
*
* ���л�����(1) STM32F103RCT;
*           (2) �ⲿ����8M9��Ƶ��
*           (3) MDK 5.0��
*
* �޸�˵�������ޣ�
*           
* ������ڣ�2016/04/24;
*
* ����������֣��˸;
*
* ������ע��ģ��mpu6050 ��iic�ӿڣ�
*
**************************************************************
*/
#ifndef __MPU_IIC_H__
#define __MPU_IIC_H__

#include "sys.h"
#include "delay.h"
#include "gpio.h"



#define MPU_IIC_SDA  PCout(4)             //SDA�ź��ߣ�
#define MPU_IIC_SCL  PCout(5)             //SCLʱ���ߣ�
#define MPU_READ_SDA PCin(4)              //��SDA�źţ�



#define MPU_IIC_SDA_OUT()  GPIO_Init(GPIOC,P4,IO_MODE_OUT,IO_SPEED_2M,IO_OTYPER_PP,IO_PULL,IO_H)
#define MPU_IIC_SDA_IN()  GPIO_Init(GPIOC,P4,IO_MODE_IN,IO_SPEED_2M,IO_OTYPER_PP,IO_NOT,IO_H)

//#define MPU_IIC_SDA_OUT() GPIO_Init(GPIOB,P11,OUT_10M,G_PUSH,SET_L)                //SDAΪ���ģʽ��
//#define MPU_IIC_SDA_IN()  GPIO_Init(GPIOB,P11,IN_MODE,PULL,PULL_UP)                //SDAΪ����ģʽ��



/*
* �������ƣ�MPU_IIC_Init��
* ����˵����ģ��IIC��ʼ����
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ��PC12��PC11�ֱ�ģ��SCL��SDA��
*/
void MPU_IIC_Init(void);


/*
* �������ƣ�MPU_IIC_Start��
* ����˵����������ʼ�źţ�
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void MPU_IIC_Start(void);


/*
* �������ƣ�MPU_IIC_Spot��
* ����˵�������������źţ�
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void MPU_IIC_Stop(void);


/*
* �������ƣ�MPU_IIC_Wait_Ack��
* ����˵�����ȴ�Ӧ���źţ�
* ��ڲ������ޣ�
* ����ֵ  ��0->����Ӧ��ɹ���1->����Ӧ��ʧ�ܣ�
* ��ע    ���ޣ�
*/
u8 MPU_IIC_Wait_Ack(void);



/*
* �������ƣ�MPU_IIC_Ack��
* ����˵��������ACK�źţ�
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ����Ӧ�������ڼ䱣���ȶ��͵�ƽ��
*/
void MPU_IIC_Ack(void);


/*
* �������ƣ�MPU_IIC_NAck��
* ����˵��������NACK�źţ�
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ����Ӧ�������ڼ䱣���ȶ��ߵ�ƽ��
*/
void MPU_IIC_NAck(void);


/*
* �������ƣ�MPU_IIC_Send_Byte��
* ����˵�����ͳ�һ���ֽڣ�
* ��ڲ�����tx_data->���͵�һ�ֽ����ݣ�
* ����ֵ  ���ޣ�
* ��ע    ��ÿ��ʱ�Ӵ���һλ���ݣ�SCLΪ��ʱSDA���뱣���ȶ���
*           ��Ϊ��ʱSDA�ĸı�ᱻ��Ϊ�ǿ����ź�(��ʼ����������Ӧ��)��
*           ��Ҳ�ǿ����źź������źŵ�����
*/	  
void MPU_IIC_Send_Byte(u8 tx_data);	


/*
* �������ƣ�MPU_IIC_Read_Byte��
* ����˵������ȡһ���ֽڣ�
* ��ڲ�����ack->0������NACK��ack->1������ACK��
* ����ֵ  ����ȡ����8λ���ݣ�
* ��ע    ���ޣ�
*/	
u8 MPU_IIC_Read_Byte(u8 ack);


#endif



