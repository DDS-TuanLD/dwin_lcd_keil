/******************************************************************************
�� �� ��   : sys.h
�� �� ��   : V1.0
��    ��   : pinot
��������   : 2019��10��21��
��������   : ϵͳ�Ĵ����������
�޸���ʷ   :
��    ��   :
��    ��   :
�޸�����   :
******************************************************************************/

#ifndef _SYS_H_
#define _SYS_H_

/*****************************************************************************
ϵͳ��*/

/*****************************************************************************
�Զ���ͷ�ļ�*/
#include "T5LOS8051.H"

/*****************************************************************************
�궨�����*/
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
#define DWIN_NULL_POINT       (0x02)                /* ��ָ�� */
#define DWIN_PARAM_ERROR      (0x03)
#define NULL                  ((void *)0)           /* ����NULL */
#define FOSC                  (206438400UL)         /* T5L��Ƶ=����Ƶ��(11.0592M)*56/3 */
#define T1MS                  (65536-FOSC/12/1000)  /* 10MS��ʱ�� */

/*****************************************************************************
�궨�庯��*/
#define WDT_ON()  MUX_SEL|=0x02         /******�������Ź�*********/
#define WDT_OFF() MUX_SEL&=0xFD         /******�رտ��Ź�*********/
#define WDT_RST() MUX_SEL|=0x01         /******ι��*********/

/*****************************************************************************
���⺯������*/
void InitCPU(void);                    /* �Ĵ������ó�ʼ�� */
void DelayUs(uint16_t n);
void DelayMs(uint16_t n);

#endif
