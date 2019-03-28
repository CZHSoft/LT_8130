struct TAudio{
               INT32U Faddr;    //Flash ��ַ
               INT32U WavAddr;    //Flash Data��ַ
               INT32U Len;
              };

#define PCM_REC_MAX  0x2000  //PCM¼�����������ֵ
#define PCM_BUF_NUM  2       //PCM��������ռ�ڴ�ҳ������64K*2
#define PCM_BUF_MAX  0x20000  //PCM�������������ֵ

#ifndef x_audio_h
#define x_audio_h
extern void InitPcm(void);    //20120104 xu

extern void CreateAudioSend_Stream(void);

extern void StartRecAudio(void);
extern void StopRecAudio(void);
extern void StartPlayAudio(void);
extern void StopPlayAudio(void);

extern void CheckPlayAudioBuf(void);

extern void StartPlayWav(INT32U FlashAddr, INT8U PlayFlag); //0 ���β���  1 ѭ������
extern void StartPlayWav_Rec(INT32U FlashAddr, INT8U PlayFlag); //0 ���β���  1 ѭ������
extern void StopPlayWavFile(void);
extern void WaitAudioUnuse(INT16U delayms); //�ȴ���������

extern void StartRecWav(void);
extern void StopRecWav(void);

//��ȡ wav �ļ�
extern INT8U ReadWavFile(INT32U FlashAddr);

extern INT8U *PcmO_Stream[PCM_BUF_NUM];
extern INT8U *PcmI_Stream;
extern INT8U *Rec_Stream;

extern INT32U PcmO_Len;
extern INT32U PcmO_iPut;
extern INT32U PcmO_iGet;
extern INT16U BlockLen;

extern void SetOldMicAdc(INT8U old_value);    //��lib ����
extern void SetOldSpkDac(INT8U old_value);    //��lib ����
extern void SetOldSpkVolume(INT8U old_value);    //��lib ����


extern INT8U Video_Send_Flag;
extern INT8U Audio_Send_Flag;

extern void SetMicVolume(void);
extern void StartNoiseDect(void);
extern void GetIdlePlayValue(INT8U *PlayBuf);

extern void RestoreSpeakerDac(void);      //x_udp.c  ����
#endif

