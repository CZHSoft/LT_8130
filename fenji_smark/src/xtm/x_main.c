#include "includes.h"

#include "stdlib.h"

#define x_main_h

#define CommonH
#include "common.h"

/*
****************************************
201310
增加读写广告文件类
201311
增加读写序列号文件类
****************************************
*/


void Init_LocalVar(void); //初始化 Local 结构

void Init_Module_Clock(void); //根据需要关闭各模块时钟
void Set_VideoCapture_Clock(INT8U Type); //操作视频采集时钟  1  开   0  关
void Set_Encode_Clock(INT8U Type); //操作视频编码时钟  1  开   0  关
void Set_Decode_Clock(INT8U Type); //操作视频解码时钟  1  开   0  关
void Set_Disp_Clock(INT8U Type); //操作显示时钟  1  开   0  关

void ResetLocalCfg(void);    //复位配置文件

void SetPTC1(INT8U freq);
//freq  1  --  正常　　16 --  16分频
void SetPTC2(INT8U freq);

INT8U *jpeg_bitstream;    //静态jpeg 图片
INT8U *mjpeg_stream[MAX_STREAM_NUM];   //mjpeg 流
INT16U mjpeg_stream_len[MAX_STREAM_NUM];   //mjpeg 流
INT8U Curr_Mjpeg_Stream;

INT8U *Mdata_Stream;      //网卡接收
INT8U *Send_Stream;       //发送
INT8U *AudioSend_Stream;       //音频发送包头Stream
INT8U *VideoSend_Stream;       //视频发送包头Stream
INT8U *Flash_Stream;      //Flash存储

INT8U *Net_Stream[4];       //256K 缓冲  网卡接收

INT8U *Memory6_Stream;     //0x60 0000 内存地址
void Init_BitStream(void); //初始化 bitstream

//读设置文件
void ReadCfgFile(void);
//写设置文件
void WriteCfgFile(void);
//写数据到Flash
void SaveToFlash(INT32U FlashAddr, INT8U *buf, INT16U length);
//从Flash读数据
void ReadFromFlash(INT32U FlashAddr, INT8U *buf, INT16U length);

void ReadAdFile(void);

void WriteAdFile(void);

void ReadSwitchFile(void);

void WriteSwitchFile(void);

