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
* 简单描述 :此程序使用标准PS2键盘输入，LCD1602液晶屏输出显示。此样例仅作测试使用
晶振使用12M或者11.0592M,本键盘使用部分字母和数字测试，其他按键不能使用，用
户可以自行扩展。由于开发板和程序的各种参数，程序中没有使用奇偶校验，不保证没有
误码,校验程序请自行添加。
*******************************************************************************/
#include<reg52.h>        //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include "scancodes.h"
#include "ps2.h"
#include "1602.h"

sbit Key_Data = P3^3 ;   //定义Keyboard引脚
sbit Key_CLK  = P3^2;      //使用中断


bit BF=0;          
bit Shift;               //定义上档键标志
bit Key_UP;              //定义通码断码标志
unsigned char KeyV;
unsigned char IntNum;
unsigned char DisNum;

/*-----------------------------------------------
              外部中断读入信息
-----------------------------------------------*/
void Keyboard_out(void) interrupt 0
{
if ((IntNum > 0) && (IntNum < 9))
   { 
	KeyV = KeyV >> 1;                //因键盘数据是低>>高，结合上一句所以右移一位
	if (Key_Data)
    	KeyV = KeyV | 0x80;          //当键盘数据线为1时到最高位
	}
	IntNum++;
	while (!Key_CLK);                //等待PS/2CLK拉高

	if (IntNum > 10)
	   { 
 		IntNum = 0;                  //当中断11次后表示一帧数据收完，清变量准备下一次接收
 		BF = 1;                      //标识有字符输入完了
		EA = 0;                      //关中断等显示完后再开中断 
		}
}

/*-----------------------------------------------
                解码信息
注意:如SHIFT+G为12H 34H F0H 34H F0H 12H
也就是说shift的通码+G的通码+shift的断码+G的断码
-----------------------------------------------*/
void Decode(unsigned char ScanCode) //
{
unsigned char TempCyc,Val; 
if (!Key_UP)                        //当键盘按下时
	{
	switch (ScanCode)
			{
			case 0xF0 :             // 当收到0xF0，Key_UP置1表示断码开始
				Key_UP = 1;
				break;

			case 0x12 :             // 左 SHIFT
				Shift = 1;
				break;

			case 0x59 :             // 右 SHIFT
				Shift = 1;
				break;

			default: 
				if(!Shift)          //如果SHIFT没按下
					{
					for (TempCyc = 0;(UnShifted[TempCyc][0]!=ScanCode)&&(TempCyc<59); TempCyc++); //查表显示
					if (UnShifted[TempCyc][0] == ScanCode)
						{
   						Val= UnShifted[TempCyc][1];
						LCD_Write_Char(DisNum%16,DisNum/16,Val);
						DisNum++;
   						if(DisNum==33)
     					  {
	  					  LCD_Clear(); //清屏
	  					  DisNum=0;    //重头写数据
	 				      }
 					     }
                     } 
				else //按下SHIFT
                    {
					for(TempCyc = 0; (Shifted[TempCyc][0]!=ScanCode)&&(TempCyc<59); TempCyc++); //查表显示
					if (Shifted[TempCyc][0] == ScanCode)
                       {
  						Val= Shifted[TempCyc][1];
                        LCD_Write_Char(DisNum%16,DisNum/16,Val);
						DisNum++;
   						if(DisNum==33)
     					  {
	  					  LCD_Clear(); //清屏
	  					  DisNum=0;    //重头写数据
	 				      }
  						}
                      }
					break;
 			}
	}
else
	{ 
	Key_UP = 0;
	switch (ScanCode) //当键松开时不处理判码，如G 34H F0H 34H 那么第二个34H不会被处理
			{
			case 0x12 : // 左 SHIFT
				Shift = 0;
				break;

			case 0x59 : // 右 SHIFT
				Shift = 0;
				break;
			}
	}
BF = 0; //标识字符处理完了

} 

/*-----------------------------------------------
     ps2初始化（实际初始化外部中断）
-----------------------------------------------*/
void PS2_Init(void)
{
 IT1 = 0;                              //设外部中断1为低电平触发
 EA = 1;                               //外部中断开
 EX0 = 1;                              //开中断
}
/*-----------------------------------------------
                   读取键盘值
-----------------------------------------------*/
void Read_KeyBoard(void)
{

 if (BF)
   Decode(KeyV);
 else
   EA = 1; //开中断
}

