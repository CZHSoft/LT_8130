#ifndef x_talk_h
#define x_talk_h

extern void Talk_Func(void);       //通话 接听
extern void Call_Func(INT8U uFlag, char *call_addr, char *CenterAddr);    //呼叫   1  中心  2 住户
extern void Watch_Func(void);       //监视
extern void TalkEnd_Func(void);
extern void WatchEnd_Func(void);
extern void CallTimeOut_Func(void); //呼叫超时
extern void CheckDelayOpenLock_Func(void);//检查开锁延时
extern void OpenLock_Func(void);  //开锁

extern void CreateTalkBuff(INT8U *sendb, INT8U Order1, INT8U Order2, INT8U Order3, INT8U *Remote_Addr, INT8U *Remote_IP);

extern void CreateNSBuff(INT8U *send_b, INT8U Order1, INT8U Order2);

extern void DisplayTalkWindow(INT8U RefreshType);    //显示对讲呼出窗口
extern void OperateTalkWindow(INT8U wintype, INT8U currwindow);    //对讲呼出窗口操作

extern void DisplayWatchWindow(INT8U RefreshType);    //显示监视窗口
extern void OperateWatchWindow(INT8U wintype, INT8U currwindow);    //监视窗口操作
extern void ShowTalkWatchTime(INT8U cType, INT8U RefreshType); //显示监视时间   cType  0  监视   1  呼叫正在连接  2 通话

extern void DisplayTalkConnectWindow(void);    //显示对讲 正在连接窗口
extern void OperateTalkConnectWindow(INT8U wintype, INT8U currwindow);    //对讲 正在连接窗口操作
extern void ShowTalkConnectImage(INT8U RefreshType); //呼叫正在连接

extern void DisplayTalkConnectWindow(void);    //显示对讲 正在连接窗口
extern void OperateTalkConnectWindow(INT8U wintype, INT8U currwindow);    //对讲 正在连接窗口操作

extern void DisplayTalkStartWindow(void);    //显示对讲 通话窗口
extern void OperateTalkStartWindow(INT8U wintype, INT8U currwindow);    //对讲 通话窗口操作

extern void DisplayOpenLockWindow(void);    //显示密码开锁窗口
extern void OperateOpenLockWindow(INT8U wintype, INT8U currwindow);    //密码开锁窗口操作

extern void DisplayWatchWindow(INT8U RefreshType);    //显示监视窗口
extern void PreDealWatchWindow(INT8U wintype);    //监视窗口预处理，按键按下效果
extern void OperateWatchWindow(INT8U wintype, INT8U currwindow);    //监视窗口操作

extern void DisplayCallFailWindow(INT8U cType);    //显示呼叫失败窗口
extern void OperateCallFailWindow(INT8U wintype, INT8U currwindow);    //呼叫失败窗口操作

extern void DisplayOpenLockResultWindow(void);    //显示密码开锁成功窗口
extern void OperateOpenLockResultWindow(INT8U wintype, INT8U currwindow);    //密码开锁窗口成功操作

extern void DisplayCallFailWindow(INT8U cType);    //显示呼叫失败窗口
extern void OperateCallFailWindow(INT8U wintype, INT8U currwindow);    //呼叫失败窗口操作

extern struct TalkCall1 TalkCall;

extern void PlayBackRingWav(void);
extern void PlayDoorOpenWav(void);
#endif