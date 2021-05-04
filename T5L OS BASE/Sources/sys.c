
#include "sys.h"
#include "uart.h"
#include "timer.h"



/*****************************************************************************/
void DelayUs(uint16_t n)
{
  uint16_t i,j;
  for(i=0;i<n;i++)
    for(j=0;j<15;j++);
}
/*****************************************************************************/

void DelayMs(uint16_t n)
{
  uint16_t i,j;
  for(i=0;i<n;i++)
    for(j=0;j<7400;j++);
}

/*****************************************************************************/
void InitCFG(void)
{
  CKCON      = 0x00;
  T2CON      = 0x70;
  DPC        = 0x00;
  PAGESEL    = 0x01;
  D_PAGESEL  = 0x02;
  MUX_SEL    = 0x60;
  PORTDRV    = 0x01;
  RAMMODE    = 0x00;
}

/******************************************************************************/
void InitInt(void)
{
  IEN0       = 0x00;                    
  IEN1       = 0x00;
  IEN2       = 0x00;
  IP0        = 0x00;                     
  IP1        = 0x00;
}
/******************************************************************************/
void InitGPIO(void)
{
  PORTDRV    = 0x01;
  P0MDOUT    = 0x03;                   
  P1MDOUT    = 0x03;                    
  P2MDOUT    = 0x00;
  P3MDOUT    = 0x00;
  P0         = 0x00;
  P1         = 0x00;
  P2         = 0x00;
  P3         = 0x00;
}
void EX0_ISR_PC(void)    interrupt 0
{
    EA=0;

    EA=1;
}
void EX1_ISR_PC(void)    interrupt 2
{
    EA=0;

    EA=1;
}

void InitCPU(void)
{
  EA=0;
//  InitCFG();     
  InitInt();
  InitGPIO();
  InitUart();
  InitTimer();
//  WDT_ON();
  EA=1;
}
