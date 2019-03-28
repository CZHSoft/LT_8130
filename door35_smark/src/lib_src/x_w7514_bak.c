#include "includes.h"

#if 0
 #define I2C_PORT_OUT 0x2d4
 #define I2C_PORT_OEN 0x2d5
 #define I2C_PORT_IN  0x2d6

 #define I2C_SCL_H 0x40     // GPIOB.6
 #define I2C_SCL_L 0xbf
 #define I2C_SDA_H 0x80     // GPIOB.7
 #define I2C_SDA_L 0x7f
#else
 #define I2C_PORT_OUT 0x2d0
 #define I2C_PORT_OEN 0x2d1
 #define I2C_PORT_IN  0x2d2

 #define I2C_SCL_H 0x04     // GPIOA.2
 #define I2C_SCL_L 0xfb
 #define I2C_SDA_H 0x08     // GPIOA.3
 #define I2C_SDA_L 0xf7
#endif
//#define es8388_WriteDeviceAddress 0x20
//#define es8388_ReadDeviceAddress 0x21

#define I2C_ACKTIMES 0x1f
void Write_i2c(INT8U WriteDeviceAddress, INT8U Wdata);
INT16U Read_i2c(INT8U ReadDeviceAddress);
static void i2c_START(void);
static void i2c_STOP(void);
static void i2c_ACK(void);
static void i2c_NOACK(void);
static void i2c_TESTACK(void);
static void i2c_WRITE8B(INT8U input);
static void i2c_READ8B(void);
//INT8U gpiob;
void readtp(void);
void init_w7514(INT8U WriteDeviceAddress);

void test_i2c(void);
void test_i2c(void)
{
  i2c_START();
}
void readtp(void)
{
	
   INT16U val; 
   Write_i2c(0x90,0xC0);
   val=Read_i2c(0x91);

   xtm_printf("val = 0x%X\n", val);
   //outportb(UART0_TX_DATA,val>>8);
   //outportb(UART0_TX_DATA,val);
	
}

void init_w7514(INT8U WriteDeviceAddress)
{
  i2c_STOP();
  Write_i2c(WriteDeviceAddress, 15);   //向WTC7514DSI(W)写入新的灵敏度级数（15级）
}

INT16U Read_i2c(INT8U ReadDeviceAddress)
{
    INT8U r_byte;
    INT8U tmp_data;
    //tmp_data = inportb(GPIOA_OUT_DATA);
    //xtm_printf("111inportb(GPIOA_OUT_DATA) = 0x%X,\n", inportb(GPIOA_OUT_DATA));
    i2c_START();

    i2c_WRITE8B(ReadDeviceAddress);

    i2c_TESTACK();

    i2c_READ8B();

    i2c_NOACK();

    i2c_STOP();
    //outportb(GPIOA_OUT_DATA, tmp_data);
    //xtm_printf("222inportb(GPIOA_OUT_DATA) = 0x%X,\n", inportb(GPIOA_OUT_DATA));
    return _BX;
}


void Write_i2c(INT8U WriteDeviceAddress,INT8U Wdata)
{
    //xtm_printf("111inportb(GPIOA_OUT_DATA) = 0x%X,\n", inportb(GPIOA_OUT_DATA));
    i2c_START();
    //xtm_printf("222inportb(GPIOA_OUT_DATA) = 0x%X,\n", inportb(GPIOA_OUT_DATA));
    i2c_WRITE8B(WriteDeviceAddress);
    //xtm_printf("333inportb(GPIOA_OUT_DATA) = 0x%X,\n", inportb(GPIOA_OUT_DATA));
    i2c_TESTACK();
    //xtm_printf("444inportb(GPIOA_OUT_DATA) = 0x%X,\n", inportb(GPIOA_OUT_DATA));
    i2c_WRITE8B(Wdata);
    //xtm_printf("555inportb(GPIOA_OUT_DATA) = 0x%X,\n", inportb(GPIOA_OUT_DATA));
    i2c_NOACK();
    //xtm_printf("666inportb(GPIOA_OUT_DATA) = 0x%X,\n", inportb(GPIOA_OUT_DATA));
    i2c_STOP();
    //xtm_printf("777inportb(GPIOA_OUT_DATA) = 0x%X,\n", inportb(GPIOA_OUT_DATA));
}

static void i2c_START(void)
{
  asm{
        mov dx, I2C_PORT_OUT
        in al, dx

        /* SDA = 1 */
        or al, I2C_SDA_H
        out dx, al
        nop
        nop
        nop
        nop
        /* SCL = 1 */
        or al, I2C_SCL_H
        out dx, al
        nop

        nop
        nop
        /* SDA = 0 */
        //and al, I2C_SDA_L
        //out dx, al
        /* SCL = 0 */
        and al, I2C_SCL_L
        out dx, al
        nop
        nop

        nop
    }
}

