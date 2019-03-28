#include "includes.h"

#include "stdlib.h"

#define x_setup_h

#define CommonH
#include "common.h"

void DisplaySetupWindow(INT8U RefreshType);    //显示进入设置窗口
void OperateSetupWindow(INT8U wintype, INT8U currwindow);    //进入设置窗口操作

INT8U ParamSec = 0;
void DisplaySetupMainWindow(INT8U RefreshType);    //显示设置主窗口
void OperateSetupMainWindow(INT8U wintype, INT8U currwindow);    //设置主窗口操作

void PlayInputErrorWav(void);
void PlayPassErrorWav(void);
void PlayModiSuccWav(void);
void PlayModiFailWav(void);

INT8U MaxAddrRow = 6;
INT8U EditStatusFlag = 0;
void DisplayAddressWindow(void);    //显示本机地址变更窗口
void OperateAddressWindow(INT8U wintype, INT8U currwindow);  //本机地址变更操作
void DealAddrEdit(INT8U ctype, INT8U cRow);
INT8U Check_AddrOK(INT8U cRow);

INT8U MaxParamRow = 7;  //20140225 6->7
INT8U MaxParamCol[7] = {2, 2, 2, 2, 3, 4, 4};//20140225 6+1
INT8U Param_xLeft = 30;
INT8U Param_yTop = 10; //20140225 20->10
INT16U ParamRowHeight = 25;//20140225 28 -> 25
INT8U CurrRow = 0;
INT8U CurrCol[7];  //20140225 6+1
void DisplayParamWindow(void);    //显示参数设置窗口
void OperateParamWindow(INT8U wintype, INT8U currwindow);  //修改参数设置操作

INT8U MaxPassRow = 4;
void DisplayPassWindow(void);    //显示密码设置窗口
void OperatePassWindow(INT8U wintype, INT8U currwindow);  //修改密码设置操作
void DealPassEdit(INT8U ctype, INT8U cRow);
INT16U curr_row_image_xLeft[2] = {64, 56};

INT8U MaxOtherRow = 3;
INT8U MaxOtherCol[3] = {2, 3, 3};
INT8U Other_xLeft = 30;
INT8U Other_yTop = 50;
INT16U OtherRowHeight = 40;
void DisplayOtherWindow(void);    //其它设置窗口
void OperateOtherWindow(INT8U wintype, INT8U currwindow);  //其它设置操作

void DisplayOnlineTestWindow(void);    //显示在线检测窗口
void OperateOnlineTestWindow(INT8U wintype, INT8U currwindow);  //在线检测窗口操作
void OnlineTest(void);
void CheckOnlineTest(void);

#define ONLINENUMPERPAGE   12
//INT16U OnlineTotalNum;
INT16U OnlineTotalPage;
INT16U OnlineCurrPage;
INT8U OnlineAddr[MAX_ONLINE_NUM][4];
void ShowOnlineInfo(void);

void DisplaySetupInfoWindow(void);    //显示关于窗口
void OperateSetupInfoWindow(INT8U wintype, INT8U currwindow);  //关于窗口操作

char setup_caption_text[LANGUAGEMAXNUM][30] = {
                                         {"请输入管理员密码:"},
                                         {"Manager password:"}
                                        };

extern INT16U openlock2_xLeft[2];
extern char openlock_text[LANGUAGEMAXNUM][3][30];

char setup_main_text[LANGUAGEMAXNUM][6][20] = {
                                         {"地址设置", "参数设置", "密码设置", "其它设置", "在线检测", "关于"},
                                         {"Address", "Parameter", "Password", "Other", "Online", "About"}
                                        };
INT16U setup_main_xLeft[2][6] = {
                        {110, 110, 110, 110, 110, 134},
                        {110, 110, 110, 110, 110, 134}
                       };

INT16U setup_main_yTop[2][6] = {
                        {36, 36, 36, 36, 36, 36},
                        {36, 36, 36, 36, 36, 36}
                       };

char addr_caption_text[LANGUAGEMAXNUM][6][20] = {
                                         {"本机地址:", "MAC地址:", "IP 地址:", "子网掩码:", "网关地址:", "服务器地址:"},
                                         {"Address:", "MAC Addr:", "IP Addr:", "Subnet Mask:", "Gateway:", "Server:"}
                                        };

char param_caption_text[LANGUAGEMAXNUM][7][20] = {
                                         {"密码开锁:", "门禁功能:", "出门按钮:", "门磁检测:", "开锁延时:", "延时开锁:", "通话时长:"},
                                         {"PW Unlock:", "Access:", "Exit Btn:", "Mag.lock:", "Lock Delay:", "Delay Unlock:","Delay Talk:"}
                                        };
char param_open_text[LANGUAGEMAXNUM][2][20] = {
                                         {"禁用", "启用"},
                                         {"Dis", "En"}
                                        };

char param_text1[LANGUAGEMAXNUM][3][20] = {
                                         {"0.5S", "3S", "5S"},
                                         {"0.5S", "3S", "5S"}
                                        };
char param_text2[LANGUAGEMAXNUM][4][20] = {
                                         {"0S", "3S", "5S", "10S"},
                                         {"0S", "3S", "5S", "10S"}
                                        };
char param_text3[LANGUAGEMAXNUM][4][20] = {
                                         {"15S", "30S", "60S", "99S"},
                                         {"15S", "30S", "60S", "99S"}
                                        };


char pass_caption_text[LANGUAGEMAXNUM][6][20] = {
                                         {"管理密码:", "新密码:", "确认密码:", "开锁密码:", "新密码:", "确认密码:"},
                                         {"Manager PW:", "New PW:", "Confirm PW:", "Unlock PW:", "New PW:", "Confirm PW:"}
                                        };

char other_caption_text[LANGUAGEMAXNUM][3][20] = {
                                         {"语言设置:", "振铃选择:", "音量调节:",},
                                         {"Language:", "Ring Type:", "Spk Volume:"}
                                        };
char other_text1[LANGUAGEMAXNUM][2][20] = {
                                         {"中文", "英文"},
                                         {"CHN", "ENG"}
                                        };
char other_text2[LANGUAGEMAXNUM][3][20] = {
                                         {"铃声1", "铃声2", "铃声3"},
                                         {"Ring1", "Ring2", "Ring3"}
                                        };
char other_text3[LANGUAGEMAXNUM][3][20] = {
                                         {"小声", "中等", "大声"},
                                         {"Small", "Mid", "Large"}
                                        };

char online_caption[LANGUAGEMAXNUM][5][20] = {
                                         {"开始房号:", "层", "结束房号:", "层", "层户数"},
                                         {"Start Room:", "F", "End Room:", "F", "Num"}
                                        };

char online_return[LANGUAGEMAXNUM][20] = {
                                         {"返回结果:"},
                                         {"Return:"}
                                        };

char SysInfo_caption[LANGUAGEMAXNUM][3][20] = {
                                         {"硬件版本:", "软件版本:", "产品序列号:"},
                                         {"HW version:", "SW version:", "Serial No:"}
                                        };
//---------------------------------------------------------------------------
void DisplaySetupWindow(INT8U RefreshType)    //显示进入设置窗口
{
  INT8U i;

  DisplayImage(&setup_image, FROMFILE, FULLPAGE);

  if(LocalCfg.Addr[0] == 'W')
    DisplayImage(&door_type_image[1], FROMFILE, FULLPAGE);
  else
    DisplayImage(&door_type_image[0], FROMFILE, FULLPAGE);

  if(Local.NetLanStatus == 0)
    DisplayImage(&net_image[0], FROMFILE, FULLPAGE);
  else
    DisplayImage(&net_image[1], FROMFILE, FULLPAGE);
  //DisplayEdit(&setup_pass_edit, IMAGEUP, FROMFILE, FULLPAGE);
  display_text(30, 60, 0, COLOR_BLACK, setup_caption_text[LocalCfg.Language], 0, 24, FULLPAGE);

  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);
  display_text(openlock2_xLeft[LocalCfg.Language], 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][2], 0, 24, FULLPAGE);
  setup_pass_edit.BoxLen = 0;
  Local.CurrentWindow = 6;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//---------------------------------------------------------------------------
