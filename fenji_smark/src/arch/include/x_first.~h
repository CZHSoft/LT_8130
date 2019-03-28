#ifndef x_first_h
#define x_first_h

extern void InterfaceInit(void);

extern void DisplayFirstWindow(void);    //显示主窗口
extern void PreDealFirstWindow(INT8U wintype);    //主窗口预处理，按键按下效果
extern void OperateFirstWindow(INT8U wintype, INT8U currwindow);    //主窗口操作
extern void CheckTouchFirstWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void RefreshBackWindow(INT8U RefreshType, INT8U FullPageType); //刷新背景
extern void DisplayStateBar(void);      //状态条
extern void PlaySoundTip(void); //播放提示音

extern void PreDealBigmenuButton(INT8U wintype);  //快捷按键 预处理
extern void ShowPressQuickmenu_Button(INT8U wintype);  //快捷按键   弹起
extern void OperateQuickmenu(INT8U wintype, INT8U currtype);
#endif