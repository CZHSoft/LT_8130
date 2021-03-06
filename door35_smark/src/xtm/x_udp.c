#include "includes.h"

#include "stdlib.h"

#define x_udp_h

#define CommonH
#include "common.h"

#define MAX_UDP_LENGTH 1520
#define MAX_DOWN_PACK  512  //最大下载包数  512K

#define MAX_VIDEO_PACK  60  //每帧最大视频包数

void Init_Udp(void);    //初始化UDP
INT8U Udp_Deal(INT8U *recv_buf, INT16U recv_len);    //UDP 处理
void DataPortDeal(INT8U *buff, INT8U *FromIP, INT8U *DestIP, INT16U DestPort, INT16U recv_len);
void VideoPortDeal(INT8U *buff, INT8U *FromIP, INT16U DestPort, INT16U recv_len);

void SmarkPortDeal(INT8U *buff, INT8U *FromIP, INT8U *DestIP,INT16U DestPort, INT16U recv_len);
void SmarkMediaPortDeal(INT8U *buff, INT8U *FromIP, INT8U *DestIP,INT16U DestPort, INT16U recv_len);


#define _ADD_UDP_LOCK
#ifdef _ADD_UDP_LOCK
  INT8U MultiUdp_Lock = 0;
#endif
void CreateMultiUdpBuff(INT16U DestPort, INT8U SendMode, INT8U *RemoteIP, INT8U *udpdata, INT16U nlength, INT8U CurrOrder);
void CreateMultiUdpBuff_Audio(INT16U DestPort, INT8U SendMode, INT8U *RemoteIP, INT8U *udpdata, INT16U nlength, INT8U CurrOrder); //给音频中断调用
void CheckAudioUdpBuff(void);   //检查Audio UDP发送缓存
void CheckMultiUdpBuff(void);   //检查UDP发送缓存
void MultiUdpSendFailDeal(INT8U i);
INT8U ClearMultiUdpBuff(INT8U Order1, INT8U Order2, INT8U Order3, INT8U *cFromIP);   //Clear UDP发送缓存

//INT8U UdpSendBuff(INT16U DestPort, INT8U *RemoteMac, INT8U *RemoteIP, INT8U *udpdata, INT16U nlength);
INT8U UdpSendBuff_1(INT16U DestPort, INT8U *RemoteMac, INT8U *RemoteIP, INT8U *udpdata, INT16U nlength);
INT8U UdpSendBuff_Test(INT16U DestPort, INT8U *RemoteMac, INT8U *RemoteIP, INT8U *udpdata, INT16U nlength);
INT8U Build_Udp_Ip_Crc(INT8U *send_buf);
//udp 校验
INT8U Build_Udp_Crc(INT8U *send_buf, INT16U data_len);

void CheckRemoteAddr(void);  //检查远端地址

void RecvUdpTest_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
void RecvUdpTest1_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
//搜索所有设备应答
void RecvSearchAllEquip_Func(INT8U *recv_buf, INT8U *cFromIP, INT8U *DestIP, INT16U DestPort, INT16U length);
//写设备地址应答
void RecvWriteEquipAddr_Func(INT8U *recv_buf, INT8U *cFromIP, INT8U *DestIP, INT16U DestPort, INT16U length);

//下载
void RecvDownLoadFile_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
//检查是否需要删除Secter 4096 byte
void CheckFlashSecterErase(INT32U paddr);
INT8U WriteAllToFlash(INT8U *recv_buf, struct downfile1 *DownData);
void WriteFlash(INT8U WriteType, INT8U *recv_buf, struct downfile1 *DownData);      //WriteType  0 -- 文件头   1 -- 数据

//读Flash内容
void RecvReadFlash_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);

//远程重启动
void RecvRemoteReset_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);

//发送远程调试信息
void RecvRemoteDebugInfo_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
void send_debug_info(char *debug_info);

#ifdef _REMOTECALLTEST  //远程呼叫测试
//接收远程呼叫测试　
void RecvRemoteCallTest_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
#endif

#ifdef _BRUSHIDCARD_SUPPORT   //刷卡功能支持
 //写ID卡信息
 void RecvWriteIDCard_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
 //存储ID卡信息
 void SaveWriteIDCard_Func(void);
 //读ID卡信息
 void RecvReadIDCard_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
 //读ID卡应答
 void SendReadIDCardReply(void);
 //刷卡记录
 void RecvBrushIDCard_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
#endif

#ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
void Deal_CapturePic_Center(void);
//单元门口机、围墙机、二次门口机发送呼叫照片->发送开始
void RecvCapture_Send_Start_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
//单元门口机、围墙机、二次门口机发送呼叫照片->发送数据
void RecvCapture_Send_Data_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
//单元门口机、围墙机、二次门口机发送呼叫照片->发送结束（成功）
void RecvCapture_Send_Succ_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
//单元门口机、围墙机、二次门口机发送呼叫照片->发送结束（失败）
void RecvCapture_Send_Fail_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
#endif

//解析
void RecvNSAsk_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //解析请求
void RecvNSAsk2_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  ////解析请求2

void RecvNSReply_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);//解析应答

INT16U Curr_Frameno = 10000;
INT16U PackFrameno[MAXVIDEOBUF][MAX_VIDEO_PACK];
INT8U PackExist[MAXVIDEOBUF][MAX_VIDEO_PACK];

//设备定时报告状态
void RecvReportStatus_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
//管理中心查询设备状态
void RecvQueryStatus_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
//报警
void RecvAlarm_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U lengtht);
//取消报警
void RecvCancelAlarm_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
//复位密码
void RecvResetPass_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);

//对讲
void RecvTalkCall_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //对讲呼叫
void RecvTalkCallAnswer_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //对讲呼叫应答
void RecvTalkLineUse_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //对讲占线应答
void RecvTalkCallConfirm_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort); //对讲在线确认
INT8U AudioStartFlag = 0;
INT8U AudioStartTime = 0;
void RecvTalkCallStart_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //对讲开始通话
void RecvTalkCallEnd_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //对讲呼叫结束
void RecvTalkRemoteOpenLock_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //远程开锁
void RecvTalkCenterTurnTalk_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //中心人工转接
void RecvTalkTrusteeship_Talk_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //托管

void RecvTalkCallUpDown_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);  //对讲数据

//监视
void RecvWatchCall_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //监视呼叫
void RecvWatchCallAnswer_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //监视呼叫应答
void RecvWatchLineUse_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //监视占线应答
void RecvWatchCallConfirm_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort); //监视在线确认
void RecvWatchCallEnd_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //监视呼叫结束

void RecvWatchCallUpDown_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);  //监视数据

//smark
void RecvSmarkMonitorCall_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //监视呼叫
void RecvSmarkMonitorStart_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort);  //监视
void RecvSmarkMonitorCallConfirm_Func(void); //监视在线确认
void RecvSmarkMonitorCallEnd_Func(INT8U *recv_buf, INT8U *cFromIP);  //监视呼叫结束


void AudioDeal_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U length);  //音频数据处理
void VideoDeal_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U length);  //视频数据处理

void WatchEnd_ClearStatus(void); //监视结束，清状态和关闭音视频
void TalkEnd_ClearStatus(int isTurn); //对讲结束，清状态和关闭音视频

#ifdef _REMOTEWRITEOV7725REGISTER   //远程写OV7725寄存器  调试用
 void RecvRemoteWriteOV7725Register_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);
#endif

#ifdef ELEVATORCONTROL_HANGZHOU
 void RecvElevatorControl_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length);  //电梯控制
#endif

//主机与副机
#ifdef _MULTI_MACHINE_SUPPORT  //多分机支持
void RecvTalkLeaveGroup_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort); //退出组播组（主叫方->被叫方，被叫方应答）
void ExitGroup(INT8U *buf);      //向其它被叫方退出组播组命令
void SingleExitGroup(INT8U *buf); //向单个被叫方退出组播组命令
#endif

INT16U video_decode_no;  //视频解码包号
INT16U old_mjpeg_stream_no;  //视频解码包号
INT8U send_b[1520];
//视频接收缓冲  未解码

INT8U Down_RecvFlag[MAX_DOWN_PACK];          //下载 包标志

char LineInuse_Text[LANGUAGEMAXNUM][20] = {"对方正忙", "The line is busy"};
//---------------------------------------------------------------------------
void Init_Udp(void)    //初始化UDP
{
  strcpy(UdpPackageHead, "XXXCID");
  strcpy(UdpPackageHead2, "LTNW");
  
  LocalPort[0] = 8300;
      LocalPort[1] = 8302;
      LocalPort[2] = 8888;
      LocalPort[3] = 8889;
      LocalPort[4] = 8889;

  
      RemotePort[0] = LocalPort[0];
      RemotePort[1] = LocalPort[1];
      RemotePort[2] = LocalPort[2];
      RemotePort[3] = LocalPort[3];
      RemotePort[4] = LocalPort[4];

}
//---------------------------------------------------------------------------
//UDP 处理
INT8U Udp_Deal(INT8U *recv_buf, INT16U recv_len)
{
  INT8U i;
  INT8U PortOk;
  INT16U DestPort;
  INT8U *buff;
  INT8U *FromIP;
  INT8U *DestIP;

  if((recv_buf[12] == 0x08)&&(recv_buf[13] == 0x00)&&(recv_buf[23] == 0x11))  //UDP
   {
    /*if((recv_buf[30] != LocalCfg.IP[0])||(recv_buf[31] != LocalCfg.IP[1])||(recv_buf[32] != LocalCfg.IP[2])||(recv_buf[33] != LocalCfg.IP[3]))
     {
      //xtm_printf("Udp_Deal::DestIP = %d.%d.%d.%d, LocalCfg.IP = %d.%d.%d.%d\n", \
      //     recv_buf[30], recv_buf[31], recv_buf[32], recv_buf[33], LocalCfg.IP[0], LocalCfg.IP[1], LocalCfg.IP[2], LocalCfg.IP[3]);
      return 0;
     }       */
//    RefreshArp_Table(recv_buf + 6, recv_buf + 26);
    
    DestPort = recv_buf[36];
    DestPort <<= 8;
    DestPort += recv_buf[37];
    PortOk = 0;
    for(i=0; i<PORT_NUM; i++)
     if(LocalPort[i] == DestPort)
      {
       PortOk = 1;
       break;
      }

    if(!PortOk)
     {
      //xtm_printf("Udp_Deal::PortOk = %d, DestPort = %d\n", PortOk, DestPort);
      return 1;
     }
    //xtm_printf("Udp_Deal::DestPort = %d, recv_len = %d, 0x%X  0x%X  0x%X \n", DestPort, recv_len, recv_buf[48], recv_buf[49], recv_buf[50]);
    //xtm_memcopy(recv_text, recv_buf + 42, 6);
    //recv_text[6] = '\0';
    //xtm_printf("Udp_Deal::recv_text = %s, UdpPackageHead = %s\n", recv_text, UdpPackageHead);

    buff = recv_buf + 42;
    FromIP = recv_buf + 26;
    DestIP = recv_buf + 30;
    if((buff[0]==UdpPackageHead[0])&&(buff[1]==UdpPackageHead[1])&&(buff[2]==UdpPackageHead[2])
      &&(buff[3]==UdpPackageHead[3])&&(buff[4]==UdpPackageHead[4])&&(buff[5]==UdpPackageHead[5]))
     {
      //xtm_printf("Udp_Deal::1111\n");
     #if 0
      recv_len -= 46;//42;    // 46
     #else
      recv_len = (recv_buf[16] << 8) + recv_buf[17] - 28;
      if(recv_len > 1500)
        xtm_printf("Udp_Deal::recv_len = %d\n", recv_len);
     #endif
      switch(DestPort)
       {
        case 8300:
                  RefreshArp_Table(recv_buf + 6, recv_buf + 26);
                  DataPortDeal(buff, FromIP, DestIP, DestPort, recv_len);
                  break;
        case 8302:
                  //不为音视频数据
                  if (!(((buff[6] == VIDEOTALK)||(buff[6] == VIDEOWATCH))&&((buff[8] == CALLUP)||(buff[8] == CALLDOWN))))
                     RefreshArp_Table(recv_buf + 6, recv_buf + 26);
                  VideoPortDeal(buff, FromIP, DestPort, recv_len);
                  break;
       }
    }
    else if((buff[0]==UdpPackageHead2[0])&&(buff[1]==UdpPackageHead2[1])&&(buff[2]==UdpPackageHead2[2])
				&&(buff[3]==UdpPackageHead2[3]))
			{
#if 0
				recv_len -= 46;//42;    // 46
#else
				recv_len = (recv_buf[16] << 8) + recv_buf[17] - 28;
				if(recv_len > 1500)
					xtm_printf("Udp_Deal::recv_len = %d\n", recv_len);
#endif
				switch(DestPort)
				{
				case 8888:

					break;
                case 8889:
                    SmarkMediaPortDeal(buff, FromIP, DestIP, DestPort, recv_len);
                    //PlaySoundTip();
                    break;
				}
			}
    return 1;
   }
  return 0;
}
//---------------------------------------------------------------------------
void DataPortDeal(INT8U *buff, INT8U *FromIP, INT8U *DestIP, INT16U DestPort, INT16U recv_len)
{
     #ifdef _SEND_TO_SELF_TEST    //发送数据包给自己
      INT16U val;
      if((FromIP[0] == LocalCfg.IP[0])&&(FromIP[1] == LocalCfg.IP[1])&&(FromIP[2] == LocalCfg.IP[2])&&(FromIP[3] == LocalCfg.IP[3]))
       {
        val = phy_reg_read(0, 0);
        val = val & 0xbfff;
        phy_reg_write(0, 0, val);   //loopback
        xtm_printf("DataPortDeal::recv SEND_SELF\n");

        if((buff[2] != UdpPackageHead[2])&&(buff[3] != UdpPackageHead[3])&&(buff[4] != UdpPackageHead[4])&&(buff[5] != UdpPackageHead[5]))
         {
          xtm_printf("DataPortDeal::SEND_SELF 重启动\n");
          LocalCfg.SelfBootNo ++;                      //编码错误数目
          WriteCfgFile();
          Local.GpioWatchDogStarted = 0;  //在GPIO检测线程中启动WatchDog
         }
        return;
       }
     #endif
      switch(buff[6])
       {
        case REPORTSTATUS:   //设备定时报告状态
                if(recv_len >= SMALLESTSIZE)
                 {
                  RecvReportStatus_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                    xtm_printf("设备定时报告状态应答长度异常\n");
                 }
                break;
        case QUERYSTATUS:   //管理中心查询设备状态
                if(recv_len >= SMALLESTSIZE)
                 {
                  RecvQueryStatus_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                    xtm_printf("管理中心查询设备状态长度异常\n");
                 }
                break;
        case ALARM:   //报警
                if(recv_len >= SMALLESTSIZE)
                 {
                  RecvAlarm_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                    xtm_printf("报警应答长度异常\n");
                 }
                break;
        case CANCELALARM:   //取消报警
                if(recv_len >= SMALLESTSIZE)
                 {
                  RecvCancelAlarm_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                    xtm_printf("取消报警应答长度异常\n");
                 }
                break;
        case RESETPASS:    //复位密码
                if(recv_len >= SMALLESTSIZE)
                 {
                  RecvResetPass_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                    xtm_printf("复位密码长度异常\n");
                 }
                break;
        case SEARCHALLEQUIP:  //252  //搜索所有设备（管理中心－＞设备）
                if(recv_len >= SMALLESTSIZE)
                 {
                  RecvSearchAllEquip_Func(buff, FromIP, DestIP, DestPort, recv_len);
                 }
                else
                 {
                  xtm_printf("搜索所有设备长度异常\n");
                 }
                break;
        case WRITEEQUIPADDR:  //254  //写设备地址（管理中心－＞设备）
                if(recv_len >= SMALLESTSIZE)
                 {
                  RecvWriteEquipAddr_Func(buff, FromIP, DestIP, DestPort, recv_len);
                 }
                else
                 {
                  xtm_printf("写设备地址长度异常\n");
                 }
                break;
        case DOWNLOADFILE:   //下载
                if(recv_len >= 9)
                 {
                  //xtm_printf("DataPortDeal::1111\n");
                  RecvDownLoadFile_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                  xtm_printf("下载长度异常\n");
                 }
                break;
        case READFLASH:      //读FLASH内容
                if(recv_len >= 9)
                 {
                  //xtm_printf("DataPortDeal::1111\n");
                  RecvReadFlash_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                  xtm_printf("读FLASH内容长度异常\n");
                 }
                break;
        case REMOTERESET:      //远程重启动
                if(recv_len >= 9)
                 {
                  RecvRemoteReset_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                  xtm_printf("远程重启动长度异常\n");
                 }
                break;
        case UDP_TEST:
                RecvUdpTest_Func(buff, FromIP, DestPort, recv_len);
                break;
        case 250:
                RecvUdpTest1_Func(buff, FromIP, DestPort, recv_len);
                break;
        #ifdef _REMOTEWRITEOV7725REGISTER   //远程写OV7725寄存器  调试用
         case OV7725_OPERATE:

                 if((recv_len == 31)||(recv_len == 32))
                 {
                  //xtm_printf("1111 buff[6] = %d, recv_len = %d\n", buff[6], recv_len);
                  RecvRemoteWriteOV7725Register_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                    xtm_printf("远程读写OV7725寄存器长度异常\n");
                 }
                break;
        #endif                
        case REMOTEDEBUGINFO:      //发送远程调试信息
                 if(recv_len == 29)
                 {
                  RecvRemoteDebugInfo_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                   xtm_printf("发送远程调试信息长度异常\n");
                 }
                break;
#ifdef _REMOTECALLTEST  //远程呼叫测试
        case REMOTETEST:      //接收远程呼叫测试　
                 if(recv_len == 49)
                 {
                  RecvRemoteCallTest_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                  xtm_printf("接收远程呼叫测试长度异常\n");
                 }
                break;
#endif
        //case NSORDER:   //主机名解析（子网内广播）
        case NSSERVERORDER:  //主机名解析(NS服务器)
                switch(buff[7] & 0x03)
                 {
                  case 2://解析回应
                         if(recv_len >= SMALLESTSIZE)
                          {
                           RecvNSReply_Func(buff, FromIP, DestPort);
                          }
                         else
                          {
                            xtm_printf("解析应答数据长度异常\n");
                          }
                         break;
                 }
                break;
        #ifdef _BRUSHIDCARD_SUPPORT   //刷卡功能支持
        case WRITEIDCARD: //写ID卡信息
                //xtm_printf("WRITEIDCARD::3\n");
                RecvWriteIDCard_Func(buff, FromIP, DestPort, recv_len);
                break;
        case READIDCARD: //读ID卡信息
                RecvReadIDCard_Func(buff, FromIP, DestPort, recv_len);
                break;
        case BRUSHIDCARD: //刷卡记录
                RecvBrushIDCard_Func(buff, FromIP, DestPort, recv_len);
                break;
        #endif
        #ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
        case CAPTUREPIC_SEND_START:   //单元门口机、围墙机、二次门口机发送呼叫照片->发送开始
                if(recv_len >= 67)
                 {
                  RecvCapture_Send_Start_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                  xtm_printf("发送呼叫照片->发送开始长度异常\n");
                 }
                break;
        case CAPTUREPIC_SEND_DATA:   //单元门口机、围墙机、二次门口机发送呼叫照片->发送数据
                if(recv_len >= 58)
                 {
                  RecvCapture_Send_Data_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                  xtm_printf("发送呼叫照片->发送开始长度异常\n");
                 }
                break;
        case CAPTUREPIC_SEND_SUCC:   //单元门口机、围墙机、二次门口机发送呼叫照片->发送结束（成功）
                if(recv_len >= 42)
                 {
                  RecvCapture_Send_Succ_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                  xtm_printf("发送呼叫照片->发送开始长度异常\n");
                 }
                break;
        case CAPTUREPIC_SEND_FAIL:   //单元门口机、围墙机、二次门口机发送呼叫照片->发送结束（失败）
                if(recv_len >= 42)
                 {
                  RecvCapture_Send_Fail_Func(buff, FromIP, DestPort, recv_len);
                 }
                else
                 {
                  xtm_printf("发送呼叫照片->发送开始长度异常\n");
                 }
                break;
        #endif                      
       }
}
//---------------------------------------------------------------------------
//设备定时报告状态
void RecvReportStatus_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT8U j;
  INT8U isAddrOK;
  INT16U curr_time;

  isAddrOK = 1;
  for(j=8; j<8+Local.AddrLen; j++)
   if(LocalCfg.Addr[j-8] != recv_buf[j])
    {
     isAddrOK = 0;
     break;
    }
  //xtm_printf("RecvReportStatus_Func::isAddrOK = %d\n", isAddrOK);
  //地址匹配
  if(isAddrOK == 1)
   {
    if((recv_buf[7] & 0x03) == REPLY)   //应答
     {
      if(ClearMultiUdpBuff(REPORTSTATUS, ASK, LocalCfg.Addr[0], cFromIP))
       {
              curr_time = recv_buf[29];
              curr_time = curr_time << 8;
              curr_time += recv_buf[28];
              if(curr_time != LocalCfg.ReportTime)
               {
                LocalCfg.ReportTime = curr_time;
                WriteCfgFile();
                xtm_printf("收到设备定时报告状态应答, LocalCfg.ReportTime = %d\n", LocalCfg.ReportTime);
               }
              //xtm_printf("收到设备定时报告状态应答\n");
       }
     }
   }
}
//---------------------------------------------------------------------------
//管理中心查询设备状态
void RecvQueryStatus_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT8U j, k;
  INT8U isAddrOK;
  INT16U sendlength;
  isAddrOK = 1;
  for(j=8; j<8+Local.AddrLen; j++)
   if(LocalCfg.Addr[j-8] != recv_buf[j])
    {
     isAddrOK = 0;
     break;
    }
  //地址匹配
  if(isAddrOK == 1)
   {
         #ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
          CapturePic.ConnectCentered = 1;   //与中心连接状态
         #endif
            
         //头部
         xtm_memcopy(send_b, UdpPackageHead, 6);
         //命令
         send_b[6] = QUERYSTATUS;
         send_b[7] = REPLY;    //主叫
         xtm_memcopy(send_b + 8, LocalCfg.Addr, 20);
         xtm_memcopy(send_b + 28, LocalCfg.Mac_Addr, 6);
         send_b[34] = LocalCfg.DefenceStatus;
         send_b[35] = LocalCfg.DefenceNum;
         for(k=0; k<(LocalCfg.DefenceNum*6); k++)
           xtm_memcopy(send_b + 36 + 10*k, LocalCfg.DefenceInfo[k], 10);
         sendlength = 36 + LocalCfg.DefenceNum*6*10;
         CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);

         if(((recv_buf[29] << 8) + recv_buf[28]) != LocalCfg.ReportTime)
          {
           LocalCfg.ReportTime = (recv_buf[29] << 8) + recv_buf[28];
           WriteCfgFile();
          }
         xtm_printf("收到管理中心查询设备状态\n");
    }
}
//---------------------------------------------------------------------------
//报警
void RecvAlarm_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT8U j;
  INT8U isAddrOK;
  unsigned char AlarmByte;
  unsigned char tmp_p;

  isAddrOK = 1;
  for(j=8; j<8+Local.AddrLen; j++)
   if(LocalCfg.Addr[j-8] != recv_buf[j])
    {
     isAddrOK = 0;
     break;
    }
  //地址匹配
  if(isAddrOK == 1)
   {
    if((recv_buf[7] & 0x03) == REPLY)   //应答
     if(ClearMultiUdpBuff(ALARM, ASK, LocalCfg.Addr[0], cFromIP))
       {
              AlarmByte = (recv_buf[37] & 0x3F);
              tmp_p = 0x01;
              for(j=0; j<6; j++)
               {
                if((AlarmByte & tmp_p) == tmp_p)
                  LocalCfg.DefenceInfo[j][3] = 2; //报警已接收
                tmp_p = (tmp_p << 1);
               }
               xtm_printf("收到报警应答\n");
       }
   }
}
//---------------------------------------------------------------------------
//取消报警
void RecvCancelAlarm_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT8U j;
  INT8U isAddrOK;

  isAddrOK = 1;
  for(j=8; j<8+Local.AddrLen; j++)
   if(LocalCfg.Addr[j-8] != recv_buf[j])
    {
     isAddrOK = 0;
     break;
    }
  //地址匹配
  if(isAddrOK == 1)
   {
    if((recv_buf[7] & 0x03) == REPLY)   //应答
     if(ClearMultiUdpBuff(CANCELALARM, ASK, LocalCfg.Addr[0], cFromIP))
       {
        xtm_printf("收到取消报警应答\n");
       }
   }
}
//---------------------------------------------------------------------------
void RecvResetPass_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT8U j;
  INT8U isAddrOK;
  INT16U sendlength;

  isAddrOK = 1;
  for(j=8; j<8+Local.AddrLen; j++)
   if(LocalCfg.Addr[j-8] != recv_buf[j])
    {
     isAddrOK = 0;
     break;
    }
  //地址匹配
  if(isAddrOK == 1)
   {
    if((recv_buf[7] & 0x03) == ASK)   //应答
     {
      xtm_memcopy(send_b, recv_buf, length);
      send_b[7] = REPLY;    //应答
      sendlength = length;
      CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);

      //复位密码
      switch(recv_buf[28])
       {
        case 1: //复位工程密码
               xtm_printf("复位工程密码成功\n");
               if(strcmp(LocalCfg.EngineerPass, "888888") != 0)
                {
                 strcpy(LocalCfg.EngineerPass, "888888");
                 WriteCfgFile();     //向Flash中存储文件
                }
               break;
        case 2: //复位开锁密码
               xtm_printf("复位开锁密码成功\n");
               if(strcmp(LocalCfg.OpenLockPass, "123456") != 0)
                {
                 strcpy(LocalCfg.OpenLockPass, "123456");
                 WriteCfgFile();     //向Flash中存储文件
                }
               break;
     }
     }
   }
}
//---------------------------------------------------------------------------
//INT8U dma_mode = 1;
INT16U  OldTestPackNo = 0;
//收发测试
void RecvUdpTest_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{

  INT16U PackNo;
  /*INT8U temp, temp1;

  temp = 0;
  for(i=11; i<(length); i++)
   {
    if(temp == 0xff)
      temp1 = 0;
    else
      temp1 = temp + 1;
    if(recv_buf[i] != temp1)
      xtm_printf("RecvUdpTest_Func:: i = %d, recv_buf[i] = 0x%X, temp = 0x%X\n", i, recv_buf[i], temp);
    temp = recv_buf[i];
   }      */

  //xtm_printf("RecvUdpTest_Func:: %d, %d, %d\n", length, recv_buf[8], recv_buf[9]);
  PackNo = recv_buf[8];
  PackNo = PackNo << 8;
  PackNo += recv_buf[9];
  if((OldTestPackNo + 1) != PackNo)
    xtm_printf("OldTestPackNo = %d, PackNo = %d\n", OldTestPackNo, PackNo);
  OldTestPackNo = PackNo;
  xtm_printf("%d, %d\n", recv_buf[8], recv_buf[9]);
  //xtm_printf("0x%X\n", inportb(BITBLK_STATUS_REG));

  Send_Stream = (INT32U)recv_buf - 0x0a;

  Send_Stream[7+0x0a] = REPLY;    //应答
  //sendlength = length;
 #if 1
  CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, Send_Stream+0x0a, length, 0);
 #else
  recv_buf[7] = REPLY;
  if(SearchArp_Table(cFromIP, RemoteMac))
    UdpSendBuff_1(DestPort, RemoteMac, cFromIP, recv_buf, length);
 #endif
  //xtm_printf("2222\n");
}
//---------------------------------------------------------------------------
#define MAXTESTPACKNUM  28
//连续包接收测试
void RecvUdpTest1_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT16U PackNo;
  INT16U CurrNo;
  INT8U j, PackOK, LostPackNo[20], LostPackNum;;

  //xtm_printf("RecvUdpTest_Func:: %d, %d, %d\n", length, recv_buf[8], recv_buf[9]);
  PackNo = recv_buf[8];
  PackNo = PackNo << 8;
  PackNo += recv_buf[9];
  if(OldTestPackNo != PackNo)
  //  xtm_printf("OldTestPackNo = %d, PackNo = %d\n", OldTestPackNo, PackNo);
     {
      for(j=0; j<MAX_VIDEO_PACK; j++)
       {
        PackExist[0][j] = 0;
        PackFrameno[0][j] = 0;
       }
     }
  CurrNo =  recv_buf[11];
  OldTestPackNo = PackNo;
 // xtm_printf("%d, %d, %d\n", recv_buf[8], recv_buf[9], CurrNo);
