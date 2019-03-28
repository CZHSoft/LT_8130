#include "includes.h"

#include "stdlib.h"

#define x_gpio_h

#define CommonH
#include "common.h"                      

#define TOUCH_IO     0x10              //触摸屏
#define WATCHDOG_IO     0x04              //看门狗

#define IO_S1     0x01    //GPIOD0
#define IO_S2     0x02    //GPIOD1
#define IO_S3     0x04    //GPIOD2
#define IO_S4     0x08    //GPIOD3
#define IO_S5     0x10    //GPIOD4

struct gpio_key1         //触摸屏检测
{
	INT8U curr_input; //本次状态
	INT8U downnum;  //检测到有输入次数
	INT8U press_flag;
};
struct gpio_key1 gpio_key[6];
void get_key_value(INT8U gpiovalue);

#ifdef _NIGHT_CAMERA_ADJUST_SUPPORT   //光暗CAMERA自动调节支持
void Check_Night_IO(void);
#endif

INT16U openlockpressnum = 0;      //开锁键长按计数
INT16U touchpressnum = 0;      //触摸屏长按计数
struct touch_io1         //触摸屏检测
{
	INT8U curr_input; //本次状态
	INT8U downnum;  //检测到有输入次数
	INT8U press_flag;
};
struct touch_io1 touch_io;

INT16U timenum = 0;
void InitGpio(void);  //初始化GPIO
void CheckGpio(void);    //检测GPIO函数
void SetGpio(INT16U IO_Register, INT8U userIO, INT8U SetFlag);  //设置GPIO

void InitKeyDelay(void);
void CheckKeyDelay(INT16U timenum);

INT8U Touch_Read_XY(void); //触摸屏I2C读取函数

void ClearWatchDog(void);
void OpenDoorFunc(void);  //开锁函数

