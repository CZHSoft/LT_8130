#ifndef x_talk_h
#define x_talk_h

extern void Talk_Func(void);       //ͨ�� ����
extern void CallCenter_Func(char *CenterAddr);  //��������
extern void CallR2R_Func(INT8U *DestAddr);    //�����Խ�
extern void Watch_Func(void);       //����
extern void TalkEnd_Func(void);
extern void WatchEnd_Func(void);
extern void CallTimeOut_Func(void); //���г�ʱ
extern void OpenLock_Func(void);  //����

extern void CreateTalkBuff(INT8U *sendb, INT8U Order1, INT8U Order2, INT8U Order3, INT8U *Remote_Addr, INT8U *Remote_IP);

extern void CreateNSBuff(INT8U *sendb, INT8U Order1, INT8U Order2);

extern void DisplayTalkWindow(INT8U RefreshType);    //��ʾ�Խ���������
extern void PreDealTalkWindow(INT8U wintype);    //�Խ���������Ԥ������������Ч��
extern void OperateTalkWindow(INT8U wintype, INT8U currwindow);    //�Խ��������ڲ���
extern void CheckTouchTalkWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void DisplayEPWWindow(INT8U RefreshType);    //��ʾ�������봰��
extern void PreDealEPWWindow(INT8U wintype);    //�������봰��Ԥ������������Ч��
extern void OperateEPWWindow(INT8U wintype, INT8U currwindow);    //�������봰�ڲ���
extern void CheckTouchEPWWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void DisplayWatchWindow(INT8U RefreshType);    //��ʾ���Ӵ���
extern void PreDealWatchWindow(INT8U wintype);    //���Ӵ���Ԥ������������Ч��
extern void OperateWatchWindow(INT8U wintype, INT8U currwindow);    //���Ӵ��ڲ���
extern void CheckTouchWatchWindow(INT16U XLCD, INT16U YLCD); //��鴥��������


extern void DisplayTalkWatchPicWindow(INT8U ReturnType, INT8U WinType);    //��ʾ�Խ����봰��
extern void PreDealTalkWatchPicWindow(INT8U wintype);    //�Խ����봰��Ԥ������������Ч��
extern void OperateTalkWatchPicWindow(INT8U wintype, INT8U currwindow);    //�Խ����봰�ڲ���
extern void CheckTouchTalkWatchPicWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern struct TalkCall1 TalkCall;
#endif