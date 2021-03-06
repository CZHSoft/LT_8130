/*
***************************************************************************
* NAME:         main.c
*****************************************************************************
*/

#define MAIN_GLOBALS

#include "includes.h"

#include "stdlib.h"

#include "common.h"

void delay(INT16U);

void decode_dqt_init(void);
void encode_dqt_init(void);

int main(void)
{
	INT8U i;

	main_init();

	encode_dqt_init();
	decode_dqt_init();
	Init_Module_Clock();//根据需要打开、关闭各模块时钟
	//Lcd_Power(1, LocalCfg.BrightVolume);
	Local.fb_page = 0;
	Local.fb_addr[0] = Y1START;
	Local.fb_addr[1] = Y2START;
	selectym(Local.fb_page);
	//gra_dispcn24("系统初始化中....请稍候",96,200,COLOR_WHITE,COLOR_BLUE,NOFULLPAGE);

	Vflag=0;

	dtime[0]=200;

	i=0;
	
	dtime[0] = 0;
	dtime[1] = TIMERTIME;
	dtime[2] = TIMER2TIME;
	dtime[3] = KEYDELAYTIME;
	dtime[4] = GPIOTIME;
	
	ClearWatchDog();
	xtm_printf("LocalCfg.Mac_Addr = %x:%x:%x:%x:%x:%x, LocalCfg.IP = %d.%d.%d.%d, LocalCfg.Addr = %s\n", LocalCfg.Mac_Addr[0], LocalCfg.Mac_Addr[1], LocalCfg.Mac_Addr[2], LocalCfg.Mac_Addr[3], LocalCfg.Mac_Addr[4], LocalCfg.Mac_Addr[5],
		LocalCfg.IP[0], LocalCfg.IP[1], LocalCfg.IP[2], LocalCfg.IP[3], LocalCfg.Addr);
	Init_Udp();    //初始化UDP
	StartPlayAudio();
	DelayMs(300);
	StopPlayAudio();
	time_count = 0;
	ClearWatchDog();

	InterfaceInit(); //初始化界面资源
	ClearWatchDog();

	Reset_Param_Window();
	if(Local.Status == 0)
		DisplayTalkWindow(REFRESH);

	LocalCfg.BrightVolume = 5;
	xtm_printf("LocalCfg.BrightVolume = %d\n ", LocalCfg.BrightVolume);
	Lcd_Power(1, LocalCfg.BrightVolume);

	while(1){

        //测试点
		//SetGpio(GPIOD_OUT_DATA, 0xf7, 0x0); 
		
		//SetGpio(GPIOD_OUT_DATA, 0x08, 0x1);  
        //SetGpio(GPIOD_OUT_DATA, 0xfb, 0x0); 

		//SetGpio(GPIOD_OUT_DATA, 0xfb, 0x0); 
		//SetGpio(GPIOD_OUT_DATA, 0xf7, 0x0); 
		
		dcomm1();
		dcomm2();//com2数据解析
		if (!dtime[4])
		{
			CheckGpio();    //检测GPIO函数

			dtime[4] = GPIOTIME;
		}
#if 1
		if (Vflag){
			mac_int_deal();
			Vflag=0;
		}
#else
		while (Vflag){
			//xtm_printf("1, %d\n", video_decode_no);
			Vflag = 0;
			mac_int_deal();
			//xtm_printf("2, %d\n", video_decode_no);
			//WaitDecodeFinish();
		}
#endif

		if (!dtime[1])
		{
			timer_thread_func();  //定时器
			dtime[1] = TIMERTIME;
		}
		if (!dtime[2])
		{
			Arp_Table_Timer();

			CheckMultiUdpBuff();  //检查UDP发送缓存

			dtime[2] = TIMER2TIME;

#ifdef _BRUSHIDCARD_SUPPORT   //刷卡功能支持
			if(IDCardReadFlag == 1)
				SendReadIDCardReply();
#endif
			time_count ++;
		}

		CheckAudioUdpBuff();   //检查Audio UDP发送缓存
		CheckVideoEncodeBuf();     //检查视频编码

		if(Local.ClearVideoWindowFlag)    //清视频窗口标志
			if(Local.Status == 0)
			{
				clear_video_ram();
				Local.ClearVideoWindowFlag = 0;
			}          
	}
}

void delay(unsigned milliseconds)
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
}
//#define DQT_50
#define DQT_60
//#define DQT_70
//#define DQT_80

void decode_dqt_init(void)
{
#ifdef DQT_50
	//50
	outportb(DE_J_DQT_ADD, 0x00);	outportb(DE_J_DQT_DAT, 0x10);
	outportb(DE_J_DQT_ADD, 0x01);	outportb(DE_J_DQT_DAT, 0x0B);
	outportb(DE_J_DQT_ADD, 0x02);	outportb(DE_J_DQT_DAT, 0x0C);
	outportb(DE_J_DQT_ADD, 0x03);	outportb(DE_J_DQT_DAT, 0x0E);
	outportb(DE_J_DQT_ADD, 0x04);	outportb(DE_J_DQT_DAT, 0x0C);
	outportb(DE_J_DQT_ADD, 0x05);	outportb(DE_J_DQT_DAT, 0x0A);
	outportb(DE_J_DQT_ADD, 0x06);	outportb(DE_J_DQT_DAT, 0x10);
	outportb(DE_J_DQT_ADD, 0x07);	outportb(DE_J_DQT_DAT, 0x0E);
	outportb(DE_J_DQT_ADD, 0x08);	outportb(DE_J_DQT_DAT, 0x0D);
	outportb(DE_J_DQT_ADD, 0x09);	outportb(DE_J_DQT_DAT, 0x0E);
	outportb(DE_J_DQT_ADD, 0x0a);	outportb(DE_J_DQT_DAT, 0x12);
	outportb(DE_J_DQT_ADD, 0x0b);	outportb(DE_J_DQT_DAT, 0x11);
	outportb(DE_J_DQT_ADD, 0x0c);	outportb(DE_J_DQT_DAT, 0x10);
	outportb(DE_J_DQT_ADD, 0x0d);	outportb(DE_J_DQT_DAT, 0x13);
	outportb(DE_J_DQT_ADD, 0x0e);	outportb(DE_J_DQT_DAT, 0x18);
	outportb(DE_J_DQT_ADD, 0x0f);	outportb(DE_J_DQT_DAT, 0x28);
	outportb(DE_J_DQT_ADD, 0x10);	outportb(DE_J_DQT_DAT, 0x1A);
	outportb(DE_J_DQT_ADD, 0x11);	outportb(DE_J_DQT_DAT, 0x18);
	outportb(DE_J_DQT_ADD, 0x12);	outportb(DE_J_DQT_DAT, 0x16);
	outportb(DE_J_DQT_ADD, 0x13);	outportb(DE_J_DQT_DAT, 0x16);
	outportb(DE_J_DQT_ADD, 0x14);	outportb(DE_J_DQT_DAT, 0x18);
	outportb(DE_J_DQT_ADD, 0x15);	outportb(DE_J_DQT_DAT, 0x31);
	outportb(DE_J_DQT_ADD, 0x16);	outportb(DE_J_DQT_DAT, 0x23);
	outportb(DE_J_DQT_ADD, 0x17);	outportb(DE_J_DQT_DAT, 0x25);
	outportb(DE_J_DQT_ADD, 0x18);	outportb(DE_J_DQT_DAT, 0x1D);
	outportb(DE_J_DQT_ADD, 0x19);	outportb(DE_J_DQT_DAT, 0x28);
	outportb(DE_J_DQT_ADD, 0x1a);	outportb(DE_J_DQT_DAT, 0x3A);
	outportb(DE_J_DQT_ADD, 0x1b);	outportb(DE_J_DQT_DAT, 0x33);
	outportb(DE_J_DQT_ADD, 0x1c);	outportb(DE_J_DQT_DAT, 0x3D);
	outportb(DE_J_DQT_ADD, 0x1d);	outportb(DE_J_DQT_DAT, 0x3C);
	outportb(DE_J_DQT_ADD, 0x1e);	outportb(DE_J_DQT_DAT, 0x39);
	outportb(DE_J_DQT_ADD, 0x1f);	outportb(DE_J_DQT_DAT, 0x33);
	outportb(DE_J_DQT_ADD, 0x20);	outportb(DE_J_DQT_DAT, 0x38);
	outportb(DE_J_DQT_ADD, 0x21);	outportb(DE_J_DQT_DAT, 0x37);
	outportb(DE_J_DQT_ADD, 0x22);	outportb(DE_J_DQT_DAT, 0x40);
	outportb(DE_J_DQT_ADD, 0x23);	outportb(DE_J_DQT_DAT, 0x48);
	outportb(DE_J_DQT_ADD, 0x24);	outportb(DE_J_DQT_DAT, 0x5C);
	outportb(DE_J_DQT_ADD, 0x25);	outportb(DE_J_DQT_DAT, 0x4E);
	outportb(DE_J_DQT_ADD, 0x26);	outportb(DE_J_DQT_DAT, 0x40);
	outportb(DE_J_DQT_ADD, 0x27);	outportb(DE_J_DQT_DAT, 0x44);
	outportb(DE_J_DQT_ADD, 0x28);	outportb(DE_J_DQT_DAT, 0x57);
	outportb(DE_J_DQT_ADD, 0x29);	outportb(DE_J_DQT_DAT, 0x45);
	outportb(DE_J_DQT_ADD, 0x2a);	outportb(DE_J_DQT_DAT, 0x37);
	outportb(DE_J_DQT_ADD, 0x2b);	outportb(DE_J_DQT_DAT, 0x38);
	outportb(DE_J_DQT_ADD, 0x2c);	outportb(DE_J_DQT_DAT, 0x50);
	outportb(DE_J_DQT_ADD, 0x2d);	outportb(DE_J_DQT_DAT, 0x6D);
	outportb(DE_J_DQT_ADD, 0x2e);	outportb(DE_J_DQT_DAT, 0x51);
	outportb(DE_J_DQT_ADD, 0x2f);	outportb(DE_J_DQT_DAT, 0x57);
	outportb(DE_J_DQT_ADD, 0x30);	outportb(DE_J_DQT_DAT, 0x5F);
	outportb(DE_J_DQT_ADD, 0x31);	outportb(DE_J_DQT_DAT, 0x62);
	outportb(DE_J_DQT_ADD, 0x32);	outportb(DE_J_DQT_DAT, 0x67);
	outportb(DE_J_DQT_ADD, 0x33);	outportb(DE_J_DQT_DAT, 0x68);
	outportb(DE_J_DQT_ADD, 0x34);	outportb(DE_J_DQT_DAT, 0x67);
	outportb(DE_J_DQT_ADD, 0x35);	outportb(DE_J_DQT_DAT, 0x3E);
	outportb(DE_J_DQT_ADD, 0x36);	outportb(DE_J_DQT_DAT, 0x4D);
	outportb(DE_J_DQT_ADD, 0x37);	outportb(DE_J_DQT_DAT, 0x71);
	outportb(DE_J_DQT_ADD, 0x38);	outportb(DE_J_DQT_DAT, 0x79);
	outportb(DE_J_DQT_ADD, 0x39);	outportb(DE_J_DQT_DAT, 0x70);
	outportb(DE_J_DQT_ADD, 0x3a);	outportb(DE_J_DQT_DAT, 0x64);
	outportb(DE_J_DQT_ADD, 0x3b);	outportb(DE_J_DQT_DAT, 0x78);
	outportb(DE_J_DQT_ADD, 0x3c);	outportb(DE_J_DQT_DAT, 0x5C);
	outportb(DE_J_DQT_ADD, 0x3d);	outportb(DE_J_DQT_DAT, 0x65);
	outportb(DE_J_DQT_ADD, 0x3e);	outportb(DE_J_DQT_DAT, 0x67);
	outportb(DE_J_DQT_ADD, 0x3f);	outportb(DE_J_DQT_DAT, 0x63);

	outportb(DE_J_DQT_ADD, 0x40);	outportb(DE_J_DQT_DAT, 0x11);
	outportb(DE_J_DQT_ADD, 0x41);	outportb(DE_J_DQT_DAT, 0x12);
	outportb(DE_J_DQT_ADD, 0x42);	outportb(DE_J_DQT_DAT, 0x12);
	outportb(DE_J_DQT_ADD, 0x43);	outportb(DE_J_DQT_DAT, 0x18);
	outportb(DE_J_DQT_ADD, 0x44);	outportb(DE_J_DQT_DAT, 0x15);
	outportb(DE_J_DQT_ADD, 0x45);	outportb(DE_J_DQT_DAT, 0x18);
	outportb(DE_J_DQT_ADD, 0x46);	outportb(DE_J_DQT_DAT, 0x2F);
	outportb(DE_J_DQT_ADD, 0x47);	outportb(DE_J_DQT_DAT, 0x1A);
	outportb(DE_J_DQT_ADD, 0x48);	outportb(DE_J_DQT_DAT, 0x1A);
	outportb(DE_J_DQT_ADD, 0x49);	outportb(DE_J_DQT_DAT, 0x2F);
	outportb(DE_J_DQT_ADD, 0x4a);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x4b);	outportb(DE_J_DQT_DAT, 0x42);
	outportb(DE_J_DQT_ADD, 0x4c);	outportb(DE_J_DQT_DAT, 0x38);
	outportb(DE_J_DQT_ADD, 0x4d);	outportb(DE_J_DQT_DAT, 0x42);
	outportb(DE_J_DQT_ADD, 0x4e);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x4f);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x50);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x51);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x52);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x53);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x54);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x55);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x56);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x57);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x58);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x59);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x5a);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x5b);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x5c);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x5d);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x5e);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x5f);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x60);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x61);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x62);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x63);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x64);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x65);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x66);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x67);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x68);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x69);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x6a);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x6b);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x6c);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x6d);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x6e);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x6f);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x70);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x71);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x72);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x73);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x74);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x75);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x76);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x77);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x78);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x79);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x7a);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x7b);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x7c);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x7d);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x7e);	outportb(DE_J_DQT_DAT, 0x63);
	outportb(DE_J_DQT_ADD, 0x7f);	outportb(DE_J_DQT_DAT, 0x63);
