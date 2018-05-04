
#include "my_usart.h"
#include "FreeRTOS.h"
#include "event_groups.h"

extern EventGroupHandle_t xCreatedEventGroup;

//#include "my_cc1101.h"

//===HAL�������
//extern INT8U   COM_TxNeed;
//extern INT8U   COM_TimeOut;
//extern INT8U   COM_RxCounter;
//extern INT8U   COM_TxCounter;
//extern INT8U   COM_RxBuffer[];
//extern INT8U   COM_TxBuffer[];

//======END








/*
���ô���2�������ݣ����Ѵ���������CC1101����
*/
uint8_t my_UART1_Status=0;
uint8_t my_UART2_Status=0;
uint8_t my_UART3_Status=0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    uint8_t ret = HAL_OK;
    uint16_t KK=0;
    uint8_t temp=0;
    BaseType_t xResult;
    BaseType_t xHigherPriorityTaskWoken=pdFAIL;

    if(UartHandle==&huart1)
    {


        temp=rsbuf1[rsbuf1pt_write];


        rsbuf1pt_write++;
        if(rsbuf1pt_write>=rsbuf_max)
            rsbuf1pt_write=0;
        __HAL_UART_CLEAR_OREFLAG(&huart1);

        if(temp==0X16)
        {
            xResult=	xEventGroupSetBitsFromISR(xCreatedEventGroup, 0X01,&xHigherPriorityTaskWoken); //m35
            if(xResult!=pdFAIL)
            {
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
        }

        do
        {   KK++;
            if(KK>=0xFFF)
            {
                my_UART1_Status=1;
                break;
            }
            ret = 	HAL_UART_Receive_IT(&huart1,&rsbuf1[rsbuf1pt_write],1);  //��������USART4�������ȴ�������һ���ַ�
        } while(ret != HAL_OK);
        //HAL_UART_Receive_IT(&huart1,&rsbuf1[rsbuf1pt_write],1);
    }

    else if(UartHandle==&huart3)
    {

        temp=rsbuf3[rsbuf3pt_write];

        rsbuf3pt_write++;
        if(rsbuf3pt_write>=rsbuf_max)
            rsbuf3pt_write=0;


        if(temp==0X16)
        {
            xResult=	xEventGroupSetBitsFromISR(xCreatedEventGroup, 0X04,&xHigherPriorityTaskWoken);
            if(xResult!=pdFAIL)
            {
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
        }

        do
        {   KK++;
            if(KK>=0xFFF)
            {
                my_UART3_Status=1;
                //xEventGroupSetBitsFromISR(xCreatedEventGroup, 0X84,&xHigherPriorityTaskWoken);
                break;
            }
            __HAL_UART_CLEAR_OREFLAG(&huart3);
            ret = 	HAL_UART_Receive_IT(&huart3,&rsbuf3[rsbuf3pt_write],1);  //��������USART4�������ȴ�������һ���ַ�
        } while(ret != HAL_OK);


    }

    //===USART2  ���ٷ��ص�֡Ϊ6+24���ֽ�
    if(UartHandle==&huart2)
    {

        temp=rsbuf2[rsbuf2pt_write];
        rsbuf2pt_write++;
        if(rsbuf2pt_write>=30)
            rsbuf2pt_write=0;
        __HAL_UART_CLEAR_OREFLAG(&huart2);

        if(temp==0X16)
        {

        }

        do
        {   KK++;
            if(KK>=0xFFF)
            {
                my_UART2_Status=1;
                break;
            }
            ret = 	HAL_UART_Receive_IT(&huart2,&rsbuf2[rsbuf2pt_write],1);  //��������USART4�������ȴ�������һ���ַ�
        } while(ret != HAL_OK);
        //HAL_UART_Receive_IT(&huart1,&rsbuf1[rsbuf1pt_write],1);
    }



    //

}



