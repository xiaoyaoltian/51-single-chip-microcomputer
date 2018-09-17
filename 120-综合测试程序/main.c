/*******************************************************************************
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
* 简单描述 : 
*******************************************************************************/

#include"keyboard.h"
#include"delay.h"
#include"18b20.h"
#include "ds1302.h"
#include "i2c.h"
#include"uart.h"
#include "pcf8591.h"
#include "LED8x8.h"
#include "timer.h"
#include "1602.h"
#include<stdio.h>
extern unsigned char TempData[8]; //存储显示值的全局变量
/*------------------------------------------------
                   定义端口
------------------------------------------------*/
sbit LATCH1=P2^7;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存
/*------------------------------------------------
                   S级延时
------------------------------------------------*/
void DelayS(unsigned char u)
{
 while(u--)
      {
	  DelayMs(250);
	  DelayMs(250);
	  DelayMs(250);
	  DelayMs(250);
	  }
}
/*------------------------------------------------
                   主函数
------------------------------------------------*/
main()
{
 unsigned char doflye=0x55;//用于24c02测试
// unsigned char i;
 unsigned char keynum;     //按键返回值
 int temp;                 //温度值



 Ds1302_Init();   //初始化时钟芯片
 InitUART ();     //初始化串口
 LCD_Init();      //初始化LCD1602
 DelayMs(30);     //稳定
 LCD_Write_String(0,0,"www.doflye.net");//液晶显示测试
 LCD_Write_String(0,1,"LCD1602 test ok!");
 EN_CLR;
 Init_Timer0();

  	   P0=0xff;   //全部点亮
       LATCH1=1;  //段锁存
       LATCH1=0;

       P0=0;      //8位全部选中 
       LATCH2=1;  //位锁存
       LATCH2=0;
 DelayS(1);

 while(1)
     {
          ISendStr(0xae,400,&doflye,1);//写入24c02
		  DelayMs(60);                 //延时为了让24c02自身写入值
		  doflye=0;                    //清零该值，为了验证存储有效性
		  IRcvStr(0xae,400,&doflye,1); //读出doflye的值，并进行如下比较
		  if(doflye==0x55)             //读出与写入值相等，表示操作正确，器件正常
		    printf("1、24c02 测试正常\n");
		  else
		    printf("1、24c02 错误XXXX，请检测杜邦线连接\n");
          DelayS(1);
		  if(2==ADCheck())             //2个电位器调节至中间位置，如果有2路检测到这个数值，表示ADC芯片工作正常，也可以自行添加4路
		    printf("2、ADC 测试正常\n");
		  else
		    printf("2、ADC 错误XXXX，请检测杜邦线连接\n");
          DelayS(1);
		  Ds1302_Write_Time();//写入时间
		  Ds1302_Read_Time(); //读出时间
		  if(time_buf1[1]==10)//判读写入与读出其中之一个数值是否相等，相等表示运行正确
            printf("3、DS1302 测试正常\n");
		  else
		    printf("3、DS1302 测试错误XXXX,请检测杜邦线连接\n");
          DelayS(1);
		  printf("4、检查8位数码管是否全亮\n");
		  DelayS(1);
		  printf("5、J9全部接GND，检查8位LED是否全亮\n");
		  printf("6、J6全部接GND，检查单个共阳数码管是否全亮\n");
		  DelayS(1);

		  printf("7、请插入18B20，请等待！\n");
          DelayS(1);
		  temp=ReadTemperature();//读取温度
          DelayS(1);
          temp=ReadTemperature();//第二次读取温度，以便取得正确值
		  printf("   现在温度 %7.3f 度\n",(float)temp*0.0625);
          DelayS(1);
	      Ds1302_Read_Time(); //读出时间
		  printf("   日期 %02d-%02d-%02d 星期%d\n",(int)time_buf1[1],(int)time_buf1[2],(int)time_buf1[3],(int)time_buf1[7]);//年月周
          printf("   时间 %02d:%02d:%02d\n",(int)time_buf1[4],(int)time_buf1[5],(int)time_buf1[6]);//时分秒
		  DelayS(1);
          printf("8、连接P0口到J24，请按任意矩阵按键！\n");
		  DelayS(1);
		  printf("9、请插入点阵，检查是否全部点亮！\n");
		  printf("   如果正常，请拔掉点阵，防止功耗过大\n");
          
	   while(1)
	        {
          
          keynum=KeyPro(); //读取矩阵键盘的值
          if(keynum!=0xff) //判断是0~15 有效值
		    {
		    printf("矩阵键盘键值是：%02d\n",(int)keynum);//2位数字显示有效值
			Ds1302_Read_Time(); //读出时间
            printf("时间 %02d:%02d:%02d\n",(int)time_buf1[4],(int)time_buf1[5],(int)time_buf1[6]);//时分秒
            DelayMs(50);
			}
			}
	 }
}
/*------------------------------------------------
                 定时器中断子程序
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 TH0=(65536-2000)/256;		  //重新赋值 2ms
 TL0=(65536-2000)%256;
 
 LED8x8Display();

}