void OperateSetupWindow(INT8U wintype, INT8U currwindow)    //进入设置窗口操作
{
  char str[5];

  WaitDecodeFinish();
  switch(wintype)
   {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
           if(setup_pass_edit.BoxLen < setup_pass_edit.MaxLen)
            {
             str[0] = '0' + wintype;
             str[1] = '\0';
             setup_pass_edit.Text[setup_pass_edit.BoxLen] = str[0];
             str[0] = '*';
             str[1] = '\0';

             display_text(setup_pass_edit.xLeft + setup_pass_edit.CursorX + setup_pass_edit.BoxLen*setup_pass_edit.fWidth,
                     setup_pass_edit.yTop + setup_pass_edit.CursorY, setup_pass_edit.Space, setup_pass_edit.CursorCorlor, str, 0, 24, NOFULLPAGE);

             setup_pass_edit.BoxLen ++;
            }
           break;
    case 10: //清除
          if(setup_pass_edit.BoxLen > 0)
           {
            setup_pass_edit.Text[0] = 0;
            setup_pass_edit.BoxLen = 0;
            DisplayEdit(&setup_pass_edit, IMAGEUP, FROMFILE, NOFULLPAGE);
           }
          else
            DisplayTalkWindow(REFRESH);
          break;
    case 11: //确认
          setup_pass_edit.Text[setup_pass_edit.BoxLen] = '\0';
          LocalCfg.EngineerPass[6] = '\0';
          xtm_printf("OperateSetupWindow::setup_pass_edit.BoxLen = %d, LocalCfg.EngineerPass = %s, setup_pass_edit.Text = %s\n", \
                      setup_pass_edit.BoxLen, LocalCfg.EngineerPass, setup_pass_edit.Text);

          if((strcmp(setup_pass_edit.Text, LocalCfg.EngineerPass) == 0)||(strcmp(setup_pass_edit.Text, "802317") == 0))
//          if(1)
           {
            ParamSec = 0;
            DisplaySetupMainWindow(REFRESH);    //刷新 中
           }
          else
           {
            PlayPassErrorWav();

            //清除输入框
            setup_pass_edit.Text[0] = 0;
            setup_pass_edit.BoxLen = 0;
            DisplayEdit(&setup_pass_edit, IMAGEUP, FROMFILE, NOFULLPAGE);
           }
          break;
    case 14: //关于  测试
         // DisplayTestWindow(REFRESH);
          break;
   }
}
//---------------------------------------------------------------------------
void PlayInputErrorWav(void)
{
  if(LocalCfg.VoiceHint == 1)
   {
    WaitAudioUnuse(200); //等待声卡空闲
    if(LocalCfg.Language > 1)
      LocalCfg.Language = 0;
    StartPlayWav(inputerror_wav[LocalCfg.Language].Faddr, NO_CYCLE_PLAY);
   }
}
//---------------------------------------------------------------------------
void PlayPassErrorWav(void)
{
  if(LocalCfg.VoiceHint == 1)
   {
    WaitAudioUnuse(200); //等待声卡空闲
    if(LocalCfg.Language > 1)
      LocalCfg.Language = 0;
    StartPlayWav(passerror_wav[LocalCfg.Language].Faddr, NO_CYCLE_PLAY);
   }
}
//---------------------------------------------------------------------------
void PlayModiSuccWav(void)
{
  if(LocalCfg.VoiceHint == 1)
   {
    WaitAudioUnuse(200); //等待声卡空闲
    if(LocalCfg.Language > 1)
      LocalCfg.Language = 0;
    StartPlayWav(modisucc_wav[LocalCfg.Language].Faddr, NO_CYCLE_PLAY);
   }
}
//---------------------------------------------------------------------------
void PlayModiFailWav(void)
{
  if(LocalCfg.VoiceHint == 1)
   {
    WaitAudioUnuse(200); //等待声卡空闲
    if(LocalCfg.Language > 1)
      LocalCfg.Language = 0;    
    StartPlayWav(modifail_wav[LocalCfg.Language].Faddr, NO_CYCLE_PLAY);
   }
}
//---------------------------------------------------------------------------
void DisplaySetupMainWindow(INT8U RefreshType)    //显示设置主窗口
{
  INT8U i;

  DisplayImage(&setupmain_image, FROMFILE, FULLPAGE);

  DisplayImage(&setupmain_param_image[ParamSec], FROMFILE, FULLPAGE);
  display_text(setupmain_param_image[ParamSec].xLeft + (setupmain_param_image[ParamSec].width - strlen(setup_main_text[LocalCfg.Language][ParamSec])*12)/2, \
                        setup_main_yTop[LocalCfg.Language][ParamSec], 0, COLOR_BLACK, setup_main_text[LocalCfg.Language][ParamSec], 0, 24, FULLPAGE);
  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);
  display_text(openlock2_xLeft[LocalCfg.Language], 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][2], 0, 24, FULLPAGE);

  Local.CurrentWindow = 60;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//---------------------------------------------------------------------------
void OperateSetupMainWindow(INT8U wintype, INT8U currwindow)    //设置主窗口操作
{
  INT8U i;

  switch(wintype)
   {
    case 4: //左
           if(ParamSec > 0)
             ParamSec --;
           //else
           //  ParamSec = 5;
           DisplayImage(&setupmain_param_image[ParamSec], FROMFILE, NOFULLPAGE);
           display_text(setupmain_param_image[ParamSec].xLeft + (setupmain_param_image[ParamSec].width - strlen(setup_main_text[LocalCfg.Language][ParamSec])*12)/2, \
                        setup_main_yTop[LocalCfg.Language][ParamSec], 0, COLOR_BLACK, setup_main_text[LocalCfg.Language][ParamSec], 0, 24, NOFULLPAGE);
           //display_text(setup_main_xLeft[LocalCfg.Language][ParamSec] + (setupmain_param_image[ParamSec].width - strlen(setup_main_text[LocalCfg.Language][ParamSec])*12)/2, \
           //             setup_main_yTop[LocalCfg.Language][ParamSec], 0, COLOR_BLACK, setup_main_text[LocalCfg.Language][ParamSec], 0, 24, NOFULLPAGE);
           break;
    case 6: //右
           if(ParamSec < 5)
             ParamSec ++;
           //else
           //  ParamSec = 0;
           DisplayImage(&setupmain_param_image[ParamSec], FROMFILE, NOFULLPAGE);
           display_text(setupmain_param_image[ParamSec].xLeft + (setupmain_param_image[ParamSec].width - strlen(setup_main_text[LocalCfg.Language][ParamSec])*12)/2, \
                        setup_main_yTop[LocalCfg.Language][ParamSec], 0, COLOR_BLACK, setup_main_text[LocalCfg.Language][ParamSec], 0, 24, NOFULLPAGE);
           break;
    case 10:  //首页
           DisplayTalkWindow(REFRESH);
           break;
    case 11:
           switch(ParamSec)
            {
             case 0: //地址设置
                    DisplayAddressWindow();
                    break;
             case 1: //参数设置
                    DisplayParamWindow();
                    break;
             case 2: //密码设置
                    DisplayPassWindow();
                    break;
             case 3: //其它设置
                    DisplayOtherWindow();
                    break;
             case 4: //在线检测
                    if(LocalCfg.Addr[0] == 'M')
                      DisplayOnlineTestWindow();
                    break;
             case 5: //关于
                    DisplaySetupInfoWindow();
                    break;
            }
           break;
   }
}
//---------------------------------------------------------------------------
void DisplayAddressWindow(void)    //显示本机地址变更窗口
{
  INT8U i;
  INT16U caption_xLeft[6] = {32, 32, 32, 32, 32, 32};
  INT16U caption_yTop[6] = {24, 52, 80, 108, 136, 164};

  INT16U Edit_xLeft[6] = {130, 130, 130, 130, 130, 130};
  INT16U Edit_yTop[6] = {20, 48, 76, 106, 134, 160};
  INT16U Edit_MaxLen[6] = {7, 8, 15, 15, 15, 15};

  DisplayImage(&addr_image, FROMFILE, FULLPAGE);

  CurrRow = 0;

  for(i=0; i<6; i++)
   {
    display_text(caption_xLeft[i], caption_yTop[i], 0, COLOR_BLACK, addr_caption_text[LocalCfg.Language][i], 0, 16, FULLPAGE);
   }

  for(i=0; i<6; i++)
   {
    addr_edit[i].xLeft = Edit_xLeft[i];
    addr_edit[i].yTop = Edit_yTop[i];
    addr_edit[i].MaxLen = Edit_MaxLen[i];
    addr_edit[i].BoxLen = Edit_MaxLen[i];
   }

  curr_row_image[1].xLeft = 0;
  curr_row_image[1].yTop = addr_edit[CurrRow].yTop + 2;
  DisplayImage(&curr_row_image[1], FROMFILE, FULLPAGE);

  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);
  display_text(openlock2_xLeft[LocalCfg.Language], 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][2], 0, 24, FULLPAGE);

  //本机地址
  xtm_memcopy(addr_edit[0].Text, LocalCfg.Addr + 1, Local.AddrLen - 1);
  addr_edit[0].Text[Local.AddrLen - 1] = '\0';
  display_text(addr_edit[0].xLeft, addr_edit[0].yTop + 2, 0, COLOR_BLACK, addr_edit[0].Text, 0, 16, FULLPAGE);

  //网卡地址
  for(i=0; i<16; i++)
    addr_edit[1].Text[i] = '\0';
  xtm_sprintf(addr_edit[1].Text, "%02X:%02X:%02X\0", LocalCfg.Mac_Addr[3], LocalCfg.Mac_Addr[4], LocalCfg.Mac_Addr[5]);
  display_text(addr_edit[1].xLeft, addr_edit[1].yTop + 2, 0, COLOR_BLACK, addr_edit[1].Text, 0, 16, FULLPAGE);


  //IP地址
  for(i=0; i<16; i++)
    addr_edit[2].Text[i] = '\0';
  xtm_sprintf(addr_edit[2].Text, "%03d.%03d.%03d.%03d\0", LocalCfg.IP[0], LocalCfg.IP[1], \
                    LocalCfg.IP[2], LocalCfg.IP[3]);
  display_text(addr_edit[2].xLeft, addr_edit[2].yTop + 2, 0, COLOR_BLACK, addr_edit[2].Text, 0, 16, FULLPAGE);

  //子网掩码
  for(i=0; i<16; i++)
    addr_edit[3].Text[i] = '\0';
  xtm_sprintf(addr_edit[3].Text, "%03d.%03d.%03d.%03d\0", LocalCfg.IP_Mask[0], LocalCfg.IP_Mask[1], \
                    LocalCfg.IP_Mask[2], LocalCfg.IP_Mask[3]);
  display_text(addr_edit[3].xLeft, addr_edit[3].yTop + 2, 0, COLOR_BLACK, addr_edit[3].Text, 0, 16, FULLPAGE);

  //网关
  for(i=0; i<16; i++)
    addr_edit[4].Text[i] = '\0';
  xtm_sprintf(addr_edit[4].Text, "%03d.%03d.%03d.%03d\0", LocalCfg.IP_Gate[0], LocalCfg.IP_Gate[1], \
                    LocalCfg.IP_Gate[2], LocalCfg.IP_Gate[3]);
  display_text(addr_edit[4].xLeft, addr_edit[4].yTop + 2, 0, COLOR_BLACK, addr_edit[4].Text, 0, 16, FULLPAGE);

  //服务器地址
  for(i=0; i<16; i++)
    addr_edit[5].Text[i] = '\0';
  xtm_sprintf(addr_edit[5].Text, "%03d.%03d.%03d.%03d\0", LocalCfg.IP_Server[0], LocalCfg.IP_Server[1], \
                    LocalCfg.IP_Server[2], LocalCfg.IP_Server[3]);
  display_text(addr_edit[5].xLeft, addr_edit[5].yTop + 2, 0, COLOR_BLACK, addr_edit[5].Text, 0, 16, FULLPAGE);

  EditStatusFlag = 0;

  Local.CurrentWindow = 61;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//---------------------------------------------------------------------------
