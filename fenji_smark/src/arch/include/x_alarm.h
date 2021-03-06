#ifndef x_alarm_h
#define x_alarm_h

extern void DisplayCancelFortifyWindow(INT8U RefreshType);    //显示撤防窗口
extern void PreDealCancelFortifyWindow(INT8U wintype);    //撤防窗口预处理，按键按下效果
extern void OperateCancelFortifyWindow(INT8U wintype, INT8U currwindow);    //撤防窗口操作
extern void CheckTouchCancelFortifyWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作
extern void CancelFortifyDeal(void); //撤防界面处理
extern void PlayCancelFortifyWav(void);
extern void PlayDefenceDelayWav(void); //播放 布防延时中 提示

extern void PlayCancelAlarmWav(void);

extern void DisplayFortifyWindow(INT8U RefreshType);    //显示布防窗口
extern void PreDealFortifyWindow(INT8U wintype);    //布防窗口预处理，按键按下效果
extern void OperateFortifyWindow(INT8U wintype, INT8U currwindow);    //布防窗口操作
extern void CheckTouchFortifyWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void DisplayAlarmWindow(INT8U RefreshType);    //显示报警窗口
extern void PreDealAlarmWindow(INT8U wintype);    //报警窗口预处理，按键按下效果
extern void OperateAlarmWindow(INT8U wintype, INT8U currwindow);    //报警窗口操作
extern void CheckTouchAlarmWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void ShowAlarmImage(void);

extern void DisplayCancelAlarmWindow(INT8U RefreshType);    //显示取消报警窗口
extern void PreDealCancelAlarmWindow(INT8U wintype);    //取消报警窗口预处理，按键按下效果
extern void OperateCancelAlarmWindow(INT8U wintype, INT8U currwindow);    //取消报警窗口操作
extern void CheckTouchCancelAlarmWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作
extern void CancelAlarmDeal(void); //取消报警界面处理

extern void CheckDefenceDelay_Func(void); //检查布防延时函数
extern void FortifyFinishDeal(void); //布防完成界面处理
extern void CheckDelayAlarm(void);  //检查延时报警函数
extern void SendAlarmFunc(INT8U SpecialByte, INT8U AlarmByte[2]); //防区报警函数
extern void SendCancelAlarmFunc(void); //取消报警函数

extern void InitDefence(void);
extern void CheckDefence(unsigned char DefenceByte);    //检测防区函数
#endif