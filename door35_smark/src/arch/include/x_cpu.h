#define _XTM_ADD  //20111108 xu

#define LCD_DE_MODE   0

#define CPU_MAIN_FREQ   49152000     //50000000

//#define SCREEN_WIDTH		800            //��Ļ���
//#define SCREEN_HEIGHT		480            //��Ļ�߶�

//#define SCREEN_WIDTH		480            //��Ļ���
//#define SCREEN_HEIGHT		272            //��Ļ�߶�

#define SCREEN_WIDTH		320            //��Ļ���
#define SCREEN_HEIGHT		240            //��Ļ�߶�

//���ܵ�ǰ�ֱ��ʡ�DISP_WIDTH��DISP_HEIGHT������Ϊ��800 480
#define DISP_WIDTH  800
#define DISP_HEIGHT  480

#define _GPIOA_MUX   0xFF //0x10 //0x90              //20111108 xu  5(0x10) Ϊ���롡������Ϊ���
                                        //20110705 xu  4 Ϊ����    ������
#define _GPIOB_MUX   0xFF //0x20              //20111108 xu  5 Ϊ���롡������Ϊ���
#define _GPIOC_MUX   0xFF //0x20              //20111108 xu  5 Ϊ���롡������Ϊ���

#define RX_CONTROL_NUM_128

//128��  256K ���ջ���
#ifdef RX_CONTROL_NUM_128
 #define MAC_RX_MAX   128  //�������ջ�����
 #define MAC_RX_MAX1  127
 //#define RX_CONTROL_EN_RCV  0x61      //  ����ʹ��              0110 0001
 //#define RX_CONTROL_EN_INT  0x63      //128��  �����ж�ʹ��      0110 0011
 #define RX_CONTROL_EN_RCV  0xE1      //  ����ʹ��              1110 0001
 #define RX_CONTROL_EN_INT  0xE3      //128��  �����ж�ʹ��      1110 0011  //D7��0�����н��յ��İ���������ջ�������1��MAC��ַƥ�䡢�㲥���ಥ���Ž�����ջ�����
#endif
