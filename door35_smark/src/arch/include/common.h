
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

//20140225 呼叫超时时间 15 30 60 99

#define VIDEO_VGA  0
#define VIDEO_QVGA  1

#define LOCAL_VIDEO   0
#define REMOTE_VIDEO  1

#define _REMOTECALLTEST  //远程呼叫测试
//#define _ADDTABLE_SUPPORT  //地址表支持
#define _MULTI_MACHINE_SUPPORT  //多分机支持
#define _CAMERA_SUPPORT     //如不用摄像头，GPIOB6、7为Comm1, GPIOC 作为8个防区使用
#define _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
#define _BRUSHIDCARD_SUPPORT   //刷卡功能支持
#define _MULTI_CENTER_SUPPORT  //多中心机支持   20130613
#define _NIGHT_CAMERA_ADJUST_SUPPORT   //光暗CAMERA自动调节支持   20130626

#define ELEVATORCONTROL_HANGZHOU      //20120822
#ifdef ELEVATORCONTROL_HANGZHOU      //20120822
  #define ELEVATR_ORDER1  181
#endif
//#define _SEND_TO_SELF_TEST    //发送数据包给自己

//#define _TESTNSSERVER   //测试服务器解析模式

//Flash secter 删除每块为4K， 所以存储地址设为 4K的倍数
#define INFO_ADDR       0x7f8000   //信息存储地址   第一个 512 为配置  后20个512 为信息条
#define LOCALCFG_ADDR   0x7ff000   //配置文件存储地址
#define LOCALCFG_ADDR1   0x7fe000   //配置文件存储地址2  备份用

#define RECORDCFG_ADDR       0x7f0000   //通话记录存储地址   第一个 512 为配置  后20个512 为通话记录条
#define RECORDPIC_ADDR       0x570000   //通话记录相片存储地址   每64K 一张照片, 最多20张

#ifdef _ADDTABLE_SUPPORT  //地址表支持
 #define ADDRTABLE_ADDR       0x790000   //地址表   长度 0x50000  到  0x7effff
 #define ADDR_MAXNUM          10000      //地址表最大数量
#endif

#define FLAGTEXT   "xtm8130sdkkkkdfdsIM"
#define HARDWAREVER "M-HW VER 4.0.2 For 8130"    //硬件版本
#define SOFTWAREVER "M-SW35A VER 4.2.0"   //软件版本
#define SERIALNUM "20130605A35"    //产品序列号

#define IMAGEUP  0
#define IMAGEDOWN  1

#define TOUCH_CHIP_ADDR_WR     0x70      //触摸屏I2C地址 read:10010001 write 10010000
#define TOUCH_CHIP_ADDR_RE      0x71      //触摸屏I2C地址 read:10010001 write 10010000

#define REFRESH  1
#define NOREFRESH 0

#define FULLPAGE  1
#define NOFULLPAGE 0

#define DELTACALIB 50   //触摸屏校准十字位置

#define REFRESH_TOP  0x01
#define REFRESH_MIDDLE  0x02
#define REFRESH_BOTTOM  0x04

#define MAXCOUNT   8      //防区最大数量
#define SUBMAXNUM  3      //副机最大数量
#define MAXDEFENCENUM  8   //最大防区数量
#define UDPPACKMAX  1200
#define UDPSENDMAX  20  //UDP多次发送缓冲最大值
#define AUDIOSENDMAX  10  //Audio UDP多次发送缓冲最大值
#define COMMSENDMAX  10  //COMM多次发送缓冲最大值
#define MAXSENDNUM  6  //最大发送次数
#define MAXVIDEOBUF  2  //最大视频缓冲
#define MAXPACKNUM  30     //帧最大数据包数量

#define TIMERTIME 100       //500 ms
#define TIMERPERSEC 2       //每秒2次线程
#define TIMER2TIME 1        //5 ms
#define GPIOTIME 4          //20 ms  GPIO检测
#define GPIOPERSEC 50       //每秒50次线程

#define IDLE_TIME      30      //空闲返回主界面时间
#define CLOSELCD_TIME  60//10 //60      //待机关LCD时间
#define POWERSTAY_TIME  30 //500 // 30      //关LCD后 进入低功耗时间

#define NO_CYCLE_PLAY  0   //音频单次播放
#define CYCLE_PLAY  1   //音频循环播放
#define KEYDELAYTIME  30      //按钮压下时间  150ms

#define PCM_REC_MAX  0x2000  //PCM录音缓冲区最大值
#define PCM_BUF_NUM  2       //PCM放音缓冲占内存页面数　64K*2
#define PCM_BUF_MAX  0x20000  //PCM放音缓冲区最大值

//短信息
#define INFOMAXITEM   20  //短信息最大条数
#define INFOMAXSIZE   402 //短信息内容最大容量
#define INFONUMPERPAGE  5  //一页显示信息数

#define INFOROWLEN   44    //信息每行长度
#define INFOROWPIXEL   528    //信息每行像素　44*12
#define MAXROW  12          //最大行数
#define TOTALPAGE  3          //总页数
#define PAGEPERROW  4          //页行数

