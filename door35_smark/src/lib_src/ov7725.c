#include "includes.h"

//#define CommonH
//#include "common.h"
#include "x_printf.h"

#if 1
 #define OV7725_PORT 0x2d0
 #define OV7725_PORT_OEN 0x2d1
 
 #define OV7725_SCL_H 0x04     // GPIOA.2
 #define OV7725_SCL_L 0xfb
 #define OV7725_SDA_H 0x08     // GPIOA.3
 #define OV7725_SDA_L 0xf7

/* #define OV7725_SCL_H 0x08     // GPIOA.3
 #define OV7725_SCL_L 0xf7
 #define OV7725_SDA_H 0x04     // GPIOA.2
 #define OV7725_SDA_L 0xfb   */
#else
 #define OV7725_PORT 0x2d4
 #define OV7725_PORT_OEN 0x2d5

 #define OV7725_SCL_H 0x40     // GPIOB.6
 #define OV7725_SCL_L 0xbf
 #define OV7725_SDA_H 0x80     // GPIOB.7
 #define OV7725_SDA_L 0x7f
#endif

INT8U WriteDeviceAddress,ReadDeviceAddress;

void init7725(INT8U FrameRate);
void Write7725(INT8U RomAddress,INT8U Wdata);
INT8U Read7725(INT8U RomAddress);
unsigned int init_camera(INT8U FrameRate);
INT8U test7725(void);

//#define _MULTI_WRITE_7725
#define _MULTI_NOP
unsigned int init_camera(INT8U FrameRate)
{
   INT8U val;
    WriteDeviceAddress=0x42;
    ReadDeviceAddress=0x43;

    nop();
    nop();
     if((Read7725(0x0a)==0x77)&&(Read7725(0x0b)==0x21)){
        init7725(FrameRate);
        return(0x7725);
     }
     else{
//        init7113();
//        init7725(FrameRate);
        return(0x7113);
     }
	
}

INT8U Read7725(INT8U RomAddress)
{

    //Local.GpioWatchDogStarted = 0;  //在GPIO检测线程中启动WatchDog
    /* Start() */
    asm{
        mov dx, OV7725_PORT
        in al, dx
        or al, OV7725_SDA_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        and al, OV7725_SDA_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
    }
    /* Write8Bit(WriteDeviceAddress); */
    asm {
        mov bl, WriteDeviceAddress
        mov cx, 8
    }
  write8_0:
    asm{
        and al, OV7725_SDA_L
        test bl, 80h //0x80
        jz  pdsda002
        or al, OV7725_SDA_H
    }
  pdsda002:
    asm{
    	  out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
    	  nop
    	  nop
    	  nop
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
        nop
        nop
        and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        shl bl, 1
        loop write8_0


    }
    /* TestAck(); */
    asm{
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
        nop
        nop
        nop
        nop
         and  al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    }

    /* Write8Bit(RomAddress); */
    asm {
        mov bl, RomAddress
        mov cx, 8
    }
  write8_41:
    asm{
        and al, OV7725_SDA_L
        test bl, 80h //0x80
        jz  pdsda012
        or al, OV7725_SDA_H
    }
  pdsda012:
    asm{
        out dx, al
        nop
        nop
        nop
        nop
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
        nop
        nop
        and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        shl bl, 1
        loop write8_41


    }
    /* TestAck(); */
    asm{
    	  nop
    	  nop
    	  nop
     	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
        nop
        nop
        nop
        nop
        and  al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
        nop
    }
    /* Stop(); */
    asm{
        and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        and al, OV7725_SDA_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        or  al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
        nop
        nop
        or  al, OV7725_SDA_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
        nop
        nop
        nop

    }
    /* Start(); */
    asm{
        /* SDA = 1 */
        or al, OV7725_SDA_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        /* SCL = 1 */
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        /* SDA = 0 */
        and al, OV7725_SDA_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        /* SCL = 0 */
        and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
    }
    /* Write8Bit(ReadDviceAddress); */
    asm {
        mov bl, ReadDeviceAddress
        mov cx, 8
    }
  write8_21:
    asm{
        and al, OV7725_SDA_L
        test bl, 80h //0x80
        jz  pdsda022
        or al, OV7725_SDA_H
    }
  pdsda022:
    asm{
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
        nop
        nop
        nop
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
        nop
        nop
        and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
        nop
        nop
        shl bl, 1
        loop write8_21

    }

    /* TestAck(); */
    asm{
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
        nop
        nop
        nop
        nop
        and  al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
        nop
    }
    asm{
        mov cx, 8
        mov bh, al

        mov dx,OV7725_PORT_OEN
      //#if 0       //20120406 xu
        in al, dx
        or al, OV7725_SDA_H //80h //8
      //#else
      //  mov al,80h
      //#endif
        out dx,al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        mov al,bh
        mov bl, 0
        dec dx
    }
  read8:
    asm{
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        mov bh, al
        nop
        nop
        nop
        shl bl, 1
        inc dx
        inc dx
        in  al, dx
        dec dx
        dec dx

        test al, OV7725_SDA_H
        mov al,bh
        jz read81
        or bl, 1
    }
  read81:
    asm{

        and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
        nop
        nop
        loop read8

    }
    /* NoAck(); */
    asm{
          mov dx, OV7725_PORT_OEN
      //#if 0       //20120406 xu
        in al, dx
        and al, OV7725_SDA_L //7fh //0f7h
      //#else
      //  mov al, 0
      //#endif
    	  out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
    	  mov al, bh
    	  dec dx
        or  al, OV7725_SDA_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
        or  al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
        nop
        nop
        nop
        nop
         and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    	  nop
    }
    /* Stop(); */
    asm{
        and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        and al, OV7725_SDA_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        or  al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        or  al, OV7725_SDA_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
    }
    //Local.GpioWatchDogStarted = 1;  //在GPIO检测线程中启动WatchDog
    return _BL;
}


