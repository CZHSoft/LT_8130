#include "includes.h"

#include "stdlib.h"

#define x_audio_h

#define CommonH
#include "common.h"

#define _REC_NO  1   //录音缓冲区 No
INT16U BlockLen;

void Pcm_Enable(uchar uFlag);    //20120305 xu
void Audio_PowerOn(void);
void Audio_PowerDown(void);

void InitPcm(void);    //20120104 xu
void interrupt PCMInt(void);
void interrupt PCMOInt(void);
void interrupt PCMPInt(void);

INT8U AudioRecIsStart = 0;
INT8U AudioPlayIsStart = 0;
INT8U WavRecFlag = 0;
INT8U WavPlayFlag = 0;

INT32U audio_timestamp;      //音视频发送Stream
INT16U Audio_Frameno;
void CreateAudioSend_Stream(void);

void StartRecAudio(void);
void StopRecAudio(void);
void StartPlayAudio(void);
void StopPlayAudio(void);

void CheckPlayAudioBuf(void);

INT32U WavFileLen = 0;
INT32U RecWavFileLen = 0;
INT8U RecWavPlayFlag = 0;
void StartPlayWav(INT32U FlashAddr, INT8U PlayFlag); //0 单次播放  1 循环播放
void StartPlayWav_Rec(INT32U FlashAddr, INT8U PlayFlag); //0 单次播放  1 循环播放
void StopPlayWavFile(void);
void WaitAudioUnuse(INT16U delayms); //等待声卡空闲

void StartRecWav(void);
void StopRecWav(void);

//读取 wav 文件
INT8U ReadWavFile(INT32U FlashAddr);


INT8U pcmi_bn;
INT8U pcmo_bn;   //pcm 输出块位置
void CheckRecAudioBuf(void);     //检查录音缓冲区　并发送
INT8U SendNullFlag;
INT8U rec_null_data[64];
void send_rec_audio_data(INT8U *audio_buf, INT16U length);


INT8U  WavPlayMode;  //音频播放模式　　单次　　循环
INT32U PcmO_Len;
INT32U PcmO_iPut;  //加到缓冲区位置
INT32U PcmO_iGet;  //取位置
INT8U *PcmO_Stream[PCM_BUF_NUM];

INT16U PcmI_Len;
INT16U PcmI_iPut;  //加到缓冲区位置
INT16U PcmI_iGet;  //取位置
INT8U *PcmI_Stream;

INT8U *Rec_Stream;

INT8U HalfSpeakerVolume = 0x00;
INT8U HalfMicVolume = 0x20;

#define _HALF_DUPLEX    //半双工
#ifdef _HALF_DUPLEX    //半双工
 #define HALF_NUM   5//4
 #define HALF_CHECK_NUM   4//4
 #define HALF_DELAY_NUM   16//16

 #define  DB_LOW  0x78//0x14//0x78 //0x78 -- -60db  0xC0 -- -96db //0x58  //-44db      有可能需要调整
 #define  MIC_DB_MIDDLE  0x2C  //0x16  //-22db
 #define  SPK_DB_MIDDLE  0x2C  //-22db
 #define  DB_HIGH  0x00  //0db

 #define  SPK_LOW  0x00  //-44db
 #define  SPK_MIDDLE  0x0F  //-22db
 #define  SPK_HIGH  0x21  //0db

 INT8U Half_Status;     //0  -- 待机  1 -- 发送  2 -- 接收

 INT8U Half_Duplex_PlayFlag;
 INT8U Half_Duplex_DelayFlag;
 INT8U Half_Duplex_DelayType;  //1  录音大于放音延时    2  放音大于录音延时
 INT8U Half_Duplex_DelayTime;
 INT16U PlayPcmTotalNum[HALF_NUM], RecPcmTotalNum[HALF_NUM];   //一个接收音频包总的幅度值
 void Half_Duplex_Deal_Func(void);
 void Cal_PcmTotalNum(INT8U pcmi_h, INT8U pcmi_l, INT8U pcmo_h, INT8U pcmo_l);
#endif
INT8U Video_Send_Flag = 0;
INT8U Audio_Send_Flag = 0;

//#define _CAL_OUTPUT_AUDIO  //统计固定输出能量
#ifdef  _CAL_OUTPUT_AUDIO  //统计固定输出能量
  INT32U Energy_Count;
  INT32U TotalEnergy;
#endif

//#define _SCREEN_PRINT  //屏幕打印能量
#ifdef  _SCREEN_PRINT  //屏幕打印能量
  INT16U Print_Count;
