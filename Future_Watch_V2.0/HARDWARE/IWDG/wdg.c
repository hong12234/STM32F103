#include "wdg.h"
 

//��ʼ���������Ź�
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//rlr:�Զ���װ��ֵ,0~0XFFF.
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms).
void IWDG_Init(u8 prer,u16 rlr) 
{
	IWDG->KR=0X5555;//ʹ�ܶ�IWDG->PR��IWDG->RLR��д		 										  
  	IWDG->PR=prer;  //���÷�Ƶϵ��   
  	IWDG->RLR=rlr;  //�Ӽ��ؼĴ��� IWDG->RLR  
	IWDG->KR=0XAAAA;//reload											   
  	IWDG->KR=0XCCCC;//ʹ�ܿ��Ź�	
}
//ι�������Ź�
void IWDG_Feed(void)
{
	IWDG->KR=0XAAAA;//reload											   
}








