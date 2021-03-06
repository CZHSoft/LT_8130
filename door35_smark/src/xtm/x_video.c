#include "includes.h"

#include "stdlib.h"

#include "main.h"

#define x_video_h

#define CommonH
#include "common.h"

#define _VINT_ENABLE  0       //VINT 使用, 可避免图像快速变化时的闪动现象

void InitVideo(void);    //20120203 xu

void interrupt MJPEGInt(void);

INT8U Mjpeg_DecodeError = 0;
void interrupt MJPEGDecInt(void);
void interrupt VInt(void);
INT16U bs_num_last;
INT16U bs_addr;

INT32U video_timestamp;     
INT16U video_Frameno;
void CreateVideoSend_Stream(void);
void send_video_data(INT32U video_addr, INT16U length);

INT8U video_encode_flag = 0;
INT32U video_encode_addr[2];
INT16U video_encode_length[2];
INT8U video_encode_no;
void CheckVideoEncodeBuf(void);


INT16U bsj_length;
INT8U have_mjpeg_stream;
INT8U dec_sa_sw;

INT8U  CaptureYUV = 0;

void StartRecVideo(INT8U Type);     //打开视频捕获
void StopRecVideo(void);            //关闭视频捕获

void StartEncodeVideo(INT8U Type);     //打开视频编码
void StopEncodeVideo(void);            //关闭视频编码

void StartVideoClock(void);     //打开视频采集编码时钟
void StopVideoClock(void);      //关闭视频采集编码时钟


INT8U Have_Video_Stream;
INT8U DecodeBuffNo; //解码缓冲编号
INT8U CurrBitStreamNo, OldCurrBitStreamNo;

void videow(INT16U Hstart,INT16U Hsize,INT16U Vstart,INT16U Vsize);
void clear_video_ram(void);  //清视频窗口

INT8U VideoFrameRate[6] = {1, 5, 10, 15, 20, 25};

INT16U  Mjpeg_Count = 0;

INT8U EncodeStartFlag = 0;
void CheckEn_J_Status(void);
INT8U En_J_Status_Count = 0;

void Reset_Param_Window(void);

