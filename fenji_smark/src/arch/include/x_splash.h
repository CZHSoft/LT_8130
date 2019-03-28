#ifndef x_splash_h
#define x_splash_h

extern void DisplaySplashWindow(INT8U RefreshType);    //显示Splash
extern void PreDealSplashWindow(INT8U wintype);    //Splash窗口预处理
extern void CheckTouchSplashWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作
extern void OperateSplashWindow(INT8U wintype, INT8U currwindow); //splash窗口操作

#endif