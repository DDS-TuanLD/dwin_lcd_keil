#include "uart_handler.h"
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
uint8_t reset_module[2] = {0};
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

void lcd_scene_call_send_uart(uint8_t btn, uint8_t ack){
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

	scall_uart_send_dt[6] = 0x01;
	count = count + 1;

	scall_uart_send_dt[7] = btn;
	count = count + 1;

	scall_uart_send_dt[8] = create_crc_check(scall_uart_send_dt);
	count = count + 1;
	
	t_strcpy(uart_last_mess_sent.dt, scall_uart_send_dt, count);
	uart_last_mess_sent.len = count;
	uart_last_mess_sent.flag = ack;
	
	Uart2SendStr(uart_last_mess_sent.dt, uart_last_mess_sent.len);
}

void lcd_scene_call_handler(uint8_t *dt, uint8_t ack){
	
	lcd_scene_call_send_uart(dt[1], ack);
}

void lcd_send_scene_call_uart_process(){
	ReadDGUS(LCD_CALL_SCENE_ADR, scene_call, 2);
	if(scene_call[1] == 0){
		
	}else{
		lcd_scene_call_handler(scene_call, UART_TYPE_CMD);
		WriteDGUS(LCD_CALL_SCENE_ADR, reset_data, 2);
	}
	
	ReadDGUS(LCD_RESET_MODULE_ADR, reset_module, 2);
	if((reset_module[0] == 0) && (reset_module[1] == 0)){
	
	}else{
		lcd_send_reset_cmd_to_module();
		reset_module[0] = reset_module[1] = 0;
		WriteDGUS(LCD_RESET_MODULE_ADR, reset_module, 2);
	}
	return;
}

uint16_t get_valid_data_header_loc(){
	uint16_t count = 0;
	uint16_t i = 0;
	uint16_t i1;
	uint16_t i2;
	uint16_t i3;
	uint16_t r_point = user_fifo_get_r_point();
	while (count <= user_fifo_get_number_bytes_written())
	{
		i1 = (i + r_point);
		i2 = (i + r_point + 1);
		i3 = (i + r_point + 2);
		if(i1 > MAX_QUEUE_LEN -1){
			i1 = i1 - MAX_QUEUE_LEN;
		}
		if(i2 > MAX_QUEUE_LEN -1){
			i2 = i2 - MAX_QUEUE_LEN;
		}if(i1 > MAX_QUEUE_LEN -1){
			i3 = i3 - MAX_QUEUE_LEN;
		}
		if((user_fifo_get(i1) == UART_HEADER_DATA_0) && (user_fifo_get(i2) == UART_HEADER_DATA_0) && (user_fifo_get(i3) == UART_HMI_TO_MODULE) && (i1 != r_point)){
			return i;
		}
		i = i + 1;
		count = count + 1;
		if(i == MAX_QUEUE_LEN){
			i = 0;
		}
	}
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

void uart_data_handler(uint8_t *dt, uint16_t len){
	uint16_t act_reiv = dt[4] << 8 | dt[5];
	uart_parse_data p_data = {0};
	if(is_uart_mess_response_waiting()){
		uint16_t temp = uart_last_mess_sent.dt[4] << 8 | uart_last_mess_sent.dt[5];
		if(!is_reiv_data_valid(dt, len)){
			Uart2SendStr(uart_last_mess_sent.dt, uart_last_mess_sent.len);
			uart_last_mess_sent.sent_count = uart_last_mess_sent.sent_count + 1;
			if(uart_last_mess_sent.sent_count > 5){
				uart_last_mess_sent.flag = 0;
				uart_last_mess_sent.sent_count = 0;
			}
			return;
		}
		if((temp == act_reiv) && (dt[3] == UART_TYPE_RES) && (len == uart_last_mess_sent.len)){
			uart_last_mess_sent.flag = 0;
			uart_last_mess_sent.sent_count = 0;
		}else{
			Uart2SendStr(uart_last_mess_sent.dt, uart_last_mess_sent.len);
			uart_last_mess_sent.sent_count = uart_last_mess_sent.sent_count + 1;
			if(uart_last_mess_sent.sent_count > 5){
				uart_last_mess_sent.flag = 0;
				uart_last_mess_sent.sent_count = 0;
			}
		}
		return;
	}else{
		uint16_t humi;
		uint8_t time_data[2] = {0};
		uint8_t temp_data[2] = {0};
		uint8_t humi_data[2] = {0};
		if(!is_reiv_data_valid(dt, len)){
			return;
		}
		if(dt[3] == UART_TYPE_CMD){
			dt[2] = UART_HMI_TO_MODULE;
			dt[3] = UART_TYPE_RES;
			dt[len - 1] = create_crc_check(dt);
			Uart2SendStr(dt, len);
		}
	
		parse_uart_data(&p_data, dt);
		switch (act_reiv){
			case LCD_UPDATE_SENSOR_PARA_CMD :
				humi = p_data.dt[2] << 8 | p_data.dt[3];
				temp_data[0] = p_data.dt[0];
				temp_data[1] = p_data.dt[1];
				humi_data[0] = humi/10;
				humi_data[1] = humi%10;
				WriteDGUS(LCD_UPDATE_TEM_ADR, temp_data, 2);
				WriteNorFlash(LCD_TEM_SAVE_ADR, LCD_UPDATE_TEM_ADR, 2);
				WriteDGUS(LCD_UPDATE_HUM_ADR, humi_data, 2);
				WriteNorFlash(LCD_HUM_SAVE_ADR, LCD_UPDATE_HUM_ADR, 2);				
				break;
			case LCD_UPDTE_TIME_CMD : 
				time_data[0] = p_data.dt[0];
				time_data[1] = p_data.dt[1];
				WriteDGUS(LCD_UPDTE_TIME_ADR, time_data, 2);
				WriteNorFlash(LCD_TIM_SAVE_ADR, LCD_UPDTE_TIME_ADR, 2);
				break;
			default:
				break;
		}
	}
	return;
}

void lcd_send_reset_cmd_to_module(){
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

	scall_uart_send_dt[4] = LCD_RESET_MODULE_CMD >> 8;
	count = count + 1;

	scall_uart_send_dt[5] = LCD_RESET_MODULE_CMD & 0x00ff;
	count = count + 1;

	scall_uart_send_dt[6] = 0x00;
	count = count + 1;

	scall_uart_send_dt[8] = create_crc_check(scall_uart_send_dt);
	count = count + 1;
	
	t_strcpy(uart_last_mess_sent.dt, scall_uart_send_dt, count);
	uart_last_mess_sent.len = count;
	uart_last_mess_sent.flag = 0x01;
	
	Uart2SendStr(uart_last_mess_sent.dt, uart_last_mess_sent.len);
}