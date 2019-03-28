#include "includes.h"

#define es8388_PORT 0x2d0
#define es8388_WriteDeviceAddress 0x20
#define es8388_ReadDeviceAddress 0x21
#define es8388_SCL_H 0x40     /* GPIOA.6 */
#define es8388_SCL_L 0xbf
#define es8388_SDA_H 0x80     /* GPIOA.7 */
#define es8388_SDA_L 0x7f
#define es8388_ACKTIMES 0x2f
void Writees8388(INT8U RomAddress,INT8U Wdata);
INT8U Reades8388(INT8U RomAddress);
static void es8388_START(void);
static void es8388_STOP(void);
static void es8388_ACK(void);
static void es8388_NOACK(void);
static void es8388_TESTACK(void);
static void es8388_WRITE8B(INT8U input);
void init_es8388(void);

INT8U SpeakerVolume[6] = {0x00, 0x06, 0x12, 0x18, 0x1B, 0x1E};//0x1E;//0x12;//0x0D;
INT8U MicVolume[6] = {0x00, 0x20, 0x40, 0x50, 0x65, 0x80};//0x00;//0x80;   //0x40

void SoundSetSpeakerVolume(INT8U volume);
void SoundSetMicVolume(INT8U volume);

void SoundSetSpeakerDac(INT8U volume);
void SoundSetMicAdc(INT8U volume);

