#include "includes.h"

#include "stdlib.h"

#include "x_printf.h"
#include "x_disp.h"
#include "x_main.h"

void Lcd_Power(INT8U uFlag, INT8U level);    //20110608 xu
void Disp_Enable(uchar uFlag);    //20120305 xu

void 	disp_init(void);
//---------------------------------------------------------------------------
void Lcd_Power(INT8U uFlag, INT8U level)    //20110608 xu    1 -- ������   0 -- �ر���
{
  INT32U luminance;
  INT8U e_level[6]; //= {2, 4, 6, 8, 9, 10};
  e_level[0] = 1;
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
    Set_Disp_Clock(1); //������ʾʱ��  1  ��   0  ��
    outportb(CONTROL_REG, inportb(CONTROL_REG)|0x01);  //0 0000 001  ��ʾʹ��
   }
  else
   {
    outportb(CONTROL_REG, inportb(CONTROL_REG)&0xfe);  //1 1111 110  ��ʾ��ֹ    0xfe
    Set_Disp_Clock(0); //������ʾʱ��  1  ��   0  ��
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
    outport(HTOTAL_L, 900);//1000);//900);
    outport(HDISP_L, 800);
    outport(HSYNC_START_L, 810);


    outportb(HSYNC_SIZE, 10);//1);//10);
    outportb(HBACKPORCH, 16);


    outport(VTOTAL_L, 500);
    outport(VDISP, 480);
    outport(VSYNC_START, 486);

    outportb(VSYNC_SIZE, 2);//1);//2);
    outportb(VBACKPORCH, 5);

    outportb(DISPLAY_MODE, 0xd);
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
     	 xtm_printf("320x240 LCD_SYNC_MODE\n");
     	 outport(HTOTAL_L, 408);
     	 outport(HDISP_L, 320);       
     	 outport(HSYNC_START_L, 346);
     	 outportb(HSYNC_SIZE, 10);
     	 outportb(HBACKPORCH, 38);
     	 outport(VTOTAL_L, 258);
     	 outport(VDISP, 240);
     	 outport(VSYNC_START, 240);
     	 outportb(VSYNC_SIZE, 3);//3);
     	 outportb(VBACKPORCH, 12);//12);      
     	 outportb(DISPLAY_MODE, 0xc7);      // SYNC ģʽ    1100 0111    
      }
  }

    xtm_printf("disp_init::1 CONTROL_REG2 = 0x%X\n", inportb(CONTROL_REG2));
    outportb(CONTROL_REG2, inportb(CONTROL_REG2) & 0xfd);    //��Ƶ������ƼĴ���2
    //outportb(CONTROL_REG2, inportb(CONTROL_REG2) | 0x02);      //����ʾ�� ��һ���ִ��������, �������������� BitBlk ����
    xtm_printf("disp_init::2 CONTROL_REG2 = 0x%X\n", inportb(CONTROL_REG2));
}
//---------------------------------------------------------------------------
