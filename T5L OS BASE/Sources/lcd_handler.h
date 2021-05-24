#ifndef __LCD_HANDLER_H__
#define __LCD_HANDLER_H__
#include "sys.h"

#define LCD_RESET_MODULE_CMD				0x0000
#define LCD_REQUEST_TIME_CMD				0x0010
#define LCD_SEND_RELAY_CMD					0x1000
#define LCD_CALL_SCENE_CMD					0x2000
#define LCD_UPDATE_SENSOR_PARA_CMD	0x2010
#define LCD_UPDTE_TIME_CMD					0x2020

#define LCD_CALL_SCENE_ADR  		0x5000
#define LCD_BRIGHTNESS_CTL_ADR  0x5010
#define LCD_VOLUMN_ONOFF_ADR		0x5020

#define LCD_UPDATE_TEM_ADR		0x5100
#define LCD_DISPLAY_TEM_ADR_O 0x5110
#define LCD_DISPLAY_TEM_ADR_I 0x5120
#define LCD_DISPLAY_TEM_ADR_D 0x5130

#define LCD_UPDATE_HUM_ADR 			0x5200
#define LCD_DISPLAY_HUM_ADR_I 	0x5210
#define LCD_DISPLAY_HUM_ADR_D 	0x5220

#define LCD_UPDTE_TIME_ADR			0x5300
#define LCD_DISPLAY_TIME_H_ADR	0x5310
#define LCD_DISPLAY_TIME_M_ADR  0x5320

#define LCD_RESET_MODULE_ADR		0x6000

#define LCD_SET_PIC_DISPLAY 0x0084

#define LCD_TEM_SAVE_ADR		0x000000
#define LCD_HUM_SAVE_ADR		0x000002
#define LCD_TIM_SAVE_ADR    0x000004
#define LCD_BRG_SAVE_ADR    0x000006
#define LCD_VLM_SAVE_ADR		0x000008

#define UART_DATA_MAX_LEN   30
#define UART_DATA_MIN_LEN		8

#define UART_HEADER_DATA_0  0xaa
#define UART_HEADER_DATA_1  0xaa
#define UART_HMI_TO_MODULE	0xfa
#define UART_MODULE_TO_HMI  0xf5
#define UART_TYPE_FEEDBACK       		0x01
#define UART_TYPE_NOFEEDBACK				0x00

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
	uint8_t sent_count;
}uart_mess_t;

typedef struct{
	uint8_t dt_len;
	uint8_t dt[UART_DATA_MAX_LEN - 8];
}uart_parse_data;

typedef struct{
	uint8_t h;
	uint8_t m;
	uint8_t s;
}real_time_t;

extern uart_mess_t uart_last_mess_sent;
extern real_time_t lcd_real_time;
extern uint16_t timer_2_count;

extern void lcd_para_init();
extern void lcd_handler_uart_data_reiv_process();
extern void lcd_send_uart_data_process();
extern void lcd_update_home_para();
extern void lcd_request_time_update();
extern void real_time_run();

#endif