INT8U Reades8388(INT8U RomAddress)
{	
    /* Start() */
    asm{
        mov dx, es8388_PORT
        mov al, 0c0h
        or al, es8388_SDA_H
        out dx, al
        or al, es8388_SCL_H
        out dx, al
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
        nop
        nop
        and al, es8388_SDA_L
        out dx, al
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
        nop
        nop
        and al, es8388_SCL_L
        out dx, al
    }
    /* Write8Bit(WriteDeviceAddress); */
    asm {
        mov bl, es8388_WriteDeviceAddress
        mov cx, 8
    }
  write8_0:
    asm{
        and al, es8388_SDA_L
        test bl, 80h
        jz  pdsda00
        or al, es8388_SDA_H
    }
  pdsda00:
    asm{
    	  out dx, al
        or al, es8388_SCL_H
        out dx, al
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
        nop
        nop        
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
        shl bl, 1
        loop write8_0
    }
    /* TestAck(); */
    asm{
    	  or  al, es8388_SDA_H
        //and al, es8388_SDA_L
        out dx, al
 
        or  al, es8388_SCL_H
        out dx, al
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
        nop
        nop         
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop        
        mov  cx,es8388_ACKTIMES

    }
#if 1
  waitack0:
    asm{
    	  cmp cx, 0
    	  je  noack0
    	  dec cx
        mov dx, 2d1h
    //  #if 1       //20120406 xu
        in al, dx
        or al, 80h
   //   #else
   //     mov al, 80h
   //   #endif
        out dx, al           //input
        inc dx
        in  al, dx
        test al, es8388_SDA_H
        jnz waitack0
      }
      noack0:
      	asm{
      		push ax
        	mov dx, 2d1h
    //  #if 1       //20120406 xu
        in al, dx
        and al, 07fh
   //   #else
   //       mov al, 0
   //   #endif
          out dx, al             //output
          pop ax
          and al, es8388_SCL_L
          mov dx, es8388_PORT
          out dx, al
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
        nop
        nop    
        }
#endif
    /* Write8Bit(RomAddress); */
    asm {
        mov bl, RomAddress
        mov cx, 8
    }
  write8_1:
    asm{
        and al, es8388_SDA_L
        test bl, 80h
        jz  pdsda01
        or al, es8388_SDA_H
    }
  pdsda01:
    asm{
        out dx, al
        or al, es8388_SCL_H
        out dx, al
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
        nop
        nop
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
        shl bl, 1
        loop write8_1
    }
    /* TestAck(); */
   
    asm{
    	  or  al, es8388_SDA_H
        //and al, es8388_SDA_L
        out dx, al
        or  al, es8388_SCL_H
        out dx, al
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
        nop
        nop

        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
        mov cx, es8388_ACKTIMES
    }
#if 1 
  waitack1:
    asm{
        cmp cx, 0
        je  noack1
        dec cx
        mov dx, 2d1h
    //  #if 1       //20120406 xu
        in al, dx
        or al, 80h
   //   #else
   //     mov al, 80h
   //   #endif
        out dx, al
        inc dx
        in  al, dx
        test al, es8388_SDA_H
        jnz waitack1
      }
      noack1:
      	asm{
        push ax
        mov dx, 2d1h
    //  #if 1       //20120406 xu
        in al, dx
        and al, 07fh
   //   #else
   //     mov  al,0
   //   #endif
        out dx, al
        dec dx
        pop ax
        
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
    }
#endif
    /* Start(); */
    asm{
        /* SDA = 1 */
        or al, es8388_SDA_H
        out dx, al
        /* SCL = 1 */
        or al, es8388_SCL_H
        out dx, al
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
        nop
        nop
        /* SDA = 0 */
        and al, es8388_SDA_L
        out dx, al
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
        nop
        nop
        /* SCL = 0 */
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
    }

    /* Write8Bit(ReadDviceAddress); */
    asm {
        mov bl, es8388_ReadDeviceAddress
        mov cx, 8
    }
  write8_2:
    asm{
        and al, es8388_SDA_L
        test bl, 80h
        jz  pdsda02
        or al, es8388_SDA_H
    }
  pdsda02:
    asm{
        out dx, al
        or al, es8388_SCL_H
        out dx, al
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
        nop
        nop
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
        shl bl, 1
        loop write8_2
    }
    /* TestAck(); */
    asm{
    	  or  al, es8388_SDA_H
        //and al, es8388_SDA_L
        out dx, al
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
        nop
        nop
        or  al, es8388_SCL_H
        out dx, al
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
        nop
        nop;
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
        nop
    }
#if 1
    asm{
        mov dx, 2d1h
    //  #if 1       //20120406 xu
        in al, dx
        or al, 80h
   //   #else
   //     mov al, 80h
   //   #endif
        out dx, al
        inc dx
        mov cx, es8388_ACKTIMES
      }

  waitack3:
    asm{
      
      //  mov dx, 2d1h
      //  mov al, 8h
     //   out dx, al
     //   inc dx
        cmp cx,0
        je  noack3
        dec cx
        in  al, dx
        test al, es8388_SDA_H
        jnz waitack3
      }
      noack3:
      asm{
        push ax
        mov dx, 2d1h
    //  #if 1       //20120406 xu
        in al, dx
        and al, 07fh
   //   #else
   //     mov al, 0
   //   #endif
        out dx, al
        dec dx
        pop ax
        
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
    }
#endif
    asm{
        mov cx, 8
        mov bh, al
        mov dx,2d1h
    //  #if 1       //20120406 xu
        in al, dx
        or al, 80h
   //   #else
   //     mov al,80h
   //   #endif
        out dx,al
        mov al,bh
        mov bl, 0
        dec dx    
        }
  read8:
    asm{
        or al, es8388_SCL_H
        mov bh, al
        out dx, al

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
        nop
        nop
        shl bl, 1
        inc dx
        inc dx
        in  al, dx
        dec dx
        dec dx
          
        test al, es8388_SDA_H
        mov al, bh
        jz read81
        or bl, 1
    }
  read81:
    asm{
     
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
        
        loop read8
    
    }
#if 1  
    /* NoAck(); */
    asm{

    	  mov dx, 2d1h
    //  #if 1       //20120406 xu
        in al, dx
        and al, 07fh
   //   #else
   //   mov al, 0
   //   #endif
    	  out dx, al
    	  mov al,bh
    	  dec dx
        or  al, es8388_SDA_H
        out dx, al
        or  al, es8388_SCL_H
        out dx, al
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
        nop
        nop
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
    }
#endif
    /* Stop(); */
    asm{
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
        and al, es8388_SDA_L
        out dx, al
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
        nop
        nop
        or  al, es8388_SCL_H
        out dx, al
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
        nop
        nop
        or  al, es8388_SDA_H
        out dx, al
    
    }
    return _BL;
} 


