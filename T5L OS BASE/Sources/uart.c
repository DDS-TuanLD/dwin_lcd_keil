
#include "uart.h"
#include "user_fifo.h"
#include "sys.h"
uint8_t uart2_busy = 0;
uint16_t uart2_rx_count = 0;
uint8_t xdata Uart2_Rx[UART2_MAX_LEN];

uint8_t uart3_busy = 0;
uint16_t uart3_rx_count = 0;
uint8_t xdata Uart3_Rx[UART3_MAX_LEN];

uint16_t uart4_rx_count=0;
uint8_t xdata Uart4_Rx[UART4_MAX_LEN];

uint16_t uart5_rx_count=0;
uint8_t xdata Uart5_Rx[UART5_MAX_LEN];

void Uart2Init(void)
{
  P0MDOUT    |= 0x10;
  MUX_SEL    |= 0x40;
  uart2_busy = 0;
  uart2_rx_count = 0;
  ADCON      = 0x80;        /*0x80=ʹ��SREL0H:L*/
  SCON0      = 0x50;        /*��ʽ1:10λUART*/
  PCON      &= 0x7F;        /*.7=SMOD,�����ʱ�Ƶѡ��,0=����Ƶ*/
  SREL0H     = 0x03;        /*1024-FOSC/(64*������)*/
  SREL0L     = 0xE4;        /*1024-206438400/(64*115200)*/
	REN0       = 1;
  ES0        = 1;
}

void Uart2SendByte(uint8_t dat)
{
  while(uart2_busy==1);
  uart2_busy=1;
  SBUF0 = dat;
}

void Uart2SendStr(uint8_t *pstr,uint8_t strlen)
{
  if((NULL == pstr)||(0 == strlen))
  {
    return;
  }
  while(strlen--)
  {
    Uart2SendByte(*pstr);
    pstr++;
  }
}

void UART2_ISR_PC(void)    interrupt 4
{
  uint8_t res = 0;
	EA=0;
//		 if  (RI0) { SBUF0=SBUF0;while (!TI0){;}TI0=0;RI0=0; } //�Ի���ʵ��,
//		 if  (TI0) {TI0=0;} 
	if(RI0==1)
	{
			res=SBUF0;
			// Uart2_Rx[uart2_rx_count]=res; 
			// uart2_rx_count++;
			if (user_fifo_get_number_bytes_written() == MAX_QUEUE_LEN) {
				user_fifo_pop();
			}
      user_fifo_push(res);
			
			RI0=0;       
	}
	if(TI0==1)
	{
			TI0=0;
			uart2_busy=0;
	}
//  WDT_RST();
	EA=1;
}

/*****************************************************************************
����3����*/
void Uart3Init(void)
{
  P0MDOUT    |= 0xC0;
  MUX_SEL    |= 0x20;
  uart3_busy = 0;
  uart3_rx_count = 0;
  SCON1      = 0xD0;
  SREL1H     = 0x03;        /*1024-FOSC/(32*������)*/
  SREL1L     = 0xC8;
  IEN2       = 0x01;
}
/*****************************************************************************
����3�����ֽ�*/
void Uart3SendByte(uint8_t dat)
{
  while(uart3_busy==1);
  uart3_busy=1;
  SBUF1 = dat;
}
//ֱ��д�룬����Ҫ�жϲ�ѯ
//{
//  SBUF1 = dat;
//  while((SCON1&0x02)==0);
//  SCON1 &=0xFD;
//  SCON1 &=0xFD;
//}
/*****************************************************************************
����3�����ַ���*/
void Uart3SendStr(uint8_t *pstr,uint8_t strlen)
{
  if((NULL == pstr)||(0 == strlen))
  {
    return;
  }
  while(strlen--)
  {
    Uart3SendByte(*pstr);
    pstr++;
  }  
}
/*****************************************************************************
����3�����ж�*/
void UART3_ISR_PC(void)    interrupt 16
{
  uint8_t res=0;
  EA=0;
  if(SCON1&0x01)
  {
    res=SBUF1;
    Uart3_Rx[uart3_rx_count]=res; 
    uart3_rx_count++;
    SCON1 &= 0xFE;
    SCON1 &= 0xFE;
  }
  if(SCON1&0x02)
  {
    SCON1&=0xFD;
    SCON1&=0xFD;
    uart3_busy=0;
  }    
  WDT_RST();
  EA=1;
}

