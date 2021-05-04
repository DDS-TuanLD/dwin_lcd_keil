#include "lcd_handler.h"
#include "uart.h"
#include "dgus.h"
#include "user_fifo.h"
#include "tstring.h"
#include "dgus.h"
#include "norflash.h"

uint8_t reset_data[2] = {0x00, 0x00};
uint8_t last_scene_call[2] = {0};
uint8_t scene_call[2] = {0};
uart_mess_t uart_last_mess_sent = {0};

uint8_t create_crc_check(uint8_t *d){
	int i = 0;
	uint16_t temp = 0;
	for(i=2; i<7; i++){
		temp = temp + d[i];
	}
	return temp & 0x00ff;
}

uint8_t is_uart_mess_response_waiting(){
	return uart_last_mess_sent.flag;
}

void lcd_scene_call_send_uart(uint8_t btn, uint8_t mod){
	uint8_t scall_uart_send_dt[UART_DATA_MAX_LEN] = {0};
	int count = 0;

	scall_uart_send_dt[0] = UART_HEADER_DATA_0;
	count = count + 1;

	scall_uart_send_dt[1] = UART_HEADER_DATA_1;
	count = count + 1;
	
	scall_uart_send_dt[2] = UART_HMI_TO_MODULE;
	count = count + 1;
	
	scall_uart_send_dt[3] = UART_TYPE_CMD;
	count = count + 1;

	scall_uart_send_dt[4] = LCD_CALL_SCENE_CMD >> 8;
	count = count + 1;

	scall_uart_send_dt[5] = LCD_CALL_SCENE_CMD & 0x00ff;
	count = count + 1;

	scall_uart_send_dt[6] = 0x02;
	count = count + 1;

	scall_uart_send_dt[7] = btn;
	count = count + 1;

	scall_uart_send_dt[8] = mod;
	count = count + 1;

	scall_uart_send_dt[9] = create_crc_check(scall_uart_send_dt);
	count = count + 1;
	
	t_strcpy(uart_last_mess_sent.dt, scall_uart_send_dt, count);
	uart_last_mess_sent.len = count;
	uart_last_mess_sent.flag = 1;
	
	Uart2SendStr(uart_last_mess_sent.dt, uart_last_mess_sent.len);
}

void lcd_scene_call_handler(uint8_t *dt){
	lcd_scene_call_send_uart(dt[0], dt[1]);
}

void lcd_send_scene_call_uart_process(){
	ReadDGUS(LCD_CALL_SCENE_ADR, scene_call, 2);
	
	if((scene_call[0] == 0) || (scene_call[1] == 0)){
		
	}else{
		lcd_scene_call_handler(scene_call);
		WriteDGUS(LCD_CALL_SCENE_ADR, reset_data, 2);
	}
	return;
}

void lcd_send_uart_data_process(){
	if(is_uart_mess_response_waiting() == 1){
		return;
	}
	lcd_send_scene_call_uart_process();
}

uint16_t get_valid_data_header_loc(){
	return 0xffff;
}

int is_reiv_data_valid(uint8_t *dt, uint16_t len){
	if((dt[0] == UART_HEADER_DATA_0) && (dt[1] == UART_HEADER_DATA_1) && (dt[2] == UART_MODULE_TO_HMI) && (dt[len-1] == create_crc_check(dt))){
		return 1;
	}
	return 0;
}

void parse_uart_data(uart_parse_data *p_data, uint8_t *dt){
	int i;
	int j = 0;
	p_data->dt_len = dt[6];
	for(i = 7; i< 7 + dt[6]; i++){
		p_data->dt[j] = dt[i];
		j = j + 1;
	} 
	return;
}

