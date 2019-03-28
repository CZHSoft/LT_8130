/*****************************************************\
*                                                     *
*                                                     *
\*****************************************************/

/*
//========
//�ܽ�˵��
//========
    ��оƬΪ128�ܽŵ�PQFP��װ��VSSΪIO��,VDDΪIO��Դ,VSSIΪ�ں˵�,VDDIΪ�ں˵�Դ;��ԴΪ3.3V.
�ܽ���	   �ܽź�
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

    ��оƬ��107���źŹܽţ������е�һ���ܹܽţ�Ҳ�ж๦�ܹܽţ��๦�ܹܽſ�ͨ���ڲ��Ĵ������á�
    �����ܷ������¡�
    
1��ʱ�Ӹ�λ��3���ܽ�
	XTLI(����)		��������
	XTLO(���)		�������
	RSTN�����롢����Ч��	��λ����

2��SPI FLASH�ӿڣ�����Ƭ�ӿڣ���4���ܽ�
    ��оƬ���һƬ����256K�ֽڵ�SPI FLASH��ΪCPU������ʾͼ������ݵĴ洢����
    ����λ��RSTNΪ��ʱ��оƬ����ӵ�һƬSPI FLASH����256KB������8088 CPU 1MB�ڴ�ռ�ĸ�256KB��Ȼ���FFFF0H��������
	SPI_SCK(���)		SPI����ʱ��	
	SPI_MOSI(���)		SPI�����������
	SPI_SS(���������Ч) 	SPIƬѡ
	SPI_MISO(����)		SPI����������

3��SDRAM�ӿڣ�38���ܽ�
    ��оƬ���һƬ8M�ֽڵ�SDRAM��ΪCPU���ݳ�����Ƶ���롢��Ƶ���롢��ʾ�����ݵĴ洢����
	CSN(���������Ч)	SDRAMƬѡ�ź�
	RASN(���������Ч)	SDRAM��ѡͨ�ź�
	CASN(���������Ч)	SDRAM��ѡͨ�ź�
	WEN(���������Ч)	SDRAMд�ź�
	BA11-0(���)		SDRAM��ַ
	BANK1-0(���)	SDRAM bank��ַ
	DQ15-0(����/���)	SDRAM����
	DQML(���������Ч)	SDRAM�������ε��ź�
	DQMH(���������Ч)	SDRAM�������θ��ź�
	CLKO(���)		SDRAMʱ���������XTLIͬƵ�ʣ�
	CKE(���������Ч)	SDRAMʱ��ʹ���ź�

4����Ƶ����ӿڣ�21���ܽ�
    ��оƬ���������Һ����ʾ��ģ�顣
	HSYNC�������		��Ƶˮƽͬ���ź�
	VSYNC/DE�������	��Ƶ��ֱͬ������ʾʹ���ź�
	R7-2�������		��ƵR����
	G7-2�������		��ƵG����
	B7-2�������		��ƵB����
	DCLK�������		��Ƶ��ʱ��

5��MAC�ӿڣ�14���ܽ�
    ��оƬ��Ӵ���MII�ӿڵ���̫��PHY����DM9161�ȡ�
	RX_DATA3-0�����룩	MAC��������
	RX_DV�����룩		MAC��������
	CRS�����룩		MAC�����ز����
	RX_CLK�����룩		MAC����ʱ��
	TX_CLK�����룩		MAC����ʱ��
	TX_DATA3-0�������	MAC��������
	TX_EN�������		MAC����ʹ��
	ע�⣬MII�ź�RX_ER��TX_ER��COLLδ���ӣ�PHY��TX_ER�ӵأ�RX_ER��COLL���ա�

6����Ƶ����ӿڣ�9���ܽ�
    ��оƬ����Ӵ�ITU656��BT656���ӿڵ�PAL/NTSC������������Ƶ�����ͼ����ʾ����ʾ������Ƶ�����ϡ�
	PCLK�����룩			��Ƶ����ʱ��
	VPO7-0�����룬GPIOC7-0��	��Ƶ��������

7����Ƶ�ӿڣ�5���ܽ�
    ��оƬ��������MC14581��������ƵPCM CODECоƬ��
    оƬ���ṩ������������32��16λFIFO��IIS����������IIS������δʹ��ʱ����Ƶ�ӿڽ�ΪPCM�źţ�IR3���⣩��
��IIS������ʹ��ʱ����Ƶ�ӿڽ�ת��ΪIIS�źš�
	CLK_25M�������		(��ΪPCM��MCLK)
	BCLK(���)		PCMλʱ��
	FS(���)			PCM֡ͬ��
	PCMO(���)		PCM�����������DR��
	PCMI(����)		PCM�������루��DT��
	IR3(����/�����GPIOB2)  	�ⲿ�ж����루�ж�3��

8��UART�ӿڣ�4���ܽ�
    ��оƬ�ṩ����8��FIFO��UART��
	RXD1�����룩		����1��������
	TXD1�������		����1��������
	RXD2�����룬GPIOB6��		����2��������
	TXD2�������GPIOB7��		����2��������

9����������1���ܽ�
    ��оƬ������������������������0��ΪPWM�����������PWM0�źţ�������1�����ڲ����Ź�ʹ�ã�������2��Ϊ��ʱ��ʹ�á�
	PWM0�������GPIOB3��		�����ȵ������

10��ͨ�����������GPIO����8���ܽ�
    ��оƬ������5��GPIO���ֱ�ΪGPIOA��GPIOB��GPIOC�����г���GPIOA��ר�ùܽ����⣬����GPIO�����Ϲ���
�ܽ�ͨ���Ĵ���ѡ���á�
	GPIOA7-0�����������		ͨ���������A7-0

	GPIOB7����TXD2���ã�	ͨ���������B7
	GPIOB6����RXD2���ã�	ͨ���������B6
	GPIOB5���ޣ�		ͨ���������B5
	GPIOB4���ޣ�		ͨ���������B4
	GPIOB3����PWM0���ã�	ͨ���������B3
	GPIOB2����IR3����)   	ͨ���������B2
	GPIOB1����)		ͨ���������B1
	GPIOB0����)		ͨ���������B0

	GPIOC7-0����VPO���ã�		ͨ���������C7-0

11�����CPU�ӿڣ���ռ��ר�ùܽ�
    ��оƬ�ں�16λCPU����CPU����������ʱ�رգ��ⲿCPUͨ��CPU���߿��Ʊ�оƬ�������������£�DQ0��������
���裬��û�����4.7K�������������£��ڸ�λ�ڼ�ùܽż��Ϊ0���ڲ�CPU�������������4.7K�������������£��ڸ�
λ�ڼ�ùܽż��Ϊ1���ڲ�CPU�رգ��ⲿCPUͨ�����¹ܽ�д��Ͷ����ڲ��Ĵ�����
    IR3--ALE��ַ�������壨���룬����Ч��
    PWM0--WRNд���壨���룬����Ч��
    RXD2--RDN�����壨���룬����Ч��
    TXD2--READY�źţ����������Ч��
    GPIOA--DATA����������������������

    ��һ��ALE��IR3������ͨ��DATA����16λ��ַ�еĸ�8λ��ַ���ڶ���ALE��IR3������ͨ��DATA����16λ��ַ�еĵ�8λ��ַ��
Ȼ����WRN��PWM0�����壬��ͨ��DATAд����Ӧ��ַ�ļĴ�������RDN��RXD2�����壬��DATA������Ӧ��ַ�ļĴ�����ֵ������д
������SDRAM����������Ҫ�ж�READY��TXD2���Ƿ�Ϊ1����ȥ��WRN��RDN���塣

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

//#define SCREEN_WIDTH		800            //��Ļ���
//#define SCREEN_HEIGHT		480            //��Ļ�߶�

//�ֽ�д�Ĵ�����
/*
    ����Ĵ�������CPUд�ڴ��һ�ַ�ʽ������ʼ��ַд���ֽ�д��ַ�Ĵ����󣬸÷�ʽÿд�ֽ�д���ݼĴ���һ�Σ�
���üĴ���һ���ֽڵ�����д���ڴ棬ͬʱ�Զ�����ַ��һ��׼��������һ�����ݡ�
*/
#define BYTE_WR_LOW_ADD		0xfee2           //�ֽ�д�͵�ַ�Ĵ�����0-7��
#define BYTE_WR_HIGH_ADD	0xfee3           //�ֽ�д�ߵ�ַ�Ĵ�����8-15��
#define BYTE_WR_MOST_ADD	0xfee4           //�ֽ�д��ߵ�ַ�Ĵ�����16-23��
#define BYTE_WR_DATA		0xfee5           //�ֽ�д���ݼĴ���

//�ֽڶ��Ĵ�����
/*
    ����Ĵ�������CPU���ڴ��һ�ַ�ʽ������ʼ��ַд���ֽڶ���ַ�Ĵ����󣬸÷�ʽÿ���ֽڶ����ݼĴ���һ�Σ�
��ǰ�ֽڶ���ַ�Ĵ���ָ��ĵ�ַ���ݶ��أ�ͬʱ�Զ�����ַ��һ��׼��������һ�����ݡ�
*/
#define BYTE_RD_LOW_ADD		0xfee6            //�ֽڶ��͵�ַ�Ĵ�����0-7��                            
#define BYTE_RD_HIGH_ADD	0xfee7            //�ֽڶ��ߵ�ַ�Ĵ�����8-15��                           
#define BYTE_RD_MOST_ADD	0xfee8            //�ֽڶ���ߵ�ַ�Ĵ�����16-23?
#define BYTE_RD_DATA		0xfee9            //�ֽڶ����ݼĴ���    

//�ַ�д�Ĵ�����
/*
    ����Ĵ�������CPUд�ڴ��һ�ַ�ʽ������ʼ��ַ����16�ֽ�Ϊ�磬����4λ��ַΪ0��д���ַ�д��ַ�Ĵ����󣬸÷�ʽÿд�ַ�д���ݼĴ���һ�Σ�
���üĴ���һ���ֽڵ����ݰ�λ�����·�ʽд���ڴ��8���֣�ͬʱ�Զ�����ַ��16��׼��������һ�����ݣ�
    0���ڴ���д���ַ������Ĵ���
    1���ڴ���д���ַ�ǰ���Ĵ���
*/
#define FONT_WR_LOW_ADD		0xfed0           //�ַ�д�͵�ַ�Ĵ�����0-7��
#define FONT_WR_HIGH_ADD	0xfed1           //�ַ�д�ߵ�ַ�Ĵ�����8��15��
#define FONT_WR_MOST_ADD	0xfed2           //�ַ�д��ߵ�ַ�Ĵ�����16��23�� 
#define FONT_WR_DATA		0xfed3           //�ַ�д���ݼĴ���
#define FGND_LOW		0xfed4           //�ַ�ǰ�����ֽڼĴ���
#define FGND_HIGH		0xfed5           //�ַ�ǰ�����ֽڼĴ���
#define BGND_LOW		0xfed6           //�ַ��������ֽڼĴ���   
#define BGND_HIGH		0xfed7           //�ַ��������ֽڼĴ���

//MAC���ͣ�ͨ�ð���--�������ǳ�����Ƶ����PCM�������ͨ�ð�
/*
    MAC���͵����ݰ�����SDRAM�з��Ͱ���ʼ��ַ�Ĵ�������ʼֵΪ0x1a0040��ָ��ĵ�ַ��ʼ��2K�ֽڽ��ջ�������
����Ҫ���͵�����д��û������󣬽�������д�뷢�Ͱ����ȼĴ�����Ȼ��1д�뷢�Ϳ�ʼ�Ĵ���D0����鷢��״̬
�Ĵ���D0λ���ȴ����ͽ�����

//��ʼ��
	setvect(9, MACTXInt);
//���Ͱ�
	while(inportb(TX_STATUS)&0x01);

	outportb(TX_SA2_H, 0x1a);
	outportb(TX_SA2_M, 0x00);
	outportb(TX_SA2_L, 0x40);

	outport(TXPL_LOW, 182);
	outport(TXPL2_LOW, add_data_length);
	outportb(TX_START, 0xd);//���Ͳ�ʹ��MAC�����ж�

//�����жϷ������

void interrupt MACTXInt()
{
	outportb(TX_START, 0x02);//���MAC�����ж�
	//
	//�жϴ���
	//
	outport(TXPL_LOW, 182);
	outport(TXPL2_LOW, add_data_length);
	outportb(TX_START, 0xf);//����ʹ��MAC�����ж�
	add_data_length++;
}

*/
#define TX_START		0xfe08                 // ���Ϳ�ʼ�Ĵ���
//D0�����Ϳ�ʼλ
//D1��CPU�������뿪ʼλ
//D2��MAC�����ж�ʹ��
//D3: 0:���Ͱ�Ϊһ�����ݣ�1�����Ͱ�Ϊ��������
//D4�����Ͱ����ݵڶ���64KB����()
//D5��MJPEG����׼�����ж�ʹ��()
//D6��
//D7��H264����׼�����ж�ʹ��()

#define TX_STATUS		0xfe01                 // ����״̬�Ĵ���
//D[7:0](R, 0x00)
//D0: 0���޷��ͣ�1���������ڽ���
//D1�������ط�����
//D2: ����FIFO������
//D3������FIFO�մ���
//D4�����ͳ�ʱ����
//D5�����ͳ�ͻ��0���޳�ͻ��1���г�ͻ
//D6���ز������źţ�0�����źţ�1�����ź�

//TX Packet Length Register
//#define TXPL_LOW		0xfefc                 // ���Ͱ����ȼĴ������ֽ�
//#define TXPL_HIGH		0xfefd                 // ���Ͱ����ȼĴ������ֽ�
#define TXPL_LOW		0xfe06                 // ���Ͱ���һ�����ݳ��ȼĴ������ֽ�
#define TXPL_HIGH		0xfe07                 // ���Ͱ���һ�����ݳ��ȼĴ������ֽ�
/*
	�üĴ����ڶ��η���ʱ����Ϊż��
*/

#define TXPL2_LOW		0xfe20                 // ���Ͱ��ڶ������ݳ��ȼĴ������ֽ�
#define TXPL2_HIGH		0xfe21                 // ���Ͱ��ڶ������ݳ��ȼĴ������ֽ�

//���Ͱ���һ��������ʼ��ַ�Ĵ�������ʼֵΪ0x1a0040��
#define TX_SA_L		0xfed8	//���Ͱ���һ��������ʼ��ַ���ֽ�
#define TX_SA_M		0xfed9	//���Ͱ���һ��������ʼ��ַ���ֽ�
#define TX_SA_H		0xfeda	//���Ͱ���һ��������ʼ��ַ���ֽ�
/*
	�üĴ����������λΪ0������һ��������16�ֽ�Ϊ��
*/

//���Ͱ��ڶ���������ʼ��ַ�Ĵ���
#define TX_SA2_L	0xfedb	//���Ͱ��ڶ���������ʼ��ַ���ֽ�
#define TX_SA2_M	0xfedc	//���Ͱ��ڶ���������ʼ��ַ���ֽ�
#define TX_SA2_H	0xfedd	//���Ͱ��ڶ���������ʼ��ַ���ֽ�
/*
	�üĴ����������λΪ0�����ڶ���������16�ֽ�Ϊ��
*/

//MAC���գ�ͨ�ð���--�������ǳ�����Ƶ����PCM�������ͨ�ð�
/*
    MAC���յ����ݰ�����SDRAM��ַ0x0c0000��ʼ��64K�ֽڽ��ջ�����,ÿ��ռ2K�ֽ�,��������32����,32��2K�ֽ�ѭ��ʹ��;
    2K�ֽڵ����˸��ֽڸ�ʽ����:
    ���ֽ������ֽ�,���ֽ������ֽ�,��״̬���ֽڣ�RX_PACKET_STATUS_L��,��״̬���ֽڣ�RX_PACKET_STATUS_H��,4���ֽڱ���.
    ÿ���յ�һ�����ͻ����һ���жϡ�

//��ʼ��
	outportb(RX_CONTROL, 0x03);//MAC�����ж�ʹ��
	setvect(8, MACRXInt);

//�����ж�
void interrupt MACRXInt()
{
	outportb(RX_CONTROL, 0x01);//���MAC�����ж�
	//
	//�жϴ���
	//
	outportb(RX_CONTROL, 0x03);//����ʹ��MAC�����ж�
}

*/
#define RX_PACKET_NUM		0xfe02             // ���ܰ����Ĵ���
//D[7:0]����ǰ���հ���

#define MAC_CONTROL		0xfe00             // MAC���ƼĴ���
//D[7:0](R/W, 0x01)
//D0��MACȫ˫����0����˫����1��ȫ˫��
//D1��MAC�ٶȣ�0��10M��1��100M

#define TX_BACKOFF_NUM		0xfe1e             //����BACKOFF���Ĵ���
//D[7:0](R/W, 0x40)
//D[7:0]������BACKOFF��

#define TX_ATTEMTP_NUM		0xfe1f             //���ͳ��Դ����Ĵ���
//D[7:0](R/W, 0x07)
//D[7:0]�����ͳ��Դ����Ĵ���

#define TX_JAM_CODE0		0xfe24             //����������0�Ĵ���
//D[7:0](R/W, 0x00)
//D[7:0]������������
#define TX_JAM_CODE1		0xfe25             //����������1�Ĵ���
//D[7:0](R/W, 0x00)
//D[7:0]������������
#define TX_JAM_CODE2		0xfe26             //����������2�Ĵ���
//D[7:0](R/W, 0x00)
//D[7:0]������������
#define TX_JAM_CODE3		0xfe27             //����������3�Ĵ���
//D[7:0](R/W, 0x00)
//D[7:0]������������

#define RX_CONTROL		0xfe03             // ���տ��ƼĴ���
//D0������ʹ��
//D1�������ж�ʹ��
//D2������CRC�����
//D3�������㲥��
//D4�������ಥ��
//D[6:5]: MAC���ջ�������С
//	00: 64KB(RX_PACKET_NUMΪ5λ)
//	01: 128KB(RX_PACKET_NUMΪ6λ)
//	10: ����
//	11: 256KB(RX_PACKET_NUMΪ7λ)
//D7��0�����н��յ��İ���������ջ�������1��MAC��ַƥ�䡢�㲥���ಥ���Ž�����ջ�����

#define RX_PACKET_STATUS_L	0xfe04             // ���հ�״̬�Ĵ�����
//D0�������ж�״̬��0���޽����жϣ�1���н����жϣ�(ԭ����,���޸�Ϊ)
//D0: MAC��ַƥ�䣬0����ƥ�䣻1��ƥ��
//D1��CRC����
//D2�����ֽڴ���
//D3��MII�ܽ�RX_ERΪ1
//D4���㲥��
//D5���ಥ��
//D6��PCM��
//D7����Ƶ��

#define RX_PACKET_STATUS_H	0xfe05             // ���հ�״̬�Ĵ�����
//D0��IP��
//D1��ARP��
//D2��RARP����ԭ���壩
//D2��ARP����IP��ַƥ��
//D3��TCP��
//D4��UDP��
//D5��ICMP��
//D6��IGMP��
//D7��OSPF����ԭ���壩
//D7�����ճ�ͻ��0���޳�ͻ��1���г�ͻ

//CPU���������Ĵ���
#define BS_CPU_PN_L		0xfe0a
#define BS_CPU_PN_H		0xfe0b

//CPU����������ʼ��ַ�Ĵ���
#define BS_CPU_SA_L		0xfe0c
#define BS_CPU_SA_M		0xfe0d
#define BS_CPU_SA_H		0xfe0e

//����MAC�����ַ
#define PAR0		0xfe10
#define PAR1		0xfe11
#define PAR2		0xfe12
#define PAR3		0xfe13
#define PAR4		0xfe14
#define PAR5		0xfe15

//�ಥ����ַ�Ĵ���
#define MAR0		0xfe16//������ֽڣ�
#define MAR1		0xfe17
#define MAR2		0xfe18
#define MAR3		0xfe19
#define MAR4		0xfe1a
#define MAR5		0xfe1b//����6�ֽ�MAC��ַ������ֽڣ�
#define MAR6		0xfe1c
#define MAR7		0xfe1d//������ֽڣ�

//����IP��ַ
#define LIP0		0xfe30
#define LIP1		0xfe31
#define LIP2		0xfe32
#define LIP3		0xfe33

#define PCMI_SEQ_NUM_L		0xfe30
#define PCMI_SEQ_NUM_H		0xfe31

#define HTOTAL_L		0xfe60              //ˮƽ�������ֽ�
#define HTOTAL_H		0xfe61              //ˮƽ�������ֽ�
#define HDISP_L			0xfe62              //ˮƽ��ʾ���ֽ�
#define HDISP_H			0xfe63              //ˮƽ��ʾ���ֽ�
#define HSYNC_START_L		0xfe64          //ˮƽͬ����ʼ���ֽ�
#define HSYNC_START_H		0xfe65          //ˮƽͬ����ʼ���ֽ�
#define HSYNC_SIZE		0xfe66            //ˮƽͬ����С
#define HBACKPORCH		0xfe67            //ˮƽ��ɨ

#define VTOTAL_L		0xfe68              //��ֱ�������ֽ�
#define VTOTAL_H		0xfe69              //��ֱ�������ֽ�
#define VDISP			0xfe6a                //��ֱ��ʾ
#define VSYNC_START		0xfe6c            //��ֱ��ʼ
#define VSYNC_SIZE		0xfe6e            //��ֱ��С
#define VBACKPORCH		0xfe6f            //��ֱ��ɨ