void Write7725(INT8U RomAddress,INT8U Wdata)
{
    //Local.GpioWatchDogStarted = 0;  //在GPIO检测线程中启动WatchDog
    /* Start() */

    asm{
        mov dx, OV7725_PORT
        in al, dx
        or al, OV7725_SDA_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        and al, OV7725_SDA_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
    }
    /* Write8Bit(WriteDeviceAddress); */
    asm {
        mov bl, WriteDeviceAddress
        mov cx, 8
    }
  write8_10:
    asm{
        and al, OV7725_SDA_L
        test bl, 0x80
        jz  pdsda001
        or al, OV7725_SDA_H
    }
  pdsda001:
    asm{
    	  out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        shl bl, 1
        loop write8_10
      //  or al, OV7725_SDA_H
      // out dx, al


    }
    /* TestAck(); */
    asm{
        and  al, OV7725_SDA_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        and  al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
      //  nop
    }

    /* Write8Bit(RomAddress); */
    asm {
        mov bl, RomAddress
        mov cx, 8
    }
  write8_31:
    asm{
        and al, OV7725_SDA_L
        test bl, 0x80
        jz  pdsda014
        or al, OV7725_SDA_H
    }
  pdsda014:
    asm{
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        shl bl, 1
        loop write8_31
     //   or al, OV7725_SDA_H
     //   out dx, al


    }
    /* TestAck(); */
    asm{
        and  al, OV7725_SDA_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        and  al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
    }

    asm {
        mov bl, Wdata
        mov cx, 8
    }
  write83:
    asm{
        and al, OV7725_SDA_L
        test bl, 0x80
        jz  pdsda20
        or al, OV7725_SDA_H
    }
  pdsda20:
    asm{
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        shl bl, 1
        loop write83
       // or al, OV7725_SDA_H
       // out dx, al

    }
    /* TestAck(); */
    asm{
        and  al, OV7725_SDA_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        or al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        and  al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        nop
    }

    /* Stop(); */
    asm{
        and al, OV7725_SCL_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        and al, OV7725_SDA_L
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        or  al, OV7725_SCL_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
        or  al, OV7725_SDA_H
        out dx, al
      #ifdef _MULTI_NOP
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
      #endif
    }
   //Local.GpioWatchDogStarted = 1;  //在GPIO检测线程中启动WatchDog 
}


