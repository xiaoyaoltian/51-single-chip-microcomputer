/*******************************************************************************
* 版  权 	: 红梅科技工作室
* 论  坛	：https://blog.csdn.net/Qingzhusshuiyun
* Q Q	    ：2369099714
* 版权所有，盗版必究。
* 工  程   : ps2键盘1602显示
* 文件名   : ps2键盘1602显示.c
* 处理器   : STC89C52RC
* 编译环境 : Keil 5
* 系统时钟 : 12MHZ
* 版    本	: V1.0 
* 生成日期	: 2018-08-06	   					
* 修改日期	:  
* 简单描述 : 测试Ps2键盘读入信息  
*******************************************************************************/

#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include "delay.h"
#include "1602.h"
#include "ps2.h"

void main(void)
{


 LCD_Init();
 DelayMs(10);
 LCD_Clear();
 PS2_Init();
 LCD_Write_String(0,0,"www.doflye.net ");
 LCD_Write_String(0,1,"press the key! ");
 

 while(1)
 {
  Read_KeyBoard(); //读取键盘值，循环显示到液晶
 }
}
