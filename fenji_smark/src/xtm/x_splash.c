#include "includes.h"

#include "stdlib.h"

#define x_splash_h

#define CommonH
#include "common.h"

/*
****************************************
201309
�����ӵĿ������壬���Խ׶�
201310
���ӹ��Ԫ��
****************************************
*/

void DisplaySplashWindow(INT8U RefreshType);    //��ʾSplash
void PreDealSplashWindow(INT8U wintype);    //Splash����Ԥ����,���°���ʱ��Ч��
void CheckTouchSplashWindow(INT16U XLCD, INT16U YLCD); //��鴥��������
void OperateSplashWindow(INT8U wintype, INT8U currwindow); //splash���ڲ���

void DisplayAdWindow(INT8U RefreshType);    //��ʾSplash
void PreDealAdWindow(INT8U wintype);    //Splash����Ԥ����,���°���ʱ��Ч��
void CheckTouchAdWindow(INT16U XLCD, INT16U YLCD); //��鴥��������
void OperateAdWindow(INT8U wintype, INT8U currwindow); //splash���ڲ���


//--------------------------------------------------------------------------
void DisplaySplashWindow(INT8U RefreshType)
{
	INT8U i;

    //for(i=0; i<SPLASH_COUNT-6; i++)
	{
	    
		//DisplayImage(&splash_image[i], FROMFILE, NOFULLPAGE);
		//Local.CurrentWindow = 6;
		//Local.fb_page = !Local.fb_page;
		//selectym(Local.fb_page); 
		//delay(100);
		//DelayMs(200);
	}

    for(i=SPLASH_COUNT-6; i<SPLASH_COUNT; i++)
	{
	    
		//DisplayImage(&splash_image[i], FROMFILE, NOFULLPAGE);
		
		//Local.CurrentWindow = 6;
		//Local.fb_page = !Local.fb_page;
		//selectym(Local.fb_page); 
		//delay(300);
		//DelayMs(200);
	}

	for(i=SPLASH_COUNT-1; i<SPLASH_COUNT-6; i--)
	{
	    
		//DisplayImage(&splash_image[i], FROMFILE, NOFULLPAGE);
        
		//Local.CurrentWindow = 6;
		//Local.fb_page = !Local.fb_page;
		//selectym(Local.fb_page); 
		//delay(300);
		//DelayMs(200);
	}

	DisplayFirstWindow();
}
//--------------------------------------------------------------------------
void PreDealSplashWindow(INT8U wintype)    //����splash����Ԥ������������Ч��
{
  PreDealBigmenuButton(wintype);  //��ݰ��� Ԥ����
}
//--------------------------------------------------------------------------
void CheckTouchSplashWindow(INT16U XLCD, INT16U YLCD) //��鴥��������
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   {
    TouchNum = CheckBigmenuFunc(XLCD, YLCD);  //��ݰ�ť����
    if(TouchNum >= 100)
      isInTouch = 1;
   }

  if(isInTouch == 1)
   {
    KeyTouchOperate(TouchNum);
   }
}