void init7725(INT8U FrameRate)
{
  INT8U val;
#if 0
  INT8U val;
    WriteDeviceAddress=0x42;
    ReadDeviceAddress=0x43;

    Write7725(0x12,0x80);
//    xtm_printf("0x12 = 0x%X\n", Read7725(0x12));
    DelayMs(20);
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0x11) != 0x01)
   #endif
      Write7725(0x11,0x01);
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0x12) != 0x20)
   #endif
      Write7725(0x12,0x20);
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0x0c) != 0x00)
   #endif
      Write7725(0x0c,0x00);
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0x69) != 0x5c)
   #endif
      Write7725(0x69,0x5c);
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0x13) != 0x8b)
   #endif
      Write7725(0x13,0x8b);
 //    Write7725(0x13,0xab);
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0xa6) != 0x06)
   #endif
      Write7725(0xa6,0x06);   //d3
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0x9b) != 0x18)
   #endif
      Write7725(0x9b,0x18);   //brightness +3
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0xab) != 0x06)
   #endif
      Write7725(0xab,0x06);   //
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0x9c) != 0x40)
   #endif
      Write7725(0x9c,0x40);   //Contrast
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0x60) != 0x80)
   #endif
      Write7725(0x60,0x80);
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0x61) != 0x80)
   #endif
      Write7725(0x61,0x80);
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0xa7) != 0x60)
   #endif
      Write7725(0xa7,0x60);   //  U
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0xa8) != 0x60)
   #endif
      Write7725(0xa8,0x60);   //  V
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0xa9) != 0x80)
   #endif
      Write7725(0xa9,0x80);   // coshue
   #ifdef _MULTI_WRITE_7725
    while(Read7725(0xaa) != 0x80)
   #endif
      Write7725(0xaa,0x80);   // sinhue


#else
INT8U i;
    Write7725(0x12,0x80);
//    xtm_printf("0x12 = 0x%X\n", Read7725(0x12));
    DelayMs(20);
Write7725(0x00, 0x02);
Write7725(0x01, 0x7e);
Write7725(0x02, 0x65);
Write7725(0x03, 0x40);
Write7725(0x04, 0x00);
Write7725(0x05, 0x79);
Write7725(0x06, 0x78);
Write7725(0x07, 0x78);
Write7725(0x08, 0x01);
Write7725(0x09, 0x00);
Write7725(0x0a, 0x77);
Write7725(0x0b, 0x21);
Write7725(0x0c, 0xc0);

switch(FrameRate)
 {
  case 0:
  case 1:
         Write7725(0x0d, 0x41);       //xu 20120418
         break;
  case 2:
         Write7725(0x0d, 0x81);     ///////////////////////////////
         break;
 }
Write7725(0x0e, 0x79);
Write7725(0x0f, 0xa5);
Write7725(0x10, 0x7d);

switch(FrameRate)
 {
  case 0:
  case 1:
         Write7725(0x11, 0x01);     //xu 20120418
         break;
  case 2:
         Write7725(0x11, 0x03);   ///////////////////////////////
         break;
 }

