#include "includes.h"

#include "stdlib.h"

#define x_gpio_h

#define CommonH
#include "common.h"                      

/*
****************************************
201309
网络灯修改
****************************************
*/


#define TOUCH_IO     0x10              //触摸屏
#define WATCHDOG_IO     0x04              //看门狗

#define IO_S1     0x01    //GPIOD0
#define IO_S2     0x02    //GPIOD1
#define IO_S3     0x04    //GPIOD2
#define IO_S4     0x08    //GPIOD3
#define IO_S5     0x10    //GPIOD4
#define IO_S7     0x80    //GPIOD7

struct gpio_key1         //触摸屏检测
{
	INT8U curr_input; //本次状态
	INT8U downnum;  //检测到有输入次数
	INT8U press_flag;
};
struct gpio_key1 gpio_key[8];
void get_key_value(INT8U gpiovalue);

INT16U openlockpressnum = 0;      //开锁键长按计数
INT16U touchpressnum = 0;      //触摸屏长按计数
struct touch_io1         //触摸屏检测
{
	INT8U curr_input; //本次状态
	INT8U downnum;  //检测到有输入次数
	INT8U press_flag;
	INT8U touch_count; //触摸屏按下计数
};
struct touch_io1 touch_io;

INT16U timenum = 0;
void InitGpio(void);  //初始化GPIO
void CheckGpio(void);    //检测GPIO函数
void SetGpio(INT16U IO_Register, INT8U userIO, INT8U SetFlag);  //设置GPIO

void InitKeyDelay(void);
void CheckKeyDelay(INT16U timenum);

INT8U Read_X(void);
INT8U Touch_Read_XY(void); //触摸屏I2C读取函数

