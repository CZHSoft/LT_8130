#ifndef x_video_h
#define x_video_h
extern void InitVideo(void);    //20120203 xu
extern void interrupt MJPEGInt(void);

extern void interrupt MJPEGDecInt(void);
extern void interrupt VInt(void);

extern void DisplayVideoWindow(INT16U xLeft,INT16U width,INT16U yTop,INT16U height, INT8U cType);
extern void CloseVideoWindow(void);

extern void StartRecVideo(INT8U Type);     //打开视频捕获
extern void StopRecVideo(void);            //关闭视频捕获

extern void StartEncodeVideo(INT8U Type);     //打开视频编码
extern void StopEncodeVideo(void);            //关闭视频编码

extern void StartVideoClock(void);     //打开视频采集编码时钟
extern void StopVideoClock(void);      //关闭视频采集编码时钟

extern INT8U Mjpeg_DecodeError;

extern INT8U VideoFrameRate[6];

extern void CheckVideoEncodeBuf(void);

extern void clear_video_ram(void);  //清视频窗口

extern void CheckEn_J_Status(void);

extern void Reset_Param_Window(void);
#endif