//通话记录
#define RECORDMAXITEM   20  //通话记录最大条数
#define RECORDNUMPERPAGE  5  //一页显示通话记录数

#define DIRECTCALLTIME  600                 //直接呼叫每次时间
#define WATCHTIMEOUT  (30*TIMERPERSEC)    //监视最长时间   600
#define CALLTIMEOUT  (30*TIMERPERSEC)     //呼叫最长时间   250
//#define TALKTIMEOUT  (15*TIMERPERSEC)    //99 130  30*20   //通话最长时间      600
#define TALKTIMEOUT DelayTalkTimeOut[LocalCfg.TalkTimeOut]*TIMERPERSEC

#define LANGUAGEMAXNUM   2   //最大语言

//命令 管理中心
#define ALARM         1    //报警
#define CANCELALARM   2    //取消报警
#define SENDMESSAGE   3   //发送信息
#define REPORTSTATUS  4   //设备定时报告状态
#define QUERYSTATUS   5   //管理中心查询设备状态
#define REMOTEDEFENCE   20   //远程布防
#define RESETPASS       30   //复位密码
#define SEARCHALLEQUIP  252  //搜索所有设备（管理中心－＞设备）
#define WRITEEQUIPADDR      254  //写设备地址（管理中心－＞设备）

//对讲
#define VIDEOTALK      150 //局域网可视对讲
#define VIDEOWATCH     152 //局域网监控
#define NSORDER        154 //主机名解析（子网内广播）
#define NSSERVERORDER  155 //主机名解析(NS服务器)
#define NSSERVERORDER2  156 //设备终端解析(NS服务器)
#define FINDEQUIP      170 //查找设备
#define ASK              1     //命令类型 主叫
#define REPLY            2     //命令类型 应答
#define SINGLE_SEND           1   //单次
#define MULTI_SEND            2     //多次

#define CALL             1     //呼叫
#define LINEUSE          2     //占线
#define QUERYFAIL        3      //通信失败
#define CALLANSWER       4     //呼叫应答
#define CALLSTART        6     //开始通话

#define CALLUP           7     //通话数据1（主叫方->被叫方）
#define CALLDOWN         8     //通话数据2（被叫方->主叫方）
#define CALLCONFIRM      9     //通话在线确认（接收方发送，以便发送方确认在线）
#define REMOTEOPENLOCK   10     //远程开锁

#define JOINGROUP        22     //加入组播组（主叫方->被叫方，被叫方应答）
#define LEAVEGROUP       23     //退出组播组（主叫方->被叫方，被叫方应答）
#define TURNTALK         24     //转接（被叫方->主叫方，主叫方应答）
#define CENTERTURNTALK         25     //中心人工转接
#define TRUSTEESHIP_TALK  26    //托管

#define CALLEND          30     //通话结束

#ifdef _BRUSHIDCARD_SUPPORT   //刷卡功能支持
 #define IDCARD_SETUP_ADDR       0x56f000   //ID卡设置存储地址
 #define IDCARD_ADDR       0x570000   //ID卡存储地址
 #define IDCARD_MAXPACK   2000    //ID卡最大包数
 #define _IDCARDHAVEADDR      //ID卡带地址
 #define IDCARDMAXNUM  90000    //ID卡最大数量
 #ifdef _IDCARDHAVEADDR      //ID卡带地址
  #define CARDPERPACK  50    //每个数据包内ID卡最大数量
  #define BYTEPERSN    24    //每个SN 占用字节数
  #define BYTEPERSN_NEW  4   //为节省空间，去掉地址
 #else
  #define CARDPERPACK  350  //每个数据包内ID卡最大数量
  #define BYTEPERSN    4     //每个SN 占用字节数
 #endif
 #define IDCARDBRUSHNUM 100 //最多离线刷卡条数
 
 #define WRITEIDCARD    54   //写ID卡信息
 #define READIDCARD     55   //读ID卡信息
 #define BRUSHIDCARD     56   //刷ID卡信息，向服务器发送
#endif

#ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
 #define MAX_CAPTUREPIC_NUM   20
 #define CAPTUREPIC_SEND_START     60   //单元门口机、围墙机、二次门口机发送呼叫照片->发送开始
 #define CAPTUREPIC_SEND_DATA      61   //单元门口机、围墙机、二次门口机发送呼叫照片->发送数据
 #define CAPTUREPIC_SEND_SUCC      62   //单元门口机、围墙机、二次门口机发送呼叫照片->发送结束（成功）
 #define CAPTUREPIC_SEND_FAIL      63   //单元门口机、围墙机、二次门口机发送呼叫照片->发送结束（失败）
#endif

