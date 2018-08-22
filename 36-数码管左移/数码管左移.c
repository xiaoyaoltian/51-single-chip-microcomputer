﻿/*******************************************************************************
* 版  权 	: 红梅科技工作室
* 论  坛	：https://blog.csdn.net/Qingzhusshuiyun
* Q Q	    ：2369099714
* 版权所有，盗版必究。
* 工  程   : 数码管左移
* 文件名   : 数码管左移.c
* 处理器   : STC89C52RC
* 编译环境 : Keil 5
* 系统时钟 : 12MHZ
* 版    本	: V1.0 
* 生成日期	: 2018-06-06	   					
* 修改日期	:  
* 简单描述 : 多位数码管分别显示不同数字，这种扫描显示方式成为动态扫描，并不停变化赋值
*******************************************************************************/

#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换
sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分别对应相应的数码管点亮,即位码
unsigned char TempData[8]; //存储显示值的全局变量
void Delay(unsigned int t); //函数声明
void Display(unsigned char FirstBit,unsigned char Num);
/*------------------------------------------------
                    主函数
------------------------------------------------*/
main()
{

 unsigned int i,k,j;
 unsigned char s;
 while(1)
      {
	   j++;
	   if(j==20)  
	     {
		  j=0;
		 if(0==k)
	     {
	     for(s=0;s<8;s++) //完全循环完成后清零所有缓冲区
			TempData[s]=0;
		  }
          TempData[8-i]=dofly_DuanMa[k]; //把需要显示的字符依次送缓冲区
          TempData[8+1-i]=0;  //不需要显示的区域清零
		  if(i==(8-k))
		    {
		    i=0;
            k++;
			if(k==8)
			   k=0;
			}
		  i++;
		 }
	   Display(0,8);//从第一位显示，共显示8位
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
/*------------------------------------------------
 显示函数，用于动态扫描数码管
 输入参数 FirstBit 表示需要显示的第一位，如赋值2表示从第三个数码管开始显示
 如输入0表示从第一个显示。
 Num表示需要显示的位数，如需要显示99两位数值则该值输入2
------------------------------------------------*/
void Display(unsigned char FirstBit,unsigned char Num)
{
      unsigned char i;
	  
	  for(i=0;i<Num;i++)
	   { 
	   DataPort=0;   //清空数据，防止有交替重影
       LATCH1=1;     //段锁存
       LATCH1=0;

       DataPort=dofly_WeiMa[i+FirstBit]; //取位码 
       LATCH2=1;     //位锁存
       LATCH2=0;

       DataPort=TempData[i]; //取显示数据，段码
       LATCH1=1;     //段锁存
       LATCH1=0;
       
	   Delay(200); // 扫描间隙延时，时间太长会闪烁，太短会造成重影

       }

}