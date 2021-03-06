#ifndef x_net_h
#define x_net_h
extern void 	mac_init(void);
extern void clear_mac_ram(void);

extern void 	phy_init(void);
extern INT16U phy_reg_read(INT16U phy_addr, INT16U reg_no);
extern void phy_reg_write(INT16U phy_addr, INT16U reg_no, INT16U val);

extern void interrupt MACInt(void);

extern void mac_int_deal(void);
extern void mac_recv(INT8U r_pos);
#endif