#ifdef _MULTI_MACHINE_SUPPORT  //多分机支持
 #define HOSTTOSUB  222 //主机与副机通信
 #define SYNCSUB        1   //同步状态（主机－＞副机）
 #define SUBDEFENCE     2   //布防（副机－＞主机）
 #define SUBALARM       3   //报警（副机－＞主机）
 #define SUBALARMTIP    5   //报警提示（主机－＞副机）
 #define SUBDEFENCETIP    6   //布防提示（主机－＞副机）
 #define SUBCONTROL        10    //家电控制（副机－＞主机）
 #define SUBCONTROLTIP     11    //家电控制提示（主机－＞副机）
 #define SUBFIND        255   //查找副机（主机－＞副机）

 #define SUBFIND_START_NUM  4  //查找副机 发送次数   6 - 4 = 2
#endif

#define UPLOADFILE  225    //上传
#define UPLOADYUV  227    //上传YUV
#define READFLASH  228    //读FLASH内容
#define REMOTERESET  229    //远程重启动

#define UDP_TEST  245

#define DOWNLOADFILE  224    //下载
#define STARTDOWN  1       //开始下载一个
#define DOWN       2       //下载
#define DOWNFINISHONE       3  //下载完成一个
#define STOPDOWN       10      //停止下载
#define DOWNFAIL         21 //下载失败  设备－》管理中心

#define REMOTEDEBUGINFO      253   //发送远程调试信息
#define STARTDEBUG  1    //开始
#define STOPDEBUG  0    //停止
#define DEBUGDATA  2    //调试数据
#define REMOTE_REBOOT  3    //重启动

#ifdef _REMOTECALLTEST  //远程呼叫测试
 #define REMOTETEST      200   //发送远程呼叫测试
 #define STARTTEST  1    //开始
 #define ENTERTESTMODE  2    //进入测试模式
 #define STOPCALL       3    //停止呼叫通话
#endif

#define SEND_PCM  226      //发送PCM数据
#define _REMOTEWRITEOV7725REGISTER   //远程写OV7725寄存器  调试用
#ifdef _REMOTEWRITEOV7725REGISTER   //远程写OV7725寄存器  调试用
 #define OV7725_OPERATE   227       //读写OV7725
 #define OV7725READREGISTER  1
 #define OV7725WRITEREGISTER 2
#endif

#ifdef _SEND_TO_SELF_TEST    //发送数据包给自己
 #define SEND_SELF  251
#endif

#define _PACK_1024

#define PORT_NUM  5
#define SMALLESTSIZE  18  //512  //UDP最小包大小    20121102 512 -- 18 , MAC最小为60
#define PACKDATALEN  1200   //数据包大小

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

//LT专用
#define SMARK_MONITOR 90

struct Local1{
               INT8U PreStatus;  //20130115 预状态，防止同时的操作
               int Status;
               int TalkEndStatus;
               INT8U KeyTouchNo; //按钮按下标志
               //状态 0 空闲 1 主叫对讲  2 被叫对讲  3 监视  4 被监视  5 主叫通话
               //6 被叫通话
               //30 电话通话
               int RecordPic;  //留照片  0 不留  1 呼叫留照片  2 通话留照片
               int IFrameCount; //I帧计数
               int IFrameNo;    //留第几个I帧
               int HavePicRecorded;  //有照片已录制

               char RemoteAddr[21];  //远端地址

               unsigned char AlarmByte[2];  //报警

               int CallConfirmFlag; //在线标志
               int Timer1Num;  //定时器1计数
               int OnlineFlag; //需检查在线确认
               int OnlineNum;  //在线确认序号
               int TimeOut;    //监视超时,  通话超时,  呼叫超时，无人接听
               int TalkTimeOut; //通话最长时间

               int PrevWindow;      //上一个窗口编号
               int TmpWindow;       //暂存窗口编号 用于弹出窗口时
               int CurrentWindow;   //当前窗口编号
               int ChildWindow;
               int isDisplayWindowFlag; //窗口正在显示中
               int DefenceDelayFlag;    //布防延时标志
               int DefenceDelayTime;   //计数
               int PassLen;            //密码长度
               int AlarmDelayFlag[MAXCOUNT];    //报警延时标志
               int AlarmDelayTime[MAXCOUNT];   //计数

               int CalibratePos;   //校准触摸屏十字位置 0 1 2 3
               int CalibrateSucc;  //校准成功
               unsigned char IP_Group[4];  //组播地址
               unsigned char Weather[3];   //天气预报

               int AddrLen;          //地址长度  S 12  B 6 M 8 H 6                 

               int isHost;           //'0' 主机 '1' 副机 '2' ...
               int ConnToHost;       //与主机连接正常 1 正常 0 不正常
               int ConnToHostTime;
               unsigned char HostIP[4]; //主机IP
               char HostAddr[21]; //主机Addr
               int DenNum;             //目标数量  副机
               unsigned char DenIP[SUBMAXNUM][4];    //副机IP
               char DenAddr[SUBMAXNUM][21];         //副机Addr
               int DenFlag[SUBMAXNUM];              //副机存在标志
               int TmpDenNum;          //目标数量暂存  副机
               unsigned char TmpDenIP[SUBMAXNUM][4];    //副机IP
               char TmpDenAddr[SUBMAXNUM][21];          //副机Addr

