
#include "x_printf.h"
#include "x_disp.h"
#include "x_flash.h"

#include "x_touch.h"
#include "x_audio.h"
#include "x_video.h"

#include "x_main.h"
#include "x_lcd.h"
#include "x_comm.h"
#include "x_gpio.h"

#include "x_net.h"
#include "x_ip.h"
#include "x_udp.h"

#include "x_zt2083.h"
#include "x_es8388.h"
#include "x_ov7725.h"

#include "x_first.h"
#include "x_info.h"
#include "x_record.h"
#include "x_setup.h"
#include "x_talk.h"
#include "x_alarm.h"
//#include "x_test.h"
#include "m_init.h"

//2013.08.30����
#include "x_splash.h"
#include "x_switch.h"

/*
****************************************
2013.8.29
�����µĿ�������
20131008
�����ṹ 
20131015
ʱ����ṹ
201311
�������к�
Local1.AlarmStatus 20131010 ����״̬
Local1.WorkStatus 201311 ��֤��
���Ӳ���״̬��ť���� 20131010
#define TIMERCLOCK 12000      ʱ�� 1����  201310������ʱ������

2013.11.27 ���Ӻ��а�ť

****************************************
*/


#define VIDEO_VGA  0
#define VIDEO_QVGA  1

#define LOCAL_VIDEO   0
#define REMOTE_VIDEO  1

#define _REMOTECALLTEST  //Զ�̺��в���
//#define _ADDTABLE_SUPPORT  //��ַ��֧��
#define _MULTI_MACHINE_SUPPORT  //��ֻ�֧��
//#define _CAMERA_SUPPORT     //�粻������ͷ��GPIOB6��7ΪComm1, GPIOC ��Ϊ8������ʹ��   ����ģʽ
//#define _SEND_TO_SELF_TEST    //�������ݰ����Լ�
#define _REMOTE_FORTIFY_SUPPORT  //Զ�̲�����֧��
#define _MULTI_CENTER_SUPPORT  //�����Ļ�֧��   20130613

#define ELEVATORCONTROL_HANGZHOU      //20120822
#ifdef ELEVATORCONTROL_HANGZHOU      //20120822
  #define ELEVATR_ORDER1  181
#endif

//Flash secter ɾ��ÿ��Ϊ4K�� ���Դ洢��ַ��Ϊ 4K�ı���
#define INFO_ADDR       0x7f8000   //��Ϣ�洢��ַ   ��һ�� 512 Ϊ����  ��20��512 Ϊ��Ϣ��
#define LOCALCFG_ADDR   0x7ff000   //�����ļ��洢��ַ
#define LOCALCFG_ADDR1   0x7fe000   //�����ļ��洢��ַ2  ������

#define RECORDCFG_ADDR       0x7f0000   //ͨ����¼�洢��ַ   ��һ�� 512 Ϊ����  ��20��512 Ϊͨ����¼��
#define RECORDPIC_ADDR       0x570000   //ͨ����¼��Ƭ�洢��ַ   ÿ64K һ����Ƭ, ���20��

#define SWITCH_ADDR    0x0d9000

#ifdef _ADDTABLE_SUPPORT  //��ַ��֧��
 #define ADDRTABLE_ADDR       0x790000   //��ַ��   ���� 0x50000  ��  0x7effff
 #define ADDR_MAXNUM          10000      //��ַ���������
#endif

#define FLAGTEXT   "xtm8130sdkkkkdfdsIM"
#define HARDWAREVER "S-HW VER 4.0.2 For 8130"    //Ӳ���汾
#define SOFTWAREVER "S-SW70A VER 4.2.0"   //�����汾
#define SERIALNUM "20131111A70"    //��Ʒ���к�

#define IMAGEUP  0
#define IMAGEDOWN  1

#define TOUCH_WRITE_ADDR     0x90      //������I2C��ַ read:10010001 write 10010000
#define TOUCH_READ_ADDR      0x91      //������I2C��ַ read:10010001 write 10010000

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
#define TIMERCLOCK 12000      //ʱ�� 1����  201310