#endif
#ifdef DQT_60  //60
	outportb(DE_J_DQT_ADD, 0x00);   outportb(DE_J_DQT_DAT, 0x0D);
	outportb(DE_J_DQT_ADD, 0x01);   outportb(DE_J_DQT_DAT, 0x09);	outportb(DE_J_DQT_ADD, 0x02);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x03);   outportb(DE_J_DQT_DAT, 0x0B);	outportb(DE_J_DQT_ADD, 0x04);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x05);   outportb(DE_J_DQT_DAT, 0x08);	outportb(DE_J_DQT_ADD, 0x06);   outportb(DE_J_DQT_DAT, 0x0D);	outportb(DE_J_DQT_ADD, 0x07);   outportb(DE_J_DQT_DAT, 0x0B);	outportb(DE_J_DQT_ADD, 0x08);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x09);   outportb(DE_J_DQT_DAT, 0x0B);	outportb(DE_J_DQT_ADD, 0x0A);   outportb(DE_J_DQT_DAT, 0x0E);	outportb(DE_J_DQT_ADD, 0x0B);   outportb(DE_J_DQT_DAT, 0x0E);	outportb(DE_J_DQT_ADD, 0x0C);   outportb(DE_J_DQT_DAT, 0x0D);	outportb(DE_J_DQT_ADD, 0x0D);   outportb(DE_J_DQT_DAT, 0x0F);	outportb(DE_J_DQT_ADD, 0x0E);   outportb(DE_J_DQT_DAT, 0x13);	outportb(DE_J_DQT_ADD, 0x0F);   outportb(DE_J_DQT_DAT, 0x20);	outportb(DE_J_DQT_ADD, 0x10);   outportb(DE_J_DQT_DAT, 0x15);	outportb(DE_J_DQT_ADD, 0x11);   outportb(DE_J_DQT_DAT, 0x13);	outportb(DE_J_DQT_ADD, 0x12);   outportb(DE_J_DQT_DAT, 0x12);	outportb(DE_J_DQT_ADD, 0x13);   outportb(DE_J_DQT_DAT, 0x12);	outportb(DE_J_DQT_ADD, 0x14);   outportb(DE_J_DQT_DAT, 0x13);	outportb(DE_J_DQT_ADD, 0x15);   outportb(DE_J_DQT_DAT, 0x27);	outportb(DE_J_DQT_ADD, 0x16);   outportb(DE_J_DQT_DAT, 0x1C);	outportb(DE_J_DQT_ADD, 0x17);   outportb(DE_J_DQT_DAT, 0x1E);	outportb(DE_J_DQT_ADD, 0x18);   outportb(DE_J_DQT_DAT, 0x17);	outportb(DE_J_DQT_ADD, 0x19);   outportb(DE_J_DQT_DAT, 0x20);	outportb(DE_J_DQT_ADD, 0x1A);   outportb(DE_J_DQT_DAT, 0x2E);	outportb(DE_J_DQT_ADD, 0x1B);   outportb(DE_J_DQT_DAT, 0x29);	outportb(DE_J_DQT_ADD, 0x1C);   outportb(DE_J_DQT_DAT, 0x31);	outportb(DE_J_DQT_ADD, 0x1D);   outportb(DE_J_DQT_DAT, 0x30);	outportb(DE_J_DQT_ADD, 0x1E);   outportb(DE_J_DQT_DAT, 0x2E);	outportb(DE_J_DQT_ADD, 0x1F);   outportb(DE_J_DQT_DAT, 0x29);	outportb(DE_J_DQT_ADD, 0x20);   outportb(DE_J_DQT_DAT, 0x2D);	outportb(DE_J_DQT_ADD, 0x21);   outportb(DE_J_DQT_DAT, 0x2C);	outportb(DE_J_DQT_ADD, 0x22);   outportb(DE_J_DQT_DAT, 0x33);	outportb(DE_J_DQT_ADD, 0x23);   outportb(DE_J_DQT_DAT, 0x3A);	outportb(DE_J_DQT_ADD, 0x24);   outportb(DE_J_DQT_DAT, 0x4A);	outportb(DE_J_DQT_ADD, 0x25);   outportb(DE_J_DQT_DAT, 0x3E);	outportb(DE_J_DQT_ADD, 0x26);   outportb(DE_J_DQT_DAT, 0x33);	outportb(DE_J_DQT_ADD, 0x27);   outportb(DE_J_DQT_DAT, 0x36);	outportb(DE_J_DQT_ADD, 0x28);   outportb(DE_J_DQT_DAT, 0x46);	outportb(DE_J_DQT_ADD, 0x29);   outportb(DE_J_DQT_DAT, 0x37);	outportb(DE_J_DQT_ADD, 0x2A);   outportb(DE_J_DQT_DAT, 0x2C);	outportb(DE_J_DQT_ADD, 0x2B);   outportb(DE_J_DQT_DAT, 0x2D);	outportb(DE_J_DQT_ADD, 0x2C);   outportb(DE_J_DQT_DAT, 0x40);	outportb(DE_J_DQT_ADD, 0x2D);   outportb(DE_J_DQT_DAT, 0x57);	outportb(DE_J_DQT_ADD, 0x2E);   outportb(DE_J_DQT_DAT, 0x41);	outportb(DE_J_DQT_ADD, 0x2F);   outportb(DE_J_DQT_DAT, 0x46);	outportb(DE_J_DQT_ADD, 0x30);   outportb(DE_J_DQT_DAT, 0x4C);	outportb(DE_J_DQT_ADD, 0x31);   outportb(DE_J_DQT_DAT, 0x4E);	outportb(DE_J_DQT_ADD, 0x32);   outportb(DE_J_DQT_DAT, 0x52);	outportb(DE_J_DQT_ADD, 0x33);   outportb(DE_J_DQT_DAT, 0x53);	outportb(DE_J_DQT_ADD, 0x34);   outportb(DE_J_DQT_DAT, 0x52);	outportb(DE_J_DQT_ADD, 0x35);   outportb(DE_J_DQT_DAT, 0x32);	outportb(DE_J_DQT_ADD, 0x36);   outportb(DE_J_DQT_DAT, 0x3E);	outportb(DE_J_DQT_ADD, 0x37);   outportb(DE_J_DQT_DAT, 0x5A);	outportb(DE_J_DQT_ADD, 0x38);   outportb(DE_J_DQT_DAT, 0x61);	outportb(DE_J_DQT_ADD, 0x39);   outportb(DE_J_DQT_DAT, 0x5A);	outportb(DE_J_DQT_ADD, 0x3A);   outportb(DE_J_DQT_DAT, 0x50);	outportb(DE_J_DQT_ADD, 0x3B);   outportb(DE_J_DQT_DAT, 0x60);	outportb(DE_J_DQT_ADD, 0x3C);   outportb(DE_J_DQT_DAT, 0x4A);	outportb(DE_J_DQT_ADD, 0x3D);   outportb(DE_J_DQT_DAT, 0x51);	outportb(DE_J_DQT_ADD, 0x3E);   outportb(DE_J_DQT_DAT, 0x52);	outportb(DE_J_DQT_ADD, 0x3F);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x40);   outportb(DE_J_DQT_DAT, 0x0E);	outportb(DE_J_DQT_ADD, 0x41);   outportb(DE_J_DQT_DAT, 0x0E);	outportb(DE_J_DQT_ADD, 0x42);   outportb(DE_J_DQT_DAT, 0x0E);	outportb(DE_J_DQT_ADD, 0x43);   outportb(DE_J_DQT_DAT, 0x13);	outportb(DE_J_DQT_ADD, 0x44);   outportb(DE_J_DQT_DAT, 0x11);	outportb(DE_J_DQT_ADD, 0x45);   outportb(DE_J_DQT_DAT, 0x13);	outportb(DE_J_DQT_ADD, 0x46);   outportb(DE_J_DQT_DAT, 0x26);	outportb(DE_J_DQT_ADD, 0x47);   outportb(DE_J_DQT_DAT, 0x15);	outportb(DE_J_DQT_ADD, 0x48);   outportb(DE_J_DQT_DAT, 0x15);	outportb(DE_J_DQT_ADD, 0x49);   outportb(DE_J_DQT_DAT, 0x26);	outportb(DE_J_DQT_ADD, 0x4A);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x4B);   outportb(DE_J_DQT_DAT, 0x35);	outportb(DE_J_DQT_ADD, 0x4C);   outportb(DE_J_DQT_DAT, 0x2D);	outportb(DE_J_DQT_ADD, 0x4D);   outportb(DE_J_DQT_DAT, 0x35);	outportb(DE_J_DQT_ADD, 0x4E);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x4F);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x50);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x51);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x52);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x53);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x54);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x55);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x56);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x57);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x58);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x59);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x5A);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x5B);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x5C);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x5D);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x5E);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x5F);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x60);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x61);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x62);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x63);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x64);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x65);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x66);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x67);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x68);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x69);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x6A);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x6B);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x6C);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x6D);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x6E);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x6F);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x70);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x71);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x72);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x73);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x74);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x75);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x76);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x77);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x78);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x79);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x7A);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x7B);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x7C);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x7D);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x7E);   outportb(DE_J_DQT_DAT, 0x4F);	outportb(DE_J_DQT_ADD, 0x7F);   outportb(DE_J_DQT_DAT, 0x4F);#endif
