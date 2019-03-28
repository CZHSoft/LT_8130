#ifndef x_first_h
#define x_first_h

extern void InterfaceInit(void);

extern void DisplayFirstWindow(void);    //显示主窗口
extern void PreDealFirstWindow(INT8U wintype);    //主窗口预处理，按键按下效果
extern void OperateFirstWindow(INT8U wintype, INT8U currwindow);    //主窗口操作
extern void CheckTouchFirstWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void CheckFirstWindowFlag(void);  //检查返回首页标志

extern void RefreshBackWindow(INT8U RefreshType, INT8U FullPageType); //刷新背景
extern void DisplayStateBar(void);      //状态条
extern void PlaySoundTip(void); //播放提示音
extern void PlaySoundDoorBell(void); //20131205

extern void PreDealBigmenuButton(INT8U wintype);  //快捷按键 预处理
extern void ShowPressQuickmenu_Button(INT8U wintype);  //快捷按键   弹起
extern void OperateQuickmenu(INT8U wintype, INT8U currtype);

extern void DisplayFortifyStateImage(INT8U State);    //显示布防图标
extern void DisplayInfoStateImage(INT8U State);    //显示信息图标
extern void DisplayPictureStateImage(INT8U State);    //显示留影图标

extern void DisplayLTpwWindow(INT8U RefreshType,INT8U CurrentWindowNo);    //显示进入密码窗口
extern void PreDealLTpwWindow(INT8U wintype);    //进入密码窗口预处理，按键按下效果
extern void OperateLTpwWindow(INT8U wintype, INT8U currwindow);    //进入密码窗口操作
extern void CheckTouchLTpwWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

//供lib 调用
extern INT32U GetLocal_fb_addr(void);
extern INT8U GetLocal_fb_page(void);
extern INT8U *Get_jpeg_bitstream(void);
extern INT8U Get_Mjpeg_DecodeError(void);
extern void Set_Local_DecodeJpegFlag(INT8U disp_DecodeJpegFlag);
extern INT8U Get_Local_DecodeStreamFlag(void);
#endif