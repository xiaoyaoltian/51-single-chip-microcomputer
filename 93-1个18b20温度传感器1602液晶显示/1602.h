
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<intrins.h>

#ifndef __1602_H__
#define __1602_H__


 bit LCD_Check_Busy(void) ;

 void LCD_Write_Com(unsigned char com) ;

 void LCD_Write_Data(unsigned char Data) ;

 void LCD_Clear(void) ;

 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) ;

 void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) ;

 void LCD_Init(void) ;

 void Lcd_User_Chr(void);
   
#endif