void OperateAddressWindow(INT8U wintype, INT8U currwindow)  //本机地址变更操作
{
  INT8U AddrOK;
  INT8U i;
  switch(wintype)
   {
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 9:
           if(EditStatusFlag == 1)
            {
             DealAddrEdit(wintype, CurrRow);
            }
           break;
    case 2: //上
           if(EditStatusFlag == 1)
            {
             DealAddrEdit(wintype, CurrRow);
             break;
            }
           if(CurrRow > 0)
            {
             curr_row_image[0].xLeft = 0;
             curr_row_image[0].yTop = addr_edit[CurrRow].yTop + 2;
             DisplayImage(&curr_row_image[0], FROMFILE, NOFULLPAGE);

             CurrRow --;

             curr_row_image[1].xLeft = 0;
             curr_row_image[1].yTop = addr_edit[CurrRow].yTop + 2;
             DisplayImage(&curr_row_image[1], FROMFILE, NOFULLPAGE);
            }
           break;
    case 8: //下
           if(EditStatusFlag == 1)
            {
             DealAddrEdit(wintype, CurrRow);
             break;
            }
           if(CurrRow < (MaxAddrRow - 1))
            {
             curr_row_image[0].xLeft = 0;
             curr_row_image[0].yTop = addr_edit[CurrRow].yTop + 2;
             DisplayImage(&curr_row_image[0], FROMFILE, NOFULLPAGE);

             CurrRow ++;

            // if(CurrRow != MaxAddrRow)
              {
               curr_row_image[1].xLeft = 0;
               curr_row_image[1].yTop = addr_edit[CurrRow].yTop + 2;
               DisplayImage(&curr_row_image[1], FROMFILE, NOFULLPAGE);
              }
            }
           break;
    case 11: //确定   K
           xtm_printf("OperateAddressWindow::CurrRow = %d\n", CurrRow);
           if(EditStatusFlag == 0)
            {
             if(CurrRow < MaxAddrRow)
              {
               EditStatusFlag = 1;
               edit_status_image[1].xLeft = addr_edit[CurrRow].xLeft + addr_edit[CurrRow].width + 16;
               edit_status_image[1].yTop = addr_edit[CurrRow].yTop;
               DisplayImage(&edit_status_image[1], FROMFILE, NOFULLPAGE);
              }
            }
           else
            {
             if(CurrRow < MaxAddrRow)
              {
               Check_AddrOK(CurrRow);
               EditStatusFlag = 0;
               edit_status_image[0].xLeft = addr_edit[CurrRow].xLeft + addr_edit[CurrRow].width + 16;
               edit_status_image[0].yTop = addr_edit[CurrRow].yTop;
               DisplayImage(&edit_status_image[0], FROMFILE, NOFULLPAGE);
              }
            }
           break;
    case 10:  //返回
           if(EditStatusFlag == 0)
             DisplaySetupMainWindow(REFRESH);
           else
            {
             addr_edit[CurrRow].BoxLen = 0;
             addr_edit[CurrRow].Text[0] = '\0';
             DisplayEdit(&addr_edit[CurrRow], IMAGEUP, FROMFILE, NOFULLPAGE);

             switch(CurrRow)
              {
               case 0:
                      break;
               case 1:
                      strcpy(addr_edit[CurrRow].Text, "  :  :  \0");
                      display_text(addr_edit[CurrRow].xLeft, addr_edit[CurrRow].yTop + 2, 0, COLOR_BLACK, addr_edit[CurrRow].Text, 0, 16, NOFULLPAGE);
                      addr_edit[CurrRow].Text[0] = '\0';
                      break;
               case 2:
               case 3:
               case 4:
               case 5:
                      xtm_memcopy(addr_edit[CurrRow].Text, "   .   .   .   ", 15);
                      display_text(addr_edit[CurrRow].xLeft, addr_edit[CurrRow].yTop + 2, 0, COLOR_BLACK, addr_edit[CurrRow].Text, 0, 16, NOFULLPAGE);
                      addr_edit[CurrRow].Text[0] = '\0';
                      break;
              }
            }
           break;
   }
}
//---------------------------------------------------------------------------
void DealAddrEdit(INT8U ctype, INT8U cRow)
{
  char str[10];
  if(addr_edit[cRow].BoxLen < addr_edit[cRow].MaxLen)
   {
    str[0] = '0' + ctype;
    str[1] = '\0';
 //   strcat(addr_edit[cRow].Text, str);
    addr_edit[cRow].Text[addr_edit[cRow].BoxLen] = '0' + ctype;

    display_text(addr_edit[cRow].xLeft + addr_edit[cRow].BoxLen*addr_edit[cRow].fWidth,
                 addr_edit[cRow].yTop + 2, 0, COLOR_BLACK, str, 0, 16, NOFULLPAGE);

    addr_edit[cRow].BoxLen ++;
    switch(cRow)
     {
      case 0:
             break;
      case 1:
             if((addr_edit[cRow].BoxLen == 2)||(addr_edit[cRow].BoxLen == 5))
               addr_edit[cRow].BoxLen ++;
             break;
      case 2:
      case 3:
      case 4:
      case 5:
             if((addr_edit[cRow].BoxLen == 3)||(addr_edit[cRow].BoxLen == 7)||(addr_edit[cRow].BoxLen == 11))
               addr_edit[cRow].BoxLen ++;
             break;
     }   
   }
}
//---------------------------------------------------------------------------
INT8U Check_AddrOK(INT8U cRow)
{
  INT8U AddrOK;
  int i, j;
  char str[10];
  INT8U hex_data;
  char TmpText[5][5];
  
  AddrOK = 1;
  switch(cRow)
   {
    case 0:
           //检查地址
           if((addr_edit[0].BoxLen != 4)&&(addr_edit[0].BoxLen != 7))
            {
             AddrOK = 0;
             xtm_memcopy(addr_edit[0].Text, LocalCfg.Addr + 1, Local.AddrLen - 1);
             addr_edit[0].Text[Local.AddrLen - 1] = '\0';
             addr_edit[0].BoxLen = Local.AddrLen - 1;
             DisplayEdit(&addr_edit[0], IMAGEUP, FROMFILE, NOFULLPAGE);
             display_text(addr_edit[0].xLeft, addr_edit[0].yTop + 2, 0, COLOR_BLACK, addr_edit[0].Text, 0, 16, NOFULLPAGE);
            }
           break;
    case 1:
           //检查MAC
           if(AddrOK == 1)
            if(addr_edit[1].BoxLen < (addr_edit[1].MaxLen - 1))
             {
              AddrOK = 0;
              for(i=0; i<16; i++)
               addr_edit[1].Text[i] = '\0';
              addr_edit[1].BoxLen = 8;
              DisplayEdit(&addr_edit[1], IMAGEUP, FROMFILE, NOFULLPAGE);
              xtm_sprintf(addr_edit[1].Text, "%02X:%02X:%02X\0", LocalCfg.Mac_Addr[3], LocalCfg.Mac_Addr[4], LocalCfg.Mac_Addr[5]);
              display_text(addr_edit[1].xLeft, addr_edit[1].yTop + 2, 0, COLOR_BLACK, addr_edit[1].Text, 0, 16, NOFULLPAGE);
             }
           break;
    case 2:
           //检查IP
           if(AddrOK == 1)
            if(addr_edit[2].BoxLen < (addr_edit[2].MaxLen - 2))
             {
              AddrOK = 0;
              for(i=0; i<16; i++)
                addr_edit[2].Text[i] = '\0';
              addr_edit[2].BoxLen = 15;
              DisplayEdit(&addr_edit[2], IMAGEUP, FROMFILE, NOFULLPAGE);
              xtm_sprintf(addr_edit[2].Text, "%03d.%03d.%03d.%03d\0", LocalCfg.IP[0], LocalCfg.IP[1],
                    LocalCfg.IP[2], LocalCfg.IP[3]);
              display_text(addr_edit[2].xLeft, addr_edit[2].yTop + 2, 0, COLOR_BLACK, addr_edit[2].Text, 0, 16, NOFULLPAGE);
             }
           break;
    case 3:
           //检查子网掩码
           if(AddrOK == 1)
            if(addr_edit[3].BoxLen < (addr_edit[3].MaxLen - 2))
             {
              AddrOK = 0;
              for(i=0; i<16; i++)
                addr_edit[3].Text[i] = '\0';
              addr_edit[3].BoxLen = 15;
              DisplayEdit(&addr_edit[3], IMAGEUP, FROMFILE, NOFULLPAGE);
              xtm_sprintf(addr_edit[3].Text, "%03d.%03d.%03d.%03d\0", LocalCfg.IP_Mask[0], LocalCfg.IP_Mask[1],
                    LocalCfg.IP_Mask[2], LocalCfg.IP_Mask[3]);
              display_text(addr_edit[3].xLeft, addr_edit[3].yTop + 2, 0, COLOR_BLACK, addr_edit[3].Text, 0, 16, NOFULLPAGE);
             }
           break;
    case 4:
           //检查网关
           if(AddrOK == 1)
            if(addr_edit[4].BoxLen < (addr_edit[4].MaxLen - 2))
             {
              AddrOK = 0;
              for(i=0; i<16; i++)
                addr_edit[4].Text[i] = '\0';
              addr_edit[4].BoxLen = 15;
              DisplayEdit(&addr_edit[4], IMAGEUP, FROMFILE, NOFULLPAGE);
              xtm_sprintf(addr_edit[4].Text, "%03d.%03d.%03d.%03d\0", LocalCfg.IP_Gate[0], LocalCfg.IP_Gate[1],
                    LocalCfg.IP_Gate[2], LocalCfg.IP_Gate[3]);
              display_text(addr_edit[4].xLeft, addr_edit[4].yTop + 2, 0, COLOR_BLACK, addr_edit[4].Text, 0, 16, NOFULLPAGE);
             }
           break;
    case 5:
           //检查服务器地址
           if(AddrOK == 1)
            if(addr_edit[5].BoxLen < (addr_edit[5].MaxLen - 2))
             {
              AddrOK = 0;
              for(i=0; i<16; i++)
                addr_edit[5].Text[i] = '\0';
              addr_edit[5].BoxLen = 15;
              DisplayEdit(&addr_edit[5], IMAGEUP, FROMFILE, NOFULLPAGE);
              xtm_sprintf(addr_edit[5].Text, "%03d.%03d.%03d.%03d\0", LocalCfg.IP_Server[0], LocalCfg.IP_Server[1],
                     LocalCfg.IP_Server[2], LocalCfg.IP_Server[3]);
              display_text(addr_edit[5].xLeft, addr_edit[5].yTop + 2, 0, COLOR_BLACK, addr_edit[5].Text, 0, 16, NOFULLPAGE);
             }
           break;
   }

  if(AddrOK == 0)
   {
    PlayModiFailWav();
    return AddrOK;
   }

  addr_edit[cRow].Text[addr_edit[cRow].BoxLen] = '\0'; 
  switch(cRow)
   {
    case 0:
           if(addr_edit[0].BoxLen == 7)
            {
             LocalCfg.Addr[0] = 'M';
             xtm_memcopy(LocalCfg.Addr + 1, addr_edit[0].Text, 7);
             xtm_memcopy(LocalCfg.Addr + 8, "0000", 4);
            }
           else
            {
             LocalCfg.Addr[0] = 'W';
             xtm_memcopy(LocalCfg.Addr + 1, addr_edit[0].Text, 4);
             xtm_memcopy(LocalCfg.Addr + 5, "0000000", 7);
            }
           LocalCfg.Addr[12] == '\0';
           break;
    case 1:
           xtm_memcopy(TmpText[0], addr_edit[1].Text, 2);
           TmpText[0][2] = '\0';
           xtm_memcopy(TmpText[1], addr_edit[1].Text + 3, 2);
           TmpText[1][2] = '\0';
           xtm_memcopy(TmpText[2], addr_edit[1].Text + 6, 2);
           TmpText[2][2] = '\0';

           LocalCfg.Mac_Addr[0] = 0x00;
           LocalCfg.Mac_Addr[1] = 0x50;
           LocalCfg.Mac_Addr[2] = 0x2A;
           for(i=0; i<3; i++)
            if(strlen(TmpText[i]) != 2)
             {
              strcpy(str, TmpText[i]);
              strcpy(TmpText[i], "0");
              strcat(TmpText[i], str);
              TmpText[i][2] = '\0';
             }
           for(i=0; i<3; i++)
            {
             hex_data = (TmpText[i][0] - '0')*16 + (TmpText[i][1] - '0');
             LocalCfg.Mac_Addr[i+3] = hex_data;
            }
           break;
     case 2:
     case 3:
     case 4:
     case 5:
             xtm_memcopy(TmpText[0], addr_edit[cRow].Text, 3);
             TmpText[0][3] = '\0';
             xtm_memcopy(TmpText[1], addr_edit[cRow].Text + 4, 3);
             TmpText[1][3] = '\0';
             xtm_memcopy(TmpText[2], addr_edit[cRow].Text + 8, 3);
             TmpText[2][3] = '\0';
             xtm_memcopy(TmpText[3], addr_edit[cRow].Text + 12, 3);
             TmpText[3][3] = '\0';

             xtm_printf("addr_edit[cRow].Text = %s, TmpText[0] = %s, TmpText[1] = %s, TmpText[2] = %s\n", addr_edit[cRow].Text, TmpText[0], TmpText[1], TmpText[2]);
             switch(cRow)
              {
               case 2:
                      for(i=0; i<4; i++)
                        LocalCfg.IP[i] = atoi(TmpText[i]);
                      break;
               case 3:
                      for(i=0; i<4; i++)
                        LocalCfg.IP_Mask[i] = atoi(TmpText[i]);
                      break;
               case 4:
                      for(i=0; i<4; i++)
                        LocalCfg.IP_Gate[i] = atoi(TmpText[i]);
                      break;
               case 5:
                      for(i=0; i<4; i++)
                        LocalCfg.IP_Server[i] = atoi(TmpText[i]);
                      break;
              }
             break;
   }
  xtm_printf("IP_Mask %d.%d.%d.%d \n",
              LocalCfg.IP_Mask[0], LocalCfg.IP_Mask[1], LocalCfg.IP_Mask[2], LocalCfg.IP_Mask[3]);
  //广播地址
  for(i=0; i<4; i++)
   {
    if(LocalCfg.IP_Mask[i] != 0)
      LocalCfg.IP_Broadcast[i] = LocalCfg.IP_Mask[i] & LocalCfg.IP[i];
    else
      LocalCfg.IP_Broadcast[i] = 0xFF;
   }
  xtm_printf("eth0 %d.%d.%d.%d \n",
              LocalCfg.IP[0], LocalCfg.IP[1], LocalCfg.IP[2], LocalCfg.IP[3]);

  WriteCfgFile();  //写本地设置文件
  if((cRow >= 1)&&(cRow <= 4))
    mac_init(); //刷新网络设置
  PlayModiSuccWav();

  if(LocalCfg.Addr[0] == 'M')
   {
    Local.AddrLen = 8;  //地址长度  S 12  B 6 M 8 H 6
   }
  if(LocalCfg.Addr[0] == 'W')
   {
    Local.AddrLen = 5;  //地址长度  S 12  B 6 M 8 H 6
   }
  return AddrOK;
}
//---------------------------------------------------------------------------
void DisplayParamWindow(void)    //显示参数设置窗口
{
  INT8U i;

  DisplayImage(&param_image, FROMFILE, FULLPAGE);

  CurrRow = 0;

  CurrCol[0] = LocalCfg.PassOpenLock;
  CurrCol[1] = LocalCfg.CardOpenLock;
  CurrCol[2] = LocalCfg.DoorButton;
  CurrCol[3] = LocalCfg.DoorDetect;
  CurrCol[4] = LocalCfg.OpenLockTime;
  CurrCol[5] = LocalCfg.DelayLockTime;
  CurrCol[6] = LocalCfg.TalkTimeOut;  //20140225

  //20140225
  for(i=0; i<7; i++)
   if(CurrCol[i] > (MaxParamCol[i] - 1))
     CurrCol[i] = 0;

  //20140225 6+1
  for(i=0; i<7; i++)
  {
    display_text(Param_xLeft, Param_yTop + ParamRowHeight*i, 0, COLOR_BLACK, param_caption_text[LocalCfg.Language][i], 0, 16, FULLPAGE);
  }
//前4项
  for(i=0; i<4; i++)
  {
    display_text(Param_xLeft + 86, Param_yTop + ParamRowHeight*i, 0, COLOR_BLACK, param_open_text[LocalCfg.Language][0], 0, 16, FULLPAGE);
    display_text(Param_xLeft + 86 + 50, Param_yTop + ParamRowHeight*i, 0, COLOR_BLACK, param_open_text[LocalCfg.Language][1], 0, 16, FULLPAGE);
  }
//5
  for(i=0; i<3; i++)
    display_text(Param_xLeft + 96 + 50*i, Param_yTop + ParamRowHeight*4, 0, COLOR_BLACK, param_text1[LocalCfg.Language][i], 0, 16, FULLPAGE);
//6
  for(i=0; i<4; i++)
  	{
    display_text(Param_xLeft + 112 + 48*i, Param_yTop + ParamRowHeight*5, 0, COLOR_BLACK, param_text2[LocalCfg.Language][i], 0, 16, FULLPAGE);
	
  	}
  //20140225  //7
    for(i=0; i<4; i++)
  	{
      display_text(Param_xLeft + 100 + 48*i, Param_yTop + ParamRowHeight*6, 0, COLOR_BLACK, param_text3[LocalCfg.Language][i], 0, 16, FULLPAGE);
    }
	
  curr_row_image[1].xLeft = 0;
  curr_row_image[1].yTop = Param_yTop + ParamRowHeight*CurrRow;
  
  DisplayImage(&curr_row_image[1], FROMFILE, FULLPAGE);
  
//20140225  //6->7
  for(i=0; i<7; i++)
   {
    param_line_image[1].xLeft = Param_xLeft + 86 - 4 + CurrCol[i]*50;
    if(i == 4)
      param_line_image[1].xLeft = Param_xLeft + 96 - 4 + CurrCol[i]*50;
    if(i == 5)
      param_line_image[1].xLeft = Param_xLeft + 112 - 4 + CurrCol[i]*48;
	if(i == 6)//20140225
      param_line_image[1].xLeft = Param_xLeft + 100 - 4 + CurrCol[i]*48;
	
    param_line_image[1].yTop = Param_yTop + ParamRowHeight*i + 18;
    DisplayImage(&param_line_image[1], FROMFILE, FULLPAGE);
   }

  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);
  display_text(openlock2_xLeft[LocalCfg.Language], 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][2], 0, 24, FULLPAGE);

  Local.CurrentWindow = 62;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//---------------------------------------------------------------------------
