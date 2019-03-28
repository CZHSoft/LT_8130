#include "includes.h"
#include "ComDrv.h"

#include "stdlib.h"

#define x_comm_h

#define CommonH
#include "common.h"

/*
****************************************
201309
新增加splash窗体的预处理
201310
新增加对键盘窗体的预处理
****************************************
*/

#define _COMM_OPERATE   0

void dcomm1(void);
void dcomm2(void);
void CommSendBuff(INT8U comm_fd, INT8U *comm_buf, INT16U send_len);

void KeyTouchOperate(INT8U TouchNum); //按键响应
void KeyPreDeal(void); //按键预处理，按下效果
void KeyBackDeal(void); //按键后处理，弹起效果 并处理

INT8U CheckKeyPressAndLcd(void); //检查按键并点亮LCD
//---------------------------------------------------------------------------
void CommSendBuff(INT8U comm_fd, INT8U *comm_buf, INT16U send_len)
{
  INT16U i;
  for (i=0; i<send_len; i++)
   {
    if(comm_fd == 0)
     {
      while((inportb(UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
      outportb(UART0_TX_DATA, comm_buf[i]);				// 发送字符
     }
    if(comm_fd == 1)
     {
      while((inportb(UART1_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
      outportb(UART1_TX_DATA, comm_buf[i]);				// 发送字符
     }
   }
}
//---------------------------------------------------------------------------
INT8U ErrorStopDecode = 0;
void dcomm1(void)
{
	INT32U padd, padd1;
	INT16U i, j;
	INT8U k, gpio_data;
        INT8U DestIP[4];
        INT16U val;
        INT8U buf[512];
        struct file_head1 file_head;
        INT8U flash_id[8];
	while (comm1.RP!=comm1.WP){
       #ifdef _XTM_ADD
        xtm_printf("dcomm1::comm1.RP = %d, comm1.WP = %d, comm1.Rbuf[comm1.RP] = 0x%X\n", comm1.RP, comm1.WP, comm1.Rbuf[comm1.RP]);
       #endif
       switch(comm1.Rbuf[comm1.RP])
        {
#if _COMM_OPERATE
         case 'A':
                  xtm_printf("dcomm1::Recv = 0x%X\n", comm1.Rbuf[comm1.RP]);
                  Lcd_Power(0, LocalCfg.BrightVolume);
                  break;
         case 'B':
                  xtm_printf("dcomm1::Recv = 0x%X\n", comm1.Rbuf[comm1.RP]);
                  //Lcd_Power(1, LocalCfg.BrightVolume);
                  InitVideo();
                  for(i=0; i<100; i++)
                   {
                    LocalCfg.EncodeFrameSize = VIDEO_VGA; //编码大小

                    //开始录制视频
                    xtm_printf("1  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                    StartVideoClock();     //打开视频采集编码时钟
                    xtm_printf("2  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                    StartRecVideo(LocalCfg.EncodeFrameSize);
                    xtm_printf("3  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                    StartEncodeVideo(LocalCfg.EncodeFrameSize);     //打开视频编码
                    xtm_printf("4  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                    ClearWatchDog();
                    DelayMs(100);
                    StopEncodeVideo();
                    StopRecVideo();
                    StopVideoClock();            //关闭视频采集编码时钟
                    xtm_printf("5  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                   }                  
                  break;
         case 'C':
                  xtm_printf("C\n");
                  //xtm_printf("dcomm1::Recv1 = 0x%X\n", init_camera());
                  //DisplayFirstWindow();
                    DisplayImage(&main_image, FROMFILE, NOFULLPAGE);       // FROMFILE
                    DisplayImage(&logo_image, FROMFILE, NOFULLPAGE);

                    for(i=0; i<5; i++)
                     {
                      DisplayImageButton(&menu_button[i], IMAGEUP, FROMFILE, NOFULLPAGE);
                     }
                  break;
         case 'P':
                  xtm_printf("P\n");

                  ErrorStopDecode = 0;
                  for(i=0; i<1000; i++)
                   {
                    DisplayFirstWindow();
                   // DisplayRecordsWindow(REFRESH_MIDDLE | REFRESH_BOTTOM, 0);
                    if(ErrorStopDecode)
                     {
                      xtm_printf("ErrorStopDecode 1\n");
                      ErrorStopDecode = 0;
                      //break;
                     }
                    xtm_printf("Local.fb_page = %d, i = %d\n", Local.fb_page, i);
                   }      

                  //xtm_clear_disp_ram(COLOR_RED);
                  //selectym(0);
                  //Touch_Read_XY();
                  /*xtm_printf("PreDealImage1111  111\n");
                  for(i=0; i<50000; i++)
                   {
                    PreDealImage1111(&main480_image);
                    ClearWatchDog();
                   }
                  xtm_printf("PreDealImage1111  222\n");*/
//                  PrintArp_Table();
                  break;
         case 'D':
                  xtm_printf("D\n");
                  //outportb(SPER, 0x1);
                  xtm_printf("SPER = 0x%X\n", inportb(SPER));
//亮度 -
         /*  if(LocalCfg.BrightVolume > 0)
            {
             LocalCfg.BrightVolume --;
             Lcd_Power(1, LocalCfg.BrightVolume);
            }
                      */

                  break;
         case 'E':
                  xtm_printf("E\n");
 //亮度 +
           if(LocalCfg.BrightVolume < 5)
            {
             LocalCfg.BrightVolume ++;
             Lcd_Power(1, LocalCfg.BrightVolume);
            }                  
                  break;
         case 'F':
                  xtm_printf("F\n");
                  LocalCfg.SpkVolume = 5;
                  SoundSetSpeakerVolume(LocalCfg.SpkVolume);
                  //StartPlayWav(soundtip_wav.Faddr, CYCLE_PLAY);
				  StartPlayWav(soundtip2_wav.Faddr, CYCLE_PLAY);
                  break;
         case 'G':
                  read25x32_unique_id(flash_id);
                  xtm_printf("flash_id = 0x%X:0x%X:0x%X:0x%X:0x%X:0x%X:0x%X:0x%X\n", flash_id[0], flash_id[1], flash_id[2], flash_id[3], flash_id[4], flash_id[5], flash_id[6], flash_id[7]);
                  //selectym(1);
                  //StartRecWav();
                  break;
         case 'H':
                  Local.GpioWatchDogStarted = 0;  //在GPIO检测线程中启动WatchDog
                  //selectym(0);
                  //StopRecWav();
                  break;
         case 'I':
                  StopPlayWavFile();
                  break;
         case 'J':
                  xtm_printf("J\n");
                  break;

         case 'K':
                  xtm_printf("K\n");
                  break;
         case 'L':
                  break;
         case 'T':
                  break;
         case 'M':
                  outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
                  for(j=0; j<16; j++)                   {                    outportb(MEMORY_WINDOW_BASE_H, 0x60 + j);                    xtm_printf("%X ::: \n", \                              0x60 + j);                    for (i=0; i<10; i++)                     {                      xtm_printf(" bitstream[%d] = %d, ", \                              i, Memory6_Stream[i]);                     }
                    xtm_printf("\n");
                   }

                  outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);
                    xtm_printf("%X ::: \n", \                              0x70 + 0x0a);                    for (i=0; i<10; i++)                     {                      xtm_printf("Memory6_Stream[%d] = %d,  ", \                              i, Memory6_Stream[i]);                     }
                    xtm_printf("\n");
                  break;
         case 'N':
                  for (i=0;i<10;i++)
                   {
                    Memory6_Stream[i] = i + 200;                   }                  outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);                  for(j=0; j<16; j++)                   {                    outportb(MEMORY_WINDOW_BASE_H, 0x60 + j);                    for (i=0; i<10; i++)                     {                      Memory6_Stream[i] = 10+ j*10 + i;                     }
                   }

            /*      DisplayImage(&logo_image, FROMFILE);

                  for(i=0; i<5; i++)
                    DisplayImageButton(&menu_button[i], IMAGEUP, FROMFILE);  */

                  break;
         case 'O':
                  DisplayCalibrateWindow();
                  break;
         case 'R':
                  xtm_printf("Read7725(0x0a) = 0x%X\n", Read7725(0x0a));
                  //AddMultiGroup(Local.NsMultiAddr);
                  //xtm_printf("AddMultiGroup\n");

                 //TestClientTcp();
                 break;
         case 'S':
                  xtm_printf("S\n");
                  break;
         case 'U':
                  xtm_printf("U\n");
         case 'W':
                  //CloseClientTcp();
                  StartVideoClock();     //打开视频采集编码时钟
                  StartRecVideo(Local.Test_FrameSize);
                  if(Local.Test_FrameSize == VIDEO_QVGA)
                    DisplayVideoWindow(160, 320, 120, 240, LOCAL_VIDEO);
                  if(Local.Test_FrameSize == VIDEO_VGA)
                    DisplayVideoWindow(0, 640, 0, 480, LOCAL_VIDEO);
                  break;
         case 'Y':
                  xtm_printf("start video1\n");
                  xtm_printf("dcomm1 1::EN_J_CONTROL = 0x%X\n", inportb(EN_J_CONTROL));
                  Local.Test_EncodeWatchFlag = 1;
                  Set_Decode_Clock(1);  //操作视频解码时钟  1  开   0  关
                  //outportb(TX_START, inportb(TX_START)|0x20);//使能MJPEG中断
              //	outportb(EN_J_CONTROL2, 0x4f);//原图像30帧每秒，编码15帧每秒
                  StartVideoClock();     //打开视频采集编码时钟
                  StartRecVideo(Local.Test_FrameSize);

                  Local.DecodeVideoWidth = 640;
                  Local.DecodeVideoHeight = 480;
              	  StartEncodeVideo(VIDEO_VGA);     //打开视频编码

                  xtm_printf("dcomm1 2::EN_J_CONTROL = 0x%X\n", inportb(EN_J_CONTROL));

                  DisplayVideoWindow(0, 640, 0, 480, REMOTE_VIDEO);//REMOTE_VIDEO);   LOCAL_VIDEO
                  break;
         case 'Z':
                  xtm_printf("stop video1\n");
                  xtm_printf("dcomm1 3::EN_J_CONTROL = 0x%X\n", inportb(EN_J_CONTROL));
                  //outportb(TX_START, inportb(TX_START)&0xdf);//使能MJPEG中断

                  StopEncodeVideo();
                  StopRecVideo();

                  xtm_printf("dcomm1 4::EN_J_CONTROL = 0x%X\n", inportb(EN_J_CONTROL));
                  //outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xfe);
                  CloseVideoWindow();
                  StopRecVideo();
                  Local.Test_EncodeWatchFlag = 0;
                  break;
         case 'a':  //0
         case 'b':  //1
         case 'c':  //2
         case 'd':  //3
         case 'e':  //4
         case 'f':  //5
         case 'g':  //6
         case 'h':  //7
         case 'i':  //8
         case 'j':  //9
         case 'k':  //10
         case 'l':  //11
         case 'm':  //12
         case 'n':  //13
         case 'o':  //14
                  xtm_printf("dcomm1::read25x32_status = 0x%X\n", read25x32_status());
                  KeyTouchOperate(comm1.Rbuf[comm1.RP] - 'a');
                  xtm_printf("dcomm1:: Local.KeyTouchNo = %d, comm1.Rbuf[comm1.RP] = 0x%X\n", Local.KeyTouchNo, comm1.Rbuf[comm1.RP]);
                  break;
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
                  KeyTouchOperate(100 + comm1.Rbuf[comm1.RP] - '0');
                  break;

         case '7':
                  break;
         case '8':
                  break;
         case '9':
                  break;
#endif

         }
    comm1.RP = comm1.WP;
 }
}
//---------------------------------------------------------------------------
void KeyTouchOperate(INT8U TouchNum) //按键响应
{
  if(CheckKeyPressAndLcd()) //检查按键并点亮LCD
    return;
  if(Local.KeyTouchNo == 0)
   {
    if(Local.Status == 0)
      PlaySoundTip(); //播放提示音
    Local.KeyTouchNo = TouchNum + 1; //按钮按下标志
    KeyPreDeal(); //按键预处理，按下效果
    dtime[3] = KEYDELAYTIME;
   }
}
//---------------------------------------------------------------------------
void KeyPreDeal(void) //按键预处理，按下效果
{
  xtm_printf("KeyPreDeal::Local.CurrentWindow = %d\n", Local.CurrentWindow);
  switch(Local.CurrentWindow)
   {
    case 0:     //主窗口预处理，按键按下效果
           PreDealFirstWindow(Local.KeyTouchNo - 1);
           break;
    case 5:    //进入设置窗口预处理，按键按下效果
           PreDealSetupWindow(Local.KeyTouchNo - 1);
           break;
    case 6: //splash窗口
		   //PreDealSplashWindow(Local.KeyTouchNo - 1);
		   break;
    case 7: //20131008窗口
		   PreDealEPWWindow(Local.KeyTouchNo - 1);
		   break;  
    case 12://呼出窗口
           PreDealTalkWindow(Local.KeyTouchNo - 1);
           break;
    case 13://监视窗口
           PreDealWatchWindow(Local.KeyTouchNo - 1);
           break;
    case 16:    //对讲呼入窗口预处理，按键按下效果
           PreDealTalkWatchPicWindow(Local.KeyTouchNo - 1);
           break;
    case 31:    //撤防窗口预处理，按键按下效果
           PreDealCancelFortifyWindow(Local.KeyTouchNo - 1);
           break;
    case 32:    //布防窗口预处理，按键按下效果
           PreDealFortifyWindow(Local.KeyTouchNo - 1);
           break;
    case 35:    //取消报警窗口预处理，按键按下效果
           PreDealCancelAlarmWindow(Local.KeyTouchNo - 1);
           break;
    case 21:    //通话记录窗口预处理，按键按下效果
           PreDealRecordsWindow(Local.KeyTouchNo - 1);
           break;
    case 22:    //查看通话记录窗口预处理，按键按下效果
           PreDealRecordsContentWindow(Local.KeyTouchNo - 1);
           break;
	case 40://201402 NEW 密码输入窗体
		   PreDealLTpwWindow(Local.KeyTouchNo - 1);
           break;
    case 88://20140315 ad window
           PreDealSmarkStreamWindow(Local.KeyTouchNo - 1);
           break;
    case 89://20140315 ad window
           PreDealSmarkStreamWindow(Local.KeyTouchNo - 1);
           break;
    case 111:    //信息窗口预处理，按键按下效果
           //无奈处理 20130922
           PreDealInfoWindow(Local.KeyTouchNo - 1);
           break;
    case 112:    //信息查看窗口预处理，按键按下效果
           //无奈处理 20130922
           PreDealInfoContentWindow(Local.KeyTouchNo - 1);
           break;
    case 152:    //设置主窗口预处理，按键按下效果
           PreDealSetupMainWindow(Local.KeyTouchNo - 1);
           break;
    case 149:    //键盘窗口预处理，按键按下效果
           PreDealKbWindow(Local.KeyTouchNo - 1);
           break;
    case 151:    //Lan设置窗口预处理，按键按下效果
           PreDealLanWindow(Local.KeyTouchNo - 1);
           break;
    case 154:    //房号设置窗口预处理，按键按下效果
           PreDealAddrWindow(Local.KeyTouchNo - 1);
           break;
    case 165:    //屏幕设置窗口预处理，按键按下效果
           PreDealScreenSetupWindow(Local.KeyTouchNo - 1);
           break;
    case 166:    //音量调节窗口预处理，按键按下效果
           PreDealVoiceSetupWindow(Local.KeyTouchNo - 1);
           break;
    case 168:    //修改密码窗口预处理，按键按下效果
           PreDealPassSetupWindow(Local.KeyTouchNo - 1);
           break;
    case 158:    //版本窗口预处理，按键按下效果
           PreDealVersionWindow(Local.KeyTouchNo - 1);
           break;
    case 190:    //校准触摸屏窗口预处理，按键按下效果
           PreDealCalibrateWindow(Local.KeyTouchNo - 1);
           break;
    case 192:    //测试窗口预处理，按键按下效果
           //PreDealTestWindow(Local.KeyTouchNo - 1);
           break;
   }
}
//---------------------------------------------------------------------------
void KeyBackDeal(void) //按键后处理，弹起效果 并处理
{
  switch(Local.CurrentWindow)
   {
    case 0: //主界面
           OperateFirstWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 5: //进入设置
           OperateSetupWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
	case 6:
		   //OperateSplashWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
		   break;
    case 7://20131008 密码输入窗体
		   OperateEPWWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 12://呼出窗口
           OperateTalkWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 13://监视窗口
           OperateWatchWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 16://呼入窗口
           OperateTalkWatchPicWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 17://呼入密码窗口
           break;

    case 31:    //撤防窗口
           OperateCancelFortifyWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 32:    //布防窗口
           OperateFortifyWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 35:    //取消报警窗口
           OperateCancelAlarmWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 21:    //通话记录窗口
           OperateRecordsWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 22:    //查看通话记录窗口
           OperateRecordsContentWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 40://201402 NEW 密码输入窗体
		   OperateLTpwWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 88:
           OperateSmarkStreamWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 89:
           OperateSmarkStreamWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 111:    //信息窗口
           OperateInfoWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 112:    //信息查看窗口
           OperateInfoContentWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 152: //设置主窗口
           OperateSetupMainWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 149: //键盘窗口
           OperateKbWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 151: //LAN设置窗口
           OperateLanWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 154: //房号设置窗口
           OperateAddrWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 165:    //屏幕设置窗口预处理，按键按下效果
           OperateScreenSetupWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 166:    //音量调节窗口预处理，按键按下效果
           OperateVoiceSetupWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 168: //修改密码窗口
           OperatePassSetupWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 158: //版本窗口
           OperateVersionWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 190: //校准触摸屏操作
           OperateCalibrateWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 192: //测试窗口操作
           //OperateTestWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
   }
}
//---------------------------------------------------------------------------
INT8U CheckKeyPressAndLcd(void) //检查按键并点亮LCD
{
  Local.KeyPressTime = 0;
  Local.LcdLightTime = 0;
  if(Local.LcdLightFlag == 0)
   {
    //点亮LCD背光
    Lcd_Power(1, LocalCfg.BrightVolume);
    Local.LcdLightFlag = 1; //LCD背光标志
    Local.LcdLightTime = 0; //时间
    return 1;
   }
  return 0;
}
//---------------------------------------------------------------------------
void dcomm2(void)
{
      INT16U comlen;
      INT8U buf[512];
      comlen = 0;
      while (comm2.RP!=comm2.WP)
       {
        #ifdef _XTM_ADD
         //xtm_printf("dcomm2::comm2.RP = %d, comm2.WP = %d, comm2.Rbuf[comm2.RP] = 0x%X\n", comm2.RP, comm2.WP, comm2.Rbuf[comm2.RP]);
        #endif
        buf[comlen] = comm2.Rbuf[comm2.RP];
        comm2.RP ++;
        if(comm2.RP >= COMBUFFLEN)
          comm2.RP = 0;
        comlen ++;
       }
      if(comlen > 0)
       {
        buf[comlen] = '\0';
        xtm_printf("dcomm2::len = %d, buf = %s, comm2.RP = %d, comm2.WP = %d\n", comlen, buf, comm2.RP, comm2.WP);
       }
}
//---------------------------------------------------------------------------
