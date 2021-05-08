#include "lcd_handler.h"
#include "uart.h"
#include "dgus.h"
#include "user_fifo.h"
#include "tstring.h"
#include "dgus.h"
#include "norflash.h"
#include "uart_handler.h"

void lcd_send_uart_data_process(){
	if(is_uart_mess_response_waiting() == 1){
		return;
	}
	lcd_send_scene_call_uart_process();
}

void lcd_handler_uart_data_reiv_process(){
	uint16_t i;
	if((!is_uart_mess_response_waiting()) && (user_fifo_get_number_bytes_written() < UART_DATA_MIN_LEN)){
		return;
	}
	if(is_uart_mess_response_waiting()){
		DelayMs(200);
	}
	i = get_valid_data_header_loc();
	if((is_uart_mess_response_waiting()) && (user_fifo_get_number_bytes_written() < uart_last_mess_sent.len)){
		Uart2SendStr(uart_last_mess_sent.dt, uart_last_mess_sent.len);
			uart_last_mess_sent.sent_count = uart_last_mess_sent.sent_count + 1;
			if(uart_last_mess_sent.sent_count > 5){
				uart_last_mess_sent.flag = 0;
				uart_last_mess_sent.sent_count = 0;
			}
	}
	if(i == 0xffff){
		uint16_t num = user_fifo_get_number_bytes_written();
		if(user_fifo_get_number_bytes_written() < UART_DATA_MIN_LEN){
			return;
		}
		DelayMs(50);
		if(num == user_fifo_get_number_bytes_written()){
			uint16_t j;
			uint8_t buff[UART_DATA_MAX_LEN];
			for(j = 0; j <num; j++){
				buff[j] = user_fifo_pop();
			}
			uart_data_handler(buff, num);
		}
		
	}else{
		uint16_t j;
		uint8_t buff[UART_DATA_MAX_LEN];
		for(j = 0;j <i;j++){
			buff[j] = user_fifo_pop();
		}
		uart_data_handler(buff, i);
	}
	return;
}

void lcd_update_temp(){
	uint8_t temp_data[2] = {0};
	uint16_t temp;
	uint8_t temp_display_o[1];
	uint8_t temp_display_i[1];
	uint8_t temp_display_d[1];
	
	ReadNorFlash(LCD_TEM_SAVE_ADR, LCD_UPDATE_TEM_ADR, 2);
	ReadDGUS(LCD_UPDATE_TEM_ADR, temp_data, 2);
	
	temp_display_o[0] = temp_data[0] >> 7;
	temp_display_i[0] = temp_data[0] & 0x7f;
	temp_display_d[0] = temp_data[1];
	temp = temp_display_i[0] << 8 | temp_display_d[0];
	temp_display_i[0] = temp/10;
	temp_display_d[0] = temp%10;
	if(temp_display_o[0] == 1){
		
	}
	WriteDGUS(LCD_DISPLAY_TEM_ADR_O, temp_display_o, 1);
	WriteDGUS(LCD_DISPLAY_TEM_ADR_I, temp_display_i, 1);
	WriteDGUS(LCD_DISPLAY_TEM_ADR_D, temp_display_d, 1);
	return;
}
void lcd_update_humi(){
	uint8_t humi_data[2] = {0};
	uint8_t humi_i[1];
	uint8_t humi_d[1];
	ReadNorFlash(LCD_HUM_SAVE_ADR, LCD_UPDATE_HUM_ADR, 2);
	ReadDGUS(LCD_UPDATE_HUM_ADR, humi_data, 2);
	humi_i[0] = humi_data[0];
	humi_d[0] = humi_data[1];
	WriteDGUS(LCD_DISPLAY_HUM_ADR_I, humi_i, 1);
	WriteDGUS(LCD_DISPLAY_HUM_ADR_D, humi_d, 1);
	return;
}

void lcd_update_sensor_para(){
	lcd_update_temp();
	lcd_update_humi();
	return;
}

void lcd_update_real_time(){
	uint8_t time_data[2] = {0};
	uint8_t time_h[1];
	uint8_t time_m[1];
	ReadNorFlash(LCD_TIM_SAVE_ADR, LCD_UPDTE_TIME_ADR, 2);
	ReadDGUS(LCD_UPDTE_TIME_ADR, time_data, 2);
	time_h[0] = time_data[0];
	time_m[0] = time_data[1];
	WriteDGUS(LCD_DISPLAY_TIME_H_ADR, time_h, 1);
	WriteDGUS(LCD_DISPLAY_TIME_M_ADR, time_m, 1);
	return;
}

void lcd_update_home_para(){
	lcd_update_sensor_para();
	lcd_update_real_time();
}

void lcd_para_init(){
	return ;
}