INT8U DoorDectWorked = 0; //电平低，门磁工作
INT16U DoorDectTime = 0;  //门磁检测时间
INT8U DoorDectAlarmFlag = 0; //门磁报警标志
INT16U DoorDectAlarmTipTime = 0;  //门磁报警提示时间
void SendAlarmFunc(INT8U SpecialByte, INT8U AlarmByte[2]); //防区报警函数
//---------------------------------------------------------------------------
void InitGpio(void)  //初始化GPIO
{
	INT8U i;
	xtm_printf("InitGpio...\n");
	for(i=0; i<6; i++)
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
#ifdef _CAMERA_SUPPORT     //如用摄像头，GPIOB6、7为Comm1, GPIOC 作为8个防区使用
	//如用于 OV7725 注释下面行
	outportb(GPIOB_SEL, inportb(GPIOB_SEL) & 0x3F);   //GPIOB6、GPIOB7 串口1
#else
	//如用于 OV7725 注释下面行
	outportb(GPIOC_SEL, inportb(GPIOC_SEL) | 0xFF);
	//outportb(GPIOC_OEN_DATA, inportb(GPIOC_OEN_DATA) & 0x00);    //输出
	outportb(GPIOC_OEN_DATA, inportb(GPIOC_OEN_DATA) | 0xFF);    //输入
#endif

#if 1 //FPGA 不要操作GPIOD
	//GPIOD
	outportb(GPIOD_SEL, inportb(GPIOD_SEL) | 0xFF);
	//0-3 输入   4 输出
#ifdef _NIGHT_CAMERA_ADJUST_SUPPORT   //光暗CAMERA自动调节支持
	outportb(GPIOD_OEN_DATA, inportb(GPIOD_OEN_DATA) | 0x1F);
#else
	outportb(GPIOD_OEN_DATA, inportb(GPIOD_OEN_DATA) | 0x0F); //0x0f
#endif
    //测试用
    //outportb(GPIOD_OEN_DATA, inportb(GPIOD_OEN_DATA) | 0x00);
	//5-7 输出  LED1、2、3   CAMERA、布防、网络
	outportb(GPIOD_OEN_DATA, inportb(GPIOD_OEN_DATA) & 0x1F);
	SetGpio(GPIOD_OUT_DATA, IO5_HIGH, SET_HIGH); //IO5 高
	InitKeyDelay();
	//xtm_printf("1 inportb(GPIOA_OEN_DATA) = 0x%X\n", inportb(GPIOA_OEN_DATA));
	//xtm_printf("2 inportb(GPIOD_OEN_DATA) = 0x%X\n", inportb(GPIOD_OEN_DATA));
	//xtm_printf("3 inportb(GPIOD_IN_DATA) = 0x%X\n", inportb(GPIOD_IN_DATA));

	//SetGpio(GPIOD_OUT_DATA, CAMERA_POWER_HIGH, SET_HIGH); //开CAMERA
	//SetGpio(GPIOD_OUT_DATA, CAMERA_POWER_LOW, SET_LOW); //关CAMERA
	//SetGpio(GPIOD_OUT_DATA, NET_LED_HIGH, SET_HIGH); //关网络LED

	SetGpio(GPIOD_OUT_DATA, COMM_SEC_HIGH, SET_HIGH); //1 -- 232   0 -- 485
	SetGpio(GPIOD_OUT_DATA, SEND_485_LOW, SET_LOW);   //1 -- 485 send enable   0 -- 485 send disable
	//SetGpio(GPIOD_OUT_DATA, SEND_485_HIGH, SET_HIGH);
#endif

	SetGpio(GPIOA_OUT_DATA, OPEN_LOCK_LOW, SET_LOW); //关门锁
	//SetGpio(GPIOA_OUT_DATA, OPEN_LOCK__HIGH, SET_HIGH); //开门锁

	init_w7514(TOUCH_CHIP_ADDR_WR, 29);  //初始化W7514   灵敏度: 0  --- 31

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

	CheckEn_J_Status();

	ClearWatchDog();

	//开锁
	if(Local.OpenDoorFlag > 0)
	{
		OpenDoorFunc();
	}

	tmpvalue = 0;
	gpio_value = 0;

	CheckKeyDelay(timenum);
	timenum ++;

#ifdef _NIGHT_CAMERA_ADJUST_SUPPORT   //光暗CAMERA自动调节支持
	if(Local.Status != 0)
		if((timenum % 20)==0)   //400ms 检测一次
			Check_Night_IO();//BUG驻留?
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
		}

		//触摸屏检测
		if(touch_io.curr_input == 0) //本次状态
		{
			touch_io.downnum = 0;  //检测到有输入次数
			touch_io.press_flag = 0;
		} 
		else
		{
			//xtm_printf("tmpvalue = 0x%X\n", tmpvalue);
			touch_io.downnum ++;  //检测到有输入次数
			if(touch_io.press_flag == 0)
				if(touch_io.downnum >= 5)
				{
					touch_io.press_flag = 1;
					xtm_printf("tmpvalue = 0x%X\n", tmpvalue);
					//xtm_printf("CONTROL_REG 11= 0x%X\n", inportb(CONTROL_REG));
					touch_io.downnum = 0;

					Touch_Read_XY();
					return;
				}
		}

