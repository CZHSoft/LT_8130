#ifndef x_ov7725_h
#define x_ov7725_h
extern void init7725(INT8U FrameRate);
extern void Write7725(INT8U RomAddress,INT8U Wdata);
extern INT8U Read7725(INT8U RomAddress);
extern unsigned int init_camera(INT8U FrameRate);
extern INT8U test7725(void);
#endif