Write7725(0x12, 0x20);
Write7725(0x13, 0xff);
Write7725(0x14, 0x10);
Write7725(0x15, 0x01);
Write7725(0x16, 0x00);
Write7725(0x17, 0x23);//0x26);   555  20121231
Write7725(0x18, 0xa0);
Write7725(0x19, 0x07);
Write7725(0x1a, 0xf0);
Write7725(0x1b, 0x40);
Write7725(0x1c, 0x7f);
Write7725(0x1d, 0xa2);
Write7725(0x1e, 0x00);
Write7725(0x1f, 0x00);
Write7725(0x20, 0x10);
Write7725(0x21, 0x00);
//Write7725(0x22, 0x7f);
//Write7725(0x23, 0x07);
switch(FrameRate)      // 0x22    0xaa  -- 30   0x99  --  25      0x73  -- 20
 {                                      // 0x23    0x02  -- 30   0x03  --- 25      0x04  -- 20
  case 0:
         Write7725(0x22, 0xaa);  //xu 20120418
         Write7725(0x23, 0x02);  //xu 20120418
         break;
  case 1:
         Write7725(0x22, 0x99);  //xu 20120418
         Write7725(0x23, 0x03);  //xu 20120418
         break;
  case 2:
         Write7725(0x22, 0x73);  //xu 20120418
         Write7725(0x23, 0x04);  //xu 20120418
         break;
 }

Write7725(0x24, 0x40);
Write7725(0x25, 0x30);
Write7725(0x26, 0xa1);
Write7725(0x27, 0x00);
Write7725(0x28, 0x00);
Write7725(0x29, 0xa0);
Write7725(0x2a, 0x02);
Write7725(0x2b, 0x00);
Write7725(0x2c, 0xf0);
Write7725(0x2d, 0x00);
Write7725(0x2e, 0x00);
Write7725(0x2f, 0x37);
Write7725(0x30, 0x80);
Write7725(0x31, 0x60);
Write7725(0x32, 0x00);//0x02);  555  20121231
//Write7725(0x33, 0x00);
switch(FrameRate)      // 0x00 -- 30    0x66  -- 25    0x40  -- 20
 {
  case 0:
         Write7725(0x33, 0x00);  //xu 20120418       0x66  -- 25    0x00  30
         break;
  case 1:
         Write7725(0x33, 0x66);  //xu 20120418       0x66  -- 25    0x00  30
         break;
  case 2:
         Write7725(0x33, 0x40);                             ///////////////////////////////
         break;
 }

