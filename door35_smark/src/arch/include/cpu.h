/*****************************************************\
*                                                     *
*                                                     *
\*****************************************************/

/*
//========
//管脚说明
//========
    本芯片为128管脚的PQFP封装。VSS为IO地,VDD为IO电源,VSSI为内核地,VDDI为内核电源;电源为3.3V.
管脚名	   管脚号
GPIOA7  	1 
GPIOA6  	2 
GPIOA5  	3 
DCLK 	 	4 
R2 	 	5 
R3 	 	6 
R4 	 	7 
R5 	 	8 
VSS1 	 	9 
R6 	       10 
R7 	       11 
G2 	       12 
G3 	       13 
G4 	       14 
G5 	       15 
G6 	       16 
VDD2 	       17 
G7 	       18 
B2 	       19 
B3 	       20 
B4 	       21 
B5 	       22 
B6 	       23 
VDDI1 	       24 
B7 	       25 
VSSI0 	       26 
HSYNC 	       27 
VSYNC 	       28 
CLK_25M        29 
RX_DV 	       30 
CRS 	       31 
RX_CLK         32 
RX_DATA0       33 
RX_DATA1       34 
RX_DATA2       35 
RX_DATA3       36 
TX_CLK 	       37 
VSS6 	       38 
TX_EN 	       39 
TX_DATA0       40 
TX_DATA1       41 
TX_DATA2       42 
TX_DATA3       43 
K2048 	       44 
K8 	       45 
PCMI 	       46 
PCMO 	       47 
VDD5 	       48 
SPI_SCK	       49 
SPI_MOSI       50 
SPI_SS 	       51 
SPI_MISO       52 
VSSI3 	       53 
RXD1 	       54 
TXD1 	       55 
RXD2 	       56 
TXD2 	       57 
IR3            58 
PWM0 	       59 
VSS0 	       60 
GPIOA1         61 
GPIOA2         62 
GPIOA3         63 
GPIOA4         64 
GPIOA0         65 
VPO0 	       66 
VPO1 	       67 
VPO2 	       68 
VPO3 	       69 
VPO4 	       70 
VPO5 	       71 
VPO6 	       72 
VSSI2 	       73 
VPO7 	       74 
PCLK 	       75 
VSS2 	       76 
VDDI2 	       77 
RSTN 	       78 
VDD1 	       79 
XTLO  	       80 
XTLI  	       81 
VSS4 	       82 
CLKO 	       83 
DQ0 	       84 
DQ15 	       85 
DQ1 	       86 
DQ14 	       87 
DQ2 	       88 
DQ13 	       89 
VSS5 	       90 
VDD0 	       91 
DQ3 	       92 
DQ12 	       93 
DQ4 	       94 
DQ11 	       95 
DQ5 	       96 
DQ10 	       97 
DQ6 	       98 
DQ9 	       99 
VSS3 	      100 
DQ7 	      101 
DQ8 	      102 
DQML 	      103 
DQMH 	      104 
WEN 	      105 
VDD4 	      106 
VDDI0 	      107 
CASN 	      108 
VSSI1 	      109 
RASN 	      110 
CKE 	      111 
CSN 	      112 
BANK0 	      113 
BANK1 	      114 
BA11 	      115 
BA9 	      116 
BA10 	      117 
BA8 	      118 
BA0 	      119 
VDD3 	      120 
BA7 	      121 
BA1 	      122 
BA6 	      123 
VSS7 	      124 
BA2 	      125 
BA5 	      126 
BA3 	      127 
BA4 	      128 

    本芯片有107个信号管脚，其中有单一功能管脚，也有多功能管脚，多功能管脚可通过内部寄存器设置。
    按功能分组如下。
    
1、时钟复位，3个管脚
	XTLI(输入)		振荡器输入
	XTLO(输出)		振荡器输出
	RSTN（输入、低有效）	复位输入

2、SPI FLASH接口（程序片接口），4个管脚
    本芯片外接一片最少256K字节的SPI FLASH作为CPU程序、显示图标等数据的存储区。
    当复位脚RSTN为高时，芯片从外接的一片SPI FLASH下载256KB程序至8088 CPU 1MB内存空间的高256KB，然后从FFFF0H处启动。
	SPI_SCK(输出)		SPI串行时钟	
	SPI_MOSI(输出)		SPI主输出次输入
	SPI_SS(输出、低有效) 	SPI片选
	SPI_MISO(输入)		SPI主输入次输出

3、SDRAM接口，38个管脚
    本芯片外接一片8M字节的SDRAM作为CPU数据程序、视频解码、视频输入、显示等数据的存储区。
	CSN(输出、低有效)	SDRAM片选信号
	RASN(输出、低有效)	SDRAM行选通信号
	CASN(输出、低有效)	SDRAM列选通信号
	WEN(输出、低有效)	SDRAM写信号
	BA11-0(输出)		SDRAM地址
	BANK1-0(输出)	SDRAM bank地址
	DQ15-0(输入/输出)	SDRAM数据
	DQML(输出、高有效)	SDRAM数据屏蔽低信号
	DQMH(输出、高有效)	SDRAM数据屏蔽高信号
	CLKO(输出)		SDRAM时钟输出（与XTLI同频率）
	CKE(输出、高有效)	SDRAM时钟使能信号

4、视频输出接口，21个管脚
    本芯片可外接数字液晶显示屏模组。
	HSYNC（输出）		视频水平同步信号
	VSYNC/DE（输出）	视频垂直同步、显示使能信号
	R7-2（输出）		视频R数据
	G7-2（输出）		视频G数据
	B7-2（输出）		视频B数据
	DCLK（输出）		视频点时钟

5、MAC接口，14个管脚
    本芯片外接带有MII接口的以太网PHY，如DM9161等。
	RX_DATA3-0（输入）	MAC接收数据
	RX_DV（输入）		MAC接收驱动
	CRS（输入）		MAC接收载波检测
	RX_CLK（输入）		MAC接收时钟
	TX_CLK（输入）		MAC发送时钟
	TX_DATA3-0（输出）	MAC发送数据
	TX_EN（输出）		MAC发送使能
	注意，MII信号RX_ER、TX_ER、COLL未连接，PHY上TX_ER接地，RX_ER、COLL悬空。

6、视频输入接口，9个管脚
    本芯片可外接带ITU656（BT656）接口的PAL/NTSC解码器，将视频输入的图像显示在显示屏的视频窗口上。
	PCLK（输入）			视频输入时钟
	VPO7-0（输入，GPIOC7-0）	视频输入数据

7、音频接口，5个管脚
    本芯片可连接如MC14581这样的音频PCM CODEC芯片。
    芯片内提供有输入和输入各32级16位FIFO的IIS控制器，当IIS控制器未使能时，音频接口脚为PCM信号（IR3除外），
当IIS控制器使能时，音频接口脚转换为IIS信号。
	CLK_25M（输出）		(改为PCM的MCLK)
	BCLK(输出)		PCM位时钟
	FS(输出)			PCM帧同步
	PCMO(输出)		PCM数据输出（接DR）
	PCMI(输入)		PCM数据输入（接DT）
	IR3(输入/输出，GPIOB2)  	外部中断输入（中断3）

8、UART接口，4个管脚
    本芯片提供两个8级FIFO的UART。
	RXD1（输入）		串口1接收数据
	TXD1（输出）		串口1发送数据
	RXD2（输入，GPIOB6）		串口2接收数据
	TXD2（输出，GPIOB7）		串口2发送数据

9、计数器，1个管脚
    本芯片内置有三个计数器，计数器0作为PWM发生器，输出PWM0信号，计数器1用于内部看门狗使用，计数器2作为定时器使用。
	PWM0（输出，GPIOB3）		脉冲宽度调制输出

10、通用输入输出（GPIO），8个管脚
    本芯片设置有5组GPIO，分别为GPIOA、GPIOB、GPIOC；其中除了GPIOA是专用管脚以外，其它GPIO与以上功能
管脚通过寄存器选择共用。
	GPIOA7-0（输入输出）		通用输入输出A7-0

	GPIOB7（与TXD2共用）	通用输入输出B7
	GPIOB6（与RXD2共用）	通用输入输出B6
	GPIOB5（无）		通用输入输出B5
	GPIOB4（无）		通用输入输出B4
	GPIOB3（与PWM0共用）	通用输入输出B3
	GPIOB2（与IR3共用)   	通用输入输出B2
	GPIOB1（无)		通用输入输出B1
	GPIOB0（无)		通用输入输出B0

	GPIOC7-0（与VPO共用）		通用输入输出C7-0

11、外接CPU接口，不占用专用管脚
    本芯片内含16位CPU，该CPU可以在启动时关闭，外部CPU通过CPU总线控制本芯片。具体做法如下，DQ0有弱下拉
电阻，在没有外接4.7K上拉电阻的情况下，在复位期间该管脚检测为0，内部CPU启动；在有外接4.7K上拉电阻的情况下，在复
位期间该管脚检测为1，内部CPU关闭，外部CPU通过如下管脚写入和读出内部寄存器。
    IR3--ALE地址锁存脉冲（输入，高有效）
    PWM0--WRN写脉冲（输入，低有效）
    RXD2--RDN读脉冲（输入，低有效）
    TXD2--READY信号（输出，高有效）
    GPIOA--DATA数据输入输出（输入输出）

    第一个ALE（IR3）脉冲通过DATA锁存16位地址中的高8位地址，第二个ALE（IR3）脉冲通过DATA锁存16位地址中的低8位地址，
然后如WRN（PWM0）脉冲，则通过DATA写入相应地址的寄存器，如RDN（RXD2）脉冲，则DATA读回相应地址的寄存器的值。其中写
入或读回SDRAM的数据则需要判断READY（TXD2）是否为1，再去掉WRN或RDN脉冲。

*/
#include "x_cpu.h"

#define COLOR_BLACK         0x0000
#define COLOR_WHITE         0x7FFF
#define COLOR_RED           0x7C00 //0xf800
#define COLOR_ORANGE        0x7DE7 //0x7E08
#define COLOR_YELLOW        0x7FE0 //0xFFE0
#define COLOR_GREEN         0x03E0 //0x07E0
#define COLOR_BLUE          0x001F
#define COLOR_CYAN          0x07FF
#define COLOR_PURPLE        0x401F //0x05

