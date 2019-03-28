#include "includes.h"

#include "stdlib.h"

#define x_main_h

#define CommonH
#include "common.h"

/*
****************************************
201310
���Ӷ�д����ļ���
201311
���Ӷ�д���к��ļ���
****************************************
*/


void Init_LocalVar(void); //��ʼ�� Local �ṹ

void Init_Module_Clock(void); //������Ҫ�رո�ģ��ʱ��
void Set_VideoCapture_Clock(INT8U Type); //������Ƶ�ɼ�ʱ��  1  ��   0  ��
void Set_Encode_Clock(INT8U Type); //������Ƶ����ʱ��  1  ��   0  ��
void Set_Decode_Clock(INT8U Type); //������Ƶ����ʱ��  1  ��   0  ��
void Set_Disp_Clock(INT8U Type); //������ʾʱ��  1  ��   0  ��

void ResetLocalCfg(void);    //��λ�����ļ�

void SetPTC1(INT8U freq);
//freq  1  --  ��������16 --  16��Ƶ
void SetPTC2(INT8U freq);

INT8U *jpeg_bitstream;    //��̬jpeg ͼƬ
INT8U *mjpeg_stream[MAX_STREAM_NUM];   //mjpeg ��
INT16U mjpeg_stream_len[MAX_STREAM_NUM];   //mjpeg ��
INT8U Curr_Mjpeg_Stream;

INT8U *Mdata_Stream;      //��������
INT8U *Send_Stream;       //����
INT8U *AudioSend_Stream;       //��Ƶ���Ͱ�ͷStream
INT8U *VideoSend_Stream;       //��Ƶ���Ͱ�ͷStream
INT8U *Flash_Stream;      //Flash�洢

INT8U *Net_Stream[4];       //256K ����  ��������

INT8U *Memory6_Stream;     //0x60 0000 �ڴ��ַ
void Init_BitStream(void); //��ʼ�� bitstream

//�������ļ�
void ReadCfgFile(void);
//д�����ļ�
void WriteCfgFile(void);
//д���ݵ�Flash
void SaveToFlash(INT32U FlashAddr, INT8U *buf, INT16U length);
//��Flash������
void ReadFromFlash(INT32U FlashAddr, INT8U *buf, INT16U length);

void ReadAdFile(void);

void WriteAdFile(void);

void ReadSwitchFile(void);

void WriteSwitchFile(void);

//---------------------------------------------------------------------------
void Init_BitStream(void) //��ʼ�� bitstream
{
  //C-F���ǳ��򣬷���������0-1�����ݣ�2-3��4-7��8-B�ɷ���
  INT8U i;
  //ͼƬ���뻺��
  jpeg_bitstream = MK_FP(0x2000, 0);

  Mdata_Stream = MK_FP(0x3000, 0);         //2K
  Send_Stream = MK_FP(0x3000, 2048);       //2K
  send_mac_buf = MK_FP(0x3000, 4096);      //2K
  send_mac_buf2 = MK_FP(0x3000, 4096+1024);  //1K
  PcmI_Stream = MK_FP(0x3000, 6144);       // 8K

  AudioSend_Stream = MK_FP(0x3000, 6144 + 8192);                    //1K
  VideoSend_Stream = MK_FP(0x3000, 6144 + 8192 + 1024);             //1K
  Flash_Stream = MK_FP(0x3000, 6144 + 8192 + 2048);      //Flash�洢    4K
  for(i=0; i<UDPSENDMAX; i++)
   {
    Multi_Udp_Buff[i].isValid = 0;
    Multi_Udp_Buff[i].buf = MK_FP(0x3000, 6144 + 8192 + 2048 + 4096 + i*1536);
   }

  //��Ϣ���ݣ�������˶���Ϣ���������Ҫע��˴����ڴ����
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

  //6���ڴ�ʹ��, �轫64K ӳ�䵽 0xA000 �ſ�ʹ��
  Memory6_Stream = MK_FP(0xA000, 0);

  Curr_Mjpeg_Stream = 0;
}
//---------------------------------------------------------------------------
void Init_LocalVar(void) //��ʼ�� Local �ṹ
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
  
  Init_BitStream(); //��ʼ�� bitstream

  ReadCfgFile();    //�����������ļ�

  LocalCfg.MicVolume = 5;
  
  Local.NetLanStatus = 0;   //����״̬ 0 �Ͽ�  1 ��ͨ
  Local.OldNetSpeed = 100;  //�����ٶ�
  Local.NewInfo = 0;  //������Ϣ
  Local.NewRecPic = 0;  //������Ӱ
  
  Local.RecordPic = 0;  //����Ƭ  0 ����
  Local.IFrameCount = 0; //I֡����
  Local.IFrameNo = 125;    //���ڼ���I֡

  Local.ForceEndWatch = 0;  //�к���ʱ��ǿ�ƹؼ���


  Local.LcdLightFlag = 1; //LCD�����־
  Local.LcdLightTime = 0; //ʱ��
  Local.KeyPressTime = 0;

  LocalCfg.VoiceHint = 1;
  Local.OldVoiceHint = LocalCfg.VoiceHint;         //������ʾ  0 �ر�  1  ��
  Local.OldSpkVolume = LocalCfg.SpkVolume;         //SPK ����
  Local.OldMicVolume = LocalCfg.MicVolume;         //MIC ����
  Local.OldTalkVoiceOpened = LocalCfg.TalkVoiceOpened;         //�Խ�ʱ����  0 �ر�  1  ��
  Local.OldTalkVolume = LocalCfg.TalkVolume;        //�Խ�ʱ����
  for(i=0; i<3; i++)
    Local.OldScrSave[i] =  LocalCfg.ScrSave[i];        //��Ļ����  0 �ر�  1  ��
  if(LocalCfg.BrightVolume > 4)
    LocalCfg.BrightVolume = 3;  
  Local.OldBrightVolume = LocalCfg.BrightVolume;      //����
  Local.OldKeyVoice = LocalCfg.KeyVoice;          //������    0 �ر�  1  ��

/*  //��������
   for(i=0; i<MAXDEFENCENUM; i++)
    {
      Local.OldDefenceSetup[i].dType = LocalCfg.DefenceSetup[i].dType;
      Local.OldDefenceSetup[i].dDelay = LocalCfg.DefenceSetup[i].dDelay;
      Local.OldDefenceSetup[i].tType = LocalCfg.DefenceSetup[i].tType;
    }        */


  //20101116 ������ʱ��Ϊ 100S
  //�����ʱ
  LocalCfg.Out_DelayTime = 30;//100;
  //������ʱ
  LocalCfg.Alarm_DelayTime = 30;
  if((LocalCfg.Addr[0] != 'S')&&(LocalCfg.Addr[0] != 'B'))
    LocalCfg.Addr[0] = 'B';
  if((LocalCfg.Addr[0] == 'S')||(LocalCfg.Addr[0] == 'B'))
   {
    Local.AddrLen = 12;  //��ַ����  S 12  B 12 M 8 H 6
   }
  LocalCfg.Addr[Local.AddrLen] = '\0';
  LocalCfg.AddrVerifyMa[6] = '\0';
  
  Local.Weather[0] = 1;  //����Ԥ��
  Local.Weather[1] = 20;//0;
  Local.Weather[2] = 27;//0;

  Local.ReportSend = 0;  //�豸��ʱ����״̬�ѷ���
  Local.RandReportTime = 1;
  Local.ReportTimeNum  = 0;

  Local.nowvideoframeno = 1;   //��ǰ��Ƶ֡���
  Local.nowaudioframeno = 1;   //��ǰ��Ƶ֡���  

  Local.NoBreak = 0;   //����״̬ 1 ����  0 ����
  //����
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

  //����״̬
  LocalCfg.DefenceNum = 1;
  //20101028  ���� ������Ϣ

   for(j=0; j<5; j++)
    for(i=0; i<MAXDEFENCENUM; i++)
     {
      LocalCfg.Scene_Defence[j][i] = Scene_Defence1[j][i];
      if(LocalCfg.Scene_Defence[j][i] == 1)
       {
        LocalCfg.DefenceInfo[i][2] = 1;  // ����
        LocalCfg.DefenceInfo[i][3] = 0;  // �ޱ���
       }
      else
       {
        LocalCfg.DefenceInfo[i][2] = 0;  // ����
        LocalCfg.DefenceInfo[i][3] = 0;  // �ޱ���
       }
     }

   //���������Ϣ��ʼ��
   
   //��������   0  1  2 - ��ʱ����   3  4  5  ��ͨ����   6  7  24Сʱ����
   for(i=0; i<=2; i++)
    {
          LocalCfg.DefenceSetup_dType[i] = 3;   //��������     0 - �շ���  1 - ҹ����  2 - 24Сʱ����  3 - �Զ������
          LocalCfg.DefenceSetup_dDelay[i] = 5;  //��ʱʱ��     0 - 0��   1 - 5��  2 - 10��  3 - 15��  4 - 20��  5 - 25��  6 - 60��
          LocalCfg.DefenceSetup_tType[i] = 4;   //̽ͷ����     0 - �̸�  1 - ȼ��  2 - ����  3 - ����  4 - ����  5 - �Ŵ�  6 - ����
    }
   for(i=3; i<=5; i++)
    {
          LocalCfg.DefenceSetup_dType[i] = 3;   //��������     0 - �շ���  1 - ҹ����  2 - 24Сʱ����  3 - �Զ������
          LocalCfg.DefenceSetup_dDelay[i] = 0;  //��ʱʱ��     0 - 0��   1 - 5��  2 - 10��  3 - 15��  4 - 20��  5 - 25��  6 - 60��
          LocalCfg.DefenceSetup_tType[i] = 4;   //̽ͷ����     0 - �̸�  1 - ȼ��  2 - ����  3 - ����  4 - ����  5 - �Ŵ�  6 - ����
    }
   for(i=6; i<=7; i++)
   //for(i=0; i<=7; i++)
    {
          LocalCfg.DefenceSetup_dType[i] = 2;   //��������     0 - �շ���  1 - ҹ����  2 - 24Сʱ����  3 - �Զ������
          LocalCfg.DefenceSetup_dDelay[i] = 0;  //��ʱʱ��     0 - 0��   1 - 5��  2 - 10��  3 - 15��  4 - 20��  5 - 25��  6 - 60��
          LocalCfg.DefenceSetup_tType[i] = 4;   //̽ͷ����     0 - �̸�  1 - ȼ��  2 - ����  3 - ����  4 - ����  5 - �Ŵ�  6 - ����
    }

  DeltaLen = 9 + sizeof(struct talkdata1);  //���ݰ���Ч����ƫ����

  Local.Status = 0;  //״̬Ϊ����
  Local.TalkEndStatus = 0;  //״̬Ϊ����

  Local.AlarmByte[0] = 0x0;         //����
  Local.AlarmByte[1] = 0x0;         //����
  Local.DefenceDelayFlag = 0;    //������ʱ��־
  Local.DefenceDelayTime = 0;    //����
  for(i=0; i<MAXCOUNT; i++)
   {
    Local.AlarmDelayFlag[i] = 0;     //������ʱ��־
    Local.AlarmDelayTime[i] = 0;    //����
   }

  Local.IP_Group[0] = 0; //�鲥��ַ
  Local.IP_Group[1] = 0;
  Local.IP_Group[2] = 0;
  Local.IP_Group[3] = 0;

  Local.NsMultiAddr[0] = 238;    //NS�鲥��ַ
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

  Local.Test_OV7725FrameRate = 0;    //OV7725 ���֡��

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

  if(LocalCfg.Language == 2)  //����  0 -- ���ļ���   1 -- ���ķ���   2 -- Ӣ��
    Local.LanguageNo = 1;
  else
    Local.LanguageNo = 0;
  xtm_printf("Init_LocalVar 1\n");

  //��ʱ����ʾ��Ϣ��־
  PicStatBuf.Flag = 0;
  PicStatBuf.Type = 0;
  PicStatBuf.Time = 0;

  Local.Conflict_Flag = 0;    //IP��ַ��ͻ
  for(i=0; i<6; i++)
    Local.Conflict_Mac[i] = 0x0;

  Local.GpioWatchDogStarted = 1;  //��GPIO����߳�������WatchDog
  Local.RemoteDebugInfo = 0;      //����Զ�̵�����Ϣ

        LocalCfg.EncodeFrameSize = VIDEO_VGA; //�����С
        LocalCfg.VideoClosed = 1; //��Ƶ�ر�

 #ifdef _REMOTECALLTEST  //Զ�̺��в���
  Local.EnterTestMode = 0;  //����ģʽ, 5���Զ�����
  Local.CloseTestRing = 0;  //����ģʽ, �ر�����
 #endif
  //20130318 ����
  //LocalCfg.VideoSdramRefresh = 1;    //LCD��ʾʱ SDRAMˢ������  0  8����  1 4����
  if(LocalCfg.MicDecValue > 3) //չ�䡢չ��ģʽ,���С Mic����
    LocalCfg.MicDecValue = 0;  
}
//---------------------------------------------------------------------------
void ResetLocalCfg(void)    //��λ�����ļ�
{
  INT8U i, j;
        xtm_printf("ResetLocalCfg::\n");
        //ͷ��������
        xtm_memcopy(LocalCfg.FlashHead, "XXXCID", 6);
        //��ַ����
        xtm_memcopy(LocalCfg.Addr,NullAddr,20);
        xtm_memcopy(LocalCfg.Addr,"S00010108090",12);
        //������ͬ����
        strcpy(LocalCfg.AddrVerifyMa, "123456");
        //������ַ
        LocalCfg.Mac_Addr[0] = 0x00;
        LocalCfg.Mac_Addr[1] = 0x23;
        LocalCfg.Mac_Addr[2] = 0x60;
        LocalCfg.Mac_Addr[3] = 0x22;
        LocalCfg.Mac_Addr[4] = 0x41;
        LocalCfg.Mac_Addr[5] = 0x63;
        //IP��ַ
        LocalCfg.IP[0] = 198;
        LocalCfg.IP[1] = 168;
        LocalCfg.IP[2] = 0;
        LocalCfg.IP[3] = 163;
        //��������
        LocalCfg.IP_Mask[0] = 255;
        LocalCfg.IP_Mask[1] = 255;
        LocalCfg.IP_Mask[2] = 255;
        LocalCfg.IP_Mask[3] = 0;
        //���ص�ַ
        LocalCfg.IP_Gate[0] = 198;
        LocalCfg.IP_Gate[1] = 168;
        LocalCfg.IP_Gate[2] = 0;
        LocalCfg.IP_Gate[3] = 1;
        //NS�����ƽ�������������ַ
        LocalCfg.IP_NS[0] = 192;
        LocalCfg.IP_NS[1] = 168;
        LocalCfg.IP_NS[2] = 68;
        LocalCfg.IP_NS[3] = 1;
        //����������ַ
        LocalCfg.IP_Server[0] = 192;
        LocalCfg.IP_Server[1] = 168;
        LocalCfg.IP_Server[2] = 68;
        LocalCfg.IP_Server[3] = 1;
        //�㲥��ַ
        LocalCfg.IP_Broadcast[0] = 198;
        LocalCfg.IP_Broadcast[1] = 168;
        LocalCfg.IP_Broadcast[2] = 0;
        LocalCfg.IP_Broadcast[3] = 255;
        
        //�豸��ʱ����״̬ʱ��
        LocalCfg.ReportTime = 10;
        //����״̬
        LocalCfg.DefenceStatus = 0;
        //����ģ�����
        LocalCfg.DefenceNum = 1;
        for(i=0; i<8; i++)
         for(j=0; j<10; j++)
          LocalCfg.DefenceInfo[i][j] = 0;
        //��������
        strcpy(LocalCfg.EngineerPass, "123456");

        //��������   0  1  2 - ��ʱ����   3  4  5  ��ͨ����   6  7  24Сʱ����
        for(i=0; i<=2; i++)
         {
          LocalCfg.DefenceSetup_dType[i] = 3;   //��������     0 - �շ���  1 - ҹ����  2 - 24Сʱ����  3 - �Զ������
          LocalCfg.DefenceSetup_dDelay[i] = 5;  //��ʱʱ��     0 - 0��   1 - 5��  2 - 10��  3 - 15��  4 - 20��  5 - 25��  6 - 60��
          LocalCfg.DefenceSetup_tType[i] = 4;   //̽ͷ����     0 - �̸�  1 - ȼ��  2 - ����  3 - ����  4 - ����  5 - �Ŵ�  6 - ����
         }
        for(i=3; i<=5; i++)
         {
          LocalCfg.DefenceSetup_dType[i] = 3;   //��������     0 - �շ���  1 - ҹ����  2 - 24Сʱ����  3 - �Զ������
          LocalCfg.DefenceSetup_dDelay[i] = 0;  //��ʱʱ��     0 - 0��   1 - 5��  2 - 10��  3 - 15��  4 - 20��  5 - 25��  6 - 60��
          LocalCfg.DefenceSetup_tType[i] = 4;   //̽ͷ����     0 - �̸�  1 - ȼ��  2 - ����  3 - ����  4 - ����  5 - �Ŵ�  6 - ����
         }
        for(i=6; i<=7; i++)
         {
          LocalCfg.DefenceSetup_dType[i] = 2;   //��������     0 - �շ���  1 - ҹ����  2 - 24Сʱ����  3 - �Զ������
          LocalCfg.DefenceSetup_dDelay[i] = 0;  //��ʱʱ��     0 - 0��   1 - 5��  2 - 10��  3 - 15��  4 - 20��  5 - 25��  6 - 60��
          LocalCfg.DefenceSetup_tType[i] = 4;   //̽ͷ����     0 - �̸�  1 - ȼ��  2 - ����  3 - ����  4 - ����  5 - �Ŵ�  6 - ����
         }

        //������ʱ
        LocalCfg.In_DelayTime = 30;
        //�����ʱ
        LocalCfg.Out_DelayTime = 30;
        //������ʱ
        LocalCfg.Alarm_DelayTime = 30;

        //����
        for(i=0; i<8; i++)
          LocalCfg.SoundSetup[i] = 0;
        //�ÿ���Ӱ
        LocalCfg.RecPhoto = 0;
        //����ѡ��
        LocalCfg.Language = 1;
        //��������  0 ��ͨ��  1  ��ͨ��  2  ҡ����   3  ������
        LocalCfg.RingType = 0;
        //��������  0 ��ͨ��  1  ��ͨ��  2  ҡ����   3  ������
        LocalCfg.CallWaitRing = 0;
        //������ʾ  0 �ر�  1  ��
        LocalCfg.VoiceHint = 1;
        //SPK ����
        LocalCfg.SpkVolume = 2;
        //MIC ����
        LocalCfg.MicVolume = 2;
        //�Խ�ʱ����
        LocalCfg.TalkVoiceOpened = 1;
        LocalCfg.TalkVolume = 2;

        //��Ļ����
        LocalCfg.ScrSave[0] = 1;
        LocalCfg.ScrSave[1] = 0;
        LocalCfg.ScrSave[2] = 0;
        //��ҳ����  0 Ĭ�Ͻ���  1 �绰����
        LocalCfg.MainPage = 0;
        //����
        LocalCfg.BrightVolume = 2;
        //�Աȶ�
        LocalCfg.ContrastVolume = 2;
        //������    0 �ر�  1  ��
        LocalCfg.KeyVoice = 1;
        //������
        LocalCfg.Ts_X0 = 1996;//1901;
        LocalCfg.Ts_Y0 = 2005;//2001;
        LocalCfg.Ts_deltaX = 3685;//3744;
        LocalCfg.Ts_deltaY = -3112;//3555;
        LocalCfg.Ts_Mode = 0;     //X   Y  ����

        LocalCfg.Language = 0;  //����  0 -- ���ļ���   1 -- ���ķ���   2 -- Ӣ��

        LocalCfg.EncodeFrameRate = 3; //����֡��
        LocalCfg.EncodeFrameSize = VIDEO_VGA; //�����С
        LocalCfg.VideoClosed = 0; //��Ƶ�ر�
        LocalCfg.VideoOutput = 0;    //��Ƶ���  0  ����  1 �Լ�
        LocalCfg.VideoSdramRefresh = 0;    //LCD��ʾʱ SDRAMˢ������  0  8����  1 4����
        LocalCfg.MultiMachineSupport = 0;  //��ֻ�֧��,��������Ч���粻֧�֣��򲻷��㲥��ѯ��
        LocalCfg.DenMaxNum = 1;  //���������������һ������С�� SUBMAXNUM�� �����ڼ�С�㲥��
        //outportb(POWER_CONTROL1, 0x04);//��Դ���ƼĴ���1    4����
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
//freq  1  --  ��������16 --  16��Ƶ
void SetPTC2(INT8U freq)
{
  INT32U luminance;
  outportb(PTC2_CTRL0,0x41);
  luminance = CPU_MAIN_FREQ/100/freq;    //20120222 �ľ���

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
void Init_Module_Clock(void) //������Ҫ�򿪡��رո�ģ��ʱ��
{
#if 1
  //CONTROL_REG2
    //D4���ر�PTC0ʱ��
    //D5���ر�PTC1ʱ��
    //D6���ر�PTC2ʱ��
    //D7���ر�SPIʱ��

  //POWER_CONTROL2
    //D[7:0](R/W)
    //D0���ر�MJPEG��Ƶ�ɼ�ʱ��  30mA
    //D1���ر�MJPEG��Ƶ����ʱ��
    //D2���ر�MJPEG��Ƶ����ʱ��  30mA
    //D3���ر���ʾʱ��     20 mA
    //D4���ر�PCMʱ��      10 mA
    //D5���ر�MACʱ��      10 mA
    //D6���ر�UART0ʱ��
    //D7���ر�UART1ʱ��

           //xtm_printf("�ر� PTC0 ʱ��\n");
           //outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x10);
           xtm_printf("�� PTC0 clock\n");
           outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xEF);

           xtm_printf("�ر� PTC1 ʱ��\n");
           outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x20);
           //xtm_printf("�� PTC1 ʱ��\n");
           //outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xDF);

           //xtm_printf("�ر� PTC2 ʱ��\n");
           //outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x40);
           xtm_printf("�� PTC2 ʱ��\n");
           outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xBF);

           //xtm_printf("�ر� SPI ʱ��\n");
           //outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x80);
           xtm_printf("�� SPI ʱ��\n");
           outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0x7F);

           Set_VideoCapture_Clock(0);  //������Ƶ�ɼ�ʱ��  1  ��   0  ��

           Set_Encode_Clock(1); //������Ƶ����ʱ��  1  ��   0  ��

         #ifndef _CAMERA_SUPPORT     //�粻������ͷ��GPIOB6��7ΪComm1, GPIOC ��Ϊ8������ʹ��
           xtm_printf("�ر�MJPEG��Ƶ�ɼ�ʱ��\n");
           outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x01);
           xtm_printf("�ر�MJPEG��Ƶ����ʱ��\n");
           outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x02);
         #endif

           xtm_printf("��MJPEG��Ƶ����ʱ��\n");
           Set_Decode_Clock(1);  //������Ƶ����ʱ��  1  ��   0  ��

           Set_Disp_Clock(1); //������ʾʱ��  1  ��   0  ��

           //xtm_printf("�ر�PCMʱ��\n");
           //outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x10);
           xtm_printf("��PCMʱ��\n");
           outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xEF);

           //xtm_printf("�ر�MACʱ��\n");
           //outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x20);
           xtm_printf("��MACʱ��\n");
           outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xDF);

           //xtm_printf("�ر�UART0ʱ��\n");
           //outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x40);
           xtm_printf("��UART0ʱ��\n");
           outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xBF);

           xtm_printf("�ر�UART1ʱ��\n");
           outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x80);
           //xtm_printf("��UART1ʱ��\n");
           //outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0x7F);