               int SendSyncFlag;  //发送同步信息标志
               int SendSyncTime;  //发送同步信息计数

               char SoundName[6][50];  //声音文件名称

               int NetLanStatus;   //Lan网络状态 1 断开  0 接通
               int OldNetSpeed;  //网络速度                
               int NoBreak;     //免扰状态 1 免扰  0 正常

               int ReportSend;  //设备定时报告状态已发送
               int RandReportTime; //设备定时报告状态随机时间
               int ReportTimeNum;  //计时

               int LcdLightFlag; //LCD背光标志
               int LcdLightTime; //时间
               int KeyPressTime;

               int NewInfo;  //有新信息
               int NewRecPic;  //有新留影

               int nowvideoframeno;   //当前视频帧编号
               int nowaudioframeno;   //当前音频帧编号

               int ForceEndWatch;  //有呼叫时，强制关监视

               int WatchTarget;  //监视目标
               int WatchTargetMaxNum; //监视目标最大数量

               int WatchKeyTouched;     //监视键 响应
               int WatchKeyTouchedTime;  //监视键 响应 计数

               int CenterKeyTouched;     //呼叫中心键 响应
               int CenterKeyTouchedTime;  //呼叫中心键 响应 计数                         

               int TestTouch; //测试触摸屏

               unsigned char OldVoiceHint;         //语音提示  0 关闭  1  打开
               unsigned char OldSpkVolume;         //SPK 音量
               unsigned char OldMicVolume;         //MIC 音量
               unsigned char OldTalkVoiceOpened;         //对讲时音量  0 关闭  1  打开
               unsigned char OldTalkVolume;        //对讲时音量
               unsigned char OldScrSave[3];        //屏幕保护  0 关闭  1  打开
               unsigned char OldBrightVolume;      //亮度
               unsigned char OldKeyVoice;          //按键音    0 关闭  1  打开

               int SceneStatus;  //情景模式当前状态               

               int CurrRingSec;  //当前振铃音乐选择项
               int isPlayRing;   //正在播放振铃

               int isRecordWav;  //正在录制留言
               int isPlayWav;    //正在播放留言

               int BootNo;

               #ifdef _CAPTURESCREEN
                 int OpenLockKeyNum;
                 int OpenLockTime;
               #endif


               int TalkWatchReturnType;  //0 --  主界面   1 -- Talk Watch 界面
               int TalkWatchWinType;  //0 -- Talk窗口  1 -- Watch窗口
               int TalkWatchButtonNum;

               unsigned char LanguageNo;    //语言下标  0 -- 中文    1 -- 英文
               int ImageButton_Text_wd;     //按钮文字间距

               int DefaultPage;

               int StateBar_Top_Flag; //顶部状态条显示标志

               int Text_Space;  //文本间隔

               INT8U ClearVideoWindowFlag; //清视频窗口标志

               INT8U NsMultiAddr[4];    //NS组播地址

               INT8U Camera;      //0  -- 无  1 -- ov7725
               INT8U VideoType;
               INT8U MachineType;  //0x80  --- 8130

               INT8U  PlayNullVoice;

               INT8U Test_Disp_No;   //测试窗口 显示测试
               INT8U Test_CurrPage;  //当前页
               INT8U Test_FrameRate; //编码帧率
               INT8U Test_FrameSize; //编码大小
               INT8U Test_VideoClosed;    //视频关闭
               INT8U Test_LocalWatchFlag; //本地视频监视
               INT8U Test_EncodeWatchFlag; //本地视频编码解码后显示
               INT8U Test_PlayAudioFlag; //播放音乐
               INT8U Test_RecAudioFlag; //录音
               INT8U Test_VideoOutput;    //视频输出  0  正常  1 自检
               INT8U Test_PowerStayOnFreq;    //低功耗待机分频数
               INT8U Test_VideoSdramRefresh;    //LCD显示时 SDRAM刷新周期  0  8周期  1 4周期
               INT8U Test_OV7725FrameRate;    //OV7725 输出帧率

               INT8U DecodeFrameSize; //解码大小
               INT16U DecodeVideoWidth;
               INT16U DecodeVideoHeight;

               INT8U DecodeStreamFlag;  //正在解码流
               INT8U DecodeJpegFlag;  //正在解静态图片

               INT8U fb_page;  //显示页 0 - 1       当前页
               INT8U fb_backpage;   //后台页
               INT32U fb_addr[2];

               INT8U Conflict_Flag;    //IP地址冲突
               INT8U Conflict_Mac[6];

               INT8U GpioWatchDogStarted;  //在GPIO检测线程中启动WatchDog
               INT8U DebugIP[4];
               INT8U DebugMac[6];
               INT8U RemoteDebugInfo;  //发送远程调试信息

               INT8U OpenDoorFlag; //开锁标志 0 未开锁  1 开锁延时中  2 开锁中
               INT16U OpenDoorTime; //时间

               INT8U TalkFinishFlag;  //通话结束标志     20101029  xu
               INT16U TalkFinishTime;  //通话结束计数