//#define SCREEN_WIDTH		800            //屏幕宽度
//#define SCREEN_HEIGHT		480            //屏幕高度

//字节写寄存器组
/*
    该组寄存器控制CPU写内存的一种方式，将初始地址写入字节写地址寄存器后，该方式每写字节写数据寄存器一次，
将该寄存器一个字节的内容写入内存，同时自动将地址加一以准备接收下一个数据。
*/
#define BYTE_WR_LOW_ADD		0xfee2           //字节写低地址寄存器（0-7）
#define BYTE_WR_HIGH_ADD	0xfee3           //字节写高地址寄存器（8-15）
#define BYTE_WR_MOST_ADD	0xfee4           //字节写最高地址寄存器（16-23）
#define BYTE_WR_DATA		0xfee5           //字节写数据寄存器

//字节读寄存器组
/*
    该组寄存器控制CPU读内存的一种方式，将初始地址写入字节读地址寄存器后，该方式每读字节读数据寄存器一次，
当前字节读地址寄存器指向的地址数据读回，同时自动将地址加一以准备接收下一个数据。
*/
#define BYTE_RD_LOW_ADD		0xfee6            //字节读低地址寄存器（0-7）                            
#define BYTE_RD_HIGH_ADD	0xfee7            //字节读高地址寄存器（8-15）                           
#define BYTE_RD_MOST_ADD	0xfee8            //字节读最高地址寄存器（16-23?
#define BYTE_RD_DATA		0xfee9            //字节读数据寄存器    

//字符写寄存器组
/*
    该组寄存器控制CPU写内存的一种方式，将初始地址（以16字节为界，即低4位地址为0）写入字符写地址寄存器后，该方式每写字符写数据寄存器一次，
将该寄存器一个字节的内容按位以如下方式写入内存的8个字，同时自动将地址加16以准备接收下一个数据：
    0：内存字写入字符背景寄存器
    1：内存字写入字符前景寄存器
*/
#define FONT_WR_LOW_ADD		0xfed0           //字符写低地址寄存器（0-7）
#define FONT_WR_HIGH_ADD	0xfed1           //字符写高地址寄存器（8—15）
#define FONT_WR_MOST_ADD	0xfed2           //字符写最高地址寄存器（16—23） 
#define FONT_WR_DATA		0xfed3           //字符写数据寄存器
#define FGND_LOW		0xfed4           //字符前景低字节寄存器
#define FGND_HIGH		0xfed5           //字符前景高字节寄存器
#define BGND_LOW		0xfed6           //字符背景低字节寄存器   
#define BGND_HIGH		0xfed7           //字符背景高字节寄存器

//MAC发送（通用包）--本部分是除了视频包和PCM包以外的通用包
/*
    MAC发送的数据包放在SDRAM中发送包起始地址寄存器（初始值为0x1a0040）指向的地址开始的2K字节接收缓冲区，
程序将要发送的数据写入该缓冲区后，将包长度写入发送包长度寄存器，然后将1写入发送开始寄存器D0。检查发送状态
寄存器D0位，等待发送结束。

//初始化
	setvect(9, MACTXInt);
//发送包
	while(inportb(TX_STATUS)&0x01);

	outportb(TX_SA2_H, 0x1a);
	outportb(TX_SA2_M, 0x00);
	outportb(TX_SA2_L, 0x40);

	outport(TXPL_LOW, 182);
	outport(TXPL2_LOW, add_data_length);
	outportb(TX_START, 0xd);//发送并使能MAC发送中断

//发送中断服务程序

void interrupt MACTXInt()
{
	outportb(TX_START, 0x02);//清除MAC发送中断
	//
	//中断处理
	//
	outport(TXPL_LOW, 182);
	outport(TXPL2_LOW, add_data_length);
	outportb(TX_START, 0xf);//重新使能MAC发送中断
	add_data_length++;
}

*/
#define TX_START		0xfe08                 // 发送开始寄存器
//D0：发送开始位
//D1：CPU码流解码开始位
//D2：MAC发送中断使能
//D3: 0:发送包为一段数据；1：发送包为二段数据
//D4：发送包数据第二段64KB回绕()
//D5：MJPEG码流准备好中断使能()
//D6：
//D7：H264码流准备好中断使能()

#define TX_STATUS		0xfe01                 // 发送状态寄存器
//D[7:0](R, 0x00)
//D0: 0：无发送；1：发送正在进行
//D1：发送重发错误
//D2: 发送FIFO满错误
//D3：发送FIFO空错误
//D4：发送超时错误
//D5：发送冲突，0：无冲突；1：有冲突
//D6：载波侦听信号，0：无信号；1：有信号

//TX Packet Length Register
//#define TXPL_LOW		0xfefc                 // 发送包长度寄存器低字节
//#define TXPL_HIGH		0xfefd                 // 发送包长度寄存器高字节
#define TXPL_LOW		0xfe06                 // 发送包第一段数据长度寄存器低字节
#define TXPL_HIGH		0xfe07                 // 发送包第一段数据长度寄存器高字节
/*
	该寄存器在二段发送时必须为偶数
*/

#define TXPL2_LOW		0xfe20                 // 发送包第二段数据长度寄存器低字节
#define TXPL2_HIGH		0xfe21                 // 发送包第二段数据长度寄存器高字节

//发送包第一段数据起始地址寄存器（初始值为0x1a0040）
#define TX_SA_L		0xfed8	//发送包第一段数据起始地址低字节
#define TX_SA_M		0xfed9	//发送包第一段数据起始地址中字节
#define TX_SA_H		0xfeda	//发送包第一段数据起始地址高字节
/*
	该寄存器建议低四位为0，即第一段数据以16字节为界
*/

//发送包第二段数据起始地址寄存器
#define TX_SA2_L	0xfedb	//发送包第二段数据起始地址低字节
#define TX_SA2_M	0xfedc	//发送包第二段数据起始地址中字节
#define TX_SA2_H	0xfedd	//发送包第二段数据起始地址高字节
/*
	该寄存器建议低四位为0，即第二段数据以16字节为界
*/

//MAC接收（通用包）--本部分是除了视频包和PCM包以外的通用包
/*
    MAC接收的数据包放在SDRAM地址0x0c0000开始的64K字节接收缓冲区,每包占2K字节,可以容纳32个包,32个2K字节循环使用;
    2K字节的最后八个字节格式如下:
    包字节数低字节,包字节数高字节,包状态低字节（RX_PACKET_STATUS_L）,包状态低字节（RX_PACKET_STATUS_H）,4个字节保留.
    每接收到一个包就会产生一个中断。

//初始化
	outportb(RX_CONTROL, 0x03);//MAC接收中断使能
	setvect(8, MACRXInt);

//接收中断
void interrupt MACRXInt()
{
	outportb(RX_CONTROL, 0x01);//清除MAC接收中断
	//
	//中断处理
	//
	outportb(RX_CONTROL, 0x03);//重新使能MAC接收中断
}

*/
#define RX_PACKET_NUM		0xfe02             // 接受包数寄存器
//D[7:0]：当前接收包号

#define MAC_CONTROL		0xfe00             // MAC控制寄存器
//D[7:0](R/W, 0x01)
//D0：MAC全双工，0：半双工；1：全双工
//D1：MAC速度，0：10M；1：100M

#define TX_BACKOFF_NUM		0xfe1e             //发送BACKOFF数寄存器
//D[7:0](R/W, 0x40)
//D[7:0]：发送BACKOFF数

#define TX_ATTEMTP_NUM		0xfe1f             //发送尝试次数寄存器
//D[7:0](R/W, 0x07)
//D[7:0]：发送尝试次数寄存器

#define TX_JAM_CODE0		0xfe24             //发送阻塞码0寄存器
//D[7:0](R/W, 0x00)
//D[7:0]：发送阻塞码
#define TX_JAM_CODE1		0xfe25             //发送阻塞码1寄存器
//D[7:0](R/W, 0x00)
//D[7:0]：发送阻塞码
#define TX_JAM_CODE2		0xfe26             //发送阻塞码2寄存器
//D[7:0](R/W, 0x00)
//D[7:0]：发送阻塞码
#define TX_JAM_CODE3		0xfe27             //发送阻塞码3寄存器
//D[7:0](R/W, 0x00)
//D[7:0]：发送阻塞码

#define RX_CONTROL		0xfe03             // 接收控制寄存器
//D0：接收使能
//D1：接收中断使能
//D2：丢弃CRC错误包
//D3：丢弃广播包
//D4：丢弃多播包
//D[6:5]: MAC接收缓冲区大小
//	00: 64KB(RX_PACKET_NUM为5位)
//	01: 128KB(RX_PACKET_NUM为6位)
//	10: 保留
//	11: 256KB(RX_PACKET_NUM为7位)
//D7：0：所有接收到的包都进入接收缓冲区；1：MAC地址匹配、广播、多播包才进入接收缓冲区

#define RX_PACKET_STATUS_L	0xfe04             // 接收包状态寄存器低
//D0：接收中断状态，0：无接收中断；1：有接收中断；(原定义,现修改为)
//D0: MAC地址匹配，0：不匹配；1：匹配
//D1：CRC错误
//D2：半字节错误
//D3：MII管脚RX_ER为1
//D4：广播包
//D5：多播包
//D6：PCM包
//D7：视频包

#define RX_PACKET_STATUS_H	0xfe05             // 接收包状态寄存器低
//D0：IP包
//D1：ARP包
//D2：RARP包（原定义）
//D2：ARP包中IP地址匹配
//D3：TCP包
//D4：UDP包
//D5：ICMP包
//D6：IGMP包
//D7：OSPF包（原定义）
//D7：接收冲突，0：无冲突；1：有冲突

//CPU码流包数寄存器
#define BS_CPU_PN_L		0xfe0a
#define BS_CPU_PN_H		0xfe0b

//CPU码流解码起始地址寄存器
#define BS_CPU_SA_L		0xfe0c
#define BS_CPU_SA_M		0xfe0d
#define BS_CPU_SA_H		0xfe0e

