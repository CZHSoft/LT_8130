#include "includes.h"

#include "stdlib.h"

#define x_test_h

#define CommonH
#include "common.h"

#define _TEST_PAGE_NUM  4

#define _TEST_LABEL_NUM  20
char VideoFrameSize[2][10] = {"VGA", "QVGA"};
char VideoClosed_Text[2][10] = {"�ر�", "��"};
char CameraExist_Text[2][10] = { "δ��⵽", "��⵽"};
char I2C_Text[2][10] = { "����", "�쳣"};
INT8U Freq_Text[3] = { 16, 32, 64};
char SDRAM_Text[2][10] = { "8����", "4����"};
char Bright_Text[6][10] = { "0", "1", "2", "3", "4", "5"};
char CameraMode_Text[2][10] = { "����", "�Լ�"};
char OV7725Frame_Text[3][10] = { "30֡", "25֡", "20֡"};
#define MAXSOUNDNUM   5//3
INT8U SoundNo = 0;
//char SoundName[MAXSOUNDNUM][50] = {"123456789 ����ͣ", "987654321 ����", "123-456-789"};
char SoundName[MAXSOUNDNUM][50] = {"300Hz-9dB", "600Hz-9dB", "1000Hz-9dB", "2000Hz-9dB", "3000Hz-9dB"};
char test_label[_TEST_LABEL_NUM][100] = {
                           {"1. �� ���� ��, ��Ļ���� �� �� �� �� �� �� �� �� �� �л�"},
                           {"2. �� ���� ��, �ɸ��ı���֡��, ��ǰ֡��: "},
                           {"3. �� ���� ��, �ɸ��ı����С, ��ǰ��С: "},
                           {"4. �� ���� ��, �ɴ򿪹ر�Camera(�Խ�ͨ��ʱ), ��ǰ״̬: "},
                           {"5. �� ���� ��, ������Cameraͼ�����ģʽ, ��ǰģʽ: "},

                           {"6. �� ���� ��, ����ʾ������Ƶ(������): "},
                           {"7. �� ���� ��, ����ʾ������Ƶ(���벢����): "},
                         /*  {"8. �� ���� ��, �ɵ��β�������, ѭ�� ��, ��ѭ����������: "},   */
                           {"8. �� ����ѡ�� ��, ��ѡ�񲥷�����, ѭ�� ��, ��ѭ����������: "},
                           {"9. �� ¼�� ��, ��¼��, ���� ��, �ɲ���¼��: "},
                           {"10. �� ���� ��, �����÷�������, ��ǰ����: "},

                           {"11. �� ���� ��, ������¼������, ��ǰ����: "},
                           {"12. �� ���� ��, �ɲ���OV7725 I2C��д: "},
                           {"13. �� ���� ��, �ɲ���ES8328E I2C��д: "},
                           {"14. �� ���� ��, �����õ͹��Ĵ�����Ƶ��: "},
                           {"15. �� ���� ��, �ɽ���͹��Ĵ���ģʽ: "},

                           {"16. �� ѭ�� ��, �ɽ��� �͹��Ĵ��� ѭ��ģʽ����(100��): "},
                           {"17. �� ���� ��, ������LCD��ʾʱ SDRAMˢ������, ��ǰ����: "},
                           {"18. �� ���� ��, �ɵ���LCD����, ��ǰ����ֵ: "},
                           {"19. �� �ø�  �õ� ��, ������GPIOC �ĸߵ͵�ƽֵ, Camera ���ָ�: "},
                           {"20. �� ����  ��, ������OV7725 ���֡��: "},
                          };                              //4����  8����
INT16U test_label_xLeft[_TEST_LABEL_NUM] = {40,  40,  40,  40,  40,   40,  40,  40,  40,  40,   40,   40,  40,  40,  40,   40,  40,  40,  40, 40};
INT16U test_label_yTop[_TEST_LABEL_NUM] =  {20, 110, 200, 290, 380,   20, 110, 200, 290, 380,   20,  110, 200, 290, 380,   20,  110, 200, 290, 380};

