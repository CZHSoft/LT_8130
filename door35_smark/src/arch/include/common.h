
#include "x_printf.h"
#include "x_disp.h"
#include "x_flash.h"

#include "x_audio.h"
#include "x_video.h"

#include "x_main.h"
#include "x_lcd.h"
#include "x_comm.h"
#include "x_gpio.h"

#include "x_net.h"
#include "x_ip.h"
#include "x_udp.h"

#include "x_es8388.h"
#include "x_ov7725.h"
#include "x_w7514.h"

#include "x_first.h"
#include "x_setup.h"
#include "x_talk.h"
#include "x_timer.h"
#include "m_init.h"

//20140225 ���г�ʱʱ�� 15 30 60 99

#define VIDEO_VGA  0
#define VIDEO_QVGA  1

#define LOCAL_VIDEO   0
#define REMOTE_VIDEO  1

#define _REMOTECALLTEST  //Զ�̺��в���
//#define _ADDTABLE_SUPPORT  //��ַ��֧��
#define _MULTI_MACHINE_SUPPORT  //��ֻ�֧��
#define _CAMERA_SUPPORT     //�粻������ͷ��GPIOB6��7ΪComm1, GPIOC ��Ϊ8������ʹ��
#define _CAPTUREPIC_TO_CENTER  //����ͼƬ��������
#define _BRUSHIDCARD_SUPPORT   //ˢ������֧��
#define _MULTI_CENTER_SUPPORT  //�����Ļ�֧��   20130613
#define _NIGHT_CAMERA_ADJUST_SUPPORT   //�ⰵCAMERA�Զ�����֧��   20130626

#define ELEVATORCONTROL_HANGZHOU      //20120822
#ifdef ELEVATORCONTROL_HANGZHOU      //20120822
  #define ELEVATR_ORDER1  181
#endif
//#define _SEND_TO_SELF_TEST    //�������ݰ����Լ�

//#define _TESTNSSERVER   //���Է���������ģʽ

//Flash secter ɾ��ÿ��Ϊ4K�� ���Դ洢��ַ��Ϊ 4K�ı���
#define INFO_ADDR       0x7f8000   //��Ϣ�洢��ַ   ��һ�� 512 Ϊ����  ��20��512 Ϊ��Ϣ��
#define LOCALCFG_ADDR   0x7ff000   //�����ļ��洢��ַ
#define LOCALCFG_ADDR1   0x7fe000   //�����ļ��洢��ַ2  ������

#define RECORDCFG_ADDR       0x7f0000   //ͨ����¼�洢��ַ   ��һ�� 512 Ϊ����  ��20��512 Ϊͨ����¼��
#define RECORDPIC_ADDR       0x570000   //ͨ����¼��Ƭ�洢��ַ   ÿ64K һ����Ƭ, ���20��

#ifdef _ADDTABLE_SUPPORT  //��ַ��֧��
 #define ADDRTABLE_ADDR       0x790000   //��ַ��   ���� 0x50000  ��  0x7effff
 #define ADDR_MAXNUM          10000      //��ַ���������
#endif

#define FLAGTEXT   "xtm8130sdkkkkdfdsIM"
#define HARDWAREVER "M-HW VER 4.0.2 For 8130"    //Ӳ���汾
#define SOFTWAREVER "M-SW35A VER 4.2.0"   //����汾
#define SERIALNUM "20130605A35"    //��Ʒ���к�

#define IMAGEUP  0
#define IMAGEDOWN  1

#define TOUCH_CHIP_ADDR_WR     0x70      //������I2C��ַ read:10010001 write 10010000
#define TOUCH_CHIP_ADDR_RE      0x71      //������I2C��ַ read:10010001 write 10010000

#define REFRESH  1
#define NOREFRESH 0

#define FULLPAGE  1
#define NOFULLPAGE 0

#define DELTACALIB 50   //������У׼ʮ��λ��

#define REFRESH_TOP  0x01
#define REFRESH_MIDDLE  0x02
#define REFRESH_BOTTOM  0x04

#define MAXCOUNT   8      //�����������
#define SUBMAXNUM  3      //�����������
#define MAXDEFENCENUM  8   //����������
#define UDPPACKMAX  1200
#define UDPSENDMAX  20  //UDP��η��ͻ������ֵ
#define AUDIOSENDMAX  10  //Audio UDP��η��ͻ������ֵ
#define COMMSENDMAX  10  //COMM��η��ͻ������ֵ
#define MAXSENDNUM  6  //����ʹ���
#define MAXVIDEOBUF  2  //�����Ƶ����
#define MAXPACKNUM  30     //֡������ݰ�����

#define TIMERTIME 100       //500 ms
#define TIMERPERSEC 2       //ÿ��2���߳�
#define TIMER2TIME 1        //5 ms
#define GPIOTIME 4          //20 ms  GPIO���
#define GPIOPERSEC 50       //ÿ��50���߳�

#define IDLE_TIME      30      //���з���������ʱ��
#define CLOSELCD_TIME  60//10 //60      //������LCDʱ��
#define POWERSTAY_TIME  30 //500 // 30      //��LCD�� ����͹���ʱ��

#define NO_CYCLE_PLAY  0   //��Ƶ���β���
#define CYCLE_PLAY  1   //��Ƶѭ������
#define KEYDELAYTIME  30      //��ťѹ��ʱ��  150ms

#define PCM_REC_MAX  0x2000  //PCM¼�����������ֵ
#define PCM_BUF_NUM  2       //PCM��������ռ�ڴ�ҳ������64K*2
#define PCM_BUF_MAX  0x20000  //PCM�������������ֵ

