#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include "stm32f10x.h"

#define GPIO_I2C_WR	0		/* д����bit */
#define GPIO_I2C_RD	1		/* ������bit */


/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define GPIO_PORT_I2C	GPIOA			/* GPIO�˿� */
#define GPIO_RCC_I2C_PORT 	RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
#define GPIO_I2C_SCL_PIN		GPIO_Pin_2			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define GPIO_I2C_SDA_PIN		GPIO_Pin_3			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 0	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define GPIO_I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, GPIO_I2C_SCL_PIN)		/* SCL = 1 */
	#define GPIO_I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, GPIO_I2C_SCL_PIN)		/* SCL = 0 */
	
	#define GPIO_I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, GPIO_I2C_SDA_PIN)		/* SDA = 1 */
	#define GPIO_I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, GPIO_I2C_SDA_PIN)		/* SDA = 0 */
	
	#define GPIO_I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, GPIO_I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define GPIO_I2C_SCL_1()  GPIO_PORT_I2C->BSRR = GPIO_I2C_SCL_PIN				/* SCL = 1 */
	#define GPIO_I2C_SCL_0()  GPIO_PORT_I2C->BRR = GPIO_I2C_SCL_PIN				/* SCL = 0 */
	
	#define GPIO_I2C_SDA_1()  GPIO_PORT_I2C->BSRR = GPIO_I2C_SDA_PIN				/* SDA = 1 */
	#define GPIO_I2C_SDA_0()  GPIO_PORT_I2C->BRR = GPIO_I2C_SDA_PIN				/* SDA = 0 */
	
	#define GPIO_I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & GPIO_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif

void i2c_GPIO_Init(void);
void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);


#endif