               INT8U TalkStartOpenLockFlag;  //通话开锁标志     20101029  xu
               INT16U TalkStartOpenLockTime;  //通话开锁计数

               INT16U OnlineRecvNum;
               INT8U OnlineRecvAddr[MAX_ONLINE_NUM][4];

               INT8U OnlineTestFlag;  //在线检测标志     20101029  xu
               INT16U OnlineTestTime;  //在线检测计数

               INT32U OldEncodeErrorNum;                      //编码错误数目

              #ifdef _REMOTECALLTEST  //远程呼叫测试
               INT8U EnterTestMode;  //测试模式, 5秒自动接听
               INT8U CloseTestRing;  //测试模式, 关闭铃声
              #endif

              INT8U TalkDoorOpenPlayFlag;  //通话过程中门开了提示

             #ifdef _NIGHT_CAMERA_ADJUST_SUPPORT   //光暗CAMERA自动调节支持
              INT8U Night_IO;
             #endif
              };

#ifdef _BRUSHIDCARD_SUPPORT   //刷卡功能支持
//ID卡信息
struct IDCardNo1{
               INT32U Num;        //数量
               INT8U *SN;//[IDCARDMAXNUM*BYTEPERSN_NEW]; //ID卡号  最大10万张
               INT32U serialnum;     //序号
              };
//写ID卡信息
struct RecvIDCardNo1{
               INT8U isNewWriteFlag;  //新写标志
               INT32U Num;             //卡数量
               INT16U PackNum;         //包数量
               INT8U PackIsRecved[IDCARDMAXNUM/CARDPERPACK + 1];  //包已接收标志
               INT8U *SN;//[IDCARDMAXNUM*BYTEPERSN_NEW]; //ID卡号  最大10万张
               INT32U serialnum;     //序号
              };
//刷ID卡信息
struct BrushIDCard1{
               INT32U Num;        //数量
               INT8U Info[IDCARDBRUSHNUM*11]; //ID卡号  离线信息最多1万条
               //前4字节卡号，后7字节时间
              };
//读写ID卡数据结构
struct iddata1
  {
   #ifdef _IDCARDHAVEADDR      //ID卡带地址
    char Addr[20];       //地址编码
   #endif 
   INT32U serialnum;  //序号
   INT32U  Num;          //卡总数
   INT32U  CurrNum;           //当前数据包卡数量
   INT32U  TotalPackage;      //总包数
   INT32U  CurrPackage;       //当前包数
  };
#endif

#ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
struct CapturePic1
              {
                INT8U isVilid;
                INT8U RecordPic;
                INT16U IFrameCount; //帧计数
                INT8U IFrameNo;    //留第几个I帧
                INT16U Pic_Width;
                INT16U Pic_Height;
                INT8U *jpeg_pic;
                INT32U FileLen;
                INT16U FilePos;
                INT16U TotalPackage;
                INT16U CurrPackage;
                INT8U HavePicRecorded;  //有照片已录制
                char RemoteAddr[21];  //远端地址
                INT8U SendToCetnering;  //正在传给中心
                INT8U ConnectCentered;   //与中心连接状态
                INT32U Capture_Total_Package;
                INT8U Capture_Send_Flag[60];
              };
#endif

struct LocalCfg1{
               char FlashHead[6];         //头部特征码
               char Addr[20];             //地址编码
               char AddrVerifyMa[10];         //主副机同步码
               INT8U Mac_Addr[6]; //网卡地址
               INT8U IP[4];       //IP地址
               INT8U IP_Mask[4];  //子网掩码
               INT8U IP_Gate[4];  //网关地址
               INT8U IP_NS[4];    //NS（名称解析）服务器地址
               INT8U IP_Server[4];  //主服务器地址（与NS服务器可为同一个）
               INT8U IP_Broadcast[4];  //广播地址

               int ReportTime;      //设备定时报告状态时间
               INT8U DefenceStatus;       //布防状态      0 - 撤防  1 - 外出   2 - 在家   3 - 就寝   4 - 起床   5 - 自定义
                                                  //                        11 - 外出   12 - 在家   13 - 就寝   14 - 起床   15 - 自定义     延时
               INT8U DefenceNum;          //防区模块个数
               INT8U DefenceInfo[32][10]; //防区信息

               char EngineerPass[10];             //工程密码

               int In_DelayTime;                //进入延时
               int Out_DelayTime;               //外出延时
               int Alarm_DelayTime;               //报警延时
               int BellNum;                     //振铃次数
               INT8U SoundSetup[8];          //声音          0 -- 7
               INT8U RecPhoto;          //访客留影          0 --禁用 1 --启用
               INT8U Language;          //语言选择    0 --简体中文 1 --繁体中文 2 -- ENLISH
               INT8U RingType;          //铃声类型  0 普通型  1  卡通型  2  摇滚型   3  抒情型
               INT8U CallWaitRing;      //回铃类型  0 普通型  1  卡通型  2  摇滚型   3  抒情型

