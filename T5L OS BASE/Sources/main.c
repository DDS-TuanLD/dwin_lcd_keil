
#include "T5LOS8051.H"
#include "sys.h"
#include <rtx51tny.h>
#include "uart.h"
#include "dgus.h"
#include <stdio.h>
#include "user_fifo.h"
#include "lcd_handler.h"

void main(void)
{
  InitCPU();
	user_fifo_init();
	lcd_para_init();
//  InitUartSend();
	
  while(1)
  {
		lcd_handler_uart_data_reiv_process();
		lcd_send_uart_data_process();
		lcd_update_home_para();
    //HandleProc();
  }
}
