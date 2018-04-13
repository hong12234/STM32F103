#include "main.h"
#include "ucosii_task.h"

#define FLASH_SAVE_ADDR  0X080FFF00 
/**********�ⲿ��������********/
extern  u8  Data_All[];
extern _Wifi_Information My_Wifi;
extern u16 AdcDma_Buf;
extern int acc;   
extern short a_x,a_y,a_z;    
extern GUI_CONST_STORAGE GUI_BITMAP bmstartmcu;

/**********��������************/
void ALL_test(void);
int System_Init(void);

/**********ȫ�ֱ�������*********/
SYSTEM_STA System_Sta;			//ϵͳ��־λ
SYSTEM_DATA	System_Data;		//ϵͳ����
SYSTEM_SET	System_Set;			//ϵͳ����
u8 H_Average_Buf[12]={0,0,0,0,0,0,0,0,0,0,0,0};//������ʵ���ֵ����11λ��ʾ�����Ƿ��Ѿ�10������10λ��ʾ��ǰ��ŵ��±�
u8 T_Average_Buf[12]={0,0,0,0,0,0,0,0,0,0,0,0};//������µ���ֵ����11λ��ʾ�����Ƿ��Ѿ�10������10λ��ʾ��ǰ��ŵ��±�

int main(void)
{  
	System_Init();
	OSInit();   				//��ʼ��UCOSII
	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	  	 			//����UCOSII
}


//ϵͳ�ܳ�ʼ��
int System_Init(void)
{
	/*ϵͳʱ��*/
	Stm32_Clock_Init(336,8,2,7);	//����ʱ��,168Mhz
	delay_init(168);				//��ʼ����ʱ����
	/*��������*/
	Key_Init();				//������ʼ��
	delay_ms(5000);		//�ȴ�10s
	START_MCU;				//����
	MOTOR_START;
	delay_ms(500);
	MOTOR_STOP;
	System_Sta.Mcu_Power = 1;
	/*��ȫ���ж�*/
	INTX_DISABLE();
	/*Ӳ����ʼ*/
	Usart_1_Init(84,115200);		//����2������WIFI
	Time_4_Init(1000-1,8399);		//��ʱ��4��ϵͳ���� 100ms
	TIM3_PWM_Init(500,83);			//����PWM			
	TIM3->CCR3 = 500;
	Time_1_Init(0xffff,16799);			//��ʱ��1���ʣ�0.1msһ������ֵ	
	MyAdc_DMAConfig(DMA2_Stream0,0,(u32)&ADC1->DR,(u32)&AdcDma_Buf,1);
	//MyAdc_DMAConfig(DMA2_Stream0,0,(u32)&ADC1->DR,(u32)&Adc_Buf,1);	//ADC��DMA
	RTC_Init();		//RTC��ʼ��
	/*�����ʼ��*/
	RCC->AHB1ENR |= (1<<12);		//ʹ��CRCʱ��
	GUI_Init();						//GUI��ʼ��
	GUI_UC_SetEncodeUTF8();
	/*�����ʼ*/
	Init_ST7789();		   	//��ʼ��LCD
	FT6236_Init();			//��ʼ��������
	GUI_SetBkColor(GUI_BLACK);
	GUI_Clear();
	GUI_DrawBitmap(&bmstartmcu,0,20);
	GUI_Exec();
	GUI_AA_EnableHiRes();
	GUI_AA_SetFactor(2);
	SMBus_Init();			//�¶ȴ�����
	Motor_Init();			//������ʼ��
	Adc_Init();				//����ADC��ʼ��
	delay_ms(1000);
	if(MPU6050_Init()){GUI_DispStringAt("mpu6050 err",120,120);GUI_Exec1();}//6050��ʼ��
	delay_ms(200);
	mpu_dmp_init(); 		//DMP��ʼ��
	MPU_Set_Accel_Fsr(3);	 //���ٶȴ�����,��2g
	Lsens_Init();			//������ʼ��
	IWDG_Init(4,20000);		//���ʱ��Ϊ2s
	/*��ȫ���ж�*/
	Interrupt_Set();
	INTX_ENABLE();
	System_Sta.Disp_Sta |= (1<<1);		//����������
	Wifi_Init();
	Read_Data();
	RTC_Set_Time(12,30,00,0);
	System_Sta.Get_Heart = 300;		//����һ�����ʼ��
	System_Sta.Get_Temp = 300;		//����һ�����¼��
	return 0;
}


//void ALL_test(void)
//{
////	u8 i = 0;
////	GUI_SetBkColor(GUI_WHITE);
////	GUI_Clear();
////	GUI_SetFont(&GUI_Font32_ASCII);
////	GUI_SetColor(GUI_RED);
////	while(1)
////	{
////		Check_Fall();
////		GUI_DispDecAt(acc,120,120,5);
//////			i = Lsens_Get_Val();
//////			TIM3->CCR3 = (i*10);
//////		GUI_Exec();
////		delay_ms(10);
////	}
//}

//��FLASH������
int Read_Data(void)
{
	STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)(&System_Set.warning_temp),1);
	STMFLASH_Read(FLASH_SAVE_ADDR+4,(u32*)(&System_Set.eat_min1),1);
	STMFLASH_Read(FLASH_SAVE_ADDR+8,(u32*)(&System_Set.eat_time1),1);
	STMFLASH_Read(FLASH_SAVE_ADDR+4,(u32*)(&System_Set.eat_min1),2);
	STMFLASH_Read(FLASH_SAVE_ADDR+8,(u32*)(&System_Set.eat_time1),2);
	STMFLASH_Read(FLASH_SAVE_ADDR+4,(u32*)(&System_Set.eat_min1),3);
	STMFLASH_Read(FLASH_SAVE_ADDR+8,(u32*)(&System_Set.eat_time1),3);
	STMFLASH_Read(FLASH_SAVE_ADDR+16,(u32*)(&System_Set.warning_fall),1);
	STMFLASH_Read(FLASH_SAVE_ADDR+20,(u32*)(&System_Set.warning_heart),1);
	STMFLASH_Read(FLASH_SAVE_ADDR+24,(u32*)(&System_Set.warning_eat),1);
	return 0;
}
//�������ݵ�FLASH
int Save_Data(void)
{
	STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)(&System_Set.warning_temp),1);
	STMFLASH_Write(FLASH_SAVE_ADDR+4,(u32*)(&System_Set.eat_min1),1);
	STMFLASH_Write(FLASH_SAVE_ADDR+8,(u32*)(&System_Set.eat_time1),1);	
	STMFLASH_Write(FLASH_SAVE_ADDR+4,(u32*)(&System_Set.eat_min2),1);
	STMFLASH_Write(FLASH_SAVE_ADDR+8,(u32*)(&System_Set.eat_time2),1);	
	STMFLASH_Write(FLASH_SAVE_ADDR+4,(u32*)(&System_Set.eat_min3),1);
	STMFLASH_Write(FLASH_SAVE_ADDR+8,(u32*)(&System_Set.eat_time3),1);
	STMFLASH_Write(FLASH_SAVE_ADDR+16,(u32*)(&System_Set.warning_fall),1);
	STMFLASH_Write(FLASH_SAVE_ADDR+20,(u32*)(&System_Set.warning_heart),1);
	STMFLASH_Write(FLASH_SAVE_ADDR+24,(u32*)(&System_Set.warning_eat),1);
	return 0;
}

