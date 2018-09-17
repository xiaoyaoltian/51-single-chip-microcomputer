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
* 简单描述 : 使用4路AD中的4路检测外部模拟量输入
*******************************************************************************/
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "pcf8591.h"

#define AddWr 0x90   //写数据地址 
#define AddRd 0x91   //读数据地址

extern bit ack;


/*------------------------------------------------
                   AD处理函数
------------------------------------------------*/
unsigned char  ADCheck(void)
{
 unsigned char num=0,i;
 unsigned char Val=0;
 for(i=0;i<5;i++)
     WriteDAC(0);//写入固定值到DA输出，用于检测
 for(i=0;i<5;i++)//连续读5次，输入通道后多读几次，去最后一次值，以便读出稳定值
     num=ReadADC(0);
 if((num>100)&&(num<150)) // 2.5V
    Val++;

 for(i=0;i<5;i++)
   num=ReadADC(1);
if((num>100)&&(num<150)) // 2.5V
    Val++;  

 for(i=0;i<5;i++)
    num=ReadADC(2);
if((num>100)&&(num<150)) // 2.5V
    Val++;

 for(i=0;i<5;i++)
     num=ReadADC(3);
if((num>100)&&(num<150)) // 2.5V
    Val++;
return Val;
}
/*------------------------------------------------
             读AD转值程序
输入参数 Chl 表示需要转换的通道，范围从0-3
返回值范围0-255
------------------------------------------------*/
unsigned char ReadADC(unsigned char Chl)
 {
  unsigned char Val;
   Start_I2c();               //启动总线
   SendByte(AddWr);             //发送器件地址
     if(ack==0)return(0);
   SendByte(0x40|Chl);            //发送器件子地址
     if(ack==0)return(0);
   Start_I2c();
   SendByte(AddRd);
      if(ack==0)return(0);
   Val=RcvByte();
   NoAck_I2c();                 //发送非应位
   Stop_I2c();                  //结束总线
  return(Val);
 }
/*------------------------------------------------
               写入DA转换数值
输入参数：dat 表示需要转换的DA数值，范围是0-255
------------------------------------------------*/
bit WriteDAC(unsigned char dat)
{
   Start_I2c();               //启动总线
   SendByte(AddWr);             //发送器件地址
     if(ack==0)return(0);
   SendByte(0x40);            //发送器件子地址
     if(ack==0)return(0);
   SendByte(dat);             //发送数据
     if(ack==0)return(0);
   Stop_I2c();  
}


