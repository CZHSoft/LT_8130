#ifndef x_es8388_h
#define x_es8388_h
extern void Writees8388(INT8U RomAddress,INT8U Wdata);
extern void WriteCorrect_es8388(INT8U RomAddress,INT8U Wdata);
extern INT8U Reades8388(INT8U RomAddress);
extern void init_es8388(void);
extern void set_8388_regisger(void); //20130220

extern INT8U SpeakerVolume[6];//0x1E;//0x12;//0x0D;
extern INT8U MicVolume[6];//0x00;//0x80;   //0x40

extern void SoundSetSpeakerVolume(INT8U volume);
extern void SoundSetMicVolume(INT8U volume);

extern void SoundSetSpeakerDac(INT8U volume);
extern void SoundSetMicAdc(INT8U volume);
#endif