#define IDLE_TIME      30      //���з���������ʱ��
#define CLOSELCD_TIME  60//10 //60      //������LCDʱ��
#define POWERSTAY_TIME  30 //500 // 30      //��LCD�� ����͹���ʱ��

#define NO_CYCLE_PLAY  0   //��Ƶ���β���
#define CYCLE_PLAY  1   //��Ƶѭ������
#define KEYDELAYTIME  30      //��ťѹ��ʱ��  150ms

#define PCM_REC_MAX  0x2000  //PCM¼�����������ֵ
#define PCM_BUF_NUM  2      //PCM��������ռ�ڴ�ҳ������64K*2
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
#define VERSIONPERROW  10          //ҳ����

//ͨ����¼
#define RECORDMAXITEM   20  //ͨ����¼�������
#define RECORDNUMPERPAGE  5  //һҳ��ʾͨ����¼��

#define DIRECTCALLTIME  600                 //ֱ�Ӻ���ÿ��ʱ��
#define WATCHTIMEOUT  (30*TIMERPERSEC)    //(65500*TIMERPERSEC) //�����ʱ��   600
#define CALLTIMEOUT  (30*TIMERPERSEC)     //�����ʱ��   250
#define TALKTIMEOUT  (120*TIMERPERSEC)    //130  30*20   //ͨ���ʱ��      600
#define ADTIMEOUT  (250*TIMERPERSEC)    //130  30*20   //AD�ʱ��      600  // 20140317

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

#ifdef _REMOTE_FORTIFY_SUPPORT  //Զ�̲�����֧��
 #define REMOTE_FORTIFY   160    //Զ�̲�����
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

#define TALK_PW_TAKE  210   //�¹��ܣ��Ժ�У������
#define TALK_PW_VEFY  211   //�¹��ܣ��Ժ�У��������֤

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

#define _TEST_BTN_NUM  1 //31

#define SET_LOW             0x0
#define SET_HIGH            0x1

#define FORTIFY_LED_HIGH      0x20    //GPIOD5
#define FORTIFY_LED_LOW       0xdf

#define INFO_LED_HIGH         0x40    //GPIOD6
#define INFO_LED_LOW          0xbf

//#define NET_LED_HIGH       0x80    //GPIOD7
//#define NET_LED_LOW        0x7f
//2013.09.10 GPIOD7תGPIOD3
#define NET_LED_HIGH       0x08    //GPIOD4 08/F7
#define NET_LED_LOW        0xf7

#define ALARM_OUT_HIGH     0x20
#define ALARM_OUT_LOW      0xdf

//LTר��
#define LOGO_BTNIMG_COUNT 2
#define MENU_BTN_COUNT  6
#define STATE_COUNT 8
#define SPLASH_COUNT  60
#define AD_PIC_COUNT  26

#define SMART_SWITCH  81
#define SMART_AD  88
#define SMART_P2P  89

//20131220
//#define FT5316
//20140113
#define SMART_BELL
//20140115
//#define MAIN_SPLASH
//20140115
//#define MAIN_SERIALNO



//�����ṹ 201310
struct AdPic1
{
    int PicCount;
	INT32U AdPicAdd[10];
};

//�������к� 201311
struct SerialNo1{
	char head[10];
	char no[20];
	char pw[5];
};

//ʱ����ṹ 201310
struct DataTime1{
	int second;
	int minute;
	int hour;
	int day;
	int month;
	int year;
};

//������Ϣ��ṹ 201311
struct DefenceInfo1{
	INT8U DefenceType[MAXDEFENCENUM];
	INT8U DefenceDelayTime[MAXDEFENCENUM];
	INT8U ObjectType[MAXDEFENCENUM];
};

//������Ϣ
struct SwitchInfo1
{
    /*
    char SwitchCount[5];
    
    char SwitchNo[50][5];
    
    char SwitchType1[50][2];
    char SwitchEnable[50][2];
    char SwitchState[50][2];
    char SwitchType2[50][2];
    
    char SwitchName[50][10];
    */

