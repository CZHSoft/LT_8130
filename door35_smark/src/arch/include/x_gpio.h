#ifndef x_gpio_h
#define x_gpio_h
extern void InitGpio(void);  //��ʼ��GPIO
extern void CheckGpio(void);    //���GPIO����

extern void SetGpio(INT16U IO_Register, INT8U userIO, INT8U SetFlag);  //����GPIO

extern void ClearWatchDog(void);

extern void Check_Night_IO(void);
#endif