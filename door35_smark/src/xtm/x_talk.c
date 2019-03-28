#include "includes.h"

#include "stdlib.h"

#define x_talk_h

#define CommonH
#include "common.h"

/*
****************************************
20131128
将添加LOGO和提示文本
****************************************
*/


void Talk_Func(void);       //通话 接听
void Call_Func(INT8U uFlag, char *call_addr, char *CenterAddr);    //呼叫   1  中心  2 住户
void TalkEnd_Func(void);
void WatchEnd_Func(void);
void CallTimeOut_Func(void); //呼叫超时
void CheckDelayOpenLock_Func(void);//检查开锁延时
void OpenLock_Func(void);  //开锁

void CreateTalkBuff(INT8U *send_b, INT8U Order1, INT8U Order2, INT8U Order3, INT8U *Remote_Addr, INT8U *Remote_IP);

void CreateNSBuff(INT8U *send_b, INT8U Order1, INT8U Order2);

void DisplayTalkWindow(INT8U RefreshType);    //显示对讲呼出窗口
void OperateTalkWindow(INT8U wintype, INT8U currwindow);    //对讲呼出窗口操作

void ClearTalkEdit(INT8U FullPageType);  //清 文本框

struct TalkCall1 TalkCall;

void DisplayWatchWindow(INT8U RefreshType);    //显示监视窗口
void OperateWatchWindow(INT8U wintype, INT8U currwindow);    //监视窗口操作
void ShowTalkWatchTime(INT8U cType, INT8U RefreshType); //显示监视时间   cType  0  监视   1  呼叫正在连接  2 通话

INT8U TalkConnectImageNo;
void DisplayTalkConnectWindow(void);    //显示对讲 正在连接窗口
void OperateTalkConnectWindow(INT8U wintype, INT8U currwindow);    //对讲 正在连接窗口操作
void ShowTalkConnectImage(INT8U RefreshType); //呼叫正在连接

void DisplayTalkStartWindow(void);    //显示对讲 通话窗口
void OperateTalkStartWindow(INT8U wintype, INT8U currwindow);    //对讲 通话窗口操作

void DisplayOpenLockWindow(void);    //显示密码开锁窗口
void OperateOpenLockWindow(INT8U wintype, INT8U currwindow);    //密码开锁窗口操作

void DisplayOpenLockResultWindow(void);    //显示密码开锁成功窗口
void OperateOpenLockResultWindow(INT8U wintype, INT8U currwindow);    //密码开锁窗口成功操作

void DisplayCallFailWindow(INT8U cType);    //显示呼叫失败窗口
void OperateCallFailWindow(INT8U wintype, INT8U currwindow);    //呼叫失败窗口操作

void PlayBackRingWav(void);
void PlayDoorOpenWav(void);

INT16U r2r_xLeft[3][4] = {
                        {50, 66, 82, 98},
                        {168, 184, 0, 0},
                        {24, 98, 168, 240}
                       };
//20131211
INT16U r2r_yTop[3][4] = {
                        {32, 32, 32, 32},
                        {32, 32, 0, 0},
                        {78, 78, 78, 78}
                       };
char talk_caption_text[LANGUAGEMAXNUM][30] = {
                                         {"请输入房号..."},
                                         {"Please Input..."}
                                        };
char talk_text[LANGUAGEMAXNUM][5][30] = {
                                         {"栋", "单元", "*", " - 删除", "# - 呼叫"},
                                         {"Bd", "Unit", "*", " - Del", "# - Call"}
                                        };

char openlock_caption_text[LANGUAGEMAXNUM][30] = {
                                         {"请输入开锁密码:"},
                                         {"Unlock password:"}
                                        };

INT16U openlock2_xLeft[2] = {190, 180};                                        
char openlock_text[LANGUAGEMAXNUM][3][30] = {
                                         {"*", " - 返回", "# - 确定"},
                                         {"*", " - Return", "# - Confirm"}
                                        };

char openlock_result_text[LANGUAGEMAXNUM][30] = {
                                         {"门锁已打开"},
                                         {"Door unlock"}
                                         //{"Door lock is open"}
                                        };

char openlock_result_text1[LANGUAGEMAXNUM][30] = {
                                         {"开锁"},
                                         {"unlock"}
                                        };

char call_fail_text[LANGUAGEMAXNUM][2][30] = {
                                         {"呼叫失败...", "对方正忙..."},
                                         {"Call fail...", "Busy..."}
                                        };

char 
attention_text[2][40]={"1.输入四位房间号,按#确认\0",
                       "2.呼叫中心机,按0和#确认\0"};