#if 1
  if(PackExist[0][CurrNo] == 1)
    xtm_printf("multi recv %d, %d, %d\n", recv_buf[8], recv_buf[9], CurrNo);
  PackExist[0][CurrNo] = 1;
  if(CurrNo == (MAXTESTPACKNUM - 1))
   {
      //xtm_printf("%d, %d, %d\n", recv_buf[8], recv_buf[9], CurrNo);
      PackOK = 1;
      LostPackNo[0] = 100;
      LostPackNum = 0;
      for(j=0; j<MAXTESTPACKNUM; j++)
       if(PackExist[0][j] == 0)
        {
         PackOK = 0;
         LostPackNo[LostPackNum] = j;
         LostPackNum ++;
        // break;
        }
      for(j=0; j<MAX_VIDEO_PACK; j++)
       {
        PackExist[0][j] = 0;
        PackFrameno[0][j] = 0;
       }
      if(PackOK == 0)
       {
        for(j=0; j<LostPackNum; j++)
          xtm_printf("%d, %d, LostPackNo[%d] = %d\n", recv_buf[8], recv_buf[9], j, LostPackNo[j]);
        return;
       }
   }
#endif   

}
//---------------------------------------------------------------------------
#ifdef _REMOTEWRITEOV7725REGISTER   //远程写OV7725寄存器  调试用
void RecvRemoteWriteOV7725Register_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT8U val;
  INT8U send_b[128];
  INT16U sendlength;
  char FlagText[50];
  //unsigned char usb_order[10];
  xtm_memcopy(FlagText, recv_buf + 9, 20);
  if((FlagText[0] == FLAGTEXT[0])&&(FlagText[1] == FLAGTEXT[1])&&(FlagText[2] == FLAGTEXT[2])
     &&(FlagText[3] == FLAGTEXT[3])&&(FlagText[4] == FLAGTEXT[4])&&(FlagText[5] == FLAGTEXT[5]))
   {
    switch(recv_buf[8])
     {
      case OV7725READREGISTER:       //读
             xtm_printf("\nread register 0x%X\n", recv_buf[29]);
             val = Read7725(recv_buf[29]);
             xtm_printf("reg = 0x%X, val = 0x%X\n", recv_buf[29], val);

             xtm_memcopy(send_b, recv_buf, length);
             send_b[31] = val;
             send_b[7] = REPLY;    //应答
             sendlength = 32;
             CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
             break;
      case OV7725WRITEREGISTER:       //写
             xtm_printf("\nread register 0x%X\n", recv_buf[29]);
             Write7725(recv_buf[29], recv_buf[31]);

             xtm_memcopy(send_b, recv_buf, length);
             send_b[7] = REPLY;    //应答
             sendlength = length;
             CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
             break;
     }
   }
}
//---------------------------------------------------------------------------
#endif
//搜索所有设备应答
void RecvSearchAllEquip_Func(INT8U *recv_buf, INT8U *cFromIP, INT8U *DestIP, INT16U DestPort, INT16U length)
{
  INT8U isAddrOK, i;
  INT16U sendlength;
  char tmp_text[20];  

  isAddrOK = 1;
  xtm_printf("RecvSearchAllEquip_Func1\n");
  if(recv_buf[7] == REPLY)
    return;
  #if 0
   for(j=8; j<8+Local.AddrLen; j++)
    if(LocalCfg.Addr[j-8] != recv_buf[j])
     {
      isAddrOK = 0;
      break;
     }
  #endif
  if((DestIP[0] == 255)&&(DestIP[1] == 255)&&(DestIP[2] == 255)&&(DestIP[3] == 255))
   {
    cFromIP[0] = 255;
    cFromIP[1] = 255;
    cFromIP[2] = 255;
    cFromIP[3] = 255;
   }
    
  //地址匹配
  if(isAddrOK == 1)
   {
    xtm_printf("RecvSearchAllEquip_Func2\n");

    xtm_memcopy(send_b, recv_buf, length);
    send_b[7] = REPLY;    //应答

    send_b[8] = 0x05;
    send_b[9] = 0;

    //地址编码
    xtm_memcopy(send_b + 10, LocalCfg.Addr, 20);
    //网卡地址
    xtm_memcopy(send_b + 30, LocalCfg.Mac_Addr, 6);
    //IP地址
    xtm_memcopy(send_b + 36, LocalCfg.IP, 4);
    //子网掩码
    xtm_memcopy(send_b + 40, LocalCfg.IP_Mask, 4);
    //网关地址
    xtm_memcopy(send_b + 44, LocalCfg.IP_Gate, 4);
    //服务器地址
    xtm_memcopy(send_b + 48, LocalCfg.IP_Server, 4);
    //版本号
    for(i=0; i<20; i++)
      tmp_text[i] = '\0';
   #ifdef _SEND_TO_SELF_TEST    //发送数据包给自己
    xtm_sprintf(tmp_text, "%s-%04X-%04X\0", SERIALNUM, LocalCfg.BootNo, LocalCfg.SelfBootNo);
   #else
    xtm_sprintf(tmp_text, "%s-%04X\0", SERIALNUM, LocalCfg.BootNo);
   #endif
    xtm_memcopy(send_b + 52, tmp_text, 16);
    //多分机支持
    send_b[68] = LocalCfg.MultiMachineSupport;
    send_b[69] = LocalCfg.DenMaxNum;  //副机最大数量，这一数量需小于 SUBMAXNUM， 有利于减小广播包
    xtm_memcopy(send_b + 70, &LocalCfg.EncodeErrorNum, sizeof(INT32U));  //编码错误数目
    send_b[74] = LocalCfg.Language;

    send_b[75] = LocalCfg.PassOpenLock;   //密码开锁
    send_b[76] = LocalCfg.CardOpenLock;   //刷卡开锁
    send_b[77] = LocalCfg.DoorButton;   //出门按钮
    send_b[78] = LocalCfg.DoorDetect;       //门磁检测
    send_b[79] = LocalCfg.OpenLockTime;   //开锁时间  0 200ms  1 500ms  2 5s  3 10s
    send_b[80] = LocalCfg.DelayLockTime;   //延时开锁  0 0s  1 3s  2 5s  3 10s
    send_b[82] = LocalCfg.EncodeFrameRate;   //门口机帧率

    send_b[90] = LocalCfg.MicDecValue;  //展箱、展板模式,需减小 Mic音量
    sendlength = 91;
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
  }
}
//---------------------------------------------------------------------------
//写设备地址应答
void RecvWriteEquipAddr_Func(INT8U *recv_buf, INT8U *cFromIP, INT8U *DestIP, INT16U DestPort, INT16U length)
{
  INT8U j, isAddrOK, OldLanguage;
  INT16U sendlength;

  if(recv_buf[7] == REPLY)
    return;

  xtm_printf("RecvWriteEquipAddr_Func::length = %d\n", length);

  isAddrOK = 1;
  #if 1
   for(j=10; j<10+Local.AddrLen; j++)
    if(LocalCfg.Addr[j-10] != recv_buf[j])
     {
      isAddrOK = 0;
      break;
     }

   for(j=30; j<30+6; j++)
    if(LocalCfg.Mac_Addr[j-30] != recv_buf[j])
     {
      isAddrOK = 0;
      break;
     }

   for(j=36; j<36+4; j++)
    if(LocalCfg.IP[j-36] != recv_buf[j])
     {
      isAddrOK = 0;
      break;
     }
  #endif

  if((DestIP[0] == 255)&&(DestIP[1] == 255)&&(DestIP[2] == 255)&&(DestIP[3] == 255))
   {
    cFromIP[0] = 255;
    cFromIP[1] = 255;
    cFromIP[2] = 255;
    cFromIP[3] = 255;
   }
  //地址匹配
  if(isAddrOK == 1)
   {
    xtm_memcopy(send_b, recv_buf, length);
    send_b[7] = REPLY;    //应答
    sendlength = length;
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);

    //设备定时报告状态时间
    LocalCfg.ReportTime = 10;
        
    //写地址设置
    xtm_memcopy(LocalCfg.Addr, recv_buf + 52, 20);
    xtm_memcopy(LocalCfg.Mac_Addr, recv_buf + 72, 6);
    xtm_memcopy(LocalCfg.IP, recv_buf + 78, 4);
    xtm_memcopy(LocalCfg.IP_Mask, recv_buf + 82, 4);
    xtm_memcopy(LocalCfg.IP_Gate, recv_buf + 86, 4);
    xtm_memcopy(LocalCfg.IP_Server, recv_buf + 90, 4);
    LocalCfg.MultiMachineSupport = recv_buf[94];
    LocalCfg.DenMaxNum = recv_buf[95];  //副机最大数量，这一数量需小于 SUBMAXNUM， 有利于减小广播包
    OldLanguage = LocalCfg.Language;
    LocalCfg.Language = recv_buf[96];
    if(LocalCfg.Language > 1)
      LocalCfg.Language = 0;

    LocalCfg.PassOpenLock = recv_buf[97];  //密码开锁
    if(LocalCfg.PassOpenLock > 1)
      LocalCfg.PassOpenLock = 0;

    LocalCfg.CardOpenLock = recv_buf[98];   //刷卡开锁
    if(LocalCfg.CardOpenLock > 1)
      LocalCfg.CardOpenLock = 0;

    LocalCfg.DoorButton = recv_buf[99];   //出门按钮
    if(LocalCfg.DoorButton > 1)
      LocalCfg.DoorButton = 0;

    LocalCfg.DoorDetect = recv_buf[100];       //门磁检测
    if(LocalCfg.DoorDetect > 1)
      LocalCfg.DoorDetect = 0;

    LocalCfg.OpenLockTime = recv_buf[101];   //开锁时间  0 500ms  1 3s  2 5s
    if(LocalCfg.OpenLockTime > 2)
      LocalCfg.OpenLockTime = 0;

    LocalCfg.DelayLockTime = recv_buf[102];   //延时开锁  0 0s  1 3s  2 5s  3 10s
    if(LocalCfg.DelayLockTime > 3)
      LocalCfg.DelayLockTime = 0;

    LocalCfg.EncodeFrameRate = recv_buf[104];   //门口机帧率  1, 5, 10, 15, 20, 25
    if(LocalCfg.EncodeFrameRate > 5)
      LocalCfg.EncodeFrameRate = 3;

    LocalCfg.MicDecValue = recv_buf[112];  //展箱、展板模式,需减小 Mic音量
    if(LocalCfg.MicDecValue > 3) //展箱、展板模式,需减小 Mic音量
      LocalCfg.MicDecValue = 0;
    SetMicVolume();

    if((LocalCfg.Addr[0] != 'M')&&(LocalCfg.Addr[0] != 'W'))
      LocalCfg.Addr[0] = 'M';
    if(LocalCfg.Addr[0] == 'M')
     {
      Local.AddrLen = 8;  //地址长度  S 12  B 12 M 8 H 6
     }
    if(LocalCfg.Addr[0] == 'W')
     {
      Local.AddrLen = 5;  //地址长度  S 12  B 12 M 8 H 6
     }
    WriteCfgFile();  //写本地设置文件
    //Init_LocalVar();    //此处可能引起写失败
    mac_init(); //刷新网络设置

    if(LocalCfg.Language != OldLanguage)
     if(Local.CurrentWindow == 0)
       DisplayTalkWindow(REFRESH);       
   }
}
//---------------------------------------------------------------------------
INT16U OldPackage = 0;
//下载
void RecvDownLoadFile_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT16U PackDataLen = 1024;//256;
  INT16U sendlength, i;
  INT8U DownOK;
  INT16U RemotePort1;
  INT8U RemoteIP1[4];

  struct downfile1 DownData;

  xtm_memcopy(&DownData, recv_buf + 10, sizeof(struct downfile1));
  if((DownData.FlagText[0] == FLAGTEXT[0])&&(DownData.FlagText[1] == FLAGTEXT[1])&&(DownData.FlagText[2] == FLAGTEXT[2])
     &&(DownData.FlagText[3] == FLAGTEXT[3])&&(DownData.FlagText[4] == FLAGTEXT[4])&&(DownData.FlagText[5] == FLAGTEXT[5]))
   {
    //xtm_printf("RecvDownLoadFile_Func::recv_buf[8] = 0x%X\n", recv_buf[8]);
    switch(recv_buf[8])
     {
      case STARTDOWN:

                     if((Local.Status == 1)||(Local.Status == 2)||(Local.Status == 5)||(Local.Status == 6)
                          ||(Local.Status == 7)||(Local.Status == 8)||(Local.Status == 9)||(Local.Status == 10))
                        TalkEnd_Func();
                     if(Local.Status == 3|| Local.Status == 90)  //状态为监视
                        WatchEnd_Func();

                     xtm_printf("RecvDownLoadFile_Func::  STARTDOWN  length = %d\n", length);
                     for(i=0; i<MAX_DOWN_PACK; i++)
                       Down_RecvFlag[i] = 0;           //下载 包标志
                     xtm_memcopy(send_b, recv_buf, length);
                     send_b[7] = REPLY;    //应答
                     sendlength = length;
                     CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
                     break;
      case DOWN:              //下载
                     //xtm_printf("0x%X, 0x%X, 0x%X, 0x%X\n", recv_buf[29], recv_buf[30], recv_buf[31], recv_buf[32]);

                     //xtm_printf("0x%X, 0x%X, 0x%X, 0x%X\n", recv_buf[33], recv_buf[34], recv_buf[35], recv_buf[36]);

                     //xtm_printf("0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", recv_buf[37], recv_buf[38], recv_buf[39], recv_buf[40], recv_buf[41], recv_buf[42]);
                     //xtm_printf("0x%X, 0x%X, 0x%X, 0x%X\n", recv_buf[43], recv_buf[44], recv_buf[45], recv_buf[46]);
                    // xtm_printf("0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", recv_buf[504+43], recv_buf[505+43], recv_buf[506+43], recv_buf[507+43], recv_buf[508+43], recv_buf[509+43], recv_buf[510+43], recv_buf[511+43]);


                     //xtm_printf("下载  FileLen = 0x%X, TotalPackage = %d, CurrPackage = %d\n", DownData.Filelen, DownData.TotalPackage, DownData.CurrPackage);

                     //xtm_printf("下载 Faddr = 0x%X%X , Filelen = 0x%X%X, Flen = 0x%X%X, FileType = %d, TotalPackage = %d, CurrPackage = %d, Width = %d, Height = %d, xLeft = %d, yTop = %d, Datalen = %d\n", \
                     //           (INT16U)((DownData.Faddr & 0xffff0000) >> 16), (INT16U)(DownData.Faddr & 0xffff), \
                     //           (INT16U)((DownData.Filelen & 0xffff0000) >> 16), (INT16U)(DownData.Filelen & 0xffff), \
                     //           (INT16U)((DownData.FLen & 0xffff0000) >> 16), (INT16U)(DownData.FLen & 0xffff), \
                     //           DownData.FileType, DownData.TotalPackage, DownData.CurrPackage, DownData.Width, DownData.Height, DownData.xLeft, DownData.yTop, DownData.Datalen);

                     if(DownData.TotalPackage > MAX_DOWN_PACK)
                      {
                       xtm_printf("DownData.TotalPackage = %d, too large\n", DownData.TotalPackage);
                       break;
                      }
                     Down_RecvFlag[DownData.CurrPackage] = 1;

                    #ifdef _PACK_1024
                     outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
                     outportb(MEMORY_WINDOW_BASE_H, 0x60 + DownData.CurrPackage/64);
                     xtm_memcopy(Memory6_Stream + DownData.CurrPackage * 1024, recv_buf + 10 + sizeof(struct downfile1), DownData.Datalen);
                     outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);
                    #endif

  //xtm_printf(" Down_Stream11 = 0x%X %X, 0x%X %X %X %X\n",  (INT16U)(((INT32U)( Down_Stream[0] + DownData.CurrPackage * 1024) & 0xffff0000) >> 16), \
  //           (INT16U)( (INT32U)(Down_Stream[0] + DownData.CurrPackage * 1024) & 0xffff), Down_Stream[0][0], Down_Stream[0][1], Down_Stream[0][2], Down_Stream[0][3]);

                     xtm_printf("RecvDownLoadFile_Func::length = %d, DownData.TotalPackage = %d, DownData.CurrPackage = %d\n", length, DownData.TotalPackage, DownData.CurrPackage);
                     DownOK = 1;
                     //xtm_printf("DestIP2 = %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
                     RemoteIP1[0] = cFromIP[0];
                     RemoteIP1[1] = cFromIP[1];
                     RemoteIP1[2] = cFromIP[2];
                     RemoteIP1[3] = cFromIP[3];
                     RemotePort1 = DestPort;
                     xtm_memcopy(send_b, recv_buf, length);
                     if(DownData.CurrPackage == (DownData.TotalPackage - 1))
                      {
                       xtm_printf("WriteAllToFlash start\n");
                       DownOK = WriteAllToFlash(recv_buf, &DownData);
                       xtm_printf("WriteAllToFlash finish\n");
                      }
                     if(DownOK)
                      {
                       send_b[7] = REPLY;    //应答
                       sendlength = length;
                       CreateMultiUdpBuff(RemotePort1, SINGLE_SEND, RemoteIP1, send_b, sendlength, 0);
                      }
                     else
                      {
                       send_b[7] = ASK;    //应答
                       send_b[8] = DOWNFAIL;
                       sendlength = length;
                       CreateMultiUdpBuff(RemotePort1, SINGLE_SEND, RemoteIP1, send_b, sendlength, 0);
                       xtm_printf("send DOWNFAIL\n");
                      }

                     break;
      case DOWNFINISHONE:         //下载完成一个
                     xtm_memcopy(send_b, recv_buf, length);
                     send_b[7] = REPLY;    //应答
                     sendlength = length;
                     CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);

                     xtm_printf("下载完成一个\n");
                     break;
      case STOPDOWN:             //停止下载
                     xtm_memcopy(send_b, recv_buf, length);
                     send_b[7] = REPLY;    //应答
                     sendlength = length;
                     CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);

                     xtm_printf("停止下载\n");
                     break;
     }
   }
}
//---------------------------------------------------------------------------
INT8U WriteAllToFlash(INT8U *recv_buf, struct downfile1 *DownData)
{
  INT16U i, j, k, PackOK;
  INT32U paddr, offset;
  //INT8U send_b[1600];
  INT16U sendlength;
  INT8U m_crc, m_crc1;
  INT32U file_len;

  PackOK = 1;
  for(i=0; i<DownData->TotalPackage; i++)
   if(Down_RecvFlag[i] == 0)
    {
      PackOK = 0;
      xtm_printf("WriteAllToFlash::PackOK = %d, i = %d\n", PackOK, i);

      break;
    }
  if(PackOK)
   {
    //check crc
    if(DownData->FileType == 0)
     {
      m_crc = recv_buf[9];
      m_crc1 = 0;
      file_len = 0;
     #ifdef _PACK_1024
      for(i=0; i<DownData->TotalPackage; i++)
       for(j=0; j<4; j++)
        {
         ClearWatchDog();
         offset = ((i*4 + j)%1024)*256;

         outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
         outportb(MEMORY_WINDOW_BASE_H, 0x60 + i/64);

         for(k=0; k<256; k++)
          {
           if(file_len < DownData->Filelen)
            {
             m_crc1 += Memory6_Stream[offset + k];
             file_len ++;
            }
          }
         outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);
        }
     #endif
       xtm_printf("m_crc = 0x%X, m_crc1 = 0x%X\n", m_crc, m_crc1);
       if(m_crc != m_crc1)
        {
         PackOK = 0;
         xtm_printf("WriteAllToFlash::crc error\n");
        }
     }
    }

  if(PackOK)
   {
    paddr = DownData->Faddr;
    if((DownData->FileType == 1)||(DownData->FileType == 2))   //图片  声音
     {
      CheckFlashSecterErase(paddr);
      w25x32writeenable();
      programpage25x32(paddr,  recv_buf + 30,  256);
      w25x32writedisable();
  //xtm_printf("paddr = 0x%X%X, 0x%X  0x%X  0x%X  0x%X \n", (INT16U)((paddr & 0xffff0000) >> 16), \
  //           (INT16U)(paddr & 0xffff), recv_buf[30], recv_buf[31], recv_buf[32], recv_buf[33]);
      paddr += 256;
     }
   #ifdef _PACK_1024
    for(i=0; i<DownData->TotalPackage; i++)
     for(j=0; j<4; j++)
     {
      CheckFlashSecterErase(paddr);
      w25x32writeenable();

      offset = ((i*4 + j)%1024)*256;

      if((DownData->FileType == 1)||(DownData->FileType == 2))   //图片  声音
       if(offset > DownData->FLen)
         break;
      outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
      outportb(MEMORY_WINDOW_BASE_H, 0x60 + i/64);
      programpage25x32(paddr, Memory6_Stream + offset, 256);
      outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);

      w25x32writedisable();

      //xtm_printf("paddr = 0x%X %X, TotalPackage = %d, i = %d, j = %d, no = %d, offset = 0x%X %X,\n", (INT16U)((paddr & 0xffff0000) >> 16), \
      //       (INT16U)(paddr & 0xffff), DownData->TotalPackage, i, j, (i*4)/256, (INT16U)((offset & 0xffff0000) >> 16), \
      //       (INT16U)(offset & 0xffff));
      //xtm_printf("11 0x%X, 0x%X, 0x%X, 0x%X\n", Down_Stream[0][offset+0], Down_Stream[0][offset+1], Down_Stream[0][offset+2], Down_Stream[0][offset+3]);
  //xtm_printf("paddr = 0x%X %X, TotalPackage = %d, i = %d, j = %d, no = %d, offset = 0x%X %X, Down_Stream = 0x%X %X, DownData->FLen = 0x%X\n", (INT16U)((paddr & 0xffff0000) >> 16), \
  //           (INT16U)(paddr & 0xffff), DownData->TotalPackage, i, j, (i*4)/256, (INT16U)((offset & 0xffff0000) >> 16), \
  //           (INT16U)(offset & 0xffff), (INT16U)(((INT32U)( Down_Stream[(i*4)/256] + offset) & 0xffff0000) >> 16), \
  //           (INT16U)( (INT32U)(Down_Stream[(i*4)/256] + offset) & 0xffff), DownData->FLen);
      paddr += 256;
    }
  #endif
   }

  for(i=0; i<MAX_DOWN_PACK; i++)
    Down_RecvFlag[i] = 0;           //下载 包标志
  if(PackOK)
    return 1;
  else
    return 0;
}
//---------------------------------------------------------------------------
void WriteFlash(INT8U WriteType, INT8U *recv_buf, struct downfile1 *DownData)      //WriteType  0 -- 文件头   1 -- 数据
{
  INT8U i;
  INT32U  temp1;
  INT32U paddr;

  if(WriteType == 1)
   if(DownData->Faddr != 0)
    //#ifdef _PACK_256
     DownData->Faddr += 256;
    //#endif
    //#ifdef _PACK_512
    // DownData->Faddr += 512;
    //#endif
  temp1 = DownData->CurrPackage;   

  #ifdef _PACK_1024
    DownData->Faddr += (temp1 << 10);     //每包 1024 byte
  #endif
  //xtm_printf("%d, Faddr = 0x%X%X\n", DownData->CurrPackage, (INT16U)((DownData->Faddr & 0xffff0000) >> 16), \
  //           (INT16U)(DownData->Faddr & 0xffff));
  if((OldPackage + 1) != DownData->CurrPackage)
    xtm_printf("OldPackage = %d, CurrPackage = %d\n", OldPackage, DownData->CurrPackage);
                     //if(TotalPackage != 1024)
                     //  xtm_printf("TotalPackage = %d......................\n", TotalPackage);
  OldPackage = DownData->CurrPackage;
  #if 1
   #ifdef _PACK_1024

    if(WriteType == 0)   //文件头
     {
      CheckFlashSecterErase(DownData->Faddr);
      w25x32writeenable();
      programpage25x32(DownData->Faddr, recv_buf + 30, 256);
      w25x32writedisable();
     }
    else
     for(i=0; i<4; i++)
      {
       paddr = DownData->Faddr + 256 * i;
       CheckFlashSecterErase(paddr);
       w25x32writeenable();
       programpage25x32(paddr, recv_buf + 10 + sizeof(struct downfile1) + 256 * i, 256);

       w25x32writedisable();
       //if(inportb(SPSR) != 0x85)
       //  xtm_printf("inportb(SPSR) = 0x%X\n", (INT16U)(inportb(SPSR)));
       //xtm_printf("DownData->Faddr = 0x%X%X\n", (INT16U)((paddr & 0xffff0000) >> 16), \
       //         (INT16U)(paddr & 0xffff));
      }
   #endif
  #endif
}
//---------------------------------------------------------------------------
//检查是否需要删除Secter 4096 byte
void CheckFlashSecterErase(INT32U paddr)
{
  if ((paddr%4096) == 0)
   {
    w25x32writeenable();
    sectererase25x32(paddr);
    w25x32writedisable();
   }
}
//---------------------------------------------------------------------------
//读Flash内容
void RecvReadFlash_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT16U sendlength;
  INT32U tmp_addr;
  INT16U ReadLen;

  struct downfile1 DownData;

  xtm_memcopy(&DownData, recv_buf + 10, sizeof(struct downfile1));