#ifdef DQT_70  //70
	outportb(DE_J_DQT_ADD, 0x00);   outportb(DE_J_DQT_DAT, 0x0A);
	outportb(DE_J_DQT_ADD, 0x01);   outportb(DE_J_DQT_DAT, 0x07);	outportb(DE_J_DQT_ADD, 0x02);   outportb(DE_J_DQT_DAT, 0x07);	outportb(DE_J_DQT_ADD, 0x03);   outportb(DE_J_DQT_DAT, 0x08);	outportb(DE_J_DQT_ADD, 0x04);   outportb(DE_J_DQT_DAT, 0x07);	outportb(DE_J_DQT_ADD, 0x05);   outportb(DE_J_DQT_DAT, 0x06);	outportb(DE_J_DQT_ADD, 0x06);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x07);   outportb(DE_J_DQT_DAT, 0x08);	outportb(DE_J_DQT_ADD, 0x08);   outportb(DE_J_DQT_DAT, 0x08);	outportb(DE_J_DQT_ADD, 0x09);   outportb(DE_J_DQT_DAT, 0x08);	outportb(DE_J_DQT_ADD, 0x0A);   outportb(DE_J_DQT_DAT, 0x0B);	outportb(DE_J_DQT_ADD, 0x0B);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x0C);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x0D);   outportb(DE_J_DQT_DAT, 0x0B);	outportb(DE_J_DQT_ADD, 0x0E);   outportb(DE_J_DQT_DAT, 0x0E);	outportb(DE_J_DQT_ADD, 0x0F);   outportb(DE_J_DQT_DAT, 0x18);	outportb(DE_J_DQT_ADD, 0x10);   outportb(DE_J_DQT_DAT, 0x10);	outportb(DE_J_DQT_ADD, 0x11);   outportb(DE_J_DQT_DAT, 0x0E);	outportb(DE_J_DQT_ADD, 0x12);   outportb(DE_J_DQT_DAT, 0x0D);	outportb(DE_J_DQT_ADD, 0x13);   outportb(DE_J_DQT_DAT, 0x0D);	outportb(DE_J_DQT_ADD, 0x14);   outportb(DE_J_DQT_DAT, 0x0E);	outportb(DE_J_DQT_ADD, 0x15);   outportb(DE_J_DQT_DAT, 0x1D);	outportb(DE_J_DQT_ADD, 0x16);   outportb(DE_J_DQT_DAT, 0x15);	outportb(DE_J_DQT_ADD, 0x17);   outportb(DE_J_DQT_DAT, 0x16);	outportb(DE_J_DQT_ADD, 0x18);   outportb(DE_J_DQT_DAT, 0x11);	outportb(DE_J_DQT_ADD, 0x19);   outportb(DE_J_DQT_DAT, 0x18);	outportb(DE_J_DQT_ADD, 0x1A);   outportb(DE_J_DQT_DAT, 0x23);	outportb(DE_J_DQT_ADD, 0x1B);   outportb(DE_J_DQT_DAT, 0x1F);	outportb(DE_J_DQT_ADD, 0x1C);   outportb(DE_J_DQT_DAT, 0x25);	outportb(DE_J_DQT_ADD, 0x1D);   outportb(DE_J_DQT_DAT, 0x24);	outportb(DE_J_DQT_ADD, 0x1E);   outportb(DE_J_DQT_DAT, 0x22);	outportb(DE_J_DQT_ADD, 0x1F);   outportb(DE_J_DQT_DAT, 0x1F);	outportb(DE_J_DQT_ADD, 0x20);   outportb(DE_J_DQT_DAT, 0x22);	outportb(DE_J_DQT_ADD, 0x21);   outportb(DE_J_DQT_DAT, 0x21);	outportb(DE_J_DQT_ADD, 0x22);   outportb(DE_J_DQT_DAT, 0x26);	outportb(DE_J_DQT_ADD, 0x23);   outportb(DE_J_DQT_DAT, 0x2B);	outportb(DE_J_DQT_ADD, 0x24);   outportb(DE_J_DQT_DAT, 0x37);	outportb(DE_J_DQT_ADD, 0x25);   outportb(DE_J_DQT_DAT, 0x2F);	outportb(DE_J_DQT_ADD, 0x26);   outportb(DE_J_DQT_DAT, 0x26);	outportb(DE_J_DQT_ADD, 0x27);   outportb(DE_J_DQT_DAT, 0x29);	outportb(DE_J_DQT_ADD, 0x28);   outportb(DE_J_DQT_DAT, 0x34);	outportb(DE_J_DQT_ADD, 0x29);   outportb(DE_J_DQT_DAT, 0x29);	outportb(DE_J_DQT_ADD, 0x2A);   outportb(DE_J_DQT_DAT, 0x21);	outportb(DE_J_DQT_ADD, 0x2B);   outportb(DE_J_DQT_DAT, 0x22);	outportb(DE_J_DQT_ADD, 0x2C);   outportb(DE_J_DQT_DAT, 0x30);	outportb(DE_J_DQT_ADD, 0x2D);   outportb(DE_J_DQT_DAT, 0x41);	outportb(DE_J_DQT_ADD, 0x2E);   outportb(DE_J_DQT_DAT, 0x31);	outportb(DE_J_DQT_ADD, 0x2F);   outportb(DE_J_DQT_DAT, 0x34);	outportb(DE_J_DQT_ADD, 0x30);   outportb(DE_J_DQT_DAT, 0x39);	outportb(DE_J_DQT_ADD, 0x31);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x32);   outportb(DE_J_DQT_DAT, 0x3E);	outportb(DE_J_DQT_ADD, 0x33);   outportb(DE_J_DQT_DAT, 0x3E);	outportb(DE_J_DQT_ADD, 0x34);   outportb(DE_J_DQT_DAT, 0x3E);	outportb(DE_J_DQT_ADD, 0x35);   outportb(DE_J_DQT_DAT, 0x25);	outportb(DE_J_DQT_ADD, 0x36);   outportb(DE_J_DQT_DAT, 0x2E);	outportb(DE_J_DQT_ADD, 0x37);   outportb(DE_J_DQT_DAT, 0x44);	outportb(DE_J_DQT_ADD, 0x38);   outportb(DE_J_DQT_DAT, 0x49);	outportb(DE_J_DQT_ADD, 0x39);   outportb(DE_J_DQT_DAT, 0x43);	outportb(DE_J_DQT_ADD, 0x3A);   outportb(DE_J_DQT_DAT, 0x3C);	outportb(DE_J_DQT_ADD, 0x3B);   outportb(DE_J_DQT_DAT, 0x48);	outportb(DE_J_DQT_ADD, 0x3C);   outportb(DE_J_DQT_DAT, 0x37);	outportb(DE_J_DQT_ADD, 0x3D);   outportb(DE_J_DQT_DAT, 0x3D);	outportb(DE_J_DQT_ADD, 0x3E);   outportb(DE_J_DQT_DAT, 0x3E);	outportb(DE_J_DQT_ADD, 0x3F);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x40);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x41);   outportb(DE_J_DQT_DAT, 0x0B);	outportb(DE_J_DQT_ADD, 0x42);   outportb(DE_J_DQT_DAT, 0x0B);	outportb(DE_J_DQT_ADD, 0x43);   outportb(DE_J_DQT_DAT, 0x0E);	outportb(DE_J_DQT_ADD, 0x44);   outportb(DE_J_DQT_DAT, 0x0D);	outportb(DE_J_DQT_ADD, 0x45);   outportb(DE_J_DQT_DAT, 0x0E);	outportb(DE_J_DQT_ADD, 0x46);   outportb(DE_J_DQT_DAT, 0x1C);	outportb(DE_J_DQT_ADD, 0x47);   outportb(DE_J_DQT_DAT, 0x10);	outportb(DE_J_DQT_ADD, 0x48);   outportb(DE_J_DQT_DAT, 0x10);	outportb(DE_J_DQT_ADD, 0x49);   outportb(DE_J_DQT_DAT, 0x1C);	outportb(DE_J_DQT_ADD, 0x4A);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x4B);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x4C);   outportb(DE_J_DQT_DAT, 0x22);	outportb(DE_J_DQT_ADD, 0x4D);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x4E);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x4F);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x50);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x51);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x52);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x53);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x54);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x55);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x56);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x57);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x58);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x59);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x5A);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x5B);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x5C);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x5D);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x5E);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x5F);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x60);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x61);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x62);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x63);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x64);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x65);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x66);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x67);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x68);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x69);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x6A);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x6B);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x6C);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x6D);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x6E);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x6F);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x70);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x71);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x72);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x73);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x74);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x75);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x76);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x77);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x78);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x79);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x7A);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x7B);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x7C);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x7D);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x7E);   outportb(DE_J_DQT_DAT, 0x3B);	outportb(DE_J_DQT_ADD, 0x7F);   outportb(DE_J_DQT_DAT, 0x3B);#endif
