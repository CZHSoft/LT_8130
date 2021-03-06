#include "includes.h"

#include "stdlib.h"

#define x_ip_h

#define CommonH
#include "common.h"

/*
****************************************
20131119
暂无修改
****************************************
*/

#define ARP_TABLE_NUM  10
#define ARP_REFRESH_TIME  (60*200)

INT8U Arp_Deal(INT8U *recv_buf, INT16U recv_len);
INT8U Icmp_Deal(INT8U *recv_buf, INT16U recv_len);
INT8U Build_Ip_Crc(INT8U *send_buf);
INT8U Build_Icmp_Crc(INT8U *send_buf, INT16U data_len);
INT8U Build_Icmp_Crc_1(INT8U *send_buf, INT8U *data_buf, INT16U data_len);

INT8U Igmp_Deal(INT8U *recv_buf, INT16U recv_len);
void AddMultiGroup(INT8U *McastAddr);  //加入组播组
void DropMultiGroup(INT8U *McastAddr);  //退出组播组
INT8U Build_Igmp_Crc(INT8U *send_buf);

struct Arp_Table1
{
  INT8U mac[6];
  INT8U ip[4];
  INT16U surv_time;
};

struct Arp_Table1 Arp_Table[ARP_TABLE_NUM];
void InitArp_Table(void);
INT8U RefreshArp_Table(INT8U *r_mac, INT8U *r_ip); //更新ARP缓存表
void Arp_Table_Timer(void);
INT8U SearchArp_Table(INT8U *DestIP, INT8U *DestMac);
void SendArp_Ask(INT8U *DestIP);   //发送 ARP请求表
void PrintArp_Table(void);
void SendFreeArp(void);

INT16U ip_pack_no = 1;  //ip首部标识
void 	send_mac_packet_1(INT16U len, INT8U *udpdata);
void send_mac_packet(INT16U len1, INT8U *data1, INT16U len2, INT8U *data2);
void send_mac_video_packet(INT16U len1, INT8U *data1, INT16U len2, INT32U video_addr);

