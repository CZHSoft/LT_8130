#include    <dos.h>

typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef unsigned int   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */

#define BOOT_CPU_MAIN_FREQ   49152000     //50000000
//UART0
#define BOOT_UART0_TX_STATUS 0xff80      //UART0发送状态寄存器
#define BOOT_UART0_TX_DATA   0xff81      //UART0发送数据寄存器
#define BOOT_UART0_RX_CTRL   0xff82      //UART0接收控制寄存器
#define BOOT_UART0_RX_DATA       0xff83  //UART0接收数据寄存器

//位块（BitBlk）传送
#define BOOT_BITBLK_CONTROL_REG	0xfe80	//位块控制寄存器
//D0:	位块传送启动
//D1：	透明使能
//D2：透明方式选择，0：数据最高位为零透明；1：数据全零透明
//D3: 0：源自SPI FLASH；1：源自SDRAM
//D4: 0：SDRAM与SPI FLASH之间传送; 1：SDRAM内部传送
//D5: SPI_sel, 0: SPI FLASH（SPI FLASH读命令和编程命令）; 1: SPI控制器
//D6: 位块传送完成中断(cpu中断号21）使能，0：清除位块传送完成中断；1：位块传送完成中断使能

#define BOOT_SPCR  0xffb0              //SPI 控制寄存器  默认值 0x10
#define BOOT_SPDR  0xffb2              //SPI 数据寄存器

#define BOOT_DOWNLOADFILE  224    //下载
#define BOOT_ASK              1     //命令类型 主叫
#define BOOT_REPLY            2     //命令类型 应答
#define BOOT_STARTDOWN            3     //命令类型 请求下载


extern INT16U boot_dtime;
extern INT8U boot_down_flag;

extern INT8U dtemp_valid;
extern INT16U dtemp_pos;
extern INT16U dtemp_len;
extern INT8U boot_dtemp[288];

/*INT16U boot_dtime;
extern INT8U boot_down_flag;

INT8U dtemp_valid;
INT16U dtemp_pos;
INT16U dtemp_len;
INT8U boot_dtemp[512];   */

void Boot_InitComm(INT16U, INT32U, INT8U, INT8U, INT8U);
void interrupt Boot_IdleInt();
void Boot_comm_upgrade(void);    //20110608 xu 初始升级

//void boot_delay(unsigned milliseconds);

void boot_w25x32writeenable(void);
void boot_w25x32writedisable(void);
void boot_read25x32status(void);
void boot_sectererase25x32(INT32U paddr);
void boot_programpage25x32(INT32U paddr,INT8U *data,INT16U len);
void boot_wait_till_ready(void);

//GPIO A
#define BOOT_GPIOA_OUT_DATA  0x02d0		//GPIO A 输出数据寄存器