void valid_uart_data_handler(uint8_t *dt, uint16_t len){
	uint16_t act_reiv = dt[4] << 8 | dt[5];
	uart_parse_data p_data = {0};
	if(is_uart_mess_response_waiting()){
		uint16_t temp = uart_last_mess_sent.dt[4] << 8 | uart_last_mess_sent.dt[5];
		if(!is_reiv_data_valid(dt, len)){
			Uart2SendStr(uart_last_mess_sent.dt, uart_last_mess_sent.len);
			return;
		}
		if((temp == act_reiv) && (dt[3] == UART_TYPE_RES)){
				uart_last_mess_sent.flag = 0;
				WriteDGUS(LCD_CALL_SCENE_ADR, reset_data, 2);
		}else{
			//Uart2SendStr(uart_last_mess_sent.dt, uart_last_mess_sent.len);
		}
		return;
	}else{
		uint8_t temp_data[1] = {0};
		uint8_t humi_data[1] = {0};
		uint8_t time_data[2] = {0};

		if(!is_reiv_data_valid(dt, len)){
			return;
		}
		parse_uart_data(&p_data, dt);
		switch (act_reiv){
			case LCD_UPDATE_TEM_CMD :
				temp_data[0] = p_data.dt[0];
				WriteDGUS(LCD_UPDATE_TEM_ADR, temp_data, 1);
				WriteNorFlash(LCD_TEM_SAVE_ADR, LCD_UPDATE_TEM_ADR, 2);
				break;
			case LCD_UPDATE_HUM_CMD : 
				humi_data[0] = p_data.dt[0];
				WriteDGUS(LCD_UPDATE_TEM_ADR, temp_data, 1);
				WriteNorFlash(LCD_TEM_SAVE_ADR, LCD_UPDATE_HUM_ADR, 2);				
				break;
			case LCD_UPDTE_TIME_CMD : 	
				time_data[0] = p_data.dt[0];
				time_data[1] = p_data.dt[1];
				WriteDGUS(LCD_UP_TIME_MI_ADR, (uint8_t *)time_data[0], 1);
				WriteDGUS(LCD_UP_TIME_SC_ADR, (uint8_t *)time_data[1], 1);
				WriteDGUS(LCD_UPDTE_TIME_ADR, time_data, 2);
				WriteNorFlash(LCD_TIM_SAVE_ADR, LCD_UPDTE_TIME_ADR, 2);
				break;
			default:
				break;
		}
	}
	return;
}

void lcd_handler_uart_data_reiv_process(){
	uint16_t i;
	DelayMs(150);
	i = get_valid_data_header_loc();
	if(user_fifo_get_number_bytes_written() < UART_DATA_MIN_LEN){
		return;
	}
	if(i == 0xffff){
		uint16_t num = user_fifo_get_number_bytes_written();
		DelayMs(50);
		if(num == user_fifo_get_number_bytes_written()){
			uint16_t j;
			uint8_t buff[UART_DATA_MAX_LEN];
			for(j = 0; j <num; j++){
				buff[j] = user_fifo_pop();
			}
			valid_uart_data_handler(buff, num);
		}
		
	}else{
		uint16_t j;
		uint8_t buff[UART_DATA_MAX_LEN];
		for(j = 0;j <i;j++){
			buff[j] = user_fifo_pop();
		}
		valid_uart_data_handler(buff, i);
	}
	return;
}

void lcd_update_temp(){
	ReadNorFlash(LCD_TEM_SAVE_ADR, LCD_UPDATE_TEM_ADR, 2);
	return;
}

void lcd_update_humi(){
	ReadNorFlash(LCD_TEM_SAVE_ADR, LCD_UPDATE_HUM_ADR, 2);
	return;
}

void lcd_update_real_time(){
	uint8_t time_data[2] = {0};
	ReadNorFlash(LCD_TIM_SAVE_ADR, LCD_UPDTE_TIME_ADR, 2);
	ReadDGUS(LCD_UPDTE_TIME_ADR, time_data, 2);
	WriteDGUS(LCD_UP_TIME_MI_ADR, (uint8_t *)time_data[0], 1);
	WriteDGUS(LCD_UP_TIME_SC_ADR, (uint8_t *)time_data[1], 1);
	Uart2SendStr(time_data, 2);
	DelayMs(5000);
	return;
}

void lcd_update_home_para(){
	lcd_update_temp();
	lcd_update_humi();
	lcd_update_real_time();
}

void lcd_para_init(){
	return ;
}