//����Ϣ
#define INFOMAXITEM   20  //����Ϣ�������
#define INFOMAXSIZE   402 //����Ϣ�����������
#define INFONUMPERPAGE  5  //һҳ��ʾ��Ϣ��

#define INFOROWLEN   44    //��Ϣÿ�г���
#define INFOROWPIXEL   528    //��Ϣÿ�����ء�44*12
#define MAXROW  12          //�������
#define TOTALPAGE  3          //��ҳ��
#define PAGEPERROW  4          //ҳ����

//ͨ����¼
#define RECORDMAXITEM   20  //ͨ����¼�������
#define RECORDNUMPERPAGE  5  //һҳ��ʾͨ����¼��

#define DIRECTCALLTIME  600                 //ֱ�Ӻ���ÿ��ʱ��
#define WATCHTIMEOUT  (30*TIMERPERSEC)    //�����ʱ��   600
#define CALLTIMEOUT  (30*TIMERPERSEC)     //�����ʱ��   250
//#define TALKTIMEOUT  (15*TIMERPERSEC)    //99 130  30*20   //ͨ���ʱ��      600
#define TALKTIMEOUT DelayTalkTimeOut[LocalCfg.TalkTimeOut]*TIMERPERSEC

#define LANGUAGEMAXNUM   2   //�������

//���� ��������
#define ALARM         1    //����
#define CANCELALARM   2    //ȡ������
#define SENDMESSAGE   3   //������Ϣ
#define REPORTSTATUS  4   //�豸��ʱ����״̬
#define QUERYSTATUS   5   //�������Ĳ�ѯ�豸״̬
#define REMOTEDEFENCE   20   //Զ�̲���
#define RESETPASS       30   //��λ����
#define SEARCHALLEQUIP  252  //���������豸���������ģ����豸��
#define WRITEEQUIPADDR      254  //д�豸��ַ���������ģ����豸��

//�Խ�
#define VIDEOTALK      150 //���������ӶԽ�
#define VIDEOWATCH     152 //���������
#define NSORDER        154 //�����������������ڹ㲥��
#define NSSERVERORDER  155 //����������(NS������)
#define NSSERVERORDER2  156 //�豸�ն˽���(NS������)
#define FINDEQUIP      170 //�����豸
#define ASK              1     //�������� ����
#define REPLY            2     //�������� Ӧ��
#define SINGLE_SEND           1   //����
#define MULTI_SEND            2     //���

#define CALL             1     //����
#define LINEUSE          2     //ռ��
#define QUERYFAIL        3      //ͨ��ʧ��
#define CALLANSWER       4     //����Ӧ��
#define CALLSTART        6     //��ʼͨ��

#define CALLUP           7     //ͨ������1�����з�->���з���
#define CALLDOWN         8     //ͨ������2�����з�->���з���
#define CALLCONFIRM      9     //ͨ������ȷ�ϣ����շ����ͣ��Ա㷢�ͷ�ȷ�����ߣ�
#define REMOTEOPENLOCK   10     //Զ�̿���

#define JOINGROUP        22     //�����鲥�飨���з�->���з������з�Ӧ��
#define LEAVEGROUP       23     //�˳��鲥�飨���з�->���з������з�Ӧ��
#define TURNTALK         24     //ת�ӣ����з�->���з������з�Ӧ��
#define CENTERTURNTALK         25     //�����˹�ת��
#define TRUSTEESHIP_TALK  26    //�й�

#define CALLEND          30     //ͨ������

#ifdef _BRUSHIDCARD_SUPPORT   //ˢ������֧��
 #define IDCARD_SETUP_ADDR       0x56f000   //ID�����ô洢��ַ
 #define IDCARD_ADDR       0x570000   //ID���洢��ַ
 #define IDCARD_MAXPACK   2000    //ID��������
 #define _IDCARDHAVEADDR      //ID������ַ
 #define IDCARDMAXNUM  90000    //ID���������
 #ifdef _IDCARDHAVEADDR      //ID������ַ
  #define CARDPERPACK  50    //ÿ�����ݰ���ID���������
  #define BYTEPERSN    24    //ÿ��SN ռ���ֽ���
  #define BYTEPERSN_NEW  4   //Ϊ��ʡ�ռ䣬ȥ����ַ
 #else
  #define CARDPERPACK  350  //ÿ�����ݰ���ID���������
  #define BYTEPERSN    4     //ÿ��SN ռ���ֽ���
 #endif
 #define IDCARDBRUSHNUM 100 //�������ˢ������
 
 #define WRITEIDCARD    54   //дID����Ϣ
 #define READIDCARD     55   //��ID����Ϣ
 #define BRUSHIDCARD     56   //ˢID����Ϣ�������������
#endif

#ifdef _CAPTUREPIC_TO_CENTER  //����ͼƬ��������
 #define MAX_CAPTUREPIC_NUM   20
 #define CAPTUREPIC_SEND_START     60   //��Ԫ�ſڻ���Χǽ���������ſڻ����ͺ�����Ƭ->���Ϳ�ʼ
 #define CAPTUREPIC_SEND_DATA      61   //��Ԫ�ſڻ���Χǽ���������ſڻ����ͺ�����Ƭ->��������
 #define CAPTUREPIC_SEND_SUCC      62   //��Ԫ�ſڻ���Χǽ���������ſڻ����ͺ�����Ƭ->���ͽ������ɹ���
 #define CAPTUREPIC_SEND_FAIL      63   //��Ԫ�ſڻ���Χǽ���������ſڻ����ͺ�����Ƭ->���ͽ�����ʧ�ܣ�
#endif

