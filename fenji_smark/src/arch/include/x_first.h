#ifndef x_first_h
#define x_first_h

extern void InterfaceInit(void);

extern void DisplayFirstWindow(void);    //��ʾ������
extern void PreDealFirstWindow(INT8U wintype);    //������Ԥ������������Ч��
extern void OperateFirstWindow(INT8U wintype, INT8U currwindow);    //�����ڲ���
extern void CheckTouchFirstWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void CheckFirstWindowFlag(void);  //��鷵����ҳ��־

extern void RefreshBackWindow(INT8U RefreshType, INT8U FullPageType); //ˢ�±���
extern void DisplayStateBar(void);      //״̬��
extern void PlaySoundTip(void); //������ʾ��
extern void PlaySoundDoorBell(void); //20131205

extern void PreDealBigmenuButton(INT8U wintype);  //��ݰ��� Ԥ����
extern void ShowPressQuickmenu_Button(INT8U wintype);  //��ݰ���   ����
extern void OperateQuickmenu(INT8U wintype, INT8U currtype);

extern void DisplayFortifyStateImage(INT8U State);    //��ʾ����ͼ��
extern void DisplayInfoStateImage(INT8U State);    //��ʾ��Ϣͼ��
extern void DisplayPictureStateImage(INT8U State);    //��ʾ��Ӱͼ��

extern void DisplayLTpwWindow(INT8U RefreshType,INT8U CurrentWindowNo);    //��ʾ�������봰��
extern void PreDealLTpwWindow(INT8U wintype);    //�������봰��Ԥ������������Ч��
extern void OperateLTpwWindow(INT8U wintype, INT8U currwindow);    //�������봰�ڲ���
extern void CheckTouchLTpwWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

//��lib ����
extern INT32U GetLocal_fb_addr(void);
extern INT8U GetLocal_fb_page(void);
extern INT8U *Get_jpeg_bitstream(void);
extern INT8U Get_Mjpeg_DecodeError(void);
extern void Set_Local_DecodeJpegFlag(INT8U disp_DecodeJpegFlag);
extern INT8U Get_Local_DecodeStreamFlag(void);
#endif