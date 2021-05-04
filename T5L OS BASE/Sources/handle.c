/******************************************************************************
* �� �� ��   : handle.c
* �� �� ��   : V1.0
* ��    ��   : pinot
* ��������   : 2019��11��27��
* ��������   : �߼�����
* �޸���ʷ   :
* ��    ��   :
* ��    ��   :
* �޸�����   :
******************************************************************************/

/*****************************************************************************
�Զ���ͷ�ļ�*/
#include "handle.h"


/*****************************************************************************
ȫ�ֱ���*/
//�ڲ�RAM
uint8_t value0F00[4] = {0};
uint8_t xp_status=0;

//�ⲿRAM
typedef struct dlt_645_8283
{
    uint8_t st_byte1;//֡��ʼ��
    uint8_t st_byte2;//֡��ʼ��
    uint8_t lenth;//����
    uint8_t func;//����
    uint8_t addr1;//��ַ
    uint8_t addr2;//��ַ	
    uint8_t edata[250];//��������
}dl_t_645_8283_body;
dl_t_645_8283_body xdata s_dl_t_645_8283_body={0};

#define FRAME_BUFFER_LEN 255
uint8_t xdata frame_buf[FRAME_BUFFER_LEN];//������ȡһ֡����
uint8_t xdata frame_buf1[FRAME_BUFFER_LEN]=0;//������ȡһ֡����
#define DLT_645_LEAST_LEN 7


//����ռ�
uint8_t code CUart2[] = "Uart2Init Done!!!123";
uint8_t code CUart3[] = "Uart3Init Done!!!";
uint8_t code CUart4[] = "Uart4Init Done!!!";
uint8_t code CUart5[] = "Uart5Init Done!!!";

//������һ֡���ݿ��ܳ��ֵ����
typedef enum frame_result
{
    UNKNOWN,
    OK,           //�ɹ��ҵ�һ֡
    UNFINISHED,   //δ�������
    ERROR,        //�������Э��
}frame_result_t;

//����Э������
typedef enum protocol_type
{
    PROTOCOL_UNKNOWN,
    PROTOCOL_DL_T_698,
    PROTOCOL_DL_T_645,
    PROTOCOL_OTHER,
}protocol_type_t;

/*****************************************************************************
��������*/
void SetBlight(uint8_t n)
{
//	uint8_t temp[4]={0};
  WriteDGUS(DHW_LEDCFG,(uint8_t *)&n,sizeof(n));
//	do
//	{
//    DelayMs(5);
//	  ReadDGUS(DHW_LEDNOW,temp,2);//�����������ȡ�������Ȳ�һ��
//	}while(!(temp[1]==n));
}
/*****************************************************************************
���ư���*/
void SetVoice(int8_t status)
{
	uint8_t temp[4]={0};
	ReadDGUS(DHW_SYSCFG,temp,sizeof(temp));
	temp[0]=0x5A;
	if(1==status){temp[3]&=0xF7;}
	else if(2==status){temp[3]|=0x08;}
	else{return;}
  WriteDGUS(DHW_SYSCFG,temp,sizeof(temp));
	do
	{
    DelayMs(5);
	  ReadDGUS(DHW_SYSCFG,temp,1);
	}while(temp[0]!=0);
}
/*****************************************************************************
�жϰ���ֵ�Ƿ����ϴ�*/
int8_t GetValue0F00(void)
{
	uint8_t cleanData[4]={0};
	ReadDGUS(DHW_0F00,value0F00,sizeof(value0F00));
	if(0x5A == value0F00[0])
	{
		WriteDGUS(DHW_0F00,cleanData,sizeof(cleanData));
		return 1;
	}
	return 0;
}
/*****************************************************************************
���ڳ��δ�ӡ*/
void InitUartSend(void)
{
  Uart2SendStr(CUart2,sizeof(CUart2));
//  Uart3SendStr(CUart3,sizeof(CUart3));
//  Uart4SendStr(CUart4,sizeof(CUart4));
//  Uart5SendStr(CUart5,sizeof(CUart5));
}
/*****************************************************************************
��תָ��ҳ��*/
void JumpPage(uint16_t pageId)
{
	uint8_t temp[4]={0x5A,0x01,0,0};
	temp[2]=(uint8_t)(pageId>>8);
	temp[3]=pageId;
  WriteDGUS(DHW_SPAGE,temp,sizeof(temp));
	do
	{
    DelayMs(5);
	  ReadDGUS(DHW_SPAGE,temp,1);
	}while(temp[0]!=0);
}