#ifdef _MULTI_MACHINE_SUPPORT  //��ֻ�֧��
 #define HOSTTOSUB  222 //�����븱��ͨ��
 #define SYNCSUB        1   //ͬ��״̬����������������
 #define SUBDEFENCE     2   //��������������������
 #define SUBALARM       3   //��������������������
 #define SUBALARMTIP    5   //������ʾ����������������
 #define SUBDEFENCETIP    6   //������ʾ����������������
 #define SUBCONTROL        10    //�ҵ���ƣ���������������
 #define SUBCONTROLTIP     11    //�ҵ������ʾ����������������
 #define SUBFIND        255   //���Ҹ�������������������

 #define SUBFIND_START_NUM  4  //���Ҹ��� ���ʹ���   6 - 4 = 2
#endif

#define UPLOADFILE  225    //�ϴ�
#define UPLOADYUV  227    //�ϴ�YUV
#define READFLASH  228    //��FLASH����
#define REMOTERESET  229    //Զ��������

#define UDP_TEST  245

#define DOWNLOADFILE  224    //����
#define STARTDOWN  1       //��ʼ����һ��
#define DOWN       2       //����
#define DOWNFINISHONE       3  //�������һ��
#define STOPDOWN       10      //ֹͣ����
#define DOWNFAIL         21 //����ʧ��  �豸������������

#define REMOTEDEBUGINFO      253   //����Զ�̵�����Ϣ
#define STARTDEBUG  1    //��ʼ
#define STOPDEBUG  0    //ֹͣ
#define DEBUGDATA  2    //��������
#define REMOTE_REBOOT  3    //������

#ifdef _REMOTECALLTEST  //Զ�̺��в���
 #define REMOTETEST      200   //����Զ�̺��в���
 #define STARTTEST  1    //��ʼ
 #define ENTERTESTMODE  2    //�������ģʽ
 #define STOPCALL       3    //ֹͣ����ͨ��
#endif

#define SEND_PCM  226      //����PCM����
#define _REMOTEWRITEOV7725REGISTER   //Զ��дOV7725�Ĵ���  ������
#ifdef _REMOTEWRITEOV7725REGISTER   //Զ��дOV7725�Ĵ���  ������
 #define OV7725_OPERATE   227       //��дOV7725
 #define OV7725READREGISTER  1
 #define OV7725WRITEREGISTER 2
#endif

#ifdef _SEND_TO_SELF_TEST    //�������ݰ����Լ�
 #define SEND_SELF  251
#endif

#define _PACK_1024

#define PORT_NUM  5
#define SMALLESTSIZE  18  //512  //UDP��С����С    20121102 512 -- 18 , MAC��СΪ60
#define PACKDATALEN  1200   //���ݰ���С

#define MAINRETURNTYPE  0
#define TALKWATCHRETURNTYPE  1
#define TALKWINTYPE  0
#define WATCHWINTYPE  1

#define _TEST_BTN_NUM  31

#define SET_LOW             0x0
#define SET_HIGH            0x1

#define CAMERA_POWER_HIGH      0x20
#define CAMERA_POWER_LOW       0xdf
#define NET_LED_HIGH       0x80
#define NET_LED_LOW        0x7f
#define OPEN_LOCK_HIGH     0x20
#define OPEN_LOCK_LOW      0xdf
#define IO5_HIGH     0x10
#define IO5_LOW      0xef

#define COMM_SEC_HIGH       0x40      //1 -- 232   0 -- 485
#define COMM_SEC_LOW        0xbf      //1 -- 232   0 -- 485

#define SEND_485_HIGH       0x80      //1 -- 485 send enable   0 -- 485 send disable
#define SEND_485_LOW        0x7f      //1 -- 485 send enable   0 -- 485 send disable

#define MAX_ONLINE_NUM  200

//LTר��
#define SMARK_MONITOR 90

struct Local1{
               INT8U PreStatus;  //20130115 Ԥ״̬����ֹͬʱ�Ĳ���
               int Status;
               int TalkEndStatus;
               INT8U KeyTouchNo; //��ť���±�־
               //״̬ 0 ���� 1 ���жԽ�  2 ���жԽ�  3 ����  4 ������  5 ����ͨ��
               //6 ����ͨ��
               //30 �绰ͨ��
               int RecordPic;  //����Ƭ  0 ����  1 ��������Ƭ  2 ͨ������Ƭ
               int IFrameCount; //I֡����
               int IFrameNo;    //���ڼ���I֡
               int HavePicRecorded;  //����Ƭ��¼��

               char RemoteAddr[21];  //Զ�˵�ַ

               unsigned char AlarmByte[2];  //����

               int CallConfirmFlag; //���߱�־
               int Timer1Num;  //��ʱ��1����
               int OnlineFlag; //��������ȷ��
               int OnlineNum;  //����ȷ�����
               int TimeOut;    //���ӳ�ʱ,  ͨ����ʱ,  ���г�ʱ�����˽���
               int TalkTimeOut; //ͨ���ʱ��

               int PrevWindow;      //��һ�����ڱ��
               int TmpWindow;       //�ݴ洰�ڱ�� ���ڵ�������ʱ
               int CurrentWindow;   //��ǰ���ڱ��
               int ChildWindow;
               int isDisplayWindowFlag; //����������ʾ��
               int DefenceDelayFlag;    //������ʱ��־
               int DefenceDelayTime;   //����
               int PassLen;            //���볤��
               int AlarmDelayFlag[MAXCOUNT];    //������ʱ��־
               int AlarmDelayTime[MAXCOUNT];   //����

               int CalibratePos;   //У׼������ʮ��λ�� 0 1 2 3
               int CalibrateSucc;  //У׼�ɹ�
               unsigned char IP_Group[4];  //�鲥��ַ
               unsigned char Weather[3];   //����Ԥ��

               int AddrLen;          //��ַ����  S 12  B 6 M 8 H 6                 

