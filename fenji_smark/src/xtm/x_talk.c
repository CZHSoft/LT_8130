#include "includes.h"

#include "stdlib.h"

#define x_talk_h

#define CommonH
#include "common.h"

/*
****************************************
201310
新增加密码键盘窗体
20131127
呼叫栋数、单元号初始化
****************************************
*/


void Talk_Func(void);       //通话 接听
void CallCenter_Func(char *CenterAddr);  //呼叫中心
void CallR2R_Func(char *DestAddr);    //户户对讲
void Watch_Func(void);       //监视
void TalkEnd_Func(void);
void WatchEnd_Func(void);
void CallTimeOut_Func(void); //呼叫超时
void OpenLock_Func(void);  //开锁

void CreateTalkBuff(INT8U *sendb, INT8U Order1, INT8U Order2, INT8U Order3, INT8U *Remote_Addr, INT8U *Remote_IP);
void CreateSmartTalkBuff(INT8U *sendb, INT8U Order1, INT8U Order2, INT8U Order3, INT8U *Remote_Addr, INT8U *Remote_IP);


void CreateNSBuff(INT8U *sendb, INT8U Order1, INT8U Order2);

void DisplayTalkWindow(INT8U RefreshType);    //显示对讲呼出窗口
void PreDealTalkWindow(INT8U wintype);    //对讲呼出窗口预处理，按键按下效果
void OperateTalkWindow(INT8U wintype, INT8U currwindow);    //对讲呼出窗口操作
void CheckTouchTalkWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

//201310新增加密码键盘窗体
void DisplayEPWWindow(INT8U RefreshType);    //显示进入密码窗口
void PreDealEPWWindow(INT8U wintype);    //进入密码窗口预处理，按键按下效果
void OperateEPWWindow(INT8U wintype, INT8U currwindow);    //进入密码窗口操作
void CheckTouchEPWWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

struct TalkCall1 TalkCall;
void ClearTalkText(void);
void CheckBuildingText(INT8U tmplen); //检查幢号输入

void DisplayWatchWindow(INT8U RefreshType);    //显示监视窗口
void PreDealWatchWindow(INT8U wintype);    //监视窗口预处理，按键按下效果
void OperateWatchWindow(INT8U wintype, INT8U currwindow);    //监视窗口操作
void CheckTouchWatchWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

#ifdef ELEVATORCONTROL_HANGZHOU
 void ElevatorControl_Func(void);  //控制电梯
#endif

  char S_WatchTarget[LANGUAGEMAXNUM][6][20] = {
                                                {"单元1", "单元2", "单元3", "单元4", "二次1", "二次2"},
                                                {"Unit 1", "Unit 2", "Unit 3", "Unit 4", "Second 1", "Second 2"}
                                               };

  char B_WatchTarget[LANGUAGEMAXNUM][2][20] = {
                                                {"二次1", "二次2"},
                                                {"Second 1", "Second 2"}
                                               };
void ShowWatchTarget(INT8U FullPageType);

