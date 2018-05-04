#include "bsp_spi_flash.h"


#define EEPROM_RTC_Address 0
#define EEPROM_DTU_Address 7
#define EEPROM_IP_Address 9
#define EEPROM_IPPort_Address 13
#define EEPROM_Hearttime_Address 15
#define EEPROM_Cycetime_Address 17
#define EEPROM_SPEED_Gate_H_Address 19
#define EEPROM_SPEED_H_Cyc_Address 21
#define EEPROM_SPEED_L_Cyc_Address 23
#define EEPROM_sys_start_status 25
#define EEPROM_RE_Alarmdata_t_status 27


#define EEPROM_Table1_Address 201
#define EEPROM_Table2_Address 213

#define EEPROM_Alarm_Table1_StartAddress 1024
#define EEPROM_Alarm_Table1_EndAddress EEPROM_Chip_size-1

#define EEPROM_Cycle_Table2_StartAddress EEPROM_Chip_size
#define EEPROM_Cycle_Table2_EndAddress EEPROM_Chip_size*2-1

#define EEPROM_SIM_Number_StartAddress 461




void my_buf_writeto_val(uint8_t *tempbuf,uint32_t *startaddressPT,uint32_t *endaddressPT,uint32_t *writeaddressPT,uint32_t *readaddressPT);
void my_val_writeto_eeprom(uint8_t *tempbuf,uint32_t startaddressPT,uint32_t endaddressPT,uint32_t writeaddressPT,uint32_t readaddressPT,uint16_t table_startaddress);
void my_buf1_to_buf2(uint8_t *source_buf,uint8_t startaddress1,uint8_t *direc_buf,uint8_t startaddress2,uint8_t number);

void my_cycle_data_to_eeprom(void);  //�������ݵ�EEPROM��
void my_alarm_data_to_eeprom(void);  //��������д��EEPROM��

uint8_t my_eeprom_data_to_cycle_array(void);//EEPROM���������ݵ� �ض�������
uint8_t my_eeprom_data_to_alarm_array(void);//EEPROM�б������ݵ� �ض�������

void my_GPRS_chang_tablereadpt(uint8_t table_number,uint8_t transmint_OK);  //�޸�����֡Ϊ��GPRS����״̬��ͬʱ�ƶ���ָ�뵽��һ֡���ݿ�ʼȥtable_numberΪ1��ʾ������Ϊ2��ʾ�������ݱ�
void MY_Value_init(void);  //������ʼ����������ǵ�һ���ϵ磬���Ĭ�ϲ���д�뵽EEprom�У�������ǵ�һ���ϵ磬���EEPROM�ж�ȡ��������ֵ����Ӧ��ȫ�ֱ���


//*********************
/*
���ܣ����Գ��򣬶�ң�š�ң�⡢�����洢������и�ֵ

*/
void my_test_analoydata_array_to_eeprom(void);
void my_test_Eepromtable_zero(void);//��EEPROM�����ݱ����������

//���ܣ�����ʹ�ã���table2�������ݣ��Ķ�дָ����г�ʼ��Ϊ0x2000
void my_test_TABLE2_init(void);

void HexToStr2(uint8_t *pbDest, uint8_t *pbSrc, int nLen);