//--------------------------------------------------------------------------
void OperateSplashWindow(INT8U wintype, INT8U currwindow)    //splash���ڲ���
{
  INT8U i;
  xtm_printf("OperateWatchWindow:: wintype = %d\n", wintype);
  if(wintype < 3)
   {
    WaitDecodeFinish();
    DisplayImageButton(&watch_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);
   } 

  ShowPressQuickmenu_Button(wintype);  //��ݰ���  ����
  xtm_printf("OperateWatchWindow:: 2 wintype = %d\n", wintype);
  switch(wintype)
   {
    case 0: //��
           if(Local.Status == 0)
            {
             if(Local.WatchTarget > 0)
              {
               Local.WatchTarget --;
               ShowWatchTarget(NOFULLPAGE);
              }
             else
              {
               Local.WatchTarget = Local.WatchTargetMaxNum - 1;
               ShowWatchTarget(NOFULLPAGE);
              }
            }
           break;
    case 1: //��
           if(Local.Status == 0)
            {
             if(Local.WatchTarget < (Local.WatchTargetMaxNum - 1))
              {
               Local.WatchTarget ++;
               ShowWatchTarget(NOFULLPAGE);
              }
             else
              {
               Local.WatchTarget = 0;
               ShowWatchTarget(NOFULLPAGE);
              }
           }
           break;
    case 2:
          if(Local.Status == 0)
           {
            //DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, WATCHWINTYPE);
            //Watch_Func();
           }
          break;
    case 3:
          xtm_printf("OperateWatchWindow Local.Status = %d\n", Local.Status);
          if(Local.Status == 0)
           {
            //DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, WATCHWINTYPE);
            //Watch_Func();
           }
          break;
    default:
          OperateQuickmenu(wintype, 107);
          break;
   }
}
//--------------------------------------------------------------------------
void DisplayAdWindow(INT8U RefreshType)
{
	INT8U i;

	//RefreshBackWindow(RefreshType, FULLPAGE); //ˢ�±���
	//DisplayImage(&splash_image[1], FROMFILE, FULLPAGE);

    //delay(300);

    if(LocalCfg.VoiceHint == 1)
	{
		WaitAudioUnuse(200); //�ȴ���������               //  wav_1k[5].Faddr
		StartPlayWav(splash_wav.Faddr, NO_CYCLE_PLAY);       // soundtip_wav.Faddr
	}
    
    for(i=0; i<AD_PIC_COUNT; i++)
	{
		RefreshBackWindow(RefreshType, FULLPAGE); //ˢ�±���
		//DisplayImage(&ad_image[i], FROMFILE, FULLPAGE);
		Local.CurrentWindow = 6;
		Local.fb_page = !Local.fb_page;
		selectym(Local.fb_page); 
		delay(1000);
		//DelayMs(200);
	}

	
    //ReadAdFile();
	//itoa(AdPic.PicCount, abc, 16);
	
	//if(AdPic.AdPicAdd[0]==0x800000)
	{
	    //display_text(64, 126, 0, COLOR_BLACK, "OK", 0, 24, NOFULLPAGE);
	}
	//else
	{
		//display_text(64, 126, 0, COLOR_BLACK, "Error", 0, 24, NOFULLPAGE);
	}
	
	//display_text(64, 126, 0, COLOR_BLACK, abc, 0, 24, NOFULLPAGE);

    //3��Ϊ0
	for(i=0;i>0;i--)
	{
		RefreshBackWindow(RefreshType, FULLPAGE); //ˢ�±���
		//DisplayImage(&splash_image[i], FROMFILE, FULLPAGE);
		
		
		Local.CurrentWindow = 6;
		Local.fb_page = !Local.fb_page;
		selectym(Local.fb_page); 

		delay(1000);
		//DelayMs(1000);
	}



}
//--------------------------------------------------------------------------
void PreDealAdWindow(INT8U wintype)    //����splash����Ԥ������������Ч��
{
  PreDealBigmenuButton(wintype);  //��ݰ��� Ԥ����
}
//--------------------------------------------------------------------------
void CheckTouchAdWindow(INT16U XLCD, INT16U YLCD) //��鴥��������
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   {
    TouchNum = CheckBigmenuFunc(XLCD, YLCD);  //��ݰ�ť����
    if(TouchNum >= 100)
      isInTouch = 1;
   }

  if(isInTouch == 1)
   {
    KeyTouchOperate(TouchNum);
   }
}

//--------------------------------------------------------------------------
void OperateAdWindow(INT8U wintype, INT8U currwindow)    //splash���ڲ���
{
  INT8U i;
  xtm_printf("OperateWatchWindow:: wintype = %d\n", wintype);
  if(wintype < 3)
   {
    WaitDecodeFinish();
    DisplayImageButton(&watch_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);
   } 

  ShowPressQuickmenu_Button(wintype);  //��ݰ���  ����
  xtm_printf("OperateWatchWindow:: 2 wintype = %d\n", wintype);
  switch(wintype)
   {
    case 0: //��
           if(Local.Status == 0)
            {
             if(Local.WatchTarget > 0)
              {
               Local.WatchTarget --;
               ShowWatchTarget(NOFULLPAGE);
              }
             else
              {
               Local.WatchTarget = Local.WatchTargetMaxNum - 1;
               ShowWatchTarget(NOFULLPAGE);
              }
            }
           break;
    case 1: //��
           if(Local.Status == 0)
            {
             if(Local.WatchTarget < (Local.WatchTargetMaxNum - 1))
              {
               Local.WatchTarget ++;
               ShowWatchTarget(NOFULLPAGE);
              }
             else
              {
               Local.WatchTarget = 0;
               ShowWatchTarget(NOFULLPAGE);
              }
           }
           break;
    case 2:
          if(Local.Status == 0)
           {
            //DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, WATCHWINTYPE);
            //Watch_Func();
           }
          break;
    case 3:
          xtm_printf("OperateWatchWindow Local.Status = %d\n", Local.Status);
          if(Local.Status == 0)
           {
            //DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, WATCHWINTYPE);
            //Watch_Func();
           }
          break;
    default:
          OperateQuickmenu(wintype, 107);
          break;
   }

}