#ifdef DQT_80  //80
	outportb(DE_J_DQT_ADD, 0x00);   outportb(DE_J_DQT_DAT, 0x06);
	outportb(DE_J_DQT_ADD, 0x01);   outportb(DE_J_DQT_DAT, 0x04);	outportb(DE_J_DQT_ADD, 0x02);   outportb(DE_J_DQT_DAT, 0x05);	outportb(DE_J_DQT_ADD, 0x03);   outportb(DE_J_DQT_DAT, 0x06);	outportb(DE_J_DQT_ADD, 0x04);   outportb(DE_J_DQT_DAT, 0x05);	outportb(DE_J_DQT_ADD, 0x05);   outportb(DE_J_DQT_DAT, 0x04);	outportb(DE_J_DQT_ADD, 0x06);   outportb(DE_J_DQT_DAT, 0x06);	outportb(DE_J_DQT_ADD, 0x07);   outportb(DE_J_DQT_DAT, 0x06);	outportb(DE_J_DQT_ADD, 0x08);   outportb(DE_J_DQT_DAT, 0x05);	outportb(DE_J_DQT_ADD, 0x09);   outportb(DE_J_DQT_DAT, 0x06);	outportb(DE_J_DQT_ADD, 0x0A);   outportb(DE_J_DQT_DAT, 0x07);	outportb(DE_J_DQT_ADD, 0x0B);   outportb(DE_J_DQT_DAT, 0x07);	outportb(DE_J_DQT_ADD, 0x0C);   outportb(DE_J_DQT_DAT, 0x06);	outportb(DE_J_DQT_ADD, 0x0D);   outportb(DE_J_DQT_DAT, 0x08);	outportb(DE_J_DQT_ADD, 0x0E);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x0F);   outportb(DE_J_DQT_DAT, 0x10);	outportb(DE_J_DQT_ADD, 0x10);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x11);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x12);   outportb(DE_J_DQT_DAT, 0x09);	outportb(DE_J_DQT_ADD, 0x13);   outportb(DE_J_DQT_DAT, 0x09);	outportb(DE_J_DQT_ADD, 0x14);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x15);   outportb(DE_J_DQT_DAT, 0x14);	outportb(DE_J_DQT_ADD, 0x16);   outportb(DE_J_DQT_DAT, 0x0E);	outportb(DE_J_DQT_ADD, 0x17);   outportb(DE_J_DQT_DAT, 0x0F);	outportb(DE_J_DQT_ADD, 0x18);   outportb(DE_J_DQT_DAT, 0x0C);	outportb(DE_J_DQT_ADD, 0x19);   outportb(DE_J_DQT_DAT, 0x10);	outportb(DE_J_DQT_ADD, 0x1A);   outportb(DE_J_DQT_DAT, 0x17);	outportb(DE_J_DQT_ADD, 0x1B);   outportb(DE_J_DQT_DAT, 0x14);	outportb(DE_J_DQT_ADD, 0x1C);   outportb(DE_J_DQT_DAT, 0x18);	outportb(DE_J_DQT_ADD, 0x1D);   outportb(DE_J_DQT_DAT, 0x18);	outportb(DE_J_DQT_ADD, 0x1E);   outportb(DE_J_DQT_DAT, 0x17);	outportb(DE_J_DQT_ADD, 0x1F);   outportb(DE_J_DQT_DAT, 0x14);	outportb(DE_J_DQT_ADD, 0x20);   outportb(DE_J_DQT_DAT, 0x16);	outportb(DE_J_DQT_ADD, 0x21);   outportb(DE_J_DQT_DAT, 0x16);	outportb(DE_J_DQT_ADD, 0x22);   outportb(DE_J_DQT_DAT, 0x1A);	outportb(DE_J_DQT_ADD, 0x23);   outportb(DE_J_DQT_DAT, 0x1D);	outportb(DE_J_DQT_ADD, 0x24);   outportb(DE_J_DQT_DAT, 0x25);	outportb(DE_J_DQT_ADD, 0x25);   outportb(DE_J_DQT_DAT, 0x1F);	outportb(DE_J_DQT_ADD, 0x26);   outportb(DE_J_DQT_DAT, 0x1A);	outportb(DE_J_DQT_ADD, 0x27);   outportb(DE_J_DQT_DAT, 0x1B);	outportb(DE_J_DQT_ADD, 0x28);   outportb(DE_J_DQT_DAT, 0x23);	outportb(DE_J_DQT_ADD, 0x29);   outportb(DE_J_DQT_DAT, 0x1C);	outportb(DE_J_DQT_ADD, 0x2A);   outportb(DE_J_DQT_DAT, 0x16);	outportb(DE_J_DQT_ADD, 0x2B);   outportb(DE_J_DQT_DAT, 0x16);	outportb(DE_J_DQT_ADD, 0x2C);   outportb(DE_J_DQT_DAT, 0x20);	outportb(DE_J_DQT_ADD, 0x2D);   outportb(DE_J_DQT_DAT, 0x2C);	outportb(DE_J_DQT_ADD, 0x2E);   outportb(DE_J_DQT_DAT, 0x20);	outportb(DE_J_DQT_ADD, 0x2F);   outportb(DE_J_DQT_DAT, 0x23);	outportb(DE_J_DQT_ADD, 0x30);   outportb(DE_J_DQT_DAT, 0x26);	outportb(DE_J_DQT_ADD, 0x31);   outportb(DE_J_DQT_DAT, 0x27);	outportb(DE_J_DQT_ADD, 0x32);   outportb(DE_J_DQT_DAT, 0x29);	outportb(DE_J_DQT_ADD, 0x33);   outportb(DE_J_DQT_DAT, 0x2A);	outportb(DE_J_DQT_ADD, 0x34);   outportb(DE_J_DQT_DAT, 0x29);	outportb(DE_J_DQT_ADD, 0x35);   outportb(DE_J_DQT_DAT, 0x19);	outportb(DE_J_DQT_ADD, 0x36);   outportb(DE_J_DQT_DAT, 0x1F);	outportb(DE_J_DQT_ADD, 0x37);   outportb(DE_J_DQT_DAT, 0x2D);	outportb(DE_J_DQT_ADD, 0x38);   outportb(DE_J_DQT_DAT, 0x30);	outportb(DE_J_DQT_ADD, 0x39);   outportb(DE_J_DQT_DAT, 0x2D);	outportb(DE_J_DQT_ADD, 0x3A);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x3B);   outportb(DE_J_DQT_DAT, 0x30);	outportb(DE_J_DQT_ADD, 0x3C);   outportb(DE_J_DQT_DAT, 0x25);	outportb(DE_J_DQT_ADD, 0x3D);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x3E);   outportb(DE_J_DQT_DAT, 0x29);	outportb(DE_J_DQT_ADD, 0x3F);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x40);   outportb(DE_J_DQT_DAT, 0x07);	outportb(DE_J_DQT_ADD, 0x41);   outportb(DE_J_DQT_DAT, 0x07);	outportb(DE_J_DQT_ADD, 0x42);   outportb(DE_J_DQT_DAT, 0x07);	outportb(DE_J_DQT_ADD, 0x43);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x44);   outportb(DE_J_DQT_DAT, 0x08);	outportb(DE_J_DQT_ADD, 0x45);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x46);   outportb(DE_J_DQT_DAT, 0x13);	outportb(DE_J_DQT_ADD, 0x47);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x48);   outportb(DE_J_DQT_DAT, 0x0A);	outportb(DE_J_DQT_ADD, 0x49);   outportb(DE_J_DQT_DAT, 0x13);	outportb(DE_J_DQT_ADD, 0x4A);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x4B);   outportb(DE_J_DQT_DAT, 0x1A);	outportb(DE_J_DQT_ADD, 0x4C);   outportb(DE_J_DQT_DAT, 0x16);	outportb(DE_J_DQT_ADD, 0x4D);   outportb(DE_J_DQT_DAT, 0x1A);	outportb(DE_J_DQT_ADD, 0x4E);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x4F);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x50);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x51);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x52);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x53);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x54);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x55);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x56);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x57);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x58);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x59);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x5A);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x5B);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x5C);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x5D);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x5E);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x5F);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x60);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x61);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x62);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x63);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x64);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x65);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x66);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x67);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x68);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x69);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x6A);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x6B);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x6C);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x6D);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x6E);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x6F);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x70);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x71);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x72);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x73);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x74);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x75);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x76);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x77);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x78);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x79);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x7A);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x7B);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x7C);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x7D);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x7E);   outportb(DE_J_DQT_DAT, 0x28);	outportb(DE_J_DQT_ADD, 0x7F);   outportb(DE_J_DQT_DAT, 0x28);#endif
}
void encode_dqt_init(void)
{

#ifdef DQT_50
	//=======
	//编码DQT
	//=======
	//输出（2的N次方）/DQT     50
	//N存入EN_J_DQT_SHIFT
	//将除法变成乘法
	outportb(EN_J_DQT_ADD, 0x00);	outportb(EN_J_DQT_DAT, (128/0x10));
	outportb(EN_J_DQT_ADD, 0x01);	outportb(EN_J_DQT_DAT, (128/0x0B));
	outportb(EN_J_DQT_ADD, 0x02);	outportb(EN_J_DQT_DAT, (128/0x0C));
	outportb(EN_J_DQT_ADD, 0x03);	outportb(EN_J_DQT_DAT, (128/0x0E));
	outportb(EN_J_DQT_ADD, 0x04);	outportb(EN_J_DQT_DAT, (128/0x0C));
	outportb(EN_J_DQT_ADD, 0x05);	outportb(EN_J_DQT_DAT, (128/0x0A));
	outportb(EN_J_DQT_ADD, 0x06);	outportb(EN_J_DQT_DAT, (128/0x10));
	outportb(EN_J_DQT_ADD, 0x07);	outportb(EN_J_DQT_DAT, (128/0x0E));
	outportb(EN_J_DQT_ADD, 0x08);	outportb(EN_J_DQT_DAT, (128/0x0D));
	outportb(EN_J_DQT_ADD, 0x09);	outportb(EN_J_DQT_DAT, (128/0x0E));
	outportb(EN_J_DQT_ADD, 0x0a);	outportb(EN_J_DQT_DAT, (128/0x12));
	outportb(EN_J_DQT_ADD, 0x0b);	outportb(EN_J_DQT_DAT, (128/0x11));
	outportb(EN_J_DQT_ADD, 0x0c);	outportb(EN_J_DQT_DAT, (128/0x10));
	outportb(EN_J_DQT_ADD, 0x0d);	outportb(EN_J_DQT_DAT, (128/0x13));
	outportb(EN_J_DQT_ADD, 0x0e);	outportb(EN_J_DQT_DAT, (128/0x18));
	outportb(EN_J_DQT_ADD, 0x0f);	outportb(EN_J_DQT_DAT, (128/0x28));
	outportb(EN_J_DQT_ADD, 0x10);	outportb(EN_J_DQT_DAT, (128/0x1A));
	outportb(EN_J_DQT_ADD, 0x11);	outportb(EN_J_DQT_DAT, (128/0x18));
	outportb(EN_J_DQT_ADD, 0x12);	outportb(EN_J_DQT_DAT, (128/0x16));
	outportb(EN_J_DQT_ADD, 0x13);	outportb(EN_J_DQT_DAT, (128/0x16));
	outportb(EN_J_DQT_ADD, 0x14);	outportb(EN_J_DQT_DAT, (128/0x18));
	outportb(EN_J_DQT_ADD, 0x15);	outportb(EN_J_DQT_DAT, (128/0x31));
	outportb(EN_J_DQT_ADD, 0x16);	outportb(EN_J_DQT_DAT, (128/0x23));
	outportb(EN_J_DQT_ADD, 0x17);	outportb(EN_J_DQT_DAT, (128/0x25));
	outportb(EN_J_DQT_ADD, 0x18);	outportb(EN_J_DQT_DAT, (128/0x1D));
	outportb(EN_J_DQT_ADD, 0x19);	outportb(EN_J_DQT_DAT, (128/0x28));
	outportb(EN_J_DQT_ADD, 0x1a);	outportb(EN_J_DQT_DAT, (128/0x3A));
	outportb(EN_J_DQT_ADD, 0x1b);	outportb(EN_J_DQT_DAT, (128/0x33));
	outportb(EN_J_DQT_ADD, 0x1c);	outportb(EN_J_DQT_DAT, (128/0x3D));
	outportb(EN_J_DQT_ADD, 0x1d);	outportb(EN_J_DQT_DAT, (128/0x3C));
	outportb(EN_J_DQT_ADD, 0x1e);	outportb(EN_J_DQT_DAT, (128/0x39));
	outportb(EN_J_DQT_ADD, 0x1f);	outportb(EN_J_DQT_DAT, (128/0x33));
	outportb(EN_J_DQT_ADD, 0x20);	outportb(EN_J_DQT_DAT, (128/0x38));
	outportb(EN_J_DQT_ADD, 0x21);	outportb(EN_J_DQT_DAT, (128/0x37));
	outportb(EN_J_DQT_ADD, 0x22);	outportb(EN_J_DQT_DAT, (128/0x40));
	outportb(EN_J_DQT_ADD, 0x23);	outportb(EN_J_DQT_DAT, (128/0x48));
	outportb(EN_J_DQT_ADD, 0x24);	outportb(EN_J_DQT_DAT, (128/0x5C));
	outportb(EN_J_DQT_ADD, 0x25);	outportb(EN_J_DQT_DAT, (128/0x4E));
	outportb(EN_J_DQT_ADD, 0x26);	outportb(EN_J_DQT_DAT, (128/0x40));
	outportb(EN_J_DQT_ADD, 0x27);	outportb(EN_J_DQT_DAT, (128/0x44));
	outportb(EN_J_DQT_ADD, 0x28);	outportb(EN_J_DQT_DAT, (128/0x57));
	outportb(EN_J_DQT_ADD, 0x29);	outportb(EN_J_DQT_DAT, (128/0x45));
	outportb(EN_J_DQT_ADD, 0x2a);	outportb(EN_J_DQT_DAT, (128/0x37));
	outportb(EN_J_DQT_ADD, 0x2b);	outportb(EN_J_DQT_DAT, (128/0x38));
	outportb(EN_J_DQT_ADD, 0x2c);	outportb(EN_J_DQT_DAT, (128/0x50));
	outportb(EN_J_DQT_ADD, 0x2d);	outportb(EN_J_DQT_DAT, (128/0x6D));
	outportb(EN_J_DQT_ADD, 0x2e);	outportb(EN_J_DQT_DAT, (128/0x51));
	outportb(EN_J_DQT_ADD, 0x2f);	outportb(EN_J_DQT_DAT, (128/0x57));
	outportb(EN_J_DQT_ADD, 0x30);	outportb(EN_J_DQT_DAT, (128/0x5F));
	outportb(EN_J_DQT_ADD, 0x31);	outportb(EN_J_DQT_DAT, (128/0x62));
	outportb(EN_J_DQT_ADD, 0x32);	outportb(EN_J_DQT_DAT, (128/0x67));
	outportb(EN_J_DQT_ADD, 0x33);	outportb(EN_J_DQT_DAT, (128/0x68));
	outportb(EN_J_DQT_ADD, 0x34);	outportb(EN_J_DQT_DAT, (128/0x67));
	outportb(EN_J_DQT_ADD, 0x35);	outportb(EN_J_DQT_DAT, (128/0x3E));
	outportb(EN_J_DQT_ADD, 0x36);	outportb(EN_J_DQT_DAT, (128/0x4D));
	outportb(EN_J_DQT_ADD, 0x37);	outportb(EN_J_DQT_DAT, (128/0x71));
	outportb(EN_J_DQT_ADD, 0x38);	outportb(EN_J_DQT_DAT, (128/0x79));
	outportb(EN_J_DQT_ADD, 0x39);	outportb(EN_J_DQT_DAT, (128/0x70));
	outportb(EN_J_DQT_ADD, 0x3a);	outportb(EN_J_DQT_DAT, (128/0x64));
	outportb(EN_J_DQT_ADD, 0x3b);	outportb(EN_J_DQT_DAT, (128/0x78));
	outportb(EN_J_DQT_ADD, 0x3c);	outportb(EN_J_DQT_DAT, (128/0x5C));
	outportb(EN_J_DQT_ADD, 0x3d);	outportb(EN_J_DQT_DAT, (128/0x65));
	outportb(EN_J_DQT_ADD, 0x3e);	outportb(EN_J_DQT_DAT, (128/0x67));
	outportb(EN_J_DQT_ADD, 0x3f);	outportb(EN_J_DQT_DAT, (128/0x63));

	outportb(EN_J_DQT_ADD, 0x40);	outportb(EN_J_DQT_DAT, (128/0x11));
	outportb(EN_J_DQT_ADD, 0x41);	outportb(EN_J_DQT_DAT, (128/0x12));
	outportb(EN_J_DQT_ADD, 0x42);	outportb(EN_J_DQT_DAT, (128/0x12));
	outportb(EN_J_DQT_ADD, 0x43);	outportb(EN_J_DQT_DAT, (128/0x18));
	outportb(EN_J_DQT_ADD, 0x44);	outportb(EN_J_DQT_DAT, (128/0x15));
	outportb(EN_J_DQT_ADD, 0x45);	outportb(EN_J_DQT_DAT, (128/0x18));
	outportb(EN_J_DQT_ADD, 0x46);	outportb(EN_J_DQT_DAT, (128/0x2F));
	outportb(EN_J_DQT_ADD, 0x47);	outportb(EN_J_DQT_DAT, (128/0x1A));
	outportb(EN_J_DQT_ADD, 0x48);	outportb(EN_J_DQT_DAT, (128/0x1A));
	outportb(EN_J_DQT_ADD, 0x49);	outportb(EN_J_DQT_DAT, (128/0x2F));
	outportb(EN_J_DQT_ADD, 0x4a);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x4b);	outportb(EN_J_DQT_DAT, (128/0x42));
	outportb(EN_J_DQT_ADD, 0x4c);	outportb(EN_J_DQT_DAT, (128/0x38));
	outportb(EN_J_DQT_ADD, 0x4d);	outportb(EN_J_DQT_DAT, (128/0x42));
	outportb(EN_J_DQT_ADD, 0x4e);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x4f);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x50);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x51);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x52);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x53);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x54);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x55);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x56);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x57);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x58);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x59);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x5a);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x5b);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x5c);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x5d);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x5e);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x5f);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x60);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x61);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x62);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x63);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x64);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x65);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x66);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x67);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x68);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x69);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x6a);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x6b);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x6c);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x6d);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x6e);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x6f);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x70);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x71);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x72);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x73);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x74);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x75);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x76);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x77);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x78);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x79);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x7a);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x7b);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x7c);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x7d);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x7e);	outportb(EN_J_DQT_DAT, (128/0x63));
	outportb(EN_J_DQT_ADD, 0x7f);	outportb(EN_J_DQT_DAT, (128/0x63));
