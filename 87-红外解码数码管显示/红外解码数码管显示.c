﻿/*******************************************************************************
* 版  权 	: 红梅科技工作室
* 论  坛	：https://blog.csdn.net/Qingzhusshuiyun
* Q Q	    ：2369099714
* 版权所有，盗版必究。
* 工  程   : 红外解码数码管显示
* 文件名   : 红外解码数码管显示.c
* 处理器   : STC89C52RC
* 编译环境 : Keil 5
* 系统时钟 : 12MHZ
* 版    本	: V1.0 
* 生成日期	: 2018-07-27   					
* 修改日期	:  
* 简单描述 : 按配套遥控器上1-9会在数码管上对应显示
*******************************************************************************/
 
#include<reg52.h>    //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

sbit IR=P3^2;  //红外接口标志

#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换
sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存
/*------------------------------------------------
                全局变量声明
------------------------------------------------*/

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值0~9
unsigned char  irtime;//红外用全局变量

bit irpro_ok,irok;
unsigned char IRcord[4];
unsigned char irdata[33];

/*------------------------------------------------
                  函数声明
------------------------------------------------*/

void Ir_work(void);
void Ircordpro(void);

/*------------------------------------------------
                  定时器0中断处理
------------------------------------------------*/

void tim0_isr (void) interrupt 1 using 1
{
  irtime++;  //用于计数2个下降沿之间的时间
}

/*------------------------------------------------
                  外部中断0中断处理
------------------------------------------------*/
void EX0_ISR (void) interrupt 0 //外部中断0服务函数
{
  static unsigned char  i;             //接收红外信号处理
  static bit startflag;                //是否开始处理标志位

if(startflag)                         
   {
    if(irtime<63&&irtime>=33)//引导码 TC9012的头码，9ms+4.5ms
                        i=0;
    		irdata[i]=irtime;//存储每个电平的持续时间，用于以后判断是0还是1
    		irtime=0;
    		i++;
   			 if(i==33)
      			{
	  			 irok=1;
				 i=0;
	  			}
          }
  	 else
		{
		irtime=0;
		startflag=1;
		}

}

/*------------------------------------------------
                定时器0初始化
------------------------------------------------*/
void TIM0init(void)//定时器0初始化
{

  TMOD=0x02;//定时器0工作方式2，TH0是重装值，TL0是初值
  TH0=0x00; //重载值
  TL0=0x00; //初始化值
  ET0=1;    //开中断
  TR0=1;    
}
/*------------------------------------------------
                  外部中断0初始化
------------------------------------------------*/
void EX0init(void)
{
 IT0 = 1;   //指定外部中断0下降沿触发，INT0 (P3.2)
 EX0 = 1;   //使能外部中断
 EA = 1;    //开总中断
}
/*------------------------------------------------
                  键值处理
------------------------------------------------*/

void Ir_work(void)//红外键值散转程序
{
       switch(IRcord[2])//判断第三个数码值
	         {
			 case 0:DataPort=dofly_DuanMa[1];break;//1 显示相应的按键值
			 case 1:DataPort=dofly_DuanMa[2];break;//2
			 case 2:DataPort=dofly_DuanMa[3];break;//3
			 case 3:DataPort=dofly_DuanMa[4];break;//4
			 case 4:DataPort=dofly_DuanMa[5];break;//5
			 case 5:DataPort=dofly_DuanMa[6];break;//6
			 case 6:DataPort=dofly_DuanMa[7];break;//7
			 case 7:DataPort=dofly_DuanMa[8];break;//8
			 case 8:DataPort=dofly_DuanMa[9];break;//9
             default:break;
			 }

		  irpro_ok=0;//处理完成标志

  }
/*------------------------------------------------
                红外码值处理
------------------------------------------------*/
void Ircordpro(void)//红外码值处理函数
{ 
  unsigned char i, j, k;
  unsigned char cord,value;

  k=1;
  for(i=0;i<4;i++)      //处理4个字节
     {
      for(j=1;j<=8;j++) //处理1个字节8位
         {
          cord=irdata[k];
          if(cord>7)//大于某值为1，这个和晶振有绝对关系，这里使用12M计算，此值可以有一定误差
             value|=0x80;
          if(j<8)
		    {
			 value>>=1;
			}
           k++;
         }
     IRcord[i]=value;
     value=0;     
     } 
	 irpro_ok=1;//处理完毕标志位置1
}

/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main(void)
{
 EX0init(); //初始化外部中断
 TIM0init();//初始化定时器

  DataPort=0xfe; //取位码 第一位数码管选通，即二进制1111 1110
  LATCH2=1;      //位锁存
  LATCH2=0;

 while(1)//主循环
   {
    if(irok)                        //如果接收好了进行红外处理
	  {   
	   Ircordpro();
 	   irok=0;
	  }

    if(irpro_ok)                   //如果处理好后进行工作处理，如按对应的按键后显示对应的数字等
	  {
	   Ir_work();
  	  }
   }
}
  
  