#endif
//---------------------------------------------------------------------------
void Audio_PowerOn(void)
{
  Pcm_Enable(1);
  //Writees8388(0x03, 0x00);  // ADC power on
  Writees8388(0x04, 0x3C);  // DAC power on

  //init_es8388();
  //SoundSetSpeakerVolume(SpeakerVolume);
  //SoundSetSpeakerDac(0x00);
  StopPlayWavFile();
}
//---------------------------------------------------------------------------
void Audio_PowerDown(void)
{
  Pcm_Enable(0);

  //SoundSetSpeakerVolume(0x00);
  //SoundSetSpeakerDac(0xC0);
  Writees8388(0x04, 0xC0);    // DAC power down
}
//---------------------------------------------------------------------------
void Pcm_Enable(uchar uFlag)    //20120305 xu
{
  if (uFlag)
    outportb(CONTROL_REG, inportb(CONTROL_REG)|0x18);  //000 11 000  PCM使能
  //  outportb(CONTROL_REG, inportb(CONTROL_REG)|0x10);  //000 10 000  PCMO使能
  else
   {
    outportb(CONTROL_REG, inportb(CONTROL_REG)&0xe7);  //111 00 111  PCM禁止
 //   outportb(CONTROL_REG, inportb(CONTROL_REG)&0xef);  //111 01 111  PCMO禁止
   }

  xtm_printf("Pcm_Enable::uFlag = %d, CONTROL_REG = 0x%X\n", uFlag, inportb(CONTROL_REG));
}
//---------------------------------------------------------------------------
void InitPcm(void)    //20120104 xu
{
  INT8U i;
  pcmi_bn = 0;
  pcmo_bn = 0;
  Pcm_Enable(1);

  for(i=0; i<64; i++)
    rec_null_data[i] = 0x55;

  DelayMs(400);

  WavPlayMode = NO_CYCLE_PLAY;  //音频播放模式　　单次　　循环
  xtm_printf("InitPcm...\n");
  xtm_printf("3.0 inportb(GPIOA_OEN_DATA) = 0x%X\n", inportb(GPIOA_OEN_DATA));
  init_es8388();

  xtm_printf("3.1 inportb(GPIOA_OEN_DATA) = 0x%X\n", inportb(GPIOA_OEN_DATA)); 
 #if 1
  //outportb(PCM_CONTROL, 0x2b);  //0010 1011   每块64byte   16K
  outportb(PCM_CONTROL, 0x23);  //0010 0011   每块64byte   8K
  BlockLen = 64;

//  outportb(PCM_CONTROL, 0x43);  //0100 0011   每块128byte
//  BlockLen = 128;
//  outportb(PCM_CONTROL, 0x63);  //0110 0011   每块256byte
//  BlockLen = 256;
  xtm_printf("InitPcm::BlockLen = %d\n", BlockLen);

  setvect(10, PCMInt);
  setvect(19, PCMPInt);

  setvect(20, PCMOInt);
 
 #else
  outportb(PCM_CONTROL, 0x1b);    //PCMI  PCMO 直连


  //Writees8388(0x04, 0x3c);   //不静音

  //bypass mode
/*  Writees8388(0x08, 0x00);  //Slave Mode
  Writees8388(0x02, 0xF3);  //Power down DEM&STM
  Writees8388(0x2B, 0x80);  //Set same LRCK
  Writees8388(0x00, 0x05);  //Set Chip to Play&Record Mode
  Writees8388(0x01, 0x40);  //Power Up Analog and Ibias
  Writees8388(0x03, 0x3F);  //Power Up ADC/Analog Input/Micbias for Record
  Writees8388(0x04, 0xFC);  //Power Up DAC and Enable LOUT/ROUT
  Writees8388(0x26, 0x1B);  //Mixer
  Writees8388(0x27, 0x00);
  Writees8388(0x28, 0x38);
  Writees8388(0x29, 0x38);
  Writees8388(0x2A, 0x50);
  Writees8388(0x2E, 0x1E);  // LOUT Volume=0dB
  Writees8388(0x2F, 0x1E);
  Writees8388(0x30, 0x1E);
  Writees8388(0x31, 0x1E);
  Writees8388(0x02, 0x00);  //Power up DEM&STM     */
 #endif
  SoundSetSpeakerVolume(LocalCfg.SpkVolume);
  LocalCfg.MicVolume = 0;
  SoundSetMicVolume(LocalCfg.MicVolume);
}
//---------------------------------------------------------------------------
void CreateAudioSend_Stream(void)      //音频发送Stream
{
    struct talkdata1 talkdata;
    //头部
    memcpy(AudioSend_Stream, UdpPackageHead, 6);
    //命令
    AudioSend_Stream[6] = VIDEOTALK;

    AudioSend_Stream[7]= ASK;        //主叫
    if((Local.Status == 1)||(Local.Status == 3)||(Local.Status == 5))  //本机为主叫方
     {
      AudioSend_Stream[8] = CALLUP;
      memcpy(talkdata.HostAddr, LocalCfg.Addr, 20);
      memcpy(talkdata.HostIP, LocalCfg.IP, 4);
      memcpy(talkdata.AssiAddr, Remote.Addr[0], 20);
      memcpy(talkdata.AssiIP, Remote.IP[0], 4);
     }
    if((Local.Status == 2)||(Local.Status == 4)||(Local.Status == 6))  //本机为被叫方
     {
      AudioSend_Stream[8] = CALLDOWN;
      memcpy(talkdata.HostAddr, Remote.Addr[0], 20);
      memcpy(talkdata.HostIP, Remote.IP[0], 4);
      memcpy(talkdata.AssiAddr, LocalCfg.Addr, 20);
      memcpy(talkdata.AssiIP, LocalCfg.IP, 4);
     }

    //时间戳
    talkdata.timestamp = 0;
      //数据类型
    talkdata.DataType = 1;
    //帧序号
    talkdata.Frameno = 0;
    //帧数据长度
    talkdata.Framelen = BlockLen;
    //总包数
    talkdata.TotalPackage = 1;
    //当前包
    talkdata.CurrPackage = 1;
    //数据长度
    talkdata.Datalen = BlockLen;
    talkdata.PackLen = PACKDATALEN;
    memcpy(AudioSend_Stream + 9, &talkdata, sizeof(talkdata));

    xtm_printf("CreateAudioSend_Stream::Local.Status = %d, Remote.DenIP = %d.%d.%d.%d, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, AudioSend_Stream[8] = 0x%X\n", \
           Local.Status, Remote.DenIP[0], Remote.DenIP[1], Remote.DenIP[2], Remote.DenIP[3], \
           AudioSend_Stream[0], AudioSend_Stream[1], AudioSend_Stream[2], AudioSend_Stream[3], AudioSend_Stream[4], AudioSend_Stream[5], AudioSend_Stream[6], AudioSend_Stream[7], AudioSend_Stream[8]);
}
//---------------------------------------------------------------------------
void StartRecAudio(void)
{
#if 1
  INT8U i;
  audio_timestamp = 0;      //音视频发送Stream
  Audio_Frameno = 0;

  SendNullFlag = 0;
  
  PcmI_Len = 0;
  PcmI_iPut = 0;  //加到缓冲区位置
  PcmI_iGet = 0;  //取位置
  pcmi_bn = 0;

  Writees8388(0x19, 0x22);//0x28);     //固定0x20 可能要调整   e0    soft ramp
                           //0x70  0xda
  Writees8388(0x09, 0x88);//0x70);     // 0-0x80    只调节高4位   左声道
/*  Writees8388(0x12, 0xe2);//0x1a);//0xda);//0xda);//0xe2);    // Reg 0x12 = 0xe2 (ALC enable, PGA Max. Gain=23.5dB, Min. Gain=0dB)        0xca
  Writees8388( 0x13, 0xa0);//0xa0  0xa2 // Reg 0x13 = 0xc0 (ALC Target=-4.5dB, ALC Hold time =0 mS)
  Writees8388( 0x14, 0x12);   // Reg 0x14 = 0x12(Decay time =820uS , Attack time = 416 uS)
  Writees8388( 0x15, 0x06);   // Reg 0x15 = 0x06(ALC mode)
  Writees8388( 0x16, 0x13);//0x73);//0xf9);//0x00);//0xf9);//0xf1);//0xf3  0x13);//0x73); // 0xc3// Reg 0x16 = 0xc3(nose gate = -40.5dB, NGG = 0x01(mute ADC))   有可能需要调整
                         */
//  Writees8388( 0x06, 0xc3);                      
  //xtm_printf("StartRecAudio::PCM_STATUS = 0x%X\n", inportb(PCM_STATUS));

  //outportb(CONTROL_REG, inportb(CONTROL_REG)|0x48);  //0 1001 000
  outportb(CONTROL_REG, inportb(CONTROL_REG)|0x40);  //0 1000 000
                                                     //D3：   PCM输入使能    启用
                                                     //D6：   PCM中断使能    启用
  outportb(PCMI_CONTROL, inportb(PCMI_CONTROL)|0x80);//使能PCMP中断

  #ifdef  _SCREEN_PRINT  //屏幕打印能量
   Print_Count = 0;
  #endif
  #ifdef _HALF_DUPLEX    //半双工
    for(i=0; i<HALF_NUM; i++)
     {
      PlayPcmTotalNum[i] = 0;       //一个接收音频包总的幅度值
      RecPcmTotalNum[i] = 0;
     }
   if(WavRecFlag)
    {
     SoundSetSpeakerVolume(LocalCfg.SpkVolume);
     SoundSetMicVolume(LocalCfg.MicVolume);
    }
   else
    {
     SoundSetMicAdc(DB_HIGH);      // MIC_DB_MIDDLE
     SoundSetSpeakerDac(DB_HIGH);    //  SPK_DB_MIDDLE
    }
    Half_Status = 0;     //0  -- 待机  1 -- 发送  2 -- 接收
  #endif
  xtm_printf("StartRecAudio::PCMI_CONTROL = 0x%X\n", inportb(PCMI_CONTROL));
#endif  
}
//---------------------------------------------------------------------------
void StopRecAudio(void)
{
  PcmI_Len = 0;
  PcmI_iPut = 0;  //加到缓冲区位置
  PcmI_iGet = 0;  //取位置
  pcmi_bn = 0;

  Writees8388(0x19, 0x08);     //固定0x20 可能要调整   e0
  //outportb(CONTROL_REG, inportb(CONTROL_REG)&0xB7);  //1 0110 111
  outportb(CONTROL_REG, inportb(CONTROL_REG)&0xBF);  //1 0111 111
                                                     //D3:  PCM输入使能    禁用
                                                     //D6： PCM中断使能    禁用

  outportb(PCMI_CONTROL, inportb(PCMI_CONTROL)&0x7f);//清除PCM Power中断

  xtm_printf("StopRecAudio::PCM_STATUS = 0x%X\n", inportb(PCM_STATUS));

  SoundSetSpeakerVolume(LocalCfg.SpkVolume);
  //SoundSetMicVolume(LocalCfg.MicVolume);
  SoundSetSpeakerDac(0x00);
}
//---------------------------------------------------------------------------
void StartPlayAudio(void)
{
#if 1
  INT8U i;
  PcmO_Len = 0;
  PcmO_iPut = 0;
  PcmO_iGet = 0;
  pcmo_bn = 0;
             
//  Writees8388(0x1d, 0x40);   //不静音
  Writees8388(0x04, 0x3c);   //不静音
  pcmo_bn = ((inport(PCM_STATUS) & 0x30) >> 4);
  //xtm_printf("StartPlayAudio::PCM_STATUS = 0x%X\n", inport(PCM_STATUS));

  //outportb(CONTROL_REG, inportb(CONTROL_REG)|0x10);  //0 0010 000
                                                     //D4: PCM输出使能    启用

  outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)|0x80);//使能PCMO中断

  outportb(GPIOA_OUT_DATA, inportb(GPIOA_OUT_DATA) | 0x20);
  
  SoundSetSpeakerDac(0);

 #ifdef  _CAL_OUTPUT_AUDIO  //统计固定输出能量
  Energy_Count = 0;
  TotalEnergy = 0;
  outportb(PCMI_CONTROL, inportb(PCMI_CONTROL)|0x80);//使能PCMP中断
 #endif
#endif
}
//---------------------------------------------------------------------------
void StopPlayAudio(void)
{
  INT32U Average;
  outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)&0x7f);//清除PCMO中断
 // outportb(GPIOA_OUT_DATA, inportb(GPIOA_OUT_DATA) & 0xdf);

  //outportb(CONTROL_REG, inportb(CONTROL_REG)&0xef);  //1 1101 111
                                                     //D4: PCM输出使能    禁用
 #ifdef  _CAL_OUTPUT_AUDIO  //统计固定输出能量
  outportb(PCMI_CONTROL, inportb(PCMI_CONTROL)&0x7f);//清除PCM Power中断
  Average = TotalEnergy / (Energy_Count - 1);
  xtm_printf("StopPlayAudio::TotalEnergy = 0x%X %X, Energy_Count = 0x%X %X, Average = 0x%X %X\n", (INT16U)((TotalEnergy & 0xffff0000) >> 16), (INT16U)(TotalEnergy & 0xffff), \
        (INT16U)((Energy_Count & 0xffff0000) >> 16), (INT16U)(Energy_Count & 0xffff), (INT16U)((Average & 0xffff0000) >> 16), (INT16U)(Average & 0xffff));
 #endif

  PcmO_iPut = 0;
  PcmO_iGet = 0;
  pcmo_bn = 0;

  //Writees8388(0x1d, 0xc0);   //静音
  Writees8388(0x04, 0xfc);   //静音
  if(Local.PlayNullVoice)
   {
    Local.PlayNullVoice = 0;
    SoundSetSpeakerVolume(LocalCfg.SpkVolume);
    SoundSetSpeakerDac(0x00);
    xtm_printf("StopPlayAudio::Local.PlayNullVoice = %d ..............\n", Local.PlayNullVoice);
   }
}
//---------------------------------------------------------------------------
void StartPlayWav(INT32U FlashAddr, INT8U PlayFlag) //0 单次播放  1 循环播放
{
  INT16U i;
  INT32U PcmO_Buf, Maddrs;
  if(AudioPlayIsStart == 0)
   {
    WavPlayFlag = 1;
    AudioPlayIsStart = 1;

    WavPlayMode = PlayFlag;
    ReadWavFile(FlashAddr);

    PcmO_Buf = 0x1B0000UL;

    StartPlayAudio();
    PcmO_Len = WavFileLen;
    PcmO_iPut = PcmO_Len;

    Maddrs = (((INT32U)(PcmO_Stream[0])&0xffff)+(((INT32U)(PcmO_Stream[0])&0xffff0000)>>12))|0x700000;
    fmem_fmem_dma(Maddrs, PcmO_Buf, BlockLen*3);
    PcmO_iGet = BlockLen*3;
    for(i=0; i<3; i++)
     {
    //  while((inportb(PCM_STATUS)&0x01)==0x01)
    //    delay(1);
      outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)|0x40);//PCMO缓冲区增加一块
      outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)&0xbf);
     }
    pcmo_bn = 3;
   }
  else
    xtm_printf("StartPlayWav::AudioPlayIsStart = %d\n", AudioPlayIsStart);
}
//---------------------------------------------------------------------------
void StartPlayWav_Rec(INT32U FlashAddr, INT8U PlayFlag) //0 单次播放  1 循环播放
{
  INT16U i;
  INT32U PcmO_Buf, Maddrs, Maddrs1;
  if(AudioPlayIsStart == 0)
   {
    RecWavPlayFlag = 1;
    WavPlayFlag = 1;
    AudioPlayIsStart = 1;

    WavPlayMode = PlayFlag;
  //  ReadWavFile(FlashAddr);

    PcmO_Buf = 0x1B0000UL;

    StartPlayAudio();
    PcmO_Len = RecWavFileLen;
    PcmO_iPut = PcmO_Len;

    Maddrs = (((INT32U)(PcmO_Stream[_REC_NO])&0xffff)+(((INT32U)(PcmO_Stream[_REC_NO])&0xffff0000)>>12))|0x700000;
   // Maddrs1 = (((INT32U)(PcmO_Stream[2])&0xffff)+(((INT32U)(PcmO_Stream[2])&0xffff0000)>>12))|0x700000;
    //fmem_fmem_dma(Maddrs1, Maddrs, 0x1100);
   // memcpy(PcmO_Stream[0], PcmO_Stream[2], 0x10000);
    fmem_fmem_dma(Maddrs, PcmO_Buf, BlockLen*3);
    PcmO_iGet = BlockLen*3;
    for(i=0; i<3; i++)
     {
    //  while((inportb(PCM_STATUS)&0x01)==0x01)
    //    delay(1);
      outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)|0x40);//PCMO缓冲区增加一块
      outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)&0xbf);
     }
    pcmo_bn = 3;
   }
  else
    xtm_printf("StartPlayWav::AudioPlayIsStart = %d\n", AudioPlayIsStart);
}
//---------------------------------------------------------------------------
void StopPlayWavFile(void)
{
  StopPlayAudio();
  AudioPlayIsStart = 0;
  WavPlayFlag = 0;
  RecWavPlayFlag = 0;
  Local.Test_PlayAudioFlag = 0;
  xtm_printf("StopPlayWavFile::AudioPlayIsStart = %d\n", AudioPlayIsStart);
}
//---------------------------------------------------------------------------
void WaitAudioUnuse(INT16U delayms) //等待声卡空闲
{
  INT16U  tmp;
  tmp = 0;
  while(AudioPlayIsStart == 1)
   {
    DelayMs(10);
    tmp += 10;
    if(tmp > delayms)
      break;
   }
}
//---------------------------------------------------------------------------
void StartRecWav(void)
{
  INT16U i;
  INT32U PcmO_Buf, Maddrs;
  if(AudioRecIsStart == 0)
   {
    WavRecFlag = 1;
    AudioRecIsStart = 1;

    StartRecAudio();

    RecWavFileLen = 0;
    PcmO_Len = 0;
    PcmO_iGet = 0;
   }
  else
    xtm_printf("StartRecWav::AudioRecIsStart = %d\n", AudioRecIsStart);

}
//---------------------------------------------------------------------------
void StopRecWav(void)
{
  StopRecAudio();
  AudioRecIsStart = 0;
  WavRecFlag = 0;
  //memcpy(PcmO_Stream[0], Rec_Stream, 0x10000);
  xtm_printf("StopRecWav:: RecWavFileLen= 0x%X %X\n", (INT16U)((RecWavFileLen & 0xffff0000) >> 16), (INT16U)(RecWavFileLen & 0xffff));
  Local.Test_RecAudioFlag = 0; //录音
}
//---------------------------------------------------------------------------
void interrupt PCMInt(void)
{
#if 1
	INT32U Maddrs;
	INT32U Maddrd;
        INT8U Stream_Pos;
        INT16U pcm_b_pos;


        INT32U PcmI_Buf;
        INT32U PcmO_Buf;
        INT32U tmp_Maddr;
        uchar i;

        outportb(CONTROL_REG, inportb(CONTROL_REG)&0xbf); //清除PCM中断

//        xtm_printf("PCMI = 0x%X\n", ((inport(PCM_STATUS) & 0x0C) >> 2));//        xtm_printf("PCM_STATUS = 0x%X\n", inport(PCM_STATUS));        Maddrs = 0x190000UL;	Maddrs = Maddrs+(BlockLen*(pcmi_bn&0x3));        if(WavRecFlag)         {         #if 1          Stream_Pos = (RecWavFileLen >> 16) & 0xffff;          Stream_Pos = _REC_NO;          pcm_b_pos = (RecWavFileLen & 0xffff);          PcmO_Buf = (((INT32U)(PcmO_Stream[Stream_Pos] + pcm_b_pos)&0xffff)+(((INT32U)(PcmO_Stream[Stream_Pos] + pcm_b_pos)&0xffff0000)>>12))|0x700000;          fmem_fmem_dma(Maddrs, PcmO_Buf, BlockLen);    	  pcmi_bn ++;
          RecWavFileLen += BlockLen;
          //xtm_printf("PCMInt::WavFileLen = 0x%X %X, PcmO_Buf = 0x%X %X\n", (INT16U)((WavFileLen & 0xffff0000) >> 16), (INT16U)(WavFileLen & 0xffff), \
          //            (INT16U)((PcmO_Buf & 0xffff0000) >> 16), (INT16U)(PcmO_Buf & 0xffff));
          if(RecWavFileLen >= 0x10000)
            StopRecWav();
         #else
          pcm_b_pos = (RecWavFileLen & 0xffff);     //     PcmO_Buf = (((INT32U)(Rec_Stream + pcm_b_pos)&0xffff)+(((INT32U)(Rec_Stream + pcm_b_pos)&0xffff0000)>>12))|0x700000;          fmem_fmem_dma(Maddrs, PcmO_Buf, BlockLen);    	  pcmi_bn ++;
          RecWavFileLen += BlockLen;
         #endif
         }
        else
         {
         #if 1
          PcmI_Buf = (((INT32U)(PcmI_Stream)&0xffff)+(((INT32U)(PcmI_Stream)&0xffff0000)>>12))|0x700000;          fmem_fmem_dma(Maddrs, PcmI_Buf, BlockLen);          PcmI_Len = 0;//BlockLen;          send_rec_audio_data(PcmI_Stream, BlockLen);         #else          if(PcmI_Len < PCM_REC_MAX)           {            pcm_b_pos = 0x1800 + PcmI_iPut;            PcmI_Buf = (((INT32U)(PcmI_Stream + pcm_b_pos)&0xffff)+(((INT32U)(PcmI_Stream + pcm_b_pos)&0xffff0000)>>12))|0x700000;            fmem_fmem_dma(Maddrs, PcmI_Buf, BlockLen);            PcmI_iPut += BlockLen;            if(PcmI_iPut >= PCM_REC_MAX)
              PcmI_iPut = 0;            PcmI_Len += BlockLen;            //PcmI_Len = 0;            //send_rec_audio_data(PcmI_Stream + pcm_b_pos, BlockLen);           }         #endif      	  pcmi_bn ++;

         }
        outportb(CONTROL_REG, inportb(CONTROL_REG)|0x40);  //使能PCM中断#endif        }
