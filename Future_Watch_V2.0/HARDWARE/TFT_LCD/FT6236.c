/*
**************************************************************
*
* �ļ����ƣ�FT6236.c;
*
* �汾˵����V1.0��
*
* ���л�����(1) STM32F103RCT;
*           (2) �ⲿ����8M9��Ƶ��
*           (3) MDK 5.0��
*
* �޸�˵�����ޣ�
*           
* ������ڣ�2016/06/26;
*
* ����������֣��˸;
*
* ������ע������оƬFT6236��������
*           ���Գ������̣��ο�����ԭ��FT5206��
*          
* �޸�˵�����ޣ�
*
**************************************************************
*/

#include "FT6236.h"
#include "main.h"

TouchPointRefTypeDef TPR_Structure; 
u8 Touch_Buf[30];
const u16 FT6236_TPX_TBL[5]=
{
	FT_TP1_REG,
	FT_TP2_REG,
	FT_TP3_REG,
	FT_TP4_REG,
	FT_TP5_REG
};


/*
* �������ƣ�FT6236_Init��
* ����˵����FT6236��ʼ����
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/		
void FT6236_Init(void)
{
	u8 temp; 
	GPIO_Clock_Set(IOPBEN);                              //��ʹ������IO PORTCʱ�� 	
	GPIO_Clock_Set(IOPCEN);
	TPR_Structure.Point_Num = 0;		//����������0
	TPR_Structure.Gesture_Sta = 0;		//����������0
	GPIO_Init(GPIOC,P12,IO_MODE_OUT,IO_SPEED_50M,GPIO_OTYPE_PP,IO_PULL,IO_H); //PC12 SDA
	GPIO_Init(GPIOB,P3,IO_MODE_OUT,IO_SPEED_50M,GPIO_OTYPE_PP,IO_PULL,IO_H); //PB3 SCL
	GPIO_Init(GPIOC,P11,IO_MODE_OUT,IO_SPEED_50M,GPIO_OTYPE_PP,IO_PULL,IO_H); //PC11 RST
	GPIO_Init(GPIOB,P4,IO_MODE_IN,IO_SPEED_50M,GPIO_OTYPE_PP,IO_PULL,IO_H);	//PB4 ENINT
	FT6236_RST = 0;
	delay_ms(20);
	FT6236_RST = 1;
	delay_ms(60);
	temp=0;  
	FT6236_Write_Reg(FT_DEVIDE_MODE,&temp,1);   //������������ģʽ 	
 	temp=22;                                      //������Чֵ��22��ԽСԽ����	
 	FT6236_Write_Reg(FT_ID_G_THGROUP,&temp,1);  //���ô�����Чֵ
 	temp=12;                                      //�������ڣ�����С��12�����14
 	FT6236_Write_Reg(FT_ID_G_PERIODACTIVE,&temp,1); 
}


/*
* �������ƣ�FT6236_Write_Reg��
* ����˵����FT6236����д�Ĵ�����
* ��ڲ�����reg-->�Ĵ�����ַ��buf-->���ͻ���ռ䣬len-->�������ȣ�
* ����ֵ  ��0-->�ɹ���1-->ʧ�ܣ�
* ��ע    ���ޣ�
*/
u8 FT6236_Write_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	FT6236_IIC_Start();
	
	FT6236_Send_Byte(FT_CMD_WR);        //����д���
	FT6236_Wait_Ack();
	
	FT6236_Send_Byte(reg&0XFF);         //���͵�8λ��ַ
	FT6236_Wait_Ack();
	
	for(i=0;i<len;i++)
	{
    	FT6236_Send_Byte(buf[i]);       //������
		ret=FT6236_Wait_Ack();
		if(ret)break;                   //Ӧ��Ϊ'1'��ʧ�ܣ��������أ�
	}
    FT6236_IIC_Stop();                  //����һ��ֹͣ����	    
	return ret; 
}


/*
* �������ƣ�FT6236_Read_Reg��
* ����˵����FT6236�������Ĵ�����
* ��ڲ�����reg-->�Ĵ�����ַ��buf-->���ջ���ռ䣬len-->�������ȣ�
* ����ֵ  ��0-->�ɹ���1-->ʧ�ܣ�
* ��ע    ���ޣ�
*/		  
u8 FT6236_Read_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	FT6236_IIC_Start();	
	
 	FT6236_Send_Byte(FT_CMD_WR);                  //����д���� 	 
	FT6236_Wait_Ack();		
	
	FT6236_Send_Byte(reg&0XFF);                   //���͵�8λ��ַ
	FT6236_Wait_Ack();
	  
 	FT6236_IIC_Start();  
	
	FT6236_Send_Byte(FT_CMD_RD);                  //���Ͷ�����		
	FT6236_Wait_Ack();			

	for(i=0;i<len;i++)
	{	   
		*buf++=FT6236_Read_Byte(i==(len-1)?0:1);  //����1B���ݵ��������ݻ�������  
	} 
    FT6236_IIC_Stop();                            //����һ��ֹͣ����	  
	return 0;
} 



int fun(u16 a[], int n)
{
    if(n==1)
        return 1;
    if( n==2 )
        return (a[n-1]) >= a[n-2];
    return fun(a,n-1) && ((a[n-1]) >= a[n-2]);
}


int Gesture_Scan(void)
{
	float TanPoint;
	if(fun(TPR_Structure.Point_X, 29))		//X����
	{
		if(fun(TPR_Structure.Point_Y, 29))	//Y����
		{
			TanPoint = (float)(TPR_Structure.Point_Y[5]-TPR_Structure.Point_Y[29])/(float)((TPR_Structure.Point_X[5]-TPR_Structure.Point_X[29]));
			
			if(TanPoint>=(float)1.1)		//����
			{	
				return 3;
			}
			else					//����
			{
				if(((float)-0.10<=TanPoint) & (TanPoint<= (float)0.6))
				{
					return 2;
				}
				else
				{
					if(TanPoint <= (float)-0.2)
					{
						return 3;
					}
					if(!(TanPoint == TanPoint))
					{
						return 2;
					}
				}
				return 5;
			}
		}
		else						//Y�ݼ�
		{
			TanPoint = (-(float)(TPR_Structure.Point_Y[5]-TPR_Structure.Point_Y[29]))/(float)((TPR_Structure.Point_X[5]-TPR_Structure.Point_X[29]));
			if(TanPoint >= (float)1.1)	//����
			{
				return 1;
			}
			else			//����
			{
				if(((float)-0.10<=TanPoint) & (TanPoint<= (float)0.6))
				{
					return 2;
				}
				else
				{
					if(TanPoint <= -0.2)
					{
						return 1;
					}
					if(!(TanPoint == TanPoint))
					{
						return 1;
					}
				}
				return 4;
			}
		}
	}
	else				//X�ݼ�
	{
		
		if(fun(TPR_Structure.Point_Y, 29))	//Y����
		{
			TanPoint = (float)((TPR_Structure.Point_Y[5]-TPR_Structure.Point_Y[29]))/(-(float)(TPR_Structure.Point_X[5]-TPR_Structure.Point_X[29]));
			if(TanPoint >= (float)1.1)	//����
			{
				return 3;
			}
			else			//����
			{
				if(((float)-0.10<=TanPoint) && (TanPoint<= (float)0.6))
				{
					return 0;
				}
				else
				{
					if(TanPoint <= (float)-0.2)
					{
						return 3;
					}
					if(!(TanPoint == TanPoint))
					{
						return 0;
					}
				}
				return 5;
			}	
		}
		else						//Y�ݼ�
		{
			TanPoint = ((float)(TPR_Structure.Point_Y[5]-TPR_Structure.Point_Y[29]))/((float)(TPR_Structure.Point_X[5]-TPR_Structure.Point_X[29]));
			if((TanPoint) >= (float)(1.1))	//����
			{
				return 1;
			}
			else			//����
			{
				if(((float)-0.10<=TanPoint) && (TanPoint<= (float)0.6))
				{
					return 0;
				}
				else
				{
					if(TanPoint <= -0.2)
					{
						return 1;
					}
					if(!(TanPoint == TanPoint))
					{
						return 0;
					}
				}
				return 5;
			}
		}
	}
}


/*
* �������ƣ�FT6236_Scan��
* ����˵��������ɨ�裻
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/	
void FT6236_Scan(void)
{
	u8 sta = 0;	
	u8 buf[4] = {0}; 	
	FT6236_Read_Reg(0x02,&sta,1);                           //��ȡ�������״̬�Ĵ�����  	   
 	if(sta&0x0f)                                            //�ж��Ƿ��д����㰴�£�0x02�Ĵ����ĵ�4λ��ʾ��Ч�������(0~5)��
 	{

		TPR_Structure.TouchSta=~(0xFF<<(sta&0x0F));         //~(0xFF << (sta & 0x0F))����ĸ���ת��Ϊ�����㰴����Ч��־
 		if(TPR_Structure.TouchSta&(1<<0))               //��ȡ����������
 		{											    //���������ȡ��Ӧ��������������
			FT6236_Read_Reg(FT6236_TPX_TBL[0],buf,4);	//��ȡXY����ֵ
			if((buf[0]&0XC0)!=0X80)
 			{
				TPR_Structure.x[0]=TPR_Structure.y[0]=0xffff;	
				TPR_Structure.Point_Num = 0;		//��0
				return;
			}
			TPR_Structure.LastX_Point=TPR_Structure.x[0];
			TPR_Structure.LastY_Point=TPR_Structure.y[0];
			TPR_Structure.x[0]=((u16)(buf[0]&0X0F)<<8)|buf[1];		
			TPR_Structure.y[0]=((u16)(buf[2]&0X0F)<<8)|buf[3];
 			if((TPR_Structure.Gesture_Sta&(1<<7)) == 0)
			{
				TPR_Structure.Point_X[TPR_Structure.Point_Num]= TPR_Structure.x[0];	//�洢X��
				TPR_Structure.Point_Y[TPR_Structure.Point_Num]= TPR_Structure.y[0];	//�洢Y��
				TPR_Structure.Point_Num++;		//����+1
			}
 		}
 		TPR_Structure.TouchSta |= TP_PRES_DOWN;             //�������±��
		if(TPR_Structure.Point_Num == 30)
		{
			TPR_Structure.Gesture_Sta |= (1<<7);			//���Ʒ�����־λ
			TPR_Structure.Point_Num = 0;					//������0
		}
	}
 	else
 	{
 		if(TPR_Structure.TouchSta &TP_PRES_DOWN) 	        //֮ǰ�Ǳ����µ� ��һ���ɿ����������
 		{
			TPR_Structure.TouchSta &= ~0x80;                //�����ɿ����
			if((TPR_Structure.Gesture_Sta&(1<<7)) != 0)
			{	
				TPR_Structure.Gesture_Sta |= (1<<6);			//��ʼ�����жϱ�־λ
			}
			TPR_Structure.Point_Num = 0;
		}
 		else
 		{
			TPR_Structure.x[0]=TPR_Structure.y[0]=0xffff;
 			TPR_Structure.TouchSta &=0xe0;	                //�����������Ч���
 		}
 	}
}


