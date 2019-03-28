#define MAX_STREAM_NUM  2
#ifndef x_main_h
#define x_main_h
extern void Init_LocalVar(void); //初始化 Local 结构
extern void Init_Module_Clock(void); //根据需要关闭各模块时钟
extern void Set_VideoCapture_Clock(INT8U Type); //操作视频采集时钟  1  开   0  关
extern void Set_Encode_Clock(INT8U Type); //操作视频编码时钟  1  开   0  关
extern void Set_Decode_Clock(INT8U Type); //操作视频解码时钟  1  开   0  关
extern void Set_Disp_Clock(INT8U Type); //操作显示时钟  1  开   0  关

extern void ResetLocalCfg(void);    //复位配置文件

extern void SetPTC1(INT8U freq);
//freq  1  --  正常　　16 --  16分频
extern void SetPTC2(INT8U freq);

//读设置文件
extern void ReadCfgFile(void);
//写设置文件
extern void WriteCfgFile(void);
//写数据到Flash
extern void SaveToFlash(INT32U FlashAddr, INT8U *buf, INT16U length);
//从Flash读数据
extern void ReadFromFlash(INT32U FlashAddr, INT8U *buf, INT16U length);

//读广告配置文件
extern void ReadAdFile(void);
//写广告配置文件
extern void WriteAdFile(void);

extern void ReadSwitchFile(void);

extern void WriteSwitchFile(void);

extern void ReadSerialFile(void);

extern INT8U *jpeg_bitstream;    //静态jpeg 图片
extern INT8U *Flash_Stream;      //Flash存储
extern INT8U *Memory6_Stream;

extern INT8U *mjpeg_stream[MAX_STREAM_NUM];   //mjpeg 流
extern INT16U mjpeg_stream_len[MAX_STREAM_NUM];   //mjpeg 流

extern INT8U *AudioSend_Stream;       //音视频发送Stream
extern INT8U *VideoSend_Stream;       //视频发送Stream

extern INT8U Curr_Mjpeg_Stream;

extern INT8U *Mdata_Stream;      //网卡接收
extern INT8U *Send_Stream;
extern INT8U *Net_Stream[4];       //256K 缓冲  网卡接收
#endif