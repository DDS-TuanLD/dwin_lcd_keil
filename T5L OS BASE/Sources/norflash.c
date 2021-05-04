/******************************************************************************
* �� �� ��   : norflash.c
* �� �� ��   : V1.0
* ��    ��   : pinot
* ��������   : 2019��11��18��
* ��������   : ��дNorFlash
* �޸���ʷ   :
* ��    ��   :
* ��    ��   :
* �޸�����   :
******************************************************************************/

/*****************************************************************************
�Զ���ͷ�ļ�*/
#include "norflash.h"


/*****************************************************************************
��NorFlash*/
/*****************************************************************************
 �� �� ��  : ReadNorFlash
 ��������  : ��NorFlash
 �������  : uint32_t NorAddr    NorFlash�׵�ַ
             uint32_t DgusAddr  �洢���ݵ�DGUS��ַ
             uint16_t Len       ��ȡ�����ֳ���(������ż��)
 �������  : ��
 �޸���ʷ  :
 ��    ��  : 2019��11��04��
 ��    ��  :
 �޸�����  : ����
*****************************************************************************/
void ReadNorFlash(uint32_t NorAddr, uint32_t DgusAddr, uint16_t Len)
{
  uint8_t temp[8]={0};
	temp[0] = 0x5A;
	temp[1] = (uint8_t)(NorAddr >> 16);
	temp[2] = (uint8_t)(NorAddr >> 8);
	temp[3] = (uint8_t)(NorAddr);
	temp[4] = (uint8_t)(DgusAddr >> 8);
	temp[5] = (uint8_t)(DgusAddr);
	temp[6] = (uint8_t)(Len >> 8);
	temp[7] = (uint8_t)(Len);
  WriteDGUS(NOR_FLASH_RW_CMD,temp,sizeof(temp));
	do
	{
    DelayMs(5);
	  ReadDGUS(NOR_FLASH_RW_CMD,temp,1);
	}while(temp[0]!=0);
}


/*****************************************************************************
дNorFlash*/
/*****************************************************************************
 �� �� ��  : WriteNorFlash
 ��������  : дNorFlash
 �������  : uint32_t NorAddr    NorFlash�׵�ַ
             uint32_t DgusAddr  �洢���ݵ�DGUS��ַ
             uint16_t Len       д�������ֳ���(������ż��)
 �������  : ��
 �޸���ʷ  :
 ��    ��  : 2019��11��04��
 ��    ��  :
 �޸�����  : ����
*****************************************************************************/
void WriteNorFlash(uint32_t NorAddr, uint32_t DgusAddr, uint16_t Len)
{
  uint8_t temp[8]={0};
	temp[0] = 0xA5;
	temp[1] = (uint8_t)(NorAddr >> 16);
	temp[2] = (uint8_t)(NorAddr >> 8);
	temp[3] = (uint8_t)(NorAddr);
	temp[4] = (uint8_t)(DgusAddr >> 8);
	temp[5] = (uint8_t)(DgusAddr);
	temp[6] = (uint8_t)(Len >> 8);
	temp[7] = (uint8_t)(Len);
  WriteDGUS(NOR_FLASH_RW_CMD,temp,sizeof(temp));
	do
	{
    DelayMs(5);
	  ReadDGUS(NOR_FLASH_RW_CMD,temp,1);
	}while(temp[0]!=0);
}