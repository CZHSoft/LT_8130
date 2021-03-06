#include "includes.h"

#include "stdlib.h"

//#define x_flash_h

#include "x_printf.h"
#include "x_video.h"
#include "x_gpio.h"
#include "x_udp.h"
#include "x_disp.h"

//#define CommonH
//#include "common.h"

/*
****************************************
20131119
暂无修改
****************************************
*/


void sectererase25x32(INT32U paddr);
void blockerase25x32(INT32U paddr);
void programpage25x32(INT32U paddr,INT8U *data,INT16U len);
void write25x32status(INT8U reg);
void w25x32writeenable(void);
void w25x32writedisable(void);
void erasechip25x32(void);
void read25x32id(void);
void read25x32_unique_id(INT8U *flash_id);
void xtm_read25x32data(INT32U Raddr,INT8U *data_in, INT16U len);
void XTM_SPI_DRAM(INT32U src_addr, INT32U dest_addr, INT32U len);

void wait_till_ready(void);
INT8U read25x32_status(void);
//---------------------------------------------------------------------------
void xtm_read25x32data(INT32U Raddr,INT8U *data_in, INT16U len)
{
    INT16U dcounter;
    disable();
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1
    outportb(SPDR, 0x03);                  //read data command
    inportb(SPDR);
    outportb(SPDR, Raddr>>16);                  //write dummy Address
    inportb(SPDR);
    outportb(SPDR, Raddr>>8);                  //write dummy Address
    inportb(SPDR);
    outportb(SPDR, Raddr);                  //write A7-A0 0x00
    inportb(SPDR);

//    outportb(SPDR, 0x00);
//    inportb(SPDR);

    for(dcounter=0;dcounter<len;dcounter++){
        outportb(SPDR, 0x00);
        data_in[dcounter]=inportb(SPDR);                    //write dummy data
     }
    outportb(SPCR, inportb(SPCR)|0x70);      //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash
    enable();

    wait_till_ready();
}
//---------------------------------------------------------------------------
void xtm_read25x32data111(INT32U Raddr,INT8U *data_in, INT16U len)
{
disable();
    outportb(BITBLK_CONTROL_REG, 0x20);    // spi controller
    outportb(SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1
    outportb(SPDR, 0x03);                  //read data command
    outportb(SPDR, Raddr >> 16);                //write dummy Address
    outportb(SPDR, Raddr >> 8);                //write dummy Address
    outportb(SPDR, Raddr);                  //write A7-A0 0x00
    inportb(SPDR);
    inportb(SPDR);
    inportb(SPDR);
	
  //  outportb(SPDR, 0x00);		//预写入一字节
  //  inportb(SPDR);				//读走一个无效字节，使FIFO内还剩一个字节
	//FIFO内总有一个未读字节， 可防止inportb阻塞浪费时间

	#if 1
    while (len--)
    {
    	outportb(SPDR, 0x00);
        *data_in++ = inportb(SPDR);
    }
	#else
	while (len >= 2)
	{
		len -= 2;
		outportb(SPDR, 0x00);
		outportb(SPDR, 0x00);
		*data_in++ = inportb(SPDR);
		*data_in++ = inportb(SPDR);
	}
	#endif

      	inportb(SPDR);	//读走最后一个无用的字节
	
    outportb(SPCR, inportb(SPCR) | 0x70);    //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      // spi flash
    enable();

#if 0
    INT16U dcounter;
    disable();
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1
    outportb(SPDR, 0x03);//0x03);                  //read data command
    inportb(SPDR);
    outportb(SPDR, Raddr>>16);                  //write dummy Address
    inportb(SPDR);
    outportb(SPDR, Raddr>>8);                  //write dummy Address
    inportb(SPDR);
    outportb(SPDR, Raddr);                  //write A7-A0 0x00

    outportb(SPDR, 0x00);
    inportb(SPDR);

    while(len--){
        outportb(SPDR, 0x00);
        *data_in++ = inportb(SPDR);                    //write dummy data
     }

    inportb(SPDR);
    outportb(SPCR, inportb(SPCR)|0x70);      //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash
    enable();

    wait_till_ready();
#endif

}
//---------------------------------------------------------------------------
void read25x32id(void)
{
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int enable, spi enable, ss=0, mstr =1
    outportb(SPDR, 0x90);                  //read ID command
    inportb(SPDR);
    outportb(SPDR, 0x00);                  //write dummy Address
    inportb(SPDR);
    outportb(SPDR, 0x00);                  //write dummy Address
    inportb(SPDR);
    outportb(SPDR, 0x00);                  //write A7-A0 0x00
    inportb(SPDR);

    outportb(SPDR, 0x00);                  //write dummy data
//    outportb(UART0_TX_DATA, inportb(SPDR));  //read M7-M0 ==0xef
		inportb(SPDR);
    outportb(SPDR, 0x00);                    //write dummy data
//    outportb(UART0_TX_DATA, inportb(SPDR));  //read ID7-ID0 ==0x15
		inportb(SPDR);
    outportb(SPCR, inportb(SPCR)|0x70);      //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash
}
//---------------------------------------------------------------------------
void read25x32_unique_id(INT8U *flash_id)
{
    INT8U i;
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int enable, spi enable, ss=0, mstr =1
    outportb(SPDR, 0x4B);                  //read Unique ID command
    inportb(SPDR);
    outportb(SPDR, 0x00);                  //write dummy Address
    inportb(SPDR);
    outportb(SPDR, 0x00);                  //write dummy Address
    inportb(SPDR);
    outportb(SPDR, 0x00);                  //write dummy Address
    inportb(SPDR);
    outportb(SPDR, 0x00);                  //write dummy Address
    inportb(SPDR);

    for(i=0; i<8; i++)
     {
      outportb(SPDR, 0x00);                  //write dummy data
      flash_id[i] = inportb(SPDR);
      //xtm_printf("i = %d, inportb(SPDR) = 0x%X\n", i, inportb(SPDR));
     }

    outportb(SPCR, inportb(SPCR)|0x70);      //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash
}
//---------------------------------------------------------------------------
void w25x32writeenable(void)
{
 		disable();
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1

    outportb(SPDR, 0x06);                  //write enable command
    inportb(SPDR);

    outportb(SPCR, inportb(SPCR)|0x70);      //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash
		enable();
}
//---------------------------------------------------------------------------
void w25x32writedisable(void)
{
		disable();
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int enable, spi enable, ss=0, mstr =1

    outportb(SPDR, 0x04);                  //write disable command
    inportb(SPDR);

    outportb(SPCR, inportb(SPCR)|0x70);      //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash
		enable();
}
//---------------------------------------------------------------------------
void wait_till_ready(void)
{
    INT16U i;
    INT8U tmp;
	      	disable();
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1

    outportb(SPDR, 0x05);                  //read status register command
    inportb(SPDR);
    tmp = 0x01;

    i = 0;
    while((tmp & 0x01) == 0x01)                         //write status register
     {
      asm{
               nop
               nop
               nop
               nop
               nop
               nop
               nop
               nop
               nop
               nop
          }
      outportb(SPDR, 0x00);                  //write dummy Address
      tmp = inportb(SPDR);
      i++;
      if(i > 40000)
       {
        xtm_printf("wait_till_write_ready:: timeout, 0x%X, i = %d\n", tmp, i);
        break;
       }
     }

    outportb(SPCR, inportb(SPCR)|0x70);      //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash
	       	enable();

}
//---------------------------------------------------------------------------
INT8U read25x32_status(void)
{
    INT8U tmp;
	      	disable();
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1

    outportb(SPDR, 0x05);                  //write status register command
    inportb(SPDR);

    outportb(SPDR, 0x00);                  //write dummy Address
    tmp = inportb(SPDR);


    outportb(SPCR, inportb(SPCR)|0x70);      //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash
	       	enable();
    return tmp;
             //   xtm_printf("wait_till_write_ready:: timeout, 0x%X\n", tmp);
}
//---------------------------------------------------------------------------
void write25x32status(INT8U reg)
{
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1

    outportb(SPDR, 0x01);                  //write status register command
    inportb(SPDR);

    outportb(SPDR, reg);
    inportb(SPDR);                           //write status register

    outportb(SPCR,  0xf0);                   //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash

}
//---------------------------------------------------------------------------
void erasechip25x32(void)
{

    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1 
   
    outportb(SPDR, 0xc7);                  //erase chip command
    inportb(SPDR);
    
    
    outportb(SPCR, inportb(SPCR)|0x70);    //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);     //sel spi flash	   

}

//---------------------------------------------------------------------------
void programpage25x32(INT32U paddr, INT8U *data, INT16U len)
{
    INT16U i;

    ClearWatchDog();
    //xtm_printf("inportb(SPSR)&0x40 = 0x%X\n", (INT16U)(inportb(SPSR)&0x40));
    wait_till_ready();
    
    disable();
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1
    outportb(SPDR, 0x02);                  //program page 256 bytes data command
    inportb(SPDR);


    outportb(SPDR, paddr>>16);             //write A23-A16 Address
    inportb(SPDR);
    outportb(SPDR, paddr>>8);              //write A15-A8 Address
    inportb(SPDR);
    outportb(SPDR, paddr);                 //write A7-A0 address
    inportb(SPDR);

    for(i=0;i<len;i++){
        outportb(SPDR, *data++);                 //write dummy data
        inportb(SPDR);                     //read chip data
     }

    //等待写 fifo 为空  xu
    //while(inportb(SPSR)&0x01 != 0x01);

    outportb(SPCR, inportb(SPCR)|0x70);      //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash

		enable();

   wait_till_ready();
   ClearWatchDog();
}

//---------------------------------------------------------------------------
void programpage25x32_test(INT32U paddr, INT8U *data, INT16U len)
{
    INT16U i;
    ClearWatchDog();
    //xtm_printf("inportb(SPSR)&0x40 = 0x%X\n", (INT16U)(inportb(SPSR)&0x40));
    //等待写 fifo 为空  xu
    //while(inportb(SPSR)&0x04 != 0x04);

    //busy
    //while((inportb(SPSR)&0x01) == 0x01);

   // if((inportb(SPSR)&0x40) == 0x40)


    disable();
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1
    outportb(SPDR, 0x02);                  //program page 256 bytes data command
    //inportb(SPDR);
    outportb(SPDR, paddr>>16);             //write A23-A16 Address
    //inportb(SPDR);
    outportb(SPDR, paddr>>8);              //write A15-A8 Address
    //inportb(SPDR);
    outportb(SPDR, paddr);                 //write A7-A0 address
    //inportb(SPDR);
    
    for(i=0;i<len;i++){
        outportb(SPDR, *data++);                 //write dummy data
        //inportb(SPDR);                     //read chip data
     }
    outportb(SPCR, inportb(SPCR)|0x70);      //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash

    //busy
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1
    outportb(SPDR, 0x05);                  //read status register command
    while((inportb(SPDR)&0x01)==0x01);        // read status register
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash    
    
    enable();
}

//---------------------------------------------------------------------------
void blockerase25x32(INT32U paddr)   //erase 64k
{
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int enable, spi enable, ss=0, mstr =1 
    outportb(SPDR, 0xd8);                  //block erase command
    inportb(SPDR);
    outportb(SPDR, paddr>>16);             //write A23-A16 Address
    inportb(SPDR);
    outportb(SPDR, paddr>>8);              //write A15-A8 Address
    inportb(SPDR);
    outportb(SPDR, paddr);                 //write A7-A0 address
    inportb(SPDR);
    
    outportb(SPCR, inportb(SPCR)|0x70);      //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash	

}
//---------------------------------------------------------------------------
void sectererase25x32(INT32U paddr)   // erase 4k
{
    ClearWatchDog();
    wait_till_ready();
    
    disable();
    outportb(BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(SPCR, 0x50);                  //int enable, spi enable, ss=0, mstr =1 
    outportb(SPDR, 0x20);                  //secter erase command
    inportb(SPDR);
    outportb(SPDR, paddr>>16);             //write A23-A16 Address
    inportb(SPDR);
    outportb(SPDR, paddr>>8);              //write A15-A8 Address
    inportb(SPDR);
    outportb(SPDR, paddr);                 //write A7-A0 address
    inportb(SPDR);
//    while(i&1){
//    	outportb(SPDR, 0);                 	//write dummy data
//   	i=inportb(SPDR);
//    }
    outportb(SPCR, inportb(SPCR)|0x70);      //ss=1
    outportb(BITBLK_CONTROL_REG, 0x00);      //sel spi flash
		enable();

    wait_till_ready();
}
//---------------------------------------------------------------------------
void XTM_SPI_DRAM(INT32U src_addr, INT32U dest_addr, INT32U len)
{
//xtm_printf("XTM_SPI_DRAM..............................\n");
       ClearWatchDog();
       WaitDecodeFinish();
       wait_till_ready();
      	disable();
     	while((inportb(BITBLK_STATUS_REG)&0x01)!=0x01)	// 等待块传送完成
         {
          asm{
               nop
               nop
               nop
               nop
               nop
             }
         }
        //xtm_printf("STATUS_REG = 0x%X, BITBLK_CONTROL_REG = 0x%X\n", inportb(STATUS_REG), inportb(BITBLK_CONTROL_REG));
	outportb(BITBLK_SA_H, (src_addr>>16) & 0xff);
	outportb(BITBLK_SA_M, (src_addr>>8) & 0xff);
	outportb(BITBLK_SA_L, src_addr & 0xff);

	outportb(BITBLK_DA_H, (dest_addr>>16) & 0xff);
	outportb(BITBLK_DA_M, (dest_addr>>8) & 0xff);
	outportb(BITBLK_DA_L, dest_addr  & 0xff);

	outportb(BITBLK_X_SIZE_L, len & 0xff);
	outportb(BITBLK_X_SIZE_H, (len>>8) & 0xff);

	outportb(BITBLK_Y_SIZE_L, (len>>16) & 0xff);
	outportb(BITBLK_Y_SIZE_H, 0x00);

	outportb(BITBLK_CONTROL_REG, 0x05);
      	while((inportb(BITBLK_STATUS_REG)&0x01)!=0x01)	// 等待块传送完成
         {
          asm{
               nop
               nop
               nop
               nop
               nop
             }
         }
      	enable();
        wait_till_ready();
}
//---------------------------------------------------------------------------