//---------------------------------------------------------------------------
void CheckRecAudioBuf(void)   //检查录音缓冲区　并发送
{
  INT8U i;
  INT16U pcm_b_pos;
  if(PcmI_Len > 1024)
   {
    //xtm_printf("CheckRecAudioBuf::PcmI_Len = 0x%X %X\n", (INT16U)((PcmI_Len & 0xffff0000) >> 16), (INT16U)(PcmI_Len & 0xffff));
    //for(i=0; i<4; i++)
     {
      pcm_b_pos = 0x1800 + PcmI_iGet;
      send_rec_audio_data(PcmI_Stream + pcm_b_pos, BlockLen);
      PcmI_iGet += BlockLen;
      if(PcmI_iGet >= PCM_REC_MAX)
        PcmI_iGet = 0;
      PcmI_Len -= BlockLen;
     } 
   }
}
//---------------------------------------------------------------------------
INT8U SendAudioNum = 0;
void send_rec_audio_data(INT8U *audio_buf, INT16U length)
{
#if 1
  INT8U RemoteMac[6];
  INT16U sendlength;
  //if(SearchArp_Table(Remote.DenIP, RemoteMac))
   {
    //xtm_printf("send_rec_audio_data1::Remote.DenIP = %d.%d.%d.%d, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, AudioSend_Stream[8] = 0x%X\n", \
    //       Remote.DenIP[0], Remote.DenIP[1], Remote.DenIP[2], Remote.DenIP[3], \
    //       AudioSend_Stream[0], AudioSend_Stream[1], AudioSend_Stream[2], AudioSend_Stream[3], AudioSend_Stream[4], AudioSend_Stream[5], AudioSend_Stream[6], AudioSend_Stream[7], AudioSend_Stream[8]);
    //AudioSend_Stream[8] = CALLDOWN;
    //时间戳
    memcpy(AudioSend_Stream + 57, &audio_timestamp, sizeof(INT32U));
    //数据类型
    AudioSend_Stream[61] = 1;
    AudioSend_Stream[62] = 0;
    //帧序号
    memcpy(AudioSend_Stream + 63, &Audio_Frameno, sizeof(INT16U));
    //帧数据长度
    AudioSend_Stream[65] = BlockLen;
    AudioSend_Stream[66] = 0;
    AudioSend_Stream[67] = 0;
    AudioSend_Stream[68] = 0;
    //总包数
    AudioSend_Stream[69] = 1;
    AudioSend_Stream[70] = 0;
    //当前包
    AudioSend_Stream[71] = 1;
    AudioSend_Stream[72] = 0;
    //数据长度
    AudioSend_Stream[73] = BlockLen;
    AudioSend_Stream[74] = 0;
    //数据
    if(SendNullFlag == 0)
      memcpy(AudioSend_Stream + 86, audio_buf, length);
    else
      memcpy(AudioSend_Stream + 86, rec_null_data, length);
    sendlength = 86 + length;

   #if 1
    CreateMultiUdpBuff(LocalPort[1], SINGLE_SEND, Remote.DenIP, AudioSend_Stream, sendlength, 0);
   #else
    Audio_Send_Flag = 1;
    UdpSendBuff_1(LocalPort[1], Remote.DenMac, Remote.DenIP, AudioSend_Stream, sendlength);
    Audio_Send_Flag = 0;
    SendAudioNum ++;
   #endif
    //xtm_printf("send_rec_audio_data::Remote.DenIP = %d.%d.%d.%d, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, AudioSend_Stream[8] = 0x%X\n", \
    //       Remote.DenIP[0], Remote.DenIP[1], Remote.DenIP[2], Remote.DenIP[3], \
    //       AudioSend_Stream[0], AudioSend_Stream[1], AudioSend_Stream[2], AudioSend_Stream[3], AudioSend_Stream[4], AudioSend_Stream[5], AudioSend_Stream[6], AudioSend_Stream[7], AudioSend_Stream[8]);

    audio_timestamp += 8;      //音视频发送Stream
    Audio_Frameno ++;

    if(Local.Status == 0)
      xtm_printf("send_rec_audio_data but Local.Status = %d\n", Local.Status);
   }
#endif   
}
//---------------------------------------------------------------------------
void CheckPlayAudioBuf(void)
{
  INT16U i;
  INT32U PcmO_Buf, Maddrs;
  INT8U Stream_Pos;
  INT16U pcm_b_pos;
  if((inport(PCM_STATUS) & 0x40) == 0x40)  //PCMO缓冲区少于一块
   {
    outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)&0x7f);//清除PCMO中断
    if(PcmO_Len > (3*BlockLen))
    //if((inport(PCM_STATUS) & 0x80) == 0x80)  //PCMO缓冲区空
     {
      //xtm_printf("CheckPlayAudioBuf::inport(PCM_STATUS) = 0x%X\n", inport(PCM_STATUS));
      //outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)&0x7f);//清除PCMO中断
      PcmO_Buf = 0x1B0000UL;
      for(i=0; i<3; i++)
       {
        PcmO_Buf = PcmO_Buf+(BlockLen*(pcmo_bn&0x3));        Stream_Pos = (PcmO_iGet >> 16) & 0xffff;        pcm_b_pos = (PcmO_iGet & 0xffff);        //tmp_Maddr = (((INT32U)PcmO_Stream[Stream_Pos]&0xffff)+(((INT32U)PcmO_Stream[Stream_Pos]&0xffff0000)>>12))|0x700000;        //xtm_printf("11 tmp_Maddr = 0x%X %X\n", (INT16U)((tmp_Maddr & 0xffff0000) >> 16), (INT16U)(tmp_Maddr & 0xffff));        Maddrs = (((INT32U)(PcmO_Stream[Stream_Pos] + pcm_b_pos)&0xffff)+(((INT32U)(PcmO_Stream[Stream_Pos] + pcm_b_pos)&0xffff0000)>>12))|0x700000;        //xtm_printf("11 Maddrs = 0x%X %X\n", (INT16U)((Maddrs & 0xffff0000) >> 16), (INT16U)(Maddrs & 0xffff));        fmem_fmem_dma(Maddrs, PcmO_Buf, BlockLen);        //xtm_printf("Stream_Pos = %d, pcmo_bn = %d  PcmO_iGet = 0x%X\n", Stream_Pos, pcmo_bn, PcmO_iGet);        PcmO_iGet += BlockLen;
        if(PcmO_iGet >= PCM_BUF_MAX)
          PcmO_iGet = 0;
        pcmo_bn ++;

        outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)|0x40);//PCMO缓冲区增加一块
        outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)&0xbf);
        if(PcmO_Len > 0)
          PcmO_Len -= BlockLen;
      }
    }
   outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)|0x80);//使能PCMO中断
  }
}
//---------------------------------------------------------------------------
void interrupt PCMOInt(void)//PCMO Int
{
#if 1
  INT32U Maddrs;
  INT8U Stream_Pos;
  INT16U pcm_b_pos;

  INT32U PcmO_Buf;
  INT32U Tmp_PcmO_iPut;
  INT8U Int_Enabled;
  INT8U i;

  outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)&0x7f);//清除PCMO中断

  if(PcmO_Len > 0)
   {
    //xtm_printf("PCMOInt::PCM_STATUS = 0x%X\n", inport(PCM_STATUS));

    PcmO_Buf = 0x1B0000UL;
    PcmO_Buf = PcmO_Buf+(BlockLen*(pcmo_bn&0x3));    Stream_Pos = (PcmO_iGet >> 16) & 0xffff;    pcm_b_pos = (PcmO_iGet & 0xffff);    //播放录音状态    if(RecWavPlayFlag == 1)      Stream_Pos = _REC_NO;   /* memcpy(PcmO_Stream[Stream_Pos] + pcm_b_pos, rec_null_data, BlockLen);    for(i=0; i<BlockLen; i++)     {      PcmO_Stream[Stream_Pos][pcm_b_pos + i] = 0x55;     }    */    Maddrs = (((INT32U)(PcmO_Stream[Stream_Pos] + pcm_b_pos)&0xffff)+(((INT32U)(PcmO_Stream[Stream_Pos] + pcm_b_pos)&0xffff0000)>>12))|0x700000;    //xtm_printf("11 Maddrs = 0x%X %X\n", (INT16U)((Maddrs & 0xffff0000) >> 16), (INT16U)(Maddrs & 0xffff));    fmem_fmem_dma(Maddrs, PcmO_Buf, BlockLen);    //xtm_printf("Stream_Pos = %d, pcmo_bn = %d  PcmO_iGet = 0x%X\n", Stream_Pos, pcmo_bn, PcmO_iGet);    PcmO_iGet += BlockLen;
    if(PcmO_iGet >= PCM_BUF_MAX)
      PcmO_iGet = 0;
    pcmo_bn ++;

    outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)|0x40);//PCMO缓冲区增加一块
    outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)&0xbf);
   #if 0
    if(PcmO_iPut >= PcmO_iGet)
      PcmO_Len = PcmO_iPut - PcmO_iGet;
    else
      PcmO_Len = PcmO_iPut + PCM_BUF_MAX - PcmO_iGet;
   #else
    if(PcmO_Len > 0)
      PcmO_Len -= BlockLen;
   #endif
   }
