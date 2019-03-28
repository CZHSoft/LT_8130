#ifndef x_info_h
#define x_info_h

extern void DisplayInfoWindow(INT8U RefreshType, INT8U Curr_Sec);    //��ʾ��Ϣ����
extern void PreDealInfoWindow(INT8U wintype);    //��Ϣ����Ԥ��������������Ч��
extern void OperateInfoWindow(INT8U wintype, INT8U currwindow);    //��Ϣ���ڲ���
extern void CheckTouchInfoWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern void DisplayInfoContentWindow(INT8U infotype, INT8U infono);    //��ʾ�鿴��Ϣ����
extern void PreDealInfoContentWindow(INT8U wintype);    //�鿴��Ϣ����Ԥ��������������Ч��
extern void OperateInfoContentWindow(INT8U wintype, INT8U currwindow);    //�鿴��Ϣ���ڲ���
extern void CheckTouchInfoContentWindow(INT16U XLCD, INT16U YLCD); //��鴥��������

extern INT8U Init_InfoVar(void); //��ʼ�� Info �ṹ
extern INT8U Uninit_InfoVar(void); //�ͷ� Info �ṹ
//����Ϣ�ļ�
extern void ReadInfoFile(void);
//д��Ϣ�ļ�
extern void WriteInfoFile(void);

extern void ResetInfoCfg(void);    //��λ��Ϣ�����ļ�
extern void WriteInfoCfgFile(void);     //д��Ϣ�����ļ�

extern void Recv_NewInfo(void);
#endif