#include "stm32f1xx_hal.h"
//#include "bsp_i2c_ee.h"
#include "bsp_rtc.h"
#include "bsp_spi_flash.h"
#include "bsp_SysTick.h"



void my_array_to_RTCtime(uint8_t *_pWriteBuf);  //�������ֵд�뵽RTC��
void my_EEPROM_TO_RTC(uint8_t *_pWriteBuf,uint8_t _address);//��ȡEEPROM�е�ʱ�꣬д��RTC��

void my_RTCtime_to_array(uint8_t *_pWriteBuf);//��RTC�е�ֵ��������д�뵽������
void my_RTC_TO_EEPROM(uint8_t *_pWriteBuf,uint8_t _address);//��RTC��ǰ��ʱ��ֵ��д�뵽EEPROM��

