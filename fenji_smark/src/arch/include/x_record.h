#ifndef x_record_h
#define x_record_h

extern void DisplayRecordsWindow(INT8U RefreshType, INT8U Curr_Sec);    //��ʾͨ����¼����
extern void PreDealRecordsWindow(INT8U wintype);    //ͨ����¼����Ԥ������������Ч��
extern void OperateRecordsWindow(INT8U wintype, INT8U currwindow);    //ͨ����¼���ڲ���
extern void CheckTouchRecordsWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void DisplayRecordsContentWindow(INT8U recordsno);    //ͨ����¼�鿴��Ϣ����
extern void PreDealRecordsContentWindow(INT8U wintype);    //�鿴ͨ����¼����Ԥ������������Ч��
extern void OperateRecordsContentWindow(INT8U wintype, INT8U currwindow);    //�鿴ͨ����¼���ڲ���
extern void CheckTouchRecordsContentWindow(INT16U XLCD, INT16U YLCD); //��鴥��������


extern INT8U Init_RecordsVar(void); //��ʼ�� Records �ṹ
extern INT8U Uninit_RecordsVar(void); //�ͷ� Records �ṹ
//��ͨ����¼�ļ�
extern void ReadRecordsFile(void);
//дͨ����¼�ļ�
extern void WriteRecordsFile(void);

extern void ResetRecordsCfg(void);    //��λͨ����¼�����ļ�
extern void WriteRecordsCfgFile(void);     //дͨ����¼�����ļ�

extern void Recv_NewRecords(void);
//д��Ƭ����
extern void WritePicFunc(void);
#endif