Write7725(0x34, 0x00);
Write7725(0x35, 0x7c);
Write7725(0x36, 0x7e);
Write7725(0x37, 0x7f);
Write7725(0x38, 0x7f);
Write7725(0x39, 0x94);
Write7725(0x3a, 0xa4);
Write7725(0x3b, 0xa8);
Write7725(0x3c, 0xaa);
Write7725(0x3d, 0x03);
Write7725(0x3e, 0xe2);
Write7725(0x3f, 0x1f);
Write7725(0x40, 0xe8);
Write7725(0x41, 0x00);
Write7725(0x42, 0x7f);
Write7725(0x42, 0x80);
Write7725(0x44, 0x80);
Write7725(0x45, 0x80);
Write7725(0x46, 0x00);
Write7725(0x47, 0x00);
Write7725(0x48, 0x00);
Write7725(0x49, 0x50);
Write7725(0x4a, 0x30);
Write7725(0x4b, 0x50);
Write7725(0x4c, 0x50);
Write7725(0x4d, 0x00);
Write7725(0x4e, 0xef);
Write7725(0x4f, 0x10);
Write7725(0x50, 0x60);
Write7725(0x51, 0x00);
Write7725(0x52, 0x00);
Write7725(0x53, 0x24);
Write7725(0x54, 0x7a);
Write7725(0x55, 0xfc);
Write7725(0x56, 0xfc);
Write7725(0x57, 0xfc);
Write7725(0x58, 0xfc);
Write7725(0x59, 0xfc);
Write7725(0x5a, 0xfc);
Write7725(0x5b, 0xfc);
Write7725(0x5c, 0xfc);
Write7725(0x5d, 0xfc);
Write7725(0x5e, 0xfc);
Write7725(0x5f, 0xfc);
Write7725(0x60, 0x00);
Write7725(0x61, 0x05);
Write7725(0x62, 0xff);
Write7725(0x63, 0xe0);
Write7725(0x64, 0xff);
Write7725(0x65, 0x20);
Write7725(0x66, 0x00);
Write7725(0x67, 0x00);
Write7725(0x68, 0x00);
Write7725(0x69, 0x5c);
Write7725(0x6a, 0x11);
Write7725(0x6b, 0xaa);
Write7725(0x6c, 0x01);
Write7725(0x6d, 0x50);
Write7725(0x6e, 0x80);
Write7725(0x6f, 0x80);
Write7725(0x70, 0x0f);
Write7725(0x71, 0x00);
Write7725(0x72, 0x00);
Write7725(0x73, 0x0f);
Write7725(0x74, 0x0f);
Write7725(0x75, 0xff);
Write7725(0x76, 0x00);
Write7725(0x77, 0x10);
Write7725(0x78, 0x10);
Write7725(0x79, 0x70);
Write7725(0x7a, 0x70);
Write7725(0x7b, 0xf0);
Write7725(0x7c, 0xf0);
Write7725(0x7d, 0xf0);
Write7725(0x7e, 0x11);
Write7725(0x7f, 0x21);
Write7725(0x80, 0x3f);
Write7725(0x81, 0x65);
Write7725(0x82, 0x74);
Write7725(0x83, 0x81);
Write7725(0x84, 0x8f);
Write7725(0x85, 0x9b);
Write7725(0x86, 0xa5);
Write7725(0x87, 0xab);
Write7725(0x88, 0xb6);
Write7725(0x89, 0xbf);
Write7725(0x8a, 0xd0);
Write7725(0x8b, 0xda);
Write7725(0x8c, 0xec);
Write7725(0x8d, 0x1a);
Write7725(0x8e, 0x03);
Write7725(0x8f, 0x04);
Write7725(0x90, 0x05);
Write7725(0x91, 0x01);
Write7725(0x92, 0x05);
Write7725(0x93, 0x00);
Write7725(0x94, 0xd4);
Write7725(0x95, 0xed);
Write7725(0x96, 0x17);
Write7725(0x97, 0x24);
Write7725(0x98, 0x76);
Write7725(0x99, 0x98);
Write7725(0x9a, 0x1a);
Write7725(0x9b, 0x08);
Write7725(0x9c, 0x20);
Write7725(0x9d, 0x00);
Write7725(0x9e, 0x81);
Write7725(0x9f, 0xfa);

Write7725(0xa0, 0x02);
Write7725(0xa1, 0x50);
Write7725(0xa2, 0x40);
Write7725(0xa3, 0x06);
Write7725(0xa4, 0x00);
Write7725(0xa5, 0x00);
Write7725(0xa6, 0x04);
Write7725(0xa7, 0x40);
Write7725(0xa8, 0x40);
Write7725(0xa9, 0x80);
Write7725(0xaa, 0x00);

Write7725(0xab, 0x06);
Write7725(0xac, 0xff);
for(i = 0xad; i <= 0xf3; i++)
  Write7725(i, 0x65);
#endif

}

INT8U test7725(void)
{
  INT8U i, val, ReadOK;
  INT8U reg[16] = {0x00, 0x01, 0x02, 0x03, 0x04,  0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13};
  INT8U value[16] = {0x02, 0x7e, 0x65, 0x40, 0x00, 0x00, 0x77, 0x21, 0xc0, 0x41, 0x79, 0xa5, 0x7d, 0x01, 0x20, 0xff};
  ReadOK = 1;
  for(i=0; i<16; i++)
   if(ReadOK)
    {
     Write7725(reg[i], value[i]);
     val = Read7725(reg[i]);
     xtm_printf("reg = 0x%X, val = 0x%X, value[i] = 0x%X\n", reg[i], val, value[i]);
     //if((i == 5)||(i == 6))
    //   continue;
     if(val != value[i])
       ReadOK = 0;
    }

  xtm_printf("ReadOK = %d\n", ReadOK);
  return ReadOK;
}