//---------------------------------------------------------------------------
void Init_BitStream(void) //初始化 bitstream
{
  //C-F段是程序，分配会崩溃；0-1是数据；2-3、4-7、8-B可分配
  INT8U i;
  //图片解码缓存
  jpeg_bitstream = MK_FP(0x2000, 0);

  Mdata_Stream = MK_FP(0x3000, 0);         //2K
  Send_Stream = MK_FP(0x3000, 2048);       //2K
  send_mac_buf = MK_FP(0x3000, 4096);      //2K
  send_mac_buf2 = MK_FP(0x3000, 4096+1024);  //1K
  PcmI_Stream = MK_FP(0x3000, 6144);       // 8K

  AudioSend_Stream = MK_FP(0x3000, 6144 + 8192);                    //1K
  VideoSend_Stream = MK_FP(0x3000, 6144 + 8192 + 1024);             //1K
  Flash_Stream = MK_FP(0x3000, 6144 + 8192 + 2048);      //Flash存储    4K
  for(i=0; i<UDPSENDMAX; i++)
   {
    Multi_Udp_Buff[i].isValid = 0;
    Multi_Udp_Buff[i].buf = MK_FP(0x3000, 6144 + 8192 + 2048 + 4096 + i*1536);
   }

  //信息内容，如更改了短信息最大条数，要注意此处的内存分配
  for(i=0; i<INFOMAXITEM; i++)
   {
    InfoBuff[i] = MK_FP(0x3000, 52*1024 + i*INFOMAXSIZE);
   }

  Net_Stream[0] = MK_FP(0x4000, 0);
  Net_Stream[1] = MK_FP(0x5000, 0);
  Net_Stream[2] = MK_FP(0x6000, 0);
  Net_Stream[3] = MK_FP(0x7000, 0);

  mjpeg_stream[0] = MK_FP(0x8000, 0);
  mjpeg_stream[1] = MK_FP(0x9000, 0);

  PcmO_Stream[0] = MK_FP(0xA000, 0);       //  0xA000
  PcmO_Stream[1] = MK_FP(0xB000, 0);       //  0xB000
  //PcmO_Stream[2] = MK_FP(0xC000, 0); 
  //PcmO_Stream[3] = MK_FP(0xD000, 0); 

  //6段内存使用, 需将64K 映射到 0xA000 才可使用
  Memory6_Stream = MK_FP(0xA000, 0);

  Curr_Mjpeg_Stream = 0;
}
//---------------------------------------------------------------------------
void Init_LocalVar(void) //初始化 Local 结构
{
  INT8U i, j;
  INT32U temp_addr;
  INT8U Scene_Defence1[5][MAXDEFENCENUM] = {
                                          {1, 1, 1, 1, 1, 1, 1, 1},
                                          {1, 1, 0, 0, 0, 0, 0, 0},
                                          {0, 0, 1, 1, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 1, 1, 0, 0},
                                          {0, 0, 0, 0, 0, 0, 1, 1}
                                         };
  Local.MachineType = 0x80;  //0x80  --- 8130
  Local.KeyTouchNo = 0;
  
  Init_BitStream(); //初始化 bitstream

  ReadCfgFile();    //读本地设置文件

  LocalCfg.MicVolume = 5;
  
  Local.NetLanStatus = 0;   //网络状态 0 断开  1 接通
  Local.OldNetSpeed = 100;  //网络速度
  Local.NewInfo = 0;  //有新信息
  Local.NewRecPic = 0;  //有新留影
  
  Local.RecordPic = 0;  //留照片  0 不留
  Local.IFrameCount = 0; //I帧计数
  Local.IFrameNo = 125;    //留第几个I帧

  Local.ForceEndWatch = 0;  //有呼叫时，强制关监视


  Local.LcdLightFlag = 1; //LCD背光标志
  Local.LcdLightTime = 0; //时间
  Local.KeyPressTime = 0;

  LocalCfg.VoiceHint = 1;
  Local.OldVoiceHint = LocalCfg.VoiceHint;         //语音提示  0 关闭  1  打开
  Local.OldSpkVolume = LocalCfg.SpkVolume;         //SPK 音量
  Local.OldMicVolume = LocalCfg.MicVolume;         //MIC 音量
  Local.OldTalkVoiceOpened = LocalCfg.TalkVoiceOpened;         //对讲时音量  0 关闭  1  打开
  Local.OldTalkVolume = LocalCfg.TalkVolume;        //对讲时音量
  for(i=0; i<3; i++)
    Local.OldScrSave[i] =  LocalCfg.ScrSave[i];        //屏幕保护  0 关闭  1  打开
  if(LocalCfg.BrightVolume > 4)
    LocalCfg.BrightVolume = 3;  
  Local.OldBrightVolume = LocalCfg.BrightVolume;      //亮度
  Local.OldKeyVoice = LocalCfg.KeyVoice;          //按键音    0 关闭  1  打开

/*  //防区设置
   for(i=0; i<MAXDEFENCENUM; i++)
    {
      Local.OldDefenceSetup[i].dType = LocalCfg.DefenceSetup[i].dType;
      Local.OldDefenceSetup[i].dDelay = LocalCfg.DefenceSetup[i].dDelay;
      Local.OldDefenceSetup[i].tType = LocalCfg.DefenceSetup[i].tType;
    }        */


  //20101116 布防延时改为 100S
  //外出延时
  LocalCfg.Out_DelayTime = 30;//100;
  //报警延时
  LocalCfg.Alarm_DelayTime = 30;
  if((LocalCfg.Addr[0] != 'S')&&(LocalCfg.Addr[0] != 'B'))
    LocalCfg.Addr[0] = 'B';
  if((LocalCfg.Addr[0] == 'S')||(LocalCfg.Addr[0] == 'B'))
   {
    Local.AddrLen = 12;  //地址长度  S 12  B 12 M 8 H 6
   }
  LocalCfg.Addr[Local.AddrLen] = '\0';
  LocalCfg.AddrVerifyMa[6] = '\0';
  
  Local.Weather[0] = 1;  //天气预报
  Local.Weather[1] = 20;//0;
  Local.Weather[2] = 27;//0;

  Local.ReportSend = 0;  //设备定时报告状态已发送
  Local.RandReportTime = 1;
  Local.ReportTimeNum  = 0;

  Local.nowvideoframeno = 1;   //当前视频帧编号
  Local.nowaudioframeno = 1;   //当前音频帧编号  

  Local.NoBreak = 0;   //免扰状态 1 免扰  0 正常
  //主机
  Local.isHost = '0';
  Local.ConnToHost = 0;
  Local.ConnToHostTime = 0;
  Local.HostIP[0] = 0;
  Local.HostIP[1] = 0;
  Local.HostIP[2] = 0;
  Local.HostIP[3] = 0;
  Local.isHost = LocalCfg.Addr[11];
  Local.DenNum = 0;
  xtm_memcopy(Local.HostAddr, LocalCfg.Addr, 20);
  Local.HostAddr[11] = '0';

  //防区状态
  LocalCfg.DefenceNum = 1;
  //20101028  保存 布防信息

   for(j=0; j<5; j++)
    for(i=0; i<MAXDEFENCENUM; i++)
     {
      LocalCfg.Scene_Defence[j][i] = Scene_Defence1[j][i];
      if(LocalCfg.Scene_Defence[j][i] == 1)
       {
        LocalCfg.DefenceInfo[i][2] = 1;  // 布防
        LocalCfg.DefenceInfo[i][3] = 0;  // 无报警
       }
      else
       {
        LocalCfg.DefenceInfo[i][2] = 0;  // 布防
        LocalCfg.DefenceInfo[i][3] = 0;  // 无报警
       }
     }

   //这里防区信息初始化
   
   //防区设置   0  1  2 - 延时防区   3  4  5  普通防区   6  7  24小时防区
   for(i=0; i<=2; i++)
    {
          LocalCfg.DefenceSetup_dType[i] = 3;   //防区类型     0 - 日防区  1 - 夜防区  2 - 24小时防区  3 - 自定义防区
          LocalCfg.DefenceSetup_dDelay[i] = 5;  //延时时间     0 - 0秒   1 - 5秒  2 - 10秒  3 - 15秒  4 - 20秒  5 - 25秒  6 - 60秒
          LocalCfg.DefenceSetup_tType[i] = 4;   //探头类型     0 - 烟感  1 - 燃气  2 - 防拆  3 - 紧急  4 - 红外  5 - 门磁  6 - 窗磁
    }
   for(i=3; i<=5; i++)
    {
          LocalCfg.DefenceSetup_dType[i] = 3;   //防区类型     0 - 日防区  1 - 夜防区  2 - 24小时防区  3 - 自定义防区
          LocalCfg.DefenceSetup_dDelay[i] = 0;  //延时时间     0 - 0秒   1 - 5秒  2 - 10秒  3 - 15秒  4 - 20秒  5 - 25秒  6 - 60秒
          LocalCfg.DefenceSetup_tType[i] = 4;   //探头类型     0 - 烟感  1 - 燃气  2 - 防拆  3 - 紧急  4 - 红外  5 - 门磁  6 - 窗磁
    }
   for(i=6; i<=7; i++)
   //for(i=0; i<=7; i++)
    {
          LocalCfg.DefenceSetup_dType[i] = 2;   //防区类型     0 - 日防区  1 - 夜防区  2 - 24小时防区  3 - 自定义防区
          LocalCfg.DefenceSetup_dDelay[i] = 0;  //延时时间     0 - 0秒   1 - 5秒  2 - 10秒  3 - 15秒  4 - 20秒  5 - 25秒  6 - 60秒
          LocalCfg.DefenceSetup_tType[i] = 4;   //探头类型     0 - 烟感  1 - 燃气  2 - 防拆  3 - 紧急  4 - 红外  5 - 门磁  6 - 窗磁
    }

  DeltaLen = 9 + sizeof(struct talkdata1);  //数据包有效数据偏移量

  Local.Status = 0;  //状态为空闲
  Local.TalkEndStatus = 0;  //状态为空闲

  Local.AlarmByte[0] = 0x0;         //报警
  Local.AlarmByte[1] = 0x0;         //报警
  Local.DefenceDelayFlag = 0;    //布防延时标志
  Local.DefenceDelayTime = 0;    //计数
  for(i=0; i<MAXCOUNT; i++)
   {
    Local.AlarmDelayFlag[i] = 0;     //报警延时标志
    Local.AlarmDelayTime[i] = 0;    //计数
   }

  Local.IP_Group[0] = 0; //组播地址
  Local.IP_Group[1] = 0;
  Local.IP_Group[2] = 0;
  Local.IP_Group[3] = 0;

  Local.NsMultiAddr[0] = 238;    //NS组播地址
  Local.NsMultiAddr[1] = 9;
  Local.NsMultiAddr[2] = 9;
  Local.NsMultiAddr[3] = 1;

  strcpy(NullAddr, "00000000000000000000");

  Remote.IP[0][0] = 192;
  Remote.IP[0][1] = 168;
  Remote.IP[0][2] = 68;
  Remote.IP[0][3] = 88;
  xtm_memcopy(Remote.Addr[0],NullAddr,20);
  xtm_memcopy(Remote.Addr[0],"S0001010101",11);      //

  Local.Test_OV7725FrameRate = 0;    //OV7725 输出帧率

 /* sprintf(Local.SoundName[0], "%s/%s\0", syswav_dir1, "ring1.wav");
  sprintf(Local.SoundName[1], "%s/%s\0", syswav_dir1, "ring2.wav");
  sprintf(Local.SoundName[2], "%s/%s\0", syswav_dir1, "ring3.wav");
  sprintf(Local.SoundName[3], "%s/%s\0", syswav_dir1, "ring4.wav");
  sprintf(Local.SoundName[4], "%s/%s\0", syswav_dir1, "ring4.wav");
  sprintf(Local.SoundName[5], "%s/%s\0", syswav_dir1, "ring4.wav");   */

  #ifdef _CAPTURESCREEN
    Local.OpenLockKeyNum = 0;
    Local.OpenLockTime = 0;
  #endif

  if(LocalCfg.Language == 2)  //语言  0 -- 中文简体   1 -- 中文繁体   2 -- 英文
    Local.LanguageNo = 1;
  else
    Local.LanguageNo = 0;
  xtm_printf("Init_LocalVar 1\n");

  //延时清提示信息标志
  PicStatBuf.Flag = 0;
  PicStatBuf.Type = 0;
  PicStatBuf.Time = 0;

  Local.Conflict_Flag = 0;    //IP地址冲突
  for(i=0; i<6; i++)
    Local.Conflict_Mac[i] = 0x0;

  Local.GpioWatchDogStarted = 1;  //在GPIO检测线程中启动WatchDog
  Local.RemoteDebugInfo = 0;      //发送远程调试信息

        LocalCfg.EncodeFrameSize = VIDEO_VGA; //编码大小
        LocalCfg.VideoClosed = 1; //视频关闭

 #ifdef _REMOTECALLTEST  //远程呼叫测试
  Local.EnterTestMode = 0;  //测试模式, 5秒自动接听
  Local.CloseTestRing = 0;  //测试模式, 关闭铃声
 #endif
  //20130318 测试
  //LocalCfg.VideoSdramRefresh = 1;    //LCD显示时 SDRAM刷新周期  0  8周期  1 4周期
  if(LocalCfg.MicDecValue > 3) //展箱、展板模式,需减小 Mic音量
    LocalCfg.MicDecValue = 0;  
}
//---------------------------------------------------------------------------
void ResetLocalCfg(void)    //复位配置文件
{
  INT8U i, j;
        xtm_printf("ResetLocalCfg::\n");
        //头部特征码
        xtm_memcopy(LocalCfg.FlashHead, "XXXCID", 6);
        //地址编码
        xtm_memcopy(LocalCfg.Addr,NullAddr,20);
        xtm_memcopy(LocalCfg.Addr,"S00010108090",12);
        //主副机同步码
        strcpy(LocalCfg.AddrVerifyMa, "123456");
        //网卡地址
        LocalCfg.Mac_Addr[0] = 0x00;
        LocalCfg.Mac_Addr[1] = 0x23;
        LocalCfg.Mac_Addr[2] = 0x60;
        LocalCfg.Mac_Addr[3] = 0x22;
        LocalCfg.Mac_Addr[4] = 0x41;
        LocalCfg.Mac_Addr[5] = 0x63;
        //IP地址
        LocalCfg.IP[0] = 198;
        LocalCfg.IP[1] = 168;
        LocalCfg.IP[2] = 0;
        LocalCfg.IP[3] = 163;
        //子网掩码
        LocalCfg.IP_Mask[0] = 255;
        LocalCfg.IP_Mask[1] = 255;
        LocalCfg.IP_Mask[2] = 255;
        LocalCfg.IP_Mask[3] = 0;
        //网关地址
        LocalCfg.IP_Gate[0] = 198;
        LocalCfg.IP_Gate[1] = 168;
        LocalCfg.IP_Gate[2] = 0;
        LocalCfg.IP_Gate[3] = 1;
        //NS（名称解析）服务器地址
        LocalCfg.IP_NS[0] = 192;
        LocalCfg.IP_NS[1] = 168;
        LocalCfg.IP_NS[2] = 68;
        LocalCfg.IP_NS[3] = 1;
        //主服务器地址
        LocalCfg.IP_Server[0] = 192;
        LocalCfg.IP_Server[1] = 168;
        LocalCfg.IP_Server[2] = 68;
        LocalCfg.IP_Server[3] = 1;
        //广播地址
        LocalCfg.IP_Broadcast[0] = 198;
        LocalCfg.IP_Broadcast[1] = 168;
        LocalCfg.IP_Broadcast[2] = 0;
        LocalCfg.IP_Broadcast[3] = 255;
        
        //设备定时报告状态时间
        LocalCfg.ReportTime = 10;
        //布防状态
        LocalCfg.DefenceStatus = 0;
        //防区模块个数
        LocalCfg.DefenceNum = 1;
        for(i=0; i<8; i++)
         for(j=0; j<10; j++)
          LocalCfg.DefenceInfo[i][j] = 0;
        //工程密码
        strcpy(LocalCfg.EngineerPass, "123456");

        //防区设置   0  1  2 - 延时防区   3  4  5  普通防区   6  7  24小时防区
        for(i=0; i<=2; i++)
         {
          LocalCfg.DefenceSetup_dType[i] = 3;   //防区类型     0 - 日防区  1 - 夜防区  2 - 24小时防区  3 - 自定义防区
          LocalCfg.DefenceSetup_dDelay[i] = 5;  //延时时间     0 - 0秒   1 - 5秒  2 - 10秒  3 - 15秒  4 - 20秒  5 - 25秒  6 - 60秒
          LocalCfg.DefenceSetup_tType[i] = 4;   //探头类型     0 - 烟感  1 - 燃气  2 - 防拆  3 - 紧急  4 - 红外  5 - 门磁  6 - 窗磁
         }
        for(i=3; i<=5; i++)
         {
          LocalCfg.DefenceSetup_dType[i] = 3;   //防区类型     0 - 日防区  1 - 夜防区  2 - 24小时防区  3 - 自定义防区
          LocalCfg.DefenceSetup_dDelay[i] = 0;  //延时时间     0 - 0秒   1 - 5秒  2 - 10秒  3 - 15秒  4 - 20秒  5 - 25秒  6 - 60秒
          LocalCfg.DefenceSetup_tType[i] = 4;   //探头类型     0 - 烟感  1 - 燃气  2 - 防拆  3 - 紧急  4 - 红外  5 - 门磁  6 - 窗磁
         }
        for(i=6; i<=7; i++)
         {
          LocalCfg.DefenceSetup_dType[i] = 2;   //防区类型     0 - 日防区  1 - 夜防区  2 - 24小时防区  3 - 自定义防区
          LocalCfg.DefenceSetup_dDelay[i] = 0;  //延时时间     0 - 0秒   1 - 5秒  2 - 10秒  3 - 15秒  4 - 20秒  5 - 25秒  6 - 60秒
          LocalCfg.DefenceSetup_tType[i] = 4;   //探头类型     0 - 烟感  1 - 燃气  2 - 防拆  3 - 紧急  4 - 红外  5 - 门磁  6 - 窗磁
         }

        //进入延时
        LocalCfg.In_DelayTime = 30;
        //外出延时
        LocalCfg.Out_DelayTime = 30;
        //报警延时
        LocalCfg.Alarm_DelayTime = 30;

        //声音
        for(i=0; i<8; i++)
          LocalCfg.SoundSetup[i] = 0;
        //访客留影
        LocalCfg.RecPhoto = 0;
        //语言选择
        LocalCfg.Language = 1;
        //铃声类型  0 普通型  1  卡通型  2  摇滚型   3  抒情型
        LocalCfg.RingType = 0;
        //回铃类型  0 普通型  1  卡通型  2  摇滚型   3  抒情型
        LocalCfg.CallWaitRing = 0;
        //语音提示  0 关闭  1  打开
        LocalCfg.VoiceHint = 1;
        //SPK 音量
        LocalCfg.SpkVolume = 2;
        //MIC 音量
        LocalCfg.MicVolume = 2;
        //对讲时音量
        LocalCfg.TalkVoiceOpened = 1;
        LocalCfg.TalkVolume = 2;

        //屏幕保护
        LocalCfg.ScrSave[0] = 1;
        LocalCfg.ScrSave[1] = 0;
        LocalCfg.ScrSave[2] = 0;
        //首页设置  0 默认界面  1 电话界面
        LocalCfg.MainPage = 0;
        //亮度
        LocalCfg.BrightVolume = 2;
        //对比度
        LocalCfg.ContrastVolume = 2;
        //按键音    0 关闭  1  打开
        LocalCfg.KeyVoice = 1;
        //触摸屏
        LocalCfg.Ts_X0 = 1996;//1901;
        LocalCfg.Ts_Y0 = 2005;//2001;
        LocalCfg.Ts_deltaX = 3685;//3744;
        LocalCfg.Ts_deltaY = -3112;//3555;
        LocalCfg.Ts_Mode = 0;     //X   Y  正常

        LocalCfg.Language = 0;  //语言  0 -- 中文简体   1 -- 中文繁体   2 -- 英文

        LocalCfg.EncodeFrameRate = 3; //编码帧率
        LocalCfg.EncodeFrameSize = VIDEO_VGA; //编码大小
        LocalCfg.VideoClosed = 0; //视频关闭
        LocalCfg.VideoOutput = 0;    //视频输出  0  正常  1 自检
        LocalCfg.VideoSdramRefresh = 0;    //LCD显示时 SDRAM刷新周期  0  8周期  1 4周期
        LocalCfg.MultiMachineSupport = 0;  //多分机支持,对主机有效，如不支持，则不发广播查询包
        LocalCfg.DenMaxNum = 1;  //副机最大数量，这一数量需小于 SUBMAXNUM， 有利于减小广播包
        //outportb(POWER_CONTROL1, 0x04);//电源控制寄存器1    4周期
}
//---------------------------------------------------------------------------
void SetPTC1(INT8U freq)
{
//watchdog
 /* INT32U luminance;
    luminance=CPU_MAIN_FREQ/100; //5ms
    
    outportb(PTC1_HRC0,(luminance/4));
    outportb(PTC1_HRC1,(luminance/4)>>8);
    outportb(PTC1_HRC2,(luminance/4)>>16);

    outportb(PTC1_LRC0,luminance);
    outportb(PTC1_LRC1,luminance>>8);
    outportb(PTC1_LRC2,luminance>>16);	
    outportb(PTC1_CTRL0,0x21);         */
}
//---------------------------------------------------------------------------
//freq  1  --  正常　　16 --  16分频
void SetPTC2(INT8U freq)
{
  INT32U luminance;
  outportb(PTC2_CTRL0,0x41);
  luminance = CPU_MAIN_FREQ/100/freq;    //20120222 改晶振

  outportb(PTC2_HRC0,(luminance/2));
  outportb(PTC2_HRC1,(luminance/2)>>8);
  outportb(PTC2_HRC2,(luminance/2)>>16);

  outportb(PTC2_LRC0,luminance);
  outportb(PTC2_LRC1,luminance>>8);
  outportb(PTC2_LRC2,luminance>>16);
  outportb(PTC2_CTRL0,0x21);
  //outportb(PTC2_CTRL0,inportb(PTC2_CTRL0)|0x80);
}
//---------------------------------------------------------------------------
void Init_Module_Clock(void) //根据需要打开、关闭各模块时钟
{
#if 1
  //CONTROL_REG2
    //D4：关闭PTC0时钟
    //D5：关闭PTC1时钟
    //D6：关闭PTC2时钟
    //D7：关闭SPI时钟

  //POWER_CONTROL2
    //D[7:0](R/W)
    //D0：关闭MJPEG视频采集时钟  30mA
    //D1：关闭MJPEG视频编码时钟
    //D2：关闭MJPEG视频解码时钟  30mA
    //D3：关闭显示时钟     20 mA
    //D4：关闭PCM时钟      10 mA
    //D5：关闭MAC时钟      10 mA
    //D6：关闭UART0时钟
    //D7：关闭UART1时钟

           //xtm_printf("关闭 PTC0 时钟\n");
           //outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x10);
           xtm_printf("打开 PTC0 clock\n");
           outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xEF);

           xtm_printf("关闭 PTC1 时钟\n");
           outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x20);
           //xtm_printf("打开 PTC1 时钟\n");
           //outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xDF);

           //xtm_printf("关闭 PTC2 时钟\n");
           //outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x40);
           xtm_printf("打开 PTC2 时钟\n");
           outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xBF);

           //xtm_printf("关闭 SPI 时钟\n");
           //outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x80);
           xtm_printf("打开 SPI 时钟\n");
           outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0x7F);

           Set_VideoCapture_Clock(0);  //操作视频采集时钟  1  开   0  关

           Set_Encode_Clock(1); //操作视频编码时钟  1  开   0  关

         #ifndef _CAMERA_SUPPORT     //如不用摄像头，GPIOB6、7为Comm1, GPIOC 作为8个防区使用
           xtm_printf("关闭MJPEG视频采集时钟\n");
           outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x01);
           xtm_printf("关闭MJPEG视频编码时钟\n");
           outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x02);
         #endif

           xtm_printf("打开MJPEG视频解码时钟\n");
           Set_Decode_Clock(1);  //操作视频解码时钟  1  开   0  关

           Set_Disp_Clock(1); //操作显示时钟  1  开   0  关

           //xtm_printf("关闭PCM时钟\n");
           //outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x10);
           xtm_printf("打开PCM时钟\n");
           outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xEF);

           //xtm_printf("关闭MAC时钟\n");
           //outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x20);
           xtm_printf("打开MAC时钟\n");
           outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xDF);

           //xtm_printf("关闭UART0时钟\n");
           //outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x40);
           xtm_printf("打开UART0时钟\n");
           outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xBF);

           xtm_printf("关闭UART1时钟\n");
           outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x80);
           //xtm_printf("打开UART1时钟\n");
           //outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0x7F);
