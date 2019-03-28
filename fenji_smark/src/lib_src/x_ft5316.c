#include "includes.h"

#include "stdlib.h"

#define x_ft5316_h

#define CommonH
#include "common.h"

// Data port  //20131220
#define SDA_SET    outportb(GPIOA_OUT_DATA, inportb(GPIOA_OUT_DATA) | 0X08)
#define SDA_CLR    outportb(GPIOA_OUT_DATA, inportb(GPIOA_OUT_DATA) & 0xf7)
#define SDA_READ   inportb(GPIOA_IN_DATA)&(1<<3)
#define SDA_IN     outportb(GPIOA_OEN_DATA, inportb(GPIOA_OEN_DATA) | 0X08)
#define SDA_OUT    outportb(GPIOA_OEN_DATA, inportb(GPIOA_OEN_DATA) & 0XF7)

//SCK port //20131220
#define SCK_SET    outportb(GPIOA_OUT_DATA, inportb(GPIOA_OUT_DATA) | 0X04)
#define SCK_CLR    outportb(GPIOA_OUT_DATA, inportb(GPIOA_OUT_DATA) & 0xfB)
#define SCK_READ   inportb(GPIOA_IN_DATA)&(1<<2)
#define SCK_IN     outportb(GPIOA_OEN_DATA, inportb(GPIOA_OEN_DATA) | 0X04)
#define SCK_OUT    outportb(GPIOA_OEN_DATA, inportb(GPIOA_OEN_DATA) & 0XFB)

char ack;

INT8U I2cWriteLcd( INT8U I2cAddress, INT8U DeviceAddress, INT8U *Data, INT8U 
ByteCount);
INT8U I2cReadLcd( INT8U I2cAddress, INT8U DeviceAddress, INT8U *Data, INT8U 
ByteCount);



//---------------------------------------------------------------------------
void delay_nop(void)
{
	INT8U i,j;
	//for(i=0;i<254;i++)
		//for(j=0;j<254;j++);
	//	delay(1);
	for(i=0;i<1;i++)
		for(j=0;j<1;j++);
	//DelayMs(1);


//	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
//	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
//	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
//	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
//	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();

}

void Start_I2C(void)
{
	SDA_SET;
	SDA_OUT;
	SCK_OUT;
	delay_nop();
	SCK_SET;
	delay_nop();
	SDA_CLR;
	delay_nop();
	delay_nop();
	SCK_CLR;
	delay_nop();
}

void Stop_I2C(void)
{
	SDA_OUT;
	SCK_OUT;

	SDA_CLR;    
	delay_nop();
	SCK_SET;
	delay_nop();
	SDA_SET;
	delay_nop();
}


void mnack(void)
{
	SDA_SET;    
	SDA_OUT;
	delay_nop();
	SCK_SET;
	delay_nop();    
	SCK_CLR;
	delay_nop();
	SDA_CLR;
    delay_nop();  
}

void mack(void)
{
	SDA_CLR;    
	SDA_OUT;
	delay_nop();
	SCK_SET;
	delay_nop();
	SCK_CLR;
	delay_nop();
	SDA_SET;
	delay_nop();    
}

void I2C_1Byte_Write(INT8U wdata)
{
	INT8U i;
    
    ack = 0;	
    
	//SDA_OUT;
	
	for(i=0; i < 8; i++)
	{
		if(wdata&0x80)
			SDA_SET;
		else
			SDA_CLR;
		delay_nop();
		
	 	SCK_SET;
		delay_nop();//slave get the data
		
		wdata <<= 1;
		SCK_CLR;   
		delay_nop();        
	}
    delay_nop();

    SDA_SET;//TO WAIT THE  ACK     
    SDA_IN;
    delay_nop();    

    SCK_SET;
    delay_nop();
	//delay_nop();
    
    for(i=0; i<200; i++)
    {
       if((SDA_READ)==0x10)// THE SDA IS NOT LOW ,NOT GET THE ACK
        {
             ack =0;
        }     
        else 
        {    
    //    send_byte0(0x33);
             ack =1; 
             break;
        }
    }
//    while(1);
    SCK_CLR;
    delay_nop(); 
    SDA_SET;//.....
    SDA_OUT;
    delay_nop();      
}

INT8U I2C_1Byte_Read(void)
{
	INT8U i;
    INT8U rdata;
   	
    rdata = 0;

    SDA_SET;    //TO GET THE DATA 
	SDA_IN;
  	
	for(i=0;	i<8;	i++)
	{
		//delay_nop();        
		SCK_CLR;
		delay_nop();
		SCK_SET;   
		delay_nop();        

        rdata = rdata <<1;    
		if(SDA_READ)	
		{
			rdata = rdata +1;
		}	
	}
    
    SCK_CLR;	
	delay_nop();  
    SDA_OUT;
	delay_nop();      
	
	return rdata;
}



INT8U I2cWriteLcd( INT8U I2cAddress, INT8U DeviceAddress, INT8U *Data, INT8U ByteCount)
{
	INT8U i;
	INT8U *WriteDataPtr;
    
    //readPIN_SET;
    //readPIN_OUT;    

	Start_I2C();
//	I2C_1Byte_Write(I2cAddress << 1);//0 IS WRITE 1 IS READ
	I2C_1Byte_Write(0x70);

    if(ack == 1)
    {
         I2C_1Byte_Write(DeviceAddress);
    
         if(ack == 1)
         {
            	WriteDataPtr = Data;
	            for(i = 0; i < ByteCount; i++)
		        {
		            I2C_1Byte_Write(*WriteDataPtr++);
                    
                    if(ack == 0)
                    {
        	            Stop_I2C();   
                        return 1;                        
                    }
		        }
	            Stop_I2C();   
                return 0;
         }  
         else
         {
                Stop_I2C(); 
                return 1;
         }
         
    }
    else
    {
         //readPIN_CLR;
         //readPIN_IN; 
         
         Stop_I2C(); 
//		 send_byte0(0x33);
         return 1;
    }     
}


INT8U I2cReadLcd( INT8U I2cAddress, INT8U DeviceAddress, INT8U *Data, INT8U ByteCount)
{
	INT8U i;
	INT8U *ReadDataPtr;
    
    Start_I2C();
  //  I2C_1Byte_Write(I2cAddress<<1);
	I2C_1Byte_Write(0x70);
    if(ack == 1)
    {
        I2C_1Byte_Write(DeviceAddress);
        
        if(ack == 1)
        {
            Stop_I2C();           //STEP 2 IS BEGIN     
            Start_I2C(); 
            
  //          I2C_1Byte_Write((I2cAddress << 1) + 1);
            I2C_1Byte_Write(0x71);
                     
            if(ack == 1)
            {
                ReadDataPtr = Data;
                for ( i = 0 ; i < ByteCount-1; i++)
                {
                    *ReadDataPtr++ = I2C_1Byte_Read();
                    mack();
                    //Ack_Check(1);
                }
                *ReadDataPtr = I2C_1Byte_Read();
                mnack();
                Stop_I2C();
                return 0;
            }
            else
            {
                Stop_I2C(); 
            }            
        }
        else
        {
            Stop_I2C(); 
        }
    }
    else
    {
        Stop_I2C();
	//	send_byte0(0x66);
    }
    return 1;
}