void ClearWatchDog(void);
//---------------------------------------------------------------------------
void InitGpio(void)  //初始化GPIO
{
	INT8U i;
	xtm_printf("InitGpio...\n");
	for(i=0; i<8; i++)
	{
		gpio_key[i].curr_input = 0;
		gpio_key[i].press_flag = 0;
		gpio_key[i].downnum = 0;
	} 
	//outportb(GPIOA_OEN_DATA, _GPIOA_MUX | 0x00);

	outportb(GPIOA_OEN_DATA, inportb(GPIOA_OEN_DATA) | TOUCH_IO);   //gpioa4 为输入
	outportb(GPIOA_OEN_DATA, inportb(GPIOA_OEN_DATA) & 0xDB);       //11011011 gpioa2、gpio5 为输出

	//outportb(GPIOB_SEL, inportb(GPIOB_SEL) | 0xC0);
	//outportb(GPIOB_OEN_DATA, inportb(GPIOB_OEN_DATA) & 0x3F);
#ifdef _CAMERA_SUPPORT     //如不用摄像头，GPIOB6、7为Comm1, GPIOC 作为8个防区使用
	//如用于 OV7725 注释下面行
	//outportb(GPIOB_SEL, inportb(GPIOB_SEL) & 0x3F);   //GPIOB6、GPIOB7 串口1


	//outportb(GPIOC_SEL, inportb(GPIOC_SEL) | 0xFF);
	//outportb(GPIOC_OEN_DATA, inportb(GPIOC_OEN_DATA) & 0x00);    //输出
	//outportb(GPIOC_OEN_DATA, inportb(GPIOC_OEN_DATA) | 0xFF);    //输入
#else
	//如用于 OV7725 注释下面行
	outportb(GPIOB_SEL, inportb(GPIOB_SEL) & 0x3F);   //GPIOB6、GPIOB7 串口1

	outportb(GPIOC_SEL, inportb(GPIOC_SEL) | 0xFF);
	//outportb(GPIOC_OEN_DATA, inportb(GPIOC_OEN_DATA) & 0x00);    //输出
	outportb(GPIOC_OEN_DATA, inportb(GPIOC_OEN_DATA) | 0xFF);    //输入
#endif

	//GPIOD
	outportb(GPIOD_SEL, inportb(GPIOD_SEL) | 0xFF);
	//0-4 输入
	//outportb(GPIOD_OEN_DATA, inportb(GPIOD_OEN_DATA) | 0x1F);
	//0、1、2、4、7输入
	outportb(GPIOD_OEN_DATA, inportb(GPIOD_OEN_DATA) | 0x97);
	//5-7 输出  LED1、2、3    信息、布防、网络 
	//outportb(GPIOD_OEN_DATA, inportb(GPIOD_OEN_DATA) & 0x1F);
	//6、5、4 信息、布防、网络  20130917
	outportb(GPIOD_OEN_DATA, inportb(GPIOD_OEN_DATA) & 0x97);
#if 0
	outportb(GPIOA_OUT_DATA, inportb(GPIOA_OUT_DATA) & 0xdf);
	//  outportb(GPIOC_SEL, 0xFF);  //GPIO
	//  outportb(GPIOC_OEN_DATA, 0x00); //输出
	outportb(GPIOA_OUT_DATA, inportb(GPIOA_OUT_DATA) | 0x38);
	xtm_printf("1 inportb(GPIOA_OUT_DATA) = 0x%X\n", inportb(GPIOA_OUT_DATA));
	outportb(GPIOA_OUT_DATA, inportb(GPIOA_OUT_DATA) & 0xc7);
	xtm_printf("2 inportb(GPIOA_OUT_DATA) = 0x%X\n", inportb(GPIOA_OUT_DATA));

#endif
	InitKeyDelay();
	xtm_printf("1 inportb(GPIOA_OEN_DATA) = 0x%X\n", inportb(GPIOA_OEN_DATA));
	xtm_printf("2 inportb(GPIOD_OEN_DATA) = 0x%X\n", inportb(GPIOD_OEN_DATA));
	xtm_printf("3 inportb(GPIOD_IN_DATA) = 0x%X\n", inportb(GPIOD_IN_DATA));

	//SetGpio(GPIOD_OUT_DATA, INFO_LED_HIGH, SET_HIGH); //关信息LED  //
	//SetGpio(GPIOD_OUT_DATA, NET_LED_HIGH, SET_HIGH); //关网络LED    //
	if(LocalCfg.DefenceStatus != 0)
	{
		//SetGpio(GPIOD_OUT_DATA, FORTIFY_LED_LOW, SET_LOW); //开布防LED  //
		Local.AlarmStatus=1;
	}
	else
	{
		//SetGpio(GPIOD_OUT_DATA, FORTIFY_LED_HIGH, SET_HIGH); //关布防LED   //
		Local.AlarmStatus=0;
	}

	//SetGpio(GPIOA_OUT_DATA, ALARM_OUT_LOW, SET_LOW); //关报警输出   //
	Local.AlarmStatus=0;

	//SetGpio(GPIOA_OUT_DATA, ALARM_OUT_HIGH, SET_HIGH); //开报警输出  //
	ClearWatchDog();
	xtm_printf("4 inportb(GPIOD_OUT_DATA) = 0x%X\n", inportb(GPIOD_OUT_DATA));

}
//---------------------------------------------------------------------------
#if 1
void InitKeyDelay(void)
{
	Local.WatchKeyTouched = 0;  //监视键 响应
	Local.WatchKeyTouchedTime = 0;  //监视键 响应 计数

	Local.CenterKeyTouched = 0;  //呼叫中心键 响应
	Local.CenterKeyTouchedTime = 0;  //呼叫中心键 响应 计数
}
//---------------------------------------------------------------------------
void CheckKeyDelay(INT16U timenum)
{
	if(Local.WatchKeyTouched == 1)      //监视键 响应     20101029  xu
	{
		if((timenum % (GPIOPERSEC))==0)
		{
			Local.WatchKeyTouchedTime ++;      //监视键 响应 计数
			xtm_printf("监视键 延时中...  %d\n", Local.WatchKeyTouchedTime);
		}
		if(Local.WatchKeyTouchedTime >= 2)
		{
			Local.WatchKeyTouched = 0;      //监视键 响应 标志     20101029  xu
			Local.WatchKeyTouchedTime = 0;      //监视键 响应 计数
		}
	}

	if(Local.CenterKeyTouched == 1)      //呼叫中心键 响应     20101029  xu
	{
		if((timenum % (GPIOPERSEC))==0)
		{
			Local.CenterKeyTouchedTime ++;      //呼叫中心键 响应 计数
			xtm_printf("呼叫中心键 延时中...  %d\n", Local.CenterKeyTouchedTime);
		}
		if(Local.CenterKeyTouchedTime >= 2)
		{
			Local.CenterKeyTouched = 0;      //呼叫中心键 响应 标志     20101029  xu
			Local.CenterKeyTouchedTime = 0;      //呼叫中心键 响应 计数
		}
	}
}
#endif
//---------------------------------------------------------------------------
void SetGpio(INT16U IO_Register, INT8U userIO, INT8U SetFlag)  //设置GPIO
{
	if(SetFlag == SET_LOW)
		outportb(IO_Register, inportb(IO_Register) & userIO);
	else
		outportb(IO_Register, inportb(IO_Register) | userIO);
}
//---------------------------------------------------------------------------
void CheckGpio(void)    //检测GPIO函数
{
	INT8U gpio_value;
	INT8U tmpvalue, i;

	ClearWatchDog();

	tmpvalue = 0;
	gpio_value = 0;

	CheckKeyDelay(timenum);
	timenum ++;
#ifndef _CAMERA_SUPPORT     //如不用摄像头，GPIOB6、7为Comm1, GPIOC 作为8个防区使用
	CheckDefence(inportb(GPIOC_IN_DATA));
#endif
#if 1
	tmpvalue = inportb(GPIOA_IN_DATA);

	//xtm_printf("tmpvalue = 0x%X\n", tmpvalue);
	touch_io.curr_input = 0;
	if(gpio_value == 0)
		if(!(tmpvalue & TOUCH_IO) )
		{
			//触摸屏检测
			touch_io.curr_input = 1; //本次状态
		}

		if(touch_io.curr_input == 1)
			touchpressnum ++;                //触摸屏长按计数
		else
			touchpressnum = 0;

		//触摸屏长按8S以上
		if((touchpressnum > (GPIOPERSEC * 8))&&(Local.CurrentWindow != 190)&&(Local.CurrentWindow != 191))
		{
			touchpressnum = 0;
			if((Local.Status == 0)&&(Local.AlarmByte[0] == 0)&&(Local.AlarmByte[1] == 0))
				DisplayCalibrateWindow();   //校准触摸屏
		}

		//触摸屏检测
		if(touch_io.curr_input == 0) //本次状态
		{
			touch_io.downnum = 0;  //检测到有输入次数
			touch_io.press_flag = 0;
			touch_io.touch_count = 0; //触摸屏按下计数
		} 
		else
		{
			//xtm_printf("tmpvalue = 0x%X\n", tmpvalue);
			touch_io.downnum ++;  //检测到有输入次数
			if(Read_X() == 1)
				touch_io.touch_count ++;
			if(touch_io.press_flag == 0)
				if(touch_io.downnum >= 5)
				{
					touch_io.press_flag = 1;
					xtm_printf("tmpvalue = 0x%X\n", tmpvalue);
					//xtm_printf("CONTROL_REG 11= 0x%X\n", inportb(CONTROL_REG));
					touch_io.downnum = 0;

					if(touch_io.touch_count >= 2)
						Touch_Read_XY();
					else
					{
						gpio_key[5].curr_input = 1; //本次状态
						gpio_value = 1;
						get_key_value(5);
					}
					return;
				}
		}
#endif
		tmpvalue = inportb(GPIOD_IN_DATA);
		//xtm_printf("CheckGpio::GPIOB tmpvalue = 0x%X\n", tmpvalue);
		gpio_key[0].curr_input = 0;
		if(gpio_value == 0)
			if(!(tmpvalue & IO_S1) )
			{
				gpio_key[0].curr_input = 1; //本次状态
				gpio_value = 1;
			}

			gpio_key[1].curr_input = 0;
			if(gpio_value == 0)
				if(!(tmpvalue & IO_S2) )
				{
					gpio_key[1].curr_input = 1; //本次状态
					gpio_value = 1;
				}

				gpio_key[2].curr_input = 0;
				if(gpio_value == 0)
					if(!(tmpvalue & IO_S3) )
					{
						gpio_key[2].curr_input = 1; //本次状态
						gpio_value = 1;
					}

					//gpio_key[3].curr_input = 0;
					// if(gpio_value == 0)
					//if(!(tmpvalue & IO_S4) )
					// {
					// gpio_key[3].curr_input = 1; //本次状态
					//  gpio_value = 1;
					// }

					gpio_key[4].curr_input = 0;
					if(gpio_value == 0)
						if(!(tmpvalue & IO_S5) )
						{
							gpio_key[4].curr_input = 1; //本次状态
							gpio_value = 1;
						}

						//20131205
#ifdef SMART_BELL
						gpio_key[6].curr_input = 0;
						if(gpio_value == 0)
							if(!(tmpvalue & IO_S7) )
							{
								gpio_key[6].curr_input = 1; //本次状态
								gpio_value = 1;
							}
#endif

							//开锁键长按计数  
							if(gpio_key[2].curr_input == 1)
								openlockpressnum ++;
							else
								openlockpressnum = 0;

							//开锁键长按3S以上
							if((openlockpressnum > (GPIOPERSEC * 3))&&(Local.CurrentWindow != 190)&&(Local.CurrentWindow != 191))
							{
								openlockpressnum = 0;
								if((Local.Status == 0)&&(Local.AlarmByte[0] == 0)&&(Local.AlarmByte[1] == 0))
									DisplayCalibrateWindow();   //校准触摸屏
							}

							//IO检测
							for(i=0; i<8; i++)
							{
								if(gpio_key[i].curr_input == 0) //本次状态
								{
									if(gpio_key[i].press_flag == 0)
									{
										if(gpio_key[i].downnum >= 5)
										{
											gpio_key[i].press_flag = 1;
											get_key_value(i);

											gpio_key[i].downnum = 0;
										}
									}
									else
									{
										gpio_key[i].downnum = 0;  //检测到有输入次数
										gpio_key[i].press_flag = 0;
									} 
								}
								else
								{
									gpio_key[i].downnum ++;  //检测到有输入次数

#ifdef SMART_BELL
									if(i==6)
									{
										if(gpio_key[i].downnum >= 20)
										{ 
											gpio_key[i].press_flag = 1;
											get_key_value(i);

											gpio_key[i].downnum = 0;
										}
									}
#endif

								}
							}
}
//---------------------------------------------------------------------------
INT8U wav_num = 20;
void get_key_value(INT8U gpiovalue)
{
	INT8U val;
	//已经处在结束通话切换界面状态
	if(PicStatBuf.Flag != 0)
	{
		xtm_printf("get_key_value::PicStatBuf.Flag = %d\n", PicStatBuf.Flag);
		return;
	}
	Local.LcdLightTime = 0;
	if(Local.Status == 0)
	{
		//PlaySoundTip(); //播放提示音
	}

	if(Local.LcdLightFlag == 0)
	{
		//点亮LCD背光
		Lcd_Power(1, LocalCfg.BrightVolume);
		Local.LcdLightFlag = 1; //LCD背光标志
		Local.LcdLightTime = 0; //时间
	}
	else
	{
#if 1
		switch(gpiovalue)
		{
		case 0:   //0  //通话
			xtm_printf("get_key_value::IO_S1\n");
			if((Local.CurrentWindow == 16)&&(Local.Status != 0))  //通话
			{
				switch(Local.Status)
				{

				case 2: //被叫对讲
					OperateTalkWatchPicWindow(0, 16);
					break;
				case 1:                             
				case 5: //通话
				case 6: //通话
					OperateTalkWatchPicWindow(2, 16);
					break;

				}
				Local.RecordPic = 0;   //呼叫留照片
				break;
			}
            if((Local.Status == 89))
            {
                OperateSmarkStreamWindow(0,89);
                break;
            }

			//Writees8388(0x0e, 0x30);
			/* xtm_printf("3.0 inportb(GPIOA_OEN_DATA) = 0x%X\n", inportb(GPIOA_OEN_DATA));
			val = Reades8388(0x0e);
			xtm_printf("reg = 0x%X, val = 0x%X\n", 0x0e, val);
			xtm_printf("3.1 inportb(GPIOA_OEN_DATA) = 0x%X\n", inportb(GPIOA_OEN_DATA));
			test_es8328e();
			xtm_printf("3.2 inportb(GPIOA_OEN_DATA) = 0x%X\n", inportb(GPIOA_OEN_DATA));        */
			//Writees8388( 0x16, 0x00);
			//Writees8388(0x0e, 0x00);

			//Local.LcdLightFlag = 0;
			// StartRecAudio();
			// Lcd_Power(0, LocalCfg.BrightVolume);
			//outportb(PTC0_CTRL0,0x01);
			//Disp_Enable(1);    //开显示

			//WaitAudioUnuse(200); //等待声卡空闲
			//StartPlayWav(test_wav.Faddr, CYCLE_PLAY);

			//DelayMs(300);
			/*     StartPlayWav(test_wav.Faddr, CYCLE_PLAY);
			//xtm_printf("wav_1k[wav_num] = %d\n", wav_num);
			//StartPlayWav(wav_1k[wav_num].Faddr, NO_CYCLE_PLAY);
			wav_num ++;
			if(wav_num > 20)
			wav_num = 0;   */
			break;
		case 1:   //1     监视
			xtm_printf("get_key_value::IO_S2\n");
			// DisplayImage(&main_image11111, FROMFILE, NOFULLPAGE);
			if(Local.WatchKeyTouched == 1)  //监视键 响应
				break;

			Local.WatchKeyTouched = 1;  //监视键 响应
			Local.WatchKeyTouchedTime = 0;  //监视键 响应 计数

			//不在报警或取消报警窗口下
			if((Local.CurrentWindow != 34)&&(Local.CurrentWindow != 35))
			{
				if((Local.Status == 0)&&(Local.CurrentWindow != 16))
				{
					DisplayTalkWatchPicWindow(MAINRETURNTYPE, WATCHWINTYPE);
					Watch_Func();
				}
				else
					if((Local.Status == 0)&&(Local.CurrentWindow == 13))
						Watch_Func();
				if(Local.Status == 3)
				{
					WatchEnd_Func();
				}
			}

			break;
		case 2:     // 开锁
			xtm_printf("get_key_value::IO_S3\n");
#if 1
#ifdef _ZHUHAIJINZHEN      //珠海进祯  呼叫时可开锁
			if((Local.Status == 2)||(Local.Status == 6))  //状态为被对讲 或被叫通话
#else
			if(Local.Status == 6)  //状态为被对讲
#endif
			{
				OpenLock_Func();
			}
#else
			strcpy(Remote.Addr[0], "S00010308090");
			strcpy(TalkCall.TalkText, "0001030809");

			if(Local.Status == 0)
			{
				if(Local.CurrentWindow == 16)
					break;
				DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, TALKWINTYPE);
				CallR2R_Func(TalkCall.TalkText);
			}
#endif
			break;
		case 3:  //留影
			xtm_printf("get_key_value::IO_S4\n");
			if((Local.Status == 2)||(Local.Status == 6))
			{
				Local.ManualRecordFlag = 1; //手动录制
				Local.IFrameCount = 30;
				break;
			}

			//不在报警或取消报警窗口下
			if((Local.CurrentWindow != 34)&&(Local.CurrentWindow != 35))
				if((Local.Status == 0)&&(Local.CurrentWindow != 16))
					if(Local.CurrentWindow != 21)
					{
						DisplayRecordsWindow(REFRESH_MIDDLE | REFRESH_BOTTOM, 0);    //刷新 中 下
					}

					//Local.LcdLightFlag = 0;
					//StartRecAudio();
					//Lcd_Power(1, 5);
					/*if(Local.Status == 0)
					{
					if(Local.CurrentWindow == 16)
					break;
					DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, WATCHWINTYPE);
					Watch_Func();
					}    */
					break;
		case 4:   //信息 20131010改为呼叫中心 测试用
			xtm_printf("get_key_value::IO_S5\n");

			/* StopEncodeVideo();
			StopRecVideo();
			StopVideoClock();            //关闭视频采集编码时钟      */


			//不在报警或取消报警窗口下
			if((Local.CurrentWindow != 34)&&(Local.CurrentWindow != 35))
				if((Local.Status == 0)&&(Local.CurrentWindow != 16))
					//if(Local.CurrentWindow != 53)
				{
					//DisplayInfoWindow(REFRESH_MIDDLE | REFRESH_BOTTOM, 0);    //刷新 中 下
					//20131010
					xtm_memcopy(Remote.Addr[0], "Z00010000000", 12);
					Remote.Addr[0][12] = '\0';
					DisplayTalkWatchPicWindow(MAINRETURNTYPE, TALKWINTYPE);
					CallCenter_Func("Z00010000000");
					//20131014
					//DisplaySplashWindow(REFRESH_MIDDLE);

				}

				break;
		case 5:  //中心
			xtm_printf("get_key_value::IO_S6\n");
			if(Local.CenterKeyTouched == 1)  //呼叫中心键 响应
				break;
			Local.CenterKeyTouched = 1;  //呼叫中心键 响应
			Local.CenterKeyTouchedTime = 0;  //呼叫中心键 响应 计数

			//不在报警或取消报警窗口下
			if((Local.CurrentWindow != 34)&&(Local.CurrentWindow != 35))
			{
				if((Local.Status == 0)&&(Local.CurrentWindow != 16))
				{
					xtm_memcopy(Remote.Addr[0], "Z00010000000", 12);
					Remote.Addr[0][12] = '\0';
					DisplayTalkWatchPicWindow(MAINRETURNTYPE, TALKWINTYPE);
					CallCenter_Func("Z00010000000");
				}
			}
			break;
#ifdef SMART_BELL		   
		case 6:
			//PlaySoundDoorBell();
			break;
#endif
		default:
			break;
		}
#endif
	}
}
//---------------------------------------------------------------------------
INT8U Read_X(void)
{
	INT8U reg = 0;
	INT16U touch_x;

	reg = 0xC0;
	Write_i2c(TOUCH_WRITE_ADDR, reg);
	touch_x = Read_i2c(TOUCH_READ_ADDR);
	touch_x = touch_x>>4;
	if(touch_x == 0)
		return 0;
	else
		return 1;
}
//---------------------------------------------------------------------------
INT8U Touch_Read_XY(void) //触摸屏I2C读取函数
{
	INT8U i, j;
	INT8U tmpvalue;
	INT8U reg = 0;
	INT16U touch_x[10];
	INT16U touch_y[10];
	INT8U tmp_x;
	INT16U tmp_value[10];
	INT8U result_i;
	INT16U result_value[10];
	INT16U tmptouch;
	INT8U dec_max;
	INT8U dec_tmp_x[2];
	INT16U touch_cal_x;
	INT16U touch_cal_y;
	INT8U touch_ok = 0;
	INT8U key_count;
	//xtm_printf("11inportb(GPIOA_OEN_DATA) = 0x%X,\n", inportb(GPIOA_OEN_DATA));
	//xtm_printf("inportb(GPIOA_OUT_DATA) = 0x%X,\n", inportb(GPIOA_OUT_DATA));
	key_count = 0;
	for(i=0; i<10; i++)
	{
		reg = 0xC0;
		Write_i2c(TOUCH_WRITE_ADDR, reg);
		touch_x[i] = Read_i2c(TOUCH_READ_ADDR);
		touch_x[i] = touch_x[i]>>4;

		reg = 0xD0;
		Write_i2c(TOUCH_WRITE_ADDR, reg);
		touch_y[i] = Read_i2c(TOUCH_READ_ADDR);
		touch_y[i] = touch_y[i]>>4;
		if(touch_x[i] == 0x00)
			key_count ++;
		//xtm_printf("touch_x[%d] = 0x%X, touch_y[%d] = 0x%X\n", i, touch_x[i], i, touch_y[i]);
	}

	tmpvalue = inportb(GPIOA_IN_DATA);
	if((tmpvalue & TOUCH_IO))
	{
		xtm_printf("read touch but IO is low\n");
		return 1;
	}

	//  if(key_count >= 8)
	//   return 0;
	//for(i=0; i<10; i++)
	//  xtm_printf("touch_x[%d] = 0x%X, touch_y[%d] = 0x%X\n", i, touch_x[i], i, touch_y[i]);

	//排序 X
	dec_max = 10;
	result_i = 0;
	for(j=0; j<10; j++)
	{
		tmptouch = touch_x[0];
		dec_tmp_x[0] = 0;
		for(i=1; i<dec_max; i++)
			if(touch_x[i] < tmptouch)
			{
				tmptouch = touch_x[i];
				dec_tmp_x[0] = i;
			}
			tmp_x = 0;
			for(i=0; i<dec_max; i++)
				if((i != dec_tmp_x[0]))
				{
					tmp_value[tmp_x] = touch_x[i];
					tmp_x ++;
				}
				result_value[result_i] = touch_x[dec_tmp_x[0]];
				result_i ++;
				dec_max --;
				for(i=0; i<dec_max; i++)
					touch_x[i] = tmp_value[i];
	}

	tmptouch = 0;
	for(i=4; i<7; i++)
		tmptouch += result_value[i];
	touch_cal_x = tmptouch/3;

	//  for(i=0; i<10; i++)
	//    printf("result_value[%d] = 0x%X\n", i, result_value[i]);

	//排序 Y
	dec_max = 10;
	result_i = 0;
	for(j=0; j<10; j++)
	{
		tmptouch = touch_y[0];
		dec_tmp_x[0] = 0;
		for(i=1; i<dec_max; i++)
			if(touch_y[i] < tmptouch)
			{
				tmptouch = touch_y[i];
				dec_tmp_x[0] = i;
			}
			tmp_x = 0;
			for(i=0; i<dec_max; i++)
				if((i != dec_tmp_x[0]))
				{
					tmp_value[tmp_x] = touch_y[i];
					tmp_x ++;
				}
				result_value[result_i] = touch_y[dec_tmp_x[0]];
				result_i ++;
				dec_max --;
				for(i=0; i<dec_max; i++)
					touch_y[i] = tmp_value[i];
	}

	tmptouch = 0;
	for(i=4; i<7; i++)
		tmptouch += result_value[i];
	touch_cal_y = tmptouch/3;
	//  for(i=0; i<10; i++)
	//    printf("result_value[%d] = 0x%X\n", i, result_value[i]);

	if((touch_cal_x != 0x0)&&(touch_cal_y != 0xFFF0))
		touch_ok = 1;
	else
		xtm_printf("read i2c error %d\n", i);

	//outportb(GPIOA_OEN_DATA, inportb(GPIOA_OEN_DATA) & 0x3F);

	if(touch_ok == 1)
		TouchScr_Func(touch_cal_x, touch_cal_y);
	return touch_ok;
}
//--------------------------------------------------------------------------
void ClearWatchDog(void)
{
	INT8U tmp_data;
	if(Local.GpioWatchDogStarted)  //在GPIO检测线程中启动WatchDog
	{
		outportb(GPIOA_OEN_DATA, inportb(GPIOA_OEN_DATA) & 0xfb);       //gpioa2为输出
		tmp_data = inportb(GPIOA_OUT_DATA);
		if(tmp_data & 0x04)
			outportb(GPIOA_OUT_DATA, tmp_data & 0xfb);
		else
			outportb(GPIOA_OUT_DATA, tmp_data | 0x04);
	}
}
//--------------------------------------------------------------------------