/*  xtm_printf("PCMOInt::PcmO_iGet = 0x%X  %X, PcmO_iPut = 0x%X  %X, PcmO_Len = 0x%X  %X, Maddrs = 0x%X %X\n", \
          (INT16U)((PcmO_iGet & 0xffff0000) >> 16), (INT16U)(PcmO_iGet & 0xffff), \
          (INT16U)((PcmO_iPut & 0xffff0000) >> 16), (INT16U)(PcmO_iPut & 0xffff), \
           (INT16U)((PcmO_Len & 0xffff0000) >> 16), (INT16U)(PcmO_Len & 0xffff), \
           (INT16U)((Maddrs & 0xffff0000) >> 16), (INT16U)(Maddrs & 0xffff));   */

/*  xtm_printf("PCMOInt::PcmO_iGet = 0x%X  %X, PcmO_iPut = 0x%X  %X,  Maddrs = 0x%X %X\n", \
          (INT16U)((PcmO_iGet & 0xffff0000) >> 16), (INT16U)(PcmO_iGet & 0xffff), \
          (INT16U)((PcmO_iPut & 0xffff0000) >> 16), (INT16U)(PcmO_iPut & 0xffff), \
           (INT16U)((Maddrs & 0xffff0000) >> 16), (INT16U)(Maddrs & 0xffff));       */

  Int_Enabled = 1;
  if(WavPlayFlag)
   if(PcmO_iGet >= (PcmO_iPut - BlockLen))
    {
     if(WavPlayMode == NO_CYCLE_PLAY)  //音频播放模式　　单次　　循环
      {
       StopPlayWavFile();
       Int_Enabled = 0;
       //xtm_printf("PCMOInt::StopPlayWavFile, PcmO_iGet = 0x%X  %X, PcmO_iPut = 0x%X  %X\n", (INT16U)((PcmO_iGet & 0xffff0000) >> 16), (INT16U)(PcmO_iGet & 0xffff), (INT16U)((PcmO_iPut & 0xffff0000) >> 16), (INT16U)(PcmO_iPut & 0xffff));
      }
     else
      {
       PcmO_Len = WavFileLen;
       PcmO_iPut = PcmO_Len;
       PcmO_iGet = 0;
      }
    }
  if(Int_Enabled)
    outportb(PCMO_CONTROL, inportb(PCMO_CONTROL)|0x80);//使能PCMO中断