#define WH_START_L		0xfe70            //��Ƶ����ˮƽ��ʾ��ʼ���ֽ�
#define WH_START_H		0xfe71            //��Ƶ����ˮƽ��ʾ��ʼ���ֽ�
#define WH_SIZE_L		0xfe72              //��Ƶ����ˮƽ��С���ֽ� 
#define WH_SIZE_H		0xfe73              //��Ƶ����ˮƽ��С���ֽ�  
#define WV_START		0xfe74              //��Ƶ���ڴ�ֱ��ʼ
#define WV_SIZE			0xfe76              //��Ƶ���ڴ�ֱ��С

#define SCALE_SEL		0xfe78              //��ʾ����
//D[2:0]��ˮƽѹ��
//	000��8ѡ8
//	001��8ѡ7
//	010��8ѡ6
//	011��8ѡ5
//	100��8ѡ4
//D[5:3]����ֱѹ��
//	000��8ѡ8
//	001��8ѡ7
//	010��8ѡ6
//	011��8ѡ5
//	100��8ѡ4
//D6��ˮƽ�Ŵ�
//D7����ֱ�Ŵ�

#define DISPLAY_MODE		0xfe79          //��ʾģʽ
//D[2:0]: ʱ��ѡ��  	111��DCLK��CLK 8��Ƶ
//			110��DCLK��CLK 4��Ƶ
//			101��DCLK��CLK 2��Ƶ
//D3��	VSYNC��ΪDE
//D4��  VSYNC��ΪCS
//D5��  DCLK����ѡ��
//D6��  HSYNC����ѡ��
//D7��  VSYNC����ѡ��

#define rx_gen_SA		0x180000
#define tx_gen_SA		0x1a0040
#define pcmi_tx_haeder_SA	0x1a0000

//rgb_rd_SA 2010-02-08 �޸� 
//#define rgb_rd_SA		0x3c0000
#define rgb_rd_SA		0x400000
#define pcmi_SA			0x190000
#define pcmo_SA			0x1b0000


//��ʾ�ڴ���ʼ��ַ�Ĵ������ڴ��ȼĴ���   2010-02-08 ���
#define DM_START_L		0xfe7a
#define DM_START_H		0xfe7b
#define DM_START_M		0xfe7c
#define DM_WIDTH_L		0xfe7e
#define DM_WIDTH_H		0xfe7f

//���ƼĴ���
#define CONTROL_REG		0xfeea          //���ƼĴ���
//D0:	��ʾʹ��
//D1��  ��ʾ����
//D2��  ����ѡ��,0:�����Ĵ���ֵ;1:SDRAM���ֲ���
//D3��	PCM����ʹ��
//D4��	PCM���ʹ��
//D5��  0��PCM�Զ�����1��PCM�ֶ�����
//D6��  PCM�ж�ʹ��
//D7��  ��������ʹ��

//���ƼĴ���2
#define CONTROL_REG2		0xfeee          //���ƼĴ���2
//D0:	�ַ�дʱ��ֻд�Ҷȣ�0�������ַ�д��1���Ҷ��ַ�д
//D1: ��ʾʱ�ӱ�����0��������1����25MHZ��ʾʱ��ʱ����ʱ�ӱ���1/8�������㹻ʱ�������Ƶ�ɼ�
//D2��ÿ��ɫ8λ��ʾ��0��6λ��1��8λ
//D3: 64KB�ڴ洰��ʹ�ܣ�0��CPU�ڴ��A����ԭֵ��1��64KB CPU�ڴ��Aӳ�䵽8M�洢����һ64KB�洢����λ���ɼĴ���MEMORY_WINDOW_BASE_H������
//D4���ر�PTC0ʱ��
//D5���ر�PTC1ʱ��
//D6���ر�PTC2ʱ��
//D7���ر�SPIʱ��

//�ڴ洰�ڻ���ַ�߼Ĵ���
#define MEMORY_WINDOW_BASE_H		0xfeef
//D[6:0]:(R/W, 0x7a)
//8MB�洢����ַ��7λ����16λ��CPU��16λ��ַ����

//PCM���ݼĴ���
#define PCM_DATA_REG		0xfeeb          //PCM���ݼĴ���

/*
//��ʼ��
	setvect(10, PCMInt);
	outportb(CONTROL_REG, 0xb9);//���PCM�ж�
	outportb(CONTROL_REG, 0xf9);//PCM�ж�ʹ��

//�жϷ������
void interrupt PCMInt()
{
	outportb(CONTROL_REG, 0xb9);//���PCM�ж�
	outportb(PCM_DATA_REG, add_data_length);
	outportb(CONTROL_REG, 0xf9);//ʹ��PCM�ж�
	add_data_length++;
}
*/

//״̬�Ĵ���
#define STATUS_REG		0xfee0          //״̬�Ĵ���
//D1: �͹���ģʽ, 0����������ģʽ��1���͹���ģʽ
//D2��	SDRAM׼����

//λ�飨BitBlk������
#define BITBLK_CONTROL_REG	0xfe80	//λ����ƼĴ���
//D0:	λ�鴫������
//D1��	͸��ʹ��
//D2��͸����ʽѡ��0���������λΪ��͸����1������ȫ��͸��
//D3: 0��Դ��SPI FLASH��1��Դ��SDRAM
//D4: 0��SDRAM��SPI FLASH֮�䴫��; 1��SDRAM�ڲ�����
//D5: SPI_sel, 0: SPI FLASH��SPI FLASH������ͱ�����; 1: SPI������
//D6: λ�鴫������ж�(cpu�жϺ�21��ʹ�ܣ�0�����λ�鴫������жϣ�1��λ�鴫������ж�ʹ��

#define BITBLK_STATUS_REG	0xfe81	//λ��״̬�Ĵ���
//D0��λ�鴫�����

#define BITBLK_SOURCE_WIDTH_L	0xfe82	//λ��Դ�ڴ��ȵ�
#define BITBLK_SOURCE_WIDTH_H	0xfe83	//λ��Դ�ڴ��ȸ�
#define BITBLK_SA_L	0xfe84	//λ��Դ��ַ��
#define BITBLK_SA_M	0xfe85	//λ��Դ��ַ��
#define BITBLK_SA_H	0xfe86	//λ��Դ��ַ��
#define BITBLK_DESTINATION_WIDTH_L	0xfe88	//λ��Ŀ���ڴ��ȵ�
#define BITBLK_DESTINATION_WIDTH_H	0xfe89	//λ��Ŀ���ڴ��ȸ�
#define BITBLK_DA_L	0xfe8c	//λ��Ŀ�ĵ�ַ��
#define BITBLK_DA_M	0xfe8d	//λ��Ŀ�ĵ�ַ��
#define BITBLK_DA_H	0xfe8e	//λ��Ŀ�ĵ�ַ��
#define BITBLK_X_SIZE_L	0xfe90	//λ��X�ߴ�ͣ�SPI FLASH��SDRAM�����ֽ����ͣ�
#define BITBLK_X_SIZE_H	0xfe91	//λ��X�ߴ�ߣ�SPI FLASH��SDRAM�����ֽ����У�
#define BITBLK_Y_SIZE_L	0xfe92	//λ��Y�ߴ�ͣ�SPI FLASH��SDRAM�����ֽ����ߣ�
#define BITBLK_Y_SIZE_H	0xfe93	//λ��Y�ߴ��

//UART0
#define UART0_TX_STATUS 0xff80      //UART0����״̬�Ĵ��� 
//D0: ����д����
//D1: �Զ�������ʶ�����

#define UART0_TX_DATA   0xff81      //UART0�������ݼĴ���
#define UART0_RX_CTRL   0xff82      //UART0���տ��ƼĴ���
//D[3:0] ����FIFO�ֽڼ���
//D4:    ����FIFO������ָʾ(����FIFO��ָʾ)
//D5:    �ж�״ָ̬ʾ
//D6:    ����ʹ��
//D7:    �ж�ʹ��
#define UART0_RX_DATA       0xff83  //UART0�������ݼĴ���
#define UART0_DIVIDER_LOW   0xff84  //UART0��Ƶϵ����8λ
#define UART0_DIVIDER_HIGH  0xff85  //UART0��Ƶϵ����8λ
#define UART0_ABR_LOW   0xff86  //UART0�Զ������ʷ�Ƶϵ����8λ
#define UART0_ABR_HIGH  0xff87  //UART0�Զ������ʷ�Ƶϵ����8λ

//UART1
#define UART1_TX_STATUS 0xff88      //UART0����״̬�Ĵ��� 
//D0: ����д����
//D1: �Զ�������ʶ�����

#define UART1_TX_DATA   0xff89      //UART0�������ݼĴ���
#define UART1_RX_CTRL   0xff8a      //UART0���տ��ƼĴ���
//D[3:0] ����FIFO�ֽڼ���
//D4:    ����FIFO������ָʾ(����FIFO��ָʾ)
//D5:    �ж�״ָ̬ʾ
//D6:    ����ʹ��
//D7:    �ж�ʹ��
#define UART1_RX_DATA       0xff8b  //UART0�������ݼĴ���
#define UART1_DIVIDER_LOW   0xff8c  //UART0��Ƶϵ����8λ
#define UART1_DIVIDER_HIGH  0xff8d  //UART0��Ƶϵ����8λ
#define UART1_ABR_LOW   0xff8e  //UART0�Զ������ʷ�Ƶϵ����8λ
#define UART1_ABR_HIGH  0xff8f  //UART0�Զ������ʷ�Ƶϵ����8λ

//PTC0
/*
	��PTC��Ҫ����LCD���⡣
*/
//�Ĵ���
#define PTC0_HRC0   0xffa0          // PWM/��ʱ��/��ʱ��(PTC)�߲ο�/����Ĵ���0
#define PTC0_HRC1   0xffa1          // PWM/��ʱ��/��ʱ��(PTC)�߲ο�/����Ĵ���1
#define PTC0_HRC2   0xffa2          // PWM/��ʱ��/��ʱ��(PTC)�߲ο�/����Ĵ���2
//PTC0_HRC0-2�ڲο�ģʽ�£���PWM�������Ϊ��
#define PTC0_LRC0   0xffa3          // PWM/��ʱ��/��ʱ��(PTC)�Ͳο�/����Ĵ���0
#define PTC0_LRC1   0xffa4          // PWM/��ʱ��/��ʱ��(PTC)�Ͳο�/����Ĵ���1
#define PTC0_LRC2   0xffa5          // PWM/��ʱ��/��ʱ��(PTC)�Ͳο�/����Ĵ���2
//PTC0_LRC0-2�ڲο�ģʽ�£���PWM�������Ϊ��

#define PTC0_CTRL0  0xffa6          // PWM/��ʱ��/��ʱ��(PTC)���ƼĴ���0
//D0: R/W, EN    1: PTC0_CNTR����������0��PTC0_CNTR������
//D1: R/W, ECLK  1: PTC_ECGT��ΪPTC0_CNTR��ʱ�ӣ�0��ϵͳʱ����ΪPTC0_CNTR��ʱ��
//D2: R/W, NEC   1: PTC_ECGT�½�����Ч���͵�ƽʱ�ſؼ�����0: PTC_ECGT��������Ч���͵�ƽʱ�ſؼ���
//D3: R/W, OE    1: PWM���ʹ��
//D4: R/W, SINGLE   1: ��ʱ��PTC0_CNTRֻ����һ�Σ����ӵ�PTC0_LRC��0����ʱ��PTC0_CNTR���ӵ�PTC0_LRC���Զ����¿�ʼ����
//D5: R/W, INTE  1: PTC��PTC0_CNTR����PTC0_LRC����PTC0_HRCʱ�����жϣ�0���������ж�
//D6: R/W, INT  1: INT �жϵĵ�ǰ״̬�����豸д��1������ж�
//D7: R/W, CNTRRST  1: ��������λ��0����������������

#define PTC0_CTRL1  0xffa7          // PWM/��ʱ��/��ʱ��(PTC)���ƼĴ���1
//D0: R/W CAPTE  1����������ģʽ��0������ģʽ�ر� 

//PTC1
/*
    ��PTC��Ҫ���ڿ��Ź���
*/
//�Ĵ���
#define PTC1_HRC0   0xffa8          // PWM/��ʱ��/��ʱ��(PTC)�߲ο�/����Ĵ���0
#define PTC1_HRC1   0xffa9          // PWM/��ʱ��/��ʱ��(PTC)�߲ο�/����Ĵ���1
#define PTC1_HRC2   0xffaa          // PWM/��ʱ��/��ʱ��(PTC)�߲ο�/����Ĵ���2
//PTC1_HRC0-2�ڲο�ģʽ�£���PWM�������Ϊ��
#define PTC1_LRC0   0xffab          // PWM/��ʱ��/��ʱ��(PTC)�Ͳο�/����Ĵ���0
#define PTC1_LRC1   0xffac          // PWM/��ʱ��/��ʱ��(PTC)�Ͳο�/����Ĵ���1
#define PTC1_LRC2   0xffad          // PWM/��ʱ��/��ʱ��(PTC)�Ͳο�/����Ĵ���2
//PTC1_LRC0-2�ڲο�ģʽ�£���PWM�������Ϊ��

#define PTC1_CTRL0  0xffae          // PWM/��ʱ��/��ʱ��(PTC)���ƼĴ���0
//D0: R/W, EN    1: PTC1_CNTR����������0��PTC1_CNTR������
//D1: R/W, ECLK  1: PTC_ECGT��ΪPTC1_CNTR��ʱ�ӣ�0��ϵͳʱ����ΪPTC1_CNTR��ʱ��
//D2: R/W, NEC   1: PTC_ECGT�½�����Ч���͵�ƽʱ�ſؼ�����0: PTC_ECGT��������Ч���͵�ƽʱ�ſؼ���
//D3: R/W, OE    1: PWM���ʹ��
//D4: R/W, SINGLE   1: ��ʱ��PTC1_CNTRֻ����һ�Σ����ӵ�PTC1_LRC��0����ʱ��PTC1_CNTR���ӵ�PTC1_LRC���Զ����¿�ʼ����
//D5: R/W, INTE  1: PTC��PTC1_CNTR����PTC1_LRC����PTC1_HRCʱ�����жϣ�0���������ж�
//D6: R/W, INT  1: INT �жϵĵ�ǰ״̬�����豸д��1������ж�
//D7: R/W, CNTRRST  1: ��������λ��0����������������

#define PTC1_CTRL1  0xffaf          // PWM/��ʱ��/��ʱ��(PTC)���ƼĴ���1
//D0: R/W CAPTE  1����������ģʽ��0������ģʽ�ر� 

//PTC2
/*
    ��PTC��Ҫ����ͨ�ö�ʱ����
*/
//�Ĵ���
#define PTC2_HRC0   0xffc0          // PWM/��ʱ��/��ʱ��(PTC)�߲ο�/����Ĵ���0
#define PTC2_HRC1   0xffc1          // PWM/��ʱ��/��ʱ��(PTC)�߲ο�/����Ĵ���1
#define PTC2_HRC2   0xffc2          // PWM/��ʱ��/��ʱ��(PTC)�߲ο�/����Ĵ���2
//PTC2_HRC0-2�ڲο�ģʽ�£���PWM�������Ϊ��
#define PTC2_LRC0   0xffc3          // PWM/��ʱ��/��ʱ��(PTC)�Ͳο�/����Ĵ���0
#define PTC2_LRC1   0xffc4          // PWM/��ʱ��/��ʱ��(PTC)�Ͳο�/����Ĵ���1
#define PTC2_LRC2   0xffc5          // PWM/��ʱ��/��ʱ��(PTC)�Ͳο�/����Ĵ���2
//PTC2_LRC0-2�ڲο�ģʽ�£���PWM�������Ϊ��

#define PTC2_CTRL0  0xffc6          // PWM/��ʱ��/��ʱ��(PTC)���ƼĴ���0
//D0: R/W, EN    1: PTC2_CNTR����������0��PTC2_CNTR������
//D1: R/W, ECLK  1: PTC_ECGT��ΪPTC2_CNTR��ʱ�ӣ�0��ϵͳʱ����ΪPTC2_CNTR��ʱ��
//D2: R/W, NEC   1: PTC_ECGT�½�����Ч���͵�ƽʱ�ſؼ�����0: PTC_ECGT��������Ч���͵�ƽʱ�ſؼ���
//D3: R/W, OE    1: PWM���ʹ��
//D4: R/W, SINGLE   1: ��ʱ��PTC2_CNTRֻ����һ�Σ����ӵ�PTC2_LRC��0����ʱ��PTC2_CNTR���ӵ�PTC2_LRC���Զ����¿�ʼ����
//D5: R/W, INTE  1: PTC��PTC2_CNTR����PTC2_LRC����PTC2_HRCʱ�����жϣ�0���������ж�
//D6: R/W, INT  1: INT �жϵĵ�ǰ״̬�����豸д��1������ж�
//D7: R/W, CNTRRST  1: ��������λ��0����������������

#define PTC2_CTRL1  0xffc7          // PWM/��ʱ��/��ʱ��(PTC)���ƼĴ���1
//D0: R/W CAPTE  1����������ģʽ��0������ģʽ�ر� 

//=========
//SPI������
//=========

#define SPCR  0xffb0              //SPI ���ƼĴ���  Ĭ��ֵ 0x10
//D7: R/W SPIE  0: SPI�жϱ����� 1��SPI�жϿ���
//D6: R/W SPE   0: SPI�ر�   1��SPIʹ��
//D5: R/W SS    0: ���豸ѡ�� 1�����豸δѡ��
//D4: R/W MSTR  0: ���豸����ģʽ  1�����豸����ģʽ
//D3: R/W CPOL  �������ݴ���ģʽ
//D2: R/W CPHA  �������ݴ���ģʽ�� �������źţ�����4��ģʽ
//D[1:0] R/W SPR  ��λ����չ�Ĵ����е�ESPRλ��ͬ����SPI�˵Ĵ���ʱ��Ƶ��

#define SPSR  0xffb1              //SPI ״̬�Ĵ���   Ĭ��ֵ 0x05
//D7: R/W SPIF  �жϱ�ǣ������ɸ��ֽڵ����ݴ�����ɺ󣬴�λ�ᱻ��1��
//              ���ͬʱSPCR�Ĵ�����SPIEλҲ����1�Ļ����ͻ����һ���жϡ�
//              Ҫ�������жϣ���Ҫ���λд��1
//D6: R/W WCOL  д��ͻ�����д�����������������ݼĴ���ִ��д���������λ�ᱻ��1
//              Ҫ����λ���㣬��Ҫ���λд��1
//D[5:4] R  ����
//D3: R  WFFULL  дFIFO��
//D2: R  WFEMPTY дFIFO��
//D1: R  RFFULL  ��FIFO��
//D0: R  RFEMPTY ��FIFO��

#define SPDR  0xffb2              //SPI ���ݼĴ���

#define SPER  0xffb3              //SPI ��չ�Ĵ���
//D[7:6] R/W ICNT  �жϼ��� (ICNT+1)���ֽ����ݴ������SPIFλ����1
//D[1:0] R/W ESPR  ��չSPIʱ��Ƶ��ѡ��
//    ESPR              SPR                  ��Ƶ����
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
//�жϿ�����
//==========
/*
    �жϿ�������8���ж����룬һ���ж��������CPU���ж����룻8���������ж�����ʱ����CPU�����ж�����CPU���������ж���Ӧ
���壬�������ڵڶ����ж���Ӧ���巵�ذ�λ�жϺţ�CPU�����жϺŲ�ѯ�ж������������жϷ������
    8���ж����������ֱ����£�
    �ж�0�������ж�
    �ж�1�������ж�
    �ж�2��PCM�жϣ�IIS_CONTROL�Ĵ���D0=0��/IIS�����жϣ�IIS_CONTROL�Ĵ���D0=1��
    �ж�3���ⲿ�ܽ�ir3���루IIS_CONTROL�Ĵ���D0=0��/IIS�����жϣ�IIS_CONTROL�Ĵ���D0=1��
    �ж�4��PCT2
    �ж�5��UART0
    �ж�6��UART1
    �ж�7��SPI
    �ж�8��MJPEG����׼����
*/
//�Ĵ���
#define INT_MASK  0xff20              //�ж����μĴ�������/д����ʼֵ��0XFF��
//0���ж�ʹ�ܣ�1���ж�����
#define INT_MASK_H  0xff24              //�ж����θ߼Ĵ�������/д����ʼֵ��0XFF��
//0���ж�ʹ�ܣ�1���ж�����

#define INT_EDGE  0xff21              //�жϼ��ԼĴ�������/д����ʼֵ��0X00��
//0�������ش�����1���½��ش�����
#define INT_EDGE_H  0xff25              //�жϼ��Ը߼Ĵ�������/д����ʼֵ��0X00��
//0�������ش�����1���½��ش�����

#define INT_VECTOR  0xff22              //�ж������Ĵ�������/д����ʼֵ��0X08��
//D7[7��3]���жϺŸ���λ�����жϷ���Ĵ�������λ�ϲ�����ɰ�λ�жϺ�,��λ�жϺų�������Ϊ�ж��������ַ��

//#define INT_VECTOR  0xff23              //�жϷ���Ĵ�����ֻ������ʼֵ��0X00��
#define INT_ISR  0xff23              //�жϷ���Ĵ�����ֻ������ʼֵ��0X00��//��2011-0120����
//D[2��0]�����ڷ�����жϺ�

/*
��̷�����
	outportb(INT_MASK, 0xf0);//���ĸ��ж�ʹ�ܣ����ĸ��жϹر�
	outportb(INT_EDGE, 0x00);//���������ش���
	outportb(INT_VECTOR, 0x08);//�жϺ�Ϊ8-15
	enable();
	
	
*/

//========
//��Ƶ����
//========
#define VI_CONTROL  0xff30              //��Ƶ������ƼĴ���
//D0����Ƶ����ʹ�ܣ�R/W��
//D1����Ƶ����ˮƽѹ��ʹ�ܣ�R/W��
//D2����Ƶ�������ʹ�ܣ�R/W��
//D3: ��Ƶ����4:2:2��4:1:1�任��ʽ��R/W����0��
//D4: ��Ƶ����ɼ����ڣ�0��CIF��1��VGA
//D6: ��Ƶ���봹ֱѹ��ʹ�ܣ�R/W��
//D7: ��Ƶ�������ʹ�ܣ�R/W��

#define VI_STATUS  0xff31              //��Ƶ����״̬�Ĵ���
//D0: ��Ƶ�����ѿ���״̬��R��
//D[2��1]����Ƶ���뵱ǰ���ţ�R��

#define VI_HSIZE_L  0xff32              //��Ƶ����ˮƽ�ߴ�ͼĴ���
//D[7:0]:(R)
#define VI_HSIZE_H  0xff33              //��Ƶ����ˮƽ�ߴ�߼Ĵ���
//D[1:0]:(R)

#define VI_VSIZE_L  0xff34              //��Ƶ���봹ֱ�ߴ�ͼĴ���
//D[7:0]:(R)
#define VI_VSIZE_H  0xff35              //��Ƶ���봹ֱ�ߴ�߼Ĵ���
//D[1:0]:(R)

#define VI_CONTROL2  0xff36              //��Ƶ������ƼĴ���2
//D0��������R/W��
//D1����Ƶ���봰�ڼĴ�����0xff38-0xff3f��ʹ�ܣ�0��ʹ��VI_CONTROL D2��D4 ������Ƶ���봰�ڣ�1��ʹ�ã�0xff38-0xff3f��������Ƶ���봰��
//D2: 

//��2011-01-20��ӣ���
#define VI_MLINE  0xff37              //��Ƶ����δ����������Ĵ���
//D[7:0]:(R)

#define VI_WIN_HSTART_L  0xff38              //��Ƶ���봰��ˮƽ���ͼĴ���
//D[7:0]:(R/W,0x90)
#define VI_WIN_HSTART_H  0xff39              //��Ƶ���봰��ˮƽ���߼Ĵ���
//D[7:0]:(R/W,0x00)

#define VI_WIN_VSTART_L  0xff3a              //��Ƶ���봰�ڴ�ֱ���ͼĴ���
//D[7:0]:(R/W,0x00)
#define VI_WIN_VSTART_H  0xff3b              //��Ƶ���봰�ڴ�ֱ���߼Ĵ���
//D[7:0]:(R/W,0x00)

#define VI_WIN_HSIZE_L  0xff3c              //��Ƶ���봰��ˮƽ�ߴ�ͼĴ���
//D[7:0]:(R/W,0x60)
#define VI_WIN_HSIZE_H  0xff3d              //��Ƶ���봰��ˮƽ�ߴ�߼Ĵ���
//D[7:0]:(R/W,0x01)

#define VI_WIN_VSIZE_L  0xff3e              //��Ƶ���봰�ڴ�ֱ�ߴ�ͼĴ���
//D[7:0]:(R/W,0x20)
#define VI_WIN_VSIZE_H  0xff3f              //��Ƶ���봰�ڴ�ֱ�ߴ�߼Ĵ���
//D[7:0]:(R/W,0x01)

//========
//��Ƶ����
//========
#define DE_QP  0xff40              //��Ƶ����QP�Ĵ���
//D[5:0](R)

#define DE_ERROR  0xff41              //��Ƶ����������Ĵ���
//D[7:0](R)

#define DE_HSIZE_L  0xff42              //��Ƶ����ˮƽ�ߴ�ͼĴ���
//D[7:0](R)

#define DE_HSIZE_H  0xff43              //��Ƶ����ˮƽ�ߴ�߼Ĵ���
//D[5:0](R)

#define DE_VSIZE_L  0xff44              //��Ƶ���봹ֱ�ߴ�ͼĴ���
//D[7:0](R)

#define DE_VSIZE_H  0xff45              //��Ƶ���봹ֱ�ߴ�߼Ĵ���
//D[5:0](R)

#define DE_RATE_L  0xff46              //��Ƶ��������KB���ͼĴ���
//D[7:0](R)

#define DE_RATE_H  0xff47              //��Ƶ��������KB���߼Ĵ���
//D[7:0](R)

//==============
//GPIO A,B,C,D,E
//==============

//GPIO A
#define GPIOA_OUT_DATA  0x02d0		//GPIO A ������ݼĴ���
//D[7:0](R/W)

#define GPIOA_OEN_DATA  0x02d1		//GPIO A ���ʹ�ܷǼĴ���
//D[7:0](R/W): 0:���ʹ��;1:�������

#define GPIOA_IN_DATA  0x02d2		//GPIO A �������ݼĴ���
//D[7:0](R)

//GPIO B
#define GPIOB_OUT_DATA  0x02d4		//GPIO B ������ݼĴ���
//D[7:0](R/W)

#define GPIOB_OEN_DATA  0x02d5		//GPIO B ���ʹ�ܷǼĴ���
//D[7:0](R/W): 0:���ʹ��;1:�������

#define GPIOB_IN_DATA  0x02d6		//GPIO B �������ݼĴ���
//D[7:0](R)

#define GPIOB_SEL  0x02d7		//GPIO B ����ѡ��Ĵ���
//D[7:0](R/W): 0:�ܽ�ԭ����;1:�ܽ���ΪGPIO

//GPIO C
#define GPIOC_OUT_DATA  0x02d8		//GPIO C ������ݼĴ���
//D[7:0](R/W)

#define GPIOC_OEN_DATA  0x02d9		//GPIO C ���ʹ�ܷǼĴ���
//D[7:0](R/W): 0:���ʹ��;1:�������

#define GPIOC_IN_DATA  0x02da		//GPIO C �������ݼĴ���
//D[7:0](R)

#define GPIOC_SEL  0x02db		//GPIO C ����ѡ��Ĵ���
//D[7:0](R/W): 0:�ܽ�ԭ����;1:�ܽ���ΪGPIO

//GPIO D
#define GPIOD_OUT_DATA  0x02dc		//GPIO D ������ݼĴ���
//D[7:0](R/W������)

#define GPIOD_OEN_DATA  0x02dd		//GPIO D ���ʹ�ܷǼĴ���
//D[7:0](R/W������): 0:���ʹ��;1:�������

#define GPIOD_IN_DATA  0x02de		//GPIO D �������ݼĴ���
//D[7:0](R������)

#define GPIOD_SEL  0x02df		//GPIO D ����ѡ��Ĵ���
//D[7:0](R/W������): 0:�ܽ�ԭ����;1:�ܽ���ΪGPIO

//GPIO E
#define GPIOE_OUT_DATA  0x02e0		//GPIO E ������ݼĴ���
//D[7:0](R/W������)

#define GPIOE_OEN_DATA  0x02e1		//GPIO E ���ʹ�ܷǼĴ���
//D[7:0](R/W������): 0:���ʹ��;1:�������

#define GPIOE_IN_DATA  0x02e2		//GPIO E �������ݼĴ���
//D[7:0](R������)

#define GPIOE_SEL  0x02e3		//GPIO E ����ѡ��Ĵ���
//D[7:0](R/W������): 0:�ܽ�ԭ����;1:�ܽ���ΪGPIO

//��2011-01-20��ӣ���
//��Ƶ����
#define EN_CONTROL  0xfc00		//��Ƶ������ƼĴ���
//D[7:0](R/W��0x00)
//D0������ʹ�ܣ�0�������ֹ��1������ʹ��
//D1����������ʹ�ܣ�0������������ͣ�1�������Զ�����
//D2��H264ѡ�񣬣���δʵ�֣�
//D3����Ƶ����ģ�����㣬1����Ҫ�ź�ǿ������()
//D4����Ƶ����ģ�����㣬1����Ҫ�ź�ǿ������()
//D5����������ģ�����㣬1����Ҫ�ź�ǿ������()
//D6�����������ж�ʹ�ܣ�0���ж������1���ж�ʹ��()
//D7������

#define EN_STATUS  0xfc01		//��Ƶ����״̬�Ĵ���
//D[7:0](R��0x00)
//D0������ͷ����
//D1��Iͼ��ͷ����
//D2��Pͼ��ͷ����
//D3����������
//D4����Ƶ�ɼ�����
//D5��������������
//D6����������
//D7������������

#define EN_BAUD_RATE_L  0xfc02		//��Ƶ���벨���ʵͼĴ���
//D[7:0](R/W��0xe8)
#define EN_BAUD_RATE_H  0xfc03		//��Ƶ���벨���ʸ߼Ĵ���
//D[15:8](R/W��0x03)
//

#define EN_QP_PRE  0xfc04		//��Ƶ����QPԤ�üĴ���
//D[7:0](R/W��0x20��0x1a(H.264))
//D[5:0]:Ԥ��QPֵ
//D7:VBR���ɱ䲨���ʣ�ʹ��

#define EN_QP  0xfc05		//��Ƶ����QP���ؼĴ���
//D[5:0](R��0x20����/0x1a(H.264))

#define EN_MBWIDTH  0xfc06		//��Ƶ�������ȼĴ���
//D[6:0](R/W��0x16)

#define EN_MBHEIGHT  0xfc07		//��Ƶ������߶ȼĴ���
//D[6:0](R/W��0x12)

#define EN_BUF_USED_L  0xfc08		//��Ƶ����������������ʹ�ô�С�ͼĴ���
//D[7:0](R)
#define EN_BUF_USED_H  0xfc09		//��Ƶ����������������ʹ�ô�С�߼Ĵ���
//D[15:8](R)

#define EN_TX_BS_PACK_L  0xfc0a		//��Ƶ�����������Ͱ����ͼĴ���
//D[7:0](R)
#define EN_TX_BS_PACK_H  0xfc0b		//��Ƶ�����������Ͱ����߼Ĵ���
//D[11:8](R)

#define EN_PICTURE  0xfc0c		//��Ƶ����ͼ��żĴ���
//D[7:0](R)

