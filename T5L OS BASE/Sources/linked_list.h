#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__
#include "sys.h"

#define BTN_ADDR_BEGIN      1010
#define SCENE_MAX_NUMBER    8
#define BTN_SCENE_NUMBER    SCENE_MAX_NUMBER

extern const uint16_t btn_adr[8] = {1010, 1012, 1014, 1016, 1018, 1020, 1022, 1024};
extern uint8_t page_diplay[20] = {0};

extern const uint8_t page_1[4] = {10, 11, 12, 13};
extern uint8_t page_1_t = 0;

extern const uint8_t page_2[4] = {14, 15, 16, 17};
extern uint8_t page_1_2 = 0;

typedef struct btn
{
   uint8_t isActive;
   uint8_t isOnpage;
   uint16_t addr;
   uint8_t content[20];
   uint8_t data;
   uint8_t needUpdate;
}btn_t;

typedef struct btn_list
{
    btn_t btn[8];
    uint8_t number_btn_written;

}btn_list_t;

extern btn_list_t my_list;
#endif