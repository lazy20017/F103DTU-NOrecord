#include "stm32f1xx_hal.h"
#include "my_def_value.h"
//#include "my_OS.h"






//=====OLD����
//�������ݲ���
extern uint8_t rsbuf1[rsbuf_max];	 //���ڽ��ջ�����
extern uint8_t rsbuf2[rsbuf_max];
extern uint8_t rsbuf3[rsbuf_max];
extern uint8_t rsbuf4[rsbuf_max];
extern uint8_t rsbuf5[rsbuf_max];

extern uint8_t txbuf1[256];
extern uint8_t txbuf2[256];
extern uint8_t txbuf3[256];
extern uint8_t txbuf4[256];
extern uint8_t txbuf5[256];

extern uint16_t rsbuf1pt_write;
extern uint16_t rsbuf1pt_read;
extern uint16_t rsbuf1pt_TEMP_read;
extern uint16_t rsbuf1pt_COMM_read;


extern uint16_t rsbuf2pt_write;
extern uint16_t rsbuf2pt_read;
extern uint16_t rsbuf2pt_TEMP_read;
extern uint16_t rsbuf2pt_COMM_read;

extern uint16_t rsbuf3pt_write;
extern uint16_t rsbuf3pt_read;
extern uint16_t rsbuf3pt_TEMP_read;
extern uint16_t rsbuf3pt_COMM_read;

extern uint16_t rsbuf4pt_write;
extern uint16_t rsbuf4pt_read;
extern uint16_t rsbuf4pt_TEMP_read;
extern uint16_t rsbuf4pt_COMM_read;

extern uint16_t rsbuf5pt_write;
extern uint16_t rsbuf5pt_read;
extern uint16_t rsbuf5pt_TEMP_read;
extern uint16_t rsbuf5pt_COMM_read;

//HAL�������
extern uint8_t  my_usart3_re_buf[200];
extern uint16_t my_usart3_re_count;
//END

//������
extern uint8_t gprs_status;
extern uint8_t USART1_TRANSMINT_STATUS;
extern uint8_t USART2_TRANSMINT_STATUS;
extern uint8_t FLASH_DTU;
extern uint8_t my_tim3_dog_cnt;

//=====OLD END====


extern	uint8_t MY_433_Alarmdata_NOtimeBuf[256]; //�洢 ��ʱ�� ��������
extern	uint8_t MY_433_Alarmdata_TimeBuf[256];  //�洢����ʱ�꣬��������
extern	uint8_t MY_433_ALarmdata_NOtime_status; //Ϊ1����ʾ�յ���ʱ�걨������
extern	uint8_t MY_433_ALarmdata_Time_status;   //Ϊ1����ʾ�յ���ʱ�걨������
extern	uint8_t MY_433_ALarmdata_number;  // �洢��������Ϣ�����


extern  uint8_t MY_MCU_RsBuf[8];  //�洢�������Ե�ص�ѹ��̫���ܵ�ѹ���¶ȡ�ʪ�ȹ�4��8���ֽڵ�����
extern  uint8_t MY_MCU_getdata_status; //Ϊ1��ʾ�����ٳɹ��ˣ��յ�����������

extern uint8_t MY_433_Call_Single_data_buf[];
extern uint8_t MY_433_Call_Analog_data_buf[];
extern uint8_t MY_433_Call_Time_data_buf[];
extern uint8_t MY_433_Call_Single_data_number;
extern uint8_t MY_433_Call_Analog_data_number;
extern uint8_t MY_433_Call_Status;

extern uint8_t RE_ALarmData_Status;
extern uint8_t RE_CycleData_Status;


extern uint8_t MY_GPRS_MCU_RsBuf[8];  //�洢�������Ե�ص�ѹ��̫���ܵ�ѹ���¶ȡ�ʪ�ȹ�4��8���ֽڵ�����

extern uint8_t MY_GPRS_Call_Single_data_buf[40];
extern uint8_t MY_GPRS_Call_Analog_data_buf[110];
extern uint8_t MY_GPRS_Call_Time_data_buf[7];
extern uint8_t MY_GPRS_Call_Single_data_number;
extern uint8_t MY_GPRS_Call_Analog_data_number;

extern uint16_t MY_H_speed_cyc;  //����10����
extern uint16_t MY_H_speed_heart;  //����5����
extern uint16_t MY_M_speed_cyc;  //����15����
extern uint16_t MY_M_speed_heart;  //����9����
extern uint16_t MY_L_speed_cyc;  //����30����
extern uint16_t MY_L_speed_heart; //����7����
extern float MY_Speed_H_Gate;
extern float MY_Speed_L_Gate;

//====EEPROM===
extern uint32_t MY_Table1_Alarmdata_StartAddress;
extern uint32_t MY_Table1_Alarmdata_EndAddress;
extern uint32_t MY_Table1_Alarmdata_WriteAddress;
extern uint32_t MY_Table1_Alarmdata_ReadAddress;

extern uint32_t MY_Table2_Cycledata_StartAddress;
extern uint32_t MY_Table2_Cycledata_EndAddress;
extern uint32_t MY_Table2_Cycledata_WriteAddress;
extern uint32_t MY_Table2_Cycledata_ReadAddress;

extern uint8_t MY_Table2_Newdata_status; //EEPROM��Table2���ٲ��������ݱ�ʶ��Ϊ1��ʾ��������д�룬Ϊ0��ʾ������д��
extern uint8_t MY_Table1_Newdata_status; //EEPROM��Table1���������ݱ�ʶ��Ϊ1��ʾ��������д�룬Ϊ0��ʾ������д��
//====END

extern uint8_t MY_433_Call_Single_data_buf[256];
extern uint8_t MY_433_Call_Analog_data_buf[256];
extern uint8_t MY_433_Call_Time_data_buf[7];
extern uint8_t MY_433_Call_Single_data_number;
extern uint8_t MY_433_Call_Analog_data_number;
extern uint8_t MY_433_Call_Status;  //433ģ����������״̬��Ϊ1��ʾ�յ����ݲ����뵽���黺�����У�0Ϊû������

extern uint8_t MY_IP[4];
extern uint16_t MY_PORT;

//====TIM6
extern uint32_t TimingDelay;

extern uint16_t  my_GPRS_all_step;
extern uint8_t my_gprs_TX_status;

extern RTC_DateTypeDef my_RTC_date;
extern RTC_TimeTypeDef my_RTC_time;
extern RTC_HandleTypeDef hrtc;

extern uint16_t MY_ACT_CYC_DTU;
extern uint16_t MY_ACT_Heart_DTU;
extern uint8_t my_act_cyc_dtu_delay;
extern float  my_AD_value;
extern float  my_AD_SUN_value;



extern struct my_ZSQ_change_vale my_zsq_value;
extern uint8_t my_system_restart_status;