//---------------------------------------------------------------------------
void CreateTalkBuff(INT8U *sendb, INT8U Order1, INT8U Order2, INT8U Order3, INT8U *Remote_Addr, INT8U *Remote_IP)
{
    //头部
    xtm_memcopy(sendb, UdpPackageHead, 6);
    //命令  ,子网广播解析
    sendb[6] = Order1;
    //Multi_Udp_Buff[i].CurrOrder = Multi_Udp_Buff[i].buf[6];
    sendb[7] = Order2;    //主叫
    sendb[8] = Order3;


    //本机为主叫方
    if((Local.Status == 1)||(Local.Status == 3)||(Local.Status == 5)||(Local.Status == 7)||(Local.Status == 9)
       ||(Local.Status == 0))
     {
      xtm_memcopy(sendb+9, LocalCfg.Addr, 20);
      xtm_memcopy(sendb+29, LocalCfg.IP, 4);
      xtm_memcopy(sendb+33, Remote_Addr, 20);
      xtm_memcopy(sendb+53, Remote_IP, 4);
     }
    //本机为被叫方
    if((Local.Status == 2)||(Local.Status == 4)||(Local.Status == 6)||(Local.Status == 8)||(Local.Status == 10))
     {
      xtm_memcopy(sendb+9, Remote_Addr, 20);
      xtm_memcopy(sendb+29, Remote_IP, 4);
      xtm_memcopy(sendb+33, LocalCfg.Addr, 20);
      xtm_memcopy(sendb+53, LocalCfg.IP, 4);
     }
}
//---------------------------------------------------------------------------
void CreateNSBuff(INT8U *sendb, INT8U Order1, INT8U Order2)
{
    //头部
    xtm_memcopy(sendb, UdpPackageHead, 6);
    //命令  ,子网广播解析
    sendb[6] = Order1;
    sendb[7] = Order2;    //主叫

    //本机为主叫方
    xtm_memcopy(sendb+8, LocalCfg.Addr, 20);
    xtm_memcopy(sendb+28, LocalCfg.IP, 4);
    xtm_memcopy(sendb+32, Remote.Addr[0], 20);
    Remote.IP[0][0] = 0;
    Remote.IP[0][1] = 0;
    Remote.IP[0][2] = 0;
    Remote.IP[0][3] = 0;
    xtm_memcopy(sendb+52, Remote.IP[0], 4);
}
//---------------------------------------------------------------------------
void Talk_Func(void)       //通话 接听
{
  INT8U sendb[128];
  INT16U sendlength;

  if(Local.Status == 2)  //状态为被对讲
   {
    xtm_printf("Talk_Func\n");
    StopPlayWavFile();  //关闭铃音
    if(LocalCfg.EncodeFrameSize == VIDEO_VGA)
      Local.VideoType = 0x10;
    else
      Local.VideoType = 0x08;
    CreateTalkBuff(sendb, VIDEOTALK, Local.MachineType | Local.VideoType | ASK, CALLSTART, Remote.Addr[0], Remote.IP[0]);

    sendlength = 57;

    CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Remote.IP[0], sendb, sendlength, 0);
   }
}
//---------------------------------------------------------------------------
void TalkEnd_Func(void)
{
  INT8U sendb[128];
  INT16U sendlength;
  INT8U j;
  if((Local.Status == 1)||(Local.Status == 2)||(Local.Status == 5)||(Local.Status == 6)
    ||(Local.Status == 7)||(Local.Status == 8)||(Local.Status == 9)||(Local.Status == 10))  //状态为对讲
     {
      xtm_printf("TalkEnd_Func\n");
      for(j=0; j<Remote.DenNum; j++)
       {
        CreateTalkBuff(sendb, VIDEOTALK, Local.MachineType | ASK, CALLEND, Remote.Addr[j], Remote.IP[j]);

        sendlength = 57;

        CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Remote.IP[j], sendb, sendlength, 0);
       }
     }
}
//---------------------------------------------------------------------------
void WatchEnd_Func(void)
{
  INT8U sendb[128];
  INT16U sendlength;
  if((Local.Status == 3)||(Local.Status == 4)||(Local.Status == 90))  //状态为监视
   {
    xtm_printf("WatchEnd_Func \n");
    CreateTalkBuff(sendb, VIDEOWATCH, Local.MachineType | ASK, CALLEND, Remote.Addr[0], Remote.IP[0]);

    sendlength = 57;

    if(Local.Status == 90)
        {
        xtm_memcopy(sendb, UdpPackageHead2, 4);
        sendb[4]=8;
        sendb[5]=SMARK_MONITOR;
        sendb[6]=4;
        sendb[7]=4;
        sendb[8]=0;
    CreateMultiUdpBuff(LocalPort[3], MULTI_SEND, Remote.IP[0], sendb, sendlength, 0);
        }
    else
        {
        CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Remote.IP[0], sendb, sendlength, 0);
        }
   }
}
//---------------------------------------------------------------------------
void Call_Func(INT8U uFlag, char *call_addr, char *CenterAddr)    //呼叫   1  中心  2 住户
{
  int i;
  INT8U sendb[128];
  INT16U sendlength;
  Local.ForceEndWatch = 0;  //有呼叫时，强制关监视
  xtm_memcopy(Remote.Addr[0], NullAddr, 20);

  xtm_printf("Call_Func::call_addr = %s, Local.PreStatus = %d\n", call_addr, Local.PreStatus);

  switch(uFlag)
   {
    case 1: //呼叫中心
            xtm_memcopy(Remote.Addr[0],CenterAddr,12);
            break;
    case 2: //呼叫住户
            switch(LocalCfg.Addr[0])
             {
                          case 'M':
                                   Remote.Addr[0][0] = 'S';
                                   xtm_memcopy(Remote.Addr[0] + 1, LocalCfg.Addr + 1, 6);
                                   xtm_memcopy(Remote.Addr[0] + 7, call_addr, 4);
                                   Remote.Addr[0][11] = '0';
                                   break;
                          case 'W':
                                  if(strlen(call_addr) == 4) //别墅
                                   {
                                    Remote.Addr[0][0] = 'B';
                                    xtm_memcopy(Remote.Addr[0] + 1, call_addr, 4);
                                    Remote.Addr[0][11] = '0';
                                   }
                                  else
                                   {
                                    Remote.Addr[0][0] = 'S';
                                    xtm_memcopy(Remote.Addr[0] + 1, call_addr, 10);
                                    Remote.Addr[0][11] = '0';
                                   }
                                  break;
             }
            break;
    case 3: //中心人工转接
            xtm_memcopy(Remote.Addr[0], call_addr, 12);
            Remote.Addr[0][11] = '0';
            Remote.Addr[0][12] = '\0';
            break;
   }

  xtm_printf("Remote.Addr[0] = %s\n", Remote.Addr[0]);
  if(Local.Status == 0)
   {
      if(Local.PreStatus == 1)  //20130115 预状态，防止同时的操作
        return;
      Local.PreStatus = 1;

      CreateNSBuff(sendb, NSORDER, ASK);

      sendlength = 56;

      CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Local.NsMultiAddr, sendb, sendlength, VIDEOTALK);
   }
  else
   {
    xtm_printf("本机正忙,无法呼叫\n");
   }
}
//---------------------------------------------------------------------------
//呼叫超时
void CallTimeOut_Func(void)
{
  //StopPlayWavFile();  //关闭铃音
  //如本机为被叫方
  //查看SD是否存在和留影标志 和 主叫方地址为围墙机和门口机和别墅门口机
  xtm_printf("Local.Status = %d, LocalCfg.RecPhoto = %d, Remote.Addr[0][0] = %c\n", \
         Local.Status, LocalCfg.RecPhoto, Remote.Addr[0][0]);

  if((Local.Status == 1)||(Local.Status == 2)||(Local.Status == 5)||(Local.Status == 6)
    ||(Local.Status == 7)||(Local.Status == 8)||(Local.Status == 9)||(Local.Status == 10))
      TalkEnd_Func();
  if(Local.Status == 0)
    Local.OnlineFlag = 0;

  xtm_printf("呼叫超时\n");
}
//---------------------------------------------------------------------------
void CheckDelayOpenLock_Func(void)//检查开锁延时
{
    if(LocalCfg.DelayLockTime > 0)
     {
      Local.OpenDoorFlag = 3;   //开锁标志 0 未开锁  1 开锁延时中  2 开锁中  3 延时开锁中
      Local.OpenDoorTime = 0;   //时间
      xtm_printf("延时开锁中...\n");
     }
    else
      OpenLock_Func();  //开锁
}
//---------------------------------------------------------------------------
void OpenLock_Func(void)  //开锁
{
    Local.OpenDoorFlag = 2;   //开锁标志 0 未开锁  1 开锁延时中  2 开锁中  3 延时开锁中
    Local.OpenDoorTime = 0;   //时间
    xtm_printf("开锁中..., LocalCfg.DelayLockTime = %d\n", LocalCfg.DelayLockTime);
    //打开门锁   
    SetGpio(GPIOA_OUT_DATA, OPEN_LOCK_HIGH, SET_HIGH); //开门锁

    if(Local.CurrentWindow == 2)
      display_text(190 + 40, 210, 0, COLOR_BLACK, openlock_result_text1[LocalCfg.Language], 0, 24, NOFULLPAGE);

    if(Local.CurrentWindow == 3) //通话连接界面.通话界面
     {
       if(Local.TalkStartOpenLockFlag == 0)
        {
         DisplayImage(&talk_start_openlock[1], FROMFILE, NOFULLPAGE);
         display_text(talk_start_openlock[1].xLeft + 20, talk_start_openlock[1].yTop + 20, 0, COLOR_BLACK, openlock_result_text[LocalCfg.Language], 0, 16, NOFULLPAGE);
        }
       Local.TalkStartOpenLockFlag = 1;  //通话开锁标志     20101029  xu
       Local.TalkStartOpenLockTime = 0;  //通话开锁计数
     }
}
//--------------------------------------------------------------------------
void DisplayTalkWindow(INT8U RefreshType)    //显示对讲呼出窗口
{
  INT8U i, tmp;
  
  DisplayImage(&talk1_image, FROMFILE, FULLPAGE);

  DisplayImage(&logo_image, FROMFILE, FULLPAGE);
  //xtm_printf("1\n");

  if(LocalCfg.Addr[0] == 'M')
   {
    CurrBox = 2;
    for(i=0; i<4; i++)
     {
      tmp = LocalCfg.Addr[i+1] - '0';
      small_num_image[tmp].xLeft = r2r_xLeft[0][i];
      small_num_image[tmp].yTop = r2r_yTop[0][i]+30;
      DisplayImage(&small_num_image[tmp], FROMFILE, FULLPAGE);
      //xtm_printf("%d\n", 2+i);
     }
    for(i=0; i<2; i++)
     {
      tmp = LocalCfg.Addr[i+5] - '0';
      small_num_image[tmp].xLeft = r2r_xLeft[1][i];
      small_num_image[tmp].yTop = r2r_yTop[1][i]+30;
      DisplayImage(&small_num_image[tmp], FROMFILE, FULLPAGE);
      //xtm_printf("%d\n", 6+i);
     }
   }
  else
   {
    CurrBox = 0;
   }

  r2r_edit[0].BoxLen = 0;         //文本框当前输入长度
  r2r_edit[0].MaxLen = 4;         //文本框最大输入长度
  r2r_edit[1].BoxLen = 0;         //文本框当前输入长度
  r2r_edit[1].MaxLen = 2;         //文本框最大输入长度
  r2r_edit[2].BoxLen = 0;         //文本框当前输入长度]
  r2r_edit[2].MaxLen = 4;         //文本框最大输入长度
//  ClearTalkEdit(FULLPAGE);  //清 文本框


  display_text(80, 96, 0, COLOR_BLACK, talk_caption_text[LocalCfg.Language], 0, 24, FULLPAGE);
  display_text(128, 30+30, 0, COLOR_RED, talk_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(212, 30+30, 0, COLOR_RED, talk_text[LocalCfg.Language][1], 0, 24, FULLPAGE);
  display_text(14, 210, 0, COLOR_BLACK, talk_text[LocalCfg.Language][2], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, talk_text[LocalCfg.Language][3], 0, 24, FULLPAGE);
  display_text(190, 210, 0, COLOR_BLACK, talk_text[LocalCfg.Language][4], 0, 24, FULLPAGE);
  
  //添加提示文本
  display_text(14, 130, 0, COLOR_BLACK, attention_text[0], 0, 24, FULLPAGE);
  display_text(14, 160, 0, COLOR_BLACK, attention_text[1], 0, 24, FULLPAGE);


  //xtm_printf("13\n");
  
  Local.CurrentWindow = 0;

  Local.PreStatus = 0;  //20130115 预状态，防止同时的操作
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);

  SoundSetSpeakerVolume(LocalCfg.SpkVolume);
  //xtm_printf("14\n");
}
//--------------------------------------------------------------------------
void OperateTalkWindow(INT8U wintype, INT8U currwindow)    //对讲呼出窗口操作
{
  INT8U i;
  INT8U InputOK, tmp;
  int tmplen;
  char tmpchar[10];  
  char str[5];
  char tmp_text[20];
  
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
           if(CurrBox == 2)
             if(r2r_edit[CurrBox].BoxLen == 0)
               ClearTalkEdit(NOFULLPAGE);  //清 文本框
           if(r2r_edit[CurrBox].BoxLen < r2r_edit[CurrBox].MaxLen)
            {
             str[0] = '0' + wintype;
             str[1] = '\0';
             r2r_edit[CurrBox].Text[r2r_edit[CurrBox].BoxLen] = str[0];
            // display_text(r2r_edit[CurrBox].xLeft + r2r_edit[CurrBox].CursorX + r2r_edit[CurrBox].BoxLen*r2r_edit.fWidth,
            //         r2r_edit.yTop + r2r_edit.CursorY, r2r_edit.Space, r2r_edit.CursorCorlor, str, 0, 24, NOFULLPAGE);

             if((CurrBox == 0)||(CurrBox == 1))
              {
               small_num_image[wintype].xLeft = r2r_xLeft[CurrBox][r2r_edit[CurrBox].BoxLen];
               small_num_image[wintype].yTop = r2r_yTop[CurrBox][r2r_edit[CurrBox].BoxLen];
               DisplayImage(&small_num_image[wintype], FROMFILE, NOFULLPAGE);
              }
             else
              {
               big_num_image[wintype].xLeft = r2r_xLeft[CurrBox][r2r_edit[CurrBox].BoxLen];
               big_num_image[wintype].yTop = r2r_yTop[CurrBox][r2r_edit[CurrBox].BoxLen];
               DisplayImage(&big_num_image[wintype], FROMFILE, NOFULLPAGE);
              }
             r2r_edit[CurrBox].BoxLen ++;
             switch(CurrBox)
              {
               case 0:
               case 1:
                      if(r2r_edit[CurrBox].BoxLen >= r2r_edit[CurrBox].MaxLen)
                        CurrBox ++;
                      break;
              }
            }
           break;
    case 10: //清除
          if(Local.Status != 0)
           {
            if((Local.Status == 3)||(Local.Status == 4))
              WatchEnd_Func();
            else
              TalkEnd_Func();
            break;
           }

          if(LocalCfg.Addr[0] == 'M')
           {
            ClearTalkEdit(NOFULLPAGE);  //清 文本框
           }
          else
           {
            CurrBox = 0;
            ClearTalkEdit(NOFULLPAGE);  //清 文本框
            CurrBox = 1;
            ClearTalkEdit(NOFULLPAGE);  //清 文本框
            CurrBox = 2;
            ClearTalkEdit(NOFULLPAGE);  //清 文本框
            CurrBox = 0;
           }

     for(i=0; i<4; i++)
     {
      tmp = LocalCfg.Addr[i+1] - '0';
      small_num_image[tmp].xLeft = r2r_xLeft[0][i];
      small_num_image[tmp].yTop = r2r_yTop[0][i]+30;
      DisplayImage(&small_num_image[tmp], FROMFILE, NOFULLPAGE);
      //xtm_printf("%d\n", 2+i);
     }
    for(i=0; i<2; i++)
     {
      tmp = LocalCfg.Addr[i+5] - '0';
      small_num_image[tmp].xLeft = r2r_xLeft[1][i];
      small_num_image[tmp].yTop = r2r_yTop[1][i]+30;
      DisplayImage(&small_num_image[tmp], FROMFILE, NOFULLPAGE);
      //xtm_printf("%d\n", 6+i);
     }

	   display_text(80, 96, 0, COLOR_BLACK, talk_caption_text[LocalCfg.Language], 0, 24, NOFULLPAGE);
  display_text(128, 30+30, 0, COLOR_RED, talk_text[LocalCfg.Language][0], 0, 24, NOFULLPAGE);
  display_text(212, 30+30, 0, COLOR_RED, talk_text[LocalCfg.Language][1], 0, 24, NOFULLPAGE);
  display_text(14, 210, 0, COLOR_BLACK, talk_text[LocalCfg.Language][2], 0, 24, NOFULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, talk_text[LocalCfg.Language][3], 0, 24, NOFULLPAGE);
  display_text(190, 210, 0, COLOR_BLACK, talk_text[LocalCfg.Language][4], 0, 24, NOFULLPAGE);
  
  //添加提示文本
  display_text(14, 130, 0, COLOR_BLACK, attention_text[0], 0, 24, NOFULLPAGE);
  display_text(14, 160, 0, COLOR_BLACK, attention_text[1], 0, 24, NOFULLPAGE);
  
    //display_text(80, 96, 0, COLOR_BLACK, talk_caption_text[LocalCfg.Language], 0, 24, NOFULLPAGE); 
          break;
    case 11: //呼叫
           if((r2r_edit[0].BoxLen == 0)&&(r2r_edit[1].BoxLen == 0)&&(r2r_edit[2].BoxLen == 0))
            {
             xtm_printf("OperateTalkWindow::LocalCfg.PassOpenLock = %d\n", LocalCfg.PassOpenLock);
             if(LocalCfg.PassOpenLock == 0)
               DisplaySetupWindow(REFRESH);    //显示设置窗口
             else
               DisplayOpenLockWindow();    //显示密码开锁窗口
             break;
            }
           r2r_edit[CurrBox].Text[r2r_edit[CurrBox].BoxLen] = '\0';

           InputOK = 0;
           if((r2r_edit[CurrBox].BoxLen == 1)&&(r2r_edit[CurrBox].Text[0] == '0'))
             InputOK = 1;
           if(LocalCfg.Addr[0] == 'M')
            {
             if((r2r_edit[CurrBox].BoxLen == 3) ||(r2r_edit[CurrBox].BoxLen == 4))
               InputOK = 1;
            }
           if(LocalCfg.Addr[0] == 'W')
            {
             if((r2r_edit[0].BoxLen == 4)&&(r2r_edit[1].BoxLen == 0)&&(r2r_edit[2].BoxLen == 0))
               InputOK = 1;
             if((r2r_edit[0].BoxLen == 4)&&(r2r_edit[1].BoxLen == 2)&&((r2r_edit[2].BoxLen == 3)||(r2r_edit[2].BoxLen == 4)))
               InputOK = 1;
            }
           if(InputOK == 1)
            {
             if(r2r_edit[CurrBox].BoxLen == 3)
              {
               tmp_text[0] = '0';
               tmp_text[1] = r2r_edit[CurrBox].Text[0];
               tmp_text[2] = r2r_edit[CurrBox].Text[1];
               tmp_text[3] = r2r_edit[CurrBox].Text[2];
               tmp_text[4] = '\0';
               strcpy(r2r_edit[CurrBox].Text, tmp_text);
               r2r_edit[CurrBox].BoxLen = 4;
               DisplayEdit(&r2r_edit[CurrBox], IMAGEUP, FROMFILE, NOFULLPAGE);
               r2r_edit[CurrBox].BoxLen = strlen(r2r_edit[CurrBox].Text);

               for(i=0; i<r2r_edit[CurrBox].BoxLen; i++)
                {
                 tmp = r2r_edit[CurrBox].Text[i] - '0';
                 big_num_image[tmp].xLeft = r2r_xLeft[2][i];
                 big_num_image[tmp].yTop = r2r_yTop[2][i];
                 DisplayImage(&big_num_image[tmp], FROMFILE, NOFULLPAGE);
                }
              }

               if(LocalCfg.Addr[0] == 'M')
                {
                 if((strcmp(r2r_edit[2].Text, "0000")== 0)||(strcmp(r2r_edit[2].Text, "0")== 0))
                   Call_Func(1, r2r_edit[2].Text, "Z00010000000");    //呼叫   1  中心  2 住户
                 else
                   Call_Func(2, r2r_edit[2].Text, "");    //呼叫   1  中心  2 住户
                }
               else
                {
                 if((strcmp(r2r_edit[0].Text, "0000")== 0)||(strcmp(r2r_edit[0].Text, "0")== 0))
                   Call_Func(1, r2r_edit[0].Text, "Z00010000000");    //呼叫   1  中心  2 住户
                 else
                  {
                   r2r_edit[0].Text[4] = '\0';
                   r2r_edit[1].Text[2] = '\0';
                   r2r_edit[2].Text[4] = '\0';
                   xtm_memcopy(tmp_text, r2r_edit[0].Text, 4);
                   xtm_memcopy(tmp_text + 4, r2r_edit[1].Text, 2);
                   xtm_memcopy(tmp_text + 6, r2r_edit[2].Text, 4);
                   tmp_text[10] = '\0';
                   Call_Func(2, tmp_text, "");    //呼叫   1  中心  2 住户
                  }
                }
            }
           else
            {
              if(LocalCfg.VoiceHint == 1)
               {
                PlayInputErrorWav();
               }
            }
          break;
   }
}
//--------------------------------------------------------------------------
void ClearTalkEdit(INT8U FullPageType)  //清 文本框
{
  if(Local.CurrentWindow == 0)
   {

         DisplayImage(&talk1_image, FROMFILE, NOFULLPAGE);

         DisplayImage(&logo_image, FROMFILE, NOFULLPAGE);

         //display_text(15, 55, 0, COLOR_RED, talk_caption_text[LocalCfg.Language], 0, 24, NOFULLPAGE);
         
         display_text(14, 210, 0, COLOR_BLACK, talk_text[LocalCfg.Language][2], 0, 24, NOFULLPAGE);
         display_text(14+16, 210, 0, COLOR_BLACK, talk_text[LocalCfg.Language][3], 0, 24, NOFULLPAGE);
         display_text(190, 210, 0, COLOR_BLACK, talk_text[LocalCfg.Language][4], 0, 24, NOFULLPAGE);

   
    r2r_edit[CurrBox].BoxLen = 0;
    r2r_edit[CurrBox].Text[r2r_edit[CurrBox].BoxLen] = '\0';
    DisplayEdit(&r2r_edit[CurrBox], IMAGEUP, FROMFILE, FullPageType);
   }
  if(Local.CurrentWindow == 5)
   {
    openlock_edit.BoxLen = 0;
    openlock_edit.Text[openlock_edit.BoxLen] = '\0';
    DisplayEdit(&openlock_edit, IMAGEUP, FROMFILE, FullPageType);
   }
}
//---------------------------------------------------------------------------
void DisplayWatchWindow(INT8U RefreshType)    //显示监视窗口
{
  INT8U i;

  if(Local.LcdLightFlag == 0)
   {
      //点亮LCD背光
      Lcd_Power(1, LocalCfg.BrightVolume);
      Local.LcdLightFlag = 1; //LCD背光标志
      Local.LcdLightTime = 0; //时间
   }

  DisplayImage(&watch_image, FROMFILE, FULLPAGE);
  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);

  Local.CurrentWindow = 4;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//--------------------------------------------------------------------------