void Writees8388(INT8U RomAddress,INT8U Wdata)
{
    es8388_START();
    es8388_WRITE8B(es8388_WriteDeviceAddress);
    es8388_TESTACK();
    es8388_WRITE8B(RomAddress);
    es8388_TESTACK();
    es8388_WRITE8B(Wdata);
    es8388_NOACK();
    es8388_STOP();
}

static void es8388_START(void)
{
  #if 0
    es8388_SDA_1();		/* SDA = 1 */
    es8388_SCL_1();          	/* SCL = 1 */
    es8388_SDA_0();		/* SDA = 0 */
    es8388_SCL_0();          	/* SCL = 0 */
  #else
  asm{
        mov dx, es8388_PORT
        in al, dx
        /* SDA = 1 */
        or al, es8388_SDA_H
        out dx, al
        /* SCL = 1 */
        or al, es8388_SCL_H
        out dx, al
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
        nop
        nop
        /* SDA = 0 */
        and al, es8388_SDA_L
        out dx, al
        /* SCL = 0 */
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
    }
  #endif
}

static void es8388_STOP(void)
{
  #if 0
    es8388_SCL_0();		/* SCL = 0 */
    es8388_SDA_0();		/* SDA = 0 */
    es8388_SCL_1();		/* SCL = 1 */
    es8388_SDA_1();		/* SDA = 1 */
  #else
    asm{
        mov dx, es8388_PORT
      //  mov al, es8388_DAT
        in al, dx
        and al, es8388_SCL_L
        out dx, al
        and al, es8388_SDA_L
        out dx, al
        or  al, es8388_SCL_H
        out dx, al
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
        nop
        nop
        or  al, es8388_SDA_H
        out dx, al
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
        nop
        nop
    }
  #endif
}



static void es8388_NOACK(void)
{
  #if 0
    Mp3_SDA_1(); 
    Mp3_SCL_1();
    Mp3_SCL_0();
  #else
    asm{
        mov dx, es8388_PORT
       // mov al, es8388_DAT
        in al, dx
        or  al, es8388_SDA_H
        out dx, al
        or  al, es8388_SCL_H
        out dx, al
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
    }
  #endif
}

static void es8388_TESTACK(void)
{
  #if 0
    Mp3_SDA_1();
    Mp3_SCL_0();
    Mp3_SCL_1();
    while(Mp3_SDA());
    Mp3_SCL_0();
  #else
    asm{
        mov dx, es8388_PORT
        in al, dx
     //   or  al, es8388_SDA_H
        and al, es8388_SDA_L
        out dx, al
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
        or  al, es8388_SCL_H
        out dx, al
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
        nop
        nop
        nop
        mov dx, 2d1h
    //  #if 1       //20120406 xu
        in al, dx
        or al, 80h
   //   #else
   //     mov al, 80h
   //   #endif
        out dx, al
        mov cx, es8388_ACKTIMES
    }
  waitack:
    asm{
    	  cmp cx, 0
    	  je  noack
    	  dec cx
        mov dx, 2d2h
        in  al, dx
        test al, es8388_SDA_H
        jnz waitack
      }
    noack:
    	asm{
        push ax
        mov dx, 2d1h
    //  #if 1       //20120406 xu
        in al, dx
        and al, 7fh  
    //  #else
    //    mov al, 0
    //  #endif
        out dx, al
        pop ax
        dec dx
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
    }
  #endif
}