               int isHost;           //'0' ���� '1' ���� '2' ...
               int ConnToHost;       //�������������� 1 ���� 0 ������
               int ConnToHostTime;
               unsigned char HostIP[4]; //����IP
               char HostAddr[21]; //����Addr
               int DenNum;             //Ŀ������  ����
               unsigned char DenIP[SUBMAXNUM][4];    //����IP
               char DenAddr[SUBMAXNUM][21];         //����Addr
               int DenFlag[SUBMAXNUM];              //�������ڱ�־
               int TmpDenNum;          //Ŀ�������ݴ�  ����
               unsigned char TmpDenIP[SUBMAXNUM][4];    //����IP
               char TmpDenAddr[SUBMAXNUM][21];          //����Addr

               int SendSyncFlag;  //����ͬ����Ϣ��־
               int SendSyncTime;  //����ͬ����Ϣ����

               char SoundName[6][50];  //�����ļ�����

               int NetLanStatus;   //Lan����״̬ 1 �Ͽ�  0 ��ͨ
               int OldNetSpeed;  //�����ٶ�                
               int NoBreak;     //����״̬ 1 ����  0 ����

               int ReportSend;  //�豸��ʱ����״̬�ѷ���
               int RandReportTime; //�豸��ʱ����״̬���ʱ��
               int ReportTimeNum;  //��ʱ

               int LcdLightFlag; //LCD�����־
               int LcdLightTime; //ʱ��
               int KeyPressTime;

               int NewInfo;  //������Ϣ
               int NewRecPic;  //������Ӱ

               int nowvideoframeno;   //��ǰ��Ƶ֡���
               int nowaudioframeno;   //��ǰ��Ƶ֡���

               int ForceEndWatch;  //�к���ʱ��ǿ�ƹؼ���

               int WatchTarget;  //����Ŀ��
               int WatchTargetMaxNum; //����Ŀ���������

               int WatchKeyTouched;     //���Ӽ� ��Ӧ
               int WatchKeyTouchedTime;  //���Ӽ� ��Ӧ ����

               int CenterKeyTouched;     //�������ļ� ��Ӧ
               int CenterKeyTouchedTime;  //�������ļ� ��Ӧ ����                         

               int TestTouch; //���Դ�����

               unsigned char OldVoiceHint;         //������ʾ  0 �ر�  1  ��
               unsigned char OldSpkVolume;         //SPK ����
               unsigned char OldMicVolume;         //MIC ����
               unsigned char OldTalkVoiceOpened;         //�Խ�ʱ����  0 �ر�  1  ��
               unsigned char OldTalkVolume;        //�Խ�ʱ����
               unsigned char OldScrSave[3];        //��Ļ����  0 �ر�  1  ��
               unsigned char OldBrightVolume;      //����
               unsigned char OldKeyVoice;          //������    0 �ر�  1  ��

               int SceneStatus;  //�龰ģʽ��ǰ״̬               

               int CurrRingSec;  //��ǰ��������ѡ����
               int isPlayRing;   //���ڲ�������

               int isRecordWav;  //����¼������
               int isPlayWav;    //���ڲ�������

               int BootNo;

               #ifdef _CAPTURESCREEN
                 int OpenLockKeyNum;
                 int OpenLockTime;
               #endif


               int TalkWatchReturnType;  //0 --  ������   1 -- Talk Watch ����
               int TalkWatchWinType;  //0 -- Talk����  1 -- Watch����
               int TalkWatchButtonNum;

               unsigned char LanguageNo;    //�����±�  0 -- ����    1 -- Ӣ��
               int ImageButton_Text_wd;     //��ť���ּ��

               int DefaultPage;

               int StateBar_Top_Flag; //����״̬����ʾ��־

               int Text_Space;  //�ı����

               INT8U ClearVideoWindowFlag; //����Ƶ���ڱ�־

               INT8U NsMultiAddr[4];    //NS�鲥��ַ

               INT8U Camera;      //0  -- ��  1 -- ov7725
               INT8U VideoType;
               INT8U MachineType;  //0x80  --- 8130

               INT8U  PlayNullVoice;

               INT8U Test_Disp_No;   //���Դ��� ��ʾ����
               INT8U Test_CurrPage;  //��ǰҳ
               INT8U Test_FrameRate; //����֡��
               INT8U Test_FrameSize; //�����С
               INT8U Test_VideoClosed;    //��Ƶ�ر�
               INT8U Test_LocalWatchFlag; //������Ƶ����
               INT8U Test_EncodeWatchFlag; //������Ƶ����������ʾ
               INT8U Test_PlayAudioFlag; //��������
               INT8U Test_RecAudioFlag; //¼��
               INT8U Test_VideoOutput;    //��Ƶ���  0  ����  1 �Լ�
               INT8U Test_PowerStayOnFreq;    //�͹��Ĵ�����Ƶ��
               INT8U Test_VideoSdramRefresh;    //LCD��ʾʱ SDRAMˢ������  0  8����  1 4����
               INT8U Test_OV7725FrameRate;    //OV7725 ���֡��

               INT8U DecodeFrameSize; //�����С
               INT16U DecodeVideoWidth;
               INT16U DecodeVideoHeight;

               INT8U DecodeStreamFlag;  //���ڽ�����
               INT8U DecodeJpegFlag;  //���ڽ⾲̬ͼƬ

               INT8U fb_page;  //��ʾҳ 0 - 1       ��ǰҳ
               INT8U fb_backpage;   //��̨ҳ
               INT32U fb_addr[2];

               INT8U Conflict_Flag;    //IP��ַ��ͻ
               INT8U Conflict_Mac[6];

               INT8U GpioWatchDogStarted;  //��GPIO����߳�������WatchDog
               INT8U DebugIP[4];
               INT8U DebugMac[6];
               INT8U RemoteDebugInfo;  //����Զ�̵�����Ϣ

