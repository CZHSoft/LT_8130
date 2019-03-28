#ifndef x_talk_h
#define x_talk_h

extern void Talk_Func(void);       //ͨ�� ����
extern void Call_Func(INT8U uFlag, char *call_addr, char *CenterAddr);    //����   1  ����  2 ס��
extern void Watch_Func(void);       //����
extern void TalkEnd_Func(void);
extern void WatchEnd_Func(void);
extern void CallTimeOut_Func(void); //���г�ʱ
extern void CheckDelayOpenLock_Func(void);//��鿪����ʱ
extern void OpenLock_Func(void);  //����

extern void CreateTalkBuff(INT8U *sendb, INT8U Order1, INT8U Order2, INT8U Order3, INT8U *Remote_Addr, INT8U *Remote_IP);

extern void CreateNSBuff(INT8U *send_b, INT8U Order1, INT8U Order2);

extern void DisplayTalkWindow(INT8U RefreshType);    //��ʾ�Խ���������
extern void OperateTalkWindow(INT8U wintype, INT8U currwindow);    //�Խ��������ڲ���

extern void DisplayWatchWindow(INT8U RefreshType);    //��ʾ���Ӵ���
extern void OperateWatchWindow(INT8U wintype, INT8U currwindow);    //���Ӵ��ڲ���
extern void ShowTalkWatchTime(INT8U cType, INT8U RefreshType); //��ʾ����ʱ��   cType  0  ����   1  ������������  2 ͨ��

extern void DisplayTalkConnectWindow(void);    //��ʾ�Խ� �������Ӵ���
extern void OperateTalkConnectWindow(INT8U wintype, INT8U currwindow);    //�Խ� �������Ӵ��ڲ���
extern void ShowTalkConnectImage(INT8U RefreshType); //������������

extern void DisplayTalkConnectWindow(void);    //��ʾ�Խ� �������Ӵ���
extern void OperateTalkConnectWindow(INT8U wintype, INT8U currwindow);    //�Խ� �������Ӵ��ڲ���

extern void DisplayTalkStartWindow(void);    //��ʾ�Խ� ͨ������
extern void OperateTalkStartWindow(INT8U wintype, INT8U currwindow);    //�Խ� ͨ�����ڲ���

extern void DisplayOpenLockWindow(void);    //��ʾ���뿪������
extern void OperateOpenLockWindow(INT8U wintype, INT8U currwindow);    //���뿪�����ڲ���

extern void DisplayWatchWindow(INT8U RefreshType);    //��ʾ���Ӵ���
extern void PreDealWatchWindow(INT8U wintype);    //���Ӵ���Ԥ������������Ч��
extern void OperateWatchWindow(INT8U wintype, INT8U currwindow);    //���Ӵ��ڲ���

extern void DisplayCallFailWindow(INT8U cType);    //��ʾ����ʧ�ܴ���
extern void OperateCallFailWindow(INT8U wintype, INT8U currwindow);    //����ʧ�ܴ��ڲ���

extern void DisplayOpenLockResultWindow(void);    //��ʾ���뿪���ɹ�����
extern void OperateOpenLockResultWindow(INT8U wintype, INT8U currwindow);    //���뿪�����ڳɹ�����

extern void DisplayCallFailWindow(INT8U cType);    //��ʾ����ʧ�ܴ���
extern void OperateCallFailWindow(INT8U wintype, INT8U currwindow);    //����ʧ�ܴ��ڲ���

extern struct TalkCall1 TalkCall;

extern void PlayBackRingWav(void);
extern void PlayDoorOpenWav(void);
#endif