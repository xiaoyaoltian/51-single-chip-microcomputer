﻿/*******************************************************************************
* 版  权 	: 红梅科技工作室
* 论  坛	：https://blog.csdn.net/Qingzhusshuiyun
* Q Q	    ：2369099714
* 版权所有，盗版必究。
* 工  程   : 单个共阳数码管静态显示
* 文件名   : 单个共阳数码管静态显示.c
* 处理器   : STC89C52RC
* 编译环境 : Keil 5
* 系统时钟 : 12MHZ
* 版    本	: V1.0 
* 生成日期	: 2018-05-15	   					
* 修改日期	:  
* 简单描述 :通过赋值给P1，让数码管显示特定的字符或者数字
*******************************************************************************/


#include<reg52.h> //包含头文件，一般情况不需要改动，
//头文件包含特殊功能寄存器的定义


void main (void)
{
 P0=0xc0;   //二进制 为 1100 0000  参考数码管排列，
//可以得出0对应的段点亮，1对应的段熄灭，结果显示数字"0"   
 while(1)
 {
   P2=0x00;
 }
}