#endif
}
//---------------------------------------------------------------------------
//读取 wav 文件
INT8U ReadWavFile(INT32U FlashAddr)
{
  INT8U read_spi[512];
  struct file_head1 file_head;
  INT32U WavAddr;
  INT32U tmp_Maddr;
  INT16U i, ReadNum;
  INT8U Total_Stream;

  WavAddr = FlashAddr;
  //xtm_printf("WavAddr = 0x%X %X\n", \
  //                              (INT16U)((WavAddr & 0xffff0000) >> 16), (INT16U)(WavAddr & 0xffff));
  xtm_read25x32data(WavAddr, read_spi, sizeof(struct file_head1));
  //xtm_printf("0x%X, 0x%X, 0x%X, 0x%X\n", read_spi[0], read_spi[1], read_spi[2], read_spi[3]);
  memcpy(&file_head, read_spi, sizeof(struct file_head1));
  //xtm_printf("读 Faddr = 0x%X%X , Filelen = 0x%X%X, Flen = 0x%X%X\n", \
  //                              (INT16U)((file_head.Faddr & 0xffff0000) >> 16), (INT16U)(file_head.Faddr & 0xffff), \
  //                              (INT16U)((file_head.Filelen & 0xffff0000) >> 16), (INT16U)(file_head.Filelen & 0xffff), \
  //                              (INT16U)((file_head.FLen & 0xffff0000) >> 16), (INT16U)(file_head.FLen & 0xffff)
  //                             );

  WavFileLen = file_head.Filelen;
  if((WavFileLen & 0xffff) == 0)
    Total_Stream = (WavFileLen >> 16) & 0xffff;
  else
    Total_Stream = ((WavFileLen >> 16) & 0xffff) + 1;
  //xtm_printf("Total_Stream = %d\n", Total_Stream);
  WavAddr += 256;
  if(Total_Stream > PCM_BUF_NUM)
    Total_Stream = PCM_BUF_NUM;
  for(i=0; i<Total_Stream; i++)
   {
    tmp_Maddr = (((INT32U)PcmO_Stream[i]&0xffff)+(((INT32U)PcmO_Stream[i]&0xffff0000)>>12))|0x700000;
    //xtm_printf("11 tmp_Maddr = 0x%X %X\n", (INT16U)((tmp_Maddr & 0xffff0000) >> 16), (INT16U)(tmp_Maddr & 0xffff));
    //xtm_printf("18 0x%X, 0x%X, 0x%X, 0x%X\n", PcmO_Stream[i][0], PcmO_Stream[i][1], PcmO_Stream[i][2], PcmO_Stream[i][3]);

    //xtm_printf("11 WavAddr = 0x%X %X\n", (INT16U)((WavAddr & 0xffff0000) >> 16), (INT16U)(WavAddr & 0xffff));
    #if 0
          ReadNum = 0;
          while(1)
           {
            if(i == (Total_Stream - 1))
              XTM_SPI_DRAM(WavAddr, tmp_Maddr, WavFileLen & 0xffff);//file_head.FLen);
            else
              XTM_SPI_DRAM(WavAddr, tmp_Maddr, 0x10000);
            ReadNum ++;
            if((PcmO_Stream[i][WavFileLen - 8] == 0x01)&&(PcmO_Stream[i][WavFileLen - 7] == 0x02)
             &&(PcmO_Stream[i][WavFileLen - 6] == 0x03)&&(PcmO_Stream[i][WavFileLen - 5] == 0x04)
             &&(PcmO_Stream[i][WavFileLen - 4] == 0x05)&&(PcmO_Stream[i][WavFileLen - 3] == 0x06)
             &&(PcmO_Stream[i][WavFileLen - 2] == 0x07)&&(PcmO_Stream[i][WavFileLen - 1] == 0x08))
             {
              if(ReadNum > 1)
                xtm_printf("ReadWavFile:: ReadNum = %d\n", ReadNum);
              break;
             }
            if(ReadNum > 10)
             {
              xtm_printf("ReadWavFile:: ReadNum = %d\n", ReadNum);
              break;
             }
           }
          xtm_printf("Total_Stream = %d, WavFileLen = 0x%X %X, PcmO_Stream = 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", \
                  Total_Stream, (INT16U)((WavFileLen & 0xffff0000) >> 16), (INT16U)(WavFileLen & 0xffff), \
                  PcmO_Stream[i][WavFileLen - 10], PcmO_Stream[i][WavFileLen - 9], PcmO_Stream[i][WavFileLen - 8], PcmO_Stream[i][WavFileLen - 7], \
                  PcmO_Stream[i][WavFileLen - 6], PcmO_Stream[i][WavFileLen - 5], \
                  PcmO_Stream[i][WavFileLen - 4], PcmO_Stream[i][WavFileLen - 3], PcmO_Stream[i][WavFileLen - 2], PcmO_Stream[i][WavFileLen - 1]);
    #else
            if(i == (Total_Stream - 1))
              //XTM_SPI_DRAM(WavAddr, tmp_Maddr, WavFileLen & 0xffff);//file_head.FLen);
              xtm_read25x32data(WavAddr, PcmO_Stream[i], WavFileLen & 0xffff);
            else
              //XTM_SPI_DRAM(WavAddr, tmp_Maddr, 0x10000);
              xtm_read25x32data(WavAddr, PcmO_Stream[i], 0xffff);
    #endif

    //xtm_printf("20 0x%X, 0x%X, 0x%X, 0x%X\n", PcmO_Stream[i][0], PcmO_Stream[i][1], PcmO_Stream[i][2], PcmO_Stream[i][3]);
    //xtm_printf("256 0x%X, 0x%X, 0x%X, 0x%X\n", PcmO_Stream[i][256], PcmO_Stream[i][256+1], PcmO_Stream[i][256+2], PcmO_Stream[i][256+3]);
    //xtm_printf("0x3000 0x%X, 0x%X, 0x%X, 0x%X\n", PcmO_Stream[i][0x3000], PcmO_Stream[i][0x3000+1], PcmO_Stream[i][0x3000+2], PcmO_Stream[i][0x3000+3]);
    //xtm_printf("0xa000 0x%X, 0x%X, 0x%X, 0x%X\n", PcmO_Stream[i][0xa000], PcmO_Stream[i][0xa000+1], PcmO_Stream[i][0xa000+2], PcmO_Stream[i][0xa000+3]);
    //xtm_printf("0xa580 0x%X, 0x%X, 0x%X, 0x%X\n", PcmO_Stream[i][0xa580], PcmO_Stream[i][0xa580+1], PcmO_Stream[i][0xa580+2], PcmO_Stream[i][0xa580+3]);

    //xtm_printf("20 0x%X, 0x%X, 0x%X, 0x%X\n", PcmO_Stream[i][0], PcmO_Stream[i][1], PcmO_Stream[i][2], PcmO_Stream[i][3]);
    //xtm_read25x32data(WavAddr + 256 + 256, (INT8U *)((INT32U)PcmO_Stream + 256), 256);
    //xtm_printf("21 0x%X, 0x%X, 0x%X, 0x%X\n", PcmO_Stream[i][0], PcmO_Stream[i][1], PcmO_Stream[i][2], PcmO_Stream[i][3]);

    //xtm_printf("52 0x%X, 0x%X, 0x%X, 0x%X\n", PcmO_Stream[i][52], PcmO_Stream[i][53], PcmO_Stream[i][54], PcmO_Stream[i][55]);
    //xtm_printf("22 0x%X, 0x%X, 0x%X, 0x%X\n", PcmO_Stream[i][file_head.Filelen-4-48], PcmO_Stream[i][file_head.Filelen-3-48], PcmO_Stream[i][file_head.Filelen-2-48], PcmO_Stream[i][file_head.Filelen-1-48]);
    WavAddr += 0x10000;
   }
}
//---------------------------------------------------------------------------
void interrupt PCMPInt(void)//PCM Power Int
{
  INT8U i;
  INT16U  TotalValue;
  INT32U Average;
  INT8U tmp_text[50];
  INT8U pcmi_h, pcmi_l, pcmo_h, pcmo_l;
	outportb(PCMI_CONTROL, inportb(PCMI_CONTROL)&0x7f);//清除PCM Power中断
#if 1 //xu
  pcmi_h =  inportb(PCMI_POWER_H);
  pcmi_l =  inportb(PCMI_POWER_L);
  pcmo_h =  inportb(PCMO_POWER_H);
  pcmo_l =  inportb(PCMO_POWER_L);
       // xtm_printf("PCMPInt: PCMI_POWER = 0x%X 0x%X, PCMO_POWER = 0x%X 0x%X\n", \
       //             pcmi_h, pcmi_l, pcmo_h, pcmo_l);

 #ifdef  _SCREEN_PRINT  //屏幕打印能量
  Print_Count ++;
  if(Print_Count > 125)
   {
    Print_Count = 0;
    for(i=0; i<40; i++)
      tmp_text[i] = '\0';
    xtm_sprintf(tmp_text, "%X %X,  %X %X\0", \
                    pcmi_h, pcmi_l, pcmo_h, pcmo_l);
    clear_area( 96, 200, 24, 60, COLOR_BLUE, Local.fb_page);
    gra_dispcn24(tmp_text, 96, 200, COLOR_WHITE, COLOR_BLUE, NOFULLPAGE);
    //xtm_printf(tmp_text);
   }
 #endif

 #ifdef  _CAL_OUTPUT_AUDIO  //统计固定输出能量
  if(Energy_Count != 0)
   {
    /*TotalValue = pcmo_h;
    TotalValue = TotalValue << 8;
    TotalEnergy += (TotalValue + pcmo_l);     */
    TotalValue = pcmi_h;
    TotalValue = TotalValue << 8;
    TotalEnergy += (TotalValue + pcmi_l);
   }
  Energy_Count ++;
  if((Energy_Count - 1) == 30)
   {
    Average = TotalEnergy / (Energy_Count - 1);
    xtm_printf("PCMPInt::TotalEnergy = 0x%X %X, Energy_Count = 0x%X %X, Average = 0x%X %X\n", (INT16U)((TotalEnergy & 0xffff0000) >> 16), (INT16U)(TotalEnergy & 0xffff), \
        (INT16U)((Energy_Count & 0xffff0000) >> 16), (INT16U)(Energy_Count & 0xffff), (INT16U)((Average & 0xffff0000) >> 16), (INT16U)(Average & 0xffff));
    TotalEnergy = 0;
    Energy_Count = 1;
   }
 #endif
   #ifdef _HALF_DUPLEX    //半双工
//xtm_printf("WavRecFlag = %d \n", WavRecFlag);
    if(!WavRecFlag)
     {
      Cal_PcmTotalNum(pcmi_h, pcmi_l, pcmo_h, pcmo_l);
   /* TotalValue = 0;
    for(i=0; i<4; i++)
      TotalValue += RecPcmTotalNum[i];
    if(TotalValue >= 0x80)
       xtm_printf("0x%X, 0x%X, 0x%X, 0x%X,\n", \
                    RecPcmTotalNum[0], RecPcmTotalNum[1], RecPcmTotalNum[2], RecPcmTotalNum[3]);   */
      Half_Duplex_Deal_Func();
     } 
   #endif
#endif
	outportb(PCMI_CONTROL, inportb(PCMI_CONTROL)|0x80);//使能PCM Power中断
}
//---------------------------------------------------------------------------
#ifdef _HALF_DUPLEX    //半双工

