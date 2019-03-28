#include "includes.h"

#include "stdlib.h"

#define x_first_h

#define CommonH
#include "common.h"

void InterfaceInit(void);

void PlaySoundTip(void); //������ʾ��

//��lib ����
INT32U GetLocal_fb_addr(void);
INT8U GetLocal_fb_page(void);
INT8U *Get_jpeg_bitstream(void);
INT8U Get_Mjpeg_DecodeError(void);
void Set_Local_DecodeJpegFlag(INT8U disp_DecodeJpegFlag);
INT8U Get_Local_DecodeStreamFlag(void);
//---------------------------------------------------------------------------
//��ǰ����  0 -- ��������  1 -- �������   2 -- �豸����   3 -- ��������  4 -- ͨѶ���ܽ���  5 -- ��ݷ���  6 -- ��ý�����   7 -- ���ý���   8 -- �龰ģʽ


//          12 -- ��Ƶ����
//          13 -- �����ſ�
//          14 -- ��¼��ѯ
//          16 -- �Խ�ͼ�񴰿�
//          21 -- ͨ����¼����
//          22 -- �鿴ͨ����¼����


// 	    152 -- ����������
//          149 -- Digital KB1����
//          150 -- KB1����
//          151 -- LAN��ַ����
// 	    154 -- ��������
// 	    158 -- �汾��Ϣ
// 	    159 -- ϵͳ����
//          165 -- ��Ļ����
//          166 -- ��������
//          167 -- ʱ������
//          168 -- ��������
//          169 -- ��������

// 	    160 -- ��������ѡ��

//���²���Ϊ�ֲ�������
INT32U r2r_edit_Faddr[3] = {0x00164000UL, 0x00165000UL, 0x00166000UL};

INT32U small_num_image_Faddr[10] = {0x0015A000UL, 0x0015B000UL, 0x0015C000UL, 0x0015D000UL, 0x0015E000UL,
                                    0x0015F000UL, 0x00160000UL, 0x00161000UL, 0x00162000UL, 0x00163000UL};

INT32U big_num_image_Faddr[10] = {0x00150000UL, 0x00151000UL, 0x00152000UL, 0x00153000UL, 0x00154000UL,
                                    0x00155000UL, 0x00156000UL, 0x00157000UL, 0x00158000UL, 0x00159000UL};

//������                                    
INT32U talk1_image_Faddr = 0x000f9000UL;
//logo
INT32U logo_image_Faddr = 0x000fc000UL;

//��������
INT32U openlock_image_Faddr = 0x00180000UL;
INT32U openlock_edit_Faddr = 0x00183000UL;
INT32U locking_image_Faddr = 0x00184000UL;
INT32U openlock_result_image_Faddr = 0x00187000UL;

//���Ӵ���
INT32U watch_image_Faddr = 0x00200000UL;

//���������д���
INT32U talk_connect_image_Faddr = 0x00100000UL;
INT32U talk_connect_Faddr[4] = {0x00104000UL, 0x00106000UL, 0x00108000UL, 0x0010A000UL};

//�Խ�ͨ������
INT32U talk_start_image_Faddr = 0x00190000UL;
INT32U talk_start_openlock_Faddr[2] = {0x00193000UL, 0x00194000UL};

//��������
INT32U setup_image_Faddr = 0x00210000UL;
INT32U door_type_image_Faddr[2] = {0x00213000UL, 0x00214000UL};
INT32U net_image_Faddr[2] = {0x00215000UL, 0x00216000UL};
INT32U setup_pass_edit_Faddr = 0x00183000UL;

//����������
INT32U setupmain_image_Faddr = 0x00220000UL;
INT32U setupmain_param_Faddr[6] = {0x00223000UL, 0x00225000UL, 0x00227000UL, 0x00229000UL, 0x0022B000UL, 0x0022D000UL};

//��ַ���ô���
INT32U addr_image_Faddr = 0x00260000UL;;
INT32U addr_edit_Faddr = 0x00263000UL;;
INT32U edit_status_image_Faddr[2] = {0x00264000UL, 0x00265000UL};

//�������ô���
INT32U param_image_Faddr = 0x00240000UL;
INT32U param_line_image_Faddr[2] = {0x00243000UL, 0x00244000UL};
INT32U curr_row_image_Faddr[2] = {0x00245000UL, 0x00246000UL};