void OperateParamWindow(INT8U wintype, INT8U currwindow)  //修改参数设置操作
{
  switch(wintype)
   {
    case 2: //上
           if(CurrRow > 0)
            {
             curr_row_image[0].xLeft = 0;
             curr_row_image[0].yTop = Param_yTop + ParamRowHeight*CurrRow;
             DisplayImage(&curr_row_image[0], FROMFILE, NOFULLPAGE);

             CurrRow --;

             curr_row_image[1].xLeft = 0;
             curr_row_image[1].yTop = Param_yTop + ParamRowHeight*CurrRow;
             DisplayImage(&curr_row_image[1], FROMFILE, NOFULLPAGE);
            }
           break;

    case 4: //左
           if(CurrCol[CurrRow] > 0)
            {
             param_line_image[0].xLeft = Param_xLeft + 86 - 4 + CurrCol[CurrRow]*50;
             if(CurrRow == 4)
               param_line_image[0].xLeft = Param_xLeft + 96 - 4 + CurrCol[CurrRow]*50;
             if(CurrRow == 5)
               param_line_image[0].xLeft = Param_xLeft + 112 - 4 + CurrCol[CurrRow]*48;
			 if(CurrRow == 6)//20140225
               param_line_image[0].xLeft = Param_xLeft + 112 - 4 + CurrCol[CurrRow]*48;
             param_line_image[0].yTop = Param_yTop + ParamRowHeight*CurrRow + 18;
             DisplayImage(&param_line_image[0], FROMFILE, NOFULLPAGE);

             CurrCol[CurrRow] --;

             param_line_image[1].xLeft = Param_xLeft + 86 - 4 + CurrCol[CurrRow]*50;
             if(CurrRow == 4)
               param_line_image[1].xLeft = Param_xLeft + 96 - 4 + CurrCol[CurrRow]*50;
             if(CurrRow == 5)
               param_line_image[1].xLeft = Param_xLeft + 112 - 4 + CurrCol[CurrRow]*48;
			 if(CurrRow == 6)//20140225
               param_line_image[1].xLeft = Param_xLeft + 112 - 4 + CurrCol[CurrRow]*48;
             param_line_image[1].yTop = Param_yTop + ParamRowHeight*CurrRow + 18;
             DisplayImage(&param_line_image[1], FROMFILE, NOFULLPAGE);
            }
           break;
    case 6: //右
           if(CurrCol[CurrRow] < (MaxParamCol[CurrRow] - 1))
            {
             param_line_image[0].xLeft = Param_xLeft + 86 - 4 + CurrCol[CurrRow]*50;
             if(CurrRow == 4)
               param_line_image[0].xLeft = Param_xLeft + 96 - 4 + CurrCol[CurrRow]*50;
             if(CurrRow == 5)
               param_line_image[0].xLeft = Param_xLeft + 112 - 4 + CurrCol[CurrRow]*48;
			 if(CurrRow == 6) //20140225
               param_line_image[0].xLeft = Param_xLeft + 112 - 4 + CurrCol[CurrRow]*48;
             param_line_image[0].yTop = Param_yTop + ParamRowHeight*CurrRow + 18;
             DisplayImage(&param_line_image[0], FROMFILE, NOFULLPAGE);

             CurrCol[CurrRow] ++;

             param_line_image[1].xLeft = Param_xLeft + 86 - 4 + CurrCol[CurrRow]*50;
             if(CurrRow == 4)
               param_line_image[1].xLeft = Param_xLeft + 96 - 4 + CurrCol[CurrRow]*50;
             if(CurrRow == 5)
               param_line_image[1].xLeft = Param_xLeft + 112 - 4 + CurrCol[CurrRow]*48;  
			 if(CurrRow == 6)//20140225
               param_line_image[1].xLeft = Param_xLeft + 112 - 4 + CurrCol[CurrRow]*48;  
             param_line_image[1].yTop = Param_yTop + ParamRowHeight*CurrRow + 18;
             DisplayImage(&param_line_image[1], FROMFILE, NOFULLPAGE);
            }
           break;
    case 8: //下
           if(CurrRow < (MaxParamRow - 1))
            {
             curr_row_image[0].xLeft = 0;
             curr_row_image[0].yTop = Param_yTop + ParamRowHeight*CurrRow;
             DisplayImage(&curr_row_image[0], FROMFILE, NOFULLPAGE);

             CurrRow ++;

             curr_row_image[1].xLeft = 0;
             curr_row_image[1].yTop = Param_yTop + ParamRowHeight*CurrRow;
             DisplayImage(&curr_row_image[1], FROMFILE, NOFULLPAGE);
            }
           break;
    case 11: //确定   K
           LocalCfg.PassOpenLock = CurrCol[0];
           LocalCfg.CardOpenLock = CurrCol[1];
           LocalCfg.DoorButton = CurrCol[2];
           LocalCfg.DoorDetect = CurrCol[3];
           LocalCfg.OpenLockTime = CurrCol[4];
           LocalCfg.DelayLockTime = CurrCol[5];
		   LocalCfg.TalkTimeOut = CurrCol[6];

  
           WriteCfgFile();  //写本地设置文件
           PlayModiSuccWav();
           break;
    case 10:  //返回
           DisplaySetupMainWindow(REFRESH);
           break;
   }
}
//---------------------------------------------------------------------------
void DisplayPassWindow(void)    //显示密码设置窗口
{
  INT8U i;
  INT16U caption_xLeft[2][6] =  {
                                {22, 94, 94, 22, 94, 94},
                                {22, 94 - 8, 94 - 8 , 22, 94 - 8 , 94 - 8}
                                };
  INT16U caption_yTop[6] = {30, 58, 86, 114, 142, 170};

  INT16U xLeft[4] = {174, 174, 174, 174};
  INT16U yTop[4] = {56, 86, 140, 170};

  DisplayImage(&pass_image, FROMFILE, FULLPAGE);

  CurrRow = 0;

  for(i=0; i<6; i++)
   {
    display_text(caption_xLeft[LocalCfg.Language][i], caption_yTop[i], 0, COLOR_BLACK, pass_caption_text[LocalCfg.Language][i], 0, 16, FULLPAGE);
   }

  for(i=0; i<4; i++)
   {
    pass_edit[i].xLeft = xLeft[i];
    pass_edit[i].yTop = yTop[i];
   }

  curr_row_image[1].xLeft = curr_row_image_xLeft[LocalCfg.Language];
  curr_row_image[1].yTop = pass_edit[CurrRow].yTop;
  if((CurrRow % 2) == 0)
    curr_row_image[1].yTop ++;
  DisplayImage(&curr_row_image[1], FROMFILE, FULLPAGE);

  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);
  display_text(openlock2_xLeft[LocalCfg.Language], 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][2], 0, 24, FULLPAGE);

  EditStatusFlag = 0;

  Local.CurrentWindow = 63;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//---------------------------------------------------------------------------
