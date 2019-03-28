#include "includes.h"

#include "stdlib.h"

#define x_first_h

#define CommonH
#include "common.h"


/*
****************************************
2013.8.29
添加启动splash界面
201309
广告元素初始化
呼叫数组 20131129
20140315 ad windows

****************************************
*/


void InterfaceInit(void);

void DisplayFirstWindow(void);    //显示主窗口
void PreDealFirstWindow(INT8U wintype);    //主窗口预处理，按键按下效果
void OperateFirstWindow(INT8U wintype, INT8U currwindow);    //主窗口操作
void CheckTouchFirstWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

void CheckFirstWindowFlag(void);  //检查返回首页标志

void RefreshBackWindow(INT8U RefreshType, INT8U FullPageType); //刷新背景
void DisplayStateBar(void);      //状态条
void PlaySoundTip(void); //播放提示音
void PlaySoundDoorBell(void); //20131205

void PreDealBigmenuButton(INT8U wintype);  //快捷按键 预处理
void ShowPressQuickmenu_Button(INT8U wintype);  //快捷按键   弹起
void OperateQuickmenu(INT8U wintype, INT8U currtype);

void DisplayInfoStateImage(INT8U NetState);    //显示信息图标
void DisplayPictureStateImage(INT8U NetState);    //显示留影图标

//201402 新增加 密码键盘窗体
void DisplayLTpwWindow(INT8U RefreshType,INT8U CurrentWindowNo);    //显示进入密码窗口
void PreDealLTpwWindow(INT8U wintype);    //进入密码窗口预处理，按键按下效果
void OperateLTpwWindow(INT8U wintype, INT8U currwindow);    //进入密码窗口操作
void CheckTouchLTpwWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作

//20140315
void DisplaySmarkStreamWindow(INT8U WinType);    //显示呼入窗口
void PreDealSmarkStreamWindow(INT8U wintype);    //呼入窗口预处理，按键按下效果
void OperateSmarkStreamWindow(INT8U wintype, INT8U currwindow);    //对讲呼入窗口操作
void CheckTouchSmarkStreamWindow(INT16U XLCD, INT16U YLCD); //检查触摸屏操作


//供lib 调用
INT32U GetLocal_fb_addr(void);
INT8U GetLocal_fb_page(void);
INT8U *Get_jpeg_bitstream(void);
INT8U Get_Mjpeg_DecodeError(void);
void Set_Local_DecodeJpegFlag(INT8U disp_DecodeJpegFlag);
INT8U Get_Local_DecodeStreamFlag(void);
//---------------------------------------------------------------------------
//当前窗口  0 -- 开机界面  1 -- 服务介绍   2 -- 设备管理   3 -- 安防界面  4 -- 通讯功能界面  5 -- 便捷服务  6 -- 多媒体界面   7 -- 设置界面   8 -- 情景模式


//          12 -- 视频呼叫
//          13 -- 监视门口
//          14 -- 记录查询
//          16 -- 对讲图像窗口
//          21 -- 通话记录窗口
//          22 -- 查看通话记录窗口

//          31 -- 撤防
//          32 -- 布防
//          34 -- 报警窗口
//          35 -- 取消报警窗口

//          111 -- 社区服务窗口  多媒体
//          112 -- 信息内容显示窗口

// 	    152 -- 设置主窗口
//          149 -- Digital KB1窗口
//          150 -- KB1窗口
//          151 -- LAN地址设置
// 	    154 -- 房号设置
// 	    158 -- 版本信息
// 	    159 -- 系统升级
//          165 -- 屏幕设置
//          166 -- 声音设置
//          167 -- 时间设置
//          168 -- 密码设置
//          169 -- 语言设置

// 	    160 -- 振铃音乐选择

//          188 -- 语音提示、按键音设置窗口
//          190 -- 校准触摸屏窗口
//          191 -- 校准触摸屏提示窗口
//          192 -- 测试窗口
//以下不能为局部变量　


//2013.8.29 splash
//20130901 变动为数组
INT32U splash_image_Faddr[SPLASH_COUNT] = {0x00430000UL,
                                           0x00431000UL,
                                           0x00432000UL,
                                           0x00433000UL,
                                           0x00434000UL,
                                           0x00435000UL,
                                           0x00436000UL,
                                           0x00437000UL,
                                           0x00438000UL,
                                           0x00439000UL,
                                           0x0043A000UL,
                                           0x0043B000UL,
                                           0x0043C000UL,
                                           0x0043D000UL,
                                           0x0043E000UL,
                                           0x0043F000UL,
                                           0x00440000UL,
                                           0x00441000UL,
                                           0x00442000UL,
                                           0x00443000UL,
                                           0x00444000UL,
                                           0x00445000UL,
                                           0x00446000UL,
                                           0x00447000UL,
                                           0x00448000UL,
                                           0x00449000UL,
                                           0x0044A000UL,
                                           0x0044B000UL,
                                           0x0044C000UL,
                                           0x0044D000UL,
                                           0x0044E000UL,
                                           0x0044F000UL,
                                           0x00450000UL,
                                           0x00451000UL,
                                           0x00452000UL,
                                           0x00453000UL,
                                           0x00454000UL,
                                           0x00455000UL,
                                           0x00456000UL,
                                           0x00457000UL,
                                           0x00458000UL,
                                           0x00459000UL,
                                           0x0045A000UL,
                                           0x0045B000UL,
                                           0x0045C000UL,
                                           0x0045D000UL,
                                           0x0045E000UL,
                                           0x0045F000UL,
                                           0x00460000UL,
                                           0x00461000UL,
                                           0x00462000UL,
                                           0x00463000UL,
                                           0x00464000UL,
                                           0x00465000UL,
                                           0x00466000UL,
                                           0x00467000UL,
                                           0x00468000UL,
                                           0x00469000UL,
                                           0x0046A000UL,
                                           0x0046B000UL
                                           };

INT32U ad_image_Faddr[AD_PIC_COUNT]={0x00470000UL,
	                       0x00471000UL,
	                       0x00473000UL,
	                       0x00475000UL,
	                       0x00477000UL,
	                       0x00479000UL,
	                       0x0047B000UL,
	                       0x0047D000UL,
	                       0x0047F000UL,
	                       0x00481000UL,
	                       0x00483000UL,
	                       0x00485000UL,
	                       0x00487000UL,
	                       0x00489000UL,
	                       0x0048B000UL,
	                       0x0048D000UL,
	                       0x0048F000UL,
	                       0x00491000UL,
	                       0x00493000UL,
	                       0x00495000UL,
	                       0x00497000UL,
	                       0x00499000UL,
	                       0x0049B000UL,
	                       0x0049D000UL,
	                       0x0049F000UL,
	                       0x004A1000UL
	                       };

//主界面
INT32U main_image_Faddr = 0x000f9000UL;
INT32U logo_image_Faddr = 0x00108000UL;

//2013.08.30 增加主界面按钮 5-6
INT32U menu_button_Faddr[MENU_BTN_COUNT][2] = {
	{0x0010B000UL, 0x0010E000UL},
	{0x00110000UL, 0x00112000UL},
	{0x00115000UL, 0x00117000UL},
	{0x0011A000UL, 0x0011C000UL},
	{0x0011E000UL, 0x00120000UL},
	{0x00158000UL, 0x00159000UL}
};

INT32U state_image_Faddr[STATE_COUNT] = {
	0x00122000UL,
	0x00123000UL,
	0x00124000UL,
	0x00125000UL,
	0x00126000UL,
	0x00127000UL,
	0x00128000UL,
	0x00129000UL
};

INT32U top_image_Faddr = 0x00182000UL;
INT32U middle_image_Faddr = 0x00185000UL;
INT32U bottom_image_Faddr = 0x00170000UL;

//20131010 增加布防状态按钮两组 7+2
INT32U bigmenu_button_Faddr[9][2] = {
	{0x00174000UL, 0x00175000UL},
	{0x00176000UL, 0x00177000UL},
	{0x00178000UL, 0x00179000UL},
	{0x0017A000UL, 0x0017B000UL},
	{0x0017C000UL, 0x0017D000UL},
	{0x0017E000UL, 0x0017F000UL},
	{0x00180000UL, 0x00181000UL},
	{0x00198000UL, 0x00199000UL},
	{0x0019A000UL, 0x0019B000UL}
};