    INT8U SwitchCount;
    INT8U SwitchNo[50];
    INT8U SwitchType1[50];
    INT8U SwitchEnable[50];
    INT8U SwitchState[50];
    INT8U SwitchType2[50];

    char SwitchName[50][10];

    
};

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
               INT8U ManualRecordFlag; //�ֶ�¼��

               char RemoteAddr[21];  //Զ�˵�ַ
               char RemotePW[5];    //Զ������

               unsigned char AlarmByte[2];  //����

               int CallConfirmFlag; //���߱�־
               int Timer1Num;  //��ʱ��1����
               int OnlineFlag; //��������ȷ��
               int OnlineNum;  //����ȷ�����
               INT16U TimeOut;    //���ӳ�ʱ,  ͨ����ʱ,  ���г�ʱ�����˽���,ad 
               int TalkTimeOut; //ͨ���ʱ��

               //20140317
               int StreamOnlineFlag;
               int StreamTimeOut; 
               
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
               int AlarmStatus;                //20131010 ����״̬

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
			   int WorkStatus;   //201311 ��֤��
               int CloudStatus;
               
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

               INT8U VideoWindowFlag;  //��Ƶ���ڴ򿪱�־
              #ifdef _REMOTECALLTEST  //Զ�̺��в���
               INT8U EnterTestMode;  //����ģʽ, 5���Զ�����
               INT8U CloseTestRing;  //����ģʽ, �ر�����
              #endif

               INT8U FirstWindowFlag;     //������ҳ��־
               INT16U FirstWindowTime;  //������ҳ ����

              #ifdef ELEVATORCONTROL_HANGZHOU      //20120822
               char Elevator_Addr[21];
               INT8U Elevator_IP[4];
              #endif               
              };

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
               int DenNum;             //Ŀ������  ����     2
               unsigned char DenIP[SUBMAXNUM][4];    //����IP    12
               char DenAddr[SUBMAXNUM][22];         //����Addr   66

               //�������� ��������  *************
               unsigned char DefenceStatus;       //����״̬     1
               unsigned char DefenceNum;          //����ģ�����    1
               unsigned char DefenceInfo[32][10]; //������Ϣ       320

               char EngineerPass[10];             //��������       10
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

