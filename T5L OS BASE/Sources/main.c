
#include "T5LOS8051.H"
#include "sys.h"
#include "uart.h"
#include "dgus.h"
#include <stdio.h>
#include "user_fifo.h"
#include "lcd_handler.h"
#include <stdlib.h>
// setting some input parameter
void lcd_sys_begin_set(){
//	uint8_t led_on[2]={0x00, 0x08};
//	uint8_t vlt_on[2]={0x00, 0x00};
//	uint8_t t[4]={0x5a,0x00,0x00,0x2C};
//	uint8_t v[4] = {0x20,0x01,0x03,0xe8};
//	
//	WriteDGUS(0x5010, led_on, 2);
//	WriteDGUS(0x5020, vlt_on, 2);
//	WriteDGUS(0x0080, t, 4);
//	WriteDGUS(0x0082, v, 4);
	uint8_t led_begin[2] = {0x00, 0x64};
	uint8_t vlt_begin[2] = {0x00, 0x00};
	uint8_t sys_begin_set[4] = {0x5a,0x00,0x00,0x2C};
	uint8_t les_standby_begin_set[4] = {0x20,0x01,0x0b,0xb8};
	
	WriteDGUS(LCD_BRIGHTNESS_CTL_ADR, led_begin, 2);
	WriteDGUS(LCD_VOLUMN_ONOFF_ADR, vlt_begin, 2);
	WriteDGUS(0x0080, sys_begin_set, 4);
	WriteDGUS(0x0082, les_standby_begin_set, 4);
	
}

void main(void)
{
  InitCPU();
	user_fifo_init_t(&user_fifo);
	user_fifo_init_t(&uart_4_fifo);
	lcd_request_time_update();
	lcd_sys_begin_set();
	lcd_para_init();
  while(1)
  {
		lcd_handler_uart_data_reiv_process();
		lcd_send_uart_data_process();
		lcd_update_home_para();
		real_time_run();
  }
}
