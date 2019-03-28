#include "includes.h"
#include "ComDrv.h"

#include "stdlib.h"

#define x_comm_h

#define CommonH
#include "common.h"

/*
****************************************
201309
������splash�����Ԥ����
201310
�����ӶԼ��̴����Ԥ����
****************************************
*/

#define _COMM_OPERATE   0

void dcomm1(void);
void dcomm2(void);
void CommSendBuff(INT8U comm_fd, INT8U *comm_buf, INT16U send_len);

void KeyTouchOperate(INT8U TouchNum); //������Ӧ
void KeyPreDeal(void); //����Ԥ��������Ч��
void KeyBackDeal(void); //������������Ч�� ������

INT8U CheckKeyPressAndLcd(void); //��鰴��������LCD
//---------------------------------------------------------------------------
void CommSendBuff(INT8U comm_fd, INT8U *comm_buf, INT16U send_len)
{
  INT16U i;
  for (i=0; i<send_len; i++)
   {
    if(comm_fd == 0)
     {
      while((inportb(UART0_TX_STATUS)&0x1)!=0x1);	// �ȴ����ͱ�����Ϊ��
      outportb(UART0_TX_DATA, comm_buf[i]);				// �����ַ�
     }
    if(comm_fd == 1)
     {
      while((inportb(UART1_TX_STATUS)&0x1)!=0x1);	// �ȴ����ͱ�����Ϊ��
      outportb(UART1_TX_DATA, comm_buf[i]);				// �����ַ�
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
                    LocalCfg.EncodeFrameSize = VIDEO_VGA; //�����С

                    //��ʼ¼����Ƶ
                    xtm_printf("1  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                    StartVideoClock();     //����Ƶ�ɼ�����ʱ��
                    xtm_printf("2  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                    StartRecVideo(LocalCfg.EncodeFrameSize);
                    xtm_printf("3  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                    StartEncodeVideo(LocalCfg.EncodeFrameSize);     //����Ƶ����
                    xtm_printf("4  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                    ClearWatchDog();
                    DelayMs(100);
                    StopEncodeVideo();
                    StopRecVideo();
                    StopVideoClock();            //�ر���Ƶ�ɼ�����ʱ��
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
//���� -
         /*  if(LocalCfg.BrightVolume > 0)
            {
             LocalCfg.BrightVolume --;
             Lcd_Power(1, LocalCfg.BrightVolume);
            }
                      */

                  break;
         case 'E':
                  xtm_printf("E\n");
 //���� +
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
                  Local.GpioWatchDogStarted = 0;  //��GPIO����߳�������WatchDog
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
                  StartVideoClock();     //����Ƶ�ɼ�����ʱ��
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
                  Set_Decode_Clock(1);  //������Ƶ����ʱ��  1  ��   0  ��
                  //outportb(TX_START, inportb(TX_START)|0x20);//ʹ��MJPEG�ж�
              //	outportb(EN_J_CONTROL2, 0x4f);//ԭͼ��30֡ÿ�룬����15֡ÿ��
                  StartVideoClock();     //����Ƶ�ɼ�����ʱ��
                  StartRecVideo(Local.Test_FrameSize);

                  Local.DecodeVideoWidth = 640;
                  Local.DecodeVideoHeight = 480;
              	  StartEncodeVideo(VIDEO_VGA);     //����Ƶ����

                  xtm_printf("dcomm1 2::EN_J_CONTROL = 0x%X\n", inportb(EN_J_CONTROL));

                  DisplayVideoWindow(0, 640, 0, 480, REMOTE_VIDEO);//REMOTE_VIDEO);   LOCAL_VIDEO
                  break;
         case 'Z':
                  xtm_printf("stop video1\n");
                  xtm_printf("dcomm1 3::EN_J_CONTROL = 0x%X\n", inportb(EN_J_CONTROL));
                  //outportb(TX_START, inportb(TX_START)&0xdf);//ʹ��MJPEG�ж�

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
void KeyTouchOperate(INT8U TouchNum) //������Ӧ
{
  if(CheckKeyPressAndLcd()) //��鰴��������LCD
    return;
  if(Local.KeyTouchNo == 0)
   {
    if(Local.Status == 0)
      PlaySoundTip(); //������ʾ��
    Local.KeyTouchNo = TouchNum + 1; //��ť���±�־
    KeyPreDeal(); //����Ԥ��������Ч��
    dtime[3] = KEYDELAYTIME;
   }
}
//---------------------------------------------------------------------------
void KeyPreDeal(void) //����Ԥ��������Ч��
{
  xtm_printf("KeyPreDeal::Local.CurrentWindow = %d\n", Local.CurrentWindow);
  switch(Local.CurrentWindow)
   {
    case 0:     //������Ԥ������������Ч��
           PreDealFirstWindow(Local.KeyTouchNo - 1);
           break;
    case 5:    //�������ô���Ԥ������������Ч��
           PreDealSetupWindow(Local.KeyTouchNo - 1);
           break;
    case 6: //splash����
		   //PreDealSplashWindow(Local.KeyTouchNo - 1);
		   break;
    case 7: //20131008����
		   PreDealEPWWindow(Local.KeyTouchNo - 1);
		   break;  
    case 12://��������
           PreDealTalkWindow(Local.KeyTouchNo - 1);
           break;
    case 13://���Ӵ���
           PreDealWatchWindow(Local.KeyTouchNo - 1);
           break;
    case 16:    //�Խ����봰��Ԥ������������Ч��
           PreDealTalkWatchPicWindow(Local.KeyTouchNo - 1);
           break;
    case 31:    //��������Ԥ������������Ч��
           PreDealCancelFortifyWindow(Local.KeyTouchNo - 1);
           break;
    case 32:    //��������Ԥ������������Ч��
           PreDealFortifyWindow(Local.KeyTouchNo - 1);
           break;
    case 35:    //ȡ����������Ԥ������������Ч��
           PreDealCancelAlarmWindow(Local.KeyTouchNo - 1);
           break;
    case 21:    //ͨ����¼����Ԥ������������Ч��
           PreDealRecordsWindow(Local.KeyTouchNo - 1);
           break;
    case 22:    //�鿴ͨ����¼����Ԥ������������Ч��
           PreDealRecordsContentWindow(Local.KeyTouchNo - 1);
           break;
	case 40://201402 NEW �������봰��
		   PreDealLTpwWindow(Local.KeyTouchNo - 1);
           break;
    case 88://20140315 ad window
           PreDealSmarkStreamWindow(Local.KeyTouchNo - 1);
           break;
    case 89://20140315 ad window
           PreDealSmarkStreamWindow(Local.KeyTouchNo - 1);
           break;
    case 111:    //��Ϣ����Ԥ������������Ч��
           //���δ��� 20130922
           PreDealInfoWindow(Local.KeyTouchNo - 1);
           break;
    case 112:    //��Ϣ�鿴����Ԥ������������Ч��
           //���δ��� 20130922
           PreDealInfoContentWindow(Local.KeyTouchNo - 1);
           break;
    case 152:    //����������Ԥ������������Ч��
           PreDealSetupMainWindow(Local.KeyTouchNo - 1);
           break;
    case 149:    //���̴���Ԥ������������Ч��
           PreDealKbWindow(Local.KeyTouchNo - 1);
           break;
    case 151:    //Lan���ô���Ԥ������������Ч��
           PreDealLanWindow(Local.KeyTouchNo - 1);
           break;
    case 154:    //�������ô���Ԥ������������Ч��
           PreDealAddrWindow(Local.KeyTouchNo - 1);
           break;
    case 165:    //��Ļ���ô���Ԥ������������Ч��
           PreDealScreenSetupWindow(Local.KeyTouchNo - 1);
           break;
    case 166:    //�������ڴ���Ԥ������������Ч��
           PreDealVoiceSetupWindow(Local.KeyTouchNo - 1);
           break;
    case 168:    //�޸����봰��Ԥ������������Ч��
           PreDealPassSetupWindow(Local.KeyTouchNo - 1);
           break;
    case 158:    //�汾����Ԥ������������Ч��
           PreDealVersionWindow(Local.KeyTouchNo - 1);
           break;
    case 190:    //У׼����������Ԥ������������Ч��
           PreDealCalibrateWindow(Local.KeyTouchNo - 1);
           break;
    case 192:    //���Դ���Ԥ������������Ч��
           //PreDealTestWindow(Local.KeyTouchNo - 1);
           break;
   }
}
//---------------------------------------------------------------------------
void KeyBackDeal(void) //������������Ч�� ������
{
  switch(Local.CurrentWindow)
   {
    case 0: //������
           OperateFirstWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 5: //��������
           OperateSetupWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
	case 6:
		   //OperateSplashWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
		   break;
    case 7://20131008 �������봰��
		   OperateEPWWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 12://��������
           OperateTalkWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 13://���Ӵ���
           OperateWatchWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 16://���봰��
           OperateTalkWatchPicWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 17://�������봰��
           break;

    case 31:    //��������
           OperateCancelFortifyWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 32:    //��������
           OperateFortifyWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 35:    //ȡ����������
           OperateCancelAlarmWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 21:    //ͨ����¼����
           OperateRecordsWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 22:    //�鿴ͨ����¼����
           OperateRecordsContentWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 40://201402 NEW �������봰��
		   OperateLTpwWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 88:
           OperateSmarkStreamWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 89:
           OperateSmarkStreamWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 111:    //��Ϣ����
           OperateInfoWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 112:    //��Ϣ�鿴����
           OperateInfoContentWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 152: //����������
           OperateSetupMainWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 149: //���̴���
           OperateKbWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 151: //LAN���ô���
           OperateLanWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 154: //�������ô���
           OperateAddrWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 165:    //��Ļ���ô���Ԥ������������Ч��
           OperateScreenSetupWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 166:    //�������ڴ���Ԥ������������Ч��
           OperateVoiceSetupWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 168: //�޸����봰��
           OperatePassSetupWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 158: //�汾����
           OperateVersionWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 190: //У׼����������
           OperateCalibrateWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 192: //���Դ��ڲ���
           //OperateTestWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
   }
}
//---------------------------------------------------------------------------
INT8U CheckKeyPressAndLcd(void) //��鰴��������LCD
{
  Local.KeyPressTime = 0;
  Local.LcdLightTime = 0;
  if(Local.LcdLightFlag == 0)
   {
    //����LCD����
    Lcd_Power(1, LocalCfg.BrightVolume);
    Local.LcdLightFlag = 1; //LCD�����־
    Local.LcdLightTime = 0; //ʱ��
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