extern INT8U TestEncodeErrorReset;
//---------------------------------------------------------------------------
void videow(INT16U Hstart,INT16U Hsize,INT16U Vstart,INT16U Vsize)
{
	outport(WH_START_L, Hstart);//水平起始 
	outport(WH_SIZE_L, Hsize);//水平线数
	outport(WV_START, Vstart);
	outport(WV_SIZE, Vsize);
}
//---------------------------------------------------------------------------
void clear_video_ram(void)
{
	INT16U i,j;
	INT16U color_val, bcolor_val;
	INT32U addr;
	addr=0UL;
	outportb(FONT_WR_MOST_ADD, addr>>16);
	outportb(FONT_WR_HIGH_ADD, addr>>8);
	outportb(FONT_WR_LOW_ADD, addr);
	color_val = 0x8080;
	bcolor_val = 0x8080;
	outportb(FGND_LOW, color_val);
	outportb(FGND_HIGH, (color_val>>8));
	outportb(BGND_LOW, bcolor_val);
	outportb(BGND_HIGH, (bcolor_val>>8));
	for(i=0;i<1024;i++)
	 {	 
	 	 for(j=0;j<128;j++)
	 	  {	 	
	 	  	outportb(FONT_WR_DATA, 0xff);
	 	  }
	 }
}
//---------------------------------------------------------------------------
void InitVideo(void)
{
  INT16U val;

     	setvect(16, MJPEGInt);
	outportb(TX_START, inportb(TX_START)|0x20);//使能MJPEG中断
	setvect(17, MJPEGDecInt);
	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x04);//使能MJPEG解码中断
      #if _VINT_ENABLE    //VINT 使用
	setvect(18, VInt);
        outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x20);//使能V中断
      #else
        outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xdf);//禁止V中断
      #endif


 outportb(INT_MASK_H, 0xe0);//高三个中断关闭，低五个中断使能 1：屏蔽 0:使能         20111209 jpegdec  0xf2 -> 0xf0     20120203 jpegdec  0xf0 -> 0xe0
 outportb(INT_EDGE_H, 0x00);//都是上升沿触发

	bs_num_last=0;
	bs_addr=0;

        Have_Video_Stream = 0;
        DecodeBuffNo = 0;
        Local.DecodeVideoWidth = 640;
        Local.DecodeVideoHeight = 480;
        CurrBitStreamNo = 0;

	have_mjpeg_stream=0;
	dec_sa_sw=0;
        Local.Test_OV7725FrameRate = 1;  //0 - 30   1 - 25
      #ifdef _CAMERA_SUPPORT     //如不用摄像头，GPIOB6、7为Comm1, GPIOC 作为8个防区使用
        SetGpio(GPIOD_OUT_DATA, CAMERA_POWER_HIGH, SET_HIGH); //开CAMERA
        DelayMs(100);
  	val = init_camera(Local.Test_OV7725FrameRate);
        SetGpio(GPIOD_OUT_DATA, CAMERA_POWER_LOW, SET_LOW); //关CAMERA    
        Local.Camera = 0;
        Local.VideoType = 0x00;
        if(val == 0x7725)
         {
          xtm_printf("init ov7725\n");
          Local.Camera = 1;
          if((Local.DecodeVideoWidth == 640)&&(Local.DecodeVideoHeight == 480))
            Local.VideoType = 0x40;
          else
            Local.VideoType = 0x08;
         }
        else
          xtm_printf("no 7725\n");
      #else
        Local.Camera = 0;
      #endif
        Local.DecodeStreamFlag = 0;  //正在解码流
        Local.DecodeJpegFlag = 0;  //正在解静态图片
  //StopRecVideo();         //关闭视频捕获
}
//---------------------------------------------------------------------------
void StartRecVideo(INT8U Type)   //打开视频捕获    0 - VGA  1 - QVGA
{
  INT16U val;
  xtm_printf("StartRecVideo::LocalCfg.VideoClosed = %d, Local.Camera = %d, EncodeStartFlag = %d\n", LocalCfg.VideoClosed, Local.Camera, EncodeStartFlag);
  if(EncodeStartFlag)
    return;
  Type = VIDEO_VGA;
  //Type = VIDEO_QVGA;
  if(!LocalCfg.VideoClosed) //视频关闭
    return;
    
  //if(Local.Camera != 0)
   {
    SetGpio(GPIOD_OUT_DATA, CAMERA_POWER_HIGH, SET_HIGH); //开CAMERA
    DelayMs(50);
    val = init_camera(Local.Test_OV7725FrameRate);
    if(val == 0x7725)
      xtm_printf("init ov7725\n");
    else
     {
      xtm_printf("no 7725\n");
      return;
     }

   #ifdef _NIGHT_CAMERA_ADJUST_SUPPORT   //光暗CAMERA自动调节支持
    Check_Night_IO();
   #endif     

    CreateVideoSend_Stream();
    CaptureYUV = 1;

    video_timestamp = 0;
    video_Frameno = 0;
    if(Type == VIDEO_QVGA)
     {
      outportb(VI_CONTROL, 0x43);
      outport(VW_WIDTH_L, 512);//必须为512     }
    if(Type == VIDEO_VGA)
     {
      outportb(VI_CONTROL, 0x11);
      outport(VW_WIDTH_L, 640);
     }
    xtm_printf("StartRecVideo::1 VI_CONTROL2 = 0x%X, VI_STATUS = 0x%X\n", inportb(VI_CONTROL2), inportb(VI_STATUS));
    outportb(VI_CONTROL2, 0x4);    //视频输入控制寄存器2
    xtm_printf("StartRecVideo::2 VI_CONTROL2 = 0x%X, VI_STATUS = 0x%X\n", inportb(VI_CONTROL2), inportb(VI_STATUS));
   }
}
//---------------------------------------------------------------------------
void StopRecVideo(void)         //关闭视频捕获
{
  if(Local.Camera != 0)
   {
    xtm_printf("StopRecVideo::1 VI_CONTROL2 = 0x%X, VI_STATUS = 0x%X\n", inportb(VI_CONTROL2), inportb(VI_STATUS));
    outportb(VI_CONTROL, 0x00);
    xtm_printf("StopRecVideo::2 VI_CONTROL2 = 0x%X, VI_STATUS = 0x%X\n", inportb(VI_CONTROL2), inportb(VI_STATUS));
    SetGpio(GPIOD_OUT_DATA, CAMERA_POWER_LOW, SET_LOW); //关CAMERA    
   }
}
//---------------------------------------------------------------------------
void StartEncodeVideo(INT8U Type)     //打开视频编码     0 - VGA  1 - QVGA
{
  if(!LocalCfg.VideoClosed) //视频关闭
    return;

  Type = VIDEO_VGA;
  //Type = VIDEO_QVGA;
  xtm_printf("StartEncodeVideo::TX_START = 0x%X, Local.Camera = %d, EncodeStartFlag = %d\n", inportb(TX_START), Local.Camera, EncodeStartFlag);
  if(EncodeStartFlag)
    return;
  Mjpeg_Count = 0;
  if(Local.Camera != 0)
   {
    EncodeStartFlag = 1;
    if((inportb(EN_J_STATUS)&0x01)==0x01)
      xtm_printf("StartEncodeVideo::EN_J_STATUS = 0x%X\n", inportb(EN_J_STATUS));

    setvect(16, MJPEGInt);
    outportb(TX_START, inportb(TX_START)|0x20);//使能MJPEG中断

//  outportb(EN_J_CONTROL2, 0x5e);//原图像30帧每秒，编码1帧每秒     0101 1110
//  outportb(EN_J_CONTROL2, 0x4A);//原图像30帧每秒，编码15帧每秒     0100 1111
//  outportb(EN_J_CONTROL2, 0x0f);//原图像30帧每秒，编码15帧每秒     0001 1001      0x19              VideoFrameRate
    if(LocalCfg.EncodeFrameRate > 5)
      LocalCfg.EncodeFrameRate = 3;
    xtm_printf("LocalCfg.EncodeFrameRate = %d, %d, Type = %d\n", LocalCfg.EncodeFrameRate, VideoFrameRate[LocalCfg.EncodeFrameRate], Type);
    outportb(EN_J_CONTROL2, VideoFrameRate[LocalCfg.EncodeFrameRate]);//原图像30帧每秒，编码15帧每秒     0001 1001      0x19
    //outportb(EN_J_CONTROL2, 0x40);      //30帧 自动帧率
    //outportb(EN_J_CONTROL2, 0x5e);

    if(Type == VIDEO_QVGA)
     {
      outportb(EN_J_MBWIDTH, 0x14);
      outportb(EN_J_MBHEIGHT, 0x0F);
     }
    if(Type == VIDEO_VGA)
     {
      outportb(EN_J_MBWIDTH, 0x28);
      outportb(EN_J_MBHEIGHT, 0x1e);
     }

     outportb(EN_J_CONTROL, inportb(EN_J_CONTROL)|0x17); //0000 0111     0x07
    }    
}
//---------------------------------------------------------------------------
void StopEncodeVideo(void)            //关闭视频编码
{
  INT8U tmp;
  outportb(TX_START, inportb(TX_START)&0xdf);//禁止MJPEG中断
  xtm_printf("StopEncodeVideo1::TX_START = 0x%X\n", inportb(TX_START));

  outportb(EN_J_CONTROL, inportb(EN_J_CONTROL)&0xfe);

  WaitDecodeFinish();   //20130513
  
  tmp = 0;
  while((inportb(EN_J_STATUS)&0x01)==0x01)
   {
    xtm_printf("StopEncodeVideo::wait EN_J_STATUS finish\n");
    DelayMs(1);
    tmp ++;
    if(tmp > 120)
     {
      if(Local.OldEncodeErrorNum == LocalCfg.EncodeErrorNum)
       {
        LocalCfg.EncodeErrorNum ++;                      //编码错误数目
        WriteCfgFile();
        Local.GpioWatchDogStarted = 0;
        SetGpio(GPIOD_OUT_DATA, IO5_LOW, SET_LOW); //IO5 低
       }
      break;
     }
   }
   
  EncodeStartFlag = 0;
  xtm_printf("StopEncodeVideo2::TX_START = 0x%X\n", inportb(TX_START));
}
//---------------------------------------------------------------------------
void CheckEn_J_Status(void)
{
#if 0  //某些机器有可能判断失误而引起重启
  INT8U i;
  INT16U Max_Count;
  if(EncodeStartFlag == 1)
   {
    if(Remote.DenNum > 1)
      return;
    if(Remote.DenNum == 1)
      Max_Count = 16;
    else
      Max_Count = 16;
    if(TestEncodeErrorReset == 0)
     {
      if((inportb(EN_J_STATUS)&0x01)==0x01)
        En_J_Status_Count ++;
      else
        En_J_Status_Count = 0;
     }
    else
      En_J_Status_Count = Max_Count;
    if(En_J_Status_Count >= Max_Count)
     {
      xtm_printf("CheckGpio::En_J_Status_Count = %d, Reset machine, TestEncodeErrorReset = %d\n", En_J_Status_Count, TestEncodeErrorReset); 
      if(Local.GpioWatchDogStarted == 0)
        return;

     #if 0
      StopRecVideo();
      StopEncodeVideo();
      StopVideoClock();            //关闭视频采集编码时钟
      Local.Status = 0;  //状态为空闲
     #else
      if(Local.OldEncodeErrorNum == LocalCfg.EncodeErrorNum)
       {
        LocalCfg.EncodeErrorNum ++;                      //编码错误数目
       }
      LocalCfg.ResetStatus = Local.Status;
      LocalCfg.Reset_DenNum = Remote.DenNum;
      for(i=0; i<LocalCfg.Reset_DenNum; i++)
       {
        xtm_memcopy(LocalCfg.Reset_IP[i], Remote.IP[i], 4);     //对方IP
        xtm_memcopy(LocalCfg.Reset_Mac[i], Remote.Mac[i], 6);   //对方Mac
        xtm_memcopy(LocalCfg.Reset_Addr[i], Remote.Addr[i], 20);
       }
      WriteCfgFile();
      Local.GpioWatchDogStarted = 0;
      SetGpio(GPIOD_OUT_DATA, IO5_LOW, SET_LOW); //IO5 低
     #endif     
     }
   }
  else
    En_J_Status_Count = 0;
#endif
}
//---------------------------------------------------------------------------
void Reset_Param_Window(void)
{
  INT8U i;
  Local.Status = LocalCfg.ResetStatus;
  if(Local.Status > 6)
    Local.Status = 0;

  Remote.DenNum = LocalCfg.Reset_DenNum ;
  if(Remote.DenNum > 4)
    Remote.DenNum = 0;
  for(i=0; i<Remote.DenNum; i++)
   {
    xtm_memcopy(Remote.IP[i], LocalCfg.Reset_IP[i], 4);     //对方IP
    xtm_memcopy(Remote.Mac[i], LocalCfg.Reset_Mac[i],6);   //对方Mac
    xtm_memcopy(Remote.Addr[i], LocalCfg.Reset_Addr[i], 20);

    RefreshArp_Table(LocalCfg.Reset_Mac[i], LocalCfg.Reset_IP[i]); //更新ARP缓存表
   }
  xtm_printf("Reset_Param_Window::Local.Status = %d, Remote.DenNum = %d\n", Local.Status, Remote.DenNum);

  LocalCfg.ResetStatus = 0;
  WriteCfgFile();
  
  switch(Local.Status)
   {
    case 1: //本机主叫
            DisplayTalkConnectWindow();

            Local.TalkEndStatus = 1;
            Local.CallConfirmFlag = 1; //设置在线标志
            Local.Timer1Num = 0;
            Local.TimeOut = 0;       //监视超时,  通话超时,  呼叫超时，无人接听
            Local.OnlineNum = 0;     //在线确认序号
            Local.OnlineFlag = 1;
            xtm_printf("Local.Status = %d\n", Local.Status);

            LocalCfg.EncodeFrameSize = VIDEO_VGA; //编码大小

            //开始录制视频
            StartVideoClock();     //打开视频采集编码时钟
            StartRecVideo(LocalCfg.EncodeFrameSize);
            StartEncodeVideo(LocalCfg.EncodeFrameSize);     //打开视频编码

            CreateAudioSend_Stream();      //音视频发送Stream

            PlayBackRingWav();
           break;
    case 4: //本机被监视
           DisplayWatchWindow(REFRESH);
           Local.CallConfirmFlag = 1; //设置在线标志
           Local.Timer1Num = 0;
           Local.TimeOut = 0;       //监视超时,  通话超时,  呼叫超时，无人接听
           Local.OnlineNum = 0;     //在线确认序号
           Local.OnlineFlag = 1;

           LocalCfg.EncodeFrameSize = VIDEO_VGA; //编码大小

           //开始录制视频
           StartVideoClock();     //打开视频采集编码时钟
           StartRecVideo(LocalCfg.EncodeFrameSize);
           StartEncodeVideo(LocalCfg.EncodeFrameSize);     //打开视频编码

           CreateAudioSend_Stream();      //音视频发送Stream
           break;
    case 5: //本机主叫通话
            DisplayTalkStartWindow();
            Local.TalkEndStatus = 1;
            Local.CallConfirmFlag = 1; //设置在线标志
            Local.Timer1Num = 0;
            Local.TimeOut = 0;       //监视超时,  通话超时,  呼叫超时，无人接听
            Local.OnlineNum = 0;     //在线确认序号
            Local.OnlineFlag = 1;
             //20140225
            //TALKTIMEOUT = DelayTalkTimeOut[LocalCfg.TalkTimeOut]*TIMERPERSEC; //通话最长时间
            Local.TalkTimeOut = TALKTIMEOUT; //通话最长时间
            
            xtm_printf("Local.Status = %d\n", Local.Status);

            LocalCfg.EncodeFrameSize = VIDEO_VGA; //编码大小

            //开始录制视频
            StartVideoClock();     //打开视频采集编码时钟
            StartRecVideo(LocalCfg.EncodeFrameSize);
            StartEncodeVideo(LocalCfg.EncodeFrameSize);     //打开视频编码

            CreateAudioSend_Stream();      //音视频发送Stream

            StartRecAudio();
            StartPlayAudio();
           break;
    default:
           Local.Status = 0;
           break;
   }
}
//---------------------------------------------------------------------------
void StartVideoClock(void)     //打开视频采集编码时钟
{
#if 1
  if(EncodeStartFlag)
    return;
  //outportb(CONTROL_REG2, inportb(CONTROL_REG2) | 0x02);      //从显示处 移一部分带宽到编解码, 这样处理后会增加 BitBlk 错误

  Set_VideoCapture_Clock(1);  //操作视频采集时钟  1  开   0  关
  Set_Encode_Clock(1); //操作视频编码时钟  1  开   0  关
  encode_dqt_init();
#endif  
}
//---------------------------------------------------------------------------
void StopVideoClock(void)            //关闭视频采集编码时钟
{
#if 1
  INT8U tmp;
  //while((inportb(VI_STATUS)&0x01)==0x00)
  //  xtm_printf("StopVideoClock::wait VI_STATUS finish\n");
  tmp = 0;
/*  while((inportb(EN_J_STATUS)&0x01)==0x01)
   {
    xtm_printf("StopEncodeVideo::wait EN_J_STATUS finish\n");
    tmp ++;
    if(tmp > 20)
      break;
   }    */
  Set_Encode_Clock(0); //操作视频编码时钟  1  开   0  关
  Set_VideoCapture_Clock(0);  //操作视频采集时钟  1  开   0  关

  //outportb(CONTROL_REG2, inportb(CONTROL_REG2) & 0xfd);    //视频输入控制寄存器2
#endif
}
//---------------------------------------------------------------------------
void interrupt MJPEGInt()
{

	INT16U i, j, k, l;
	INT8U bs_counter, len_overflow, most_addr;
	INT16U bs_length;
	INT8U picture_counter;
	INT16U bs_addr_temp;
        INT32U bs_addr_h;

	outportb(TX_START, inportb(TX_START)&0xdf);//清除MJPEG中断

      /*  Mjpeg_Count ++;
        if((Mjpeg_Count % 100) == 0)
          xtm_printf("MJPEGInt::Mjpeg_Count = %d\n", Mjpeg_Count);       */

	k=inport(EN_J_TX_BS_PACK_L);
	//asm sti

	bs_counter=k-bs_num_last;
	bs_counter=bs_counter&0xf;

        //xtm_printf("MJPEGInt 1::EN_J_CONTROL = 0x%X, Local.Test_EncodeWatchFlag = %d\n", inportb(EN_J_CONTROL), Local.Test_EncodeWatchFlag);
        //xtm_printf("MJPEGInt 1::EN_J_CONTROL2 = 0x%X\n", inportb(EN_J_CONTROL2));
	k=bs_num_last;

	for(j=0;j<bs_counter;j++){


                bs_addr = 0;

                if((inportb(EN_J_STATUS)&0x04)==0x04)
                 {
                        bs_addr_h = 0x170000;
                        bs_addr = 0;
                        video_encode_no = 0;

                        most_addr = 0x17;
                      //  outportb(BYTE_RD_MOST_ADD, 0x17);
                 }	
                else
                 {
                 	bs_addr_h = 0x180000;
                 	bs_addr = 0x0;
                 	most_addr = 0x18;
                 	video_encode_no = 1;
                 }
               /* len_overflow = 0;
                if((inportb(EN_J_STATUS)&0x08) == 0x08)
                 {
                  len_overflow = 1;
                  xtm_printf("MJPEGInt::1111 len_overflow \n");
                 }        */
                //xtm_printf("MJPEGInt::1111\n");
                outportb(BYTE_RD_MOST_ADD, most_addr);
                outportb(BYTE_RD_HIGH_ADD, bs_addr>>8);
                outportb(BYTE_RD_LOW_ADD, bs_addr);
                bs_length=inportb(BYTE_RD_DATA);
                bs_length=bs_length+(inportb(BYTE_RD_DATA)<<8)-16;

                inportb(BYTE_RD_DATA);
                inportb(BYTE_RD_DATA);
                inportb(BYTE_RD_DATA);
                inportb(BYTE_RD_DATA);
                len_overflow = inportb(BYTE_RD_DATA);

                if(len_overflow & 0x01)
                  xtm_printf("MJPEGInt:: len_overflow = 0x%X\n", len_overflow);
                else
                 {
                    if(Local.Test_EncodeWatchFlag == 1)
                     {
                     }
                    else
                     {
                      //xtm_printf("MJPEGInt2::bs_length = 0x%X\n", bs_length);
                      if(bs_length > 0xff00)
                        xtm_printf("MJPEGInt2::bs_length = 0x%X\n", bs_length);
                      else
                      #if 1
                       //xtm_printf("dd = %d\n", (INT16U)((SWTIMER_Tick() * 5) & 0x7fff));
                       if(video_encode_flag == 0)
                        {
                         video_encode_addr[video_encode_no] = bs_addr_h + bs_addr + 16;
                         video_encode_length[video_encode_no] = bs_length;
                         video_encode_flag = 1;
                        }
                      #else
                       send_video_data(bs_addr_h + bs_addr + 16, bs_length);
                      #endif
                     }
		k++;
               }

	}
	bs_num_last=k;

	outportb(TX_START, inportb(TX_START)|0x20);//使能MJPEG中断
        //xtm_printf("MJPEGInt 222\n");
        //DisplayImage(&main_image);
}

void CreateVideoSend_Stream(void)      //音频发送Stream
{
    INT16 i, DestPort;
    struct talkdata1 talkdata;

    if(Local.Status == 90)
        {
    DestPort = LocalPort[4];
        }
    else
        {
        DestPort = LocalPort[1];
        }
    //==========
    //MAC header
    //==========
    /* 0 -- 5  目标MAC地址*/
    xtm_memcopy(send_mac_buf2 + 6, LocalCfg.Mac_Addr, 6);
    send_mac_buf2[12] = 0x08;
    send_mac_buf2[13] = 0x00;

    //=========
    //IP header
    //=========
    send_mac_buf2[14] = 0x45;          //Ver HLEN
    send_mac_buf2[15] = 0x00;
    i = 86 + PACKDATALEN + 28;//nlength+28;
    send_mac_buf2[16] = i>>8;
    send_mac_buf2[17] = i&0xff;
    /* 18 -- 19  IP首部标识*/

    send_mac_buf2[20] = 0x00;
    send_mac_buf2[21] = 0x00;
    send_mac_buf2[22] = 0x40;
    send_mac_buf2[23] = 0x11;
    send_mac_buf2[24] = 0x00;       //checksum
    send_mac_buf2[25] = 0x00;
    xtm_memcopy(send_mac_buf2 + 26, LocalCfg.IP, 4);     //Source IP
    /* 30 -- 33  Dest IP*/
    send_mac_buf2[34] = (DestPort >> 8)& 0xff;
    send_mac_buf2[35] = DestPort & 0xff;
    send_mac_buf2[36] = (DestPort >> 8)& 0xff;
    send_mac_buf2[37] = DestPort & 0xff;

    i = 86 + PACKDATALEN + 8;//(nlength+8);
    send_mac_buf2[38] = i>>8;
    send_mac_buf2[39] = i&0xff;
    send_mac_buf2[40] = 0x00;
    send_mac_buf2[41] = 0x00;

    //头部
    if(Local.Status == 90)
        {
    
            xtm_memcopy(VideoSend_Stream, UdpPackageHead2, 4);
        VideoSend_Stream[4]=8;
        VideoSend_Stream[5]=90;
        }
    else
        {
        xtm_memcopy(VideoSend_Stream, UdpPackageHead, 6);

        }
    //命令
    VideoSend_Stream[6] = VIDEOTALK;
    if((Local.Status == 3)||(Local.Status == 4)||(Local.Status == 90))
      VideoSend_Stream[6] = VIDEOWATCH;

    VideoSend_Stream[7]= ASK;        //主叫
    if((Local.Status == 1)||(Local.Status == 3)||(Local.Status == 5))  //本机为主叫方
     {
      VideoSend_Stream[8] = CALLUP;
      xtm_memcopy(talkdata.HostAddr, LocalCfg.Addr, 20);
      xtm_memcopy(talkdata.HostIP, LocalCfg.IP, 4);
      xtm_memcopy(talkdata.AssiAddr, Remote.Addr[0], 20);
      xtm_memcopy(talkdata.AssiIP, Remote.IP[0], 4);
     }
    if((Local.Status == 2)||(Local.Status == 4)||(Local.Status == 6)||(Local.Status == 90))  //本机为被叫方
     {
      VideoSend_Stream[8] = CALLDOWN;
      xtm_memcopy(talkdata.HostAddr, Remote.Addr[0], 20);
      xtm_memcopy(talkdata.HostIP, Remote.IP[0], 4);
      xtm_memcopy(talkdata.AssiAddr, LocalCfg.Addr, 20);
      xtm_memcopy(talkdata.AssiIP, LocalCfg.IP, 4);
     }

    //时间戳
    talkdata.timestamp = 0;
      //数据类型
    talkdata.DataType = 2;
    //帧序号
    talkdata.Frameno = 0;
    //帧数据长度
    talkdata.Framelen = 0;
    //总包数
    talkdata.TotalPackage = 1;
    //当前包
    talkdata.CurrPackage = 1;
    //数据长度
    talkdata.Datalen = 0;
    talkdata.PackLen = PACKDATALEN;
    xtm_memcopy(VideoSend_Stream + 9, &talkdata, sizeof(talkdata));

    xtm_printf("CreateVideoSend_Stream::Local.Status = %d, Remote.IP[0] = %d.%d.%d.%d, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, VideoSend_Stream[8] = 0x%X\n", \
           Local.Status, Remote.IP[0][0], Remote.IP[0][1], Remote.IP[0][2], Remote.IP[0][3], \
           VideoSend_Stream[0], VideoSend_Stream[1], VideoSend_Stream[2], VideoSend_Stream[3], VideoSend_Stream[4], VideoSend_Stream[5], VideoSend_Stream[6], VideoSend_Stream[7], VideoSend_Stream[8]);
}
//---------------------------------------------------------------------------
void CheckVideoEncodeBuf(void)
{
  INT32U  Maddrs;
  if(video_encode_flag == 1)
   {
     video_encode_flag = 0;
    #ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
     if(CapturePic.RecordPic == 1)
      {
       //存储第50帧
       CapturePic.IFrameCount ++;
       if(CapturePic.IFrameCount > CapturePic.IFrameNo)
        {
         outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
         outportb(MEMORY_WINDOW_BASE_H, 0x6f);
         xtm_printf("CheckVideoEncodeBuf::1, len = 0x%X\n", video_encode_length[video_encode_no]);
         Maddrs = (((INT32U)(Memory6_Stream)&0xffff)+(((INT32U)(Memory6_Stream)&0xffff0000)>>12))|0x700000;
         //fmem_fmem_dma(video_encode_addr[video_encode_no], Maddrs, video_encode_length[video_encode_no]);
         fmem_insb(video_encode_addr[video_encode_no], Memory6_Stream, video_encode_length[video_encode_no]);
         xtm_printf("CheckVideoEncodeBuf::2\n");
         //xtm_memcopy(Memory6_Stream, mjpeg_stream[Curr_Mjpeg_Stream], mjpeg_stream_len[Curr_Mjpeg_Stream]);
         outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);

         CapturePic.Pic_Width = 640;
         CapturePic.Pic_Height = 480;
         CapturePic.IFrameCount = 0;
         CapturePic.RecordPic = 0;
         CapturePic.FileLen = video_encode_length[video_encode_no] + 0x26F + 2;
         CapturePic.HavePicRecorded = 1;  //有照片已录制
         xtm_printf("capture talk pic\n");
        }
      }
    #endif     
     send_video_data(video_encode_addr[video_encode_no], video_encode_length[video_encode_no]);
   }
}
//---------------------------------------------------------------------------
void send_video_data(INT32U video_addr, INT16U v_length)
{
  INT16U  nlength, Datalen;
  
  //xtm_printf("dd = %d\n", (INT16U)((SWTIMER_Tick() * 5) & 0x7fff));
   //xtm_printf("send_video_data::Remote.DenMac = %X:%X:%X:%X:%X:%X\n", Remote.DenMac[0], Remote.DenMac[1], Remote.DenMac[2], Remote.DenMac[3], Remote.DenMac[4], Remote.DenMac[5]);
  //if(SearchArp_Table(Remote.DenIP, RemoteMac))
   {
  INT16U i, j, TotalPackage;

/*  Remote.DenNum = 2;
  Remote.IP[1][0] = 192;
  Remote.IP[1][1] = 168;
  Remote.IP[1][2] = 68;
  Remote.IP[1][3] = 98;
  Remote.Mac[1][0] = 0x00;
  Remote.Mac[1][1] = 0x26;
  Remote.Mac[1][2] = 0x18;
  Remote.Mac[1][3] = 0x77;
  Remote.Mac[1][4] = 0x4C;
  Remote.Mac[1][5] = 0x44;       */

  if((send_mac_buf2[34] == 0)&&(send_mac_buf2[35] == 0))
    xtm_printf("send_video_data::11111111\n");
  if((send_mac_buf2[36] == 0)&&(send_mac_buf2[37] == 0))
    xtm_printf("send_video_data::11111112\n");
  if(Local.Status==90)
    {
  send_mac_buf2[34] = (LocalPort[4] >> 8)& 0xff;
  send_mac_buf2[35] = LocalPort[4] & 0xff;
    }
  else
    {
    send_mac_buf2[34] = (LocalPort[1] >> 8)& 0xff;
  send_mac_buf2[35] = LocalPort[1] & 0xff;
    }
  nlength = v_length + 86;
  //UDP 数据必须为偶数字节
  if((nlength % 2) == 1)
    nlength ++;

  if(nlength < SMALLESTSIZE)
    nlength = SMALLESTSIZE;

  send_mac_buf2[18]= (ip_pack_no >> 8) & 0xff;
  send_mac_buf2[19]= ip_pack_no & 0xff;   //IP首部标识

    //时间戳
    xtm_memcopy(VideoSend_Stream + 57, &video_timestamp, sizeof(INT32U));
    //数据类型
    VideoSend_Stream[61] = 2;
    VideoSend_Stream[62] = 0;
    //帧序号
    xtm_memcopy(VideoSend_Stream + 63, &video_Frameno, sizeof(INT16U));
    //帧数据长度
    VideoSend_Stream[65] = v_length & 0xff;
    VideoSend_Stream[66] = v_length >> 8;
    VideoSend_Stream[67] = 0;
    VideoSend_Stream[68] = 0;

    //总包数
    if((v_length % PACKDATALEN)==0)
      TotalPackage = v_length/PACKDATALEN;
    else
      TotalPackage = v_length/PACKDATALEN + 1;    
    VideoSend_Stream[69] = TotalPackage;
    VideoSend_Stream[70] = 0;
    //当前包
    VideoSend_Stream[71] = 1;
    VideoSend_Stream[72] = 0;

    xtm_memcopy(send_mac_buf2 + 42, VideoSend_Stream, 86);

    for(i=1; i<=TotalPackage; i++)
     {
        //当前包
        send_mac_buf2[113] = i;
        send_mac_buf2[114] = 0;
        if(i==TotalPackage)
         {
          ///数据长度
          Datalen = v_length-(i-1)*PACKDATALEN;
          send_mac_buf2[115] = Datalen & 0xff;
          send_mac_buf2[116] = Datalen >> 8;

          //xtm_printf("send_rec_audio_data::PCMI_INT_Flag = %d\n", PCMI_INT_Flag);
          Video_Send_Flag = 1;
          for(j=0; j<Remote.DenNum; j++)
           {
            xtm_memcopy(send_mac_buf2, Remote.Mac[j], 6);
           #if 1   //此处会影响 send_mac_buf2[34]、send_mac_buf2[35]?
            xtm_memcopy(send_mac_buf2 + 30, Remote.IP[j], 4);
           #else
            send_mac_buf2[30] = Remote.IP[j][0];
            send_mac_buf2[31] = Remote.IP[j][1];
            send_mac_buf2[32] = Remote.IP[j][2];
            send_mac_buf2[33] = Remote.IP[j][3];
           #endif
            send_mac_buf2[24] = 0x00;       //checksum
            send_mac_buf2[25] = 0x00;
            Build_Udp_Ip_Crc(send_mac_buf2 + 14);

            //xtm_printf("send_video_data::crc = 0x%X 0x%X\n", send_mac_buf2[24], send_mac_buf2[25]);
            if((Local.Status == 1)||(Local.Status == 3)||(Local.Status == 5))  //本机为主叫方
             {
              xtm_memcopy(send_mac_buf2 + 42 + 33, Remote.Addr[j], 20);
              xtm_memcopy(send_mac_buf2 + 42 + 53, Remote.IP[j], 4);
             }
            if((Local.Status == 2)||(Local.Status == 4)||(Local.Status == 6)||(Local.Status == 90))  //本机为被叫方
             {
              xtm_memcopy(send_mac_buf2 + 42 + 9, Remote.Addr[j], 20);
              xtm_memcopy(send_mac_buf2 + 42 + 29, Remote.IP[j], 4);
             }
            send_mac_video_packet(128, send_mac_buf2, PACKDATALEN, video_addr + (i-1)*PACKDATALEN);
           }
          Video_Send_Flag = 0;
         }
        else
         {

          ///数据长度
          send_mac_buf2[115] = PACKDATALEN & 0xff;
          send_mac_buf2[116] = PACKDATALEN >> 8;

          //xtm_printf("send_rec_audio_data::PCMI_INT_Flag = %d\n", PCMI_INT_Flag);
          Video_Send_Flag = 1;
          for(j=0; j<Remote.DenNum; j++)
           {
            xtm_memcopy(send_mac_buf2, Remote.Mac[j], 6);
           #if 1   //此处会影响 send_mac_buf2[34]、send_mac_buf2[35]?
            xtm_memcopy(send_mac_buf2 + 30, Remote.IP[j], 4);
           #else
            send_mac_buf2[30] = Remote.IP[j][0];
            send_mac_buf2[31] = Remote.IP[j][1];
            send_mac_buf2[32] = Remote.IP[j][2];
            send_mac_buf2[33] = Remote.IP[j][3];
           #endif

            send_mac_buf2[24] = 0x00;       //checksum
            send_mac_buf2[25] = 0x00;

            Build_Udp_Ip_Crc(send_mac_buf2 + 14);
            //xtm_printf("%d.%d.%d.%d, %X:%X:%X:%X:%X:%X\n", Remote.IP[j][0], Remote.IP[j][1], Remote.IP[j][2], Remote.IP[j][3], \
            //            Remote.Mac[j][0], Remote.Mac[j][1], Remote.Mac[j][2], Remote.Mac[j][3], Remote.Mac[j][4], Remote.Mac[j][5]);
            if((Local.Status == 1)||(Local.Status == 3)||(Local.Status == 5))  //本机为主叫方
             {
              xtm_memcopy(send_mac_buf2 + 42 + 33, Remote.Addr[j], 20);
              xtm_memcopy(send_mac_buf2 + 42 + 53, Remote.IP[j], 4);
             }
            if((Local.Status == 2)||(Local.Status == 4)||(Local.Status == 6)||(Local.Status == 90))  //本机为被叫方
             {
              xtm_memcopy(send_mac_buf2 + 42 + 9, Remote.Addr[j], 20);
              xtm_memcopy(send_mac_buf2 + 42 + 29, Remote.IP[j], 4);
             }            
            send_mac_video_packet(128, send_mac_buf2, PACKDATALEN, video_addr + (i-1)*PACKDATALEN);
           }
          Video_Send_Flag = 0;
         }
     }
    //xtm_printf("send_rec_audio_data::Remote.DenIP = %d.%d.%d.%d, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, VideoSend_Stream[8] = 0x%X\n", \
    //       Remote.DenIP[0], Remote.DenIP[1], Remote.DenIP[2], Remote.DenIP[3], \
    //       VideoSend_Stream[0], VideoSend_Stream[1], VideoSend_Stream[2], VideoSend_Stream[3], VideoSend_Stream[4], VideoSend_Stream[5], VideoSend_Stream[6], VideoSend_Stream[7], VideoSend_Stream[8]);

    video_timestamp += 40;      //音视频发送Stream
    video_Frameno ++;
   }
}
void interrupt VInt()
{
#if _VINT_ENABLE    //VINT 使用
  //20120229
  INT32U tmp_data;
  //xtm_printf("VInt\n");

  outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xdf);//清除V中断
  if(Have_Video_Stream)
   {
    //xtm_printf("VInt::Have_Video_Stream= %d, 0x%X\n", Have_Video_Stream, inportb(DE_J_STATUS));
    if((inportb(DE_J_STATUS)&0x01)!=0x01)
     {
      outportb(VW_CONTROL, 0x03);
      outport(VW_WIDTH_L, Local.DecodeVideoWidth);

      //xtm_printf("VInt::\n");
      if(DecodeBuffNo)
       {
        tmp_data = (INT32U)Local.DecodeVideoWidth*Local.DecodeVideoHeight*2;
        DecodeBuffNo = 0;
       }
      else
       {
        tmp_data = 0;
        DecodeBuffNo = 1;
       }

      outportb(VW_SA_H, tmp_data>>16);
      outportb(VW_SA_M, tmp_data>>8);
      outportb(VW_SA_L, tmp_data);
     }
    Have_Video_Stream = 0;
    //xtm_printf("VInt2::Have_Video_Stream= %d\n", Have_Video_Stream);
   }
  outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x20);//使能V中断
