#include "GUI.h"
#include "includes.h"
    
/*********************************************************************
*
* Global data
*/
static OS_EVENT 	*DispSem;  	//��ʾ���ź���
static OS_EVENT	    *EventMbox;  

static OS_EVENT	*KeySem;  	//�����ź���
static int		KeyPressed;
static char		KeyIsInited;


//ʱ���ȡ����
int GUI_X_GetTime(void)
{
  return ((int) OSTimeGet());
}

//emWin��ʱ����
//���յ���ʱʱ����period*1000
void GUI_X_Delay(int period)
{
  INT32U ticks;
  ticks = (period*1000)/OS_TICKS_PER_SEC;
  OSTimeDly(ticks);
}

static void CheckInit(void)
{
	if(KeyIsInited == OS_FALSE)
	{
		KeyIsInited = OS_TRUE;
		GUI_X_Init();
	}
}

void GUI_X_Init(void)
{
	KeySem = OSSemCreate(0);	//������ʼֵΪ0���ź���
}


int GUI_X_GetKey(void)
{
	int r;
	r = KeyPressed;
	CheckInit();
	KeyPressed = 0;
	return (r);
}

int GUI_X_WaitKey(void)
{
	int r;
	INT8U err;
	CheckInit();
	if(KeyPressed == 0)
	{
		OSSemPend(KeySem,0,&err);	//�����ź���
	}
	r = KeyPressed;
	KeyPressed = 0;
	return (r);
}

void GUI_X_StoreKey(int k)
{
	KeyPressed = k;
	OSSemPost(KeySem);
}
void GUI_X_ExecIdle(void) {}


//OS��ʼ��
void GUI_X_InitOS(void)
{
	DispSem = OSSemCreate(1);			//������ʼֵΪ1���ź���
	EventMbox = OSMboxCreate((void*)0);	//������Ϣ����
	
}

//�������
void GUI_X_Unlock(void)
{ 
  OSSemPost(DispSem);		//�����ź���
}

//��������
void GUI_X_Lock(void)
{
  INT8U err;
  OSSemPend(DispSem,0,&err);//�����ź���
}

/* Get Task handle */
U32 GUI_X_GetTaskId(void) 
{ 
//  return ((U32) osThreadGetId());
	return 0;
}


//OS�ȴ��¼�
void GUI_X_WaitEvent (void) 
{
	INT8U err;
	(void)OSMboxPend(EventMbox,0,&err);	//������Ϣ���� 
}

//OS�����¼�
void GUI_X_SignalEvent (void) 
{
  (void)OSMboxPost(EventMbox,(void*)1);	//������Ϣ����
}



void GUI_X_Log (const char *s) { }
void GUI_X_Warn (const char *s) { }
void GUI_X_ErrorOut(const char *s) { }

/*************************** End of file ****************************/