#endif
#ifdef DQT_60  //60
	outportb(EN_J_DQT_ADD, 0x00);	outportb(EN_J_DQT_DAT, (128/0x0D));
	outportb(EN_J_DQT_ADD, 0x01);	outportb(EN_J_DQT_DAT, (128/0x09));	outportb(EN_J_DQT_ADD, 0x02);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x03);	outportb(EN_J_DQT_DAT, (128/0x0B));	outportb(EN_J_DQT_ADD, 0x04);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x05);	outportb(EN_J_DQT_DAT, (128/0x08));	outportb(EN_J_DQT_ADD, 0x06);	outportb(EN_J_DQT_DAT, (128/0x0D));	outportb(EN_J_DQT_ADD, 0x07);	outportb(EN_J_DQT_DAT, (128/0x0B));	outportb(EN_J_DQT_ADD, 0x08);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x09);	outportb(EN_J_DQT_DAT, (128/0x0B));	outportb(EN_J_DQT_ADD, 0x0A);	outportb(EN_J_DQT_DAT, (128/0x0E));	outportb(EN_J_DQT_ADD, 0x0B);	outportb(EN_J_DQT_DAT, (128/0x0E));	outportb(EN_J_DQT_ADD, 0x0C);	outportb(EN_J_DQT_DAT, (128/0x0D));	outportb(EN_J_DQT_ADD, 0x0D);	outportb(EN_J_DQT_DAT, (128/0x0F));	outportb(EN_J_DQT_ADD, 0x0E);	outportb(EN_J_DQT_DAT, (128/0x13));	outportb(EN_J_DQT_ADD, 0x0F);	outportb(EN_J_DQT_DAT, (128/0x20));	outportb(EN_J_DQT_ADD, 0x10);	outportb(EN_J_DQT_DAT, (128/0x15));	outportb(EN_J_DQT_ADD, 0x11);	outportb(EN_J_DQT_DAT, (128/0x13));	outportb(EN_J_DQT_ADD, 0x12);	outportb(EN_J_DQT_DAT, (128/0x12));	outportb(EN_J_DQT_ADD, 0x13);	outportb(EN_J_DQT_DAT, (128/0x12));	outportb(EN_J_DQT_ADD, 0x14);	outportb(EN_J_DQT_DAT, (128/0x13));	outportb(EN_J_DQT_ADD, 0x15);	outportb(EN_J_DQT_DAT, (128/0x27));	outportb(EN_J_DQT_ADD, 0x16);	outportb(EN_J_DQT_DAT, (128/0x1C));	outportb(EN_J_DQT_ADD, 0x17);	outportb(EN_J_DQT_DAT, (128/0x1E));	outportb(EN_J_DQT_ADD, 0x18);	outportb(EN_J_DQT_DAT, (128/0x17));	outportb(EN_J_DQT_ADD, 0x19);	outportb(EN_J_DQT_DAT, (128/0x20));	outportb(EN_J_DQT_ADD, 0x1A);	outportb(EN_J_DQT_DAT, (128/0x2E));	outportb(EN_J_DQT_ADD, 0x1B);	outportb(EN_J_DQT_DAT, (128/0x29));	outportb(EN_J_DQT_ADD, 0x1C);	outportb(EN_J_DQT_DAT, (128/0x31));	outportb(EN_J_DQT_ADD, 0x1D);	outportb(EN_J_DQT_DAT, (128/0x30));	outportb(EN_J_DQT_ADD, 0x1E);	outportb(EN_J_DQT_DAT, (128/0x2E));	outportb(EN_J_DQT_ADD, 0x1F);	outportb(EN_J_DQT_DAT, (128/0x29));	outportb(EN_J_DQT_ADD, 0x20);	outportb(EN_J_DQT_DAT, (128/0x2D));	outportb(EN_J_DQT_ADD, 0x21);	outportb(EN_J_DQT_DAT, (128/0x2C));	outportb(EN_J_DQT_ADD, 0x22);	outportb(EN_J_DQT_DAT, (128/0x33));	outportb(EN_J_DQT_ADD, 0x23);	outportb(EN_J_DQT_DAT, (128/0x3A));	outportb(EN_J_DQT_ADD, 0x24);	outportb(EN_J_DQT_DAT, (128/0x4A));	outportb(EN_J_DQT_ADD, 0x25);	outportb(EN_J_DQT_DAT, (128/0x3E));	outportb(EN_J_DQT_ADD, 0x26);	outportb(EN_J_DQT_DAT, (128/0x33));	outportb(EN_J_DQT_ADD, 0x27);	outportb(EN_J_DQT_DAT, (128/0x36));	outportb(EN_J_DQT_ADD, 0x28);	outportb(EN_J_DQT_DAT, (128/0x46));	outportb(EN_J_DQT_ADD, 0x29);	outportb(EN_J_DQT_DAT, (128/0x37));	outportb(EN_J_DQT_ADD, 0x2A);	outportb(EN_J_DQT_DAT, (128/0x2C));	outportb(EN_J_DQT_ADD, 0x2B);	outportb(EN_J_DQT_DAT, (128/0x2D));	outportb(EN_J_DQT_ADD, 0x2C);	outportb(EN_J_DQT_DAT, (128/0x40));	outportb(EN_J_DQT_ADD, 0x2D);	outportb(EN_J_DQT_DAT, (128/0x57));	outportb(EN_J_DQT_ADD, 0x2E);	outportb(EN_J_DQT_DAT, (128/0x41));	outportb(EN_J_DQT_ADD, 0x2F);	outportb(EN_J_DQT_DAT, (128/0x46));	outportb(EN_J_DQT_ADD, 0x30);	outportb(EN_J_DQT_DAT, (128/0x4C));	outportb(EN_J_DQT_ADD, 0x31);	outportb(EN_J_DQT_DAT, (128/0x4E));	outportb(EN_J_DQT_ADD, 0x32);	outportb(EN_J_DQT_DAT, (128/0x52));	outportb(EN_J_DQT_ADD, 0x33);	outportb(EN_J_DQT_DAT, (128/0x53));	outportb(EN_J_DQT_ADD, 0x34);	outportb(EN_J_DQT_DAT, (128/0x52));	outportb(EN_J_DQT_ADD, 0x35);	outportb(EN_J_DQT_DAT, (128/0x32));	outportb(EN_J_DQT_ADD, 0x36);	outportb(EN_J_DQT_DAT, (128/0x3E));	outportb(EN_J_DQT_ADD, 0x37);	outportb(EN_J_DQT_DAT, (128/0x5A));	outportb(EN_J_DQT_ADD, 0x38);	outportb(EN_J_DQT_DAT, (128/0x61));	outportb(EN_J_DQT_ADD, 0x39);	outportb(EN_J_DQT_DAT, (128/0x5A));	outportb(EN_J_DQT_ADD, 0x3A);	outportb(EN_J_DQT_DAT, (128/0x50));	outportb(EN_J_DQT_ADD, 0x3B);	outportb(EN_J_DQT_DAT, (128/0x60));	outportb(EN_J_DQT_ADD, 0x3C);	outportb(EN_J_DQT_DAT, (128/0x4A));	outportb(EN_J_DQT_ADD, 0x3D);	outportb(EN_J_DQT_DAT, (128/0x51));	outportb(EN_J_DQT_ADD, 0x3E);	outportb(EN_J_DQT_DAT, (128/0x52));	outportb(EN_J_DQT_ADD, 0x3F);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x40);	outportb(EN_J_DQT_DAT, (128/0x0E));	outportb(EN_J_DQT_ADD, 0x41);	outportb(EN_J_DQT_DAT, (128/0x0E));	outportb(EN_J_DQT_ADD, 0x42);	outportb(EN_J_DQT_DAT, (128/0x0E));	outportb(EN_J_DQT_ADD, 0x43);	outportb(EN_J_DQT_DAT, (128/0x13));	outportb(EN_J_DQT_ADD, 0x44);	outportb(EN_J_DQT_DAT, (128/0x11));	outportb(EN_J_DQT_ADD, 0x45);	outportb(EN_J_DQT_DAT, (128/0x13));	outportb(EN_J_DQT_ADD, 0x46);	outportb(EN_J_DQT_DAT, (128/0x26));	outportb(EN_J_DQT_ADD, 0x47);	outportb(EN_J_DQT_DAT, (128/0x15));	outportb(EN_J_DQT_ADD, 0x48);	outportb(EN_J_DQT_DAT, (128/0x15));	outportb(EN_J_DQT_ADD, 0x49);	outportb(EN_J_DQT_DAT, (128/0x26));	outportb(EN_J_DQT_ADD, 0x4A);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x4B);	outportb(EN_J_DQT_DAT, (128/0x35));	outportb(EN_J_DQT_ADD, 0x4C);	outportb(EN_J_DQT_DAT, (128/0x2D));	outportb(EN_J_DQT_ADD, 0x4D);	outportb(EN_J_DQT_DAT, (128/0x35));	outportb(EN_J_DQT_ADD, 0x4E);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x4F);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x50);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x51);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x52);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x53);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x54);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x55);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x56);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x57);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x58);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x59);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x5A);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x5B);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x5C);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x5D);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x5E);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x5F);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x60);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x61);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x62);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x63);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x64);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x65);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x66);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x67);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x68);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x69);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x6A);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x6B);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x6C);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x6D);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x6E);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x6F);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x70);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x71);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x72);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x73);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x74);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x75);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x76);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x77);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x78);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x79);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x7A);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x7B);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x7C);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x7D);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x7E);	outportb(EN_J_DQT_DAT, (128/0x4F));	outportb(EN_J_DQT_ADD, 0x7F);	outportb(EN_J_DQT_DAT, (128/0x4F));#endif
