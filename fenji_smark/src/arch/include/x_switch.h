#ifndef x_switch_h
#define x_switch_h

extern void DisplaySplashWindow(INT8U RefreshType);    //��ʾSplash
extern void PreDealSplashWindow(INT8U wintype);    //Splash����Ԥ����
extern void CheckTouchSplashWindow(INT16U XLCD, INT16U YLCD); //��鴥��������
extern void OperateSplashWindow(INT8U wintype, INT8U currwindow); //splash���ڲ���

extern void DisplaySwitchSetWindow(INT8U RefreshType);    //��ʾ
extern void PreDealSwitchSetWindow(INT8U wintype);    //���°���ʱ��Ч��
extern void CheckTouchSwitchSetWindow(INT16U XLCD, INT16U YLCD); //��鴥��������
extern void OperateSwitchSetWindow(INT8U wintype, INT8U currwindow); 

#endif
