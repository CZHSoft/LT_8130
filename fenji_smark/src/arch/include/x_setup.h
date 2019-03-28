#ifndef x_setup_h
#define x_setup_h

extern void DisplaySetupWindow(INT8U RefreshType);    //��ʾ�������ô���
extern void PreDealSetupWindow(INT8U wintype);    //�������ô���Ԥ������������Ч��
extern void OperateSetupWindow(INT8U wintype, INT8U currwindow);    //�������ô��ڲ���
extern void CheckTouchSetupWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void DisplaySetupMainWindow(INT8U RefreshType);    //��ʾ����������
extern void PreDealSetupMainWindow(INT8U wintype);    //����������Ԥ������������Ч��
extern void OperateSetupMainWindow(INT8U wintype, INT8U currwindow);    //���������ڲ���
extern void CheckTouchSetupMainWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void PlayInputErrorWav(void);
extern void PlayPassErrorWav(void);
extern void PlayModiSuccWav(void);
extern void PlayModiFailWav(void);

extern void DisplayKbWindow(INT8U EditType);    //��ʾ���̴���
extern void PreDealKbWindow(INT8U wintype);    //���̴���Ԥ������������Ч��
extern void OperateKbWindow(INT8U wintype, INT8U currwindow);    //���̴��ڲ���
extern void CheckTouchKbWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void WriteNullKb_Edit(INT8U FullPageType);
extern INT8U Kb_Edit_Deal(void);  //Kb �ı�����Ϣ����


extern void DisplayLanWindow(INT8U RefreshType, INT8U Curr_Sec);    //LAN���ô���
extern void PreDealLanWindow(INT8U wintype);    //LAN���ô���Ԥ������������Ч��
extern void OperateLanWindow(INT8U wintype, INT8U currwindow);    //LAN���ô��ڲ���
extern void CheckTouchLanWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void DisplayAddrWindow(INT8U RefreshType, INT8U Curr_Sec);    //�������ô���
extern void PreDealAddrWindow(INT8U wintype);    //�������ô���Ԥ������������Ч��
extern void OperateAddrWindow(INT8U wintype, INT8U currwindow);    //�������ô��ڲ���
extern void CheckTouchAddrWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void DisplayPassSetupWindow(INT8U RefreshType, INT8U Curr_Sec);    //�޸����봰��
extern void PreDealPassSetupWindow(INT8U wintype);    //�޸����봰��Ԥ������������Ч��
extern void OperatePassSetupWindow(INT8U wintype, INT8U currwindow);    //�޸����봰�ڲ���
extern void CheckTouchPassSetupWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void DisplayScreenSetupWindow(INT8U RefreshType);    //��Ļ���ڴ���
extern void PreDealScreenSetupWindow(INT8U wintype);    //��Ļ���ڴ���Ԥ������������Ч��
extern void OperateScreenSetupWindow(INT8U wintype, INT8U currwindow);    //��Ļ���ڴ��ڲ���
extern void CheckTouchScreenSetupWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void DisplayVoiceSetupWindow(INT8U RefreshType);    //�������ڴ���
extern void PreDealVoiceSetupWindow(INT8U wintype);    //�������ڴ���Ԥ������������Ч��
extern void OperateVoiceSetupWindow(INT8U wintype, INT8U currwindow);    //�������ڴ��ڲ���
extern void CheckTouchVoiceSetupWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void DisplayVersionWindow(INT8U RefreshType);    //��ʾ�汾����
extern void PreDealVersionWindow(INT8U wintype);    //�汾����Ԥ������������Ч��
extern void OperateVersionWindow(INT8U wintype, INT8U currwindow);    //�汾����
extern void CheckTouchVersionWindow(INT16U XLCD, INT16U YLCD); //��鴥��������
extern void RefreshVersionWindow(INT8U FullPageType);    //ˢ�°汾��Ϣ����

extern void DisplayCalibrateWindow(void);    //У׼����������
extern void PreDealCalibrateWindow(INT8U wintype);    //У׼����������Ԥ������������Ч��
extern void OperateCalibrateWindow(INT8U wintype, INT8U currwindow);  //У׼����������
extern void CheckTouchCalibrateWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern INT16 Calib_X[4];
extern INT16 Calib_Y[4];
extern INT16 Calib_OldX[4];
extern INT16 Calib_OldY[4];
#endif