/*****************************************************************************
������ѹ֮��������*/
void TouchHandle(void)
{
	uint8_t temp[16]={0};
	uint16_t flag=0;
	uint16_t iconid=0;
	uint16_t getDar=0;
  getDar = value0F00[1]<<8|value0F00[2];
	LOG((uint8_t *)&getDar,sizeof(getDar));
	
	s_dl_t_645_8283_body.st_byte1=0x5A;
	s_dl_t_645_8283_body.st_byte2=0xA5;
	s_dl_t_645_8283_body.lenth=value0F00[3]*2+3;	
	s_dl_t_645_8283_body.func=0x83;	
	s_dl_t_645_8283_body.addr1=value0F00[1];
  s_dl_t_645_8283_body.addr2=value0F00[2];
	ReadDGUS(getDar,(uint8_t *)&s_dl_t_645_8283_body.edata[0],value0F00[3]*2);
	Uart2SendStr((uint8_t *)&s_dl_t_645_8283_body,s_dl_t_645_8283_body.lenth+3);

//	switch(getDar)
//	{
//		default:
//		break;
//	}
}

/*****************************************************************************
���ܣ��ش�����*/
void dlt645_handle(void)
{
	uint16_t ADDR=0;

	ADDR = ((uint16_t)(s_dl_t_645_8283_body.addr1))<<8 | s_dl_t_645_8283_body.addr2;

	if(s_dl_t_645_8283_body.func == 0x82)
	{
    WriteDGUS(ADDR,(uint8_t *)&(s_dl_t_645_8283_body.edata),s_dl_t_645_8283_body.lenth-3);
		s_dl_t_645_8283_body.lenth=3;
		s_dl_t_645_8283_body.addr1=0x4F;
		s_dl_t_645_8283_body.addr2=0x4B;
		Uart2SendStr((uint8_t *)&s_dl_t_645_8283_body,s_dl_t_645_8283_body.lenth+3);
	}
	else if(s_dl_t_645_8283_body.func == 0x83)
	{
    ReadDGUS(ADDR,(uint8_t *)&(s_dl_t_645_8283_body.edata[1]),s_dl_t_645_8283_body.edata[0]*2);
		s_dl_t_645_8283_body.lenth+=s_dl_t_645_8283_body.edata[0]*2;
		Uart2SendStr((uint8_t *)&s_dl_t_645_8283_body,s_dl_t_645_8283_body.lenth+3);
	}
	
}
/*****************************************************************************
����:����һ֡���ݽ�ֵ�����ṹ��*/
uint8_t parse_dlt645_frame(uint8_t *p_frame, uint16_t frame_len, uint8_t* sframe_body)
{
    uint16_t temp16_t = 0;//һ֡���ݵ��ܳ���
    uint16_t i = 0;

		if(1){
				memcpy(sframe_body,p_frame, frame_len);
        return 1;
    }
    return 0;
} 
/*****************************************************************************
����:�ӻ�����buf�м���dlt645֡����*/
/*
 * ��һ֡���ݶ�ȡ��frame_buf��     
 * line:������0x7F��ͷ������
 * out:���������֡���Ƶ���������
 * frame_len:�������֡�ĳ��ȣ�
 * line_len:������buf��0x7F��ͷ�����ݳ���
 **/
frame_result_t find_dlt645_frame(uint8_t* line, uint8_t* out, uint16_t* frame_len, uint16_t line_len) {
    uint16_t frame_length = 0;//һ֡���ݵ��ܳ���
    uint16_t temp_len = 0;

    if (line_len < DLT_645_LEAST_LEN) {
        return UNFINISHED;
    }
    //�жϵ�0λ
    if (line[0] != 0x5A) {	
        return ERROR;
    }
    if (line[1] != 0xA5) {	
        return ERROR;
    }		
		frame_length = line[2]+3;
		
		if (frame_length <= line_len) {
						//�쵽һ֡����                
						for (temp_len = 0; temp_len < frame_length; temp_len ++) {
								out[temp_len] = *line;
								line++;
						}
						*frame_len = frame_length;
						return OK;
		} else {
				//���ݻ�û��������
				return UNFINISHED;
		}
}