void OperateWatchWindow(INT8U wintype, INT8U currwindow)    //监视窗口操作
{
  INT8U i;

  switch(wintype)
   {
    case 10: //清除
          xtm_printf("wintype = %d, Local.Status = %d\n", wintype, Local.Status);
          if(Local.Status != 0)
           {
            if((Local.Status == 3)||(Local.Status == 4))
              WatchEnd_Func();
            PicStatBuf.Type = Local.CurrentWindow;
            PicStatBuf.Time = 0;
            PicStatBuf.Flag = 1;
           }
          break;
   }
}
//--------------------------------------------------------------------------
void ShowTalkWatchTime(INT8U cType, INT8U RefreshType) //显示监视时间   cType  0  监视   1  呼叫正在连接  2 通话
{
#if 1
  INT16U xLeft[2] = {170, 242};
  INT16U yTop[2] = {68, 68};
  INT8U i, time, tmp;
    
  switch(cType)
   {
    case 0: //监视
           if(Local.CurrentWindow != 4)
            {
             return;
            }
           if(Local.TimeOut > WATCHTIMEOUT)
            {
             return;
            }
           //xtm_printf("ShowTalkWatchTime::Local.TimeOut = 0x%X\n", Local.TimeOut);
           time = (WATCHTIMEOUT - Local.TimeOut)/TIMERPERSEC;
           break;
   /* case 1: //呼叫正在连接
           if(Local.CurrentWindow != 2)
             return;
           if(Local.TimeOut > CALLTIMEOUT)
             return;
           //xtm_printf("ShowTalkWatchTime::Local.TimeOut = 0x%X\n", Local.TimeOut);
           time = (CALLTIMEOUT - Local.TimeOut)/TIMERPERSEC;
           break;   */
    case 2: //通话
           if(Local.CurrentWindow != 3)
            {
             return;
            }
		   
           if(Local.TimeOut > TALKTIMEOUT)
            {
             return;
            }
           //xtm_printf("ShowTalkWatchTime::Local.TimeOut = 0x%X\n", Local.TimeOut);
           time = (TALKTIMEOUT - Local.TimeOut)/TIMERPERSEC;
           break;
   }
  for(i=0; i<2; i++)
   {
    if(i == 0)
      tmp = time / 10;
    else
      tmp = time % 10;
    tmp = tmp % 10;
    if(cType == 2)
     {
       if(Local.TalkStartOpenLockFlag == 1)  //通话开锁标志     20101029  xu
        {
         Local.TalkStartOpenLockTime ++;  //通话开锁计数
         if(Local.TalkStartOpenLockTime > 3)
          {
           Local.TalkStartOpenLockFlag = 0;
           DisplayImage(&talk_start_openlock[0], FROMFILE, RefreshType); //           FROMMEMORY
          }
         return;
        }
     }
    big_num_image[tmp].xLeft = xLeft[i];
    big_num_image[tmp].yTop = yTop[i];
    DisplayImage(&big_num_image[tmp], FROMFILE, RefreshType);     // FROMFILE
   }
#endif   
}
//--------------------------------------------------------------------------
void DisplayTalkConnectWindow(void)    //显示对讲 正在连接窗口
{
  DisplayImage(&talk_connect_image, FROMFILE, FULLPAGE);

  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);

  TalkConnectImageNo = 0;
  Local.CurrentWindow = 2;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//--------------------------------------------------------------------------
