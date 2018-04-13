/*
**************************************************************
*
* �ļ����ƣ�st7789.c;
*
* �汾˵����V1.1��
*
* ���л�����(1) STM32F103RCT;
*           (2) �ⲿ����8M9��Ƶ��
*           (3) MDK 5.0��
*
* �޸�˵�����ޣ�
*           
* ������ڣ�2016/06/25;
*
* ����������֣��˸;
*
* ������ע��1.54��Һ������������ 80���ڣ�����оƬst7789��
			ffffHΪ��ɫ��0000HΪ��ɫ����������ֵԽ����ɫԽ����
*          
* �޸�˵����2016/6/25 ���Ӷ��㺯����
*
**************************************************************
*/

#include "st7789.h"


_lcd_dev lcd;


/*
* �������ƣ�Init_ST7789��
* ����˵����st7789��ʼ����
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void Init_ST7789(void)
{
	GPIO_Clock_Set(IOPCEN);
	GPIO_Clock_Set(IOPAEN);
	GPIO_Clock_Set(IOPBEN);
	//GPIO_Init(GPIOB,P0,IO_MODE_OUT,IO_SPEED_50M,GPIO_OTYPE_PP,IO_KEEP,IO_H);
	GPIO_Init(GPIOC,P3,IO_MODE_OUT,IO_SPEED_50M,GPIO_OTYPE_PP,IO_KEEP,IO_H);//PC3 RD
	GPIO_Init(GPIOC,P2,IO_MODE_OUT,IO_SPEED_50M,GPIO_OTYPE_PP,IO_KEEP,IO_H);//PC2 RST
	GPIO_Init(GPIOB,P7,IO_MODE_OUT,IO_SPEED_50M,GPIO_OTYPE_PP,IO_KEEP,IO_H);//PB7 WR
	GPIO_Init(GPIOB,P8,IO_MODE_OUT,IO_SPEED_50M,GPIO_OTYPE_PP,IO_KEEP,IO_H);//PB8 RS
	GPIO_Init(GPIOB,P9,IO_MODE_OUT,IO_SPEED_50M,GPIO_OTYPE_PP,IO_KEEP,IO_H);//PB9 WR

	GPIOC->BSRRH &= ~(1<<2);
	delay_ms(100);
	GPIOC->BSRRL |= (1<<2);	//����RST
	//��������
	GPIOA->MODER &= 0xffff0000;		//PA0-7 ��0
	GPIOA->MODER |= 0x00005555;		//PA0-7 ���
	GPIOA->OTYPER &= 0xff00;		//����
	GPIOA->ODR &= 0xFF00;     		//ȫ�������
	
	Write_ST7789_Reg(0x11); 
	delay_ms(120);                                 //Delay 120ms 
//--------Display and color format setting---------// 

	Write_ST7789_Reg(0x36); 
	Write_ST7789_Data (0x00); 
	Write_ST7789_Reg(0x36); 
	Write_ST7789_Data (0x00); 

	Write_ST7789_Reg(0x3a); 
	Write_ST7789_Data (0x05); //05

	Write_ST7789_Reg(0x2a); 
	Write_ST7789_Data (0x00); 
	Write_ST7789_Data (0x00); 
	Write_ST7789_Data (0x00); 
	Write_ST7789_Data (0xef); 

	Write_ST7789_Reg(0x2b); 
	Write_ST7789_Data (0x00); 
	Write_ST7789_Data (0x00); 
	Write_ST7789_Data (0x00); 
	Write_ST7789_Data (0xef); 
//-----------ST7789S Frame rate setting-----------// 

	Write_ST7789_Reg(0xb2); 
	Write_ST7789_Data (0x0c); 
	Write_ST7789_Data (0x0c); 
	Write_ST7789_Data (0x00); 
	Write_ST7789_Data (0x33); 
	Write_ST7789_Data (0x33); 

	Write_ST7789_Reg(0xb7); 
	Write_ST7789_Data (0x35); 

//-----------ST7789S Power setting----------------// 

	Write_ST7789_Reg(0xbb); 
	Write_ST7789_Data (0x2C); 

	Write_ST7789_Reg(0xc0); 
	Write_ST7789_Data (0x2c); 

	Write_ST7789_Reg(0xc2); 
	Write_ST7789_Data (0x01); 

	Write_ST7789_Reg(0xc3); 
	Write_ST7789_Data (0x0b); 

	Write_ST7789_Reg(0xc4); 
	Write_ST7789_Data (0x20); 

	Write_ST7789_Reg(0xc6); 
	Write_ST7789_Data (0x0f); 

	Write_ST7789_Reg(0xd0); 
	Write_ST7789_Data (0xa4); 
	Write_ST7789_Data (0xa1); 

//----------ST7789S gamma setting---------------// 
	Write_ST7789_Reg(0xe0); 
	Write_ST7789_Data (0xd0); 
	Write_ST7789_Data (0x06); 
	Write_ST7789_Data (0x01); 
	Write_ST7789_Data (0x0e); 
	Write_ST7789_Data (0x0e); 
	Write_ST7789_Data (0x08); 
	Write_ST7789_Data (0x32); 
	Write_ST7789_Data (0x44); 
	Write_ST7789_Data (0x40); 
	Write_ST7789_Data (0x08); 
	Write_ST7789_Data (0x10); 
	Write_ST7789_Data (0x0f); 
	Write_ST7789_Data (0x15); 
	Write_ST7789_Data (0x19); 

	Write_ST7789_Reg(0xe1); 
	Write_ST7789_Data (0xd0); 
	Write_ST7789_Data (0x06); 
	Write_ST7789_Data (0x01); 
	Write_ST7789_Data (0x0f); 
	Write_ST7789_Data (0x0e); 
	Write_ST7789_Data (0x09); 
	Write_ST7789_Data (0x2f); 
	Write_ST7789_Data (0x54); 
	Write_ST7789_Data (0x44); 
	Write_ST7789_Data (0x0f); 
	Write_ST7789_Data (0x1d); 
	Write_ST7789_Data (0x1a); 
	Write_ST7789_Data (0x16); 
	Write_ST7789_Data (0x19); 
	
	Write_ST7789_Reg(0x21);                 //��ɫ��ת����

	Write_ST7789_Reg(0x29);                 //����ʾ��0x28�ǹأ�
	Address_Set(0,0);
}

 
/*
* �������ƣ�Write_ST7789_Reg��
* ����˵����д���
* ��ڲ�����hibyte->���
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void Write_ST7789_Reg(u8 hibyte)
{ 
	RS =0;//delay_us(1);
	RDB=1;//delay_us(1);
	CS1=0;//delay_us(1);  //Ƭѡ��
    WRD=0;
	DATA_OUT(hibyte);
	WRD=1;
    CS1=1;
}
 

/*
* �������ƣ�Write_ST7789_Data��
* ����˵����д���ݣ�
* ��ڲ�����hibyte->���ݣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void Write_ST7789_Data(u8 hibyte)
{    
	RS =1;//delay_us(1);
	RDB=1;//delay_us(1);  
	CS1=0;//delay_us(1);  //Ƭѡ��
    WRD=0;
	DATA_OUT(hibyte);
	WRD=1;
    CS1=1;	
}


/*
* �������ƣ�Read_ST7789_Data��
* ����˵���������ݣ�
* ��ڲ������ޣ�
* ����ֵ  ��8λ���ݣ�
* ��ע    ���ޣ�
*/
u8 Read_ST7789_Data(void)
{   
	u8 r_data;
//	GPIOF->MODER &= 0xffff0000;		//PF0-7 ����
//	GPIOF->PUPDR &= 0xffff0000;		//PF0-7 ����
//	GPIOF->PUPDR |= 0x00005555;
//	GPIOF->ODR &= 0Xff00;     		//ȫ�����0
	
	GPIOA->MODER &= 0xffff0000;		//PF0-7 ����
	GPIOA->PUPDR &= 0xffff0000;		//PF0-7 ����
	GPIOA->PUPDR |= 0x00005555;
	GPIOA->ODR &= 0Xff00;     

	RS =1;
	WRD=1;
	CS1=0;
    RDB=0;
	delay_us(1);            //�����أ�
	r_data=DATA_READ();     //�������ݣ�
	RDB=1;
	delay_us(1); 
    CS1=1;
	
//	GPIOF->MODER &= 0xffff0000;		//PA0-7 ��0
//	GPIOF->MODER |= 0x00005555;		//PA0-7 ���
//	GPIOF->OTYPER &= 0xff00;		//����
//	GPIOF->ODR |= 0x00FF;     		//ȫ�������
	GPIOA->MODER &= 0xffff0000;		//PA0-7 ��0
	GPIOA->MODER |= 0x00005555;		//PA0-7 ���
	GPIOA->OTYPER &= 0xff00;		//����
	GPIOA->ODR |= 0x00FF;   
	

	return r_data;  
}


