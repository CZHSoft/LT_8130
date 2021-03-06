#include "includes.h"

#include "stdlib.h"

#define x_record_h

#define CommonH
#include "common.h"

/*
****************************************
20131119
暂无修改
20131127
计划249处添加按钮
呼叫ICON 20131129 
****************************************
*/

void DisplayRecordsWindow(INT8U RefreshType, INT8U Curr_Sec);    //显示通话记录窗口
void PreDealRecordsWindow(INT8U wintype);    //通话记录窗口预处理，按键按下效果
void OperateRecordsWindow(INT8U wintype, INT8U currwindow);    //通话记录窗口操作
void CheckTouchRecordsWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

void ShowRecordsList(INT8U refreshflag, INT8U FullPageType);    //通话记录列表
void CheckRecordsRemoteAddr(char *RemoteAddr, char *tmp_con);

INT8U Records_CurrBox;
void DisplayRecordsContentWindow(INT8U recordsno);    //通话记录查看信息窗口
void PreDealRecordsContentWindow(INT8U wintype);    //查看通话记录窗口预处理，按键按下效果
void OperateRecordsContentWindow(INT8U wintype, INT8U currwindow);    //查看通话记录窗口操作
void CheckTouchRecordsContentWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

void ShowRecordsContent(INT8U recordsno, INT8U FullPageType);    // 通话记录内容

void Recv_NewRecords(void);

INT8U Init_RecordsVar(void); //初始化 Records 结构
INT8U Uninit_RecordsVar(void); //释放 Records 结构
//读通话记录文件
void ReadRecordsFile(void);
//写通话记录文件
void WriteRecordsFile(void);

