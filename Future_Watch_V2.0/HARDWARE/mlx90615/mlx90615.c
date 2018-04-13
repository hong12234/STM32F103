#include "mlx90615.h"
#include "delay.h"
#include "gpio.h"
#include "sys.h"

#define ACK	 0
#define	NACK 1



//MLX90615
#define SA			    0x00	// �ӻ���ַ
#define DEFAULT_SA		0x5B	// ���ʱ��ַĬ��Ϊ0x5B
#define RAM_ACCESS	    0x20	// RAM ��������
#define EEPROM_ACCESS	0x10	// EEPROM ��������
#define RAM_TA		    0x06	// Ta ��RAM�еĵ�ַ��TaΪ��װ(����) �¶�
#define RAM_TO  		0x07	// To ��RAM�еĵ�ַ��ToΪ�����¶�


#define SMBUS_PORT	    GPIOA
#define SMBUS_SCK		11
#define SMBUS_SDA		12

#define SMBUS_SCK_H()	    SMBUS_PORT->ODR |= (1<<SMBUS_SCK)
#define SMBUS_SCK_L()	    SMBUS_PORT->ODR &= ~(1<<SMBUS_SCK)
#define SMBUS_SDA_H()	    SMBUS_PORT->ODR |= (1<<SMBUS_SDA)
#define SMBUS_SDA_L()	    SMBUS_PORT->ODR &= ~(1<<SMBUS_SDA)

#define SMBUS_SDA_PIN()	   ((SMBUS_PORT->IDR) & (1<<SMBUS_SDA)) //��ȡ���ŵ�ƽ


/*******************************************************************************
** ��������: SMBus_StartBit
** ��������: ��SMBus�����ϲ�����ʼ״̬
** ��    ��: ��
** �� �� ֵ: ��       
** ����  ��: 
*******************************************************************************/
void SMBus_StartBit(void)
{
    SMBUS_SDA_H();		//����SDA��Ϊ�ߵ�ƽ
    delay_us(5);	    //��ʱ5us
    SMBUS_SCK_H();		//����SCL��Ϊ�ߵ�ƽ
    delay_us(5);	    //����ֹ����ʼ״̬֮��������߿���ʱ��(Tbuf=4.7us��Сֵ)
    SMBUS_SDA_L();		//����SDA��Ϊ�͵�ƽ
    delay_us(5);	    //(�ظ�)��ʼ״̬��ı���ʱ�䣬�ڸ�ʱ��󣬲�����һ��ʱ���ź�(Thd:sta=4us��Сֵ)
    SMBUS_SCK_L();	    //����SCL��Ϊ�͵�ƽ
    delay_us(5);	    
}

/*******************************************************************************
** ��������: SMBus_StopBit
** ��������: ��SMBus�����ϲ�����ֹ״̬
** ��    ��: ��
** �� �� ֵ: ��       
** ����  ��: 
*******************************************************************************/
void SMBus_StopBit(void)
{
    SMBUS_SCK_L();		//����SCL��Ϊ�͵�ƽ
    delay_us(5);	    
    SMBUS_SDA_L();		//����SDA��Ϊ�͵�ƽ
    delay_us(5);	    
    SMBUS_SCK_H();		//����SCL��Ϊ�ߵ�ƽ
    delay_us(5);	    //��ֹ״̬����ʱ��(Tsu:sto=4.0us��Сֵ)
    SMBUS_SDA_H();		//����SDA��Ϊ�ߵ�ƽ 
}

/*******************************************************************************
** ��������: SMBus_SendByte
** ��������: ��SMBus�����Ϸ���һ���ֽ�
** ��    ��: ��Ҫ�������Ϸ��͵��ֽ�
** �� �� ֵ: SMBus�����Ͻ��յ���λ����      
** ����  ��: 
*******************************************************************************/
u8 SMBus_SendByte(u8 Tx_buffer)
{
    u8	Bit_counter;
    u8	Ack_bit;
    u8	bit_out;

    for(Bit_counter=8; Bit_counter; Bit_counter--)
    {
        if (Tx_buffer&0x80)
        {
            bit_out=1;   //���TX_buffer�ĵ�ǰλ��1,����bit_outΪ1
        }
        else
        {
            bit_out=0;  //��������bit_outΪ0
        }
        SMBus_SendBit(bit_out);	//����SMBus�����ϵĵ�ǰλ   
        Tx_buffer<<=1;			//�˶���һλ	
    }

    Ack_bit=SMBus_ReceiveBit();	//�õ�SMBus�����Ͻ��յ���λ����
    return	Ack_bit;            //����SMBus�����Ͻ��յ���λ����
}