#define EN_MICRO_LINE  0xfc0d		//��Ƶ����ͼ��������Ĵ���
//D[6:0](R)

#define EN_MICRO_BLOCK  0xfc0e		//��Ƶ����ͼ�������Ĵ���
//D[6:0](R)

#define EN_BLOCK  0xfc0f		//��Ƶ����ͼ������Ĵ���
//D[2:0](R)

#define EN_TX_BS_RATE_KB_L  0xfc10		//��Ƶ�����������Ͳ����ʵͼĴ�������KBΪ��λ��
//D[7:0](R)
#define EN_TX_BS_RATE_KB_H  0xfc11		//��Ƶ�����������Ͳ����ʸ߼Ĵ�������KBΪ��λ��
//D[15:8](R)

//��2011-05-27��ӣ���
//PCM�����
#define PCMI_CONTROL  0xfc20		//��Ƶ������ƼĴ���
//D[7:0](R/W��0x00)
//D[3:0]:������������
// 0000����������
// 0001��������ȥ1/16
// 0010��������ȥ2/16
// 0011��������ȥ3/16
// 0100��������ȥ4/16
// 0101��������ȥ5/16
// 0110��������ȥ6/16
// 0111��������ȥ7/16
// 1000��������ȥ8/16
// 1001��������ȥ9/16
// 1010��������ȥ10/16
// 1011��������ȥ11/16
// 1100��������ȥ12/16
// 1101��������ȥ13/16
// 1110��������ȥ14/16
// 1111��������ȥ15/16
//D4:������������ʹ�ܣ�0:��ֹ�������������Ʋ������ã�1:������������������(δʵ��)
//D5:�����Զ��������ڣ�0:��ֹ; 1:�Զ���������(δʵ��)
//D6��PCM����빦�ʸ�ʽ��0��24λ��������1��24λ��������16λ���ȼ�8λ��λ
//D7�������ж�ʹ��

#define PCM_STATUS  0xfc21		//��Ƶ״̬�Ĵ���
//D[7:0](R, 0x00)
//D0:	�ֶ��жϣ�ȡ����
//D1:	�����жϣ�ȡ����
//D2:	PCM����FIFO�գ�0�����գ�1���գ�����D0��
//D3��	PCM���FIFO����0��������1����������D1��
//D[3:2]: PCMI���ݿ��
//D[5:4]: PCMO���ݿ��
//D6: PCMO����������һ�飻0��PCMO����������һ�飻1��PCMO����������һ��
//D7: PCMO�������գ�0:PCMO���������գ�1��PCMO��������

//#define PCMI_THRE_L  0xfc22		//160������Ƶ������ֵ�ͼĴ���(δʵ��)
////D[7:0](R/W��0x00)
//#define PCMI_THRE_H  0xfc23		//160������Ƶ������ֵ�߼Ĵ���(δʵ��)
////D[7:0](R/W��0x00)

#define PCMI_POWER_L  0xfc24		//160/64/128/256������Ƶ���빦�ʵͼĴ���
//D[7:0](R��0x00)
#define PCMI_POWER_H  0xfc25		//160/64/128/256������Ƶ���빦�ʸ߼Ĵ���
//D[7:0](R��0x00)
//24λ��������ʽʱ��8λPCMת��16λPCM��Ȼ�����ֵ��ӣ�64���128��256�����24λ�����16λ
//24λ��������ʽʱ��8λPCMת��16λPCM��Ȼ�����ֵ��ӣ�64���128��256����ӽ������16λ�����λΪС����ǰһλ������15λΪС����

#define PCMI_POWER_S  0xfc22		//160/64/128/256������Ƶ���빦����λ�Ĵ���
//D[7:0](R/W��0x00)
//24λ��������ʽʱ��8λPCMת��16λPCM��Ȼ�����ֵ��ӣ�64���128��256�����24λ�����8λ
//24λ��������ʽʱ��8λPCMת��16λPCM��Ȼ�����ֵ��ӣ�64���128��256����ӽ������λ��

#define PCMO_CONTROL  0xfc28		//��Ƶ������ƼĴ���
//D[7:0](R/W��0x00)
//D[3:0]:������������
// 0000����������
// 0001��������ȥ1/16
// 0010��������ȥ2/16
// 0011��������ȥ3/16
// 0100��������ȥ4/16
// 0101��������ȥ5/16
// 0110��������ȥ6/16
// 0111��������ȥ7/16
// 1000��������ȥ8/16
// 1001��������ȥ9/16
// 1010��������ȥ10/16
// 1011��������ȥ11/16
// 1100��������ȥ12/16
// 1101��������ȥ13/16
// 1110��������ȥ14/16
// 1111��������ȥ15/16
//D4:������������ʹ�ܣ�0:��ֹ�������������Ʋ������ã�1:������������������(δʵ��)
//D5:�����Զ��������ڣ�0:��ֹ; 1:�Զ���������(δʵ��)
//D6: =1��PCMO������������1
//D7: PCMO�ж�ʹ�ܣ�0�����Σ�1��ʹ��

#define PCM_CONTROL  0xfc29		//PCM�ӿڿ��ƼĴ���
//D[7:0](R/W��0x00)
//D0��PCM�ӿ�λ����0:8λ��1:16λ
//D1��K8��ȣ�0���˸����ڣ�1��һ������
//D2����һ����������Ч��0����Ч��1������
//D3�������ʣ�0:8K��1:16K
//D4: PCMO�ڲ�������������
//D[6:5]:PCM���ݿ��С
//	00:160�ֽ�
//	01: 64�ֽڣ����0-3��
//	10:128�ֽڣ����0-3��
//	11:256�ֽڣ����0-3��
//D7��A�ɡ�u��PCMѡ��0��A�ɣ�1��u��

#define PCMO_POWER_L  0xfc2c		//160/64/128/256������Ƶ���빦�ʵͼĴ���
//D[7:0](R��0x00)
#define PCMO_POWER_H  0xfc2d		//160/64/128/256������Ƶ���빦�ʸ߼Ĵ���
//D[7:0](R��0x00)
//24λ��������ʽʱ��8λPCMת��16λPCM��Ȼ�����ֵ��ӣ�64���128��256�����24λ�����16λ
//24λ��������ʽʱ��8λPCMת��16λPCM��Ȼ�����ֵ��ӣ�64���128��256����ӽ������16λ�����λΪС����ǰһλ������15λΪС����

#define PCMO_POWER_S  0xfc23		//160/64/128/256������Ƶ���빦����λ�Ĵ���
//D[7:0](R/W��0x00)
//24λ��������ʽʱ��8λPCMת��16λPCM��Ȼ�����ֵ��ӣ�64���128��256�����24λ�����8λ
//24λ��������ʽʱ��8λPCMת��16λPCM��Ȼ�����ֵ��ӣ�64���128��256����ӽ������λ��

//��ƵMJPEG����
#define EN_J_CONTROL  0xfb00		//��ƵMJPEG������ƼĴ���1
//D[7:0](R/W��0x00)
//D0��MJPEG����ʹ�ܣ�0��MJPEG�����ֹ��1��MJPEG����ʹ��
//D1��MJPEG����������ŷ�ʽ,0��ѭ����ʽ��1��ҳ��ʽ���������ֳɴ�С��ͬ��2ҳ��
//D2��MJPEG������������С,0:64KB��1��128KB
//D3��
//D4����ƵMJPEG����ģ�����㣬1����Ҫ�ź�ǿ������()
//D5����������ģ�����㣬1����Ҫ�ź�ǿ������()
//D6�����������ж�ʹ�ܣ�0���ж������1���ж�ʹ��()
//D7������

#define EN_J_STATUS  0xfb01		//��ƵMJPEG����״̬�Ĵ���
//D[7:0](R��0x00)
//D0���������ڽ���
//D1������������Ч
//D2������ҳ�ţ�0����32/64KB����ַ0x170000��ʼ32KB�����ַ0x170000��ʼ64KB����1����32/64KB����ַ0x178000��ʼ32KB�����ַ0x180000��ʼ64KB��
//D3��MJPEG��������������0��������1��������
//D4��
//D5��
//D6��
//D7��

#define EN_J_DQT_ADD  0xfb02		//��ƵMJPEG����DQT��ַ�Ĵ���
//D[7:0](R/W)
#define EN_J_DQT_DAT  0xfb03		//��ƵMJPEG����(1/DQT)���ݼĴ���
//D[6:0](R/W)
#define EN_J_DQT_SHIFT  0xfb04		//��ƵMJPEG����(1/DQT)������λ�Ĵ���
//D[3:0](R/W)
//�ȸ���0--100�õ������һ����DQTֵ��������ǳ�����������level/DQT,������1/DQT��Ҳ���ǣ�2��N�η���/��DQT*��2��N�η���������N����EN_J_DQT_SHIFT�Ĵ�����
//����2��N�η���/DQT����EN_J_DQT_DAT��EN_J_DQT_DAT��128����λ�Ĵ�������EN_J_DQT_ADDѡ��ǰ64������Y����64������UVN����EN_J_DQT_DAT�õ���󾫶Ⱦ���

#define EN_J_CONTROL2  0xfb05		//��ƵMJPEG������ƼĴ���2
//D[7:0](R/W��0x00)
//D[5:0]: �����ͼ��ÿ��֡��
//D[7:6]: ԭͼ��ÿ��֡��
//	=00:25֡
//	=01:30֡
//	=10:50֡��������
//	=11:60֡��������

//D[7:6]=00ʱ��D[5:0]Ϊ1-25(0�ͷ�Χ���Ǹ��ݱ����ٶ��Զ�ѡ��֡�ʣ�
//D[7:6]=01ʱ��D[5:0]Ϊ1-30(0�ͷ�Χ���Ǹ��ݱ����ٶ��Զ�ѡ��֡�ʣ�

#define EN_J_MBWIDTH  0xfb06		//��ƵMJPEG�������ȼĴ���
//D[6:0](R/W��0x16)

#define EN_J_MBHEIGHT  0xfb07		//��ƵMJPEG������߶ȼĴ���
//D[6:0](R/W��0x12)

#define EN_J_TX_BS_PACK_L  0xfb0a		//��ƵMJPEG�����������Ͱ����Ĵ���
//D[3:0](R)

#define EN_J_PICTURE  0xfb0c		//��ƵMJPEG����ͼ��żĴ���
//D[7:0](R)

#define EN_J_MICRO_LINE  0xfb0d		//��ƵMJPEG����ͼ��������Ĵ���
//D[6:0](R)

#define EN_J_MICRO_BLOCK  0xfb0e		//��ƵMJPEG����ͼ�������Ĵ���
//D[6:0](R)

#define EN_J_BLOCK  0xfb0f		//��ƵMJPEG����ͼ������Ĵ���
//D[2:0](R)