void DisplayTalkWatchPicWindow(INT8U ReturnType, INT8U WinType);    //显示对讲呼入窗口
void PreDealTalkWatchPicWindow(INT8U wintype);    //对讲呼入窗口预处理，按键按下效果
void OperateTalkWatchPicWindow(INT8U wintype, INT8U currwindow);    //对讲呼入窗口操作
void CheckTouchTalkWatchPicWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作
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
void CreateSmartTalkBuff(INT8U *sendb, INT8U Order1, INT8U Order2, INT8U Order3, INT8U *Remote_Addr, INT8U *Remote_IP)
{
    //头部
    xtm_memcopy(sendb, UdpPackageHead2, 4);
    //命令  ,子网广播解析
    sendb[4] = 8;
    sendb[5] = Order1;
    sendb[6] = Order2;
    //Multi_Udp_Buff[i].CurrOrder = Multi_Udp_Buff[i].buf[6];
    sendb[7] = 4;    //主叫
    sendb[8] = 0;
    sendb[9] = Order3;

    xtm_memcopy(sendb+10, LocalCfg.Addr, 20);
    xtm_memcopy(sendb+30, LocalCfg.IP, 4);
    //xtm_memcopy(sendb+34, Remote.Addr[0], 20);
    //xtm_memcopy(sendb+54, Remote.IP[0], 4);


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
  else if((Local.Status == 89)&& (Local.CurrentWindow==89))
  {
        xtm_printf("Talk_Func\n");
        
        StopPlayWavFile();  //关闭铃音
        
        CreateSmartTalkBuff(sendb,89,1,6, Remote.Addr[0], Remote.IP[0]);

        sendlength = 57;

        CreateMultiUdpBuff(LocalPort[3], MULTI_SEND, Remote.IP[0], sendb, sendlength, 0);
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
  else if(Local.Status == 89)
    {   
    
      xtm_printf("TalkEnd_Func\n");
      for(j=0; j<Remote.DenNum; j++)
       {
        CreateSmartTalkBuff(sendb, 89, 4,8, Remote.Addr[j], Remote.IP[j]);

        sendlength = 57;

        CreateMultiUdpBuff(LocalPort[3], MULTI_SEND, Remote.IP[j], sendb, sendlength, 0);
       }
    }
}
//---------------------------------------------------------------------------
void Watch_Func(void)       //监视
{
  INT8U sendb[128];
  INT16U sendlength;
  INT8U Order;

  Local.ForceEndWatch = 0;  //有呼叫时，强制关监视
  xtm_memcopy(Remote.Addr[0], NullAddr, 20);
  switch(LocalCfg.Addr[0])
   {
    case 'S':
             switch(Local.WatchTarget)
                {
                  case 0:   //单元1
                  case 1:   //单元2
                  case 2:   //单元3
                  case 3:   //单元4
                         Remote.Addr[0][0] = 'M';
                         xtm_memcopy(Remote.Addr[0] + 1, LocalCfg.Addr + 1, 6);
                         Remote.Addr[0][7] = Local.WatchTarget + '0';
                         break;
                  case 4:   //二次1
                  case 5:   //二次2
                         Remote.Addr[0][0] = 'H';
                         xtm_memcopy(Remote.Addr[0] + 1, LocalCfg.Addr + 1, 10);
                         Remote.Addr[0][11] = Local.WatchTarget - 4 + '0';
                         break;
                }
             break;
    case 'B':
             switch(Local.WatchTarget)
                {
                 case 0:  //二次1
                 case 1:  //二次2
                        Remote.Addr[0][0] = 'H';
                        xtm_memcopy(Remote.Addr[0] + 1, LocalCfg.Addr + 1, 4);
                        Remote.Addr[0][11] = Local.WatchTarget + '0';
                        break;
                 case 2:
                 case 3:
                 case 4:
                 case 5:                 
                        return;
                }
             break;
   }
  //strcpy(Remote.Addr[0], "H00010108090");
  Remote.Addr[0][12] = '\0';
  xtm_printf("Remote.Addr[0] = %s\n", Remote.Addr[0]);
  if((Local.Status == 0)&&(PicStatBuf.Flag == 0))
   {

     #if 1
      CreateNSBuff(sendb, NSORDER, ASK);

      sendlength = 56;

      CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Local.NsMultiAddr, sendb, sendlength, VIDEOWATCH);
     #else
      xtm_printf("Watch_Func::Remote.Addr[0] = %s\n", Remote.Addr[0]);
      if(QueryAddrTableFile(Remote.Addr[0], Remote.IP[0]) == 1)
       {
           Remote.DenNum = 1;
           Remote.GroupIP[0] = 236;
           Remote.GroupIP[1] = LocalCfg.IP[1];
           Remote.GroupIP[2] = LocalCfg.IP[2];
           Remote.GroupIP[3] = LocalCfg.IP[3];
           Order = Local.MachineType | ASK;
           CreateTalkBuff(sendb, VIDEOWATCH, Order, CALL, Remote.Addr[0], Remote.IP[0]);

           sendlength = 62;

           CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Remote.IP[0], sendb, sendlength, 0);
           xtm_printf("查找地址表成功,正在发起监视 %d.%d.%d.%d\n", Remote.IP[0][0], Remote.IP[0][1], Remote.IP[0][2], Remote.IP[0][3]);
       }
      else
       {
        xtm_printf("查找地址表失败\n");
        PicStatBuf.Type = Local.CurrentWindow;
        PicStatBuf.Time = 0;
        PicStatBuf.Flag = 1;
       }
     #endif
   }
  else
   {
    xtm_printf("本机正忙,无法监视\n");
   }
}
//---------------------------------------------------------------------------
void WatchEnd_Func(void)
{
  INT8U sendb[128];
  INT16U sendlength;
  if((Local.Status == 3)||(Local.Status == 4))  //状态为监视
   {
    xtm_printf("WatchEnd_Func \n");
    CreateTalkBuff(sendb, VIDEOWATCH, Local.MachineType | ASK, CALLEND, Remote.Addr[0], Remote.IP[0]);

    sendlength = 57;

    CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Remote.IP[0], sendb, sendlength, 0);
   }
}
//---------------------------------------------------------------------------
void CallCenter_Func(char *CenterAddr)  //呼叫中心
{
  INT8U sendb[128];
  INT16U sendlength;
  Local.ForceEndWatch = 0;  //有呼叫时，强制关监视
  xtm_memcopy(Remote.Addr[0], NullAddr, 20);
  xtm_memcopy(Remote.Addr[0], CenterAddr, 12);
  Remote.Addr[0][12] = '\0';
  xtm_printf("Remote.Addr[0] = %s\n", Remote.Addr[0]);
  if((Local.Status == 0)&&(PicStatBuf.Flag == 0))
   {
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
void CallR2R_Func(char *DestAddr)    //户户对讲
{
  int i;
  INT8U sendb[128];
  INT16U sendlength;
  Local.ForceEndWatch = 0;  //有呼叫时，强制关监视
  xtm_memcopy(Remote.Addr[0], NullAddr, 20);
  xtm_printf("CallR2R_Func::DestAddr = %s\n", DestAddr);
  if(strlen(DestAddr) == 4)
   { //为别墅机
    Remote.Addr[0][0] = 'B';
    xtm_memcopy(Remote.Addr[0] + 1, DestAddr, 4);
    Remote.Addr[0][5] = '0';
   }
  else
   { //为普通室内机
    Remote.Addr[0][0] = 'S';
    xtm_memcopy(Remote.Addr[0] + 1, DestAddr, 10);
    Remote.Addr[0][11] = '0';
   }
  xtm_printf("Remote.Addr[0] = %s, PicStatBuf.Flag = %d\n", Remote.Addr[0], PicStatBuf.Flag);
  if((Local.Status == 0)&&(PicStatBuf.Flag == 0))
   {
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
void OpenLock_Func(void)  //开锁
{
  INT8U sendb[128];
  INT16U sendlength;
  if((Remote.Addr[0][0] != 'M')&&(Remote.Addr[0][0] != 'W')&&(Remote.Addr[0][0] != 'H'))
    return;
  CreateTalkBuff(sendb, VIDEOTALK, Local.MachineType | ASK, REMOTEOPENLOCK, Remote.Addr[0], Remote.IP[0]);

  sendlength = 57;

  CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Remote.IP[0], sendb, sendlength, 0);
}
//--------------------------------------------------------------------------
void DisplayTalkWindow(INT8U RefreshType)    //显示对讲呼出窗口
{
  INT8U i;

  RefreshBackWindow(RefreshType, FULLPAGE); //刷新背景

  DisplayEdit(&r2r_edit, IMAGEUP, FROMFILE, FULLPAGE);

  DisplayImage(&call_image, FROMFILE, FULLPAGE);

  display_text(r2r_edit.xLeft, r2r_edit.yTop - 32, 0, COLOR_BLACK, "请输入地址,\"#\"呼叫,\"*\"取消:", 0, 24, FULLPAGE);

  strcpy(num_button[12].buttonname, "幢");
  strcpy(num_button[13].buttonname, "单元");
  strcpy(num_button[14].buttonname, "中心");
  //按钮
  for(i=12; i<15; i++)
   {
    num_button[i].FontType = 24;
    num_button[i].FontColor = COLOR_BLACK;
    num_button[i].Text_xLeft = (num_button[i].width - strlen(num_button[i].buttonname)*12)/2;
    num_button[i].Text_yTop = (num_button[i].height - 24)/2;
    //xtm_printf("%d   %d\n", num_button[i].Text_xLeft, num_button[i].Text_yTop);
   }
   
  WaitDecodeFinish();
  for(i=12; i<15; i++)
    DisplayImageButton_Text(&num_button[i], num_button[i].FontColor, 2, FULLPAGE);
  Local.CurrentWindow = 12;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);  


  
  TalkCall.BuildFlag = 1;   //幢
  TalkCall.Build[0] = '\0';
  TalkCall.BuildMaxlen = 4;
  TalkCall.UnitFlag = 0;    //单元
  TalkCall.Unit[0] = '\0';
  TalkCall.UnitMaxlen = 2;
  TalkCall.RoomFlag = 0;    //房号
  TalkCall.Room[0] = '\0';
  TalkCall.RoomMaxlen = 4;
  TalkCall.Len = 0;
  r2r_edit.BoxLen = 0;

  TalkCall.TalkText[0] = '\0';

  //20131127 未来这里将幢、单元初始化
  TalkCall.Build[0] = LocalCfg.Addr[1];
  TalkCall.Build[1] = LocalCfg.Addr[2];
  TalkCall.Build[2] = LocalCfg.Addr[3];
  TalkCall.Build[3] = LocalCfg.Addr[4];
  TalkCall.Build[4] = '\0';
  OperateTalkWindow(12,12);
  TalkCall.Unit[0] = LocalCfg.Addr[5];
  TalkCall.Unit[1] = LocalCfg.Addr[6];
  TalkCall.Unit[2] = '\0';
  OperateTalkWindow(13,12);
  //TalkCall.Room[0] = LocalCfg.Addr[7];
  //TalkCall.Room[1] = LocalCfg.Addr[8];
  //TalkCall.Room[2] = LocalCfg.Addr[9];
  //TalkCall.Room[3] = LocalCfg.Addr[10];
  //TalkCall.Room[4] = "\0";
}
//--------------------------------------------------------------------------
void PreDealTalkWindow(INT8U wintype)    //对讲呼出窗口预处理，按键按下效果
{
  if((wintype >= 0)&&(wintype <= 14))
   {
    DisplayImageButton(&num_button[wintype], IMAGEDOWN, FROMFILE, NOFULLPAGE);
   }

  if((wintype >= 11)&&(wintype <= 14))
   {
    DisplayImageButton_Text(&num_button[wintype], num_button[wintype].FontColor, 2, NOFULLPAGE);
   }

  PreDealBigmenuButton(wintype);  //快捷按键 预处理
}
//--------------------------------------------------------------------------
void OperateTalkWindow(INT8U wintype, INT8U currwindow)    //对讲呼出窗口操作
{
  INT8U i;
  INT8U InputOK;
  int tmplen;
  char tmpchar[10];  
  char str[5];
  if(wintype < 15)
    DisplayImageButton(&num_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);
  if((wintype >= 11)&&(wintype <= 14))
   {
    DisplayImageButton_Text(&num_button[wintype], num_button[wintype].FontColor, 2, NOFULLPAGE);
   }

  ShowPressQuickmenu_Button(wintype);  //快捷按键  弹起

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

         //  if(r2r_edit[CurrBox].BoxLen < r2r_edit[CurrBox].MaxLen)
            {
             if((TalkCall.BuildFlag == 0)&&(TalkCall.UnitFlag == 0)&&(TalkCall.RoomFlag == 0))
               break;

             str[0] = '0' + wintype;
             str[1] = '\0';

             if(TalkCall.BuildFlag == 1)   //幢
              {
               if(TalkCall.Len >= TalkCall.BuildMaxlen)
                 break;
               strcat(TalkCall.Build, str);
               TalkCall.Len ++;
              }
             if(TalkCall.UnitFlag == 1)   //单元
              {
               if(TalkCall.Len >= TalkCall.UnitMaxlen)
                 break;
               strcat(TalkCall.Unit, str);
               TalkCall.Len ++;
              }
             if(TalkCall.RoomFlag == 1)       //房号
              {
               if(TalkCall.Len >= TalkCall.RoomMaxlen)
                 break;
               strcat(TalkCall.Room, str);
               TalkCall.Len ++;
              }

             strcat(r2r_edit.Text, str);
             xtm_printf("str = %s, r2r_edit.BoxLen = %d\n", str, r2r_edit.BoxLen);
             display_text(r2r_edit.xLeft + r2r_edit.CursorX + r2r_edit.BoxLen*r2r_edit.fWidth,
                     r2r_edit.yTop + r2r_edit.CursorY, r2r_edit.Space, r2r_edit.CursorCorlor, str, 0, 24, NOFULLPAGE);

             r2r_edit.BoxLen ++;
            }
           break;
    case 10: //清除
          ClearTalkText();
          break;
    case 11: //呼叫
       #if 0
           if(LocalCfg.Addr[10] != '9')
            {
             strcpy(Remote.Addr[0], "S00010308020");
             strcpy(TalkCall.TalkText, "0001030802");

             DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, TALKWINTYPE);
             CallR2R_Func(TalkCall.TalkText);
             break;
            }
       #endif
          InputOK = 0;
          if(TalkCall.RoomFlag == 1)   //房号
           {
            tmplen = strlen(TalkCall.Room);
            if(tmplen == 0)
              break;           
            TalkCall.RoomFlag = 0;
            
            if(strlen(TalkCall.Room) != TalkCall.RoomMaxlen)
             {
              strcpy(tmpchar, TalkCall.Room);
              switch(tmplen)
               {
                case 1:
                       xtm_sprintf(TalkCall.Room, "000%s\0", tmpchar);
                       break;
                case 2:
                       xtm_sprintf(TalkCall.Room, "00%s\0", tmpchar);
                       break;
                case 3:
                       xtm_sprintf(TalkCall.Room, "0%s\0", tmpchar);
                       break;
               }
             }
            TalkCall.Len = 0;
            DisplayEdit(&r2r_edit, IMAGEUP, FROMFILE, NOFULLPAGE);
            WaitDecodeFinish();
            xtm_sprintf(r2r_edit.Text, "%s栋%s单元%s房\0", TalkCall.Build, TalkCall.Unit, TalkCall.Room);
            r2r_edit.BoxLen = strlen(r2r_edit.Text);

            display_text(r2r_edit.xLeft + r2r_edit.CursorX ,
                     r2r_edit.yTop + r2r_edit.CursorY, r2r_edit.Space, r2r_edit.CursorCorlor, r2r_edit.Text, 0, 24, NOFULLPAGE);
           }
          tmplen = strlen(TalkCall.Build);
          if((tmplen > 0)&&(tmplen < 4)&&(strlen(TalkCall.Unit) == 0)&&(strlen(TalkCall.Room) == 0))
            CheckBuildingText(tmplen); //检查幢号输入
          if((strlen(TalkCall.Build) == 4)&&(strlen(TalkCall.Unit) == 0)&&(strlen(TalkCall.Room) == 0))
            InputOK = 1;
          if((strlen(TalkCall.Build) == 4)&&(strlen(TalkCall.Unit) == 2)&&(strlen(TalkCall.Room) == 4))
            InputOK = 1;

           for(i=0; i<20; i++)
             TalkCall.TalkText[i] = '\0';
           xtm_sprintf(TalkCall.TalkText, "%s%s%s\0", TalkCall.Build, TalkCall.Unit, TalkCall.Room);
           xtm_printf("TalkCall.TalkText = %s, TalkCall.Build = %s, TalkCall.Unit = %s, TalkCall.Room = %s\n", TalkCall.TalkText, TalkCall.Build, TalkCall.Unit, TalkCall.Room);
           if(strlen(TalkCall.TalkText) == 4)
            { //为别墅机
             xtm_memcopy(Remote.Addr[0],"000000000000",12);
             Remote.Addr[0][0] = 'B';
             xtm_memcopy(Remote.Addr[0] + 1, TalkCall.TalkText, 4);
             Remote.Addr[0][11] = '0';
             Remote.Addr[0][12] = '\0';
            }
           else
            { //为普通室内机
             Remote.Addr[0][0] = 'S';
             xtm_memcopy(Remote.Addr[0] + 1, TalkCall.TalkText, 10);
             Remote.Addr[0][11] = '0';
             Remote.Addr[0][12] = '\0';
            }
            
          //20101030   修改为不能呼叫本机
         #if 0
          strcpy(DestAddr, Remote.Addr[0]);
          DestAddr[Local.AddrLen - 1] = '0';
          strcpy(LocalAddr, LocalCfg.Addr);
          LocalAddr[Local.AddrLen - 1] = '0';
          if(strcmp(DestAddr, LocalAddr) == 0)
           {
            P_Debug("目标地址为本机，无法呼叫\n");
            InputOK = 0;
            ClearTalkText();
            break;
           }
         #endif

          if(InputOK == 0)   //单元
            {
              if(LocalCfg.VoiceHint == 1)
               {
                PlayInputErrorWav();
               }
             break;
            }

		   //这里尝试添加密码录入窗体 20131008
		   
           //DisplayKbWindow(5);
		   //PreDealKbWindow(Local.CurrentWindow);
		   
		   //OperateSetupMainWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
		   //SendAddFindNSFunc();
           //DisplayEPWWindow(REFRESH_MIDDLE);

		   
		   DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, TALKWINTYPE);
           CallR2R_Func(TalkCall.TalkText);
           
          break;
    case 12: //幢
          //xtm_printf("TalkCall.BuildFlag = %d\n", TalkCall.BuildFlag);
          if(TalkCall.BuildFlag == 1)   //幢
           {
            tmplen = strlen(TalkCall.Build);
            if(tmplen == 0)
              break;
            TalkCall.BuildFlag = 0;
            CheckBuildingText(tmplen); //检查幢号输入

            TalkCall.UnitFlag = 1;    //单元
            TalkCall.Len = 0;
           }
          break;
    case 13: //单元
          if(TalkCall.UnitFlag == 1)   //单元
           {
            tmplen = strlen(TalkCall.Unit);
            if(tmplen == 0)
              break;
            TalkCall.UnitFlag = 0;

            if(strlen(TalkCall.Unit) != TalkCall.UnitMaxlen)
             {
              strcpy(tmpchar, TalkCall.Unit);
              switch(tmplen)
               {
                case 1:
                       TalkCall.Unit[0] = '0';
                       TalkCall.Unit[1] = tmpchar[0];
                       TalkCall.Unit[2] = '\0';
                       break;
               }
             }
            TalkCall.RoomFlag = 1;    //房号
            TalkCall.Len = 0;
            DisplayEdit(&r2r_edit, IMAGEUP, FROMFILE, NOFULLPAGE);
            WaitDecodeFinish();
            strcpy(r2r_edit.Text, TalkCall.Build);
            if(LocalCfg.Language == 2)  //英文
             {
              strcat(r2r_edit.Text, " B");
              strcat(r2r_edit.Text, TalkCall.Unit);
              strcat(r2r_edit.Text, " U");
              r2r_edit.BoxLen = 10;
             }
            else
             {
              strcat(r2r_edit.Text, "幢");
              strcat(r2r_edit.Text, TalkCall.Unit);
              strcat(r2r_edit.Text, "单元");
              r2r_edit.BoxLen = 12;
             }

           // r2r_edit.BoxLen = strlen(r2r_edit.Text);
            xtm_printf("r2r_edit.BoxLen = %d\n", r2r_edit.BoxLen);
            if(LocalCfg.Language == 2)  //英文
              Local.Text_Space = 0;
            else
              Local.Text_Space = 3;
            display_text(r2r_edit.xLeft + r2r_edit.CursorX,
                     r2r_edit.yTop + r2r_edit.CursorY, r2r_edit.Space, r2r_edit.CursorCorlor, r2r_edit.Text, 0, 24, NOFULLPAGE);
           }
          break;
    case 14: //中心
          if((Local.Status == 0)&&(Local.CurrentWindow != 16))
           {
            xtm_memcopy(Remote.Addr[0],"Z00010000000",12);
            Remote.Addr[0][12] = '\0';
            DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, TALKWINTYPE);
            CallCenter_Func("Z00010000000");
            break;
           }
          break;
    default:
          OperateQuickmenu(wintype, 102);
          break;
   }
}
//--------------------------------------------------------------------------
void CheckTouchTalkWindow(INT16U XLCD, INT16U YLCD) //检查触摸屏操作
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   for(i=0; i<15; i++)  //窗口按钮
    {
     isInTouch = CheckTSInButton(&num_button[i], XLCD, YLCD);
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
//---------------------------------------------------------------------------
void DisplayEPWWindow(INT8U RefreshType) //显示进入密码窗口
{
  INT8U i;

  RefreshBackWindow(RefreshType, FULLPAGE); //刷新背景

  DisplayEdit(&setup_pass_edit, IMAGEUP, FROMFILE, FULLPAGE);

  DisplayImage(&setup_key_image, FROMFILE, FULLPAGE);

  WaitDecodeFinish();
  display_text(setup_pass_edit.xLeft, setup_pass_edit.yTop - 32, 0, COLOR_BLACK, 
  "请输入房间密码:", 0, 24, FULLPAGE);
  setup_pass_edit.BoxLen = 0;

  strcpy(num_button[12].buttonname, "删除");
  strcpy(num_button[13].buttonname, "确认");
 #ifdef _CAMERA_SUPPORT     //如不用摄像头，GPIOB6、7为Comm1, GPIOC 作为8个防区使用   测试模式
  strcpy(num_button[14].buttonname, "测试");   // 测试
 #else
  strcpy(num_button[14].buttonname, "关于");
 #endif
  //按钮
  for(i=12; i<15; i++)
   {
    num_button[i].FontType = 24;
    num_button[i].FontColor = COLOR_BLACK;
    num_button[i].Text_xLeft = (num_button[i].width - strlen(num_button[i].buttonname)*12)/2;
    num_button[i].Text_yTop = (num_button[i].height - 24)/2;
    //xtm_printf("%d   %d\n", num_button[i].Text_xLeft, num_button[i].Text_yTop);
   }

  for(i=12; i<15; i++)
    DisplayImageButton_Text(&num_button[i], num_button[i].FontColor, 2, FULLPAGE);
  Local.CurrentWindow = 7;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//---------------------------------------------------------------------------
void PreDealEPWWindow(INT8U wintype)   //进入密码窗口预处理，按键按下效果
{
  if((wintype >= 0)&&(wintype <= 14))
   {
    DisplayImageButton(&num_button[wintype], IMAGEDOWN, FROMFILE, NOFULLPAGE);
   }

  if((wintype >= 12)&&(wintype <= 14))
   {
    DisplayImageButton_Text(&num_button[wintype], num_button[wintype].FontColor, 2, NOFULLPAGE);
   }

  PreDealBigmenuButton(wintype);  //快捷按键 预处理
}
//---------------------------------------------------------------------------
void OperateEPWWindow(INT8U wintype, INT8U currwindow)    //进入设置窗口操作
{
  char str[5];
  if(wintype < 15)
    DisplayImageButton(&num_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);
  if((wintype >= 12)&&(wintype <= 14))
   {
    DisplayImageButton_Text(&num_button[wintype], num_button[wintype].FontColor, 2, NOFULLPAGE);
   }

  ShowPressQuickmenu_Button(wintype);  //快捷按键  弹起

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
                     setup_pass_edit.yTop + setup_pass_edit.CursorY - 2, setup_pass_edit.Space, setup_pass_edit.CursorCorlor, str, 0, 24, NOFULLPAGE);

             setup_pass_edit.BoxLen ++;
            }
           break;
    case 10: //清除
    case 12:
          setup_pass_edit.Text[0] = 0;
          setup_pass_edit.BoxLen = 0;
          DisplayEdit(&setup_pass_edit, IMAGEUP, FROMFILE, NOFULLPAGE);
          break;
    case 11: //确认
    case 13: //确认
          xtm_printf("OperateEPWWindow::setup_pass_edit.BoxLen = %d, LocalCfg.EngineerPass = %s\n", setup_pass_edit.BoxLen, LocalCfg.EngineerPass);
          setup_pass_edit.Text[setup_pass_edit.BoxLen] = '\0';
          LocalCfg.EngineerPass[4] = '\0';
		  //SerialNo.pw[5]='\0';
          
//          if(strcmp(setup_pass_edit.Text, LocalCfg.EngineerPass) == 0)

          //if((strcmp(setup_pass_edit.Text, SerialNo.pw) == 0)||(strcmp(setup_pass_edit.Text, "8023") == 0))
            
          if(1)
           {
            //DisplaySetupMainWindow(REFRESH_MIDDLE);    //刷新 中
            //DisplaySplashWindow(REFRESH_MIDDLE);

		     DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, TALKWINTYPE);
             CallR2R_Func(TalkCall.TalkText);
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
         #ifdef _CAMERA_SUPPORT     //如不用摄像头，GPIOB6、7为Comm1, GPIOC 作为8个防区使用   测试模式
          //DisplayTestWindow(REFRESH);
         #else
          //DisplayVersionWindow(REFRESH_MIDDLE);
         #endif
          break;
    default:
          OperateQuickmenu(wintype, 106);
          break;
   }
}
//---------------------------------------------------------------------------
void CheckTouchEPWWindow(INT16U XLCD, INT16U YLCD) //检查触摸屏操作
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   for(i=0; i<15; i++)  //窗口按钮
    {
     isInTouch = CheckTSInButton(&num_button[i], XLCD, YLCD);
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
void ClearTalkText(void)
{
  TalkCall.BuildFlag = 1;   //幢
  TalkCall.Build[0] = '\0';
  TalkCall.UnitFlag = 0;    //单元
  TalkCall.Unit[0] = '\0';
  TalkCall.RoomFlag = 0;    //房号
  TalkCall.Room[0] = '\0';

  TalkCall.Len = 0;

  DisplayEdit(&r2r_edit, IMAGEUP, FROMFILE, NOFULLPAGE);

  r2r_edit.BoxLen = 0;
  r2r_edit.Text[r2r_edit.BoxLen] = '\0';
}
//--------------------------------------------------------------------------
void CheckBuildingText(INT8U tmplen) //检查幢号输入
{
  char tmpchar[10];
  
  if(tmplen != TalkCall.BuildMaxlen)
   {
    strcpy(tmpchar, TalkCall.Build);
    switch(tmplen)
     {
      case 1:
             TalkCall.Build[0] = '0';
             TalkCall.Build[1] = '0';
             TalkCall.Build[2] = '0';
             TalkCall.Build[3] = tmpchar[0];
             break;
      case 2:
             TalkCall.Build[0] = '0';
             TalkCall.Build[1] = '0';
             TalkCall.Build[2] = tmpchar[0];
             TalkCall.Build[3] = tmpchar[1];
             break;
      case 3:
             TalkCall.Build[0] = '0';
             TalkCall.Build[1] = tmpchar[0];
             TalkCall.Build[2] = tmpchar[1];
             TalkCall.Build[3] = tmpchar[2];
             break;
     }
   }
  TalkCall.Build[4] = '\0'; 
  DisplayEdit(&r2r_edit, IMAGEUP, FROMFILE, NOFULLPAGE);
  WaitDecodeFinish();
  strcpy(r2r_edit.Text, TalkCall.Build);
  if(LocalCfg.Language == 2)  //英文
    strcat(r2r_edit.Text, " B");
  else
    strcat(r2r_edit.Text, "幢");

  r2r_edit.BoxLen = 6;
  //r2r_edit.BoxLen = strlen(r2r_edit.Text);
  if(LocalCfg.Language == 2)  //英文
    Local.Text_Space = 0;
  else
    Local.Text_Space = 3;
  display_text(r2r_edit.xLeft + r2r_edit.CursorX,
               r2r_edit.yTop + r2r_edit.CursorY, r2r_edit.Space, r2r_edit.CursorCorlor, r2r_edit.Text, 0, 24, NOFULLPAGE);
}
//--------------------------------------------------------------------------
void DisplayWatchWindow(INT8U RefreshType)    //显示监视窗口
{
  INT8U i;

  RefreshBackWindow(RefreshType, FULLPAGE); //刷新背景

  for(i=0; i<3; i++)
    DisplayImageButton(&watch_button[i], IMAGEUP, FROMFILE, FULLPAGE);

  Local.WatchTarget = 0;  
  ShowWatchTarget(FULLPAGE);
  Local.CurrentWindow = 13;

  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);  
}
//--------------------------------------------------------------------------
void PreDealWatchWindow(INT8U wintype)    //监视窗口预处理，按键按下效果
{
  xtm_printf("PreDealWatchWindow:: wintype = %d\n", wintype);
  if((wintype >= 0)&&(wintype <= 2))
   {
    WaitDecodeFinish();
    DisplayImageButton(&watch_button[wintype], IMAGEDOWN, FROMFILE, NOFULLPAGE);
   }

  PreDealBigmenuButton(wintype);  //快捷按键 预处理
}
//--------------------------------------------------------------------------
void ShowWatchTarget(INT8U FullPageType)
{
  INT16U xLeft, yTop;
  char WatchTarget_Text[30];

  //xtm_printf("ShowWatchTarget 1\n");
  DisplayImage(&watchtarget_image, FROMFILE, FullPageType);
  //xtm_printf("ShowWatchTarget 2\n");
  switch(LocalCfg.Addr[0])
   {
    case 'S':
             Local.WatchTargetMaxNum = 6; //监视目标最大数量
             strcpy(WatchTarget_Text, S_WatchTarget[Local.LanguageNo][Local.WatchTarget]);
             break;
    case 'B':
             Local.WatchTargetMaxNum = 2; //监视目标最大数量
             strcpy(WatchTarget_Text, B_WatchTarget[Local.LanguageNo][Local.WatchTarget]);
             break;
   }
  xtm_printf("ShowWatchTarget::WatchTarget_Text = %s, Local.WatchTarget = %d, Local.LanguageNo = %d\n", WatchTarget_Text, Local.WatchTarget, Local.LanguageNo);
  xLeft = watchtarget_image.xLeft + (watchtarget_image.width - strlen(WatchTarget_Text)*12)/2;;
  yTop = watchtarget_image.yTop + (watchtarget_image.height - 24)/2;
  display_text(xLeft,
                     yTop, 2, COLOR_WHITE, WatchTarget_Text, 0, 24, FullPageType);
}
//---------------------------------------------------------------------------
void OperateWatchWindow(INT8U wintype, INT8U currwindow)    //监视窗口操作
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
            DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, WATCHWINTYPE);
            Watch_Func();
           }
          break;
    case 3:
          xtm_printf("OperateWatchWindow Local.Status = %d\n", Local.Status);
          if(Local.Status == 0)
           {
            DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, WATCHWINTYPE);
            Watch_Func();
           }
          break;
    default:
          OperateQuickmenu(wintype, 103);
          break;
   }
}
//--------------------------------------------------------------------------
void CheckTouchWatchWindow(INT16U XLCD, INT16U YLCD) //检查触摸屏操作
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   for(i=0; i<3; i++)  //窗口按钮
    {
     isInTouch = CheckTSInButton(&watch_button[i], XLCD, YLCD);
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
//ReturnType 0 --  主界面   1 -- Talk Watch 界面   WinType  0 -- Talk窗口  1 -- Watch窗口
void DisplayTalkWatchPicWindow(INT8U ReturnType, INT8U WinType)    //显示对讲呼入窗口
{
  char CallText[15];
  xtm_printf("DisplayTalkWatchPicWindow\n");
  CheckKeyPressAndLcd(); //检查按键并点亮LCD
  DisplayImage(&test_image1, FROMFILE, FULLPAGE);
  //DisplayVideoWindow(0, 640, 0, 480, REMOTE_VIDEO);
  if(WinType == TALKWINTYPE)
    DisplayImage(&talkpic_image, FROMFILE, FULLPAGE);
  else
    DisplayImage(&watchpic_image, FROMFILE, FULLPAGE);
  Local.TalkWatchWinType = WinType;
  Local.CurrentWindow = 16;

  //CallText[0]=RecordsContent[0].RemoteAddr[1];
  //CallText[1]=RecordsContent[0].RemoteAddr[2];
  //CallText[2]=RecordsContent[0].RemoteAddr[3];
  //CallText[3]=RecordsContent[0].RemoteAddr[4];
  
  //CallText[4]=RecordsContent[0].RemoteAddr[5];
  //CallText[5]=RecordsContent[0].RemoteAddr[6];
  
  //CallText[6]=RecordsContent[0].RemoteAddr[7];
  //CallText[7]=RecordsContent[0].RemoteAddr[8];
  //CallText[8]=RecordsContent[0].RemoteAddr[9];
  //CallText[9]=RecordsContent[0].RemoteAddr[10];
  
  //CallText[10]='0';
  //CallText[11]='\0';
  
  //display_text(700, 450, 0, COLOR_RED, CallText, 0, 20, FULLPAGE);
  
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//--------------------------------------------------------------------------
void PreDealTalkWatchPicWindow(INT8U wintype)    //对讲呼入窗口预处理，按键按下效果
{
   if(Local.TalkWatchWinType == TALKWINTYPE)
    {
     if((wintype >= 0)&&(wintype <= 2))
      {
       //WaitDecodeFinish();
       DisplayImageButton(&talkpic_button[wintype], IMAGEDOWN, FROMFILE, NOFULLPAGE);
      }
    }
   else
     if(wintype < 1)
      {
       //WaitDecodeFinish();
       DisplayImageButton(&watchpic_button[wintype], IMAGEDOWN, FROMFILE, NOFULLPAGE);
      }
}
//--------------------------------------------------------------------------
void OperateTalkWatchPicWindow(INT8U wintype, INT8U currwindow)    //对讲呼入窗口操作
{
   xtm_printf("Local.Status = %d, Local.TalkWatchWinType = %d 11111111111\n", Local.Status, Local.TalkWatchWinType);
   if(Local.TalkWatchWinType == TALKWINTYPE)
    {
     if((wintype >= 0)&&(wintype <= 2))
      {
       //WaitDecodeFinish();
       DisplayImageButton(&talkpic_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);
      }
    }
   else
     if(wintype < 1)
      {
       //WaitDecodeFinish();
       DisplayImageButton(&watchpic_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);
      }
  switch(wintype)
   {
    case 0:
           xtm_printf("Local.Status = %d, Local.TalkWatchWinType = %d\n", Local.Status, Local.TalkWatchWinType);
           switch(Local.TalkWatchWinType)
            {
             case WATCHWINTYPE: //挂断
                      if(Local.Status == 3)  //状态为监视
                        WatchEnd_Func();
                      break;
             case TALKWINTYPE: //接听
                      if(Local.Status == 2)  //状态为被对讲
                        Talk_Func();
                      break;
            }
           break;
    case 1:  //开锁
            if((Local.Status == 2)||(Local.Status == 6))  //状态为被对讲 或被叫通话
            //if(Local.Status == 6)  //状态为被对讲
            {
             #ifdef ELEVATORCONTROL_HANGZHOU
              if(Remote.Addr[0][0] == 'S')
               {
                 xtm_memcopy(Local.Elevator_Addr, Remote.Addr[0], 20);
                 xtm_memcopy(Local.Elevator_IP, Remote.IP[0], 4);               
                 ElevatorControl_Func();  //控制电梯
                 break;
               }  
             #endif
             OpenLock_Func();
            }
           break;
    case 2:  //挂断
           switch(Local.TalkWatchWinType)
            {
             case TALKWINTYPE: //挂断

                        if((Local.Status == 1)||(Local.Status == 2)||(Local.Status == 5)||(Local.Status == 6)
                          ||(Local.Status == 7)||(Local.Status == 8)||(Local.Status == 9)||(Local.Status == 10))
                          TalkEnd_Func();
                      break;
            }
           break;
    default:
          OperateQuickmenu(wintype, 102);
          break;
   }
}
//--------------------------------------------------------------------------
void CheckTouchTalkWatchPicWindow(INT16U XLCD, INT16U YLCD) //检查触摸屏操作
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   for(i=0; i<3; i++)  //窗口按钮
    {
     isInTouch = CheckTSInButton(&talkpic_button[i], XLCD, YLCD);
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
#ifdef ELEVATORCONTROL_HANGZHOU
void ElevatorControl_Func(void)  //控制电梯
{
  INT8U i;
  INT8U sendb[128];
  INT16U sendlength;
  char Door_Addr[21];
  INT8U Door_IP[4];
  xtm_memcopy(Door_Addr, NullAddr, 20);
  Door_Addr[0] = 'M';
  xtm_memcopy(Door_Addr + 1, LocalCfg.Addr + 1, 6);
  Door_Addr[7] = '0';

  if(Local.Elevator_Addr[0] != 'S')
    return;
  //同一单元内才可控制电梯  
  if((LocalCfg.Addr[1] == Local.Elevator_Addr[1])&&(LocalCfg.Addr[2] == Local.Elevator_Addr[2])&&(LocalCfg.Addr[3] == Local.Elevator_Addr[3])&&(LocalCfg.Addr[4] == Local.Elevator_Addr[4])&&(LocalCfg.Addr[5] == Local.Elevator_Addr[5])&&(LocalCfg.Addr[6] == Local.Elevator_Addr[6]))
   {
      //头部
      xtm_memcopy(sendb, UdpPackageHead, 6);
      //命令  ,子网广播解析
      sendb[6] = NSORDER;
      sendb[7] = ASK;    //主叫

      //本机为主叫方
      xtm_memcopy(sendb+8, LocalCfg.Addr, 20);
      xtm_memcopy(sendb+28, LocalCfg.IP, 4);
      xtm_memcopy(sendb+32, Door_Addr, 20);
      Door_IP[0] = 0;
      Door_IP[1] = 0;
      Door_IP[2] = 0;
      Door_IP[3] = 0;
      xtm_memcopy(sendb+52, Door_IP, 4);

      sendlength = 56;

      CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Local.NsMultiAddr, sendb, sendlength, ELEVATR_ORDER1);
    }
  else
    xtm_printf("不同单元不可控制电梯\n");
}
#endif
//--------------------------------------------------------------------------
