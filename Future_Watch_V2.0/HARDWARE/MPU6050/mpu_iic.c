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
* ������ע��ģ��mpu6050��iic�ӿڣ�
*
**************************************************************
*/
#include "mpu_iic.h"

/*
* �������ƣ�MPU_IIC_Init��
* ����˵����ģ��IIC��ʼ����
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ��PC12��PC11�ֱ�ģ��SCL��SDA��
*/
void MPU_IIC_Init(void)
{
	GPIO_Clock_Set(IOPCEN);                                   //��ʹ������IO PORTCʱ�� 	
	
	 GPIO_Init(GPIOC,P4,IO_MODE_OUT,IO_SPEED_2M,IO_OTYPER_PP,IO_PULL,IO_H);	// SDA
	 GPIO_Init(GPIOC,P5,IO_MODE_OUT,IO_SPEED_2M,IO_OTYPER_PP,IO_PULL,IO_H);	//	SCL
}



/*
* �������ƣ�MPU_IIC_Start��
* ����˵����������ʼ�źţ�
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ������iicЭ��Ľ����������ԣ�
*/
void MPU_IIC_Start(void)
{
	MPU_IIC_SDA_OUT();
	
	MPU_IIC_SCL=1;
	
	delay_us(2);               //�˴������������ʱ2us��
	MPU_IIC_SDA=1;             //�ο�ʱ��ͼ(��Ҫ!)
	delay_us(2);
	MPU_IIC_SDA=0;
	delay_us(2);
	
	MPU_IIC_SCL=0;
}


/*
* �������ƣ�MPU_IIC_Spot��
* ����˵�������������źţ�
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ������iicЭ��Ľ����������ԣ�
*           Ӧ��ע��SDA=0Ҫ��SCL=1֮ǰ��ɣ�֮���ٽ���SDA=1�ı仯��
*           ������SCLΪ��ʱִ��SDA=0һ����ȼ�⵽�½���(��Ϊǰ�����������SDA)��
*           ����������Ϊ�ǿ�ʼ�źţ�
*/
void MPU_IIC_Stop(void)
{
	MPU_IIC_SDA_OUT();
	
	MPU_IIC_SDA=0;                          //�ο�ʱ��ͼ(��Ҫ!)
	MPU_IIC_SCL=0;
	delay_us(2);                            //�˴������������ʱ2us��
	MPU_IIC_SCL=1;
	delay_us(1);
	MPU_IIC_SDA=1;
}


/*
* �������ƣ�MPU_IIC_Wait_Ack��
* ����˵�����ȴ�Ӧ���źţ�
* ��ڲ������ޣ�
* ����ֵ  ��0->����Ӧ��ɹ���1->����Ӧ��ʧ�ܣ�
* ��ע    ���ޣ�
*/
u8 MPU_IIC_Wait_Ack(void)
{
	u8 nack_overtime=0;
	MPU_IIC_SDA_IN();
	
	MPU_IIC_SDA=1;
	delay_us(2);
	MPU_IIC_SCL=1;                          //����ʱ��ͼ����SCLΪ�ߵȴ�Ӧ���źţ�
	delay_us(2);                            //�˴���ʱ�ɲο�iicӦ���źŵ�ʱ��
	while(MPU_READ_SDA)                        //����ʱ��ͼ������1��ʾ�յ�NACK��
	{
		nack_overtime++;
		if(nack_overtime>250)               //�ӻ�û��Ӧ��ʱ(�����Ǵӻ�û��Ӧ������Ƿ�Ӧ��)��
		{
			MPU_IIC_Stop();                 //������������stop��
			return 1;
		}
	}
	MPU_IIC_SCL=0;                          //����Ӧ���ź�ʱ�ӣ�
	return 0;                               //���յ�ACK����NACK��
}


/*
* �������ƣ�MPU_IIC_Ack��
* ����˵��������ACK�źţ�
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ����Ӧ�������ڼ䱣���ȶ��͵�ƽ��
*/
void MPU_IIC_Ack(void)
{
	MPU_IIC_SCL=0;
	MPU_IIC_SDA_OUT();
	MPU_IIC_SDA=0;                        //����SDA��ACK��
	delay_us(2);
	MPU_IIC_SCL=1;                        //��SCLΪ�߱����ȶ���
	delay_us(2);
	MPU_IIC_SCL=0;                        //����Ӧ��ʱ�ӣ�
}


/*
* �������ƣ�MPU_IIC_NAck��
* ����˵��������NACK�źţ�
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ����Ӧ�������ڼ䱣���ȶ��ߵ�ƽ��
*/
void MPU_IIC_NAck(void)
{
	MPU_IIC_SCL=0;
	MPU_IIC_SDA_OUT();
	MPU_IIC_SDA=1;                        //����SDA��NACK��
	delay_us(2);
	MPU_IIC_SCL=1;                        //��SCLΪ�߱����ȶ���
	delay_us(2);
	MPU_IIC_SCL=0;                        //����Ӧ��ʱ�ӣ�
}


/*
* �������ƣ�MPU_IIC_Send_Byte��
* ����˵�����ͳ�һ���ֽڣ�
* ��ڲ�����tx_data->���͵�һ�ֽ����ݣ�
* ����ֵ  ���ޣ�
* ��ע    ��ÿ��ʱ�Ӵ���һλ���ݣ�SCLΪ��ʱSDA���뱣���ȶ���
*           ��Ϊ��ʱSDA�ĸı�ᱻ��Ϊ�ǿ����ź�(��ʼ����������Ӧ��)��
*           ��Ҳ�ǿ����źź������źŵ�����
*/	  
void MPU_IIC_Send_Byte(u8 tx_data)
{                        
    u8 i;   
	MPU_IIC_SDA_OUT(); 	    

    for(i=0;i<8;i++)
    {           
		MPU_IIC_SCL=0;                  //����ʱ�ӿ�ʼ���ݴ��䣻
        MPU_IIC_SDA=(tx_data&0x80)>>7;
        tx_data<<=1; 	  
		delay_us(2);                    //�����ȶ���
		MPU_IIC_SCL=1;
		delay_us(2); 
    }	 
	MPU_IIC_SCL=0;                       //���ݸ���ʱ��ͼ�����Ҫ���ʱ�ӣ�
} 	


/*
* �������ƣ�MPU_IIC_Read_Byte��
* ����˵������ȡһ���ֽڣ�
* ��ڲ�����ack->0������NACK��ack->1������ACK��
* ����ֵ  ����ȡ����8λ���ݣ�
* ��ע    ���ޣ�
*/	
u8 MPU_IIC_Read_Byte(u8 ack)
{
	u8 i,temp;
	MPU_IIC_SDA_IN();
	
	for(i=0;i<8;i++)
	{
		MPU_IIC_SCL=0;
		delay_us(2);
		MPU_IIC_SCL=1;                   //��ʱ��Ϊ��ʱ������
		temp<<=1;
		if(MPU_READ_SDA)
		{temp++;}
		delay_us(2);
	}
	
	if(ack)
		MPU_IIC_Ack();
	else
		MPU_IIC_NAck();
	return temp;
}


