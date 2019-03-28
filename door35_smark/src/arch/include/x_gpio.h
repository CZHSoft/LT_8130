#ifndef x_gpio_h
#define x_gpio_h
extern void InitGpio(void);  //初始化GPIO
extern void CheckGpio(void);    //检测GPIO函数

extern void SetGpio(INT16U IO_Register, INT8U userIO, INT8U SetFlag);  //设置GPIO

extern void ClearWatchDog(void);

extern void Check_Night_IO(void);
#endif