//  xtm_memcopy(&tmp_addr, recv_buf + 8, 4);
//  xtm_memcopy(&ReadLen, recv_buf + 12, 2);

  //xtm_printf("0x%X, 0x%X, 0x%X, 0x%X\n", recv_buf[29], recv_buf[30], recv_buf[31], recv_buf[32]);
  xtm_printf("RecvReadFlash_Func::DownData.Faddr = 0x%X %X, DownData.FLen = 0x%X %X, DownData.CurrPackage = 0x%X\n", \
                (INT16U)((DownData.Faddr & 0xffff0000) >> 16), (INT16U)(DownData.Faddr & 0xffff), \
                (INT16U)((DownData.FLen & 0xffff0000) >> 16), (INT16U)(DownData.FLen & 0xffff), \
                DownData.CurrPackage);
  ReadLen = 1024;
  if((DownData.FlagText[0] == FLAGTEXT[0])&&(DownData.FlagText[1] == FLAGTEXT[1])&&(DownData.FlagText[2] == FLAGTEXT[2])
     &&(DownData.FlagText[3] == FLAGTEXT[3])&&(DownData.FlagText[4] == FLAGTEXT[4])&&(DownData.FlagText[5] == FLAGTEXT[5]))
   {
    xtm_memcopy(send_b, recv_buf, length);
    send_b[7] = REPLY;    //应答
    sendlength = 10 + sizeof(struct downfile1) + ReadLen;
    ReadFromFlash(DownData.Faddr + (INT32U)(DownData.CurrPackage) * ReadLen, send_b + 10 + sizeof(struct downfile1), ReadLen);
    //ReadFromFlash(DownData.Faddr + DownData.CurrPackage * ReadLen, send_b + 10 + sizeof(struct downfile1), ReadLen);
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
   }

}
//---------------------------------------------------------------------------
//远程重启动
void RecvRemoteReset_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT16U sendlength;
  INT8U RemoteMac[6];
  xtm_memcopy(send_b, recv_buf, length);
  send_b[7] = REPLY;    //应答
  sendlength = length;
  if(SearchArp_Table(cFromIP, RemoteMac))
    UdpSendBuff_1(DestPort, RemoteMac, cFromIP, send_b, sendlength);
  xtm_printf("DataPortDeal::远程重启动, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
  Local.GpioWatchDogStarted = 0;  //在GPIO检测线程中启动WatchDog
}
//---------------------------------------------------------------------------
#ifdef _BRUSHIDCARD_SUPPORT   //刷卡功能支持
INT16U Card_Count = 0;
//写ID卡信息
void RecvWriteIDCard_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{

  INT16U i,j;
  INT8U isAddrOK;
  INT16U sendlength;
  INT32U CardPos;
  INT8U RamNo;
  INT16U tmp;
  INT8U RemoteMac[6];
  
  INT8U RecvOk;
  //读写ID卡数据结构
  struct iddata1 iddata;

  INT8U start_pos, end_pos, convert_pos;
  INT16U card_left;
  INT8U *tmp_SN, *tmp_recv;
  INT8U k;
  INT8U recv_addr[20];

  // xtm_printf("RecvWriteIDCard_Func:1 %d\n", Card_Count);
  i = 0;
  isAddrOK = 1;
  for(j=8; j<(8+Local.AddrLen); j++)
   if(LocalCfg.Addr[j-8] != recv_buf[j])
    {
     isAddrOK = 0;
     break;
    }
  if(isAddrOK == 0)
   {
    xtm_memcopy(recv_addr, recv_buf + 8, Local.AddrLen);
    recv_addr[Local.AddrLen] = '\0';
    xtm_printf("RecvWriteIDCard_Func::isAddrOK = %d, LocalCfg.Addr = %s, recv_addr = %s\n", isAddrOK, LocalCfg.Addr, recv_addr);
   } 

 // ClearWatchDog();


  //地址匹配
  if(isAddrOK == 1)
   {
    xtm_memcopy(send_b, recv_buf, length);
    send_b[7] = REPLY;    //应答
    sendlength = length;
   #if 1
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
   // xtm_printf("RecvWriteIDCard_Func:1 %d\n", Card_Count);
   #else
    if(SearchArp_Table(cFromIP, RemoteMac))
     {
      //xtm_printf("UdpSendBuff_1.......................\n");

      UdpSendBuff_1(DestPort, RemoteMac, cFromIP, send_b, sendlength);
      xtm_printf("RecvWriteIDCard_Func:1 %d\n", Card_Count);
     } 
   #endif

  #if 1
    xtm_memcopy(&iddata, recv_buf + 8, sizeof(iddata));

    if(iddata.Num > IDCARDMAXNUM)
     {
      xtm_printf("RecvWriteIDCard_Func::iddata.Num = 0x%X %X\n", (INT16U)((iddata.Num & 0xffff0000) >> 16), (INT16U)((iddata.Num & 0xffff)));
      return;
     }
    if(iddata.serialnum == IDCardNo.serialnum)
      xtm_printf("RecvWriteIDCard_Func::IDCardNo.serialnum = iddata.serialnum = 0x%X %X\n", (INT16U)((iddata.serialnum & 0xffff0000) >> 16), (INT16U)((iddata.serialnum & 0xffff)));
    //比较占系统时间
    /*xtm_printf("RecvIDCardNo.isNewWriteFlag = %d, iddata.serialnum = 0x%X %X, IDCardNo.serialnum = 0x%X %X, iddata.TotalPackage = 0x%X, iddata.CurrPackage = 0x%X\n", \
            RecvIDCardNo.isNewWriteFlag, (INT16U)((iddata.serialnum & 0xffff0000) >> 16), (INT16U)(iddata.serialnum & 0xffff), \
            (INT16U)((IDCardNo.serialnum & 0xffff0000) >> 16), (INT16U)(IDCardNo.serialnum & 0xffff), \
            (INT16U)(iddata.TotalPackage & 0xffff), (INT16U)(iddata.CurrPackage & 0xffff));*/

    //查看是否在写ID卡信息状态
    if(RecvIDCardNo.isNewWriteFlag == 1)
     {
     #if 1
       //上次写未完成，新开始写
       if(iddata.serialnum != RecvIDCardNo.serialnum)
        {
         RecvIDCardNo.isNewWriteFlag = 1;
         //卡数量
         RecvIDCardNo.Num = iddata.Num;
         //总包数
         RecvIDCardNo.PackNum = (INT16U)iddata.TotalPackage;
         //包已接收标志
         for(i=0; i<RecvIDCardNo.PackNum; i++)
           RecvIDCardNo.PackIsRecved[i] = 0;
         //序号
         RecvIDCardNo.serialnum = iddata.serialnum;
        }
       //当前数据包卡数量
       if(iddata.CurrNum > CARDPERPACK)
         iddata.CurrNum = CARDPERPACK;
       //当前包
       if(iddata.CurrPackage > RecvIDCardNo.PackNum)
         iddata.CurrPackage = RecvIDCardNo.PackNum;
       RecvIDCardNo.PackIsRecved[iddata.CurrPackage-1] = 1;
     #if 0
       for(i=0; i<iddata.CurrNum; i++)
        {
           CardPos = CARDPERPACK*(iddata.CurrPackage-1)*BYTEPERSN_NEW + BYTEPERSN_NEW*i;
           outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
           outportb(MEMORY_WINDOW_BASE_H, 0x68 + (CardPos >> 16) & 0xffff);
           xtm_memcopy(RecvIDCardNo.SN + (CardPos & 0xffff), recv_buf + 48 + i*BYTEPERSN, BYTEPERSN_NEW);
           outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);
        }
     #else
        CardPos = CARDPERPACK*(iddata.CurrPackage-1)*BYTEPERSN_NEW;
        outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
        tmp = (INT16U)iddata.CurrNum;

        start_pos = (INT16U)((CardPos & 0xffff0000) >> 16);
        end_pos = (INT16U)(((CardPos + tmp*BYTEPERSN_NEW) & 0xffff0000) >> 16);
        convert_pos = CARDPERPACK + 1;
        if(start_pos != end_pos)
         {
          for(i=0; i<tmp; i++)
           {
            if((INT16U)(CardPos + i*BYTEPERSN_NEW) == 0)
             {
              convert_pos = i;
              break;
             }
           }
         }

        outportb(MEMORY_WINDOW_BASE_H, 0x68 + start_pos);

        card_left = (INT16U)(CardPos & 0xffff);
        tmp_SN = RecvIDCardNo.SN + card_left;
        tmp_recv = recv_buf + 48;
        for(k=0; k<tmp; k++)     // tmp
         {
          if(convert_pos == k)
           {
            outportb(MEMORY_WINDOW_BASE_H, 0x68 + end_pos);
            card_left = 0;
            tmp_SN = RecvIDCardNo.SN;
           }

           tmp_SN[0] = tmp_recv[0];
           tmp_SN[1] = tmp_recv[1];
           tmp_SN[2] = tmp_recv[2];
           tmp_SN[3] = tmp_recv[3];
           tmp_SN += BYTEPERSN_NEW;
           tmp_recv += BYTEPERSN;
         }
        outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);
     #endif

       //printf("RecvIDCardNo.PackNum = %d, iddata.CurrPackage = %d, iddata.serialnum = %d\n", RecvIDCardNo.PackNum, iddata.CurrPackage, iddata.serialnum);
       //包已接收标志，查看是否全部接收
       RecvOk = 1;
       for(i=0; i<RecvIDCardNo.PackNum; i++)
        {
         //printf("RecvIDCardNo.PackIsRecved[%d] = %d\n", i, RecvIDCardNo.PackIsRecved[i]);
         if(RecvIDCardNo.PackIsRecved[i] == 0)
          {
            RecvOk = 0;
            break;
          }
        }
       if(RecvOk == 1)
         SaveWriteIDCard_Func();
     #endif
     }
    else
      if(iddata.serialnum != IDCardNo.serialnum)
       {
      #if 1
        RecvIDCardNo.isNewWriteFlag = 1;

        //卡数量
        RecvIDCardNo.Num = iddata.Num;

        //总包数
        RecvIDCardNo.PackNum = iddata.TotalPackage;

        //包已接收标志
        for(i=0; i<RecvIDCardNo.PackNum; i++)
          RecvIDCardNo.PackIsRecved[i] = 0;
        //序号
        RecvIDCardNo.serialnum = iddata.serialnum;

        RecvIDCardNo.PackIsRecved[iddata.CurrPackage-1] = 1;

        CardPos = CARDPERPACK*(iddata.CurrPackage-1)*BYTEPERSN_NEW;
        outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
        tmp = (INT16U)iddata.CurrNum;

        start_pos = (INT16U)((CardPos & 0xffff0000) >> 16);
        end_pos = (INT16U)(((CardPos + tmp*BYTEPERSN_NEW) & 0xffff0000) >> 16);
        convert_pos = CARDPERPACK + 1;
        if(start_pos != end_pos)
         {
          for(i=0; i<tmp; i++)
           {
            if((INT16U)(CardPos + i*BYTEPERSN_NEW) == 0)
             {
              convert_pos = i;
              break;
             }
           }
         }

        outportb(MEMORY_WINDOW_BASE_H, 0x68 + start_pos);

       // xtm_printf("RecvWriteIDCard_Func:1111111111111111111111111\n");
        card_left = (INT16U)(CardPos & 0xffff);
        tmp_SN = RecvIDCardNo.SN + card_left;
        tmp_recv = recv_buf + 48;
        for(k=0; k<tmp; k++)     // tmp
         {
          if(convert_pos == k)
           {
            outportb(MEMORY_WINDOW_BASE_H, 0x68 + end_pos);
            card_left = 0;
            tmp_SN = RecvIDCardNo.SN;
           }

           tmp_SN[0] = tmp_recv[0];
           tmp_SN[1] = tmp_recv[1];
           tmp_SN[2] = tmp_recv[2];
           tmp_SN[3] = tmp_recv[3];

           tmp_SN += BYTEPERSN_NEW;
           tmp_recv += BYTEPERSN;
         }
        outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);

        //xtm_printf("iddata.CurrPackage = %d\n", iddata.CurrPackage);
        //xtm_printf("RecvIDCardNo.PackNum1 = %d\n", RecvIDCardNo.PackNum);
        //包已接收标志，查看是否全部接收
        RecvOk = 1;
        for(i=0; i<RecvIDCardNo.PackNum; i++)
         {
          //xtm_printf("RecvIDCardNo.PackIsRecved[%d] = %d\n", i, RecvIDCardNo.PackIsRecved[i]);
          if(RecvIDCardNo.PackIsRecved[i] == 0)
           {
             RecvOk = 0;
             break;
           }
         }
        //xtm_printf("RecvWriteIDCard_Func::RecvIDCardNo.PackNum = %d, iddata.Num = 0x%X %X, RecvOk= %d\n", RecvIDCardNo.PackNum, (INT16U)((iddata.Num & 0xffff0000) >> 16), \
        //     (INT16U)(iddata.Num & 0xffff), RecvOk);

        if(RecvOk == 1)
          SaveWriteIDCard_Func();        
      #endif
       }
