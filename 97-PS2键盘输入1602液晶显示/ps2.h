#ifndef __PS2_H__
#define __PS2_H__


#include<reg52.h>        //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义


void PS2_Init(void);

void Read_KeyBoard(void);

void Decode(unsigned char ScanCode);

#endif


