#include "lcd_handler.h"
#include "uart.h"
#include "dgus.h"
#include "user_fifo.h"
#include "tstring.h"
#include "dgus.h"
#include "norflash.h"
#include "uart_handler.h"

real_time_t lcd_real_time = {0};
uint16_t timer_2_count = 0;
uint8_t lcd_brg_old = 0;
uint8_t lcd_vlm_st_old = 1;

void lcd_send_uart_data_process(){
	if(is_uart_mess_response_waiting() == 1){
		return;
	}
	lcd_send_uart_cmd_process();
}

void lcd_handler_uart_data_reiv_process(){
	uint16_t i;
	if(is_uart_mess_response_waiting()){
		DelayMs(200);
	}
	i = get_valid_data_header_loc();
	if((is_uart_mess_response_waiting()) && (user_fifo_get_number_bytes_written_t(&user_fifo) < uart_last_mess_sent.len)){
		Uart2SendStr(uart_last_mess_sent.dt, uart_last_mess_sent.len);
			uart_last_mess_sent.sent_count = uart_last_mess_sent.sent_count + 1;
			if(uart_last_mess_sent.sent_count > 5){
				uart_last_mess_sent.flag = 0;
				uart_last_mess_sent.sent_count = 0;
			}
	}
	if(i == 0xffff){
		uint16_t num = user_fifo_get_number_bytes_written_t(&user_fifo);
		if(num < UART_DATA_MIN_LEN){
			return;
		}
		DelayMs(50);
		if(num == user_fifo_get_number_bytes_written_t(&user_fifo)){
			uint16_t j;
			uint8_t buff[UART_DATA_MAX_LEN];
			for(j = 0; j <num; j++){
				buff[j] = user_fifo_pop_t(&user_fifo);
			}
			uart_data_handler(buff, num);
		}
		
	}else{
		uint16_t j;
		uint8_t buff[UART_DATA_MAX_LEN];
		for(j = 0;j <i;j++){
			buff[j] = user_fifo_pop_t(&user_fifo);
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
	if(temp_display_o[0] == 0x01){
		
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

void lcd_request_time_update(){
	 lcd_send_update_time_mess(UART_TYPE_FEEDBACK);
}

void lcd_update_brightness(){
	uint8_t led_brg_now[2];
	uint8_t lcd_brg_set;
	uint8_t led_on[4]={0x64,0x04,0x17,0x70};
	
	ReadDGUS(LCD_BRIGHTNESS_CTL_ADR, led_brg_now, 2);
	lcd_brg_set = led_brg_now[1];
	if(lcd_brg_set == 0){
		return;
	}
	if(lcd_brg_set != lcd_brg_old){
		lcd_brg_old = lcd_brg_set;
		led_on[0] = lcd_brg_set;
		WriteDGUS(0x0082, led_on, 4);
		WriteNorFlash(LCD_BRG_SAVE_ADR, LCD_BRIGHTNESS_CTL_ADR, 2);
		
	}
}

void lcd_update_vlm_st(){
	uint8_t vlm_st[2];
	uint8_t vlm_st_set;
	uint8_t vlm_on[4]={0x5a,0x00,0x00,0x2C};
	ReadDGUS(LCD_VOLUMN_ONOFF_ADR, vlm_st, 2);
	vlm_st_set = vlm_st[1];
	if(vlm_st_set != lcd_vlm_st_old){
		lcd_vlm_st_old = vlm_st_set;
		if(vlm_st_set == 0){
			vlm_on[3] = 0x24;
		}
		if(vlm_st_set == 1){
			vlm_on[3] = 0x2C;
		}
		WriteDGUS(0x0080, vlm_on, 4);
		WriteNorFlash(LCD_VLM_SAVE_ADR, LCD_VOLUMN_ONOFF_ADR, 2);
	}
}

void real_time_run(){
	uint8_t time_h[1];
	uint8_t time_m[1];
	time_h[0] = lcd_real_time.h;
	time_m[0] = lcd_real_time.m;
	WriteDGUS(LCD_DISPLAY_TIME_H_ADR, time_h, 1);
	WriteDGUS(LCD_DISPLAY_TIME_M_ADR, time_m, 1);
}

void lcd_update_home_para(){
	lcd_update_sensor_para();
	lcd_update_brightness();
	lcd_update_vlm_st();
}

void lcd_para_init(){

	ReadNorFlash(LCD_BRG_SAVE_ADR, LCD_BRIGHTNESS_CTL_ADR, 2);
	ReadNorFlash(LCD_VLM_SAVE_ADR, LCD_VOLUMN_ONOFF_ADR, 2);
	return ;
}

