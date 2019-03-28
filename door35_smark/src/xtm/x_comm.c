#include "includes.h"
#include "ComDrv.h"

#include "stdlib.h"

#define x_comm_h

#define CommonH
#include "common.h"

#define _COMM_OPERATE   0

#define SEND_TO_232  1
#define SEND_TO_485  2
void dcomm1(void);
void dcomm2(void);
void CommSendBuff(INT8U comm_fd, INT8U *comm_buf, INT16U send_len, INT8U send_dest);

void KeyTouchOperate(INT8U TouchNum); //������Ӧ
void KeyPreDeal(void); //����Ԥ��������Ч��
void KeyBackDeal(void); //������������Ч�� ������

INT8U CheckKeyPressAndLcd(void); //��鰴��������LCD

INT8U TestEncodeErrorReset = 0;
extern INT8U EncodeStartFlag;

#define COMMMAX 96
INT32U prev_comm_time = 0;
INT8U commbuf_len = 0;
INT8U commbuf[COMMMAX];
void Comm_Data_Deal(INT8U *buff, INT16U len);

void M8Input_Func(INT8U *inputbuff); //M8����
#ifdef _BRUSHIDCARD_SUPPORT   //ˢ������֧��
 void IDCard_Func(INT8U *inputbuff); //�û�ˢID��
 void CheckBrushAndSend(void);  //���ˢ����¼�������������
 void SendIDCardAck(INT8U *inputbuff, INT8U result, INT8U nLength);//����ID��ˢ��Ӧ��
 void PlayCardOkWav(void);
 void PlayCardFailWav(void);
#endif

