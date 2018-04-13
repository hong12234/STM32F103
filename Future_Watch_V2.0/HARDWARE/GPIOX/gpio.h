#ifndef __GPIO_H__
#define __GPIO_H__

#include "delay.h"
#include "sys.h"

//#define RESET (1<<4)
//#define SCL   (1<<5)
//#define MOSI  (1<<6)
//#define WR    (1<<7)
//#define CS    (1<<8)


#define P0  0                      //���ű�ź궨�壻
#define P1  1
#define P2  2
#define P3  3
#define P4  4
#define P5  5
#define P6  6
#define P7  7
#define P8  8
#define P9  9
#define P10 10
#define P11 11
#define P12 12
#define P13 13
#define P14 14
#define P15 15

//IOʱ��ʹ��λ
#define IOPAEN 0                      //�˿�ʱ��λ�궨�壻
#define IOPBEN 1
#define IOPCEN 2
#define IOPDEN 3
#define IOPEEN 4
#define IOPFEN 5
#define IOPGEN 6
#define IOPHEN 7
#define IOPIEN 8


//IOģʽ
#define IO_MODE_OUT  0x01		//���ģʽ
#define IO_MODE_IN   0x00		//����ģʽ
#define IO_MODE_TWO  0x02		//���ù���
#define IO_MODE_ANA	 0x03		//ģ������

//IO����ٶ�
#define IO_SPEED_2M 	0x00				//2MHz
#define IO_SPEED_25M    0x1                //25M�����
#define IO_SPEED_50M    0x2                //50M�����
#define IO_SPEED_100M   0x3                //100M�����
//IO������߿�©
#define IO_OTYPER_PP 	0			//����
#define IO_OTYPER_OD 	1			//��©

//IO״̬
#define IO_NOT		0x00		//����/����
#define IO_PULL		0X01		//����
#define IO_DOWN		0X02		//����
#define IO_KEEP		0X03		//����

//IO��ƽ
#define IO_H      1                   //����ߣ�
#define IO_L      0                   //����ͣ�


/*
* �������ƣ�GPIO_Init��
* ����˵����GPIO��ʼ����
* ��ڲ�����IO���ĸ��˿�
*           PORT_NUM���˿�λ��
*           MODE���˿�ģʽ
*           SPEED���˿��ٶ�
*			PUPD��������������
*           IO_DR���˿ڵ�ƽ
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void GPIO_Init(GPIO_TypeDef * IO,u8 PORT_NUM,u8 MODE,u8 SPEED,u8 OTY,u8 PUPD,u8 IO_DR);

/*
* �������ƣ�GPIO_Clock_Set��
* ����˵����GPIOʱ��ʹ�ܣ�
* ��ڲ�����IOPxEN->IOPAEN--IOPEEN,�˿�ʱ�ӣ�
*           EN_PULL_OR_OUT-->ODR���ã�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void GPIO_Clock_Set(u8 IOPxEN);



#endif