//�������ô���
INT32U pass_image_Faddr = 0x00250000UL;
INT32U pass_edit_Faddr = 0x00253000UL;

//���߼�ⴰ��
INT32U onlinetest_image_Faddr = 0x00270000UL;
INT32U online_content_image_Faddr = 0x00273000UL;
INT32U online_edit_Faddr = 0x00275000UL;

//����
INT32U setup_info_image_Faddr = 0x000f9000UL;

void InterfaceInit(void)     //��ʼ��������Դ
{
  INT8U i;

  talk1_image.Faddr = talk1_image_Faddr;
  PreDealImage(&talk1_image);

  logo_image.Faddr = logo_image_Faddr;
  PreDealImage(&logo_image);

  //���������д���
  talk_connect_image.Faddr = talk_connect_image_Faddr;
  PreDealImage(&talk_connect_image);

  for(i=0; i<4; i++)
   {
    talk_connect[i].Faddr = talk_connect_Faddr[i];
    PreDealImage(&talk_connect[i]);
   }

  //ͨ������
  talk_start_image.Faddr = talk_start_image_Faddr;
  PreDealImage(&talk_start_image);

  for(i=0; i<2; i++)
   {
    talk_start_openlock[i].Faddr = talk_start_openlock_Faddr[i];
    PreDealImage(&talk_start_openlock[i]);
   }

  //��������
  openlock_image.Faddr = openlock_image_Faddr;
  PreDealImage(&openlock_image);

  locking_image.Faddr = locking_image_Faddr;
  PreDealImage(&locking_image);

  openlock_result_image.Faddr = openlock_result_image_Faddr;
  PreDealImage(&openlock_result_image);

  //��������
  setup_image.Faddr = setup_image_Faddr;
  PreDealImage(&setup_image);

  for(i=0; i<2; i++)
   {
    door_type_image[i].Faddr = door_type_image_Faddr[i];
    PreDealImage(&door_type_image[i]);
   }

  for(i=0; i<2; i++)
   {
    net_image[i].Faddr = net_image_Faddr[i];
    PreDealImage(&net_image[i]);
   }

  //����������
  setupmain_image.Faddr = setupmain_image_Faddr;
  PreDealImage(&setupmain_image);

  for(i=0; i<6; i++)
   {
    setupmain_param_image[i].Faddr = setupmain_param_Faddr[i];
    PreDealImage(&setupmain_param_image[i]);
   }

  //��ַ���ô���
  addr_image.Faddr = addr_image_Faddr;
  PreDealImage(&addr_image);

  addr_edit[0].Faddr[0] = addr_edit_Faddr;
  PreDealEdit(&addr_edit[0], IMAGEUP);
  addr_edit[0].FontType = 16;
  addr_edit[0].CursorX = 16;
  addr_edit[0].CursorY = 0;//6;//16;
  addr_edit[0].xLeft = 0;
  addr_edit[0].yTop = 0;
  if(addr_edit[0].FontType == 16)
   {
    addr_edit[0].CursorHeight = 12;
    addr_edit[0].fWidth = 8;
    addr_edit[0].Space = 0;
   }
  else
   {
    addr_edit[0].CursorHeight = 24;
    addr_edit[0].fWidth = 16;
    addr_edit[0].Space = 2;
   }
  addr_edit[0].CursorCorlor = COLOR_BLACK;

  addr_edit[0].Text[0] = '\0';
  addr_edit[0].BoxLen = 0;         //�ı���ǰ���볤��
  addr_edit[0].MaxLen = 6;         //�ı���������볤��
  addr_edit[0].Focus = 0;        //��ʾ������
  addr_edit[0].Visible = 0;

  for(i=1; i<6; i++)
    addr_edit[i] = addr_edit[0];

  for(i=0; i<2; i++)
   {
    edit_status_image[i].Faddr = edit_status_image_Faddr[i];
    PreDealImage(&edit_status_image[i]);
   }

  //�������ô���
  param_image.Faddr = param_image_Faddr;
  PreDealImage(&param_image);

  for(i=0; i<2; i++)
   {
    param_line_image[i].Faddr = param_line_image_Faddr[i];
    PreDealImage(&param_line_image[i]);
   }

  for(i=0; i<2; i++)
   {
    curr_row_image[i].Faddr = curr_row_image_Faddr[i];
    PreDealImage(&curr_row_image[i]);
   }

  //�������ô���
  pass_image.Faddr = pass_image_Faddr;
  PreDealImage(&pass_image);

  pass_edit[0].Faddr[0] = pass_edit_Faddr;
  PreDealEdit(&pass_edit[0], IMAGEUP);
  pass_edit[0].FontType = 16;
  pass_edit[0].CursorX = 16;
  pass_edit[0].CursorY = 0;//6;//16;
  pass_edit[0].xLeft = 0;
  pass_edit[0].yTop = 0;
  if(pass_edit[0].FontType == 16)
   {
    pass_edit[0].CursorHeight = 12;
    pass_edit[0].fWidth = 8;
    pass_edit[0].Space = 0;
   }
  else
   {
    pass_edit[0].CursorHeight = 24;
    pass_edit[0].fWidth = 16;
    pass_edit[0].Space = 2;
   }
  pass_edit[0].CursorCorlor = COLOR_BLACK;

  pass_edit[0].Text[0] = '\0';
  pass_edit[0].BoxLen = 0;         //�ı���ǰ���볤��
  pass_edit[0].MaxLen = 6;         //�ı���������볤��
  pass_edit[0].Focus = 0;        //��ʾ������
  pass_edit[0].Visible = 0;

  for(i=1; i<4; i++)
    pass_edit[i] = pass_edit[0];

  //�������ô���
  other_image = param_image;

  //���߼�ⴰ��
  onlinetest_image.Faddr = onlinetest_image_Faddr;
  PreDealImage(&onlinetest_image);

  online_content_image.Faddr = online_content_image_Faddr;
  PreDealImage(&online_content_image);

  online_edit[0].Faddr[0] = online_edit_Faddr;
  PreDealEdit(&online_edit[0], IMAGEUP);
  online_edit[0].FontType = 16;
  online_edit[0].CursorX = 16;
  online_edit[0].CursorY = 0;//6;//16;
  online_edit[0].xLeft = 0;
  online_edit[0].yTop = 0;
  if(online_edit[0].FontType == 16)
   {
    online_edit[0].CursorHeight = 12;
    online_edit[0].fWidth = 8;
    online_edit[0].Space = 0;
   }
  else
   {
    online_edit[0].CursorHeight = 24;
    online_edit[0].fWidth = 16;
    online_edit[0].Space = 2;
   }
  online_edit[0].CursorCorlor = COLOR_BLACK;

  online_edit[0].Text[0] = '\0';
  online_edit[0].BoxLen = 0;         //�ı���ǰ���볤��
  online_edit[0].MaxLen = 6;         //�ı���������볤��
  online_edit[0].Focus = 0;        //��ʾ������
  online_edit[0].Visible = 0;

  for(i=1; i<4; i++)
    online_edit[i] = online_edit[0];

  //����
  setup_info_image.Faddr = setup_info_image_Faddr;
  PreDealImage(&setup_info_image);

  setup_pass_edit.Faddr[0] = openlock_edit_Faddr;
  PreDealEdit(&setup_pass_edit, IMAGEUP);
  setup_pass_edit.FontType = 24;
  setup_pass_edit.CursorX = 16;
  setup_pass_edit.CursorY = 0;//6;//16;
  setup_pass_edit.xLeft = 42;
  setup_pass_edit.yTop = 104;
  if(setup_pass_edit.FontType == 16)
   {
    setup_pass_edit.CursorHeight = 12;
    setup_pass_edit.fWidth = 8;
    setup_pass_edit.Space = 0;
   }
  else
   {
    setup_pass_edit.CursorHeight = 24;
    setup_pass_edit.fWidth = 16;
    setup_pass_edit.Space = 2;
   }
  setup_pass_edit.CursorCorlor = COLOR_BLACK;

  setup_pass_edit.Text[0] = '\0';
  setup_pass_edit.BoxLen = 0;         //�ı���ǰ���볤��
  setup_pass_edit.MaxLen = 6;         //�ı���������볤��
  setup_pass_edit.Focus = 0;        //��ʾ������
  setup_pass_edit.Visible = 0;

  //��������
  for(i=0; i<3; i++)
   {
    r2r_edit[i].Faddr[0] = r2r_edit_Faddr[i];
    PreDealEdit(&r2r_edit[i], IMAGEUP);
   }
  r2r_edit[0].BoxLen = 0;         //�ı���ǰ���볤��
  r2r_edit[0].MaxLen = 4;         //�ı���������볤��
  r2r_edit[0].FontType = 24;
  r2r_edit[0].CursorX = 16;
  r2r_edit[0].CursorY = 0;//4;//16;
  r2r_edit[1].BoxLen = 0;         //�ı���ǰ���볤��
  r2r_edit[1].MaxLen = 2;         //�ı���������볤��
  r2r_edit[2].BoxLen = 0;         //�ı���ǰ���볤��
  r2r_edit[2].MaxLen = 4;         //�ı���������볤��

  for(i=0; i<10; i++)
   {
    small_num_image[i].Faddr = small_num_image_Faddr[i];
    PreDealImage(&small_num_image[i]);
   }

  for(i=0; i<10; i++)
   {
    big_num_image[i].Faddr = big_num_image_Faddr[i];
    PreDealImage(&big_num_image[i]);
   }

  openlock_edit = setup_pass_edit;
  openlock_edit.BoxLen = 0;         //�ı���ǰ���볤��
  openlock_edit.MaxLen = 6;         //�ı���������볤��
  openlock_edit.FontType = 24;
  openlock_edit.CursorX = 16;
  openlock_edit.CursorY = 0;//6;//16;
  openlock_edit.xLeft = 42;
  openlock_edit.yTop = 94;  

  //���Ӵ���
  watch_image.Faddr = watch_image_Faddr;
  PreDealImage(&watch_image);


  soundtip_wav.Faddr =      0x0075A000UL;
  ring_wav.Faddr =          0x00700000UL;//0x0076C000UL;//0x00700000UL;//0x0076C000UL;

  modisucc_wav[0].Faddr =      0x0070D000UL;
  modisucc_wav[1].Faddr =      0x00710000UL;
  
  modifail_wav[0].Faddr =      0x00714000UL;
  modifail_wav[1].Faddr =      0x00717000UL;

  passerror_wav[0].Faddr =     0x0071B000UL;
  passerror_wav[1].Faddr =     0x0071E000UL;

  inputerror_wav[0].Faddr =    0x00721000UL;
  inputerror_wav[1].Faddr =    0x00725000UL;

  backring_wav[0].Faddr =          0x00728000UL;
  backring_wav[1].Faddr =          0x00735000UL;
  backring_wav[2].Faddr =          0x00744000UL;

  dooropen_wav[0].Faddr = 0x0076F000UL;//0x0075B000UL;       //20130407 ��Ϊ����

  test1k_wav.Faddr = 0x0075D000UL;

  doordect_wav.Faddr = 0x00767000UL;

  card_ok_wav.Faddr = 0x0076F000UL;
  card_fail_wav.Faddr = 0x00771000UL;

  //20140225
  	DelayTalkTimeOut[0] = 15;
	DelayTalkTimeOut[1] = 30;
	DelayTalkTimeOut[2] = 60;
	DelayTalkTimeOut[3] = 99;
}
//---------------------------------------------------------------------------
//��lib ����
INT32U GetLocal_fb_addr(void)
{
  return Local.fb_addr;
}
//---------------------------------------------------------------------------
INT8U GetLocal_fb_page(void)
{
  return Local.fb_page;
}
//---------------------------------------------------------------------------
INT8U *Get_jpeg_bitstream(void)
{
  return jpeg_bitstream;
}
//---------------------------------------------------------------------------
INT8U Get_Mjpeg_DecodeError(void)
{
  return Mjpeg_DecodeError;
}
//---------------------------------------------------------------------------
void Set_Local_DecodeJpegFlag(INT8U disp_DecodeJpegFlag)
{
  Local.DecodeJpegFlag = disp_DecodeJpegFlag;
}
//---------------------------------------------------------------------------
INT8U Get_Local_DecodeStreamFlag(void)
{
  return Local.DecodeStreamFlag;
}
//---------------------------------------------------------------------------
void PlaySoundTip(void) //������ʾ��
{
  if(LocalCfg.VoiceHint == 1)
   {
    WaitAudioUnuse(200); //�ȴ���������               //  wav_1k[5].Faddr
    StartPlayWav(soundtip_wav.Faddr, NO_CYCLE_PLAY);       // soundtip_wav.Faddr
   }
}
//---------------------------------------------------------------------------
