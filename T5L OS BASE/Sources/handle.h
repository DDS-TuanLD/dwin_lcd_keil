/******************************************************************************
�� �� ��   : handle.h
�� �� ��   : V1.0
��    ��   : pinot
��������   : 2019��11��27��
��������   : �߼�����
�޸���ʷ   :
��    ��   :
��    ��   :
�޸�����   :
******************************************************************************/

#ifndef _HANDLE_H_
#define _HANDLE_H_

/*****************************************************************************
ϵͳ��*/

/*****************************************************************************
�Զ���ͷ�ļ�*/
#include "T5LOS8051.H"
#include "sys.h"
#include "dgus.h"
#include "uart.h"
#include "norflash.h"
#include "timer.h"
#include "crc16.h"


/*****************************************************************************
�궨�����*/
//ϵͳ�����ӿ�
#define DHW_NPAGE               0x0014        //��ǰҳ��
#define DHW_XCOOR               0x0017        //X����
#define DHW_YCOOR               0x0018        //Y����
#define DHW_LEDNOW              0x0031        //LED����״̬
#define DHW_SYSCFG              0x0080        //ϵͳ����
#define DHW_LEDCFG              0x0082        //LED��������
#define DHW_SPAGE               0x0084        //����ҳ��
#define DHW_0F00                0x0F00        //�����ı�ָʾ

//��ʾ����
//--���ù���
#define DS_SCFG_WFLAG           0x1010        //��������-д���־λ
#define DS_SCFG_KGID            0x1011        //��������-����ID
#define DS_SCFG_WKID            0x1012        //��������-�¿�ID
#define DS_SCFG_XPTIME          0x1013        //��������-Ϣ��ʱ��
#define DS_SCFG_VOICE           0x1014        //��������-������
#define DS_SCFG_LANGUAGE        0x1015        //��������-����ѡ��
#define DS_SCFG_MBGN1           0x1016        //��������-��幦��1
#define DS_SCFG_MBGN2           0x1017        //��������-��幦��2
#define DS_MBGN2_TEMP           0x101A        //��幦��2��ʾ
#define DS_LOGIN                0x1020        //�����¼
//--�뻧���ҳ��
#define DS_WR                   0x1100        //��ͼ-����
#define DS_NAV                  0x1101        //��ͼ-������
#define DS_LED                  0x1102        //��ͼ-�ƹ����
#define DS_LED_N                0x1103        //�ȼ�
#define DS_MASTER               0x1104        //�ܿ���
//--������ҳ��
#define DSZ_LED                 0x1200        //��ͼ-�ƹ�
#define DSZ_LED_T               0x1201        //��ͼ-�ƹ�����
#define DSZ_LED_I               0x1202        //��ͼ-�ƹ�ͼ��
#define DSZ_LED_N               0x1203        //��ͼ-�ƹ�ȼ�
#define DSZ_CLJL                0x1210        //��ͼ-��������
#define DSZ_FS                  0x1220        //��ͼ-����
#define DSZ_FS_T                0x1221        //��ͼ-��������
#define DSZ_FS_N                0x1223        //��ͼ-��������
#define DSZ_WD                  0x1230        //��ͼ-�¶�
#define DSZ_WD_C                0x1233        //��ͼ-�¶ȷ���
#define DSZ_WD_D                0x1234        //��ͼ-�¶���ֵ
#define DSZ_WD_DP               0x12A0        //��ͼ-�¶���ֵָ��
#define DSZ_WD_DP1              0x12C0        //��ͼ-�¶���ֵָ��1

//���ر���
//--A
#define DT_A1                   0x2101        //�ϱ�����1
#define DT_A2                   0x2102        //�ϱ�����2
#define DT_A3                   0x2103        //�ϱ�����3
#define DT_A4                   0x2104        //�ϱ�����4
//--B
#define DT_B1                   0x2201        //�ϱ��ٷֱ�1
#define DT_B2                   0x2202        //�ϱ��ٷֱ�2
//--C
#define DTC_SAVE                0x2001        //��������
#define DTC_BACK                0x2002        //����������
#define DTC_MSG                 0x2003        //��ת����Ϣ
#define DTC_WTH                 0x2004        //��ת������
#define DTC_LNG                 0x2005        //��ת������
#define DTC_MBGN                0x2006        //��幦����ת
#define DTC_CLJL                0x2007        //�����л�
#define DTC_SSHS                0x2008        //�¶��л�
//--D
#define DTD_CNEN                0x2301        //�����л�
//--E
#define DTE_FS                  0x2401        //���ٿ���
#define DTE_FS_L                0x2402        //�����󰴼�
#define DTE_FS_R                0x2403        //�����Ұ���
#define DTE_WD_L                0x2404        //�����󰴼�
#define DTE_WD_R                0x2405        //�����Ұ���

//��������
#define DD_IDT_TON              0x3000        //��ʼ����-���ذ�������
#define DD_IDT_TOUT             0x3001        //��ʼ����-����̧������
#define BR_MIN                  0             //Ϣ������
#define BR_MAX                  0xFF          //��������
#define LANG_CN                 1             //����-����
#define LANG_EN                 2             //����-Ӣ��
#define COLORB                  0x001F        //��ɫ-��ɫ
#define COLORW                  0xFFFF        //��ɫ-��ɫ
#define COLORR                  0xF800        //��ɫ-��ɫ

//���ݿ��ַ
#define NAR_DATA                0x000000      //��дNorFlash���ݵ��׵�ַ
#define NAR_LENGTH              8             //��дNorFlash���ݵ��ֳ���,������ż��

//��������
#define VERS                   0x1F00         //�汾����ʾ
#define DEBUG_SET              0x1F10         //��������
#define XCOOR_P                0x1F20         //X������ʾָ��
#define YCOOR_P                0x1F40         //Y������ʾָ��
#define VERS_P                 0x1F60         //�汾����ʾָ��

#define UDEBUG                 0
#if UDEBUG
  #define LOG(x,y)    { Uart2SendStr(x,y); }
//#define LOG(x,y)    { Uart3SendStr(x,y); }
//#define LOG(x,y)    { Uart4SendStr(x,y); }
//#define LOG(x,y)    { Uart5SendStr(x,y); }
#else
  #define LOG(x,y)    { ; }
#endif

/*****************************************************************************
���⺯������*/
void InitUartSend(void);
void InitDgusData(void);
void GetConfig(void);
void SetIndex(void);
void HandleProc(void);
void JumpPage(uint16_t pageId);
#endif