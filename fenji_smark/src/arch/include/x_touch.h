#ifndef x_touch_h
#define x_touch_h

extern INT8U CheckTouchDelayTime(void);  //����������ʱ����ӳ�

extern void TouchScr_Func(INT16U touch_x, INT16U touch_y); //����������
extern INT8U CheckTSInButton(struct TImageButton *t_button, INT16U XLCD, INT16U YLCD); //��鴥�����Ƿ��ڸð�ť��
extern INT8U CheckTSInEdit(struct TEdit *t_edit, INT16U XLCD, INT16U YLCD); //��鴥�����Ƿ��ڸ�Edit��
extern INT8U CheckTSInImage(struct TImage *t_image, INT16U XLCD, INT16U YLCD); //��鴥�����Ƿ��ڸ�Image��
extern INT8U CheckTSInVideoScreen(INT16U XLCD, INT16U YLCD); //��鴥�����Ƿ��ڸ���Ƶ������
extern INT8U CheckTSInPicScreen(INT16U XLCD, INT16U YLCD); //��鴥�����Ƿ��ڸ���Ƭ������
extern INT8U CheckBigmenuFunc(INT16U XLCD, INT16U YLCD);  //��ݰ�ť����
#endif