#if 1 //FPGA 不要操作GPIOD
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

					if(LocalCfg.DoorDetect)
					{
						if(!(tmpvalue & IO_S4) )
						{
							DoorDectWorked = 1; //电平低，门磁工作
							DoorDectTime = 0;
							DoorDectAlarmFlag = 0;
							DoorDectAlarmTipTime = 0;
						}
						else
						{
							DoorDectTime ++;
							if(DoorDectWorked) //电平低，门磁工作
								if(!DoorDectAlarmFlag)
								{
									if(DoorDectTime > GPIOPERSEC*20)
									{
										DoorDectAlarmFlag = 1;
										DoorDectAlarmTipTime = 0;  //门磁报警提示时间
										xtm_printf("门磁报警\n");
										SendAlarmFunc(0x01, Local.AlarmByte);
										WaitAudioUnuse(200); //等待声卡空闲
										StartPlayWav(doordect_wav.Faddr, NO_CYCLE_PLAY);
									}
								}
						}

						if(DoorDectAlarmFlag)
						{
							DoorDectAlarmTipTime ++;
							if(DoorDectAlarmTipTime > GPIOPERSEC*60)
							{
								xtm_printf("门磁报警\n");
								DoorDectAlarmTipTime = 0;
								WaitAudioUnuse(200); //等待声卡空闲
								StartPlayWav(doordect_wav.Faddr, NO_CYCLE_PLAY);
							}
						}
					}

					//IO检测
					for(i=0; i<3; i++)
					{
						if(gpio_key[i].curr_input == 0) //本次状态
						{
							if(gpio_key[i].press_flag == 0)
							{
								if(gpio_key[i].downnum >= 5)
								{
									gpio_key[i].press_flag = 1;

									gpio_key[i].downnum = 0;
									xtm_printf("Key: %d\n", i);
									get_key_value(i+13);
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
						}
					}
#endif
#endif
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
	
	Local.KeyPressTime = 0; 
	Local.LcdLightTime = 0;
	
	if(Local.Status == 0)
	{
		PlaySoundTip(); //播放提示音
	}

	if(Local.LcdLightFlag == 0)
	{
		//点亮LCD背光
		Lcd_Power(1, LocalCfg.BrightVolume);
		Local.LcdLightFlag = 1; //LCD背光标志
		Local.LcdLightTime = 0; //时间
	}
	
	//    else      20130413 待机时调暗背光
	{
#if 1
		switch(gpiovalue)
		{
		case 1:  //1
		case 2:  //2
		case 3:  //3
		case 4:  //4
		case 5:  //5
		case 6:  //6
		case 7:  //7
		case 8:  //8
		case 9:  //9
		case 10:  //*
		case 11:  //0
		case 12:  //#
			if(gpiovalue == 11)
				gpiovalue = 0;
			if(gpiovalue == 12)
				gpiovalue = 11;
			xtm_printf("Local.CurrentWindow = %d, gpiovalue = %d\n",Local.CurrentWindow, gpiovalue);
			if((Local.CurrentWindow >= 0) &&(Local.CurrentWindow <=300))
				switch(Local.CurrentWindow)
			{
				case 0:  //对讲界面操作（一级）
					OperateTalkWindow(gpiovalue, Local.CurrentWindow);
					break;
				case 2:  //对讲 正在连接窗口操作
					OperateTalkConnectWindow(gpiovalue, Local.CurrentWindow);
					break;
				case 3:  //对讲 通话窗口操作
					OperateTalkStartWindow(gpiovalue, Local.CurrentWindow);
					break;
				case 4:  //监视界面操作(二级）
					OperateWatchWindow(gpiovalue, Local.CurrentWindow);
					break;
				case 5:  //密码开锁界面操作（一级）
					OperateOpenLockWindow(gpiovalue, Local.CurrentWindow);
					break;
				case 7: //密码开锁窗口成功操作
					OperateOpenLockResultWindow(gpiovalue, Local.CurrentWindow);
					break;
				case 8: //呼叫失败操作
					OperateCallFailWindow(gpiovalue, Local.CurrentWindow);
					break;
				case 6:  //进入设置窗口操作（一级）
					OperateSetupWindow(gpiovalue, Local.CurrentWindow);
					break;
#ifdef _DOWNLOAD_PHOTO  //下载图片并播放支持
				case 53:  //相片播放窗口操作（一级）
					OperatePlayPhotoWindow(gpiovalue, Local.CurrentWindow);
					break;
#endif
				case 60:  ////设置主界面操作（二级）
					OperateSetupMainWindow(gpiovalue, Local.CurrentWindow);
					break;
				case 61:  //地址设置界面
					OperateAddressWindow(gpiovalue, Local.CurrentWindow);
					break;
				case 62:  //修改参数设置操作
					OperateParamWindow(gpiovalue, Local.CurrentWindow);
					break;
				case 63:  //密码设置界面
					OperatePassWindow(gpiovalue, Local.CurrentWindow);
					break;
				case 64:  //其它设置操作
					OperateOtherWindow(gpiovalue, Local.CurrentWindow);
					break;
				case 65:  //在线检测操作
					OperateOnlineTestWindow(gpiovalue, Local.CurrentWindow);
					break;
				case 66:  //查询本机信息操作
					OperateSetupInfoWindow(gpiovalue, Local.CurrentWindow);
					break;
			}
			break;
		case 13:
			break;
		case 14:
			WaitAudioUnuse(200); //等待声卡空闲               //  wav_1k[5].Faddr
			StartPlayWav(test1k_wav.Faddr, NO_CYCLE_PLAY);       // soundtip_wav.Faddr        
			break;
		case 15: //出门按钮
			if(LocalCfg.DoorButton)
			{
				PlayDoorOpenWav();
				CheckDelayOpenLock_Func();//检查开锁延时
				xtm_printf("出门按钮开锁\n");
			} 
			break;
		}
#endif
	}
}
//---------------------------------------------------------------------------
INT8U Touch_Read_XY(void) //触摸屏I2C读取函数
{
	INT8U tmpvalue, keyvalue;
	INT8U ReadBuff[2];
	tmpvalue = Read_i2c(TOUCH_CHIP_ADDR_RE, ReadBuff, 2);            //         TOUCH_CHIP_ADDR_RE
	xtm_printf("Touch_Read_XY::tmpvalue = 0x%X, ReadBuff[0] = 0x%X, ReadBuff[1] = 0x%X\n", tmpvalue, ReadBuff[0], ReadBuff[1]);

	keyvalue = 0;
	if(ReadBuff[0] & 0x20)
	{   //SW13 上有触摸动作
		xtm_printf("SW13\n");
	}
	if(ReadBuff[0] & 0x10)
	{
		xtm_printf("SW12\n");

	}
	if(ReadBuff[0] & 0x08)
	{
		xtm_printf("SW11\n");
		keyvalue = 10;
	}
	if(ReadBuff[0] & 0x04)
	{
		xtm_printf("SW10\n");
		keyvalue = 7;
	}
	if(ReadBuff[0] & 0x02)
	{
		xtm_printf("SW9\n");
		keyvalue = 4;
	}
	if(ReadBuff[0] & 0x01)
	{
		xtm_printf("SW8\n");
		keyvalue = 1;
	}

	if(ReadBuff[1] & 0x80)
	{
		xtm_printf("SW7\n");
		keyvalue = 11;
	}
	if(ReadBuff[1] & 0x40)
	{
		xtm_printf("SW6\n");
		keyvalue = 8;
	}
	if(ReadBuff[1] & 0x20)
	{
		xtm_printf("SW5\n");
		keyvalue = 5;
	}
	if(ReadBuff[1] & 0x10)
	{
		xtm_printf("SW4\n");
		keyvalue = 2;
	}
	if(ReadBuff[1] & 0x08)
	{
		xtm_printf("SW3\n");
		keyvalue = 12;
	}
	if(ReadBuff[1] & 0x04 )
	{
		xtm_printf("SW2\n");
		keyvalue = 9;
	}
	if(ReadBuff[1] & 0x02)
	{
		xtm_printf("SW1\n");
		keyvalue = 6;
	}
	if(ReadBuff[1] & 0x01)
	{
		xtm_printf("SW0\n");
		keyvalue = 3;
	}
	if(keyvalue != 0)
		get_key_value(keyvalue);
	return 1;
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
void OpenDoorFunc(void)  //开锁函数
{
	INT16U OpenLockTime[3];// = {500, 3000, 5000};
	INT16U DelayOpenLockTime[4];// = {0, 3000, 5000, 10000};

	OpenLockTime[0] = 500;
	OpenLockTime[1] = 3000;
	OpenLockTime[2] = 5000;

	DelayOpenLockTime[0] = 0;
	DelayOpenLockTime[1] = 3000;                
	DelayOpenLockTime[2] = 5000;
	DelayOpenLockTime[3] = 10000;

	Local.OpenDoorTime += 20;
	//xtm_printf("Local.OpenDoorTime = %d\n", Local.OpenDoorTime);
	if(LocalCfg.OpenLockTime >= 3)
	{
		xtm_printf("LocalCfg.OpenLockTime = %d,....\n", LocalCfg.OpenLockTime);
		LocalCfg.OpenLockTime = 0;
	}
	if(LocalCfg.DelayLockTime >= 4)
	{
		xtm_printf("LocalCfg.DelayLockTime = %d,....\n", LocalCfg.DelayLockTime);
		LocalCfg.DelayLockTime = 0;
	}
	switch(Local.OpenDoorFlag)
	{
	case 2:  // 开锁中
		if(Local.OpenDoorTime > OpenLockTime[LocalCfg.OpenLockTime])
		{
			Local.OpenDoorFlag = 0;   //开锁标志 0 未开锁  1 开锁延时中  2 开锁中
			Local.OpenDoorTime = 0;   //时间
			//关闭门锁  
			SetGpio(GPIOA_OUT_DATA, OPEN_LOCK_LOW, SET_LOW); //关门锁
			xtm_printf("开锁完成\n");
		}
		break;
	case 3:  // 延时开锁中
		if(Local.OpenDoorTime > DelayOpenLockTime[LocalCfg.DelayLockTime])
		{
			OpenLock_Func();
		}
		break;
	}       
}
//---------------------------------------------------------------------------
void SendAlarmFunc(INT8U SpecialByte, INT8U AlarmByte[2]) //防区报警函数
{
	INT16U sendlength;
	INT8U sendb[128];

	//头部
	xtm_memcopy(sendb, UdpPackageHead, 6);
	//命令
	sendb[6] = ALARM;

	sendb[7] = ASK;        //主叫
	//子命令
	xtm_memcopy(sendb+8, LocalCfg.Addr, 20);
	xtm_memcopy(sendb+28, LocalCfg.Mac_Addr, 6);

	sendb[34] = 0;
	sendb[35] = 1;

	sendb[34] = 1;                //布防状态
	sendb[35] = SpecialByte;      //特殊报警
	sendb[36] = 1;                //防区模块个数
	sendb[37] = 0;
	sendb[38] = 0;
	sendb[39] = 0x0;
	sendb[40] = 0x0;

	sendlength = 41;
	CreateMultiUdpBuff(LocalPort[0], MULTI_SEND, LocalCfg.IP_Server, sendb, sendlength, 0);
}
//---------------------------------------------------------------------------
#ifdef _NIGHT_CAMERA_ADJUST_SUPPORT   //光暗CAMERA自动调节支持
void Check_Night_IO(void)
{
	INT8U tmpvalue, tmp_status;
	tmpvalue = inportb(GPIOD_IN_DATA);
	if(!(tmpvalue & IO_S5))
		tmp_status = 0;
	else
		tmp_status = 1;

	if(Local.Night_IO != tmp_status)
	{
		Local.Night_IO = tmp_status;
		if(Local.Night_IO == 0)
		{
			Write7725(0x14, 0x40);
			Write7725(0x9B, 0x48);
			Write7725(0xA6, 0x26);
		}
		else
		{
			Write7725(0x14, 0x10);
			Write7725(0x9b, 0x08);
			Write7725(0xa6, 0x04);
		}
		xtm_printf("Check_Night_IO::Local.Night_IO = %d\n", Local.Night_IO);
	}
}
#endif
//---------------------------------------------------------------------------