//进入设置
INT32U setup_pass_edit_Faddr = 0x00128000UL;
INT32U setup_key_image_Faddr = 0x0012B000UL;

INT32U num_button_Faddr[15][2] = {
	{0x00132000UL, 0x00133000UL},
	{0x00134000UL, 0x00135000UL},
	{0x00136000UL, 0x00138000UL},
	{0x00139000UL, 0x0013A000UL},
	{0x0013B000UL, 0x0013C000UL},
	{0x0013D000UL, 0x0013E000UL},
	{0x0013F000UL, 0x00140000UL},
	{0x00141000UL, 0x00142000UL},
	{0x00143000UL, 0x00144000UL},
	{0x00145000UL, 0x00146000UL},
	{0x00147000UL, 0x00148000UL},
	{0x00149000UL, 0x0014A000UL},
	{0x0014B000UL, 0x0014C000UL},
	{0x0014E000UL, 0x0014F000UL},
	{0x00151000UL, 0x00152000UL}
};

//设置主窗口
INT32U setup_button_Faddr[5][2] = {
	{0x00210000UL, 0x00212000UL},
	{0x00214000UL, 0x00216000UL},
	{0x00218000UL, 0x0021A000UL},
	{0x0021C000UL, 0x0021E000UL},
	{0x00220000UL, 0x00222000UL}
};

//键盘窗口
INT32U kb_image_Faddr = 0x00250000UL;
INT32U kb_edit_Faddr = 0x0025C000UL;

INT32U kb_button_Faddr[15][2] = {
	{0x0025E000UL, 0x0025F000UL},
	{0x00260000UL, 0x00261000UL},
	{0x00262000UL, 0x00263000UL},
	{0x00264000UL, 0x00265000UL},
	{0x00266000UL, 0x00267000UL},
	{0x00268000UL, 0x00269000UL},
	{0x0026A000UL, 0x0026B000UL},
	{0x0026C000UL, 0x0026D000UL},
	{0x0026E000UL, 0x0026F000UL},
	{0x00270000UL, 0x00271000UL},
	{0x00272000UL, 0x00273000UL},
	{0x00274000UL, 0x00275000UL},
	{0x00276000UL, 0x00277000UL},
	{0x00279000UL, 0x0027A000UL},
	{0x0027C000UL, 0x0027D000UL}
};

//LAN设置窗口
INT32U lan_image_Faddr = 0x00236000UL;
INT32U lan_button_Faddr[2][2] = {
	{0x00230000UL, 0x00231000UL},
	{0x00233000UL, 0x00234000UL}
};
INT32U lan_edit_Faddr[2] = {
	0x00240000UL, 0x00241000UL
};

//房号设置窗口
INT32U addr_image_Faddr = 0x0029A000UL;
INT32U addr_button_Faddr[2][2] = {
	{0x002A5000UL, 0x002A6000UL},
	{0x002A8000UL, 0x002A9000UL}
};

//修改密码窗口
INT32U pass_image_Faddr = 0x002C0000UL;

//屏幕调节窗口
INT32U screen_image_Faddr = 0x002D0000UL;
INT32U screen_button_Faddr[3][2] = {
	{0x002D9000UL, 0x002DA000UL},
	{0x002DC000UL, 0x002DD000UL},
	{0x002DE000UL, 0x002DF000UL}
};
INT32U brightness_image_Faddr[6] = {
	0x002E0000UL, 0x002E2000UL, 0x002E4000UL, 0x002E6000UL, 0x002E8000UL, 0x002EA000UL
};

//音量调节窗口
INT32U voice_image_Faddr = 0x002F0000UL;
INT32U voice_button_Faddr[5][2] = {
	{0x002FA000UL, 0x002FB000UL},
	{0x002FD000UL, 0x002FE000UL},
	{0x002FF000UL, 0x00300000UL},
	{0x00301000UL, 0x00302000UL},
	{0x00303000UL, 0x00304000UL}
};
INT32U spk_image_Faddr[6] = {
	0x00305000UL, 0x00307000UL, 0x00309000UL, 0x0030B000UL, 0x0030D000UL, 0x0030F000UL
};
INT32U mic_image_Faddr[6] = {
	0x00311000UL, 0x00313000UL, 0x00315000UL, 0x00317000UL, 0x00319000UL, 0x0031B000UL
};

//关于窗口
INT32U version_button_Faddr[2][2] = {
	{0x00420000UL, 0x00421000UL},
	{0x00422000UL, 0x00423000UL}
};

//触摸屏校准窗口
INT32U calibrate_image_Faddr = 0x00290000UL;
INT32U cross_button_Faddr[1][2] = {
	{0x00292000UL, 0x00293000UL}
};

//呼入窗口
INT32U talkpic_image_Faddr = 0x00160000UL;
INT32U talkpic_button_Faddr[3][2] = {
	{0x00164000UL, 0x00165000UL},
	{0x00166000UL, 0x00167000UL},
	{0x00168000UL, 0x00169000UL}
};

//监视视频窗口
INT32U watchpic_image_Faddr = 0x0016A000UL;
INT32U watchpic_button_Faddr[1][2] = {
	{0x0016D000UL, 0x0016E000UL}
};

//监视窗口
INT32U watchtarget_image_Faddr = 0x00200000UL;
INT32U watch_Faddr[3][2] = {
	{0x00202000UL, 0x00203000UL},
	{0x00204000UL, 0x00205000UL},
	{0x00206000UL, 0x00208000UL}
};

//布防窗口
INT32U fortify_button_Faddr[2][2] = {
	{0x00190000UL, 0x00192000UL},
	{0x00194000UL, 0x00196000UL}
};

//报警图像
INT32U alarm_main_image_Faddr = 0x0040E000;
INT32U alarm_image_Faddr[2] = {0x00400000UL, 0x00406000UL};

//信息窗口
INT32U info_image_Faddr = 0x00320000UL;
INT32U info_button_Faddr[3][2] = {
	{0x00329000UL, 0x0032A000UL},
	{0x0032B000UL, 0x0032C000UL},
	{0x0032D000UL, 0x0032E000UL}
};
INT32U inforow_button_Faddr[INFONUMPERPAGE][2] = {
	{0x0032F000UL, 0x00331000UL},
	{0x00333000UL, 0x00335000UL},
	{0x00337000UL, 0x00339000UL},
	{0x0033B000UL, 0x0033D000UL},
	{0x0033F000UL, 0x00341000UL}
};

//信息查看窗口
INT32U infowin_image_Faddr = 0x00350000UL;
INT32U infowin_button_Faddr[3][2] = {
	{0x00354000UL, 0x00355000UL},
	{0x00356000UL, 0x00357000UL},
	{0x00358000UL, 0x00359000UL}
};
INT32U infocontent_image_Faddr = 0x00353000UL;

//通话记录窗口
INT32U recordsicon_button_Faddr[RECORDNUMPERPAGE][2] = {
	{0x00360000UL, 0x00361000UL},
	{0x00362000UL, 0x00363000UL},
	{0x00364000UL, 0x00365000UL},
	{0x00366000UL, 0x00367000UL},
	{0x00368000UL, 0x00369000UL}
};

INT32U records_pic_icon_Faddr[RECORDNUMPERPAGE] = {0x0036A000UL, 
	0x0036B000UL, 
	0x0036C000UL, 
	0x0036D000UL, 
	0x0036E000UL};

INT32U records_call_icon_Faddr[RECORDNUMPERPAGE] = {0x0036F000UL, 
	0x00370000UL, 
	0x00371000UL, 
	0x00372000UL, 
	0x00373000UL};


//  struct TImage records_image;
//  struct TImageButton records_button[3];
//  struct TImageButton recordsrow_button[RECORDNUMPERPAGE];

