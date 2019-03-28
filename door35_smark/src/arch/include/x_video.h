#ifndef x_video_h
#define x_video_h
extern void InitVideo(void);    //20120203 xu
extern void interrupt MJPEGInt(void);

extern void interrupt MJPEGDecInt(void);
extern void interrupt VInt(void);

extern void DisplayVideoWindow(INT16U xLeft,INT16U width,INT16U yTop,INT16U height, INT8U cType);
extern void CloseVideoWindow(void);

extern void StartRecVideo(INT8U Type);     //����Ƶ����
extern void StopRecVideo(void);            //�ر���Ƶ����

extern void StartEncodeVideo(INT8U Type);     //����Ƶ����
extern void StopEncodeVideo(void);            //�ر���Ƶ����

extern void StartVideoClock(void);     //����Ƶ�ɼ�����ʱ��
extern void StopVideoClock(void);      //�ر���Ƶ�ɼ�����ʱ��

extern INT8U Mjpeg_DecodeError;

extern INT8U VideoFrameRate[6];

extern void CheckVideoEncodeBuf(void);

extern void clear_video_ram(void);  //����Ƶ����

extern void CheckEn_J_Status(void);

extern void Reset_Param_Window(void);
#endif