//��ƵMJPEG����
#define DE_J_CONTROL  0xfb10		//��ƵMJPEG������ƼĴ���
//D[7:0](R/W��0x00)
//D0��MJPEG����ʹ�ܣ�0��MJPEG�����ֹ��1��MJPEG����ʹ��
//D1��MJPEG����������1��MJPEG��������
//D2��MJPEG��������ж�ʹ�ܣ�0���ж������1���ж�ʹ��
//D3��MJPEG������16λRGB�����0��YUV�����1��16λRGB���
//D4: MJPEG����FF��ʽ��0�����任��1��FF00��ΪFF��������Ϊ��
//D4��MJPEG����64KB���ƣ�0�������ƣ�1������
//D5����ʾ֡�ж�ʹ�ܣ�0���ж������1���ж�ʹ��
//D6: YUV 4:2:0��4:2:2�任��0���ޱ任��1���任
//D7: �ļ���ʽMJPEG���룬0�����������룻1���ļ���ʽ����

#define DE_J_STATUS  0xfb11		//��ƵMJPEG����״̬�Ĵ���
//D[7:0](R��0x00)
//D0���������ڽ���
//D1������������Ч
//D2���������
//D3��
//D4��
//D5��
//D6��
//D7��

#define DE_J_CPU_SA_H  0xfb14		//��ƵMJPEG CPU����������ʼ��ַ�߼Ĵ���
//D[7:0](R/W��0x00)
#define DE_J_CPU_SA_M  0xfb13		//��ƵMJPEG CPU����������ʼ��ַ�мĴ���
//D[7:0](R/W��0x00)
#define DE_J_CPU_SA_L  0xfb12		//��ƵMJPEG CPU����������ʼ��ַ�ͼĴ���
//D[7:0](R/W��0x00)

#define DE_J_CPU_PN  0xfb15		//��ƵMJPEG CPU��������Ĵ���
//D[7:0](R/W��0x00)

#define DE_J_MBWIDTH  0xfb16		//��ƵMJPEG�������ȼĴ���
//D[6:0](R/W��0x16)

#define DE_J_MBHEIGHT  0xfb17		//��ƵMJPEG������߶ȼĴ���
//D[6:0](R/W��0x12)

#define DE_J_HSIZE_L  0xfb18		//��ƵMJPEG�����ȵͼĴ���
//D[7:0](R/W��0x0)
#define DE_J_HSIZE_H  0xfb19		//��ƵMJPEG�����ȸ߼Ĵ���
//D[10:8](R/W��0x2)

#define DE_J_VSIZE_L  0xfb1a		//��ƵMJPEG����߶ȵͼĴ���
//D[7:0](R/W��0x0)
#define DE_J_VSIZE_H  0xfb1b		//��ƵMJPEG����߶ȸ߼Ĵ���
//D[10:8](R/W��0x2)

#define DE_J_DQT_ADD  0xfb1c		//��ƵMJPEG����DQT��ַ�Ĵ���
//D[6:0](R/W)
#define DE_J_DQT_DAT  0xfb1d		//��ƵMJPEG����DQT���ݼĴ���
//D[6:0](R/W)

#define DE_J_DATA_SA_H  0xfb22		//��ƵMJPEG����������ʼ��ַ�߼Ĵ���
//D[7:0](R/W��0x00)
#define DE_J_DATA_SA_M  0xfb21		//��ƵMJPEG����������ʼ��ַ�мĴ���
//D[7:0](R/W��0x00)
#define DE_J_DATA_SA_L  0xfb20		//��ƵMJPEG����������ʼ��ַ�ͼĴ���
//D[7:0](R/W��0x00)

#define DE_J_CPU_LEN_H  0xfb26		//��ƵMJPEG CPU�����������ȸ߼Ĵ���
//D[7:0](R/W��0x00)
#define DE_J_CPU_LEN_M  0xfb25		//��ƵMJPEG CPU�������������мĴ���
//D[7:0](R/W��0x00)
#define DE_J_CPU_LEN_L  0xfb24		//��ƵMJPEG CPU�����������ȵͼĴ���
//D[7:0](R/W��0x00)

#define MAC_RX_SA_H  0xfb23		//MAC���ջ�������ʼ��ַ�߼Ĵ���
//D[7:0](R/W��0x18)
//������ջ�������ʼ��ַλ[23:16],��16λΪ0

#define USED_RATE_SEL  0xfb28		//ʹ��Ƶ���ź�ѡ��ͼĴ���
//D[7:0](R/W��0x00)
//8'h00: SDRAMʹ��Ƶ��
//8'h01: MJPEG����ʹ��Ƶ��
//8'h02: MJPEG����ʹ��Ƶ��
//8'h03: MAC����ʹ��Ƶ��
//8'h04: MAC����ʹ��Ƶ��

#define USED_RATE_L  0xfb2a		//ʹ��Ƶ�ʵͼĴ���
//D[7:0](R��0x0)
#define USED_RATE_H  0xfb2b		//ʹ��Ƶ�ʸ߼Ĵ���
//D[15:8](R��0x0)
//��һ���ڣ��ź�ʹ�ü���/65535

//��Ƶ��ʾ���ڣ����䣩
#define VW_CONTROL  0xfb30		//��Ƶ���ڿ��ƼĴ���
//D0: �ֶ�������Ƶ���ڣ�0���Զ����£�1��������ʾ�ж��л���Ƶ��������
//D1: ��Ƶ���ڸ�ʽ��
//	0��YUV�����ʽ��
//		YUV����Yͼ��Uͼ��Vͼ����ã���������Ƶ�ɼ�ͼ��
//	1��YUV�ϲ���ʽ
//		YUV����YUYV��ţ�������MJPEG����ͼ��

#define VW_SA_H  0xfb33		//��Ƶ������ʼ��ַ�߼Ĵ���
//D[7:0](R/W��0x00)
#define VW_SA_M  0xfb32		//��Ƶ������ʼ��ַ�мĴ���
//D[7:0](R/W��0x00)
#define VW_SA_L  0xfb31		//��Ƶ������ʼ��ַ�ͼĴ���
//D[7:0](R/W��0x00)

#define VW_WIDTH_L  0xfb34		//��Ƶ������ʾ�ڴ��ȵͼĴ���
//D[7:0](R/W��0x00)
#define VW_WIDTH_H  0xfb35		//��Ƶ������ʾ�ڴ��ȸ߼Ĵ���
//D[7:0](R/W��0x00)

//====================
//��Ƶ��ʾ���ڱ�̼��
//===================
/*
1��ԭ��˵��
	����Ƶ��ʾ�����У���ʾ��ͼ�������ǽ����ͼ�����Ƶ�ɼ��˿ڲɼ���ͼ�񣬹����ķ�ͼ��ÿ��ͼ���Զ�������ʾ��
ͼ��洢λ�ù̶���ͼ���ǰ���YUV���������ֱ�洢������޷���Ԥ�����µ���ʾ��ʽ�У�VW_CONTROL D[1��0]=11����Ҫ����MJPEG����ͼ��
����ʾ�����������ʾ��ֱ�жϷ��������ͨ��VW_SA_L��VW_SA_M��VW_SA_H������ʾ������ʼ��ַ��VW_WIDTH_L��VW_WIDTH_H������ʾ�ڴ��
�ȡ�
	��ʵ�ʴ����ϣ�Ϊ����ʾͼ��û�о�ݣ���Ҫ����ͼ�����ٷֳ�������һ������ʾ��һ���ڽ��룬���³������Ӿ�����������ġ�
2���Ĵ���
#define VW_CONTROL  0xfb30		//��Ƶ���ڿ��ƼĴ���
//D0: �ֶ�������Ƶ���ڣ�0���Զ����£�1��������ʾ�ж��л���Ƶ��������
//D1: ��Ƶ���ڸ�ʽ��
//	0��YUV�����ʽ��
//		YUV����Yͼ��Uͼ��Vͼ����ã���������Ƶ�ɼ�ͼ��
//	1��YUV�ϲ���ʽ
//		YUV����YUYV��ţ�������MJPEG����ͼ��

#define VW_SA_H  0xfb33		//��Ƶ������ʼ��ַ�߼Ĵ���
//D[7:0](R/W��0x00)
#define VW_SA_M  0xfb32		//��Ƶ������ʼ��ַ�мĴ���
//D[7:0](R/W��0x00)
#define VW_SA_L  0xfb31		//��Ƶ������ʼ��ַ�ͼĴ���
//D[7:0](R/W��0x00)

#define VW_WIDTH_L  0xfb34		//��Ƶ������ʾ�ڴ��ȵͼĴ���
//D[7:0](R/W��0x00)
#define VW_WIDTH_H  0xfb35		//��Ƶ������ʾ�ڴ��ȸ߼Ĵ���
//D[7:0](R/W��0x00)
3���������
ȫ�ֱ���
	INT8U have_mjpeg_stream;//
	INT8U dec_sa_sw;
��ʼ����
	have_mjpeg_stream=0;
	dec_sa_sw=0;//��Ƶ��ʾ������ʼ��ַ�л�����
	videow(VIDEO_DISP_WIN_X,VIDEO_DISP_WIN_XSIZE,VIDEO_DISP_WIN_Y,VIDEO_DISP_WIN_YSIZE);

�н���������
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
ͬʱ��
void interrupt VInt()
{
	INT16U bs_length;
        INT32U tmp_data;
	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xdf);//���MJPEG�ж�
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
	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x20);//ʹ��MJPEG�ж�
}

*/
//========
//��Դ����
//========

#define POWER_CONTROL1  0xfeec		//��Դ���ƼĴ���1
//D[7:0](R/W, 0x0c)
//D0��ʱ���л���ʼ(����͹���ģʽ)
//D1��ʱ���л�����(����͹���ģʽ)
//D[3��2]��ʱ�ӷ�Ƶѡ��
//	=11��64��Ƶ
//	=10��32��Ƶ
//	=01��16��Ƶ
//	=00���ر�ʱ��
//D4���ر���ʾʱ�ӣ�ȡ����
//D5���ر�MACʱ�ӣ�ȡ����
//D6���ر���Ƶ��׽ʱ�ӣ�ȡ����
//D[6:4]: CPU��дSDRAM�������ȼ�ѡ��0-7����0����ߣ�7�����
//D7���ر���Ƶʱ�ӣ�ȡ����
//D7��ÿ��SDRAMˢ����������0:8����1:4��
//ע������LCD��ʾʱ��ÿ������һ��ˢ�����ڣ�ÿ��8����4�����ڣ�������ʾʱ��ÿ40΢������һ��ˢ������

