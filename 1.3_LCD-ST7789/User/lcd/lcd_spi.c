#include "lcd_spi.h"
#include "delay.h"
#include "bsp_spi.h"

void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd ( SPI_LCD_CS_CLK|SPI_LCD_BL_CLK|
													 SPI_LCD_RST_CLK|SPI_LCD_DC_CLK, ENABLE);
	
	
	/* 配置LCD的 背光 引脚 */
	GPIO_InitStructure.GPIO_Pin = SPI_LCD_BL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_LCD_BL_PORT, &GPIO_InitStructure);
	
	/* 配置LCD的 RESET 引脚 */
	GPIO_InitStructure.GPIO_Pin = SPI_LCD_RST_PIN;
	GPIO_Init(SPI_LCD_RST_PORT, &GPIO_InitStructure);	
	
	/* 配置LCD的 D/C 引脚 */
	GPIO_InitStructure.GPIO_Pin = SPI_LCD_DC_PIN;
	GPIO_Init(SPI_LCD_DC_PORT, &GPIO_InitStructure);
	
	SPI_LCD_BL_OFF();
}

void LCD_Write_CMD(uint8_t CMD)
{
	SPI_LCD_CS_LOW();
	SPI_LCD_DC_CMD();
	SPIx_SendByte(CMD);
	SPI_LCD_CS_HIGH();
}

void LCD_Write_DATA(uint8_t data)
{
	SPI_LCD_CS_LOW();
	SPI_LCD_DC_DATA();
	SPIx_SendByte(data);
	SPI_LCD_CS_HIGH();
}

//void LCD_Read_DATA(uint8_t CMD,uint8_t DATA)
//{
//	
//}

void LCD_ST7789_Init(void)
{
	
	SPI_LCD_RST_LOW();
	Delay_ms(10);
	SPI_LCD_RST_HIGH();
	Delay_ms(100);
	
	LCD_Write_CMD(0xB2);			LCD_Write_DATA(0x0C); //porch setting
																LCD_Write_DATA(0x0C); 
																LCD_Write_DATA(0x00); 
																LCD_Write_DATA(0x33); 
																LCD_Write_DATA(0x33); 	

	LCD_Write_CMD(0xB7);			LCD_Write_DATA(0x02); //64;VGH&VGL,12V&-8V
									  	
	LCD_Write_CMD(0xC2);			LCD_Write_DATA(0x01);
	
	LCD_Write_CMD(0xC3);			LCD_Write_DATA(0x16); //1F;GVDD,5.1V  16
	
	LCD_Write_CMD(0xBB);			LCD_Write_DATA(0x3C); //28;VCOM
	
	LCD_Write_CMD(0xC5);			LCD_Write_DATA(0x20); //vcom shift.0V  20
															  




	LCD_Write_CMD(0xD0);			LCD_Write_DATA(0xA4); 
																LCD_Write_DATA(0xA2); //AVDD&AVCL,6.8v&-4.8v
												  
	LCD_Write_CMD(0xD2);			LCD_Write_DATA(0x4C); 													  
	
	LCD_Write_CMD(0xE8);			LCD_Write_DATA(0x83); //Booster CLK Select
				 
	LCD_Write_CMD(0xE9);			LCD_Write_DATA(0x09); //EQ
																LCD_Write_DATA(0x09);
																LCD_Write_DATA(0x08);

	LCD_Write_CMD(0x36);			LCD_Write_DATA(0x00); //ACCESS

	LCD_Write_CMD(0x35);			LCD_Write_DATA(0x00); //TE

	LCD_Write_CMD(0x3A);			LCD_Write_DATA(0x05); //16bit
															


	LCD_Write_CMD(0xC6);			LCD_Write_DATA(0x0d); //0x09orig [DB7-5] 0forDotInv,1forColumnInv; 
		                                               //[DB4-0] Frame Rate,0x0F:60Hz 64hz

		
	    
	LCD_Write_CMD(0xE0);			LCD_Write_DATA(0xD0); //V0[7-4] & V63[3-0]
																LCD_Write_DATA(0x06); //V62[5-0]
																LCD_Write_DATA(0x0C); //V61[5-0]
																LCD_Write_DATA(0x09); //V59[4-0]
																LCD_Write_DATA(0x09); //V57[4-0]
																LCD_Write_DATA(0x25); //J1[5-4] & V50[3-0]
																LCD_Write_DATA(0x2E); //V43[6-0]
																LCD_Write_DATA(0x33); //V27[6-4] & V36[2-0]
																LCD_Write_DATA(0x45); //V20[6-0]
																LCD_Write_DATA(0x36); //J0[5-4] & V13[3-0]
																LCD_Write_DATA(0x12); //V6[4-0]
																LCD_Write_DATA(0x12); //V4[4-0]
																LCD_Write_DATA(0x2E); //V2[5-0]
																LCD_Write_DATA(0x34); //V1[5-0]

	LCD_Write_CMD(0xE1);			LCD_Write_DATA(0xD0); //V0[7-4] & V63[3-0]
																LCD_Write_DATA(0x06); //V62[5-0]
																LCD_Write_DATA(0x0C); //V61[5-0]
																LCD_Write_DATA(0x09); //V59[4-0]
																LCD_Write_DATA(0x09); //V57[4-0]
																LCD_Write_DATA(0x25); //J1[5-4] & V50[3-0]
																LCD_Write_DATA(0x2E); //V43[6-0]
																LCD_Write_DATA(0x33); //V27[6-4] & V36[2-0]
																LCD_Write_DATA(0x45); //V20[6-0]
																LCD_Write_DATA(0x36); //J0[5-4] & V13[3-0]
																LCD_Write_DATA(0x12); //V6[4-0]
																LCD_Write_DATA(0x12); //V4[4-0]
																LCD_Write_DATA(0x2E); //V2[5-0]
																LCD_Write_DATA(0x34); //V1[5-0]
 	   			
	LCD_Write_CMD(0x21);
	
	LCD_Write_CMD(0x11);
	Delay_ms(120);

	LCD_Write_CMD(0x29);
	Delay_ms(50);	
	
	SPI_LCD_BL_ON();
}