//本地MAC物理地址
#define PAR0		0xfe10
#define PAR1		0xfe11
#define PAR2		0xfe12
#define PAR3		0xfe13
#define PAR4		0xfe14
#define PAR5		0xfe15

//多播包地址寄存器
#define MAR0		0xfe16//（最低字节）
#define MAR1		0xfe17
#define MAR2		0xfe18
#define MAR3		0xfe19
#define MAR4		0xfe1a
#define MAR5		0xfe1b//（如6字节MAC地址，最高字节）
#define MAR6		0xfe1c
#define MAR7		0xfe1d//（最高字节）

//本地IP地址
#define LIP0		0xfe30
#define LIP1		0xfe31
#define LIP2		0xfe32
#define LIP3		0xfe33

#define PCMI_SEQ_NUM_L		0xfe30
#define PCMI_SEQ_NUM_H		0xfe31

#define HTOTAL_L		0xfe60              //水平总数低字节
#define HTOTAL_H		0xfe61              //水平总数高字节
#define HDISP_L			0xfe62              //水平显示低字节
#define HDISP_H			0xfe63              //水平显示高字节
#define HSYNC_START_L		0xfe64          //水平同步开始低字节
#define HSYNC_START_H		0xfe65          //水平同步开始高字节
#define HSYNC_SIZE		0xfe66            //水平同步大小
#define HBACKPORCH		0xfe67            //水平回扫

#define VTOTAL_L		0xfe68              //垂直总数低字节
#define VTOTAL_H		0xfe69              //垂直总数高字节
#define VDISP			0xfe6a                //垂直显示
#define VSYNC_START		0xfe6c            //垂直开始
#define VSYNC_SIZE		0xfe6e            //垂直大小
#define VBACKPORCH		0xfe6f            //垂直回扫

#define WH_START_L		0xfe70            //视频窗口水平显示开始低字节
#define WH_START_H		0xfe71            //视频窗口水平显示开始高字节
#define WH_SIZE_L		0xfe72              //视频窗口水平大小低字节 
#define WH_SIZE_H		0xfe73              //视频窗口水平大小高字节  
#define WV_START		0xfe74              //视频窗口垂直开始
#define WV_SIZE			0xfe76              //视频窗口垂直大小

#define SCALE_SEL		0xfe78              //显示比例
//D[2:0]：水平压缩
//	000：8选8
//	001：8选7
//	010：8选6
//	011：8选5
//	100：8选4
//D[5:3]：垂直压缩
//	000：8选8
//	001：8选7
//	010：8选6
//	011：8选5
//	100：8选4
//D6：水平放大
//D7：垂直放大

#define DISPLAY_MODE		0xfe79          //显示模式
//D[2:0]: 时钟选择  	111：DCLK是CLK 8分频
//			110：DCLK是CLK 4分频
//			101：DCLK是CLK 2分频
//D3：	VSYNC作为DE
//D4：  VSYNC作为CS
//D5：  DCLK极性选择
//D6：  HSYNC极性选择
//D7：  VSYNC极性选择

#define rx_gen_SA		0x180000
#define tx_gen_SA		0x1a0040
#define pcmi_tx_haeder_SA	0x1a0000

//rgb_rd_SA 2010-02-08 修改 
//#define rgb_rd_SA		0x3c0000
#define rgb_rd_SA		0x400000
#define pcmi_SA			0x190000
#define pcmo_SA			0x1b0000


//显示内存起始地址寄存器、内存宽度寄存器   2010-02-08 添加
#define DM_START_L		0xfe7a
#define DM_START_H		0xfe7b
#define DM_START_M		0xfe7c
#define DM_WIDTH_L		0xfe7e
#define DM_WIDTH_H		0xfe7f

//控制寄存器
#define CONTROL_REG		0xfeea          //控制寄存器
//D0:	显示使能
//D1：  显示覆盖
//D2：  背景选择,0:背景寄存器值;1:SDRAM保持不变
//D3：	PCM输入使能
//D4：	PCM输出使能
//D5：  0：PCM自动处理；1：PCM手动处理
//D6：  PCM中断使能
//D7：  接收码流使能

//控制寄存器2
#define CONTROL_REG2		0xfeee          //控制寄存器2
//D0:	字符写时，只写灰度，0：正常字符写；1：灰度字符写
//D1: 显示时钟便慢，0：正常；1：在25MHZ显示时钟时，将时钟变慢1/8，以有足够时间进行视频采集
//D2：每颜色8位显示，0：6位；1：8位
//D3: 64KB内存窗口使能，0：CPU内存段A保持原值；1：64KB CPU内存段A映射到8M存储器任一64KB存储区，位置由寄存器MEMORY_WINDOW_BASE_H决定。
//D4：关闭PTC0时钟
//D5：关闭PTC1时钟
//D6：关闭PTC2时钟
//D7：关闭SPI时钟

//内存窗口基地址高寄存器
#define MEMORY_WINDOW_BASE_H		0xfeef
//D[6:0]:(R/W, 0x7a)
//8MB存储器地址高7位，低16位由CPU低16位地址决定

//PCM数据寄存器
#define PCM_DATA_REG		0xfeeb          //PCM数据寄存器

/*
//初始化
	setvect(10, PCMInt);
	outportb(CONTROL_REG, 0xb9);//清除PCM中断
	outportb(CONTROL_REG, 0xf9);//PCM中断使能

//中断服务程序
void interrupt PCMInt()
{
	outportb(CONTROL_REG, 0xb9);//清除PCM中断
	outportb(PCM_DATA_REG, add_data_length);
	outportb(CONTROL_REG, 0xf9);//使能PCM中断
	add_data_length++;
}
*/

//状态寄存器
#define STATUS_REG		0xfee0          //状态寄存器
//D1: 低功耗模式, 0：正常工作模式；1：低功耗模式
//D2：	SDRAM准备好

//位块（BitBlk）传送
#define BITBLK_CONTROL_REG	0xfe80	//位块控制寄存器
//D0:	位块传送启动
//D1：	透明使能
//D2：透明方式选择，0：数据最高位为零透明；1：数据全零透明
//D3: 0：源自SPI FLASH；1：源自SDRAM
//D4: 0：SDRAM与SPI FLASH之间传送; 1：SDRAM内部传送
//D5: SPI_sel, 0: SPI FLASH（SPI FLASH读命令和编程命令）; 1: SPI控制器
//D6: 位块传送完成中断(cpu中断号21）使能，0：清除位块传送完成中断；1：位块传送完成中断使能

#define BITBLK_STATUS_REG	0xfe81	//位块状态寄存器
//D0：位块传送完成

#define BITBLK_SOURCE_WIDTH_L	0xfe82	//位块源内存宽度低
#define BITBLK_SOURCE_WIDTH_H	0xfe83	//位块源内存宽度高
#define BITBLK_SA_L	0xfe84	//位块源地址低
#define BITBLK_SA_M	0xfe85	//位块源地址中
#define BITBLK_SA_H	0xfe86	//位块源地址高
#define BITBLK_DESTINATION_WIDTH_L	0xfe88	//位块目的内存宽度低
#define BITBLK_DESTINATION_WIDTH_H	0xfe89	//位块目的内存宽度高
#define BITBLK_DA_L	0xfe8c	//位块目的地址低
#define BITBLK_DA_M	0xfe8d	//位块目的地址中
#define BITBLK_DA_H	0xfe8e	//位块目的地址高
#define BITBLK_X_SIZE_L	0xfe90	//位块X尺寸低（SPI FLASH与SDRAM传送字节数低）
#define BITBLK_X_SIZE_H	0xfe91	//位块X尺寸高（SPI FLASH与SDRAM传送字节数中）
#define BITBLK_Y_SIZE_L	0xfe92	//位块Y尺寸低（SPI FLASH与SDRAM传送字节数高）
#define BITBLK_Y_SIZE_H	0xfe93	//位块Y尺寸高

//UART0
#define UART0_TX_STATUS 0xff80      //UART0发送状态寄存器 
//D0: 发送写允许
//D1: 自动波特率识别完成

#define UART0_TX_DATA   0xff81      //UART0发送数据寄存器
#define UART0_RX_CTRL   0xff82      //UART0接收控制寄存器
//D[3:0] 接收FIFO字节计数
//D4:    接收FIFO有数据指示(接收FIFO满指示)
//D5:    中断状态指示
//D6:    接收使能
//D7:    中断使能
#define UART0_RX_DATA       0xff83  //UART0接收数据寄存器
#define UART0_DIVIDER_LOW   0xff84  //UART0分频系数低8位
#define UART0_DIVIDER_HIGH  0xff85  //UART0分频系数高8位
#define UART0_ABR_LOW   0xff86  //UART0自动波特率分频系数低8位
#define UART0_ABR_HIGH  0xff87  //UART0自动波特率分频系数高8位

//UART1
#define UART1_TX_STATUS 0xff88      //UART0发送状态寄存器 
//D0: 发送写允许
//D1: 自动波特率识别完成

#define UART1_TX_DATA   0xff89      //UART0发送数据寄存器
#define UART1_RX_CTRL   0xff8a      //UART0接收控制寄存器
//D[3:0] 接收FIFO字节计数
//D4:    接收FIFO有数据指示(接收FIFO满指示)
//D5:    中断状态指示
//D6:    接收使能
//D7:    中断使能
#define UART1_RX_DATA       0xff8b  //UART0接收数据寄存器
#define UART1_DIVIDER_LOW   0xff8c  //UART0分频系数低8位
#define UART1_DIVIDER_HIGH  0xff8d  //UART0分频系数高8位
#define UART1_ABR_LOW   0xff8e  //UART0自动波特率分频系数低8位
#define UART1_ABR_HIGH  0xff8f  //UART0自动波特率分频系数高8位