#ifdef DQT_70  //70
	outportb(EN_J_DQT_ADD, 0x00);	outportb(EN_J_DQT_DAT, (128/0x0A));
	outportb(EN_J_DQT_ADD, 0x01);	outportb(EN_J_DQT_DAT, (128/0x07));	outportb(EN_J_DQT_ADD, 0x02);	outportb(EN_J_DQT_DAT, (128/0x07));	outportb(EN_J_DQT_ADD, 0x03);	outportb(EN_J_DQT_DAT, (128/0x08));	outportb(EN_J_DQT_ADD, 0x04);	outportb(EN_J_DQT_DAT, (128/0x07));	outportb(EN_J_DQT_ADD, 0x05);	outportb(EN_J_DQT_DAT, (128/0x06));	outportb(EN_J_DQT_ADD, 0x06);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x07);	outportb(EN_J_DQT_DAT, (128/0x08));	outportb(EN_J_DQT_ADD, 0x08);	outportb(EN_J_DQT_DAT, (128/0x08));	outportb(EN_J_DQT_ADD, 0x09);	outportb(EN_J_DQT_DAT, (128/0x08));	outportb(EN_J_DQT_ADD, 0x0A);	outportb(EN_J_DQT_DAT, (128/0x0B));	outportb(EN_J_DQT_ADD, 0x0B);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x0C);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x0D);	outportb(EN_J_DQT_DAT, (128/0x0B));	outportb(EN_J_DQT_ADD, 0x0E);	outportb(EN_J_DQT_DAT, (128/0x0E));	outportb(EN_J_DQT_ADD, 0x0F);	outportb(EN_J_DQT_DAT, (128/0x18));	outportb(EN_J_DQT_ADD, 0x10);	outportb(EN_J_DQT_DAT, (128/0x10));	outportb(EN_J_DQT_ADD, 0x11);	outportb(EN_J_DQT_DAT, (128/0x0E));	outportb(EN_J_DQT_ADD, 0x12);	outportb(EN_J_DQT_DAT, (128/0x0D));	outportb(EN_J_DQT_ADD, 0x13);	outportb(EN_J_DQT_DAT, (128/0x0D));	outportb(EN_J_DQT_ADD, 0x14);	outportb(EN_J_DQT_DAT, (128/0x0E));	outportb(EN_J_DQT_ADD, 0x15);	outportb(EN_J_DQT_DAT, (128/0x1D));	outportb(EN_J_DQT_ADD, 0x16);	outportb(EN_J_DQT_DAT, (128/0x15));	outportb(EN_J_DQT_ADD, 0x17);	outportb(EN_J_DQT_DAT, (128/0x16));	outportb(EN_J_DQT_ADD, 0x18);	outportb(EN_J_DQT_DAT, (128/0x11));	outportb(EN_J_DQT_ADD, 0x19);	outportb(EN_J_DQT_DAT, (128/0x18));	outportb(EN_J_DQT_ADD, 0x1A);	outportb(EN_J_DQT_DAT, (128/0x23));	outportb(EN_J_DQT_ADD, 0x1B);	outportb(EN_J_DQT_DAT, (128/0x1F));	outportb(EN_J_DQT_ADD, 0x1C);	outportb(EN_J_DQT_DAT, (128/0x25));	outportb(EN_J_DQT_ADD, 0x1D);	outportb(EN_J_DQT_DAT, (128/0x24));	outportb(EN_J_DQT_ADD, 0x1E);	outportb(EN_J_DQT_DAT, (128/0x22));	outportb(EN_J_DQT_ADD, 0x1F);	outportb(EN_J_DQT_DAT, (128/0x1F));	outportb(EN_J_DQT_ADD, 0x20);	outportb(EN_J_DQT_DAT, (128/0x22));	outportb(EN_J_DQT_ADD, 0x21);	outportb(EN_J_DQT_DAT, (128/0x21));	outportb(EN_J_DQT_ADD, 0x22);	outportb(EN_J_DQT_DAT, (128/0x26));	outportb(EN_J_DQT_ADD, 0x23);	outportb(EN_J_DQT_DAT, (128/0x2B));	outportb(EN_J_DQT_ADD, 0x24);	outportb(EN_J_DQT_DAT, (128/0x37));	outportb(EN_J_DQT_ADD, 0x25);	outportb(EN_J_DQT_DAT, (128/0x2F));	outportb(EN_J_DQT_ADD, 0x26);	outportb(EN_J_DQT_DAT, (128/0x26));	outportb(EN_J_DQT_ADD, 0x27);	outportb(EN_J_DQT_DAT, (128/0x29));	outportb(EN_J_DQT_ADD, 0x28);	outportb(EN_J_DQT_DAT, (128/0x34));	outportb(EN_J_DQT_ADD, 0x29);	outportb(EN_J_DQT_DAT, (128/0x29));	outportb(EN_J_DQT_ADD, 0x2A);	outportb(EN_J_DQT_DAT, (128/0x21));	outportb(EN_J_DQT_ADD, 0x2B);	outportb(EN_J_DQT_DAT, (128/0x22));	outportb(EN_J_DQT_ADD, 0x2C);	outportb(EN_J_DQT_DAT, (128/0x30));	outportb(EN_J_DQT_ADD, 0x2D);	outportb(EN_J_DQT_DAT, (128/0x41));	outportb(EN_J_DQT_ADD, 0x2E);	outportb(EN_J_DQT_DAT, (128/0x31));	outportb(EN_J_DQT_ADD, 0x2F);	outportb(EN_J_DQT_DAT, (128/0x34));	outportb(EN_J_DQT_ADD, 0x30);	outportb(EN_J_DQT_DAT, (128/0x39));	outportb(EN_J_DQT_ADD, 0x31);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x32);	outportb(EN_J_DQT_DAT, (128/0x3E));	outportb(EN_J_DQT_ADD, 0x33);	outportb(EN_J_DQT_DAT, (128/0x3E));	outportb(EN_J_DQT_ADD, 0x34);	outportb(EN_J_DQT_DAT, (128/0x3E));	outportb(EN_J_DQT_ADD, 0x35);	outportb(EN_J_DQT_DAT, (128/0x25));	outportb(EN_J_DQT_ADD, 0x36);	outportb(EN_J_DQT_DAT, (128/0x2E));	outportb(EN_J_DQT_ADD, 0x37);	outportb(EN_J_DQT_DAT, (128/0x44));	outportb(EN_J_DQT_ADD, 0x38);	outportb(EN_J_DQT_DAT, (128/0x49));	outportb(EN_J_DQT_ADD, 0x39);	outportb(EN_J_DQT_DAT, (128/0x43));	outportb(EN_J_DQT_ADD, 0x3A);	outportb(EN_J_DQT_DAT, (128/0x3C));	outportb(EN_J_DQT_ADD, 0x3B);	outportb(EN_J_DQT_DAT, (128/0x48));	outportb(EN_J_DQT_ADD, 0x3C);	outportb(EN_J_DQT_DAT, (128/0x37));	outportb(EN_J_DQT_ADD, 0x3D);	outportb(EN_J_DQT_DAT, (128/0x3D));	outportb(EN_J_DQT_ADD, 0x3E);	outportb(EN_J_DQT_DAT, (128/0x3E));	outportb(EN_J_DQT_ADD, 0x3F);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x40);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x41);	outportb(EN_J_DQT_DAT, (128/0x0B));	outportb(EN_J_DQT_ADD, 0x42);	outportb(EN_J_DQT_DAT, (128/0x0B));	outportb(EN_J_DQT_ADD, 0x43);	outportb(EN_J_DQT_DAT, (128/0x0E));	outportb(EN_J_DQT_ADD, 0x44);	outportb(EN_J_DQT_DAT, (128/0x0D));	outportb(EN_J_DQT_ADD, 0x45);	outportb(EN_J_DQT_DAT, (128/0x0E));	outportb(EN_J_DQT_ADD, 0x46);	outportb(EN_J_DQT_DAT, (128/0x1C));	outportb(EN_J_DQT_ADD, 0x47);	outportb(EN_J_DQT_DAT, (128/0x10));	outportb(EN_J_DQT_ADD, 0x48);	outportb(EN_J_DQT_DAT, (128/0x10));	outportb(EN_J_DQT_ADD, 0x49);	outportb(EN_J_DQT_DAT, (128/0x1C));	outportb(EN_J_DQT_ADD, 0x4A);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x4B);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x4C);	outportb(EN_J_DQT_DAT, (128/0x22));	outportb(EN_J_DQT_ADD, 0x4D);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x4E);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x4F);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x50);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x51);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x52);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x53);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x54);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x55);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x56);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x57);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x58);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x59);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x5A);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x5B);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x5C);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x5D);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x5E);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x5F);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x60);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x61);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x62);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x63);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x64);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x65);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x66);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x67);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x68);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x69);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x6A);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x6B);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x6C);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x6D);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x6E);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x6F);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x70);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x71);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x72);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x73);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x74);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x75);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x76);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x77);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x78);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x79);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x7A);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x7B);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x7C);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x7D);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x7E);	outportb(EN_J_DQT_DAT, (128/0x3B));	outportb(EN_J_DQT_ADD, 0x7F);	outportb(EN_J_DQT_DAT, (128/0x3B));#endif
