
#include "timer.h"

uint16_t Wait_Count=0;
uint16_t Key_Count=0;
uint16_t Toset_Count=0;

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
  TH2=0x00;
  TL2=0x00;
  TRL2H=0xBC;
  TRL2L=0xCD;        
  IEN0|=0x20;        
  TR2=0x01;
}
void T2_ISR_PC(void)    interrupt 5
{
  TF2=0;    
//  WDT_RST();
}

void InitTimer(void)
{
//  T0_Init();
//  T2_Init();
}