//PTC0
/*
	本PTC主要用于LCD背光。
*/
//寄存器
#define PTC0_HRC0   0xffa0          // PWM/定时器/计时器(PTC)高参考/捕获寄存器0
#define PTC0_HRC1   0xffa1          // PWM/定时器/计时器(PTC)高参考/捕获寄存器1
#define PTC0_HRC2   0xffa2          // PWM/定时器/计时器(PTC)高参考/捕获寄存器2
//PTC0_HRC0-2在参考模式下，将PWM输出设置为高
#define PTC0_LRC0   0xffa3          // PWM/定时器/计时器(PTC)低参考/捕获寄存器0
#define PTC0_LRC1   0xffa4          // PWM/定时器/计时器(PTC)低参考/捕获寄存器1
#define PTC0_LRC2   0xffa5          // PWM/定时器/计时器(PTC)低参考/捕获寄存器2
//PTC0_LRC0-2在参考模式下，将PWM输出设置为低

#define PTC0_CTRL0  0xffa6          // PWM/定时器/计时器(PTC)控制寄存器0
//D0: R/W, EN    1: PTC0_CNTR正常工作，0：PTC0_CNTR不工作
//D1: R/W, ECLK  1: PTC_ECGT作为PTC0_CNTR的时钟，0：系统时钟作为PTC0_CNTR的时钟
//D2: R/W, NEC   1: PTC_ECGT下降沿有效，低电平时门控计数，0: PTC_ECGT上升沿有效，低电平时门控计数
//D3: R/W, OE    1: PWM输出使能
//D4: R/W, SINGLE   1: 计时器PTC0_CNTR只运行一次，增加到PTC0_LRC，0：计时器PTC0_CNTR增加到PTC0_LRC后自动重新开始计数
//D5: R/W, INTE  1: PTC在PTC0_CNTR等于PTC0_LRC或者PTC0_HRC时产生中断，0：不产生中断
//D6: R/W, INT  1: INT 中断的当前状态，主设备写入1将清除中断
//D7: R/W, CNTRRST  1: 计数器复位，0：计数器正常工作

#define PTC0_CTRL1  0xffa7          // PWM/定时器/计时器(PTC)控制寄存器1
//D0: R/W CAPTE  1：启动捕获模式，0：捕获模式关闭 

//PTC1
/*
    本PTC主要用于看门狗。
*/
//寄存器
#define PTC1_HRC0   0xffa8          // PWM/定时器/计时器(PTC)高参考/捕获寄存器0
#define PTC1_HRC1   0xffa9          // PWM/定时器/计时器(PTC)高参考/捕获寄存器1
#define PTC1_HRC2   0xffaa          // PWM/定时器/计时器(PTC)高参考/捕获寄存器2
//PTC1_HRC0-2在参考模式下，将PWM输出设置为高
#define PTC1_LRC0   0xffab          // PWM/定时器/计时器(PTC)低参考/捕获寄存器0
#define PTC1_LRC1   0xffac          // PWM/定时器/计时器(PTC)低参考/捕获寄存器1
#define PTC1_LRC2   0xffad          // PWM/定时器/计时器(PTC)低参考/捕获寄存器2
//PTC1_LRC0-2在参考模式下，将PWM输出设置为低

#define PTC1_CTRL0  0xffae          // PWM/定时器/计时器(PTC)控制寄存器0
//D0: R/W, EN    1: PTC1_CNTR正常工作，0：PTC1_CNTR不工作
//D1: R/W, ECLK  1: PTC_ECGT作为PTC1_CNTR的时钟，0：系统时钟作为PTC1_CNTR的时钟
//D2: R/W, NEC   1: PTC_ECGT下降沿有效，低电平时门控计数，0: PTC_ECGT上升沿有效，低电平时门控计数
//D3: R/W, OE    1: PWM输出使能
//D4: R/W, SINGLE   1: 计时器PTC1_CNTR只运行一次，增加到PTC1_LRC，0：计时器PTC1_CNTR增加到PTC1_LRC后自动重新开始计数
//D5: R/W, INTE  1: PTC在PTC1_CNTR等于PTC1_LRC或者PTC1_HRC时产生中断，0：不产生中断
//D6: R/W, INT  1: INT 中断的当前状态，主设备写入1将清除中断
//D7: R/W, CNTRRST  1: 计数器复位，0：计数器正常工作

#define PTC1_CTRL1  0xffaf          // PWM/定时器/计时器(PTC)控制寄存器1
//D0: R/W CAPTE  1：启动捕获模式，0：捕获模式关闭 

//PTC2
/*
    本PTC主要用于通用定时器。
*/
//寄存器
#define PTC2_HRC0   0xffc0          // PWM/定时器/计时器(PTC)高参考/捕获寄存器0
#define PTC2_HRC1   0xffc1          // PWM/定时器/计时器(PTC)高参考/捕获寄存器1
#define PTC2_HRC2   0xffc2          // PWM/定时器/计时器(PTC)高参考/捕获寄存器2
//PTC2_HRC0-2在参考模式下，将PWM输出设置为高
#define PTC2_LRC0   0xffc3          // PWM/定时器/计时器(PTC)低参考/捕获寄存器0
#define PTC2_LRC1   0xffc4          // PWM/定时器/计时器(PTC)低参考/捕获寄存器1
#define PTC2_LRC2   0xffc5          // PWM/定时器/计时器(PTC)低参考/捕获寄存器2
//PTC2_LRC0-2在参考模式下，将PWM输出设置为低

#define PTC2_CTRL0  0xffc6          // PWM/定时器/计时器(PTC)控制寄存器0
//D0: R/W, EN    1: PTC2_CNTR正常工作，0：PTC2_CNTR不工作
//D1: R/W, ECLK  1: PTC_ECGT作为PTC2_CNTR的时钟，0：系统时钟作为PTC2_CNTR的时钟
//D2: R/W, NEC   1: PTC_ECGT下降沿有效，低电平时门控计数，0: PTC_ECGT上升沿有效，低电平时门控计数
//D3: R/W, OE    1: PWM输出使能
//D4: R/W, SINGLE   1: 计时器PTC2_CNTR只运行一次，增加到PTC2_LRC，0：计时器PTC2_CNTR增加到PTC2_LRC后自动重新开始计数
//D5: R/W, INTE  1: PTC在PTC2_CNTR等于PTC2_LRC或者PTC2_HRC时产生中断，0：不产生中断
//D6: R/W, INT  1: INT 中断的当前状态，主设备写入1将清除中断
//D7: R/W, CNTRRST  1: 计数器复位，0：计数器正常工作

#define PTC2_CTRL1  0xffc7          // PWM/定时器/计时器(PTC)控制寄存器1
//D0: R/W CAPTE  1：启动捕获模式，0：捕获模式关闭 

//=========
//SPI控制器
//=========

#define SPCR  0xffb0              //SPI 控制寄存器  默认值 0x10
//D7: R/W SPIE  0: SPI中断被屏蔽 1：SPI中断开启
//D6: R/W SPE   0: SPI关闭   1：SPI使能
//D5: R/W SS    0: 从设备选择 1：从设备未选择
//D4: R/W MSTR  0: 从设备工作模式  1：主设备工作模式
//D3: R/W CPOL  定义数据传输模式
//D2: R/W CPHA  定义数据传输模式， 这两个信号，定义4种模式
//D[1:0] R/W SPR  此位和扩展寄存器中的ESPR位共同决定SPI核的串行时钟频率

#define SPSR  0xffb1              //SPI 状态寄存器   默认值 0x05
//D7: R/W SPIF  中断标记，当若干个字节的数据传输完成后，此位会被置1。
//              如果同时SPCR寄存器的SPIE位也被置1的话，就会产生一个中断。
//              要清除这个中断，需要向此位写入1
//D6: R/W WCOL  写冲突，如果写队列已满，再向数据寄存器执行写入操作，此位会被置1
//              要将此位清零，需要向此位写入1
//D[5:4] R  保留
//D3: R  WFFULL  写FIFO满
//D2: R  WFEMPTY 写FIFO空
//D1: R  RFFULL  读FIFO满
//D0: R  RFEMPTY 读FIFO空

#define SPDR  0xffb2              //SPI 数据寄存器

#define SPER  0xffb3              //SPI 扩展寄存器
//D[7:6] R/W ICNT  中断计数 (ICNT+1)个字节数据传输完后，SPIF位被置1
//D[1:0] R/W ESPR  扩展SPI时钟频率选择
//    ESPR              SPR                  分频倍数
//    00                 00                    2
//    00                 01                    4
//    00                 10                    16
//    00                 11                    32
//    01                 00                    8
//    01                 01                    64
//    01                 10                    128
//    01                 11                    256
//    10                 00                    512
//    10                 01                    1024
//    10                 10                    2048
//    10                 11                    4096

//==========
//中断控制器
//==========
/*
    中断控制器有8个中断输入，一个中断输出接至CPU的中断输入；8个输入有中断输入时，向CPU发出中断请求，CPU返回两个中断响应
脉冲，控制器在第二个中断响应脉冲返回八位中断号，CPU根据中断号查询中断向量表，进入中断服务程序。
    8个中断输入的输入分别如下：
    中断0：接收中断
    中断1：发送中断
    中断2：PCM中断（IIS_CONTROL寄存器D0=0）/IIS发送中断（IIS_CONTROL寄存器D0=1）
    中断3：外部管脚ir3输入（IIS_CONTROL寄存器D0=0）/IIS接收中断（IIS_CONTROL寄存器D0=1）
    中断4：PCT2
    中断5：UART0
    中断6：UART1
    中断7：SPI
    中断8：MJPEG编码准备好
*/
//寄存器
#define INT_MASK  0xff20              //中断屏蔽寄存器（读/写，初始值：0XFF）
//0：中断使能；1：中断屏蔽
#define INT_MASK_H  0xff24              //中断屏蔽高寄存器（读/写，初始值：0XFF）
//0：中断使能；1：中断屏蔽

#define INT_EDGE  0xff21              //中断极性寄存器（读/写，初始值：0X00）
//0：上升沿触发；1：下降沿触发；
#define INT_EDGE_H  0xff25              //中断极性高寄存器（读/写，初始值：0X00）
//0：上升沿触发；1：下降沿触发；

#define INT_VECTOR  0xff22              //中断向量寄存器（读/写，初始值：0X08）
//D7[7：3]：中断号高五位，与中断服务寄存器低三位合并，组成八位中断号,八位中断号乘以四作为中断向量表地址。

