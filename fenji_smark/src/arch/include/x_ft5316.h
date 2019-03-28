#ifndef x_ft5316_h
#define x_ft5316_h

extern INT8U I2cWriteLcd( INT8U I2cAddress, INT8U DeviceAddress, INT8U *Data, INT8U 
ByteCount);
extern INT8U I2cReadLcd( INT8U I2cAddress, INT8U DeviceAddress, INT8U *Data, INT8U 
ByteCount);

#endif