static void i2c_STOP(void)
{
    asm{
        mov dx, I2C_PORT_OUT
        in al, dx

        and al, I2C_SCL_L
        out dx, al
        nop
        nop
        nop
        nop
        and al, I2C_SDA_L
        out dx, al
        nop
        nop
        nop
        nop
        or  al, I2C_SCL_H
        out dx, al
        nop

        nop
        or  al, I2C_SDA_H
        out dx, al
        nop

        nop
    }
}



static void i2c_NOACK(void)
{
    asm{

    	mov dx, I2C_PORT_OEN
        in al, dx
        and al, I2C_SDA_L//7fh
        out dx, al
        nop
        nop
        nop
        nop

        mov dx, I2C_PORT_OUT
        in al, dx

        and al, I2C_SCL_L
        out dx, al
        nop
        nop
        nop
        nop
                
        or  al, I2C_SDA_H
        out dx, al
        nop
        nop
        nop
        nop
        
        or  al, I2C_SCL_H
        out dx, al
        nop
        nop
        nop
        nop

        and al, I2C_SCL_L
        out dx, al
        nop
        nop
        nop
        nop
    }
}

static void i2c_TESTACK(void)
{
    asm{
        mov dx, I2C_PORT_OUT
        in al, dx

        and al, I2C_SDA_L
        out dx, al
        and al, I2C_SCL_L
        out dx, al
        nop
   
        nop
        or  al, I2C_SCL_H
        out dx, al
        nop

        nop
        mov dx, I2C_PORT_OEN
        //20120406 xu
        in al, dx
        or al, I2C_SDA_H //80h

        out dx, al
        mov cx, I2C_ACKTIMES
    }
  waitack:
    asm{
    	  cmp cx, 0
    	  je  noack
    	  dec cx
        mov dx, I2C_PORT_IN
        in  al, dx
        test al, I2C_SDA_H
        jnz waitack
      }
    noack:
    	asm{
        push ax
        mov dx, I2C_PORT_OEN
        //20120406 xu
        in al, dx
        and al, I2C_SDA_L //3fh

        out dx, al
        pop ax

        //dec dx
        mov dx, I2C_PORT_OUT
        in al, dx
        
        and al, I2C_SCL_L
        out dx, al
        nop
    }
}

static void i2c_WRITE8B(INT8U input)
{
    asm {
        mov bl, input
        mov cx, 8
        mov dx, I2C_PORT_OUT      //I2C_PORT_OUT
        in al, dx
    }
  write8:
    asm{
        and al, I2C_SDA_L
        test bl, 80h
        jz  pdsda0
        or al, I2C_SDA_H
    }
  pdsda0:
    asm{
        out dx, al
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        or al, I2C_SCL_H
        out dx, al
        nop
        nop
  
        nop
        nop
        nop
        and al, I2C_SCL_L
        out dx, al
        nop
 
        nop
        shl bl, 1
        loop write8
    }
}
static void i2c_READ8B(void)
{
    asm{
        mov cx, 8
        mov dx,I2C_PORT_OEN
        //20120406 xu
        in al, dx
        or al, I2C_SDA_H//80h

        out dx,al
        mov bh, 0

        mov dx, I2C_PORT_OUT
        in al, dx
        }
  read28:
    asm{
        or al, I2C_SCL_H
        out dx, al

        shl bh, 1
        inc dx
        inc dx
        in  al, dx
        dec dx
        dec dx

        test al, I2C_SDA_H

        in al, dx

        jz read281
        or bh, 1
    }
  read281:
    asm{

        and al, I2C_SCL_L
        out dx, al


        loop read28

    }
#if 1
  asm{

        out dx, al

        mov dx, I2C_PORT_OEN
        //20120406 xu
        in al, dx
        and al, I2C_SDA_L//7fh

        out dx, al

        and al, I2C_SDA_L

        //dec dx
        mov dx, I2C_PORT_OUT
        in al, dx
        
        out dx, al
        or  al, I2C_SCL_H
        out dx, al
        and al, I2C_SCL_L
        out dx, al
      }
#endif    
   asm{
        mov cx, 8
        out dx, al

        mov dx,I2C_PORT_OEN
        //20120406 xu
        in al, dx
        or al, I2C_SDA_H//80h

        out dx,al
        //mov al,gpiob
        //mov bl, 0
        //dec dx
        mov dx, I2C_PORT_OUT
        in al, dx
        }
  read8:
    asm{
        or al, I2C_SCL_H

        out dx, al

        nop

        nop
        nop
        shl bl, 1
        inc dx
        inc dx
        in  al, dx
        dec dx
        dec dx

        test al, I2C_SDA_H

        in al, dx

        jz read81
        or bl, 1
    }
  read81:
    asm{

        and al, I2C_SCL_L
        out dx, al
        nop

        nop
        nop

        loop read8

    }
 // return _BX;
}

