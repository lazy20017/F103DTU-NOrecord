#include "bsp_usart1.h"





/**
 * @brief  USART1 GPIO 配置,工作模式配置。9600 8-N-1
 * @param  无
 * @retval 无
 */


/// 重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{

    HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);

    return (ch);
}




/////////////////////////////
/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 */
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

/*
 * 函数名：USART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 *         典型应用USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 */




void USART_printf(UART_HandleTypeDef* USARTx, uint8_t *Data,...)
{

    const char *s=0;
    int d;
    char buf[16];

    int16_t n=0;
    uint8_t status=0;
    uint8_t  my_0A=0X0A;
    uint8_t  my_0D=0X0D;

    va_list ap;
    va_start(ap, Data);
    ////////////	判断长度

    if(*Data==0x10 && *(Data+5)==0X16)
    {   n=6;
        status=1;
    }  //发送101协议帧
    else if(*Data==0x68&& *(Data+3)==0x68)
    {   n=(*(Data+1)+6);
        status=1;
    }
    else if(*Data==0x10&& *(Data+7)==0x16)
    {   n=8;
        status=1;
    }
    else if(*Data==0x68&& *(Data+5)==0x68)
    {
        n=*(Data+2);
        n=(n<<8)+*(Data+1);
        n=n+8;
        status=1;

    }

    else {
        status=0;
    }

    //



    if(status==1)
    {
        HAL_UART_Transmit(USARTx,Data,n,1000);
    }
    /////////////


    while ( status==0&&*Data != 0)     // 判断是否到达字符串结束符
    {
        if (*Data == 0x5c )  //'\'
        {
            switch ( *++Data )
            {
            case 'r':							          //回车符
                HAL_UART_Transmit(USARTx,&my_0D,1,1000);
                Data ++;
                break;

            case 'n':							          //换行符
                HAL_UART_Transmit(USARTx,&my_0A,1,1000);
                Data ++;
                break;

            default:
                Data ++;
                break;
            }
        }
        else if ( *Data == '%')
        {   //
            switch ( *++Data )
            {
            case 's':										  //字符串
                s = va_arg(ap, const char *);
                HAL_UART_Transmit(USARTx,(uint8_t *)s,strlen(s),1000);
                Data++;
                break;

            case 'd':										//十进制
                d = va_arg(ap, int);
                itoa(d, buf, 10);
                s=buf;
                HAL_UART_Transmit(USARTx,(uint8_t *)s,strlen(s),1000);
                Data++;
                break;
            case 'c':										//字符
                HAL_UART_Transmit(USARTx,(uint8_t *)s,1,1000);
                Data++;
                break;

            default:
                Data++;
                break;
            }
        } //end of else if
        else HAL_UART_Transmit(USARTx,Data++,1,1000);


    }

    //------------
}

//==20170608=====
void USART_printf_long_data(UART_HandleTypeDef* USARTx, uint8_t *Data,uint16_t data_size )
{

    int16_t n=data_size;


    ////////////	判断长度


    HAL_UART_Transmit(USARTx,Data,n,1000);

    /////////////




    //------------
}






/*
功能：433模块发送数据用串口函数，给翻牌功能用
*/


void my_UART4_printf(UART_HandleTypeDef* USARTx, uint8_t *Data)
{



    int16_t n=0;
    uint8_t status=0;


    ////////////

    if(*Data==0x10) {
        n=6;    //发送101协议帧
        status=1;
    }
    else if(*Data==0x68 && *(Data+3)==0x68) {
        n=(*(Data+1)+6);
        status=1;
    }
    else {
        status=0;
    }

    if(status==1)
        HAL_UART_Transmit(USARTx,Data,n,1000);
    /////////////

}

/*********************************************END OF FILE**********************/