#endif
   }

 //xtm_printf("RecvWriteIDCard_Func: 2 %d\n", Card_Count);
 Card_Count ++;
}
//---------------------------------------------------------------------------
//存储ID卡信息
void SaveWriteIDCard_Func(void)
{
  INT16U i;
  xtm_printf("SaveWriteIDCard_Func, RecvIDCardNo.Num = 0x%X %X\n", (INT16U)((RecvIDCardNo.Num & 0xffff0000) >> 16), (INT16U)((RecvIDCardNo.Num & 0xffff)));
  IDCardNo.Num = RecvIDCardNo.Num;
  IDCardNo.serialnum = RecvIDCardNo.serialnum;
  //xtm_memcopy(IDCardNo.SN, RecvIDCardNo.SN, BYTEPERSN*IDCardNo.Num);


//  for(i=0; i<IDCardNo.Num; i++)
//    xtm_memcopy(IDCardNo.SN[i], RecvIDCardNo.SN[i], 4);
  RecvIDCardNo.serialnum = 0;
  
  RecvIDCardNo.isNewWriteFlag = 0;

  //写ID卡文件
  WriteIDCardFile();
}
//---------------------------------------------------------------------------
//读ID卡信息
void RecvReadIDCard_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT16U i,j;
  INT8U isAddrOK;
  //读写ID卡数据结构
  struct iddata1 iddata;

  i = 0;
  isAddrOK = 1;
  for(j=8; j<8+Local.AddrLen; j++)
   if(LocalCfg.Addr[j-8] != recv_buf[j])
    {
     isAddrOK = 0;
     break;
    }
  xtm_printf("RecvReadIDCard_Func::isAddrOK = %d, cFromIP = %d.%d.%d.%d\n", isAddrOK, cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);

  //地址匹配
  if(isAddrOK == 1)
   {
    if((IDCardNo.Num % CARDPERPACK ) == 0)
      iddata.TotalPackage = IDCardNo.Num / CARDPERPACK;
    else
      iddata.TotalPackage = IDCardNo.Num / CARDPERPACK + 1;
    xtm_printf("RecvReadIDCard_Func::iddata.TotalPackage = 0x%X %X, cFromIP = %d.%d.%d.%d\n", (INT16U)((iddata.TotalPackage & 0xffff0000) >> 16), (INT16U)(iddata.TotalPackage & 0xffff), \
               cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
    if(iddata.TotalPackage == 0)
      iddata.TotalPackage = 1;

    for(i=0; i<iddata.TotalPackage; i++)
      IDCardHaveRead[i] = 0;
    xtm_memcopy(IDCardReadIP, cFromIP, 4);
    IDCardReadFlag = 1;
   }
}
//---------------------------------------------------------------------------
//读ID卡应答
void SendReadIDCardReply(void)
{
  INT16U sendlength;
  INT16U i, j, Udp_Num;
  INT8U SendFinishFlag;
  //读写ID卡数据结构
  struct iddata1 iddata;
  INT32U CardPos;
  INT16U tmp;

  if((IDCardNo.Num % CARDPERPACK ) == 0)
    iddata.TotalPackage = IDCardNo.Num / CARDPERPACK;
  else
    iddata.TotalPackage = IDCardNo.Num / CARDPERPACK + 1;
  xtm_printf("SendReadIDCardReply::iddata.TotalPackage = 0x%X %X, IDCardReadIP = %d.%d.%d.%d\n", (INT16U)((iddata.TotalPackage & 0xffff0000) >> 16), (INT16U)(iddata.TotalPackage & 0xffff), \
             IDCardReadIP[0], IDCardReadIP[1], IDCardReadIP[2], IDCardReadIP[3]);
  if(iddata.TotalPackage == 0)
    iddata.TotalPackage = 1;

    
  Udp_Num = 0;
  for(i=0; i<UDPSENDMAX; i++)
   if(Multi_Udp_Buff[i].isValid == 0)
     Udp_Num ++;
  xtm_printf("SendReadIDCardReply::Udp_Num = %d\n", Udp_Num);
  xtm_printf("iddata.CurrNum = %d\n", (INT16U)(iddata.CurrNum & 0xffff));
  if(Udp_Num > 0)
   {
    for(i=0; i<iddata.TotalPackage; i++)
     {
      if(Udp_Num == 0)
        break;
      if(IDCardHaveRead[i] == 0)
       {
         xtm_printf("SendReadIDCardReply::i = %d\n", i);
         IDCardHaveRead[i] = 1;
         //头部
         xtm_memcopy(send_b, UdpPackageHead, 6);
         //命令
         send_b[6] = READIDCARD;
         send_b[7] = REPLY;    //应答

         xtm_memcopy(iddata.Addr, LocalCfg.Addr, 20);
         //序号
         iddata.serialnum = IDCardNo.serialnum;

         //卡总数
         iddata.Num = IDCardNo.Num;

         iddata.CurrPackage = i + 1;

         if(iddata.CurrPackage == iddata.TotalPackage)
           iddata.CurrNum = IDCardNo.Num - CARDPERPACK*(iddata.CurrPackage-1);
         else
           iddata.CurrNum = CARDPERPACK;

         xtm_memcopy(send_b + 8, &iddata, sizeof(iddata));
         //for(i=0; i<iddata.CurrNum; i++)
         //  xtm_memcopy(send_b + 48 + i*BYTEPERSN, IDCardNo.SN + CARDPERPACK*(iddata.CurrPackage-1)*BYTEPERSN_NEW + i*BYTEPERSN_NEW, BYTEPERSN_NEW);
         //xtm_printf("iddata.CurrNum = %d\n", (INT16U)(iddata.CurrNum & 0xffff));
         for(j=0; j<iddata.CurrNum; j++)
          {
            CardPos = CARDPERPACK*(iddata.CurrPackage-1)*BYTEPERSN_NEW + j*BYTEPERSN_NEW;
            tmp = CardPos & 0xffff;
            outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
            outportb(MEMORY_WINDOW_BASE_H, 0x68 + (CardPos >> 16) & 0xffff);
            xtm_memcopy(send_b + 48 + j*BYTEPERSN, IDCardNo.SN + tmp, BYTEPERSN_NEW);
       //     xtm_printf("IDCardNo.SN = 0x%X, 0x%X, 0x%X, 0x%X\n", IDCardNo.SN[tmp], IDCardNo.SN[tmp+1], IDCardNo.SN[tmp+2], IDCardNo.SN[tmp+3]);
            outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);

            ClearWatchDog();
          }

         sendlength = 48 + BYTEPERSN*iddata.CurrNum;

         CreateMultiUdpBuff(LocalPort[0], SINGLE_SEND, IDCardReadIP, send_b, sendlength, 0);

         SendFinishFlag = 1;
         for(j=0; j<iddata.TotalPackage; j++)
           if(IDCardHaveRead[j] == 0)
            {
             SendFinishFlag = 0;
            }

         if(SendFinishFlag == 1)
          {
           IDCardReadFlag = 0;
           break;
          }
         Udp_Num --;
       }
     }
   }
}
//---------------------------------------------------------------------------
//刷卡记录
void RecvBrushIDCard_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT16U j;
  INT8U isAddrOK;
  //INT8U tmpbrushinfo[IDCARDBRUSHNUM*11];
  //INT32U tmpnum;
  //INT16U sendlength;

  isAddrOK = 1;
  for(j=8; j<8+Local.AddrLen; j++)
   if(LocalCfg.Addr[j-8] != recv_buf[j])
    {
     isAddrOK = 0;
     break;
    }
  //地址匹配
  if(isAddrOK == 1)
   {
    if(ClearMultiUdpBuff(BRUSHIDCARD, ASK, LocalCfg.Addr[0], cFromIP))
     {
              BrushIDCard.Num = 0;
              /*tmpnum = (recv_buf[29] << 8) + recv_buf[28];
              if(tmpnum <= BrushIDCard.Num)
               {
                //锁定互斥锁
                pthread_mutex_lock (&Local.idcard_lock);
                xtm_memcopy(tmpbrushinfo, BrushIDCard.Info + tmpnum*11, (IDCARDBRUSHNUM-tmpnum)*11);
                xtm_memcopy(BrushIDCard.Info, tmpbrushinfo, (IDCARDBRUSHNUM-tmpnum)*11);
                BrushIDCard.Num -= tmpnum;
                //打开互斥锁
                pthread_mutex_unlock (&Local.idcard_lock);
               }
              if(BrushIDCard.Num == 0)
                Multi_Udp_Buff[i].isValid = 0;
              else
               {
                //数量
                if(BrushIDCard.Num > 120)
                  tmpnum = 120;
                else
                  tmpnum = BrushIDCard.Num;
                Multi_Udp_Buff[i].buf[29] = (tmpnum & 0xFF00) >> 8;
                Multi_Udp_Buff[i].buf[28] = (tmpnum & 0x00FF);

                xtm_memcopy( Multi_Udp_Buff[i].buf + 30, BrushIDCard.Info, 11*tmpnum);

                Multi_Udp_Buff[i].nlength = 30 + 11*tmpnum;
                Multi_Udp_Buff[i].DelayTime = 100;
                Multi_Udp_Buff[i].SendDelayTime = 0; //发送等待时间计数  20101112
                Multi_Udp_Buff[i].isValid = 1;
                sem_post(&multi_send_sem);
               }      */

              xtm_printf("收到刷卡记录应答\n");
     }
   }
}
#endif
//---------------------------------------------------------------------------
#ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
//处理捕获的图片
void Deal_CapturePic_Center(void)
{
    if(CapturePic.isVilid == 0)
     {
      CapturePic.isVilid  = 1;

     // strcpy(Capture_Pic_Center[i].RemoteAddr, CapturePic.RemoteAddr);
     // Capture_Pic_Center[i].recpic_tm_t = Local.recpic_tm_t;

     }
}
//---------------------------------------------------------------------------
//单元门口机、围墙机、二次门口机发送呼叫照片->发送开始
void RecvCapture_Send_Start_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT8U i,j;
  INT8U isAddrOK;
  INT8U Start_OK;
  INT16U CurrPack;
  Start_OK = 0;

  //printf("RecvCapture_Send_Start_Func1 \n");
  isAddrOK = 1;
  for(j=8; j<8+Local.AddrLen; j++)
   if(LocalCfg.Addr[j-8] != recv_buf[j])
    {
     isAddrOK = 0;
     break;
    }
  //printf("isAddrOK  = %d\n", isAddrOK);
  //地址匹配
  if(isAddrOK == 1)
   {
    if((recv_buf[7] & 0x03) == REPLY)   //应答
     if(ClearMultiUdpBuff(CAPTUREPIC_SEND_START, ASK, LocalCfg.Addr[0], cFromIP))
      {
       Start_OK = 1;
       CurrPack = 0;

       xtm_printf("收到呼叫照片->发送开始应答\n");
      }

    if(Start_OK == 1)
      SendCapturePicData(CurrPack);
   }
}
//---------------------------------------------------------------------------
//单元门口机、围墙机、二次门口机发送呼叫照片->发送数据
void RecvCapture_Send_Data_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT8U i, j;
  INT8U isAddrOK;
  INT8U sendlength;
  INT8U Finish_OK;
  INT16U CurrNo;
  INT16U CurrPack;

  i = 0;
  isAddrOK = 1;
  for(j=8; j<8+Local.AddrLen; j++)
   if(LocalCfg.Addr[j-8] != recv_buf[j])
    {
     isAddrOK = 0;
     break;
    }
  //printf("RecvCapture_Send_Data_Func   isAddrOK = %d\n", isAddrOK);
  //地址匹配
  if(isAddrOK == 1)
   {
    if((recv_buf[7] & 0x03) == REPLY)   //应答
     if(ClearMultiUdpBuff(CAPTUREPIC_SEND_DATA, ASK, LocalCfg.Addr[0], cFromIP))
       {
              xtm_memcopy(&CurrNo, recv_buf + 48, sizeof(CurrNo));
              xtm_memcopy(&CurrPack, recv_buf + 56, sizeof(CurrPack));
              if(CurrPack >= 2000)
               {
                xtm_printf("CurrPack = %d\n", CurrPack);
                CurrPack = 0;
               }
              CapturePic.Capture_Send_Flag[CurrPack] = 1;

              CurrPack ++;
              if(CurrPack < CapturePic.Capture_Total_Package)
                SendCapturePicData(CurrPack);
               //sprintf(Local.DebugInfo, "收到呼叫照片->发送数据应答, %d\n", CurrPack);

       }


    Finish_OK = 1;
    for(j=0; j<CapturePic.Capture_Total_Package; j++)
     if(CapturePic.Capture_Send_Flag[j] == 0)
      {
       Finish_OK = 0;
       break;
      }
    if(Finish_OK == 1)
      SendCapturePicFinish(CurrNo, 1);
   }
}
//---------------------------------------------------------------------------
//单元门口机、围墙机、二次门口机发送呼叫照片->发送结束（成功）
void RecvCapture_Send_Succ_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT8U j;
  INT8U isAddrOK;
  INT8U sendlength;

  isAddrOK = 1;
  for(j=8; j<8+Local.AddrLen; j++)
   if(LocalCfg.Addr[j-8] != recv_buf[j])
    {
     isAddrOK = 0;
     break;
    }
  //地址匹配
  if(isAddrOK == 1)
   {
    if((recv_buf[7] & 0x03) == REPLY)   //应答
     if(ClearMultiUdpBuff(CAPTUREPIC_SEND_SUCC, ASK, LocalCfg.Addr[0], cFromIP))
       {
        CapturePic.isVilid = 0;
        CapturePic.SendToCetnering = 0;  //正在传给中心
        xtm_printf("收到呼叫照片->发送结束（成功）应答\n");
       }
   }
}
//---------------------------------------------------------------------------
//单元门口机、围墙机、二次门口机发送呼叫照片->发送结束（失败）
void RecvCapture_Send_Fail_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT8U j;
  INT8U isAddrOK;
  INT8U sendlength;

  isAddrOK = 1;
  for(j=8; j<8+Local.AddrLen; j++)
   if(LocalCfg.Addr[j-8] != recv_buf[j])
    {
     isAddrOK = 0;
     break;
    }
  //地址匹配
  if(isAddrOK == 1)
   {
    if((recv_buf[7] & 0x03) == REPLY)   //应答
     if(ClearMultiUdpBuff(CAPTUREPIC_SEND_FAIL, ASK, LocalCfg.Addr[0], cFromIP))
      {
       CapturePic.SendToCetnering = 0;  //正在传给中心
       xtm_printf("收到呼叫照片->发送结束（失败）应答\n");
      }
   }
}
#endif
//---------------------------------------------------------------------------
void VideoPortDeal(INT8U *buff, INT8U *FromIP, INT16U DestPort, INT16U recv_len)
{
      switch(buff[6])
       {
        case NSORDER:   //主机名解析（子网内广播）
    //    case NSSERVERORDER:  //主机名解析(NS服务器)
                switch(buff[7] & 0x03)
                 {
                  case 1://解析
                         if(recv_len >= SMALLESTSIZE)
                          {
                            xtm_printf("解析请求, recv_len = %d\n", recv_len);
                            RecvNSAsk_Func(buff, FromIP, DestPort);
                          }
                         else
                          {
                            xtm_printf("解析请求数据长度异常, recv_len = %d\n", recv_len);
                          }
                         break;
                  case 2://解析回应
                         if(recv_len >= SMALLESTSIZE)
                          {
                           RecvNSReply_Func(buff, FromIP, DestPort);
                          }
                         else
                          {
                            xtm_printf("解析应答数据长度异常\n");
                          }
                         break;
                 }
                break;
            case NSSERVERORDER2:  
                switch(buff[7] & 0x03)
                {
                case 1://解析
                    if(recv_len >= SMALLESTSIZE)
                    {
                        xtm_printf("解析请求, recv_len = %d\n", recv_len);
                        RecvNSAsk2_Func(buff, FromIP, DestPort);
  
                    }
                    else
                    {
                        xtm_printf("解析请求数据长度异常, recv_len = %d\n", recv_len);
                    }
                    break;
                }
                break;
        case VIDEOTALK:    //局域网可视对讲
               //xtm_printf("buff[6] = %d, buff[7] = %d, buff[8] = %d\n", buff[6], buff[7], buff[8]);
               switch(buff[8])    //42 + 8
                {
                  #ifdef _MULTI_MACHINE_SUPPORT  //多分机支持
                  case LEAVEGROUP:        //退出组播组（主叫方->被叫方，被叫方应答）
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvTalkLeaveGroup_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                                xtm_printf("对讲退出组播组数据长度异常\n");
                             }
                            break;
                  #endif
                  case CALL:        //对方发起对讲
                            if(recv_len >= SMALLESTSIZE)
                             {
                               RecvTalkCall_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                                xtm_printf("对讲呼叫数据长度异常\n");
                             }
                            break;
                  case LINEUSE:        //对方正忙
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvTalkLineUse_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                                xtm_printf("占线应答数据长度异常\n");
                             }
                            break;
                  case CALLANSWER:  //呼叫应答
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvTalkCallAnswer_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                                xtm_printf("对讲呼叫应答数据长度异常\n");
                             }
                            break;
                  case CALLSTART:  //被叫方开始通话
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvTalkCallStart_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                                xtm_printf("对讲开始通话数据长度异常\n");
                             }
                            break;
                  case CALLCONFIRM:     //通话在线确认
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvTalkCallConfirm_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                                xtm_printf("对讲通话在线确认数据长度异常\n");
                             }
                            break;
                  case CALLEND:  //通话结束
                            //结束播放视频
                            //如为对方结束，需应答
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvTalkCallEnd_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                              xtm_printf("结束对讲数据长度异常\n");
                             }
                            break;
                  case REMOTEOPENLOCK:    //远程开锁
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvTalkRemoteOpenLock_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                              xtm_printf("远程开锁数据长度异常\n");
                             }
                            break;
                  case CENTERTURNTALK:     //中心人工转接
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvTalkCenterTurnTalk_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                              xtm_printf("中心人工转接数据长度异常\n");
                             }
                            break;
                  case TRUSTEESHIP_TALK:    //托管
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvTalkTrusteeship_Talk_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                              xtm_printf("托管数据长度异常\n");
                             }
                            break;
                  case CALLUP: //通话上行
                  case CALLDOWN: //通话下行
                           RecvTalkCallUpDown_Func(buff, FromIP, DestPort, recv_len);
                           break;
                  }
                 break;
        case VIDEOWATCH:     //局域网监控
               switch(buff[8])
                {
                  case CALL:        //对方发起监视
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvWatchCall_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                                xtm_printf("监视呼叫数据长度异常\n");
                             }
                            break;
                  case LINEUSE:        //对方正忙
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvWatchLineUse_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                                xtm_printf("占线应答数据长度异常\n");
                             }
                            break;
                  case CALLANSWER:  //呼叫应答
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvWatchCallAnswer_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                                xtm_printf("监视呼叫应答数据长度异常\n");
                             }
                            break;
                  case CALLCONFIRM:     //通话在线确认
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvWatchCallConfirm_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                                xtm_printf("监视通话在线确认数据长度异常\n");
                             }
                            break;
                  case CALLEND:  //通话结束
                            //结束播放视频
                            //如为对方结束，需应答
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvWatchCallEnd_Func(buff, FromIP, DestPort);
                             }
                            else
                             {
                                xtm_printf("结束监视数据长度异常\n");
                             }
                            break;
                  case CALLUP: //通话上行
                  case CALLDOWN: //通话下行
                           RecvWatchCallUpDown_Func(buff, FromIP, DestPort, recv_len);
                           break;
                }
               break;
       #ifdef ELEVATORCONTROL_HANGZHOU
        case ELEVATR_ORDER1:
               if(recv_len >= SMALLESTSIZE)
                {
                 RecvElevatorControl_Func(buff, FromIP, DestPort, recv_len);
                }
               else
                {
                 xtm_printf("电梯控制数据长度异常\n");
                }
               break;
       #endif
       }
}
//---------------------------------------------------------------------------
void SmarkPortDeal(INT8U *buff, INT8U *FromIP, INT8U *DestIP,INT16U DestPort, INT16U recv_len)
{

}
//---------------------------------------------------------------------------
void SmarkMediaPortDeal(INT8U *buff, INT8U *FromIP, INT8U *DestIP,INT16U DestPort, INT16U recv_len)
{
    switch(buff[5])
    {
     
    case SMARK_MONITOR:
        
        switch(buff[6])
                {
                  case 1:        //对方发起监视
                            if(recv_len >= SMALLESTSIZE)
                             {
                                RecvSmarkMonitorCall_Func(buff, FromIP, DestPort);
                                
                             }
                            else
                             {
                                xtm_printf("监视呼叫数据长度异常\n");
                             }
                            break;
                  case 2:
                             if(recv_len >= SMALLESTSIZE)
                             {
                                RecvSmarkMonitorStart_Func(buff, FromIP, DestPort);
                                
                             }
                            else
                             {
                                xtm_printf("监视呼叫数据长度异常\n");
                             }
                            break;
                  case 3:     //通话在线确认
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvSmarkMonitorCallConfirm_Func();
                             }
                            else
                             {
                                xtm_printf("监视通话在线确认数据长度异常\n");
                             }
                            break;
                  case 4:  //通话结束
                            //结束播放视频
                            //如为对方结束，需应答
                            if(recv_len >= SMALLESTSIZE)
                             {
                              RecvSmarkMonitorCallEnd_Func(buff, FromIP);
                             }
                            else
                             {
                                xtm_printf("结束监视数据长度异常\n");
                             }
                            break;
                  case CALLUP: //通话上行
                  case CALLDOWN: //通话下行
                           RecvWatchCallUpDown_Func(buff, FromIP, DestPort, recv_len);
                           break;
                }
               break;
    }

}
//---------------------------------------------------------------------------
#ifdef ELEVATORCONTROL_HANGZHOU
void RecvElevatorControl_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)  //电梯控制
{
  INT8U sendlength;
  INT8U DestAddr[21], SrcAddr[21];

    xtm_memcopy(send_b, recv_buf, 57);
    send_b[7] = Local.MachineType | REPLY;    //应答
    sendlength = 57;
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);

    xtm_memcopy(DestAddr, recv_buf + 33, 20);
    xtm_memcopy(SrcAddr, recv_buf + 9, 20);

    SendElevatorControl_Order(DestAddr, SrcAddr);
    xtm_printf("RecvElevatorControl_Func::对方控制电梯, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
}
#endif
//-----------------------------------------------------------------------
void CreateMultiUdpBuff(INT16U DestPort, INT8U SendMode, INT8U *RemoteIP, INT8U *udpdata, INT16U nlength, INT8U CurrOrder)
{
  INT8U i, j;
  INT8U Net1[4], Net2[4];
  INT8U RemoteIP1[4];
#ifdef _ADD_UDP_LOCK
  MultiUdp_Lock = 1;
  //xtm_printf("1\n");
#endif  
  for(i=0; i<UDPSENDMAX; i++)
   if(Multi_Udp_Buff[i].isValid == 0)
    {
     if((Multi_Udp_Buff[i].buf[6] == VIDEOWATCH)&&(Multi_Udp_Buff[i].buf[8] == CALL))
       xtm_printf("CreateMultiUdpBuff:: VIDEOWATCH i = %d\n", i);
     Multi_Udp_Buff[i].isValid = 1; //是否有效
     Multi_Udp_Buff[i].SendMode = SendMode; //ASK -- 多次发送   REPLY --- 单次发送
     
     if(Multi_Udp_Buff[i].SendMode == MULTI_SEND)
       Multi_Udp_Buff[i].MaxSendNum = 6; //当前发送次数
     else
       Multi_Udp_Buff[i].MaxSendNum = 1; //当前发送次数
       
     Multi_Udp_Buff[i].CurrOrder = CurrOrder;//当前命令状态,VIDEOTALK  VIDEOWATCH
                                 //主要用于需解析时，如单次命令置为0
     Multi_Udp_Buff[i].DestPort = DestPort;
     Multi_Udp_Buff[i].MacValid = 0;
     //组播地址，无需ARP
     if((RemoteIP[0] == Local.NsMultiAddr[0])&&(RemoteIP[1] == Local.NsMultiAddr[1])&&(RemoteIP[2] == Local.NsMultiAddr[2])&&(RemoteIP[3] == Local.NsMultiAddr[3]))
      {
       Multi_Udp_Buff[i].MacValid = 1;
       Multi_Udp_Buff[i].RemoteMac[0] = 0x01;
       Multi_Udp_Buff[i].RemoteMac[1] = 0x00;
       Multi_Udp_Buff[i].RemoteMac[2] = 0x5E;
       Multi_Udp_Buff[i].RemoteMac[3] = Local.NsMultiAddr[1];
       Multi_Udp_Buff[i].RemoteMac[4] = Local.NsMultiAddr[2];
       Multi_Udp_Buff[i].RemoteMac[5] = Local.NsMultiAddr[3];
      }
     else
      {
       if(RemoteIP[3] == 0xFF)    //广播
        {
         Multi_Udp_Buff[i].MacValid = 1;
         Multi_Udp_Buff[i].RemoteMac[0] = 0xFF;
         Multi_Udp_Buff[i].RemoteMac[1] = 0xFF;
         Multi_Udp_Buff[i].RemoteMac[2] = 0xFF;
         Multi_Udp_Buff[i].RemoteMac[3] = 0xFF;
         Multi_Udp_Buff[i].RemoteMac[4] = 0xFF;
         Multi_Udp_Buff[i].RemoteMac[5] = 0xFF;
        }
       else
        {
         for(j=0; j<4; j++)
          {
           Net1[j] = LocalCfg.IP[j] & LocalCfg.IP_Mask[j];
           Net2[j] = RemoteIP[j] & LocalCfg.IP_Mask[j];
          }
         if((Net1[0] == Net2[0])&&(Net1[1] == Net2[1])&&(Net1[2] == Net2[2])&&(Net1[3] == Net2[3]))  //同一子网
           xtm_memcopy(RemoteIP1, RemoteIP, 4);
         else                                                                                        //不同子网
           xtm_memcopy(RemoteIP1, LocalCfg.IP_Gate, 4);

        #ifdef _SEND_TO_SELF_TEST    //发送数据包给自己
         if((RemoteIP1[0] == LocalCfg.IP[0])&&(RemoteIP1[1] == LocalCfg.IP[1])&&(RemoteIP1[2] == LocalCfg.IP[2])&&(RemoteIP1[3] == LocalCfg.IP[3]))
          {
           Multi_Udp_Buff[i].MacValid = 1;
           Multi_Udp_Buff[i].RemoteMac[0] = LocalCfg.Mac_Addr[0];
           Multi_Udp_Buff[i].RemoteMac[1] = LocalCfg.Mac_Addr[1];
           Multi_Udp_Buff[i].RemoteMac[2] = LocalCfg.Mac_Addr[2];
           Multi_Udp_Buff[i].RemoteMac[3] = LocalCfg.Mac_Addr[3];
           Multi_Udp_Buff[i].RemoteMac[4] = LocalCfg.Mac_Addr[4];
           Multi_Udp_Buff[i].RemoteMac[5] = LocalCfg.Mac_Addr[5];
          }
         else
          {
        #endif
           if(SearchArp_Table(RemoteIP1, Multi_Udp_Buff[i].RemoteMac))
            {
             Multi_Udp_Buff[i].MacValid = 1;
            }
           else
            {
             //xtm_printf("SendArp_Ask::%d.%d.%d.%d, CurrOrder = %d\n", RemoteIP1[0], RemoteIP1[1], RemoteIP1[2], RemoteIP1[3], CurrOrder);
             SendArp_Ask(RemoteIP1);   //发送 ARP请求表
             Multi_Udp_Buff[i].ArpNum = 1;  //ARP次数
            }
        #ifdef _SEND_TO_SELF_TEST    //发送数据包给自己
          }
        #endif
        }
      }

     xtm_memcopy(Multi_Udp_Buff[i].RemoteIP, RemoteIP, 4);
   #if 0
   #else
     xtm_memcopy(Multi_Udp_Buff[i].buf, udpdata, nlength);
   #endif
     Multi_Udp_Buff[i].SendNum = 0;
     Multi_Udp_Buff[i].TimeNum = 0;
     Multi_Udp_Buff[i].DelayTime = 600;  //等待时间  200ms
     Multi_Udp_Buff[i].SendDelayTime = 0; //发送等待时间计数  20101112
     Multi_Udp_Buff[i].nlength = nlength;

     if((Multi_Udp_Buff[i].buf[6] == VIDEOWATCH)&&(Multi_Udp_Buff[i].buf[8] == CALLEND))
       xtm_printf("CreateMultiUdpBuff::VIDEOWATCH:CALLEND: i = %d, %d %d, MaxSendNum = %d,  0x%X %X %X %X %X %X\n", i, Multi_Udp_Buff[i].buf[6], Multi_Udp_Buff[i].buf[8], Multi_Udp_Buff[i].MaxSendNum, \
                   Multi_Udp_Buff[i].buf[0], Multi_Udp_Buff[i].buf[1], Multi_Udp_Buff[i].buf[2], Multi_Udp_Buff[i].buf[3], Multi_Udp_Buff[i].buf[4], Multi_Udp_Buff[i].buf[5]);

     //xtm_printf("CreateMultiUdpBuff:: i = %d, %d, %d, RemoteHost = %d.%d.%d.%d, RemoteMac = %X:%X:%X:%X:%X:%X\n", i, Multi_Udp_Buff[i].buf[6], Multi_Udp_Buff[i].buf[8], \
     //            Multi_Udp_Buff[i].RemoteIP[0], Multi_Udp_Buff[i].RemoteIP[1], Multi_Udp_Buff[i].RemoteIP[2], Multi_Udp_Buff[i].RemoteIP[3], \
     //            Multi_Udp_Buff[i].RemoteMac[0], Multi_Udp_Buff[i].RemoteMac[1], Multi_Udp_Buff[i].RemoteMac[2], Multi_Udp_Buff[i].RemoteMac[3], Multi_Udp_Buff[i].RemoteMac[4], Multi_Udp_Buff[i].RemoteMac[5]);
   //for test  
   /*  if(Multi_Udp_Buff[i].SendMode != MULTI_SEND)
      {
       UdpSendBuff(Multi_Udp_Buff[i].DestPort, Multi_Udp_Buff[i].RemoteMac, Multi_Udp_Buff[i].RemoteIP, Multi_Udp_Buff[i].buf, Multi_Udp_Buff[i].nlength);
       Multi_Udp_Buff[i].isValid = 0;
      }         */
     //xtm_printf("CreateMultiUdpBuff::i = %d, %d %d, MaxSendNum = %d,  0x%X %X %X %X %X %X\n", i, Multi_Udp_Buff[i].buf[6], Multi_Udp_Buff[i].buf[8], Multi_Udp_Buff[i].MaxSendNum, \
     //            Multi_Udp_Buff[i].buf[0], Multi_Udp_Buff[i].buf[1], Multi_Udp_Buff[i].buf[2], Multi_Udp_Buff[i].buf[3], Multi_Udp_Buff[i].buf[4], Multi_Udp_Buff[i].buf[5]);
     break;
    }
#ifdef _ADD_UDP_LOCK
   MultiUdp_Lock = 0;
   //xtm_printf("0\n");
#endif   
   if(i >= UDPSENDMAX)
     xtm_printf("CreateMultiUdpBuff::buff is full\n");
}
//---------------------------------------------------------------------------
INT8U Audio_Start = 0;
//给音频中断调用
void CreateMultiUdpBuff_Audio(INT16U DestPort, INT8U SendMode, INT8U *RemoteIP, INT8U *udpdata, INT16U nlength, INT8U CurrOrder)
{
  INT8U i, j;
  INT8U Net1[4], Net2[4];
  INT8U RemoteIP1[4];
#ifdef _ADD_UDP_LOCK
  //xtm_printf("1:0\n");
#endif
  for(i=0; i<AUDIOSENDMAX; i++)
   if(Audio_Udp_Buff[i].isValid == 0)
    {
    /* if(Audio_Start < 20)
      {
       xtm_printf("1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\n");
       Audio_Start ++;
      }           */

     Audio_Udp_Buff[i].isValid = 1; //是否有效
     Audio_Udp_Buff[i].SendMode = SendMode; //ASK -- 多次发送   REPLY --- 单次发送
     if(Audio_Udp_Buff[i].SendMode == MULTI_SEND)
       Audio_Udp_Buff[i].MaxSendNum = 6; //当前发送次数
     else
       Audio_Udp_Buff[i].MaxSendNum = 1; //当前发送次数
     Audio_Udp_Buff[i].CurrOrder = CurrOrder;//当前命令状态,VIDEOTALK  VIDEOWATCH
                                 //主要用于需解析时，如单次命令置为0
     Audio_Udp_Buff[i].DestPort = DestPort;
     Audio_Udp_Buff[i].MacValid = 0;
     //组播地址，无需ARP
     if((RemoteIP[0] == Local.NsMultiAddr[0])&&(RemoteIP[1] == Local.NsMultiAddr[1])&&(RemoteIP[2] == Local.NsMultiAddr[2])&&(RemoteIP[3] == Local.NsMultiAddr[3]))
      {
       Audio_Udp_Buff[i].MacValid = 1;
       Audio_Udp_Buff[i].RemoteMac[0] = 0x01;
       Audio_Udp_Buff[i].RemoteMac[1] = 0x00;
       Audio_Udp_Buff[i].RemoteMac[2] = 0x5E;
       Audio_Udp_Buff[i].RemoteMac[3] = Local.NsMultiAddr[1];
       Audio_Udp_Buff[i].RemoteMac[4] = Local.NsMultiAddr[2];
       Audio_Udp_Buff[i].RemoteMac[5] = Local.NsMultiAddr[3];
      }
     else
      {
       if(RemoteIP[3] == 0xFF)    //广播
        {
         Audio_Udp_Buff[i].MacValid = 1;
         Audio_Udp_Buff[i].RemoteMac[0] = 0xFF;
         Audio_Udp_Buff[i].RemoteMac[1] = 0xFF;
         Audio_Udp_Buff[i].RemoteMac[2] = 0xFF;
         Audio_Udp_Buff[i].RemoteMac[3] = 0xFF;
         Audio_Udp_Buff[i].RemoteMac[4] = 0xFF;
         Audio_Udp_Buff[i].RemoteMac[5] = 0xFF;
        }
       else
        {
         for(j=0; j<4; j++)
          {
           Net1[j] = LocalCfg.IP[j] & LocalCfg.IP_Mask[j];
           Net2[j] = RemoteIP[j] & LocalCfg.IP_Mask[j];
          }
         if((Net1[0] == Net2[0])&&(Net1[1] == Net2[1])&&(Net1[2] == Net2[2])&&(Net1[3] == Net2[3]))  //同一子网
           xtm_memcopy(RemoteIP1, RemoteIP, 4);
         else                                                                                        //不同子网
           xtm_memcopy(RemoteIP1, LocalCfg.IP_Gate, 4);
           
         if(SearchArp_Table(RemoteIP1, Audio_Udp_Buff[i].RemoteMac))
          {
           Audio_Udp_Buff[i].MacValid = 1;
          }
         else
          {
           //xtm_printf("SendArp_Ask::%d.%d.%d.%d, CurrOrder = %d\n", RemoteIP1[0], RemoteIP1[1], RemoteIP1[2], RemoteIP1[3], CurrOrder);
           SendArp_Ask(RemoteIP1);   //发送 ARP请求表
           Audio_Udp_Buff[i].ArpNum = 1;  //ARP次数
          }
         /*if(Audio_Udp_Buff[i].MacValid == 0)
          {
           xtm_printf("CreateMultiUdpBuff_Audio::Audio_Udp_Buff[i].MacValid = %d, RemoteIP1 = %d.%d.%d.%d, Net1 = %d.%d.%d.%d, Net2 = %d.%d.%d.%d\n", Audio_Udp_Buff[i].MacValid, \
                       RemoteIP1[0], RemoteIP1[1], RemoteIP1[2], RemoteIP1[3], Net1[0], Net1[1], Net1[2], Net1[3], Net2[0], Net2[1], Net2[2], Net2[3]);
           PrintArp_Table();
          }       */
        }
      }

     xtm_memcopy(Audio_Udp_Buff[i].RemoteIP, RemoteIP, 4);
   #if 0
   #else
     xtm_memcopy(Audio_Udp_Buff[i].buf, udpdata, nlength);
   #endif
     Audio_Udp_Buff[i].SendNum = 0;
     Audio_Udp_Buff[i].TimeNum = 0;
     Audio_Udp_Buff[i].DelayTime = 200;  //等待时间  200ms
     Audio_Udp_Buff[i].SendDelayTime = 0; //发送等待时间计数  20101112
     Audio_Udp_Buff[i].nlength = nlength;
     break;
    }
#ifdef _ADD_UDP_LOCK
   //xtm_printf("1:1\n");
#endif   
   if(i >= UDPSENDMAX)
    {
     xtm_printf("CreateMultiUdpBuff_Audio::buff is full\n");
    }
}
//---------------------------------------------------------------------------
void CheckAudioUdpBuff(void)   //检查Audio UDP发送缓存
{
  INT8U i, j;
  INT8U Net1[4], Net2[4];
  INT8U RemoteIP1[4];

  for(i=0; i<AUDIOSENDMAX; i++)
   if(Audio_Udp_Buff[i].isValid == 1)
    {
     if(Audio_Udp_Buff[i].TimeNum == 0)
      {
       if(Audio_Udp_Buff[i].MacValid == 1)
        {
         UdpSendBuff_1(Audio_Udp_Buff[i].DestPort, Audio_Udp_Buff[i].RemoteMac, Audio_Udp_Buff[i].RemoteIP, Audio_Udp_Buff[i].buf, Audio_Udp_Buff[i].nlength);
        }
       else
        {
         Audio_Udp_Buff[i].MacValid = 0;

         for(j=0; j<4; j++)
          {
           Net1[j] = LocalCfg.IP[j] & LocalCfg.IP_Mask[j];
           Net2[j] = Audio_Udp_Buff[i].RemoteIP[j] & LocalCfg.IP_Mask[j];
          }
         if((Net1[0] == Net2[0])&&(Net1[1] == Net2[1])&&(Net1[2] == Net2[2])&&(Net1[3] == Net2[3]))  //同一子网
           xtm_memcopy(RemoteIP1, Audio_Udp_Buff[i].RemoteIP, 4);
         else                                                                                        //不同子网
           xtm_memcopy(RemoteIP1, LocalCfg.IP_Gate, 4);

         if(SearchArp_Table(RemoteIP1, Audio_Udp_Buff[i].RemoteMac))
          {
           Audio_Udp_Buff[i].MacValid = 1;
          }
         else
          {
           SendArp_Ask(RemoteIP1);   //发送 ARP请求表
           Audio_Udp_Buff[i].ArpNum ++;  //ARP次数
           if(Audio_Udp_Buff[i].ArpNum > 10)
             Audio_Udp_Buff[i].isValid = 0;
          }
         //xtm_printf("dddddddddddddddddd\n");
         continue;
        }
       if(Audio_Udp_Buff[i].SendMode == SINGLE_SEND)  //只发一次
        {
         Audio_Udp_Buff[i].isValid = 0;
         continue;
        }
      }
    }
}
//---------------------------------------------------------------------------
void CheckMultiUdpBuff(void)   //检查UDP发送缓存
{
  INT8U i, j;
  INT8U Net1[4], Net2[4];
  INT8U RemoteIP1[4];

  for(i=0; i<UDPSENDMAX; i++)
   if(Multi_Udp_Buff[i].isValid == 1)
    {
     //if(Audio_Start < 20)
     //  xtm_printf("2222222222222\n");
     if(Multi_Udp_Buff[i].TimeNum == 0)
      {
       if((Multi_Udp_Buff[i].buf[6] == VIDEOWATCH)&&(Multi_Udp_Buff[i].buf[8] == CALLEND))
         xtm_printf("CheckMultiUdpBuff::VIDEOWATCH:CALLEND: i = %d, %d %d, MaxSendNum = %d,  0x%X %X %X %X %X %X\n", i, Multi_Udp_Buff[i].buf[6], Multi_Udp_Buff[i].buf[8], Multi_Udp_Buff[i].MaxSendNum, \
                     Multi_Udp_Buff[i].buf[0], Multi_Udp_Buff[i].buf[1], Multi_Udp_Buff[i].buf[2], Multi_Udp_Buff[i].buf[3], Multi_Udp_Buff[i].buf[4], Multi_Udp_Buff[i].buf[5]);
       //if(Audio_Start < 20)
       //  xtm_printf("33333333333\n");
       if(Multi_Udp_Buff[i].MacValid == 1)
        {
        // xtm_printf("1 i = %d  %X %X %X %X %X %X, %d.%d.%d.%d\n", i, Multi_Udp_Buff[i].RemoteMac[0], Multi_Udp_Buff[i].RemoteMac[1], Multi_Udp_Buff[i].RemoteMac[2], Multi_Udp_Buff[i].RemoteMac[3], Multi_Udp_Buff[i].RemoteMac[4], Multi_Udp_Buff[i].RemoteMac[5],
        //   Multi_Udp_Buff[i].RemoteIP[0], Multi_Udp_Buff[i].RemoteIP[1], Multi_Udp_Buff[i].RemoteIP[2], Multi_Udp_Buff[i].RemoteIP[3]);
         //if(Multi_Udp_Buff[i].buf[6] == NSSERVERORDER)
         //  xtm_printf("Multi_Udp_Buff[i].DestPort = %d, RemoteMac = %X:%X:%X:%X:%X:%X, RemoteIP = %d.%d.%d.%d\n", Multi_Udp_Buff[i].DestPort, Multi_Udp_Buff[i].RemoteMac[0], Multi_Udp_Buff[i].RemoteMac[1], Multi_Udp_Buff[i].RemoteMac[2], Multi_Udp_Buff[i].RemoteMac[3], Multi_Udp_Buff[i].RemoteMac[4], Multi_Udp_Buff[i].RemoteMac[5], \
         //              Multi_Udp_Buff[i].RemoteIP[0], Multi_Udp_Buff[i].RemoteIP[1], Multi_Udp_Buff[i].RemoteIP[2], Multi_Udp_Buff[i].RemoteIP[3]);
         UdpSendBuff_1(Multi_Udp_Buff[i].DestPort, Multi_Udp_Buff[i].RemoteMac, Multi_Udp_Buff[i].RemoteIP, Multi_Udp_Buff[i].buf, Multi_Udp_Buff[i].nlength);
        // xtm_printf("2 i = %d %X %X\n", i, Multi_Udp_Buff[i].buf[8], Multi_Udp_Buff[i].buf[9]);
         //xtm_printf("CheckMultiUdpBuff::%d %d, %d.%d.%d.%d, %d, SendNum = %d, i = %d, MaxSendNum = %d, nlength = %d\n", Multi_Udp_Buff[i].buf[6], Multi_Udp_Buff[i].buf[8], \
          //  Multi_Udp_Buff[i].RemoteIP[0], Multi_Udp_Buff[i].RemoteIP[1], Multi_Udp_Buff[i].RemoteIP[2], Multi_Udp_Buff[i].RemoteIP[3], Multi_Udp_Buff[i].DestPort, \
          //  Multi_Udp_Buff[i].SendNum, i, Multi_Udp_Buff[i].MaxSendNum, Multi_Udp_Buff[i].nlength);
        }
       else
        {
         Multi_Udp_Buff[i].MacValid = 0;

         for(j=0; j<4; j++)
          {
           Net1[j] = LocalCfg.IP[j] & LocalCfg.IP_Mask[j];
           Net2[j] = Multi_Udp_Buff[i].RemoteIP[j] & LocalCfg.IP_Mask[j];
          }
         if((Net1[0] == Net2[0])&&(Net1[1] == Net2[1])&&(Net1[2] == Net2[2])&&(Net1[3] == Net2[3]))  //同一子网
           xtm_memcopy(RemoteIP1, Multi_Udp_Buff[i].RemoteIP, 4);
         else                                                                                        //不同子网
           xtm_memcopy(RemoteIP1, LocalCfg.IP_Gate, 4);

         if(SearchArp_Table(RemoteIP1, Multi_Udp_Buff[i].RemoteMac))
          {
           Multi_Udp_Buff[i].MacValid = 1;
          }
         else
          {
           //xtm_printf("SendArp_Ask::%d.%d.%d.%d\n", Multi_Udp_Buff[i].RemoteIP[0], Multi_Udp_Buff[i].RemoteIP[1], Multi_Udp_Buff[i].RemoteIP[2], Multi_Udp_Buff[i].RemoteIP[3]);
           SendArp_Ask(RemoteIP1);   //发送 ARP请求表
           Multi_Udp_Buff[i].ArpNum ++;  //ARP次数
           if(Multi_Udp_Buff[i].ArpNum > 10)
            {
             Multi_Udp_Buff[i].isValid = 0;
             MultiUdpSendFailDeal(i);
            } 
          }
         //xtm_printf("dddddddddddddddddd\n");
         continue;
        }
       if((Multi_Udp_Buff[i].buf[7] & 0x03) == REPLY)  //应答数据包， 只发一次
        {
         //xtm_printf("isValid 1\n");
         Multi_Udp_Buff[i].isValid = 0;
         continue;
        }
       if(Multi_Udp_Buff[i].SendMode == SINGLE_SEND)  //只发一次
        {
         //xtm_printf("isValid 2\n");
         Multi_Udp_Buff[i].isValid = 0;
         continue;
        }
       Multi_Udp_Buff[i].SendNum ++;
       Multi_Udp_Buff[i].TimeNum = 0;

       if(Multi_Udp_Buff[i].SendNum > 1)
        if(Multi_Udp_Buff[i].DelayTime > 200)
          Multi_Udp_Buff[i].DelayTime = 200;       
       //xtm_printf("i = %d, Multi_Udp_Buff[i].SendNum = %d,  %d.%d.%d.%d, buf[6] = %d, buf[8] = %d, 0x%X %X %X %X %X %X\n", \
       //    i, Multi_Udp_Buff[i].SendNum, Multi_Udp_Buff[i].RemoteIP[0], Multi_Udp_Buff[i].RemoteIP[1], Multi_Udp_Buff[i].RemoteIP[2], Multi_Udp_Buff[i].RemoteIP[3],\
       //    Multi_Udp_Buff[i].buf[6], Multi_Udp_Buff[i].buf[8], \
       //    Multi_Udp_Buff[i].buf[0], Multi_Udp_Buff[i].buf[1], Multi_Udp_Buff[i].buf[2], Multi_Udp_Buff[i].buf[3], Multi_Udp_Buff[i].buf[4], Multi_Udp_Buff[i].buf[5]);
       if(Multi_Udp_Buff[i].SendNum >= Multi_Udp_Buff[i].MaxSendNum)
        {
           //xtm_printf("MultiUdpSendFailDeal i = %d\n", i);
           MultiUdpSendFailDeal(i);
        }
       else
        {
         if(Multi_Udp_Buff[i].MacValid == 0)
          {
           for(j=0; j<4; j++)
            {
             Net1[j] = LocalCfg.IP[j] & LocalCfg.IP_Mask[j];
             Net2[j] = Multi_Udp_Buff[i].RemoteIP[j] & LocalCfg.IP_Mask[j];
            }
           if((Net1[0] == Net2[0])&&(Net1[1] == Net2[1])&&(Net1[2] == Net2[2])&&(Net1[3] == Net2[3]))  //同一子网
             xtm_memcopy(RemoteIP1, Multi_Udp_Buff[i].RemoteIP, 4);
           else                                                                                        //不同子网
             xtm_memcopy(RemoteIP1, LocalCfg.IP_Gate, 4);
           if(SearchArp_Table(RemoteIP1, Multi_Udp_Buff[i].RemoteMac))
            {
             Multi_Udp_Buff[i].MacValid = 1;
            }
           else
             SendArp_Ask(RemoteIP1);   //发送 ARP请求表
          }
        }
      }
     Multi_Udp_Buff[i].TimeNum += 5;
     if(Multi_Udp_Buff[i].TimeNum >= Multi_Udp_Buff[i].DelayTime)
       Multi_Udp_Buff[i].TimeNum = 0;
    }
}
//---------------------------------------------------------------------------
void MultiUdpSendFailDeal(INT8U i)
{
#if 1
  INT8U k, j;
  INT8U Net1[4], Net2[4];
  INT8U RemoteIP[4], RemoteIP1[4];

  INT8U SendCapturePicFailFlag;
  SendCapturePicFailFlag = 0;  

  switch(Multi_Udp_Buff[i].buf[5])
    {
       case SMARK_MONITOR:
         if(Multi_Udp_Buff[i].buf[6] == 4)
            {
                 Multi_Udp_Buff[i].isValid = 0;
                 Local.OnlineFlag = 0;
                 Local.CallConfirmFlag = 0; //设置在线标志
                 xtm_printf("MultiUdpSendFailDeal:: CALLEND\n");
                 WatchEnd_ClearStatus();
                 return;
                
            }
        break;
    }
  
  switch(Multi_Udp_Buff[i].buf[6])
   {
    case REPORTSTATUS:
                            Multi_Udp_Buff[i].isValid = 0;
   #ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
                            CapturePic.ConnectCentered = 0;   //与中心连接状态
   #endif
                            //xtm_printf("设备定时报告状态发送失败\n");
                            break;
   #ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心

    case CAPTUREPIC_SEND_START:   //单元门口机、围墙机、二次门口机发送呼叫照片->发送开始
                            Multi_Udp_Buff[i].isValid = 0;
                            break;
    case CAPTUREPIC_SEND_DATA:   //单元门口机、围墙机、二次门口机发送呼叫照片->发送数据
                            Multi_Udp_Buff[i].isValid = 0;
                            //xtm_memcopy(&CurrNo, Multi_Udp_Buff[i].buf + 48, sizeof(CurrNo));
                            //xtm_memcopy(&CurrPack, Multi_Udp_Buff[i].buf + 56, sizeof(CurrPack));
                            SendCapturePicFailFlag = 1;
                            CapturePic.SendToCetnering = 0;  //正在传给中心
                            break;
    case CAPTUREPIC_SEND_SUCC:   //单元门口机、围墙机、二次门口机发送呼叫照片->发送结束（成功）
                            Multi_Udp_Buff[i].isValid = 0;
                            CapturePic.SendToCetnering = 0;  //正在传给中心
                            break;
    case CAPTUREPIC_SEND_FAIL:   //单元门口机、围墙机、二次门口机发送呼叫照片->发送结束（失败）
                            Multi_Udp_Buff[i].isValid = 0;
                            CapturePic.SendToCetnering = 0;  //正在传给中心
                            break;
   #endif   
    #ifdef _MULTI_MACHINE_SUPPORT  //多分机支持
    case HOSTTOSUB: //主机与副机通信
                 Multi_Udp_Buff[i].isValid = 0;
                 if(Multi_Udp_Buff[i].buf[8] == SYNCSUB)
                  {
                    Local.DenNum --;
                  }
                 xtm_printf("主副机通信失败, %d, %d, %d, %d.%d.%d.%d\n", Multi_Udp_Buff[i].buf[6], Multi_Udp_Buff[i].buf[8],  Multi_Udp_Buff[i].buf[44] - '0', \
                            Multi_Udp_Buff[i].RemoteIP[0], Multi_Udp_Buff[i].RemoteIP[1], Multi_Udp_Buff[i].RemoteIP[2], Multi_Udp_Buff[i].RemoteIP[3]);
                 break;
    #endif
    case NSORDER:
                 if(Multi_Udp_Buff[i].CurrOrder == 255) //主机向副机解析
                  {
                   Multi_Udp_Buff[i].isValid = 0;
                   xtm_printf("查找副机失败\n");
                  }
                 else
                  {
                   // Multi_Udp_Buff[i].isValid = 0;
                    xtm_printf("解析失败\n");
                //    DisplayCallFailWindow(0);    //显示呼叫失败窗口
                   /* if(Local.CurrentWindow == 16)
                     {
                      //  strcpy(Label_Call.Text, FindAddress1_Text[Local.LanguageNo]);
                      //  ShowOsdLabel24(&Label_Call, REFRESH, cWhite, 0);
                      display_text(20, 200, 0, COLOR_YELLOW, "解析失败", 0, 24, NOFULLPAGE);
                      //延时清提示信息标志
                      PicStatBuf.Type = Local.CurrentWindow;
                      PicStatBuf.Time = 0;
                      PicStatBuf.Flag = 1;
                     }    */
                   //若该命令为子网解析，则下一个命令为向服务器解析
                   Multi_Udp_Buff[i].SendNum = 0;
                   Multi_Udp_Buff[i].TimeNum = 0;
                   Multi_Udp_Buff[i].DelayTime = 600;  //等待时间  200ms
                   Multi_Udp_Buff[i].SendDelayTime = 0; //发送等待时间计数  20101112
                   //更改UDP端口
                   Multi_Udp_Buff[i].DestPort = LocalPort[0];

                   xtm_memcopy(RemoteIP, LocalCfg.IP_Server, 4);
                   for(j=0; j<4; j++)
                    {
                     Net1[j] = LocalCfg.IP[j] & LocalCfg.IP_Mask[j];
                     Net2[j] = RemoteIP[j] & LocalCfg.IP_Mask[j];
                    }
                   if((Net1[0] == Net2[0])&&(Net1[1] == Net2[1])&&(Net1[2] == Net2[2])&&(Net1[3] == Net2[3]))  //同一子网
                     xtm_memcopy(RemoteIP1, RemoteIP, 4);
                   else                                                                                        //不同子网
                     xtm_memcopy(RemoteIP1, LocalCfg.IP_Gate, 4);
           
                   if(SearchArp_Table(RemoteIP1, Multi_Udp_Buff[i].RemoteMac))
                    {
                      Multi_Udp_Buff[i].MacValid = 1;
                    }
                   else
                    {
                     //xtm_printf("SendArp_Ask::%d.%d.%d.%d, CurrOrder = %d\n", RemoteIP1[0], RemoteIP1[1], RemoteIP1[2], RemoteIP1[3], CurrOrder);
                     SendArp_Ask(RemoteIP1);   //发送 ARP请求表
                     Multi_Udp_Buff[i].ArpNum = 1;  //ARP次数
                    }
                   xtm_memcopy(Multi_Udp_Buff[i].RemoteIP, RemoteIP, 4);

                   //命令, 服务器解析
                   Multi_Udp_Buff[i].buf[6] = NSSERVERORDER;
                   xtm_printf("正在向NS服务器解析地址, %d.%d.%d.%d\n", Multi_Udp_Buff[i].RemoteIP[0], Multi_Udp_Buff[i].RemoteIP[1], \
                               Multi_Udp_Buff[i].RemoteIP[2], Multi_Udp_Buff[i].RemoteIP[3]);
                  }
                 break;
    case NSSERVERORDER: //服务器解析
                 Multi_Udp_Buff[i].isValid = 0;
                 xtm_printf("服务器解析失败, Local.Status = %d, Local.CurrentWindow = %d\n", Local.Status, Local.CurrentWindow);
                 if(Local.CurrentWindow == 0)
                  {
                   DisplayCallFailWindow(0);    //显示呼叫失败窗口
                  }

                 if((Local.Status == 0)&&(Local.CurrentWindow == 3))
                  {
                   Local.OnlineFlag = 0;
                   StopRecVideo();
                   DelayMs(50);
                   StopEncodeVideo();
                   StopVideoClock();            //关闭视频采集编码时钟
                   DisplayCallFailWindow(0);    //显示呼叫失败窗口
                  }
                 break;
    case VIDEOTALK:    //局域网可视对讲
                 xtm_printf("VIDEOTALK\n");
                 switch(Multi_Udp_Buff[i].buf[8])
                  {
                   case JOINGROUP:        //加入组播组（主叫方->被叫方，被叫方应答）
                                          Multi_Udp_Buff[i].isValid = 0;
                                          xtm_printf("加入组播组失败, %d\n", Multi_Udp_Buff[i].buf[6]);
                                          break;
                   case CALL:
                                            //有主副机，清其它的呼叫
                                            for(k=0; k<UDPSENDMAX; k++)
                                             if(Multi_Udp_Buff[k].isValid == 1)
                                              if(Multi_Udp_Buff[k].buf[8] == CALL)
                                               if(k != i)
                                                 Multi_Udp_Buff[k].isValid = 0;

                                            Multi_Udp_Buff[i].isValid = 0;
                                            if(Local.CurrentWindow == 0)
                                             {
                                              DisplayCallFailWindow(0);    //显示呼叫失败窗口
                                             }
                                          xtm_printf("呼叫失败, %d, %d, %d.%d.%d.%d\n", Multi_Udp_Buff[i].buf[6], Multi_Udp_Buff[i].buf[8], Multi_Udp_Buff[i].RemoteIP[0], Multi_Udp_Buff[i].RemoteIP[1], Multi_Udp_Buff[i].RemoteIP[2], Multi_Udp_Buff[i].RemoteIP[3]);
                                          break;
                   case CALLEND:  //通话结束
                   #ifdef _MULTI_MACHINE_SUPPORT  //多分机支持
                   case TURNTALK: //转接
                   #endif
                                          Multi_Udp_Buff[i].isValid = 0;
                                          Local.OnlineFlag = 0;
                                          Local.CallConfirmFlag = 0; //设置在线标志
                                          if(Multi_Udp_Buff[i].buf[8] == TURNTALK)
                                            xtm_printf("转接失败, %d, %d, RemoteHost = %d.%d.%d.%d\n", Multi_Udp_Buff[i].buf[6], Multi_Udp_Buff[i].buf[8], \
                                                   Multi_Udp_Buff[i].RemoteIP[0], Multi_Udp_Buff[i].RemoteIP[1], Multi_Udp_Buff[i].RemoteIP[2], Multi_Udp_Buff[i].RemoteIP[3]);
                                          //对讲结束，清状态和关闭音视频
                                          TalkEnd_ClearStatus(0);
                                          break;
                   default: //为其它命令，本次通信结束
                                          Multi_Udp_Buff[i].isValid = 0;
                                          xtm_printf("通信失败1, %d, %d, RemoteHost = %d.%d.%d.%d\n", Multi_Udp_Buff[i].buf[6], Multi_Udp_Buff[i].buf[8], \
                                                   Multi_Udp_Buff[i].RemoteIP[0], Multi_Udp_Buff[i].RemoteIP[1], Multi_Udp_Buff[i].RemoteIP[2], Multi_Udp_Buff[i].RemoteIP[3]);
                                          break;
                  }
                 break;
    case VIDEOWATCH:     //局域网监控
                           switch(Multi_Udp_Buff[i].buf[8])
                            {
                             case CALL:
                                            Multi_Udp_Buff[i].isValid = 0;
                                          //  Local.Status = 0;
                                            //延时清提示信息标志
                                            PicStatBuf.Type = Local.CurrentWindow;
                                            PicStatBuf.Time = 0;
                                            PicStatBuf.Flag = 1;

                                            //display_text(20, 200, 0, COLOR_YELLOW, "监视失败", 0, 24, NOFULLPAGE);
                                            /*if(Local.CurrentWindow == 13)
                                             {
                                               strcpy(Label_Call.Text, WatchFail_Text[Local.LanguageNo]);
                                               ShowOsdLabel24(&Label_Call, REFRESH, cWhite, 0);
                                             }     */
                                            xtm_printf("监视失败, %d, i = %d\n", Multi_Udp_Buff[i].buf[6], i);
                                          break;
                             case CALLEND:  //通话结束
                                          Multi_Udp_Buff[i].isValid = 0;
                                          Local.OnlineFlag = 0;
                                          Local.CallConfirmFlag = 0; //设置在线标志
                                          xtm_printf("MultiUdpSendFailDeal:: CALLEND\n");
                                          WatchEnd_ClearStatus();
                                          break;
                             default: //为其它命令，本次通信结束
                                          Multi_Udp_Buff[i].isValid = 0;
                                          xtm_printf("通信失败2, %d\n", Multi_Udp_Buff[i].buf[6]);
                                          break;
                            }
                           break;
    default: //为其它命令，本次通信结束
                           Multi_Udp_Buff[i].isValid = 0;
                      //     Local.Status = 0;
                           if(Multi_Udp_Buff[i].buf[6] != REPORTSTATUS)
                             xtm_printf("通信失败3, %d\n", Multi_Udp_Buff[i].buf[6]);
                           break;
   }
 #ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
  if(SendCapturePicFailFlag == 1)
   {
    //printf("3333333333, CurrNo = %d, CurrPack = %d\n", CurrNo, CurrPack);
    SendCapturePicFinish(0, 0);
   }
  SendCapturePicFailFlag = 0;
 #endif   
#endif   
}
//---------------------------------------------------------------------------
INT8U ClearMultiUdpBuff(INT8U Order1, INT8U Order2, INT8U Order3, INT8U *cFromIP)   //Clear UDP发送缓存
{
  INT8U i, clear_ok;

  clear_ok = 0;
     for(i=0; i<UDPSENDMAX; i++)
      if(Multi_Udp_Buff[i].isValid == 1)
        if(Multi_Udp_Buff[i].SendNum  < MAXSENDNUM)
         if(Multi_Udp_Buff[i].buf[6] == Order1)
          if((Multi_Udp_Buff[i].buf[7] & 0x03) == Order2)
           if(Multi_Udp_Buff[i].buf[8] == Order3)
            if((Multi_Udp_Buff[i].RemoteIP[0] == cFromIP[0])&&(Multi_Udp_Buff[i].RemoteIP[1] == cFromIP[1]) \
              &&(Multi_Udp_Buff[i].RemoteIP[2] == cFromIP[2])&&(Multi_Udp_Buff[i].RemoteIP[3] == cFromIP[3]))
             {
              Multi_Udp_Buff[i].isValid = 0;

              clear_ok = 1;
             }
  return clear_ok;
}
//-----------------------------------------------------------------------
#if 0
INT8U UdpSendBuff(INT16U DestPort, INT8U *RemoteMac, INT8U *RemoteIP, INT8U *udpdata, INT16U nlength)
{
	INT16U i,k,val;
	INT8U ip_head[20];
	INT8U ipchecksumbit[5];
	INT32U udpchecksum;
	INT16U udpchecksumn;
        INT16U crc_len;
	INT8U UDP_data[MAX_UDP_LENGTH];

        //UDP 数据必须为偶数字节
        if((nlength % 2) == 1)
          nlength ++;
        //nlength = 8;
        if(nlength < SMALLESTSIZE)
          nlength = SMALLESTSIZE;

        /*INT8U DestMac[6];

        //xtm_printf("UdpSendBuff 1:: \n");
        if(!SearchArp_Table(RemoteIP, DestMac))
         {
          xtm_printf("UdpSendBuff:: ARP Fail\n");
          return 0;
         }
        else
          xtm_printf("UdpSendBuff:: ARP %d.%d.%d.%d  Mac:%X:%X:%X:%X:%X:%X\n", RemoteIP[0], RemoteIP[1], RemoteIP[2], RemoteIP[3], \
                      DestMac[0], DestMac[1], DestMac[2], DestMac[3], DestMac[4], DestMac[5]);      */
	ip_head[0] = 0x45;          //Ver HLEN
	ip_head[1] = 0x00;
        i=nlength+28;
	ip_head[2] = i>>8;
	ip_head[3] = i&0xff;
        ip_head[4]= (ip_pack_no >> 8) & 0xff;
        ip_head[5]= ip_pack_no & 0xff;   //IP首部标识
        ip_pack_no ++;
	ip_head[6] = 0x00;
	ip_head[7] = 0x00;
	ip_head[8] = 0x40;
	ip_head[9] = 0x11;
	ip_head[10] = 0x00;       //checksum
	ip_head[11] = 0x00;
	ip_head[12] = LocalCfg.IP[0];     //Source IP
	ip_head[13] = LocalCfg.IP[1];
	ip_head[14] = LocalCfg.IP[2];
	ip_head[15] = LocalCfg.IP[3];

        ip_head[16] = RemoteIP[0];          //Destination IP
        ip_head[17] = RemoteIP[1];
        ip_head[18] = RemoteIP[2];
        ip_head[19] = RemoteIP[3];


        Build_Udp_Ip_Crc(ip_head);
/*  for(i=0; i<5; i++){
	
	  ipchecksumbit[i] = 0x00; 
	
 }

  for(i=1; i<20; ){
	  ipchecksumbit[0] += (ip_head[i]&0x0f); 
	  ipchecksumbit[1] += (ip_head[i]>>4);
	  i += 2;
 }	

  for(i=0; i<19; ){
	  ipchecksumbit[2] += (ip_head[i]&0x0f); 
	  ipchecksumbit[3] += (ip_head[i]>>4);
	  i += 2;
 }	  

 ipchecksumbit[1] = (ipchecksumbit[0]>>4) + ipchecksumbit[1];
 ipchecksumbit[2] = (ipchecksumbit[1]>>4) + ipchecksumbit[2];
 ipchecksumbit[3] = (ipchecksumbit[2]>>4) + ipchecksumbit[3];
 ipchecksumbit[0] = (ipchecksumbit[3]>>4) + ipchecksumbit[0];

 ip_head[10]= ~((ipchecksumbit[3]<<4) + (ipchecksumbit[2]&0x0f) );
 ip_head[11]= ~((ipchecksumbit[1]<<4) + (ipchecksumbit[0]&0x0f) );     */
 
	outportb(BYTE_WR_MOST_ADD, 0x1a);
	outportb(BYTE_WR_HIGH_ADD, 0x00);

	outportb(BYTE_WR_LOW_ADD, 0x40);
	
//==========
//MAC header
//==========

//	outportb(BYTE_WR_LOW_ADD, 6);
	outportb(BYTE_WR_DATA, RemoteMac[0]);
//	outportb(BYTE_WR_LOW_ADD, 7);
	outportb(BYTE_WR_DATA, RemoteMac[1]);
//	outportb(BYTE_WR_LOW_ADD, 8);
	outportb(BYTE_WR_DATA, RemoteMac[2]);
//	outportb(BYTE_WR_LOW_ADD, 9);
	outportb(BYTE_WR_DATA, RemoteMac[3]);
//	outportb(BYTE_WR_LOW_ADD, 10);
	outportb(BYTE_WR_DATA, RemoteMac[4]);
//	outportb(BYTE_WR_LOW_ADD, 11);
	outportb(BYTE_WR_DATA, RemoteMac[5]);

	outportb(BYTE_WR_DATA, LocalCfg.Mac_Addr[0]);
//	outportb(BYTE_WR_LOW_ADD, 1);
	outportb(BYTE_WR_DATA, LocalCfg.Mac_Addr[1]);
//	outportb(BYTE_WR_LOW_ADD, 2);
	outportb(BYTE_WR_DATA, LocalCfg.Mac_Addr[2]);
//	outportb(BYTE_WR_LOW_ADD, 3);
	outportb(BYTE_WR_DATA, LocalCfg.Mac_Addr[3]);
//	outportb(BYTE_WR_LOW_ADD, 4);
	outportb(BYTE_WR_DATA, LocalCfg.Mac_Addr[4]);
//	outportb(BYTE_WR_LOW_ADD, 5);
	outportb(BYTE_WR_DATA, LocalCfg.Mac_Addr[5]);


//	outportb(BYTE_WR_LOW_ADD, 12);
	outportb(BYTE_WR_DATA, 0x08);
//	outportb(BYTE_WR_LOW_ADD, 13);
	outportb(BYTE_WR_DATA, 0x00);
//=========
//IP header
//=========


for(i=0;i<20;i++){

	 outportb(BYTE_WR_DATA, ip_head[i]);
	 
}

   UDP_data[0] = LocalCfg.IP[0];    // P_headpart
   UDP_data[1] = LocalCfg.IP[1];
   UDP_data[2] = LocalCfg.IP[2];
   UDP_data[3] = LocalCfg.IP[3];
   
   UDP_data[4] = RemoteIP[0];
   UDP_data[5] = RemoteIP[1];
   UDP_data[6] = RemoteIP[2];
   UDP_data[7] = RemoteIP[3];
   
   UDP_data[8]=0x00;
   UDP_data[9]=0x11;
   i = (nlength+8);
   UDP_data[10]= i>>8;
   UDP_data[11]= i&0xff;     //
   
   UDP_data[12] = (DestPort >> 8)& 0xff;
   UDP_data[13] = DestPort & 0xff;
   UDP_data[14] = (DestPort >> 8)& 0xff;
   UDP_data[15] = DestPort & 0xff;

   UDP_data[16] = i>>8;
   UDP_data[17]= i&0xff;
   UDP_data[18] = 0x00;
   UDP_data[19] = 0x00;

 /*  if((nlength % 2) == 0)
     crc_len = nlength;
   else
    {
     crc_len = nlength + 1;
     udpdata[crc_len-1] = 0;
    }
for(i=0;i<crc_len;i++){

	 UDP_data[i+20] = udpdata[i];
}
//for(i=nlength;i<MAX_UDP_LENGTH;i++)               ////最大 UDP 包128字节
//	UDP_data[i+20]=0;
k=0;

udpchecksum = 0x00000000;
while(k<crc_len){
 		val=UDP_data[k];
 		val<<=8;
 		if (k<nlength)
 			val+=UDP_data[k+1];
    udpchecksum += val;
    k+=2;
}

 while(udpchecksum>>16){
 	  udpchecksum = ( (udpchecksum>>16) + (udpchecksum&0xffff) );
}
 val= udpchecksum&0xffff;
 udpchecksumn = ~val ;

 UDP_data[18]= udpchecksumn>>8;
 UDP_data[19]= udpchecksumn;    */

 // for(i=0;i<nlength;i++){
 //	 UDP_data[i+20] = udpdata[i];
 // }
//  Build_Udp_Crc(UDP_data, nlength);

//xtm_printf("UdpSendBuff:: UDP_data[18] = %X  UDP_data[19] = %X , %X, %X, %X, %X, %X, %X\n", \
//          UDP_data[18], UDP_data[19],  UDP_data[20],  UDP_data[21],  UDP_data[22],  UDP_data[23],  UDP_data[24],  UDP_data[25]);

        UDP_data[18]= 0;
        UDP_data[19]= 0;
    //    for(i=0;i<(nlength + 8);i++){
    //	  outportb(BYTE_WR_DATA, UDP_data[i+12]);
    //     }
        for(i=0;i<8;i++){
    	  outportb(BYTE_WR_DATA, UDP_data[i+12]);
         }
        for(i=0;i<nlength;i++){
     	  outportb(BYTE_WR_DATA, udpdata[i]);
         }
//xtm_printf("UdpSendBuff:: %X  %X  %X  %X,  len = %d\n", \
//          UDP_data[nlength+16], UDP_data[nlength+17], UDP_data[nlength+18], UDP_data[nlength+19], nlength + 28 + 14);

        i=nlength + 28 + 14;
	outport(TXPL_LOW, i&0xff);
	outport(TXPL_HIGH, i>>8);
//	outportb(TX_START, 1);
	outportb(TX_START, inportb(TX_START)|0x01);

	while(inportb(TX_STATUS)&0x01);
//	outportb(TX_START, 0x0);
 	  outportb(TX_START, inportb(TX_START)&0xfe);

        //xtm_printf("UdpSendBuff1 i= %d\n", i);
}
#endif
//---------------------------------------------------------------------------
INT8U UdpSendBuff_1(INT16U DestPort, INT8U *RemoteMac, INT8U *RemoteIP, INT8U *udpdata, INT16U nlength)
{
  INT16U i;

  //UDP 数据必须为偶数字节
  if((nlength % 2) == 1)
    nlength ++;
  //nlength = 8;
  if(nlength < SMALLESTSIZE)
    nlength = SMALLESTSIZE;

  //给管理软件发送的保持最小字节为512  
  if(DestPort == RemotePort[0])
   if(nlength < 512)
     nlength = 512;
         
//==========
//MAC header
//==========
  for(i=0; i<6; i++)
   {
    send_mac_buf[i] = RemoteMac[i];
    send_mac_buf[i+6] = LocalCfg.Mac_Addr[i];
   }
  send_mac_buf[12] = 0x08;
  send_mac_buf[13] = 0x00;

//=========
//IP header
//=========
  send_mac_buf[14] = 0x45;          //Ver HLEN
  send_mac_buf[15] = 0x00;
  i = nlength+28;
  send_mac_buf[16] = i>>8;
  send_mac_buf[17] = i&0xff;
  send_mac_buf[18]= (ip_pack_no >> 8) & 0xff;
  send_mac_buf[19]= ip_pack_no & 0xff;   //IP首部标识

  send_mac_buf[20] = 0x00;
  send_mac_buf[21] = 0x00;
  send_mac_buf[22] = 0x40;
  send_mac_buf[23] = 0x11;
  send_mac_buf[24] = 0x00;       //checksum
  send_mac_buf[25] = 0x00;
  for(i=0; i<4; i++)
   {
    send_mac_buf[26+i] = LocalCfg.IP[i];     //Source IP
    send_mac_buf[30+i] = RemoteIP[i];          //Destination IP
   }

  Build_Udp_Ip_Crc(send_mac_buf + 14);

#if 0
   UDP_data[0] = LocalCfg.IP[0];    // P_headpart
   UDP_data[1] = LocalCfg.IP[1];
   UDP_data[2] = LocalCfg.IP[2];
   UDP_data[3] = LocalCfg.IP[3];

   UDP_data[4] = RemoteIP[0];
   UDP_data[5] = RemoteIP[1];
   UDP_data[6] = RemoteIP[2];
   UDP_data[7] = RemoteIP[3];

   UDP_data[8]=0x00;
   UDP_data[9]=0x11;
   i = (nlength+8);
   UDP_data[10]= i>>8;
   UDP_data[11]= i&0xff;     //

   UDP_data[12] = (DestPort >> 8)& 0xff;
   UDP_data[13] = DestPort & 0xff;
   UDP_data[14] = (DestPort >> 8)& 0xff;
   UDP_data[15] = DestPort & 0xff;

   UDP_data[16] = i>>8;
   UDP_data[17]= i&0xff;
   UDP_data[18] = 0x00;
   UDP_data[19] = 0x00;
#endif

   send_mac_buf[34] = (DestPort >> 8)& 0xff;
   send_mac_buf[35] = DestPort & 0xff;
   send_mac_buf[36] = (DestPort >> 8)& 0xff;
   send_mac_buf[37] = DestPort & 0xff;

   i = (nlength+8);
   send_mac_buf[38] = i>>8;
   send_mac_buf[39]= i&0xff;
   send_mac_buf[40] = 0x00;
   send_mac_buf[41] = 0x00;

   send_mac_packet(42, send_mac_buf, nlength, udpdata);

//   xtm_printf("UdpSendBuff1 nlength= %d\n", nlength);
//   xtm_printf("UdpSendBuff1 i\n");
   return 0;
}
//---------------------------------------------------------------------------
INT8U UdpSendBuff_Test(INT16U DestPort, INT8U *RemoteMac, INT8U *RemoteIP, INT8U *udpdata, INT16U nlength)
{
  INT16U i;

  //UDP 数据必须为偶数字节
  if((nlength % 2) == 1)
    nlength ++;
  //nlength = 8;
  //if(nlength < SMALLESTSIZE)
  //  nlength = SMALLESTSIZE;

//==========
//MAC header
//==========
  for(i=0; i<6; i++)
   {
    send_mac_buf[i] = RemoteMac[i];
    send_mac_buf[i+6] = LocalCfg.Mac_Addr[i];
   }
  send_mac_buf[12] = 0x08;
  send_mac_buf[13] = 0x00;

//=========
//IP header
//=========
  send_mac_buf[14] = 0x45;          //Ver HLEN
  send_mac_buf[15] = 0x00;
  i = nlength+28;
  send_mac_buf[16] = i>>8;
  send_mac_buf[17] = i&0xff;
  send_mac_buf[18]= (ip_pack_no >> 8) & 0xff;
  send_mac_buf[19]= ip_pack_no & 0xff;   //IP首部标识

  send_mac_buf[20] = 0x00;
  send_mac_buf[21] = 0x00;
  send_mac_buf[22] = 0x40;
  send_mac_buf[23] = 0x11;
  send_mac_buf[24] = 0x00;       //checksum
  send_mac_buf[25] = 0x00;
  for(i=0; i<4; i++)
   {
    send_mac_buf[26+i] = LocalCfg.IP[i];     //Source IP
    send_mac_buf[30+i] = RemoteIP[i];          //Destination IP
   }

  Build_Udp_Ip_Crc(send_mac_buf + 14);

#if 0
   UDP_data[0] = LocalCfg.IP[0];    // P_headpart
   UDP_data[1] = LocalCfg.IP[1];
   UDP_data[2] = LocalCfg.IP[2];
   UDP_data[3] = LocalCfg.IP[3];

   UDP_data[4] = RemoteIP[0];
   UDP_data[5] = RemoteIP[1];
   UDP_data[6] = RemoteIP[2];
   UDP_data[7] = RemoteIP[3];

   UDP_data[8]=0x00;
   UDP_data[9]=0x11;
   i = (nlength+8);
   UDP_data[10]= i>>8;
   UDP_data[11]= i&0xff;     //

   UDP_data[12] = (DestPort >> 8)& 0xff;
   UDP_data[13] = DestPort & 0xff;
   UDP_data[14] = (DestPort >> 8)& 0xff;
   UDP_data[15] = DestPort & 0xff;

   UDP_data[16] = i>>8;
   UDP_data[17]= i&0xff;
   UDP_data[18] = 0x00;
   UDP_data[19] = 0x00;
#endif

   send_mac_buf[34] = (DestPort >> 8)& 0xff;
   send_mac_buf[35] = DestPort & 0xff;
   send_mac_buf[36] = (DestPort >> 8)& 0xff;
   send_mac_buf[37] = DestPort & 0xff;

   i = (nlength+8);
   send_mac_buf[38] = i>>8;
   send_mac_buf[39]= i&0xff;
   send_mac_buf[40] = 0x00;
   send_mac_buf[41] = 0x00;

   send_mac_packet(42, send_mac_buf, nlength, udpdata);

//   xtm_printf("UdpSendBuff1 nlength= %d\n", nlength);
//   xtm_printf("UdpSendBuff1 i\n");
   return 0;
}
//---------------------------------------------------------------------------
INT8U Build_Udp_Ip_Crc(INT8U *send_buf)
{
  INT16U i;
  INT16U sum1 = 0;
  INT16U sum2 = 0;
  for(i=0;i<20;i+=2)
   {
    sum1 = sum1 + send_buf[i];
    sum2 = sum2 + send_buf[i + 1];
   }
  //xtm_printf("Build_Udp_Ip_Crc 1:: sum1= %X, sum2= %X\n", sum1, sum2);
  for(i=0; i<2; i++)
   {
    if(sum1 >= 256)
     {
      sum2 = sum2 + (sum1 >> 8);
      sum1 = sum1 & 0xff;
     }
    if(sum2 >= 256)
     {
      sum1 = sum1 + (sum2 >> 8);
      sum2 = sum2 & 0xff;
     }
   }
  //xtm_printf("Build_Ip_Crc 3:: sum1= %X, sum2= %X\n", sum1, sum2);
  send_buf[10] = 0xff - sum1;
  send_buf[11] = 0xff - sum2;
  return 1;
}
//---------------------------------------------------------------------------
//udp 校验
INT8U Build_Udp_Crc(INT8U *send_buf, INT16U data_len)
{
  INT16U i;
  INT16U sum1 = 0;
  INT16U sum2 = 0;
  INT32U sum;
  if((data_len%2)!=0)      //数据为奇数个，则补一个字节
   {
    send_buf[20+data_len]=0;
    for(i=0;i<(20+data_len+1);i+=2)
     {
      sum1 = sum1 + send_buf[i];
      sum2 = sum2 + send_buf[i + 1];
     }
   }
  else
   {
    for(i=0;i<(20+data_len);i+=2)
     {
      sum1 = sum1 + send_buf[i];
      sum2 = sum2 + send_buf[i + 1];
     }
   }
  xtm_printf("Build_Udp_Crc:: sum1= %X, sum2= %X\n", sum1, sum2);

  sum = (sum1 << 8)+ sum2;
  sum = (sum & 0xffff) + ((sum >> 16) & 0xffff);
  sum = (sum & 0xffff) + ((sum >> 16) & 0xffff);
  sum1 = (sum >> 8) & 0xff;
  sum2 = sum  & 0xff;
 /* for(i=0; i<2; i++)
   {
    if(sum1 >= 256)
     {
      sum2 = sum2 + (sum1 >> 8);
      sum1 = sum1 & 0xff;
     }
    if(sum2 >= 256)
     {
      sum1 = sum1 + (sum2 >> 8);
      sum2 = sum2 & 0xff;
     }
   }    */
  send_buf[18] = 0xff - sum1;
  send_buf[19] = 0xff - sum2;
  return 1;
}
//---------------------------------------------------------------------------
//发送远程调试信息
void RecvRemoteDebugInfo_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT16U sendlength;
  char FlagText[50];
  xtm_memcopy(FlagText, recv_buf + 9, 20);

  if((FlagText[0] == FLAGTEXT[0])&&(FlagText[1] == FLAGTEXT[1])&&(FlagText[2] == FLAGTEXT[2])
     &&(FlagText[3] == FLAGTEXT[3])&&(FlagText[4] == FLAGTEXT[4])&&(FlagText[5] == FLAGTEXT[5]))
   {
    //xtm_printf("RecvDownLoadFile_Func::recv_buf[8] = 0x%X\n", recv_buf[8]);
    xtm_memcopy(send_b, recv_buf, length);
    send_b[7] = REPLY;    //应答
    sendlength = length;
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
    switch(recv_buf[8])
     {
      case STOPDEBUG:       //停止
             xtm_printf("收到远程调试信息 停止\n");
             Local.RemoteDebugInfo = 0;      //发送远程调试信息
             break;
      case STARTDEBUG:       //开始
             Local.RemoteDebugInfo = 1;      //发送远程调试信息
             xtm_memcopy(Local.DebugIP, cFromIP, 4);
             SearchArp_Table(Local.DebugIP, Local.DebugMac);
             xtm_printf("收到远程调试信息 开始, %s, Local.DebugIP = %d.%d.%d.%d\n", LocalCfg.Addr, Local.DebugIP[0], Local.DebugIP[1], Local.DebugIP[2], Local.DebugIP[3]);
             break;
      case REMOTE_REBOOT:       //重启动
             xtm_printf("收到远程调试信息 重启动\n");
             //sync();
             //system("reboot");
             break;
     }
   }
}
//---------------------------------------------------------------------------
void send_debug_info(char *debug_info)
{
  INT16U sendlength;
  INT32U infolen;
  INT8U RemoteMac[6];
  //xtm_printf("send_debug_info::Local.RemoteDebugInfo = %d, debug_info = %s\n", Local.RemoteDebugInfo, debug_info);
  if(Local.RemoteDebugInfo == 1)      //发送远程调试信息
   {
    xtm_memcopy(send_b, UdpPackageHead, 6);
    send_b[6] = REMOTEDEBUGINFO;
    send_b[7] = ASK;    //主叫
    send_b[8] = DEBUGDATA;    //子命令
    xtm_memcopy(send_b + 9, FLAGTEXT, 20);       //标志
    infolen = strlen(debug_info);
    xtm_memcopy(send_b + 29, &infolen, 4);
    xtm_memcopy(send_b + 33, debug_info, infolen);
    sendlength = 33 + infolen;
  #if 0
    CreateMultiUdpBuff(RemotePort[0], SINGLE_SEND, Local.DebugIP, send_b, sendlength, 0);
  #else
    UdpSendBuff_1(RemotePort[0], Local.DebugMac, Local.DebugIP, send_b, sendlength);
  #endif
   }
}
//---------------------------------------------------------------------------
#ifdef _REMOTECALLTEST  //远程呼叫测试
//接收远程呼叫测试　
void RecvRemoteCallTest_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)
{
  INT8U RemoteAddr[20];
  INT8U FlagText[50];

  xtm_printf("RecvRemoteCallTest_Func\n");
  xtm_memcopy(FlagText, recv_buf + 9, 20);
  if((FlagText[0] == FLAGTEXT[0])&&(FlagText[1] == FLAGTEXT[1])&&(FlagText[2] == FLAGTEXT[2])
     &&(FlagText[3] == FLAGTEXT[3])&&(FlagText[4] == FLAGTEXT[4])&&(FlagText[5] == FLAGTEXT[5]))
   {
    if(Local.LcdLightFlag == 0)
     {
      //点亮LCD背光
      Lcd_Power(1, LocalCfg.BrightVolume);
      Local.LcdLightFlag = 1; //LCD背光标志
      Local.LcdLightTime = 0; //时间
     }
        
    xtm_memcopy(RemoteAddr, recv_buf + 29 + 1 + 6, 4);
    RemoteAddr[19] = '\0';
    xtm_printf("接收远程呼叫测试 %s, Local.Status = %d\n", RemoteAddr, Local.Status);
    if(Local.Status == 0)
     {
      Local.EnterTestMode = 1;  //测试模式, 5秒自动接听;
      Local.CloseTestRing = 1;  //测试模式, 关闭铃声

      switch(recv_buf[8])
       {
        case STARTTEST:
              if(recv_buf[29] == 'Z')
               {
                xtm_memcopy(RemoteAddr, recv_buf + 29, 12);
                RemoteAddr[12] = '\0';
                Call_Func(1, RemoteAddr, RemoteAddr);     //呼叫中心
               }
              else
                Call_Func(2, RemoteAddr, "");     //呼叫住户
              break;
        case ENTERTESTMODE:
              xtm_printf("RecvRemoteCallTest_Func::Local.EnterTestMode = %d\n", Local.EnterTestMode);
              break;
       }
     }
    else
     {
      switch(recv_buf[8])
       {
        case STOPCALL:
              xtm_printf("RecvRemoteCallTest_Func::STOPCALL\n");
              if(Local.Status != 0)
               {
                 TalkEnd_Func();
               }
              break;
       }
     }
   }
}
#endif
//---------------------------------------------------------------------------
//解析
void RecvNSAsk_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //解析请求
{
  INT8U i,j;
  INT8U isAddrOK;
  //INT8U send_b[1520];
  INT16U sendlength;
  isAddrOK = 1;
  for(j=8; j<(8+Local.AddrLen); j++)
   if(LocalCfg.Addr[j-8] != recv_buf[j])
   {
    isAddrOK = 0;
    break;
   }
  //xtm_printf("RecvNSAsk_Func 1:: isAddrOK = %d, Local.AddrLen = %d\n", isAddrOK, Local.AddrLen);
  //xtm_printf("RecvNSAsk_Func 2:: %X %X %X %X %X %X %X %X %X %X %X --- %X %X %X %X %X %X %X %X %X %X %X --- %X %X %X %X %X %X %X %X %X %X %X\n", LocalCfg.Addr[0], LocalCfg.Addr[1], LocalCfg.Addr[2], LocalCfg.Addr[3], LocalCfg.Addr[4], LocalCfg.Addr[5], LocalCfg.Addr[6], LocalCfg.Addr[7], LocalCfg.Addr[8], LocalCfg.Addr[9], LocalCfg.Addr[10], \
  //    recv_buf[8], recv_buf[9], recv_buf[10], recv_buf[11], recv_buf[12], recv_buf[13], recv_buf[14], recv_buf[15], recv_buf[16], recv_buf[17], recv_buf[18], \
  //    recv_buf[32], recv_buf[33], recv_buf[34], recv_buf[35], recv_buf[36], recv_buf[37], recv_buf[38], recv_buf[39], recv_buf[40], recv_buf[41], recv_buf[42]);
  //不是本机所发
  if(isAddrOK == 0)
   {
    isAddrOK = 1;

    for(j=32; j<(32+Local.AddrLen); j++)
     if(LocalCfg.Addr[j-32] != recv_buf[j])
      {
       isAddrOK = 0;
       break;
      }
    xtm_printf("RecvNSAsk_Func 2:: isAddrOK = %d, Local.AddrLen = %d, LocalCfg.Addr = %s\n", isAddrOK, Local.AddrLen, LocalCfg.Addr);
  //要求解析的是本机地址
    if(isAddrOK == 1)
     {
      xtm_memcopy(send_b, recv_buf, 32);
      send_b[7] = Local.MachineType | REPLY;    //应答

      if(Local.isHost == '0')  //主机
       {
        send_b[32] = Local.DenNum + 1;   //地址个数

        xtm_memcopy(send_b + 33, LocalCfg.Addr, 20);
        xtm_memcopy(send_b + 53, LocalCfg.IP, 4);
        for(i=0; i<Local.DenNum; i++)
         {
          xtm_memcopy(send_b + 57 + 24*i, Local.DenAddr[i], 20);
          xtm_memcopy(send_b + 57 + 20 +24*i, Local.DenIP[i], 4);
         }
        sendlength = 57 + 24*Local.DenNum;
       }
      else                    //副机
       {
        send_b[32] = 1;   //地址个数

        xtm_memcopy(send_b + 33, LocalCfg.Addr, 20);
        xtm_memcopy(send_b + 53, LocalCfg.IP, 4);
        sendlength = 57;
       }
      CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
      xtm_printf("Local.isHost = 0x%X, Local.DenNum = %d, cFromIP = %d.%d.%d.%d\n", Local.isHost, Local.DenNum, cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
     }
  }
}
//-----------------------------------------------------------------------
//解析
void RecvNSAsk2_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  
//解析请求
{
	INT8U i,j;
	INT8U isAddrOK;
	char tmp_text[20];
	INT16U sendlength,DestPort2;
	isAddrOK = 1;
	for(j=8; j<(8+Local.AddrLen); j++)
		if(LocalCfg.Addr[j-8] != recv_buf[j])
		{
			isAddrOK = 0;
			break;
		}
		//xtm_printf("RecvNSAsk_Func 1:: isAddrOK = %d, Local.AddrLen = %d\n", isAddrOK, Local.AddrLen);
		//xtm_printf("RecvNSAsk_Func 2:: %X %X %X %X %X %X %X %X %X %X %X --- %X %X %X %X %X %X %X %X %X %X %X --- %X %X %X %X %X %X %X %X %X %X %X\n", LocalCfg.Addr[0], LocalCfg.Addr[1], LocalCfg.Addr[2], LocalCfg.Addr[3], LocalCfg.Addr[4], LocalCfg.Addr[5], LocalCfg.Addr[6], LocalCfg.Addr[7], LocalCfg.Addr[8], LocalCfg.Addr[9], LocalCfg.Addr[10], \
		//    recv_buf[8], recv_buf[9], recv_buf[10], recv_buf[11], recv_buf[12], recv_buf[13], recv_buf[14], recv_buf[15], recv_buf[16], recv_buf[17], recv_buf[18], \
		//    recv_buf[32], recv_buf[33], recv_buf[34], recv_buf[35], recv_buf[36], recv_buf[37], recv_buf[38], recv_buf[39], recv_buf[40], recv_buf[41], recv_buf[42]);
		//不是本机所发
		if(isAddrOK == 0)
		{
			isAddrOK = 1;

			for(j=32; j<(32+Local.AddrLen); j++)
				if(LocalCfg.Addr[j-32] != recv_buf[j])
				{
					isAddrOK = 0;
					break;
				}
				xtm_memcopy(tmp_text, recv_buf + 32, 12);
				tmp_text[12] = '\0';
				xtm_printf("RecvNSAsk_Func 2:: isAddrOK = %d, Local.AddrLen = %d, LocalCfg.Addr = %s, Dest = %s\n", isAddrOK, Local.AddrLen, LocalCfg.Addr, tmp_text);
				//要求解析的是本机地址
				if(isAddrOK == 1)
				{
					xtm_memcopy(send_b, recv_buf, 32);
					send_b[7] = Local.MachineType | REPLY;    //应答

                    DestPort2 = recv_buf[143];
                    DestPort2 <<= 8;
                    DestPort2 += recv_buf[142];
                    
                    LocalPort[2] = DestPort2;



					if(Local.isHost == '0')  //主机
					{
						send_b[32] = Local.DenNum + 1;   //地址个数

						xtm_memcopy(send_b + 33, LocalCfg.Addr, 20);
						xtm_memcopy(send_b + 53, LocalCfg.IP, 4);
						for(i=0; i<Local.DenNum; i++)
						{
							xtm_memcopy(send_b + 57 + 24*i, Local.DenAddr[i], 20);
							xtm_memcopy(send_b + 57 + 20 +24*i, Local.DenIP[i], 4);
						}
						sendlength = 57 + 24*Local.DenNum;
					}
					else                    //副机
					{
						send_b[32] = 1;   //地址个数

						xtm_memcopy(send_b + 33, LocalCfg.Addr, 20);
						xtm_memcopy(send_b + 53, LocalCfg.IP, 4);
						sendlength = 57;
					}

					CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
					xtm_printf("Local.isHost = 0x%X, Local.DenNum = %d, cFromIP = %d.%d.%d.%d\n", Local.isHost, Local.DenNum, cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
				}
		}
}
//---------------------------------------------------------------------------
void RecvNSReply_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)//解析应答
{
  int i,j,k;
  int CurrOrder;
  int isAddrOK;
  int AddrNo;
  int DenAddrLen;
  char Old_Addr[21], New_Addr[21];
  //INT8U send_b[128];
  INT16U sendlength;
  INT8U Order;

  xtm_printf("RecvNSReply_Func:: %d.%d.%d.%d, recv_buf[32] = %d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3], recv_buf[32]);

  //暂时关闭本地解析，试验服务器解析
  #ifdef _TESTNSSERVER
   if(DestPort == 8302)
     return;
  #endif

  isAddrOK = 0;
  for(i=0; i<UDPSENDMAX; i++)
   if(Multi_Udp_Buff[i].isValid == 1)
     if(Multi_Udp_Buff[i].SendNum  < MAXSENDNUM)
      if((Multi_Udp_Buff[i].buf[6] == NSORDER)||(Multi_Udp_Buff[i].buf[6] == NSSERVERORDER))
       if(((Multi_Udp_Buff[i].buf[7] & 0x03) == ASK)&&(recv_buf[32] > 0))
        {
         //判断要求解析地址是否匹配
         switch(Multi_Udp_Buff[i].buf[32])
          {
           case 'S':
           case 'B':
           case 'H':
                    DenAddrLen = 12;
                    break;
           case 'W':
           case 'Z':
                    DenAddrLen = 5;
                    break;
           case 'M':
                    DenAddrLen = 8;
                    break;
          }
         AddrNo = 0;
         isAddrOK = 0;
         xtm_memcopy(Old_Addr, Multi_Udp_Buff[i].buf + 32, DenAddrLen);
         Old_Addr[DenAddrLen] = '\0';
         for(k=0; k<recv_buf[32]; k++)
          {
           xtm_memcopy(New_Addr, recv_buf + 32 + 1 + k*24, DenAddrLen);
           New_Addr[DenAddrLen] = '\0';
           if(strcmp(Old_Addr, New_Addr) == 0)
            {
             isAddrOK = 1;
             AddrNo = k;
             break;
            }
          }  
         if(isAddrOK == 1)
          {
           CurrOrder = Multi_Udp_Buff[i].CurrOrder;
           Multi_Udp_Buff[i].isValid = 0;
           Multi_Udp_Buff[i].SendNum = 0;
           break;
          }
        }

  if(Local.OnlineTestFlag == 1)
   {
    xtm_memcopy(Local.OnlineRecvAddr[Local.OnlineRecvNum], recv_buf + 32 + 1 + 7, 4);
    Local.OnlineRecvNum ++;
    return;
   } 

  if(isAddrOK == 1)
   { //收到正确的解析回应
       if(CurrOrder == VIDEOTALK) //对讲
        {
           Remote.DenNum = recv_buf[32];
           if(Remote.DenNum > (SUBMAXNUM + 1))
             Remote.DenNum = SUBMAXNUM + 1;
           Remote.LineUseNum = 0;
           if((Remote.DenNum >= 1)&&(Remote.DenNum <= 10))
            {
               for(j=0; j<Remote.DenNum; j++)
                {
                 Remote.IP[j][0] = recv_buf[53+24*j];
                 Remote.IP[j][1] = recv_buf[54+24*j];
                 Remote.IP[j][2] = recv_buf[55+24*j];
                 Remote.IP[j][3] = recv_buf[56+24*j];

                 SearchArp_Table(Remote.IP[j], Remote.Mac[j]);

                 for(k=0; k<20; k++)
                   Remote.Addr[j][k] = recv_buf[33+24*j+k];
                 Remote.GroupIP[0] = 236;
                 Remote.GroupIP[1] = LocalCfg.IP[1];
                 Remote.GroupIP[2] = LocalCfg.IP[2];
                 Remote.GroupIP[3] = LocalCfg.IP[3];

                 if(LocalCfg.EncodeFrameSize == VIDEO_VGA)
                   Local.VideoType = 0x10;
                 else
                   Local.VideoType = 0x08;
                 CreateTalkBuff(send_b, VIDEOTALK, Local.MachineType | Local.VideoType | ASK, CALL, Remote.Addr[j], Remote.IP[j]);

                 sendlength = 62;

                 CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Remote.IP[j], send_b, sendlength, 0);
                 xtm_printf("解析地址成功,正在发起呼叫, Remote.IP = %d.%d.%d.%d, Remote.Addr = %s\n", Remote.IP[j][0], Remote.IP[j][1], Remote.IP[j][2], Remote.IP[j][3], \
                             Remote.Addr[j]);
                }
              }
        }

       if(CurrOrder == VIDEOWATCH) //监视
        {
           Remote.DenNum = 1;
           Remote.IP[0][0] = recv_buf[53+24*AddrNo];
           Remote.IP[0][1] = recv_buf[54+24*AddrNo];
           Remote.IP[0][2] = recv_buf[55+24*AddrNo];
           Remote.IP[0][3] = recv_buf[56+24*AddrNo];
           SearchArp_Table(Remote.IP[0], Remote.Mac[0]);
           for(k=0; k<20; k++)
             Remote.Addr[0][k] = recv_buf[33+24*AddrNo+k];
           Remote.GroupIP[0] = 236;
           Remote.GroupIP[1] = LocalCfg.IP[1];
           Remote.GroupIP[2] = LocalCfg.IP[2];
           Remote.GroupIP[3] = LocalCfg.IP[3];
           Order = Local.MachineType | ASK;     //本机支持的视频编码类型;
           CreateTalkBuff(send_b, VIDEOWATCH, Order, CALL, Remote.Addr[0], Remote.IP[0]);

           sendlength = 62;

          /* Remote.IP[0][0] = 192;
           Remote.IP[0][1] = 168;
           Remote.IP[0][2] = 68;
           Remote.IP[0][3] = 98;  */
           CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Remote.IP[0], send_b, sendlength, 0);
           xtm_printf("解析地址成功,正在发起监视 %d.%d.%d.%d\n", Remote.IP[0][0], Remote.IP[0][1], Remote.IP[0][2], Remote.IP[0][3]);
        }
  }
}
//---------------------------------------------------------------------------
void RecvTalkCall_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //对讲呼叫
{
  xtm_printf("RecvTalkCall_Func:: cFromIP = %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
}
//---------------------------------------------------------------------------
void CheckRemoteAddr(void)  //检查远端地址
{
    switch(Remote.Addr[0][0])
     {
      case 'Z':  //中心机呼叫
      case 'W':  //围墙机呼叫
               Remote.Addr[0][5] = '\0';
               break;
      case 'M':  //单元门口机呼叫
               Remote.Addr[0][8] = '\0';
               break;
      case 'S':  //室内机呼叫
      case 'B':  //别墅型室内机呼叫
      case 'H':  //别墅门口机呼叫
               Remote.Addr[0][12] = '\0';
               break;
     }
}
//-----------------------------------------------------------------------
void RecvTalkCallAnswer_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //对讲呼叫应答
{
  INT8U j;
  if((recv_buf[7] & 0x03) == ASK)   //应答
   if(ClearMultiUdpBuff(VIDEOTALK, ASK, CALL, cFromIP))
    {
            xtm_printf("Remote.DenNum = %d\n",Remote.DenNum);
              if((LocalCfg.Addr[0] == 'Z')
                 ||(Remote.DenNum == 1))
               {
                //xtm_memcopy(Remote.DenIP, cFromIP, 4);
                //SearchArp_Table(Remote.DenIP, Remote.DenMac);
               }
              else
               if(Remote.DenNum > 1)
                {
                 /*Remote.DenIP[0] = Remote.GroupIP[0];
                 Remote.DenIP[1] = Remote.GroupIP[1];
                 Remote.DenIP[2] = Remote.GroupIP[2];
                 Remote.DenIP[3] = Remote.GroupIP[3]; */
                }

               for(j=0; j<Remote.DenNum; j++)
                {
                 if((Remote.IP[j][0] == cFromIP[0])&&(Remote.IP[j][1] == cFromIP[1])&&(Remote.IP[j][2] == cFromIP[2])&&(Remote.IP[j][3] == cFromIP[3]))
                  {
                   SearchArp_Table(Remote.IP[j], Remote.Mac[j]);
                   break;
                  } 
                }
            xtm_printf("收到对讲呼叫应答, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
            if(Local.Status == 1)    //状态为主叫对讲
              return;

            if(Local.Status == 0)
              Local.Status = 1;  //状态为主叫对讲

            DisplayTalkConnectWindow();    //显示对讲 正在连接窗口
            
            Local.TalkEndStatus = 1;
            Local.CallConfirmFlag = 1; //设置在线标志
            Local.Timer1Num = 0;
            Local.TimeOut = 0;       //监视超时,  通话超时,  呼叫超时，无人接听
            Local.OnlineNum = 0;     //在线确认序号
            Local.OnlineFlag = 1;
            xtm_printf("Local.Status = %d\n", Local.Status);

            LocalCfg.EncodeFrameSize = VIDEO_VGA; //编码大小

            //开始录制视频
            StartVideoClock();     //打开视频采集编码时钟
            StartRecVideo(LocalCfg.EncodeFrameSize);
            StartEncodeVideo(LocalCfg.EncodeFrameSize);     //打开视频编码
                        
            CreateAudioSend_Stream();      //音视频发送Stream

           #ifdef _REMOTECALLTEST  //远程呼叫测试
            if(Local.CloseTestRing == 0)  //测试模式, 关闭铃声
           #endif
              PlayBackRingWav();
           #ifdef _REMOTECALLTEST  //远程呼叫测试
            Local.CloseTestRing = 0;  //测试模式, 关闭铃声
           #endif

          #ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
            CapturePic.RecordPic = 1;   //通话留照片
            CapturePic.IFrameCount = 0; //I帧计数
            CapturePic.HavePicRecorded = 0;
            xtm_memcopy(CapturePic.RemoteAddr, Remote.Addr[0], 20);
            CapturePic.RemoteAddr[12] = '\0';
          #endif

       //     strcpy(Label_Call.Text, CallAnswer_Text[Local.LanguageNo]);
       //     ShowOsdLabel24(&Label_Call, REFRESH, cWhite, 0);
           }
}
//-----------------------------------------------------------------------
void RecvTalkLineUse_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //对讲占线应答
{
  int isUse;
  int CallNextCenter = 0;
  char CenterAddr[20];


  if((recv_buf[7] & 0x03) == ASK)   //应答
   if(ClearMultiUdpBuff(VIDEOTALK, ASK, CALL, cFromIP))
    {
            isUse = 1;
            #ifdef _MULTI_CENTER_SUPPORT  //多中心机支持
             if(Remote.Addr[0][0] == 'Z')
              {
               xtm_printf("中心 %s 占线\n", Remote.Addr[0]);

               Remote.Addr[0][4] ++;
               if(Remote.Addr[0][4] <= '9')
                {
                 strcpy(CenterAddr, Remote.Addr[0]);
                 isUse = 0;
                 CallNextCenter = 1;
                }
              }
            #endif
            Remote.LineUseNum ++;
            if(isUse == 1)
             //20101202 xu 此处会引起，主叫呼叫多分机，振铃过程中，其它分机呼叫无法退出, 增加　占线数量
             if(Remote.DenNum == Remote.LineUseNum)
              {
               if(Local.CurrentWindow == 0)
                {
                   DisplayCallFailWindow(0);    //显示呼叫失败窗口
                }

               //display_text(90, 72, 0, COLOR_RED, LineInuse_Text[LocalCfg.Language], 0, 24, NOFULLPAGE);
               //延时清提示信息标志
               PicStatBuf.Type = 16;
               PicStatBuf.Time = 0;
               PicStatBuf.Flag = 1;
              }
            xtm_printf("收到对讲占线应答, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
    }

  #ifdef _MULTI_CENTER_SUPPORT  //多中心机支持
   if(CallNextCenter == 1)
    {
     xtm_printf("自动呼叫 %s\n", Remote.Addr[0]);
     Local.PreStatus = 0;
     Call_Func(1, "", CenterAddr);    //呼叫   1  中心  2 住户
    }
  #endif
}
//-----------------------------------------------------------------------
void RecvTalkCallConfirm_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort) //对讲在线确认
{
  INT8U sendlength;
  //本机为主叫方
  if(((Local.Status == 1)||(Local.Status == 5)||(Local.Status == 7)||(Local.Status == 9))
     &&((recv_buf[7] & 0x03) == ASK))
   {
    xtm_memcopy(send_b, recv_buf, 61);
    send_b[7] = Local.MachineType | REPLY;    //应答
    sendlength = 61;
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
    Local.CallConfirmFlag = 1;

    //xtm_printf("收到对讲在线确认\n");

    //如已在通话中，但收到不为通话方的在线确认，则发退出组播组命令
    #ifdef _MULTI_MACHINE_SUPPORT  //多分机支持
     if(Local.Status == 5)
       if((Remote.IP[0][0] != recv_buf[53])||(Remote.IP[0][1] != recv_buf[54])||(Remote.IP[0][2] != recv_buf[55])||(Remote.IP[0][3] != recv_buf[56]))
         SingleExitGroup(recv_buf); //向单个被叫方退出组播组命令
    #endif
   }
  else  //本机为被叫方
   if(((Local.Status == 2)||(Local.Status == 6)||(Local.Status == 8)||(Local.Status == 10))
      &&((recv_buf[7] & 0x03) == REPLY))
   {
    Local.CallConfirmFlag = 1;

    //xtm_printf("收到对方应答本机对讲在线确认\n");
   }
}
//-----------------------------------------------------------------------
void RecvTalkCallStart_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //对讲开始通话
{
    INT8U sendlength;

   //20140225
   //TALKTIMEOUT = DelayTalkTimeOut[LocalCfg.TalkTimeOut]*TIMERPERSEC; //通话最长时间
   Local.TalkTimeOut = TALKTIMEOUT; //通话最长时间
    
  //本机为主叫方 应答
  if((Local.Status == 1)&&((recv_buf[7] & 0x03) == ASK))
   {
    xtm_memcopy(send_b, recv_buf, 57);
    send_b[7]=Local.MachineType | REPLY;    //应答
    sendlength=57;
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);

    #ifdef _MULTI_MACHINE_SUPPORT  //多分机支持
     ExitGroup(recv_buf);      //向其它被叫方退出组播组命令
    #endif

    //获取被叫方地址
    xtm_memcopy(Remote.Addr[0], recv_buf+33, 20);
    xtm_memcopy(Remote.IP[0], recv_buf+53, 4);
    SearchArp_Table(Remote.IP[0], Remote.Mac[0]);
    Remote.DenNum = 1;
    xtm_printf("RecvTalkCallStart_Func::Remote.Addr[0] = %s, Remote.IP[0] = %d.%d.%d.%d\n", Remote.Addr[0], Remote.IP[0][0], Remote.IP[0][1], Remote.IP[0][2], Remote.IP[0][3]);

    //xtm_memcopy(Remote.DenIP, cFromIP, 4);

    StopPlayWavFile();  //关闭铃音
    WaitAudioUnuse(1000); //延时是为了等待声音播放完成

    SoundSetSpeakerDac(0xc0);
    StartRecAudio();
    AudioStartFlag = 1;
    AudioStartTime = 0;
    StartPlayAudio();
    SoundSetSpeakerDac(0xc0);

    Local.Status = 5;  //状态为主叫通话
    Local.TalkEndStatus = 1;
    Local.TimeOut = 0;       //监视超时,  通话超时,  呼叫超时，无人接听

    DisplayTalkStartWindow();    //显示对讲 通话窗口
    
    xtm_printf("对方开始通话, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
    //计时
  /*  sprintf(talkpic_osd.Text, "%02d:%02d\0", Local.TimeOut/INTRPERSEC/60,
           (Local.TimeOut/INTRPERSEC)%60);
    SetOsdPos(&talkpic_osd);
    ShowTalkWatchOsd(0, &talkpic_osd, REFRESH);

    time(&t);
    Local.call_tm_t = localtime(&t);      */
   }
  else  //本机为被叫方 发起
   if(Local.Status == 2)
   {
        
   }

  if((Local.Status == 5)||(Local.Status == 6))
   {
  /*  if(LocalCfg.TalkVoiceOpened == 0)
      //WbSetSpeakerMute(); //静音
      SoundSetSpeakerMute(); //静音
    else
      //WbSetSpeakerVolume(LocalCfg.TalkVolume); //设置音量
     {
      SoundSetSpeakerVolume(LocalCfg.TalkVolume); //设置音量
      if((Remote.Addr[0][0] == 'S')||(Remote.Addr[0][0] == 'B'))
        WbSet1463Volume(0x14);
     }                */
   }               
}

//-----------------------------------------------------------------------

void RecvTalkCallEnd_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //对讲呼叫结束
{
  INT8U isAddrOK;
  //INT8U send_b[128];
  INT8U sendlength;
  INT8U RemoteMac[6];
  //本机被动
  if(((Local.Status == 1)||(Local.Status == 2)||(Local.Status == 5)||(Local.Status == 6) \
     ||(Local.Status == 7)||(Local.Status == 8)||(Local.Status == 9)||(Local.Status == 10)) \
     &&((recv_buf[7] & 0x03) == ASK))
   {
    isAddrOK = 0;

    //本机为主叫方
    if((Local.Status == 1)||(Local.Status == 5)||(Local.Status == 7)||(Local.Status == 9))
      if((recv_buf[53] == Remote.IP[0][0]) && (recv_buf[54] == Remote.IP[0][1]) && (recv_buf[55] == Remote.IP[0][2]) &&(recv_buf[56] == Remote.IP[0][3]))
        isAddrOK = 1;

    //本机为被叫方
    if((Local.Status == 2)||(Local.Status == 6)||(Local.Status == 8)||(Local.Status == 10))
      if((recv_buf[29] == Remote.IP[0][0]) && (recv_buf[30] == Remote.IP[0][1]) && (recv_buf[31] == Remote.IP[0][2]) &&(recv_buf[32] == Remote.IP[0][3]))
        isAddrOK = 1;

     if(isAddrOK == 1)
      {
       Local.OnlineFlag = 0;
       Local.CallConfirmFlag = 0; //设置在线标志

       xtm_memcopy(send_b, recv_buf, 57);
       send_b[7]=Local.MachineType | REPLY;    //应答
       sendlength=57;
      #if 0
       CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
      #else   //此处为避免 后面的存储图片占用资源，改为直接发送
       if(SearchArp_Table(cFromIP, RemoteMac))
         UdpSendBuff_1(DestPort, RemoteMac, cFromIP, send_b, sendlength);
      #endif

       #ifdef _MULTI_MACHINE_SUPPORT  //多分机支持
        ExitGroup(recv_buf);      //向其它被叫方退出组播组命令
       #endif
       //printf("TalkEnd_ClearStatus 114\n");
       TalkEnd_ClearStatus(0);
     /*  if(Local.Status == 10)
        {
         if(Local.CurrentWindow == 16)
          {
           strcpy(Label_Call.Text, LeaveMovieEnd_Text[Local.LanguageNo]);
           ShowOsdLabel24(&Label_Call, REFRESH, cWhite, 0);
          }
        }
       else
        {
         if(Local.CurrentWindow == 16)
          {
           strcpy(Label_Call.Text, CallEnd1_Text[Local.LanguageNo]);
           ShowOsdLabel24(&Label_Call, REFRESH, cWhite, 0);
          }
        }        */

       xtm_printf("对方结束对讲, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
      }

   }
  else  //本机主动
   if((Local.Status == 1)||(Local.Status == 2)||(Local.Status == 5)||(Local.Status == 6)
     ||(Local.Status == 7)||(Local.Status == 8)||(Local.Status == 9)||(Local.Status == 10))   
   {
    Local.OnlineFlag = 0;
    Local.CallConfirmFlag = 0; //设置在线标志

    //本机主动
    if((recv_buf[7]& 0x03) == REPLY)
     if(ClearMultiUdpBuff(VIDEOTALK, ASK, CALLEND, cFromIP))
      {
              TalkEnd_ClearStatus(0);

              if(Local.CurrentWindow == 16)
               {
                /* #ifndef _CLOSETALKPIC
                  strcpy(Label_Call.Text, CallEnd2_Text[Local.LanguageNo]);
                  ShowOsdLabel24(&Label_Call, REFRESH, cWhite, 0);
                 #endif         */
               }
              xtm_printf("对方应答本机结束对讲\n");
      }
   }
}

//-----------------------------------------------------------------------
void TalkEnd_ClearStatus(int isTurn) //对讲结束，清状态和关闭音视频
{
  //INT8U buf[10];
  
  if(isTurn == 1)
   {
    Local.Status = 0;  //状态为空闲
    Local.PreStatus = 0;

    /*StopRecVideo();
    DelayMs(50);
    StopEncodeVideo();
    StopVideoClock();            //关闭视频采集编码时钟    */

    StopRecAudio();
    StopPlayAudio();
    return;
   }

  if(Local.TalkEndStatus == 1)
   {
    Local.TalkEndStatus = 0;
    Local.OnlineFlag = 0;
    Local.CallConfirmFlag = 0; //设置在线标志
    xtm_printf("TalkEnd_ClearStatus Local.Status=%d\n", Local.Status);

    //查看是否在其它组播组内
    //DropMultiGroup(m_VideoSocket, NULL);
    switch(Remote.Addr[0][0])
     {
      case 'Z':  //中心机呼叫
      case 'W':  //围墙机呼叫
               Remote.Addr[0][5] = '\0';
               break;
      case 'H':  //别墅门口机呼叫
               Remote.Addr[0][6] = '\0';
               break;
      case 'M':  //单元门口机呼叫
               Remote.Addr[0][8] = '\0';
                        break;
      case 'S':  //室内机呼叫
      case 'B':  //别墅型室内机呼叫      
               Remote.Addr[0][12] = '\0';
               break;
     }
#if 1
    StopPlayWavFile();  //关闭铃音
    switch(Local.Status)
     {
      case 1: //本机主叫
             StopRecVideo();
             DelayMs(50);
             StopEncodeVideo();

             StopVideoClock();            //关闭视频采集编码时钟

             #ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
              if(CapturePic.HavePicRecorded == 1)  //有照片已录制
               {
                CapturePic.HavePicRecorded = 0;
                Deal_CapturePic_Center();
               }
             #endif
             break;
      case 2: //本机被叫
             break;
      case 5: //本机主叫通话
             StopRecVideo();
             DelayMs(50);
             StopEncodeVideo();
             StopVideoClock();            //关闭视频采集编码时钟


             StopRecAudio();
             StopPlayAudio();

             #ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
              if(CapturePic.HavePicRecorded == 1)  //有照片已录制
               {
                CapturePic.HavePicRecorded = 0;
                Deal_CapturePic_Center();
               }
             #endif             
             break;
      case 6: //本机被叫通话
             StopRecVideo();
             DelayMs(50);      
             StopEncodeVideo();
             StopVideoClock();            //关闭视频采集编码时钟
             
             StopRecAudio();

             StopPlayAudio();

             break;
     }

    if(Local.CurrentWindow != 16)  //在其它窗口
      Local.CurrentWindow = 16;
    //延时清提示信息标志
    PicStatBuf.Type = Local.CurrentWindow;
    PicStatBuf.Time = 0;
    PicStatBuf.Flag = 1;

//    SoundSetSpeakerVolume(LocalCfg.SpkVolume); //设置音量
#endif
    //Local.Status = 0;
   }
  else
    xtm_printf("TalkEnd_ClearStatus 重复\n");
}
//-----------------------------------------------------------------------
void RecvTalkRemoteOpenLock_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //远程开锁
{
  INT8U sendlength; 
  //本机被动
  if(((Local.Status == 1)||(Local.Status == 2)||(Local.Status == 3)||(Local.Status == 4)||(Local.Status == 5)||(Local.Status == 6))
     &&((recv_buf[7] & 0x03) == ASK))
   {
    xtm_memcopy(send_b, recv_buf, 57);
    send_b[7] = Local.MachineType | REPLY;    //主叫
    sendlength = 57;
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);

    if(Local.TalkDoorOpenPlayFlag == 0)  //通话过程中门开了提示
     {
      PlayDoorOpenWav();
      CheckDelayOpenLock_Func();//检查开锁延时
     }

    #ifdef ELEVATORCONTROL_HANGZHOU
     SendElevatorControl(Remote.Addr[0]);
    #endif     
    xtm_printf("对方远程开锁\n");
   }
}
//-----------------------------------------------------------------------
void RecvTalkCenterTurnTalk_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //中心人工转接
{
  INT8U j;
  INT8U isAddrOK;
  INT8U sendlength;
  INT8U DestAddr[21];

  //查看目标地址是否是本机
  isAddrOK = 1;
  for(j=33; j<33+Local.AddrLen; j++)
   if(LocalCfg.Addr[j-33] != recv_buf[j])
   {
    isAddrOK = 0;
    break;
   }
  xtm_printf("RecvTalkCenterTurnTalk_Func:: isAddrOK = %d\n", isAddrOK);
  if(isAddrOK == 0)
    return;

  if((Local.Status == 5)||(Local.Status == 6))
   {
    //完成处理后再给应答
    xtm_memcopy(send_b, recv_buf, 81);
    send_b[7] = Local.MachineType | REPLY;    //主叫
    sendlength = 81;
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);

    TalkEnd_ClearStatus(1);            //本机为主叫方，不关对讲，只关音频 、视频播放

    xtm_memcopy(DestAddr, recv_buf + 57, 20);
    DestAddr[12] = '\0';
    /*if(DestAddr[0] == 'B')
     {
      xtm_memcopy(DestAddr1, DestAddr + 1, 4);
      DestAddr1[4] = '\0';
     }
    else
     {
      xtm_memcopy(DestAddr1, DestAddr + 1, 10);
      DestAddr1[10] = '\0';
     }    */
    Call_Func(3, DestAddr, "");
   }
}
//-----------------------------------------------------------------------
void RecvTalkTrusteeship_Talk_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //托管
{
  //INT8U j;
  INT16U sendlength;
  if((recv_buf[7] & 0x03) == ASK)   //应答
   if(ClearMultiUdpBuff(VIDEOTALK, ASK, CALL, cFromIP))
    {
      xtm_memcopy(Remote.Addr[0], recv_buf + 33, 20);
      xtm_printf("RecvTalkTrusteeship_Talk_Func::Remote.Addr[0] = %s\n", Remote.Addr[0]);
      if(Local.Status == 0)
       {
         CreateNSBuff(send_b, NSORDER, ASK);

         sendlength = 56;

         CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Local.NsMultiAddr, send_b, sendlength, VIDEOTALK);
       }
      else
       {
        xtm_printf("托管, 本机正忙,无法呼叫\n");
       }
    }
}
//-----------------------------------------------------------------------
void RecvTalkCallUpDown_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)  //对讲数据
{
  //已经处在结束通话切换界面状态
  if(PicStatBuf.Flag != 0)
   {
    //xtm_printf("RecvTalkCallUpDown_Func::PicStatBuf.Flag = %d\n", PicStatBuf.Flag);
    return;
   }
  //xtm_printf("recv_buf[6] = %d, recv_buf[7] = %d, recv_buf[8] = %d, recv_buf[61] = %d\n", recv_buf[6], recv_buf[7], recv_buf[8], recv_buf[61]);
  if((Local.Status == 1)||(Local.Status == 2)||(Local.Status == 5)||(Local.Status == 6)
     ||(Local.Status == 7)||(Local.Status == 8)||(Local.Status == 9)||(Local.Status == 10))  //状态为对讲
   switch(recv_buf[61])
   {
    case 1://音频
           AudioDeal_Func(recv_buf, cFromIP, length);  //音频数据处理
           break;
    case 2://视频  I帧  352*288
    case 3://视频  P帧  352*288
    case 4://视频  I帧  720*480
    case 5://视频  P帧  720*480
           VideoDeal_Func(recv_buf, cFromIP, length);
           break;
  }
}
//-----------------------------------------------------------------------
//监视
void RecvWatchCall_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //监视呼叫
{
  INT8U j;
  INT8U isAddrOK;
  INT16U sendlength;

  //查看目标地址是否是本机
  isAddrOK = 1;
  for(j=33; j<(33+Local.AddrLen); j++)
   if(LocalCfg.Addr[j-33] != recv_buf[j])
   {
    isAddrOK = 0;
    break;
   }
  if(isAddrOK == 0)
    return;

  if(PicStatBuf.Flag == 1)
    return;   

  //本机状态为空闲
  if(Local.Status == 0)
   {
    //xtm_memcopy(Remote.DenIP, cFromIP, 4);
    //xtm_printf("Remote.DenIP, %d.%d.%d.%d\0",
    //        Remote.DenIP[0],Remote.DenIP[1],Remote.DenIP[2],Remote.DenIP[3]);
    //获取对方地址
    xtm_memcopy(Remote.Addr[0], recv_buf+9, 20);
    xtm_memcopy(Remote.IP[0], recv_buf+29, 4);
    xtm_printf("Remote.IP[0], %d.%d.%d.%d\0",
            Remote.IP[0][0], Remote.IP[0][1], Remote.IP[0][2], Remote.IP[0][3]);
    Remote.DenNum = 1;
    SearchArp_Table(Remote.IP[0], Remote.Mac[0]);


    xtm_printf("对方发起直通监视呼叫, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);

    xtm_memcopy(send_b, recv_buf, 57);
    send_b[7] = Local.MachineType | ASK;    //主叫
    send_b[8] = CALLANSWER;//监视应答
    sendlength = 57;
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);

    Local.Status = 4;  //状态为被监视

    Local.CallConfirmFlag = 1; //设置在线标志
    Local.Timer1Num = 0;
    Local.TimeOut = 0;       //监视超时,  通话超时,  呼叫超时，无人接听
    Local.OnlineNum = 0;     //在线确认序号
    Local.OnlineFlag = 1;

    LocalCfg.EncodeFrameSize = VIDEO_VGA; //编码大小

    //开始录制视频
    StartVideoClock();     //打开视频采集编码时钟
    StartRecVideo(LocalCfg.EncodeFrameSize);
    StartEncodeVideo(LocalCfg.EncodeFrameSize);     //打开视频编码

    CreateAudioSend_Stream();      //音视频发送Stream

    DisplayWatchWindow(REFRESH);
   }
  //本机为忙
  else
   {
    xtm_memcopy(send_b, recv_buf, 57);
    send_b[7]= Local.MachineType | ASK;    //主叫
    send_b[8] = LINEUSE;//占线应答
    sendlength = 57;
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
    xtm_printf("对方发起监视呼叫, 本机忙 %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
   }
}
//-----------------------------------------------------------------------
void RecvWatchCallAnswer_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //监视呼叫应答
{
  xtm_printf("收到监视呼叫应答, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
}
//-----------------------------------------------------------------------
void RecvWatchLineUse_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //监视占线应答
{
  xtm_printf("收到监视占线应答, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
}
//-----------------------------------------------------------------------
void RecvWatchCallConfirm_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort) //监视在线确认
{
  INT8U sendlength;
  //本机为主叫方
  if(((Local.Status == 3)||(Local.Status == 4))&&((recv_buf[7] & 0x03) == ASK))
   {
    xtm_memcopy(send_b, recv_buf, 61);
    send_b[7] = Local.MachineType | REPLY;    //应答
    sendlength = 61;
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
    Local.CallConfirmFlag = 1;

    //xtm_printf("收到监视在线确认\n");
   }
  else  //本机为被叫方
   if(Local.Status == 3)
    {
     Local.CallConfirmFlag = 1;

     //xtm_printf("收到对方应答本机监视在线确认\n");
    }
}
//-----------------------------------------------------------------------
void RecvWatchCallEnd_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //监视呼叫结束
{
  //INT8U isAddrOK;
  INT8U sendlength;

  //本机被动
  if(((Local.Status == 3)||(Local.Status == 4))&&((recv_buf[7] & 0x03) == ASK))
   {
       Local.OnlineFlag = 0;
       Local.CallConfirmFlag = 0; //设置在线标志

       xtm_memcopy(send_b, recv_buf, 57);
       send_b[7]=Local.MachineType | REPLY;    //应答
       sendlength=57;
       CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);

       WatchEnd_ClearStatus();

       xtm_printf("对方结束监视, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
   }
  else  //本机主动
   if(Local.Status == 4)
   {
    Local.OnlineFlag = 0;
    Local.CallConfirmFlag = 0; //设置在线标志
    //本机主动
    if((recv_buf[7]& 0x03) == REPLY)
     if(ClearMultiUdpBuff(VIDEOWATCH, ASK, CALLEND, cFromIP))
      {
              WatchEnd_ClearStatus();

              xtm_printf("对方应答本机结束监视\n");
      }
   }
}
//-----------------------------------------------------------------------
void RecvWatchCallUpDown_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort, INT16U length)  //监视数据
{
  if((Local.Status == 3)||(Local.Status == 4))  //状态为监视
   switch(recv_buf[61])
   {
    case 2://视频  I帧  352*288
    case 3://视频  P帧  352*288
    case 4://视频  I帧  720*480
    case 5://视频  P帧  720*480
           VideoDeal_Func(recv_buf, cFromIP, length);
           break;
  }
}
//-----------------------------------------------------------------------
void RecvSmarkMonitorCall_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)  //监视呼叫
{
    INT8U j;
     INT8U isAddrOK;
     INT16U sendlength;
    
     //查看目标地址是否是本机
     isAddrOK = 1;
     for(j=34; j<(34+Local.AddrLen); j++)
      if(LocalCfg.Addr[j-34] != recv_buf[j])
      {
       isAddrOK = 0;
       break;
      }
     if(isAddrOK == 0)
        {
        //PlaySoundTip();
       return;
        }
    
     if(PicStatBuf.Flag == 1)
       return;   
    
     //本机状态为空闲
     if(Local.Status == 0)
      {
       //xtm_memcopy(Remote.DenIP, cFromIP, 4);
       //xtm_printf("Remote.DenIP, %d.%d.%d.%d\0",
       //        Remote.DenIP[0],Remote.DenIP[1],Remote.DenIP[2],Remote.DenIP[3]);
       //获取对方地址
       xtm_memcopy(Remote.Addr[0], recv_buf+142, 20);
       xtm_memcopy(Remote.IP[0], recv_buf+94, 4);
       /*
       Remote.IP[0][0] = cFromIP[0];
       Remote.IP[0][1] = cFromIP[1];
       Remote.IP[0][2] = cFromIP[2];
       Remote.IP[0][3] = cFromIP[3];
*/
       
       xtm_printf("Remote.IP[0], %d.%d.%d.%d\0",Remote.IP[0][0], Remote.IP[0][1], Remote.IP[0][2], Remote.IP[0][3]);
       Remote.DenNum = 1;
       SearchArp_Table(Remote.IP[0], Remote.Mac[0]);
       
       xtm_printf("对方发起直通监视呼叫, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
    
       xtm_memcopy(send_b, recv_buf, 142);
       send_b[9] = 0;    //主叫
       sendlength = 142;
       CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
    
       //Local.Status = 90;  //状态为被监视
       //PlaySoundTip();
    /*
       Local.CallConfirmFlag = 1; //设置在线标志
       Local.Timer1Num = 0;
       Local.TimeOut = 0;       //监视超时,  通话超时,  呼叫超时，无人接听
       Local.OnlineNum = 0;     //在线确认序号
       Local.OnlineFlag = 1;
    
       LocalCfg.EncodeFrameSize = VIDEO_VGA; //编码大小
    
       //开始录制视频
       StartVideoClock();     //打开视频采集编码时钟
       StartRecVideo(LocalCfg.EncodeFrameSize);
       StartEncodeVideo(LocalCfg.EncodeFrameSize);     //打开视频编码
    
       CreateAudioSend_Stream();      //音视频发送Stream
    
       DisplayWatchWindow(REFRESH);
       */
      }
     //本机为忙
     else
      {
       xtm_memcopy(send_b, recv_buf, 142);
       send_b[9] = 1;
       sendlength = 142;
       CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
       xtm_printf("对方发起监视呼叫, 本机忙 %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
       //PlaySoundTip();
      }

}
//-----------------------------------------------------------------------
void RecvSmarkMonitorStart_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)
{
        INT8U j;
        INT16U DestPort2;
         INT8U isAddrOK;
         INT16U sendlength;
        
         //查看目标地址是否是本机
         isAddrOK = 1;
         for(j=34; j<(34+Local.AddrLen); j++)
          if(LocalCfg.Addr[j-34] != recv_buf[j])
          {
           isAddrOK = 0;
           break;
          }
         if(isAddrOK == 0)
            {
            //PlaySoundTip();
           return;
            }
        
         if(PicStatBuf.Flag == 1)
           return;   

    if(Local.Status == 0)
    {

       Local.Status = 90;  //状态为被监视

       DestPort2 = recv_buf[99];
       DestPort2 <<= 8;
       DestPort2 += recv_buf[98];
       
       LocalPort[4] = DestPort2;

       xtm_memcopy(send_b, recv_buf, 142);
       send_b[9] = 0;    //主叫
       sendlength = 142;
       CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);

       Local.CallConfirmFlag = 1; //设置在线标志
       Local.Timer1Num = 0;
       Local.TimeOut = 0;       //监视超时,  通话超时,  呼叫超时，无人接听
       Local.OnlineNum = 0;     //在线确认序号
       Local.OnlineFlag = 1;
    
       LocalCfg.EncodeFrameSize = VIDEO_VGA; //编码大小
    
       //开始录制视频
       StartVideoClock();     //打开视频采集编码时钟
       StartRecVideo(LocalCfg.EncodeFrameSize);
       StartEncodeVideo(LocalCfg.EncodeFrameSize);     //打开视频编码
    
       CreateAudioSend_Stream();      //音视频发送Stream
    
       DisplayWatchWindow(REFRESH);
    }
    else
        {
            xtm_memcopy(send_b, recv_buf, 142);
       send_b[9] = 1;
       sendlength = 142;
       CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);
       xtm_printf("对方发起监视呼叫, 本机忙 %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
       Local.Status == 0;
        }
}