void  Address_Set(u16 x,u16 y)
 {
	Write_ST7789_Reg(0x2a);        //����x���ꣻ
	Write_ST7789_Data(x>>8);
	Write_ST7789_Data(x&0xff); 
	Write_ST7789_Data(0x00);
	Write_ST7789_Data(0xf0); 
	Write_ST7789_Reg(0x2b);        //����y���ꣻ
	Write_ST7789_Data(y>>8);
	Write_ST7789_Data(y&0xff);
	Write_ST7789_Data(0x00);
	Write_ST7789_Data(0xf0); 
	Write_ST7789_Reg(0x2c);
 }


/*
* �������ƣ�Write_ST7789_16Bit��
* ����˵����д16λ��ɫ���ݣ�
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void Write_ST7789_16Bit(u8 hibyte,u8 lowbyte)
{   	
	RS =1;
	RDB=1; 
	CS1=0;                 //Ƭѡ��
    WRD=0;
	DATA_OUT(hibyte);
	WRD=1;
    CS1=1;	
	
	RS=1;
	CS1=0;
    WRD=0; 
	DATA_OUT(lowbyte);
    WRD=1;
    CS1=1;
}


/*
* �������ƣ�Write_ST7789_Point��
* ����˵����д�����ɫ���ݣ�
* ��ڲ�����Ŀ�����ꣻ
* ����ֵ  ��16λ��ɫֵ��
* ��ע    ���ޣ�
*/
void Write_ST7789_Point(u16 x,u16 y,u16 color)
{
	Write_ST7789_Reg(0x2a);        //����x���ꣻ
	Write_ST7789_Data((x>>8));
	Write_ST7789_Data((x&0xff)); 		
	Write_ST7789_Reg(0x2b);        //����y���ꣻ	if((x&0xff00) != 0)	
	Write_ST7789_Data((y>>8));
	Write_ST7789_Data((y&0xff));

	Write_ST7789_Reg(0x2c);
	
	RS =1;
	RDB=1; 
	CS1=0;                         //Ƭѡ��
    WRD=0;
	DATA_OUT(color>>8);
	//DATA_OUT(0xff);
	WRD=1;
    CS1=1;	
	
	RS=1;
	CS1=0;
    WRD=0; 
	DATA_OUT(color&0x00ff);
	//DATA_OUT(0xff);
    WRD=1;
    CS1=1;
}



