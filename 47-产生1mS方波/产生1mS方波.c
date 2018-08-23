﻿/*******************************************************************************
* 版  权 	: 红梅科技工作室
* 论  坛	：https://blog.csdn.net/Qingzhusshuiyun
* Q Q	    ：2369099714
* 版权所有，盗版必究。
* 工  程   : 产生1mS方波
* 文件名   : 产生1mS方波.c
* 处理器   : STC89C52RC
* 编译环境 : Keil 5
* 系统时钟 : 12MHZ
* 版    本	: V1.0 
* 生成日期	: 2018-06-17	   					
* 修改日期	:  
* 简单描述 : 通过定时产生1mS方波
*******************************************************************************/

#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

sbit OUT=P1^2;    //定义OUT输出端口

/*------------------------------------------------
                    定时器初始化子程序
------------------------------------------------*/
void Init_Timer0(void)
{
 TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响		     
 TH0=0x00;	      //给定初值，这里使用定时器最大值从0开始计数一直到65535溢出
 TL0=0x00;
 EA=1;            //总中断打开
 ET0=1;           //定时器中断打开
 TR0=1;           //定时器开关打开
}
/*------------------------------------------------
                    主程序
------------------------------------------------*/
main()
{
 Init_Timer0();
 while(1);
}

/*------------------------------------------------
                 定时器中断子程序
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 using 1
{
 TH0=(65536-500)/256;		  //重新赋值 12M晶振计算，指令周期1uS，
 TL0=(65536-500)*256;         //1mS方波半个周期500uS，即定时500次
                              //溢出然后输出端取反

 OUT=~OUT;        //用示波器可看到方波输出 

}