#endif  
}

void interrupt MJPEGDecInt()
{
        INT8U tmp_status;
        INT16U i, j, val, tmp_pos;
        Mjpeg_DecodeError = 0;
        tmp_status = inportb(DE_J_STATUS);
        outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xfb);//清除MJPEG解码中断
        //xtm_printf("1\n");
        //xtm_printf("MJPEGDecInt: DE_J_STATUS = 0x%X\n", inportb(DE_J_STATUS));

	if((tmp_status&0x04)==0x04){
                Mjpeg_DecodeError = 1;
              if((Local.Test_EncodeWatchFlag == 0)&&(Local.Status != 0))
                xtm_printf("%d\n", video_decode_no);
                //xtm_printf("MJPEGDecInt:: decode error ,,,,,,,,,,,%d, Mjpeg_DecodeError = %d, %d, %d, old_mjpeg_stream_no = %d, len = 0x%X, encode 0 = 0x%X, 1 = 0x%X\n",  \
                   //video_decode_no, Mjpeg_DecodeError, mjpeg_stream[old_mjpeg_stream_no][0], mjpeg_stream[old_mjpeg_stream_no][1], old_mjpeg_stream_no, mjpeg_stream_len[old_mjpeg_stream_no], video_encode_length[0], video_encode_length[1]);
              else
                xtm_printf("MJPEGDecInt::decode error  bitstream_image[%d].Maddr = 0x%X %X, bs_length = 0x%X\n", \
                          OldCurrBitStreamNo, (INT16U)((bitstream_image[OldCurrBitStreamNo].Maddr & 0xffff0000) >> 16), (INT16U)(bitstream_image[OldCurrBitStreamNo].Maddr & 0xffff), bitstream_image[OldCurrBitStreamNo].Len);
              error_image.Maddr = bitstream_image[OldCurrBitStreamNo].Maddr;
              error_image.Len = bitstream_image[OldCurrBitStreamNo].Len;
              outportb(CONTROL_REG, inportb(CONTROL_REG)&0x7f);//MJPEG解码清零
              outportb(CONTROL_REG, inportb(CONTROL_REG)|0x80);//MJPEG解码复原

              Have_Video_Stream = 0;
	}
       else
         Have_Video_Stream = 1;

     outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x04);//使能MJPEG解码中断

     //if((inportb(0xfb1e) != 0x00)||(inportb(0xfb1f) != 0x00))
     //  xtm_printf("ddddddddddddddddddddddddddddddddddddddddddddddddssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss\n");
     //xtm_printf("MJPEGDecInt: DE_J_STATUS = 0x%X, 0xfb1e = 0x%X, 0xfb1f = 0x%X\n", tmp_status, inportb(0xfb1e), inportb(0xfb1f));
}
//---------------------------------------------------------------------------
