#ifndef x_alarm_h
#define x_alarm_h

extern void DisplayCancelFortifyWindow(INT8U RefreshType);    //��ʾ��������
extern void PreDealCancelFortifyWindow(INT8U wintype);    //��������Ԥ��������������Ч��
extern void OperateCancelFortifyWindow(INT8U wintype, INT8U currwindow);    //�������ڲ���
extern void CheckTouchCancelFortifyWindow(INT16U XLCD, INT16U YLCD); //��鴥��������
extern void CancelFortifyDeal(void); //�������洦��
extern void PlayCancelFortifyWav(void);
extern void PlayDefenceDelayWav(void); //���� ������ʱ�� ��ʾ

extern void PlayCancelAlarmWav(void);

extern void DisplayFortifyWindow(INT8U RefreshType);    //��ʾ��������
extern void PreDealFortifyWindow(INT8U wintype);    //��������Ԥ��������������Ч��
extern void OperateFortifyWindow(INT8U wintype, INT8U currwindow);    //�������ڲ���
extern void CheckTouchFortifyWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void DisplayAlarmWindow(INT8U RefreshType);    //��ʾ��������
extern void PreDealAlarmWindow(INT8U wintype);    //��������Ԥ��������������Ч��
extern void OperateAlarmWindow(INT8U wintype, INT8U currwindow);    //�������ڲ���
extern void CheckTouchAlarmWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void ShowAlarmImage(void);

extern void DisplayCancelAlarmWindow(INT8U RefreshType);    //��ʾȡ����������
extern void PreDealCancelAlarmWindow(INT8U wintype);    //ȡ����������Ԥ��������������Ч��
extern void OperateCancelAlarmWindow(INT8U wintype, INT8U currwindow);    //ȡ���������ڲ���
extern void CheckTouchCancelAlarmWindow(INT16U XLCD, INT16U YLCD); //��鴥��������
extern void CancelAlarmDeal(void); //ȡ���������洦��

extern void CheckDefenceDelay_Func(void); //��鲼����ʱ����
extern void FortifyFinishDeal(void); //������ɽ��洦��
extern void CheckDelayAlarm(void);  //�����ʱ��������
extern void SendAlarmFunc(INT8U SpecialByte, INT8U AlarmByte[2]); //������������
extern void SendCancelAlarmFunc(void); //ȡ����������

extern void InitDefence(void);
extern void CheckDefence(unsigned char DefenceByte);    //����������
#endif