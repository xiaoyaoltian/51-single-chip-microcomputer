/*******************************************************************************
* 版  权 	: 红梅科技工作室
* 论  坛	：https://blog.csdn.net/Qingzhusshuiyun
* Q Q	    ：2369099714
* 版权所有，盗版必究。
* 工  程   : DA输出锯齿波
* 文件名   : DA输出锯齿波.c
* 处理器   : STC89C52RC
* 编译环境 : Keil 5
* 系统时钟 : 12MHZ
* 版    本	: V1.0 
* 生成日期	: 2018-07-21	   					
* 修改日期	:  
* 简单描述 : 使用DA输入，数码管显示输出数字量，LED显示模拟电压大小
*******************************************************************************/
 
#include <reg52.h>                
#include "i2c.h"


#define AddWr 0x90   //写数据地址 
#define AddRd 0x91   //读数据地址

/*unsigned char code tab[]={
                          0,25,50,75,100,125,150,175,200,225,250  //表格数值越多，波形越平滑
                          };*/
unsigned char code tab1[]={
                          0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,
						  170,180,190,200,210,220,230,240,250
                          };

extern bit ack;
bit WriteDAC(unsigned char dat,unsigned char num);
/*------------------------------------------------
              主程序
------------------------------------------------*/
main()
{
 unsigned char i;
 while (1)         //主循环
  {
  for(i=0;i<26;i++)
     WriteDAC(tab1[i],1);
  }
}

/*------------------------------------------------
               写入DA转换数值
输入参数：dat 表示需要转换的DA数值，范围是0-255
------------------------------------------------*/
bit WriteDAC(unsigned char dat,unsigned char num)
{  unsigned char i;
   Start_I2c();               //启动总线
   SendByte(AddWr);             //发送器件地址
     if(ack==0)return(0);
   SendByte(0x40);            //发送器件子地址
     if(ack==0)return(0);
  for(i=0;i<num;i++)
     {
   SendByte(dat);             //发送数据
     if(ack==0)return(0);
     }
   Stop_I2c();  
}
