///*
//**************************************************************
//*
//* �ļ����ƣ�mpu6050.c;
//*
//* �汾˵����V.10��
//*
//* ���л�����(1) STM32F103RCT;
//*           (2) �ⲿ����8M9��Ƶ��
//*           (3) MDK 5.0��
//*
//* �޸�˵�������ޣ�
//*           
//* ������ڣ�2016/04/24;
//*
//* ����������֣��˸;
//*
//* ������ע��mpu6050��ȡʵ�飬ģ������ԭ�����̣�
//*
//**************************************************************
//*/

#include "mpu6050.h"
#include "main.h"

extern SYSTEM_STA System_Sta;	
extern float Pitch;       //pitch(��inv_mpu.c�ﶨ��)
extern float Roll;        //roll
extern float Yaw;         //float

//u8 unlock = 0;            //6050������־��1->��⵽����0->û�У�



short a_x,a_y,a_z;             //���ٶ�ֵ�ݴ浥Ԫ��
int acc;                       //�ϼ��ٶȣ�
int sin_temp;                //�ռ�����ֵ��

u8 MPU6050_Init(void)
{
	u8 res=0;
	MPU_IIC_Init();                               //�˿ڳ�ʼ����
	delay_ms(1000);
	//GPIO_Init(GPIOB,P12,IN_MODE,PULL,PULL_UP);    //PA2Ϊ�������룻
	//JTAG_Set(SWD_ENABLE);	                      //��ֹJTAG,�Ӷ�PA15��������ͨIOʹ��,����PA15��������ͨIO!!!
	//
	res = MPU_Write_Reg(MPU_PWR_MGMT1_REG,0X80);	      //��λMPU6050��
	res = MPU_Write_Reg(MPU_PWR_MGMT1_REG,0X80);	      //��λMPU6050��
	res = MPU_Read_Reg(MPU_PWR_MGMT1_REG);
    delay_ms(5000);
	res = MPU_Write_Reg(MPU_PWR_MGMT1_REG,0X01);	      //����MPU6050 ��
	res =MPU_Read_Reg(MPU_PWR_MGMT1_REG);
	delay_ms(1000);
	MPU_Set_Gyros_Fsr(3);					      //�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(3);					      //���ٶȴ�����,��2g
	MPU_Set_Rate(50);						      //���ò�����50Hz
	MPU_Write_Reg(MPU_INT_EN_REG,0X00);	          //�ر������ж�
	MPU_Write_Reg(MPU_USER_CTRL_REG,0X00);	      //I2C��ģʽ�ر�
	MPU_Write_Reg(MPU_FIFO_EN_REG,0X00);	      //�ر�FIFO
	MPU_Write_Reg(MPU_INTBP_CFG_REG,0X80);	      //INT���ŵ͵�ƽ��Ч
	res=MPU_Read_Reg(MPU6050_ADDR);
	if(res==MPU_ADDR)                             //����ID��ȷ
	{
		res=MPU_Write_Reg(MPU_PWR_MGMT1_REG,0X01);    //����CLKSEL,PLL X��Ϊ�ο�
		MPU_Write_Reg(MPU_PWR_MGMT2_REG,0X00);	  //���ٶ��������Ƕ�����
		MPU_Set_Rate(50);						  //���ò�����Ϊ50Hz
 	}
	else return 1;                                //����
	res =MPU_Read_Reg(MPU_PWR_MGMT1_REG);
	//MPU_Write_Reg(MPU_PWR_MGMT1_REG,0X01);
	return 0;                                     //��ʼ���ɹ���
}


/*
* �������ƣ�MPU_Write_Reg��
* ����˵����дһ���ֽڵ��Ĵ�����
* ��ڲ�����reg->�Ĵ����أ�data->���͵����ݣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
u8 MPU_Write_Reg(u8 reg,u8 data)
{
	MPU_IIC_Start();
	MPU_IIC_Send_Byte(MPU_WRITE);
	if(MPU_IIC_Wait_Ack())
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Send_Byte(reg);
	if(MPU_IIC_Wait_Ack())
	{
		MPU_IIC_Stop();
		return 2;
	}
	MPU_IIC_Send_Byte(data);
	MPU_IIC_Wait_Ack();	
	MPU_IIC_Stop();
	return 0;
}


/*
* �������ƣ�MPU_Read_Reg��
* ����˵�������Ĵ���һ���ֽ����ݣ�
* ��ڲ�����Reg->�Ĵ����أ�
* ����ֵ  ��data��
* ��ע    ���ޣ�
*/
u8 MPU_Read_Reg(u8 reg)
{
	u8 data;
	MPU_IIC_Start();
	MPU_IIC_Send_Byte(MPU_WRITE);
	MPU_IIC_Wait_Ack();
	MPU_IIC_Send_Byte(reg);
	MPU_IIC_Wait_Ack();
	MPU_IIC_Start();
	MPU_IIC_Send_Byte(MPU_READ);
	MPU_IIC_Wait_Ack();
	data=MPU_IIC_Read_Byte(0);
	MPU_IIC_Stop();
	return data;
}