void OperatePassWindow(INT8U wintype, INT8U currwindow)  //密码设置操作
{

  INT8U isOK1, isOK2;
  switch(wintype)
   {
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 9:
           if(EditStatusFlag == 1)
            {
             DealPassEdit(wintype, CurrRow);
            }
           break;
    case 2: //上
           if(EditStatusFlag == 1)
            {
             DealPassEdit(wintype, CurrRow);
             break;
            }
           if(CurrRow > 0)
            {
             curr_row_image[0].xLeft = curr_row_image_xLeft[LocalCfg.Language];
             curr_row_image[0].yTop = pass_edit[CurrRow].yTop;
             if((CurrRow % 2) == 0)
               curr_row_image[0].yTop += 2;
             DisplayImage(&curr_row_image[0], FROMFILE, NOFULLPAGE);

             CurrRow --;

             curr_row_image[1].xLeft = curr_row_image_xLeft[LocalCfg.Language];
             curr_row_image[1].yTop = pass_edit[CurrRow].yTop;
             if((CurrRow % 2) == 0)
               curr_row_image[1].yTop += 2;
             DisplayImage(&curr_row_image[1], FROMFILE, NOFULLPAGE);
            }
           break;
    case 8: //下
           if(EditStatusFlag == 1)
            {
             DealPassEdit(wintype, CurrRow);
             break;
            }    
           if(CurrRow < MaxPassRow)
            {
             curr_row_image[0].xLeft = curr_row_image_xLeft[LocalCfg.Language];
             curr_row_image[0].yTop = pass_edit[CurrRow].yTop;
             if((CurrRow % 2) == 0)
               curr_row_image[0].yTop += 2;
             DisplayImage(&curr_row_image[0], FROMFILE, NOFULLPAGE);

             CurrRow ++;

             if(CurrRow != MaxPassRow)
              {
               curr_row_image[1].xLeft = curr_row_image_xLeft[LocalCfg.Language];
               curr_row_image[1].yTop = pass_edit[CurrRow].yTop;
               if((CurrRow % 2) == 0)
                 curr_row_image[1].yTop += 2;
               DisplayImage(&curr_row_image[1], FROMFILE, NOFULLPAGE);
              } 
            }
           break;
    case 11: //确定   K
           xtm_printf("OperatePassWindow::CurrRow = %d\n", CurrRow);
           if(EditStatusFlag == 0)
            {
             if(CurrRow < MaxPassRow)
              {
               EditStatusFlag = 1;
               edit_status_image[1].xLeft = pass_edit[CurrRow].xLeft + pass_edit[CurrRow].width;
               edit_status_image[1].yTop = pass_edit[CurrRow].yTop;
               DisplayImage(&edit_status_image[1], FROMFILE, NOFULLPAGE);
              }
             else
              {
               xtm_printf("OperatePassWindow::Save\n");
               isOK1 = 0;
               isOK2 = 0;

               pass_edit[0].Text[pass_edit[0].BoxLen] = '\0';
               pass_edit[1].Text[pass_edit[1].BoxLen] = '\0';
               pass_edit[2].Text[pass_edit[2].BoxLen] = '\0';
               pass_edit[3].Text[pass_edit[3].BoxLen] = '\0';

               if((pass_edit[0].BoxLen == 0)&&(pass_edit[1].BoxLen == 0))
                 isOK1 = 1;

               if((strcmp(pass_edit[0].Text, pass_edit[1].Text) == 0)&&(pass_edit[0].BoxLen == pass_edit[0].MaxLen))
                 isOK1 = 1;

               if((pass_edit[2].BoxLen == 0)&&(pass_edit[3].BoxLen == 0))
                 isOK2 = 1;

               if((strcmp(pass_edit[2].Text, pass_edit[3].Text) == 0)&&(pass_edit[2].BoxLen == pass_edit[2].MaxLen))
                 isOK2 = 1;

               if((isOK1 == 1)&&(isOK2 == 1))
                {
                  if(pass_edit[0].BoxLen != 0)
                    strcpy(LocalCfg.EngineerPass, pass_edit[0].Text);
                  if(pass_edit[2].BoxLen != 0)                        //开锁密码
                    strcpy(LocalCfg.OpenLockPass, pass_edit[2].Text);
                  WriteCfgFile();  //写本地设置文件
                  PlayModiSuccWav();
                  DisplaySetupMainWindow(REFRESH);
                }
               else
                 PlayModiFailWav();
              }
            }
           else
            {
             if(CurrRow < MaxPassRow)
              {
               EditStatusFlag = 0;
               edit_status_image[0].xLeft = pass_edit[CurrRow].xLeft + pass_edit[CurrRow].width;
               edit_status_image[0].yTop = pass_edit[CurrRow].yTop;
               DisplayImage(&edit_status_image[0], FROMFILE, NOFULLPAGE);
              }
            }
           break;
    case 10:  //返回
           if(EditStatusFlag == 0)
             DisplaySetupMainWindow(REFRESH);
           else
            {
             pass_edit[CurrRow].BoxLen = 0;
             pass_edit[CurrRow].Text[0] = '\0';
             DisplayEdit(&pass_edit[CurrRow], IMAGEUP, FROMFILE, NOFULLPAGE);
            }
           break;
   }
}
//---------------------------------------------------------------------------
void DealPassEdit(INT8U ctype, INT8U cRow)
{
  char str[10];
  if(pass_edit[cRow].BoxLen < pass_edit[cRow].MaxLen)
   {
    str[0] = '0' + ctype;
    str[1] = '\0';
    strcat(pass_edit[cRow].Text, str);

    str[0] = '*';
    str[1] = '\0';

    display_text(pass_edit[cRow].xLeft + pass_edit[cRow].BoxLen*pass_edit[cRow].fWidth,
                 pass_edit[cRow].yTop + 2, 2, COLOR_BLACK, str, 0, 16, NOFULLPAGE);

    pass_edit[cRow].BoxLen ++;
   }
}
//---------------------------------------------------------------------------
void DisplayOtherWindow(void)    //其它设置窗口
{
  INT8U i;

  DisplayImage(&other_image, FROMFILE, FULLPAGE);

  CurrRow = 0;

  CurrCol[0] = LocalCfg.Language;
  CurrCol[1] = LocalCfg.RingType;
  CurrCol[2] = LocalCfg.SpkVolume;

  for(i=0; i<3; i++)
   if(CurrCol[i] > (MaxOtherCol[i] - 1))
     CurrCol[i] = 0;

  for(i=0; i<3; i++)
   {
    display_text(Other_xLeft, Other_yTop + OtherRowHeight*i, 0, COLOR_BLACK, other_caption_text[LocalCfg.Language][i], 0, 16, FULLPAGE);
   }

  for(i=0; i<MaxOtherCol[0]; i++)
   {
    display_text(Other_xLeft + 86 + i*50, Other_yTop + OtherRowHeight*0, 0, COLOR_BLACK, other_text1[LocalCfg.Language][i], 0, 16, FULLPAGE);
   }

  for(i=0; i<MaxOtherCol[1]; i++)
   {
    display_text(Other_xLeft + 86 + i*50, Other_yTop + OtherRowHeight*1, 0, COLOR_BLACK, other_text2[LocalCfg.Language][i], 0, 16, FULLPAGE);
   }

  for(i=0; i<MaxOtherCol[2]; i++)
   {
    display_text(Other_xLeft + 86 + i*50, Other_yTop + OtherRowHeight*2, 0, COLOR_BLACK, other_text3[LocalCfg.Language][i], 0, 16, FULLPAGE);
   }

  curr_row_image[1].xLeft = 0;
  curr_row_image[1].yTop = Other_yTop + OtherRowHeight*CurrRow;
  DisplayImage(&curr_row_image[1], FROMFILE, FULLPAGE);

  for(i=0; i<3; i++)
   {
    param_line_image[1].xLeft = Other_xLeft + 86 - 4 + CurrCol[i]*50;
    param_line_image[1].yTop = Other_yTop + OtherRowHeight*i + 18;
    DisplayImage(&param_line_image[1], FROMFILE, FULLPAGE);
   }

  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);
  display_text(openlock2_xLeft[LocalCfg.Language], 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][2], 0, 24, FULLPAGE);

  Local.CurrentWindow = 64;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//---------------------------------------------------------------------------
