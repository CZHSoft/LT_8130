#include "includes.h"

#include "stdlib.h"

#include "main.h"

#define x_video_h

#define CommonH
#include "common.h"

/*
****************************************
20131119
�����޸�
****************************************
*/

#define _VINT_ENABLE  1       //VINT ʹ��, �ɱ���ͼ����ٱ仯ʱ����������

void InitVideo(void);    //20120203 xu

void interrupt MJPEGInt(void);

INT8U Mjpeg_DecodeError = 0;
void interrupt MJPEGDecInt(void);
void interrupt VInt(void);
INT16U bs_num_last;
INT16U bs_addr;

INT32U video_timestamp;     
INT16U video_Frameno;
void CreateVideoSend_Stream(void);
void send_video_data(INT32U video_addr, INT16U length);

INT8U video_encode_flag = 0;
INT32U video_encode_addr[2];
INT16U video_encode_length[2];
INT8U video_encode_no;
void CheckVideoEncodeBuf(void);


INT16U bsj_length;
INT8U have_mjpeg_stream;
INT8U dec_sa_sw;

INT8U  CaptureYUV = 0;

//��ʾ��Ƶ���ڡ�cType -- 0  ����   1 -- Զ��
void DisplayVideoWindow(INT16U xLeft,INT16U width,INT16U yTop,INT16U height, INT8U cType);
void CloseVideoWindow(void);

void StartRecVideo(INT8U Type);     //����Ƶ����
void StopRecVideo(void);            //�ر���Ƶ����

void StartEncodeVideo(INT8U Type);     //����Ƶ����
void StopEncodeVideo(void);            //�ر���Ƶ����

void StartVideoClock(void);     //����Ƶ�ɼ�����ʱ��
void StopVideoClock(void);      //�ر���Ƶ�ɼ�����ʱ��

void StartVideoStreamDecode(void);     //����Ƶ������
void StopVideoStreamDecode(void);      //�ر���Ƶ������

INT8U Have_Video_Stream;
INT8U DecodeBuffNo; //���뻺����
INT8U Have_Decode_No;
INT8U CurrBitStreamNo, OldCurrBitStreamNo;
void DecoceVideo(INT32U Video_Maddr, INT32U Video_Len);

void videow(INT16U Hstart,INT16U Hsize,INT16U Vstart,INT16U Vsize);
void clear_video_ram(void);  //����Ƶ����

INT8U VideoFrameRate[6] = {1, 5, 10, 15, 20, 25};