//#define INT_VECTOR  0xff23              //中断服务寄存器（只读，初始值：0X00）
#define INT_ISR  0xff23              //中断服务寄存器（只读，初始值：0X00）//于2011-0120修正
//D[2：0]：正在服务的中断号

/*
编程范例：
	outportb(INT_MASK, 0xf0);//高四个中断使能，低四个中断关闭
	outportb(INT_EDGE, 0x00);//都是上升沿触发
	outportb(INT_VECTOR, 0x08);//中断号为8-15
	enable();
	
	
*/

//========
//视频输入
//========
#define VI_CONTROL  0xff30              //视频输入控制寄存器
//D0：视频输入使能（R/W）
//D1：视频输入水平压缩使能（R/W）
//D2：视频输入居中使能（R/W）
//D3: 视频输入4:2:2至4:1:1变换方式（R/W），0：
//D4: 视频输入采集窗口；0：CIF；1：VGA
//D6: 视频输入垂直压缩使能（R/W）
//D7: 视频输入隔行使能（R/W）

#define VI_STATUS  0xff31              //视频输入状态寄存器
//D0: 视频输入已可用状态（R）
//D[2：1]：视频输入当前场号（R）

#define VI_HSIZE_L  0xff32              //视频输入水平尺寸低寄存器
//D[7:0]:(R)
#define VI_HSIZE_H  0xff33              //视频输入水平尺寸高寄存器
//D[1:0]:(R)

#define VI_VSIZE_L  0xff34              //视频输入垂直尺寸低寄存器
//D[7:0]:(R)
#define VI_VSIZE_H  0xff35              //视频输入垂直尺寸高寄存器
//D[1:0]:(R)

#define VI_CONTROL2  0xff36              //视频输入控制寄存器2
//D0：保留（R/W）
//D1：视频输入窗口寄存器（0xff38-0xff3f）使能，0：使用VI_CONTROL D2、D4 控制视频输入窗口；1：使用（0xff38-0xff3f）控制视频输入窗口
//D2: 

//于2011-01-20添加（）
#define VI_MLINE  0xff37              //视频输入未编码宏行数寄存器
//D[7:0]:(R)

#define VI_WIN_HSTART_L  0xff38              //视频输入窗口水平起点低寄存器
//D[7:0]:(R/W,0x90)
#define VI_WIN_HSTART_H  0xff39              //视频输入窗口水平起点高寄存器
//D[7:0]:(R/W,0x00)

#define VI_WIN_VSTART_L  0xff3a              //视频输入窗口垂直起点低寄存器
//D[7:0]:(R/W,0x00)
#define VI_WIN_VSTART_H  0xff3b              //视频输入窗口垂直起点高寄存器
//D[7:0]:(R/W,0x00)

#define VI_WIN_HSIZE_L  0xff3c              //视频输入窗口水平尺寸低寄存器
//D[7:0]:(R/W,0x60)
#define VI_WIN_HSIZE_H  0xff3d              //视频输入窗口水平尺寸高寄存器
//D[7:0]:(R/W,0x01)

#define VI_WIN_VSIZE_L  0xff3e              //视频输入窗口垂直尺寸低寄存器
//D[7:0]:(R/W,0x20)
#define VI_WIN_VSIZE_H  0xff3f              //视频输入窗口垂直尺寸高寄存器
//D[7:0]:(R/W,0x01)

//========
//视频解码
//========
#define DE_QP  0xff40              //视频解码QP寄存器
//D[5:0](R)

#define DE_ERROR  0xff41              //视频解码错误数寄存器
//D[7:0](R)

#define DE_HSIZE_L  0xff42              //视频解码水平尺寸低寄存器
//D[7:0](R)

#define DE_HSIZE_H  0xff43              //视频解码水平尺寸高寄存器
//D[5:0](R)

#define DE_VSIZE_L  0xff44              //视频解码垂直尺寸低寄存器
//D[7:0](R)

#define DE_VSIZE_H  0xff45              //视频解码垂直尺寸高寄存器
//D[5:0](R)

#define DE_RATE_L  0xff46              //视频解码码流KB数低寄存器
//D[7:0](R)

#define DE_RATE_H  0xff47              //视频解码码流KB数高寄存器
//D[7:0](R)

//==============
//GPIO A,B,C,D,E
//==============

//GPIO A
#define GPIOA_OUT_DATA  0x02d0		//GPIO A 输出数据寄存器
//D[7:0](R/W)

#define GPIOA_OEN_DATA  0x02d1		//GPIO A 输出使能非寄存器
//D[7:0](R/W): 0:输出使能;1:输出屏蔽

#define GPIOA_IN_DATA  0x02d2		//GPIO A 输入数据寄存器
//D[7:0](R)

//GPIO B
#define GPIOB_OUT_DATA  0x02d4		//GPIO B 输出数据寄存器
//D[7:0](R/W)

#define GPIOB_OEN_DATA  0x02d5		//GPIO B 输出使能非寄存器
//D[7:0](R/W): 0:输出使能;1:输出屏蔽

#define GPIOB_IN_DATA  0x02d6		//GPIO B 输入数据寄存器
//D[7:0](R)

#define GPIOB_SEL  0x02d7		//GPIO B 功能选择寄存器
//D[7:0](R/W): 0:管脚原功能;1:管脚作为GPIO

//GPIO C
#define GPIOC_OUT_DATA  0x02d8		//GPIO C 输出数据寄存器
//D[7:0](R/W)

#define GPIOC_OEN_DATA  0x02d9		//GPIO C 输出使能非寄存器
//D[7:0](R/W): 0:输出使能;1:输出屏蔽

#define GPIOC_IN_DATA  0x02da		//GPIO C 输入数据寄存器
//D[7:0](R)

#define GPIOC_SEL  0x02db		//GPIO C 功能选择寄存器
//D[7:0](R/W): 0:管脚原功能;1:管脚作为GPIO

//GPIO D
#define GPIOD_OUT_DATA  0x02dc		//GPIO D 输出数据寄存器
//D[7:0](R/W、保留)

#define GPIOD_OEN_DATA  0x02dd		//GPIO D 输出使能非寄存器
//D[7:0](R/W、保留): 0:输出使能;1:输出屏蔽

#define GPIOD_IN_DATA  0x02de		//GPIO D 输入数据寄存器
//D[7:0](R、保留)

#define GPIOD_SEL  0x02df		//GPIO D 功能选择寄存器
//D[7:0](R/W、保留): 0:管脚原功能;1:管脚作为GPIO

//GPIO E
#define GPIOE_OUT_DATA  0x02e0		//GPIO E 输出数据寄存器
//D[7:0](R/W、保留)

#define GPIOE_OEN_DATA  0x02e1		//GPIO E 输出使能非寄存器
//D[7:0](R/W、保留): 0:输出使能;1:输出屏蔽

#define GPIOE_IN_DATA  0x02e2		//GPIO E 输入数据寄存器
//D[7:0](R、保留)

#define GPIOE_SEL  0x02e3		//GPIO E 功能选择寄存器
//D[7:0](R/W、保留): 0:管脚原功能;1:管脚作为GPIO

//于2011-01-20添加（）
//视频编码
#define EN_CONTROL  0xfc00		//视频编码控制寄存器
//D[7:0](R/W，0x00)
//D0：编码使能，0：编码禁止；1：编码使能
//D1：码流发送使能，0：码流软件发送；1：码流自动发送
//D2：H264选择，（暂未实现）
//D3：视频输入模块清零，1：主要信号强制清零()
//D4：视频编码模块清零，1：主要信号强制清零()
//D5：码流发送模块清零，1：主要信号强制清零()
//D6：码流产生中断使能，0：中断清除；1：中断使能()
//D7：保留

#define EN_STATUS  0xfc01		//视频编码状态寄存器
//D[7:0](R，0x00)
//D0：序列头周期
//D1：I图像头周期
//D2：P图像头周期
//D3：条带周期
//D4：视频采集周期
//D5：发送请求周期
//D6：发送周期
//D7：码流缓冲满

#define EN_BAUD_RATE_L  0xfc02		//视频编码波特率低寄存器
//D[7:0](R/W，0xe8)
#define EN_BAUD_RATE_H  0xfc03		//视频编码波特率高寄存器
//D[15:8](R/W，0x03)
//

#define EN_QP_PRE  0xfc04		//视频编码QP预置寄存器
//D[7:0](R/W，0x20（0x1a(H.264))
//D[5:0]:预置QP值
//D7:VBR（可变波特率）使能

#define EN_QP  0xfc05		//视频编码QP读回寄存器
//D[5:0](R，0x20（）/0x1a(H.264))

#define EN_MBWIDTH  0xfc06		//视频编码宏块宽度寄存器
//D[6:0](R/W，0x16)

#define EN_MBHEIGHT  0xfc07		//视频编码宏块高度寄存器
//D[6:0](R/W，0x12)

#define EN_BUF_USED_L  0xfc08		//视频编码码流缓存区已使用大小低寄存器
//D[7:0](R)
#define EN_BUF_USED_H  0xfc09		//视频编码码流缓存区已使用大小高寄存器
//D[15:8](R)

#define EN_TX_BS_PACK_L  0xfc0a		//视频编码码流发送包数低寄存器
//D[7:0](R)
#define EN_TX_BS_PACK_H  0xfc0b		//视频编码码流发送包数高寄存器
//D[11:8](R)

#define EN_PICTURE  0xfc0c		//视频编码图像号寄存器
//D[7:0](R)

#define EN_MICRO_LINE  0xfc0d		//视频编码图像宏行数寄存器
//D[6:0](R)

#define EN_MICRO_BLOCK  0xfc0e		//视频编码图像宏块数寄存器
//D[6:0](R)

#define EN_BLOCK  0xfc0f		//视频编码图像块数寄存器
//D[2:0](R)

#define EN_TX_BS_RATE_KB_L  0xfc10		//视频编码码流发送波特率低寄存器（以KB为单位）
//D[7:0](R)
#define EN_TX_BS_RATE_KB_H  0xfc11		//视频编码码流发送波特率高寄存器（以KB为单位）
//D[15:8](R)

