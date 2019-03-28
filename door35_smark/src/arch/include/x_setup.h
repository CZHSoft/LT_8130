#ifndef x_setup_h
#define x_setup_h

extern void DisplaySetupWindow(INT8U RefreshType);    //��ʾ�������ô���
extern void OperateSetupWindow(INT8U wintype, INT8U currwindow);    //�������ô��ڲ���

extern void DisplaySetupMainWindow(INT8U RefreshType);    //��ʾ����������
extern void OperateSetupMainWindow(INT8U wintype, INT8U currwindow);    //���������ڲ���

extern void PlayInputErrorWav(void);
extern void PlayPassErrorWav(void);
extern void PlayModiSuccWav(void);
extern void PlayModiFailWav(void);

extern void DisplayAddressWindow(void);    //��ʾ������ַ�������
extern void OperateAddressWindow(INT8U wintype, INT8U currwindow);  //������ַ�������

extern void DisplayParamWindow(void);    //��ʾ�������ô���
extern void OperateParamWindow(INT8U wintype, INT8U currwindow);  //�޸Ĳ������ò���

extern void DisplayPassWindow(void);    //��ʾ�������ô���
extern void OperatePassWindow(INT8U wintype, INT8U currwindow);  //�޸��������ò���

extern void DisplayOtherWindow(void);    //�������ô���
extern void OperateOtherWindow(INT8U wintype, INT8U currwindow);  //�������ò���

extern void DisplayOnlineTestWindow(void);    //��ʾ���߼�ⴰ��
extern void OperateOnlineTestWindow(INT8U wintype, INT8U currwindow);  //���߼�ⴰ�ڲ���
extern void CheckOnlineTest(void);

void DisplaySetupInfoWindow(void);    //��ʾ���ڴ���
void OperateSetupInfoWindow(INT8U wintype, INT8U currwindow);  //���ڴ��ڲ���
#endif