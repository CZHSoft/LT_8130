#ifndef x_udp_h
#define x_udp_h
extern void Init_Udp(void);    //初始化UDP
extern INT8U Udp_Deal(INT8U *recv_buf, INT16U recv_len);    //UDP 处理

extern void CheckAudioUdpBuff(void);   //检查Audio UDP发送缓存
extern void CheckMultiUdpBuff(void);   //检查UDP发送缓存

extern INT8U Build_Udp_Ip_Crc(INT8U *send_buf);
//udp 校验
extern INT8U Build_Udp_Crc(INT8U *send_buf, INT16U data_len);

extern void CheckFlashSecterErase(INT32U paddr);

extern void CreateMultiUdpBuff(INT16U DestPort, INT8U SendMode, INT8U *RemoteIP, INT8U *udpdata, INT16U nlength, INT8U CurrOrder);
extern void CreateMultiUdpBuff_Audio(INT16U DestPort, INT8U SendMode, INT8U *RemoteIP, INT8U *udpdata, INT16U nlength, INT8U CurrOrder); //给音频中断调用
extern void CheckMultiUdpBuff(void);   //检查UDP发送缓存

extern INT8U UdpSendBuff_1(INT16U DestPort, INT8U *RemoteMac, INT8U *RemoteIP, INT8U *udpdata, INT16U nlength);

extern void send_debug_info(char *debug_info);

extern INT16U video_decode_no;  //视频解码包号
extern INT16U old_mjpeg_stream_no;  //视频解码包号

extern void SendReadIDCardReply(void);
#endif