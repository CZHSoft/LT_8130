#include "includes.h"

#include "ComDrv.h"

#define m_init_h

#define CommonH
#include "common.h"

/*
****************************************
201310
����ĳ�ֶεĳ�ʼ��
****************************************
*/

void DelayMs(unsigned int number);
void main_init(void);
void InitVectorTable(void);
void interrupt IdleInt();
void interrupt Int0();
void interrupt Int1();
void interrupt Int2();
void interrupt Int3();
void interrupt Int4();
void interrupt Int5();
void interrupt Int6();
void interrupt Int7();
void interrupt Int9();
void interrupt Int11();
void interrupt TimeInt();


void *xtm_memcopy(void *dest, void *src, INT16U n);
INT16U dtime[8];
INT32U timer_counter;
INT32U SWTIMER_Tick(void);

INT8U Vflag;


void main_init(void)
{
    INT8U i;
    INT16U val;
    Local.RemoteDebugInfo = 0;      //����Զ�̵�����Ϣ
    
    Local.AlarmStatus=0;         //20131010 �������

	//AdPic.PicCount=0;            //201310 �����
	
    InitComm((INT16U)UART0_TX_STATUS, (INT32U)115200, 3, 0xc7, 1);
    outportb(UART0_TX_DATA, 0x55);
    //InitComm(UART1_TX_STATUS, 9600, 3, 0xc7, 1);
    xtm_printf("main_init 1, inportb(CONTROL_REG) = 0x%X\n", inportb(CONTROL_REG));
    InitVectorTable();

    #ifdef _XTM_ADD
     Init_LocalVar(); //��ʼ�� Local �ṹ
     Local.Test_OV7725FrameRate = 1;    //OV7725 ���֡��
    #endif
    InitPcm();    //20120104 xu
    //�ڲ� watchdog, �ݲ���    
    SetPTC1(1);
    //freq  1  --  ��������16 --  16��Ƶ
    SetPTC2(1);
    Local.ClearVideoWindowFlag = 1; //����Ƶ���ڱ�־

    for (i=0;i<8;i++)
      dtime[i]=0;

    disp_init();
    #ifdef _XTM_ADD
     mac_init();
     phy_init();
     
     AddMultiGroup(Local.NsMultiAddr);
     xtm_printf("AddMultiGroup\n");
    #endif

  //  InitVideo();

    if(LocalCfg.VideoSdramRefresh == 0)
      outportb(POWER_CONTROL1, 0x04);//��Դ���ƼĴ���1    4����
    else
      outportb(POWER_CONTROL1, 0x84);//��Դ���ƼĴ���1    8����
    if(LocalCfg.VideoOutput == 0)      //��Ƶ���  0  ����  1 �Լ�
     {
      Write7725(0x66, 0x00);
      Write7725(0x0c, 0xc0);
     }
    else
     {
      Write7725(0x66, 0x20);
      Write7725(0x0c, 0xc1);
     }
    InitDefence();   //��ʼ������
    InitGpio();  //��ʼ��GPIO
    
    Init_InfoVar(); //��ʼ�� Info �ṹ
    Init_RecordsVar(); //��ʼ�� Records �ṹ
    
    InitVideo();

    StartNoiseDect();

    set_8388_regisger(); //20130220       
}
void interrupt SPIInt(void)
{
//	outportb(UART0_TX_DATA, inportb(SPDR));				// �����ַ�
}

void InitVectorTable(void)
{
	int i;


 for(i=0;i<256;i++)setvect(i, IdleInt);

 setvect(8, MACInt);

 timer_counter = 1800; 
 setvect(12, TimeInt);
 setvect(13, Com1Int);
 setvect(14, Com2Int);
// setvect(15, SPIInt);

	
 outportb(INT_MASK, 0x0);//���ĸ��ж�ʹ�ܣ����ĸ��жϹر� 1������ 0:ʹ��
 outportb(INT_EDGE, 0x00);//���������ش���

 outportb(INT_VECTOR, 0x08);//�жϺ�Ϊ8-15
 outportb(UART0_RX_CTRL, 0xc0);//����0�����ж�ʹ��
 outportb(UART1_RX_CTRL, 0xc0);//����1�����ж�ʹ��
// outportb(SPCR, 0xd0);//SPI�ж�ʹ��
 enable();

}

#pragma option -w-
void interrupt IdleInt()
{
	outportb(UART0_TX_DATA, 16);				// �����ַ�
}

void interrupt TimeInt()
{
	INT8U i;
	outportb(PTC2_CTRL0,0x41);
        
        timer_counter ++;
	for (i=0;i<8;i++){
		if (dtime[i])
			dtime[i]--;
	}

	outportb(PTC2_CTRL0,0x21);
}

INT32U SWTIMER_Tick(void)
{
  return timer_counter;
}

void DelayMs(unsigned int number)
{
    unsigned int temp;
    for(;number!=0;number--){
	       for(temp=430;temp!=0;temp--){
	       }
    }
}
#pragma option -w.
void *xtm_memcopy(void *dest, void *src, INT16U n)
{
 #if 1
  asm cli
  memcpy(dest, src, n);
  asm sti
 #else
  if(n)
   {
    *(dest++) = *(src++);
    n --;
   }
 #endif
}