void LCD_Init(void)
{
	LCD_GPIO_Init();
	LCD_ST7789_Init();
}

//行设置（0 ~ LCD_RWO-1）
void LCD_ROW_SET(uint16_t start, uint16_t end)
{
	LCD_Write_CMD(0x2B);
	LCD_Write_DATA(start >> 8);
	LCD_Write_DATA(start & 0xFF);
	LCD_Write_DATA(end >> 8);
	LCD_Write_DATA(end & 0xFF);
}

//列设置（0 ~ LCD_COL-1）
void LCD_COL_SET(uint16_t start, uint16_t end)
{
	LCD_Write_CMD(0x2A);
	LCD_Write_DATA(start >> 8);
	LCD_Write_DATA(start & 0xFF);
	LCD_Write_DATA(end >> 8);
	LCD_Write_DATA(end & 0xFF);
}

//清屏
void LCD_Clear(uint16_t color)
{
	uint16_t temp = LCD_ROW*LCD_COL;
	LCD_ROW_SET(0,LCD_ROW-1);
	LCD_COL_SET(0,LCD_COL-1);
	LCD_Write_CMD(0x2C);
	while(temp--)
	{
		LCD_Write_DATA(color >> 8);
		LCD_Write_DATA(color & 0xFF);
	}
}

//矩形填充
void LCD_Fill(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend, uint16_t color)
{
	uint32_t num;
	LCD_ROW_SET(xstart,xend);
	LCD_COL_SET(ystart,yend);
	num = (uint32_t)((xend-xstart+1)*(yend-ystart+1));
	LCD_Write_CMD(0x2C);
	while(num--)
	{
		LCD_Write_DATA(color >> 8);
		LCD_Write_DATA(color & 0xFF);		
	}	
}