//于2011-05-27添加（）
//PCM编解码
#define PCMI_CONTROL  0xfc20		//音频编码控制寄存器
//D[7:0](R/W，0x00)
//D[3:0]:编码音量控制
// 0000：音量不变
// 0001：音量减去1/16
// 0010：音量减去2/16
// 0011：音量减去3/16
// 0100：音量减去4/16
// 0101：音量减去5/16
// 0110：音量减去6/16
// 0111：音量减去7/16
// 1000：音量减去8/16
// 1001：音量减去9/16
// 1010：音量减去10/16
// 1011：音量减去11/16
// 1100：音量减去12/16
// 1101：音量减去13/16
// 1110：音量减去14/16
// 1111：音量减去15/16
//D4:编码音量调节使能，0:禁止，编码音量控制不起作用；1:编码音量控制起作用(未实现)
//D5:编码自动侧音调节，0:禁止; 1:自动侧音调节(未实现)
//D6：PCM编解码功率格式；0：24位定点数；1：24位浮点数，16位精度加8位移位
//D7：功率中断使能

#define PCM_STATUS  0xfc21		//音频状态寄存器
//D[7:0](R, 0x00)
//D0:	手动中断（取消）
//D1:	功率中断（取消）
//D2:	PCM输入FIFO空；0：不空；1：空（移至D0）
//D3：	PCM输出FIFO满；0：不满；1：满（移至D1）
//D[3:2]: PCMI数据块号
//D[5:4]: PCMO数据块号
//D6: PCMO缓冲区少于一块；0：PCMO缓冲区多于一块；1：PCMO缓冲区少于一块
//D7: PCMO缓冲区空；0:PCMO缓冲区不空；1：PCMO缓冲区空

//#define PCMI_THRE_L  0xfc22		//160样点音频编码阈值低寄存器(未实现)
////D[7:0](R/W，0x00)
//#define PCMI_THRE_H  0xfc23		//160样点音频编码阈值高寄存器(未实现)
////D[7:0](R/W，0x00)

#define PCMI_POWER_L  0xfc24		//160/64/128/256样点音频编码功率低寄存器
//D[7:0](R，0x00)
#define PCMI_POWER_H  0xfc25		//160/64/128/256样点音频编码功率高寄存器
//D[7:0](R，0x00)
//24位定点数格式时：8位PCM转成16位PCM，然后绝对值相加，64项（或128、256）相加24位结果高16位
//24位浮点数格式时：8位PCM转成16位PCM，然后绝对值相加，64项（或128、256）相加结果浮点16位（最高位为小数点前一位，后面15位为小数）

#define PCMI_POWER_S  0xfc22		//160/64/128/256样点音频编码功率移位寄存器
//D[7:0](R/W，0x00)
//24位定点数格式时：8位PCM转成16位PCM，然后绝对值相加，64项（或128、256）相加24位结果低8位
//24位浮点数格式时：8位PCM转成16位PCM，然后绝对值相加，64项（或128、256）相加结果的移位数

#define PCMO_CONTROL  0xfc28		//音频解码控制寄存器
//D[7:0](R/W，0x00)
//D[3:0]:解码音量控制
// 0000：音量不变
// 0001：音量减去1/16
// 0010：音量减去2/16
// 0011：音量减去3/16
// 0100：音量减去4/16
// 0101：音量减去5/16
// 0110：音量减去6/16
// 0111：音量减去7/16
// 1000：音量减去8/16
// 1001：音量减去9/16
// 1010：音量减去10/16
// 1011：音量减去11/16
// 1100：音量减去12/16
// 1101：音量减去13/16
// 1110：音量减去14/16
// 1111：音量减去15/16
//D4:解码音量调节使能，0:禁止，解码音量控制不起作用；1:解码音量控制起作用(未实现)
//D5:解码自动侧音调节，0:禁止; 1:自动侧音调节(未实现)
//D6: =1：PCMO缓冲区块数加1
//D7: PCMO中断使能；0：屏蔽；1：使能

#define PCM_CONTROL  0xfc29		//PCM接口控制寄存器
//D[7:0](R/W，0x00)
//D0：PCM接口位数；0:8位；1:16位
//D1：K8宽度；0：八个周期；1：一个周期
//D2：第一周期数据有效；0：有效；1：跳过
//D3：采样率；0:8K；1:16K
//D4: PCMO内部相连（反馈）
//D[6:5]:PCM数据块大小
//	00:160字节
//	01: 64字节（块号0-3）
//	10:128字节（块号0-3）
//	11:256字节（块号0-3）
//D7：A律、u律PCM选择，0：A律；1：u律

#define PCMO_POWER_L  0xfc2c		//160/64/128/256样点音频解码功率低寄存器
//D[7:0](R，0x00)
#define PCMO_POWER_H  0xfc2d		//160/64/128/256样点音频解码功率高寄存器
//D[7:0](R，0x00)
//24位定点数格式时：8位PCM转成16位PCM，然后绝对值相加，64项（或128、256）相加24位结果高16位
//24位浮点数格式时：8位PCM转成16位PCM，然后绝对值相加，64项（或128、256）相加结果浮点16位（最高位为小数点前一位，后面15位为小数）

#define PCMO_POWER_S  0xfc23		//160/64/128/256样点音频解码功率移位寄存器
//D[7:0](R/W，0x00)
//24位定点数格式时：8位PCM转成16位PCM，然后绝对值相加，64项（或128、256）相加24位结果低8位
//24位浮点数格式时：8位PCM转成16位PCM，然后绝对值相加，64项（或128、256）相加结果的移位数

//视频MJPEG编码
#define EN_J_CONTROL  0xfb00		//视频MJPEG编码控制寄存器1
//D[7:0](R/W，0x00)
//D0：MJPEG编码使能，0：MJPEG编码禁止；1：MJPEG编码使能
//D1：MJPEG编码码流存放方式,0：循环方式；1：页方式（码流区分成大小相同的2页）
//D2：MJPEG编码码流区大小,0:64KB；1：128KB
//D3：
//D4：视频MJPEG编码模块清零，1：主要信号强制清零()
//D5：码流发送模块清零，1：主要信号强制清零()
//D6：码流产生中断使能，0：中断清除；1：中断使能()
//D7：保留

#define EN_J_STATUS  0xfb01		//视频MJPEG编码状态寄存器
//D[7:0](R，0x00)
//D0：编码正在进行
//D1：编码数据有效
//D2：编码页号，0：低32/64KB（地址0x170000开始32KB、或地址0x170000开始64KB）；1：高32/64KB（地址0x178000开始32KB、或地址0x180000开始64KB）
//D3：MJPEG编码码流区满，0：不满；1：满错误
//D4：
//D5：
//D6：
//D7：

#define EN_J_DQT_ADD  0xfb02		//视频MJPEG编码DQT地址寄存器
//D[7:0](R/W)
#define EN_J_DQT_DAT  0xfb03		//视频MJPEG编码(1/DQT)数据寄存器
//D[6:0](R/W)
#define EN_J_DQT_SHIFT  0xfb04		//视频MJPEG编码(1/DQT)数据移位寄存器
//D[3:0](R/W)
//先根据0--100得到与解码一样的DQT值，因编码是除法，本来是level/DQT,即乘以1/DQT，也就是（2的N次方）/（DQT*（2的N次方）），将N放人EN_J_DQT_SHIFT寄存器，
//将（2的N次方）/DQT放人EN_J_DQT_DAT，EN_J_DQT_DAT是128个七位寄存器，由EN_J_DQT_ADD选择，前64个用于Y，后64个用于UVN根据EN_J_DQT_DAT得到最大精度决定

#define EN_J_CONTROL2  0xfb05		//视频MJPEG编码控制寄存器2
//D[7:0](R/W，0x00)
//D[5:0]: 编码后图像每秒帧数
//D[7:6]: 原图像每秒帧数
//	=00:25帧
//	=01:30帧
//	=10:50帧（保留）
//	=11:60帧（保留）

//D[7:6]=00时，D[5:0]为1-25(0和范围外是根据编码速度自动选择帧率）
//D[7:6]=01时，D[5:0]为1-30(0和范围外是根据编码速度自动选择帧率）

#define EN_J_MBWIDTH  0xfb06		//视频MJPEG编码宏块宽度寄存器
//D[6:0](R/W，0x16)

#define EN_J_MBHEIGHT  0xfb07		//视频MJPEG编码宏块高度寄存器
//D[6:0](R/W，0x12)

#define EN_J_TX_BS_PACK_L  0xfb0a		//视频MJPEG编码码流发送包数寄存器
//D[3:0](R)

#define EN_J_PICTURE  0xfb0c		//视频MJPEG编码图像号寄存器
//D[7:0](R)

#define EN_J_MICRO_LINE  0xfb0d		//视频MJPEG编码图像宏行数寄存器
//D[6:0](R)

#define EN_J_MICRO_BLOCK  0xfb0e		//视频MJPEG编码图像宏块数寄存器
//D[6:0](R)

#define EN_J_BLOCK  0xfb0f		//视频MJPEG编码图像块数寄存器
//D[2:0](R)

//视频MJPEG解码
#define DE_J_CONTROL  0xfb10		//视频MJPEG解码控制寄存器
//D[7:0](R/W，0x00)
//D0：MJPEG解码使能，0：MJPEG解码禁止；1：MJPEG解码使能
//D1：MJPEG解码启动，1：MJPEG解码启动
//D2：MJPEG解码结束中断使能，0：中断清除；1：中断使能
//D3：MJPEG解码以16位RGB输出，0：YUV输出；1：16位RGB输出
//D4: MJPEG解码FF格式，0：不变换；1：FF00变为FF（即将改为）
//D4：MJPEG码流64KB回绕，0：不回绕；1：回绕
//D5：显示帧中断使能，0：中断清除；1：中断使能
//D6: YUV 4:2:0到4:2:2变换，0：无变换；1：变换
//D7: 文件格式MJPEG解码，0：纯码流解码；1：文件格式解码