               INT8U VoiceHint;         //语音提示  0 关闭  1  打开
               INT8U SpkVolume;         //SPK 音量
               INT8U MicVolume;         //MIC 音量
               INT8U TalkVoiceOpened;         //对讲时音量  0 关闭  1  打开
               INT8U TalkVolume;        //对讲时音量
               INT8U PhoneVoiceOpened;         //电话时音量  0 关闭  1  打开
               INT8U PhoneVolume;        //电话时音量
               INT8U ScrSave[3];        //屏幕保护  0 关闭  1  打开
               INT8U MainPage;          //首页设置  0 默认界面  1 电话界面
               INT8U BrightVolume;      //亮度
               INT8U ContrastVolume;    //对比度
               INT8U KeyVoice;          //按键音    0 关闭  1  打开

               INT16 Ts_X0;                   //触摸屏
               INT16 Ts_Y0;
               INT16 Ts_deltaX;
               INT16 Ts_deltaY;
               INT16 Ts_Mode;                 //模式  X   Y

               INT8U EncodeFrameRate; //编码帧率
               INT8U EncodeFrameSize; //编码大小
               INT8U VideoClosed;    //视频关闭
               INT8U VideoOutput;    //视频输出  0  正常  1 自检

               INT8U VideoSdramRefresh;    //LCD显示时 SDRAM刷新周期  0  8周期  1 4周期

               INT8U MultiMachineSupport;  //多分机支持,对主机有效，如不支持，则不发广播查询包
               INT8U DenMaxNum;  //副机最大数量，这一数量需小于 SUBMAXNUM， 有利于减小广播包

               //防区设置
               INT8U DefenceSetup_dType[MAXDEFENCENUM];    //防区类型     2 - 24小时防区  3 - 自定义防区
               INT8U DefenceSetup_dDelay[MAXDEFENCENUM];   //延时时间     0 - 0秒   1 - 5秒  2 - 10秒  3 - 15秒  4 - 20秒  5 - 25秒  6 - 60秒
               INT8U DefenceSetup_tType[MAXDEFENCENUM];    //探头类型     0 - 烟感  1 - 燃气  2 - 防拆  3 - 紧急  4 - 红外  5 - 门磁  6 - 窗磁

               INT8U Scene_Defence[5][MAXDEFENCENUM];      //安防情景模式设置

               char OpenLockPass[10]; //开锁密码

               INT8U PassOpenLock;   //密码开锁
               INT8U CardOpenLock;   //刷卡开锁
               INT8U DoorButton;   //出门按钮
               INT8U DoorDetect;       //门磁检测
               INT8U OpenLockTime;   //开锁时间  0 200ms  1 500ms  2 5s  3 10s
               INT8U DelayLockTime;   //延时开锁  0 0s  1 3s  2 5s  3 10s
               INT8U TalkTimeOut;     //20140225 呼叫超时时间 15 30 60 99
               
               INT32U EncodeErrorNum;                      //编码错误数目

               INT8U ResetStatus;
               INT8U Reset_DenNum;
               INT8U Reset_IP[10][4];     //对方IP
               INT8U Reset_Mac[10][6];    //对方Mac
               char Reset_Addr[10][21];   //对方Addr

               INT16U BootNo;
              #ifdef _SEND_TO_SELF_TEST    //发送数据包给自己
               INT16U SelfBootNo;
              #endif

               INT8U MicDecValue; //展箱、展板模式,需减小 Mic音量
              };

struct Remote1{
               INT8U DenNum;             //目标数量  主机+副机
               INT8U LineUseNum;         //20101202 xu 占线数量
               //INT8U DenMac[6]; //对方Mac
               //INT8U DenIP[4]; //对方IP
               INT8U GroupIP[4]; //GroupIP
               INT8U IP[10][4];    //对方IP
               INT8U Mac[10][6];    //对方Mac
               INT8U Added[10];                //已加入组
               char Addr[10][21];         //对方Addr
               int TimeOut[10];      //超时时间 20131023
              };
struct TalkCall1{
                INT8U BuildFlag; //幢
                char Build[5];
                INT8U BuildMaxlen;
                INT8U UnitFlag;  //单元
                char Unit[5];
                INT8U UnitMaxlen;
                INT8U RoomFlag;  //房号
                char Room[5];
                INT8U RoomMaxlen;
                INT8U Len;
                char TalkText[15];
               };
#ifdef _MULTI_MACHINE_SUPPORT  //多分机支持
struct SyncSubData1
      {
               int DenNum;             //目标数量  副机
               unsigned char DenIP[SUBMAXNUM][4];    //副机IP
               char DenAddr[SUBMAXNUM][22];         //副机Addr

               //以下设置 传到副机  *************
               unsigned char DefenceStatus;       //布防状态
               unsigned char DefenceNum;          //防区模块个数
               unsigned char DefenceInfo[32][10]; //防区信息

               char EngineerPass[10];             //工程密码
      };
#endif

