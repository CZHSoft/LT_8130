#ifndef x_talk_h
#define x_talk_h

extern void Talk_Func(void);       //通话 接听
extern void CallCenter_Func(char *CenterAddr);  //呼叫中心
extern void CallR2R_Func(INT8U *DestAddr);    //户户对讲
extern void Watch_Func(void);       //监视
extern void TalkEnd_Func(void);
extern void WatchEnd_Func(void);
extern void CallTimeOut_Func(void); //呼叫超时
extern void OpenLock_Func(void);  //开锁

extern void CreateTalkBuff(INT8U *sendb, INT8U Order1, INT8U Order2, INT8U Order3, INT8U *Remote_Addr, INT8U *Remote_IP);

extern void CreateNSBuff(INT8U *sendb, INT8U Order1, INT8U Order2);

extern void DisplayTalkWindow(INT8U RefreshType);    //显示对讲呼出窗口
extern void PreDealTalkWindow(INT8U wintype);    //对讲呼出窗口预处理，按键按下效果
extern void OperateTalkWindow(INT8U wintype, INT8U currwindow);    //对讲呼出窗口操作
extern void CheckTouchTalkWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void DisplayEPWWindow(INT8U RefreshType);    //显示进入密码窗口
extern void PreDealEPWWindow(INT8U wintype);    //进入密码窗口预处理，按键按下效果
extern void OperateEPWWindow(INT8U wintype, INT8U currwindow);    //进入密码窗口操作
extern void CheckTouchEPWWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void DisplayWatchWindow(INT8U RefreshType);    //显示监视窗口
extern void PreDealWatchWindow(INT8U wintype);    //监视窗口预处理，按键按下效果
extern void OperateWatchWindow(INT8U wintype, INT8U currwindow);    //监视窗口操作
extern void CheckTouchWatchWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作


extern void DisplayTalkWatchPicWindow(INT8U ReturnType, INT8U WinType);    //显示对讲呼入窗口
extern void PreDealTalkWatchPicWindow(INT8U wintype);    //对讲呼入窗口预处理，按键按下效果
extern void OperateTalkWatchPicWindow(INT8U wintype, INT8U currwindow);    //对讲呼入窗口操作
extern void CheckTouchTalkWatchPicWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern struct TalkCall1 TalkCall;
#endif