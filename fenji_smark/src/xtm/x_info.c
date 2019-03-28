#include "includes.h"

#include "stdlib.h"

#define x_info_h

#define CommonH
#include "common.h"

/*
****************************************
20131119
�����޸�
****************************************
*/


void DisplayInfoWindow(INT8U RefreshType, INT8U Curr_Sec);    //��ʾ��Ϣ����
void PreDealInfoWindow(INT8U wintype);    //��Ϣ����Ԥ��������������Ч��
void OperateInfoWindow(INT8U wintype, INT8U currwindow);    //��Ϣ���ڲ���
void CheckTouchInfoWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

void ShowInfoList(INT8U infotype, INT8U refreshflag, INT8U FullPageType);    //��Ϣ��ʾ����һ����  ��Ϣ�б�

INT8U Info_CurrBox;
void DisplayInfoContentWindow(INT8U infotype, INT8U infono);    //��ʾ�鿴��Ϣ����
void PreDealInfoContentWindow(INT8U wintype);    //�鿴��Ϣ����Ԥ��������������Ч��
void OperateInfoContentWindow(INT8U wintype, INT8U currwindow);    //�鿴��Ϣ���ڲ���
void CheckTouchInfoContentWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

void ShowInfoContent(INT8U infotype, INT8U infono, INT8U pageno, INT8U FullPageType);    // ��Ϣ����
void CreateContentRow(char *Content, INT16U nLength, char create_row[MAXROW][INFOROWLEN + 2]);  //����Ҫ��ʾ��������

INT8U Init_InfoVar(void); //��ʼ�� Info �ṹ
INT8U Uninit_InfoVar(void); //�ͷ� Info �ṹ
//����Ϣ�ļ�
void ReadInfoFile(void);
//д��Ϣ�ļ�
void WriteInfoFile(void);

void ResetInfoCfg(void);    //��λ��Ϣ�����ļ�
void WriteInfoCfgFile(void);     //д��Ϣ�����ļ�

void Recv_NewInfo(void);

