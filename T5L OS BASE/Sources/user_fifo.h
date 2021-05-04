#ifndef __USER_FIFO_H__
#define __USER_FIFO_H__

#include "sys.h"
#include "string.h"

#define MAX_QUEUE_LEN   2048

typedef struct {
    uint8_t is_init;
    uint16_t num_bytes_written;
    uint16_t w;
    uint16_t r;
    uint8_t dt[MAX_QUEUE_LEN];
}user_fifo_t;

//extern user_fifo_t user_fifo;

extern void user_fifo_init();
extern uint8_t user_fifo_get(uint8_t loc);
extern uint8_t user_fifo_pop();
extern void user_fifo_push(uint8_t d);
extern void user_fifo_reset();
extern uint16_t user_fifo_get_r_point();
extern uint16_t user_fifo_get_w_point();
extern uint16_t user_fifo_get_number_bytes_written();

#endif