//通话数据结构
struct talkdata1
  {
//   unsigned char Order;     //操作命令
   char HostAddr[20];       //主叫方地址
   INT8U HostIP[4]; //主叫方IP地址
   char AssiAddr[20];       //被叫方地址
   INT8U AssiIP[4]; //被叫方IP地址
   INT32U timestamp;  //时间戳
   INT16U DataType;          //数据类型
   INT16U Frameno;           //帧序号
   INT32U Framelen;            //帧数据长度
   INT16U TotalPackage;      //总包数
   INT16U CurrPackage;       //当前包数
   INT16U Datalen;           //数据长度
   INT16U PackLen;       //数据包大小
  };

//UDP数据发送结构
struct Multi_Udp_Buff1{
               INT8U isValid; //是否有效
               INT8U SendMode; //ASK -- 多次发送   REPLY --- 单次发送
               INT8U SendNum; //当前发送次数
               INT8U MaxSendNum; //发送次数
               INT8U CurrOrder;//当前命令状态,VIDEOTALK  VIDEOWATCH
                               //主要用于需解析时，如单次命令置为0
               INT16U DestPort;
               INT8U MacValid;  //MAC地址有效
               INT8U ArpNum;  //ARP次数
               INT8U RemoteMac[6];
               INT8U RemoteIP[4];
               INT8U *buf;//[1520];
              // INT8U buf[1520];
               INT16U TimeNum;
               INT16U DelayTime;  //等待时间
               INT16U SendDelayTime; //发送等待时间计数  20101112
               INT16U nlength;
              };

//Audio UDP数据发送结构
struct Audio_Udp_Buff1{
               INT8U isValid; //是否有效
               INT8U SendMode; //ASK -- 多次发送   REPLY --- 单次发送
               INT8U SendNum; //当前发送次数
               INT8U MaxSendNum; //发送次数
               INT8U CurrOrder;//当前命令状态,VIDEOTALK  VIDEOWATCH
                               //主要用于需解析时，如单次命令置为0
               INT16U DestPort;
               INT8U MacValid;  //MAC地址有效
               INT8U ArpNum;  //ARP次数
               INT8U RemoteMac[6];
               INT8U RemoteIP[4];
               INT8U buf[86 + 64];
               INT16U TimeNum;
               INT16U DelayTime;  //等待时间
               INT16U SendDelayTime; //发送等待时间计数  20101112
               INT16U nlength;
              };
              
//COMM主动命令数据发送结构
struct Multi_Comm_Buff1{
               INT8U isValid; //是否有效
               INT8U SendNum; //当前发送次数
               INT8U m_Comm;
               INT8U buf[256];
               INT16U nlength;
              };

struct PicStatBuf1{
               INT8U Flag;                      //延时清提示信息标志
               INT8U Type;                      //类型
               INT16U Time;
               INT16U MaxTime;                   //最长时间
              };

struct Info1{
               char FlashHead[8]; //Flash已存标志
               INT16U MaxNum;   //最大信息数
               INT16U TotalNum; //信息总数
               INT16U NoReadedNum; //未读信息总数
               INT16U TotalInfoPage;   //总信息页数
               INT16U CurrentInfoPage; //当前信息页
               INT16U CurrNo;    //当前信息序号
               INT16U CurrPlayNo;  //当前播放序号
              };

//单条信息内容结构体
struct InfoContent1{
               INT8U isValid;  //有效，未删除标志   1
               INT8U isReaded; //已读标志    1
               INT8U HavePic;  //有无图像标志  0   1 -- 640x480  2 -- 320x240
               char RemoteAddr[21];
               char Time[32];    //接收时间    32
               INT8U Type;     //类型        1    信息类型或事件类型     0 -- 主叫　　1 -- 被叫未接听  2 -- 被叫接听
               INT32U Sn;      //序号        4
               INT16U Length;      //长度        4
              };                               //内存分配为444