               INT8U OpenDoorFlag; //������־ 0 δ����  1 ������ʱ��  2 ������
               INT16U OpenDoorTime; //ʱ��

               INT8U TalkFinishFlag;  //ͨ��������־     20101029  xu
               INT16U TalkFinishTime;  //ͨ����������

               INT8U TalkStartOpenLockFlag;  //ͨ��������־     20101029  xu
               INT16U TalkStartOpenLockTime;  //ͨ����������

               INT16U OnlineRecvNum;
               INT8U OnlineRecvAddr[MAX_ONLINE_NUM][4];

               INT8U OnlineTestFlag;  //���߼���־     20101029  xu
               INT16U OnlineTestTime;  //���߼�����

               INT32U OldEncodeErrorNum;                      //���������Ŀ

              #ifdef _REMOTECALLTEST  //Զ�̺��в���
               INT8U EnterTestMode;  //����ģʽ, 5���Զ�����
               INT8U CloseTestRing;  //����ģʽ, �ر�����
              #endif

              INT8U TalkDoorOpenPlayFlag;  //ͨ���������ſ�����ʾ

             #ifdef _NIGHT_CAMERA_ADJUST_SUPPORT   //�ⰵCAMERA�Զ�����֧��
              INT8U Night_IO;
             #endif
              };

#ifdef _BRUSHIDCARD_SUPPORT   //ˢ������֧��
//ID����Ϣ
struct IDCardNo1{
               INT32U Num;        //����
               INT8U *SN;//[IDCARDMAXNUM*BYTEPERSN_NEW]; //ID����  ���10����
               INT32U serialnum;     //���
              };
//дID����Ϣ
struct RecvIDCardNo1{
               INT8U isNewWriteFlag;  //��д��־
               INT32U Num;             //������
               INT16U PackNum;         //������
               INT8U PackIsRecved[IDCARDMAXNUM/CARDPERPACK + 1];  //���ѽ��ձ�־
               INT8U *SN;//[IDCARDMAXNUM*BYTEPERSN_NEW]; //ID����  ���10����
               INT32U serialnum;     //���
              };
//ˢID����Ϣ
struct BrushIDCard1{
               INT32U Num;        //����
               INT8U Info[IDCARDBRUSHNUM*11]; //ID����  ������Ϣ���1����
               //ǰ4�ֽڿ��ţ���7�ֽ�ʱ��
              };
//��дID�����ݽṹ
struct iddata1
  {
   #ifdef _IDCARDHAVEADDR      //ID������ַ
    char Addr[20];       //��ַ����
   #endif 
   INT32U serialnum;  //���
   INT32U  Num;          //������
   INT32U  CurrNum;           //��ǰ���ݰ�������
   INT32U  TotalPackage;      //�ܰ���
   INT32U  CurrPackage;       //��ǰ����
  };
#endif

#ifdef _CAPTUREPIC_TO_CENTER  //����ͼƬ��������
struct CapturePic1
              {
                INT8U isVilid;
                INT8U RecordPic;
                INT16U IFrameCount; //֡����
                INT8U IFrameNo;    //���ڼ���I֡
                INT16U Pic_Width;
                INT16U Pic_Height;
                INT8U *jpeg_pic;
                INT32U FileLen;
                INT16U FilePos;
                INT16U TotalPackage;
                INT16U CurrPackage;
                INT8U HavePicRecorded;  //����Ƭ��¼��
                char RemoteAddr[21];  //Զ�˵�ַ
                INT8U SendToCetnering;  //���ڴ�������
                INT8U ConnectCentered;   //����������״̬
                INT32U Capture_Total_Package;
                INT8U Capture_Send_Flag[60];
              };
#endif

struct LocalCfg1{
               char FlashHead[6];         //ͷ��������
               char Addr[20];             //��ַ����
               char AddrVerifyMa[10];         //������ͬ����
               INT8U Mac_Addr[6]; //������ַ
               INT8U IP[4];       //IP��ַ
               INT8U IP_Mask[4];  //��������
               INT8U IP_Gate[4];  //���ص�ַ
               INT8U IP_NS[4];    //NS�����ƽ�������������ַ
               INT8U IP_Server[4];  //����������ַ����NS��������Ϊͬһ����
               INT8U IP_Broadcast[4];  //�㲥��ַ

               int ReportTime;      //�豸��ʱ����״̬ʱ��
               INT8U DefenceStatus;       //����״̬      0 - ����  1 - ���   2 - �ڼ�   3 - ����   4 - ��   5 - �Զ���
                                                  //                        11 - ���   12 - �ڼ�   13 - ����   14 - ��   15 - �Զ���     ��ʱ
               INT8U DefenceNum;          //����ģ�����
               INT8U DefenceInfo[32][10]; //������Ϣ

               char EngineerPass[10];             //��������

               int In_DelayTime;                //������ʱ
               int Out_DelayTime;               //�����ʱ
               int Alarm_DelayTime;               //������ʱ
               int BellNum;                     //�������
               INT8U SoundSetup[8];          //����          0 -- 7
               INT8U RecPhoto;          //�ÿ���Ӱ          0 --���� 1 --����
               INT8U Language;          //����ѡ��    0 --�������� 1 --�������� 2 -- ENLISH
               INT8U RingType;          //��������  0 ��ͨ��  1  ��ͨ��  2  ҡ����   3  ������
               INT8U CallWaitRing;      //��������  0 ��ͨ��  1  ��ͨ��  2  ҡ����   3  ������

