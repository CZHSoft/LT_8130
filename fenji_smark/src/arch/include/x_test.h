#ifndef x_test_h
#define x_test_h
extern void DisplayTestWindow(INT8U RefreshType);    //显示测试窗口
extern void PreDealTestWindow(INT8U wintype);    //测试窗口预处理，按键按下效果
extern void OperateTestWindow(INT8U wintype, INT8U currwindow);    //测试窗口操作
extern void CheckTouchTestWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作
#endif

