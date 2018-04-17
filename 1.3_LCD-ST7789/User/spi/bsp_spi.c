#include "bsp_spi.h"

void SPIx_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��SPIʱ�� */
	SPI_APBxClock_FUN ( SPI_CLK, ENABLE );

	/* ʹ��SPI������ص�ʱ�� */
	RCC_APB2PeriphClockCmd ( SPI_LCD_CS_CLK|SPI_SCK_CLK|
																SPI_MISO_PIN|SPI_MOSI_PIN, ENABLE );

	/* ����SPI�� CS���ţ���ͨIO���� */
	GPIO_InitStructure.GPIO_Pin = SPI_LCD_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_LCD_CS_PORT, &GPIO_InitStructure);

	/* ����SPI�� SCK����*/
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_SCK_PORT, &GPIO_InitStructure);

	/* ����SPI�� MISO����*/
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
	GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

	/* ����SPI�� MOSI����*/
	GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
	GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);

	/* ֹͣ�ź� LCD_CS���Ÿߵ�ƽ*/
	SPI_LCD_CS_HIGH();

	/* SPI ģʽ���� */
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

	/* ʹ�� SPI  */
	SPI_Cmd(SPIx , ENABLE);
}

uint8_t SPIx_SendByte(uint8_t byte)
{
	/* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
	while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_TXE) == RESET);

	/* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
	SPI_I2S_SendData(SPIx , byte);

	/* �ȴ����ջ������ǿգ�RXNE�¼� */
	while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_RXNE) == RESET);

	/* ��ȡ���ݼĴ�������ȡ���ջ��������� */
	return SPI_I2S_ReceiveData(SPIx );	
}

uint16_t SPIx_SendHalfWord(uint16_t HalfWord)
{
	/* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
	while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_TXE) == RESET);

	/* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
	SPI_I2S_SendData(SPIx , HalfWord);

	/* �ȴ����ջ������ǿգ�RXNE�¼� */
	while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_RXNE) == RESET);
	/* ��ȡ���ݼĴ�������ȡ���ջ��������� */
	return SPI_I2S_ReceiveData(SPIx );
}