void ResetRecordsCfg(void);    //复位通话记录配置文件
void WriteRecordsCfgFile(void);     //写通话记录设置文件
//写照片函数
void WritePicFunc(void);
//---------------------------------------------------------------------------
void DisplayRecordsWindow(INT8U RefreshType, INT8U Curr_Sec)    //显示通话记录窗口
{
  //RefreshBackWindow(RefreshType, FULLPAGE); //刷新背景

  //DisplayImage(&records_image, FROMFILE, FULLPAGE);

  Records.CurrNo = Curr_Sec;            //当前通话记录序号  0---n-1
  Records.CurrentInfoPage = Records.CurrNo /RECORDNUMPERPAGE + 1;     //当前通话记录页

  ShowRecordsList(REFRESH, FULLPAGE);

  Local.CurrentWindow = 21;
  Local.NewRecPic = 0;

  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//--------------------------------------------------------------------------
void PreDealRecordsWindow(INT8U wintype)    //通话记录窗口预处理，按键按下效果
{
  if((wintype >= 0)&&(wintype <= 2))
   {
    DisplayImageButton(&records_button[wintype], IMAGEDOWN, FROMFILE, NOFULLPAGE);
   }

  PreDealBigmenuButton(wintype);  //快捷按键 预处理
}
//--------------------------------------------------------------------------
void OperateRecordsWindow(INT8U wintype, INT8U currwindow)    //通话记录窗口操作
{
  INT8U i;
  INT8U CurrNo;
  INT16U tmp;
  char CallText[20];
  if(wintype < 3)
    DisplayImageButton(&records_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);

  ShowPressQuickmenu_Button(wintype);  //快捷按键  弹起

  switch(wintype)
   {
    case 0://上翻       a
             if(Records.CurrentInfoPage > 1)
              {
               Records.CurrentInfoPage --;
               xtm_printf("OperateRecordsWindow1:: Records.CurrNo = %d, Records.CurrentInfoPage = %d\n",  Records.CurrNo, Records.CurrentInfoPage);
               tmp = (INT16U)RECORDNUMPERPAGE * (Records.CurrentInfoPage - 1);
               Records.CurrNo = tmp;
               xtm_printf("OperateRecordsWindow2:: Records.CurrNo = %d, Records.CurrentInfoPage = %d\n",  Records.CurrNo, Records.CurrentInfoPage);
               ShowRecordsList(REFRESH, NOFULLPAGE);
              }
           break;
    case 1://下翻     b
             if(Records.CurrentInfoPage < Records.TotalInfoPage)
              {
               Records.CurrentInfoPage ++;
               xtm_printf("OperateRecordsWindow3:: Records.CurrNo = %d, Records.CurrentInfoPage = %d\n",  Records.CurrNo, Records.CurrentInfoPage);
               tmp = (INT16U)RECORDNUMPERPAGE * (Records.CurrentInfoPage - 1);
               Records.CurrNo = tmp;
               xtm_printf("OperateRecordsWindo4:: Records.CurrNo = %d, Records.CurrentInfoPage = %d\n",  Records.CurrNo, Records.CurrentInfoPage);
               ShowRecordsList(REFRESH, NOFULLPAGE);
              }
           break;
    case 2://删除           c
               if(Records.CurrNo < Records.TotalNum)
                {
                 //顺序前移
                 for(i = Records.CurrNo; i < (Records.TotalNum - 1); i ++)
                  {
                   xtm_memcopy(&RecordsContent[i], &RecordsContent[i+1], sizeof(struct InfoContent1));
                   RecordsBuff[i] = RecordsBuff[i+1];
                  }
                 Records.TotalNum --;
                 xtm_printf("Records.CurrNo = %d, Records.TotalNum = %d\n", Records.CurrNo, Records.TotalNum);
                 if(Records.CurrNo > (Records.TotalNum - 1))
                   Records.CurrNo = Records.TotalNum - 1;
                 ShowRecordsList(REFRESH, NOFULLPAGE);
                 WriteRecordsCfgFile();     //写通话记录设置文件
                 WriteRecordsFile();       //写通话记录文件
                }
             break;
    case 40://通话记录1行           c
    case 41://通话记录2行           c
    case 42://通话记录3行           c
    case 43://通话记录4行           c
    case 44://通话记录5行           c
           //当前信息序号
             CurrNo = (Records.CurrentInfoPage-1)*RECORDNUMPERPAGE+(wintype - 40);
             xtm_printf("CurrNo = %d, Records.TotalNum = %d \n", CurrNo,
                    Records.TotalNum);
             if(Records.CurrNo == CurrNo)
              {
               if(CurrNo < (Records.TotalNum))
                if(RecordsContent[CurrNo].HavePic == 1)
                 {
                  Records.CurrNo = CurrNo;
                  DisplayRecordsContentWindow(Records.CurrNo);
                 }
              }
             else
              {
               if(CurrNo < (Records.TotalNum))
                {
                 Records.CurrNo = CurrNo;
                 ShowRecordsList(REFRESH, NOFULLPAGE);
                }
              }
           break;
	case 80://通话记录1行           c
    case 81://通话记录2行           c
    case 82://通话记录3行           c
    case 83://通话记录4行           c
    case 84://通话记录5行           c
           //当前信息序号
             CurrNo = (Records.CurrentInfoPage-1)*RECORDNUMPERPAGE+(wintype - 80);
             xtm_printf("CurrNo = %d, Records.TotalNum = %d \n", CurrNo,
                    Records.TotalNum);
             if(Records.CurrNo == CurrNo)
              {
               if(CurrNo < (Records.TotalNum))
                 if(RecordsContent[CurrNo].RemoteAddr[0]=='S')
                 {
					CallText[0]=RecordsContent[CurrNo].RemoteAddr[1];
					CallText[1]=RecordsContent[CurrNo].RemoteAddr[2];
					CallText[2]=RecordsContent[CurrNo].RemoteAddr[3];
					CallText[3]=RecordsContent[CurrNo].RemoteAddr[4];
					
					CallText[4]=RecordsContent[CurrNo].RemoteAddr[5];
					CallText[5]=RecordsContent[CurrNo].RemoteAddr[6];
					
					CallText[6]=RecordsContent[CurrNo].RemoteAddr[7];
					CallText[7]=RecordsContent[CurrNo].RemoteAddr[8];
					CallText[8]=RecordsContent[CurrNo].RemoteAddr[9];
					CallText[9]=RecordsContent[CurrNo].RemoteAddr[10];
					
					CallText[10]='0';
					CallText[11]='\0';
					
                    //这里呼叫
                    DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, TALKWINTYPE);
                    CallR2R_Func(CallText);
					
                 }
              }
           break;
    default:
          OperateQuickmenu(wintype, 105);
          break;
   }
}
//--------------------------------------------------------------------------
void CheckTouchRecordsWindow(INT16U XLCD, INT16U YLCD) //检查触摸屏操作
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   for(i=0; i<3; i++)  //窗口按钮
    {
     isInTouch = CheckTSInButton(&records_button[i], XLCD, YLCD);
     if(isInTouch == 1)
      {
       TouchNum = i;
       break;
      }
    }

  if(isInTouch == 0)
   for(i=0; i<RECORDNUMPERPAGE; i++)  //窗口按钮
    {
     isInTouch = CheckTSInButton(&recordsrow_button[i], XLCD, YLCD);
     if(isInTouch == 1)
      {
       TouchNum = i + 40;
       break;
      }
    }

  //20131129
  if(isInTouch == 0)
   for(i=0; i<RECORDNUMPERPAGE; i++)  //窗口按钮
    {
      
     isInTouch = CheckTSInButton(&records_call_icon[i], XLCD, YLCD);
     if(isInTouch == 1)
      {
      
       TouchNum = i + 80;
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
void ShowRecordsList(INT8U refreshflag, INT8U FullPageType)    //通话记录显示方框 通话记录列表
{
  INT8U i, j;
  INT8U PageTotalNum;  //当页信息数量
  INT8U CurrNo;
  char tmp_con[80];
  INT16U tmp_len;  //预览长度
  INT8U NoInPage;
  INT16U ascnum, tmp;
  INT8U DeltaHei;
  INT16U FontColor;

  RefreshBackWindow(REFRESH_MIDDLE, FullPageType); //刷新背景
  DisplayImage(&records_image, FROMFILE, FullPageType);

   
  tmp_len = 10;

           if(Records.TotalNum > 0)
            {
             //总页数
             if((Records.TotalNum % INFONUMPERPAGE) == 0)
               tmp = Records.TotalNum /INFONUMPERPAGE;
             else
               tmp = Records.TotalNum /INFONUMPERPAGE + 1;
             Records.TotalInfoPage = tmp;

             //当前页
             tmp = Records.CurrNo / INFONUMPERPAGE + 1;
             Records.CurrentInfoPage = tmp;
             if(Records.CurrentInfoPage < Records.TotalInfoPage)
               PageTotalNum = INFONUMPERPAGE;
             else
               PageTotalNum = Records.TotalNum - (Records.CurrentInfoPage - 1)*RECORDNUMPERPAGE;
             //当前信息在本页中的位置
             NoInPage = (Records.CurrNo)%RECORDNUMPERPAGE;

             xtm_printf("ShowRecordsList::Records.CurrNo = %d, Records.TotalNum = %d, Records.CurrentInfoPage = %d\n", Records.CurrNo, Records.TotalNum, Records.CurrentInfoPage);
             for(i = 0; i < RECORDNUMPERPAGE; i++)
              {
               if(i == NoInPage)
                 DisplayImageButton(&recordsrow_button[i], IMAGEDOWN, FROMFILE, FullPageType);
               else
                 DisplayImageButton(&recordsrow_button[i], IMAGEUP, FROMFILE, FullPageType);
              }
             WaitDecodeFinish();

             for(i = 0; i < PageTotalNum; i++)
              {
               if(i == NoInPage)
                 FontColor = COLOR_WHITE;
               else
                 FontColor = COLOR_BLACK;
                 
               //xtm_printf("FontColor = 0x%X\n", FontColor);
               CurrNo = (Records.CurrentInfoPage - 1)*RECORDNUMPERPAGE + i;

               DeltaHei = (recordsrow_button[i].height - 24)/2;
               //序号
               xtm_sprintf(tmp_con, "%02d\0", (Records.CurrentInfoPage-1)*RECORDNUMPERPAGE+i+1);
               tmp_con[2] = '\0';
               //xtm_printf("tmp_con = %s\n", tmp_con);
               //display_text(recordsrow_button[i].xLeft+10, recordsrow_button[i].yTop + DeltaHei, 0, FontColor, tmp_con, 0, 24, FullPageType);
               display_text(recordsrow_button[i].xLeft+2, recordsrow_button[i].yTop + DeltaHei, 0, FontColor, tmp_con, 0, 24, FullPageType);
               switch(RecordsContent[CurrNo].Type)
                {
                 case 0:
                        strcpy(tmp_con,  "主叫");
                        break;
                 case 1:
                        strcpy(tmp_con,  "未接听");
                        break;
                 case 2:
                        strcpy(tmp_con,  "已接听");
                        break;
                 default:
                        strcpy(tmp_con,  "未接听");
                        break;
                }

               //display_text(recordsrow_button[i].xLeft+100, recordsrow_button[i].yTop+DeltaHei, 0, FontColor, tmp_con, 0, 24, FullPageType);
               //CheckRecordsRemoteAddr(RecordsContent[CurrNo].RemoteAddr);
               //display_text(recordsrow_button[i].xLeft+280, recordsrow_button[i].yTop+DeltaHei, 0, FontColor, RecordsContent[CurrNo].RemoteAddr, 0, 24, FullPageType);


               display_text(recordsrow_button[i].xLeft+50, recordsrow_button[i].yTop+DeltaHei, 0, FontColor, tmp_con, 0, 24, FullPageType);
               CheckRecordsRemoteAddr(RecordsContent[CurrNo].RemoteAddr, tmp_con);
               display_text(recordsrow_button[i].xLeft+138, recordsrow_button[i].yTop+DeltaHei, 0, FontColor, tmp_con, 0, 24, FullPageType);

               //有无照片Icon
               if(RecordsContent[CurrNo].HavePic == 1)
                {
                  DisplayImage(&records_pic_icon[i], FROMFILE, FullPageType);
                }

			   //呼叫ICON 20131129 
               if(RecordsContent[CurrNo].RemoteAddr[0]=='S')
               	{
				   DisplayImageButton(&records_call_icon[i], IMAGEDOWN, FROMFILE, FullPageType);
               	}
               
              }
            }
           else
             for(i = 0; i < RECORDNUMPERPAGE; i++)
               DisplayImageButton(&recordsrow_button[i], IMAGEUP, FROMFILE, FullPageType);
}
//--------------------------------------------------------------------------
void CheckRecordsRemoteAddr(char *RemoteAddr, char *tmp_con)
{
  char Build[5];
  char Unit[3];
  char Storey[3];
  char Room[3];
  char Equiq[2];
  INT8U i;
  for(i=0; i<40; i++)
    tmp_con[i] = '\0';
  switch(RemoteAddr[0])
   {
    case 'M':
            xtm_memcopy(Build, RemoteAddr + 1, 4);
            Build[4] = '\0';
            xtm_memcopy(Unit, RemoteAddr + 5, 2);
            Unit[2] = '\0';
            xtm_memcopy(Equiq, RemoteAddr + 7, 1);
            Equiq[1] = '\0';
            xtm_sprintf(tmp_con, "门口 %s幢%s单元 %s", Build, Unit, Equiq);
            RemoteAddr[8] = '\0';
            break;
    case 'W':
            xtm_memcopy(Build, RemoteAddr + 1, 4);
            Build[4] = '\0';
            xtm_sprintf(tmp_con, "围墙 %s", Build);
            RemoteAddr[5] = '\0';
            break;
    case 'B':
            xtm_memcopy(Build, RemoteAddr + 1, 4);
            Build[4] = '\0';

            xtm_memcopy(Equiq, RemoteAddr + 7, 1);
            Equiq[1] = '\0';
            xtm_sprintf(tmp_con, "别墅 %s幢 %s", Build, Equiq);

            RemoteAddr[5] = RemoteAddr[11];
            RemoteAddr[6] = '\0';
            break;
    case 'S':
		    
            xtm_memcopy(Build, RemoteAddr + 1, 4);
            Build[4] = '\0';
            xtm_memcopy(Unit, RemoteAddr + 5, 2);
            Unit[2] = '\0';
            xtm_memcopy(Storey, RemoteAddr + 7, 2);
            Storey[2] = '\0';
            xtm_memcopy(Room, RemoteAddr + 9, 2);
            Room[2] = '\0';
            xtm_memcopy(Equiq, RemoteAddr + 11, 1);
            Equiq[1] = '\0';
            xtm_sprintf(tmp_con, "%s幢%s单元%s层%s房 设备%s", Build, Unit, Storey, Room, Equiq);
            RemoteAddr[12] = '\0';
            break;
    case 'H':
            xtm_memcopy(Equiq, RemoteAddr + 11, 1);
            Equiq[1] = '\0';
            xtm_sprintf(tmp_con, "二次门口 %s", Equiq);            
            if((RemoteAddr[5] == '0')&&(RemoteAddr[6] == '0')&&(RemoteAddr[7] == '0')&&(RemoteAddr[8] == '0')&&(RemoteAddr[9] == '0')&&(RemoteAddr[10] == '0'))
             {
              RemoteAddr[5] = RemoteAddr[11];
              RemoteAddr[6] = '\0';
             }
            else
              RemoteAddr[12] = '\0';

            break;
   }
}
//--------------------------------------------------------------------------
void DisplayRecordsContentWindow(INT8U recordsno)    //显示查看通话记录窗口
{
  DisplayImage(&picwin_image, FROMFILE, FULLPAGE);

  Records_CurrBox = recordsno;
  ShowRecordsContent(recordsno, FULLPAGE);

  Local.CurrentWindow = 22;

  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//--------------------------------------------------------------------------
void PreDealRecordsContentWindow(INT8U wintype)    //查看通话记录窗口预处理，按键按下效果
{
  if((wintype >= 0)&&(wintype <= 2))
   {
    DisplayImageButton(&picwin_button[wintype], IMAGEDOWN, FROMFILE, NOFULLPAGE);
   }

  PreDealBigmenuButton(wintype);  //快捷按键 预处理
}
//--------------------------------------------------------------------------
void OperateRecordsContentWindow(INT8U wintype, INT8U currwindow)    //查看通话记录窗口操作
{
  INT8 i;
  if(wintype < 3)
    DisplayImageButton(&picwin_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);

  ShowPressQuickmenu_Button(wintype);  //快捷按键  弹起

  switch(wintype)
   {
    case 0://上翻       a
             for(i=Records_CurrBox; i > 0; i--)
              if(Records_CurrBox > 0)
               {
                Records_CurrBox --;
                if(RecordsContent[Records_CurrBox].HavePic == 1)
                 {
                  ShowRecordsContent(Records_CurrBox, NOFULLPAGE);
                  break;
                 }
               }
             break;
    case 1://下翻     b
             for(i=Records_CurrBox; i < (Records.TotalNum -1); i++)
              if(Records_CurrBox < (Records.TotalNum -1))
               {
                Records_CurrBox ++;
                if(RecordsContent[Records_CurrBox].HavePic == 1)
                 {
                  ShowRecordsContent(Records_CurrBox, NOFULLPAGE);
                  break;
                 }
               }
             break;
    case 2://返回
             DisplayRecordsWindow(REFRESH_TOP | REFRESH_MIDDLE | REFRESH_BOTTOM, Records_CurrBox);    //刷新 中
             break;
    default:
             OperateQuickmenu(wintype, 105);
             break;
   }
}
//--------------------------------------------------------------------------
void CheckTouchRecordsContentWindow(INT16U XLCD, INT16U YLCD) //检查触摸屏操作
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   for(i=0; i<3; i++)  //窗口按钮
    {
     isInTouch = CheckTSInButton(&picwin_button[i], XLCD, YLCD);
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
void ShowRecordsContent(INT8U recordsno, INT8U FullPageType)    // 通话记录内容
{
  int xLeft,yTop;
  struct TImage tmp_image;
  char tmp_con[80];

  xLeft = 16;
  yTop = 10;
  if(recordsno <= (Records.TotalNum - 1))
   {
    tmp_image.Jpegaddr = RecordsBuff[recordsno];
    tmp_image.Maddr = ((INT32U)jpeg_bitstream&0xffff)+(((INT32U)jpeg_bitstream&0xffff0000)>>12)|0x700000;
    switch(RecordsContent[recordsno].HavePic)  //有无图像标志  0   1 -- 640x480  2 -- 320x240
     {
      case 1:
             tmp_image.width = 640;
             tmp_image.height = 480;
             tmp_image.xLeft = 0;
             tmp_image.yTop = 0;
             break;
      case 2:
             tmp_image.width = 320;
             tmp_image.height = 240;
             tmp_image.xLeft = 160;
             tmp_image.yTop = 120;
             break;
     }
    tmp_image.Len = RecordsContent[recordsno].Length;
    if((tmp_image.Len % 256) == 0)
      tmp_image.FLen = tmp_image.Len;
    else
      tmp_image.FLen = (tmp_image.Len / 256)*256 + 256;

    DisplayImage(&tmp_image, FROMMEMORY, FullPageType);

    //序号
    xtm_sprintf(tmp_con, "%02d\0", recordsno+1);
    tmp_con[2] = '\0';
    display_text(xLeft,yTop, 0, COLOR_BLACK, tmp_con, 0, 24, FullPageType);

    //display_text(xLeft + 50, yTop, 0, COLOR_BLACK, RecordsContent[recordsno].RemoteAddr, 0, 24, FullPageType);
    CheckRecordsRemoteAddr(RecordsContent[recordsno].RemoteAddr, tmp_con);
    display_text(xLeft + 50, yTop, 0, COLOR_BLACK, tmp_con, 0, 24, FullPageType);
   }
}
//--------------------------------------------------------------------------
INT8U Init_RecordsVar(void) //初始化 Records 结构
{
  INT8U i;
  //初始化通话记录结构
  Records.MaxNum = RECORDMAXITEM;

  //读通话记录文件
  ReadRecordsFile();

  Records.MaxNum = RECORDMAXITEM;   //最大通话记录数
  xtm_printf("Records.TotalNum = %d, Records.NoReadedNum = %d, Records.MaxNum = %d\n", Records.TotalNum, Records.NoReadedNum, Records.MaxNum);
  Records.CurrentInfoPage = 1;

  return 0;
}
//--------------------------------------------------------------------------
INT8U Uninit_RecordsVar(void) //释放 Info 结构
{
}
//--------------------------------------------------------------------------
//读通话记录文件
void ReadRecordsFile(void)
{
  INT16U i;
  ReadFromFlash(RECORDCFG_ADDR, Flash_Stream, 0x200);
  xtm_memcopy(&Records, Flash_Stream, sizeof(struct Info1));
  if((Records.FlashHead[0] == 'R')&&(Records.FlashHead[1] == 'E')&&(Records.FlashHead[2] == 'C')&&(Records.FlashHead[3] == 'O'))
   {
     if(Records.TotalNum > RECORDMAXITEM)
       Records.TotalNum = 0;
     for(i=0; i<Records.TotalNum; i++)
      {
       ReadFromFlash(RECORDCFG_ADDR + 0x200 + i*0x200, Flash_Stream, 0x200);
       xtm_memcopy(&RecordsContent[i], Flash_Stream, sizeof(struct InfoContent1));
       xtm_memcopy(&RecordsBuff[i], Flash_Stream + 120, sizeof(INT32U));
       xtm_printf("RecordsContent[%d].Length = %d, RecordsBuff[%d] = 0x%X 0x%X\n", i, RecordsContent[i].Length, i, (INT16U)((RecordsBuff[i] & 0xffff0000) >> 16), (INT16U)(RecordsBuff[i] & 0xffff));
      }
   }
  else
   {
    ResetRecordsCfg();    //复位通话记录配置文件
    WriteRecordsCfgFile();     //写通话记录设置文件
   }
}
//--------------------------------------------------------------------------
//写通话记录文件
void WriteRecordsFile(void)
{
  INT16U i;
  for(i=0; i<RECORDMAXITEM; i++)               //   Records.TotalNum
   {
    xtm_memcopy(Flash_Stream, &RecordsContent[i], sizeof(struct InfoContent1));
    xtm_memcopy(Flash_Stream + 120, &RecordsBuff[i], sizeof(INT32U));
    SaveToFlash(RECORDCFG_ADDR + 0x200 + i*0x200, Flash_Stream, 0x200);
   }
}
//--------------------------------------------------------------------------
void ResetRecordsCfg(void)    //复位通话记录配置文件
{
  strcpy(Records.FlashHead, "RECO"); //Flash已存标志
  Records.MaxNum = RECORDMAXITEM;   //最大信息数
  Records.TotalNum = 0; //信息总数
  Records.NoReadedNum = 0; //未读信息总数
  Records.TotalInfoPage = 0;   //总信息页数
  Records.CurrentInfoPage = 0; //当前信息页
  Records.CurrNo = 0;    //当前信息序号
  Records.CurrPlayNo = 0;  //当前播放序号
}
//--------------------------------------------------------------------------
void WriteRecordsCfgFile(void)     //写通话记录设置文件
{
  xtm_memcopy(Flash_Stream, &Records, sizeof(struct Info1));
  SaveToFlash(RECORDCFG_ADDR, Flash_Stream, 0x200);
}
//--------------------------------------------------------------------------
void Recv_NewRecords(void)
{
}
//--------------------------------------------------------------------------
//写照片函数
void WritePicFunc(void)
{
  INT16 j, i;
  INT8U isValid, isFull;
  INT16U tmplen;
  INT32U tmp_data;
      TmpRecordsContent.isValid = 1;
      TmpRecordsContent.isReaded = 0;

      isFull = 0;
      //如信息未到最大数量，顺序后移，将新信息插入到头部
      if(Records.TotalNum < Records.MaxNum)
       {
         if(Records.TotalNum > 0)
          for(j = (Records.TotalNum - 1); j >= 0; j --)
           {
            xtm_memcopy(&RecordsContent[j+1], &RecordsContent[j], sizeof(struct InfoContent1));
            RecordsBuff[j+1] = RecordsBuff[j];
           }
          Records.TotalNum ++;
       }
      else   //丢弃最后一条，顺序后移，将新信息插入到头部
       {
          isFull = 1;
          tmp_data = RecordsBuff[Records.TotalNum - 1];
          for(j = (Records.TotalNum - 1); j >=1; j --)
           {
            xtm_memcopy(&RecordsContent[j], &RecordsContent[j-1], sizeof(struct InfoContent1));
            RecordsBuff[j] = RecordsBuff[j-1];
           }
          RecordsBuff[0] = tmp_data;
       }

      RecordsContent[0].isValid = TmpRecordsContent.isValid;  //有效，未删除标志   1
      RecordsContent[0].isReaded = TmpRecordsContent.isReaded; //已读标志    1
      RecordsContent[0].HavePic = TmpRecordsContent.HavePic;  //有无图像标志  0   1 -- 640x480  2 -- 320x240
      strcpy(RecordsContent[0].RemoteAddr, TmpRecordsContent.RemoteAddr);
      strcpy(RecordsContent[0].Time, TmpRecordsContent.Time);    //接收时间    32
      RecordsContent[0].Type = TmpRecordsContent.Type;     //类型        1    信息类型或事件类型
      RecordsContent[0].Length = TmpRecordsContent.Length;       //长度        4

      if(isFull == 0)
       for(j=0; j<RECORDMAXITEM; j++)
        {
         RecordsBuff[0] = RECORDPIC_ADDR + 0x10000 * j;
         isValid = 1;
         for(i=1; i<Records.TotalNum; i++)
          if(RecordsBuff[0] == RecordsBuff[i])
           {
            isValid = 0;
            break;
           }
         if(isValid == 1)
           break;
        }   


      xtm_printf("WritePicFunc::isValid = %d, RecordsBuff[0] = 0x%X 0x%X, RecordsContent[0].Length = %d\n", isValid, (INT16U)((RecordsBuff[0] & 0xffff0000) >> 16), (INT16U)(RecordsBuff[0] & 0xffff), RecordsContent[0].Length);

      //如当前为通话记录窗口，刷新屏幕
      if(Local.CurrentWindow == 21)
       {
          ShowRecordsList(REFRESH, NOFULLPAGE);
       }
      WriteRecordsCfgFile();     //写通话记录设置文件
      WriteRecordsFile();        //写通话记录文件
      Recv_NewRecords();
      if(RecordsContent[0].HavePic != 0)
       {
          Local.NewRecPic = 1;  //有新留影
          
          outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
          outportb(MEMORY_WINDOW_BASE_H, 0x6f);
          tmplen = (RecordsContent[0].Length /4096 + 1)*4096;
          SaveToFlash(RecordsBuff[0], Memory6_Stream, tmplen);
          outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);
       }
}
//--------------------------------------------------------------------------
