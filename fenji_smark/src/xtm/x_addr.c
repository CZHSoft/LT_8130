#include "includes.h"

#include "stdlib.h"

#define CommonH
#include "common.h"

/*
****************************************
20131119
暂无修改
****************************************
*/


#ifdef _ADDTABLE_SUPPORT  //地址表支持
//INT8U Init_RecordsVar(void); //初始化 Records 结构
//INT8U Uninit_RecordsVar(void); //释放 Records 结构
//读地址表文件
void ReadAddrTableFile(void);
//写地址表文件
void WriteAddrTableFile(void);

INT32U AddrTablePos = 0;

//从地址表 查询地址
INT8U QueryAddrTableFile(char *DestAddr, INT8U *DestIP);
INT8U CaptureAddrFromLine(char *a_Addr, INT8U *a_IP, char *LineText);

//从地址表 读取一行
INT8U ReadAddrTableLine(char *LineText);
//---------------------------------------------------------------------------
/*INT8U Init_RecordsVar(void) //初始化 Records 结构
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
}                                                                */
//--------------------------------------------------------------------------
//读地址表文件
void ReadAddrTableFile(void)
{
  INT16U i;
  ReadFromFlash(RECORDCFG_ADDR, Flash_Stream, 0x200);
  memcpy(&Records, Flash_Stream, sizeof(struct Info1));
  if((Records.FlashHead[0] == 'R')&&(Records.FlashHead[1] == 'E')&&(Records.FlashHead[2] == 'C')&&(Records.FlashHead[3] == 'O'))
   {
     if(Records.TotalNum > RECORDMAXITEM)
       Records.TotalNum = 0;
     for(i=0; i<Records.TotalNum; i++)
      {
       ReadFromFlash(RECORDCFG_ADDR + 0x200 + i*0x200, Flash_Stream, 0x200);
       memcpy(&RecordsContent[i], Flash_Stream, sizeof(struct InfoContent1));
       memcpy(&RecordsBuff[i], Flash_Stream + 120, sizeof(INT32U));
       xtm_printf("RecordsContent[%d].Length = %d, RecordsBuff[%d] = 0x%X 0x%X\n", i, RecordsContent[i].Length, i, (INT16U)((RecordsBuff[i] & 0xffff0000) >> 16), (INT16U)(RecordsBuff[i] & 0xffff));
      }
   }
  else
   {
   }
}
//--------------------------------------------------------------------------
//写地址表文件
void WriteAddrTableFile(void)
{
  INT16U i;
  for(i=0; i<RECORDMAXITEM; i++)               //   Records.TotalNum
   {
    memcpy(Flash_Stream, &RecordsContent[i], sizeof(struct InfoContent1));
    memcpy(Flash_Stream + 120, &RecordsBuff[i], sizeof(INT32U));
    SaveToFlash(RECORDCFG_ADDR + 0x200 + i*0x200, Flash_Stream, 0x200);
   }
}
//--------------------------------------------------------------------------
//从地址表 查询地址
INT8U QueryAddrTableFile(char *DestAddr, INT8U *DestIP)
{
  INT16U i, TotalNum;
  INT8U j, addr_len;
  char LineText[50];
  char a_Addr[20];
  INT8U a_IP[4];
  AddrTablePos = 0;
  ReadAddrTableLine(LineText);
  xtm_printf("QueryAddrTableFile 1, %s\n", LineText);
  if(strcmp(LineText, "ADDRTABLE") == 0)
   {
     xtm_printf("QueryAddrTableFile 2, %s\n", LineText);
     ReadAddrTableLine(LineText);  //版本号
     xtm_printf("AddrTable Version = %s\n", LineText);
     ReadAddrTableLine(LineText);  //数量
     TotalNum = atoi(LineText);
     if(TotalNum > ADDR_MAXNUM)
       TotalNum = ADDR_MAXNUM;
     xtm_printf("AddrTable TotalNum = %d\n", TotalNum);
     for(i=0; i<TotalNum; i++)
      {
       //xtm_printf("i = %d, TotalNum = %d\n", i, TotalNum);
       ReadAddrTableLine(LineText);  //地址
       xtm_printf("i = %d, LineText = %s\n", i, LineText);
       CaptureAddrFromLine(a_Addr, a_IP, LineText);

       switch(a_Addr[0])
        {
         case 'S':
                  a_Addr[12] = '\0';
                  DestAddr[12] = '\0';
                  break;
         case 'B':
                  a_Addr[6] = '\0';
                  DestAddr[6] = '\0';
                  break;
         case 'M':
                  a_Addr[8] = '\0';
                  DestAddr[8] = '\0';
                  break;
         case 'W':
                  a_Addr[5] = '\0';
                  DestAddr[5] = '\0';
                  break;
         case 'H':
                  a_Addr[12] = '\0';
                  DestAddr[12] = '\0';
                  break;
         case 'Z':
                  a_Addr[5] = '\0';
                  DestAddr[5] = '\0';
                  break;
        }

       if(strcmp(a_Addr, DestAddr) == 0)
        {
         memcpy(DestIP, a_IP, 4);
         xtm_printf("a_Addr = %s, DestIP = %d.%d.%d.%d\n", a_Addr, DestIP[0], DestIP[1], DestIP[2], DestIP[3]);
         return 1;
        }
       //xtm_printf("RecordsContent[%d].Length = %d, RecordsBuff[%d] = 0x%X 0x%X\n", i, RecordsContent[i].Length, i, (INT16U)((RecordsBuff[i] & 0xffff0000) >> 16), (INT16U)(RecordsBuff[i] & 0xffff));
      }
   }
  else
   {
     return 0;
   }
}
//--------------------------------------------------------------------------
INT8U CaptureAddrFromLine(char *a_Addr, INT8U *a_IP, char *LineText)
{
  INT8U j, addr_len, Addr_pos;
  char ip_text[20];
  char TmpText[4][5];

  addr_len = strlen(LineText);
  for(j=0; j<addr_len; j++)
   {
    if((LineText[j] == '-')&&(LineText[j+1] == '-'))
     {
      Addr_pos = j;
      break;
     }
   }
  memcpy(a_Addr, LineText, Addr_pos);
  a_Addr[Addr_pos] = '\0';

  memcpy(ip_text, LineText + Addr_pos + 2, addr_len - Addr_pos - 2);
  ip_text[addr_len - Addr_pos - 2] = '\0';

  memcpy(TmpText[0], ip_text, 3);
  TmpText[0][3] = '\0';
  memcpy(TmpText[1], ip_text + 4, 3);
  TmpText[1][3] = '\0';
  memcpy(TmpText[2], ip_text + 8, 3);
  TmpText[2][3] = '\0';
  memcpy(TmpText[3], ip_text + 12, 3);
  TmpText[3][3] = '\0';

  for(j=0; j<4; j++)
    a_IP[j] = atoi(TmpText[j]);
  //xtm_printf("a_Addr = %s, ip_text = %s\n", a_Addr, ip_text);
}
//--------------------------------------------------------------------------
//从地址表 读取一行
INT8U ReadAddrTableLine(char *LineText)
{
  INT8U LinePos;
  LinePos = 0;
  while(1)
   {
    xtm_read25x32data(ADDRTABLE_ADDR + AddrTablePos, LineText + LinePos, 1);
    AddrTablePos ++;
    if(LineText[LinePos] == '\r')
     {
      LineText[LinePos] = '\0';
      AddrTablePos ++;
      break;
     }
    LinePos ++;

    if(LinePos >= 39)
     {
      LineText[LinePos] = '\0';
      break;
     }
   }
  //xtm_printf("LinePos = %d\n", LinePos);
  return 1;
}
#endif
//--------------------------------------------------------------------------
