#include    <dos.h>

typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef unsigned int   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */

#define BOOT_CPU_MAIN_FREQ   49152000     //50000000
//UART0
#define BOOT_UART0_TX_STATUS 0xff80      //UART0����״̬�Ĵ���
#define BOOT_UART0_TX_DATA   0xff81      //UART0�������ݼĴ���
#define BOOT_UART0_RX_CTRL   0xff82      //UART0���տ��ƼĴ���
#define BOOT_UART0_RX_DATA       0xff83  //UART0�������ݼĴ���

//λ�飨BitBlk������
#define BOOT_BITBLK_CONTROL_REG	0xfe80	//λ����ƼĴ���
//D0:	λ�鴫������
//D1��	͸��ʹ��
//D2��͸����ʽѡ��0���������λΪ��͸����1������ȫ��͸��
//D3: 0��Դ��SPI FLASH��1��Դ��SDRAM
//D4: 0��SDRAM��SPI FLASH֮�䴫��; 1��SDRAM�ڲ�����
//D5: SPI_sel, 0: SPI FLASH��SPI FLASH������ͱ�����; 1: SPI������
//D6: λ�鴫������ж�(cpu�жϺ�21��ʹ�ܣ�0�����λ�鴫������жϣ�1��λ�鴫������ж�ʹ��

#define BOOT_SPCR  0xffb0              //SPI ���ƼĴ���  Ĭ��ֵ 0x10
#define BOOT_SPDR  0xffb2              //SPI ���ݼĴ���

#define BOOT_DOWNLOADFILE  224    //����
#define BOOT_ASK              1     //�������� ����
#define BOOT_REPLY            2     //�������� Ӧ��
#define BOOT_STARTDOWN            3     //�������� ��������


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
void Boot_comm_upgrade(void);    //20110608 xu ��ʼ����

//void boot_delay(unsigned milliseconds);

void boot_w25x32writeenable(void);
void boot_w25x32writedisable(void);
void boot_read25x32status(void);
void boot_sectererase25x32(INT32U paddr);
void boot_programpage25x32(INT32U paddr,INT8U *data,INT16U len);
void boot_wait_till_ready(void);

//GPIO A
#define BOOT_GPIOA_OUT_DATA  0x02d0		//GPIO A ������ݼĴ���

#define BOOT_GPIOA_OEN_DATA  0x02d1		//GPIO A ���ʹ�ܷǼĴ���
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
  boot_send_comm[4] = BOOT_STARTDOWN;             //�������� ��������;
  crc = 0;
  for(i=1; i<(sendlength - 1); i++)
    crc += boot_send_comm[i];
  boot_send_comm[sendlength - 1] = crc;

  outportb(BOOT_UART0_TX_DATA, 0x66);
  outportb(BOOT_UART0_TX_DATA, 0x77);
  boot_down_flag = 0;
  for(i=0; i<sendlength; i++)
   {
    while((inportb(BOOT_UART0_TX_STATUS)&0x1)!=0x1);	// �ȴ����ͱ�����Ϊ��
    outportb(BOOT_UART0_TX_DATA, boot_send_comm[i]);				// �����ַ�
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

        gen = BOOT_CPU_MAIN_FREQ/baud/freq;   //20120222 �ľ���

   outportb(port+4, (gen&0xff));
   outportb(port+5, (gen>>8));
   outportb(port+2, fifolevel);

  dtemp_valid = 0;
  dtemp_pos = 0;

}
//---------------------------------------------------------------------------
void interrupt Boot_IdleInt()
{
  outportb(BOOT_UART0_TX_DATA, 16);				// �����ַ�
}
//---------------------------------------------------------------------------
void Boot_comm_upgrade(void)    //20110608 xu ��ʼ����
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
             if(recv_byte == 0xAA)      //����ʼ
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
            if(dtemp_pos == (dtemp_len + 3))   //���յ�һ����
             {
               crc = 0;
               for(i=1; i<(dtemp_pos - 1); i++)
                 crc += boot_dtemp[i];
               if(boot_dtemp[dtemp_pos - 1] == crc)
                {
                 boot_dtime = 0;
                 boot_down_flag = 1;

                 //д flash
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
                   while((inportb(BOOT_UART0_TX_STATUS)&0x1)!=0x1);	// �ȴ����ͱ�����Ϊ��
                   outportb(BOOT_UART0_TX_DATA, boot_dtemp[i]);				// �����ַ�
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
  if(boot_GpioWatchDogStarted)  //��GPIO����߳�������WatchDog
   {
    outportb(BOOT_GPIOA_OEN_DATA, inportb(BOOT_GPIOA_OEN_DATA) & 0xfb);       //gpioa2Ϊ���
    tmp_data = inportb(BOOT_GPIOA_OUT_DATA);
    if(tmp_data & 0x04)
      outportb(BOOT_GPIOA_OUT_DATA, tmp_data & 0xfb);
    else
      outportb(BOOT_GPIOA_OUT_DATA, tmp_data | 0x04);
   }
}
//--------------------------------------------------------------------------
//copy /B ..\bin\sound.bin + ..\bin\sound.bin + ..\bin\sound.bin + ..\bin\sound.bin ..\bin\8130.bin