﻿/*******************************************************************************
* 版  权 	: 红梅科技工作室
* 论  坛	：https://blog.csdn.net/Qingzhusshuiyun
* Q Q	    ：2369099714
* 版权所有，盗版必究。
* 工  程   : 按键长按短按效果
* 文件名   : 按键长按短按效果.c
* 处理器   : STC89C52RC
* 编译环境 : Keil 5
* 系统时钟 : 12MHZ
* 版    本	: V1.0 
* 生成日期	: 2018-06-27	   					
* 修改日期	:  
* 简单描述 : 按键加减数字，多个数码管显示，使用定时器做数码管动态扫描 并区别长按短按效果，完全可以应用的实际生产中
*******************************************************************************/

#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

sbit KEY_ADD=P3^3;  //定义按键输入端口
sbit KEY_DEC=P3^4;

#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换
sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分别对应相应的数码管点亮,即位码
unsigned char TempData[8]; //存储显示值的全局变量

void DelayUs2x(unsigned char t);//函数声明 
void DelayMs(unsigned char t);
void Display(unsigned char FirstBit,unsigned char Num);
void Init_Timer0(void);
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
unsigned char num=0,key_press_num;                
KEY_ADD=1; //按键输入端口电平置高
KEY_DEC=1;
Init_Timer0();

while (1)         //主循环
  {

  if(!KEY_ADD)  //如果检测到低电平，说明按键按下
    {
	 DelayMs(10); //延时去抖，一般10-20ms
     if(!KEY_ADD)     //再次确认按键是否按下，没有按下则退出
	   {
	   while(!KEY_ADD)
	    {
		key_press_num++;
        DelayMs(10);           //10x200=2000ms=2s 
		if(key_press_num==200) //大约2s
		  {
		   key_press_num=0;    //如果达到长按键标准 
                               //则进入长按键动作
           while(!KEY_ADD)     //这里用于识别是否按
							   //键还在按下，如果按
					     //下执行相关动作，否则退出
		       {
		       	if(num<99)    //加操作
	              num++;
				//即时把显示数据处理，如果去掉下面2
                //句处理信息，实际上看不到渐变效果，
                //而是看到跳变效果
				//用户可以自行屏蔽测试
     //分解显示信息，如要显示68，则68/10=6  68%10=8
				TempData[0]=dofly_DuanMa[num/10];  
	            TempData[1]=dofly_DuanMa[num%10];
                DelayMs(50);//用于调节长按循环操作
     //的速度，可以自行调整此值以便达到最佳效果
				}
		    }
		}
      key_press_num=0;//防止累加造成错误识别
		   if(num<99)    //加操作
	          num++;
	   }
	}

 if(!KEY_DEC)  //如果检测到低电平，说明按键按下
    {
	 DelayMs(10); //延时去抖，一般10-20ms
     if(!KEY_DEC) //再次确认按键是否按下，没有
                  //按下则退出
	   {
	   while(!KEY_DEC)
	    {
		key_press_num++;
        DelayMs(10);
		if(key_press_num==200) //大约2s
		  {
		   key_press_num=0;
           while(!KEY_DEC)
		       {
		       	if(num>0)  //减操作
	               num--;
//分解显示信息，如要显示68，则68/10=6  68%10=8
				TempData[0]=dofly_DuanMa[num/10];  
	            TempData[1]=dofly_DuanMa[num%10];
                DelayMs(50);
//用于调节长按循环操作的速度
				}
		    }
		}
      key_press_num=0;//防止累加造成错误识别
	       if(num>0)  //减操作
	          num--;
	 	
	   }
	}
//分解显示信息，如要显示68，则68/10=6  68%10=8  
	   TempData[0]=dofly_DuanMa[num/10];
	   TempData[1]=dofly_DuanMa[num%10];
	  // Display(0,8); //显示全部8位
     //主循环中添加其他需要一直工作的程序
	
  }
}
/*------------------------------------------------
 uS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编,大致延时
 长度如下 T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
 while(--t);
}
/*------------------------------------------------
 mS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //大致延时1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}
/*------------------------------------------------
 显示函数，用于动态扫描数码管
 输入参数 FirstBit 表示需要显示的第一位，如赋值2表示从第三个数码管开始显示
 如输入0表示从第一个显示。
 Num表示需要显示的位数，如需要显示99两位数值则该值输入2
------------------------------------------------*/
void Display(unsigned char FirstBit,unsigned char Num)
{
      static unsigned char i=0;
	  

	   DataPort=0;   //清空数据，防止有交替重影
       LATCH1=1;     //段锁存
       LATCH1=0;

       DataPort=dofly_WeiMa[i+FirstBit]; //取位码 
       LATCH2=1;     //位锁存
       LATCH2=0;

       DataPort=TempData[i]; //取显示数据，段码
       LATCH1=1;     //段锁存
       LATCH1=0;
       
	   i++;
       if(i==Num)
	      i=0;


}
/*------------------------------------------------
                    定时器初始化子程序
------------------------------------------------*/
void Init_Timer0(void)
{
 TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响		     
 //TH0=0x00;	      //给定初值
 //TL0=0x00;
 EA=1;            //总中断打开
 ET0=1;           //定时器中断打开
 TR0=1;           //定时器开关打开
}
/*------------------------------------------------
                 定时器中断子程序
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 TH0=(65536-2000)/256;		  //重新赋值 2ms
 TL0=(65536-2000)%256;
 
 Display(0,8);

}