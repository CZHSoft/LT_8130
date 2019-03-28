#define _XTM_ADD  //20111108 xu

#define LCD_DE_MODE   0

#define CPU_MAIN_FREQ   49152000     //50000000

//#define SCREEN_WIDTH		800            //屏幕宽度
//#define SCREEN_HEIGHT		480            //屏幕高度

//#define SCREEN_WIDTH		480            //屏幕宽度
//#define SCREEN_HEIGHT		272            //屏幕高度

#define SCREEN_WIDTH		320            //屏幕宽度
#define SCREEN_HEIGHT		240            //屏幕高度

//不管当前分辨率　DISP_WIDTH、DISP_HEIGHT　必须为　800 480
#define DISP_WIDTH  800
#define DISP_HEIGHT  480

#define _GPIOA_MUX   0xFF //0x10 //0x90              //20111108 xu  5(0x10) 为输入　　其它为输出
                                        //20110705 xu  4 为输入    触控屏
#define _GPIOB_MUX   0xFF //0x20              //20111108 xu  5 为输入　　其它为输出
#define _GPIOC_MUX   0xFF //0x20              //20111108 xu  5 为输入　　其它为输出

#define RX_CONTROL_NUM_128

//128个  256K 接收缓存
#ifdef RX_CONTROL_NUM_128
 #define MAC_RX_MAX   128  //网卡接收缓冲区
 #define MAC_RX_MAX1  127
 //#define RX_CONTROL_EN_RCV  0x61      //  接收使能              0110 0001
 //#define RX_CONTROL_EN_INT  0x63      //128个  接收中断使能      0110 0011
 #define RX_CONTROL_EN_RCV  0xE1      //  接收使能              1110 0001
 #define RX_CONTROL_EN_INT  0xE3      //128个  接收中断使能      1110 0011  //D7：0：所有接收到的包都进入接收缓冲区；1：MAC地址匹配、广播、多播包才进入接收缓冲区
#endif