//通话记录查看窗口
INT32U picwin_image_Faddr = 0x00380000UL;
INT32U pic_blank_image_Faddr = 0x00384000UL;
INT32U picwin_button_Faddr[3][2] = {
	{0x00386000UL, 0x00387000UL},
	{0x00388000UL, 0x00389000UL},
	{0x0038A000UL, 0x0038B000UL}
};

//测试窗口
INT32U test_image_Faddr = 0x002B0000UL;
INT32U test_image1_Faddr = 0x002B5000UL;
INT32U test_button_Faddr[1][2] = {
	{0x002B2000UL, 0x002B3000UL}
};
INT32U label_null_image_Faddr = 0x002B4000UL;

INT32U main_image11111_Faddr = 0x00500000UL;

//switch窗口
INT32U switch_in_button_Faddr[8][2] = {
	{0x00158000UL, 0x00159000UL},
	{0x00158000UL, 0x00159000UL},
	{0x00158000UL, 0x00159000UL},
	{0x00158000UL, 0x00159000UL},
	{0x00158000UL, 0x00159000UL},
	{0x00158000UL, 0x00159000UL},
	{0x00158000UL, 0x00159000UL},
	{0x00158000UL, 0x00159000UL}
};

INT32U switch_set_button_Faddr[12][2] = {
	{0x00530000UL, 0x00531000UL},
	{0x00532000UL, 0x00533000UL},
	{0x00534000UL, 0x00535000UL},
	{0x00536000UL, 0x00537000UL},
	{0x00538000UL, 0x00539000UL},
	{0x0053A000UL, 0x0053B000UL},
	{0x0053C000UL, 0x0053D000UL},
	{0x0053E000UL, 0x0053F000UL},
	{0x00540000UL, 0x00540000UL},
	{0x00541000UL, 0x00541000UL},
    {0x00542000UL, 0x00542000UL},
    {0x00543000UL, 0x00543000UL}
};