INT8U *send_mac_buf;
INT8U *send_mac_buf2;
//---------------------------------------------------------------------------
INT8U Arp_Deal(INT8U *recv_buf, INT16U recv_len)
{
  if((recv_buf[12] == 0x08)&&(recv_buf[13] == 0x06))  //ARP
   {
    //xtm_printf("RX_PACKET_STATUS_H = 0x%X\n", inportb(RX_PACKET_STATUS_H));
    switch(recv_buf[21])
     {
      case 0x01:  //请求
                if((recv_buf[0] == 0xff)&&(recv_buf[1] == 0xff)&&(recv_buf[2] == 0xff)&&(recv_buf[3] == 0xff)&&(recv_buf[4] == 0xff)&&(recv_buf[5] == 0xff))
                 {
                //xtm_printf("recv arp ask 88 ip_addr = %d.%d.%d.%d, %x:%x:%x:%x:%x:%x , len = %d\n", recv_buf[28], recv_buf[29], recv_buf[30], recv_buf[31], \
                //            recv_buf[22], recv_buf[23], recv_buf[24], recv_buf[25], recv_buf[26], recv_buf[27], recv_len);

                  //查看是否本机所发，如是不应答
                  if((recv_buf[28] == LocalCfg.IP[0])&&(recv_buf[29] == LocalCfg.IP[1])&&(recv_buf[30] == LocalCfg.IP[2])&&(recv_buf[31] == LocalCfg.IP[3])
                     &&(recv_buf[22] == LocalCfg.Mac_Addr[0])&&(recv_buf[23] == LocalCfg.Mac_Addr[1])&&(recv_buf[24] == LocalCfg.Mac_Addr[2])&&(recv_buf[25] == LocalCfg.Mac_Addr[3])&&(recv_buf[26] == LocalCfg.Mac_Addr[4])&&(recv_buf[27] == LocalCfg.Mac_Addr[5]))
                   {
                    xtm_printf("Arp_Deal:: local send free arp\n");
                    break;
                   }

                  //发送方 IP 地址与本机相同，但 MAC地址不相同
                  if((recv_buf[28] == LocalCfg.IP[0])&&(recv_buf[29] == LocalCfg.IP[1])&&(recv_buf[30] == LocalCfg.IP[2])&&(recv_buf[31] == LocalCfg.IP[3])
                     &&((recv_buf[22] == LocalCfg.Mac_Addr[0])||(recv_buf[23] == LocalCfg.Mac_Addr[1])||(recv_buf[24] == LocalCfg.Mac_Addr[2])||(recv_buf[25] == LocalCfg.Mac_Addr[3])||(recv_buf[26] == LocalCfg.Mac_Addr[4])||(recv_buf[27] == LocalCfg.Mac_Addr[5])))
                   {
                    xtm_printf("Arp_Deal Ask::IP地址冲突, IP = %d.%d.%d.%d, 对方Mac = %x:%x:%x:%x:%x:%x\n", LocalCfg.IP[0], LocalCfg.IP[1], LocalCfg.IP[2], LocalCfg.IP[3], \
                                      recv_buf[22], recv_buf[23], recv_buf[24], recv_buf[25], recv_buf[26], recv_buf[27]);
                    Local.Conflict_Flag = 1;    //IP地址冲突
                    xtm_memcopy(Local.Conflict_Mac, recv_buf + 22, 6);
                   }
                  if((recv_buf[38] == LocalCfg.IP[0])&&(recv_buf[39] == LocalCfg.IP[1])&&(recv_buf[40] == LocalCfg.IP[2])&&(recv_buf[41] == LocalCfg.IP[3]))
                   {
                    xtm_memcopy(send_mac_buf, recv_buf + 6, 6);        //对方MAC地址
                    xtm_memcopy(send_mac_buf + 6, LocalCfg.Mac_Addr, 6);    //本机MAC地址
                    xtm_memcopy(send_mac_buf + 12, recv_buf + 12, 9);
                    send_mac_buf[21] = 0x02;                      //应答
                    xtm_memcopy(send_mac_buf + 22, LocalCfg.Mac_Addr, 6);    //本机MAC地址
                    xtm_memcopy(send_mac_buf + 28, LocalCfg.IP, 4);    //本机IP地址
                    xtm_memcopy(send_mac_buf + 32, recv_buf + 22, 6);    //对方MAC地址
                    xtm_memcopy(send_mac_buf + 38, recv_buf + 28, 4);    //对方IP地址

               //     xtm_printf("send_mac_packet ....\n");
              //      send_mac_packet_1(42, send_mac_buf);
              //      send_mac_packet(512, send_mac_buf, 0, send_mac_buf + 256);
                       send_mac_packet(60, send_mac_buf, 0, send_mac_buf + 256);
              //      xtm_printf("recv arp ask src_addr = %d.%d.%d.%d, ip_addr = %d.%d.%d.%d, len = %d\n", recv_buf[28], recv_buf[29], recv_buf[30], recv_buf[31], recv_buf[38], recv_buf[39], recv_buf[40], recv_buf[41], recv_len);
                    RefreshArp_Table(recv_buf + 22, recv_buf + 28);
                   }
                  //xtm_printf("recv arp ask ip_addr = %d.%d.%d.%d, len = %d\n", recv_buf[38], recv_buf[39], recv_buf[40], recv_buf[41], recv_len);
                 } 
                break;
      case 0x02:  //应答
                //xtm_printf("recv arp reply 11 ip_addr = %d.%d.%d.%d, len = %d\n", recv_buf[38], recv_buf[39], recv_buf[40], recv_buf[41], recv_len);
                //xtm_printf("recv arp reply 11 ip_addr = %d.%d.%d.%d, %x:%x:%x:%x:%x:%x , len = %d\n", recv_buf[28], recv_buf[29], recv_buf[30], recv_buf[31], \
                //            recv_buf[22], recv_buf[23], recv_buf[24], recv_buf[25], recv_buf[26], recv_buf[27], recv_len);
                //xtm_printf("recv arp reply 22 ip_addr = %d.%d.%d.%d, len = %d\n", recv_buf[38], recv_buf[39], recv_buf[40], recv_buf[41], recv_len);
                //发送方 IP 地址与本机相同，但 MAC地址不相同
                if((recv_buf[28] == LocalCfg.IP[0])&&(recv_buf[29] == LocalCfg.IP[1])&&(recv_buf[30] == LocalCfg.IP[2])&&(recv_buf[31] == LocalCfg.IP[3])
                     &&((recv_buf[22] == LocalCfg.Mac_Addr[0])||(recv_buf[23] == LocalCfg.Mac_Addr[1])||(recv_buf[24] == LocalCfg.Mac_Addr[2])||(recv_buf[25] == LocalCfg.Mac_Addr[3])||(recv_buf[26] == LocalCfg.Mac_Addr[4])||(recv_buf[27] == LocalCfg.Mac_Addr[5])))
                 {
                    xtm_printf("Arp_Deal Reply::IP地址冲突, IP = %d.%d.%d.%d, 对方Mac = %x:%x:%x:%x:%x:%x\n", LocalCfg.IP[0], LocalCfg.IP[1], LocalCfg.IP[2], LocalCfg.IP[3], \
                                      recv_buf[22], recv_buf[23], recv_buf[24], recv_buf[25], recv_buf[26], recv_buf[27]);
                    Local.Conflict_Flag = 1;    //IP地址冲突
                    xtm_memcopy(Local.Conflict_Mac, recv_buf + 22, 6);                                      
                    break;
                 }
                if((recv_buf[38] == LocalCfg.IP[0])&&(recv_buf[39] == LocalCfg.IP[1])&&(recv_buf[40] == LocalCfg.IP[2])&&(recv_buf[41] == LocalCfg.IP[3]))
                 {
                  xtm_printf("recv arp reply ip_addr = %d.%d.%d.%d, len = %d\n", recv_buf[28], recv_buf[29], recv_buf[30], recv_buf[31], recv_len);
                  RefreshArp_Table(recv_buf + 22, recv_buf + 28);
                 }
                break;
     }
    return 1;
   }
  return 0;
}
//---------------------------------------------------------------------------
INT8U Icmp_Deal(INT8U *recv_buf, INT16U recv_len)
{
  INT16U data_len;
  if((recv_buf[12] == 0x08)&&(recv_buf[13] == 0x00)&&(recv_buf[23] == 0x01))  //ICMP
   {
    switch(recv_buf[34])
     {
      case 0x08:  //请求
                if((recv_buf[0] == LocalCfg.Mac_Addr[0])&&(recv_buf[1] == LocalCfg.Mac_Addr[1])&&(recv_buf[2] == LocalCfg.Mac_Addr[2])&&(recv_buf[3] == LocalCfg.Mac_Addr[3])&&(recv_buf[4] == LocalCfg.Mac_Addr[4])&&(recv_buf[5] == LocalCfg.Mac_Addr[5]))
                 {
                  if((recv_buf[30] == LocalCfg.IP[0])&&(recv_buf[31] == LocalCfg.IP[1])&&(recv_buf[32] == LocalCfg.IP[2])&&(recv_buf[33] == LocalCfg.IP[3]))
                   {
                    data_len = (recv_buf[16] << 8) + recv_buf[17] - 28;
                    //xtm_printf("recv ping ask ip_addr = %d.%d.%d.%d, data_len = %d, %d, %d\n", recv_buf[26], recv_buf[27], recv_buf[28], recv_buf[29], data_len, recv_buf[16], recv_buf[17]);
                    if(data_len > 1400)
                     {
                      xtm_printf("recv ping ask ip_addr = %d.%d.%d.%d, data_len = %d, too long\n", recv_buf[26], recv_buf[27], recv_buf[28], recv_buf[29], data_len);
                      break;
                     }
                    xtm_memcopy(send_mac_buf, recv_buf + 6, 6);        //对方MAC地址
                    xtm_memcopy(send_mac_buf + 6, LocalCfg.Mac_Addr, 6);    //本机MAC地址

                    send_mac_buf[12]= 0x8;
                    send_mac_buf[13]= 0x0;            //Mac包长度

                    send_mac_buf[14]=0x45;
                    send_mac_buf[15]=0x00;      //版本、首部长度及服务类型

                    send_mac_buf[16]= recv_buf[16];//(20+8+data_len)/256;
                    send_mac_buf[17]= recv_buf[17];//(20+8+data_len)%256; //IP包总长度

                    send_mac_buf[18]= (ip_pack_no >> 8) & 0xff;
                    send_mac_buf[19]= ip_pack_no & 0xff;   //IP首部标识

                    send_mac_buf[20]=0x0;
                    send_mac_buf[21]= 0x0;  //标志和片偏移

                    send_mac_buf[22]=0x40; //生存时间
                    send_mac_buf[23]= 0x1;  //ICMP协议

                    send_mac_buf[24]=0;
                    send_mac_buf[25]=0;  // IP首部校验和

                    xtm_memcopy(send_mac_buf + 26, LocalCfg.IP, 4);    //本机IP地址
                    xtm_memcopy(send_mac_buf + 30, recv_buf + 26, 4);    //对方IP地址

                    send_mac_buf[34]=0x0;   // ICMP类型 ICMP->Echo_Reply 0x0

                    send_mac_buf[35]=0x0;  //代码

                    send_mac_buf[36]=0;
                    send_mac_buf[37]=0;//ICMP校验和

                    send_mac_buf[38]=recv_buf[38];
                    send_mac_buf[39]=recv_buf[39];  //标识符 回显

                    send_mac_buf[40]=recv_buf[40];
                    send_mac_buf[41]=recv_buf[41];  //顺序号 回显

                    //xtm_memcopy(send_mac_buf + 42, recv_buf + 42, data_len);    //数据 回显

                    Build_Ip_Crc(send_mac_buf);             //ip 首部校验
                    //Build_Icmp_Crc(send_mac_buf, data_len); //icmp 校验
                    Build_Icmp_Crc_1(send_mac_buf, recv_buf + 42, data_len); //icmp 校验

                    send_mac_packet(42, send_mac_buf, data_len, recv_buf + 42);

                    RefreshArp_Table(recv_buf + 6, recv_buf + 26);
                   }
                  //xtm_printf("recv ping ask ip_addr = %d.%d.%d.%d, len = %d\n", recv_buf[26], recv_buf[27], recv_buf[28], recv_buf[29], recv_len);
                 }
                break;
      case 0x00:  //应答
                RefreshArp_Table(recv_buf + 6, recv_buf + 26);
                break;
     }
    return 1;
   }
  return 0;
}
//---------------------------------------------------------------------------
INT8U Igmp_Deal(INT8U *recv_buf, INT16U recv_len)
{
  INT16U data_len;
  if((recv_buf[12] == 0x08)&&(recv_buf[13] == 0x00)&&(recv_buf[14] == 0x46)&&(recv_buf[15] == 0x00)&&(recv_buf[23] == 0x02))  //IGMP
   {
    switch(recv_buf[38])
     {
      case 0x11: //会员查询
                xtm_printf("Igmp_Deal::会员查询 %d.%d.%d.%d, from  %d.%d.%d.%d\n", recv_buf[42], recv_buf[43], recv_buf[44], recv_buf[45], recv_buf[26], recv_buf[27], recv_buf[28], recv_buf[29]);
                break;
      case 0x22: //第3版本会员报告
                xtm_printf("Igmp_Deal::第3版本会员报告 %d.%d.%d.%d, from %d.%d.%d.%d\n", recv_buf[42], recv_buf[43], recv_buf[44], recv_buf[45], recv_buf[26], recv_buf[27], recv_buf[28], recv_buf[29]);
                break;
      case 0x12: //第2版本会员报告
                //xtm_printf("Igmp_Deal::第2版本会员报告 %d.%d.%d.%d, from %d.%d.%d.%d\n", recv_buf[42], recv_buf[43], recv_buf[44], recv_buf[45], recv_buf[26], recv_buf[27], recv_buf[28], recv_buf[29]);
                break;
      case 0x16: //第2版本会员报告
                //xtm_printf("Igmp_Deal::第2版本会员报告 %d.%d.%d.%d, from %d.%d.%d.%d\n", recv_buf[42], recv_buf[43], recv_buf[44], recv_buf[45], recv_buf[26], recv_buf[27], recv_buf[28], recv_buf[29]);
                break;
      case 0x17: //第2版本离开组
                xtm_printf("Igmp_Deal::第2版本离开组 %d.%d.%d.%d, from %d.%d.%d.%d\n", recv_buf[42], recv_buf[43], recv_buf[44], recv_buf[45], recv_buf[26], recv_buf[27], recv_buf[28], recv_buf[29]);
                break;
     }
    return 1;
   }
  return 0;
}
//---------------------------------------------------------------------------
void AddMultiGroup(INT8U *McastAddr)  //加入组播组
{
  send_mac_buf[0] = 0x01;
  send_mac_buf[1] = 0x00;
  send_mac_buf[2] = 0x5E;
  send_mac_buf[3] = McastAddr[1];
  send_mac_buf[4] = McastAddr[2];
  send_mac_buf[5] = McastAddr[3];          //对方MAC地址
  xtm_memcopy(send_mac_buf + 6, LocalCfg.Mac_Addr, 6);    //本机MAC地址

  send_mac_buf[12]= 0x8;
  send_mac_buf[13]= 0x0;            //Mac包长度

  send_mac_buf[14]=0x46;
  send_mac_buf[15]=0x00;      //版本、首部长度及服务类型

  send_mac_buf[16]= 0;
  send_mac_buf[17]= 0x20; //IP包总长度

  send_mac_buf[18]= (ip_pack_no >> 8) & 0xff;
  send_mac_buf[19]= ip_pack_no & 0xff;   //IP首部标识

  send_mac_buf[20]=0x0;
  send_mac_buf[21]= 0x0;  //标志和片偏移

  send_mac_buf[22]=0x40; //生存时间
  send_mac_buf[23]= 0x02;  //IGMP协议

  send_mac_buf[24]=0;
  send_mac_buf[25]=0;  // IP首部校验和

  xtm_memcopy(send_mac_buf + 26, LocalCfg.IP, 4);    //本机IP地址
  xtm_memcopy(send_mac_buf + 30, McastAddr, 4);    //对方IP地址

  send_mac_buf[34]=0x0;   // 保留
  send_mac_buf[35]=0x0;  //
  send_mac_buf[36]=0x0;  //
  send_mac_buf[37]=0x0;  //

  send_mac_buf[38]=0x16;  //加入组
  send_mac_buf[39]=0x0;  //

  send_mac_buf[40]=0;
  send_mac_buf[41]=0;//IGMP校验和

  xtm_memcopy(send_mac_buf + 42, McastAddr, 4); 


  Build_Ip_Crc(send_mac_buf);             //ip 首部校验
  Build_Igmp_Crc(send_mac_buf); //icmp 校验

  send_mac_packet(512, send_mac_buf, 0, send_mac_buf + 256);
}
//---------------------------------------------------------------------------
void DropMultiGroup(INT8U *McastAddr)  //退出组播组
{
  send_mac_buf[0] = 0x01;
  send_mac_buf[1] = 0x00;
  send_mac_buf[2] = 0x5E;
  send_mac_buf[3] = 0;
  send_mac_buf[4] = 0;
  send_mac_buf[5] = 2;          //对方MAC地址
  xtm_memcopy(send_mac_buf + 6, LocalCfg.Mac_Addr, 6);    //本机MAC地址

  send_mac_buf[12]= 0x8;
  send_mac_buf[13]= 0x0;            //Mac包长度

  send_mac_buf[14]=0x46;
  send_mac_buf[15]=0x00;      //版本、首部长度及服务类型

  send_mac_buf[16]= 0;
  send_mac_buf[17]= 0x20; //IP包总长度

  send_mac_buf[18]= (ip_pack_no >> 8) & 0xff;
  send_mac_buf[19]= ip_pack_no & 0xff;   //IP首部标识

  send_mac_buf[20]=0x0;
  send_mac_buf[21]= 0x0;  //标志和片偏移

  send_mac_buf[22]=0x40; //生存时间
  send_mac_buf[23]= 0x02;  //IGMP协议

  send_mac_buf[24]=0;
  send_mac_buf[25]=0;  // IP首部校验和

  xtm_memcopy(send_mac_buf + 26, LocalCfg.IP, 4);    //本机IP地址
  send_mac_buf[30] = 224;
  send_mac_buf[31] = 0;
  send_mac_buf[32] = 0;
  send_mac_buf[33] = 2;                         //对方IP地址

  send_mac_buf[34]=0x0;   // 保留
  send_mac_buf[35]=0x0;  //
  send_mac_buf[36]=0x0;  //
  send_mac_buf[37]=0x0;  //

  send_mac_buf[38]=0x17;  //离开组
  send_mac_buf[39]=0x0;  //

  send_mac_buf[40]=0;
  send_mac_buf[41]=0;//IGMP校验和

  xtm_memcopy(send_mac_buf + 42, McastAddr, 4); 


  Build_Ip_Crc(send_mac_buf);             //ip 首部校验
  Build_Igmp_Crc(send_mac_buf);           //icmp 校验

  send_mac_packet(512, send_mac_buf, 0, send_mac_buf + 256);
}
//---------------------------------------------------------------------------
//igmp 校验
INT8U Build_Igmp_Crc(INT8U *send_buf)
{
  INT16U i;
  INT16U sum1 = 0;
  INT16U sum2 = 0;
  for(i=38;i<46;i+=2)
   {
    sum1 = sum1 + send_buf[i];
    sum2 = sum2 + send_buf[i + 1];
   }
  //xtm_printf("Build_Ip_Crc 1:: sum1= %X, sum2= %X\n", sum1, sum2);
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
  send_buf[40] = 0xff - sum1;
  send_buf[41] = 0xff - sum2;
  return 1;
}
//---------------------------------------------------------------------------
//ip 首部校验
INT8U Build_Ip_Crc(INT8U *send_buf)
{
  INT16U i;
  INT16U sum1 = 0;
  INT16U sum2 = 0;
  for(i=14;i<34;i+=2)
   {
    sum1 = sum1 + send_buf[i];
    sum2 = sum2 + send_buf[i + 1];
   }
  //xtm_printf("Build_Ip_Crc 1:: sum1= %X, sum2= %X\n", sum1, sum2);
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
  send_buf[24] = 0xff - sum1;
  send_buf[25] = 0xff - sum2;
  return 1;
}
//---------------------------------------------------------------------------
//icmp 校验
INT8U Build_Icmp_Crc(INT8U *send_buf, INT16U data_len)
{
  INT16U i;
  INT16U sum1 = 0;
  INT16U sum2 = 0;
  if((data_len%2)!=0)      //数据为奇数个，则补一个字节
   {
    send_buf[42+data_len]=0;
    for(i=34;i<(42+data_len+1);i+=2)
     {
      sum1 = sum1 + send_buf[i];
      sum2 = sum2 + send_buf[i + 1];
     }
   }
  else
   {
    for(i=34;i<(42+data_len);i+=2)
     {
      sum1 = sum1 + send_buf[i];
      sum2 = sum2 + send_buf[i + 1];
     }
   }

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
  send_buf[36] = 0xff - sum1;
  send_buf[37] = 0xff - sum2;
  return 1;
}
//---------------------------------------------------------------------------
INT8U Build_Icmp_Crc_1(INT8U *send_buf, INT8U *data_buf, INT16U data_len)
{
  INT16U i;
  INT32U sum1 = 0;
  INT32U sum2 = 0;
  for(i=34; i<42; i+=2)
   {
    sum1 = sum1 + send_buf[i];
    sum2 = sum2 + send_buf[i + 1];
   }
  if((data_len%2) != 0)      //数据为奇数个，则补一个字节
   {
    send_buf[42+data_len]=0;
    data_buf[data_len] = 0;
    for(i=0; i<(data_len+1); i+=2)
     {
      sum1 = sum1 + data_buf[i];
      sum2 = sum2 + data_buf[i + 1];
     }
   }
  else
   {
    for(i=0; i<data_len; i+=2)
     {
      sum1 = sum1 + data_buf[i];
      sum2 = sum2 + data_buf[i + 1];
     }
   }
  //xtm_printf("Build_Icmp_Crc_1 :: sum1= %X %X, sum2= %X %X\n", (INT16U)(((INT32U)sum1 & 0xffff0000) >> 16), (INT16U)((INT32U)sum1 & 0xffff), \
  //    (INT16U)(((INT32U)sum2 & 0xffff0000) >> 16), (INT16U)((INT32U)sum2 & 0xffff));

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
  send_buf[36] = 0xff - sum1;
  send_buf[37] = 0xff - sum2;
  return 1;
}
//---------------------------------------------------------------------------
void 	send_mac_packet_1(INT16U len,INT8U *udpdata)
{
  INT16U i;

  //xtm_printf("send_mac_packet 1:: len = %d\n", len);

  outportb(BYTE_WR_MOST_ADD, 0x1a);
  outportb(BYTE_WR_HIGH_ADD, 0x00);

  outportb(BYTE_WR_LOW_ADD, 0x40);

  for(i=0; i<len; i++)
    outportb(BYTE_WR_DATA, udpdata[i]);

  if(len < 60)    //20121102 512 -- 18 , MAC最小为60
    i = 60;
  else
    i = len;
  outport(TXPL_LOW, i&0xff);
  outport(TXPL_HIGH, i>>8);
  outportb(TX_START, 1);
  i = 0;
  while(inportb(TX_STATUS)&0x01)
   {
          asm{
               nop
               nop
               nop
               nop
               nop
               nop
               nop
               nop
               nop
               nop
             }
    i ++;
    if(i > 8600)  //430*20   20ms
     {
      xtm_printf("send_mac_packet_1::timeout, i= %d\n", i);
      break;
     }
   }
  outportb(TX_START, 0x0);

  ip_pack_no ++;  //ip首部标识

  //xtm_printf("send_mac_packet::ip_pack_no = %d\n", ip_pack_no);
}
//---------------------------------------------------------------------------
void send_mac_packet(INT16U len1, INT8U *data1, INT16U len2, INT8U *data2)
{
#if 1
  INT16U i;
  INT32U addr;

  addr = (((INT32U)data1&0xffff)+(((INT32U)data1&0xffff0000)>>12))|0x700000;
  outportb(TX_SA_H, addr>>16);
  outportb(TX_SA_M, addr>>8);
  outportb(TX_SA_L, addr);

  outportb(TXPL_LOW, len1);
  outportb(TXPL_HIGH, len1 >> 8);


  //xtm_printf("send_mac_packet::len1 = %d, len2 = %d\n", len1, len2);

  if(len2 != 0)
   {
    addr = (((INT32U)data2&0xffff)+(((INT32U)data2&0xffff0000)>>12))|0x700000;
    outportb(TX_SA2_H, addr>>16);
    outportb(TX_SA2_M, addr>>8);
    outportb(TX_SA2_L, addr);

    outportb(TXPL2_LOW, len2);
    outportb(TXPL2_HIGH, len2 >> 8);
   }

  if(len2 == 0)
    outportb(TX_START, inportb(TX_START)|0x1);
  else
    outportb(TX_START, inportb(TX_START)|0x9);

  i = 0;
  while(inportb(TX_STATUS)&0x01)
   {
    i ++;
    if(i > 8600)  //430*20   20ms
     {
      xtm_printf("send_mac_packet::timeout, i= %d\n", i);
      break;
     }
   }

  if(len2 == 0)
    outportb(TX_START, inportb(TX_START)&0xfe);
  else
    outportb(TX_START, inportb(TX_START)&0xf6);

  outportb(TX_SA_H, 0x1a);//恢复通用包起始地址
  outportb(TX_SA_M, 0x00);
  outportb(TX_SA_L, 0x40);

  ip_pack_no ++;  //ip首部标识
#endif
}
//---------------------------------------------------------------------------
void send_mac_video_packet(INT16U len1, INT8U *data1, INT16U len2, INT32U video_addr)
{
  INT16U i;
  INT32U addr;

  addr = (((INT32U)data1&0xffff)+(((INT32U)data1&0xffff0000)>>12))|0x700000;
  outportb(TX_SA_H, addr>>16);
  outportb(TX_SA_M, addr>>8);
  outportb(TX_SA_L, addr);

  outportb(TXPL_LOW, len1);
  outportb(TXPL_HIGH, len1 >> 8);


  // xtm_printf("send_mac_packet::len1 = %d, len2 = %d\n", len1, len2);
  if(len2 != 0)
   {
    outportb(TX_SA2_H, video_addr>>16);
    outportb(TX_SA2_M, video_addr>>8);
    outportb(TX_SA2_L, video_addr);

    outportb(TXPL2_LOW, len2);
    outportb(TXPL2_HIGH, len2 >> 8);
   }

  if(len2 == 0)
    outportb(TX_START, inportb(TX_START)|0x1);
  else
    outportb(TX_START, inportb(TX_START)|0x9);

  i = 0;
  while(inportb(TX_STATUS)&0x01)
   {
    i ++;
    if(i > 8600)  //430*20   20ms
     {
      xtm_printf("send_mac_video_packet::timeout, i= %d\n", i);
      break;
     }
   }        

  if(len2 == 0)
    outportb(TX_START, inportb(TX_START)&0xfe);
  else
    outportb(TX_START, inportb(TX_START)&0xf6);

  outportb(TX_SA_H, 0x1a);//恢复通用包起始地址
  outportb(TX_SA_M, 0x00);
  outportb(TX_SA_L, 0x40);

  ip_pack_no ++;  //ip首部标识
}
//---------------------------------------------------------------------------
void InitArp_Table(void)
{
  INT8U i;
  for(i=0; i<ARP_TABLE_NUM; i++)
    Arp_Table[i].surv_time = 0;
}
//---------------------------------------------------------------------------
//更新ARP缓存表
INT8U RefreshArp_Table(INT8U *r_mac, INT8U *r_ip)
{
  INT8U i;
  INT8U pos;
  for(i=0; i<ARP_TABLE_NUM; i++)
   {
    if((r_ip[0] == Arp_Table[i].ip[0])&&(r_ip[1] == Arp_Table[i].ip[1])&&(r_ip[2] == Arp_Table[i].ip[2])&&(r_ip[3] == Arp_Table[i].ip[3]))
      if(Arp_Table[i].surv_time != 0)
       {
        xtm_memcopy(Arp_Table[i].mac, r_mac, 6);
        Arp_Table[i].surv_time = ARP_REFRESH_TIME;
        return 1;
       }
   }

  for(i=0; i<ARP_TABLE_NUM; i++)
   {
    if(Arp_Table[i].surv_time == 0)
     {
        xtm_memcopy(Arp_Table[i].mac, r_mac, 6);
        xtm_memcopy(Arp_Table[i].ip, r_ip, 4);
        Arp_Table[i].surv_time = ARP_REFRESH_TIME;

        //xtm_printf("RefreshArp_Table:: Add No:%d  %X:%X:%X:%X:%X:%X  --- %d.%d.%d.%d  --- %d\n", i, Arp_Table[i].mac[0], Arp_Table[i].mac[1], Arp_Table[i].mac[2], Arp_Table[i].mac[3], Arp_Table[i].mac[4], Arp_Table[i].mac[5], \
        //           Arp_Table[i].ip[0], Arp_Table[i].ip[1], Arp_Table[i].ip[2], Arp_Table[i].ip[3], Arp_Table[i].surv_time);
        return 1;
     }
   }

  pos = 0;
  for(i=1; i<ARP_TABLE_NUM; i++)
   {
    if(Arp_Table[i].surv_time < Arp_Table[pos].surv_time)
     {
       pos = i;
     }
   }

  xtm_memcopy(Arp_Table[pos].mac, r_mac, 6);
  xtm_memcopy(Arp_Table[pos].ip, r_ip, 4);
  Arp_Table[pos].surv_time = ARP_REFRESH_TIME;
}
//---------------------------------------------------------------------------
void Arp_Table_Timer(void)
{
  INT8U i;
  for(i=0; i<ARP_TABLE_NUM; i++)
   {
      if(Arp_Table[i].surv_time != 0)
       {
        //xtm_printf("Arp_Table_Timer:: %d, %d\n", i, Arp_Table[i].surv_time);
        Arp_Table[i].surv_time --;
       }
   }
}
//---------------------------------------------------------------------------
INT8U SearchArp_Table(INT8U *DestIP, INT8U *DestMac)
{
  INT8U i;
  for(i=0; i<ARP_TABLE_NUM; i++)
   {
      if(Arp_Table[i].surv_time != 0)
       if((DestIP[0] == Arp_Table[i].ip[0])&&(DestIP[1] == Arp_Table[i].ip[1])&&(DestIP[2] == Arp_Table[i].ip[2])&&(DestIP[3] == Arp_Table[i].ip[3]))
       {
        xtm_memcopy(DestMac, Arp_Table[i].mac, 6);
        return 1;
       }
   }
  return 0;
}
//---------------------------------------------------------------------------
void SendArp_Ask(INT8U *DestIP)   //发送 ARP请求表
{
  INT8U i;
  for(i=0; i<6; i++)
    send_mac_buf[i] = 0xFF;        //对方MAC地址
  xtm_memcopy(send_mac_buf + 6, LocalCfg.Mac_Addr, 6);    //本机MAC地址
  send_mac_buf[12] = 0x8;
  send_mac_buf[13] = 0x6;
  send_mac_buf[14] = 0x0;
  send_mac_buf[15] = 0x1;
  send_mac_buf[16] = 0x8;
  send_mac_buf[17] = 0x0;
  send_mac_buf[18] = 0x6;
  send_mac_buf[19] = 0x4;
  send_mac_buf[20] = 0x0;

  send_mac_buf[21] = 0x01;                      //应答
  xtm_memcopy(send_mac_buf + 22, LocalCfg.Mac_Addr, 6);    //本机MAC地址
  xtm_memcopy(send_mac_buf + 28, LocalCfg.IP, 4);    //本机IP地址
  for(i=32; i<38; i++)
    send_mac_buf[i] = 0xFF;        //对方MAC地址
  xtm_memcopy(send_mac_buf + 38, DestIP, 4);    //对方IP地址
  //send_mac_packet(512, send_mac_buf, 0, send_mac_buf);
  send_mac_packet(60, send_mac_buf, 0, send_mac_buf);
}
//---------------------------------------------------------------------------
void SendFreeArp(void)
{
  INT8U i;
  Local.Conflict_Flag = 0;    //IP地址冲突
  for(i=0; i<6; i++)
    Local.Conflict_Mac[i] = 0x0;
  SendArp_Ask(LocalCfg.IP);
}
//---------------------------------------------------------------------------
void PrintArp_Table(void)
{
  INT8U i, j;
  for(i=0; i<ARP_TABLE_NUM; i++)
   {
    if(Arp_Table[i].surv_time != 0)
     {
       xtm_printf("PrintArp_Table:: No:%d  %X:%X:%X:%X:%X:%X  --- %d.%d.%d.%d  --- %d\n", i, Arp_Table[i].mac[0], Arp_Table[i].mac[1], Arp_Table[i].mac[2], Arp_Table[i].mac[3], Arp_Table[i].mac[4], Arp_Table[i].mac[5], \
                   Arp_Table[i].ip[0], Arp_Table[i].ip[1], Arp_Table[i].ip[2], Arp_Table[i].ip[3], Arp_Table[i].surv_time);
     }
   }
}
//---------------------------------------------------------------------------
