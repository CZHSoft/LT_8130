#ifndef x_comm_h
#define x_comm_h
extern void dcomm1(void);
extern void dcomm2(void);
extern void CommSendBuff(INT8U comm_fd, INT8U *comm_buf, INT16U send_len, INT8U send_dest);

extern void KeyTouchOperate(INT8U TouchNum); //按键响应
extern void KeyPreDeal(void); //按键预处理，按下效果
extern void KeyBackDeal(void); //按键后处理，弹起效果 并处理

extern INT8U CheckKeyPressAndLcd(void); //检查按键并点亮LCD

extern void SendElevatorControl(INT8U *Addr);
extern void SendElevatorControl_Order(INT8U *DestAddr, INT8U *SrcAddr);
#endif