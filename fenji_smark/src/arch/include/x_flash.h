#ifndef x_flash_h
#define x_flash_h

extern void sectererase25x32(INT32U paddr);
extern void blockerase25x32(INT32U paddr);
extern void programpage25x32(INT32U paddr,INT8U *data,INT16U len);
extern void write25x32status(INT8U reg);
extern void w25x32writeenable(void);
extern void w25x32writedisable(void);
extern void erasechip25x32(void);
extern void read25x32id(void);
extern void read25x32_unique_id(INT8U *flash_id);
extern void xtm_read25x32data(INT32U Raddr,INT8U *data_in, INT16U len);
extern void XTM_SPI_DRAM(INT32U src_addr, INT32U dest_addr, INT32U len);

extern void wait_till_ready(void);
extern INT8U read25x32_status(void);
#endif