/*******************************************************************************
** ��������: SMBus_SendBit
** ��������: ��SMBus�����Ϸ���һλ����
** ��    ��: ��Ҫ�������Ϸ��͵�λ
** �� �� ֵ: ��      
** ����  ��: 
*******************************************************************************/
void SMBus_SendBit(u8 bit_out)
{
    if(bit_out==0)                  //�˶��ֽڵ�λ
    {                               //���bit_out=1������SDA��Ϊ�ߵ�ƽ
        SMBUS_SDA_L();
    }
    else                            
    {
        SMBUS_SDA_H();              //���bit_out=0������SDA��Ϊ�͵�ƽ
    }
    delay_us(2);					//Tsu:dat=250ns ��Сֵ
    SMBUS_SCK_H();					//����SCL��Ϊ�ߵ�ƽ
    delay_us(6);					//ʱ������ߵ�ƽ����(10.6us)
    SMBUS_SCK_L();					//����SCL��Ϊ�͵�ƽ
    delay_us(3);					//ʱ������͵�ƽ����  

    return;
}

/*******************************************************************************
** ��������: SMBus_ReceiveBit
** ��������: ��SMBus�����Ͻ���һλ����
** ��    ��: ��
** �� �� ֵ: ���յ�SMBus�����Ͻ���һλ����      
** ����  ��: 
*******************************************************************************/
u8 SMBus_ReceiveBit(void)
{
    u8 Ack_bit;

    SMBUS_SDA_H();          //���ſ��ⲿ������������������
	delay_us(2);			//ʱ������͵�ƽ���� 
    SMBUS_SCK_H();		    //����SCL��Ϊ�ߵ�ƽ
    delay_us(5);			//ʱ������ߵ�ƽ����(10.6us)
    
    if (SMBUS_SDA_PIN())     //�������϶�ȡһλ����Ack_bit
    {
        Ack_bit=1;
    }
    else
    {
        Ack_bit=0;
    }
    SMBUS_SCK_L();			//����SCL��Ϊ�͵�ƽ
    delay_us(3);			//ʱ������͵�ƽ���� 

    return	Ack_bit;        //����Ack_bit
}

/*******************************************************************************
** ��������: SMBus_ReceiveByte
** ��������: ��SMBus�����Ͻ���һ���ֽ�
** ��    ��: ������������ ACK(0)/NACK(1)
** �� �� ֵ: ���յ�SMBus���߽��յ��ֽ�  
** ����  ��: 
*******************************************************************************/
u8 SMBus_ReceiveByte(u8 ack_nack)
{
    u8 	RX_buffer;
    u8	Bit_Counter;

    for(Bit_Counter=8; Bit_Counter; Bit_Counter--)
    {
        if(SMBus_ReceiveBit())			 //��SDA�߶�ȡһλ
        {
            RX_buffer <<= 1;			//���λΪ"1"����"1"��RX_buffer 
            RX_buffer |=0x01;
        }
        else
        {
            RX_buffer <<= 1;			//���λΪ"0"����"0"��RX_buffer
            RX_buffer &=0xfe;
        }
    }
    SMBus_SendBit(ack_nack);			 //����ȷ��λ
    return RX_buffer;
}

/*******************************************************************************
** ��������: SMBus_Init
** ��������: SMBus��ʼ��
** ��    ��: ��
** �� �� ֵ: ��
** ����  ��: 
*******************************************************************************/
void SMBus_Init(void)
{
  	GPIO_Clock_Set(IOPAEN); //ʹ��PA�˿�
	GPIO_Init(GPIOA,11,IO_MODE_OUT,IO_SPEED_25M,IO_OTYPER_OD,IO_PULL,IO_H);	//PA11�����������
	GPIO_Init(GPIOA,12,IO_MODE_OUT,IO_SPEED_25M,IO_OTYPER_OD,IO_PULL,IO_H);	//PA12�����������

	
	// GPIO_InitTypeDef    GPIO_InitStructure;
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SMBUS_PORT, ENABLE);//ʹ��ʱ��

    /*����SMBUS_SCK��SMBUS_SDAΪ���缫��©���*/
//    GPIO_InitStructure.GPIO_Pin = SMBUS_SCK | SMBUS_SDA;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(SMBUS_PORT, &GPIO_InitStructure);

    SMBUS_SCK_H();
    SMBUS_SDA_H();
}