/*
* �������ƣ�MPU_Write_Len��
* ����˵��������д�Ĵ�����
* ��ڲ�����reg->�Ĵ����أ�buf->���͵����ݣ�
* ����ֵ  ��0->:�ɹ�������->:ʧ�ܣ�
* ��ע    ���ޣ�
*/
u8 MPU_Write_Len(u8 mpu_addr,u8 reg,u8 len,u8* buf)
{
	u8 i;
	MPU_IIC_Start();
	MPU_IIC_Send_Byte(mpu_addr<<1);
	if(MPU_IIC_Wait_Ack())
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Send_Byte(reg);
	MPU_IIC_Wait_Ack();
	for(i=0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);
		if(MPU_IIC_Wait_Ack())
		{
			MPU_IIC_Stop();
			return 0xff;
		}			
	}
	MPU_IIC_Stop();
	return 0;
}


/*
* �������ƣ�MPU_Read_Len��
* ����˵�����������Ĵ�����
* ��ڲ�����reg->�Ĵ�����buf->���ݽ�������
* ����ֵ  ��0->:�ɹ�������->:ʧ�ܣ���
* ��ע    ���ޣ�
*/
u8 MPU_Read_Len(u8 mpu_addr,u8 reg,u8 len,u8* buf)
{
	u8 i;
	MPU_IIC_Start();
	MPU_IIC_Send_Byte(mpu_addr<<1);
	MPU_IIC_Wait_Ack();
	MPU_IIC_Send_Byte(reg);
	MPU_IIC_Wait_Ack();
	
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((mpu_addr<<1)|1);
	MPU_IIC_Wait_Ack();  
	for(i=0;i<len-1;i++)
	{
		buf[i]=MPU_IIC_Read_Byte(1);       //����ACK��
	}
	buf[i]=MPU_IIC_Read_Byte(0);           //����NACK��
	MPU_IIC_Stop();
	return 0;
}


/*
* �������ƣ�MPU_Read_Temperature��
* ����˵������ȡ�¶ȣ�
* ��ڲ������ޣ�
* ����ֵ  ���¶�ֵ(������100��)��
* ��ע    ���ޣ�
*/
short MPU_Read_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}


/*
* �������ƣ�MPU_Read_Accel��
* ����˵������ȡ���ٶ�ԭʼ���ݣ�
* ��ڲ��������ٶȴ洢����
* ����ֵ  ��0->�ɹ�������->��ʧ�ܣ�
* ��ע    ���ޣ�
*/
u8 MPU_Read_Accel(short *ax,short *ay,short *az)
{
	u8 read,buf[6];
	read=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(read==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];
		*ay=((u16)buf[2]<<8)|buf[3];
		*az=((u16)buf[4]<<8)|buf[5];
	}
	return read;
}


/*
* �������ƣ�MPU_Read_Gyros��
* ����˵������ȡ������ԭʼ���ݣ�
* ��ڲ��������ٶȴ洢����
* ����ֵ  ��0->�ɹ�������->��ʧ�ܣ�
* ��ע    ���ޣ�
*/
u8 MPU_Read_Gyros(short *gx,short *gy,short *gz)
{
	u8 read,buf[6];
	read=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(read==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];
		*gy=((u16)buf[2]<<8)|buf[3];
		*gz=((u16)buf[4]<<8)|buf[5];
	}
	return read;
}


/*
* �������ƣ�MPU_Set_Gyros_Fsr��
* ����˵�������������������̣�
* ��ڲ�����0,1,2,3��
* ����ֵ  ��0->�ɹ�������->��ʧ�ܣ�
* ��ע    ���ޣ�
*/
u8 MPU_Set_Gyros_Fsr(u8 fs_sel)
{
	return MPU_Write_Reg(MPU_GYRO_CFG_REG,fs_sel<<3);
}


/*
* �������ƣ�MPU_Set_Accel_Fsr��
* ����˵�������ü��ٶȼ������̣�
* ��ڲ�����0,1,2,3��
* ����ֵ  ��0->�ɹ�������->��ʧ�ܣ�
* ��ע    ���ޣ�
*/
u8 MPU_Set_Accel_Fsr(u8 afs_sel)
{
	return MPU_Write_Reg(MPU_ACCEL_CFG_REG,afs_sel<<3);
}