#define BOOT_GPIOA_OEN_DATA  0x02d1		//GPIO A 输出使能非寄存器
INT8U boot_GpioWatchDogStarted = 1;
void boot_ClearWatchDog(void);
//---------------------------------------------------------------------------
void loader(void)
{

  INT8U boot_send_comm[10];
  INT8U sendlength, crc, i;

  Boot_InitComm(BOOT_UART0_TX_STATUS, 57600, 3, 0xc7, 1);         // 0xc7

  sendlength = 6;//263;
  boot_send_comm[0] = 0xAA;
  boot_send_comm[1] = (sendlength - 3) / 256;
  boot_send_comm[2] = (sendlength - 3) % 256;
  boot_send_comm[3] = BOOT_DOWNLOADFILE;
  boot_send_comm[4] = BOOT_STARTDOWN;             //命令类型 请求下载;
  crc = 0;
  for(i=1; i<(sendlength - 1); i++)
    crc += boot_send_comm[i];
  boot_send_comm[sendlength - 1] = crc;

  outportb(BOOT_UART0_TX_DATA, 0x66);
  outportb(BOOT_UART0_TX_DATA, 0x77);
  boot_down_flag = 0;
  for(i=0; i<sendlength; i++)
   {
    while((inportb(BOOT_UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
    outportb(BOOT_UART0_TX_DATA, boot_send_comm[i]);				// 发送字符
   }
  boot_dtime = 0;
  while(1)
   {
    Boot_comm_upgrade();
    if(boot_down_flag == 0)
      boot_dtime ++;
    if(boot_dtime >= 10000)
      break;
   }
}
//---------------------------------------------------------------------------
void Boot_InitComm(INT16U port, INT32U baud, INT8U datafrm, INT8U fifolevel, INT8U freq)
{
	INT16U gen;
    /* set baudrate */

        gen = BOOT_CPU_MAIN_FREQ/baud/freq;   //20120222 改晶振

   outportb(port+4, (gen&0xff));
   outportb(port+5, (gen>>8));
   outportb(port+2, fifolevel);

  dtemp_valid = 0;
  dtemp_pos = 0;

}
//---------------------------------------------------------------------------
void interrupt Boot_IdleInt()
{
  outportb(BOOT_UART0_TX_DATA, 16);				// 发送字符
}
//---------------------------------------------------------------------------
void Boot_comm_upgrade(void)    //20110608 xu 初始升级
{
  INT8U recv_byte, crc;
  INT16U CurrPackage;
  INT16U i;
  INT32U padd;

 	while((inportb(BOOT_UART0_RX_CTRL)&0x10)==0x10)
         {
          boot_ClearWatchDog();
          recv_byte = inportb(BOOT_UART0_RX_DATA);
          if(dtemp_valid == 0)
           {
             if(recv_byte == 0xAA)      //包开始
              {
               dtemp_valid = 1;
               dtemp_pos = 0;
               boot_dtemp[dtemp_pos] = recv_byte;
               dtemp_pos ++;
              }
           }
          else
           {
            boot_dtemp[dtemp_pos] = recv_byte;
            dtemp_pos ++;
            if(dtemp_pos == 3)
             {
              dtemp_len = boot_dtemp[1];
              dtemp_len <<= 8;//*256
              dtemp_len += boot_dtemp[2];
             }
            if(dtemp_pos == (dtemp_len + 3))   //接收到一个包
             {
               crc = 0;
               for(i=1; i<(dtemp_pos - 1); i++)
                 crc += boot_dtemp[i];
               if(boot_dtemp[dtemp_pos - 1] == crc)
                {
                 boot_dtime = 0;
                 boot_down_flag = 1;

                 //写 flash
                 CurrPackage = boot_dtemp[5];
                 CurrPackage <<= 8;//*256
                 CurrPackage += boot_dtemp[6];
                 padd = (INT32U)CurrPackage * 256;
                 if (!(CurrPackage%16))       //4096
                  {
                   boot_w25x32writeenable();
                   boot_sectererase25x32(padd);
                   boot_read25x32status();
                   //boot_delay(10);
                  }
                 boot_w25x32writeenable();
                 boot_programpage25x32(padd, boot_dtemp + 7, 256);
                 boot_w25x32writedisable();
                 //boot_delay(10);

                 boot_dtemp[4] = BOOT_REPLY;
                 crc = 0;
                 for(i=1; i<(dtemp_pos - 1); i++)
                   crc += boot_dtemp[i];
                 boot_dtemp[dtemp_pos - 1] = crc;
                 for(i=0; i<dtemp_pos; i++)
                  {
                   while((inportb(BOOT_UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
                   outportb(BOOT_UART0_TX_DATA, boot_dtemp[i]);				// 发送字符
                  }
                }
               dtemp_valid = 0;
               dtemp_pos = 0;
             }
           }
	 }
}
//---------------------------------------------------------------------------
/*void boot_delay(unsigned milliseconds)
{
	asm mov cx, milliseconds
   dl0:
	asm push cx
   	asm mov cx, 193
   dl1:
   	asm nop
   	asm nop
   	asm loop dl1
 //  	ResetWdt();
   	asm pop cx
   	asm loop dl0
}                        */
//---------------------------------------------------------------------------
void boot_w25x32writeenable(void)
{
 		disable();
    outportb(BOOT_BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(BOOT_SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1

    outportb(BOOT_SPDR, 0x06);                  //write enable command
    inportb(BOOT_SPDR);

    outportb(BOOT_SPCR, inportb(BOOT_SPCR)|0x70);      //ss=1
    outportb(BOOT_BITBLK_CONTROL_REG, 0x00);      //sel spi flash
		enable();
}
//---------------------------------------------------------------------------
void boot_w25x32writedisable(void)
{
		disable();
    outportb(BOOT_BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(BOOT_SPCR, 0x50);                  //int enable, spi enable, ss=0, mstr =1

    outportb(BOOT_SPDR, 0x04);                  //write disable command
    inportb(BOOT_SPDR);

    outportb(BOOT_SPCR, inportb(BOOT_SPCR)|0x70);      //ss=1
    outportb(BOOT_BITBLK_CONTROL_REG, 0x00);      //sel spi flash
		enable();
}
//---------------------------------------------------------------------------
void boot_read25x32status(void)
{
		disable();
    outportb(BOOT_BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(BOOT_SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1

    outportb(BOOT_SPDR, 0x05);                  //read status register command
    inportb(BOOT_SPDR);

    outportb(BOOT_SPDR, 0x00);
    while(inportb(BOOT_SPDR)&0x01==0x01) {       // read status register
           outportb(BOOT_SPDR, 0x00);
      }

    outportb(BOOT_SPCR, inportb(BOOT_SPCR)|0x70);     //ss=1
    outportb(BOOT_BITBLK_CONTROL_REG, 0x00);     //sel spi flash
		enable();
}
//---------------------------------------------------------------------------
void boot_programpage25x32(INT32U paddr, INT8U *data, INT16U len)
{
    INT16U i;

    //xtm_printf("inportb(SPSR)&0x40 = 0x%X\n", (INT16U)(inportb(SPSR)&0x40));
    boot_wait_till_ready();

    disable();
    outportb(BOOT_BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(BOOT_SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1
    outportb(BOOT_SPDR, 0x02);                  //program page 256 bytes data command
    inportb(BOOT_SPDR);
    outportb(BOOT_SPDR, paddr>>16);             //write A23-A16 Address
    inportb(BOOT_SPDR);
    outportb(BOOT_SPDR, paddr>>8);              //write A15-A8 Address
    inportb(BOOT_SPDR);
    outportb(BOOT_SPDR, paddr);                 //write A7-A0 address
    inportb(BOOT_SPDR);
    
    for(i=0;i<len;i++){ 
//    		i++;
//    		i--;
        outportb(BOOT_SPDR, *data++);                 //write dummy data
        inportb(BOOT_SPDR);                     //read chip data  
     }
    outportb(BOOT_SPCR, inportb(BOOT_SPCR)|0x70);      //ss=1
    outportb(BOOT_BITBLK_CONTROL_REG, 0x00);      //sel spi flash	
		enable();
    boot_wait_till_ready();
}

//---------------------------------------------------------------------------
void boot_sectererase25x32(INT32U paddr)   // erase 4k
{
    disable();
    outportb(BOOT_BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(BOOT_SPCR, 0x50);                  //int enable, spi enable, ss=0, mstr =1 
    outportb(BOOT_SPDR, 0x20);                  //secter erase command
    inportb(BOOT_SPDR);
    outportb(BOOT_SPDR, paddr>>16);             //write A23-A16 Address
    inportb(BOOT_SPDR);
    outportb(BOOT_SPDR, paddr>>8);              //write A15-A8 Address
    inportb(BOOT_SPDR);
    outportb(BOOT_SPDR, paddr);                 //write A7-A0 address
    inportb(BOOT_SPDR);

    outportb(BOOT_SPCR, inportb(BOOT_SPCR)|0x70);      //ss=1
    outportb(BOOT_BITBLK_CONTROL_REG, 0x00);      //sel spi flash	
		enable();
    boot_wait_till_ready();            
}
//---------------------------------------------------------------------------
void boot_wait_till_ready(void)
{
    INT16U i;
    INT8U tmp;
	      	disable();
    outportb(BOOT_BITBLK_CONTROL_REG, 0x20);    //sel spi controller
    outportb(BOOT_SPCR, 0x50);                  //int disable, spi enable, ss=0, mstr =1

    outportb(BOOT_SPDR, 0x05);                  //read status register command
    inportb(BOOT_SPDR);
    tmp = 0x01;

    i = 0;
    while((tmp & 0x01) == 0x01)                         //write status register
     {
      outportb(BOOT_SPDR, 0x00);                  //write dummy Address
      tmp = inportb(BOOT_SPDR);
      i++;
      if(i > 30000)
       {
        break;
       }
     }

    outportb(BOOT_SPCR, inportb(BOOT_SPCR)|0x70);      //ss=1
    outportb(BOOT_BITBLK_CONTROL_REG, 0x00);      //sel spi flash
	       	enable();

}
//---------------------------------------------------------------------------
void boot_ClearWatchDog(void)
{
  INT8U tmp_data;
  if(boot_GpioWatchDogStarted)  //在GPIO检测线程中启动WatchDog
   {
    outportb(BOOT_GPIOA_OEN_DATA, inportb(BOOT_GPIOA_OEN_DATA) & 0xfb);       //gpioa2为输出
    tmp_data = inportb(BOOT_GPIOA_OUT_DATA);
    if(tmp_data & 0x04)
      outportb(BOOT_GPIOA_OUT_DATA, tmp_data & 0xfb);
    else
      outportb(BOOT_GPIOA_OUT_DATA, tmp_data | 0x04);
   }
}
//--------------------------------------------------------------------------
//copy /B ..\bin\sound.bin + ..\bin\sound.bin + ..\bin\sound.bin + ..\bin\sound.bin ..\bin\8130.bin
