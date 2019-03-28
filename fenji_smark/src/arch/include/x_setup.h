#ifndef x_setup_h
#define x_setup_h

extern void DisplaySetupWindow(INT8U RefreshType);    //显示进入设置窗口
extern void PreDealSetupWindow(INT8U wintype);    //进入设置窗口预处理，按键按下效果
extern void OperateSetupWindow(INT8U wintype, INT8U currwindow);    //进入设置窗口操作
extern void CheckTouchSetupWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void DisplaySetupMainWindow(INT8U RefreshType);    //显示设置主窗口
extern void PreDealSetupMainWindow(INT8U wintype);    //设置主窗口预处理，按键按下效果
extern void OperateSetupMainWindow(INT8U wintype, INT8U currwindow);    //设置主窗口操作
extern void CheckTouchSetupMainWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void PlayInputErrorWav(void);
extern void PlayPassErrorWav(void);
extern void PlayModiSuccWav(void);
extern void PlayModiFailWav(void);

extern void DisplayKbWindow(INT8U EditType);    //显示键盘窗口
extern void PreDealKbWindow(INT8U wintype);    //键盘窗口预处理，按键按下效果
extern void OperateKbWindow(INT8U wintype, INT8U currwindow);    //键盘窗口操作
extern void CheckTouchKbWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void WriteNullKb_Edit(INT8U FullPageType);
extern INT8U Kb_Edit_Deal(void);  //Kb 文本框信息处理


extern void DisplayLanWindow(INT8U RefreshType, INT8U Curr_Sec);    //LAN设置窗口
extern void PreDealLanWindow(INT8U wintype);    //LAN设置窗口预处理，按键按下效果
extern void OperateLanWindow(INT8U wintype, INT8U currwindow);    //LAN设置窗口操作
extern void CheckTouchLanWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void DisplayAddrWindow(INT8U RefreshType, INT8U Curr_Sec);    //房间设置窗口
extern void PreDealAddrWindow(INT8U wintype);    //房间设置窗口预处理，按键按下效果
extern void OperateAddrWindow(INT8U wintype, INT8U currwindow);    //房间设置窗口操作
extern void CheckTouchAddrWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void DisplayPassSetupWindow(INT8U RefreshType, INT8U Curr_Sec);    //修改密码窗口
extern void PreDealPassSetupWindow(INT8U wintype);    //修改密码窗口预处理，按键按下效果
extern void OperatePassSetupWindow(INT8U wintype, INT8U currwindow);    //修改密码窗口操作
extern void CheckTouchPassSetupWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void DisplayScreenSetupWindow(INT8U RefreshType);    //屏幕调节窗口
extern void PreDealScreenSetupWindow(INT8U wintype);    //屏幕调节窗口预处理，按键按下效果
extern void OperateScreenSetupWindow(INT8U wintype, INT8U currwindow);    //屏幕调节窗口操作
extern void CheckTouchScreenSetupWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void DisplayVoiceSetupWindow(INT8U RefreshType);    //音量调节窗口
extern void PreDealVoiceSetupWindow(INT8U wintype);    //音量调节窗口预处理，按键按下效果
extern void OperateVoiceSetupWindow(INT8U wintype, INT8U currwindow);    //音量调节窗口操作
extern void CheckTouchVoiceSetupWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void DisplayVersionWindow(INT8U RefreshType);    //显示版本窗口
extern void PreDealVersionWindow(INT8U wintype);    //版本窗口预处理，按键按下效果
extern void OperateVersionWindow(INT8U wintype, INT8U currwindow);    //版本操作
extern void CheckTouchVersionWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作
extern void RefreshVersionWindow(INT8U FullPageType);    //刷新版本信息窗口

extern void DisplayCalibrateWindow(void);    //校准触摸屏窗口
extern void PreDealCalibrateWindow(INT8U wintype);    //校准触摸屏窗口预处理，按键按下效果
extern void OperateCalibrateWindow(INT8U wintype, INT8U currwindow);  //校准触摸屏操作
extern void CheckTouchCalibrateWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern INT16 Calib_X[4];
extern INT16 Calib_Y[4];
extern INT16 Calib_OldX[4];
extern INT16 Calib_OldY[4];
#endif