struct displayinfo1
 {
  INT16U totalpage;
  INT16U pageno;
  INT16U totalrow;
  char content_row[MAXROW][INFOROWLEN + 2];
  INT16U isDeleted;  //����ʾ��Ϣ����ʱɾ������Ϣ
 };


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

  //20131008 ���ṹ���³���
  //struct AdPic1 AdPic; 

  //20131015 ʱ�䡢�³���
  //struct DataTime1 DataTime;

  //
  //struct SerialNo1 SerialNo;

  //
  struct DefenceInfo1 DefenceInfo;

  //
  struct SwitchInfo1 SwitchInfo;
  
  int DeltaLen;  //���ݰ���Ч����ƫ����
  //Զ�˵�ַ
  struct Remote1 Remote;
  char NullAddr[21];   //���ַ���

  //char *abc;   //����1
  //char date[16];  //ʱ���ַ���
  //char temp[5];  //��ʱ�ַ���

  char UdpPackageHead[15];
  char UdpPackageHead2[15];
  INT16U LocalPort[PORT_NUM];
  INT16U RemotePort[PORT_NUM];

  struct Multi_Udp_Buff1 Multi_Udp_Buff[UDPSENDMAX]; //UDP���ͻ���
  struct Audio_Udp_Buff1 Audio_Udp_Buff[AUDIOSENDMAX]; //Audio UDP���ͻ���  

  struct PicStatBuf1 PicStatBuf;

  //��Ƶͼ��
  //struct TImage video_image;

  //2013.8.29  
  //20130901 ��Ϊ����
  //20140113 ����Ϊ����ʽ
  //struct TImage splash_image[SPLASH_COUNT];   
  
  //struct TImage ad_image[AD_PIC_COUNT];    //20130916 �������
  
  //������
  struct TImage main_image;
  struct TImageButton menu_button[MENU_BTN_COUNT];  //2013.8.30����������һ��ͼ��5-6
  //struct TImage logo_image[LOGO_BTNIMG_COUNT];
  struct TImage logo_image;
  struct TImage state_image[STATE_COUNT];

  struct TImage top_image;
  struct TImage middle_image;
  struct TImage bottom_image;
  
  //20131010 ���Ӳ���״̬��ť���� 7+2
  struct TImageButton bigmenu_button[9];   

  //
  struct TImageButton switch_in_button[50];
    //��ť���ô���
  struct TImageButton switch_set_button[12];
    
  //��������
  struct TImageButton num_button[15];
  struct TEdit setup_pass_edit;
  struct TImage setup_key_image;

  //����������
  struct TImageButton setup_button[5];

  //���̴���
  struct TImage kb_image;
  struct TEdit kb_edit;
  struct TImageButton kb_button[15];

  //LAN���ô���
  struct TImage lan_image;
  struct TImageButton lan_button[2];
  struct TEdit lan_edit[5];

  //�������ô���
  struct TImage addr_image;
  struct TImageButton addr_button[2];
  struct TEdit addr_edit[8];

  //�޸����봰��
  struct TImage pass_image;
  struct TImageButton pass_button[2];
  struct TEdit pass_edit[3];

  //��Ļ���ڴ���
  struct TImage screen_image;
  struct TImageButton screen_button[3];
  struct TImage brightness_image[6];

  //�������ڴ���
  struct TImage voice_image;
  struct TImageButton voice_button[5];
  struct TImage spk_image[6];
  struct TImage mic_image[6];

  //���ڴ���
  struct TImageButton version_button[2];  

  //������У׼����
  struct TImage calibrate_image;
  struct TImageButton cross_button[1];

  //��������
  struct TImage call_image;
  struct TEdit r2r_edit;

  //���Ӵ���
  struct TImage watchtarget_image;
  struct TImageButton watch_button[3];

  //���봰��
  struct TImage talkpic_image;
  struct TImageButton talkpic_button[3];

  //������Ƶ����
  struct TImage watchpic_image;
  struct TImageButton watchpic_button[1];

  //��������
  struct TImageButton fortify_button[2];

  //��������
  struct TEdit cancelfortify_edit;                //���봰���ı���

  //����ͼ��
  struct TImage alarm_main_image;
  struct TImage alarm_image[2];

  //��Ϣ����
  struct TImage info_image;
  struct TImageButton info_button[3];
  struct TImageButton inforow_button[INFONUMPERPAGE];

  //��Ϣ�鿴����
  struct TImage infowin_image;
  struct TImageButton infowin_button[3];
  struct TImage infocontent_image;

  //����Ϣ�ṹ
  struct Info1 Info;                                       // INFOTYPENUM      INFOMAXITEM
  struct InfoContent1 InfoContent[INFOMAXITEM];
  char *InfoBuff[INFOMAXITEM];  

  //ͨ����¼����
  struct TImage records_image;
  struct TImageButton records_button[3];
  struct TImageButton recordsrow_button[RECORDNUMPERPAGE];
  struct TImageButton recordsicon_button[RECORDNUMPERPAGE];
  struct TImage records_pic_icon[RECORDNUMPERPAGE];
  struct TImageButton records_call_icon[RECORDNUMPERPAGE];  //20131127���а�ť

  //ͨ����¼�鿴����
  struct TImage picwin_image;
  struct TImageButton picwin_button[3];
  struct TImage pic_blank_image;

  //ͨ����¼�ṹ
  struct Info1 Records;                                       // INFOTYPENUM      INFOMAXITEM
  struct InfoContent1 RecordsContent[RECORDMAXITEM];
  INT32U RecordsBuff[RECORDMAXITEM];                         //��Ƭ�洢��ַ
  struct InfoContent1 TmpRecordsContent;  //��ʱ�ṹ

  //���Դ���
  struct TImage test_image;
  struct TImage test_image1;
  struct TImage label_null_image;
  struct TImageButton test_button[_TEST_BTN_NUM];

  struct TImage bitstream_image[6];
  struct TImage error_image;
  struct TImage main_image11111;


  
  struct TAudio soundtip_wav;
  struct TAudio soundtip2_wav;
  struct TAudio ring_wav;
  struct TAudio backring_wav;
  struct TAudio modisucc_wav;
  struct TAudio modifail_wav;
  struct TAudio inputerror_wav;
  struct TAudio passerror_wav;
  struct TAudio cancelfortify_wav;
  struct TAudio defencedelay_wav;
  struct TAudio fortify_wav;
  struct TAudio cancelalarm_wav;
  struct TAudio alarmdelay_wav;
  struct TAudio alarm_wav;

  struct TAudio null_wav;

  //struct TAudio wav_1k[60];

