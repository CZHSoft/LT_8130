#ifndef x_splash_h
#define x_splash_h

extern void DisplaySplashWindow(INT8U RefreshType);    //��ʾSplash
extern void PreDealSplashWindow(INT8U wintype);    //Splash����Ԥ����
extern void CheckTouchSplashWindow(INT16U XLCD, INT16U YLCD); //��鴥��������
extern void OperateSplashWindow(INT8U wintype, INT8U currwindow); //splash���ڲ���

#endif