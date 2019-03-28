#include "includes.h"

#include "stdlib.h"

#define x_timer_h

#define CommonH
#include "common.h"

/*
****************************************
20131119
�����޸�
20140317 AdCtrlFunc
****************************************
*/


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
void AdCtrlFunc(void);

//��������ź�
void CheckEth(void);

#ifdef _MULTI_MACHINE_SUPPORT  //��ֻ�֧��
#define REFRESHTIME  30
void FindSubFunc(void); //Ϊ��������ʱ���Ҹ���
void SyncSubFunc(void); //Ϊ��������ʱ�븱��ͬ��״̬
void RefreshDen(void);  //���¸��������͵�ַ
INT8U RefreshDenFlag = 0;
#endif

//extern INT16U  VideoCount;
//---------------------------------------------------------------------------
void timer_thread_func(void)
{
    INT16U val;
    if(Local.CurrentWindow == 0)
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

    //����6��û���յ�����ȷ�ϣ�����Ϊ����
    if(Local.OnlineFlag == 1)
     {
      OnlineCheckFunc();
      //�Խ����ƣ���ʾͨ��ʱ����жϳ�ʱ
      TalkCtrlFunc();
     }

    if(Local.StreamOnlineFlag)
    {
       AdCtrlFunc();
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
     


    //Ϊ��������ʱ���Ҹ���  30S
    if(LocalCfg.MultiMachineSupport == 1) 
     {
      if(Local.isHost == '0')
       {
        if(Local.Status == 0)
         if((Timer_Num % (TIMERPERSEC*REFRESHTIME))==0)
          {

           if(LocalCfg.DenMaxNum != Local.DenNum)  //��󸱻������뵱ǰ������ͬ���������
            {
             RefreshDenFlag = 1;
             Local.TmpDenNum = 0; //���¸�������
             FindSubFunc();
            }
           Local.SendSyncFlag = 1;  //����ͬ����Ϣ��־
           Local.SendSyncTime = 0;  //����ͬ����Ϣ����
          }
        if(Local.SendSyncFlag == 1)
         {
          if(Local.SendSyncTime > TIMERPERSEC*2)
           {
            if(RefreshDenFlag == 1)
             {
              RefreshDenFlag = 0;
              RefreshDen();  //���¸��������͵�ַ
             }
            if(Local.DenNum > 0)
              SyncSubFunc(); //Ϊ��������ʱ�븱��ͬ��״̬
            Local.SendSyncFlag = 0;  //����ͬ����Ϣ��־
            Local.SendSyncTime = 0;  //����ͬ����Ϣ����
           }
          Local.SendSyncTime ++;
         }
       }
      else   //Ϊ����, ��ʱ�ж�����������
       {
        if(Local.ConnToHost == 1)
         if((Timer_Num % (TIMERPERSEC*REFRESHTIME))==0)
          {
           xtm_printf("Local.ConnToHostTime = %d\n", Local.ConnToHostTime);
           Local.ConnToHostTime ++;
           if(Local.ConnToHostTime > 3)
             Local.ConnToHost = 0;
          }
       }
     }
     #endif
    if((Timer_Num % (TIMERPERSEC))==0)
      ShowAlarmImage();










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
  if((Local.CurrentWindow != 0)&&(Local.CurrentWindow != 34)&&(Local.CurrentWindow != 35)&&(Local.CurrentWindow != 192)
    &&(Local.Status == 0))
   {
    Local.KeyPressTime ++;
    if(Local.KeyPressTime > (IDLE_TIME*TIMERPERSEC))   //10*30
     {
      Local.KeyPressTime = 0;
      Local.CurrentWindow = 0;                                             
      DisplayFirstWindow();
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

        if(CloseLcd == 1)
         {
          if(Local.LcdLightFlag == 1)
           {
            //�ر�LCD
            xtm_printf("Lcd_Power 0\n");
            Lcd_Power(0, LocalCfg.BrightVolume);
            if(Local.CurrentWindow != 0)
             {
              Local.CurrentWindow = 0;
              DisplayFirstWindow();
             }
           }
          Local.LcdLightFlag = 0; //LCD�����־
          Local.LcdLightTime = 0; //ʱ��
        }  
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

  //��ʱ����ʾ��Ϣ��־
  if(PicStatBuf.Flag == 1)
   {
    PicStatBuf.Time ++;
    //1S
    if(PicStatBuf.Time >= (TIMERPERSEC*1))
     {
      PicStatBuf.Flag = 0;
      PicStatBuf.Time = 0;
      switch(PicStatBuf.Type)
       {
  /*      case 13:   //����
                //�鿴�Ƿ��ǵ�ǰ����
                if(Local.CurrentWindow == PicStatBuf.Type)
                 {
                  //�������ӣ�����ʾ
                  strcpy(Label_Watch.Text, "");
                  ShowLabel(&Label_Watch, REFRESH, cBlack, 0);

                  //�б���
                  if((Local.AlarmByte[0] != 0)||(Local.AlarmByte[1] != 0))
                     DisplayAlarmWindow(); //��������
                  Local.Status = 0;  //״̬Ϊ����
                 }            
                break;      */
        case 16:   //��ʾ�Խ�ͼ���������
                  //�б���
                  xtm_printf("Local.AlarmByte[0] = 0x%X, Local.AlarmByte[1] = 0x%X\n", Local.AlarmByte[0], Local.AlarmByte[1]);
                  CloseVideoWindow();
                  if((Local.AlarmByte[0] != 0)||(Local.AlarmByte[1] != 0))
                     DisplayAlarmWindow(REFRESH_TOP); //��������
                  else
                   {
                    DisplayFirstWindow();
                   }
                  Local.OnlineFlag = 0; 
                  Local.Status = 0;  //״̬Ϊ����
                break;
       }
      PicStatBuf.Type = 0;

      Local.Status = 0;  //״̬Ϊ����
      Local.PreStatus = 0;  //20130115 Ԥ״̬����ֹͬʱ�Ĳ���      
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
    if(Local.Timer1Num >=(TIMERPERSEC*6))
     {
      if(Local.CallConfirmFlag == 0)
       {
         if((Local.Status == 1)||(Local.Status == 2)||(Local.Status == 5)||(Local.Status == 6)
           ||(Local.Status == 7)||(Local.Status == 8)||(Local.Status == 9)||(Local.Status == 10)
           ||(Local.Status == 89)) //�Խ�
            {
             TalkEnd_Func();
             //PlaySoundDoorBell();
            }
         if((Local.Status == 3)||(Local.Status == 4))  //����
             WatchEnd_Func();
         //Ϊ��ֹ����һֱ��
         //StopPlayWavFile();  //�ر�����
         xtm_printf("û���յ�����ȷ�ϣ�ǿ�ƽ���, Local.Timer1Num = %d, Local.Status = %d, Local.CallConfirmFlag = %d\n", \
                     Local.Timer1Num, Local.Status, Local.CallConfirmFlag);         
         if(Local.Status == 0)
          {
           Local.OnlineFlag = 0;
           DisplayFirstWindow();
          }
       }
      else
        Local.CallConfirmFlag = 0;
      Local.Timer1Num = 0;
     }
    else
     if((Local.Timer1Num % TIMERPERSEC)==0)
     {
      //�Խ�ʱ���з���������ȷ�ϰ���ÿ��һ��
      //���ʱ���ط���������ȷ�ϰ���ÿ��һ��
    //  printf("Local.Status = %d\n", Local.Status);
      if((Local.Status == 2)||(Local.Status == 6)
        ||(Local.Status == 8)||(Local.Status == 10)
        ||(Local.Status == 3) || (Local.Status == 89))
       {
        //ͷ��
        xtm_memcopy(sendb, UdpPackageHead, 6);
        //����
        if((Local.Status == 2)||(Local.Status == 6)||(Local.Status == 8)||(Local.Status == 10)||(Local.Status == 89))  //�Խ�
          sendb[6] = VIDEOTALK;
        if(Local.Status == 3)  //����
          sendb[6] = VIDEOWATCH;

        sendb[7]=ASK;        //����
        sendb[8]=CALLCONFIRM;//ͨ������ȷ��
        //������
        if(Local.Status == 3) //����ʱ������Ϊ���з�
         {
          xtm_memcopy(sendb+9, LocalCfg.Addr, 20);
          xtm_memcopy(sendb+29, LocalCfg.IP, 4);
          xtm_memcopy(sendb+33, Remote.Addr[0], 20);
          xtm_memcopy(sendb+53, Remote.IP[0], 4);
         }
        //printf("Remote.IP[0] = %d.%d.%d.%d\n", Remote.IP[0][0], Remote.IP[0][1], Remote.IP[0][2], Remote.IP[0][3]);
        if((Local.Status == 2)||(Local.Status == 4)||(Local.Status == 6)||(Local.Status == 8)||(Local.Status == 10)||(Local.Status == 89))  //�Խ�ʱ������Ϊ���з�
         {
          xtm_memcopy(sendb+9, Remote.Addr[0], 20);
          xtm_memcopy(sendb+29, Remote.IP[0], 4);
          xtm_memcopy(sendb+33, LocalCfg.Addr, 20);
          xtm_memcopy(sendb+53, LocalCfg.IP, 4);
         }

        if(Local.Status == 89)
         {
            xtm_memcopy(sendb, UdpPackageHead2, 4);
            sendb[4] = 8;
            sendb[5] = SMART_P2P;
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

                if(Local.Status == 89)
                {
                    CreateMultiUdpBuff(LocalPort[3], SINGLE_SEND, Remote.IP[0], sendb, sendlength, 0);
                    //PlaySoundTip();
                }
                else
                {
                    CreateMultiUdpBuff(LocalPort[1], SINGLE_SEND, Remote.IP[0], sendb, sendlength, 0);
                }


       // CreateMultiUdpBuff(LocalPort[1], SINGLE_SEND, Remote.IP[0], sendb, sendlength, 0);

      /*  DenIP[0] = 192;
        DenIP[1] = 168;
        DenIP[2] = 68;
        DenIP[3] = 98;
        CreateMultiUdpBuff(LocalPort[1], SINGLE_SEND, DenIP, sendb, sendlength);    */
       }
     }
    Local.Timer1Num ++;
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
               Local.TimeOut = 0;
              }

            #ifdef _REMOTECALLTEST  //Զ�̺��в���
             if(Local.EnterTestMode == 1)  //����ģʽ, 5���Զ�����
              //����5���Զ�����
              if(Local.Status == 2)
               if(Local.TimeOut > 5*TIMERPERSEC)
                {
                 Local.EnterTestMode = 0;  //����ģʽ, 5���Զ�����;
                 //���жԽ�
                 OperateTalkWatchPicWindow(0, 16);
                 break;
                }
            #endif
             break;
      case 5:  //����ͨ��
      case 6:  //����ͨ��
             //��ʱ
               /*sprintf(talkpic_osd.Text, "%02d:%02d\0", Local.TimeOut/TIMERPERSEC/60,
                      (Local.TimeOut/TIMERPERSEC)%60);
               SetOsdPos(&talkpic_osd);
               ShowOsd(0, &talkpic_osd, REFRESH);  */

             if(Local.TimeOut > Local.TalkTimeOut)
              {
               Local.TalkTimeOut = TALKTIMEOUT; //ͨ���ʱ��
               TalkEnd_Func();
               if(Local.Status == 0)
                 Local.OnlineFlag = 0;
               Local.TimeOut = 0;
               xtm_printf("ͨ����ʱ\n");
              }
             break;
      case 3:  //����    ��ʱ
               if(Local.TimeOut > WATCHTIMEOUT)
                {
                 WatchEnd_Func();
                 if(Local.Status == 0)
                   Local.OnlineFlag = 0;
                 Local.TimeOut = 0;
                 xtm_printf("���ӳ�ʱ\n");
                }
               break;
      case 4:  //������
             if(Local.TimeOut > WATCHTIMEOUT)
              {
               WatchEnd_Func();
               if(Local.Status == 0)
                 Local.OnlineFlag = 0;
               Local.TimeOut = 0;
               xtm_printf("���ӳ�ʱ\n");
              }
             break;
      case 89:
             if(Local.TimeOut > ADTIMEOUT)
             {
               Local.TalkTimeOut = ADTIMEOUT; //ͨ���ʱ��
               TalkEnd_Func();
               if(Local.Status == 0)
                 Local.OnlineFlag = 0;
               Local.TimeOut = 0;
               xtm_printf("ͨ����ʱ\n");
              }
             break;
     }
    }
   Local.TimeOut ++;       //���ӳ�ʱ,  ͨ����ʱ,  ���г�ʱ�����˽���
}
//---------------------------------------------------------------------------
void AdCtrlFunc(void)  //�Խ����ƣ���ʾͨ��ʱ����жϳ�ʱ
{
  int ATimeOut;
  if((Local.TimeOut % TIMERPERSEC)==0)
  {
        if(Local.TimeOut > Local.StreamTimeOut)
        {
               Local.StreamTimeOut= ADTIMEOUT; //ͨ���ʱ��

                StopEncodeVideo();
                StopRecVideo();
                StopVideoClock();            //�ر���Ƶ�ɼ�����ʱ��
                StopVideoStreamDecode();     //�ر���Ƶ������
                StopRecAudio();
                StopPlayAudio();
                CloseVideoWindow();
                Local.Status = 0; 
                
				DisplayFirstWindow();
               if(Local.Status == 0)
                {
                 Local.StreamOnlineFlag= 0;
                }
               Local.TimeOut = 0;
              
        }
  }
  Local.TimeOut ++;       //���ӳ�ʱ,  ͨ����ʱ,  ���г�ʱ�����˽���,ad
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
      SetGpio(GPIOD_OUT_DATA, NET_LED_HIGH, SET_HIGH); //������LED
      if(Local.CurrentWindow == 0)
      	{
      	   #ifdef MAIN_SERIALNO
	  	   if(Local.WorkStatus)
	  	   {
              DisplayImage(&state_image[0], FROMFILE, NOFULLPAGE);
	  	   }
		   #else
		   DisplayImage(&state_image[0], FROMFILE, NOFULLPAGE);
		   #endif
		
      	}
     }

    if (NewStatus == 1)
     {
      xtm_printf("Link up\n");
      SetGpio(GPIOD_OUT_DATA, NET_LED_LOW, SET_LOW); //������LED
      if(Local.CurrentWindow == 0)
      {
	  	   #ifdef MAIN_SERIALNO
	  	   if(Local.WorkStatus)
	  	   {
              DisplayImage(&state_image[1], FROMFILE, NOFULLPAGE);
	  	   }
		   #else
		   DisplayImage(&state_image[1], FROMFILE, NOFULLPAGE);
		   #endif

           if(Local.CloudStatus)
           {
               DisplayImage(&state_image[8], FROMFILE, NOFULLPAGE);
           }
           else
           {
               DisplayImage(&state_image[7], FROMFILE, NOFULLPAGE);
           }

           
	  }
      //�������ARP
      SendFreeArp();
     }
   }
}
//---------------------------------------------------------------------------
#ifdef _MULTI_MACHINE_SUPPORT  //��ֻ�֧��
void FindSubFunc(void) //Ϊ��������ʱ���Ҹ���
{
  INT8U i, j, sendlength, isExist;
  INT8U sendb[128];
  Local.TmpDenNum = 0;
  //�ݶ��������Ϊ3��
  for(j=0; j<LocalCfg.DenMaxNum; j++)
   {
      //ͷ��
      xtm_memcopy(sendb, UdpPackageHead, 6);
      //����  ,�����㲥����
      sendb[6] = HOSTTOSUB;
      sendb[7] = ASK;    //����
      sendb[8] = SUBFIND;

      xtm_memcopy(sendb+9, LocalCfg.Addr,20);
      xtm_memcopy(sendb+29, LocalCfg.IP,4);
      xtm_memcopy(sendb+33, LocalCfg.Addr, 20);
      if(LocalCfg.Addr[0] == 'S')
        sendb[33+11] = j + '1';
      if(LocalCfg.Addr[0] == 'B')
        sendb[33+11] = j + '1';
      sendb[53] = 0;
      sendb[54] = 0;
      sendb[55] = 0;
      sendb[56] = 0;

      sendlength = 57;

      //���Ҹø����Ƿ��Ѿ����ڣ�����������跢�Ͳ�������
      isExist = 0;
      for(i=0; i<Local.DenNum; i++)
       {
        //xtm_printf("Local.DenAddr[%d][11] = %d, sendb[44] = %d\n", i, Local.DenAddr[i][11] - '0', sendb[44] - '0');
        if(Local.DenAddr[i][11] == sendb[44])
         {
          isExist = 1;
          break;
         }
       }
      if(!isExist)
        CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, LocalCfg.IP_Broadcast, sendb, sendlength, 0);
   }
}
//---------------------------------------------------------------------------
INT8U RefreshCount = 0;
void RefreshDen(void)  //���¸��������͵�ַ
{
  INT8U i,j;
  INT8U tmp_i;
  INT8U DenChange;
  DenChange = 0;

  if(Local.TmpDenNum > Local.DenNum)
    DenChange = 1;
  if(Local.TmpDenNum < Local.DenNum)
   {
    RefreshCount ++;
    if(RefreshCount > 3)
      DenChange = 1;
   }
  if(Local.TmpDenNum == Local.DenNum)
    for(i=0; i<Local.TmpDenNum; i++)
     if(strcmp(Local.DenAddr[i], Local.TmpDenAddr[i]) != 0)
      {
       DenChange = 1;
       break;
      }
  if(Local.TmpDenNum == Local.DenNum)
    for(i=0; i<Local.TmpDenNum; i++)
     if((Local.DenIP[i][0] != Local.TmpDenIP[i][0])||(Local.DenIP[i][1] != Local.TmpDenIP[i][1])
        ||(Local.DenIP[i][2] != Local.TmpDenIP[i][2])||(Local.DenIP[i][3] != Local.TmpDenIP[i][3]))
      {
       DenChange = 1;
       break;
      }
  if(DenChange == 1)
   {
    RefreshCount = 0;
    tmp_i = 0;
    for(i=0; i<SUBMAXNUM; i++)
     {
      if(tmp_i >= Local.TmpDenNum)
        break;
      for(j=0; j<Local.TmpDenNum; j++)
       {
        if(Local.TmpDenAddr[j][Local.AddrLen-1] == ('1' + i))
         {
          xtm_memcopy(Local.DenAddr[tmp_i], Local.TmpDenAddr[j], sizeof(Local.DenAddr[tmp_i]));
          xtm_memcopy(Local.DenIP[tmp_i], Local.TmpDenIP[j], sizeof(Local.DenIP[tmp_i]));
          Local.DenFlag[tmp_i] = 1;
          tmp_i ++;
          break;
         }
       }
     }
    Local.DenNum = Local.TmpDenNum;
   }
  if(Local.DenNum > 0)
   {
     xtm_printf("RefreshDen::Local.DenNum = %d, Local.DenIP[0] = %d.%d.%d.%d, Local.DenIP[1] = %d.%d.%d.%d\n",
             Local.DenNum, Local.DenIP[0][0], Local.DenIP[0][1], Local.DenIP[0][2], Local.DenIP[0][3],
                Local.DenIP[1][0], Local.DenIP[1][1], Local.DenIP[1][2], Local.DenIP[1][3]);
   }
}
//---------------------------------------------------------------------------
void SyncSubFunc(void) //Ϊ��������ʱ�븱��ͬ��״̬
{
  INT8U i,j;
  INT16U sendlength;
  INT8U sendb[1024];
  struct SyncSubData1 SubData1;
  if(Local.DenNum > 0)
   {
    SubData1.DenNum = Local.DenNum;             //Ŀ������  ����
    xtm_memcopy(SubData1.DenIP, Local.DenIP, sizeof(Local.DenIP));    //����IP
    xtm_memcopy(SubData1.DenAddr, Local.DenAddr, sizeof(Local.DenAddr));    //����Addr
               //�������� ��������  *************
    SubData1.DefenceStatus = LocalCfg.DefenceStatus;       //����״̬
    SubData1.DefenceNum = LocalCfg.DefenceNum;          //����ģ�����
    xtm_memcopy(SubData1.DefenceInfo, LocalCfg.DefenceInfo, sizeof(LocalCfg.DefenceInfo));    //������Ϣ

    xtm_memcopy(SubData1.EngineerPass, LocalCfg.EngineerPass, sizeof(LocalCfg.EngineerPass));    //��������
//    printf("syncsub1 size = %d\n", sizeof(struct SyncSubData1));

    //�ݶ��������Ϊ3��
    for(j=0; j<Local.DenNum; j++)
      {
       //ͷ��
       xtm_memcopy(sendb, UdpPackageHead, 6);
       //����  ,�����㲥����
       sendb[6] = HOSTTOSUB;
       sendb[7] = ASK;    //����
       sendb[8] = SYNCSUB;    //ͬ��״̬
       sendb[9] = 1;    //���ݰ����

       xtm_memcopy(sendb+10, LocalCfg.Addr,20);
       xtm_memcopy(sendb+30, LocalCfg.IP,4);

       xtm_memcopy(sendb+34, &SubData1, sizeof(struct SyncSubData1));
       sendlength = 34 + sizeof(struct SyncSubData1);

       CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Local.DenIP[j], sendb, sendlength, 0);
      }
   }
}
#endif
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