#define DE_J_STATUS  0xfb11		//视频MJPEG解码状态寄存器
//D[7:0](R，0x00)
//D0：解码正在进行
//D1：解码数据有效
//D2：解码错误
//D3：
//D4：
//D5：
//D6：
//D7：

#define DE_J_CPU_SA_H  0xfb14		//视频MJPEG CPU解码码流起始地址高寄存器
//D[7:0](R/W，0x00)
#define DE_J_CPU_SA_M  0xfb13		//视频MJPEG CPU解码码流起始地址中寄存器
//D[7:0](R/W，0x00)
#define DE_J_CPU_SA_L  0xfb12		//视频MJPEG CPU解码码流起始地址低寄存器
//D[7:0](R/W，0x00)

#define DE_J_CPU_PN  0xfb15		//视频MJPEG CPU解码包数寄存器
//D[7:0](R/W，0x00)

#define DE_J_MBWIDTH  0xfb16		//视频MJPEG解码宏块宽度寄存器
//D[6:0](R/W，0x16)

#define DE_J_MBHEIGHT  0xfb17		//视频MJPEG解码宏块高度寄存器
//D[6:0](R/W，0x12)

#define DE_J_HSIZE_L  0xfb18		//视频MJPEG解码宽度低寄存器
//D[7:0](R/W，0x0)
#define DE_J_HSIZE_H  0xfb19		//视频MJPEG解码宽度高寄存器
//D[10:8](R/W，0x2)

#define DE_J_VSIZE_L  0xfb1a		//视频MJPEG解码高度低寄存器
//D[7:0](R/W，0x0)
#define DE_J_VSIZE_H  0xfb1b		//视频MJPEG解码高度高寄存器
//D[10:8](R/W，0x2)

#define DE_J_DQT_ADD  0xfb1c		//视频MJPEG解码DQT地址寄存器
//D[6:0](R/W)
#define DE_J_DQT_DAT  0xfb1d		//视频MJPEG解码DQT数据寄存器
//D[6:0](R/W)

#define DE_J_DATA_SA_H  0xfb22		//视频MJPEG解码数据起始地址高寄存器
//D[7:0](R/W，0x00)
#define DE_J_DATA_SA_M  0xfb21		//视频MJPEG解码数据起始地址中寄存器
//D[7:0](R/W，0x00)
#define DE_J_DATA_SA_L  0xfb20		//视频MJPEG解码数据起始地址低寄存器
//D[7:0](R/W，0x00)

#define DE_J_CPU_LEN_H  0xfb26		//视频MJPEG CPU解码码流长度高寄存器
//D[7:0](R/W，0x00)
#define DE_J_CPU_LEN_M  0xfb25		//视频MJPEG CPU解码码流长度中寄存器
//D[7:0](R/W，0x00)
#define DE_J_CPU_LEN_L  0xfb24		//视频MJPEG CPU解码码流长度低寄存器
//D[7:0](R/W，0x00)

#define MAC_RX_SA_H  0xfb23		//MAC接收缓冲区起始地址高寄存器
//D[7:0](R/W，0x18)
//网络接收缓冲区起始地址位[23:16],低16位为0

#define USED_RATE_SEL  0xfb28		//使用频率信号选择低寄存器
//D[7:0](R/W，0x00)
//8'h00: SDRAM使用频率
//8'h01: MJPEG编码使用频率
//8'h02: MJPEG编码使用频率
//8'h03: MAC接收使用频率
//8'h04: MAC发送使用频率

#define USED_RATE_L  0xfb2a		//使用频率低寄存器
//D[7:0](R，0x0)
#define USED_RATE_H  0xfb2b		//使用频率高寄存器
//D[15:8](R，0x0)
//在一秒内，信号使用计数/65535

//视频显示窗口（补充）
#define VW_CONTROL  0xfb30		//视频窗口控制寄存器
//D0: 手动更新视频窗口；0：自动更新；1：利用显示中断切换视频窗口数据
//D1: 视频窗口格式；
//	0：YUV分离格式；
//		YUV按照Y图像、U图像、V图像放置，适用于视频采集图像
//	1：YUV合并格式
//		YUV按照YUYV存放，适用于MJPEG解码图像

#define VW_SA_H  0xfb33		//视频窗口起始地址高寄存器
//D[7:0](R/W，0x00)
#define VW_SA_M  0xfb32		//视频窗口起始地址中寄存器
//D[7:0](R/W，0x00)
#define VW_SA_L  0xfb31		//视频窗口起始地址低寄存器
//D[7:0](R/W，0x00)

#define VW_WIDTH_L  0xfb34		//视频窗口显示内存宽度低寄存器
//D[7:0](R/W，0x00)
#define VW_WIDTH_H  0xfb35		//视频窗口显示内存宽度高寄存器
//D[7:0](R/W，0x00)

//====================
//视频显示窗口编程简介
//===================
/*
1、原理说明
	在视频显示窗口中，显示的图像数据是解码的图像或视频采集端口采集的图像，共分四幅图像，每幅图像自动轮流显示，
图像存储位置固定，图像是按照YUV三个分量分别存储，软件无法干预；在新的显示方式中（VW_CONTROL D[1：0]=11，主要用于MJPEG解码图像
的显示），软件在显示垂直中断服务程序中通过VW_SA_L、VW_SA_M、VW_SA_H设置显示窗口起始地址，VW_WIDTH_L、VW_WIDTH_H设置显示内存宽
度。
	在实际处理上，为了显示图像没有锯齿，需要解码图像至少分成两幅，一副在显示，一幅在解码，以下程序例子就是这样处理的。
2、寄存器
#define VW_CONTROL  0xfb30		//视频窗口控制寄存器
//D0: 手动更新视频窗口；0：自动更新；1：利用显示中断切换视频窗口数据
//D1: 视频窗口格式；
//	0：YUV分离格式；
//		YUV按照Y图像、U图像、V图像放置，适用于视频采集图像
//	1：YUV合并格式
//		YUV按照YUYV存放，适用于MJPEG解码图像

#define VW_SA_H  0xfb33		//视频窗口起始地址高寄存器
//D[7:0](R/W，0x00)
#define VW_SA_M  0xfb32		//视频窗口起始地址中寄存器
//D[7:0](R/W，0x00)
#define VW_SA_L  0xfb31		//视频窗口起始地址低寄存器
//D[7:0](R/W，0x00)

#define VW_WIDTH_L  0xfb34		//视频窗口显示内存宽度低寄存器
//D[7:0](R/W，0x00)
#define VW_WIDTH_H  0xfb35		//视频窗口显示内存宽度高寄存器
//D[7:0](R/W，0x00)
3、编程例子
全局变量
	INT8U have_mjpeg_stream;//
	INT8U dec_sa_sw;
初始化：
	have_mjpeg_stream=0;
	dec_sa_sw=0;//视频显示窗口起始地址切换开关
	videow(VIDEO_DISP_WIN_X,VIDEO_DISP_WIN_XSIZE,VIDEO_DISP_WIN_Y,VIDEO_DISP_WIN_YSIZE);

有解码码流后：
void jpeg_to_rgb1(INT16U bs_length, INT16U bs_addr)
{
	int x=0, y=0;
        INT32U tmp_data;

	while((inportb(DE_J_STATUS)&0x01)==0x01);
	
#if CIF_EN
	outportb(DE_J_MBWIDTH, 0x16);
	outportb(DE_J_MBHEIGHT, 0x12);
#else
	outportb(DE_J_MBWIDTH, 0x28);
	outportb(DE_J_MBHEIGHT, 0x1e);
#endif
//	outport(DE_J_HSIZE_L, 800);
	outport(DE_J_HSIZE_L, 640);
	outport(DE_J_VSIZE_L, 480);

	if(dec_sa_sw){
		tmp_data = (INT32U)640*480*2;
	}
	else{
		tmp_data = 0;
	}
	
	outportb(DE_J_DATA_SA_H, tmp_data>>16);
	outportb(DE_J_DATA_SA_M, tmp_data>>8);
	outportb(DE_J_DATA_SA_L, tmp_data);
	
	outportb(DE_J_CPU_SA_H, 0x17);
 	outportb(DE_J_CPU_SA_M, ((bs_addr)>>8));
	outportb(DE_J_CPU_SA_L, bs_addr);
	
	outportb(DE_J_CPU_LEN_H, 0x00);
 	outportb(DE_J_CPU_LEN_M, (bs_length>>8));
	outportb(DE_J_CPU_LEN_L, bs_length);

	outportb(DE_J_CPU_PN, 1);
	
//	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xef);
	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x8a);
	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xfd);

//	while((inportb(DE_J_STATUS)&0x01)==0x01);

}
	have_mjpeg_stream=1;		
同时：
void interrupt VInt()
{
	INT16U bs_length;
        INT32U tmp_data;
	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xdf);//清除MJPEG中断
	if(have_mjpeg_stream){
		if((inportb(DE_J_STATUS)&0x01)!=0x01){
			outportb(VW_CONTROL, 0x03);
//			outport(VW_WIDTH_L, 800);
			outport(VW_WIDTH_L, 640);


		if(dec_sa_sw){
			tmp_data = (INT32U)640*480*2;
			dec_sa_sw=0;
		}
		else{
			tmp_data = 0;
			dec_sa_sw=1;
		}

			outportb(VW_SA_H, tmp_data>>16);
			outportb(VW_SA_M, tmp_data>>8);
			outportb(VW_SA_L, tmp_data);
			have_mjpeg_stream=0;
		}
	}
	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x20);//使能MJPEG中断
}

*/
//========
//电源管理
//========

#define POWER_CONTROL1  0xfeec		//电源控制寄存器1
//D[7:0](R/W, 0x0c)
//D0：时钟切换开始(进入低功耗模式)
//D1：时钟切换结束(进入低功耗模式)
//D[3：2]：时钟分频选择
//	=11：64分频
//	=10：32分频
//	=01：16分频
//	=00：关闭时钟
//D4：关闭显示时钟（取消）
//D5：关闭MAC时钟（取消）
//D6：关闭视频捕捉时钟（取消）
//D[6:4]: CPU读写SDRAM周期优先级选择，0-7级，0级最高，7级最低
//D7：关闭音频时钟（取消）
//D7：每次SDRAM刷新周期数，0:8个；1:4个
//注：在有LCD显示时，每行启动一次刷新周期，每次8个或4个周期；在无显示时，每40微秒启动一次刷新周期

