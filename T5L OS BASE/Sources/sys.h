/******************************************************************************
文 件 名   : sys.h
版 本 号   : V1.0
作    者   : pinot
生成日期   : 2019年10月21日
功能描述   : 系统寄存器配置相关
修改历史   :
日    期   :
作    者   :
修改内容   :
******************************************************************************/

#ifndef _SYS_H_
#define _SYS_H_

/*****************************************************************************
系统库*/

/*****************************************************************************
自定义头文件*/
#include "T5LOS8051.H"

/*****************************************************************************
宏定义变量*/
#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif
#ifndef uint32_t
typedef unsigned int uint32_t;
#endif
#ifndef int8_t
typedef char int8_t;
#endif
#ifndef int16_t
typedef short int16_t;
#endif
#ifndef int32_t
typedef int int32_t;
#endif

#define DWIN_OK               (0x00)
#define DWIN_ERROR            (0xFF)
#define DWIN_NULL_POINT       (0x02)                /* 空指针 */
#define DWIN_PARAM_ERROR      (0x03)
#define NULL                  ((void *)0)           /* 数据NULL */
#define FOSC                  (206438400UL)         /* T5L主频=晶体频率(11.0592M)*56/3 */
#define T1MS                  (65536-FOSC/12/1000)  /* 10MS定时器 */

/*****************************************************************************
宏定义函数*/
#define WDT_ON()  MUX_SEL|=0x02         /******开启看门狗*********/
#define WDT_OFF() MUX_SEL&=0xFD         /******关闭看门狗*********/
#define WDT_RST() MUX_SEL|=0x01         /******喂狗*********/

/*****************************************************************************
对外函数声明*/
void InitCPU(void);                    /* 寄存器配置初始化 */
void DelayUs(uint16_t n);
void DelayMs(uint16_t n);

#endif
