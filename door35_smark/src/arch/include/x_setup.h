#ifndef x_setup_h
#define x_setup_h

extern void DisplaySetupWindow(INT8U RefreshType);    //显示进入设置窗口
extern void OperateSetupWindow(INT8U wintype, INT8U currwindow);    //进入设置窗口操作

extern void DisplaySetupMainWindow(INT8U RefreshType);    //显示设置主窗口
extern void OperateSetupMainWindow(INT8U wintype, INT8U currwindow);    //设置主窗口操作

extern void PlayInputErrorWav(void);
extern void PlayPassErrorWav(void);
extern void PlayModiSuccWav(void);
extern void PlayModiFailWav(void);

extern void DisplayAddressWindow(void);    //显示本机地址变更窗口
extern void OperateAddressWindow(INT8U wintype, INT8U currwindow);  //本机地址变更操作

extern void DisplayParamWindow(void);    //显示参数设置窗口
extern void OperateParamWindow(INT8U wintype, INT8U currwindow);  //修改参数设置操作

extern void DisplayPassWindow(void);    //显示密码设置窗口
extern void OperatePassWindow(INT8U wintype, INT8U currwindow);  //修改密码设置操作

extern void DisplayOtherWindow(void);    //其它设置窗口
extern void OperateOtherWindow(INT8U wintype, INT8U currwindow);  //其它设置操作

extern void DisplayOnlineTestWindow(void);    //显示在线检测窗口
extern void OperateOnlineTestWindow(INT8U wintype, INT8U currwindow);  //在线检测窗口操作
extern void CheckOnlineTest(void);

void DisplaySetupInfoWindow(void);    //显示关于窗口
void OperateSetupInfoWindow(INT8U wintype, INT8U currwindow);  //关于窗口操作
#endif