#include "includes.h"

#define _COMMDRV_H
#include "ComDrv.h"

COMBUF comm1;
COMBUF comm2;

void InitComm(INT16U, INT32U, INT8U, INT8U, INT8U);
void interrupt int14(INT16U bp, INT16U di, INT16U si,
			INT16U ds, INT16U es, INT16U dx, 
			INT16U cx, INT16U bx, INT16U ax);
			
void interrupt Com2Int(void);
void interrupt Com1Int(void);
void interrupt SPIInt(void);

/***************************************************************\
*			InitComm				*
* description : initiate the serial communication port 		*
* argument    : port ------------- port address			*
*   		baud ------------- baud rate  			*
*		databit ---------- data length			*
*               stopbit ---------- stop bit length		*
*               paritybit -------- parity verify bit		*
*               fifolevel ------------- fifo trigger level      *
*								*
\***************************************************************/

//freq  1  --  正常　　16 --  16分频
void InitComm(INT16U port, INT32U baud, INT8U datafrm, INT8U fifolevel, INT8U freq)
{
	INT16U gen;
    /* set baudrate */

	//gen = 50000000UL/baud/freq;
        gen = CPU_MAIN_FREQ/baud/freq;   //20120222 改晶振

   outportb(port+4, (gen&0xff));
   outportb(port+5, (gen>>8));
   outportb(port+2, fifolevel);
//	outportb(port+LineControlReg, 0x80); // 波特率访问控制位
//	outport(port+DividReg, gene);
    /* set data format */
//    	outportb(port+LineControlReg, datafrm);
    /* set fifo trigger level */
//     	outportb(port+FIFOControlReg, fifolevel);
    /* set serial intterrupt */
//    	outportb(port+IntEnableReg, 1);	// 接收中断
    /* initiate seial port data structure*/
    	comm1.DP = 0;
    	comm1.WP = 0;
    	comm1.RP = 0;

    	comm2.DP = 0;
    	comm2.WP = 0;
    	comm2.RP = 0;
//outportb(UART0_TX_DATA, 0xaa);				// 发送字符
//    	setvect(COM0_VECT, Com1Int);
//    	setvect(0x14, int14);
}

void interrupt Com1Int(void)
{
	asm sti
	
 	while((inportb(UART0_RX_CTRL)&0x10)==0x10){
		comm1.Rbuf[comm1.WP] = inportb(UART0_RX_DATA);
		comm1.WP++;
		if(comm1.WP>=COMBUFFLEN){
			comm1.WP = 0;
	  }
	}
//	outportb(UART0_TX_DATA, 13);				// 发送字符
}
void interrupt Com2Int(void)
{
	asm sti

 	while((inportb(UART1_RX_CTRL)&0x10)==0x10){
		comm2.Rbuf[comm2.WP] = inportb(UART1_RX_DATA);
		comm2.WP++;
		if(comm2.WP>=COMBUFFLEN){
			comm2.WP = 0;
	  }
	}
//	outportb(UART0_TX_DATA, 14);				// 发送字符
}


#pragma argsused
void interrupt int14(INT16U bp, INT16U di, INT16U si,
			INT16U ds, INT16U es, INT16U dx, 
			INT16U cx, INT16U bx, INT16U ax)
{
	INT8U type;
	COMBUF *commp;
	
	type = highbyte(ax);
	switch(type)
	{
		case 0:
		
			break;
		case 1:
		
			break;
		
		case 2:
			
			break;
			
		case 3:
		
			break;
		
		case 4:
			
			break;
		
		case 5:
			
			break;
		default:
			
			break;
	}
}



