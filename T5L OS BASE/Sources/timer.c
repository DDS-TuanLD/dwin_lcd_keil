
#include "timer.h"
#include "lcd_handler.h"
#include "uart.h"

uint16_t Wait_Count=0;
uint16_t Key_Count=0;
uint16_t Toset_Count=0;
uint16_t timer_4_count = 0;
void T0_Init(void)
{
  TMOD|=0x01;
  TH0=T1MS>>8;        
  TL0=T1MS;
  ET0=1;              
  TR0=1;              
}
void T0_ISR_PC(void)    interrupt 1
{
  EA=0;
  TH0=T1MS>>8;
  TL0=T1MS;
  Wait_Count++;
  if(Wait_Count>1000)
  {
    Wait_Count=0;
    Key_Count++;
    Toset_Count++;
  }
  WDT_RST();
  EA=1;
}

void T2_Init(void)
{
  T2CON=0x70;
  TH2=0x3c;
  TL2=0xaf;
  TRL2H=0xBC;
  TRL2L=0xCD;        
  IEN0|=0x20;        
  TR2=0x01;
}
void T2_ISR_PC(void)    interrupt 5
{
  TF2=0; 
	timer_4_count = timer_4_count + 1;
	if(timer_4_count == 1000){
		lcd_real_time.s = lcd_real_time.s + 1;
		if(lcd_real_time.s == 60){
			lcd_real_time.m = lcd_real_time.m + 1;
			lcd_real_time.s = 0;
		}
		if(lcd_real_time.m == 60){
			lcd_real_time.h = lcd_real_time.h + 1;
			lcd_real_time.m = 0l;
		}
		if(lcd_real_time.h == 24){
			lcd_real_time.h = 0;
		}
		timer_4_count = 0;
	}
	
//  WDT_RST();
}

void InitTimer(void)
{
//  T0_Init();
  T2_Init();
}

