#define FROMFILE 0
#define FROMMEMORY 1

struct file_head1
  {
   INT32U Faddr;    //Flash ��ַ
   INT32U Filelen;            //�ļ���С  ʵ�ʴ�С
   INT32U FLen;               //Flash�洢��С
   INT16U FileType;         //�ļ����� 0 -- ������  1 -- ͼƬ   2 -- �ֿ�   3 -- ����
   INT16U Width;
   INT16U Height;
   INT16U xLeft;
   INT16U yTop;
   INT16U TotalPackage;      //�ܰ���
   INT16U CurrPackage;       //��ǰ����
   INT16U Datalen;           //���ݳ���
  };
  
struct TImage{
               INT32U Faddr;    //Flash ��ַ
               INT32U Jpegaddr;    //Flash Jpeg Data��ַ
               INT32U Maddr;    //�ڴ� ��ַ
               INT32U FLen;     //Flash ���ͳ���   256�ı���
               INT32U Len;
               INT16U width;
               INT16U height;
               INT16U xLeft;
               INT16U yTop;
              };

struct TImageButton{
               //0--up  1--down
               INT32U Faddr[2];     //Flash ��ַ
               INT32U Jpegaddr[2];  //Flash Jpeg Data��ַ
               INT32U Maddr[2];     //�ڴ� ��ַ
               INT32U FLen[2];      //Flash ���ͳ���   256�ı���
               INT32U Len[2];
               INT16U width;
               INT16U height;
               INT16U xLeft;
               INT16U yTop;
               INT16U Text_xLeft;
               INT16U Text_yTop;
               char buttonname[30];
               INT8U FontColor; 
               INT8U FontType; //���� 16  24
               INT8U Visible;
              };

struct TEdit{
               //0--up  1--down
               INT32U Faddr[2];     //Flash ��ַ
               INT32U Jpegaddr[2];  //Flash Jpeg Data��ַ
               INT32U Maddr[2];     //�ڴ� ��ַ
               INT32U FLen[2];      //Flash ���ͳ���  256�ı���
               INT32U Len[2];
               INT16U width;
               INT16U height;
               INT16U xLeft;
               INT16U yTop;
               INT16U Text_xLeft;
               INT16U Text_yTop;               
               INT8U FontType; //���� 16  24
               INT8U Space;    //�־�
               INT16U CursorX; //���X����
               INT16U CursorY; //���Y����
               INT16U CursorHeight; //���߶�
               INT16U CursorCorlor;
               INT16U fWidth;     //�ֿ���
               char Text[20];
               INT8U BoxLen;         //�ı���ǰ���볤��
               INT8U MaxLen;         //�ı���������볤��
               INT8U ValidLen;       //��Ч����
               INT8U Focus;        //��ʾ������
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
extern void display_text(INT16U x, INT16U y, INT8U wd, INT16U clr, char *s, INT8U pass, INT8U fonttype, INT8U FullPageType); //��ʾ������

//Ԥ���� Image �ļ�
extern INT8U PreDealImage(struct TImage *t_image);
extern INT8U DisplayImage(struct TImage *t_image, INT8U FromType, INT8U FullPageType);

//Ԥ���� ImageButton �ļ�
extern INT8U PreDealImageButton(struct TImageButton *t_button, INT8U buttontype);
extern INT8U DisplayImageButton(struct TImageButton *t_button, INT8U buttontype, INT8U FromType, INT8U FullPageType);
//��ʾͼ�ΰ����ı�
extern void DisplayImageButton_Text(struct TImageButton *t_button, INT16U color, INT8U wd, INT8U FullPageType);

//Ԥ���� Edit �ļ�
extern INT8U PreDealEdit(struct TEdit *t_edit, INT8U buttontype);
extern INT8U DisplayEdit(struct TEdit *t_edit, INT8U buttontype, INT8U FromType, INT8U FullPageType);
extern void DisplayEdit_Text(struct TEdit *t_edit, INT16U color, INT8U wd, INT8U FullPageType);
extern void DisplayPassEdit_Text(struct TEdit *t_edit, INT16U color, INT8U wd, INT8U FullPageType);

extern void WaitDecodeFinish(void);
#endif