#ifdef DQT_80   //80
	outportb(EN_J_DQT_ADD, 0x00);	outportb(EN_J_DQT_DAT, (128/0x06));
	outportb(EN_J_DQT_ADD, 0x01);	outportb(EN_J_DQT_DAT, (128/0x04));	outportb(EN_J_DQT_ADD, 0x02);	outportb(EN_J_DQT_DAT, (128/0x05));	outportb(EN_J_DQT_ADD, 0x03);	outportb(EN_J_DQT_DAT, (128/0x06));	outportb(EN_J_DQT_ADD, 0x04);	outportb(EN_J_DQT_DAT, (128/0x05));	outportb(EN_J_DQT_ADD, 0x05);	outportb(EN_J_DQT_DAT, (128/0x04));	outportb(EN_J_DQT_ADD, 0x06);	outportb(EN_J_DQT_DAT, (128/0x06));	outportb(EN_J_DQT_ADD, 0x07);	outportb(EN_J_DQT_DAT, (128/0x06));	outportb(EN_J_DQT_ADD, 0x08);	outportb(EN_J_DQT_DAT, (128/0x05));	outportb(EN_J_DQT_ADD, 0x09);	outportb(EN_J_DQT_DAT, (128/0x06));	outportb(EN_J_DQT_ADD, 0x0A);	outportb(EN_J_DQT_DAT, (128/0x07));	outportb(EN_J_DQT_ADD, 0x0B);	outportb(EN_J_DQT_DAT, (128/0x07));	outportb(EN_J_DQT_ADD, 0x0C);	outportb(EN_J_DQT_DAT, (128/0x06));	outportb(EN_J_DQT_ADD, 0x0D);	outportb(EN_J_DQT_DAT, (128/0x08));	outportb(EN_J_DQT_ADD, 0x0E);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x0F);	outportb(EN_J_DQT_DAT, (128/0x10));	outportb(EN_J_DQT_ADD, 0x10);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x11);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x12);	outportb(EN_J_DQT_DAT, (128/0x09));	outportb(EN_J_DQT_ADD, 0x13);	outportb(EN_J_DQT_DAT, (128/0x09));	outportb(EN_J_DQT_ADD, 0x14);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x15);	outportb(EN_J_DQT_DAT, (128/0x14));	outportb(EN_J_DQT_ADD, 0x16);	outportb(EN_J_DQT_DAT, (128/0x0E));	outportb(EN_J_DQT_ADD, 0x17);	outportb(EN_J_DQT_DAT, (128/0x0F));	outportb(EN_J_DQT_ADD, 0x18);	outportb(EN_J_DQT_DAT, (128/0x0C));	outportb(EN_J_DQT_ADD, 0x19);	outportb(EN_J_DQT_DAT, (128/0x10));	outportb(EN_J_DQT_ADD, 0x1A);	outportb(EN_J_DQT_DAT, (128/0x17));	outportb(EN_J_DQT_ADD, 0x1B);	outportb(EN_J_DQT_DAT, (128/0x14));	outportb(EN_J_DQT_ADD, 0x1C);	outportb(EN_J_DQT_DAT, (128/0x18));	outportb(EN_J_DQT_ADD, 0x1D);	outportb(EN_J_DQT_DAT, (128/0x18));	outportb(EN_J_DQT_ADD, 0x1E);	outportb(EN_J_DQT_DAT, (128/0x17));	outportb(EN_J_DQT_ADD, 0x1F);	outportb(EN_J_DQT_DAT, (128/0x14));	outportb(EN_J_DQT_ADD, 0x20);	outportb(EN_J_DQT_DAT, (128/0x16));	outportb(EN_J_DQT_ADD, 0x21);	outportb(EN_J_DQT_DAT, (128/0x16));	outportb(EN_J_DQT_ADD, 0x22);	outportb(EN_J_DQT_DAT, (128/0x1A));	outportb(EN_J_DQT_ADD, 0x23);	outportb(EN_J_DQT_DAT, (128/0x1D));	outportb(EN_J_DQT_ADD, 0x24);	outportb(EN_J_DQT_DAT, (128/0x25));	outportb(EN_J_DQT_ADD, 0x25);	outportb(EN_J_DQT_DAT, (128/0x1F));	outportb(EN_J_DQT_ADD, 0x26);	outportb(EN_J_DQT_DAT, (128/0x1A));	outportb(EN_J_DQT_ADD, 0x27);	outportb(EN_J_DQT_DAT, (128/0x1B));	outportb(EN_J_DQT_ADD, 0x28);	outportb(EN_J_DQT_DAT, (128/0x23));	outportb(EN_J_DQT_ADD, 0x29);	outportb(EN_J_DQT_DAT, (128/0x1C));	outportb(EN_J_DQT_ADD, 0x2A);	outportb(EN_J_DQT_DAT, (128/0x16));	outportb(EN_J_DQT_ADD, 0x2B);	outportb(EN_J_DQT_DAT, (128/0x16));	outportb(EN_J_DQT_ADD, 0x2C);	outportb(EN_J_DQT_DAT, (128/0x20));	outportb(EN_J_DQT_ADD, 0x2D);	outportb(EN_J_DQT_DAT, (128/0x2C));	outportb(EN_J_DQT_ADD, 0x2E);	outportb(EN_J_DQT_DAT, (128/0x20));	outportb(EN_J_DQT_ADD, 0x2F);	outportb(EN_J_DQT_DAT, (128/0x23));	outportb(EN_J_DQT_ADD, 0x30);	outportb(EN_J_DQT_DAT, (128/0x26));	outportb(EN_J_DQT_ADD, 0x31);	outportb(EN_J_DQT_DAT, (128/0x27));	outportb(EN_J_DQT_ADD, 0x32);	outportb(EN_J_DQT_DAT, (128/0x29));	outportb(EN_J_DQT_ADD, 0x33);	outportb(EN_J_DQT_DAT, (128/0x2A));	outportb(EN_J_DQT_ADD, 0x34);	outportb(EN_J_DQT_DAT, (128/0x29));	outportb(EN_J_DQT_ADD, 0x35);	outportb(EN_J_DQT_DAT, (128/0x19));	outportb(EN_J_DQT_ADD, 0x36);	outportb(EN_J_DQT_DAT, (128/0x1F));	outportb(EN_J_DQT_ADD, 0x37);	outportb(EN_J_DQT_DAT, (128/0x2D));	outportb(EN_J_DQT_ADD, 0x38);	outportb(EN_J_DQT_DAT, (128/0x30));	outportb(EN_J_DQT_ADD, 0x39);	outportb(EN_J_DQT_DAT, (128/0x2D));	outportb(EN_J_DQT_ADD, 0x3A);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x3B);	outportb(EN_J_DQT_DAT, (128/0x30));	outportb(EN_J_DQT_ADD, 0x3C);	outportb(EN_J_DQT_DAT, (128/0x25));	outportb(EN_J_DQT_ADD, 0x3D);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x3E);	outportb(EN_J_DQT_DAT, (128/0x29));	outportb(EN_J_DQT_ADD, 0x3F);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x40);	outportb(EN_J_DQT_DAT, (128/0x07));	outportb(EN_J_DQT_ADD, 0x41);	outportb(EN_J_DQT_DAT, (128/0x07));	outportb(EN_J_DQT_ADD, 0x42);	outportb(EN_J_DQT_DAT, (128/0x07));	outportb(EN_J_DQT_ADD, 0x43);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x44);	outportb(EN_J_DQT_DAT, (128/0x08));	outportb(EN_J_DQT_ADD, 0x45);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x46);	outportb(EN_J_DQT_DAT, (128/0x13));	outportb(EN_J_DQT_ADD, 0x47);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x48);	outportb(EN_J_DQT_DAT, (128/0x0A));	outportb(EN_J_DQT_ADD, 0x49);	outportb(EN_J_DQT_DAT, (128/0x13));	outportb(EN_J_DQT_ADD, 0x4A);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x4B);	outportb(EN_J_DQT_DAT, (128/0x1A));	outportb(EN_J_DQT_ADD, 0x4C);	outportb(EN_J_DQT_DAT, (128/0x16));	outportb(EN_J_DQT_ADD, 0x4D);	outportb(EN_J_DQT_DAT, (128/0x1A));	outportb(EN_J_DQT_ADD, 0x4E);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x4F);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x50);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x51);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x52);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x53);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x54);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x55);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x56);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x57);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x58);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x59);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x5A);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x5B);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x5C);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x5D);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x5E);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x5F);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x60);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x61);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x62);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x63);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x64);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x65);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x66);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x67);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x68);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x69);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x6A);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x6B);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x6C);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x6D);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x6E);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x6F);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x70);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x71);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x72);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x73);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x74);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x75);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x76);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x77);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x78);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x79);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x7A);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x7B);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x7C);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x7D);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x7E);	outportb(EN_J_DQT_DAT, (128/0x28));	outportb(EN_J_DQT_ADD, 0x7F);	outportb(EN_J_DQT_DAT, (128/0x28));#endif	outportb(EN_J_DQT_SHIFT, 7);
}



