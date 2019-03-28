#define MAX_STREAM_NUM  2
#ifndef x_main_h
#define x_main_h
extern void Init_LocalVar(void); //��ʼ�� Local �ṹ
extern void Init_Module_Clock(void); //������Ҫ�رո�ģ��ʱ��
extern void Set_VideoCapture_Clock(INT8U Type); //������Ƶ�ɼ�ʱ��  1  ��   0  ��
extern void Set_Encode_Clock(INT8U Type); //������Ƶ����ʱ��  1  ��   0  ��
extern void Set_Decode_Clock(INT8U Type); //������Ƶ����ʱ��  1  ��   0  ��
extern void Set_Disp_Clock(INT8U Type); //������ʾʱ��  1  ��   0  ��

extern void ResetLocalCfg(void);    //��λ�����ļ�

extern void SetPTC1(INT8U freq);
//freq  1  --  ��������16 --  16��Ƶ
extern void SetPTC2(INT8U freq);

//�������ļ�
extern void ReadCfgFile(void);
//д�����ļ�
extern void WriteCfgFile(void);
//д���ݵ�Flash
extern void SaveToFlash(INT32U FlashAddr, INT8U *buf, INT16U length);
//��Flash������
extern void ReadFromFlash(INT32U FlashAddr, INT8U *buf, INT16U length);

//����������ļ�
extern void ReadAdFile(void);
//д��������ļ�
extern void WriteAdFile(void);

extern void ReadSwitchFile(void);

extern void WriteSwitchFile(void);

extern void ReadSerialFile(void);

extern INT8U *jpeg_bitstream;    //��̬jpeg ͼƬ
extern INT8U *Flash_Stream;      //Flash�洢
extern INT8U *Memory6_Stream;

extern INT8U *mjpeg_stream[MAX_STREAM_NUM];   //mjpeg ��
extern INT16U mjpeg_stream_len[MAX_STREAM_NUM];   //mjpeg ��

extern INT8U *AudioSend_Stream;       //����Ƶ����Stream
extern INT8U *VideoSend_Stream;       //��Ƶ����Stream

extern INT8U Curr_Mjpeg_Stream;

extern INT8U *Mdata_Stream;      //��������
extern INT8U *Send_Stream;
extern INT8U *Net_Stream[4];       //256K ����  ��������
#endif