//-----------------------------------------------------------------------
void RecvSmarkMonitorCallConfirm_Func() //监视在线确认
{
  if(Local.Status == 90)
    {
     Local.CallConfirmFlag = 1;
     //PlaySoundTip();
    }
   
}
//-----------------------------------------------------------------------
void RecvSmarkMonitorCallEnd_Func(INT8U *recv_buf, INT8U *cFromIP)  //监视呼叫结束
{
  INT8U sendlength;

  //本机被动
  if(Local.Status == 90)
   {
       Local.OnlineFlag = 0;
       Local.CallConfirmFlag = 0; //设置在线标志

       xtm_memcopy(send_b, recv_buf, 142);
       send_b[7]=4;
       send_b[9]=0;
       sendlength=142;
       CreateMultiUdpBuff(LocalPort[3], SINGLE_SEND, cFromIP, send_b, sendlength, 0);

       WatchEnd_ClearStatus();

       xtm_printf("对方结束监视, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
   }
}
//-----------------------------------------------------------------------
//监视结束，清状态和关闭音视频
void WatchEnd_ClearStatus(void)
{
#if 1
    StopRecVideo();
    //DelayMs(50);
    StopEncodeVideo();
#else
    StopEncodeVideo();
    DelayMs(50);
    StopRecVideo();
#endif
    StopVideoClock();            //关闭视频采集编码时钟
    Local.Status = 0;  //状态为空闲

    //延时清提示信息标志
    PicStatBuf.Type = 13;
    PicStatBuf.Time = 0;
    PicStatBuf.Flag = 1;
}
//-----------------------------------------------------------------------
void AudioDeal_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U length)  //音频数据处理
{
  INT32U Maddr1;
  INT8U Stream_Pos;
  INT16U talk_Datalen;

  //xtm_printf("AudioDeal_Func::Local.TalkDoorOpenPlayFlag = %d\n", Local.TalkDoorOpenPlayFlag);
  if(Local.TalkDoorOpenPlayFlag == 1)  //通话过程中门开了提示
    return;
    
  if((Local.Status == 5)||(Local.Status == 6))
  //if(1)
   {
    if(AudioStartFlag == 1)
     {
      AudioStartTime ++;
      if(AudioStartTime > 3)
       {
        AudioStartFlag = 0;
        AudioStartTime = 0;
        RestoreSpeakerDac();
       }
     }
    //帧序号
//    xtm_memcopy(&talkdata, recv_buf + 9, sizeof(talkdata));
    talk_Datalen = recv_buf[74];
    talk_Datalen <<= 8;
    talk_Datalen += recv_buf[73];
    //talk_Datalen = 64;
    //xtm_printf("sizeof(talkdata) = %d\n", sizeof(talkdata));
    //xtm_printf("talkdata.Frameno = %d, Framelen = 0x%X%X, TotalPackage = %d, CurrPackage = %d, Datalen = %d, PackLen = %d\n", \
    //  talkdata.Frameno, (INT16U)((talkdata.Framelen & 0xffff0000) >> 16), (INT16U)(talkdata.Framelen & 0xffff), talkdata.TotalPackage, talkdata.CurrPackage, talkdata.Datalen, talkdata.PackLen);

    //xtm_printf("recv_buf = 0x%X%X\n", \
    //    (INT16U)(((INT32U)recv_buf & 0xffff0000) >> 16), (INT16U)((INT32U)recv_buf & 0xffff));
    if(talk_Datalen != BlockLen)
     {
      xtm_printf("talk_Datalen = %d\n", talk_Datalen);
      return;
     }
    GetIdlePlayValue(recv_buf + 77);
 #if 1
    Stream_Pos = (PcmO_iPut >> 16) & 0xffff;
    //xtm_printf("PcmO_iPut = %d, Stream_Pos = %d\n", PcmO_iPut, Stream_Pos);
    Maddr1 = (((INT32U)recv_buf&0xffff)+(((INT32U)recv_buf&0xffff0000)>>12))|0x700000;
    fmem_insb_dma(Maddr1 + 86, PcmO_Stream[Stream_Pos] + PcmO_iPut, talk_Datalen);//talkdata.Datalen);
    PcmO_iPut += BlockLen;
    if(PcmO_iPut >= PCM_BUF_MAX)
      PcmO_iPut = 0;

   #if 0
    PcmO_Len ++;
   #else
    if(PcmO_iPut >= PcmO_iGet)
      PcmO_Len = (PcmO_iPut - PcmO_iGet);
    else
      PcmO_Len = (PcmO_iPut + PCM_BUF_MAX - PcmO_iGet);
   #endif
         
    //xtm_printf("PcmO_iPut = 0x%X  %X, PcmO_iGet = 0x%X  %X, PcmO_Len = 0x%X  %X\n", (INT16U)((PcmO_iPut & 0xffff0000) >> 16), (INT16U)(PcmO_iPut & 0xffff), (INT16U)((PcmO_iGet & 0xffff0000) >> 16), (INT16U)(PcmO_iGet & 0xffff), (INT16U)((PcmO_Len & 0xffff0000) >> 16), (INT16U)(PcmO_Len & 0xffff));
    CheckPlayAudioBuf();
  /* #if 0
    PcmO_Len ++;
   #else
    if(PcmO_iPut >= PcmO_iGet)
      PcmO_Len = (PcmO_iPut - PcmO_iGet)/BlockLen;
    else
      PcmO_Len = (PcmO_iPut + PCM_BUF_MAX - PcmO_iGet)/BlockLen;
   #endif   */
 #endif
   }
}
//-----------------------------------------------------------------------
void VideoDeal_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U length)  //视频数据处理
{
}
//-----------------------------------------------------------------------
#ifdef _MULTI_MACHINE_SUPPORT  //多分机支持
//退出组播组（主叫方->被叫方，被叫方应答）
void RecvTalkLeaveGroup_Func(INT8U *recv_buf, INT8U *cFromIP, INT16U DestPort)
{
  INT16U sendlength;

  xtm_printf("recv exit group, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
  //本机为被叫方 被叫对讲
  if((Local.Status == 2)&&((recv_buf[7] & 0x03) == ASK))
   {
    xtm_memcopy(send_b, recv_buf, 61);
    send_b[7] = Local.MachineType | REPLY;    //应答
    sendlength = 61;
    CreateMultiUdpBuff(DestPort, SINGLE_SEND, cFromIP, send_b, sendlength, 0);

    //printf("TalkEnd_ClearStatus 111\n");
    TalkEnd_ClearStatus(0);
    xtm_printf("对方要求离开组播组, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
   }
  else  //本机为主叫方 发起
   if((recv_buf[7] & 0x03) == REPLY)
   {
    //本机主动
    if(ClearMultiUdpBuff(VIDEOTALK, ASK, LEAVEGROUP, cFromIP))
     {
       xtm_printf("对方应答本机离开组播组, %d.%d.%d.%d\n", cFromIP[0], cFromIP[1], cFromIP[2], cFromIP[3]);
     }
   }
}
//-----------------------------------------------------------------------
void ExitGroup(INT8U *buf)      //向其它被叫方退出组播组命令
{
  INT8U i,j;
  INT8U isIP;
  INT16U sendlength;
  //退出组播组
  if(Remote.DenNum > 1)
   {
     for(j=0; j<Remote.DenNum; j++)
      {
       Remote.Added[j] = 0;
       xtm_printf("%d.%d.%d.%d  %d.%d.%d.%d\n",Remote.IP[j][0],Remote.IP[j][1],Remote.IP[j][2],Remote.IP[j][3],
              buf[53],buf[54],buf[55],buf[56]);
       //不为开始通话的那一方
       if((Remote.IP[j][0] == buf[53])&&(Remote.IP[j][1] == buf[54])
        &&(Remote.IP[j][2] == buf[55])&&(Remote.IP[j][3] == buf[56]))
         isIP = 1;
       else
         isIP = 0;

       //检查是否已经在发送缓冲区内   //xu 20101109
       for(i=0; i<UDPSENDMAX; i++)
        if(Multi_Udp_Buff[i].isValid == 1)
         {
          if(((Multi_Udp_Buff[i].RemoteIP[0] == Remote.IP[j][0])&&(Multi_Udp_Buff[i].RemoteIP[1] == Remote.IP[j][1])&&\
             (Multi_Udp_Buff[i].RemoteIP[2] == Remote.IP[j][2])&&(Multi_Udp_Buff[i].RemoteIP[3] == Remote.IP[j][3]))&&(Multi_Udp_Buff[i].buf[8] == LEAVEGROUP))
           {
            isIP = 1;
            xtm_printf("退出组播组命令已在缓冲内, %d.%d.%d.%d\n", Remote.IP[j][0], Remote.IP[j][1], Remote.IP[j][2], Remote.IP[j][3]);
            break;
           }
         }

       if(isIP == 0)
        {
         //头部
         xtm_memcopy(send_b, UdpPackageHead, 6);
         //命令 
         send_b[6] = VIDEOTALK;
         send_b[7] = Local.MachineType | ASK;    //主叫
         send_b[8] = LEAVEGROUP;
         xtm_memcopy(send_b+9, LocalCfg.Addr, 20);
         xtm_memcopy(send_b+29, LocalCfg.IP, 4);
         xtm_memcopy(send_b+33, Remote.Addr[j], 20);
         xtm_memcopy(send_b+53, Remote.IP[j], 4);

         send_b[57] = Remote.GroupIP[0];
         send_b[58] = Remote.GroupIP[1];
         send_b[59] = Remote.GroupIP[2];
         send_b[60] = Remote.GroupIP[3];

         sendlength = 61;

         CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, Remote.IP[j], send_b, sendlength, 0);
        }
      }
   }
}
//-----------------------------------------------------------------------
void SingleExitGroup(INT8U *buf) //向单个被叫方退出组播组命令
{
  INT16U sendlength;
  //头部
  xtm_memcopy(send_b, UdpPackageHead, 6);
  //命令
  send_b[6] = VIDEOTALK;
  send_b[7] = Local.MachineType | ASK;    //主叫
  send_b[8] = LEAVEGROUP;
  xtm_memcopy(send_b+9, LocalCfg.Addr, 20);
  xtm_memcopy(send_b+29, LocalCfg.IP, 4);
  xtm_memcopy(send_b+33,  buf + 33, 20);
  xtm_memcopy(send_b+53, buf + 53, 4);                       

  send_b[57] = Remote.GroupIP[0];
  send_b[58] = Remote.GroupIP[1];
  send_b[59] = Remote.GroupIP[2];
  send_b[60] = Remote.GroupIP[3];

  sendlength = 61;

  CreateMultiUdpBuff(LocalPort[1], MULTI_SEND, buf + 53, send_b, sendlength, 0);
  xtm_printf("单个 正在要求退出组播组, RemoteHost = %d.%d.%d.%d\0", \
                  buf[53], buf[54], buf[55], buf[56]);

}
#endif
//-----------------------------------------------------------------------