#ifdef ELEVATORCONTROL_HANGZHOU
void SendElevatorControl(INT8U *Addr);
void SendElevatorControl_Order(INT8U *DestAddr, INT8U *SrcAddr);
#endif
//---------------------------------------------------------------------------
void CommSendBuff(INT8U comm_fd, INT8U *comm_buf, INT16U send_len, INT8U send_dest)
{
  INT16U i;
  if(comm_fd == 1)
   {
      switch(send_dest)
       {
        case  SEND_TO_232: //1
                          SetGpio(GPIOD_OUT_DATA, COMM_SEC_HIGH, SET_HIGH); //1 -- 232   0 -- 485
                          SetGpio(GPIOD_OUT_DATA, SEND_485_LOW, SET_LOW);   //1 -- 485 send enable   0 -- 485 send disable
                          break;
        case  SEND_TO_485: //2
                          SetGpio(GPIOD_OUT_DATA, COMM_SEC_LOW, SET_LOW); //1 -- 232   0 -- 485
                          SetGpio(GPIOD_OUT_DATA, SEND_485_HIGH, SET_HIGH);   //1 -- 485 send enable   0 -- 485 send disable
                          //delay(10);
                          break;
       }
   }
  for (i=0; i<send_len; i++)
   {
    if(comm_fd == 0)
     {
      while((inportb(UART0_TX_STATUS)&0x1)!=0x1);	// �ȴ����ͱ�����Ϊ��
      outportb(UART0_TX_DATA, comm_buf[i]);				// �����ַ�
     }
    if(comm_fd == 1)
     {
      while((inportb(UART1_TX_STATUS)&0x1)!=0x1);	// �ȴ����ͱ�����Ϊ��
      outportb(UART1_TX_DATA, comm_buf[i]);				// �����ַ�
     }
   }

  if(comm_fd == 1)
   {
      switch(send_dest)
       {
        case  SEND_TO_232: //1
                          break;
        case  SEND_TO_485: //2
                          DelayMs(send_len + 6);
                          break;
       }
    SetGpio(GPIOD_OUT_DATA, COMM_SEC_HIGH, SET_HIGH); //1 -- 232   0 -- 485
    SetGpio(GPIOD_OUT_DATA, SEND_485_LOW, SET_LOW);   //1 -- 485 send enable   0 -- 485 send disable
   } 
 //if(comm_fd == 1)
 //  xtm_printf("CommSendBuff::\n");
}
//---------------------------------------------------------------------------
INT8U ErrorStopDecode = 0;
void dcomm1(void)
{
	INT32U padd, padd1;
	INT16U i, j, tmp;
        INT32U ii, CardPos;
	INT8U k, gpio_data;
        INT8U DestIP[4];
        INT16U val;
        INT8U buf[512];
        struct file_head1 file_head;
        INT8U flash_id[8];
	while (comm1.RP!=comm1.WP){
       #ifdef _XTM_ADD
        xtm_printf("dcomm1::comm1.RP = %d, comm1.WP = %d, comm1.Rbuf[comm1.RP] = 0x%X\n", comm1.RP, comm1.WP, comm1.Rbuf[comm1.RP]);
       #endif
       switch(comm1.Rbuf[comm1.RP])
        {
#if _COMM_OPERATE
         case 'A':
                  xtm_printf("dcomm1::Recv = 0x%X\n", comm1.Rbuf[comm1.RP]);
                  Lcd_Power(0, LocalCfg.BrightVolume);
                  break;
         case 'B':
                  xtm_printf("dcomm1::Recv = 0x%X\n", comm1.Rbuf[comm1.RP]);
                  //Lcd_Power(1, LocalCfg.BrightVolume);
                  InitVideo();
                  for(i=0; i<100; i++)
                   {
                    LocalCfg.EncodeFrameSize = VIDEO_VGA; //�����С

                    //��ʼ¼����Ƶ
                    xtm_printf("1  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                    StartVideoClock();     //����Ƶ�ɼ�����ʱ��
                    xtm_printf("2  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                    StartRecVideo(LocalCfg.EncodeFrameSize);
                    xtm_printf("3  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                    StartEncodeVideo(LocalCfg.EncodeFrameSize);     //����Ƶ����
                    xtm_printf("4  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                    ClearWatchDog();
                    DelayMs(100);
                    StopEncodeVideo();
                    StopRecVideo();
                    StopVideoClock();            //�ر���Ƶ�ɼ�����ʱ��
                    xtm_printf("5  EN_J_STATUS = 0x%X\n",  inportb(EN_J_STATUS));
                   }                  
                  break;
         case 'C':
                  xtm_printf("C\n");

       IDCardNo.Num = 1000;
       for(ii=0; ii<IDCardNo.Num; ii++)
        {
           CardPos = BYTEPERSN_NEW*ii;
           buf[0] = 0x00;
           buf[1] = (INT8U)((ii >> 16) & 0xff);
           buf[2] = (INT8U)((ii >> 8) & 0xff);
           buf[3] = (INT8U)((ii) & 0xff);
           outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
           outportb(MEMORY_WINDOW_BASE_H, 0x68 + (CardPos >> 16) & 0xffff);
           xtm_memcopy(RecvIDCardNo.SN + (CardPos & 0xffff), buf, BYTEPERSN_NEW);
           outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);

           ClearWatchDog();
        }

       //дID���ļ�
       WriteIDCardFile();
       xtm_printf("finish\n");
                //  TestEncodeErrorReset = 1;
                //  xtm_printf("1 inportb(GPIOD_OEN_DATA) = 0x%X, EncodeStartFlag = %d\n", inportb(GPIOD_OEN_DATA), EncodeStartFlag);
                //  xtm_printf("4 inportb(GPIOD_OUT_DATA) = 0x%X\n", inportb(GPIOD_OUT_DATA));
                  break;
         case 'P':
                  padd = SWTIMER_Tick();
                  xtm_printf("P, timer_counter = 0x%X %X, IDCardNo.Num = 0x%X %X\n", (INT16U)((padd >> 16)&0xffff), (INT16U)(padd & 0xffff), \
                             (INT16U)((IDCardNo.Num >> 16)&0xffff), (INT16U)(IDCardNo.Num & 0xffff));
     outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
for(ii=0; ii<IDCardNo.Num; ii++)
    {
//     CardPos = ii*BYTEPERSN_NEW;
     CardPos = ii << 2;
     //xtm_printf("CardPos = 0x%X 0x%X\n", (INT16U)((CardPos >> 16)&0xffff), (INT16U)(CardPos & 0xffff));
     tmp = (CardPos & 0xffff);

     outportb(MEMORY_WINDOW_BASE_H, 0x68 + (CardPos >> 16) & 0xffff);
     //if(ii > 0xffff)
       xtm_printf("ii = 0x%X, 0x%X)::%X  %X  %X  %X\n", (INT16U)(ii >> 16) & 0xffff, (INT16U)(ii & 0xffff), IDCardNo.SN[0 + tmp], IDCardNo.SN[1 + tmp], IDCardNo.SN[2 + tmp], IDCardNo.SN[3 + tmp]);

    // if((ii & 0x4fff) == 0)
       ClearWatchDog();
    }
    outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);
    padd1 = SWTIMER_Tick();
    xtm_printf("P finish, timer_counter = 0x%X %d\n", (INT16U)(((padd1 - padd) >> 16)&0xffff), (INT16U)((padd1 - padd)*5 & 0xffff));
               /*   ErrorStopDecode = 0;
                  for(i=0; i<1000; i++)
                   {
                    DisplayTalkWindow(REFRESH);
                    if(ErrorStopDecode)
                     {
                      xtm_printf("ErrorStopDecode 1\n");
                      break;
                     }
                    xtm_printf("Local.fb_page = %d\n", Local.fb_page);
                    DisplayImage(&small_num_image[0], FROMFILE, NOFULLPAGE);
                    if(ErrorStopDecode)
                     {
                      xtm_printf("ErrorStopDecode 2\n");
                      break;
                     }
                    DisplayImage(&big_num_image[0], FROMFILE, NOFULLPAGE);
                    if(ErrorStopDecode)
                     {
                      xtm_printf("ErrorStopDecode 3\n");
                      break;
                     }
                   }      */
                  //ShowOnlineInfo();
                  //  0x30  16   '0'
                  //  0x45  37   'E'
                  //  0x65  69   'e'
                //  Touch_Read_XY();
//                  PrintArp_Table();
                  break;
         case 'D':
                  xtm_printf("D\n");
                /*  val = phy_reg_read(0, 0);
                  xtm_printf("val1 = 0x%X\n", val);
                  val = val | 0x4000;
                  phy_reg_write(0, 0, val);
                  val = phy_reg_read(0, 0);
                  xtm_printf("val2 = 0x%X\n", val);   */
                  //Writees8388(0x2B, 0xc0);
                  //Writees8388(0x00, 0x16);//0x06);      0x07    0x32
//                  SetGpio(GPIOA_OUT_DATA, OPEN_LOCK_LOW, SET_LOW); //������
                  //SetGpio(GPIOD_OUT_DATA, CAMERA_POWER_LOW, SET_LOW); //��CAMERA
                  //outportb(PCMI_CONTROL, inportb(PCMI_CONTROL)|0x80);//ʹ��PCMP�ж�
                  //PrintNoisePcmTotalNum();
                  break;
         case 'E':
                  xtm_printf("E\n");
                  //clear_disp_ram();
//                  SetGpio(GPIOA_OUT_DATA, OPEN_LOCK_HIGH, SET_HIGH); //������
                  //SetGpio(GPIOD_OUT_DATA, CAMERA_POWER_HIGH, SET_HIGH); //��CAMERA
                  break;
         case 'F':
                  xtm_printf("F\n");
                  LocalCfg.SpkVolume = 2;
                  SoundSetSpeakerVolume(LocalCfg.SpkVolume);                  
                  StartPlayWav(soundtip_wav.Faddr, CYCLE_PLAY);  
                  break;
         case 'G':
                  //read25x32_unique_id(flash_id);
                  //xtm_printf("flash_id = 0x%X:0x%X:0x%X:0x%X:0x%X:0x%X:0x%X:0x%X\n", flash_id[0], flash_id[1], flash_id[2], flash_id[3], flash_id[4], flash_id[5], flash_id[6], flash_id[7]);
                  //selectym(1);
                  StartRecWav();
                  break;
         case 'H':
                  //Local.GpioWatchDogStarted = 0;  //��GPIO����߳�������WatchDog
                  //selectym(0);
                  StopRecWav();
                  break;
         case 'I':
                  StopPlayWavFile();
                  break;
         case 'J':
                  xtm_printf("J\n");
 /* xtm_printf("�� Faddr = 0x%X%X , Filelen = 0x%X%X, Flen = 0x%X%X, Width = %d, Height = %d, xLeft = %d, yTop = %d\n", \
                                (INT16U)((talk_connect[0].Faddr & 0xffff0000) >> 16), (INT16U)(talk_connect[0].Faddr & 0xffff), \
                                (INT16U)((talk_connect[0].Len & 0xffff0000) >> 16), (INT16U)(talk_connect[0].Len & 0xffff), \
                                (INT16U)((talk_connect[0].FLen & 0xffff0000) >> 16), (INT16U)(talk_connect[0].FLen & 0xffff), \
                                 talk_connect[0].width, talk_connect[0].height, talk_connect[0].xLeft, talk_connect[0].yTop);
                  DisplayImage(&talk_connect[0], FROMFILE, NOFULLPAGE);    */
                  break;

         case 'K':
                  xtm_printf("K\n");
                  StartPlayWav_Rec(soundtip_wav.Faddr, NO_CYCLE_PLAY);
                  break;
         case 'L':
                  StartRecAudio();
                  break;
         case 'T':
                  StopRecAudio();
                  break;
         case 'M':
                  break;
         case 'N':

                  break;
         case 'O':
                  break;
         case 'R':
                 break;
         case 'S':
                  xtm_printf("S\n");
                  break;
         case 'U':
                  xtm_printf("U\n");
         case 'W':
                  break;
         case 'Y':
                  break;
         case 'Z':
                  break;
         case 'a':  //0
         case 'b':  //1
         case 'c':  //2
         case 'd':  //3
         case 'e':  //4
         case 'f':  //5
         case 'g':  //6
         case 'h':  //7
         case 'i':  //8
         case 'j':  //9
         case 'k':  //10
         case 'l':  //11
         case 'm':  //12
         case 'n':  //13
         case 'o':  //14
                  xtm_printf("dcomm1::read25x32_status = 0x%X\n", read25x32_status());
                  KeyTouchOperate(comm1.Rbuf[comm1.RP] - 'a');
                  xtm_printf("dcomm1:: Local.KeyTouchNo = %d, comm1.Rbuf[comm1.RP] = 0x%X\n", Local.KeyTouchNo, comm1.Rbuf[comm1.RP]);
                  break;
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
                  KeyTouchOperate(100 + comm1.Rbuf[comm1.RP] - '0');
                  break;

         case '7':
                  break;
         case '8':
                  break;
         case '9':
                  break;
#endif

         }
    comm1.RP = comm1.WP;
 }
}
//---------------------------------------------------------------------------
void KeyTouchOperate(INT8U TouchNum) //������Ӧ
{
  if(CheckKeyPressAndLcd()) //��鰴��������LCD
    return;
  if(Local.KeyTouchNo == 0)
   {
    if(Local.Status == 0)
      PlaySoundTip(); //������ʾ��
    Local.KeyTouchNo = TouchNum + 1; //��ť���±�־
    KeyPreDeal(); //����Ԥ��������Ч��
    dtime[3] = KEYDELAYTIME;
   }
}
//---------------------------------------------------------------------------
void KeyPreDeal(void) //����Ԥ��������Ч��
{
}
//---------------------------------------------------------------------------
void KeyBackDeal(void) //������������Ч�� ������
{
  switch(Local.CurrentWindow)
   {
    case 5: //��������
           OperateSetupWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 0://��������
           OperateTalkWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 4://���Ӵ���
           OperateWatchWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 2://�Խ� �������Ӵ���
           OperateTalkConnectWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 3://�Խ� ͨ������
           OperateOpenLockWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
    case 152: //����������
           OperateSetupMainWindow(Local.KeyTouchNo - 1, Local.CurrentWindow);
           break;
   }
}
//---------------------------------------------------------------------------
INT8U CheckKeyPressAndLcd(void) //��鰴��������LCD
{
  Local.KeyPressTime = 0;
  Local.LcdLightTime = 0;
  if(Local.LcdLightFlag == 0)
   {
    //����LCD����
    Lcd_Power(1, LocalCfg.BrightVolume);
    Local.LcdLightFlag = 1; //LCD�����־
    Local.LcdLightTime = 0; //ʱ��
    return 1;
   }
  return 0;
}
//---------------------------------------------------------------------------
void dcomm2(void)
{
      INT16U comlen;
      INT8U buf[512];
      comlen = 0;
      while (comm2.RP!=comm2.WP)
       {
        #ifdef _XTM_ADD
         //xtm_printf("dcomm2::comm2.RP = %d, comm2.WP = %d, comm2.Rbuf[comm2.RP] = 0x%X\n", comm2.RP, comm2.WP, comm2.Rbuf[comm2.RP]);
        #endif
        buf[comlen] = comm2.Rbuf[comm2.RP];
        comm2.RP ++;
        if(comm2.RP >= COMBUFFLEN)
          comm2.RP = 0;
        comlen ++;
       }
      if(comlen > 0)
       {
        buf[comlen] = '\0';
        //xtm_printf("dcomm2::len = %d, buf = %s, comm2.RP = %d, comm2.WP = %d\n", comlen, buf, comm2.RP, comm2.WP);
        Comm_Data_Deal(buf, comlen);
       }
}
//---------------------------------------------------------------------------
void Comm_Data_Deal(INT8U *buff, INT16U len)
{
  INT8U i;
  INT32U nowtime;
  INT8U m_crc;

  INT8U validbuff[20];
  INT8U validlen;
  INT8U DataIsOK;

       nowtime = time_count;
       //����һ�ν��ճ���50ms,���ж�Ϊ��ʱ
       if((nowtime - prev_comm_time) >= 10)
        {
          commbuf_len = 0;
        }
       prev_comm_time = time_count;


     /*  xtm_printf("len = %d\n", len);
       for(i=0; i<len; i++)
         xtm_printf("0x%X, ", buff[i]);
       xtm_printf("\n");    */

       xtm_memcopy(commbuf + commbuf_len, buff, len);
       commbuf_len += len;
       if(commbuf_len >= COMMMAX)
         commbuf_len = 0;

       DataIsOK = 0;
       if(commbuf_len >= 18)
        {
         //M8
         if(commbuf[0] == 0x7E)
          {
           DataIsOK = 1;
           validlen = 18;
           if(commbuf_len >= validlen)
            {
             xtm_memcopy(validbuff, commbuf, validlen);
             commbuf_len = 0;
             if((validbuff[0] == 0x7E)&&(validbuff[17] == 0x0D))
              {
               m_crc = 0;
               for(i=1; i<=15; i++)
                 m_crc += validbuff[i];
               if((m_crc == 0x7E)||(m_crc == 0x0D))
                 m_crc -= 1;

     /*  for(i=0; i<validlen; i++)
         xtm_printf("0x%X, ", validbuff[i]);
       xtm_printf("\n");

               xtm_printf("m_crc = 0x%X, validbuff[16] = 0x%X\n", m_crc, validbuff[16]);   */
               if(m_crc == validbuff[16])
                {
                 xtm_printf("M8Input_Func::");  
                 M8Input_Func(validbuff); //M8����
                 
                }
              }
            }
          }
        }
}
//---------------------------------------------------------------------------
void M8Input_Func(unsigned char *inputbuff) //M8����
{
  switch(inputbuff[1])
   {
    case 0x52:  //ID����
             #ifdef _BRUSHIDCARD_SUPPORT   //ˢ������֧��
			  xtm_printf("IDCard_Func::");  
              IDCard_Func(inputbuff);
             #endif 
             break;
   }
}
//---------------------------------------------------------------------------
#ifdef _BRUSHIDCARD_SUPPORT   //ˢ������֧��
void SendIDCardAck(INT8U *inputbuff, INT8U result, INT8U nLength)//����ID��ˢ��Ӧ��
{
  INT8U send_comm[30];
  INT8U i;
  xtm_memcopy(send_comm, inputbuff, nLength);
  send_comm[2] = 0x32; //Ӧ��
  send_comm[15] = result;   //1 �ɹ�  0  ʧ��
  send_comm[16] = 0;  
  for(i=1; i<=15; i++)    //crc
    send_comm[16] += send_comm[i];
  if((send_comm[16] == 0x7E)||(send_comm[16] == 0x0D))
    send_comm[16] --;
  send_comm[17] = 0x0D;
//  for(i=0; i<18; i++)
//    xtm_printf("send_comm[%d] = 0x%X, ", i, send_comm[i]);
  CommSendBuff(1, send_comm, nLength, SEND_TO_232);
}
//---------------------------------------------------------------------------
void IDCard_Func(INT8U *inputbuff) //�û�ˢID��
{
#if 1
  INT32U i, TotalByte;
  INT8U IDCardOk;
  INT32U tmpnum;
  char mac_text[5];
  INT8U hex_data[6];
  INT8U hex1, hex2;

  //xtm_printf("IDCard_Func::IDcard[0] = %c%c, IDcard[1] = %c%c, IDcard[2] = %c%c, IDcard[3] = %c%c\n",
  //       inputbuff[4], inputbuff[5], inputbuff[6], inputbuff[7], inputbuff[8], inputbuff[9], inputbuff[10], inputbuff[11]);
  xtm_printf("IDCard_Func::inputbuff[4] = 0x%X, inputbuff[5] = 0x%X, inputbuff[6] = 0x%X, inputbuff[7] = 0x%X, inputbuff[8] = 0x%X, inputbuff[9] = 0x%X, inputbuff[10] = 0x%X, inputbuff[11] = 0x%X\n",
         inputbuff[4], inputbuff[5], inputbuff[6], inputbuff[7], inputbuff[8], inputbuff[9], inputbuff[10], inputbuff[11]);

  for(i=0; i<4; i++)
    hex_data[i] = 0;
  for(i=0; i<8; i+=2)
   {
    mac_text[0] = inputbuff[4+i];
    mac_text[1] = inputbuff[5+i];
    mac_text[2] = '\0';

    if((mac_text[0] >= '0')&&(mac_text[0] <= '9'))
      hex1 = mac_text[0] - '0';
    if((mac_text[0] >= 'a')&&(mac_text[0] <= 'f'))
      hex1 = mac_text[0] - 'a' + 10;
    if((mac_text[0] >= 'A')&&(mac_text[0] <= 'F'))
      hex1 = mac_text[0] - 'A' + 10;

    if((mac_text[1] >= '0')&&(mac_text[1] <= '9'))
      hex2 = mac_text[1] - '0';
    if((mac_text[1] >= 'a')&&(mac_text[1] <= 'f'))
      hex2 = mac_text[1] - 'a' + 10;
    if((mac_text[1] >= 'A')&&(mac_text[1] <= 'F'))
      hex2 = mac_text[1] - 'A' + 10;

    hex_data[i/2] = (hex1)*16 + (hex2);
   }
  xtm_printf("IDCard_Func::hex_data[0] = 0x%X, hex_data[1] = 0x%X, hex_data[2] = 0x%X, hex_data[3] = 0x%X\n",
         hex_data[0], hex_data[1], hex_data[2], hex_data[3]);
  IDCardOk = 0;

  outportb(CONTROL_REG2, inportb(CONTROL_REG2)|0x08);
  TotalByte = IDCardNo.Num*BYTEPERSN_NEW;
  for(i=0; i<TotalByte; i+=BYTEPERSN_NEW)
   {
    outportb(MEMORY_WINDOW_BASE_H, 0x68 + (i >> 16) & 0xffff);
    //xtm_printf("i = 0x%X %X, 0x%X 0x%X 0x%X 0x%X\n", (INT16U)((i & 0xffff0000) >> 16), (INT16U)(i & 0xffff), \
    //         RecvIDCardNo.SN[(i & 0xffff)+0], RecvIDCardNo.SN[(i & 0xffff)+1], RecvIDCardNo.SN[(i & 0xffff)+2], RecvIDCardNo.SN[(i & 0xffff)+3]);
    if((IDCardNo.SN[(i & 0xffff)+0] == hex_data[0])&&
       (IDCardNo.SN[(i & 0xffff)+1] == hex_data[1])&&
       (IDCardNo.SN[(i & 0xffff)+2] == hex_data[2])&&
       (IDCardNo.SN[(i & 0xffff)+3] == hex_data[3]))
      {
       IDCardOk = 1;
       break;
      }
   }
  outportb(CONTROL_REG2, inportb(CONTROL_REG2)&0xf7);

  xtm_printf("IDCard_Func::IDCardOk = %d, IDCardNo.Num = 0x%X %X, LocalCfg.CardOpenLock = %d\n", IDCardOk, (INT16U)((IDCardNo.Num & 0xffff0000) >> 16), \
             (INT16U)(IDCardNo.Num & 0xffff), LocalCfg.CardOpenLock);

  SendIDCardAck(inputbuff, IDCardOk, 18);//����ID��ˢ��Ӧ��
  if(LocalCfg.CardOpenLock == 0)
    return;
  if(IDCardOk == 1)
    {
     PlayCardOkWav();
     //PlayDoorOpenWav();
     OpenLock_Func();  //����

     //ˢ����¼
     if(BrushIDCard.Num < IDCARDBRUSHNUM)
      {
       //ID����
       tmpnum = BrushIDCard.Num*11;

       //xtm_memcopy(BrushIDCard.Info + tmpnum, hex_data, 4);
       BrushIDCard.Info[tmpnum] = hex_data[0];
       BrushIDCard.Info[tmpnum + 1] = hex_data[1];
       BrushIDCard.Info[tmpnum + 2] = hex_data[2];
       BrushIDCard.Info[tmpnum + 3] = hex_data[3];
       BrushIDCard.Info[tmpnum + 4] = 0;
       BrushIDCard.Info[tmpnum + 5] = 0;
       BrushIDCard.Info[tmpnum + 6] = 0;
       BrushIDCard.Info[tmpnum + 7] = 0;
       BrushIDCard.Info[tmpnum + 8] =  0;
       BrushIDCard.Info[tmpnum + 9] =  0;
       BrushIDCard.Info[tmpnum + 10] =  0;
       //printf("%d %d %d %d %d %d %d\n", BrushIDCard.Info[4], BrushIDCard.Info[5], BrushIDCard.Info[6], BrushIDCard.Info[7], BrushIDCard.Info[8],
       //       BrushIDCard.Info[9], BrushIDCard.Info[10]);
       BrushIDCard.Num ++;

       //���ˢ����¼�������������
       CheckBrushAndSend();
      }
   }
  else
    PlayCardFailWav();
#endif
}
//---------------------------------------------------------------------------
//���ˢ����¼�������������
void CheckBrushAndSend(void)
{
#if 1
  INT16U sendlength;
  INT8U sendb[1520];
  INT32U tmpnum;
  if(BrushIDCard.Num > 0)
   {
    //ͷ��
    xtm_memcopy(sendb, UdpPackageHead, 6);
    //����
    sendb[6] = BRUSHIDCARD;
    sendb[7] = ASK;    //����

    xtm_memcopy(sendb+8, LocalCfg.Addr, 20);

    //����
    if(BrushIDCard.Num > 120)
      tmpnum = 120;
    else
      tmpnum = BrushIDCard.Num;
    sendb[28] = (tmpnum & 0x00FF);
    sendb[29] = (tmpnum & 0xFF00) >> 8;

    xtm_memcopy(sendb + 30, BrushIDCard.Info, 11*tmpnum);

    sendlength = 30 + 11*tmpnum;

    CreateMultiUdpBuff(LocalPort[0], MULTI_SEND, LocalCfg.IP_Server, sendb, sendlength, 0);
   }
#endif   
}
//---------------------------------------------------------------------------
void PlayCardOkWav(void)
{
  if(Local.Status == 0)
   {
    WaitAudioUnuse(200); //�ȴ���������
    StartPlayWav(card_ok_wav.Faddr, NO_CYCLE_PLAY);
   }
}
//---------------------------------------------------------------------------
void PlayCardFailWav(void)
{
  if(Local.Status == 0)
   {
    WaitAudioUnuse(200); //�ȴ���������
    StartPlayWav(card_fail_wav.Faddr, NO_CYCLE_PLAY);
   }
}
#endif
//---------------------------------------------------------------------------
#ifdef ELEVATORCONTROL_HANGZHOU
void SendElevatorControl(INT8U *Addr)
{
  INT8U send_comm[30];
  INT8U i, nLength;
  if(LocalCfg.Addr[0] == 'M')
   {
    send_comm[0] = 0x7E;
    send_comm[1] = 0xA1; //����
    send_comm[2] = 0x00;
    send_comm[3] = 0x00;
                                 //
    send_comm[4] = (Addr[7] - '0')*16 + Addr[8] - '0';
    send_comm[5] = (Addr[9] - '0')*16 + Addr[10] - '0';

    send_comm[6] = 0x01;
    send_comm[7] = LocalCfg.Addr[7] - '0';  //�豸�� 0 -- ���棬 1 -- ��1��   2 -- ��2��

    send_comm[8] = 0;
    for(i=1; i<8; i++)    //crc
      send_comm[8] += send_comm[i];
    send_comm[9] = 0x0F;
    nLength = 10;
    CommSendBuff(1, send_comm, nLength, SEND_TO_485);    //  SEND_TO_485

    xtm_printf("SendElevatorControl::Addr = %s, ���е���, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", \
                Addr, send_comm[0], send_comm[1], send_comm[2], send_comm[3], send_comm[4], send_comm[5], send_comm[6], send_comm[7], send_comm[8], send_comm[9]);
   }
}
//---------------------------------------------------------------------------
void SendElevatorControl_Order(INT8U *DestAddr, INT8U *SrcAddr)
{
  INT8U send_comm[30];
  INT8U i, nLength;
  if(LocalCfg.Addr[0] == 'M')
   {
    DestAddr[12] = '\0';
    SrcAddr[12] = '\0';

    send_comm[0] = 0x7E;
    if(strcmp(DestAddr, SrcAddr) == 0) //���ں���
     {
      send_comm[1] = 0xA2;
      send_comm[6] = 0;
      send_comm[7] = 0;
     }
    else                   //����
     {
      send_comm[1] = 0xA3;

      send_comm[6] = (SrcAddr[7] - '0')*16 + SrcAddr[8] - '0';
      send_comm[7] = (SrcAddr[9] - '0')*16 + SrcAddr[10] - '0';
     }
    send_comm[2] = 0x00;
    send_comm[3] = 0x00;
                                 //
    send_comm[4] = (DestAddr[7] - '0')*16 + DestAddr[8] - '0';
    send_comm[5] = (DestAddr[9] - '0')*16 + DestAddr[10] - '0';

    send_comm[8] = 0;
    for(i=1; i<8; i++)    //crc
      send_comm[8] += send_comm[i];
    send_comm[9] = 0x0F;
    nLength = 10;
    CommSendBuff(1, send_comm, nLength, SEND_TO_485);  //SEND_TO_485

    xtm_printf("SendElevatorControl_Order::DestAddr = %s, SrcAddr = %s, ���е���, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n",
                DestAddr, SrcAddr, send_comm[0], send_comm[1], send_comm[2], send_comm[3], send_comm[4], send_comm[5], send_comm[6], send_comm[7], send_comm[8], send_comm[9]);
   }
}
#endif
//---------------------------------------------------------------------------