#define POWER_CONTROL2  0xfeed		//��Դ���ƼĴ���2
//D[7:0](R/W)
//D0���ر�MJPEG��Ƶ�ɼ�ʱ��
//D1���ر�MJPEG��Ƶ����ʱ��
//D2���ر�MJPEG��Ƶ����ʱ��
//D3���ر���ʾʱ��
//D4���ر�PCMʱ��
//D5���ر�MACʱ��
//D6���ر�UART0ʱ��
//D7���ر�UART1ʱ��

//==================
//AUDIO CODEC���Ҫ��
//==================
/*
    ��SDRAM�У���������С�ɱ仺��������С�ֱ���256��512��1024�ֽڣ�ÿ���������ֳ�4�飬���С�ֱ���64��128��256�ֽڣ��������������ֱ��ǣ�һ����PCMI����������ַ��0X190000��
һ����PCMO����������ַ��0X1B0000��
   PCM�ж���PCMI������ÿ����һ���ǲ���һ���жϡ�
   
һ�����üĴ���
1��PCM_CONTROL
//D0��PCM�ӿ�λ����0:8λ��1:16λ
//D1��K8��ȣ�0���˸����ڣ�1��һ������
//D2����һ����������Ч��0����Ч��1������
//D3�������ʣ�0:8K��1:16K
//D4: PCMO�ڲ�������������
//D[6:5]:PCM���ݿ��С
//	00:160�ֽ�
//	01: 64�ֽڣ����0-3��
//	10:128�ֽڣ����0-3��
//	11:256�ֽڣ����0-3��

2��PCM_STATUS
//D[7:0](R, 0x00)
//D0:	�ֶ��жϣ�ȡ����
//D1:	�����жϣ�ȡ����
//D2:	PCM����FIFO�գ�0�����գ�1���գ�����D0��
//D3��	PCMO����������0��������1����������D1��
//D[3:2]: PCMI���ݿ��
//D[5:4]: PCMO���ݿ��
//D6: PCMO����������һ�飻0��PCMO����������һ�飻1��PCMO����������һ��
//D7: PCMO�������գ�0:PCMO���������գ�1��PCMO��������

3��CONTROL_REG
//D3��	PCM����ʹ��
//D4��	PCM���ʹ��
//D5��  0��PCM�Զ�����1��PCM�ֶ�����
//D6��  PCM�ж�ʹ��

4��PCMO_CONTROL
//D6: =1��PCMO������������1
//D7: PCMO�ж�ʹ�ܣ�0�����Σ�1��ʹ��

��ʼ��
	Cont=0xde;//D[6:3]=1011 
 	outportb(CONTROL_REG, Cont);//PCM�ж�ʹ��;PCM�Զ�����;PCM���ʹ��;PCM����ʹ��

	outportb(PCM_CONTROL, 0x2b);//PCM���ݿ��СΪ64�ֽڣ�������Ϊ16K����һ������������;K8���Ϊһ�����ڣ�PCM�ӿ�λ��Ϊ16λ

	setvect(10, PCMInt);

�жϳ���
	pcmi_bn=0;
	pcmo_bn=0;
	pcmo_first_time=1;
	pcmo_have_2block=0;

void interrupt PCMInt()//PCMI Int
{
	INT32U Maddrs;
	INT32U Maddrd;
	
	Cont&=0xbf;
	outportb(CONTROL_REG, Cont);//���PCM�ж�

	if(pcmo_bn==1)pcmo_have_2block=1;//Ϊ�˲��ԣ�ʵ���Ϸ�����ѭ��
	Maddrs=0x190000UL;
	Maddrd=0x1B0000UL;
//	Maddrs=Maddrs+(64*pcmi_bn);	
//	Maddrd=Maddrd+(64*pcmo_bn);	
	Maddrs=Maddrs+(64*(pcmi_bn&0x3));//Ϊ�˲��ԣ�ʵ��������MAC���ͻ����������ͳ�ȥ	
	Maddrd=Maddrd+(64*(pcmo_bn&0x3));	
	pcmi_bn++;
	pcmo_bn++;
	
       	fmem_fmem_dma(Maddrs, Maddrd, 64);//Ϊ�˲��ԣ�ʵ��������MAC���ͻ����������ͳ�ȥ
       	if(pcmo_have_2block==1){//Ϊ�˲��ԣ�ʵ���Ϸ�����ѭ��
       		if(pcmo_first_time==1){
			outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)|0x40);//PCMO����������һ��
			outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)&0xbf);
			pcmo_first_time=0;
       		}
		outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)|0x40);//PCMO����������һ��
		outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)&0xbf);
	}
	Cont|=0x40;
	outportb(CONTROL_REG, Cont);//ʹ��PCM�ж�
}

��ѭ��

    ����PCM_STATUS����D7=1��PCMO�������գ�������PCMO���������д��4�����ݣ�ÿдһ�齫PCMO_CONTROL D6д��1��д��0����D7=0��PCMO���������գ�
��D6=1��PCMO����������һ�飩�������д���������ݣ�D6=0��PCMO����������һ�飩��D3=0��PCMO����������������д��1�����ݣ�D3=1��PCMO������������
������д�����ݡ�
   д������ݿ鿪ʼλ����PCM_STATUS D[5:4]������=0����д���0��3��2��1��=1����д���1��0��3��2��=2����д���2��1��0��3��=3����д���3��2��1��0��
*/

//==================
//�͹���ģʽ���Ҫ��
//==================
/*
	��оƬ��ͨ���Ĵ������ã���оƬ��ʱ�����ó�������ʱ�ӵ�16/32/64֮һ���Ӷ�����͹���ģʽ���ڵ͹���ģʽ��ʹ��CLK_50Mʱ�ӵ����й���ģ���ʱ�Ӷ�
��ΪCLK_50M��16/32/64֮һ����ʱ����UART��SDRAM��Ƶ����Ƶ�ɼ�����Ƶ���빤������������Ƶ�����ٶ�����ֻ��MAC�жϵ�·������������MAC������ȷ������
����д��SDRAM���˳��͹���ģʽ������ģʽ��һ��CPU��Ϊ�����˳������������а��������������ݵȣ�CPUͨ���Ĵ��������͹���ģʽ������һ����MAC���յ�
�������ְ������ϱ���MAC��ַ�İ����ಥ�����㲥ARP�������뱾��IP��ַ��ͬ��

һ����ؼĴ���
1����Դ���ƼĴ���
#define POWER_CONTROL1  0xfeec		//��Դ���ƼĴ���1
//D[7:0](R/W, 0x0c)
//D0��ʱ���л���ʼ(����͹���ģʽ)
//D1��ʱ���л�����(����͹���ģʽ)
//D[3��2]��ʱ�ӷ�Ƶѡ��
//	=11��64��Ƶ
//	=10��32��Ƶ
//	=01��16��Ƶ
//	=00���ر�ʱ��
//D4���ر���ʾʱ�ӣ���δʵ�֣�
//D5���ر�MACʱ�ӣ���δʵ�֣�
//D6���ر���Ƶ��׽ʱ�ӣ���δʵ�֣�
//D7���ر���Ƶʱ�ӣ���δʵ�֣�

2��״̬�Ĵ���

//״̬�Ĵ���
#define STATUS_REG		0xfee0          //״̬�Ĵ���
//D1: �͹���ģʽ, 0����������ģʽ��1���͹���ģʽ
//D2��	SDRAM׼����
//D3: �������״̬���������ݷǷ���0��������1���Ƿ�����λ����������ж�ʹ��λΪ0ʱ�����

3��MAC�Ĵ���
//����MAC�����ַ
#define PAR0		0xfe10
#define PAR1		0xfe11
#define PAR2		0xfe12
#define PAR3		0xfe13
#define PAR4		0xfe14
#define PAR5		0xfe15

//�ಥ����ַ�Ĵ���
#define MAR0		0xfe16������ֽڣ�
#define MAR1		0xfe17
#define MAR2		0xfe18
#define MAR3		0xfe19
#define MAR4		0xfe1a
#define MAR5		0xfe1b����6�ֽ�MAC��ַ������ֽڣ�
#define MAR6		0xfe1c
#define MAR7		0xfe1d������ֽڣ�

//����IP��ַ
#define LIP0		0xfe30
#define LIP1		0xfe31
#define LIP2		0xfe32
#define LIP3		0xfe33

�������

1������͹���ģʽ
  	outportb(POWER_CONTROL1, inportb(POWER_CONTROL1)|0x01);
  	outportb(POWER_CONTROL1, inportb(POWER_CONTROL1)&0xfe);
2���˳��͹���ģʽ
  	outportb(POWER_CONTROL1, inportb(POWER_CONTROL1)|0x02);
  	outportb(POWER_CONTROL1, inportb(POWER_CONTROL1)&0xfd);
*/

//===============
//MAC�ط���̼��
//===============
/*
    �ڰ�˫��ģʽʱ��ͨ��PHY��MII���нӿڶ�ȡ������MAC_CONTROL D0���ó�0����MAC��⵽���߳�ͻʱ������32λ�������루TX_JAM_CODE0-3���ã���
Ȼ���ط����TX_ATTEMTP_NUM�Σ�ֱ����ȷ�����߷�����TX_STATUS D1Ϊ1ʱ��ʾ�ط�ʧ�ܣ���
һ����ؼĴ���
#define MAC_CONTROL		0xfe00             // MAC���ƼĴ���
//D[7:0](R/W, 0x01)
//D0��MACȫ˫����0����˫����1��ȫ˫��
//D1��MAC�ٶȣ�0��10M��1��100M

#define TX_STATUS		0xfe01                 // ����״̬�Ĵ���
//D[7:0](R, 0x00)
//D0: 0���޷��ͣ�1���������ڽ���
//D1�������ط�����
//D2: ����FIFO������
//D3������FIFO�մ���
//D4�����ͳ�ʱ����
//D5�����ͳ�ͻ��0���޳�ͻ��1���г�ͻ
//D6���ز������źţ�0�����źţ�1�����ź�

#define TX_BACKOFF_NUM		0xfe1e             //����BACKOFF���Ĵ���
//D[7:0](R/W, 0x40)
//D[7:0]������BACKOFF��

#define TX_ATTEMTP_NUM		0xfe1f             //���ͳ��Դ����Ĵ���
//D[7:0](R/W, 0x07)
//D[7:0]�����ͳ��Դ����Ĵ���

#define TX_JAM_CODE0		0xfe24             //����������0�Ĵ���
//D[7:0](R/W, 0x10)
//D[7:0]������������
#define TX_JAM_CODE1		0xfe25             //����������1�Ĵ���
//D[7:0](R/W, 0x32)
//D[7:0]������������
#define TX_JAM_CODE2		0xfe26             //����������2�Ĵ���
//D[7:0](R/W, 0x54)
//D[7:0]������������
#define TX_JAM_CODE3		0xfe27             //����������3�Ĵ���
//D[7:0](R/W, 0x76)
//D[7:0]������������

*/