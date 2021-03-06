
#include "my_spi.h"

//SPI_HandleTypeDef SpiHandle;

//#define SPI_FLASH_CS_HIGH()		GPIOC->BSRR |= GPIO_PIN_9
//#define SPI_FLASH_CS_LOW()		GPIOC->BRR |= GPIO_PIN_9


//#define W25_PORT_RCC_EN()	__HAL_RCC_GPIOC_CLK_ENABLE()
//#define W25_NSS_PORT		GPIOC
//#define W25_NSS_PIN		  GPIO_PIN_9


//#define SPIx                             SPI3
//#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI3_CLK_ENABLE()
//#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
//#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
//#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

//#define SPIx_FORCE_RESET()               __HAL_RCC_SPI3_FORCE_RESET()
//#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI3_RELEASE_RESET()

///* Definition for SPIx Pins */
//#define SPIx_SCK_PIN                     GPIO_PIN_5
//#define SPIx_SCK_GPIO_PORT               GPIOC
//#define SPIx_SCK_AF                      GPIO_AF5_SPI3
//#define SPIx_MISO_PIN                    GPIO_PIN_11
//#define SPIx_MISO_GPIO_PORT              GPIOC
//#define SPIx_MISO_AF                     GPIO_AF5_SPI3
//#define SPIx_MOSI_PIN                    GPIO_PIN_12
//#define SPIx_MOSI_GPIO_PORT              GPIOC
//#define SPIx_MOSI_AF                     GPIO_AF5_SPI3






/*===========================================================================
* 函数 ：SPI_ExchangeByte() => 通过SPI进行数据交换                          *
* 输入 ：需要写入SPI的值                                                    *
* 输出 ：通过SPI读出的值                                                    *
============================================================================*/
uint8_t SPI_ExchangeByte(uint8_t input)
{
    uint8_t re_data=0;
    //while (RESET == SPI_GetFlagStatus(SPI_FLAG_TXE));   // 等待数据传输完成
    //SPI_SendData(input);
    //while (RESET == SPI_GetFlagStatus(SPI_FLAG_RXNE));  // 等待数据接收完成
    //return (SPI_ReceiveData());

    while(HAL_SPI_GetState(&hspi3)==HAL_SPI_STATE_BUSY_TX); // 等待数据传输完成
    HAL_SPI_TransmitReceive(&hspi3,&input,&re_data,1,1000);


    return re_data;

}