static void es8388_WRITE8B(INT8U input)
{
  

    asm {
        mov bl, input
        mov cx, 8
        mov dx, es8388_PORT      //es8388_PORT
   //     mov al, es8388_DAT
        in al, dx
    }
  write8:
    asm{
        and al, es8388_SDA_L
        test bl, 80h   //0x80
        jz  pdsda0
        or al, es8388_SDA_H
    }
  pdsda0:
    asm{
        out dx, al
        or al, es8388_SCL_H
        out dx, al
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
        nop
        nop
        and al, es8388_SCL_L
        out dx, al
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
        nop
        nop
        shl bl, 1
        loop write8
    }
}


void init_es8388(void)
{
#if 1       //now   
  Writees8388(0x00, 0x06);
  Writees8388(0x01, 0x72);//chip control
  Writees8388(0x2B, 0x80);
  Writees8388(0x08, 0x00); //slave mode
  Writees8388(0x03, 0x00);//ADC power on
  Writees8388(0x04, 0x3c);  //DAC power on
  //ADC control
  Writees8388(0x09, 0x00);
  //Writees8388(0x0a, 0x00);
  Writees8388(0x0a, 0xf0);
  Writees8388(0x0b, 0x02);   //差分
  Writees8388(0x0c, 0x6F); // dsp/pcm mode,16BIT,请根据实际的格式进行修改
  Writees8388(0x0d, 0x04); // MCLK / ALRCK  2:256 16k   4:512 8k
  Writees8388(0x10, 0x00); // ADC Left Volume=0db
  Writees8388(0x11, 0x00);
  Writees8388(0x12, 0x02);  //alc setting
  Writees8388(0x13, 0xc0);
  Writees8388(0x14, 0x12);
  Writees8388(0x15, 0x36);   //0x06
  Writees8388(0x16, 0x03);  //0xb1   0xb3
  // DAC control
  Writees8388(0x17, 0x5E); // dsp/pcm mode,16BIT,请根据实际的格式进行修改
  Writees8388(0x18, 0x04); // MCLK/DLRCK RTIO 2:256 16k  4:512 8k
  Writees8388(0x19, 0x02);
  Writees8388(0x1a, 0x00);
  Writees8388(0x1b, 0x00);
  Writees8388(0x27, 0xB8);
  Writees8388(0x2A, 0xB8);
  Writees8388(0x02, 0x00);
  Writees8388(0x2E, 0x00); // LOUT VOLUME

Writees8388(0x1a, 0x00);
Writees8388(0x1b, 0x00);

Writees8388(0x1c, 0x00);
Writees8388(0x19, 0x08);     //固定0x20 可能要调整   e0

//Writees8388(0x0f, 0x10);   
Writees8388(0x1d, 0x40);   //不静音

/*  Writees8388(0x09, 0x70);     // 0-0x80    只调节高4位   左声道
  Writees8388(0x12, 0xda);//0xda);//0xe2);    // Reg 0x12 = 0xe2 (ALC enable, PGA Max. Gain=23.5dB, Min. Gain=0dB)
  Writees8388( 0x13, 0x70);//0xa0  0xa2 // Reg 0x13 = 0xc0 (ALC Target=-4.5dB, ALC Hold time =0 mS)
  Writees8388( 0x14, 0x12);   // Reg 0x14 = 0x12(Decay time =820uS , Attack time = 416 uS)
  Writees8388( 0x15, 0x06);   // Reg 0x15 = 0x06(ALC mode)
  Writees8388( 0x16, 0x73); // 0xc3// Reg 0x16 = 0xc3(nose gate = -40.5dB, NGG = 0x01(mute ADC))   有可能需要调整

  Writees8388( 0x00, 0x00);      //20121106
  Writees8388( 0x06, 0xc3);     */

//  Writees8388(0x1d, 0x00);   //不静音
/*  Writees8388(0x1E, 0x03);
  Writees8388(0x1F, 0x84);
  Writees8388(0x20, 0xED);
  Writees8388(0x21, 0xBE);
  Writees8388(0x22, 0x01);
  Writees8388(0x23, 0xF7);
  Writees8388(0x24, 0xFD);
  Writees8388(0x25, 0xFF);  */
  /*Writees8388(0x1E,0x03);       //滤波参数
  Writees8388(0x1F,0x84);
  Writees8388(0x20,0xED);
  Writees8388(0x21,0xBE);
  Writees8388(0x22,0x01);
  Writees8388(0x23,0xF7);
  Writees8388(0x24,0xFD);
  Writees8388(0x25,0xFF);   */

//  Writees8388(0x1d, 0x80);   //静音
#endif



}
void SoundSetSpeakerVolume(INT8U volume)
{
  //000000 -- -30db
  //000001 -- -29db
  //000010 -- -28db
  //...
  //011110 -- 0db
  //011111 -- 1db
  //...
  //100001 -- 3db
  if(volume < 6)
    Writees8388(0x2E, SpeakerVolume[volume]);  // LOUT VOLUME     0-0x0d   只调节低6位  左声道

  xtm_printf("SoundSetSpeakerVolume::0x2E = 0x%X \n", SpeakerVolume[volume]);
}
void SoundSetMicVolume(INT8U volume)
{
/*  //0000 -- 0db
  //0001 -- 3db
  //0010 -- 6db
  //0011 -- 9db
  //0100 -- 12db
  //0101 -- 15db
  //0110 -- 18db
  //0111 -- 21db
  //1000 -- 24db
  Writees8388(0x09, volume);     // 0-0x80    只调节高4位   左声道*/

  //0000 0000 -- 0db
  //0000 0001 -- -0.5db
  //......
  //11000 000 -- -96db
  if(volume < 6)
    Writees8388(0x09, MicVolume[volume]);     // 0-0x80    只调节高4位   左声道

  xtm_printf("SoundSetMicVolume::0x09 = 0x%X \n", MicVolume[volume]);
}