///*****************************************************************************
//����4����*/
void Uart4Init(void)
{
  uart4_rx_count=0;
  SCON2T     = 0x80;        /*����UART4����*/
  SCON2R     = 0x80;        /*����UART4����*/
  BODE2_DIV_H= 0x00;        /*FCLK/(8*DIV)*/
  BODE2_DIV_L= 0xe0;
  //ES2T=1;
  ES2R=1;
}
/*****************************************************************************
����4�����ֽ�*/
void Uart4SendByte(uint8_t dat)
{
  SBUF2_TX = dat;    
  while((SCON2T&0x01) == 0); 
  SCON2T &= 0xFE;  
}

void Uart4SendStr(uint8_t *pstr,uint8_t strlen)
{
  if((NULL == pstr)||(0 == strlen))
  {
    return;
  }
//	P0_0 = 1;
  while(strlen--)
  {
    Uart4SendByte(*pstr);
    pstr++;
  }
//	P0_0 = 0;
}
/*****************************************************************************
����4�����ж�*/
void UART4_TX_ISR_PC(void)    interrupt 10
{ 
}
/*****************************************************************************
����4�����ж�*/
void UART4_RX_ISR_PC(void)    interrupt 11
{
  uint8_t res=0;
  EA=0;
  if((SCON2R&0x01)==0x01)
  {
    res=SBUF2_RX;
    Uart4_Rx[uart4_rx_count]=res; 
    uart4_rx_count++;
    SCON2R&=0xFE;
    if (uart4_rx_count >= UART4_MAX_LEN) {
        //��ֹ���
        uart4_rx_count = 0;
		}
  }
  WDT_RST();
  EA=1;
}


///*****************************************************************************
//����5����*/
void Uart5Init(void)
{
  uart5_rx_count=0;
  SCON3T     = 0x80;        /*����UART4����*/
  SCON3R     = 0x80;        /*����UART4����*/
  BODE3_DIV_H= 0x00;        /*FCLK/(8*DIV)*/
  BODE3_DIV_L= 0xE0;
  //ES2T=1;
  ES3R=1;
}
/*****************************************************************************
����5�����ֽ�*/
void Uart5SendByte(uint8_t dat)
{
  SBUF3_TX = dat;    
  while((SCON3T&0x01) == 0); 
  SCON3T &= 0xFE;
}
/*****************************************************************************
����5�����ַ���*/
void Uart5SendStr(uint8_t *pstr,uint8_t strlen)
{
  if((NULL == pstr)||(0 == strlen))
  {
    return;
  }
  while(strlen--)
  {
    Uart5SendByte(*pstr);
    pstr++;
  }  
}
/*****************************************************************************
����5�����ж�*/
void UART5_TX_ISR_PC(void)    interrupt 12
{ 
}
/*****************************************************************************
����5�����ж�*/
void UART5_RX_ISR_PC(void)    interrupt 13
{
  uint8_t res=0;
  EA=0;
  if((SCON3R&0x01)==0x01)
  {
    res=SBUF3_RX;
    Uart5_Rx[uart5_rx_count]=res; 
    uart5_rx_count++;
    SCON3R&=0xFE;
    if (uart5_rx_count >= UART5_MAX_LEN) {
        //��ֹ���
        uart5_rx_count = 0;
		}
  }
  WDT_RST();
  EA=1;
}

/*****************************************************************************
����������*/
void InitUart(void)
{
  Uart2Init();
//  Uart3Init();
//  Uart4Init();
//  Uart5Init();
}

