#ifndef __LCD_HANDLER_H__
#define __LCD_HANDLER_H__
#include "sys.h"

#define LCD_CALL_SCENE_CMD	0x2000
#define LCD_UPDATE_TEM_CMD	0x2010
#define LCD_UPDATE_HUM_CMD 	0x2020
#define LCD_UPDTE_TIME_CMD	0x2030

#define LCD_CALL_SCENE_ADR  0x5000
#define LCD_UPDATE_TEM_ADR	0x5100
#define LCD_UPDATE_HUM_ADR 	0x5200
#define LCD_UP_TIME_MI_ADR	0x5300
#define LCD_UP_TIME_SC_ADR  0x5400
#define LCD_UPDTE_TIME_ADR	0x5500
#define LCD_SET_PIC_DISPLAY 0x0084

#define LCD_TEM_SAVE_ADR		0x000000
#define LCD_HUM_SAVE_ADR		0x000002
#define LCD_TIM_SAVE_ADR    0x000004

#define UART_DATA_MAX_LEN   30
#define UART_DATA_MIN_LEN		9

#define UART_HEADER_DATA_0  0xaa
#define UART_HEADER_DATA_1  0xaa
#define UART_HMI_TO_MODULE	0xfa
#define UART_MODULE_TO_HMI  0xf5
#define UART_TYPE_CMD       0x01
#define UART_TYPE_RES				0x00

#define SCENE_CALL_TYPE_1		0x01
#define SCENE_CALL_TYPE_2		0x02
#define SCENE_CALL_TYPE_3		0x03
#define SCENE_CALL_TYPE_4		0x04
#define SCENE_CALL_TYPE_5		0x05
#define SCENE_CALL_TYPE_6		0x06

#define SCENE_CALL_MODE_1   0x01
#define SCENE_CALL_MODE_2   0x02
#define SCENE_CALL_MODE_3   0x03

typedef struct {
	uint8_t dt[UART_DATA_MAX_LEN];
	uint8_t len;
	uint8_t flag;
}uart_mess_t;

typedef struct{
	uint8_t dt_len;
	uint8_t dt[UART_DATA_MAX_LEN - 8];
}uart_parse_data;

extern void lcd_para_init();
extern void lcd_handler_uart_data_reiv_process();
extern void lcd_send_uart_data_process();
extern void lcd_update_home_para();

#endif