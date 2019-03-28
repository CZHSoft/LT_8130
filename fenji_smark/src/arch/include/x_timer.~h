#ifndef x_touch_h
#define x_touch_h

extern INT8U CheckTouchDelayTime(void);  //触摸屏处理时检查延迟

extern void TouchScr_Func(INT16U touch_x, INT16U touch_y); //触摸屏输入
extern INT8U CheckTSInButton(struct TImageButton *t_button, INT16U XLCD, INT16U YLCD); //检查触摸点是否在该按钮内
extern INT8U CheckTSInEdit(struct TEdit *t_edit, INT16U XLCD, INT16U YLCD); //检查触摸点是否在该Edit内
extern INT8U CheckTSInImage(struct TImage *t_image, INT16U XLCD, INT16U YLCD); //检查触摸点是否在该Image内
extern INT8U CheckTSInVideoScreen(INT16U XLCD, INT16U YLCD); //检查触摸点是否在该视频窗口内
extern INT8U CheckTSInPicScreen(INT16U XLCD, INT16U YLCD); //检查触摸点是否在该照片窗口内
extern INT8U CheckBigmenuFunc(INT16U XLCD, INT16U YLCD);  //快捷按钮操作
#endif