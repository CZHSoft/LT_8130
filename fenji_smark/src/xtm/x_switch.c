#include "includes.h"

#include "stdlib.h"

#define x_switch_h

#define CommonH
#include "common.h"


void DisplaySwitchMainWindow(INT8U RefreshType);    //显示
void PreDealSwitchMainWindow(INT8U wintype);    //按下按键时的效果
void CheckTouchSwitchMainWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作
void OperateSwitchMainWindow(INT8U wintype, INT8U currwindow); 

void DisplaySwitchSetWindow(INT8U RefreshType,INT8U Index1);    //显示
void PreDealSwitchSetWindow(INT8U wintype);    //按下按键时的效果
void CheckTouchSwitchSetWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作
void OperateSwitchSetWindow(INT8U wintype, INT8U currwindow); 

INT8U index=0;
char name[20]="名字1";
INT8U no=1;

INT8U type1=0;
INT8U enable1=0;
INT8U state1=0;
INT8U type2=0;

//--------------------------------------------------------------------------
void DisplaySwitchMainWindow(INT8U RefreshType)
{
	INT8U i;

    ReadSwitchFile();

    RefreshBackWindow(RefreshType, FULLPAGE); //刷新背景
 
   for(i=0; i<SwitchInfo.SwitchCount; i++)
   {
        //xtm_printf("i = %d\n", i);
        DisplayImageButton(&switch_in_button[i], IMAGEUP, FROMFILE, FULLPAGE);
   }

    Local.fb_page = !Local.fb_page;
    selectym(Local.fb_page);


}
//--------------------------------------------------------------------------
void PreDealSwitchMainWindow(INT8U wintype)    //进入splash窗口预处理，按键按下效果
{
  PreDealBigmenuButton(wintype);  //快捷按键 预处理
}
//--------------------------------------------------------------------------
void CheckTouchSwitchMainWindow(INT16U XLCD, INT16U YLCD) //检查触摸屏操作
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   for(i=0; i<SwitchInfo.SwitchCount; i++)  //窗口按钮
    {
     isInTouch = CheckTSInButton(&switch_in_button[i], XLCD, YLCD);
     if(isInTouch == 1)
      {
       TouchNum = i;
       break;
      }
    }


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
void OperateSwitchMainWindow(INT8U wintype, INT8U currwindow)    //
{
  INT8U i;
  xtm_printf("OperateSwitchMainWindow:: wintype = %d\n", wintype);

  ShowPressQuickmenu_Button(wintype);  //快捷按键  弹起


    if(wintype>=0 && wintype<=49)
    {
        DisplaySwitchSetWindow(REFRESH_TOP| REFRESH_BOTTOM,wintype);
    }
    else if(wintype>=100 && wintype<=106)
    {
        OperateQuickmenu(wintype, 107);
    }
}
//--------------------------------------------------------------------------
void DisplaySwitchSetWindow(INT8U RefreshType,INT8U Index1)
{
	INT8U i;
    char *s;
    
    RefreshBackWindow(RefreshType, FULLPAGE); //刷新背景

    for(i=0; i<12; i++)
	{
		DisplayImageButton(&switch_set_button[i], IMAGEDOWN, FROMFILE, FULLPAGE);
	}
    /*
    index=Index1;
    //name=SwitchInfo.SwitchName[index];
    no=SwitchInfo.SwitchNo[index];

    type1=SwitchInfo.SwitchType1[index];
    enable1=SwitchInfo.SwitchEnable[index];
    state1=SwitchInfo.SwitchState[index];
    type2=SwitchInfo.SwitchType2[index];
   
    itoa(no, s, 10);
    */
    //添加文本
    
    display_text(25, 20, 0, COLOR_BLACK, "开关号:", 0, 24, FULLPAGE);
    display_text(108, 20, 0, COLOR_RED, s, 0, 24, FULLPAGE);

    display_text(250, 20, 0, COLOR_BLACK, "开关命名:", 0, 24, FULLPAGE);
    display_text(108, 20, 0, COLOR_RED, name, 0, 24, FULLPAGE);

    display_text(25, 92, 0, COLOR_BLACK, "开关类型:", 0, 24, FULLPAGE);
    display_text(25, 166, 0, COLOR_BLACK, "可用情况:", 0, 24, FULLPAGE);
    display_text(25, 244, 0, COLOR_BLACK, "开关状态:", 0, 24, FULLPAGE);
    display_text(25, 316, 0, COLOR_BLACK, "开关类别:", 0, 24, FULLPAGE);
    
    //初始化
    if(type1==0)
    {
            DisplayImageButton(&switch_set_button[0], IMAGEUP, FROMFILE, FULLPAGE);
    }
    else
    {
            DisplayImageButton(&switch_set_button[1], IMAGEUP, FROMFILE, FULLPAGE);
    }

    if(enable1==0)
        {
            DisplayImageButton(&switch_set_button[2], IMAGEUP, FROMFILE, FULLPAGE);
        }
    else
        {
            DisplayImageButton(&switch_set_button[3], IMAGEUP, FROMFILE, FULLPAGE);
        }

    if(state1==0)
        {
            DisplayImageButton(&switch_set_button[4], IMAGEUP, FROMFILE, FULLPAGE);
        }
    else
        {
            DisplayImageButton(&switch_set_button[5], IMAGEUP, FROMFILE, FULLPAGE);
        }

    if(type2==0)
        {
            DisplayImageButton(&switch_set_button[6], IMAGEUP, FROMFILE, FULLPAGE);
        }
    else
        {
            DisplayImageButton(&switch_set_button[7], IMAGEUP, FROMFILE, FULLPAGE);
        }
   
}
//--------------------------------------------------------------------------
void PreDealSwitchSetWindow(INT8U wintype)    //进入splash窗口预处理，按键按下效果
{
    
}
//--------------------------------------------------------------------------
void CheckTouchSwitchSetWindow(INT16U XLCD, INT16U YLCD) //检查触摸屏操作
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   for(i=0; i<12; i++)  //窗口按钮
    {
     isInTouch = CheckTSInButton(&switch_set_button[i], XLCD, YLCD);
     if(isInTouch == 1)
      {
       TouchNum = i;
       break;
      }
    }


  if(isInTouch == 1)
   {
    KeyTouchOperate(TouchNum);
   }
}

