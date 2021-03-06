#include "includes.h"

#include "stdlib.h"

#define DM9161_MDC_H 0x02     /* GPIOA.1 */                              
#define DM9161_MDC_L 0xfd
#define DM9161_MDIO_H 0x01     /* GPIOA.0 */
#define DM9161_MDIO_L 0xfe

#define x_net_h

#define CommonH
#include "common.h"

/*
****************************************
20131119
暂无修改
****************************************
*/


INT8U packet_num_last;
void 	mac_init(void);
void clear_mac_ram(void);
#if 1
void 	phy_init(void);
INT16U phy_reg_read(INT16U phy_addr, INT16U reg_no);
void phy_reg_write(INT16U phy_addr, INT16U reg_no, INT16U val);
#endif

void interrupt MACInt(void);

void mac_int_deal(void);
//INT16U  mac_len;
void mac_recv(INT8U r_pos);

INT8U *Mdata;

INT8U macr, macw;

void printf_mac_recv(void);
//---------------------------------------------------------------------------
void clear_mac_ram(void)
{
	INT16U i,j;
	INT16U color_val, bcolor_val;

	outportb(FONT_WR_MOST_ADD, 0x18);
	outportb(FONT_WR_HIGH_ADD, 0);
	outportb(FONT_WR_LOW_ADD, 0);

	color_val = 0;
	bcolor_val = 0;
	outportb(FGND_LOW, color_val);
	outportb(FGND_HIGH, (color_val>>8));
	outportb(BGND_LOW, bcolor_val);
	outportb(BGND_HIGH, (bcolor_val>>8));

	for(i=0;i<32768;i++)
	{
		outportb(FONT_WR_DATA, 0xff);
	}
}
//---------------------------------------------------------------------------
void 	mac_init(void)
{
  
#if 1
	INT16U i;
        INT32U Maddr1;
//	INT16U val;
        clear_mac_ram();

        InitArp_Table();

        xtm_printf("mac_init::\n");
       #if 0
        LocalCfg.Mac_Addr[0] = 0x02;
        LocalCfg.Mac_Addr[1] = 0x50;
        LocalCfg.Mac_Addr[2] = 0x2A;
        LocalCfg.Mac_Addr[3] = 0x22;
        LocalCfg.Mac_Addr[4] = 0x44;
        LocalCfg.Mac_Addr[5] = 0x66;
       #endif

	outportb(PAR0, LocalCfg.Mac_Addr[5]);
	outportb(PAR1, LocalCfg.Mac_Addr[4]);
	outportb(PAR2, LocalCfg.Mac_Addr[3]);
	outportb(PAR3, LocalCfg.Mac_Addr[2]);
	outportb(PAR4, LocalCfg.Mac_Addr[1]);
	outportb(PAR5, LocalCfg.Mac_Addr[0]);
//MAR 多播包KEN
	outportb(MAR0, 0x1);
	outportb(MAR1, 0x09);
	outportb(MAR2, 0x09);//T
	outportb(MAR3, 0x5E);//N
	outportb(MAR4, 0x00);//E
	outportb(MAR5, 0x01);//K
	outportb(MAR6, 0xff);
	outportb(MAR7, 0xff);


	outportb(LIP0, LocalCfg.IP[3]);
	outportb(LIP1, LocalCfg.IP[2]);
	outportb(LIP2, LocalCfg.IP[1]);
	outportb(LIP3, LocalCfg.IP[0]);

	outportb(0xfe40, 0xff);//PPAR
	outportb(0xfe41, 0xff);
	outportb(0xfe42, 0xff);
	outportb(0xfe43, 0xff);
	outportb(0xfe44, 0xff);
	outportb(0xfe45, 0xff);

	outportb(0xfe46, LocalCfg.IP_Broadcast[3]);//0xff);//PIP
	outportb(0xfe47, LocalCfg.IP_Broadcast[2]);
	outportb(0xfe48, LocalCfg.IP_Broadcast[1]);
	outportb(0xfe49, LocalCfg.IP_Broadcast[0]);

	outportb(0xfe4a, 0xa0);//Pport
	outportb(0xfe4b, 0x08);


	outportb(0xfe4c, 0x08);//Playload Type A law voice

	packet_num_last=31;
	outportb(0xfe4d, 0x00);//Playload Type


        #ifdef RX_CONTROL_NUM_128
         outportb(MAC_RX_SA_H, 0x74);  //网卡接收缓冲在 0x740000 位置
        #endif

  for(i=0; i<100; i++)
    Mdata_Stream[i] = i+20;
  for(i=0; i<100; i++)
    Send_Stream[i] = 0;

  Maddr1 = (((INT32U)Mdata_Stream&0xffff)+(((INT32U)Mdata_Stream&0xffff0000)>>12))|0x700000;


  fmem_insb_dma(Maddr1, Send_Stream, 100);

       #ifdef RX_CONTROL_NUM_128
        //outportb(MAC_RX_SA_H, 0x74);
        //xtm_printf("MAC_RX_SA_H = 0x74\n");
       #endif

  outportb(RX_CONTROL, RX_CONTROL_EN_RCV); //clear mac rx input INT
	
  i = inportb(RX_PACKET_NUM);
  macr = macw = i;
  xtm_printf("mac_init::macw = %d, macr = %d\n", macw, macr);
  outportb(RX_CONTROL, RX_CONTROL_EN_INT); //enable mac rx input INT
  //xtm_printf("mac_init::1\n");
  //发送免费ARP
  SendFreeArp();
  //xtm_printf("mac_init::2\n");
#endif
}
//---------------------------------------------------------------------------
void interrupt MACInt()
{
  outportb(RX_CONTROL, RX_CONTROL_EN_RCV); //clear mac rx input INT
  Vflag=1;
  //mac_int_deal(); 
  outportb(RX_CONTROL, RX_CONTROL_EN_INT); //enable mac rx input INT
}
//---------------------------------------------------------------------------
#if 1
void 	phy_init(void)
{
  INT16U val;
  INT8U i;
  val = phy_reg_read(0, 0);
  xtm_printf("read phy_reg = 0x%X\n",  val);
  val &=0x7bff;                  // enable
  phy_reg_write(0, 0, val);


#if 0  //IP101G
  phy_reg_write(0, 20, 0x0004);
  phy_reg_write(0, 22, 0xa000);
  phy_reg_write(0, 20, 0x0010);
  phy_reg_write(0, 26, 0x5b6d);
  phy_reg_write(0, 27, 0x0015);
#endif
}
//---------------------------------------------------------------------------
INT16U phy_reg_read(INT16U phy_addr, INT16U reg_no)
{
	int i;
	INT16U val;
        INT8U tmp_gpio;
	val=0;
  outportb(0x2d1, inportb(0x2d1) & 0xFC);      //d0 (io)  d1(clk) output
  tmp_gpio = inportb(0x2d0);
	tmp_gpio|=DM9161_MDC_H;
	tmp_gpio|=DM9161_MDIO_H;
  outportb(0x2d0, tmp_gpio);
	for(i=0;i<32;i++)//preample 32 bit '1'
	{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}
	tmp_gpio&=DM9161_MDC_L;
	tmp_gpio&=DM9161_MDIO_L;
	outportb(0x2d0, tmp_gpio); //0xfc;
	tmp_gpio|=DM9161_MDC_H;
	tmp_gpio&=DM9161_MDIO_L;
	outportb(0x2d0, tmp_gpio); //0xfe;
	tmp_gpio&=DM9161_MDC_L;
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xfd;
	tmp_gpio|=DM9161_MDC_H;
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xff;
	tmp_gpio&=DM9161_MDC_L;
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xfd;
	tmp_gpio|=DM9161_MDC_H;
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xff;
	tmp_gpio&=DM9161_MDC_L;
	tmp_gpio&=DM9161_MDIO_L;
	outportb(0x2d0, tmp_gpio); //0xfc;
	tmp_gpio|=DM9161_MDC_H;
	tmp_gpio&=DM9161_MDIO_L;
	outportb(0x2d0, tmp_gpio); //0xfe;

	if(phy_addr & 0x10){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(phy_addr & 0x08){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(phy_addr & 0x04){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(phy_addr & 0x02){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(phy_addr & 0x01){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(reg_no & 0x10){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(reg_no & 0x08){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(reg_no & 0x04){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(reg_no & 0x02){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(reg_no & 0x01){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}
	
  nop();
  outportb(0x2d1, inportb(0x2d1) | 0x01);
  nop();
  nop();
  nop();
  nop();

	tmp_gpio&=DM9161_MDC_L;
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xfd;
	tmp_gpio|=DM9161_MDC_H;
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xff;

	asm cli;
	for(i=0;i<16;i++)//read data
	{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
		
		val<<=1;
	  if(0x01 & inportb(0x2d2)) val= val | 0x01;
	}
	asm cli;

	tmp_gpio&=DM9161_MDC_L;
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xfd;
	tmp_gpio|=DM9161_MDC_H;
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xff;

	asm sti;
	return val;
	
}


void phy_reg_write(INT16U phy_addr, INT16U reg_no, INT16U val)
{
	int i;
        INT8U tmp_gpio;

  outportb(0x2d1, inportb(0x2d1) & 0xFC);      //d0 (io)  d1(clk) output
  tmp_gpio = inportb(0x2d0);
	tmp_gpio|=DM9161_MDC_H;
	tmp_gpio|=DM9161_MDIO_H;
  outportb(0x2d0, tmp_gpio);
	for(i=0;i<32;i++)//preample 32 bit '1'
	{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}
	tmp_gpio&=DM9161_MDC_L;
	tmp_gpio&=DM9161_MDIO_L;
	outportb(0x2d0, tmp_gpio); //0xfc;
	tmp_gpio|=DM9161_MDC_H; 
	tmp_gpio&=DM9161_MDIO_L;
	outportb(0x2d0, tmp_gpio); //0xfe;
	tmp_gpio&=DM9161_MDC_L;
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xfd;
	tmp_gpio|=DM9161_MDC_H; 
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xff;
	tmp_gpio&=DM9161_MDC_L;
	tmp_gpio&=DM9161_MDIO_L;
	outportb(0x2d0, tmp_gpio); //0xfc;
	tmp_gpio|=DM9161_MDC_H; 
	tmp_gpio&=DM9161_MDIO_L;
	outportb(0x2d0, tmp_gpio); //0xfe;
	tmp_gpio&=DM9161_MDC_L;
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xfd;
	tmp_gpio|=DM9161_MDC_H; 
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xff;

	if(phy_addr & 0x10){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(phy_addr & 0x08){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(phy_addr & 0x04){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(phy_addr & 0x02){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(phy_addr & 0x01){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(reg_no & 0x10){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(reg_no & 0x08){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(reg_no & 0x04){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(reg_no & 0x02){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	if(reg_no & 0x01){
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xfd;
		tmp_gpio|=DM9161_MDC_H; 
		tmp_gpio|=DM9161_MDIO_H;
	  outportb(0x2d0, tmp_gpio);//0xff;
	}else{
		tmp_gpio&=DM9161_MDC_L;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfc;
		tmp_gpio|=DM9161_MDC_H;
		tmp_gpio&=DM9161_MDIO_L;
		outportb(0x2d0, tmp_gpio); //0xfe;
	}

	tmp_gpio&=DM9161_MDC_L;
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xfd;
	tmp_gpio|=DM9161_MDC_H;
	tmp_gpio|=DM9161_MDIO_H;
	outportb(0x2d0, tmp_gpio);//0xff;
	tmp_gpio&=DM9161_MDC_L;
	tmp_gpio&=DM9161_MDIO_L;
	outportb(0x2d0, tmp_gpio); //0xfc;
	tmp_gpio|=DM9161_MDC_H; 
	tmp_gpio&=DM9161_MDIO_L;
	outportb(0x2d0, tmp_gpio); //0xfe;

	asm cli;
	for(i=0;i<16;i++)//write data
	{
		if(val & 0x8000){
			tmp_gpio&=DM9161_MDC_L;
			tmp_gpio|=DM9161_MDIO_H;
			outportb(0x2d0, tmp_gpio);//0xfd;
			tmp_gpio|=DM9161_MDC_H; 
			tmp_gpio|=DM9161_MDIO_H;
			outportb(0x2d0, tmp_gpio);//0xff;
		}else{
			tmp_gpio&=DM9161_MDC_L;
			tmp_gpio&=DM9161_MDIO_L;
			outportb(0x2d0, tmp_gpio); //0xfc;
			tmp_gpio|=DM9161_MDC_H;
			tmp_gpio&=DM9161_MDIO_L;
			outportb(0x2d0, tmp_gpio); //0xfe;
		}
		val = val<<1;
	}
	asm sti;


}
#endif
void mac_int_deal(void)
{
  INT8U i,j, k1, k2, k3;

  //fmem_insb(RX_PACKET_NUM, &i, 1);
  i=inportb(RX_PACKET_NUM);
  i = i & MAC_RX_MAX1;

  k1= inportb(0xfe2c);
  k2= inportb(0xfe2d);
  k3= inportb(0xfe2e);

  //if((i != k1)||(i != k2)||(i != k3)||(k1 != k2))
  //  xtm_printf("%d, %d, %d, %d\n", i, k1, k2, k3);
  macw = i;
  if(i > MAC_RX_MAX1)
   {
    xtm_printf("i = %d\n", i);
    macw = 0;
   }
  if(macw == macr)
    xtm_printf("main::macr = %d, macw = %d\n", macr, macw);
  #if 1
   while(macw != macr){
     i =  macw;
     j = macr;
     //xtm_printf("macw = %d, macr = %d\n", i, j);
     //xtm_printf("%d, %d\n", i, j);

     mac_recv(j);
     j ++;
     j = j & MAC_RX_MAX1;
     macr = j;
     //macr=macw;
    }
  #endif
}
//---------------------------------------------------------------------------
void printf_mac_recv(void)
{
  INT8U i, j, k;
  i = macw;
  for(j=0; j<32; j++)
   {
     Mdata = Net_Stream[i >> 5] + ((i & 0x1F) << 11);

     for(k=0; k<42; k++)
       xtm_printf("%X, ", Mdata[k]);

     xtm_printf("%X, %X, %X, %X, %X, %X", Mdata[105], Mdata[106], Mdata[111], Mdata[112], Mdata[113], Mdata[114]);
     xtm_printf("\n");
     if(i == 0)
       i = 127;
     else
       i --;
   }
}
//---------------------------------------------------------------------------
#if 1
void mac_recv(INT8U r_pos)
{

        INT16U  mac_len;
	INT32U Maddr;
        INT8U Pack_Status_L, Pack_Status_H;

        // 指向包状态信息(ofs = 0x7f8)
    /* 	Maddr = r_pos;
	Maddr <<= 11;
        #ifndef RX_CONTROL_NUM_128
 	 Maddr += rx_gen_SA;          
        #else
         Maddr += 0x740000UL;
        #endif
	Maddr += 0x7f8;
        fmem_insb(Maddr, &mac_len, sizeof(mac_len));
        xtm_printf("1 mac_recv::mac_len = %d, macw = %d, macr = %d, r_pos = %d\n", mac_len, macw, macr, r_pos);   */

        Mdata = Net_Stream[r_pos >> 5] + ((r_pos & 0x1F) << 11);
        mac_len = Mdata[0x7f9];
        mac_len <<= 8;
        mac_len += Mdata[0x7f8];

        Pack_Status_L = Mdata[0x7fa];
        Pack_Status_H = Mdata[0x7fb];
        //if(Pack_Status_L & 0x02)
        //  xtm_printf("mac_recv ::crc error, Pack_Status_L = 0x%X\n", Pack_Status_L);

        //          xtm_printf("mac_recv 1::TX_STATUS = 0x%X\n", inportb(TX_STATUS));
        //          xtm_printf("mac_recv 2::RX_PACKET_STATUS_H = 0x%X\n", inportb(RX_PACKET_STATUS_H));        

        //xtm_printf("mac_len = %d, r_pos = %d, r_pos>>5 = %d, Mdata[0x7f8] = %d, Mdata[0x7f9] = %d\n", mac_len, r_pos, r_pos >> 5, Mdata[0x7f8], Mdata[0x7f9]);
	if (mac_len > 1518)
         {
          xtm_printf("mac_recv::mac_len = %d, macw = %d, macr = %d, r_pos = %d\n", mac_len, macw, macr, r_pos);

	  return;
         }
        if (mac_len == 0)
         {
          xtm_printf("mac_recv::mac_len = %d, macw = %d, macr = %d, r_pos = %d, r_pos >> 5 = %d, 0x%X\n", mac_len, macw, macr, r_pos, (r_pos >> 5), ((r_pos & 0x1F) << 11));
      	  Maddr = r_pos;
	  Maddr <<= 11;
          #ifndef RX_CONTROL_NUM_128
 	   Maddr += rx_gen_SA;
          #else
           Maddr += 0x740000UL;
          #endif
	  Maddr += 0x7f8;
          fmem_insb(Maddr, &mac_len, sizeof(mac_len));
          xtm_printf("1 mac_recv::mac_len = %d, macw = %d, macr = %d, r_pos = %d\n", mac_len, macw, macr, r_pos);

          Mdata = Net_Stream[r_pos >> 5] + ((r_pos & 0x1F) << 11);

          xtm_printf("Mdata = 0x%X %X %X %X %X %X   0x%X %X %X %X %X %X\n", Mdata[0], Mdata[1], Mdata[2], Mdata[3], Mdata[4], Mdata[5], Mdata[6], Mdata[7], Mdata[8], Mdata[9], Mdata[10], Mdata[11]);
	  return;
         }
       #ifdef RX_CONTROL_NUM_128
        Mdata = Net_Stream[r_pos >> 5] + ((r_pos & 0x1F) << 11);

        //xtm_printf("Mdata = 0x%X %X, mac_len = %d\n", (INT16U)(((INT32U)Mdata & 0xffff0000) >> 16), (INT16U)((INT32U)Mdata & 0xffff), mac_len);
        //xtm_printf("%X %X %X %X %X %X %X %X %X %X %X %X\n", Mdata[0], Mdata[1], Mdata[2], Mdata[3], Mdata[4], Mdata[5], Mdata[6], Mdata[7], Mdata[8], Mdata[9], Mdata[10], Mdata[11]);
       #else
        Mdata = Mdata_Stream;
        #if 1
         // 指向包开始地址
         Maddr = r_pos;
     	 Maddr <<= 11;
	 Maddr+=rx_gen_SA;
         fmem_insb(Maddr, Mdata, mac_len);
        #endif
       #endif
      #ifdef _XTM_ADD    //20110704
       //xtm_printf("mac_recv::1\n");
       //ARP 处理
       //if(Local.Status == 0)
        if(Arp_Deal(Mdata, mac_len) == 1)
          return;
       //xtm_printf("mac_recv::2\n");
       //ICMP 处理
       if(Icmp_Deal(Mdata, mac_len) == 1)
         return;
       //xtm_printf("mac_recv::3\n");

       //IGMP 处理
       if(Igmp_Deal(Mdata, mac_len) == 1)
         return;

       //UDP 处理
       if(Udp_Deal(Mdata, mac_len) == 1)
         return;

       //xtm_printf("mac_recv::4\n");
       //for(i=0; i<42; i++)
       //  xtm_printf("%x,\n", Mdata[i]);
       //xtm_printf("%d  \n",  r_pos);
      #endif

}
#endif
//---------------------------------------------------------------------------
//程序的功能是根据多播地址multicast_address[6]的值，计算出MAR0-MAR7，就是multicast_filter[8];
//本程序指计算一个多播地址。如果有多个多播地址，将每个多播地址生成的multicast_filter[8]相或就可以了，
//例如根据多播地址 01:00:5e:00:00:01生成的 value1=multicast_filter[8];
//根据多播地址     01:00:5e:00:00:02生成的 value2=multicast_filter[8];
//那么对这两个多播地址生成的multicast_filter[8]=value1  |  value2 ;将两个值相或
//很容易从这里得到如果要接收所有多播地址的数据包，MAR0--MAR7必须设置为0xff,就是说
//multicast_filter[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
#define al ax_value.bytes.low
#define bl bx_value.bytes.low
#define cl cx_value.bytes.low
#define ah ax_value.bytes.high
#define bh bx_value.bytes.high
#define dh dx_value.bytes.high
#define ax ax_value.word
#define bx bx_value.word
#define cx cx_value.word
#define dx dx_value.word
#define shl_bx if((bx&0x8000)!=0){cf=1;}else{cf=0;};bx=bx<<1;
#define shl_al if((al&0x80)!=0){cf=1;}else{cf=0;};al=al<<1;
#define shr_al if((al&0x01)!=0){cf=1;}else{cf=0;};al=al>>1;
#define rcl_dx if((dx&0x8000)!=0){cf_temp=1;}else{cf_temp=0;};dx=dx<<1;if(cf){dx=dx+1;};cf=cf_temp;
#define rcl_ah if((ah&0x80)!=0){cf_temp=1;}else{cf_temp=0;};ah=ah<<1;if(cf){ah=ah+1;};cf=cf_temp;
#define rcr_ah if((ah&0x01)!=0){cf_temp=1;}else{cf_temp=0;};ah=ah>>1;if(cf){ah=ah+0x80;};cf=cf_temp;
#define rol_al if((al&0x80)!=0){cf=1;}else{cf=0;};al=al<<1;if(cf){al=al+1;};
#define ror_al if((al&0x01)!=0){cf=1;}else{cf=0;};al=al>>1;if(cf){al=al+0x80;};
union u {unsigned int word;
         struct{unsigned char high;unsigned char low;}bytes;//字节顺序为高位在前的2byte结构
   };
union u ax_value;
union u bx_value;
union u cx_value;
union u dx_value;
INT8U cf;
INT8U cf_temp;
unsigned char multicast_address[6]={0x01,0x00,0x5e,0x00,0x00,0x02};
//unsigned char multicast_address[6]={0x00,0x00,0x00,0x5e,0x00,0x01};  //只计算一个多播地址
//unsigned char multicast_address[6]={0x01,0x80,0xc2,0x00,0x00,0x00};
unsigned char multicast_filter[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
                                      //结果放在这里，就是MAR0--MAR7的值
//;;0x41,0x00,0x00,0x80,0x00,0x00,0x00,0x00;
//---------------------------------------------------------------------------
void up_crc(unsigned char al_byte)
{
  al=al_byte;
  ah=0;
  for (cx=0;cx<8;cx++)
   {
    shl_bx;
    rcl_dx;
    rcl_ah;
    ah=ah^al;
    rcr_ah;
    if(cf){
       dx=dx^0x04c1;//        0000,0100,1100,0001b
       bx=bx^0x1db7; //0001,1101,1011,0110b + 1 ;plus one for end-around carry.
       xtm_printf("up_crc::dx = %X, bx = %X\n", dx, bx);
      }
    shr_al;
   }
}
//---------------------------------------------------------------------------
void add_mc_bits()
{
  unsigned char i, j;
  bx=0xffff;
  dx=0xffff;
  for(i=0;i<6;i++)
   {
    up_crc(multicast_address[i]);
   }
  xtm_printf("add_mc_bits::multicast_address = %X:%X:%X:%X:%X:%X, dh = 0x%X\n", \
     multicast_address[0], multicast_address[1], multicast_address[2], multicast_address[3], multicast_address[4], multicast_address[5], dh);
  cl=8;
  al=dh;
  rol_al;
  rol_al;
  rol_al;
  al=al&0x07;
  bl=al;
  bh=bh^bh;
  al=dh;
  ror_al;
  ror_al;
  al=al&0x07;
  cl=al;
  al=1;
  for(j=0;j<cl;j++)
   {
    shl_al;
   }
  xtm_printf("add_mc_bits::bx = 0x%X, al = 0x%X\n", \
     bx, al);
  multicast_filter[bx]=multicast_filter[bx] | al;
  //}
  xtm_printf("add_mc_bits::multicast_filter = %X:%X:%X:%X:%X:%X:%X:%X\n", multicast_filter[0], multicast_filter[1], multicast_filter[2], multicast_filter[3], multicast_filter[4], multicast_filter[5], multicast_filter[6], multicast_filter[7]);
}
//---------------------------------------------------------------------------
