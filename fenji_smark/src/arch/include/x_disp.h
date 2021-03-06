#define FROMFILE 0
#define FROMMEMORY 1

struct file_head1
  {
   INT32U Faddr;    //Flash 地址
   INT32U Filelen;            //文件大小  实际大小
   INT32U FLen;               //Flash存储大小
   INT16U FileType;         //文件类型 0 -- 主程序  1 -- 图片   2 -- 字库   3 -- 声音
   INT16U Width;
   INT16U Height;
   INT16U xLeft;
   INT16U yTop;
   INT16U TotalPackage;      //总包数
   INT16U CurrPackage;       //当前包数
   INT16U Datalen;           //数据长度
  };
  
struct TImage{
               INT32U Faddr;    //Flash 地址
               INT32U Jpegaddr;    //Flash Jpeg Data地址
               INT32U Maddr;    //内存 地址
               INT32U FLen;     //Flash 传送长度   256的倍数
               INT32U Len;
               INT16U width;
               INT16U height;
               INT16U xLeft;
               INT16U yTop;
              };

struct TImageButton{
               //0--up  1--down
               INT32U Faddr[2];     //Flash 地址
               INT32U Jpegaddr[2];  //Flash Jpeg Data地址
               INT32U Maddr[2];     //内存 地址
               INT32U FLen[2];      //Flash 传送长度   256的倍数
               INT32U Len[2];
               INT16U width;
               INT16U height;
               INT16U xLeft;
               INT16U yTop;
               INT16U Text_xLeft;
               INT16U Text_yTop;
               char buttonname[30];
               INT8U FontColor; 
               INT8U FontType; //字体 16  24
               INT8U Visible;
              };

struct TEdit{
               //0--up  1--down
               INT32U Faddr[2];     //Flash 地址
               INT32U Jpegaddr[2];  //Flash Jpeg Data地址
               INT32U Maddr[2];     //内存 地址
               INT32U FLen[2];      //Flash 传送长度  256的倍数
               INT32U Len[2];
               INT16U width;
               INT16U height;
               INT16U xLeft;
               INT16U yTop;
               INT16U Text_xLeft;
               INT16U Text_yTop;               
               INT8U FontType; //字体 16  24
               INT8U Space;    //字距
               INT16U CursorX; //光标X坐标
               INT16U CursorY; //光标Y坐标
               INT16U CursorHeight; //光标高度
               INT16U CursorCorlor;
               INT16U fWidth;     //字宽度
               char Text[20];
               INT8U BoxLen;         //文本框当前输入长度
               INT8U MaxLen;         //文本框最大输入长度
               INT8U ValidLen;       //有效长度
               INT8U Focus;        //显示输入光标
               INT8U Visible;
              };
              
#ifndef x_disp_h
#define x_disp_h

extern void clear_disp_ram(void);
extern void xtm_clear_disp_ram(INT16U color);
extern void clear_area(INT16U x,INT16U y,INT16U Hi,INT16U Wi,INT16U color_val,INT8U ym);

extern void selectym(INT8U ye);
extern void gra_dispcn16(INT8U *ch, int x, int y,INT16U color_val,INT16U bcolor_val, INT8U FullPageType);
extern void gra_dispcn24(INT8U *ch, int x, int y,INT16U color_val,INT16U bcolor_val, INT8U FullPageType);
extern void display_text(INT16U x, INT16U y, INT8U wd, INT16U clr, char *s, INT8U pass, INT8U fonttype, INT8U FullPageType); //显示点阵汉字

//预处理 Image 文件
extern INT8U PreDealImage(struct TImage *t_image);
extern INT8U DisplayImage(struct TImage *t_image, INT8U FromType, INT8U FullPageType);

//预处理 ImageButton 文件
extern INT8U PreDealImageButton(struct TImageButton *t_button, INT8U buttontype);
extern INT8U DisplayImageButton(struct TImageButton *t_button, INT8U buttontype, INT8U FromType, INT8U FullPageType);
//显示图形按键文本
extern void DisplayImageButton_Text(struct TImageButton *t_button, INT16U color, INT8U wd, INT8U FullPageType);

//预处理 Edit 文件
extern INT8U PreDealEdit(struct TEdit *t_edit, INT8U buttontype);
extern INT8U DisplayEdit(struct TEdit *t_edit, INT8U buttontype, INT8U FromType, INT8U FullPageType);
extern void DisplayEdit_Text(struct TEdit *t_edit, INT16U color, INT8U wd, INT8U FullPageType);
extern void DisplayPassEdit_Text(struct TEdit *t_edit, INT16U color, INT8U wd, INT8U FullPageType);

extern void WaitDecodeFinish(void);
#endif

