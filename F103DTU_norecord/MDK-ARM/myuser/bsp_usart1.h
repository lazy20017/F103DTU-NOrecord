#ifndef __USART1_H
#define	__USART1_H

//#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include "stdarg.h"  //�������Ҫ��ӦΪ��USART1_printf�������õ���va_list����������ͣ�����������������ͣ������Ҫ��ͷ�ļ����̲���û��
#include "usart.h"
#include "string.h"
#include "my_extrn_value.h"



////********WDZ �Զ��岿��**************************
//#define USART_GPRS USART1   //����M35��Ӧ�Ĵ��ں�
//#define USART_DEBUG UART5   //������Զ˿�
//#define USART_GPRS_BUFFER rsbuf1  //����������
//#define USART_GPRS_BUFFERpt_READ rsbuf1pt_read
//#define USART_GPRS_BUFFERpt_WRITE rsbuf1pt_write


//#define rsbuf_max 256
//#define rsbuf_min	256




//****************WDZ�Զ��� ���� OVER**********************


void USART2_Config(void);
void USART1_Config(void);

void USART3_Config(void);
void UART4_Config(void);
void UART5_Config(void);

void NVIC_USART_Configuration(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

void USART_printf(UART_HandleTypeDef* USARTx, uint8_t *Data,...);
char *itoa(int value, char *string, int radix);

void my_UART4_printf(UART_HandleTypeDef* USARTx, uint8_t *Data);

//void my_USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART1_H */