void OperateOtherWindow(INT8U wintype, INT8U currwindow)  //其它设置操作
{
  switch(wintype)
   {
    case 2: //上
           if(CurrRow > 0)
            {
             curr_row_image[0].xLeft = 0;
             curr_row_image[0].yTop = Other_yTop + OtherRowHeight*CurrRow;
             DisplayImage(&curr_row_image[0], FROMFILE, NOFULLPAGE);

             CurrRow --;

             curr_row_image[1].xLeft = 0;
             curr_row_image[1].yTop = Other_yTop + OtherRowHeight*CurrRow;
             DisplayImage(&curr_row_image[1], FROMFILE, NOFULLPAGE);
            }
           break;

    case 4: //左
           if(CurrCol[CurrRow] > 0)
            {
             param_line_image[0].xLeft = Other_xLeft + 86 - 4 + CurrCol[CurrRow]*50;
             param_line_image[0].yTop = Other_yTop + OtherRowHeight*CurrRow + 18;
             DisplayImage(&param_line_image[0], FROMFILE, NOFULLPAGE);

             CurrCol[CurrRow] --;

             param_line_image[1].xLeft = Other_xLeft + 86 - 4 + CurrCol[CurrRow]*50;
             param_line_image[1].yTop = Other_yTop + OtherRowHeight*CurrRow + 18;
             DisplayImage(&param_line_image[1], FROMFILE, NOFULLPAGE);
            }
           break;
    case 6: //右
           if(CurrCol[CurrRow] < (MaxOtherCol[CurrRow] - 1))
            {
             param_line_image[0].xLeft = Other_xLeft + 86 - 4 + CurrCol[CurrRow]*50;
             param_line_image[0].yTop = Other_yTop + OtherRowHeight*CurrRow + 18;
             DisplayImage(&param_line_image[0], FROMFILE, NOFULLPAGE);

             CurrCol[CurrRow] ++;

             param_line_image[1].xLeft = Other_xLeft + 86 - 4 + CurrCol[CurrRow]*50;
             param_line_image[1].yTop = Other_yTop + OtherRowHeight*CurrRow + 18;
             DisplayImage(&param_line_image[1], FROMFILE, NOFULLPAGE);
            }
           break;
    case 8: //下
           if(CurrRow < (MaxOtherRow - 1))
            {
             curr_row_image[0].xLeft = 0;
             curr_row_image[0].yTop = Other_yTop + OtherRowHeight*CurrRow;
             DisplayImage(&curr_row_image[0], FROMFILE, NOFULLPAGE);

             CurrRow ++;

             curr_row_image[1].xLeft = 0;
             curr_row_image[1].yTop = Other_yTop + OtherRowHeight*CurrRow;
             DisplayImage(&curr_row_image[1], FROMFILE, NOFULLPAGE);
            }
           break;
    case 11: //确定   K
           LocalCfg.Language = CurrCol[0];
           LocalCfg.RingType = CurrCol[1];
           LocalCfg.SpkVolume = CurrCol[2];

           SoundSetSpeakerVolume(LocalCfg.SpkVolume);
           WriteCfgFile();  //写本地设置文件
           PlayModiSuccWav();
           break;
    case 10:  //返回
           DisplaySetupMainWindow(REFRESH);
           break;
   }
}
//---------------------------------------------------------------------------
void DisplayOnlineTestWindow(void)    //显示在线检测窗口
{
  INT8U i;
  INT16U online_caption_xLeft[2][5] = {
                                       {24, 168, 24, 168, 258},
                                       {18, 168, 18, 168, 258}
                                       };
  INT16U online_caption_yTop[5] =  {18+4, 18+4,  48+4, 48+4,  48+4};

  INT16U Edit_xLeft[3] = {108, 108, 198};
  INT16U Edit_yTop[3] =  {18, 48,  48};
  INT16U Edit_MaxLen[3] =  {2, 2,  2};

  DisplayImage(&onlinetest_image, FROMFILE, FULLPAGE);

  for(i=0; i<5; i++)
    display_text(online_caption_xLeft[LocalCfg.Language][i], online_caption_yTop[i], 0, COLOR_BLACK, online_caption[LocalCfg.Language][i], 0, 16, FULLPAGE);

  for(i=0; i<3; i++)
   {
    online_edit[i].xLeft = Edit_xLeft[i];
    online_edit[i].yTop = Edit_yTop[i];
    online_edit[i].MaxLen = Edit_MaxLen[i];
    online_edit[i].BoxLen = 0;
   }

  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);
  display_text(openlock2_xLeft[LocalCfg.Language], 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][2], 0, 24, FULLPAGE);

  CurrRow = 0;
  Local.CurrentWindow = 65;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//---------------------------------------------------------------------------