struct displayinfo1 displayinfo;
//---------------------------------------------------------------------------
void DisplayInfoWindow(INT8U RefreshType, INT8U Curr_Sec)    //��ʾ��Ϣ����
{
  INT8U j;
  RefreshBackWindow(RefreshType, FULLPAGE); //ˢ�±���

  DisplayImage(&info_image, FROMFILE, FULLPAGE);

  Local.NewInfo = 0;
  //SetGpio(GPIOD_OUT_DATA, INFO_LED_HIGH, SET_HIGH);  //����Ϣ LED
  Info.CurrNo = Curr_Sec;            //��ǰ��Ϣ���  0---n-1
  Info.CurrentInfoPage = Info.CurrNo /INFONUMPERPAGE + 1;     //��ǰ��Ϣҳ

  ShowInfoList(0, REFRESH, FULLPAGE);

  //for(j=0; j<Info[0].TotalNum; j++)
  //  xtm_printf("DisplayInfoWindow::InfoBuff[%d] = %s\n", j, InfoBuff[j]);
  Local.CurrentWindow = 111;

  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//--------------------------------------------------------------------------
void PreDealInfoWindow(INT8U wintype)    //��Ϣ����Ԥ��������������Ч��
{
  if((wintype >= 0)&&(wintype <= 2))
   {
    DisplayImageButton(&info_button[wintype], IMAGEDOWN, FROMFILE, NOFULLPAGE);
   }

  PreDealBigmenuButton(wintype);  //��ݰ��� Ԥ����
}
//--------------------------------------------------------------------------
void OperateInfoWindow(INT8U wintype, INT8U currwindow)    //��Ϣ���ڲ���
{
  INT8U i;
  INT8U TmpInfoNo;
  INT16U tmp;
  if(wintype < 3)
    DisplayImageButton(&info_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);

  ShowPressQuickmenu_Button(wintype);  //��ݰ���  ����

  switch(wintype)
   {
    case 0://�Ϸ�       a
             if(Info.CurrentInfoPage > 1)
              {
               Info.CurrentInfoPage --;
               xtm_printf("OperateInfoWindow1:: Info.CurrNo = %d, Info.CurrentInfoPage = %d\n",  Info.CurrNo, Info.CurrentInfoPage);
               tmp = (INT16U)INFONUMPERPAGE * (Info.CurrentInfoPage - 1);
               Info.CurrNo = tmp;
               xtm_printf("OperateInfoWindow2:: Info.CurrNo = %d, Info.CurrentInfoPage = %d\n",  Info.CurrNo, Info.CurrentInfoPage);
               ShowInfoList(0, REFRESH, NOFULLPAGE);
              }
           break;
    case 1://�·�     b
             //xtm_printf("Info.CurrentInfoPage = %d, Info.TotalInfoPage = %d\n", Info.CurrentInfoPage, Info.TotalInfoPage);
             if(Info.CurrentInfoPage < Info.TotalInfoPage)
              {
               Info.CurrentInfoPage ++;
               xtm_printf("OperateInfoWindow3:: Info.CurrNo = %d, Info.CurrentInfoPage = %d\n",  Info.CurrNo, Info.CurrentInfoPage);
               tmp = (INT16U)INFONUMPERPAGE * (Info.CurrentInfoPage - 1);
               Info.CurrNo = tmp;
               xtm_printf("OperateInfoWindow4:: Info.CurrNo = %d, Info.CurrentInfoPage = %d\n",  Info.CurrNo, Info.CurrentInfoPage);
               ShowInfoList(0, REFRESH, NOFULLPAGE);
              }
           break;
    case 2://ɾ��           c
               if(Info.CurrNo < Info.TotalNum)
                {
                 //˳��ǰ��
                 for(i = Info.CurrNo; i < (Info.TotalNum - 1); i ++)
                  {
                   xtm_memcopy(&InfoContent[i], &InfoContent[i+1], sizeof(struct InfoContent1));
                   strcpy(InfoBuff[i], InfoBuff[i+1]);
                  }
                 Info.TotalNum --;
                 xtm_printf("Info.CurrNo = %d, Info.TotalNum = %d\n", Info.CurrNo, Info.TotalNum);
                 if(Info.CurrNo > (Info.TotalNum - 1))
                   Info.CurrNo = Info.TotalNum - 1;
                 ShowInfoList(0, NOREFRESH, NOFULLPAGE);
                 WriteInfoCfgFile();     //д��Ϣ�����ļ�
                 WriteInfoFile();       //д��Ϣ�ļ�
                }
             break;
    case 40://��Ϣ1��           c
    case 41://��Ϣ2��           c
    case 42://��Ϣ3��           c
    case 43://��Ϣ4��           c
    case 44://��Ϣ5��           c
           //��ǰ��Ϣ���
             TmpInfoNo = (Info.CurrentInfoPage-1)*INFONUMPERPAGE+(wintype - 40);
             xtm_printf("TmpInfoNo = %d, Info.TotalNum = %d \n", TmpInfoNo,
                    Info.TotalNum);
             if(Info.CurrNo == TmpInfoNo)
              {
               if(TmpInfoNo < (Info.TotalNum))
                {
                 Info.CurrNo = TmpInfoNo;
                 DisplayInfoContentWindow(0, Info.CurrNo);
                }
              }
             else
              {
               if(TmpInfoNo < (Info.TotalNum))
                {
                 Info.CurrNo = TmpInfoNo;
                 ShowInfoList(0, NOREFRESH, NOFULLPAGE);
                }
              }
           break;
    default:
          OperateQuickmenu(wintype, 104);
          break;
   }
}
//--------------------------------------------------------------------------
void CheckTouchInfoWindow(INT16U XLCD, INT16U YLCD) //��鴥��������
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   for(i=0; i<3; i++)  //���ڰ�ť
    {
     isInTouch = CheckTSInButton(&info_button[i], XLCD, YLCD);
     if(isInTouch == 1)
      {
       TouchNum = i;
       break;
      }
    }

  if(isInTouch == 0)
   for(i=0; i<INFONUMPERPAGE; i++)  //���ڰ�ť
    {
     isInTouch = CheckTSInButton(&inforow_button[i], XLCD, YLCD);
     if(isInTouch == 1)
      {
       TouchNum = i + 40;
       break;
      }
    }

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
void ShowInfoList(INT8U infotype, INT8U refreshflag, INT8U FullPageType)    //��Ϣ��ʾ����һ����  ��Ϣ�б�
{
  INT8U i, j;
  INT8U PageTotalNum;  //��ҳ��Ϣ����
  INT8U CurrNo;
  char tmp_con[20];
  INT16U tmp_len;  //Ԥ������
  INT8U NoInPage;
  INT16U ascnum, tmp;
  INT8U DeltaHei;
  INT16U FontColor;

  tmp_len = 10;

  switch(infotype)
   {
    case 0: //��ͨ��Ϣ
           if(Info.TotalNum > 0)
            {
             //��ҳ��
             if((Info.TotalNum % INFONUMPERPAGE) == 0)
               tmp = Info.TotalNum /INFONUMPERPAGE;
             else
               tmp = Info.TotalNum /INFONUMPERPAGE + 1;
             Info.TotalInfoPage = tmp;

             //��ǰҳ
             tmp = Info.CurrNo / INFONUMPERPAGE + 1;
             Info.CurrentInfoPage = tmp;
             if(Info.CurrentInfoPage < Info.TotalInfoPage)
               PageTotalNum = INFONUMPERPAGE;
             else
               PageTotalNum = Info.TotalNum - (Info.CurrentInfoPage - 1)*INFONUMPERPAGE;
             //��ǰ��Ϣ�ڱ�ҳ�е�λ��
             NoInPage = (Info.CurrNo)%INFONUMPERPAGE;

             xtm_printf("ShowInfoList::Info.CurrNo = %d, Info.TotalNum = %d, Info.CurrentInfoPage = %d\n", Info.CurrNo, Info.TotalNum, Info.CurrentInfoPage);
             for(i = 0; i < INFONUMPERPAGE; i++)
              {
               if(i == NoInPage)
                 DisplayImageButton(&inforow_button[i], IMAGEDOWN, FROMFILE, FullPageType);
               else
                 DisplayImageButton(&inforow_button[i], IMAGEUP, FROMFILE, FullPageType);
              }
             WaitDecodeFinish();

             for(i = 0; i < PageTotalNum; i++)
              {
               if(i == NoInPage)
                 FontColor = COLOR_WHITE;
               else
                 FontColor = COLOR_BLACK;
                 
               //xtm_printf("FontColor = 0x%X\n", FontColor);
               CurrNo = (Info.CurrentInfoPage - 1)*INFONUMPERPAGE + i;

               DeltaHei = (inforow_button[i].height - 24)/2;
               //���
               xtm_sprintf(tmp_con, "%02d\0", (Info.CurrentInfoPage-1)*INFONUMPERPAGE+i+1);
               tmp_con[2] = '\0';
               //xtm_printf("tmp_con = %s\n", tmp_con);
               display_text(inforow_button[i].xLeft+10, inforow_button[i].yTop + DeltaHei, 0, FontColor, tmp_con, 0, 24, FullPageType);

               //xtm_printf("InfoBuff[%d] = %s\n", CurrNo, InfoBuff[CurrNo]);
               //����
               if(InfoContent[CurrNo].Length >= tmp_len)
                {
                 xtm_memcopy(tmp_con, InfoBuff[CurrNo], tmp_len);
                 tmp_con[tmp_len] = '\0';
                 for(j=0; j<tmp_len; j++)
                  if(tmp_con[j] == '\r')
                    tmp_con[j] = '\0';
                 if((INT8U)tmp_con[8] < 0xA1)
                   tmp_con[9] = '\0';

                 ascnum = 0;
                 for(j=0; j<strlen(tmp_con); j++)
                  {
                   if((INT8U)tmp_con[j] < 0xA1)
                     ascnum ++;
                   else
                     j ++;
                  }
                 if((ascnum % 2) != 0)
                   tmp_con[9] = '\0';
                }
               else
                {
                 xtm_memcopy(tmp_con, InfoBuff[CurrNo], InfoContent[CurrNo].Length);
                 tmp_con[InfoContent[CurrNo].Length] = '\0';
                 for(j=0; j<InfoContent[CurrNo].Length; j++)
                  if(tmp_con[j] == '\r')
                    tmp_con[j] = '\0';
                }
               display_text(inforow_button[i].xLeft+60, inforow_button[i].yTop+DeltaHei, 0, FontColor, tmp_con, 0, 24, FullPageType);

               //ʱ��
             /*  xtm_memcopy(tmp_con, InfoContent[infotype][CurrNo].Time, tmp_len);
                 tmp_con[tmp_len] = '\0';
               outxy24(inforow_button[i].xLeft+200, inforow_button[i].yTop+DeltaHei, 1,
                       fontcolor, 1, 1, tmp_con, 0, Local.CurrFbPage);         */

               display_text(inforow_button[i].xLeft+350, inforow_button[i].yTop+DeltaHei, 0, FontColor, "������", 0, 24, FullPageType);
              }
            }
           else
             for(i = 0; i < INFONUMPERPAGE; i++)
               DisplayImageButton(&inforow_button[i], IMAGEUP, FROMFILE, FullPageType);
           break;
   }
}
//--------------------------------------------------------------------------
void DisplayInfoContentWindow(INT8U infotype, INT8U infono)    //��ʾ�鿴��Ϣ����
{
  DisplayImage(&infowin_image, FROMFILE, NOFULLPAGE);

  Info_CurrBox = infono;
  xtm_printf("InfoContent[infono].Length= %d\n", InfoContent[infono].Length);
  displayinfo.totalpage = TOTALPAGE;
  displayinfo.pageno = 0;
  CreateContentRow(InfoBuff[infono], InfoContent[infono].Length, displayinfo.content_row);
  ShowInfoContent(0, infono, 0, NOFULLPAGE);
  
  Local.CurrentWindow = 112;
  
//  Local.fb_page = !Local.fb_page;
//  selectym(Local.fb_page);
}
//--------------------------------------------------------------------------
void PreDealInfoContentWindow(INT8U wintype)    //�鿴��Ϣ����Ԥ��������������Ч��
{
  if((wintype >= 0)&&(wintype <= 2))
   {
    DisplayImageButton(&infowin_button[wintype], IMAGEDOWN, FROMFILE, NOFULLPAGE);
   }

  PreDealBigmenuButton(wintype);  //��ݰ��� Ԥ����
}
//--------------------------------------------------------------------------
void OperateInfoContentWindow(INT8U wintype, INT8U currwindow)    //�鿴��Ϣ���ڲ���
{
  INT8U i;
  INT8U TmpInfoNo;
  if(wintype < 3)
    DisplayImageButton(&infowin_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);

  ShowPressQuickmenu_Button(wintype);  //��ݰ���  ����

  switch(wintype)
   {
    case 0://�Ϸ�       a
            if(displayinfo.pageno > 0)
             {
              displayinfo.pageno --;
              ShowInfoContent(0, Info_CurrBox, displayinfo.pageno, NOFULLPAGE);
             }
             break;
    case 1://�·�     b
             if(displayinfo.pageno < (displayinfo.totalpage - 1))
              {
               displayinfo.pageno ++;
               ShowInfoContent(0, Info_CurrBox, displayinfo.pageno, NOFULLPAGE);
              }      
             break;
    case 2://����
             DisplayInfoWindow(REFRESH_MIDDLE, Info_CurrBox);    //ˢ�� ��
             break;
    default:
             OperateQuickmenu(wintype, 104);
             break;
   }
}
//--------------------------------------------------------------------------
void CheckTouchInfoContentWindow(INT16U XLCD, INT16U YLCD) //��鴥��������
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   for(i=0; i<3; i++)  //���ڰ�ť
    {
     isInTouch = CheckTSInButton(&infowin_button[i], XLCD, YLCD);
     if(isInTouch == 1)
      {
       TouchNum = i;
       break;
      }
    }

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
void ShowInfoContent(INT8U infotype, INT8U infono, INT8U pageno, INT8U FullPageType)    // ��Ϣ����
{
  int xLeft,yTop,yHeight;
  int i, j;
  char tmp_con[50];
  int tmp_len;  //Ԥ������
  int tmp_row;
  int numperpage;

  DisplayImage(&infocontent_image, FROMFILE, FullPageType);
  tmp_len = 20;

  xLeft = infocontent_image.xLeft;
  yTop = infocontent_image.yTop + 6;
  yHeight = 38;
  if(infono <= (Info.TotalNum - 1))
   {
    for(i=0; i<50; i++)
      tmp_con[i] = '\0';
    xtm_sprintf(tmp_con, "�� %d ��" , infono + 1);
    display_text(xLeft+16, yTop+0*yHeight, 0, COLOR_BLACK, tmp_con, 0, 24, FullPageType);

    for(i=0; i<50; i++)
      tmp_con[i] = '\0';
    xtm_sprintf(tmp_con, "�� %d ҳ" , displayinfo.pageno + 1);
    display_text(xLeft+150, yTop+0*yHeight, 0, COLOR_BLACK, tmp_con, 0, 24, FullPageType);

  //  xtm_memcopy(tmp_con, InfoContent[infotype][infono].Time, tmp_len);
  //  tmp_con[tmp_len] = '\0';
  //  display_text(xLeft + 100, yTop+0*yHeight, 0, COLOR_BLACK, tmp_con, 0, 24);


    yTop = yTop+1*yHeight;

    if((displayinfo.totalrow % PAGEPERROW) == 0)
      displayinfo.totalpage = displayinfo.totalrow /PAGEPERROW;
    else
      displayinfo.totalpage = displayinfo.totalrow /PAGEPERROW + 1;

    if(displayinfo.totalpage == (displayinfo.pageno + 1))
      numperpage = displayinfo.totalrow - displayinfo.pageno*PAGEPERROW;
    else
      numperpage = PAGEPERROW;

    for(i=displayinfo.pageno*PAGEPERROW; i<(displayinfo.pageno*PAGEPERROW + numperpage); i++)
     {
      display_text(xLeft + 16, yTop+(i-displayinfo.pageno*PAGEPERROW)*yHeight, 0, COLOR_BLACK, displayinfo.content_row[i], 0, 24, FullPageType);
      xtm_printf("displayinfo.content_row[i] = %s\n", displayinfo.content_row[i]);
     } 
   }
}
//--------------------------------------------------------------------------
void CreateContentRow(char *Content, INT16U nLength, char create_row[MAXROW][INFOROWLEN + 2])  //����Ҫ��ʾ��������
{
  int i,j;
  int row;
  int asciinum;
  int row_pixel;
  row = 0;
  for(i=0; i<MAXROW; i++)
   for(j=0; j<(INFOROWLEN + 2); j++)
    create_row[i][j] = '\0';
  j = 0;
  asciinum = 0;
  row_pixel = 0;
  for(i=0; i<nLength; i++)
   {
    if((Content[i] == '\r')&&(Content[i+1] == '\n'))
     {
      create_row[row][j] = '\0';
      row ++;
      asciinum = 0;
      j = 0;
      row_pixel = 0;
      i++;
     }
    else
     {
      if((INT8U)Content[i] >= 0xA1)
       {
        create_row[row][j] = Content[i];
        i ++;
        j ++;
        create_row[row][j] = Content[i];
        j ++;
        row_pixel += 24;
       }
      else
       {
          create_row[row][j] = Content[i];
          j ++;
          row_pixel += 16;
          asciinum ++;
       }
      if((j >= (INFOROWLEN-1)) || (row_pixel >= INFOROWPIXEL))
       {
    //    printf("j = %d, asciinum = %d, i = %d\n", j, asciinum, i);
        if((asciinum % 2) != 0)
         {
          create_row[row][j] = ' ';
         }
        create_row[row][j+1] = '\0';
        row ++;
        asciinum = 0;
        j = 0;
        row_pixel = 0;
       }
     }
    if(row >= MAXROW)
      break;
   }
  displayinfo.totalrow = row+1;
  if((displayinfo.totalrow % PAGEPERROW) == 0)
    displayinfo.totalpage = displayinfo.totalrow / PAGEPERROW;
  else
    displayinfo.totalpage = displayinfo.totalrow / PAGEPERROW + 1;          

  //printf("0 1 2 3 4 5 6 7 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", Content[0], Content[1], Content[2], Content[3], Content[4], Content[5]);
  //for(i=0; i<displayinfo.totalrow; i++)
  //  printf("create_row[row] = %s, len = %d\n", create_row[i], strlen(create_row[i]));      
}
//--------------------------------------------------------------------------
INT8U Init_InfoVar(void) //��ʼ�� Info �ṹ
{
  INT8U i;
  //��ʼ������Ϣ�ṹ
  Info.MaxNum = INFOMAXITEM;

  //����Ϣ�ļ�
  ReadInfoFile();

  Info.MaxNum = INFOMAXITEM;   //�����Ϣ��
  xtm_printf("Info.TotalNum = %d, Info.NoReadedNum = %d, Info.MaxNum = %d\n", Info.TotalNum, Info.NoReadedNum, Info.MaxNum);
  Info.CurrentInfoPage = 1;

  return 0;
}
//--------------------------------------------------------------------------
INT8U Uninit_InfoVar(void) //�ͷ� Info �ṹ
{
}
//--------------------------------------------------------------------------
//����Ϣ�ļ�
void ReadInfoFile(void)
{
  INT16U i;
  ReadFromFlash(INFO_ADDR, Flash_Stream, 0x200);
  xtm_memcopy(&Info, Flash_Stream, sizeof(struct Info1));
  if((Info.FlashHead[0] == 'I')&&(Info.FlashHead[1] == 'N')&&(Info.FlashHead[2] == 'F')&&(Info.FlashHead[3] == 'O'))
   {
     if(Info.TotalNum > INFOMAXITEM)
       Info.TotalNum = 0;
     for(i=0; i<Info.TotalNum; i++)
      {
       ReadFromFlash(INFO_ADDR + 0x200 + i*0x200, Flash_Stream, 0x200);
       xtm_memcopy(&InfoContent[i], Flash_Stream, sizeof(struct InfoContent1));
       xtm_memcopy(InfoBuff[i], Flash_Stream + 80, INFOMAXSIZE);
       //xtm_printf("InfoContent[%d].Length = %d, %s\n", i, InfoContent[i].Length, InfoBuff[i]);
      }
   }
  else
   {
    ResetInfoCfg();    //��λ��Ϣ�����ļ�
    WriteInfoCfgFile();     //д��Ϣ�����ļ�
   }
}
//--------------------------------------------------------------------------
//д��Ϣ�ļ�
void WriteInfoFile(void)
{
  INT16U i;
  for(i=0; i<Info.TotalNum; i++)
   {
    xtm_memcopy(Flash_Stream, &InfoContent[i], sizeof(struct InfoContent1));
    xtm_memcopy(Flash_Stream + 80, InfoBuff[i], INFOMAXSIZE);
    SaveToFlash(INFO_ADDR + 0x200 + i*0x200, Flash_Stream, 0x200);
   }
}
//--------------------------------------------------------------------------
void ResetInfoCfg(void)    //��λ��Ϣ�����ļ�
{
  strcpy(Info.FlashHead, "INFO"); //Flash�Ѵ��־
  Info.MaxNum = INFOMAXITEM;   //�����Ϣ��
  Info.TotalNum = 0; //��Ϣ����
  Info.NoReadedNum = 0; //δ����Ϣ����
  Info.TotalInfoPage = 0;   //����Ϣҳ��
  Info.CurrentInfoPage = 0; //��ǰ��Ϣҳ
  Info.CurrNo = 0;    //��ǰ��Ϣ���
  Info.CurrPlayNo = 0;  //��ǰ�������
}
//--------------------------------------------------------------------------
void WriteInfoCfgFile(void)     //д��Ϣ�����ļ�
{
  xtm_memcopy(Flash_Stream, &Info, sizeof(struct Info1));
  SaveToFlash(INFO_ADDR, Flash_Stream, 0x200);
}
//--------------------------------------------------------------------------
void Recv_NewInfo(void)
{
  Local.NewInfo = 1;  //������Ϣ
  DisplayInfoStateImage(Local.NewInfo);    //��ʾ��Ϣͼ��
  SetGpio(GPIOD_OUT_DATA, INFO_LED_LOW, SET_LOW);  //����Ϣ LED
}
//--------------------------------------------------------------------------
