#include "includes.h"
#include "ComDrv.h"

#include "stdlib.h"

#define CommonH
#include "common.h"

/*
****************************************
20131119
�����޸�
****************************************
*/


INT8U CheckTouchDelayTime(void);  //����������ʱ����ӳ�

void TouchScr_Func(INT16U touch_x, INT16U touch_y); //����������
INT8U CheckTSInButton(struct TImageButton *t_button, INT16U XLCD, INT16U YLCD); //��鴥�����Ƿ��ڸð�ť��
INT8U CheckTSInEdit(struct TEdit *t_edit, INT16U XLCD, INT16U YLCD); //��鴥�����Ƿ��ڸ�Edit��
INT8U CheckTSInImage(struct TImage *t_image, INT16U XLCD, INT16U YLCD); //��鴥�����Ƿ��ڸ�Image��
INT8U CheckTSInVideoScreen(INT16U XLCD, INT16U YLCD); //��鴥�����Ƿ��ڸ���Ƶ������
INT8U CheckTSInPicScreen(INT16U XLCD, INT16U YLCD); //��鴥�����Ƿ��ڸ���Ƭ������
INT8U CheckBigmenuFunc(INT16U XLCD, INT16U YLCD);  //��ݰ�ť����
//---------------------------------------------------------------------------
void TouchScr_Func(INT16U touch_x, INT16U touch_y) //����������
{
  INT16 X,Y, XLCD, YLCD;

  INT8U isInTouch;

  //�Ѿ����ڽ���ͨ���л�����״̬
  if(PicStatBuf.Flag == 1)
   {
    xtm_printf("TouchScr_Func::PicStatBuf.Flag = %d\n", PicStatBuf.Flag);
    return;
   }

#if 1
  if(LocalCfg.Ts_Mode == 0)
   {
    X = touch_x;
    Y = touch_y;
   }
  else           //1
   {
    X = touch_y;
    Y = touch_x;
   }
  if(LocalCfg.Ts_deltaX == 0)
    LocalCfg.Ts_deltaX = 1;
  if(LocalCfg.Ts_deltaY == 0)
    LocalCfg.Ts_deltaY = 1;   
  XLCD = (INT32)(X - LocalCfg.Ts_X0)*SCREEN_WIDTH/LocalCfg.Ts_deltaX + SCREEN_WIDTH/2;
  YLCD = (INT32)(Y - LocalCfg.Ts_Y0)*SCREEN_HEIGHT/LocalCfg.Ts_deltaY + SCREEN_HEIGHT/2;

  xtm_printf("X=%d, Y=%d, XLCD=%d, YLCD=%d, LocalCfg.Ts_X0=%d, LocalCfg.Ts_Y0=%d, LocalCfg.Ts_deltaX=%d,   \
     LocalCfg.Ts_deltaY=%d\n", X, Y, XLCD, YLCD, LocalCfg.Ts_X0, LocalCfg.Ts_Y0,     \
     LocalCfg.Ts_deltaX, LocalCfg.Ts_deltaY);
  if(XLCD < 0)
    XLCD = 0;
  if(XLCD > SCREEN_WIDTH)
    XLCD = SCREEN_WIDTH;
  if(YLCD < 0)
    YLCD = 0;
  if(YLCD > SCREEN_HEIGHT)
    YLCD = SCREEN_HEIGHT;
  xtm_printf("Local.CurrentWindow=%d\n", Local.CurrentWindow);

  Local.KeyPressTime = 0;
  Local.LcdLightTime = 0;
  if(Local.LcdLightFlag == 0)
   {
    //����LCD����
    Lcd_Power(1, LocalCfg.BrightVolume);
    Local.LcdLightFlag = 1; //LCD�����־
    Local.LcdLightTime = 0; //ʱ��
   }
  else
   {
    isInTouch = 0;
    switch(Local.CurrentWindow)
     {
      case 0: //������
             CheckTouchFirstWindow(XLCD, YLCD); //��鴥��������
             break;
      case 12: //��Ƶ���д���
             CheckTouchTalkWindow(XLCD, YLCD); //��鴥��������
             break;
      case 13: //���Ӵ���
             CheckTouchWatchWindow(XLCD, YLCD); //��鴥��������
             break;
      case 16://��ʾͼ��������ڣ�������,�к���ʱ�Զ���ʾ
             CheckTouchTalkWatchPicWindow(XLCD, YLCD); //��鴥��������
             break;
 /*     #ifdef _MULTI_MACHINE_SUPPORT  //��ֻ�֧��
      case 25://ת��
             CheckTouchTalkTurnWindow(XLCD, YLCD); //��鴥��������
             break;
      #endif          */
      case 21://ͨ����¼���ڣ�һ����
             CheckTouchRecordsWindow(XLCD, YLCD); //��鴥��������
             break;
      case 22://�鿴ͨ����¼���ڣ�������������
             CheckTouchRecordsContentWindow(XLCD, YLCD); //��鴥��������
             break;
      case 31:  //�������������������
             CheckTouchCancelFortifyWindow(XLCD, YLCD); //��鴥��������
             break;
      case 32:  //�������������������
             CheckTouchFortifyWindow(XLCD, YLCD); //��鴥��������
             break;
      case 34://��ʾ�������ڣ�������
             CheckTouchAlarmWindow(XLCD, YLCD); //��鴥��������
             break;
      case 35:  //ȡ���������������������
             CheckTouchCancelAlarmWindow(XLCD, YLCD); //��鴥��������
             break;
      case 111: //��Ϣ����
             CheckTouchInfoWindow(XLCD, YLCD); //��鴥��������
             break;
      case 112://��Ϣ��ʾ���ڣ�������������
             CheckTouchInfoContentWindow(XLCD, YLCD); //��鴥��������
             break;
      case 5://���ô��ڣ�һ����
             CheckTouchSetupWindow(XLCD, YLCD); //��鴥��������
             break;
	  case 6:
	  	     //CheckTouchSplashWindow(XLCD, YLCD); //��鴥��������
	  	     break;
	  case 7:
	  	     CheckTouchEPWWindow(XLCD, YLCD);
	  	     break;
      case 88:
             CheckTouchSmarkStreamWindow(XLCD, YLCD);
             break;
      case 89:
             CheckTouchSmarkStreamWindow(XLCD, YLCD);
             break;
      case 149://KB����
             CheckTouchKbWindow(XLCD, YLCD); //��鴥��������
             break;
      case 151://LAN���ô��ڣ�������
             CheckTouchLanWindow(XLCD, YLCD); //��鴥��������
             break;
      case 152: //����������
             CheckTouchSetupMainWindow(XLCD, YLCD); //��鴥��������
             break;
      case 154://�������ô��ڣ�������
             CheckTouchAddrWindow(XLCD, YLCD); //��鴥��������
             break;
      case 165://��Ļ���ô���
             CheckTouchScreenSetupWindow(XLCD, YLCD); //��鴥��������
             break;
      case 166://�������ڴ���
             CheckTouchVoiceSetupWindow(XLCD, YLCD); //��鴥��������
             break;
      case 168://�޸����봰��
             CheckTouchPassSetupWindow(XLCD, YLCD); //��鴥��������
             break;             
      case 158://�汾��Ϣ����
             CheckTouchVersionWindow(XLCD, YLCD); //��鴥��������
             break;
     /* case 159://ϵͳ��������
             CheckTouchUpgradeWindow(XLCD, YLCD); //��鴥��������
             break;
      case 160://�����������ô���
             CheckTouchVoiceRingWindow(XLCD, YLCD); //��鴥��������
             break;

      case 166://�������ô���
             CheckTouchSoundSetupWindow(XLCD, YLCD); //��鴥��������
             break;
      case 167://ʱ�����ô���
             CheckTouchTimeSetupWindow(XLCD, YLCD); //��鴥��������
             break;

      case 169://�������ô���
             CheckTouchLanguageSetupWindow(XLCD, YLCD); //��鴥��������
             break;
      case 6://�龰ģʽ���ڣ�һ����
             CheckTouchSceneWindow(XLCD, YLCD); //��鴥��������
             break;
      */
      case 190: //У׼����������
             PlaySoundTip(); //������ʾ��

             Calib_X[Local.CalibratePos] = X;
             Calib_Y[Local.CalibratePos] = Y;
             Calib_OldX[Local.CalibratePos] = touch_x;
             Calib_OldY[Local.CalibratePos] = touch_y;
             OperateCalibrateWindow(0, 190);
             break;
      case 192://���Դ���
             //CheckTouchTestWindow(XLCD, YLCD); //��鴥��������
             break;
       }
     }
#endif     
}
//---------------------------------------------------------------------------
INT8U CheckTouchDelayTime(void)  //����������ʱ����ӳ�
{
/*  struct timeval tv;
  gettimeofday(&tv, NULL);
  Local.newtouchtime = tv.tv_sec *1000 + tv.tv_usec/1000;
  if(((Local.newtouchtime - Local.oldtouchtime) < TOUCHMAXTIME)&&(Local.newtouchtime > Local.oldtouchtime))
    return 0;
  else
    Local.oldtouchtime = Local.newtouchtime;    */
  return 1;
}
//---------------------------------------------------------------------------
INT8U CheckTSInButton(struct TImageButton *t_button, INT16U XLCD, INT16U YLCD) //��鴥�����Ƿ��ڸð�ť��
{
  if((XLCD >= t_button->xLeft)&&(XLCD <= (t_button->xLeft + t_button->width))
    &&(YLCD >= t_button->yTop)&&(YLCD <= (t_button->yTop + t_button->height)))
    return 1;
  else
    return 0;
}
//---------------------------------------------------------------------------
INT8U CheckTSInEdit(struct TEdit *t_edit, INT16U XLCD, INT16U YLCD) //��鴥�����Ƿ��ڸ�Edit��
{
  if((XLCD >= t_edit->xLeft)&&(XLCD <= (t_edit->xLeft + t_edit->width))
    &&(YLCD >= t_edit->yTop)&&(YLCD <= (t_edit->yTop + t_edit->height)))
    return 1;
  else
    return 0;
}
//---------------------------------------------------------------------------
INT8U CheckTSInImage(struct TImage *t_image, INT16U XLCD, INT16U YLCD) //��鴥�����Ƿ��ڸ�Image��
{
  if((XLCD >= t_image->xLeft)&&(XLCD <= (t_image->xLeft + t_image->width))
    &&(YLCD >= t_image->yTop)&&(YLCD <= (t_image->yTop + t_image->height)))
    return 1;
  else
    return 0;
}
//---------------------------------------------------------------------------
INT8U CheckTSInPicScreen(INT16U XLCD, INT16U YLCD) //��鴥�����Ƿ��ڸ���Ƭ������
{
  INT8U i;
  INT16U Pic_X[3] = {87, 301, 509};
  INT16U Pic_Y[3] = {127, 128, 128};
  for(i=0; i<3; i++)
   if((XLCD >= Pic_X[i])&&(XLCD <= (Pic_X[i] + 192))
    &&(YLCD >= Pic_Y[i])&&(YLCD <= (Pic_Y[i] + 144)))
    {
     return i+1;
    }
  return 0;
}
//---------------------------------------------------------------------------
INT8U CheckBigmenuFunc(INT16U XLCD, INT16U YLCD)  //��ݰ�ť����
{
  INT8U i;
  INT8U isInTouch;
  for(i=0; i<7; i++)  //��˵���ť
   {
    isInTouch = CheckTSInButton(&bigmenu_button[i], XLCD, YLCD);
    if(isInTouch == 1)
     {
      return (i + 100);
      //���̺ʹ�����������
      //KeyAndTouchFunc(i + 'a' + 30);
      //break;
     }
   }
  return 0;
}
//---------------------------------------------------------------------------

