#ifndef x_timer_h
#define x_timer_h

extern void timer_thread_func(void);

extern void SendCapturePicData(INT16U CurrPack);
extern void SendCapturePicFinish(INT8U CurrNo, INT8U Result);
#endif