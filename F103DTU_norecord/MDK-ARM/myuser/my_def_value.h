#include "stm32f1xx_hal.h"

//��������
//********���ڲ���**************************
#define USART_GPRS &huart1   //����M35��Ӧ�Ĵ��ں�
#define USART_DEBUG &huart5   //������Զ˿�
#define USART_GPRS_BUFFER rsbuf1  //����������
#define USART_GPRS_BUFFERpt_READ rsbuf1pt_read
#define USART_GPRS_BUFFERpt_WRITE rsbuf1pt_write


#define rsbuf_max 256
#define rsbuf_min	256
#define rsbuf3_max    rsbuf_max//51K,  51*1024=52224  41K=41984

#define  first_status  0  //�����һ���ϵ�״̬��1Ϊ ��һ���ϵ磬ˢ�����ݣ�0Ϊ����ʹ��

//====EEPROM
//��� ѡͨ���� ��
#define NSS1_LOW()       GPIOA->BRR |= GPIO_PIN_4//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_BIT_RESET)//GPIO_ResetBits(GPIOA, GPIO_Pin_4) 
#define NSS1_HIGH()      GPIOA->BSRR |= GPIO_PIN_4//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_BIT_SET)//GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define NSS2_LOW()       GPIOC->BRR |= GPIO_PIN_4//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_BIT_RESET)//GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define NSS2_HIGH()      GPIOC->BSRR |= GPIO_PIN_4//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_BIT_SET)//GPIO_SetBits(GPIOC, GPIO_Pin_4)


#define WREN  0x06 /* Set Write Enable Latch */
#define WRDI  0x04 /* Reset Write Enable Latch */
#define RDSR  0x05 /* Read Status Register */
#define WRSR  0x01 /* Write Status Register */
#define READ  0x03 /* Read Data From Memory Array */
#define WRITE 0x02 /* Write Data to Memory Array */




#define eeprom 32

#if eeprom==32
//CAT25256 32K
#define I2C_PageSize           64   //ÿ��ҳ����ֽ��������ɵ�
#define EEPROM_Chip_size 1024*32    //����EEPROMоƬ����ʹ�ã����Ϊ1024*128��Ҳ����
#define EEPROM_Chip_high_pin 0x010000  //ʹ��M2Ϊ1024*256 ��ҪA16��A17������ߵ�ַ�ߣ�M1Ϊ1024*128ֻ��ҪA16һ����ߵ�ַ��0x010000#

#elif eeprom==64
//CAT25512  64K
#define I2C_PageSize           128   //ÿ��ҳ����ֽ��������ɵ�
#define EEPROM_Chip_size 1024*64    //����EEPROMоƬ����ʹ�ã����Ϊ1024*128��Ҳ����
#define EEPROM_Chip_high_pin 0x010000  //ʹ��M2Ϊ1024*256 ��ҪA16��A17������ߵ�ַ�ߣ�M1Ϊ1024*128ֻ��ҪA16һ����ߵ�ַ��0x010000

#endif


//===STM FLASH
/* ��ȡ�������ĳ��� */
#define  FLASH_WriteAddress     0x807FB00           // д�ڿ���λ�ã���ֹ�ƻ�����
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_TESTSIZE         512                 //ʵ����512*2=1024�ֽ�


