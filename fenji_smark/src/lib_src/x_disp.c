#include "includes.h"

#include "stdlib.h"

#define x_disp_h

//#define CommonH
//#include "common.h"
#include "x_disp.h"

#include "x_printf.h"
#include "x_flash.h"
#include "x_main.h"
#include "x_first.h"
#include "x_gpio.h"
#include "main.h"
#include "m_init.h"

/*
****************************************
20131119
暂无修改
****************************************
*/


INT8U dtemp[512];
void clear_disp_ram(void);
void xtm_clear_disp_ram(INT16U color);
void clear_area(INT16U x,INT16U y,INT16U Hi,INT16U Wi,INT16U color_val,INT8U ym);

void selectym(INT8U ye);
void gra_dispcn16(INT8U *ch, int x, int y,INT16U color_val,INT16U bcolor_val, INT8U FullPageType);
void gra_dispcn24(INT8U *ch, int x, int y,INT16U color_val,INT16U bcolor_val, INT8U FullPageType);
void display_text(INT16U x, INT16U y, INT8U wd, INT16U clr, char *s, INT8U pass, INT8U fonttype, INT8U FullPageType); //显示点阵汉字

//预处理 Image 文件
INT8U PreDealImage(struct TImage *t_image);
INT8U DisplayImage(struct TImage *t_image, INT8U FromType, INT8U FullPageType);

//预处理 ImageButton 文件
INT8U PreDealImageButton(struct TImageButton *t_button, INT8U buttontype);
INT8U DisplayImageButton(struct TImageButton *t_button, INT8U buttontype, INT8U FromType, INT8U FullPageType);
//显示图形按键文本
void DisplayImageButton_Text(struct TImageButton *t_button, INT16U color, INT8U wd, INT8U FullPageType);

//预处理 Edit 文件
INT8U PreDealEdit(struct TEdit *t_edit, INT8U buttontype);
INT8U DisplayEdit(struct TEdit *t_edit, INT8U buttontype, INT8U FromType, INT8U FullPageType);
void DisplayEdit_Text(struct TEdit *t_edit, INT16U color, INT8U wd, INT8U FullPageType);
void DisplayPassEdit_Text(struct TEdit *t_edit, INT16U color, INT8U wd, INT8U FullPageType);

//INT8U DeocdeFinishFlag = 0;
void WaitDecodeFinish(void);

INT32U *Local_fb_addr;
INT8U Local_fb_page;
INT8U *disp_jpeg_bitstream;
//#define _PRINTINFO
//---------------------------------------------------------------------------
void clear_disp_ram()
{
#if 1
	INT16U i,j;
	INT16U color_val, bcolor_val;
	INT32U addr;

	addr=Y1START;
	outportb(FONT_WR_MOST_ADD, addr>>16);

	outportb(FONT_WR_HIGH_ADD, addr>>8);

	outportb(FONT_WR_LOW_ADD, addr);

	color_val = COLOR_BLUE;    //COLOR_WHITE
	bcolor_val = COLOR_BLUE;   //COLOR_WHITE
	outportb(FGND_LOW, color_val);
	outportb(FGND_HIGH, (color_val>>8));
	outportb(BGND_LOW, bcolor_val);
	outportb(BGND_HIGH, (bcolor_val>>8));
	for(i=0;i<100;i++){
		 for(j=0;j<480;j++){

		 	outportb(FONT_WR_DATA, 0xff);
		}


	}
#endif
}

void xtm_clear_disp_ram(INT16U color)
{
	INT16U i,j;
	INT16U color_val, bcolor_val;
	INT32U addr;

	addr=Y1START;
	outportb(FONT_WR_MOST_ADD, addr>>16);
	outportb(FONT_WR_HIGH_ADD, addr>>8);
	outportb(FONT_WR_LOW_ADD, addr);
	
	color_val = color;    //COLOR_WHITE
	bcolor_val = color;   //COLOR_WHITE
	outportb(FGND_LOW, color_val);
	outportb(FGND_HIGH, (color_val>>8));
	outportb(BGND_LOW, bcolor_val);
	outportb(BGND_HIGH, (bcolor_val>>8));
	for(i=0;i<100;i++){
		 for(j=0;j<480;j++){

		 	outportb(FONT_WR_DATA, 0xff);
		}
	}
}
//---------------------------------------------------------------------------
void clear_area(INT16U x,INT16U y,INT16U Hi,INT16U Wi,INT16U color_val,INT8U ym)
{
	INT16U i,j;
//	INT16U color_val, bcolor_val;
	INT32U addr,saddr;

	outportb(FGND_LOW, color_val);
	outportb(FGND_HIGH, (color_val>>8));
	saddr=Y1START;
	if (ym)
		saddr=Y2START;
//	outportb(BGND_LOW, bcolor_val);
//	outportb(BGND_HIGH, (bcolor_val>>8));
	
	for(i=0;i<Hi;i++){
		addr = (((INT32U) y*SCREEN_WIDTH +(INT32U) x)<<1) + saddr;
		outportb(FONT_WR_MOST_ADD, addr>>16);
		outportb(FONT_WR_HIGH_ADD, addr>>8);
		outportb(FONT_WR_LOW_ADD, addr);
		for(j=0;j<Wi;j++){

		 	outportb(FONT_WR_DATA, 0xff);
		}
		y++;
		
	}
}
//---------------------------------------------------------------------------
void selectym(INT8U ye)
{
	INT32U addr;
	addr=Y1START;
    	if (ye)
    		addr=Y2START;
	outportb(DM_START_M, (addr>>16));
	outportb(DM_START_H, (addr>>8));
	outportb(DM_START_L, addr);

}
//---------------------------------------------------------------------------
void gra_dispen(INT16U x, INT16U y, char ch, INT8U FullPageType)
{
	INT16U j, i;
	INT16U color_val, bcolor_val;
	INT32U addr;

        Local_fb_addr = GetLocal_fb_addr();
        Local_fb_page = GetLocal_fb_page();

	if(ch<' '||ch>0x7f)
		return EOF;

	j = ((ch-32)<<4);
        if(FullPageType)
          addr = (((INT32U) y*DISP_WIDTH +(INT32U) x)<<1) + Local_fb_addr[!Local_fb_page];     //显示到后台页
        else
          addr = (((INT32U) y*DISP_WIDTH +(INT32U) x)<<1) + Local_fb_addr[Local_fb_page];      //显示到当前页

	color_val = COLOR_GREEN;
	bcolor_val = COLOR_BLUE;
	outportb(FGND_LOW, color_val);
	outportb(FGND_HIGH, (color_val>>8));
	outportb(BGND_LOW, bcolor_val);
	outportb(BGND_HIGH, (bcolor_val>>8));


	for(i=0;i<16;i++){
		outportb(FONT_WR_MOST_ADD, (addr>>16));
		outportb(FONT_WR_HIGH_ADD, (addr>>8));
		outportb(FONT_WR_LOW_ADD, addr);
		//outportb(FONT_WR_DATA, nAsciiDot[j]);

		addr += (DISP_WIDTH<<1);
		j++;
	}
}
//---------------------------------------------------------------------------
void gra_dispcn16(INT8U *ch, int x, int y,INT16U color_val,INT16U bcolor_val, INT8U FullPageType)
{

	INT8U qu, wei;
//	INT16U color_val,bcolor_val;
	INT16U i,k,m;
	INT8U *fa, *fa1;
  INT8U *ascp,*hanzip;
	int cc,j,n;
	UINT hanzishu;
	INT32U addr,addr2,addr1;
        Local_fb_addr = GetLocal_fb_addr();
        Local_fb_page = GetLocal_fb_page();        
        if(FullPageType)
          addr = (((INT32U) y*DISP_WIDTH +(INT32U) x)<<1) + Local_fb_addr[!Local_fb_page];     //显示到后台页
        else
          addr = (((INT32U) y*DISP_WIDTH +(INT32U) x)<<1) + Local_fb_addr[Local_fb_page];      //显示到当前页

//  color_val = COLOR_RED;
//	bcolor_val = COLOR_BLUE;
	outportb(FGND_LOW, color_val);
	outportb(FGND_HIGH, (color_val>>8));
	outportb(BGND_LOW, bcolor_val);
	outportb(BGND_HIGH, (bcolor_val>>8));


	while(*ch){
		if(*ch>0x80){
    	qu=*ch++;
    	wei=*ch;
           #if 0  //old
			if(qu<0xaa){
    		addr1=(qu-161)*94+(wei-161);
			}else{
    		addr1=(qu-160-16+9)*94+(wei-161);
			}
           #else
                addr1=(qu-161)*94+(wei-161);
           #endif
    	addr1<<=5;
    	addr1=HZK16_START+addr1;
                      xtm_read25x32data(addr1, dtemp, 32);

	  	addr2=addr;
	  	for(i=0;i<16;i++){
		   outportb(FONT_WR_MOST_ADD, (addr>>16));
		   outportb(FONT_WR_HIGH_ADD, (addr>>8));
		   outportb(FONT_WR_LOW_ADD, addr);
		   outportb(FONT_WR_DATA, dtemp[i*2]);

		   outportb(FONT_WR_DATA, dtemp[i*2+1]);

		   addr += (DISP_WIDTH<<1);

	  	}
	  	addr=addr2+32;
	  	ch++;

		}else{
			qu=*ch++;
			addr1 = qu<<4;
    	addr1=EN16START+addr1;
                        xtm_read25x32data(addr1, dtemp, 16);
	  	addr2=addr;
			addr2=addr;
			for(i=0;i<16;i++){
				outportb(FONT_WR_MOST_ADD, (addr>>16));
				outportb(FONT_WR_HIGH_ADD, (addr>>8));
				outportb(FONT_WR_LOW_ADD, addr);
                                outportb(FONT_WR_DATA, dtemp[i]);
		
				addr += (DISP_WIDTH<<1);
//			j++;
		}
	  addr=addr2+16;	
	}
 }

}
//---------------------------------------------------------------------------
void gra_dispcn24(INT8U *ch, int x, int y,INT16U color_val,INT16U bcolor_val, INT8U FullPageType)
{

	INT8U qu, wei;
	INT16U i;
	INT32U addr,addr2,addr1;    //
        Local_fb_addr = GetLocal_fb_addr();
        Local_fb_page = GetLocal_fb_page();
        if(FullPageType)
          addr = (((INT32U) y*DISP_WIDTH +(INT32U) x)<<1) + Local_fb_addr[!Local_fb_page];     //显示到后台页
        else
          addr = (((INT32U) y*DISP_WIDTH +(INT32U) x)<<1) + Local_fb_addr[Local_fb_page];      //显示到当前页


	outportb(FGND_LOW, color_val);
	outportb(FGND_HIGH, (color_val>>8));
	outportb(BGND_LOW, bcolor_val);
	outportb(BGND_HIGH, (bcolor_val>>8));

	while(*ch){
		if(*ch>0x80){
          	          qu=*ch++;
    	                  wei=*ch;
                          if(qu <= 0xa3)    //制表符及全角字符
                           {
                            addr1=(qu-161)*94+(wei-161);
    	                    addr1*=72;
    	                    addr1+=HZK24T_START;
                           }
                          else
                           {
                            addr1=(qu-161-15)*94+(wei-161);    //24点阵字库中没有制表符等,故需减一偏移量
    	                    addr1*=72;
    	                    addr1+=HZK24S_START;
                           }

                          xtm_read25x32data(addr1, dtemp, 72);

	  	addr2=addr;
	  	for(i=0;i<24;i++){
		   outportb(FONT_WR_MOST_ADD, (addr>>16));
		   outportb(FONT_WR_HIGH_ADD, (addr>>8));
		   outportb(FONT_WR_LOW_ADD, addr);
	   	   outportb(FONT_WR_DATA, dtemp[i*3]);
		   outportb(FONT_WR_DATA, dtemp[i*3+1]);		
		   outportb(FONT_WR_DATA, dtemp[i*3+2]);

		   addr += (DISP_WIDTH<<1);

	  	}
	  	addr=addr2+48;
	  	ch++;
	  
		}else{
			qu=*ch++;
			if (qu>0x19){
				qu-=0x20;
				addr1 = qu*48;
    		addr1+=EN24START;

                        xtm_read25x32data(addr1, dtemp, 48);
	  		addr2=addr;
				for(i=0;i<24;i++){
					outportb(FONT_WR_MOST_ADD, (addr>>16));
					outportb(FONT_WR_HIGH_ADD, (addr>>8));
					outportb(FONT_WR_LOW_ADD, addr);
		  		outportb(FONT_WR_DATA, dtemp[i*2]);
		  		outportb(FONT_WR_DATA, dtemp[i*2+1]);
					addr += (DISP_WIDTH<<1);
			}
	  	addr=addr2+32;
	  }else	{
	  	y+=32;
  		addr = (((INT32U) y*DISP_WIDTH +(INT32U) x)<<1) ;
                if(FullPageType)
                  addr += Local_fb_addr[!Local_fb_page];     //显示到后台页
                else
                  addr += Local_fb_addr[Local_fb_page];      //显示到当前页
 		}
	}
 }

}
//---------------------------------------------------------------------------
void display_text(INT16U x, INT16U y, INT8U wd, INT16U clr, char *s, INT8U pass, INT8U fonttype, INT8U FullPageType) //显示点阵汉字
{
  if(fonttype == 24)
    gra_dispcn24(s, x, y, clr, 0, FullPageType);//COLOR_BLUE);
  else
    gra_dispcn16(s, x, y, clr, 0, FullPageType);//COLOR_BLUE);
}
//---------------------------------------------------------------------------
void WaitDecodeFinish(void)
{
  INT16U Count = 0;
  while((inportb(DE_J_STATUS)&0x01)==0x01)//;    //等待解码完成
        {
          asm{
               nop
               nop
               nop
               nop
               nop
               nop
               nop
             }
          Count ++;
          if(Count > 4000)
           {
            xtm_printf("WaitDecodeFinish::timeout Count = %d\n", Count);
            break;
           }
         }
}
//---------------------------------------------------------------------------
//预处理 Image 文件
INT8U PreDealImage(struct TImage *t_image)
{
  INT8U read_spi[512];
  struct file_head1 file_head;
  disp_jpeg_bitstream = Get_jpeg_bitstream();
#ifdef _PRINTINFO
  xtm_printf("t_image->Faddr = 0x%X %X\n", \
                                (INT16U)((t_image->Faddr & 0xffff0000) >> 16), (INT16U)(t_image->Faddr & 0xffff));
#endif
  xtm_read25x32data(t_image->Faddr, read_spi, sizeof(struct file_head1));
#ifdef _PRINTINFO
  xtm_printf("0x%X, 0x%X, 0x%X, 0x%X\n", read_spi[0], read_spi[1], read_spi[2], read_spi[3]);
#endif
  xtm_memcopy(&file_head, read_spi, sizeof(struct file_head1));
#ifdef _PRINTINFO
  xtm_printf("读 Faddr = 0x%X%X , Filelen = 0x%X%X, Flen = 0x%X%X, TotalPackage = %d, CurrPackage = %d, Width = %d, Height = %d, xLeft = %d, yTop = %d, Datalen = %d\n", \
                                (INT16U)((file_head.Faddr & 0xffff0000) >> 16), (INT16U)(file_head.Faddr & 0xffff), \
                                (INT16U)((file_head.Filelen & 0xffff0000) >> 16), (INT16U)(file_head.Filelen & 0xffff), \
                                (INT16U)((file_head.FLen & 0xffff0000) >> 16), (INT16U)(file_head.FLen & 0xffff), \
                                file_head.TotalPackage, file_head.CurrPackage, file_head.Width, file_head.Height, file_head.xLeft, file_head.yTop, file_head.Datalen);
#endif
  t_image->Jpegaddr = file_head.Faddr + 256;
  t_image->Maddr = ((INT32U)disp_jpeg_bitstream&0xffff)+(((INT32U)disp_jpeg_bitstream&0xffff0000)>>12)|0x700000;
  t_image->xLeft = file_head.xLeft;
  t_image->yTop = file_head.yTop;
  t_image->Len = file_head.Filelen;
  t_image->FLen = file_head.FLen;
  t_image->width = file_head.Width;
  t_image->height = file_head.Height;


#ifdef _PRINTINFO
  xtm_read25x32data(t_image->Faddr + 256, read_spi, sizeof(struct file_head1));
  xtm_printf("11 0x%X, 0x%X, 0x%X, 0x%X\n", read_spi[0], read_spi[1], read_spi[2], read_spi[3]);
#endif

#ifdef _PRINTINFO
  xtm_read25x32data(t_image->Faddr + 0x1500, read_spi, 256);
  xtm_printf("22 0x%X, 0x%X, 0x%X, 0x%X\n", read_spi[0x28], read_spi[0x29], read_spi[0x30], read_spi[0x31]);
#endif
}
//---------------------------------------------------------------------------
INT8U PreDealImage1111(struct TImage *t_image)
{
#if 0
  INT8U read_spi[512];
  struct file_head1 file_head;
  disp_jpeg_bitstream = Get_jpeg_bitstream();

  //xtm_printf("t_image->Faddr = 0x%X %X\n", \
  //                              (INT16U)((t_image->Faddr & 0xffff0000) >> 16), (INT16U)(t_image->Faddr & 0xffff));

  xtm_read25x32data111(t_image->Faddr, read_spi, 0x10000);//sizeof(struct file_head1));

  //xtm_printf("0x%X, 0x%X, 0x%X, 0x%X\n", read_spi[0], read_spi[1], read_spi[2], read_spi[3]);

  xtm_memcopy(&file_head, read_spi, sizeof(struct file_head1));

 // xtm_printf("读 Faddr = 0x%X%X , Filelen = 0x%X%X, Flen = 0x%X%X, TotalPackage = %d, CurrPackage = %d, Width = %d, Height = %d, xLeft = %d, yTop = %d, Datalen = %d\n", \
 //                               (INT16U)((file_head.Faddr & 0xffff0000) >> 16), (INT16U)(file_head.Faddr & 0xffff), \
 //                               (INT16U)((file_head.Filelen & 0xffff0000) >> 16), (INT16U)(file_head.Filelen & 0xffff), \
 //                               (INT16U)((file_head.FLen & 0xffff0000) >> 16), (INT16U)(file_head.FLen & 0xffff), \
 //                               file_head.TotalPackage, file_head.CurrPackage, file_head.Width, file_head.Height, file_head.xLeft, file_head.yTop, file_head.Datalen);

  t_image->Jpegaddr = file_head.Faddr + 256;
  t_image->Maddr = ((INT32U)disp_jpeg_bitstream&0xffff)+(((INT32U)disp_jpeg_bitstream&0xffff0000)>>12)|0x700000;
  t_image->xLeft = file_head.xLeft;
  t_image->yTop = file_head.yTop;
  t_image->Len = file_head.Filelen;
  t_image->FLen = file_head.FLen;
  t_image->width = file_head.Width;
  t_image->height = file_head.Height;



  //xtm_read25x32data111(t_image->Faddr + 256, read_spi, sizeof(struct file_head1));
  //xtm_printf("11 0x%X, 0x%X, 0x%X, 0x%X\n", read_spi[0], read_spi[1], read_spi[2], read_spi[3]);



  //xtm_read25x32data111(t_image->Faddr + 0x1500, read_spi, 256);
  //xtm_printf("22 0x%X, 0x%X, 0x%X, 0x%X\n", read_spi[0x28], read_spi[0x29], read_spi[0x30], read_spi[0x31]);
#endif
}
//---------------------------------------------------------------------------
extern INT8U ErrorStopDecode;
INT8U DisplayImage(struct TImage *t_image, INT8U FromType, INT8U FullPageType)
{
        INT32U j;
        INT32U tmp_data, tmp_data1;
        INT16U ReadNum, i;
        INT8U DecodeNum;
        INT8U DecoceErr;
        INT8U m_crc;

 /* xtm_printf("读 Faddr = 0x%X%X , Filelen = 0x%X%X, Flen = 0x%X%X, Width = %d, Height = %d, xLeft = %d, yTop = %d\n", \
                                (INT16U)((t_image->Faddr & 0xffff0000) >> 16), (INT16U)(t_image->Faddr & 0xffff), \
                                (INT16U)((t_image->Len & 0xffff0000) >> 16), (INT16U)(t_image->Len & 0xffff), \
                                (INT16U)((t_image->FLen & 0xffff0000) >> 16), (INT16U)(t_image->FLen & 0xffff), \
                                 t_image->width, t_image->height, t_image->xLeft, t_image->yTop);         */
        if((t_image->width > SCREEN_WIDTH)||(t_image->height > SCREEN_HEIGHT))
         {
          xtm_printf("DisplayImage::width = 0x%X, height = 0x%X\n", t_image->width, t_image->height);
          return 0;
         }
        if((t_image->xLeft > SCREEN_WIDTH)||(t_image->yTop > SCREEN_HEIGHT))
         {
          xtm_printf("DisplayImage::xLeft = 0x%X, yTop = 0x%X\n", t_image->xLeft, t_image->yTop);
          return 0;
         }
        Local_fb_addr = GetLocal_fb_addr();
        Local_fb_page = GetLocal_fb_page();
        disp_jpeg_bitstream = Get_jpeg_bitstream();

  //xtm_printf("DisplayImage::Local_fb_addr[%d] = 0x%X %X\n", \
  //                              !Local_fb_page, (INT16U)((Local_fb_addr[!Local_fb_page] & 0xffff0000) >> 16), (INT16U)(Local_fb_addr[!Local_fb_page] & 0xffff));

    Set_Decode_Clock(1); //操作视频解码时钟  1  开   0  关
        
    DecodeNum = 0;
    do{
        WaitDecodeFinish();     //等待解码完成

        if(FromType == FROMFILE)
         {
          //xtm_read25x32data(t_image->Jpegaddr, disp_jpeg_bitstream, t_image->FLen);
          //XTM_SPI_DRAM(t_image->Jpegaddr, t_image->Maddr, t_image->FLen);
          ReadNum = 0;
          while(1)
           {
            //outportb(CONTROL_REG2, inportb(CONTROL_REG2) | 0x02);
            //outportb(CONTROL_REG2, inportb(CONTROL_REG2) & 0xfd);
            //asm cli;
            //outportb(CONTROL_REG, inportb(CONTROL_REG)&0xfe);  //1 1111 110  显示禁止    0xfe
            XTM_SPI_DRAM(t_image->Jpegaddr, t_image->Maddr, t_image->FLen);  //  + 0x2000
            //outportb(CONTROL_REG, inportb(CONTROL_REG)|0x01);  //0 0000 001  显示使能
            //asm sti;
            //outportb(CONTROL_REG2, inportb(CONTROL_REG2) & 0xfd);
            ReadNum ++;

            //m_crc = 0;
            //for(j=0; j<t_image->Len; j++)
            //  m_crc += disp_jpeg_bitstream[j];

           #if 0
            if((disp_jpeg_bitstream[t_image->Len - 8] == 0x01)&&(disp_jpeg_bitstream[t_image->Len - 7] == 0x02)
             &&(disp_jpeg_bitstream[t_image->Len - 6] == 0x03)&&(disp_jpeg_bitstream[t_image->Len - 5] == 0x04)
             &&(disp_jpeg_bitstream[t_image->Len - 4] == 0x05)&&(disp_jpeg_bitstream[t_image->Len - 3] == 0x06)
             &&(disp_jpeg_bitstream[t_image->Len - 2] == 0x07)&&(disp_jpeg_bitstream[t_image->Len - 1] == 0x08))
           #else
            if((disp_jpeg_bitstream[0] == 0xff)&&(disp_jpeg_bitstream[1] == 0xd8)&&(disp_jpeg_bitstream[t_image->Len - 2] == 0xff)&&(disp_jpeg_bitstream[t_image->Len - 1] == 0xd9))
           #endif
             {
              if(ReadNum > 1)
                xtm_printf("DisplayImage:: ReadNum = %d\n", ReadNum);
              break;
             }
            ClearWatchDog();
            if(ReadNum > 10)
             {
              xtm_printf("DisplayImage:: ReadNum = %d\n", ReadNum);
              break;
             }
           }
         }
        else
         {
          decode_dqt_init();
          xtm_read25x32data(t_image->Jpegaddr, disp_jpeg_bitstream, t_image->FLen);
         }

        //图像宽度由DE_J_MBWIDTH乘以16
        //图像高度由DE_J_MBHEIGHT乘以16
       	outportb(DE_J_MBWIDTH, t_image->width >> 4);//0x28);
       	outportb(DE_J_MBHEIGHT, t_image->height >> 4);
	outport(DE_J_HSIZE_L, DISP_WIDTH);
	outport(DE_J_VSIZE_L, DISP_HEIGHT);

	outportb(DE_J_CPU_SA_H, (t_image->Maddr>>16));
 	outportb(DE_J_CPU_SA_M, (t_image->Maddr>>8));
	outportb(DE_J_CPU_SA_L, t_image->Maddr);

       #if 0
	outportb(DE_J_CPU_LEN_H, ((t_image->Len - 8)>>16));
 	outportb(DE_J_CPU_LEN_M, ((t_image->Len - 8)>>8));
	outportb(DE_J_CPU_LEN_L, t_image->Len - 8);
       #else
	outportb(DE_J_CPU_LEN_H, ((t_image->Len)>>16));
 	outportb(DE_J_CPU_LEN_M, ((t_image->Len)>>8));
	outportb(DE_J_CPU_LEN_L, t_image->Len);
       #endif

        tmp_data = (((INT32U) t_image->yTop*DISP_WIDTH +(INT32U) t_image->xLeft)<<1) + Local_fb_addr[!Local_fb_page];     //显示到后台页

	outportb(DE_J_DATA_SA_H, tmp_data>>16);
 	outportb(DE_J_DATA_SA_M, tmp_data>>8);
	outportb(DE_J_DATA_SA_L, tmp_data);

	outportb(DE_J_CPU_PN, 1);

        //DeocdeFinishFlag = 0;

	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x9a);
	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xfd);

        //while(!DeocdeFinishFlag);
	WaitDecodeFinish();    //等待解码完成
        if(inportb(DE_J_STATUS) != 0)
          xtm_printf("DE_J_STATUS = 0x%X\n", inportb(DE_J_STATUS));
        DecodeNum ++;
        if(DecodeNum > 10)
          break;

        ClearWatchDog();
        DecoceErr = Get_Mjpeg_DecodeError();
    }while(DecoceErr == 1);

    if(DecodeNum > 1)
     {
      xtm_printf("DisplayImage::DecodeNum = %d, DecoceErr = %d\n", DecodeNum, DecoceErr);
      ErrorStopDecode = 1;
     } 
    if(!FullPageType)
     {
          //if(DecoceErr == 0)
          // {
            for(i=0; i<t_image->height; i++)
             {
              tmp_data1 = Local_fb_addr[Local_fb_page] + (((INT32U) (i + t_image->yTop)*DISP_WIDTH +(INT32U)t_image->xLeft)<<1) ;
              fmem_fmem_dma(tmp_data + (INT32U)(i)*DISP_WIDTH*2, tmp_data1, t_image->width*2);
             }
          /* }
          else
           {
             clear_area(t_image->xLeft, t_image->yTop, t_image->height, t_image->width, COLOR_BLUE, Local_fb_page);
           }  */
     }

     Set_Decode_Clock(0); //操作视频解码时钟  1  开   0  关
}
//---------------------------------------------------------------------------
//预处理 ImageButton 文件
INT8U PreDealImageButton(struct TImageButton *t_button, INT8U buttontype)
{
  INT8U read_spi[512];
  struct file_head1 file_head;
  disp_jpeg_bitstream = Get_jpeg_bitstream();
#ifdef _PRINTINFO
  xtm_printf("t_button->Faddr[%d] = 0x%X %X\n", \
                                  buttontype, (INT16U)((t_button->Faddr[buttontype] & 0xffff0000) >> 16), (INT16U)(t_button->Faddr[buttontype] & 0xffff));
#endif
  xtm_read25x32data(t_button->Faddr[buttontype], read_spi, sizeof(struct file_head1));
#ifdef _PRINTINFO
  xtm_printf("0x%X, 0x%X, 0x%X, 0x%X\n", read_spi[0], read_spi[1], read_spi[2], read_spi[3]);
#endif
  xtm_memcopy(&file_head, read_spi, sizeof(struct file_head1));
#ifdef _PRINTINFO
  xtm_printf("读 Faddr = 0x%X%X , Filelen = 0x%X%X, Flen = 0x%X%X, TotalPackage = %d, CurrPackage = %d, Width = %d, Height = %d, xLeft = %d, yTop = %d, Datalen = %d\n", \
                                (INT16U)((file_head.Faddr & 0xffff0000) >> 16), (INT16U)(file_head.Faddr & 0xffff), \
                                (INT16U)((file_head.Filelen & 0xffff0000) >> 16), (INT16U)(file_head.Filelen & 0xffff), \
                                (INT16U)((file_head.FLen & 0xffff0000) >> 16), (INT16U)(file_head.FLen & 0xffff), \
                                file_head.TotalPackage, file_head.CurrPackage, file_head.Width, file_head.Height, file_head.xLeft, file_head.yTop, file_head.Datalen);
#endif
  t_button->Jpegaddr[buttontype] = file_head.Faddr + 256;
  t_button->Maddr[buttontype] = ((INT32U)disp_jpeg_bitstream&0xffff)+(((INT32U)disp_jpeg_bitstream&0xffff0000)>>12)|0x700000;
  t_button->xLeft = file_head.xLeft;
  t_button->yTop = file_head.yTop;
  t_button->Len[buttontype] = file_head.Filelen;
  t_button->FLen[buttontype] = file_head.FLen;
  t_button->width = file_head.Width;
  t_button->height = file_head.Height;


#ifdef _PRINTINFO
  xtm_read25x32data(t_button->Faddr[buttontype] + 256, read_spi, sizeof(struct file_head1));
  xtm_printf("11 0x%X, 0x%X, 0x%X, 0x%X\n", read_spi[0], read_spi[1], read_spi[2], read_spi[3]);
#endif

#ifdef _PRINTINFO
  xtm_read25x32data(t_button->Faddr[buttontype] + 0x1500, read_spi, 256);
  xtm_printf("22 0x%X, 0x%X, 0x%X, 0x%X\n", read_spi[0x28], read_spi[0x29], read_spi[0x30], read_spi[0x31]);
#endif
}
//---------------------------------------------------------------------------
INT8U DisplayImageButton(struct TImageButton *t_button, INT8U buttontype, INT8U FromType, INT8U FullPageType)
{
        INT32U j;
        INT32U tmp_data, tmp_data1;
        INT16U ReadNum, i;
        INT8U DecodeNum;
        INT8U m_crc;

 /* xtm_printf("读 Faddr = 0x%X%X , Filelen = 0x%X%X, Flen = 0x%X%X, Width = %d, Height = %d, xLeft = %d, yTop = %d\n", \
                                (INT16U)((t_button->Faddr[buttontype] & 0xffff0000) >> 16), (INT16U)(t_button->Faddr[buttontype] & 0xffff), \
                                (INT16U)((t_button->Len[buttontype] & 0xffff0000) >> 16), (INT16U)(t_button->Len[buttontype] & 0xffff), \
                                (INT16U)((t_button->FLen[buttontype] & 0xffff0000) >> 16), (INT16U)(t_button->FLen[buttontype] & 0xffff), \
                                 t_button->width, t_button->height, t_button->xLeft, t_button->yTop);    */

        if((t_button->width > SCREEN_WIDTH)||(t_button->height > SCREEN_HEIGHT))
         {
          xtm_printf("DisplayImageButton::width = 0x%X, height = 0x%X\n", t_button->width, t_button->height);
          return 0;
         }
        if((t_button->xLeft > SCREEN_WIDTH)||(t_button->yTop > SCREEN_HEIGHT))
         {
          xtm_printf("DisplayImageButton::xLeft = 0x%X, yTop = 0x%X\n", t_button->xLeft, t_button->yTop);
          return 0;
         }
        Local_fb_addr = GetLocal_fb_addr();
        Local_fb_page = GetLocal_fb_page();
        disp_jpeg_bitstream = Get_jpeg_bitstream();
    Set_Decode_Clock(1); //操作视频解码时钟  1  开   0  关

    DecodeNum = 0;
    do{
        WaitDecodeFinish();      //等待解码完成
        Set_Local_DecodeJpegFlag(1);  //正在解静态图片

        if(FromType == FROMFILE)
         {
          //xtm_read25x32data(t_button->Jpegaddr[buttontype], disp_jpeg_bitstream, t_button->FLen[buttontype]);
          //XTM_SPI_DRAM(t_button->Jpegaddr[buttontype], t_button->Maddr[buttontype], t_button->FLen[buttontype]);
        #if 1
          ReadNum = 0;
          while(1)
           {
            //outportb(CONTROL_REG2, inportb(CONTROL_REG2) & 0xfd);
            //outportb(CONTROL_REG2, inportb(CONTROL_REG2) | 0x02);
            //asm cli;
            //outportb(CONTROL_REG, inportb(CONTROL_REG)&0xfe);  //1 1111 110  显示禁止    0xfe
            XTM_SPI_DRAM(t_button->Jpegaddr[buttontype], t_button->Maddr[buttontype], t_button->FLen[buttontype]);  //  + 0x2000
            //outportb(CONTROL_REG, inportb(CONTROL_REG)|0x01);  //0 0000 001  显示使能
            //asm sti;
            //outportb(CONTROL_REG2, inportb(CONTROL_REG2) & 0xfd);
            ReadNum ++;

            //m_crc = 0;
            //for(j=0; j<t_button->Len[buttontype]; j++)
            //  m_crc += disp_jpeg_bitstream[j];

           #if 0
            if((disp_jpeg_bitstream[t_button->Len[buttontype] - 8] == 0x01)&&(disp_jpeg_bitstream[t_button->Len[buttontype] - 7] == 0x02)
             &&(disp_jpeg_bitstream[t_button->Len[buttontype] - 6] == 0x03)&&(disp_jpeg_bitstream[t_button->Len[buttontype] - 5] == 0x04)
             &&(disp_jpeg_bitstream[t_button->Len[buttontype] - 4] == 0x05)&&(disp_jpeg_bitstream[t_button->Len[buttontype] - 3] == 0x06)
             &&(disp_jpeg_bitstream[t_button->Len[buttontype] - 2] == 0x07)&&(disp_jpeg_bitstream[t_button->Len[buttontype] - 1] == 0x08))
           #else
            if((disp_jpeg_bitstream[0] == 0xff)&&(disp_jpeg_bitstream[1] == 0xd8)&&(disp_jpeg_bitstream[t_button->Len[buttontype] - 2] == 0xff)&&(disp_jpeg_bitstream[t_button->Len[buttontype] - 1] == 0xd9))
           #endif
             {
              if(ReadNum > 1)
                xtm_printf("DisplayImageButton:: ReadNum = %d\n", ReadNum);
              break;
             }
            if(ReadNum > 10)
             {
              xtm_printf("DisplayImageButton:: ReadNum = %d\n", ReadNum);
              break;
             }
           }
          //xtm_printf("jpeg_bitstream[t_button->Len[buttontype] = 0x%X %X, jpeg_bitstream = 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", \
          //        (INT16U)((t_button->Len[buttontype] & 0xffff0000) >> 16), (INT16U)(t_button->Len[buttontype] & 0xffff), \
          //        jpeg_bitstream[t_button->Len[buttontype] - 10], jpeg_bitstream[t_button->Len[buttontype] - 9], jpeg_bitstream[t_button->Len[buttontype] - 8], jpeg_bitstream[t_button->Len[buttontype] - 7], \
          //        jpeg_bitstream[t_button->Len[buttontype] - 6], jpeg_bitstream[t_button->Len[buttontype] - 5], \
          //        jpeg_bitstream[t_button->Len[buttontype] - 4], jpeg_bitstream[t_button->Len[buttontype] - 3], jpeg_bitstream[t_button->Len[buttontype] - 2], jpeg_bitstream[t_button->Len[buttontype] - 1]);
               //   xtm_printf("menu_button[0].FLen[IMAGEUP] = 0x%X, jpeg_bitstream = 0x%X, 0x%X, 0x%X, 0x%X\n", menu_button[0].FLen[IMAGEUP], jpeg_bitstream[20], jpeg_bitstream[21], jpeg_bitstream[22], jpeg_bitstream[23]);
               /*   for(i=0; i<t_button->FLen[IMAGEUP]; i++)
                   {
                    if(jpeg_bitstream[i] != jpeg_bitstream[0x2000 + i])
                      xtm_printf("i = %d\n", i);
                   }         */
        #endif
         }
        else
         {
          decode_dqt_init();
          xtm_read25x32data(t_button->Jpegaddr[buttontype], disp_jpeg_bitstream, t_button->FLen[buttontype]);
         }         

        //图像宽度由DE_J_MBWIDTH乘以16
        //图像高度由DE_J_MBHEIGHT乘以16
       	outportb(DE_J_MBWIDTH, t_button->width >> 4);//0x28);
       	outportb(DE_J_MBHEIGHT, t_button->height >> 4);
	outport(DE_J_HSIZE_L, DISP_WIDTH);
	outport(DE_J_VSIZE_L, DISP_HEIGHT);

	outportb(DE_J_CPU_SA_H, (t_button->Maddr[buttontype]>>16));
 	outportb(DE_J_CPU_SA_M, (t_button->Maddr[buttontype]>>8));
	outportb(DE_J_CPU_SA_L, t_button->Maddr[buttontype]);
      #if 0
	outportb(DE_J_CPU_LEN_H, ((t_button->Len[buttontype] - 8)>>16));
 	outportb(DE_J_CPU_LEN_M, ((t_button->Len[buttontype] - 8)>>8));
	outportb(DE_J_CPU_LEN_L, (t_button->Len[buttontype] - 8));
      #else
	outportb(DE_J_CPU_LEN_H, ((t_button->Len[buttontype])>>16));
 	outportb(DE_J_CPU_LEN_M, ((t_button->Len[buttontype])>>8));
	outportb(DE_J_CPU_LEN_L, (t_button->Len[buttontype]));
      #endif

        tmp_data = (((INT32U) t_button->yTop*DISP_WIDTH +(INT32U) t_button->xLeft)<<1) + Local_fb_addr[!Local_fb_page];     //显示到后台页

	outportb(DE_J_DATA_SA_H, tmp_data>>16);
 	outportb(DE_J_DATA_SA_M, tmp_data>>8);
	outportb(DE_J_DATA_SA_L, tmp_data);

	outportb(DE_J_CPU_PN, 1);

        //DeocdeFinishFlag = 0;
	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x9a);
	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xfd);
        //while(!DeocdeFinishFlag);

	WaitDecodeFinish();     //等待解码完成
        //if(inportb(DE_J_STATUS) != 0)
        //  xtm_printf("2 DE_J_STATUS = 0x%X\n", inportb(DE_J_STATUS));
        DecodeNum ++;
        if(DecodeNum > 10)
          break;
        ClearWatchDog();
    }while(Get_Mjpeg_DecodeError() == 1);

    if(DecodeNum > 1)
     {
      xtm_printf("DisplayImageButton::DecodeNum = %d\n", DecodeNum);
      ErrorStopDecode = 1;
     }

        if(!FullPageType)
         for(i=0; i<t_button->height; i++)
          {
           tmp_data1 = Local_fb_addr[Local_fb_page] + (((INT32U) (i + t_button->yTop)*DISP_WIDTH +(INT32U)t_button->xLeft)<<1) ;
           fmem_fmem_dma(tmp_data + (INT32U)(i)*DISP_WIDTH*2, tmp_data1, t_button->width*2);
          }
      Set_Local_DecodeJpegFlag(0);  //正在解静态图片
      if(Get_Local_DecodeStreamFlag() == 0)  //正在解码流
        Set_Decode_Clock(0); //操作视频解码时钟  1  开   0  关
      else
        decode_dqt_init();
}
//---------------------------------------------------------------------------
//显示图形按键文本
void DisplayImageButton_Text(struct TImageButton *t_button, INT16U color, INT8U wd, INT8U FullPageType)
{
  if(t_button->FontType == 24)
    display_text(t_button->xLeft + t_button->Text_xLeft, t_button->yTop + t_button->Text_yTop, wd, color, t_button->buttonname, 0, 24, FullPageType);
  else
    display_text(t_button->xLeft + t_button->Text_xLeft, t_button->yTop + t_button->Text_yTop, wd, color, t_button->buttonname, 0, 16, FullPageType);
}
//---------------------------------------------------------------------------
//预处理 Edit 文件
INT8U PreDealEdit(struct TEdit *t_edit, INT8U buttontype)
{
  INT8U read_spi[512];
  struct file_head1 file_head;
  disp_jpeg_bitstream = Get_jpeg_bitstream();
#ifdef _PRINTINFO
  xtm_printf("t_edit->Faddr[%d] = 0x%X %X\n", \
                                  buttontype, (INT16U)((t_edit->Faddr[buttontype] & 0xffff0000) >> 16), (INT16U)(t_edit->Faddr[buttontype] & 0xffff));
#endif
  xtm_read25x32data(t_edit->Faddr[buttontype], read_spi, sizeof(struct file_head1));
#ifdef _PRINTINFO
  xtm_printf("0x%X, 0x%X, 0x%X, 0x%X\n", read_spi[0], read_spi[1], read_spi[2], read_spi[3]);
#endif
  xtm_memcopy(&file_head, read_spi, sizeof(struct file_head1));
#ifdef _PRINTINFO
  xtm_printf("读 Faddr = 0x%X%X , Filelen = 0x%X%X, Flen = 0x%X%X, TotalPackage = %d, CurrPackage = %d, Width = %d, Height = %d, xLeft = %d, yTop = %d, Datalen = %d\n", \
                                (INT16U)((file_head.Faddr & 0xffff0000) >> 16), (INT16U)(file_head.Faddr & 0xffff), \
                                (INT16U)((file_head.Filelen & 0xffff0000) >> 16), (INT16U)(file_head.Filelen & 0xffff), \
                                (INT16U)((file_head.FLen & 0xffff0000) >> 16), (INT16U)(file_head.FLen & 0xffff), \
                                file_head.TotalPackage, file_head.CurrPackage, file_head.Width, file_head.Height, file_head.xLeft, file_head.yTop, file_head.Datalen);
#endif
  t_edit->Jpegaddr[buttontype] = file_head.Faddr + 256;
  t_edit->Maddr[buttontype] = ((INT32U)disp_jpeg_bitstream&0xffff)+(((INT32U)disp_jpeg_bitstream&0xffff0000)>>12)|0x700000;
  t_edit->xLeft = file_head.xLeft;
  t_edit->yTop = file_head.yTop;
  t_edit->Len[buttontype] = file_head.Filelen;
  t_edit->FLen[buttontype] = file_head.FLen;
  t_edit->width = file_head.Width;
  t_edit->height = file_head.Height;

#ifdef _PRINTINFO
  xtm_read25x32data(t_edit->Faddr[buttontype] + 256, read_spi, sizeof(struct file_head1));
  xtm_printf("11 0x%X, 0x%X, 0x%X, 0x%X\n", read_spi[0], read_spi[1], read_spi[2], read_spi[3]);
#endif

#ifdef _PRINTINFO
  xtm_read25x32data(t_edit->Faddr[buttontype] + 0x1500, read_spi, 256);
  xtm_printf("22 0x%X, 0x%X, 0x%X, 0x%X\n", read_spi[0x28], read_spi[0x29], read_spi[0x30], read_spi[0x31]);
#endif  
}
//---------------------------------------------------------------------------
INT8U DisplayEdit(struct TEdit *t_edit, INT8U buttontype, INT8U FromType, INT8U FullPageType)
{
        INT32U tmp_data, tmp_data1;
        INT16U ReadNum, i;
        INT8U DecodeNum;        

        if((t_edit->width > SCREEN_WIDTH)||(t_edit->height > SCREEN_HEIGHT))
         {
          xtm_printf("DisplayEdit::width = 0x%X, height = 0x%X\n", t_edit->width, t_edit->height);
          return 0;
         }
        if((t_edit->xLeft > SCREEN_WIDTH)||(t_edit->yTop > SCREEN_HEIGHT))
         {
          xtm_printf("DisplayEdit::xLeft = 0x%X, yTop = 0x%X\n", t_edit->xLeft, t_edit->yTop);
          return 0;
         }
        Local_fb_addr = GetLocal_fb_addr();
        Local_fb_page = GetLocal_fb_page();
        disp_jpeg_bitstream = Get_jpeg_bitstream();
    Set_Decode_Clock(1); //操作视频解码时钟  1  开   0  关
    
    DecodeNum = 0;         
    do{
        WaitDecodeFinish();     //等待解码完成

        if(FromType == FROMFILE)
         {
          ReadNum = 0;
          while(1)
           {
            XTM_SPI_DRAM(t_edit->Jpegaddr[buttontype], t_edit->Maddr[buttontype], t_edit->FLen[buttontype]);  //  + 0x2000
            ReadNum ++;
           #if 0
            if((disp_jpeg_bitstream[t_edit->Len[buttontype] - 8] == 0x01)&&(disp_jpeg_bitstream[t_edit->Len[buttontype] - 7] == 0x02)
             &&(disp_jpeg_bitstream[t_edit->Len[buttontype] - 6] == 0x03)&&(disp_jpeg_bitstream[t_edit->Len[buttontype] - 5] == 0x04)
             &&(disp_jpeg_bitstream[t_edit->Len[buttontype] - 4] == 0x05)&&(disp_jpeg_bitstream[t_edit->Len[buttontype] - 3] == 0x06)
             &&(disp_jpeg_bitstream[t_edit->Len[buttontype] - 2] == 0x07)&&(disp_jpeg_bitstream[t_edit->Len[buttontype] - 1] == 0x08))
           #else
            if((disp_jpeg_bitstream[t_edit->Len[buttontype] - 2] == 0xff)&&(disp_jpeg_bitstream[t_edit->Len[buttontype] - 1] == 0xd9))
           #endif
             {
              if(ReadNum > 1)
                xtm_printf("DisplayEdit:: ReadNum = %d\n", ReadNum);
              break;
             }
            if(ReadNum > 10)
             {
              xtm_printf("DisplayEdit:: ReadNum = %d\n", ReadNum);
              break;
             }
           }
         }

        //图像宽度由DE_J_MBWIDTH乘以16
        //图像高度由DE_J_MBHEIGHT乘以16
       	outportb(DE_J_MBWIDTH, t_edit->width >> 4);//0x28);
       	outportb(DE_J_MBHEIGHT, t_edit->height >> 4);
	outport(DE_J_HSIZE_L, DISP_WIDTH);
	outport(DE_J_VSIZE_L, DISP_HEIGHT);

	outportb(DE_J_CPU_SA_H, (t_edit->Maddr[buttontype]>>16));
 	outportb(DE_J_CPU_SA_M, (t_edit->Maddr[buttontype]>>8));
	outportb(DE_J_CPU_SA_L, t_edit->Maddr[buttontype]);

       #if 0
	outportb(DE_J_CPU_LEN_H, ((t_edit->Len[buttontype] - 8)>>16));
 	outportb(DE_J_CPU_LEN_M, ((t_edit->Len[buttontype] - 8)>>8));
	outportb(DE_J_CPU_LEN_L, t_edit->Len[buttontype] - 8);
       #else
	outportb(DE_J_CPU_LEN_H, ((t_edit->Len[buttontype])>>16));
 	outportb(DE_J_CPU_LEN_M, ((t_edit->Len[buttontype])>>8));
	outportb(DE_J_CPU_LEN_L, t_edit->Len[buttontype]);
       #endif

        tmp_data = (((INT32U) t_edit->yTop*DISP_WIDTH +(INT32U) t_edit->xLeft)<<1) + Local_fb_addr[!Local_fb_page];     //显示到后台页

        //tmp_data = 0x400000 + (INT32U)t_edit->xLeft*2 + (INT32U)t_edit->yTop*1600;      // 0x4271C8UL;//
	outportb(DE_J_DATA_SA_H, tmp_data>>16);
 	outportb(DE_J_DATA_SA_M, tmp_data>>8);
	outportb(DE_J_DATA_SA_L, tmp_data);

	outportb(DE_J_CPU_PN, 1);

	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x9a);
	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xfd);

	WaitDecodeFinish();    //等待解码完成
        DecodeNum ++;
        if(DecodeNum > 10)
          break;
        ClearWatchDog();
    }while(Get_Mjpeg_DecodeError() == 1);

        if(!FullPageType)
         for(i=0; i<t_edit->height; i++)
          {
           tmp_data1 = Local_fb_addr[Local_fb_page] + (((INT32U) (i + t_edit->yTop)*DISP_WIDTH +(INT32U)t_edit->xLeft)<<1) ;
           fmem_fmem_dma(tmp_data + (INT32U)(i)*DISP_WIDTH*2, tmp_data1, t_edit->width*2);
          }

     Set_Decode_Clock(0); //操作视频解码时钟  1  开   0  关
}
//---------------------------------------------------------------------------
void DisplayEdit_Text(struct TEdit *t_edit, INT16U color, INT8U wd, INT8U FullPageType)
{
  if(t_edit->FontType == 24)
    display_text(t_edit->xLeft + t_edit->Text_xLeft, t_edit->yTop + t_edit->Text_yTop, wd, color, t_edit->Text, 0, 24, FullPageType);
  else
    display_text(t_edit->xLeft + t_edit->Text_xLeft, t_edit->yTop + t_edit->Text_yTop, wd, color, t_edit->Text, 0, 16, FullPageType);
}
//---------------------------------------------------------------------------
void DisplayPassEdit_Text(struct TEdit *t_edit, INT16U color, INT8U wd, INT8U FullPageType)
{
  INT8U text_len, i;
  char str[20];
  text_len = strlen(t_edit->Text);
  for(i=0; i<text_len; i++)
    str[i] = '*';
  str[text_len] = '\0';
  if(t_edit->FontType == 24)
    display_text(t_edit->xLeft + t_edit->Text_xLeft, t_edit->yTop + t_edit->Text_yTop, wd, color, str, 0, 24, FullPageType);
  else
    display_text(t_edit->xLeft + t_edit->Text_xLeft, t_edit->yTop + t_edit->Text_yTop, wd, color, str, 0, 16, FullPageType);
}
//---------------------------------------------------------------------------

