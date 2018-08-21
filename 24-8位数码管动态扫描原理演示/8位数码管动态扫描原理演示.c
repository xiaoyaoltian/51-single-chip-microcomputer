﻿/*******************************************************************************
* 版  权 	: 红梅科技工作室
* 论  坛	：https://blog.csdn.net/Qingzhusshuiyun
* Q Q	    ：2369099714
* 版权所有，盗版必究。
* 工  程   : 8位数码管动态扫描显示
* 文件名   : 8位数码管动态扫描显示.c
* 处理器   : STC89C52RC
* 编译环境 : Keil 5
* 系统时钟 : 12MHZ
* 版    本	: V1.0 
* 生成日期	: 2018-05-23	   					
* 修改日期	:  
* 简单描述 : 8位数码管分别显示不同数字，这种扫描显示方式成为动态扫描
*******************************************************************************/

#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换
sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存

unsigned char code dofly_DuanMa[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值01234567
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分别对应相应的数码管点亮,即位码

void Delay(unsigned int t); //函数声明

/*------------------------------------------------
                    主函数
------------------------------------------------*/
main()
{
 unsigned char i=0;
 while(1)
      {
	   DataPort=dofly_WeiMa[i]; //取位码 
       LATCH2=1;     //位锁存
       LATCH2=0;

       DataPort=dofly_DuanMa[i]; //取显示数据，段码
       LATCH1=1;     //段锁存
       LATCH1=0;

	   Delay(60000); // 扫描间隙延时，时间太长会闪烁，太短会造成重影，
       //这里故意延长时间，方便直观看出动态扫描原理
	   i++;
	   if(8==i)    //检测8位扫描完全结束？如扫描完成则从第一个
                   //开始再次扫描8位
	     i=0;
      }
}
/*------------------------------------------------
 延时函数，含有输入参数 unsigned int t，无返回值
 unsigned int 是定义无符号整形变量，其值的范围是
 0~65535
------------------------------------------------*/
void Delay(unsigned int t)
{
 while(--t);
}