/*******************************************************************************
** ��������: SMBus_ReadMemory
** ��������: �����ܿص�ַ������ʱ��MLX90615��ȡ����
** ��    ��: ��
** �� �� ֵ: ��ȡ��������
** ����  ��: 
*******************************************************************************/
u16 SMBus_ReadMemory(u8 slaveAddress, u8 command)
{
    u16 data;			// ��MLX90615���صļĴ�����ֵ
    u8 Pec;				
    u8 DataL=0;			
    u8 DataH=0;			//��MLX90615��ȡ�����ݰ�
    u8 arr[6];			//�洢�ѷ����ֽڵĻ�����
    u8 PecReg;			//�洢��������PEC�ֽ�
    u8 ErrorCounter;	//������MLX90614���ݹ�ͨ

    ErrorCounter=0x00;	// Initialising of ErrorCounter
//	slaveAddress <<= 1;	//2-7λ��ʾ�ӻ���ַ�����ض���¶�̽ͷʱʹ��
	
    do
    {
repeat:
        SMBus_StopBit();			    //����ӻ�����ֹͣ�ź�
        --ErrorCounter;				    //Pre-decrement ErrorCounter
        if(!ErrorCounter) 			    //ErrorCounter=0?
        {
            break;					    //����ѭ��
        }

        SMBus_StartBit();				//������ʼλ
        if(SMBus_SendByte(slaveAddress))//���ʹӻ���ַ ���λWr=0��ʾ������д����
        {
            goto	repeat;			   
        }
        if(SMBus_SendByte(command))	    //д����
        {
            goto	repeat;		    	
        }

        SMBus_StartBit();					//�����ظ���ʼλ	
        if(SMBus_SendByte(slaveAddress+1))	//���ʹӻ���ַ�����λRd=1��ʾ������������
        {
            goto	repeat;             	
        }

        DataL = SMBus_ReceiveByte(ACK);	//��ȡ���ֽ����ݣ��������뷢�� ACK
        DataH = SMBus_ReceiveByte(ACK); //��ȡ���ֽ�����,�������뷢�� ACK
        Pec = SMBus_ReceiveByte(NACK);	//��ȡPEC�ֽ�, �������뷢�� NACK
        SMBus_StopBit();				//������ֹλ

        arr[5] = slaveAddress;		
        arr[4] = command;			
        arr[3] = slaveAddress+1;	
        arr[2] = DataL;				
        arr[1] = DataH;				
        arr[0] = 0;					
        PecReg=PEC_Calculation(arr);//���ü��� CRC �ĺ���
    }
    while(PecReg != Pec);		    //�ж��յ�����CRC�Ƿ����

	data = (DataH<<8) | DataL;	   
    return data;    
}

/*******************************************************************************
** ��������: PEC_Calculation
** ��������: ���ݽ��յ��ֽڼ���PEC��
** ��    ��: pecֵ
** �� �� ֵ: pec[0] - ���ֽڰ�����������crc��ֵ
** ����  ��: 
*******************************************************************************/
u8 PEC_Calculation(u8 pec[])
{
    u8 	crc[6];
    u8	BitPosition=47;
    u8	shift;
    u8	i;
    u8	j;
    u8	temp;

    do
    {
        //���� CRC��ֵ 0x000000000107
        crc[5]=0;
        crc[4]=0;
        crc[3]=0;
        crc[2]=0;
        crc[1]=0x01;
        crc[0]=0x07;

        BitPosition=47;//����Bitposition�����ֵΪ47

        shift=0;//����ת��λΪ0

        //�ڴ��͵��ֽ����ҳ���һ��"1"
        i=5;//������߱�־λ (�����ֽڱ�־)
        j=0;//�ֽ�λ��־�������λ��ʼ
        while((pec[i]&(0x80>>j))==0 && i>0)
        {
            BitPosition--;
            if(j<7)
            {
                j++;
            }
            else
            {
                j=0x00;
                i--;
            }
        }//while�����������ҳ�Bitposition��Ϊ"1"�����λλ��

        shift=BitPosition-8; //�õ�CRC��ֵ��Ҫ����/���Ƶ���ֵ"shift"����CRC��������"shift"λ

        while(shift)
        {
            for(i=5; i<0xFF; i--)
            {
                if((crc[i-1]&0x80) && (i>0))//�˶��ֽڵ����λ����һλ�Ƿ�Ϊ"1"
                {
                    temp=1;                //�� - ��ǰ�ֽ� + 1
                }                          //�� - ��ǰ�ֽ� + 0
                else                       //ʵ���ֽ�֮���ƶ�"1"
                {
                    temp=0;
                }
                crc[i]<<=1;
                crc[i]+=temp;
            }
            shift--;
        }

        for(i=0; i<=5; i++) //pec��crc֮�����������
        {
            pec[i] ^=crc[i];
        }/*End of for*/
    }
    while(BitPosition>8); 
    return pec[0];        //���ؼ������õ�crc��ֵ
}

/*******************************************************************************
** ��������: SMBus_ReadTemp
** ��������: SMBus��ȡ�¶�
** ��    ��: ��
** �� �� ֵ: ��ȡ�����¶�
** ����  ��: 
*******************************************************************************/
float SMBus_ReadTemp(void)
{   
	float temp;
	temp = ((float)SMBus_ReadMemory(SA, RAM_ACCESS|RAM_TO)*(float)0.02-(float)273.15)/(float)0.89;//��ȡ�����¶�
	return temp;
}


/*********************************END OF FILE*********************************/