#endif           
}
//---------------------------------------------------------------------------
void Set_VideoCapture_Clock(INT8U Type) //������Ƶ�ɼ�ʱ��  1  ��   0  ��
{
  if(Type)
   {
    xtm_printf("��MJPEG��Ƶ�ɼ�ʱ��\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xFE);
   }
  else
   {
    xtm_printf("�ر�MJPEG��Ƶ�ɼ�ʱ��\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x01);
   }   
}
//---------------------------------------------------------------------------
void Set_Encode_Clock(INT8U Type) //������Ƶ����ʱ��  1  ��   0  ��
{
//����ʱ�Ӳ����ظ�����   ���ڻ��ɹ�
#ifdef _CAMERA_SUPPORT     //�粻������ͷ��GPIOB6��7ΪComm1, GPIOC ��Ϊ8������ʹ��   ����ģʽ
  if(Type)
   {
    xtm_printf("��MJPEG��Ƶ����ʱ��\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xFD);

   }
  else
   {
    //xtm_printf("�ر�MJPEG��Ƶ����ʱ��\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x02);
   }
#endif   
}
//---------------------------------------------------------------------------
void Set_Decode_Clock(INT8U Type) //������Ƶ����ʱ��  1  ��   0  ��
{
  if(Type)
   {
    //xtm_printf("��MJPEG��Ƶ����ʱ��\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xFB);
    //outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x04);//ʹ��MJPEG�����ж�
   }
  else
   {
    //xtm_printf("�ر�MJPEG��Ƶ����ʱ��\n");
    WaitDecodeFinish();
    //outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xFB);//��ֹMJPEG�����ж�
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x04);
   }       
}
//---------------------------------------------------------------------------
void Set_Disp_Clock(INT8U Type) //������ʾʱ��  1  ��   0  ��
{
  if(Type)
   {
    xtm_printf("����ʾʱ��\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xF7);
   }
  else
   {
    xtm_printf("�ر���ʾʱ��\n");
    outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x08);
   }
}
//---------------------------------------------------------------------------
//�����������ļ�
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
      ResetLocalCfg();    //��λ�����ļ�
      WriteCfgFile();     //д���������ļ�
     } 
   }
  LocalCfg.Addr[12] = '\0';

  LocalCfg.BootNo ++;
  //WriteCfgFile(); 
}
//---------------------------------------------------------------------------
//д���������ļ�
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
//д���ݵ�Flash
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
//��Flash������
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
