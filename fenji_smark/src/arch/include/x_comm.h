#ifndef x_comm_h
#define x_comm_h
extern void dcomm1(void);
extern void dcomm2(void);
extern void CommSendBuff(INT8U comm_fd, INT8U *comm_buf, INT16U send_len);

extern void KeyTouchOperate(INT8U TouchNum); //������Ӧ
extern void KeyPreDeal(void); //����Ԥ��������Ч��
extern void KeyBackDeal(void); //������������Ч�� ������

extern INT8U CheckKeyPressAndLcd(void); //��鰴��������LCD
#endif