#define POWER_CONTROL2  0xfeed		//电源控制寄存器2
//D[7:0](R/W)
//D0：关闭MJPEG视频采集时钟
//D1：关闭MJPEG视频编码时钟
//D2：关闭MJPEG视频解码时钟
//D3：关闭显示时钟
//D4：关闭PCM时钟
//D5：关闭MAC时钟
//D6：关闭UART0时钟
//D7：关闭UART1时钟

//==================
//AUDIO CODEC编程要点
//==================
/*
    在SDRAM中，有两个大小可变缓冲区，大小分别是256、512、1024字节，每个缓冲区分成4块，块大小分别是64、128、256字节，这两个缓冲区分别是：一个是PCMI缓冲区，地址是0X190000；
一个是PCMO缓冲区，地址是0X1B0000。
   PCM中断在PCMI缓冲区每增加一块是产生一次中断。
   
一、所用寄存器
1、PCM_CONTROL
//D0：PCM接口位数；0:8位；1:16位
//D1：K8宽度；0：八个周期；1：一个周期
//D2：第一周期数据有效；0：有效；1：跳过
//D3：采样率；0:8K；1:16K
//D4: PCMO内部相连（反馈）
//D[6:5]:PCM数据块大小
//	00:160字节
//	01: 64字节（块号0-3）
//	10:128字节（块号0-3）
//	11:256字节（块号0-3）

2、PCM_STATUS
//D[7:0](R, 0x00)
//D0:	手动中断（取消）
//D1:	功率中断（取消）
//D2:	PCM输入FIFO空；0：不空；1：空（移至D0）
//D3：	PCMO缓冲区满；0：不满；1：满（移至D1）
//D[3:2]: PCMI数据块号
//D[5:4]: PCMO数据块号
//D6: PCMO缓冲区少于一块；0：PCMO缓冲区多于一块；1：PCMO缓冲区少于一块
//D7: PCMO缓冲区空；0:PCMO缓冲区不空；1：PCMO缓冲区空

3、CONTROL_REG
//D3：	PCM输入使能
//D4：	PCM输出使能
//D5：  0：PCM自动处理；1：PCM手动处理
//D6：  PCM中断使能

4、PCMO_CONTROL
//D6: =1：PCMO缓冲区块数加1
//D7: PCMO中断使能；0：屏蔽；1：使能

初始化
	Cont=0xde;//D[6:3]=1011 
 	outportb(CONTROL_REG, Cont);//PCM中断使能;PCM自动处理;PCM输出使能;PCM输入使能

	outportb(PCM_CONTROL, 0x2b);//PCM数据块大小为64字节；采样率为16K；第一周期数据跳过;K8宽度为一个周期；PCM接口位数为16位

	setvect(10, PCMInt);

中断程序
	pcmi_bn=0;
	pcmo_bn=0;
	pcmo_first_time=1;
	pcmo_have_2block=0;

void interrupt PCMInt()//PCMI Int
{
	INT32U Maddrs;
	INT32U Maddrd;
	
	Cont&=0xbf;
	outportb(CONTROL_REG, Cont);//清除PCM中断

	if(pcmo_bn==1)pcmo_have_2block=1;//为了测试，实际上放在主循环
	Maddrs=0x190000UL;
	Maddrd=0x1B0000UL;
//	Maddrs=Maddrs+(64*pcmi_bn);	
//	Maddrd=Maddrd+(64*pcmo_bn);	
	Maddrs=Maddrs+(64*(pcmi_bn&0x3));//为了测试，实际上移至MAC发送缓冲区、或发送出去	
	Maddrd=Maddrd+(64*(pcmo_bn&0x3));	
	pcmi_bn++;
	pcmo_bn++;
	
       	fmem_fmem_dma(Maddrs, Maddrd, 64);//为了测试，实际上移至MAC发送缓冲区、或发送出去
       	if(pcmo_have_2block==1){//为了测试，实际上放在主循环
       		if(pcmo_first_time==1){
			outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)|0x40);//PCMO缓冲区增加一块
			outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)&0xbf);
			pcmo_first_time=0;
       		}
		outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)|0x40);//PCMO缓冲区增加一块
		outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)&0xbf);
	}
	Cont|=0x40;
	outportb(CONTROL_REG, Cont);//使能PCM中断
}

主循环

    读回PCM_STATUS，如D7=1（PCMO缓冲区空），可向PCMO缓冲区最多写入4块数据，每写一块将PCMO_CONTROL D6写入1再写入0；如D7=0（PCMO缓冲区不空）
但D6=1（PCMO缓冲区少于一块），可最多写入三块数据；D6=0（PCMO缓冲区多于一块）但D3=0（PCMO缓冲区不满），可写入1块数据；D3=1（PCMO缓冲区满），
不能再写入数据。
   写入的数据块开始位置由PCM_STATUS D[5:4]决定：=0，可写入块0及3、2、1；=1，可写入块1及0、3、2；=2，可写入块2及1、0、3；=3，可写入块3及2、1、0。
*/

//==================
//低功耗模式编程要点
//==================
/*
	本芯片可通过寄存器设置，将芯片主时钟设置成输入主时钟的16/32/64之一，从而进入低功耗模式，在低功耗模式，使用CLK_50M时钟的所有功能模块的时钟都
降为CLK_50M的16/32/64之一，定时器、UART、SDRAM降频，视频采集、视频编码工作不正常，视频解码速度慢，只有MAC判断电路工作正常，但MAC不能正确将接收
数据写入SDRAM；退出低功耗模式有两种模式，一是CPU认为可以退出的情况如键盘有按键、串口有数据等，CPU通过寄存器结束低功耗模式；另外一种是MAC接收到
如下三种包：符合本机MAC地址的包，多播包、广播ARP包并且与本机IP地址相同。

一、相关寄存器
1、电源控制寄存器
#define POWER_CONTROL1  0xfeec		//电源控制寄存器1
//D[7:0](R/W, 0x0c)
//D0：时钟切换开始(进入低功耗模式)
//D1：时钟切换结束(进入低功耗模式)
//D[3：2]：时钟分频选择
//	=11：64分频
//	=10：32分频
//	=01：16分频
//	=00：关闭时钟
//D4：关闭显示时钟（暂未实现）
//D5：关闭MAC时钟（暂未实现）
//D6：关闭视频捕捉时钟（暂未实现）
//D7：关闭音频时钟（暂未实现）

2、状态寄存器

//状态寄存器
#define STATUS_REG		0xfee0          //状态寄存器
//D1: 低功耗模式, 0：正常工作模式；1：低功耗模式
//D2：	SDRAM准备好
//D3: 网络接收状态及长度数据非法，0：正常；1：非法（该位由网络接收中断使能位为0时清除）

3、MAC寄存器
//本地MAC物理地址
#define PAR0		0xfe10
#define PAR1		0xfe11
#define PAR2		0xfe12
#define PAR3		0xfe13
#define PAR4		0xfe14
#define PAR5		0xfe15

//多播包地址寄存器
#define MAR0		0xfe16（最低字节）
#define MAR1		0xfe17
#define MAR2		0xfe18
#define MAR3		0xfe19
#define MAR4		0xfe1a
#define MAR5		0xfe1b（如6字节MAC地址，最高字节）
#define MAR6		0xfe1c
#define MAR7		0xfe1d（最高字节）

//本地IP地址
#define LIP0		0xfe30
#define LIP1		0xfe31
#define LIP2		0xfe32
#define LIP3		0xfe33

二、编程

1、进入低功耗模式
  	outportb(POWER_CONTROL1, inportb(POWER_CONTROL1)|0x01);
  	outportb(POWER_CONTROL1, inportb(POWER_CONTROL1)&0xfe);
2、退出低功耗模式
  	outportb(POWER_CONTROL1, inportb(POWER_CONTROL1)|0x02);
  	outportb(POWER_CONTROL1, inportb(POWER_CONTROL1)&0xfd);
*/

//===============
//MAC重发编程简介
//===============
/*
    在半双工模式时（通过PHY的MII串行接口读取），将MAC_CONTROL D0设置成0，在MAC检测到总线冲突时，发送32位的阻塞码（TX_JAM_CODE0-3设置），
然后重发最多TX_ATTEMTP_NUM次，直到正确，或者放弃（TX_STATUS D1为1时表示重发失败）。
一、相关寄存器
#define MAC_CONTROL		0xfe00             // MAC控制寄存器
//D[7:0](R/W, 0x01)
//D0：MAC全双工，0：半双工；1：全双工
//D1：MAC速度，0：10M；1：100M

#define TX_STATUS		0xfe01                 // 发送状态寄存器
//D[7:0](R, 0x00)
//D0: 0：无发送；1：发送正在进行
//D1：发送重发错误
//D2: 发送FIFO满错误
//D3：发送FIFO空错误
//D4：发送超时错误
//D5：发送冲突，0：无冲突；1：有冲突
//D6：载波侦听信号，0：无信号；1：有信号

#define TX_BACKOFF_NUM		0xfe1e             //发送BACKOFF数寄存器
//D[7:0](R/W, 0x40)
//D[7:0]：发送BACKOFF数

#define TX_ATTEMTP_NUM		0xfe1f             //发送尝试次数寄存器
//D[7:0](R/W, 0x07)
//D[7:0]：发送尝试次数寄存器

#define TX_JAM_CODE0		0xfe24             //发送阻塞码0寄存器
//D[7:0](R/W, 0x10)
//D[7:0]：发送阻塞码
#define TX_JAM_CODE1		0xfe25             //发送阻塞码1寄存器
//D[7:0](R/W, 0x32)
//D[7:0]：发送阻塞码
#define TX_JAM_CODE2		0xfe26             //发送阻塞码2寄存器
//D[7:0](R/W, 0x54)
//D[7:0]：发送阻塞码
#define TX_JAM_CODE3		0xfe27             //发送阻塞码3寄存器
//D[7:0](R/W, 0x76)
//D[7:0]：发送阻塞码

*/