/*****************************************************************************
����:Э�����ݽ���*/
void ParseBuffer(void)
{
    uint16_t frame_length = 0;//һ֡���ݵ��ܳ���
    uint16_t i = 0, temp_len = 0;
    uint8_t has_content = 0;//buf���Ƿ�������
    uint8_t frame_error = 0;//��������ǰ�����ݶ�����Э�鶼������
    uint8_t*  p_buf= Uart2_Rx;
    protocol_type_t protl_type = PROTOCOL_UNKNOWN;
    frame_result_t find_frame_re = UNKNOWN;

    //��������ÿ��Э�������Ľ�� 
    //frame_results[0] ����PROTOCOL_DL_T_645Э��������
    //frame_results[1] ����PROTOCOL_DL_T_698Э��������
    frame_result_t frame_results[2] = {UNKNOWN, UNKNOWN}; 

    has_content = uart2_rx_count > 2;
    while (has_content) {
        p_buf = Uart2_Rx;
        //����0x5A��ͷ������
        while (*p_buf != 0x5A && p_buf < Uart2_Rx + uart2_rx_count) {
            p_buf ++;
        }
        if (p_buf == Uart2_Rx + uart2_rx_count) {
            //������ǰ�����ݣ��������������
            uart2_rx_count = 0;
            break;
        }
        //Uart4_Rx��ʣ������ݳ���
        temp_len = uart2_rx_count - (p_buf - Uart2_Rx);

        //���´�������У��
        switch(protl_type) {
            case PROTOCOL_UNKNOWN:
                memset(frame_buf,0,sizeof(frame_buf));
                find_frame_re = UNKNOWN;
                frame_error = 0;
                frame_length = 0;
                for (i = 0; i < 3; i ++) {
                    frame_results[i] = UNKNOWN;
                }
                
            case PROTOCOL_DL_T_645:
                find_frame_re = find_dlt645_frame(p_buf, frame_buf, &frame_length, temp_len);

                frame_results[0] = find_frame_re;
                if (find_frame_re == OK) {
											if (parse_dlt645_frame(frame_buf, frame_length, (uint8_t *)&s_dl_t_645_8283_body)) {
													//������һ����Ч����
												dlt645_handle();
											  memset(&s_dl_t_645_8283_body, 0, sizeof(dl_t_645_8283_body));
											}
											break;
                }

            case PROTOCOL_OTHER:
                //�˴��������Э�����
                //break;
                
            default :
                if (frame_results[0] == ERROR ) {
                    //�����������ݲ���������Э��Ľ���
                    //��������һ��0x7F��ʼ��
                    p_buf ++;//������ǰ��0x7F
                    //����0x7F��ͷ������
                    while (*p_buf != 0x5A && p_buf < Uart2_Rx + uart2_rx_count) {
                        p_buf ++;
                    }
                    
                    if (p_buf == Uart2_Rx + uart2_rx_count) {
                        //������ǰ�����ݣ��������������
                        uart2_rx_count = 0;
                        break;
                    }

                    //�ҵ���һ��0x5A��ͷ������֡
                    frame_error = 1;
                }
                break;
        }

        //���ɹ�������һ֡���ݻ򻺴��������ݲ���������Э��Ľ���
        //buf��ʣ�����Ч����ǰ��
        if (find_frame_re == OK || frame_error) {
            //uart_rcvd_bufʣ������ݳ���
            temp_len = uart2_rx_count - (p_buf - Uart2_Rx) - frame_length;
            if (temp_len > 0) {
                //��ǰuart_rcvd_buf��ʣ�������ǰ��
                for (i = 0; i < temp_len; i ++) {
                    Uart2_Rx[i] = *(p_buf + frame_length + i);
                    *(p_buf + frame_length + i) = 0x00;
                }
                has_content = 1;//����ѭ������
            } else {
                //��������λ���
                for (i = 0; i < (p_buf - Uart2_Rx) + frame_length; i ++) {
                    Uart2_Rx[i] = 0x00;
                }
                has_content = 0;
            }
            uart2_rx_count = temp_len;
        } else {
            has_content = 0;
        }
    }
}


/*****************************************************************************
������*/
void HandleProc(void)
{
		if(GetValue0F00())
		{
		  TouchHandle();
		}
		ParseBuffer();
}