struct downfile1
  {
   INT8U FlagText[20];     //标志字符串
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

#ifndef CommonH
#define CommonH
  //本机状态设置
  struct Local1 Local;
  struct LocalCfg1 LocalCfg;

  INT32U time_count;  
#ifdef _BRUSHIDCARD_SUPPORT   //刷卡功能支持
   INT8U IDCardReadFlag;
   INT8U IDCardHaveRead[IDCARD_MAXPACK];
   INT8U IDCardReadIP[4];
   //ID卡信息
   struct IDCardNo1 IDCardNo;
   //刷ID卡信息
   struct BrushIDCard1 BrushIDCard;
   //写ID卡信息
   struct RecvIDCardNo1 RecvIDCardNo;
#endif

#ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
  struct CapturePic1 CapturePic;
#endif

  int DeltaLen;  //数据包有效数据偏移量
  //远端地址
  struct Remote1 Remote;
  char NullAddr[21];   //空字符串

  char UdpPackageHead[15];
  char UdpPackageHead2[15];
  INT16U LocalPort[PORT_NUM];
  INT16U RemotePort[PORT_NUM];

  //20140225
  INT8U DelayTalkTimeOut[4]= {15, 30, 60, 90};// 


  struct Multi_Udp_Buff1 Multi_Udp_Buff[UDPSENDMAX]; //UDP发送缓冲
  struct Audio_Udp_Buff1 Audio_Udp_Buff[AUDIOSENDMAX]; //Audio UDP发送缓冲

  struct PicStatBuf1 PicStatBuf;

  INT8U CurrBox;

  //进入设置
  struct TImage setup_image;
  struct TImage door_type_image[2];
  struct TImage net_image[2];
  struct TEdit setup_pass_edit;

  //设置主窗口
  struct TImage setupmain_image;
  struct TImage setupmain_param_image[6];

  //地址设置窗口
  struct TImage addr_image;
  struct TEdit addr_edit[6];
  struct TImage edit_status_image[2];

  //参数设置窗口
  struct TImage param_image;
  struct TImage param_line_image[2];
  struct TImage curr_row_image[2];

  //密码设置
  struct TImage pass_image;
  struct TEdit pass_edit[4];

  //其它设置窗口
  struct TImage other_image;

  //在线检测窗口
  struct TImage onlinetest_image;
  struct TImage online_content_image;
  struct TEdit online_edit[4];

  //关于
  struct TImage setup_info_image;

  //主窗口
  struct TImage talk1_image;    //可视对讲背景图像 

  //20131128 LOGO
  struct TImage logo_image;    //LOGO

  //呼叫连接中窗口
  struct TImage talk_connect_image;
  struct TImage talk_connect[4];

  //通话窗口
  struct TImage talk_start_image;
  struct TImage talk_start_openlock[2];

  struct TImage openlock_image;
  struct TImage locking_image;
  struct TImage openlock_result_image;

  //呼出窗口
  struct TEdit r2r_edit[3];
  struct TEdit openlock_edit;

  struct TImage small_num_image[10];
  struct TImage big_num_image[10];

  //监视窗口
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
  //本机状态设置
  extern struct Local1 Local;
  extern struct LocalCfg1 LocalCfg;

  extern INT32U time_count;
#ifdef _BRUSHIDCARD_SUPPORT   //刷卡功能支持
   extern INT8U IDCardReadFlag;
   extern INT8U IDCardHaveRead[IDCARD_MAXPACK];
   extern INT8U IDCardReadIP[4];
   //ID卡信息
   extern struct IDCardNo1 IDCardNo;
   //刷ID卡信息
   extern struct BrushIDCard1 BrushIDCard;
   //写ID卡信息
   extern struct RecvIDCardNo1 RecvIDCardNo;
#endif

#ifdef _CAPTUREPIC_TO_CENTER  //捕获图片传到中心
  extern struct CapturePic1 CapturePic;
#endif

  extern int DeltaLen;  //数据包有效数据偏移量
  //远端地址
  extern struct Remote1 Remote;
  extern char NullAddr[21];   //空字符串

  extern char UdpPackageHead[15];
    extern char UdpPackageHead2[15];
  extern INT16U LocalPort[PORT_NUM];
  extern INT16U RemotePort[PORT_NUM];
  
  //20140225
  extern INT8U DelayTalkTimeOut[4];// = {15, 30, 60, 90};


  extern struct Multi_Udp_Buff1 Multi_Udp_Buff[UDPSENDMAX]; //UDP发送缓冲
  extern struct Audio_Udp_Buff1 Audio_Udp_Buff[AUDIOSENDMAX]; //Audio UDP发送缓冲

  extern struct PicStatBuf1 PicStatBuf;

  extern INT8U CurrBox;

  //进入设置
  extern struct TImage setup_image;
  extern struct TImage door_type_image[2];
  extern struct TImage net_image[2];
  extern struct TEdit setup_pass_edit;

  //设置主窗口
  extern struct TImage setupmain_image;
  extern struct TImage setupmain_param_image[6];

  //地址设置窗口
  extern struct TImage addr_image;
  extern struct TEdit addr_edit[6];
  extern struct TImage edit_status_image[2];

  //参数设置窗口
  extern struct TImage param_image;
  extern struct TImage param_line_image[2];
  extern struct TImage curr_row_image[2];

  //密码设置
  extern struct TImage pass_image;
  extern struct TEdit pass_edit[4];

  //其它设置窗口
  extern struct TImage other_image;

  //在线检测窗口
  extern struct TImage onlinetest_image;
  extern struct TImage online_content_image;
  extern struct TEdit online_edit[4];

  //关于
  extern struct TImage setup_info_image;

  //主窗口
  extern struct TImage talk1_image;    //可视对讲背景图像  单元

  //20131128 LOGO
  extern struct TImage logo_image;    //LOGO

  //呼叫连接中窗口
  extern struct TImage talk_connect_image;
  extern struct TImage talk_connect[4];

  //通话窗口
  extern struct TImage talk_start_image;
  extern struct TImage talk_start_openlock[2];

  extern struct TImage openlock_image;
  extern struct TImage locking_image;
  extern struct TImage openlock_result_image;

  //呼出窗口
  extern struct TEdit r2r_edit[3];
  extern struct TEdit openlock_edit;

  extern struct TImage small_num_image[10];
  extern struct TImage big_num_image[10];

  //监视窗口
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