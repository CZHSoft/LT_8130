#ifndef x_ip_h
#define x_ip_h

extern INT8U Arp_Deal(INT8U *recv_buf, INT16U recv_len);
extern INT8U Icmp_Deal(INT8U *recv_buf, INT16U recv_len);

extern INT8U Igmp_Deal(INT8U *recv_buf, INT16U recv_len);
extern void AddMultiGroup(INT8U *McastAddr);  //加入组播组
extern void DropMultiGroup(INT8U *McastAddr);  //退出组播组

extern void InitArp_Table(void);
extern INT8U RefreshArp_Table(INT8U *r_mac, INT8U *r_ip); //更新ARP缓存表
extern void Arp_Table_Timer(void);
extern INT8U SearchArp_Table(INT8U *DestIP, INT8U *DestMac);
extern void SendArp_Ask(INT8U *DestIP);   //发送 ARP请求表
extern void PrintArp_Table(void);
extern void SendFreeArp(void);


extern void 	send_mac_packet_1(INT16U len, INT8U *udpdata);
extern void send_mac_packet(INT16U len1, INT8U *data1, INT16U len2, INT8U *data2);
extern void send_mac_video_packet(INT16U len1, INT8U *data1, INT16U len2, INT32U video_addr);

extern INT8U *send_mac_buf;
extern INT8U *send_mac_buf2;

extern INT16U ip_pack_no;  //ip首部标识
#endif