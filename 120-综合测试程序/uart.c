﻿/*******************************************************************************
* 版  权 	: 红梅科技工作室
* 论  坛	：https://blog.csdn.net/Qingzhusshuiyun
* Q Q	    ：2369099714
* 版权所有，盗版必究。
* 工  程   : 综合测试
* 文件名   : 综合测试.c
* 处理器   : STC89C52RC
* 编译环境 : Keil 5
* 系统时钟 : 12MHZ
* 版    本	: V1.0 
* 生成日期	: 2018-08-29	   					
* 修改日期	:  
* 简单描述 : 连接好串口或者usb转串口至电脑，下载该程序，打开电源
        打开串口调试程序，将波特率设置为9600，无奇偶校验
        晶振11.0592MHz，发送和接收使用的格式相同，如都使用
        字符型格式，按复位重启程序，可以看到接收到 UART test
*******************************************************************************/
 
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义                        
#include"uart.h"
#include <stdio.h> 
/*------------------------------------------------
                   函数声明
------------------------------------------------*/
void UartSendStr(unsigned char *s);

/*------------------------------------------------
                    串口初始化
------------------------------------------------*/
void InitUART  (void)
{

    SCON  = 0x50;		        // SCON: 模式 1, 8-bit UART, 使能接收  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重装
    TH1   = 0xFD;               // TH1:  重装值 9600 波特率 晶振 11.0592MHz  
    TR1   = 1;                  // TR1:  timer 1 打开                         
    //EA    = 1;                  //打开总中断
    //ES    = 1;                  //打开串口中断
	TI    = 1;                  //发送允许
}                            

/*------------------------------------------------
                     串口中断程序
------------------------------------------------*/
void UART_SER (void) interrupt 4 //串行中断服务程序
{
    unsigned char Temp;          //定义临时变量 
   
   if(RI)                        //判断是接收中断产生
     {
	  RI=0;                      //标志位清零
	  Temp=SBUF;                 //读入缓冲区的值
	  P1=Temp;                   //把值输出到P1口，用于观察
      SBUF=Temp;                 //把接收到的值再发回电脑端
	 }
   if(TI)                        //如果是发送标志位，清零
     TI=0;
} 