//20130902 ��������
  struct TAudio splash_wav;

//20131205 ������
  struct TAudio doorbell_wav;


#else
  //extern struct ethbuf ebuf;
  //����״̬����
  extern struct Local1 Local;
  extern struct LocalCfg1 LocalCfg;

  //20131008 ����
  //extern struct AdPic1 AdPic; 

  //20131015 ����
  //extern struct DataTime1 DataTime;

  //
  //extern struct SerialNo1 SerialNo;

  //
  //extern struct DefenceInfo1 DefenceInfo;

  //
  extern struct SwitchInfo1 SwitchInfo;
   
  extern int DeltaLen;  //���ݰ���Ч����ƫ����
  //Զ�˵�ַ
  extern struct Remote1 Remote;
  extern char NullAddr[21];   //���ַ���

  //extern char *abc;

  extern char UdpPackageHead[15];
  extern char UdpPackageHead2[15];
  extern INT16U LocalPort[PORT_NUM];
  extern INT16U RemotePort[PORT_NUM];

  extern struct Multi_Udp_Buff1 Multi_Udp_Buff[UDPSENDMAX]; //UDP���ͻ���
  extern struct Audio_Udp_Buff1 Audio_Udp_Buff[AUDIOSENDMAX]; //Audio UDP���ͻ���  

  extern struct PicStatBuf1 PicStatBuf;

  //��Ƶͼ��
  //extern struct TImage video_image;
  
  //20130829 �������� 
  //20130901 ��Ϊ����
  //extern struct TImage splash_image[SPLASH_COUNT];

  //extern struct TImage ad_image[AD_PIC_COUNT];    //20130916 �������

  //������
  extern struct TImage main_image;
  
  //2013.8.30����������һ����ť 5-6
  extern struct TImageButton menu_button[MENU_BTN_COUNT];
  
  //extern struct TImage logo_image[LOGO_BTNIMG_COUNT];
  extern struct TImage logo_image;

  extern struct TImage state_image[STATE_COUNT];

  extern struct TImage top_image;
  extern struct TImage middle_image;
  extern struct TImage bottom_image;

  //20131010 ���Ӳ���״̬��ť���� 7+2
  extern struct TImageButton bigmenu_button[9];

  extern struct TImageButton switch_in_button[8];
  //��ť���ô���
  extern struct TImageButton switch_set_button[12];


  //��������
  extern struct TImageButton num_button[15];
  extern struct TEdit setup_pass_edit;
  extern struct TImage setup_key_image;

  //����������
  extern struct TImageButton setup_button[5];

  //���̴���
  extern struct TImage kb_image;
  extern struct TEdit kb_edit;
  extern struct TImageButton kb_button[15];

  //LAN���ô���
  extern struct TImage lan_image;
  extern struct TImageButton lan_button[2];
  extern struct TEdit lan_edit[5];

  //�������ô���
  extern struct TImage addr_image;
  extern struct TImageButton addr_button[2];
  extern struct TEdit addr_edit[8];

  //�޸����봰��
  extern struct TImage pass_image;
  extern struct TImageButton pass_button[2];
  extern struct TEdit pass_edit[3];

  //��Ļ���ڴ���
  extern struct TImage screen_image;
  extern struct TImageButton screen_button[3];
  extern struct TImage brightness_image[6];

  //�������ڴ���
  extern struct TImage voice_image;
  extern struct TImageButton voice_button[5];
  extern struct TImage spk_image[6];
  extern struct TImage mic_image[6];

  //���ڴ���
  extern struct TImageButton version_button[2];

  //������У׼����
  extern struct TImage calibrate_image;
  extern struct TImageButton cross_button[1];

  //��������
  extern struct TImage call_image;
  extern struct TEdit r2r_edit;

  //���Ӵ���
  extern struct TImage watchtarget_image;
  extern struct TImageButton watch_button[3];

  //���봰��
  extern struct TImage talkpic_image;
  extern struct TImageButton talkpic_button[3];

  //������Ƶ����
  extern struct TImage watchpic_image;
  extern struct TImageButton watchpic_button[1];

  //��������
  extern struct TImageButton fortify_button[2];

  //��������
  extern struct TEdit cancelfortify_edit;                //���봰���ı���

  //����ͼ��
  extern struct TImage alarm_main_image;
  extern struct TImage alarm_image[2];

  //��Ϣ����
  extern struct TImage info_image;
  extern struct TImageButton info_button[3];
  extern struct TImageButton inforow_button[INFONUMPERPAGE];

  //��Ϣ�鿴����
  extern struct TImage infowin_image;
  extern struct TImageButton infowin_button[3];
  extern struct TImage infocontent_image;

  //����Ϣ�ṹ
  extern struct Info1 Info;                                       // INFOTYPENUM      INFOMAXITEM
  extern struct InfoContent1 InfoContent[INFOMAXITEM];
  extern char *InfoBuff[INFOMAXITEM];            //��Ϣ����

  //ͨ����¼����
  extern struct TImage records_image;
  extern struct TImageButton records_button[3];
  extern struct TImageButton recordsrow_button[RECORDNUMPERPAGE];
  extern struct TImageButton recordsicon_button[RECORDNUMPERPAGE];
  extern struct TImage records_pic_icon[RECORDNUMPERPAGE];
  extern struct TImageButton records_call_icon[RECORDNUMPERPAGE]; //20131127���а�ť


  //ͨ����¼�鿴����
  extern struct TImage picwin_image;
  extern struct TImageButton picwin_button[3];
  extern struct TImage pic_blank_image;

  //ͨ����¼�ṹ
  extern struct Info1 Records;                                       // INFOTYPENUM      INFOMAXITEM
  extern struct InfoContent1 RecordsContent[RECORDMAXITEM];
  extern INT32U RecordsBuff[RECORDMAXITEM];                         //��Ƭ�洢��ַ
  extern struct InfoContent1 TmpRecordsContent;  //��ʱ�ṹ

  //���Դ���
  extern struct TImage test_image;
  extern struct TImage test_image1;
  extern struct TImage label_null_image;
  extern struct TImageButton test_button[_TEST_BTN_NUM];

  extern struct TImage bitstream_image[6];
  extern struct TImage error_image;
  extern struct TImage main_image11111;
  


  extern struct TAudio soundtip_wav;
  extern struct TAudio soundtip2_wav;
  extern struct TAudio ring_wav;
  extern struct TAudio backring_wav;
  extern struct TAudio modisucc_wav;
  extern struct TAudio modifail_wav;
  extern struct TAudio inputerror_wav;
  extern struct TAudio passerror_wav;
  extern struct TAudio cancelfortify_wav;
  extern struct TAudio fortify_wav;
  extern struct TAudio cancelalarm_wav;
  extern struct TAudio defencedelay_wav;
  extern struct TAudio alarmdelay_wav;
  extern struct TAudio alarm_wav;

  extern struct TAudio null_wav;

  //extern struct TAudio wav_1k[60];
  
  //20130902 ��������
  extern struct TAudio splash_wav;

   //20131205 ������
  extern struct TAudio doorbell_wav;
  
#endif