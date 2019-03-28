#include "includes.h"

#include "stdlib.h"

#define x_timer_h

#define CommonH
#include "common.h"

INT16U Timer_Num = 0;  //����
void timer_thread_func(void);

void CheckBrightVolume(void); //�����������������ֵ
#ifdef _SEND_TO_SELF_TEST    //�������ݰ����Լ�
void SendToSelf(void);  //�Լ����Լ�����
#endif

void OnlineCheckFunc(void); //����ȷ�ϼ�⺯��
void TimeReportStatusFunc(void); //�豸��ʱ����״̬����
void ClearTipFunc(void);    //��״̬��ʾ��Ϣ����
void TalkCtrlFunc(void);  //�Խ����ƣ���ʾͨ��ʱ����жϳ�ʱ

//��������ź�
void CheckEth(void);

#ifdef _CAPTUREPIC_TO_CENTER  //����ͼƬ��������
INT8U head_640x480[0x26f] = {
0xff, 0xd8, 0xff, 0xe0, 0x00, 0x10, 0x4a, 0x46, 0x49, 0x46, 0x00, 0x01, 0x01, 0x02, 0x00, 0x76, 
0x00, 0x76, 0x00, 0x00, 0xff, 0xdb, 0x00, 0x43, 0x00, 0x0d, 0x09, 0x0a, 0x0b, 0x0a, 0x08, 0x0d, 
};
//��鲶��ͼƬ��������
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

    if((Timer_Num % (TIMERPERSEC*10))==0)
     {
      //xtm_printf("VideoCount = %d\n", VideoCount);
      CheckBrightVolume();
     }

    if((Timer_Num % (TIMERPERSEC*30))==0)
     {
      AddMultiGroup(Local.NsMultiAddr);

     #ifdef _SEND_TO_SELF_TEST    //�������ݰ����Լ�
      if(Local.Status == 0)
        SendToSelf();  //�Լ����Լ�����
     #endif
     }

    #ifdef _SEND_TO_SELF_TEST    //�������ݰ����Լ�
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

   #ifdef _CAPTUREPIC_TO_CENTER  //����ͼƬ��������
    if((Timer_Num % (TIMERPERSEC*2))==0)
      //��鲶��ͼƬ��������
      CheckCapturePicCenter();
   #endif

    //����6��û���յ�����ȷ�ϣ�����Ϊ����
    if(Local.OnlineFlag == 1)
     {
      OnlineCheckFunc();
      //�Խ����ƣ���ʾͨ��ʱ����жϳ�ʱ
      TalkCtrlFunc();
     }

    //��״̬��ʾ��Ϣ����
    ClearTipFunc();

    //�豸��ʱ����״̬����
     if(LocalCfg.ReportTime != 0)
      {
       if(Local.ReportSend == 1)
        if(Local.ReportTimeNum >= (LocalCfg.ReportTime*TIMERPERSEC))
         {
          Local.RandReportTime = random(LocalCfg.ReportTime);     //���������
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

     CheckOnlineTest(); //���߼����ʱ�ȴ�


}
//---------------------------------------------------------------------------
void CheckBrightVolume(void) //�����������������ֵ
{
  INT8U i, j;
  INT8U SaveOK = 0;


  //��������    
  if((Local.OldVoiceHint != LocalCfg.VoiceHint)
       ||(Local.OldSpkVolume != LocalCfg.SpkVolume)
       ||(Local.OldMicVolume != LocalCfg.MicVolume)
       ||(Local.OldTalkVoiceOpened != LocalCfg.TalkVoiceOpened)
       ||(Local.OldTalkVolume != LocalCfg.TalkVolume)
       ||(Local.OldBrightVolume != LocalCfg.BrightVolume)
       )
     {
      //xtm_printf("CheckBrightVolume:: Local.OldVoiceHint = %d, LocalCfg.VoiceHint = %d, Local.OldSpkVolume = %d, LocalCfg.SpkVolume = %d,\n");
      Local.OldVoiceHint = LocalCfg.VoiceHint;         //������ʾ  0 �ر�  1  ��
      Local.OldSpkVolume = LocalCfg.SpkVolume;         //SPK ����
      Local.OldMicVolume = LocalCfg.MicVolume;         //MIC ����
      Local.OldTalkVoiceOpened = LocalCfg.TalkVoiceOpened;         //�Խ�ʱ����  0 �ر�  1  ��
      Local.OldTalkVolume = LocalCfg.TalkVolume;        //�Խ�ʱ����
      Local.OldBrightVolume = LocalCfg.BrightVolume;      //����
      SaveOK = 1;
     }
  if(SaveOK == 1)
   {
    WriteCfgFile();  //д���������ļ�
   }
}
//---------------------------------------------------------------------------
//��״̬��ʾ��Ϣ����
void ClearTipFunc(void)
{
  INT8U CloseLcd;
  if(Local.Status != 0)
      Local.KeyPressTime = 0;
  //�粻�ڿ������棬���ʱ��30S�޲����Զ�����
  //���ں��кͲ�������    �����û����ӽ���  ���ڱ������� ����ȡ����������   ���� ���Խ���
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
  //���ڿ������棬60S�޲�����LCD
  if(Local.CurrentWindow == 0)
   {
    if(Local.LcdLightFlag == 1)
     {
      Local.LcdLightTime ++;
      if(Local.LcdLightTime > (CLOSELCD_TIME*TIMERPERSEC))
       {
        CloseLcd = 1;
        DisplayTalkWindow(REFRESH);
        Local.LcdLightTime = 0; //ʱ��
       #if 0   //20130217 ����LCD
        if(CloseLcd == 1)
         {
          if(Local.LcdLightFlag == 1)
           {
            //�ر�LCD
            xtm_printf("Lcd_Power 0\n");
            DisplayTalkWindow(REFRESH);
            Lcd_Power(0, LocalCfg.BrightVolume);
           }
          Local.LcdLightFlag = 0; //LCD�����־
          Local.LcdLightTime = 0; //ʱ��
        }
       #else  //20130413 ����ʱ��������
        if(CloseLcd == 1)
         {
          if(Local.LcdLightFlag == 1)
           {
            xtm_printf("Lcd_Power 0\n");
            Lcd_Power(1, 0);
           }
          Local.LcdLightFlag = 0; //LCD�����־
          Local.LcdLightTime = 0; //ʱ��
        }
       #endif
       }
     }
   }
  else
   {
    if(Local.LcdLightFlag == 0)
     {
      //����LCD����
      Lcd_Power(1, LocalCfg.BrightVolume);
     }
    Local.LcdLightFlag = 1; //LCD�����־
    Local.LcdLightTime = 0; //ʱ��
   }

  //��ʱ����ʾ�o�Ϣ��־
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
        case 7: //��������
                DisplayTalkWindow(REFRESH);
                Local.OnlineFlag = 0;
                Local.Status = 0;  //״̬Ϊ����
                break;
        case 8: //����ʧ�ܽ���
                DisplayTalkWindow(REFRESH);
                Local.OnlineFlag = 0;
                Local.Status = 0;  //״̬Ϊ����
                break;
        case 13:   //����
        case 16:   //��ʾ�Խ�ͼ���������
                DisplayTalkWindow(REFRESH);
                Local.OnlineFlag = 0;
                Local.Status = 0;  //״̬Ϊ����
                break;
       }
      PicStatBuf.Type = 0;
     }
   }
}
//---------------------------------------------------------------------------
void OnlineCheckFunc(void) //����ȷ�ϼ�⺯��
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
           ||(Local.Status == 7)||(Local.Status == 8)||(Local.Status == 9)||(Local.Status == 10)) //�Խ�
             TalkEnd_Func();
         if((Local.Status == 3)||(Local.Status == 4)||(Local.Status == 90))  //����
            {
             WatchEnd_Func();
             //PlaySoundTip();
            }
         
         //Ϊ��ֹ����һֱ��
         //StopPlayWavFile();  //�ر�����
         xtm_printf("û���յ�����ȷ�ϣ�ǿ�ƽ���, Local.Timer1Num = %d, Local.Status = %d, Local.CallConfirmFlag = %d\n", \
                     Local.Timer1Num, Local.Status, Local.CallConfirmFlag);
         if(Local.Status == 0)
          {
           Local.OnlineFlag = 0;
           Local.CallConfirmFlag = 0;
           StopRecVideo();
           DelayMs(50);
           StopEncodeVideo();

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
      //�Խ�ʱ���з���������ȷ�ϰ���ÿ��һ��
      //���ʱ���ط���������ȷ�ϰ���ÿ��һ��
    //  printf("Local.Status = %d\n", Local.Status);
      if((Local.Status == 2)||(Local.Status == 6)
        ||(Local.Status == 8)||(Local.Status == 10)
        ||(Local.Status == 3)|| (Local.Status == 90))
       {
        //ͷ��
        xtm_memcopy(sendb, UdpPackageHead, 6);
        //����
        if((Local.Status == 2)||(Local.Status == 6)||(Local.Status == 8)||(Local.Status == 10))  //�Խ�
          sendb[6] = VIDEOTALK;
        if(Local.Status == 3)  //����
          sendb[6] = VIDEOWATCH;

        sendb[7]=ASK;        //����
        sendb[8]=CALLCONFIRM;//ͨ������ȷ��
        //������
        if((Local.Status == 3)||(Local.Status == 90)) //����ʱ������Ϊ���з�
         {
          xtm_memcopy(sendb+9, LocalCfg.Addr, 20);
          xtm_memcopy(sendb+29, LocalCfg.IP, 4);
          xtm_memcopy(sendb+33, Remote.Addr[0], 20);
          xtm_memcopy(sendb+53, Remote.IP[0], 4);
         }
        //printf("Remote.IP[0] = %d.%d.%d.%d\n", Remote.IP[0][0], Remote.IP[0][1], Remote.IP[0][2], Remote.IP[0][3]);
        if((Local.Status == 2)||(Local.Status == 4)||(Local.Status == 6)
          ||(Local.Status == 8)||(Local.Status == 10))  //�Խ�ʱ������Ϊ���з�
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
        
        //ȷ�����
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
void TimeReportStatusFunc(void) //�豸��ʱ����״̬����
{
  INT8U k;
  INT16U sendlength;
  INT8U sendb[1520];

  //ͷ��
  xtm_memcopy(sendb, UdpPackageHead, 6);
  //����
  sendb[6] = REPORTSTATUS;

  sendb[7]=ASK;        //����
  //������
  xtm_memcopy(sendb+8, LocalCfg.Addr, 20);
  xtm_memcopy(sendb+28, LocalCfg.Mac_Addr, 6);

  sendb[34] = LocalCfg.DefenceStatus;
  sendb[35] = LocalCfg.DefenceNum;
  //Ĭ��Ϊ10������
  #define _DEFENCENUM  10
  for(k=0; k<10; k++)
    xtm_memcopy(sendb + 36 + _DEFENCENUM*k, LocalCfg.DefenceInfo[k], 10);

  sendlength = 36 + _DEFENCENUM*10;
  CreateMultiUdpBuff(LocalPort[0], MULTI_SEND, LocalCfg.IP_Server, sendb, sendlength, 0);
}
//---------------------------------------------------------------------------
void TalkCtrlFunc(void)  //�Խ����ƣ���ʾͨ��ʱ����жϳ�ʱ
{
  int CallTimeOut;
  //1S
  if((Local.TimeOut % TIMERPERSEC)==0)
   {
    switch(Local.Status)
     {
      case 1:  //���жԽ�
      case 2:  //���жԽ�
             CallTimeOut = CALLTIMEOUT;
             ShowTalkConnectImage(NOFULLPAGE); //������������
             //�ڱ�����ʱ����������Ӱ���ܣ���������ʱֵ�ȸ�����3S
             if((LocalCfg.Addr[0] == 'S')||(LocalCfg.Addr[0] == 'B'))
              {
               if(Local.isHost == '0')
                 CallTimeOut = CALLTIMEOUT;
               else
                 CallTimeOut = CALLTIMEOUT + 3*TIMERPERSEC;
              }
             if(Local.TimeOut > CallTimeOut)
              {
               //�鿴�Ƿ��������鲥����
               //DropMultiGroup(m_VideoSocket, NULL);

               //���г�ʱ
               CallTimeOut_Func();
              }
             break;
      case 5:  //����ͨ��
      case 6:  //����ͨ��
             //��ʱ
             ShowTalkWatchTime(2, NOFULLPAGE); //��ʾͨ��ʱ��

             if(Local.TimeOut > Local.TalkTimeOut)
              {

				//20140225
			   //TALKTIMEOUT = DelayTalkTimeOut[LocalCfg.TalkTimeOut]*TIMERPERSEC; //ͨ���ʱ��
			   Local.TalkTimeOut = TALKTIMEOUT; //ͨ���ʱ��

	
               TalkEnd_Func();
               if(Local.Status == 0)
                 Local.OnlineFlag = 0;

               xtm_printf("ͨ����ʱ\n");
              }
             break;
      case 3:  //����    ��ʱ
               break;
      case 4:  //������
             ShowTalkWatchTime(0, NOFULLPAGE); //��ʾ����ʱ��
             if(Local.TimeOut > WATCHTIMEOUT)
              {
               WatchEnd_Func();
               if(Local.Status == 0)
                 Local.OnlineFlag = 0;

               xtm_printf("���ӳ�ʱ\n");
              }
             break;
     case 90:
             ShowTalkWatchTime(0, NOFULLPAGE); //��ʾ����ʱ��
             if(Local.TimeOut > WATCHTIMEOUT)
              {
               WatchEnd_Func();
               
               if(Local.Status == 0)
                {
                 Local.OnlineFlag = 0;
                 PlaySoundTip();
                }

               //WatchEnd_ClearStatus();

               xtm_printf("���ӳ�ʱ\n");
              }
            break;
     }
    }
   Local.TimeOut ++;       //���ӳ�ʱ,  ͨ����ʱ,  ���г�ʱ�����˽���
}
//---------------------------------------------------------------------------
//��������ź�
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
  if(NewStatus != OldStatus)   //����״̬ 0 �Ͽ�  1 ��ͨ
   {
    Local.NetLanStatus = NewStatus;
    if (NewStatus == 0)
     {
      xtm_printf("Link down\n");
      //SetGpio(GPIOD_OUT_DATA, NET_LED_HIGH, SET_HIGH); //������LED
      if(Local.CurrentWindow == 6)
        DisplayImage(&net_image[0], FROMFILE, NOFULLPAGE);
     }

    if (NewStatus == 1)
     {
      xtm_printf("Link up\n");
      //SetGpio(GPIOD_OUT_DATA, NET_LED_LOW, SET_LOW); //������LED
      if(Local.CurrentWindow == 6)
        DisplayImage(&net_image[1], FROMFILE, NOFULLPAGE);
      //�������ARP
      SendFreeArp();
     }
   }
}
//---------------------------------------------------------------------------
#ifdef _SEND_TO_SELF_TEST    //�������ݰ����Լ�
void SendToSelf(void)  //�Լ����Լ�����
{
  INT16U sendlength, val;
  INT8U sendb[128];

  xtm_printf("SendToSelf\n");
  val = phy_reg_read(0, 0);
  val = val | 0x4000;
  phy_reg_write(0, 0, val);   //loopback

  //ͷ��
  xtm_memcopy(sendb, UdpPackageHead, 6);
  //����
  sendb[6] = SEND_SELF;

  sendb[7] = ASK;        //����
  //������
  xtm_memcopy(sendb+8, LocalCfg.Addr, 20);
  xtm_memcopy(sendb+28, LocalCfg.Mac_Addr, 6);

  sendlength = 34;
  CreateMultiUdpBuff(LocalPort[0], SINGLE_SEND, LocalCfg.IP, sendb, sendlength, 0);
}
#endif
//---------------------------------------------------------------------------
#ifdef _CAPTUREPIC_TO_CENTER  //����ͼƬ��������
//��鲶��ͼƬ��������
void CheckCapturePicCenter(void)
{
   INT8U i, j, k;
   INT16U sendlength;
   INT32U file_len;
   INT8U sendb[128];
   //xtm_printf("CheckCapturePicCenter::CapturePic.ConnectCentered = %d, CapturePic.SendToCetnering = %d\n", CapturePic.ConnectCentered, CapturePic.SendToCetnering);
   if(CapturePic.ConnectCentered == 0)  //����������״̬
     return;
   if(CapturePic.SendToCetnering == 1)  //���ڴ�������
     return;

   if(CapturePic.isVilid == 1)
     {
      CapturePic.FilePos = 0;
      //ͷ��
      xtm_memcopy(sendb, UdpPackageHead, 6);
      //����
      sendb[6] = CAPTUREPIC_SEND_START;

      sendb[7]=ASK;        //����
      //������
      xtm_memcopy(sendb+8, LocalCfg.Addr, 20);
      xtm_memcopy(sendb + 28, CapturePic.RemoteAddr, 20);  //Ŀ���ַ
      sendb[48] = 0;                                  //���к�
      sendb[49] = 0;
      sendb[50] = 0;
      sendb[51] = 0;

      sendb[52] = 0;   //ʱ��
      sendb[53] = 0;
      sendb[54] = 0;
      sendb[55] = 0;
      sendb[56] = 0;
      sendb[57] = 0;
      sendb[58] = 0;


      file_len = CapturePic.FileLen;
      file_len += 0x26F;
      file_len += 2;
      xtm_memcopy(sendb + 59, &file_len, sizeof(file_len));  //�ļ�����
      if((file_len % PACKDATALEN) == 0)
        CapturePic.Capture_Total_Package = ((INT32U)file_len / PACKDATALEN);
      else
        CapturePic.Capture_Total_Package = ((INT32U)file_len / PACKDATALEN) + 1;

      xtm_printf("CheckCapturePicCenter::CapturePic.FileLen = %d, file_len = %d, CapturePic.Capture_Total_Package = %d\n", (INT16U)CapturePic.FileLen, (INT16U)file_len, (INT16U)CapturePic.Capture_Total_Package);
      xtm_memcopy(sendb + 63, &CapturePic.Capture_Total_Package, sizeof(CapturePic.Capture_Total_Package));  //�ܰ���
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
   CapturePic.SendToCetnering = 1;  //���ڴ�������

   //ͷ��
   xtm_memcopy(sendb, UdpPackageHead, 6);
   //����
   sendb[6] = CAPTUREPIC_SEND_DATA;

   sendb[7] = ASK;        //����
   //������
   xtm_memcopy(sendb+8, LocalCfg.Addr, 20);
   xtm_memcopy(sendb + 28, CapturePic.RemoteAddr, 20);  //Ŀ���ַ
   sendb[48] = 0;                                  //���к�
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
   xtm_memcopy(sendb + 52, &CurrSize, sizeof(CurrSize));  //���ݳ���
   xtm_memcopy(sendb + 56, &CurrPack, sizeof(CurrPack));  //���ݰ����

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
     xtm_printf("���ͺ�����Ƭ->���ͽ���(�ɹ�) ����\n");
   else
     xtm_printf("���ͺ�����Ƭ->���ͽ���(ʧ��) ����\n");

   //ͷ��
   xtm_memcopy(sendb, UdpPackageHead, 6);
   //����
   if(Result == 1)
     sendb[6] = CAPTUREPIC_SEND_SUCC;
   else
     sendb[6] = CAPTUREPIC_SEND_FAIL;

   sendb[7]=ASK;        //����
   //������
   xtm_memcopy(sendb+8, LocalCfg.Addr, 20);
   xtm_memcopy(sendb + 28, CapturePic.RemoteAddr, 20);  //Ŀ���ַ
   sendb[48] = 0;                                  //���к�
   sendb[49] = 0;
   sendb[50] = 0;
   sendb[51] = 0;

   sendlength = 52;
   CreateMultiUdpBuff(LocalPort[0], MULTI_SEND, LocalCfg.IP_Server, sendb, sendlength, 0);
}
#endif
//---------------------------------------------------------------------------
