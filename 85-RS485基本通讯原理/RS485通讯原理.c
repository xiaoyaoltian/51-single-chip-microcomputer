﻿/*******************************************************************************
* 版  权 	: 红梅科技工作室
* 论  坛	：https://blog.csdn.net/Qingzhusshuiyun
* Q Q	    ：2369099714
* 版权所有，盗版必究。
* 工  程   : RS485通讯原理
* 文件名   : RS485通讯原理.c
* 处理器   : STC89C52RC
* 编译环境 : Keil 5
* 系统时钟 : 12MHZ
* 版    本	: V1.0 
* 生成日期	: 2018-07-25	   					
* 修改日期	:  
* 简单描述 : 485 与 232 使用相同软件协议，是半双工，需要有2套开发板对发测试，或者使用
        另外一个485设备配套，这里提供测试程序
*******************************************************************************/
 
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义                        
#include"delay.h"
/*------------------------------------------------
                硬件端口定义
------------------------------------------------*/
sbit Ctrl_EN = P2^0;  //发送接收控制端
/*------------------------------------------------
                 函数声明
------------------------------------------------*/
void SendStr(unsigned char *s);
/*------------------------------------------------
                串口初始化
------------------------------------------------*/
void InitUART  (void)
{

    SCON  = 0x50;		        // SCON: 模式 1, 8-bit UART, 使能接收  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重装
    TH1   = 0xFD;               // TH1:  重装值 9600 波特率 晶振 11.0592MHz  
    TR1   = 1;                  // TR1:  timer 1 打开                         
    EA    = 1;                  //打开总中断
    //ES    = 1;                  //打开串口中断
}                            
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{

InitUART();

Ctrl_EN=1;  //发送模式

while (1)                       
    {
    SendStr("UART test，技术论坛：www.doflye.net thank you!");
	DelayMs(240);//延时循环发送
    DelayMs(240);
    }
}

/*------------------------------------------------
                    发送一个字节
------------------------------------------------*/
void SendByte(unsigned char dat)
{
 SBUF = dat;
 while(!TI);
      TI = 0;
}
/*------------------------------------------------
                    发送一个字符串
------------------------------------------------*/
void SendStr(unsigned char *s)
{
 while(*s!='\0')// \0 表示字符串结束标志，通过检测是否字符串末尾
  {
  SendByte(*s);
  s++;
  }
}
