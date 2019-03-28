#ifndef x_record_h
#define x_record_h

extern void DisplayRecordsWindow(INT8U RefreshType, INT8U Curr_Sec);    //显示通话记录窗口
extern void PreDealRecordsWindow(INT8U wintype);    //通话记录窗口预处理，按键按下效果
extern void OperateRecordsWindow(INT8U wintype, INT8U currwindow);    //通话记录窗口操作
extern void CheckTouchRecordsWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void DisplayRecordsContentWindow(INT8U recordsno);    //通话记录查看信息窗口
extern void PreDealRecordsContentWindow(INT8U wintype);    //查看通话记录窗口预处理，按键按下效果
extern void OperateRecordsContentWindow(INT8U wintype, INT8U currwindow);    //查看通话记录窗口操作
extern void CheckTouchRecordsContentWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作


extern INT8U Init_RecordsVar(void); //初始化 Records 结构
extern INT8U Uninit_RecordsVar(void); //释放 Records 结构
//读通话记录文件
extern void ReadRecordsFile(void);
//写通话记录文件
extern void WriteRecordsFile(void);

extern void ResetRecordsCfg(void);    //复位通话记录配置文件
extern void WriteRecordsCfgFile(void);     //写通话记录设置文件

extern void Recv_NewRecords(void);
//写照片函数
extern void WritePicFunc(void);
#endif