void OperateTalkConnectWindow(INT8U wintype, INT8U currwindow)    //对讲 正在连接窗口操作
{
  switch(wintype)
   {
    case 10: //清除
          xtm_printf("wintype = %d, Local.Status = %d\n", wintype, Local.Status);
          if(Local.Status != 0)
           {
           if((Local.Status == 1)||(Local.Status == 2)||(Local.Status == 5)||(Local.Status == 6)
             ||(Local.Status == 7)||(Local.Status == 8)||(Local.Status == 9)||(Local.Status == 10))  //状态为对讲
              TalkEnd_Func();
           }
          break;
   }
}
//--------------------------------------------------------------------------
void ShowTalkConnectImage(INT8U RefreshType) //呼叫正在连接
{
  //呼叫正在连接
  if(Local.CurrentWindow != 2)
    return;
  if(Local.TimeOut > CALLTIMEOUT)
    return;

  xtm_printf("TalkConnectImageNo = %d\n", TalkConnectImageNo);
  TalkConnectImageNo ++;
  if(TalkConnectImageNo >= 4)
    TalkConnectImageNo = 0;  
  DisplayImage(&talk_connect[TalkConnectImageNo], FROMFILE, RefreshType);
}
//--------------------------------------------------------------------------
void DisplayTalkStartWindow(void)    //显示对讲 通话窗口
{
  DisplayImage(&talk_start_image, FROMFILE, FULLPAGE);

  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);  
  Local.CurrentWindow = 3;

  Local.TalkStartOpenLockFlag = 0;  //通话开锁标志     20101029  xu
  Local.TalkStartOpenLockTime = 0;  //通话开锁计数
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//--------------------------------------------------------------------------
void OperateTalkStartWindow(INT8U wintype, INT8U currwindow)    //对讲 通话窗口操作
{
  switch(wintype)
   {
    case 10: //清除
          xtm_printf("wintype = %d, Local.Status = %d\n", wintype, Local.Status);
          if(Local.Status != 0)
           {
           if((Local.Status == 1)||(Local.Status == 2)||(Local.Status == 5)||(Local.Status == 6)
             ||(Local.Status == 7)||(Local.Status == 8)||(Local.Status == 9)||(Local.Status == 10))  //状态为对讲
              TalkEnd_Func();
           }
          break;
   }
}
//--------------------------------------------------------------------------
void DisplayOpenLockWindow(void)    //显示密码开锁窗口
{
  DisplayImage(&openlock_image, FROMFILE, FULLPAGE);

  //DisplayEdit(&openlock_edit, IMAGEUP, FROMFILE, FULLPAGE);

  display_text(28, 50, 0, COLOR_BLACK, openlock_caption_text[LocalCfg.Language], 0, 24, FULLPAGE);
  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);
  display_text(openlock2_xLeft[LocalCfg.Language], 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][2], 0, 24, FULLPAGE);
  openlock_edit.BoxLen = 0;
  openlock_edit.Text[0] = '\0';
  Local.CurrentWindow = 5;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//--------------------------------------------------------------------------