//--------------------------------------------------------------------------
void OperateSwitchSetWindow(INT8U wintype, INT8U currwindow)    //
{
    INT8U i;
    xtm_printf("OperateSwitchSetWindow:: wintype = %d\n", wintype);

    switch(wintype)
        {
            case 0:
                DisplayImageButton(&switch_set_button[0], IMAGEUP, FROMFILE, NOFULLPAGE);
                DisplayImageButton(&switch_set_button[1], IMAGEDOWN, FROMFILE, NOFULLPAGE);
                type1=0;
                break;
            case 1:
                DisplayImageButton(&switch_set_button[0], IMAGEDOWN, FROMFILE, NOFULLPAGE);
                DisplayImageButton(&switch_set_button[1], IMAGEUP, FROMFILE, NOFULLPAGE);
                type1=1;
                break;
            case 2:
                DisplayImageButton(&switch_set_button[2], IMAGEUP, FROMFILE, NOFULLPAGE);
                DisplayImageButton(&switch_set_button[3], IMAGEDOWN, FROMFILE, NOFULLPAGE);
                enable1=0;
                break;
            case 3:
                DisplayImageButton(&switch_set_button[2], IMAGEDOWN, FROMFILE, NOFULLPAGE);
                DisplayImageButton(&switch_set_button[3], IMAGEUP, FROMFILE, NOFULLPAGE);
                enable1=1;
                break;
            case 4:
                DisplayImageButton(&switch_set_button[4], IMAGEUP, FROMFILE, NOFULLPAGE);
                DisplayImageButton(&switch_set_button[5], IMAGEDOWN, FROMFILE, NOFULLPAGE);
                state1=0;
                break;
            case 5:
                DisplayImageButton(&switch_set_button[4], IMAGEDOWN, FROMFILE, NOFULLPAGE);
                DisplayImageButton(&switch_set_button[5], IMAGEUP, FROMFILE, NOFULLPAGE);
                state1=1;
                break;
            case 6:
                DisplayImageButton(&switch_set_button[6], IMAGEUP, FROMFILE, NOFULLPAGE);
                DisplayImageButton(&switch_set_button[7], IMAGEDOWN, FROMFILE, NOFULLPAGE);
                type2=0;
                break;
            case 7:
                DisplayImageButton(&switch_set_button[6], IMAGEDOWN, FROMFILE, NOFULLPAGE);
                DisplayImageButton(&switch_set_button[7], IMAGEUP, FROMFILE, NOFULLPAGE);
                type2=1;
                break;
            case 8:
                if(index==0)
                    {
                        DisplaySwitchSetWindow(REFRESH_TOP| REFRESH_BOTTOM,7);
                    }
                else
                    {
                        index++;
                        DisplaySwitchSetWindow(REFRESH_TOP| REFRESH_BOTTOM,index);
                    }
                break;
            case 9:
                 if(index==7)
                    {
                        DisplaySwitchSetWindow(REFRESH_TOP| REFRESH_BOTTOM,0);
                    }
                else
                    {
                        index++;
                        DisplaySwitchSetWindow(REFRESH_TOP| REFRESH_BOTTOM,index);
                    }
                break;
            case 10:
                    /*
                    SwitchInfo.SwitchType1[index]=type1;
                    SwitchInfo.SwitchEnable[index]=enable1;
                    SwitchInfo.SwitchState[index]=state1;
                    SwitchInfo.SwitchType2[index]=type2;
                    */
                    
                    //提示
                break;
            case 11:
                    DisplaySwitchMainWindow(REFRESH_TOP| REFRESH_BOTTOM);
                break;
        }

}
//--------------------------------------------------------------------------
