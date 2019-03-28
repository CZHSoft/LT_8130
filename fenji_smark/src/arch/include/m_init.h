#ifndef m_init_h
#define m_init_h

#define outb(v,a) outportb(a, v)
#define outw(v,a) outport(a, v)
#define inb(a)    inportb(a)


#define sets  136

#define ATTREG  0x2C0
#define MOPREG  0x2C2
#define MBSREG  0x2C3
#define SEQREG  0x2C4
#define VPMREG  0x2C6
#define PMWREG  0x2C8
#define PXDREG  0x2C9
#define GRAREG  0x2CE
#define CRTREG  0x2D4
#define RESREG  0x2DA
#define ADSREG  0x46E8  
#define LNTBSEG 0xAFE0


//--------------define printer port register---------
#define PRT_DATA 0x248  // printer data port
#define PRT_STS  0x249  // printer status port
#define PRT_CTL  0x24a  // printer control port


#define BEEP_PORT 0x244
#define EDOTYPE 0

#define CLOSESCREEN   0
#define OPENSCREEN    1
#define OPENTEXTSCR   2
#define OPENGRASCR    3
#define CLOSETEXTSCR  4
#define CLOSEGRASCR   5

//字库 Flash Address 0x40000~~0xf8fff
#define HZK24S_START   0x40000UL  //        //24点阵
#define HZK24T_START   0xBA000UL  //        //全角字符
#define EN24START      0xD3000UL        //24点阵　ascii 大
#define HZK16_START    0x6B0000UL        //16点阵
#define EN16START      0xD5000UL        //16点阵　ascii

#define Y1START   0x400000UL
#define Y2START   0x4BC000UL

extern void DelayMs(unsigned int number);
extern void main_init(void);
extern void fmem_fmem_dma(unsigned long fmems,unsigned long fmemd,int count);

extern INT16U dtime[8];

extern INT8U Vflag;

extern void fmem_insb(unsigned long fmem,unsigned char *buffer,int count);
extern void fmem_insb_dma(unsigned long fmem, unsigned char *buffer, int count);

extern INT32U SWTIMER_Tick(void);
extern void *xtm_memcopy(void *dest, void *src, INT16U n);
#endif