void InterfaceInit(void)     //初始化界面资源
{
	INT8U i;
	
	//splash
	//splash_image.Faddr=splash1_image_Faddr;
	//PreDealImage(&splash1_image);

	//splash层预备
	for(i=0; i<SPLASH_COUNT; i++)
	{
		//splash_image[i].Faddr = splash_image_Faddr[i];
		//PreDealImage(&splash_image[i]);
	}

	//广告测试层
	for(i=0; i<26; i++)
	{
		//ad_image[i].Faddr = ad_image_Faddr[i];
		//PreDealImage(&ad_image[i]);
	}
		

	//主界面
	main_image.Faddr = main_image_Faddr;
	PreDealImage(&main_image);
	
	logo_image.Faddr = logo_image_Faddr;
	PreDealImage(&logo_image);


	//2013.8.30 修改循环次数5-6
	for(i=0; i<MENU_BTN_COUNT; i++)
	{
		menu_button[i].Faddr[0] = menu_button_Faddr[i][0];
		menu_button[i].Faddr[1] = menu_button_Faddr[i][1];
		PreDealImageButton(&menu_button[i], IMAGEUP);
		PreDealImageButton(&menu_button[i], IMAGEDOWN);
	}


	for(i=0; i<STATE_COUNT; i++)
	{
		state_image[i].Faddr = state_image_Faddr[i];
		PreDealImage(&state_image[i]);
	}

    //20131010 增加布防状态按钮两组 7+2
	for(i=0; i<9; i++)
	{
		bigmenu_button[i].Faddr[0] = bigmenu_button_Faddr[i][0];
		bigmenu_button[i].Faddr[1] = bigmenu_button_Faddr[i][1];
		PreDealImageButton(&bigmenu_button[i], IMAGEUP);
		PreDealImageButton(&bigmenu_button[i], IMAGEDOWN);
	}

	top_image.Faddr = top_image_Faddr;
	middle_image.Faddr = middle_image_Faddr;
	bottom_image.Faddr = bottom_image_Faddr;
	PreDealImage(&top_image);
	PreDealImage(&middle_image);
	PreDealImage(&bottom_image);

	//进入设置
	setup_pass_edit.Faddr[0] = setup_pass_edit_Faddr;
	PreDealEdit(&setup_pass_edit, IMAGEUP);
	setup_pass_edit.FontType = 24;
	setup_pass_edit.CursorX = 40;//20;
	setup_pass_edit.CursorY = 36;//16;
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
	setup_pass_edit.CursorCorlor = COLOR_WHITE;

	setup_pass_edit.Text[0] = '\0';
	setup_pass_edit.BoxLen = 0;         //文本框当前输入长度
	setup_pass_edit.MaxLen = 4;         //文本框最大输入长度
	setup_pass_edit.Focus = 0;        //显示输入光标
	setup_pass_edit.Visible = 0;


	setup_key_image.Faddr = setup_key_image_Faddr;
	PreDealImage(&setup_key_image);

	for(i=0; i<15; i++)
	{
		num_button[i].Faddr[0] = num_button_Faddr[i][0];
		num_button[i].Faddr[1] = num_button_Faddr[i][1];
		PreDealImageButton(&num_button[i], IMAGEUP);
		PreDealImageButton(&num_button[i], IMAGEDOWN);
	}

	//设置主窗口
	for(i=0; i<5; i++)
	{
		setup_button[i].Faddr[0] = setup_button_Faddr[i][0];
		setup_button[i].Faddr[1] = setup_button_Faddr[i][1];
		PreDealImageButton(&setup_button[i], IMAGEUP);
		PreDealImageButton(&setup_button[i], IMAGEDOWN);
	}

	//键盘界面
	kb_image.Faddr = kb_image_Faddr;
	PreDealImage(&kb_image);

	kb_edit.Faddr[0] = kb_edit_Faddr;
	PreDealEdit(&kb_edit, IMAGEUP);
	kb_edit.FontType = 24;
	kb_edit.CursorX = 40;//20;
	kb_edit.CursorY = 36;//16;
	if(kb_edit.FontType == 16)
	{
		kb_edit.CursorHeight = 12;
		kb_edit.fWidth = 8;
		kb_edit.Space = 0;
	}
	else
	{
		kb_edit.CursorHeight = 24;
		kb_edit.fWidth = 16;
		kb_edit.Space = 2;
	}
	kb_edit.CursorCorlor = COLOR_WHITE;

	kb_edit.Text[0] = '\0';
	kb_edit.BoxLen = 0;         //文本框当前输入长度
	kb_edit.MaxLen = 4;         //文本框最大输入长度
	kb_edit.Focus = 0;        //显示输入光标
	kb_edit.Visible = 0;


	for(i=0; i<15; i++)
	{
		kb_button[i].Faddr[0] = kb_button_Faddr[i][0];
		kb_button[i].Faddr[1] = kb_button_Faddr[i][1];
		PreDealImageButton(&kb_button[i], IMAGEUP);
		PreDealImageButton(&kb_button[i], IMAGEDOWN);
	}

	//LAN设置窗口
	lan_image.Faddr = lan_image_Faddr;
	PreDealImage(&lan_image);
	for(i=0; i<2; i++)
	{
		lan_button[i].Faddr[0] = lan_button_Faddr[i][0];
		lan_button[i].Faddr[1] = lan_button_Faddr[i][1];
		PreDealImageButton(&lan_button[i], IMAGEUP);
		PreDealImageButton(&lan_button[i], IMAGEDOWN);
	}

	for(i=0; i<5; i++)
	{
		lan_edit[i] = setup_pass_edit;
		lan_edit[i].FontType = 16;
		lan_edit[i].MaxLen = 15;
	}

	lan_edit[0].Faddr[0] = lan_edit_Faddr[0];
	PreDealEdit(&lan_edit[0], IMAGEUP);
	lan_edit[0].Faddr[1] = lan_edit_Faddr[1];
	PreDealEdit(&lan_edit[0], IMAGEDOWN);

	for(i=1; i<5; i++)
		lan_edit[i] = lan_edit[0];

	lan_edit[0].xLeft = 180;
	lan_edit[0].yTop = 119;
	lan_edit[1].xLeft = 534;
	lan_edit[1].yTop = 119;
	lan_edit[2].xLeft = 180;
	lan_edit[2].yTop = 186;
	lan_edit[3].xLeft = 534;
	lan_edit[3].yTop = 186;
	lan_edit[4].xLeft = 180;
	lan_edit[4].yTop = 254;

	//房号设置窗口
	addr_image.Faddr = addr_image_Faddr;
	PreDealImage(&addr_image);

	for(i=0; i<2; i++)
	{
		addr_button[i].Faddr[0] = addr_button_Faddr[i][0];
		addr_button[i].Faddr[1] = addr_button_Faddr[i][1];
		PreDealImageButton(&addr_button[i], IMAGEUP);
		PreDealImageButton(&addr_button[i], IMAGEDOWN);
	}

	for(i=0; i<8; i++)
		addr_edit[i] = lan_edit[0];

	addr_edit[0].xLeft = 180;
	addr_edit[0].yTop = 97;
	addr_edit[1].xLeft = 180;
	addr_edit[1].yTop = 157;
	addr_edit[2].xLeft = 180;
	addr_edit[2].yTop = 217;
	addr_edit[3].xLeft = 180;
	addr_edit[3].yTop = 277;

	addr_edit[4].xLeft = 534;
	addr_edit[4].yTop = 97;
	addr_edit[5].xLeft = 534;
	addr_edit[5].yTop = 157;
	addr_edit[6].xLeft = 534;
	addr_edit[6].yTop = 217;
	addr_edit[7].xLeft = 534;
	addr_edit[7].yTop = 277;

	//修改密码窗口
	pass_image.Faddr = pass_image_Faddr;
	PreDealImage(&pass_image);

	for(i=0; i<2; i++)
	{
		pass_button[i] = lan_button[i];
	}

	for(i=0; i<3; i++)
		pass_edit[i] = lan_edit[0];

	pass_edit[0].xLeft = 366;
	pass_edit[0].yTop = 119;
	pass_edit[1].xLeft = 366;
	pass_edit[1].yTop = 186;
	pass_edit[2].xLeft = 366;
	pass_edit[2].yTop = 252;
	pass_edit[0].FontType = 24;
	pass_edit[1].FontType = 24;
	pass_edit[2].FontType = 24;

	//屏幕调节窗口
	screen_image.Faddr = screen_image_Faddr;
	PreDealImage(&screen_image);

	for(i=0; i<3; i++)
	{
		screen_button[i].Faddr[0] = screen_button_Faddr[i][0];
		screen_button[i].Faddr[1] = screen_button_Faddr[i][1];
		PreDealImageButton(&screen_button[i], IMAGEUP);
		PreDealImageButton(&screen_button[i], IMAGEDOWN);
	}

	for(i=0; i<6; i++)
	{
		brightness_image[i].Faddr = brightness_image_Faddr[i];
		PreDealImage(&brightness_image[i]);
	}
	//音量调节窗口
	voice_image.Faddr = voice_image_Faddr;
	PreDealImage(&voice_image);

	for(i=0; i<5; i++)
	{
		voice_button[i].Faddr[0] = voice_button_Faddr[i][0];
		voice_button[i].Faddr[1] = voice_button_Faddr[i][1];
		PreDealImageButton(&voice_button[i], IMAGEUP);
		PreDealImageButton(&voice_button[i], IMAGEDOWN);
	}

	for(i=0; i<6; i++)
	{
		spk_image[i].Faddr = spk_image_Faddr[i];
		PreDealImage(&spk_image[i]);
	}

	for(i=0; i<6; i++)
	{
		mic_image[i].Faddr = mic_image_Faddr[i];
		PreDealImage(&mic_image[i]);
	}

	//关于窗口
	for(i=0; i<2; i++)
	{
		version_button[i].Faddr[0] = version_button_Faddr[i][0];
		version_button[i].Faddr[1] = version_button_Faddr[i][1];
		PreDealImageButton(&version_button[i], IMAGEUP);
		PreDealImageButton(&version_button[i], IMAGEDOWN);
	}

	//触摸屏校准窗口
	calibrate_image.Faddr = calibrate_image_Faddr;
	PreDealImage(&calibrate_image);

	for(i=0; i<1; i++)
	{
		cross_button[i].Faddr[0] = cross_button_Faddr[i][0];
		cross_button[i].Faddr[1] = cross_button_Faddr[i][1];
		PreDealImageButton(&cross_button[i], IMAGEUP);
		PreDealImageButton(&cross_button[i], IMAGEDOWN);
	}

	//呼出窗口
	call_image.Faddr = setup_key_image.Faddr;
	PreDealImage(&call_image);

	r2r_edit = setup_pass_edit;
	r2r_edit.BoxLen = 0;         //文本框当前输入长度
	r2r_edit.MaxLen = 10;         //文本框最大输入长度

	//呼入窗口
	talkpic_image.Faddr = talkpic_image_Faddr;
	PreDealImage(&talkpic_image);

	for(i=0; i<3; i++)
	{
		talkpic_button[i].Faddr[0] = talkpic_button_Faddr[i][0];
		talkpic_button[i].Faddr[1] = talkpic_button_Faddr[i][1];
		PreDealImageButton(&talkpic_button[i], IMAGEUP);
		PreDealImageButton(&talkpic_button[i], IMAGEDOWN);
	}

	//监视视频窗口
	watchpic_image.Faddr = watchpic_image_Faddr;
	PreDealImage(&watchpic_image);

	for(i=0; i<1; i++)
	{
		watchpic_button[i].Faddr[0] = watchpic_button_Faddr[i][0];
		watchpic_button[i].Faddr[1] = watchpic_button_Faddr[i][1];
		PreDealImageButton(&watchpic_button[i], IMAGEUP);
		PreDealImageButton(&watchpic_button[i], IMAGEDOWN);
	}

	//监视窗口
	watchtarget_image.Faddr = watchtarget_image_Faddr;
	PreDealImage(&watchtarget_image);

	for(i=0; i<3; i++)
	{
		watch_button[i].Faddr[0] = watch_Faddr[i][0];
		watch_button[i].Faddr[1] = watch_Faddr[i][1];
		PreDealImageButton(&watch_button[i], IMAGEUP);
		PreDealImageButton(&watch_button[i], IMAGEDOWN);
	}

	//布防窗口
	for(i=0; i<2; i++)
	{
		fortify_button[i].Faddr[0] = fortify_button_Faddr[i][0];
		fortify_button[i].Faddr[1] = fortify_button_Faddr[i][1];
		PreDealImageButton(&fortify_button[i], IMAGEUP);
		PreDealImageButton(&fortify_button[i], IMAGEDOWN);
	}

	//撤防窗口
	//密码窗口文本框
	cancelfortify_edit = setup_pass_edit;
	cancelfortify_edit.BoxLen = 0;         //文本框当前输入长度
	cancelfortify_edit.MaxLen = 4;         //文本框最大输入长度

	//报警图像
	alarm_main_image.Faddr = alarm_main_image_Faddr;
	PreDealImage(&alarm_main_image);

	for(i=0; i<2; i++)
	{
		alarm_image[i].Faddr = alarm_image_Faddr[i];
		PreDealImage(&alarm_image[i]);
	}

	//信息窗口
	info_image.Faddr = info_image_Faddr;
	PreDealImage(&info_image);

	for(i=0; i<3; i++)
	{
		info_button[i].Faddr[0] = info_button_Faddr[i][0];
		info_button[i].Faddr[1] = info_button_Faddr[i][1];
		PreDealImageButton(&info_button[i], IMAGEUP);
		PreDealImageButton(&info_button[i], IMAGEDOWN);
	}

	for(i=0; i<INFONUMPERPAGE; i++)
	{
		inforow_button[i].Faddr[0] = inforow_button_Faddr[i][0];
		inforow_button[i].Faddr[1] = inforow_button_Faddr[i][1];
		PreDealImageButton(&inforow_button[i], IMAGEUP);
		PreDealImageButton(&inforow_button[i], IMAGEDOWN);
	}

	//信息查看窗口
	infowin_image.Faddr = infowin_image_Faddr;
	PreDealImage(&infowin_image);

	for(i=0; i<3; i++)
	{
		infowin_button[i].Faddr[0] = infowin_button_Faddr[i][0];
		infowin_button[i].Faddr[1] = infowin_button_Faddr[i][1];
		PreDealImageButton(&infowin_button[i], IMAGEUP);
		PreDealImageButton(&infowin_button[i], IMAGEDOWN);
	}

	infocontent_image.Faddr = infocontent_image_Faddr;
	PreDealImage(&infocontent_image);

	//通话记录窗口
	records_image = info_image;
	for(i=0; i<3; i++)
		records_button[i] = info_button[i];
	for(i=0; i<INFONUMPERPAGE; i++)
		recordsrow_button[i] = inforow_button[i];

	for(i=0; i<RECORDNUMPERPAGE; i++)
	{
		recordsicon_button[i].Faddr[0] = recordsicon_button_Faddr[i][0];
		recordsicon_button[i].Faddr[1] = recordsicon_button_Faddr[i][1];
		PreDealImageButton(&recordsicon_button[i], IMAGEUP);
		PreDealImageButton(&recordsicon_button[i], IMAGEDOWN);
	}

	for(i=0; i<RECORDNUMPERPAGE; i++)
	{
		records_pic_icon[i].Faddr = records_pic_icon_Faddr[i];
		PreDealImage(&records_pic_icon[i]);
	}

	//呼叫数组 20131129
	for(i=0; i<RECORDNUMPERPAGE; i++)
	{
		records_call_icon[i].Faddr[0] = records_call_icon_Faddr[i];
		records_call_icon[i].Faddr[1] = records_call_icon_Faddr[i];
		PreDealImageButton(&records_call_icon[i],IMAGEUP);
		PreDealImageButton(&records_call_icon[i], IMAGEDOWN);
	}

	//通话记录查看窗口
	picwin_image.Faddr = picwin_image_Faddr;
	PreDealImage(&picwin_image);

	for(i=0; i<3; i++)
	{
		picwin_button[i].Faddr[0] = picwin_button_Faddr[i][0];
		picwin_button[i].Faddr[1] = picwin_button_Faddr[i][1];
		PreDealImageButton(&picwin_button[i], IMAGEUP);
		PreDealImageButton(&picwin_button[i], IMAGEDOWN);
	}

	pic_blank_image.Faddr = pic_blank_image_Faddr;
	PreDealImage(&pic_blank_image);

	//测试窗口
	test_image.Faddr = test_image_Faddr;
	PreDealImage(&test_image);

	test_image1.Faddr = test_image1_Faddr;
	PreDealImage(&test_image1);

	for(i=0; i<1; i++)
	{
		test_button[i].Faddr[0] = test_button_Faddr[i][0];
		test_button[i].Faddr[1] = test_button_Faddr[i][1];
		PreDealImageButton(&test_button[i], IMAGEUP);
		PreDealImageButton(&test_button[i], IMAGEDOWN);
	}

	label_null_image.Faddr = label_null_image_Faddr;
	PreDealImage(&label_null_image);

	main_image11111.Faddr = main_image11111_Faddr;
	PreDealImage(&main_image11111);

    //switch 窗口
    for(i=0; i<50; i++)
	{
		switch_in_button[i].Faddr[0] = switch_in_button_Faddr[i][0];
		switch_in_button[i].Faddr[1] = switch_in_button_Faddr[i][1];

        switch_in_button[i].xLeft=26+70*i;
        switch_in_button[i].yTop=145;
       
		PreDealImageButton(&switch_in_button[i], IMAGEUP);
		PreDealImageButton(&switch_in_button[i], IMAGEDOWN);
	}

    for(i=0; i<12; i++)
	{
		switch_set_button[i].Faddr[0] = switch_set_button_Faddr[i][0];
		switch_set_button[i].Faddr[1] = switch_set_button_Faddr[i][1];

		PreDealImageButton(&switch_set_button[i], IMAGEUP);
		PreDealImageButton(&switch_set_button[i], IMAGEDOWN);
	}

    
	soundtip_wav.Faddr =      0x00768000UL;
	soundtip2_wav.Faddr =      0x0077D000UL;
	ring_wav.Faddr =          0x00700000UL;//0x0076C000UL;//0x00700000UL;//0x0076C000UL;
	modisucc_wav.Faddr =      0x0070D000UL;
	modifail_wav.Faddr =      0x00714000UL;
	passerror_wav.Faddr =     0x0071B000UL;
	cancelfortify_wav.Faddr = 0x0072F000UL;
	defencedelay_wav.Faddr =  0x00735000UL;
	fortify_wav.Faddr =       0x00732000UL;
	cancelalarm_wav.Faddr =   0x0072B000UL;
	alarmdelay_wav.Faddr =    0x00721000UL;
	alarm_wav.Faddr =         0x00727000UL;

	inputerror_wav.Faddr =    0x00738000UL;

	backring_wav.Faddr =          0x0073C000UL;

	null_wav.Faddr = 0x00769000UL;
	
	//20130902 开机播音
    splash_wav.Faddr=0x00420000UL;
	//20131205 门铃声
    doorbell_wav.Faddr=0x0077E000UL;

	//wav_1k[0].Faddr = 0x00600000UL;   //1K 0db
	//wav_1k[1].Faddr = 0x0060C000UL;   //1K -3db
	//wav_1k[2].Faddr = 0x00616000UL;   //1K -6db
	//wav_1k[3].Faddr = 0x00620000UL;   //1K -9db
	//wav_1k[4].Faddr = 0x0062A000UL;   //1K -12db
	//wav_1k[5].Faddr = 0x00632000UL;   //1K -15db
	//wav_1k[6].Faddr = 0x0063C000UL;   //1K -18db
	//wav_1k[7].Faddr = 0x00646000UL;   //1K -21db
	//wav_1k[8].Faddr = 0x00650000UL;   //1K -24db
	//wav_1k[9].Faddr = 0x0065A000UL;   //1K -27db
	//wav_1k[10].Faddr = 0x00664000UL;   //1K -30db
	//wav_1k[11].Faddr = 0x0066E000UL;   //1K -33db
	//wav_1k[12].Faddr = 0x00678000UL;   //1K -36db
	//wav_1k[13].Faddr = 0x00682000UL;   //1K -39db
	//wav_1k[14].Faddr = 0x0068C000UL;   //1K -42db
	//wav_1k[15].Faddr = 0x00696000UL;   //1K -45db
	//wav_1k[16].Faddr = 0x006A0000UL;   //1K -48db
	//wav_1k[17].Faddr = 0x006AA000UL;   //1K -51db
	//wav_1k[18].Faddr = 0x006B4000UL;   //1K -54db
	//wav_1k[19].Faddr = 0x006BE000UL;   //1K -57db
    //wav_1k[20].Faddr = 0x006C8000UL;   //1K -60db
}
//---------------------------------------------------------------------------
//供lib 调用
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
//---------------------------------------------------------------------------
void DisplayFirstWindow(void)    //显示主窗口
{

	INT8U i;

	#ifdef MAIN_SERIALNO
	if(Local.WorkStatus !=1)
	{
	    gra_dispcn24("机器未注册，请联系蓝天科技...",96,200,COLOR_WHITE,COLOR_BLUE,NOFULLPAGE);
		return;
	}
	#endif

    //display_text(0, 0, 0, COLOR_BLACK, "蓝天网络科技", 0, 24, FULLPAGE);

	//xtm_printf("11\n");
	if((SCREEN_WIDTH == 800)&&(SCREEN_HEIGHT == 480))
	{
		DisplayImage(&main_image, FROMFILE, FULLPAGE);       // FROMFILE
	}

	//xtm_printf("22\n");

	DisplayImage(&logo_image, FROMFILE, FULLPAGE);

	//2013.08.30 增加主界面按钮数目 5-6	
	for(i=0; i<MENU_BTN_COUNT; i++)
	{
		//xtm_printf("i = %d\n", i);
		DisplayImageButton(&menu_button[i], IMAGEUP, FROMFILE, FULLPAGE);
	}
	//显示状态条
	DisplayStateBar();    

	// display_text(90, 268, 0, COLOR_WHITE, "请输入密码:", 0, 24, FULLPAGE);
	// display_text(90, 308, 0, COLOR_BLACK, "请输入密码:", 0, 24, FULLPAGE);
	// display_text(90, 348, 0, COLOR_WHITE, "请输入密码:", 0, 24, FULLPAGE);


	Local.CurrentWindow = 0;

	Local.PreStatus = 0;  //20130115 预状态，防止同时的操作

	Local.fb_page = !Local.fb_page;
	selectym(Local.fb_page);

	Local.FirstWindowFlag = 1;     //返回首页标志
	Local.FirstWindowTime = 0;     //返回首页 计数

	//DisplaySplashWindow(REFRESH_TOP| REFRESH_BOTTOM); 
	 
    //DelayMs(500);
    //噪音制造者
	//SoundSetSpeakerVolume(LocalCfg.SpkVolume);

	//display_text(5, 460, 0, COLOR_RED, SerialNo.no, 0, 15, NOFULLPAGE);
    
#if 0
	Local.Test_FrameSize = VIDEO_VGA;
	StartRecVideo(Local.Test_FrameSize);

	if(Local.Test_FrameSize == VIDEO_VGA)
		DisplayVideoWindow(0, 640, 0, 480, LOCAL_VIDEO);
#endif    
}
//---------------------------------------------------------------------------
//检查返回首页标志
void CheckFirstWindowFlag(void)
{
	if(Local.CurrentWindow == 0)
		if(Local.FirstWindowFlag == 1)     //返回首页标志
		{
			Local.FirstWindowTime ++;     //返回首页 计数
			if(Local.FirstWindowTime > 3*TIMERPERSEC)
			{
				Local.FirstWindowFlag = 0;
				Local.FirstWindowTime = 0;
				StopPlayWavFile();

			}
		}
}
//---------------------------------------------------------------------------
void PreDealFirstWindow(INT8U wintype)    //主窗口预处理，按键按下效果
{
	//2013.08.30 新增按钮+1 5-6
	if((wintype >= 0)&&(wintype <= 5))
		DisplayImageButton(&menu_button[wintype], IMAGEDOWN, FROMFILE, NOFULLPAGE);
}
//---------------------------------------------------------------------------
void OperateFirstWindow(INT8U wintype, INT8U currwindow)    //主窗口操作
{
	//2013.08.30 新增按钮+1 5-6  
	//20130901 6改7
	if(wintype < 7)
		DisplayImageButton(&menu_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);
	switch(wintype)
	{
	case 0:
		if(Local.isHost != '0')  //副机, 安防功能无效
			break;    
		switch(LocalCfg.DefenceStatus)
		{
		case 0://未布防
			if(Local.CurrentWindow != 32)
				DisplayFortifyWindow(REFRESH_MIDDLE | REFRESH_BOTTOM);
			break;
		case 1://外出布防
		case 2://在家布防
		case 11://外出布防延时中
		case 12://在家布防延时中
			if(Local.CurrentWindow != 31)
				DisplayCancelFortifyWindow(REFRESH_MIDDLE | REFRESH_BOTTOM);
			break;
		}
		break;
	case 1://显示对讲呼出窗口
		DisplayTalkWindow(REFRESH_MIDDLE | REFRESH_BOTTOM);
		break;
	case 2://显示监视窗口
		DisplayWatchWindow(REFRESH_MIDDLE | REFRESH_BOTTOM);    //刷新 中 下
		break;
	case 3://显示信息窗口
		DisplayInfoWindow(REFRESH_MIDDLE | REFRESH_BOTTOM, 0);    //刷新 中 下
		break;
	case 4://显示通话记录窗口
		DisplayRecordsWindow(REFRESH_MIDDLE | REFRESH_BOTTOM, 0);    //刷新 中 下
		break;
	case 5: //设置
		//DisplaySetupWindow(REFRESH_MIDDLE | REFRESH_BOTTOM);
		//DisplaySplashWindow(REFRESH_MIDDLE | REFRESH_BOTTOM);
		//DisplaySwitchWindow(REFRESH_MIDDLE | REFRESH_BOTTOM);
		break;
	case 6://splash
		//DisplaySplashWindow(REFRESH_TOP| REFRESH_BOTTOM);
		//DisplaySwitchWindow(REFRESH_MIDDLE | REFRESH_BOTTOM);
		break;
	}
}
//---------------------------------------------------------------------------
void CheckTouchFirstWindow(INT16U XLCD, INT16U YLCD) //检查触摸屏操作
{
	INT8U i;
	INT8U isInTouch;
	INT8U TouchNum;
	isInTouch = 0;

	if(isInTouch == 0)
		for(i=0; i<MENU_BTN_COUNT; i++)  //窗口按钮 2013.08.31 按钮增加5-6
		{
			isInTouch = CheckTSInButton(&menu_button[i], XLCD, YLCD);
			if(isInTouch == 1)
			{
				if(i==5)
				{
					TouchNum=6; //避免跟logo冲突
				}
				else
				{
					TouchNum = i;//0-4
				}
				break;
			}
		}

		if(isInTouch == 0)
			for(i=2; i<6; i+=2)  //状态栏按钮
			{
				isInTouch = CheckTSInImage(&state_image[i], XLCD, YLCD);
				if(isInTouch == 1)
				{
					TouchNum = 8 + i;//10、12、14
					break;
				}
			}

			if(isInTouch == 0)
			{
			    isInTouch = CheckTSInImage(&logo_image, XLCD, YLCD);
					
				if(isInTouch == 1)
				{  
				    TouchNum = 6;//logo 
				}
			}

			if(isInTouch == 1)
			{
				KeyTouchOperate(TouchNum);
				//OperateFirstWindow(TouchNum, Local.CurrentWindow);
			}
}
//---------------------------------------------------------------------------
void RefreshBackWindow(INT8U RefreshType, INT8U FullPageType) //刷新背景
{
	//if(RefreshType & REFRESH_TOP)
	{
		DisplayImage(&top_image, FROMFILE, FullPageType);
		DisplayImage(&logo_image, FROMFILE, FullPageType);
	}
	//if(RefreshType & REFRESH_MIDDLE)
	DisplayImage(&middle_image, FROMFILE, FullPageType);
	//if(RefreshType & REFRESH_BOTTOM)
	DisplayImage(&bottom_image, FROMFILE, FullPageType);
	//检查安防状态201310
	if(Local.AlarmStatus)
	{
	    DisplayImageButton(&bigmenu_button[7], IMAGEUP, FROMFILE, NOFULLPAGE);
	}
	else
	{
		DisplayImageButton(&bigmenu_button[8], IMAGEUP, FROMFILE, NOFULLPAGE);
	}
}
//---------------------------------------------------------------------------
void DisplayStateBar(void)
{
	//0--网络断线
	//1--网络正常
	//2--无新短信
	//3--有新短信
	//4--无新留影
	//5--有新留影
	INT8U i;

	if(Local.isHost != '0')  //副机, 布撤防无效
		LocalCfg.DefenceStatus = 0;

	if((LocalCfg.DefenceStatus != 0)&&(LocalCfg.DefenceStatus != 1)&&(LocalCfg.DefenceStatus != 2)&&(LocalCfg.DefenceStatus != 11)&&(LocalCfg.DefenceStatus != 12))
		LocalCfg.DefenceStatus = 0;    

	if(Local.NetLanStatus == 1)  //网络状态 0 断开  1 接通
		DisplayImage(&state_image[1], FROMFILE, FULLPAGE);
	else
		DisplayImage(&state_image[0], FROMFILE, FULLPAGE);

	//有新信息
	if(Local.NewInfo == 1)
		DisplayImage(&state_image[3], FROMFILE, FULLPAGE);
	else
		DisplayImage(&state_image[2], FROMFILE, FULLPAGE);

	//有新留影
	if(Local.NewRecPic == 1)
		DisplayImage(&state_image[5], FROMFILE, FULLPAGE);
	else
		DisplayImage(&state_image[4], FROMFILE, FULLPAGE);

	/*  //布防状态
	if((LocalCfg.DefenceStatus == 1)||(LocalCfg.DefenceStatus == 2)||(LocalCfg.DefenceStatus == 3)||(LocalCfg.DefenceStatus == 4)||(LocalCfg.DefenceStatus == 5))
	DisplayImage(&state_image[3], MAINYUVFBPAGE, FROMMEMORY);
	else
	DisplayImage(&state_image[2], MAINYUVFBPAGE, FROMMEMORY);      */

	/*  DisplayClock(REFRESH); //显示时钟
	if(Local.interface_init_flag == 1)
	ShowWeather();  //显示天气     */
}
//---------------------------------------------------------------------------
void PlaySoundTip(void) //播放提示音
{
	if(LocalCfg.VoiceHint == 1)
	{
		WaitAudioUnuse(200); //等待声卡空闲               //  wav_1k[5].Faddr
		StartPlayWav(soundtip_wav.Faddr, NO_CYCLE_PLAY);       // soundtip_wav.Faddr
		//新的按钮声音201311
		//StartPlayWav(soundtip2_wav.Faddr, NO_CYCLE_PLAY);       // soundtip_wav.Faddr
	}
}
//---------------------------------------------------------------------------
void PlaySoundDoorBell(void) //20131205
{
	if(LocalCfg.VoiceHint == 1)
	{
		WaitAudioUnuse(200); //等待声卡空闲               //  wav_1k[5].Faddr
		StartPlayWav(doorbell_wav.Faddr, NO_CYCLE_PLAY);       // soundtip_wav.Faddr
	}
}
//---------------------------------------------------------------------------
void PreDealBigmenuButton(INT8U wintype)  //快捷按键 预处理
{
	if((wintype >= 100)&&(wintype <= 106))
	{   //安防状态201310
	    if(wintype==101)
	    {
	    	if(Local.AlarmStatus)
	    	{
	    	    DisplayImageButton(&bigmenu_button[7], IMAGEDOWN, FROMFILE, NOFULLPAGE);
	    	 }
			else
			{
				DisplayImageButton(&bigmenu_button[8], IMAGEDOWN, FROMFILE, NOFULLPAGE);
			}
	    }
		else
		{
		    DisplayImageButton(&bigmenu_button[wintype-100], IMAGEDOWN, FROMFILE, NOFULLPAGE);
		}
	}
}
//---------------------------------------------------------------------------
void ShowPressQuickmenu_Button(INT8U wintype)  //快捷按键  弹起
{
	if((wintype >= 100)&&(wintype <= 106))
	{
        //检查安防状态201310
		if(wintype==101)
			{
		        if(Local.AlarmStatus)
	    	    	{
	    	    	     DisplayImageButton(&bigmenu_button[7], IMAGEUP, FROMFILE, NOFULLPAGE);
	    	    	}
				else
					{
					     DisplayImageButton(&bigmenu_button[8], IMAGEUP, FROMFILE, NOFULLPAGE);
					}
			}
		else
		{
		DisplayImageButton(&bigmenu_button[wintype-100], IMAGEUP, FROMFILE, NOFULLPAGE);
		}
	}
	
}
//---------------------------------------------------------------------------
void OperateQuickmenu(INT8U wintype, INT8U currtype)  //最下面的菜单栏操作
{
	switch(wintype)
	{
	case 100:  //首页
		if(currtype != wintype)
			DisplayFirstWindow();
		break;
	case 101:  //安防
		if(currtype != wintype)
			switch(LocalCfg.DefenceStatus)
		{
			case 0://未布防
				if(Local.CurrentWindow != 32)
					DisplayFortifyWindow(REFRESH_MIDDLE);
				break;
			case 1://外出布防
			case 2://在家布防
			case 11://外出布防延时中
			case 12://在家布防延时中
				if(Local.CurrentWindow != 31)
					DisplayCancelFortifyWindow(REFRESH_MIDDLE);
				break;
		}
		break;
	case 102:  //呼叫
		if(currtype != wintype)
			DisplayTalkWindow(REFRESH_MIDDLE);
		break;
	case 103:  //监视
		if(currtype != wintype)
			DisplayWatchWindow(REFRESH_MIDDLE);    //刷新 中 
		break;
	case 104: //信息
		if(currtype != wintype)
			DisplayInfoWindow(REFRESH_MIDDLE, 0);    //刷新 中
		break;
	case 105:  //记录
		if(currtype != wintype)
			DisplayRecordsWindow(REFRESH, 0);    //显示照片窗口（一级）
		break;
	case 106:  //系统设置
		if(currtype != wintype)
			DisplaySetupWindow(REFRESH_MIDDLE);
		break;
	}
}
//---------------------------------------------------------------------------
void DisplayInfoStateImage(INT8U State)    //显示信息图标
{
	if(Local.CurrentWindow == 0)
	{
		if(State == 0)
			DisplayImage(&state_image[2], FROMFILE, NOFULLPAGE);
		else
			DisplayImage(&state_image[3], FROMFILE, NOFULLPAGE);
	}
}
//---------------------------------------------------------------------------
void DisplayPictureStateImage(INT8U State)    //显示留影图标
{
	if(Local.CurrentWindow == 0)
	{
		if(State == 0)
			DisplayImage(&state_image[4], FROMFILE, NOFULLPAGE);
		else
			DisplayImage(&state_image[5], FROMFILE, NOFULLPAGE);
	}
}
//---------------------------------------------------------------------------
void DisplayLTpwWindow(INT8U RefreshType,INT8U CurrentWindowNo) //显示进入密码窗口
{
  INT8U i;

  RefreshBackWindow(RefreshType, FULLPAGE); //刷新背景

  DisplayEdit(&setup_pass_edit, IMAGEUP, FROMFILE, FULLPAGE);

  DisplayImage(&setup_key_image, FROMFILE, FULLPAGE);

  WaitDecodeFinish();
  if(CurrentWindowNo==40)
  	{
  display_text(setup_pass_edit.xLeft, setup_pass_edit.yTop - 32, 0, COLOR_BLACK, 
  "请输入屏幕设置密码:", 0, 24, FULLPAGE);
  	}
  else
  	{
  	  display_text(setup_pass_edit.xLeft, setup_pass_edit.yTop - 32, 0, COLOR_BLACK, 
  "请输入对应密码:", 0, 24, FULLPAGE);
  	}
  setup_pass_edit.BoxLen = 0;

  strcpy(num_button[12].buttonname, "删除");
  strcpy(num_button[13].buttonname, "确认");
 #ifdef _CAMERA_SUPPORT     //如不用摄像头，GPIOB6、7为Comm1, GPIOC 作为8个防区使用   测试模式
  strcpy(num_button[14].buttonname, "测试");   // 测试
 #else
  strcpy(num_button[14].buttonname, "关于");
 #endif
  //按钮
  for(i=12; i<15; i++)
   {
    num_button[i].FontType = 24;
    num_button[i].FontColor = COLOR_BLACK;
    num_button[i].Text_xLeft = (num_button[i].width - strlen(num_button[i].buttonname)*12)/2;
    num_button[i].Text_yTop = (num_button[i].height - 24)/2;
    //xtm_printf("%d   %d\n", num_button[i].Text_xLeft, num_button[i].Text_yTop);
   }

  for(i=12; i<15; i++)
    DisplayImageButton_Text(&num_button[i], num_button[i].FontColor, 2, FULLPAGE);
  Local.CurrentWindow = CurrentWindowNo;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//---------------------------------------------------------------------------
void PreDealLTpwWindow(INT8U wintype)   //进入密码窗口预处理，按键按下效果
{
  if((wintype >= 0)&&(wintype <= 14))
   {
    DisplayImageButton(&num_button[wintype], IMAGEDOWN, FROMFILE, NOFULLPAGE);
   }

  if((wintype >= 12)&&(wintype <= 14))
   {
    DisplayImageButton_Text(&num_button[wintype], num_button[wintype].FontColor, 2, NOFULLPAGE);
   }

  PreDealBigmenuButton(wintype);  //快捷按键 预处理
}
//---------------------------------------------------------------------------
void OperateLTpwWindow(INT8U wintype, INT8U currwindow)    //进入设置窗口操作
{
  char str[5];
  if(wintype < 15)
    DisplayImageButton(&num_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);
  if((wintype >= 12)&&(wintype <= 14))
   {
    DisplayImageButton_Text(&num_button[wintype], num_button[wintype].FontColor, 2, NOFULLPAGE);
   }

  ShowPressQuickmenu_Button(wintype);  //快捷按键  弹起

  WaitDecodeFinish();
  switch(wintype)
   {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
           if(setup_pass_edit.BoxLen < setup_pass_edit.MaxLen)
            {
             str[0] = '0' + wintype;
             str[1] = '\0';
             setup_pass_edit.Text[setup_pass_edit.BoxLen] = str[0];
             str[0] = '*';
             str[1] = '\0';

             display_text(setup_pass_edit.xLeft + setup_pass_edit.CursorX + setup_pass_edit.BoxLen*setup_pass_edit.fWidth,
                     setup_pass_edit.yTop + setup_pass_edit.CursorY - 2, setup_pass_edit.Space, setup_pass_edit.CursorCorlor, str, 0, 24, NOFULLPAGE);

             setup_pass_edit.BoxLen ++;
            }
           break;
    case 10: //清除
    case 12:
          setup_pass_edit.Text[0] = 0;
          setup_pass_edit.BoxLen = 0;
          DisplayEdit(&setup_pass_edit, IMAGEUP, FROMFILE, NOFULLPAGE);
          break;
    case 11: //确认
    case 13: //确认
          xtm_printf("OperateEPWWindow::setup_pass_edit.BoxLen = %d, LocalCfg.EngineerPass = %s\n", setup_pass_edit.BoxLen, LocalCfg.EngineerPass);
          setup_pass_edit.Text[setup_pass_edit.BoxLen] = '\0';
          LocalCfg.EngineerPass[4] = '\0';
		  //SerialNo.pw[5]='\0';
          
//          if(strcmp(setup_pass_edit.Text, LocalCfg.EngineerPass) == 0)

          //if((strcmp(setup_pass_edit.Text, SerialNo.pw) == 0)||(strcmp(setup_pass_edit.Text, "8023") == 0))
            
          if(1)
           {
            //DisplaySetupMainWindow(REFRESH_MIDDLE);    //刷新 中
            //DisplaySplashWindow(REFRESH_MIDDLE);

		     //DisplayTalkWatchPicWindow(TALKWATCHRETURNTYPE, TALKWINTYPE);
             //CallR2R_Func(TalkCall.TalkText);
             //这里进入程序入口
             if(currwindow==40)
             {
             	//这里加载
             }
			 else
			 	{
			 	
			 	}
			
           }
          else
           {
            PlayPassErrorWav();

            //清除输入框
            setup_pass_edit.Text[0] = 0;
            setup_pass_edit.BoxLen = 0;
            DisplayEdit(&setup_pass_edit, IMAGEUP, FROMFILE, NOFULLPAGE);
           }
          break;
    case 14: //关于  测试
         #ifdef _CAMERA_SUPPORT     //如不用摄像头，GPIOB6、7为Comm1, GPIOC 作为8个防区使用   测试模式
          //DisplayTestWindow(REFRESH);
         #else
          //DisplayVersionWindow(REFRESH_MIDDLE);
         #endif
          break;
    default:
          OperateQuickmenu(wintype, 106);
          break;

   }
}
//---------------------------------------------------------------------------
void CheckTouchLTpwWindow(INT16U XLCD, INT16U YLCD) //检查触摸屏操作
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   for(i=0; i<15; i++)  //窗口按钮
    {
     isInTouch = CheckTSInButton(&num_button[i], XLCD, YLCD);
     if(isInTouch == 1)
      {
       TouchNum = i;
       break;
      }
    }

  if(isInTouch == 0)
   {
    TouchNum = CheckBigmenuFunc(XLCD, YLCD);  //快捷按钮操作
    if(TouchNum >= 100)
      isInTouch = 1;
   }

  if(isInTouch == 1)
   {
    KeyTouchOperate(TouchNum);
   }
}
//--------------------------------------------------------------------------
//ReturnType 0 --  主界面   1 -- Talk Watch 界面   WinType  0 -- Talk窗口  1 -- Watch窗口   3 -- s
void DisplaySmarkStreamWindow(INT8U wintype)    //显示窗口
{
  char CallText[15];
  xtm_printf("DisplaySmarkStreamWindow\n");
  CheckKeyPressAndLcd(); //检查按键并点亮LCD
  DisplayImage(&test_image1, FROMFILE, FULLPAGE);
  DisplayImage(&talkpic_image, FROMFILE, FULLPAGE);
  Local.CurrentWindow = wintype;
  Local.TalkWatchWinType = TALKWINTYPE;
  Local.fb_page = !Local.fb_page;
  selectym(Local.fb_page);
}
//--------------------------------------------------------------------------
void PreDealSmarkStreamWindow(INT8U wintype)    //对讲呼入窗口预处理，按键按下效果
{
    if(Local.TalkWatchWinType == TALKWINTYPE)
     {
      if((wintype >= 0)&&(wintype <= 2))
       {
        //WaitDecodeFinish();
        DisplayImageButton(&talkpic_button[wintype], IMAGEDOWN, FROMFILE, NOFULLPAGE);
       }
     }

}
//--------------------------------------------------------------------------
void OperateSmarkStreamWindow(INT8U wintype, INT8U currwindow)    //对讲呼入窗口操作
{
    xtm_printf("Local.Status = %d, Local.TalkWatchWinType = %d 11111111111\n", Local.Status, Local.TalkWatchWinType);
    if(Local.TalkWatchWinType == TALKWINTYPE)
     {
      if((wintype >= 0)&&(wintype <= 2))
       {
        //WaitDecodeFinish();
        DisplayImageButton(&talkpic_button[wintype], IMAGEUP, FROMFILE, NOFULLPAGE);
       }
     }


  switch(wintype)
    {
     case 0:
            xtm_printf("Local.Status = %d, Local.TalkWatchWinType = %d\n", Local.Status, Local.TalkWatchWinType);
            if((Local.Status == 89))  //状态为被对讲
                {
                    Talk_Func();
                }
            
            
            break;
     case 1:  //开锁
             if((Local.Status == 2)||(Local.Status == 6))  //状态为被对讲 或被叫通话
             //if(Local.Status == 6)  //状态为被对讲
             {
            #ifdef ELEVATORCONTROL_HANGZHOU
               if(Remote.Addr[0][0] == 'S')
                {
                  xtm_memcopy(Local.Elevator_Addr, Remote.Addr[0], 20);
                  xtm_memcopy(Local.Elevator_IP, Remote.IP[0], 4);               
                  ElevatorControl_Func();  //控制电梯
                  break;
                }  
            #endif
              OpenLock_Func();
             }
            break;
     case 2:  //挂断
           if((Local.Status == 89))
             TalkEnd_Func();
            break;
     default:
           OperateQuickmenu(wintype, 102);
           break;
    }

}
//--------------------------------------------------------------------------
void CheckTouchSmarkStreamWindow(INT16U XLCD, INT16U YLCD) //检查触摸屏操作
{
  INT8U i;
  INT8U isInTouch;
  INT8U TouchNum;
  isInTouch = 0;

  if(isInTouch == 0)
   for(i=0; i<3; i++)  //窗口按钮
    {
     isInTouch = CheckTSInButton(&talkpic_button[i], XLCD, YLCD);
     if(isInTouch == 1)
      {
       TouchNum = i;
       break;
      }
    }

  if(isInTouch == 1)
   {
    KeyTouchOperate(TouchNum);
   }
}
//--------------------------------------------------------------------------