               INT8U VoiceHint;         //������ʾ  0 �ر�  1  ��
               INT8U SpkVolume;         //SPK ����
               INT8U MicVolume;         //MIC ����
               INT8U TalkVoiceOpened;         //�Խ�ʱ����  0 �ر�  1  ��
               INT8U TalkVolume;        //�Խ�ʱ����
               INT8U PhoneVoiceOpened;         //�绰ʱ����  0 �ر�  1  ��
               INT8U PhoneVolume;        //�绰ʱ����
               INT8U ScrSave[3];        //��Ļ����  0 �ر�  1  ��
               INT8U MainPage;          //��ҳ����  0 Ĭ�Ͻ���  1 �绰����
               INT8U BrightVolume;      //����
               INT8U ContrastVolume;    //�Աȶ�
               INT8U KeyVoice;          //������    0 �ر�  1  ��

               INT16 Ts_X0;                   //������
               INT16 Ts_Y0;
               INT16 Ts_deltaX;
               INT16 Ts_deltaY;
               INT16 Ts_Mode;                 //ģʽ  X   Y

               INT8U EncodeFrameRate; //����֡��
               INT8U EncodeFrameSize; //�����С
               INT8U VideoClosed;    //��Ƶ�ر�
               INT8U VideoOutput;    //��Ƶ���  0  ����  1 �Լ�

               INT8U VideoSdramRefresh;    //LCD��ʾʱ SDRAMˢ������  0  8����  1 4����

               INT8U MultiMachineSupport;  //��ֻ�֧��,��������Ч���粻֧�֣��򲻷��㲥��ѯ��
               INT8U DenMaxNum;  //���������������һ������С�� SUBMAXNUM�� �����ڼ�С�㲥��

               //��������
               INT8U DefenceSetup_dType[MAXDEFENCENUM];    //��������     2 - 24Сʱ����  3 - �Զ������
               INT8U DefenceSetup_dDelay[MAXDEFENCENUM];   //��ʱʱ��     0 - 0��   1 - 5��  2 - 10��  3 - 15��  4 - 20��  5 - 25��  6 - 60��
               INT8U DefenceSetup_tType[MAXDEFENCENUM];    //̽ͷ����     0 - �̸�  1 - ȼ��  2 - ����  3 - ����  4 - ����  5 - �Ŵ�  6 - ����

               INT8U Scene_Defence[5][MAXDEFENCENUM];      //�����龰ģʽ����

               char OpenLockPass[10]; //��������

               INT8U PassOpenLock;   //���뿪��
               INT8U CardOpenLock;   //ˢ������
               INT8U DoorButton;   //���Ű�ť
               INT8U DoorDetect;       //�Ŵż��
               INT8U OpenLockTime;   //����ʱ��  0 200ms  1 500ms  2 5s  3 10s
               INT8U DelayLockTime;   //��ʱ����  0 0s  1 3s  2 5s  3 10s
               INT8U TalkTimeOut;     //20140225 ���г�ʱʱ�� 15 30 60 99
               
               INT32U EncodeErrorNum;                      //���������Ŀ

               INT8U ResetStatus;
               INT8U Reset_DenNum;
               INT8U Reset_IP[10][4];     //�Է�IP
               INT8U Reset_Mac[10][6];    //�Է�Mac
               char Reset_Addr[10][21];   //�Է�Addr

               INT16U BootNo;
              #ifdef _SEND_TO_SELF_TEST    //�������ݰ����Լ�
               INT16U SelfBootNo;
              #endif

               INT8U MicDecValue; //չ�䡢չ��ģʽ,���С Mic����
              };

struct Remote1{
               INT8U DenNum;             //Ŀ������  ����+����
               INT8U LineUseNum;         //20101202 xu ռ������
               //INT8U DenMac[6]; //�Է�Mac
               //INT8U DenIP[4]; //�Է�IP
               INT8U GroupIP[4]; //GroupIP
               INT8U IP[10][4];    //�Է�IP
               INT8U Mac[10][6];    //�Է�Mac
               INT8U Added[10];                //�Ѽ�����
               char Addr[10][21];         //�Է�Addr
               int TimeOut[10];      //��ʱʱ�� 20131023
              };
struct TalkCall1{
                INT8U BuildFlag; //��
                char Build[5];
                INT8U BuildMaxlen;
                INT8U UnitFlag;  //��Ԫ
                char Unit[5];
                INT8U UnitMaxlen;
                INT8U RoomFlag;  //����
                char Room[5];
                INT8U RoomMaxlen;
                INT8U Len;
                char TalkText[15];
               };
#ifdef _MULTI_MACHINE_SUPPORT  //��ֻ�֧��
struct SyncSubData1
      {
               int DenNum;             //Ŀ������  ����
               unsigned char DenIP[SUBMAXNUM][4];    //����IP
               char DenAddr[SUBMAXNUM][22];         //����Addr

               //�������� ��������  *************
               unsigned char DefenceStatus;       //����״̬
               unsigned char DefenceNum;          //����ģ�����
               unsigned char DefenceInfo[32][10]; //������Ϣ

               char EngineerPass[10];             //��������
      };
#endif

//ͨ�����ݽṹ
struct talkdata1
  {
//   unsigned char Order;     //��������
   char HostAddr[20];       //���з���ַ
   INT8U HostIP[4]; //���з�IP��ַ
   char AssiAddr[20];       //���з���ַ
   INT8U AssiIP[4]; //���з�IP��ַ
   INT32U timestamp;  //ʱ���
   INT16U DataType;          //��������
   INT16U Frameno;           //֡���
   INT32U Framelen;            //֡���ݳ���
   INT16U TotalPackage;      //�ܰ���
   INT16U CurrPackage;       //��ǰ����
   INT16U Datalen;           //���ݳ���
   INT16U PackLen;       //���ݰ���С
  };