/*
* �������ƣ�MPU_Set_Configuration��
* ����˵�������������˲�����
* ��ڲ�����0--7Hz��
* ����ֵ  ��0->�ɹ�������->��ʧ�ܣ�
* ��ע    �����ݹ�ʽת��Ϊ��Ƶϵ����
*/
u8 MPU_Set_Configuration(u8 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Reg(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}


/*
* �������ƣ�MPU_Set_Rate��
* ����˵�������ò������ʣ�
* ��ڲ�����4--1000(Hz)��
* ����ֵ  ��0->�ɹ�������->��ʧ�ܣ�
* ��ע    �����ݹ�ʽת��Ϊ��Ƶϵ����
*/
u8 MPU_Set_Rate(u16 rate)
{
	u16 div;
	if(rate>1000)                //�������ʣ�
	{rate=1000;}
	if(rate<4)
	{rate=4;}
	div=1000/rate-1;             //����Ƶ��=���������Ƶ��/��1+SMPLRT_DIV��
	MPU_Write_Reg(MPU_SAMPLE_RATE_REG,div);
	return MPU_Set_Configuration(rate/2);
}


/*
* �������ƣ�Action_Task��
* ����˵�����жϷ������ƣ�
* ��ڲ������ޣ�
* ����ֵ  ���ޣ�
* ��ע    ���ޣ�
*/
void Action_Task(void)
{
	static u8 cont_flag = 0;           //bit0:�Ƿ���з����жϱ�־��bit1:Roll�ﵽ90���־��bit2:��ɷ�������־��
	static u8 cont = 0;                
	static int last_data = 0;
	int my_angle;
//	while(mpu_dmp_get_data()!=0);
	if(mpu_dmp_get_data()==0)
	{
		my_angle = Pitch;
		if(((cont_flag&0x01)==1)||(my_angle>-10&&my_angle<10))   //�ж�С���Ƿ��Ŵ���ˮƽ״̬�����������������жϣ�
		{
			cont_flag |= 1<<0;                               //��⵽���������״̬����־��1����ʾ���������������жϣ�
			if((cont_flag&(1<<1))==0)                        //�ж��Ƿ�û�����趨ֵ��
			{
				if(my_angle<last_data)
				{	
					cont = 0;
					last_data = my_angle; 
					if(my_angle<-70)
					{
						cont_flag |= 1<<1;                    //����90�㣬��־cont_flag[1];
						cont = 0;
					}
				}
				else if(my_angle==last_data)
				{
					cont++;
					if(cont==50)
					{
						cont = 0;
						cont_flag = 0;                       //�������󣬴˴��ж���Ч��
					}
				}
			}
			else
			{		
				if(my_angle<=-70)
				{
					cont++;     
					if(cont>50)                            //�����趨�ǶȺ�ͣ����ʱ��
					{
						cont = 0;
						cont_flag = 0;                     //�������󣬴˴��ж���Ч��
					}
				}
				else if(my_angle>last_data)
				{
					last_data = my_angle;                            
					if(my_angle>-10)
					{
						System_Sta.unlock = 1;                        //�����ɹ���
						cont_flag |= 1<<2;                 //��־cont_flag[2];
						cont = 0;
						cont_flag &= ~(3<<0);
					}
				}
			}
		}
	}
}


void Check_Fall(void)
{
	static u8 time=0,j=0,flag=0;
	float buf;
	MPU_Read_Accel(&a_x,&a_y,&a_z);     //��ȡ���ٶȣ�
	acc=sqrt((a_x*a_x+a_y*a_y+a_z*a_z));  //�õ��ϼ��ٶȣ�
	if(acc>15000){flag=1;}              //���񵽿�����Ϣ��
	if(flag)                            //��������ȡֵ��
	{
		if(++time==200)                 //�ж��Ƿ񵽴�2s������ǣ�������ȡֵ20����ƽ����
		{
			if(++j<20)
			{
				arm_sqrt_f32(a_y*a_y+a_z*a_z,&buf);
				sin_temp+=(int)(buf/acc*5000); //�õ��ռ�����ֵ���Ŵ���5000��(0~5000)��
				time=199;
			}
			else
			{
				sin_temp/=20;           //�õ�20�νǶȲ�����ƽ��ֵ��
				j=0;                    //�������㣻
				time=0;
				flag=0;
				if(sin_temp>4600)
				{System_Sta.Tumble_Flag|=(1<<7);sin_temp=0;}         //ˤ��������Ƕȣ�
				else
				{System_Sta.Tumble_Flag&=~(1<<7);}        //ûˤ����
			}
		}
	}
}