#endif           
}
//---------------------------------------------------------------------------
void Set_VideoCapture_Clock(INT8U Type) //操作视频采集时钟  1  开   0  关
{
  if(Type)
   {
    xtm_printf("打开MJPEG视频采集时钟\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xFE);
   }
  else
   {
    xtm_printf("关闭MJPEG视频采集时钟\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x01);
   }   
}
//---------------------------------------------------------------------------
void Set_Encode_Clock(INT8U Type) //操作视频编码时钟  1  开   0  关
{
//编码时钟不可重复开关   室内机可关
#ifdef _CAMERA_SUPPORT     //如不用摄像头，GPIOB6、7为Comm1, GPIOC 作为8个防区使用   测试模式
  if(Type)
   {
    xtm_printf("打开MJPEG视频编码时钟\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xFD);

   }
  else
   {
    //xtm_printf("关闭MJPEG视频编码时钟\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x02);
   }
#endif   
}
//---------------------------------------------------------------------------
void Set_Decode_Clock(INT8U Type) //操作视频解码时钟  1  开   0  关
{
  if(Type)
   {
    //xtm_printf("打开MJPEG视频解码时钟\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xFB);
    //outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x04);//使能MJPEG解码中断
   }
  else
   {
    //xtm_printf("关闭MJPEG视频解码时钟\n");
    WaitDecodeFinish();
    //outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xFB);//禁止MJPEG解码中断
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x04);
   }       
}
//---------------------------------------------------------------------------
void Set_Disp_Clock(INT8U Type) //操作显示时钟  1  开   0  关
{
  if(Type)
   {
    xtm_printf("打开显示时钟\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xF7);
   }
  else
   {
    xtm_printf("关闭显示时钟\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x08);
   }
}
//---------------------------------------------------------------------------
//读本地设置文件
void ReadCfgFile(void)
{
  INT8U iCount;
  iCount = 0;
  while(iCount < 5)
   {
    ReadFromFlash(LOCALCFG_ADDR, Flash_Stream, 0x1000);
    xtm_memcopy(&LocalCfg, Flash_Stream, sizeof(struct LocalCfg1));
    iCount ++;
    if((LocalCfg.FlashHead[0] == 'X')||(LocalCfg.FlashHead[1] == 'X')||(LocalCfg.FlashHead[2] == 'X')||(LocalCfg.FlashHead[3] == 'C')||(LocalCfg.FlashHead[4] == 'I')||(LocalCfg.FlashHead[5] == 'D'))
      break;
   }
  if(iCount > 1)
    xtm_printf("ReadCfgFile::iCount = %d\n", iCount);
  if((LocalCfg.FlashHead[0] != 'X')||(LocalCfg.FlashHead[1] != 'X')||(LocalCfg.FlashHead[2] != 'X')||(LocalCfg.FlashHead[3] != 'C')||(LocalCfg.FlashHead[4] != 'I')||(LocalCfg.FlashHead[5] != 'D'))
   {
    ReadFromFlash(LOCALCFG_ADDR1, Flash_Stream, 0x1000);
    xtm_memcopy(&LocalCfg, Flash_Stream, sizeof(struct LocalCfg1));
    if((LocalCfg.FlashHead[0] != 'X')||(LocalCfg.FlashHead[1] != 'X')||(LocalCfg.FlashHead[2] != 'X')||(LocalCfg.FlashHead[3] != 'C')||(LocalCfg.FlashHead[4] != 'I')||(LocalCfg.FlashHead[5] != 'D'))
     {
      ResetLocalCfg();    //复位配置文件
      WriteCfgFile();     //写本地设置文件
     } 
   }
  LocalCfg.Addr[12] = '\0';

  LocalCfg.BootNo ++;
  //WriteCfgFile(); 
}
//---------------------------------------------------------------------------
//写本地设置文件
void WriteCfgFile(void)
{
  xtm_memcopy(Flash_Stream, &LocalCfg, sizeof(struct LocalCfg1));
  SaveToFlash(LOCALCFG_ADDR, Flash_Stream, 0x1000);
  SaveToFlash(LOCALCFG_ADDR1, Flash_Stream, 0x1000);
}
//---------------------------------------------------------------------------
void ReadAdFile(void)
{
	INT8U iCount;
	  iCount = 0;
	  while(iCount < 5)
	   {
		//ReadFromFlash(0x0D6000, Flash_Stream, 0x1000);
		//xtm_memcopy(&AdPic, Flash_Stream, sizeof(struct AdPic1));
		//iCount ++;
		//if(AdPic.PicCount!=0)
		  //break;
	    }
}
//---------------------------------------------------------------------------
void WriteAdFile(void)
{
	//xtm_memcopy(Flash_Stream, &AdPic, sizeof(struct AdPic1));
	//SaveToFlash(0x0D6000, Flash_Stream, 0x1000);

}
//---------------------------------------------------------------------------
void ReadSwitchFile(void)
{
	INT8U iCount;
	  iCount = 0;
	  while(iCount < 5)
	   {
		ReadFromFlash(SWITCH_ADDR, Flash_Stream, 0x1000);
		xtm_memcopy(&SwitchInfo, Flash_Stream, sizeof(struct SwitchInfo1));
		iCount ++;
       }
}
//---------------------------------------------------------------------------
void WriteSwitchFile(void)
{
	xtm_memcopy(Flash_Stream, &SwitchInfo, sizeof(struct SwitchInfo1));
	SaveToFlash(SWITCH_ADDR, Flash_Stream, 0x1000);

}
//---------------------------------------------------------------------------

void ReadSerialFile(void)
{
	INT8U iCount;
	  iCount = 0;
	  while(iCount < 5)
	   {
		ReadFromFlash(0x0D7000, Flash_Stream, 0x1000);
		//xtm_memcopy(&SerialNo, Flash_Stream, sizeof(struct SerialNo1));
		iCount ++;
	    }
	  
}
//---------------------------------------------------------------------------
void ReadDefenceFile(void)
{
	INT8U iCount;
	  iCount = 0;
	  while(iCount < 5)
	  {
		ReadFromFlash(0x0D8000, Flash_Stream, 0x1000);
		//xtm_memcopy(&DefenceInfo, Flash_Stream, sizeof(struct DefenceInfo1));
		iCount ++;
	  }
}
//---------------------------------------------------------------------------
//写数据到Flash
void SaveToFlash(INT32U FlashAddr, INT8U *buf, INT16U length)
{
  INT16U i;
  INT32U paddr;
  //xtm_printf("SaveToFlash::FlashAddr = 0x%X %X, length = %d\n", (INT16U)((FlashAddr & 0xffff0000) >> 16), \
  //           (INT16U)(FlashAddr & 0xffff), length);
  for(i=0; i<length; i+=256)
   {
    paddr = FlashAddr + i;
    CheckFlashSecterErase(paddr);
    w25x32writeenable();
    programpage25x32(paddr, buf + i, 256);
    w25x32writedisable();
    //xtm_printf("paddr = 0x%X%X\n", (INT16U)((paddr & 0xffff0000) >> 16), \
    //         (INT16U)(paddr & 0xffff));
   }
}
//---------------------------------------------------------------------------
//从Flash读数据
void ReadFromFlash(INT32U FlashAddr, INT8U *buf, INT16U length)
{
  INT16U i;
  INT32U paddr;
  xtm_printf("ReadFromFlash::FlashAddr = 0x%X %X, length = %d\n", (INT16U)((FlashAddr & 0xffff0000) >> 16), \
             (INT16U)(FlashAddr & 0xffff), length);

  for(i=0; i<length; i+=256)
   {
    paddr = FlashAddr + i;
    xtm_read25x32data(paddr, buf + i, 256);
    //xtm_printf("paddr = 0x%X%X\n", (INT16U)((paddr & 0xffff0000) >> 16), \
    //         (INT16U)(paddr & 0xffff));
   }
}
//---------------------------------------------------------------------------
