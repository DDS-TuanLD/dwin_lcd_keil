#ifndef __UART_HANDLER_H__
#define __UART_HANDLER_H__
#include "sys.h"

extern uint8_t is_uart_mess_response_waiting();
extern void uart_data_handler(uint8_t *dt, uint16_t len);
extern void lcd_send_scene_call_uart_process();
extern uint16_t get_valid_data_header_loc();
extern void lcd_send_reset_cmd_to_module();

#endif