/*
* �������ƣ�Read_ST7789_Point��
* ����˵�����������ɫ���ݣ�
* ��ڲ�����Ŀ�����ꣻ
* ����ֵ  ��16λ��ɫֵ��
* ��ע    ���ޣ�
*/
u16 Read_ST7789_Point(u16 x,u16 y)
{   
	u16 r,g,b;
	Address_Set(x,y);        //ָ�����ꣻ
	Write_ST7789_Reg(0x3e);  //���ڴ����
	Read_ST7789_Data();      //��Ч��
	
	r=Read_ST7789_Data();    //r��b��g�����ݸ�ʽΪǰ5λ��6λ��
	g=Read_ST7789_Data();
	b=Read_ST7789_Data();
	
	return ((r>>3)<<11)|((g>>2)<<5)|(b>>3);
}


u8 Read_LCD_Reg(u8 reg)
{
	Write_ST7789_Reg(reg);
	Read_ST7789_Data();
	return Read_ST7789_Data();
}

void Draw_Background(u8 x_s,u8 y_s,u8 length,u8 wide,u16 color)
{
	u16 i,j;
	for(i=y_s;i<=wide;i++)
	{	
		Address_Set(x_s,i);
		Write_ST7789_Reg(0x2C);
		for(j=x_s;j<=length;j++)
		{			
			RS =1;
			RDB=1; 
			CS1=0;                         //Ƭѡ��
			WRD=0;
			DATA_OUT(color>>8);
			WRD=1;
			CS1=1;	
			
			RS=1;
			CS1=0;
			WRD=0; 
			DATA_OUT(color&0x00ff);
			WRD=1;
			CS1=1;
		}
	}
}

