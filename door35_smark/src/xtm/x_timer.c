#include "includes.h"

#include "stdlib.h"

#define x_timer_h

#define CommonH
#include "common.h"

INT16U Timer_Num = 0;  //计数
void timer_thread_func(void);

void CheckBrightVolume(void); //检查亮度音量等设置值
#ifdef _SEND_TO_SELF_TEST    //发送数据包给自己
void SendToSelf(void);  //自己给自己发送
#endif

void OnlineCheckFunc(void); //在线确认检测函数
void TimeReportStatusFunc(void); //设备定时报告状态函数
void ClearTipFunc(void);    //清状态提示信息函数
void TalkCtrlFunc(void);  //对讲控制，显示通话时间和判断超时

//检测网络信号
void CheckEth(void);

#ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
INT8U head_640x480[0x26f] = {
0xff, 0xd8, 0xff, 0xe0, 0x00, 0x10, 0x4a, 0x46, 0x49, 0x46, 0x00, 0x01, 0x01, 0x02, 0x00, 0x76, 
0x00, 0x76, 0x00, 0x00, 0xff, 0xdb, 0x00, 0x43, 0x00, 0x0d, 0x09, 0x0a, 0x0b, 0x0a, 0x08, 0x0d, 0x0b, 0x0a, 0x0b, 0x0e, 0x0e, 0x0d, 0x0f, 0x13, 0x20, 0x15, 0x13, 0x12, 0x12, 0x13, 0x27, 0x1c, 0x1e, 0x17, 0x20, 0x2e, 0x29, 0x31, 0x30, 0x2e, 0x29, 0x2d, 0x2c, 0x33, 0x3a, 0x4a, 0x3e, 0x33, 0x36, 0x46, 0x37, 0x2c, 0x2d, 0x40, 0x57, 0x41, 0x46, 0x4c, 0x4e, 0x52, 0x53, 0x52, 0x32, 0x3e, 0x5a, 0x61, 0x5a, 0x50, 0x60, 0x4a, 0x51, 0x52, 0x4f, 0xff, 0xdb, 0x00, 0x43, 0x01, 0x0e, 0x0e, 0x0e, 0x13, 0x11, 0x13, 0x26, 0x15, 0x15, 0x26, 0x4f, 0x35, 0x2d, 0x35, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0xff, 0xc0, 0x00, 0x11, 0x08, 0x01, 0xe0, 0x02, 0x80, 0x03, 0x01, 0x22, 0x00, 0x02, 0x11, 0x01, 0x03, 0x11, 0x01, 0xff, 0xc4, 0x00, 0x1f, 0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0xff, 0xc4, 0x00, 0xb5, 0x10, 0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7d, 0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07, 0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0, 0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xff, 0xc4, 0x00, 0x1f, 0x01, 0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0xff, 0xc4, 0x00, 0xb5, 0x11, 0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77, 0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71, 0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0, 0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xff, 0xda, 0x00, 0x0c, 0x03, 0x01, 0x00, 0x02, 0x11, 0x03, 0x11, 0x00, 0x3f, 0x00
};
//检查捕获图片传到中心
void CheckCapturePicCenter(void);
void SendCapturePicData(INT16U CurrPack);
void SendCapturePicFinish(INT8U CurrNo, INT8U Result);
#endif
//extern INT16U  VideoCount;
//---------------------------------------------------------------------------
void timer_thread_func(void)
{
    INT16U val;
    if((Local.CurrentWindow == 0)||(Local.CurrentWindow == 6))
     if((Timer_Num % (TIMERPERSEC*2))==0)
      {
        CheckEth();
      }
    //存储亮度和音量值
    if((Timer_Num % (TIMERPERSEC*10))==0)
     {
      //xtm_printf("VideoCount = %d\n", VideoCount);
      CheckBrightVolume();
     }

    if((Timer_Num % (TIMERPERSEC*30))==0)
     {
      AddMultiGroup(Local.NsMultiAddr);

     #ifdef _SEND_TO_SELF_TEST    //发送数据包给自己
      if(Local.Status == 0)
        SendToSelf();  //自己给自己发送
     #endif
     }

    #ifdef _SEND_TO_SELF_TEST    //发送数据包给自己
     if((Timer_Num % (TIMERPERSEC*31))==0)
      {
       val = phy_reg_read(0, 0);
       if((val & 0x4000) == 0x4000)
        {
         val = val & 0xbfff;
         phy_reg_write(0, 0, val);   //loopback
        } 
      }
    #endif

   #ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
    if((Timer_Num % (TIMERPERSEC*2))==0)
      //检查捕获图片传到中心
      CheckCapturePicCenter();
   #endif

    //连续6秒没有收到在线确认，则认为断线
    if(Local.OnlineFlag == 1)
     {
      OnlineCheckFunc();
      //对讲控制，显示通话时间和判断超时
      TalkCtrlFunc();
     }

    //清状态提示信息函数
    ClearTipFunc();

    //设备定时报告状态函数
     if(LocalCfg.ReportTime != 0)
      {
       if(Local.ReportSend == 1)
        if(Local.ReportTimeNum >= (LocalCfg.ReportTime*TIMERPERSEC))
         {
          Local.RandReportTime = random(LocalCfg.ReportTime);     //产生随机数
          Local.ReportSend = 0;
          Local.ReportTimeNum = 0;
          //xtm_printf("Local.RandReportTime = %d, rand_time = %d\n", Local.RandReportTime);
//           printf("LocalCfg.ReportTime = %d, Local.RandReportTime = %d,temp_video_n = %d,            \
//                   vpbuf_n = %d, TimeStamp.CurrVideo = %d,                                            \
//                   curr_video_timestamp = %d, curr_audio_timestamp = %d, TimeStamp.CurrAudio = %d,    \
//                   temp_audio_n = %d, playbuf.n = %d, recbuf.n = %d\n",                               \
//                   LocalCfg.ReportTime, Local.RandReportTime, temp_video_n, vpbuf_n, TimeStamp.CurrVideo, \
//                   curr_video_timestamp, curr_audio_timestamp, TimeStamp.CurrAudio,                       \
//                   temp_audio_n, playbuf.n, recbuf.n);
         }
       if(Local.ReportSend == 0)
        if(Local.ReportTimeNum >= (Local.RandReportTime*TIMERPERSEC))
         {
          Local.ReportSend = 1;
          TimeReportStatusFunc();
         }
       Local.ReportTimeNum ++;
      }

     CheckOnlineTest(); //在线检测延时等待

     Timer_Num ++;
}
//---------------------------------------------------------------------------
void CheckBrightVolume(void) //检查亮度音量等设置值
{
  INT8U i, j;
  INT8U SaveOK = 0;


  //其它设置    
  if((Local.OldVoiceHint != LocalCfg.VoiceHint)
       ||(Local.OldSpkVolume != LocalCfg.SpkVolume)
       ||(Local.OldMicVolume != LocalCfg.MicVolume)
       ||(Local.OldTalkVoiceOpened != LocalCfg.TalkVoiceOpened)
       ||(Local.OldTalkVolume != LocalCfg.TalkVolume)
       ||(Local.OldBrightVolume != LocalCfg.BrightVolume)
       )
     {
      //xtm_printf("CheckBrightVolume:: Local.OldVoiceHint = %d, LocalCfg.VoiceHint = %d, Local.OldSpkVolume = %d, LocalCfg.SpkVolume = %d,\n");
      Local.OldVoiceHint = LocalCfg.VoiceHint;         //语音提示  0 关闭  1  打开
      Local.OldSpkVolume = LocalCfg.SpkVolume;         //SPK 音量
      Local.OldMicVolume = LocalCfg.MicVolume;         //MIC 音量
      Local.OldTalkVoiceOpened = LocalCfg.TalkVoiceOpened;         //对讲时音量  0 关闭  1  打开
      Local.OldTalkVolume = LocalCfg.TalkVolume;        //对讲时音量
      Local.OldBrightVolume = LocalCfg.BrightVolume;      //亮度
      SaveOK = 1;
     }
  if(SaveOK == 1)
   {
    WriteCfgFile();  //写本地设置文件
   }
}
//---------------------------------------------------------------------------
//清状态提示信息函数
void ClearTipFunc(void)
{
  INT8U CloseLcd;
  if(Local.Status != 0)
      Local.KeyPressTime = 0;
  //如不在开机界面，则计时，30S无操作自动返回
  //不在呼叫和操作界面    不在用户监视界面  不在报警界面 不在取消报警界面   不在 测试界面
  if((Local.CurrentWindow != 0)&&(Local.CurrentWindow != 34)&&(Local.CurrentWindow != 35)&&(Local.CurrentWindow != 192) &&(Local.Status == 0))
   {
    Local.KeyPressTime ++;
    if(Local.KeyPressTime > (IDLE_TIME*TIMERPERSEC))   //10*30
     {
      Local.KeyPressTime = 0;
      Local.CurrentWindow = 0;                                             
      DisplayTalkWindow(REFRESH);
     }
   }
  //如在开机界面，60S无操作关LCD
  if(Local.CurrentWindow == 0)
   {
    if(Local.LcdLightFlag == 1)
     {
      Local.LcdLightTime ++;
      if(Local.LcdLightTime > (CLOSELCD_TIME*TIMERPERSEC))
       {
        CloseLcd = 1;
        DisplayTalkWindow(REFRESH);
        Local.LcdLightTime = 0; //时间
       #if 0   //20130217 不关LCD
        if(CloseLcd == 1)
         {
          if(Local.LcdLightFlag == 1)
           {
            //关闭LCD
            xtm_printf("Lcd_Power 0\n");
            DisplayTalkWindow(REFRESH);
            Lcd_Power(0, LocalCfg.BrightVolume);
           }
          Local.LcdLightFlag = 0; //LCD背光标志
          Local.LcdLightTime = 0; //时间
        }
       #else  //20130413 待机时调暗背光
        if(CloseLcd == 1)
         {
          if(Local.LcdLightFlag == 1)
           {
            xtm_printf("Lcd_Power 0\n");
            Lcd_Power(1, 0);
           }
          Local.LcdLightFlag = 0; //LCD背光标志
          Local.LcdLightTime = 0; //时间
        }
       #endif
       }
     }
   }
  else
   {
    if(Local.LcdLightFlag == 0)
     {
      //点亮LCD背光
      Lcd_Power(1, LocalCfg.BrightVolume);
     }
    Local.LcdLightFlag = 1; //LCD背光标志
    Local.LcdLightTime = 0; //时间
   }

  //延时清提示衞畔⒈曛�
  if(PicStatBuf.Flag == 1)
   {
    PicStatBuf.Time ++;
    //1S    20130422  500ms
    if(PicStatBuf.Time >= (TIMERPERSEC))
     {
      PicStatBuf.Flag = 0;
      PicStatBuf.Time = 0;
      switch(PicStatBuf.Type)
       {
        case 7: //开锁界面
                DisplayTalkWindow(REFRESH);
                Local.OnlineFlag = 0;
                Local.Status = 0;  //状态为空闲
                break;
        case 8: //呼叫失败界面
                DisplayTalkWindow(REFRESH);
                Local.OnlineFlag = 0;
                Local.Status = 0;  //状态为空闲
                break;
        case 13:   //监视
        case 16:   //显示对讲图像操作窗口
                DisplayTalkWindow(REFRESH);
                Local.OnlineFlag = 0;
                Local.Status = 0;  //状态为空闲
                break;
       }
      PicStatBuf.Type = 0;
     }
   }
}
//---------------------------------------------------------------------------
void OnlineCheckFunc(void) //在线确认检测函数
{
  INT8U sendb[128], DenIP[4];
  INT16U sendlength;

  if((Local.Status != 21)&&(Local.Status != 22)&&(Local.Status != 23))
   {
    if(Local.Timer1Num >= (TIMERPERSEC*6))
     {
      if(Local.CallConfirmFlag == 0)
       {
         if((Local.Status == 1)||(Local.Status == 2)||(Local.Status == 5)||(Local.Status == 6)
           ||(Local.Status == 7)||(Local.Status == 8)||(Local.Status == 9)||(Local.Status == 10)) //对讲
             TalkEnd_Func();
         if((Local.Status == 3)||(Local.Status == 4)||(Local.Status == 90))  //监视
            {
             WatchEnd_Func();
             //PlaySoundTip();
            }
         
         //为防止铃音一直响
         //StopPlayWavFile();  //关闭铃音
         xtm_printf("没有收到在线确认，强制结束, Local.Timer1Num = %d, Local.Status = %d, Local.CallConfirmFlag = %d\n", \
                     Local.Timer1Num, Local.Status, Local.CallConfirmFlag);
         if(Local.Status == 0)
          {
           Local.OnlineFlag = 0;
           Local.CallConfirmFlag = 0;
           StopRecVideo();
           DelayMs(50);
           StopEncodeVideo();
           StopVideoClock();            //关闭视频采集编码时钟
           DisplayTalkWindow(REFRESH);
          }
       }
      else
        {
        Local.CallConfirmFlag = 0;
        }
      Local.Timer1Num = 0;
     }
    else
    {
     if((Local.Timer1Num % TIMERPERSEC)==0)
     {
      //对讲时被叫方发送在线确认包，每秒一个
      //监控时主控方发送在线确认包，每秒一个
    //  printf("Local.Status = %d\n", Local.Status);
      if((Local.Status == 2)||(Local.Status == 6)
        ||(Local.Status == 8)||(Local.Status == 10)
        ||(Local.Status == 3)|| (Local.Status == 90))
       {
        //头部
        xtm_memcopy(sendb, UdpPackageHead, 6);
        //命令
        if((Local.Status == 2)||(Local.Status == 6)||(Local.Status == 8)||(Local.Status == 10))  //对讲
          sendb[6] = VIDEOTALK;
        if(Local.Status == 3)  //监视
          sendb[6] = VIDEOWATCH;

        sendb[7]=ASK;        //主叫
        sendb[8]=CALLCONFIRM;//通话在线确认
        //子命令
        if((Local.Status == 3)||(Local.Status == 90)) //监视时，本机为主叫方
         {
          xtm_memcopy(sendb+9, LocalCfg.Addr, 20);
          xtm_memcopy(sendb+29, LocalCfg.IP, 4);
          xtm_memcopy(sendb+33, Remote.Addr[0], 20);
          xtm_memcopy(sendb+53, Remote.IP[0], 4);
         }
        //printf("Remote.IP[0] = %d.%d.%d.%d\n", Remote.IP[0][0], Remote.IP[0][1], Remote.IP[0][2], Remote.IP[0][3]);
        if((Local.Status == 2)||(Local.Status == 4)||(Local.Status == 6)
          ||(Local.Status == 8)||(Local.Status == 10))  //对讲时，本机为被叫方
         {
          xtm_memcopy(sendb+9, Remote.Addr[0], 20);
          xtm_memcopy(sendb+29, Remote.IP[0], 4);
          xtm_memcopy(sendb+33, LocalCfg.Addr, 20);
          xtm_memcopy(sendb+53, LocalCfg.IP, 4);
         }

        if(Local.Status == 90)
            {
            xtm_memcopy(sendb, UdpPackageHead2, 4);
            sendb[4] = 8;
            sendb[5] = SMARK_MONITOR;
            sendb[6] = 3;
            sendb[7] = 4;
            
            }
        
        //确认序号
        sendb[60] = (Local.OnlineNum & 0xFF000000) >> 24;
        sendb[59] = (Local.OnlineNum & 0x00FF0000) >> 16;
        sendb[58] = (Local.OnlineNum & 0x0000FF00) >> 8;
        sendb[57] = Local.OnlineNum & 0x000000FF;
        Local.OnlineNum ++;
        sendlength=61;

        if(Local.Status == 90)
        {
            CreateMultiUdpBuff(LocalPort[3], SINGLE_SEND, Remote.IP[0], sendb, sendlength, 0);
            //PlaySoundTip();
        }
        else
        {
            CreateMultiUdpBuff(LocalPort[1], SINGLE_SEND, Remote.IP[0], sendb, sendlength, 0);
        }

      /*DenIP[0] = 192;
        DenIP[1] = 168;
        DenIP[2] = 68;
        DenIP[3] = 98;
        CreateMultiUdpBuff(LocalPort[1], SINGLE_SEND, DenIP, sendb, sendlength);    */
       }
     }
     Local.Timer1Num ++;
   }
  }
}
//---------------------------------------------------------------------------
void TimeReportStatusFunc(void) //设备定时报告状态函数
{
  INT8U k;
  INT16U sendlength;
  INT8U sendb[1520];

  //头部
  xtm_memcopy(sendb, UdpPackageHead, 6);
  //命令
  sendb[6] = REPORTSTATUS;

  sendb[7]=ASK;        //主叫
  //子命令
  xtm_memcopy(sendb+8, LocalCfg.Addr, 20);
  xtm_memcopy(sendb+28, LocalCfg.Mac_Addr, 6);

  sendb[34] = LocalCfg.DefenceStatus;
  sendb[35] = LocalCfg.DefenceNum;
  //默认为10个防区
  #define _DEFENCENUM  10
  for(k=0; k<10; k++)
    xtm_memcopy(sendb + 36 + _DEFENCENUM*k, LocalCfg.DefenceInfo[k], 10);

  sendlength = 36 + _DEFENCENUM*10;
  CreateMultiUdpBuff(LocalPort[0], MULTI_SEND, LocalCfg.IP_Server, sendb, sendlength, 0);
}
//---------------------------------------------------------------------------
void TalkCtrlFunc(void)  //对讲控制，显示通话时间和判断超时
{
  int CallTimeOut;
  //1S
  if((Local.TimeOut % TIMERPERSEC)==0)
   {
    switch(Local.Status)
     {
      case 1:  //主叫对讲
      case 2:  //被叫对讲
             CallTimeOut = CALLTIMEOUT;
             ShowTalkConnectImage(NOFULLPAGE); //呼叫正在连接
             //在被呼叫时，主机有留影功能，故主机超时值比副机少3S
             if((LocalCfg.Addr[0] == 'S')||(LocalCfg.Addr[0] == 'B'))
              {
               if(Local.isHost == '0')
                 CallTimeOut = CALLTIMEOUT;
               else
                 CallTimeOut = CALLTIMEOUT + 3*TIMERPERSEC;
              }
             if(Local.TimeOut > CallTimeOut)
              {
               //查看是否在其它组播组内
               //DropMultiGroup(m_VideoSocket, NULL);

               //呼叫超时
               CallTimeOut_Func();
              }
             break;
      case 5:  //主叫通话
      case 6:  //被叫通话
             //计时
             ShowTalkWatchTime(2, NOFULLPAGE); //显示通话时间

             if(Local.TimeOut > Local.TalkTimeOut)
              {

				//20140225
			   //TALKTIMEOUT = DelayTalkTimeOut[LocalCfg.TalkTimeOut]*TIMERPERSEC; //通话最长时间
			   Local.TalkTimeOut = TALKTIMEOUT; //通话最长时间

	
               TalkEnd_Func();
               if(Local.Status == 0)
                 Local.OnlineFlag = 0;

               xtm_printf("通话超时\n");
              }
             break;
      case 3:  //监视    计时
               break;
      case 4:  //被监视
             ShowTalkWatchTime(0, NOFULLPAGE); //显示监视时间
             if(Local.TimeOut > WATCHTIMEOUT)
              {
               WatchEnd_Func();
               if(Local.Status == 0)
                 Local.OnlineFlag = 0;

               xtm_printf("监视超时\n");
              }
             break;
     case 90:
             ShowTalkWatchTime(0, NOFULLPAGE); //显示监视时间
             if(Local.TimeOut > WATCHTIMEOUT)
              {
               WatchEnd_Func();
               
               if(Local.Status == 0)
                {
                 Local.OnlineFlag = 0;
                 PlaySoundTip();
                }

               //WatchEnd_ClearStatus();

               xtm_printf("监视超时\n");
              }
            break;
     }
    }
   Local.TimeOut ++;       //监视超时,  通话超时,  呼叫超时，无人接听
}
//---------------------------------------------------------------------------
//检测网络信号
void CheckEth(void)
{
  INT16U val;
  INT8U NewStatus, OldStatus;
  OldStatus = Local.NetLanStatus;

  val = phy_reg_read(0, 1);
  if(val & 0x0004)
    NewStatus = 1;
  else
    NewStatus = 0;
  if(NewStatus != OldStatus)   //网络状态 0 断开  1 接通
   {
    Local.NetLanStatus = NewStatus;
    if (NewStatus == 0)
     {
      xtm_printf("Link down\n");
      //SetGpio(GPIOD_OUT_DATA, NET_LED_HIGH, SET_HIGH); //关网络LED
      if(Local.CurrentWindow == 6)
        DisplayImage(&net_image[0], FROMFILE, NOFULLPAGE);
     }

    if (NewStatus == 1)
     {
      xtm_printf("Link up\n");
      //SetGpio(GPIOD_OUT_DATA, NET_LED_LOW, SET_LOW); //开网络LED
      if(Local.CurrentWindow == 6)
        DisplayImage(&net_image[1], FROMFILE, NOFULLPAGE);
      //发送免费ARP
      SendFreeArp();
     }
   }
}
//---------------------------------------------------------------------------
#ifdef _SEND_TO_SELF_TEST    //发送数据包给自己
void SendToSelf(void)  //自己给自己发送
{
  INT16U sendlength, val;
  INT8U sendb[128];

  xtm_printf("SendToSelf\n");
  val = phy_reg_read(0, 0);
  val = val | 0x4000;
  phy_reg_write(0, 0, val);   //loopback

  //头部
  xtm_memcopy(sendb, UdpPackageHead, 6);
  //命令
  sendb[6] = SEND_SELF;

  sendb[7] = ASK;        //主叫
  //子命令
  xtm_memcopy(sendb+8, LocalCfg.Addr, 20);
  xtm_memcopy(sendb+28, LocalCfg.Mac_Addr, 6);

  sendlength = 34;
  CreateMultiUdpBuff(LocalPort[0], SINGLE_SEND, LocalCfg.IP, sendb, sendlength, 0);
}
#endif
//---------------------------------------------------------------------------
#ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
//检查捕获图片传到中心
void CheckCapturePicCenter(void)
{
   INT8U i, j, k;
   INT16U sendlength;
   INT32U file_len;
   INT8U sendb[128];
   //xtm_printf("CheckCapturePicCenter::CapturePic.ConnectCentered = %d, CapturePic.SendToCetnering = %d\n", CapturePic.ConnectCentered, CapturePic.SendToCetnering);
   if(CapturePic.ConnectCentered == 0)  //与中心连接状态
     return;
   if(CapturePic.SendToCetnering == 1)  //正在传给中心
     return;

   if(CapturePic.isVilid == 1)
     {
      CapturePic.FilePos = 0;
      //头部
      xtm_memcopy(sendb, UdpPackageHead, 6);
      //命令
      sendb[6] = CAPTUREPIC_SEND_START;

      sendb[7]=ASK;        //主叫
      //子命令
      xtm_memcopy(sendb+8, LocalCfg.Addr, 20);
      xtm_memcopy(sendb + 28, CapturePic.RemoteAddr, 20);  //目标地址
      sendb[48] = 0;                                  //序列号
      sendb[49] = 0;
      sendb[50] = 0;
      sendb[51] = 0;

      sendb[52] = 0;   //时间
      sendb[53] = 0;
      sendb[54] = 0;
      sendb[55] = 0;
      sendb[56] = 0;
      sendb[57] = 0;
      sendb[58] = 0;


      file_len = CapturePic.FileLen;
      file_len += 0x26F;
      file_len += 2;
      xtm_memcopy(sendb + 59, &file_len, sizeof(file_len));  //文件长度
      if((file_len % PACKDATALEN) == 0)
        CapturePic.Capture_Total_Package = ((INT32U)file_len / PACKDATALEN);
      else
        CapturePic.Capture_Total_Package = ((INT32U)file_len / PACKDATALEN) + 1;

      xtm_printf("CheckCapturePicCenter::CapturePic.FileLen = %d, file_len = %d, CapturePic.Capture_Total_Package = %d\n", (INT16U)CapturePic.FileLen, (INT16U)file_len, (INT16U)CapturePic.Capture_Total_Package);
      xtm_memcopy(sendb + 63, &CapturePic.Capture_Total_Package, sizeof(CapturePic.Capture_Total_Package));  //总包数
      for(k=0; k<CapturePic.Capture_Total_Package; k++)
        CapturePic.Capture_Send_Flag[k] = 0;
      sendlength = 67;
      CreateMultiUdpBuff(LocalPort[0], MULTI_SEND, LocalCfg.IP_Server, sendb, sendlength, 0);
     }
}
//---------------------------------------------------------------------------
void SendCapturePicData(INT16U CurrPack)
{
   INT32U CurrSize, CopyByte;
   INT16U sendlength;
   INT16U sendpos;
   INT8U sendb[1520];
   if(CapturePic.ConnectCentered == 0)
     return;
   CapturePic.SendToCetnering = 1;  //正在传给中心

   //头部
   xtm_memcopy(sendb, UdpPackageHead, 6);
   //命令
   sendb[6] = CAPTUREPIC_SEND_DATA;

   sendb[7] = ASK;        //主叫
   //子命令
   xtm_memcopy(sendb+8, LocalCfg.Addr, 20);
   xtm_memcopy(sendb + 28, CapturePic.RemoteAddr, 20);  //目标地址
   sendb[48] = 0;                                  //序列号
   sendb[49] = 0;
   sendb[50] = 0;
   sendb[51] = 0;

   sendpos = 58;
   if(CurrPack == 0)
    {
     xtm_memcopy(sendb + sendpos, head_640x480, 0x26f);
     CapturePic.FilePos = 0;
     sendpos += 0x26f;
     if(CurrPack == (CapturePic.Capture_Total_Package - 1))
      {
       CopyByte = ((INT32U)(CapturePic.FileLen - (PACKDATALEN - 0x26F)) % PACKDATALEN);
       sendb[sendpos + CopyByte] = 0xff;
       sendb[sendpos + CopyByte + 1] = 0xd9;
      } 
     else
       CopyByte = PACKDATALEN - 0x26f;
    }
   else
    {
     if(CurrPack == (CapturePic.Capture_Total_Package - 1))
      {
       CopyByte = ((INT32U)(CapturePic.FileLen - (PACKDATALEN - 0x26F)) % PACKDATALEN);
       sendb[sendpos + CopyByte] = 0xff;
       sendb[sendpos + CopyByte + 1] = 0xd9;
      }
     else
      {
       CopyByte = PACKDATALEN;
      }
    }
   if(CurrPack == (CapturePic.Capture_Total_Package - 1))
     CurrSize = CopyByte + 2;
   else
     CurrSize = PACKDATALEN;
   xtm_memcopy(sendb + 52, &CurrSize, sizeof(CurrSize));  //数据长度
   xtm_memcopy(sendb + 56, &CurrPack, sizeof(CurrPack));  //数据包编号

   xtm_printf("SendCapturePicData::CurrPack = %d, CurrSize = 0x%X %X, CopyByte = %d, CapturePic.FilePos = 0x%X, CapturePic.Capture_Total_Package = %d\n", \
               CurrPack, (INT16U)((CurrSize & 0xffff0000) >> 16), (INT16U)(CurrSize & 0xffff), (INT16U)CopyByte, CapturePic.FilePos, CapturePic.Capture_Total_Package);

   outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
   outportb(MEMORY_WINDOW_BASE_H, 0x6f);
   xtm_memcopy(sendb + sendpos, Memory6_Stream + CapturePic.FilePos, CopyByte);
   outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);

   CapturePic.FilePos += CopyByte;

   sendlength = 58 + CurrSize;
   CreateMultiUdpBuff(LocalPort[0], MULTI_SEND, LocalCfg.IP_Server, sendb, sendlength, 0);
}
//---------------------------------------------------------------------------
void SendCapturePicFinish(INT8U CurrNo, INT8U Result)
{
   INT16U sendlength;
   INT8U sendb[128];
   if(Result == 1)
     xtm_printf("发送呼叫照片->发送结束(成功) 命令\n");
   else
     xtm_printf("发送呼叫照片->发送结束(失败) 命令\n");

   //头部
   xtm_memcopy(sendb, UdpPackageHead, 6);
   //命令
   if(Result == 1)
     sendb[6] = CAPTUREPIC_SEND_SUCC;
   else
     sendb[6] = CAPTUREPIC_SEND_FAIL;

   sendb[7]=ASK;        //主叫
   //子命令
   xtm_memcopy(sendb+8, LocalCfg.Addr, 20);
   xtm_memcopy(sendb + 28, CapturePic.RemoteAddr, 20);  //目标地址
   sendb[48] = 0;                                  //序列号
   sendb[49] = 0;
   sendb[50] = 0;
   sendb[51] = 0;

   sendlength = 52;
   CreateMultiUdpBuff(LocalPort[0], MULTI_SEND, LocalCfg.IP_Server, sendb, sendlength, 0);
}
#endif
//---------------------------------------------------------------------------