#define RECORD_VALUE  0x80 //0x20*4
#define PLAY_VALUE    0x80 //0x20*4 //0x100

#define SEND_RECORD_VALUE  0x80 //0x20*4
#define SEND_PLAY_VALUE    0x80//0x100 //0x20*4 //0x100

#define TOZEROVALUE  10//3

#define TONOZEROVALUE  8

INT8U OneToZero = 0;
INT8U TwoToZero = 0;
INT8U OneToTwo = 0;
INT8U TwoToOne = 0;

INT16U DealCount = 0;
void Half_Duplex_Deal_Func(void)
{
    INT8U i, j, RecLargePlay;
    //0  -- 小   1 -- 中   2 -- 大
    INT8U  RecStatus, PlayStatus;
    INT16U  TotalPlayValue, TotalRecordValue;

    TotalPlayValue = 0;
    for(i=0; i<HALF_CHECK_NUM; i++)
      TotalPlayValue += PlayPcmTotalNum[i];

    TotalRecordValue = 0;
    for(i=0; i<HALF_CHECK_NUM; i++)
      TotalRecordValue += RecPcmTotalNum[i];

    DealCount ++;
    if(Half_Status == 0)
     {
      if((TotalPlayValue >= PLAY_VALUE)&&(TotalRecordValue >= RECORD_VALUE))
       {
         SoundSetSpeakerDac(DB_HIGH);
         SendNullFlag = 1;
         Half_Status = 2;
         xtm_printf("Half_Status1 = %d, 0x%X 0x%X, DealCount = 0x%X\n", Half_Status, TotalPlayValue, TotalRecordValue, DealCount);
       }
      if((TotalPlayValue < PLAY_VALUE)&&(TotalRecordValue > RECORD_VALUE))
       {
        SoundSetSpeakerDac(DB_LOW);
        SendNullFlag = 0;
        Half_Status = 1;

        xtm_printf("Half_Status1 = %d, 0x%X 0x%X, DealCount = 0x%X\n", Half_Status, TotalPlayValue, TotalRecordValue, DealCount);
       }
      if((TotalPlayValue > PLAY_VALUE)&&(TotalRecordValue < RECORD_VALUE))
       {
         SoundSetSpeakerDac(DB_HIGH);
         SendNullFlag = 1;
         Half_Status = 2;
         xtm_printf("Half_Status2 = %d, 0x%X 0x%X, DealCount = 0x%X\n", Half_Status, TotalPlayValue, TotalRecordValue, DealCount);
       }
     }

    if(Half_Status == 1)
     {
      if((TotalPlayValue < SEND_PLAY_VALUE)&&(TotalRecordValue < SEND_RECORD_VALUE))
       {
         OneToZero ++;
         if(OneToZero > TOZEROVALUE)
          {
           OneToZero = 0;
           SoundSetSpeakerDac(DB_HIGH);
           SendNullFlag = 0;
           Half_Status = 0;
           xtm_printf("Half_Status1 = %d, 0x%X 0x%X, DealCount = 0x%X\n", Half_Status, TotalPlayValue, TotalRecordValue, DealCount);
          }
       }
      else
        OneToZero = 0;
        
      if((TotalPlayValue > SEND_PLAY_VALUE)&&(TotalRecordValue < SEND_RECORD_VALUE))
       {
         OneToTwo ++;
         if(OneToTwo > TONOZEROVALUE)
          {
           OneToTwo = 0;
           SoundSetSpeakerDac(DB_HIGH);
           SendNullFlag = 1;
           Half_Status = 2;
           xtm_printf("Half_Status3 = %d, 0x%X 0x%X, DealCount = 0x%X\n", Half_Status, TotalPlayValue, TotalRecordValue, DealCount);
          }
       }
      else
        OneToTwo = 0;
     }

    if(Half_Status == 2)
     {
      if((TotalPlayValue < PLAY_VALUE)&&(TotalRecordValue < RECORD_VALUE))
       {
         TwoToZero ++;
         if(TwoToZero > TOZEROVALUE)
          {
           TwoToZero = 0;
           SoundSetSpeakerDac(DB_HIGH);
           SendNullFlag = 0;
           Half_Status = 0;
           xtm_printf("Half_Status2 = %d, 0x%X 0x%X, DealCount = 0x%X\n", Half_Status, TotalPlayValue, TotalRecordValue, DealCount);
          }
       }
      else
        TwoToZero = 0;
        
      if((TotalPlayValue < PLAY_VALUE)&&(TotalRecordValue > RECORD_VALUE))
       {
         TwoToOne ++;
         if(TwoToOne > TONOZEROVALUE)
          {
           TwoToOne = 0;
           SoundSetSpeakerDac(DB_LOW);
           SendNullFlag = 0;
           Half_Status = 1;
           xtm_printf("Half_Status2 = %d, 0x%X 0x%X, DealCount = 0x%X\n", Half_Status, TotalPlayValue, TotalRecordValue, DealCount);
          }
       }
      else
        TwoToOne = 0;
     }
}
//---------------------------------------------------------------------------
void Cal_PcmTotalNum(INT8U pcmi_h, INT8U pcmi_l, INT8U pcmo_h, INT8U pcmo_l)
{
  INT8U i;
  for(i=(HALF_NUM - 1); i>0; i--)
   {
    RecPcmTotalNum[i] = RecPcmTotalNum[i-1];
    PlayPcmTotalNum[i] = PlayPcmTotalNum[i-1];
   }
  RecPcmTotalNum[0] = pcmi_h;
  RecPcmTotalNum[0] = (RecPcmTotalNum[0]<<8) + pcmi_l;

  PlayPcmTotalNum[0] = pcmo_h;
  PlayPcmTotalNum[0] = (PlayPcmTotalNum[0]<<8) + pcmo_l;
}
#endif
//---------------------------------------------------------------------------