//UDP���ݷ��ͽṹ
struct Multi_Udp_Buff1{
               INT8U isValid; //�Ƿ���Ч
               INT8U SendMode; //ASK -- ��η���   REPLY --- ���η���
               INT8U SendNum; //��ǰ���ʹ���
               INT8U MaxSendNum; //���ʹ���
               INT8U CurrOrder;//��ǰ����״̬,VIDEOTALK  VIDEOWATCH
                               //��Ҫ���������ʱ���絥��������Ϊ0
               INT16U DestPort;
               INT8U MacValid;  //MAC��ַ��Ч
               INT8U ArpNum;  //ARP����
               INT8U RemoteMac[6];
               INT8U RemoteIP[4];
               INT8U *buf;//[1520];
              // INT8U buf[1520];
               INT16U TimeNum;
               INT16U DelayTime;  //�ȴ�ʱ��
               INT16U SendDelayTime; //���͵ȴ�ʱ�����  20101112
               INT16U nlength;
              };

//Audio UDP���ݷ��ͽṹ
struct Audio_Udp_Buff1{
               INT8U isValid; //�Ƿ���Ч
               INT8U SendMode; //ASK -- ��η���   REPLY --- ���η���
               INT8U SendNum; //��ǰ���ʹ���
               INT8U MaxSendNum; //���ʹ���
               INT8U CurrOrder;//��ǰ����״̬,VIDEOTALK  VIDEOWATCH
                               //��Ҫ���������ʱ���絥��������Ϊ0
               INT16U DestPort;
               INT8U MacValid;  //MAC��ַ��Ч
               INT8U ArpNum;  //ARP����
               INT8U RemoteMac[6];
               INT8U RemoteIP[4];
               INT8U buf[86 + 64];
               INT16U TimeNum;
               INT16U DelayTime;  //�ȴ�ʱ��
               INT16U SendDelayTime; //���͵ȴ�ʱ�����  20101112
               INT16U nlength;
              };
              
//COMM�����������ݷ��ͽṹ
struct Multi_Comm_Buff1{
               INT8U isValid; //�Ƿ���Ч
               INT8U SendNum; //��ǰ���ʹ���
               INT8U m_Comm;
               INT8U buf[256];
               INT16U nlength;
              };

struct PicStatBuf1{
               INT8U Flag;                      //��ʱ����ʾ��Ϣ��־
               INT8U Type;                      //����
               INT16U Time;
               INT16U MaxTime;                   //�ʱ��
              };

struct Info1{
               char FlashHead[8]; //Flash�Ѵ��־
               INT16U MaxNum;   //�����Ϣ��
               INT16U TotalNum; //��Ϣ����
               INT16U NoReadedNum; //δ����Ϣ����
               INT16U TotalInfoPage;   //����Ϣҳ��
               INT16U CurrentInfoPage; //��ǰ��Ϣҳ
               INT16U CurrNo;    //��ǰ��Ϣ���
               INT16U CurrPlayNo;  //��ǰ�������
              };

//������Ϣ���ݽṹ��
struct InfoContent1{
               INT8U isValid;  //��Ч��δɾ����־   1
               INT8U isReaded; //�Ѷ���־    1
               INT8U HavePic;  //����ͼ���־  0   1 -- 640x480  2 -- 320x240
               char RemoteAddr[21];
               char Time[32];    //����ʱ��    32
               INT8U Type;     //����        1    ��Ϣ���ͻ��¼�����     0 -- ���С���1 -- ����δ����  2 -- ���н���
               INT32U Sn;      //���        4
               INT16U Length;      //����        4
              };                               //�ڴ����Ϊ444

