
#ifndef __LED8X8_H__
#define __LED8X8_H__

void Send1Byte(unsigned char dat);
void Send2Byte(unsigned char dat1,unsigned char dat2);
void Out595(void);
void SendSeg(unsigned char dat);
void LED8x8Display(void);

#endif
