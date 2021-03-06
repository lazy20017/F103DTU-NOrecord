#ifndef __USART1_H
#define	__USART1_H

//#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include "stdarg.h"  //这个很重要，应为在USART1_printf函数中用到了va_list这个数据类型，这是特殊的数组类型，因此需要此头文件，教材中没提
#include "usart.h"
#include "string.h"
#include "my_extrn_value.h"



////********WDZ 自定义部分**************************
//#define USART_GPRS USART1   //定义M35对应的串口号
//#define USART_DEBUG UART5   //定义调试端口
//#define USART_GPRS_BUFFER rsbuf1  //缓冲区名字
//#define USART_GPRS_BUFFERpt_READ rsbuf1pt_read
//#define USART_GPRS_BUFFERpt_WRITE rsbuf1pt_write


//#define rsbuf_max 256
//#define rsbuf_min	256




//****************WDZ自定义 部分 OVER**********************


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
