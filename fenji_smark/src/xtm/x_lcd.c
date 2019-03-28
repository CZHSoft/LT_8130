#include "includes.h"

#include "stdlib.h"

#include "x_printf.h"
#include "x_disp.h"
#include "x_main.h"

/*
****************************************
20131119
�����޸�
****************************************
*/

void Lcd_Power(INT8U uFlag, INT8U level);    //20110608 xu
void Disp_Enable(uchar uFlag);    //20120305 xu

void 	disp_init(void);
INT8U CloseDispClockFlag = 0;
extern void interrupt VInt(void);
//---------------------------------------------------------------------------
void Lcd_Power(INT8U uFlag, INT8U level)    //20110608 xu    1 -- ������   0 -- �ر���
{
  INT32U luminance;
  INT8U e_level[6]; //= {2, 4, 6, 8, 9, 10};
  e_level[0] = 2;
  e_level[1] = 4;
  e_level[2] = 6;
  e_level[3] = 8;
  e_level[4] = 9;
  e_level[5] = 10;
  
  luminance = 0;
  xtm_printf("Lcd_Power::uFlag = %d, level = %d\n", uFlag, level);

   if (uFlag)
    {
     Disp_Enable(1);    //����ʾ
    }
   else
    {
     Disp_Enable(0);    //����ʾ
    }

  /*if (uFlag)*/{
    luminance = CPU_MAIN_FREQ/25000;///100;//25000;///100;//200;   //20120222 �ľ���
  }
  // HRC >= LRC  ������     HRC < LRC  ����
   outportb(PTC0_HRC0,((10 - e_level[level])*luminance/10));     //PWM0
   outportb(PTC0_HRC1,((10 - e_level[level])*luminance/10)>>8);
   outportb(PTC0_HRC2,((10 - e_level[level])*luminance/10)>>16);

   outportb(PTC0_LRC0,(luminance));     //PWM0
   outportb(PTC0_LRC1,(luminance)>>8);
   outportb(PTC0_LRC2,(luminance)>>16);
   //20120307 �ӡ�0x01 ��Ϊ��0x00
   if (uFlag)
    {
     outportb(PTC0_CTRL0,0x01); //������
    }
   else
    {
     outportb(PTC0_CTRL0,0x00); //�ر���
    }
}
//---------------------------------------------------------------------------
void Disp_Enable(uchar uFlag)    //20120305 xu
{
  if (uFlag)
   {
    CloseDispClockFlag = 0;
    Set_Disp_Clock(1); //������ʾʱ��  1  ��   0  ��
    outportb(CONTROL_REG, inportb(CONTROL_REG)|0x01);  //0 0000 001  ��ʾʹ��
    
    setvect(18, VInt);
    outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x20);//ʹ��V�ж�
   }
  else
   {
    outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xdf);//��ֹV�ж�
    //CloseDispClockFlag = 1;
    //outportb(CONTROL_REG, inportb(CONTROL_REG)&0xfe);  //1 1111 110  ��ʾ��ֹ    0xfe
    //Set_Disp_Clock(0); //������ʾʱ��  1  ��   0  ��
   }
}
//---------------------------------------------------------------------------
void 	disp_init(void)
{
  int i;
  outportb(SCALE_SEL, 0x00);

  //1 0000 101
  outportb(CONTROL_REG, inportb(CONTROL_REG) | 0x85);//enable display
    
  clear_disp_ram();

  if((SCREEN_WIDTH == 800)&&(SCREEN_HEIGHT == 480))
   {
    xtm_printf("800x480 LCD_DE_MODE\n");
    outport(HTOTAL_L, 912);//912);//900);//1000);//900);
    outport(HDISP_L, 800);  //800
    outport(HSYNC_START_L, 808);//816); //810

    outportb(HSYNC_SIZE, 8);//16);//10);
    outportb(HBACKPORCH, 8);//16);  // 16

    outport(VTOTAL_L, 528);//544);  //500
    outport(VDISP, 480);     //480
    outport(VSYNC_START, 496); //486

    outportb(VSYNC_SIZE, 8);//16);//2);
    outportb(VBACKPORCH, 8);//16);  //5

    outportb(DISPLAY_MODE, 0xd);  // 0xd
   }

  if((SCREEN_WIDTH == 480)&&(SCREEN_HEIGHT == 272))
   {
    if(LCD_DE_MODE)
     {
      xtm_printf("480x272 LCD_DE_MODE\n");
      outport(HTOTAL_L, 550);              //480x272     
      outport(HDISP_L, 480);    
	    outport(HSYNC_START_L, 500);  
	    outportb(HSYNC_SIZE, 10);   
	    outportb(HBACKPORCH, 16);   
	    outport(VTOTAL_L, 350);              //480x272     
	    outport(VDISP, 272);     
	    outport(VSYNC_START, 276);    
	    outportb(VSYNC_SIZE, 2);    
	    outportb(VBACKPORCH, 5);    
	    outportb(DISPLAY_MODE, 0x0e);      // DE ģʽ   1 110  4��Ƶ
	    //      outportb(DISPLAY_MODE, 0x0f);        // DE ģʽ   1 111  8��Ƶ   320x240    
	   }   
	  else    
	   {	  	
	      xtm_printf("480x272 LCD_DE_MODE111 SYNC\n");     
	      outport(HTOTAL_L, 528);              //480x272     
	      outport(HDISP_L, 480);                                                                             
	      outport(HSYNC_START_L, 488);     
	      outportb(HSYNC_SIZE, 10);     
	      outportb(HBACKPORCH, 16);    
	      outport(VTOTAL_L, 288);              //480x272     
	      outport(VDISP, 272);     
	      outport(VSYNC_START, 276);    
	      outportb(VSYNC_SIZE, 2);     
	      outportb(VBACKPORCH, 4);    
	      outportb(DISPLAY_MODE, 0xc6);      // SYNC ģʽ    
	    }
  }

  if((SCREEN_WIDTH == 320)&&(SCREEN_HEIGHT == 240))
   {
    if(LCD_DE_MODE)
     {
      xtm_printf("320x240 LCD_DE_MODE\n");
      outport(HTOTAL_L, 400);     
      outport(HDISP_L, 320);     
      outport(HSYNC_START_L, 352);    
      outportb(HSYNC_SIZE, 1);     
      outportb(HBACKPORCH, 16);    
      outport(VTOTAL_L, 288);                  
      outport(VDISP, 240);     
      outport(VSYNC_START, 246);    
      outportb(VSYNC_SIZE, 1);     
      outportb(VBACKPORCH, 4);
      //      outportb(DISPLAY_MODE, 0x0e);      
      // DE ģʽ   1 110  4��Ƶ     
      outportb(DISPLAY_MODE, 0x4f);        
      // DE ģʽ   1 111  8��Ƶ   320x240            ���� 1 Ϊ��    
     }   
    else    
     {       
     	 xtm_printf("320x240 LCD_DE_MODE111\n");       
     	 outport(HTOTAL_L, 416);       
     	 outport(HDISP_L, 320);       
     	 outport(HSYNC_START_L, 352);       
     	 outportb(HSYNC_SIZE, 26);       
     	 outportb(HBACKPORCH, 20);      
     	 outport(VTOTAL_L, 288);      
     	 outport(VDISP, 240);      
     	 outport(VSYNC_START, 248);       
     	 outportb(VSYNC_SIZE, 3);       
     	 outportb(VBACKPORCH, 15);      
     	 outportb(DISPLAY_MODE, 0xc7);      // SYNC ģʽ    1100 0111    
      }
  }

    xtm_printf("disp_init::1 CONTROL_REG2 = 0x%X\n", inportb(CONTROL_REG2));
    outportb(CONTROL_REG2, inportb(CONTROL_REG2) & 0xfd);    //��Ƶ������ƼĴ���2
    //outportb(CONTROL_REG2, inportb(CONTROL_REG2) | 0x02);      //����ʾ�� ��һ���ִ����������, ��������������� BitBlk ����
    xtm_printf("disp_init::2 CONTROL_REG2 = 0x%X\n", inportb(CONTROL_REG2));
}
//---------------------------------------------------------------------------