extern INT8U CloseDispClockFlag;
INT16U  Mjpeg_Count = 0;
//---------------------------------------------------------------------------
void videow(INT16U Hstart,INT16U Hsize,INT16U Vstart,INT16U Vsize)
{
	outport(WH_START_L, Hstart);//ˮƽ��ʼ 
	outport(WH_SIZE_L, Hsize);//ˮƽ����
	outport(WV_START, Vstart);
	outport(WV_SIZE, Vsize);
}
//---------------------------------------------------------------------------
void clear_video_ram(void)
{
	INT16U i,j;
	INT16U color_val, bcolor_val;
	INT32U addr;
	addr=0UL;
	outportb(FONT_WR_MOST_ADD, addr>>16);
	outportb(FONT_WR_HIGH_ADD, addr>>8);
	outportb(FONT_WR_LOW_ADD, addr);
	color_val = 0x8080;
	bcolor_val = 0x8080;
	outportb(FGND_LOW, color_val);
	outportb(FGND_HIGH, (color_val>>8));
	outportb(BGND_LOW, bcolor_val);
	outportb(BGND_HIGH, (bcolor_val>>8));
	for(i=0;i<1024;i++)
	 {	 
	 	 for(j=0;j<128;j++)
	 	  {	 	
	 	  	outportb(FONT_WR_DATA, 0xff);
	 	  }
	 }
}
//---------------------------------------------------------------------------
void InitVideo(void)
{
  INT16U val;

     	setvect(16, MJPEGInt);
	outportb(TX_START, inportb(TX_START)|0x20);//ʹ��MJPEG�ж�
	setvect(17, MJPEGDecInt);
       	outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x04);//ʹ��MJPEG�����ж�
      #if _VINT_ENABLE    //VINT ʹ��
	setvect(18, VInt);
        outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x20);//ʹ��V�ж�
      #else
        outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xdf);//��ֹV�ж�
      #endif


 outportb(INT_MASK_H, 0xe0);//�������жϹرգ�������ж�ʹ�� 1������ 0:ʹ��         20111209 jpegdec  0xf2 -> 0xf0     20120203 jpegdec  0xf0 -> 0xe0
 outportb(INT_EDGE_H, 0x00);//���������ش���

	bs_num_last=0;
	bs_addr=0;

        Have_Video_Stream = 0;
        DecodeBuffNo = 0;
        Have_Decode_No = 0;
        Local.DecodeVideoWidth = 640;
        Local.DecodeVideoHeight = 480;
        CurrBitStreamNo = 0;

	have_mjpeg_stream=0;
	dec_sa_sw=0;
        Local.Test_OV7725FrameRate = 1;  //0 - 30   1 - 25
      #ifdef _CAMERA_SUPPORT     //�粻������ͷ��GPIOB6��7ΪComm1, GPIOC ��Ϊ8������ʹ��
  	val = init_camera(Local.Test_OV7725FrameRate);
        Local.Camera = 0;
        Local.VideoType = 0x00;
        if(val == 0x7725)
         {
          xtm_printf("init ov7725\n");
          Local.Camera = 1;
          if((Local.DecodeVideoWidth == 640)&&(Local.DecodeVideoHeight == 480))
            Local.VideoType = 0x40;
          else
            Local.VideoType = 0x08;
         }
        else
          xtm_printf("no 7725\n");
      #else
        Local.Camera = 0;
      #endif
        Local.DecodeStreamFlag = 0;  //���ڽ�����
        Local.DecodeJpegFlag = 0;  //���ڽ⾲̬ͼƬ
  //StopRecVideo();         //�ر���Ƶ����
}
//---------------------------------------------------------------------------
//��ʾ��Ƶ���ڡ�cType -- 0  ����   1 -- Զ��
void DisplayVideoWindow(INT16U xLeft,INT16U width,INT16U yTop,INT16U height, INT8U cType)
{
  outport(WH_START_L, xLeft);//ˮƽ��ʼ
  outport(WH_SIZE_L, width);//ˮƽ����
  outport(WV_START, yTop);
  outport(WV_SIZE, height);
  
  if(cType == LOCAL_VIDEO)
    outportb(VW_CONTROL, 0x00);
  if(cType == REMOTE_VIDEO)
   {
    decode_dqt_init();
    
    outportb(VW_CONTROL, 0x03);
    //outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x20);//ʹ��V�ж�
   }

}
//---------------------------------------------------------------------------
void CloseVideoWindow(void)
{
//  outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xdf);//��ֹV�ж�
  //���õ���ǰ��ʾ����֮��
  outport(WH_START_L, SCREEN_WIDTH + 8);//ˮƽ��ʼ
  outport(WH_SIZE_L, 8);//ˮƽ����
  outport(WV_START, SCREEN_HEIGHT + 8);
  outport(WV_SIZE, 8);

  outportb(VW_CONTROL, 0x00);

  Local.ClearVideoWindowFlag = 1;    //����Ƶ���ڱ�־
}
//---------------------------------------------------------------------------
void StartRecVideo(INT8U Type)   //����Ƶ����    0 - VGA  1 - QVGA
{
  xtm_printf("StartRecVideo::LocalCfg.VideoClosed = %d, Local.Camera = %d\n", LocalCfg.VideoClosed, Local.Camera);
  Type = VIDEO_VGA;
  if(!LocalCfg.VideoClosed) //��Ƶ�ر�
    return;
    
  if(Local.Camera != 0)
   {
    CreateVideoSend_Stream();
    CaptureYUV = 1;

    video_timestamp = 0;
    video_Frameno = 0;
    if(Type == VIDEO_QVGA)
     {
      outportb(VI_CONTROL, 0x43);
      outport(VW_WIDTH_L, 512);//����Ϊ512
    if(Type == VIDEO_VGA)
     {
      outportb(VI_CONTROL, 0x11);
      outport(VW_WIDTH_L, 640);
     }
    xtm_printf("StartRecVideo::1 VI_CONTROL2 = 0x%X\n", inportb(VI_CONTROL2));
    outportb(VI_CONTROL2, 0x4);    //��Ƶ������ƼĴ���2
    xtm_printf("StartRecVideo::2 VI_CONTROL2 = 0x%X\n", inportb(VI_CONTROL2));
   }
}
//---------------------------------------------------------------------------
void StopRecVideo(void)         //�ر���Ƶ����
{
  if(Local.Camera != 0)
   {
    outportb(VI_CONTROL, 0x00);
   }
}
//---------------------------------------------------------------------------
void StartEncodeVideo(INT8U Type)     //����Ƶ����     0 - VGA  1 - QVGA
{
  if(!LocalCfg.VideoClosed) //��Ƶ�ر�
    return;
  Type = VIDEO_VGA;
  xtm_printf("StartEncodeVideo::TX_START = 0x%X, Local.Camera = %d\n", inportb(TX_START), Local.Camera);
  Mjpeg_Count = 0;
  if(Local.Camera != 0)
   {
    if((inportb(EN_J_STATUS)&0x01)==0x01)
      xtm_printf("StartEncodeVideo::EN_J_STATUS = 0x%X\n", inportb(EN_J_STATUS));

    setvect(16, MJPEGInt);
    outportb(TX_START, inportb(TX_START)|0x20);//ʹ��MJPEG�ж�

//  outportb(EN_J_CONTROL2, 0x5e);//ԭͼ��30֡ÿ�룬����1֡ÿ��     0101 1110
//  outportb(EN_J_CONTROL2, 0x4A);//ԭͼ��30֡ÿ�룬����15֡ÿ��     0100 1111
//  outportb(EN_J_CONTROL2, 0x0f);//ԭͼ��30֡ÿ�룬����15֡ÿ��     0001 1001      0x19              VideoFrameRate
    LocalCfg.EncodeFrameRate = 5;
    xtm_printf("LocalCfg.EncodeFrameRate = %d, %d, Type = %d\n", LocalCfg.EncodeFrameRate, VideoFrameRate[LocalCfg.EncodeFrameRate], Type);
    outportb(EN_J_CONTROL2, VideoFrameRate[LocalCfg.EncodeFrameRate]);//ԭͼ��30֡ÿ�룬����15֡ÿ��     0001 1001      0x19
    //outportb(EN_J_CONTROL2, 0x40);      //30֡ �Զ�֡��
    //outportb(EN_J_CONTROL2, 0x5e);

    if(Type == VIDEO_QVGA)
     {
      outportb(EN_J_MBWIDTH, 0x14);
      outportb(EN_J_MBHEIGHT, 0x0F);
     }
    if(Type == VIDEO_VGA)
     {
      outportb(EN_J_MBWIDTH, 0x28);
      outportb(EN_J_MBHEIGHT, 0x1e);
     }

     outportb(EN_J_CONTROL, inportb(EN_J_CONTROL)|0x17); //0000 0111     0x07
    }    
}
//---------------------------------------------------------------------------
void StopEncodeVideo(void)            //�ر���Ƶ����
{
  INT8U tmp;
  outportb(TX_START, inportb(TX_START)&0xdf);//��ֹMJPEG�ж�
  xtm_printf("StopEncodeVideo1::TX_START = 0x%X\n", inportb(TX_START));
  tmp = 0;
  while((inportb(EN_J_STATUS)&0x01)==0x01)
   {
    xtm_printf("StopEncodeVideo::wait EN_J_STATUS finish\n");
    tmp ++;
    if(tmp > 20)
      break;
   }
  outportb(EN_J_CONTROL, inportb(EN_J_CONTROL)&0xfe);
  xtm_printf("StopEncodeVideo2::TX_START = 0x%X\n", inportb(TX_START));
}
//---------------------------------------------------------------------------
void StartVideoClock(void)     //����Ƶ�ɼ�����ʱ��
{
#if 1
  Set_VideoCapture_Clock(1);  //������Ƶ�ɼ�ʱ��  1  ��   0  ��
  Set_Encode_Clock(1); //������Ƶ����ʱ��  1  ��   0  ��
  encode_dqt_init();
#endif  
}
//---------------------------------------------------------------------------
void StopVideoClock(void)            //�ر���Ƶ�ɼ�����ʱ��
{
#if 1
  INT8U tmp;
  //while((inportb(VI_STATUS)&0x01)==0x00)
  //  xtm_printf("StopVideoClock::wait VI_STATUS finish\n");
  tmp = 0;
/*  while((inportb(EN_J_STATUS)&0x01)==0x01)
   {
    xtm_printf("StopEncodeVideo::wait EN_J_STATUS finish\n");
    tmp ++;
    if(tmp > 20)
      break;
   }    */
  Set_Encode_Clock(0); //������Ƶ����ʱ��  1  ��   0  ��
  Set_VideoCapture_Clock(0);  //������Ƶ�ɼ�ʱ��  1  ��   0  ��
#endif
}
//---------------------------------------------------------------------------
void StartVideoStreamDecode(void)     //����Ƶ������
{
  Local.DecodeStreamFlag = 1;  //���ڽ�����
  outportb(CONTROL_REG2, inportb(CONTROL_REG2) | 0x02);      //����ʾ�� ��һ���ִ����������, ��������������� BitBlk ����
  Set_Decode_Clock(1); //������Ƶ����ʱ��  1  ��   0  ��
  decode_dqt_init();
}
//---------------------------------------------------------------------------
void StopVideoStreamDecode(void)      //�ر���Ƶ������
{
  Set_Decode_Clock(0); //������Ƶ����ʱ��  1  ��   0  ��
  Local.DecodeStreamFlag = 0;  //���ڽ�����
  outportb(CONTROL_REG2, inportb(CONTROL_REG2) & 0xfd);    //��Ƶ������ƼĴ���2
}
//---------------------------------------------------------------------------
void interrupt MJPEGInt()
{

	INT16U i, j, k, l;
	INT8U bs_counter, len_overflow, most_addr;
	INT16U bs_length;
	INT8U picture_counter;
	INT16U bs_addr_temp;
        INT32U bs_addr_h;

	outportb(TX_START, inportb(TX_START)&0xdf);//���MJPEG�ж�

        Mjpeg_Count ++;
        if((Mjpeg_Count % 100) == 0)
          xtm_printf("MJPEGInt::Mjpeg_Count = %d\n", Mjpeg_Count);

	k=inport(EN_J_TX_BS_PACK_L);
	//asm sti

	bs_counter=k-bs_num_last;
	bs_counter=bs_counter&0xf;

        //xtm_printf("MJPEGInt 1::EN_J_CONTROL = 0x%X, Local.Test_EncodeWatchFlag = %d\n", inportb(EN_J_CONTROL), Local.Test_EncodeWatchFlag);
        //xtm_printf("MJPEGInt 1::EN_J_CONTROL2 = 0x%X\n", inportb(EN_J_CONTROL2));
	k=bs_num_last;

	for(j=0;j<bs_counter;j++){


                bs_addr = 0;

                if((inportb(EN_J_STATUS)&0x04)==0x04)
                 {
                        bs_addr_h = 0x170000;
                        bs_addr = 0;
                        video_encode_no = 0;

                        most_addr = 0x17;
                      //  outportb(BYTE_RD_MOST_ADD, 0x17);
                 }	
                else
                 {
                 	bs_addr_h = 0x180000;
                 	bs_addr = 0x0;
                 	most_addr = 0x18;
                 	video_encode_no = 1;
                 }
               /* len_overflow = 0;
                if((inportb(EN_J_STATUS)&0x08) == 0x08)
                 {
                  len_overflow = 1;
                  xtm_printf("MJPEGInt::1111 len_overflow \n");
                 }        */
                //xtm_printf("MJPEGInt::1111\n");
                outportb(BYTE_RD_MOST_ADD, most_addr);
                outportb(BYTE_RD_HIGH_ADD, bs_addr>>8);
                outportb(BYTE_RD_LOW_ADD, bs_addr);
                bs_length=inportb(BYTE_RD_DATA);
                bs_length=bs_length+(inportb(BYTE_RD_DATA)<<8)-16;

                inportb(BYTE_RD_DATA);
                inportb(BYTE_RD_DATA);
                inportb(BYTE_RD_DATA);
                inportb(BYTE_RD_DATA);
                len_overflow = inportb(BYTE_RD_DATA);

                if(len_overflow & 0x01)
                  xtm_printf("MJPEGInt:: len_overflow = 0x%X\n", len_overflow);
                else
                 {
                    if(Local.Test_EncodeWatchFlag == 1)
                     {
                      //xtm_printf("MJPEGInt::bs_length = 0x%X\n", bs_length);
                      if(bs_length > 0xff00)
                        xtm_printf("MJPEGInt1::bs_length = 0x%X\n", bs_length);
                      else
                       {
                        bitstream_image[CurrBitStreamNo].Len = bs_length;
                        bitstream_image[CurrBitStreamNo].Maddr = bs_addr_h + bs_addr + 16;

                        OldCurrBitStreamNo = CurrBitStreamNo;
                        DecoceVideo(bitstream_image[CurrBitStreamNo].Maddr, bs_length);

                      //xtm_printf("MJPEGInt::bs_length = %d, bs_addr = 0x%X, bs_counter = %d, 0x%X %X\n", bs_length, bs_addr, bs_counter, \
                      //     (INT16U)((bs_addr_h & 0xffff0000) >> 16), (INT16U)(bs_addr_h & 0xffff));
                        CurrBitStreamNo ++;
                        if(CurrBitStreamNo > 2)
                          CurrBitStreamNo = 0;
                       }
                     }
                    else
                     {
                      //xtm_printf("MJPEGInt2::bs_length = 0x%X\n", bs_length);
                      if(bs_length > 0xff00)
                        xtm_printf("MJPEGInt2::bs_length = 0x%X\n", bs_length);
                      else
                      #if 1
                       if(video_encode_flag == 0)
                        {
                         video_encode_addr[video_encode_no] = bs_addr_h + bs_addr + 16;
                         video_encode_length[video_encode_no] = bs_length;
                         video_encode_flag = 1;
                        }
                      #else
                       send_video_data(bs_addr_h + bs_addr + 16, bs_length);
                      #endif
                     }
		k++;
               }

	}
	bs_num_last=k;

	outportb(TX_START, inportb(TX_START)|0x20);//ʹ��MJPEG�ж�
        //xtm_printf("MJPEGInt 222\n");
        //DisplayImage(&main_image);
}

void CreateVideoSend_Stream(void)      //��Ƶ����Stream
{
    INT16 i, DestPort;
    struct talkdata1 talkdata;

    DestPort = LocalPort[1];
    //==========
    //MAC header
    //==========
    /* 0 -- 5  Ŀ��MAC��ַ*/
    xtm_memcopy(send_mac_buf2 + 6, LocalCfg.Mac_Addr, 6);
    send_mac_buf2[12] = 0x08;
    send_mac_buf2[13] = 0x00;

    //=========
    //IP header
    //=========
    send_mac_buf2[14] = 0x45;          //Ver HLEN
    send_mac_buf2[15] = 0x00;
    i = 86 + PACKDATALEN + 28;//nlength+28;
    send_mac_buf2[16] = i>>8;
    send_mac_buf2[17] = i&0xff;
    /* 18 -- 19  IP�ײ���ʶ*/

    send_mac_buf2[20] = 0x00;
    send_mac_buf2[21] = 0x00;
    send_mac_buf2[22] = 0x40;
    send_mac_buf2[23] = 0x11;
    send_mac_buf2[24] = 0x00;       //checksum
    send_mac_buf2[25] = 0x00;
    xtm_memcopy(send_mac_buf2 + 26, LocalCfg.IP, 4);     //Source IP
    /* 30 -- 33  Dest IP*/
    send_mac_buf2[34] = (DestPort >> 8)& 0xff;
    send_mac_buf2[35] = DestPort & 0xff;
    send_mac_buf2[36] = (DestPort >> 8)& 0xff;
    send_mac_buf2[37] = DestPort & 0xff;

    i = 86 + PACKDATALEN + 8;//(nlength+8);
    send_mac_buf2[38] = i>>8;
    send_mac_buf2[39] = i&0xff;
    send_mac_buf2[40] = 0x00;
    send_mac_buf2[41] = 0x00;

    //ͷ��
    xtm_memcopy(VideoSend_Stream, UdpPackageHead, 6);
    //����
    VideoSend_Stream[6] = VIDEOTALK;
    if((Local.Status == 3)||(Local.Status == 4))
      VideoSend_Stream[6] = VIDEOWATCH;

    VideoSend_Stream[7]= ASK;        //����
    if((Local.Status == 1)||(Local.Status == 3)||(Local.Status == 5))  //����Ϊ���з�
     {
      VideoSend_Stream[8] = CALLUP;
      xtm_memcopy(talkdata.HostAddr, LocalCfg.Addr, 20);
      xtm_memcopy(talkdata.HostIP, LocalCfg.IP, 4);
      xtm_memcopy(talkdata.AssiAddr, Remote.Addr[0], 20);
      xtm_memcopy(talkdata.AssiIP, Remote.IP[0], 4);
     }
    if((Local.Status == 2)||(Local.Status == 4)||(Local.Status == 6))  //����Ϊ���з�
     {
      VideoSend_Stream[8] = CALLDOWN;
      xtm_memcopy(talkdata.HostAddr, Remote.Addr[0], 20);
      xtm_memcopy(talkdata.HostIP, Remote.IP[0], 4);
      xtm_memcopy(talkdata.AssiAddr, LocalCfg.Addr, 20);
      xtm_memcopy(talkdata.AssiIP, LocalCfg.IP, 4);
     }

    //ʱ���
    talkdata.timestamp = 0;
      //��������
    talkdata.DataType = 2;
    //֡���
    talkdata.Frameno = 0;
    //֡���ݳ���
    talkdata.Framelen = 0;
    //�ܰ���
    talkdata.TotalPackage = 1;
    //��ǰ��
    talkdata.CurrPackage = 1;
    //���ݳ���
    talkdata.Datalen = 0;
    talkdata.PackLen = PACKDATALEN;
    xtm_memcopy(VideoSend_Stream + 9, &talkdata, sizeof(talkdata));

    //xtm_printf("CreateVideoSend_Stream::Local.Status = %d, Remote.DenIP = %d.%d.%d.%d, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, VideoSend_Stream[8] = 0x%X\n", \
    //       Local.Status, Remote.DenIP[0], Remote.DenIP[1], Remote.DenIP[2], Remote.DenIP[3], \
    //       VideoSend_Stream[0], VideoSend_Stream[1], VideoSend_Stream[2], VideoSend_Stream[3], VideoSend_Stream[4], VideoSend_Stream[5], VideoSend_Stream[6], VideoSend_Stream[7], VideoSend_Stream[8]);
}
//---------------------------------------------------------------------------
void CheckVideoEncodeBuf(void)
{
  if(video_encode_flag == 1)
   {
     video_encode_flag = 0;
     send_video_data(video_encode_addr[video_encode_no], video_encode_length[video_encode_no]);
   }
}
//---------------------------------------------------------------------------
void send_video_data(INT32U video_addr, INT16U v_length)
{
  INT16U  nlength, Datalen;

   //xtm_printf("send_video_data::Remote.DenMac = %X:%X:%X:%X:%X:%X\n", Remote.DenMac[0], Remote.DenMac[1], Remote.DenMac[2], Remote.DenMac[3], Remote.DenMac[4], Remote.DenMac[5]);
  //if(SearchArp_Table(Remote.DenIP, RemoteMac))
   {
  INT16U i, j, TotalPackage;

/*  Remote.DenNum = 2;
  Remote.IP[1][0] = 192;
  Remote.IP[1][1] = 168;
  Remote.IP[1][2] = 68;
  Remote.IP[1][3] = 98;
  Remote.Mac[1][0] = 0x00;
  Remote.Mac[1][1] = 0x26;
  Remote.Mac[1][2] = 0x18;
  Remote.Mac[1][3] = 0x77;
  Remote.Mac[1][4] = 0x4C;
  Remote.Mac[1][5] = 0x44;  */

  nlength = v_length + 86;
  //UDP ���ݱ���Ϊż���ֽ�
  if((nlength % 2) == 1)
    nlength ++;

  if(nlength < SMALLESTSIZE)
    nlength = SMALLESTSIZE;

  send_mac_buf2[18]= (ip_pack_no >> 8) & 0xff;
  send_mac_buf2[19]= ip_pack_no & 0xff;   //IP�ײ���ʶ

    //ʱ���
    xtm_memcopy(VideoSend_Stream + 57, &video_timestamp, sizeof(INT32U));
    //��������
    VideoSend_Stream[61] = 2;
    VideoSend_Stream[62] = 0;
    //֡���
    xtm_memcopy(VideoSend_Stream + 63, &video_Frameno, sizeof(INT16U));
    //֡���ݳ���
    VideoSend_Stream[65] = v_length & 0xff;
    VideoSend_Stream[66] = v_length >> 8;
    VideoSend_Stream[67] = 0;
    VideoSend_Stream[68] = 0;

    //�ܰ���
    if((v_length % PACKDATALEN)==0)
      TotalPackage = v_length/PACKDATALEN;
    else
      TotalPackage = v_length/PACKDATALEN + 1;    
    VideoSend_Stream[69] = TotalPackage;
    VideoSend_Stream[70] = 0;
    //��ǰ��
    VideoSend_Stream[71] = 1;
    VideoSend_Stream[72] = 0;

    xtm_memcopy(send_mac_buf2 + 42, VideoSend_Stream, 86);

    for(i=1; i<=TotalPackage; i++)
     {
        //��ǰ��
        send_mac_buf2[113] = i;
        send_mac_buf2[114] = 0;
        if(i==TotalPackage)
         {
          ///���ݳ���
          Datalen = v_length-(i-1)*PACKDATALEN;
          send_mac_buf2[115] = Datalen & 0xff;
          send_mac_buf2[116] = Datalen >> 8;

          //xtm_printf("send_rec_audio_data::PCMI_INT_Flag = %d\n", PCMI_INT_Flag);
          Video_Send_Flag = 1;
          for(j=0; j<Remote.DenNum; j++)
           {
            xtm_memcopy(send_mac_buf2, Remote.Mac[j], 6);
            xtm_memcopy(send_mac_buf2 + 30, Remote.IP[j], 4);
            Build_Udp_Ip_Crc(send_mac_buf2 + 14);
            if((Local.Status == 1)||(Local.Status == 3)||(Local.Status == 5))  //����Ϊ���з�
             {
              xtm_memcopy(send_mac_buf2 + 42 + 33, Remote.Addr[j], 20);
              xtm_memcopy(send_mac_buf2 + 42 + 53, Remote.IP[j], 4);
             }
            if((Local.Status == 2)||(Local.Status == 4)||(Local.Status == 6))  //����Ϊ���з�
             {
              xtm_memcopy(send_mac_buf2 + 42 + 9, Remote.Addr[j], 20);
              xtm_memcopy(send_mac_buf2 + 42 + 29, Remote.IP[j], 4);
             }
            send_mac_video_packet(128, send_mac_buf2, PACKDATALEN, video_addr + (i-1)*PACKDATALEN);
           }
          Video_Send_Flag = 0;
         }
        else
         {
          ///���ݳ���
          send_mac_buf2[115] = PACKDATALEN & 0xff;
          send_mac_buf2[116] = PACKDATALEN >> 8;
          //xtm_printf("send_rec_audio_data::PCMI_INT_Flag = %d\n", PCMI_INT_Flag);
          Video_Send_Flag = 1;
          for(j=0; j<Remote.DenNum; j++)
           {
            xtm_memcopy(send_mac_buf2, Remote.Mac[j], 6);
            xtm_memcopy(send_mac_buf2 + 30, Remote.IP[j], 4);
            Build_Udp_Ip_Crc(send_mac_buf2 + 14);
            //xtm_printf("%d.%d.%d.%d, %X:%X:%X:%X:%X:%X\n", Remote.IP[j][0], Remote.IP[j][1], Remote.IP[j][2], Remote.IP[j][3], \
            //            Remote.Mac[j][0], Remote.Mac[j][1], Remote.Mac[j][2], Remote.Mac[j][3], Remote.Mac[j][4], Remote.Mac[j][5]);
            if((Local.Status == 1)||(Local.Status == 3)||(Local.Status == 5))  //����Ϊ���з�
             {
              xtm_memcopy(send_mac_buf2 + 42 + 33, Remote.Addr[j], 20);
              xtm_memcopy(send_mac_buf2 + 42 + 53, Remote.IP[j], 4);
             }
            if((Local.Status == 2)||(Local.Status == 4)||(Local.Status == 6))  //����Ϊ���з�
             {
              xtm_memcopy(send_mac_buf2 + 42 + 9, Remote.Addr[j], 20);
              xtm_memcopy(send_mac_buf2 + 42 + 29, Remote.IP[j], 4);
             }            
            send_mac_video_packet(128, send_mac_buf2, PACKDATALEN, video_addr + (i-1)*PACKDATALEN);
           }
          Video_Send_Flag = 0;
         }
     }

    //xtm_printf("send_rec_audio_data::Remote.DenIP = %d.%d.%d.%d, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, VideoSend_Stream[8] = 0x%X\n", \
    //       Remote.DenIP[0], Remote.DenIP[1], Remote.DenIP[2], Remote.DenIP[3], \
    //       VideoSend_Stream[0], VideoSend_Stream[1], VideoSend_Stream[2], VideoSend_Stream[3], VideoSend_Stream[4], VideoSend_Stream[5], VideoSend_Stream[6], VideoSend_Stream[7], VideoSend_Stream[8]);

    video_timestamp += 40;      //����Ƶ����Stream
    video_Frameno ++;
   }
}
/*INT16U VideoStamp = 0;
INT16U OldVideoStamp = 0;
      VideoStamp = (INT16U)((SWTIMER_Tick() * 5) & 0x7fff);
      xtm_printf("dd = %d\n", VideoStamp - OldVideoStamp);
      OldVideoStamp = VideoStamp;        */
void interrupt VInt()
{
#if _VINT_ENABLE    //VINT ʹ��
  //20120229
  INT32U tmp_data;
  //xtm_printf("VInt\n");
#if 0
  if(CloseDispClockFlag)
   {
    CloseDispClockFlag = 0;
    outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xdf);//��ֹV�ж�

    outportb(CONTROL_REG, inportb(CONTROL_REG)&0xfe);  //1 1111 110  ��ʾ��ֹ    0xfe
    Set_Disp_Clock(0); //������ʾʱ��  1  ��   0  ��
    return;
   }
#endif   
  outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xdf);//���V�ж�

  if(Have_Video_Stream)
   {
    //xtm_printf("VInt::Have_Video_Stream= %d, 0x%X\n", Have_Video_Stream, inportb(DE_J_STATUS));

  //  if((inportb(DE_J_STATUS)&0x01)!=0x01)
     {
      outportb(VW_CONTROL, 0x03);
      outport(VW_WIDTH_L, Local.DecodeVideoWidth);

      //xtm_printf("VInt::\n");

      if(Have_Decode_No)
        tmp_data = (INT32U)Local.DecodeVideoWidth*Local.DecodeVideoHeight*2;
      else
        tmp_data = 0;

      outportb(VW_SA_H, tmp_data>>16);
      outportb(VW_SA_M, tmp_data>>8);
      outportb(VW_SA_L, tmp_data);
     }
    Have_Video_Stream = 0;
    //xtm_printf("VInt2::Have_Video_Stream= %d\n", Have_Video_Stream);
   }
  outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x20);//ʹ��V�ж�
#endif
}
//extern INT8U DeocdeFinishFlag;
void interrupt MJPEGDecInt()
{
        INT8U tmp_status;
        INT16U i, j, val, tmp_pos;
        INT32U tmp_data;
        Mjpeg_DecodeError = 0;
        tmp_status = inportb(DE_J_STATUS);
        outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xfb);//���MJPEG�����ж�
        //xtm_printf("1\n");
        if((tmp_status & 0x02) != 0x02)
          xtm_printf("MJPEGDecInt: DE_J_STATUS = 0x%X\n", tmp_status);

	if((tmp_status&0x04)==0x04){
                Mjpeg_DecodeError = 1;
              if((Local.Test_EncodeWatchFlag == 0)&&(Local.Status != 0))
                xtm_printf("%d\n", video_decode_no);
                //xtm_printf("MJPEGDecInt:: decode error ,,,,,,,,,,,%d, Mjpeg_DecodeError = %d, %d, %d, old_mjpeg_stream_no = %d, len = 0x%X, encode 0 = 0x%X, 1 = 0x%X\n",  \
                   //video_decode_no, Mjpeg_DecodeError, mjpeg_stream[old_mjpeg_stream_no][0], mjpeg_stream[old_mjpeg_stream_no][1], old_mjpeg_stream_no, mjpeg_stream_len[old_mjpeg_stream_no], video_encode_length[0], video_encode_length[1]);
              else
                xtm_printf("MJPEGDecInt::decode error  bitstream_image[%d].Maddr = 0x%X %X, bs_length = 0x%X\n", \
                          OldCurrBitStreamNo, (INT16U)((bitstream_image[OldCurrBitStreamNo].Maddr & 0xffff0000) >> 16), (INT16U)(bitstream_image[OldCurrBitStreamNo].Maddr & 0xffff), bitstream_image[OldCurrBitStreamNo].Len);
              error_image.Maddr = bitstream_image[OldCurrBitStreamNo].Maddr;
              error_image.Len = bitstream_image[OldCurrBitStreamNo].Len;
              outportb(CONTROL_REG, inportb(CONTROL_REG)&0x7f);//MJPEG��������
              outportb(CONTROL_REG, inportb(CONTROL_REG)|0x80);//MJPEG���븴ԭ
              Have_Video_Stream = 0;

              //xtm_printf("1111111111111111111111MJPEGDecInt: DE_J_STATUS = 0x%X, 0xfb1e = 0x%X, 0xfb1f = 0x%X\n", tmp_status, inportb(0xfb1e), inportb(0xfb1f));
	}
       else
        {
         Have_Video_Stream = 1;
         
         Have_Decode_No = DecodeBuffNo;
         if(DecodeBuffNo)
            DecodeBuffNo = 0;
         else
            DecodeBuffNo = 1;
        }

     outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x04);//ʹ��MJPEG�����ж�
     //if((inportb(0xfb1e) != 0x00)||(inportb(0xfb1f) != 0x00))
     //  xtm_printf("ddddddddddddddddddddddddddddddddddddddddddddddddssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss\n");
     //xtm_printf("MJPEGDecInt: DE_J_STATUS = 0x%X, 0xfb1e = 0x%X, 0xfb1f = 0x%X\n", tmp_status, inportb(0xfb1e), inportb(0xfb1f));
     //DeocdeFinishFlag = 1;
}
//---------------------------------------------------------------------------
//INT16U  VideoCount = 0;
void DecoceVideo(INT32U Video_Maddr, INT32U Video_Len)
{
  INT32U tmp_data;
  INT8U val;

  //VideoCount ++;

  WaitDecodeFinish();

   /*   outportb(DE_J_DQT_ADD, 0x00);
      val = inportb(DE_J_DQT_DAT);
      if(val != 0x0D)
        xtm_printf("DecoceVideo::val = 0x%X\n", val);    */

  outportb(DE_J_MBWIDTH, Local.DecodeVideoWidth >> 4);
  outportb(DE_J_MBHEIGHT, Local.DecodeVideoHeight >> 4);

  outport(DE_J_HSIZE_L, Local.DecodeVideoWidth);
  outport(DE_J_VSIZE_L, Local.DecodeVideoHeight);

  if(DecodeBuffNo)
   {
    tmp_data = (INT32U)Local.DecodeVideoWidth*Local.DecodeVideoHeight*2;
    //PlaySoundTip();
   }
  else
   {
    tmp_data = 0;
    
   }

  outportb(DE_J_DATA_SA_H, tmp_data>>16);
  outportb(DE_J_DATA_SA_M, tmp_data>>8);
  outportb(DE_J_DATA_SA_L, tmp_data);

  //xtm_printf("t_image->Maddr = 0x%X %X, bs_length = %d\n", \
  //      (INT16U)((t_image->Maddr & 0xffff0000) >> 16), (INT16U)(t_image->Maddr & 0xffff), t_image->Len);

  outportb(DE_J_CPU_SA_H, (Video_Maddr>>16));
  outportb(DE_J_CPU_SA_M, (Video_Maddr>>8));
  outportb(DE_J_CPU_SA_L, Video_Maddr);

  outportb(DE_J_CPU_LEN_H, (Video_Len>>16));
  outportb(DE_J_CPU_LEN_M, (Video_Len>>8));
  outportb(DE_J_CPU_LEN_L, Video_Len);

  outportb(DE_J_CPU_PN, 1);
                
  outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x9a);
  //outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x1a);
  //outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x02);
  //outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)|0x9e);
  outportb(DE_J_CONTROL, inportb(DE_J_CONTROL)&0xfd);
  //WaitDecodeFinish();
}