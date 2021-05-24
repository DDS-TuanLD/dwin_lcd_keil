#ifndef __UART_HANDLER_H__
#define __UART_HANDLER_H__
#include "sys.h"

//check if have uart mess needed to feedback
extern uint8_t is_uart_mess_response_waiting();

//handle uart data
extern void uart_data_handler(uint8_t *dt, uint16_t len);

//send uart cmd through ui
extern void lcd_send_uart_cmd_process();

//get location of first valid data header of user_fifo
extern uint16_t get_valid_data_header_loc();
extern void lcd_send_reset_cmd_to_module(uint8_t ack);
extern void lcd_send_relay_control_cmd(uint8_t relay, uint8_t mod, uint8_t ack);
extern void lcd_send_update_time_mess(uint8_t ack);

#endif