void OperateOpenLockWindow(INT8U wintype, INT8U currwindow)    //密码开锁窗口操作
{
  int i, j;
  char str[3];

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
           if(openlock_edit.BoxLen < openlock_edit.MaxLen)
            {
             str[0] = '0' + wintype;
             str[1] = '\0';
             openlock_edit.Text[openlock_edit.BoxLen] = str[0];
             str[0] = '*';
             str[1] = '\0';
             display_text(openlock_edit.xLeft + openlock_edit.CursorX + openlock_edit.BoxLen*openlock_edit.fWidth,
                     openlock_edit.yTop + openlock_edit.CursorY, openlock_edit.Space, openlock_edit.CursorCorlor, str, 0, 24, NOFULLPAGE);
             openlock_edit.BoxLen ++;
            }
           break;
    case 10: //清除
          if(openlock_edit.BoxLen != 0)
           {
            openlock_edit.Text[0] = 0;
            openlock_edit.BoxLen = 0;
            DisplayEdit(&openlock_edit, IMAGEUP, FROMFILE, NOFULLPAGE);
           }
          else
           {
            DisplayTalkWindow(REFRESH);
           }
          break;
    case 11: //开锁
           if(openlock_edit.BoxLen == 0)
            {
             DisplaySetupWindow(REFRESH);    //显示设置窗口
             break;
            }
           xtm_printf("LocalCfg.OpenLockPass = %s, Text = %s\n", LocalCfg.OpenLockPass, openlock_edit.Text);
           openlock_edit.Text[openlock_edit.BoxLen] = '\0';
           xtm_printf("LocalCfg.OpenLockPass = %s, Text = %s\n", LocalCfg.OpenLockPass, openlock_edit.Text);
           if(strcmp(openlock_edit.Text, LocalCfg.OpenLockPass) == 0)
             {
               PlayDoorOpenWav();
               DisplayOpenLockResultWindow();    //显示密码开锁成功窗口
             }
           else
            {
             PlayPassErrorWav();
             openlock_edit.Text[0] = 0;
             openlock_edit.BoxLen = 0;
             DisplayEdit(&openlock_edit, IMAGEUP, FROMFILE, NOFULLPAGE);
            }
           break;
   }
}
//--------------------------------------------------------------------------
void DisplayOpenLockResultWindow(void)    //显示密码开锁成功窗口
{
  DisplayImage(&openlock_result_image, FROMFILE, FULLPAGE);

  display_text(158, 98, 0, COLOR_BLACK, openlock_result_text[LocalCfg.Language], 0, 24, FULLPAGE);

  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);

  CheckDelayOpenLock_Func();//检查开锁延时

  Local.CurrentWindow = 7;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);

  PicStatBuf.Type = Local.CurrentWindow;
  PicStatBuf.Time = 0;
  PicStatBuf.Flag = 1;
}
//--------------------------------------------------------------------------
void OperateOpenLockResultWindow(INT8U wintype, INT8U currwindow)    //密码开锁窗口成功操作
{
  switch(wintype)
   {
    case 10: //清除
            DisplayOpenLockWindow();
            break;
   }
}
//--------------------------------------------------------------------------
void DisplayCallFailWindow(INT8U cType)    //显示呼叫失败窗口
{
  DisplayImage(&param_image, FROMFILE, FULLPAGE);

  display_text(14, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][0], 0, 24, FULLPAGE);
  display_text(14+16, 210, 0, COLOR_BLACK, openlock_text[LocalCfg.Language][1], 0, 24, FULLPAGE);

  display_text(90, 72, 0, COLOR_RED, call_fail_text[LocalCfg.Language][cType], 0, 24, FULLPAGE);

  Local.CurrentWindow = 8;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);

  PicStatBuf.Type = Local.CurrentWindow;
  PicStatBuf.Time = 0;
  PicStatBuf.Flag = 1;
}
//--------------------------------------------------------------------------
void OperateCallFailWindow(INT8U wintype, INT8U currwindow)    //呼叫失败窗口操作
{
  switch(wintype)
   {
    case 10: //清除
            DisplayTalkWindow(REFRESH);
            break;
   }
}
//--------------------------------------------------------------------------
void PlayBackRingWav(void)
{
  WaitAudioUnuse(500); //等待声卡空闲
  StopPlayWavFile();
  if(LocalCfg.RingType > 2)
    LocalCfg.RingType = 0;
  StartPlayWav(backring_wav[LocalCfg.RingType].Faddr, CYCLE_PLAY);
}
//--------------------------------------------------------------------------
void PlayDoorOpenWav(void)
{
  if(LocalCfg.VoiceHint == 1)
   {
    WaitAudioUnuse(200); //等待声卡空闲
    if(LocalCfg.Language > 1)
      LocalCfg.Language = 0;
    //if(LocalCfg.Language == 0)
     {
      if((Local.Status == 1)||(Local.Status == 2))
        return;
        
      if((Local.Status == 5)||(Local.Status == 6))
       {
        Local.TalkDoorOpenPlayFlag = 1;  //通话过程中门开了提示
        StopPlayAudio();
       }
      StartPlayWav(dooropen_wav[0].Faddr, NO_CYCLE_PLAY);
     }
   }
}
//--------------------------------------------------------------------------