struct downfile1
  {
   INT8U FlagText[20];     //��־�ַ���
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

#ifndef CommonH
#define CommonH
  //����״̬����
  struct Local1 Local;
  struct LocalCfg1 LocalCfg;

  INT32U time_count;  
#ifdef _BRUSHIDCARD_SUPPORT   //ˢ������֧��
   INT8U IDCardReadFlag;
   INT8U IDCardHaveRead[IDCARD_MAXPACK];
   INT8U IDCardReadIP[4];
   //ID����Ϣ
   struct IDCardNo1 IDCardNo;
   //ˢID����Ϣ
   struct BrushIDCard1 BrushIDCard;
   //дID����Ϣ
   struct RecvIDCardNo1 RecvIDCardNo;
#endif

#ifdef _CAPTUREPIC_TO_CENTER  //����ͼƬ��������
  struct CapturePic1 CapturePic;
#endif

  int DeltaLen;  //���ݰ���Ч����ƫ����
  //Զ�˵�ַ
  struct Remote1 Remote;
  char NullAddr[21];   //���ַ���

  char UdpPackageHead[15];
  char UdpPackageHead2[15];
  INT16U LocalPort[PORT_NUM];
  INT16U RemotePort[PORT_NUM];

  //20140225
  INT8U DelayTalkTimeOut[4]= {15, 30, 60, 90};// 


  struct Multi_Udp_Buff1 Multi_Udp_Buff[UDPSENDMAX]; //UDP���ͻ���
  struct Audio_Udp_Buff1 Audio_Udp_Buff[AUDIOSENDMAX]; //Audio UDP���ͻ���

  struct PicStatBuf1 PicStatBuf;

  INT8U CurrBox;

  //��������
  struct TImage setup_image;
  struct TImage door_type_image[2];
  struct TImage net_image[2];
  struct TEdit setup_pass_edit;

  //����������
  struct TImage setupmain_image;
  struct TImage setupmain_param_image[6];

  //��ַ���ô���
  struct TImage addr_image;
  struct TEdit addr_edit[6];
  struct TImage edit_status_image[2];

  //�������ô���
  struct TImage param_image;
  struct TImage param_line_image[2];
  struct TImage curr_row_image[2];

  //��������
  struct TImage pass_image;
  struct TEdit pass_edit[4];

  //�������ô���
  struct TImage other_image;

  //���߼�ⴰ��
  struct TImage onlinetest_image;
  struct TImage online_content_image;
  struct TEdit online_edit[4];

  //����
  struct TImage setup_info_image;

  //������
  struct TImage talk1_image;    //���ӶԽ�����ͼ�� 

  //20131128 LOGO
  struct TImage logo_image;    //LOGO

  //���������д���
  struct TImage talk_connect_image;
  struct TImage talk_connect[4];

  //ͨ������
  struct TImage talk_start_image;
  struct TImage talk_start_openlock[2];

  struct TImage openlock_image;
  struct TImage locking_image;
  struct TImage openlock_result_image;

  //��������
  struct TEdit r2r_edit[3];
  struct TEdit openlock_edit;

  struct TImage small_num_image[10];
  struct TImage big_num_image[10];

  //���Ӵ���
  struct TImage watch_image;

  struct TImage bitstream_image[6];
  struct TImage error_image;

  struct TAudio soundtip_wav;
  struct TAudio ring_wav;
  struct TAudio backring_wav[3];
  struct TAudio modisucc_wav[2];
  struct TAudio modifail_wav[2];
  struct TAudio inputerror_wav[2];
  struct TAudio passerror_wav[2];
  struct TAudio dooropen_wav[2];
  struct TAudio doordect_wav;
  struct TAudio card_ok_wav;
  struct TAudio card_fail_wav;
  struct TAudio null_wav;

  struct TAudio test1k_wav;


#else
  //extern struct ethbuf ebuf;
  //����״̬����
  extern struct Local1 Local;
  extern struct LocalCfg1 LocalCfg;

  extern INT32U time_count;
#ifdef _BRUSHIDCARD_SUPPORT   //ˢ������֧��
   extern INT8U IDCardReadFlag;
   extern INT8U IDCardHaveRead[IDCARD_MAXPACK];
   extern INT8U IDCardReadIP[4];
   //ID����Ϣ
   extern struct IDCardNo1 IDCardNo;
   //ˢID����Ϣ
   extern struct BrushIDCard1 BrushIDCard;
   //дID����Ϣ
   extern struct RecvIDCardNo1 RecvIDCardNo;
#endif

#ifdef _CAPTUREPIC_TO_CENTER  //����ͼƬ��������
  extern struct CapturePic1 CapturePic;
#endif

  extern int DeltaLen;  //���ݰ���Ч����ƫ����
  //Զ�˵�ַ
  extern struct Remote1 Remote;
  extern char NullAddr[21];   //���ַ���

  extern char UdpPackageHead[15];
    extern char UdpPackageHead2[15];
  extern INT16U LocalPort[PORT_NUM];
  extern INT16U RemotePort[PORT_NUM];
  
  //20140225
  extern INT8U DelayTalkTimeOut[4];// = {15, 30, 60, 90};


  extern struct Multi_Udp_Buff1 Multi_Udp_Buff[UDPSENDMAX]; //UDP���ͻ���
  extern struct Audio_Udp_Buff1 Audio_Udp_Buff[AUDIOSENDMAX]; //Audio UDP���ͻ���

  extern struct PicStatBuf1 PicStatBuf;

  extern INT8U CurrBox;

  //��������
  extern struct TImage setup_image;
  extern struct TImage door_type_image[2];
  extern struct TImage net_image[2];
  extern struct TEdit setup_pass_edit;

  //����������
  extern struct TImage setupmain_image;
  extern struct TImage setupmain_param_image[6];

  //��ַ���ô���
  extern struct TImage addr_image;
  extern struct TEdit addr_edit[6];
  extern struct TImage edit_status_image[2];

  //�������ô���
  extern struct TImage param_image;
  extern struct TImage param_line_image[2];
  extern struct TImage curr_row_image[2];

  //��������
  extern struct TImage pass_image;
  extern struct TEdit pass_edit[4];

  //�������ô���
  extern struct TImage other_image;

  //���߼�ⴰ��
  extern struct TImage onlinetest_image;
  extern struct TImage online_content_image;
  extern struct TEdit online_edit[4];

  //����
  extern struct TImage setup_info_image;

  //������
  extern struct TImage talk1_image;    //���ӶԽ�����ͼ��  ��Ԫ

  //20131128 LOGO
  extern struct TImage logo_image;    //LOGO

  //���������д���
  extern struct TImage talk_connect_image;
  extern struct TImage talk_connect[4];

  //ͨ������
  extern struct TImage talk_start_image;
  extern struct TImage talk_start_openlock[2];

  extern struct TImage openlock_image;
  extern struct TImage locking_image;
  extern struct TImage openlock_result_image;

  //��������
  extern struct TEdit r2r_edit[3];
  extern struct TEdit openlock_edit;

  extern struct TImage small_num_image[10];
  extern struct TImage big_num_image[10];

  //���Ӵ���
  extern struct TImage watch_image;

  extern struct TImage bitstream_image[6];
  extern struct TImage error_image;

  extern struct TAudio soundtip_wav;
  extern struct TAudio ring_wav;
  extern struct TAudio backring_wav[3];
  extern struct TAudio modisucc_wav[2];
  extern struct TAudio modifail_wav[2];
  extern struct TAudio inputerror_wav[2];
  extern struct TAudio passerror_wav[2];
  extern struct TAudio dooropen_wav[2];
  extern struct TAudio doordect_wav;
  extern struct TAudio card_ok_wav;
  extern struct TAudio card_fail_wav;

  extern struct TAudio null_wav;

  extern struct TAudio test1k_wav;

#endif