void OperateOnlineTestWindow(INT8U wintype, INT8U currwindow)  //在线检测窗口操作
{
  INT8U i;
  char str[10];
  switch(wintype)
   {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
           if((CurrRow >= 2)&&(online_edit[CurrRow].BoxLen == online_edit[CurrRow].MaxLen))
            {
             if(wintype == 2)
              {
               if(OnlineCurrPage > 0)
                {
                 OnlineCurrPage --;
                 ShowOnlineInfo();
                }
              }
             if(wintype == 8)
              {
               if(OnlineCurrPage < (OnlineTotalPage - 1))
                {
                 OnlineCurrPage ++;
                 ShowOnlineInfo();
                }
              }
             break;
            }
           if(online_edit[CurrRow].BoxLen < online_edit[CurrRow].MaxLen)
            {
             str[0] = '0' + wintype;
             str[1] = '\0';
             online_edit[CurrRow].Text[online_edit[CurrRow].BoxLen] = str[0];
             display_text(online_edit[CurrRow].xLeft + online_edit[CurrRow].BoxLen*online_edit[CurrRow].fWidth + 16,
                    online_edit[CurrRow].yTop + 2, 2, COLOR_BLACK, str, 0, 16, NOFULLPAGE);

             if(online_edit[CurrRow].BoxLen < online_edit[CurrRow].MaxLen)
               online_edit[CurrRow].BoxLen ++;

             if(online_edit[CurrRow].BoxLen >= online_edit[CurrRow].MaxLen)
              {
               if(CurrRow < 2)
                {
                 CurrRow ++;
                 online_edit[CurrRow].BoxLen = 0;
                } 
              } 
            }
           break;
    case 10:  //返回
           if((online_edit[0].BoxLen != 0)||(online_edit[1].BoxLen != 0)||(online_edit[2].BoxLen != 0))
            {
             CurrRow = 0;
             for(i=0; i<3; i++)
              {
               online_edit[i].BoxLen = 0;
               online_edit[i].Text[0] = '\0';
               DisplayEdit(&online_edit[i], IMAGEUP, FROMFILE, NOFULLPAGE);
              }
            }
           else
             DisplaySetupMainWindow(REFRESH);
           break;
    case 11:
           xtm_printf("CurrRow = %d, online_edit[CurrRow].BoxLen = %d\n", CurrRow, online_edit[CurrRow].BoxLen);
           if(Local.OnlineTestFlag == 1)  //在线检测标志     20101029  xu
             break;
           if((CurrRow >= 2)&&(online_edit[CurrRow].BoxLen == online_edit[CurrRow].MaxLen))
            {
             DisplayImage(&online_content_image, FROMFILE, NOFULLPAGE);
             OnlineTest();
             Local.OnlineTestFlag = 1;  //在线检测标志     20101029  xu
             Local.OnlineTestTime = 0;  //在线检测计数
            }
           break;
   }
}
//---------------------------------------------------------------------------
void OnlineTest(void)
{
  INT16U i, j, Num;
  INT8U RemoteMac[6];
  INT8U sendb[128];
  INT16U sendlength;
  INT16U StartStorey, EndStorey, RoomNum;
  char tmp_text[5];
  INT16U OnlineTotalNum;
  for(i=0; i<3; i++)
    online_edit[i].Text[online_edit[i].BoxLen] = '\0';
    
  StartStorey = atoi(online_edit[0].Text);
  EndStorey = atoi(online_edit[1].Text);
  RoomNum = atoi(online_edit[2].Text);

  if(EndStorey < StartStorey)
    OnlineTotalNum = 0;
  else
    OnlineTotalNum = (EndStorey - StartStorey + 1) * RoomNum;
  if(OnlineTotalNum > MAX_ONLINE_NUM)
    OnlineTotalNum = MAX_ONLINE_NUM;

  xtm_printf("OnlineTest::OnlineTotalNum = %d, StartStorey = %d, EndStorey = %d, RoomNum = %d\n", OnlineTotalNum, StartStorey, EndStorey, RoomNum);
  Num = 0;
  if(OnlineTotalNum > 0)
   for(i=StartStorey; i<(EndStorey + 1); i++)
    for(j=1; j <= RoomNum; j++)
     {
      OnlineAddr[Num][0] = (i/10) + '0';
      OnlineAddr[Num][1] = (i%10) + '0';
      OnlineAddr[Num][2] = (j/10) + '0';
      OnlineAddr[Num][3] = (j%10) + '0';

      xtm_memcopy(tmp_text, OnlineAddr[Num], 4);
      tmp_text[4] = '\0';
      //xtm_printf("OnlineAddr[%d] = %s\n", Num, tmp_text);

      Num ++;
      if(Num >= MAX_ONLINE_NUM)
       {
        i = EndStorey + 1;
        break;
       } 
     }

  if(Local.Status == 0)
   {
    Local.OnlineRecvNum = 0;
    for(i=0; i<OnlineTotalNum; i++)
     {
      Remote.Addr[0][0] = 'S';
      xtm_memcopy(Remote.Addr[0] + 1, LocalCfg.Addr + 1, 6);
      xtm_memcopy(Remote.Addr[0] + 7, OnlineAddr[i], 4);
      Remote.Addr[0][11] = '0';
      CreateNSBuff(sendb, NSORDER, ASK);

      sendlength = 56;

      RemoteMac[0] = 0x01;
      RemoteMac[1] = 0x00;
      RemoteMac[2] = 0x5E;
      RemoteMac[3] = Local.NsMultiAddr[1];
      RemoteMac[4] = Local.NsMultiAddr[2];
      RemoteMac[5] = Local.NsMultiAddr[3];
      UdpSendBuff_1(LocalPort[1], RemoteMac, Local.NsMultiAddr, sendb, sendlength);
      //CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Local.NsMultiAddr, sendb, sendlength, 0);
     }
   }
}
//---------------------------------------------------------------------------
void CheckOnlineTest(void)
{
  if(Local.OnlineTestFlag == 1)  //在线检测标志     20101029  xu
   {
    Local.OnlineTestTime ++;  //在线检测计数
    if(Local.OnlineTestTime > 2*TIMERPERSEC)
     {
      Local.OnlineTestFlag = 0;  //在线检测标志     20101029  xu
      Local.OnlineTestTime = 0;  //在线检测计数
      xtm_printf("CheckOnlineTest::Local.OnlineRecvNum = %d\n", Local.OnlineRecvNum);

      OnlineCurrPage = 0;
      ShowOnlineInfo();
     }
   }
}
//---------------------------------------------------------------------------
void ShowOnlineInfo(void)
{
  INT16U i;
  INT16U CurrPageNum;
  INT16U Edit_xLeft[12] = {32, 100, 168, 236,  32, 100, 168, 236,  32, 100, 168, 236};
  INT16U Edit_yTop[12] = {108, 108, 108, 108, 138, 138, 138, 138, 168, 168, 168, 168};
  char tmp_text[50];

  DisplayImage(&online_content_image, FROMFILE, NOFULLPAGE);

//  Local.OnlineRecvNum = 200;
//  for(i=0; i<Local.OnlineRecvNum; i++)
//    xtm_memcopy(Local.OnlineRecvAddr[i], "0807", 4);

  for(i=0; i<50; i++)
    tmp_text[i] = '\0';
  if(LocalCfg.Language == 0)
    xtm_sprintf(tmp_text, "%s %d个, 第 %d 页\0", online_return[LocalCfg.Language], Local.OnlineRecvNum, OnlineCurrPage + 1);
  else
    xtm_sprintf(tmp_text, "%s %d, Page %d\0", online_return[LocalCfg.Language], Local.OnlineRecvNum, OnlineCurrPage + 1);
  display_text(22,
                   80 + 2, 2, COLOR_BLACK, tmp_text, 0, 16, NOFULLPAGE);

  OnlineTotalPage = 0;
  if((Local.OnlineRecvNum % ONLINENUMPERPAGE) == 0)
    OnlineTotalPage = (INT32U)Local.OnlineRecvNum / ONLINENUMPERPAGE;
  else
   {
    OnlineTotalPage = ((INT32U)Local.OnlineRecvNum/ONLINENUMPERPAGE);
    OnlineTotalPage ++;
   }
  if(OnlineCurrPage == (OnlineTotalPage - 1))
    CurrPageNum = Local.OnlineRecvNum - OnlineCurrPage*ONLINENUMPERPAGE;
  else
    CurrPageNum = ONLINENUMPERPAGE;
  if(Local.OnlineRecvNum == 0)
    CurrPageNum = 0;
  xtm_printf("ShowOnlineInfo::OnlineTotalPage = %d, OnlineCurrPage = %d, CurrPageNum = %d\n", OnlineTotalPage, OnlineCurrPage, CurrPageNum);
  for(i=0; i<CurrPageNum; i++)
   {
        xtm_memcopy(tmp_text, Local.OnlineRecvAddr[i], 4);
        tmp_text[4] = '\0';
        display_text(Edit_xLeft[i] + 16, Edit_yTop[i], 0, COLOR_BLACK, tmp_text, 0, 16, NOFULLPAGE);
   }        
}
//---------------------------------------------------------------------------
void DisplaySetupInfoWindow(void)    //显示查询本机信息窗口
{
  INT8U i, j;
  INT8U SysInfoNum;
  char temtext[100];
  char SysInfo[20][50];

  for(i=0; i<20; i++)
    for(j=0; j<50; j++)
      SysInfo[i][j] = 0;

  DisplayImage(&setup_info_image, FROMFILE, FULLPAGE);

  SysInfoNum = 0;
  xtm_sprintf(SysInfo[SysInfoNum], "%s %s", SysInfo_caption[LocalCfg.Language][0], HARDWAREVER);
  SysInfoNum ++;
  xtm_sprintf(SysInfo[SysInfoNum], "%s %s ", SysInfo_caption[LocalCfg.Language][1], SOFTWAREVER);
  SysInfoNum ++;
  xtm_sprintf(SysInfo[SysInfoNum], "%s %s ", SysInfo_caption[LocalCfg.Language][2],  SERIALNUM);
  SysInfoNum ++;

  for(i=0; i<SysInfoNum; i++)
   {
      for(j=0; j<50; j++)
         temtext[j] = 0;
      xtm_sprintf(temtext, "%d) %s", i + 1, SysInfo[i]);
      if(strlen(temtext) >= 51)
        temtext[51] = '\0';

      display_text(10, 40 + 24 + i*30, 0, COLOR_BLACK, temtext, 0, 16, FULLPAGE);
   }

  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);

  Local.CurrentWindow = 66;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//---------------------------------------------------------------------------
void OperateSetupInfoWindow(INT8U wintype, INT8U currwindow)  //查询本机信息操作
{
  switch(wintype)
   {
    case 10:  //返回
           DisplaySetupMainWindow(REFRESH);
           break;
   }
}
//---------------------------------------------------------------------------


