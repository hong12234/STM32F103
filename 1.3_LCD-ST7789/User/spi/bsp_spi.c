#include "bsp_spi.h"

void SPIx_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能SPI时钟 */
	SPI_APBxClock_FUN ( SPI_CLK, ENABLE );

	/* 使能SPI引脚相关的时钟 */
	RCC_APB2PeriphClockCmd ( SPI_LCD_CS_CLK|SPI_SCK_CLK|
																SPI_MISO_PIN|SPI_MOSI_PIN, ENABLE );

	/* 配置SPI的 CS引脚，普通IO即可 */
	GPIO_InitStructure.GPIO_Pin = SPI_LCD_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_LCD_CS_PORT, &GPIO_InitStructure);

	/* 配置SPI的 SCK引脚*/
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_SCK_PORT, &GPIO_InitStructure);

	/* 配置SPI的 MISO引脚*/
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
	GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

	/* 配置SPI的 MOSI引脚*/
	GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
	GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);

	/* 停止信号 LCD_CS引脚高电平*/
	SPI_LCD_CS_HIGH();

	/* SPI 模式配置 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPIx , &SPI_InitStructure);

	/* 使能 SPI  */
	SPI_Cmd(SPIx , ENABLE);
}

uint8_t SPIx_SendByte(uint8_t byte)
{
	/* 等待发送缓冲区为空，TXE事件 */
	while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_TXE) == RESET);

	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	SPI_I2S_SendData(SPIx , byte);

	/* 等待接收缓冲区非空，RXNE事件 */
	while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_RXNE) == RESET);

	/* 读取数据寄存器，获取接收缓冲区数据 */
	return SPI_I2S_ReceiveData(SPIx );	
}

uint16_t SPIx_SendHalfWord(uint16_t HalfWord)
{
	/* 等待发送缓冲区为空，TXE事件 */
	while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_TXE) == RESET);

	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	SPI_I2S_SendData(SPIx , HalfWord);

	/* 等待接收缓冲区非空，RXNE事件 */
	while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_RXNE) == RESET);
	/* 读取数据寄存器，获取接收缓冲区数据 */
	return SPI_I2S_ReceiveData(SPIx );
}


