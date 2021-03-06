#ifndef x_info_h
#define x_info_h

extern void DisplayInfoWindow(INT8U RefreshType, INT8U Curr_Sec);    //显示信息窗口
extern void PreDealInfoWindow(INT8U wintype);    //信息窗口预处理，按键按下效果
extern void OperateInfoWindow(INT8U wintype, INT8U currwindow);    //信息窗口操作
extern void CheckTouchInfoWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern void DisplayInfoContentWindow(INT8U infotype, INT8U infono);    //显示查看信息窗口
extern void PreDealInfoContentWindow(INT8U wintype);    //查看信息窗口预处理，按键按下效果
extern void OperateInfoContentWindow(INT8U wintype, INT8U currwindow);    //查看信息窗口操作
extern void CheckTouchInfoContentWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

extern INT8U Init_InfoVar(void); //初始化 Info 结构
extern INT8U Uninit_InfoVar(void); //释放 Info 结构
//读信息文件
extern void ReadInfoFile(void);
//写信息文件
extern void WriteInfoFile(void);

extern void ResetInfoCfg(void);    //复位信息配置文件
extern void WriteInfoCfgFile(void);     //写信息设置文件

extern void Recv_NewInfo(void);
#endif