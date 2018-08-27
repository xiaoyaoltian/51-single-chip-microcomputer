/*******************************************************************************
* 版  权 	: 红梅科技工作室
* 论  坛	：https://blog.csdn.net/Qingzhusshuiyun
* Q Q	    ：2369099714
* 版权所有，盗版必究。
* 工  程   : 24c02记忆开机次数
* 文件名   : 24c02记忆开机次数.c
* 处理器   : STC89C52RC
* 编译环境 : Keil 5
* 系统时钟 : 12MHZ
* 版    本	: V1.0 
* 生成日期	: 2018-07-10	   					
* 修改日期	:  
* 简单描述 : 每次开机，开机数值加1并存储到24c02
*******************************************************************************/

#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"


main()
{
 unsigned char num=0;
 Init_Timer0();
 IRcvStr(0xae,50,&num,1);   //从24c02读出数据                             
 num++;                 
 ISendStr(0xae,50,&num,1);  //写入24c02 
 DelayMs(10);
 TempData[0]=dofly_DuanMa[num/100];    
 TempData[1]=dofly_DuanMa[(num%100)/10];
 TempData[2]=dofly_DuanMa[(num%100)%10];

  while(1)
     {
	 }
}