INT16U  Disp_Color[9] = {COLOR_BLACK, COLOR_WHITE, COLOR_RED, COLOR_ORANGE, COLOR_YELLOW, COLOR_GREEN, COLOR_CYAN, COLOR_BLUE, COLOR_PURPLE};
INT16U  Disp_Bit;
//
//INT16U test_button_xLeft[_TEST_BTN_NUM] = {670, 670, 670, 670, 670, 670,   \
//                                           60,   60,  60,  60,  60,   \
//                                           60,   60,  60,  200,   60,  200, 60, \
//                                           60,   60,   60,  60,  60, \
//                                           60,   60,  60,   60, 200, 340, 480,   60};
//INT16U test_button_yTop[_TEST_BTN_NUM] =  {50, 120, 190, 260, 320, 390,    \
//                                           40,   130,   220,   310,   400,   \
//                                           40,   130,   220, 220,     310, 310,   400,  \
//                                           40,   130,   220,   310,  400, \
//                                           40,   130,  220,  310, 310, 310, 310,  390};
//char test_button_name[_TEST_BTN_NUM][10] = {"��ʼ", "ֹͣ", "��ҳ", "��ҳ", "����", "����",  \
//                                            "����", "����", "����", "����", "����",    \
//                                      /*      "����", "����", "����",  "ѭ��", "¼��", "����", "����", \    */
//                                            "����", "����", "����ѡ��",  "ѭ��", "¼��", "����", "����", \
//                                            "����", "����", "����",  "����", "����", \
//                                            "ѭ��", "����", "����", "�ø�", "�õ�", "����ͷ", "��",  "����" };
void DisplayTestWindow(INT8U RefreshType);    //��ʾ���Դ���
void PreDealTestWindow(INT8U wintype);    //���Դ���Ԥ��������������Ч��
void OperateTestWindow(INT8U wintype, INT8U currwindow);    //���Դ��ڲ���
void CheckTouchTestWindow(INT16U XLCD, INT16U YLCD); //��鴥��������
void ShowTestPage(INT8U FullPageType);
INT8U CheckTestBusy(void);
//---------------------------------------------------------------------------
void DisplayTestWindow(INT8U RefreshType)    //��ʾ���Դ���
{
  INT8U i;

  for(i=1; i<_TEST_BTN_NUM; i++)
    test_button[i] = test_button[0];

  DisplayImage(&test_image, FROMFILE, FULLPAGE);
  WaitDecodeFinish();

  if(RefreshType == REFRESH)
    Local.Test_CurrPage = 0;  //��ǰҳ



  //��ť
  for(i=0; i<_TEST_BTN_NUM; i++)
   {
    //strcpy(test_button[i].buttonname, test_button_name[i]);
    //test_button[i].xLeft = test_button_xLeft[i];
    //test_button[i].yTop = test_button_yTop[i];
    test_button[i].FontType = 24;
    test_button[i].FontColor = COLOR_BLACK;
    test_button[i].Text_xLeft = (test_button[i].width - strlen(test_button[i].buttonname)*12)/2;
    test_button[i].Text_yTop = (test_button[i].height - 24)/2;
   }

   for(i=0; i<6; i++)
    {
     DisplayImageButton(&test_button[i], IMAGEUP, FROMFILE, FULLPAGE);
     DisplayImageButton_Text(&test_button[i], test_button[i].FontColor, 2, FULLPAGE);
    }


  Disp_Bit = 0x0001;

  Local.Test_Disp_No = 0;   //���Դ��� ��ʾ����

  Local.Test_LocalWatchFlag = 0; //������Ƶ����
  Local.Test_EncodeWatchFlag = 0; //������Ƶ����������ʾ
  Local.Test_PlayAudioFlag = 0; //��������
  Local.Test_RecAudioFlag = 0; //¼��
  Local.Test_FrameRate = LocalCfg.EncodeFrameRate; //����֡��
  Local.Test_FrameSize = LocalCfg.EncodeFrameSize; //�����С
  Local.Test_VideoClosed = LocalCfg.VideoClosed; //��Ƶ�ر�
  Local.Test_VideoOutput = LocalCfg.VideoOutput;    //��Ƶ���  0  ����  1 �Լ�
  Local.Test_PowerStayOnFreq = 1;    //�͹��Ĵ�����Ƶ��
  Local.Test_VideoSdramRefresh = LocalCfg.VideoSdramRefresh;    //LCD��ʾʱ SDRAMˢ������  0  8����  1 4����

  ShowTestPage(FULLPAGE);
  Local.CurrentWindow = 192;

  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);  
}
//---------------------------------------------------------------------------
void ShowTestPage(INT8U FullPageType)
{
  INT8U i;
  char tmp_text[10];

  DisplayImage(&test_image1, FROMFILE, FullPageType);
  WaitDecodeFinish();
  switch(Local.Test_CurrPage)
   {
    case 0:
           for(i=6; i<11; i++)
            {
             DisplayImageButton(&test_button[i], IMAGEUP, FROMFILE, FullPageType);
             DisplayImageButton_Text(&test_button[i], test_button[i].FontColor, 2, FullPageType);
             test_button[i].Visible = 1;
            }
           for(i=11; i<18; i++)
             test_button[i].Visible = 0;
           for(i=18; i<23; i++)
             test_button[i].Visible = 0;
           for(i=23; i<_TEST_BTN_NUM; i++)
             test_button[i].Visible = 0;
           for(i=0; i<5; i++)
             display_text(test_label_xLeft[i], test_label_yTop[i], 0, COLOR_YELLOW, test_label[i], 0, 16, FullPageType);


           label_null_image.xLeft = test_label_xLeft[1] + 350;
           label_null_image.yTop = test_label_yTop[1] - 6;
           for(i=0; i<10; i++)
             tmp_text[i] = '\0';
           xtm_sprintf(tmp_text, "%d", VideoFrameRate[Local.Test_FrameRate]);
           xtm_printf("tmp_text = %s, %d, %d\n", tmp_text, Local.Test_FrameRate, VideoFrameRate[Local.Test_FrameRate]);
           DisplayImage(&label_null_image, FROMFILE, FullPageType);
           WaitDecodeFinish();
           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, tmp_text, 0, 16, FullPageType);


           label_null_image.xLeft = test_label_xLeft[2] + 350;
           label_null_image.yTop = test_label_yTop[2] - 6;
           DisplayImage(&label_null_image, FROMFILE, FullPageType);
           WaitDecodeFinish();
           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, VideoFrameSize[Local.Test_FrameSize], 0, 16, FullPageType);


           label_null_image.xLeft = test_label_xLeft[3] + 350 + 10;
           label_null_image.yTop = test_label_yTop[3] - 6;
           DisplayImage(&label_null_image, FROMFILE, FullPageType);
           WaitDecodeFinish();
           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, VideoClosed_Text[Local.Test_VideoClosed], 0, 16, FullPageType);

           label_null_image.xLeft = test_label_xLeft[3] + 350 + 96;
           label_null_image.yTop = test_label_yTop[3] - 6;
           DisplayImage(&label_null_image, FROMFILE, FullPageType);
           WaitDecodeFinish();

           if(Local.Camera == 0)
             display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, CameraExist_Text[0], 0, 16, FullPageType);
           else
             display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, CameraExist_Text[1], 0, 16, FullPageType);

           label_null_image.xLeft = test_label_xLeft[4] + 350 + 96;
           label_null_image.yTop = test_label_yTop[4] - 6;
           DisplayImage(&label_null_image, FROMFILE, FullPageType);
           WaitDecodeFinish();

           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, CameraMode_Text[Local.Test_VideoOutput], 0, 16, FullPageType);
           break;
    case 1:
           for(i=11; i<18; i++)
            {
             DisplayImageButton(&test_button[i], IMAGEUP, FROMFILE, FullPageType);
             DisplayImageButton_Text(&test_button[i], test_button[i].FontColor, 2, FullPageType);
             test_button[i].Visible = 1;
            }
           for(i=6; i<11; i++)
             test_button[i].Visible = 0;
           for(i=18; i<23; i++)
             test_button[i].Visible = 0;
           for(i=23; i<_TEST_BTN_NUM; i++)
             test_button[i].Visible = 0;
           for(i=5; i<10; i++)
             display_text(test_label_xLeft[i], test_label_yTop[i], 0, COLOR_YELLOW, test_label[i], 0, 16, FullPageType);

           Local.Test_FrameRate = LocalCfg.EncodeFrameRate; //����֡��

           for(i=0; i<10; i++)
             tmp_text[i] = '\0';
           xtm_sprintf(tmp_text, "%d", LocalCfg.SpkVolume);
           label_null_image.xLeft = test_label_xLeft[9] + 350 + 96;
           label_null_image.yTop = test_label_yTop[9] - 6;
           DisplayImage(&label_null_image, FROMFILE, FullPageType);
           WaitDecodeFinish();

           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, tmp_text, 0, 16, FullPageType);

           label_null_image.xLeft = test_label_xLeft[7] + 350;
           label_null_image.yTop = test_label_yTop[7] + 32;
           DisplayImage(&label_null_image, FROMFILE, FullPageType);
           WaitDecodeFinish();
           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, SoundName[SoundNo], 0, 16, FullPageType);
           break;
    case 2:
           for(i=18; i<23; i++)
            {
             DisplayImageButton(&test_button[i], IMAGEUP, FROMFILE, FullPageType);
             DisplayImageButton_Text(&test_button[i], test_button[i].FontColor, 2, FullPageType);
             test_button[i].Visible = 1;
            }
           for(i=6; i<11; i++)
             test_button[i].Visible = 0;
           for(i=11; i<18; i++)
             test_button[i].Visible = 0;
           for(i=23; i<_TEST_BTN_NUM; i++)
             test_button[i].Visible = 0;             
           for(i=10; i<15; i++)
             display_text(test_label_xLeft[i], test_label_yTop[i], 0, COLOR_YELLOW, test_label[i], 0, 16, FullPageType);


           for(i=0; i<10; i++)

             tmp_text[i] = '\0';
           xtm_sprintf(tmp_text, "%d", LocalCfg.MicVolume);
           label_null_image.xLeft = test_label_xLeft[10] + 350 + 96;
           label_null_image.yTop = test_label_yTop[10] - 6;
           DisplayImage(&label_null_image, FROMFILE, FullPageType);
           WaitDecodeFinish();

           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, tmp_text, 0, 16, FullPageType);


          for(i=0; i<10; i++)

            tmp_text[i] = '\0';
          xtm_sprintf(tmp_text, "%d", Freq_Text[Local.Test_PowerStayOnFreq]);

          label_null_image.xLeft = test_label_xLeft[13] + 350 + 96 + 8;
          label_null_image.yTop = test_label_yTop[13] - 6;
          DisplayImage(&label_null_image, FROMFILE, FullPageType);
          WaitDecodeFinish();

          display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, tmp_text, 0, 16, FullPageType);
           break;
    case 3:
           for(i=23; i<_TEST_BTN_NUM; i++)
            {
             DisplayImageButton(&test_button[i], IMAGEUP, FROMFILE, FullPageType);
             DisplayImageButton_Text(&test_button[i], test_button[i].FontColor, 2, FullPageType);
             test_button[i].Visible = 1;
            }
           for(i=6; i<11; i++)
             test_button[i].Visible = 0;
           for(i=11; i<18; i++)
             test_button[i].Visible = 0;
           for(i=18; i<23; i++)
             test_button[i].Visible = 0;
           for(i=15; i<20; i++)
             display_text(test_label_xLeft[i], test_label_yTop[i], 0, COLOR_YELLOW, test_label[i], 0, 16, FullPageType);

           label_null_image.xLeft = test_label_xLeft[16] + 350 + 96 + 8;
           label_null_image.yTop = test_label_yTop[16] - 6;
           DisplayImage(&label_null_image, FROMFILE, FullPageType);
           WaitDecodeFinish();

           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, SDRAM_Text[Local.Test_VideoSdramRefresh], 0, 16, FullPageType);

           label_null_image.xLeft = test_label_xLeft[17] + 350 + 96 + 8;
           label_null_image.yTop = test_label_yTop[17] - 6;
           DisplayImage(&label_null_image, FROMFILE, FullPageType);
           WaitDecodeFinish();

           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, Bright_Text[LocalCfg.BrightVolume], 0, 16, FullPageType);

           //OV7725 ���֡��
           label_null_image.xLeft = test_label_xLeft[19] + 350 + 96 + 8;
           label_null_image.yTop = test_label_yTop[19] - 6;
           DisplayImage(&label_null_image, FROMFILE, FullPageType);
           WaitDecodeFinish();

           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, OV7725Frame_Text[Local.Test_OV7725FrameRate], 0, 16, FullPageType);
           break;
   }
}
//---------------------------------------------------------------------------
void PreDealTestWindow(INT8U wintype)    //���Դ���Ԥ��������������Ч��
{
  //if(Local.Test_Disp_No != 0)   //���Դ��� ��ʾ����
  //  return;
  //xtm_printf("PreDealTestWindow:: wintype = %d\n", wintype);
  if(wintype < _TEST_BTN_NUM)
   {
    DisplayImageButton(&test_button[wintype], IMAGEDOWN, FROMFILE, NOFULLPAGE);
    DisplayImageButton_Text(&test_button[wintype], test_button[wintype].FontColor, 2, NOFULLPAGE);
   }
}
//---------------------------------------------------------------------------
void OperateTestWindow(INT8U wintype, INT8U currwindow)    //���Դ��ڲ���
{
  char tmp_text[20];
  INT8U i;

  //if(Local.Test_Disp_No == 0)   //���Դ��� ��ʾ����
   if(wintype < _TEST_BTN_NUM)
    {
     DisplayImageButton(&test_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);
     DisplayImageButton_Text(&test_button[wintype], test_button[wintype].FontColor, 2, NOFULLPAGE);
    } 

  switch(wintype)
   {
    case 0:  //��ʼ
           break;
    case 1:  //ֹͣ
          if((Local.Test_LocalWatchFlag == 1)||(Local.Test_EncodeWatchFlag == 1)) //������Ƶ����   //������Ƶ����������ʾ
           {
            Local.Test_LocalWatchFlag = 0; //������Ƶ����
            Local.Test_EncodeWatchFlag = 0; //������Ƶ����
            
            StopRecVideo();
            StopEncodeVideo();


            xtm_printf("dcomm1 4::EN_J_CONTROL = 0x%X\n", inportb(EN_J_CONTROL));
            CloseVideoWindow();

            StopVideoClock();            //�ر���Ƶ�ɼ�����ʱ��
            StopVideoStreamDecode();      //�ر���Ƶ������

           // Set_Encode_Clock(0); //������Ƶ����ʱ��  1  ��   0  ��
           // Set_VideoCapture_Clock(0);  //������Ƶ�ɼ�ʱ��  1  ��   0  ��
            //outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)|0x03);
           }
           if(Local.Test_PlayAudioFlag == 1) //��������
            {
             Local.Test_PlayAudioFlag = 0;
             StopPlayWavFile();
            }

          if(Local.Test_RecAudioFlag == 1)
           {
            Local.Test_RecAudioFlag = 0; //¼��
            StopRecWav();
           }
           break;
    case 2:  //��ҳ
           if(CheckTestBusy() == 1)
             break;
           if(Local.Test_CurrPage > 0)
            {
             Local.Test_CurrPage --;
             ShowTestPage(NOFULLPAGE);
            }
           break;
    case 3:  //��ҳ
           if(CheckTestBusy() == 1)
             break;
           if(Local.Test_CurrPage < (_TEST_PAGE_NUM - 1))
            {
             Local.Test_CurrPage ++;
             ShowTestPage(NOFULLPAGE);
            }
           break;
    case 4:  //����
           LocalCfg.EncodeFrameRate = Local.Test_FrameRate; //����֡��
           LocalCfg.EncodeFrameSize = Local.Test_FrameSize; //�����С
           LocalCfg.VideoClosed = Local.Test_VideoClosed; //��Ƶ�ر�
           LocalCfg.VideoOutput = Local.Test_VideoOutput;    //��Ƶ���  0  ����  1 �Լ�
           LocalCfg.VideoSdramRefresh = Local.Test_VideoSdramRefresh;    //LCD��ʾʱ SDRAMˢ������  0  8����  1 4����
           WriteCfgFile();   //��Flash�д洢�ļ�
           break;
    case 5: //����
           if(CheckTestBusy() == 1)
             break;
           DisplaySetupWindow(REFRESH_TOP | REFRESH_MIDDLE | REFRESH_BOTTOM);
           break;
    case 6: //��ʾ����
          Local.Test_Disp_No = 1;
          DisplayImage(&main_image11111, FROMFILE, NOFULLPAGE);
      //    xtm_clear_disp_ram(Disp_Color[Local.Test_Disp_No - 1]);
          break;
    case 7: //����֡��
          //xtm_printf("OperateTestWindow:: 1 Local.Test_FrameRate = %d\n", Local.Test_FrameRate);
          Local.Test_FrameRate ++;
          if(Local.Test_FrameRate > 5)
            Local.Test_FrameRate = 0;
          for(i=0; i<10; i++)
            tmp_text[i] = '\0';
          //xtm_printf("OperateTestWindow:: 2 Local.Test_FrameRate = %d\n", Local.Test_FrameRate);
          xtm_sprintf(tmp_text, "%d", VideoFrameRate[Local.Test_FrameRate]);
          label_null_image.xLeft = test_label_xLeft[1] + 350;
          label_null_image.yTop = test_label_yTop[1] - 6;
          DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
          WaitDecodeFinish();
          display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, tmp_text, 0, 16, NOFULLPAGE);
          break;
    case 8: //���Ĵ�С
          Local.Test_FrameSize ++;
          if(Local.Test_FrameSize > 1)
            Local.Test_FrameSize = 0;

          label_null_image.xLeft = test_label_xLeft[2] + 350;
          label_null_image.yTop = test_label_yTop[2] - 6;
          DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
          WaitDecodeFinish();
          display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, VideoFrameSize[Local.Test_FrameSize], 0, 16, NOFULLPAGE);
          break;
    case 9: //Camera �򿪡��ر�
          Local.Test_VideoClosed ++;
          if(Local.Test_VideoClosed > 1)
            Local.Test_VideoClosed = 0;

          label_null_image.xLeft = test_label_xLeft[3] + 350;
          label_null_image.yTop = test_label_yTop[3] - 6;
          DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
          WaitDecodeFinish();
          display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, VideoClosed_Text[Local.Test_VideoClosed], 0, 16, NOFULLPAGE);
          break;
    case 10: //camera ���ģʽ
          Local.Test_VideoOutput ++;
          if(Local.Test_VideoOutput > 1)
            Local.Test_VideoOutput = 0;
          if(Local.Test_VideoOutput == 0)      //��Ƶ���  0  ����  1 �Լ�
           {
            Write7725(0x66, 0x00);
            Write7725(0x0c, 0xc0);
           }
          else
           {
            Write7725(0x66, 0x20);
            Write7725(0x0c, 0xc1);
           }
          label_null_image.xLeft = test_label_xLeft[4] + 350 + 96;
          label_null_image.yTop = test_label_yTop[4] - 6;
          DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
          WaitDecodeFinish();

          display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, CameraMode_Text[Local.Test_VideoOutput], 0, 16, NOFULLPAGE);
          break;
    case 11: //���ؼ���
          if(Local.Test_LocalWatchFlag == 0) //������Ƶ����
           {
            Local.Test_LocalWatchFlag = 1; //������Ƶ����
            StartVideoClock();     //����Ƶ�ɼ�����ʱ��
            StartRecVideo(Local.Test_FrameSize);
            if(Local.Test_FrameSize == VIDEO_QVGA)
              DisplayVideoWindow(160, 320, 120, 240, LOCAL_VIDEO);
            if(Local.Test_FrameSize == VIDEO_VGA)
              DisplayVideoWindow(0, 640, 0, 480, LOCAL_VIDEO);
           }
          break;
    case 12:  //������Ƶ����������ʾ
          xtm_printf("OperateTestWindow::Local.Test_EncodeWatchFlag = %d\n", Local.Test_EncodeWatchFlag);
          if(Local.Test_EncodeWatchFlag == 0)
           {
            Local.Test_EncodeWatchFlag = 1; //������Ƶ����������ʾ

            StartVideoStreamDecode();     //����Ƶ������
            StartVideoClock();     //����Ƶ�ɼ�����ʱ��
        //Set_VideoCapture_Clock(1);  //������Ƶ�ɼ�ʱ��  1  ��   0  ��
        //Set_Encode_Clock(1); //������Ƶ����ʱ��  1  ��   0  ��
        //outportb(POWER_CONTROL2, inportb(POWER_CONTROL2)&0xfc);
        //     outportb(TX_START, inportb(TX_START)|0x20);//ʹ��MJPEG�ж�

                  StartRecVideo(Local.Test_FrameSize);

                  if(Local.Test_FrameSize == VIDEO_QVGA)
                   {
                    Local.DecodeVideoWidth = 320;
                    Local.DecodeVideoHeight = 240;
                    StartEncodeVideo(VIDEO_QVGA);     //����Ƶ����
                    DisplayVideoWindow(160, 320, 120, 240, REMOTE_VIDEO);
                   }
                  if(Local.Test_FrameSize == VIDEO_VGA)
                   {
                    Local.DecodeVideoWidth = 640;
                    Local.DecodeVideoHeight = 480;
                    StartEncodeVideo(VIDEO_VGA);     //����Ƶ����
                    DisplayVideoWindow(0, 640, 0, 480, REMOTE_VIDEO);
                   }

           }
          break;
    case 13: //���β�������
          /*if(Local.Test_PlayAudioFlag == 0)
           {
            Local.Test_PlayAudioFlag = 1; //��������
            StartPlayWav(wav_1k[1].Faddr, CYCLE_PLAY);
           }       */

          SoundNo ++;
          if(SoundNo >= MAXSOUNDNUM)
            SoundNo = 0;
            
         label_null_image.xLeft = test_label_xLeft[7] + 350;
         label_null_image.yTop = test_label_yTop[7] + 32;
         DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);

         label_null_image.xLeft = test_label_xLeft[7] + 350 + 96;
         label_null_image.yTop = test_label_yTop[7] + 32;
         DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
         WaitDecodeFinish();

         label_null_image.xLeft = test_label_xLeft[7] + 350;
         display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, SoundName[SoundNo], 0, 16, NOFULLPAGE);
          break;
    case 14: //ѭ����������
          if(Local.Test_PlayAudioFlag == 0)
           {
            Local.Test_PlayAudioFlag = 1; //��������
            //StartPlayWav(wav_1k[SoundNo].Faddr, CYCLE_PLAY);
           }
          break;
    case 15: //¼��
          if(Local.Test_PlayAudioFlag == 1)
            break;

          if(Local.Test_RecAudioFlag == 0)
           {
            Local.Test_RecAudioFlag = 1; //¼��
            xtm_printf("OperateTestWindow:: StartRecWav\n");
            StartRecWav();
           }
          break;
    case 16: //����
          if(Local.Test_PlayAudioFlag == 0)
           {
            xtm_printf("OperateTestWindow:: Local.Test_PlayAudioFlag = 0x%X\n", Local.Test_PlayAudioFlag);
            Local.Test_PlayAudioFlag = 1; //��������
            StartPlayWav_Rec(ring_wav.Faddr, NO_CYCLE_PLAY);
           }
          break;
    case 17://��������
           LocalCfg.SpkVolume ++;
           if(LocalCfg.SpkVolume > 5)
             LocalCfg.SpkVolume = 0;

           SoundSetSpeakerVolume(LocalCfg.SpkVolume);

           for(i=0; i<10; i++)
             tmp_text[i] = '\0';
           xtm_sprintf(tmp_text, "%d", LocalCfg.SpkVolume);
           label_null_image.xLeft = test_label_xLeft[9] + 350 + 96;
           label_null_image.yTop = test_label_yTop[9] - 6;
           DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
           WaitDecodeFinish();

           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, tmp_text, 0, 16, NOFULLPAGE);
          break;
    case 18://¼������
           LocalCfg.MicVolume ++;
           if(LocalCfg.MicVolume > 5)
             LocalCfg.MicVolume = 0;

           SoundSetMicVolume(LocalCfg.MicVolume);

           for(i=0; i<10; i++)
             tmp_text[i] = '\0';
           xtm_sprintf(tmp_text, "%d", LocalCfg.MicVolume);
           label_null_image.xLeft = test_label_xLeft[10] + 350 + 96;
           label_null_image.yTop = test_label_yTop[10] - 6;
           DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
           WaitDecodeFinish();

           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, tmp_text, 0, 16, NOFULLPAGE);
          break;
    case 19: //����OV7725
          label_null_image.xLeft = test_label_xLeft[11] + 350;
          label_null_image.yTop = test_label_yTop[11] - 6;
          DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
          WaitDecodeFinish();
          if(test7725() == 1)
            display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, I2C_Text[0], 0, 16, NOFULLPAGE);
          else
            display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, I2C_Text[1], 0, 16, NOFULLPAGE);
          init7725(Local.Test_OV7725FrameRate);
          break;
    case 20: //����ES8328E
          label_null_image.xLeft = test_label_xLeft[12] + 350;
          label_null_image.yTop = test_label_yTop[12] - 6;
          DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
          WaitDecodeFinish();
          if(test_es8328e() == 1)
            display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, I2C_Text[0], 0, 16, NOFULLPAGE);
          else
            display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, I2C_Text[1], 0, 16, NOFULLPAGE);
          init_es8388();
          SoundSetSpeakerVolume(LocalCfg.SpkVolume);
          SoundSetMicVolume(LocalCfg.MicVolume);
          break;
    case 21: //�͹��Ĵ�����Ƶ��
          xtm_printf("OperateTestWindow::POWER_CONTROL1 = 0x%X\n", inportb(POWER_CONTROL1));
          Local.Test_PowerStayOnFreq ++;
          if(Local.Test_PowerStayOnFreq > 2)
            Local.Test_PowerStayOnFreq = 0;

          for(i=0; i<10; i++)
            tmp_text[i] = '\0';
          xtm_sprintf(tmp_text, "%d", Freq_Text[Local.Test_PowerStayOnFreq]);

          label_null_image.xLeft = test_label_xLeft[13] + 350 + 96 + 8;
          label_null_image.yTop = test_label_yTop[13] - 6;
          DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
          WaitDecodeFinish();

          display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, tmp_text, 0, 16, NOFULLPAGE);
          break;
    case 22: //�͹��Ĵ���
          break;
    case 23: //ѭ������ ����
          break;
    case 24: //4���� LCD��ʾʱ SDRAMˢ������
          xtm_printf("OperateTestWindow::POWER_CONTROL1 = 0x%X\n", inportb(POWER_CONTROL1));
          Local.Test_VideoSdramRefresh ++;
          if(Local.Test_VideoSdramRefresh > 1)
            Local.Test_VideoSdramRefresh = 0;

          if(Local.Test_VideoSdramRefresh == 0)
            outportb(POWER_CONTROL1, 0x04);//��Դ���ƼĴ���1    4����
          else
            outportb(POWER_CONTROL1, 0x84);//��Դ���ƼĴ���1    8����


          label_null_image.xLeft = test_label_xLeft[16] + 350 + 96 + 8;
          label_null_image.yTop = test_label_yTop[16] - 6;
          DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
          WaitDecodeFinish();

          display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, SDRAM_Text[Local.Test_VideoSdramRefresh], 0, 16, NOFULLPAGE);
          break;
    case 25: //����
          xtm_printf("OperateTestWindow::POWER_CONTROL1 = 0x%X\n", inportb(POWER_CONTROL1));
          LocalCfg.BrightVolume ++;
          if(LocalCfg.BrightVolume > 5)
            LocalCfg.BrightVolume = 0;

           Lcd_Power(1, LocalCfg.BrightVolume);

           label_null_image.xLeft = test_label_xLeft[17] + 350 + 96 + 8;
           label_null_image.yTop = test_label_yTop[17] - 6;
           DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
           WaitDecodeFinish();

           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, Bright_Text[LocalCfg.BrightVolume], 0, 16, NOFULLPAGE);
          break;
    case 26: //�ø�
          outportb(GPIOC_SEL, 0xFF);  //GPIO
          outportb(GPIOC_OEN_DATA, 0x00); //���
          outportb(GPIOC_OUT_DATA, 0xFF);
          break;
    case 27: //�õ�
          outportb(GPIOC_SEL, 0xFF);  //GPIO
          outportb(GPIOC_OEN_DATA, 0x00); //���
          outportb(GPIOC_OUT_DATA, 0x00);
          break;
    case 28: //Camera
          outportb(GPIOC_SEL, 0x00);  //656
          outportb(GPIOC_OEN_DATA, 0xFF); //����
       //   outportb(GPIOC_OUT_DATA, 0x00);
          break;
    case 29: //��
          outportb(GPIOC_SEL, 0xFF);  //GPIO
          outportb(GPIOC_OEN_DATA, 0xFF); //����

          for(i=0; i<10; i++)
            tmp_text[i] = '\0';
          xtm_sprintf(tmp_text, "0x%X\0", inportb(GPIOC_IN_DATA));
          label_null_image.xLeft = test_label_xLeft[18] + 350 + 96 + 8 + 60;
          label_null_image.yTop = test_label_yTop[18] - 6;
          DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
          WaitDecodeFinish();

          display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, tmp_text, 0, 16, NOFULLPAGE);
          break;
    case 30:  //OV7725 ���֡��
           Local.Test_OV7725FrameRate ++;
           if(Local.Test_OV7725FrameRate > 2)
             Local.Test_OV7725FrameRate = 0;

           init_camera(Local.Test_OV7725FrameRate);

           label_null_image.xLeft = test_label_xLeft[19] + 350 + 96 + 8;
           label_null_image.yTop = test_label_yTop[19] - 6;
           DisplayImage(&label_null_image, FROMFILE, NOFULLPAGE);
           WaitDecodeFinish();

           display_text(label_null_image.xLeft, label_null_image.yTop + 6, 0, COLOR_YELLOW, OV7725Frame_Text[Local.Test_OV7725FrameRate], 0, 16, NOFULLPAGE);
           break;
   /* case 21: //8����
          xtm_printf("OperateTestWindow::POWER_CONTROL1 = 0x%X\n", inportb(POWER_CONTROL1));
          outportb(POWER_CONTROL1, 0x04);//��Դ���ƼĴ���1
          break;     */
    case 50:
         Local.Test_Disp_No ++;
         if(Local.Test_Disp_No > 9)
           DisplayTestWindow(REFRESH);
         else
          {
          #if 1
           xtm_clear_disp_ram(Disp_Color[Local.Test_Disp_No - 1]);
           break;
          #else
           xtm_printf("OperateTestWindow:: Disp_Bit = 0x%X\n", Disp_Bit);
           xtm_clear_disp_ram(COLOR_BLACK);
           xtm_clear_disp_ram(Disp_Bit);
          for(i=0; i<20; i++)
            tmp_text[i] = '\0';
           xtm_sprintf(tmp_text, "Disp_Bit = 0x%X\0", Disp_Bit);
           display_text(100, 100, 0, COLOR_WHITE, tmp_text, 0, 24, NOFULLPAGE);
           Disp_Bit = Disp_Bit << 1;
          #endif
          }
          // xtm_clear_disp_ram(Disp_Color[Local.Test_Disp_No - 1]);
         break;
   }
}
//---------------------------------------------------------------------------
INT8U CheckTestBusy(void)
{
  if((Local.Test_LocalWatchFlag == 1)||(Local.Test_EncodeWatchFlag == 1)) //������Ƶ����   //������Ƶ����������ʾ
    return 1;
  if(Local.Test_PlayAudioFlag == 1) //��������
    return 1;
  if(Local.Test_RecAudioFlag == 1)
    return 1;
  return 0;
}
//---------------------------------------------------------------------------
void CheckTouchTestWindow(INT16U XLCD, INT16U YLCD) //��鴥��������
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(Local.Test_Disp_No != 0)   //���Դ��� ��ʾ����
   {
    isInTouch = 1;
    TouchNum = 50;
   }

  if(isInTouch == 0)
   for(i=0; i<6; i++)  //���ڰ�ť
    {
     isInTouch = CheckTSInButton(&test_button[i], XLCD, YLCD);
     if(isInTouch == 1)
      {
       TouchNum = i;
       break;
      }
    }

  if(isInTouch == 0)
   for(i=6; i<_TEST_BTN_NUM; i++)  //���ڰ�ť
    {
     if(test_button[i].Visible)
      {
       isInTouch = CheckTSInButton(&test_button[i], XLCD, YLCD);
       if(isInTouch == 1)
        {
         TouchNum = i;
         break;
        }
      }
    }

  if(isInTouch == 1)
   {
    KeyTouchOperate(TouchNum);
   }
}
//---------------------------------------------------------------------------