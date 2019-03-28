#include "includes.h"

#include "stdlib.h"

#define x_splash_h

#define CommonH
#include "common.h"

/*
****************************************
201309
新增加的开机窗体，测试阶段
201310
增加广告元素
****************************************
*/

void DisplaySplashWindow(INT8U RefreshType);    //显示Splash
void PreDealSplashWindow(INT8U wintype);    //Splash窗口预处理,按下按键时的效果
void CheckTouchSplashWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作
void OperateSplashWindow(INT8U wintype, INT8U currwindow); //splash窗口操作

void DisplayAdWindow(INT8U RefreshType);    //显示Splash
void PreDealAdWindow(INT8U wintype);    //Splash窗口预处理,按下按键时的效果
void CheckTouchAdWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作
void OperateAdWindow(INT8U wintype, INT8U currwindow); //splash窗口操作


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
void PreDealSplashWindow(INT8U wintype)    //进入splash窗口预处理，按键按下效果
{
  PreDealBigmenuButton(wintype);  //快捷按键 预处理
}
//--------------------------------------------------------------------------
void CheckTouchSplashWindow(INT16U XLCD, INT16U YLCD) //检查触摸屏操作
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   {
    TouchNum = CheckBigmenuFunc(XLCD, YLCD);  //快捷按钮操作
    if(TouchNum >= 100)
      isInTouch = 1;
   }

  if(isInTouch == 1)
   {
    KeyTouchOperate(TouchNum);
   }
}

//--------------------------------------------------------------------------
void OperateSplashWindow(INT8U wintype, INT8U currwindow)    //splash窗口操作
{
  INT8U i;
  xtm_printf("OperateWatchWindow:: wintype = %d\n", wintype);
  if(wintype < 3)
   {
    WaitDecodeFinish();
    DisplayImageButton(&watch_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);
   } 

  ShowPressQuickmenu_Button(wintype);  //快捷按键  弹起
  xtm_printf("OperateWatchWindow:: 2 wintype = %d\n", wintype);
  switch(wintype)
   {
    case 0: //左
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
    case 1: //右
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

	//RefreshBackWindow(RefreshType, FULLPAGE); //刷新背景
	//DisplayImage(&splash_image[1], FROMFILE, FULLPAGE);

    //delay(300);

    if(LocalCfg.VoiceHint == 1)
	{
		WaitAudioUnuse(200); //等待声卡空闲               //  wav_1k[5].Faddr
		StartPlayWav(splash_wav.Faddr, NO_CYCLE_PLAY);       // soundtip_wav.Faddr
	}
    
    for(i=0; i<AD_PIC_COUNT; i++)
	{
		RefreshBackWindow(RefreshType, FULLPAGE); //刷新背景
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

    //3改为0
	for(i=0;i>0;i--)
	{
		RefreshBackWindow(RefreshType, FULLPAGE); //刷新背景
		//DisplayImage(&splash_image[i], FROMFILE, FULLPAGE);
		
		
		Local.CurrentWindow = 6;
		Local.fb_page = !Local.fb_page;
		selectym(Local.fb_page); 

		delay(1000);
		//DelayMs(1000);
	}



}
//--------------------------------------------------------------------------
void PreDealAdWindow(INT8U wintype)    //进入splash窗口预处理，按键按下效果
{
  PreDealBigmenuButton(wintype);  //快捷按键 预处理
}
//--------------------------------------------------------------------------
void CheckTouchAdWindow(INT16U XLCD, INT16U YLCD) //检查触摸屏操作
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   {
    TouchNum = CheckBigmenuFunc(XLCD, YLCD);  //快捷按钮操作
    if(TouchNum >= 100)
      isInTouch = 1;
   }

  if(isInTouch == 1)
   {
    KeyTouchOperate(TouchNum);
   }
}

//--------------------------------------------------------------------------
void OperateAdWindow(INT8U wintype, INT8U currwindow)    //splash窗口操作
{
  INT8U i;
  xtm_printf("OperateWatchWindow:: wintype = %d\n", wintype);
  if(wintype < 3)
   {
    WaitDecodeFinish();
    DisplayImageButton(&watch_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);
   } 

  ShowPressQuickmenu_Button(wintype);  //快捷按键  弹起
  xtm_printf("OperateWatchWindow:: 2 wintype = %d\n", wintype);
  switch(wintype)
   {
    case 0: //左
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
    case 1: //右
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

