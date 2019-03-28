#ifndef x_zt2083_h
#define x_zt2083_h

extern void Write_i2c(INT8U WriteDeviceAddress, INT8U Wdata);
extern INT16U Read_i2c(INT8U ReadDeviceAddress);

extern void readtp(void);

extern void test_i2c(void);
#endif