void SoundSetSpeakerDac(INT8U volume)
{
  //0000 0000 -- 0db
  //0000 0001 -- -0.5db
  //......
  //1100 0000 -- -96db
  Writees8388(0x1A, volume);     // 0-0xC0    只调节高4位   左声道
  //Writees8388(0x1b, 0x00);

  //xtm_printf("SoundSetSpeakerDac::0x1A = 0x%X \n", volume);
}

void SoundSetMicAdc(INT8U volume)
{
  //0000 0000 -- 0db
  //0000 0001 -- -0.5db
  //......
  //11000 000 -- -96db
  Writees8388(0x10, volume);     // 0-0x80    只调节高4位   左声道

  //xtm_printf("SoundSetMicAdc::0x10 = 0x%X \n", volume);
}

INT8U test_es8328e(void)
{
  INT8U i, val, ReadOK;
  INT8U reg[26] = {0x00, 0x01, 0x2B, 0x08, 0x03,  0x04, 0x09, 0x0a, 0x0c, 0x0d, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
           0x27, 0x2A, 0x02, 0x2E};
  INT8U value[26] = {0x06, 0x72, 0x80, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x6F, 0x04, 0x00, 0x00, 0x02, 0xc0, 0x12, 0x06, 0xb3, 0x5E, 0x04, 0x02, 0x00, 0x00,
           0xb8, 0xb8, 0x00, 0x00};

  ReadOK = 1;
  for(i=0; i<26; i++)
   if(ReadOK)
    {
     //Writees8388(reg[i], value[i]);
     val = Reades8388(reg[i]);
     xtm_printf("reg = 0x%X, val = 0x%X, value[i] = 0x%X\n", reg[i], val, value[i]);
     //if((i == 5)||(i == 6))
    //   continue;
     if(val != value[i])
       xtm_printf("not ok\n");
    //   ReadOK = 0;
    }

  xtm_printf("ReadOK = %d\n", ReadOK);
  return ReadOK;
}

