#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include "stm32f10x.h"

#define GPIO_I2C_WR	0		/* 写控制bit */
#define GPIO_I2C_RD	1		/* 读控制bit */


/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_PORT_I2C	GPIOA			/* GPIO端口 */
#define GPIO_RCC_I2C_PORT 	RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define GPIO_I2C_SCL_PIN		GPIO_Pin_2			/* 连接到SCL时钟线的GPIO */
#define GPIO_I2C_SDA_PIN		GPIO_Pin_3			/* 连接到SDA数据线的GPIO */


/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 0	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
	#define GPIO_I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, GPIO_I2C_SCL_PIN)		/* SCL = 1 */
	#define GPIO_I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, GPIO_I2C_SCL_PIN)		/* SCL = 0 */
	
	#define GPIO_I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, GPIO_I2C_SDA_PIN)		/* SDA = 1 */
	#define GPIO_I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, GPIO_I2C_SDA_PIN)		/* SDA = 0 */
	
	#define GPIO_I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, GPIO_I2C_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define GPIO_I2C_SCL_1()  GPIO_PORT_I2C->BSRR = GPIO_I2C_SCL_PIN				/* SCL = 1 */
	#define GPIO_I2C_SCL_0()  GPIO_PORT_I2C->BRR = GPIO_I2C_SCL_PIN				/* SCL = 0 */
	
	#define GPIO_I2C_SDA_1()  GPIO_PORT_I2C->BSRR = GPIO_I2C_SDA_PIN				/* SDA = 1 */
	#define GPIO_I2C_SDA_0()  GPIO_PORT_I2C->BRR = GPIO_I2C_SDA_PIN				/* SDA = 0 */
	
	#define GPIO_I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & GPIO_I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
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

