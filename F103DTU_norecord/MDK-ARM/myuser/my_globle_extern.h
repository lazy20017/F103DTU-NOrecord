
//M35
#define USE_INIT_M35 1  //m35оƬ��ʼ����1Ϊʹ�ã�0Ϊ��ʹ��


//OS
#define OS_heap_high_water_data 1  //OS���Դ�����ʾ��ÿ������ʣ��Ķ�ջ������@@@��1Ϊ��ʾ��0Ϊ����ʾ


//Debug_usart
#define Debug_Uart_out_DC_AC_DATA_Status 1  //���ô�����ʾ���յ�ֱ���ͽ����źţ�����ң��
#define Debug_Uart_out_AC12T_DATA_Status 0  //���ô��ڷ���12T����
#define Debug_Uart_out_960WAVE_DATA_Status 0  //���ô�����ʾ���յ���960����

//CC1101
#define Use_CC1101_receive_interrupt_status 1  //1����CC1101�����жϣ�0Ϊ�������ж�
#define Use_CC1101_send_heat_data_status 0    //1����CC101����������������ָʾ��������ʹ�ã�0Ϊ������

#define Debug_uart_out_CC1101_TX_Status 0  //��ʾCC1101������ɺ����ý���̬��״̬

#define Debug_uart_out_cc1101_rx_data_status 0  //1������ʾCC1101���յ����ݣ�0����ʾ
#define Debug_uart_out_cc1101_tx_data_status 0  //1������ʾCC1101���͵����ݣ�0Ϊ����ʾ


//GPRS test
#define Use_GPRS_auto_re_ok  0 //GPRSģ���Զ����յ�OK֡���Ի������Զ�����
#define Use_indicatour_cyc_test_satus 0//����ָʾ����ģ������
#define Use_DTU_huanjing_jisuan 1  //��DTU�Ļ������ݽ���ֱ�Ӽ��㣬����С����1λ

//����ģ��
#define Use_gprs_ALarm_simulation_word 0 //1ʹ��GPRSֱ�ӷ��ͱ���ģ�����ݣ�0Ϊ��ʹ��


//�Ƿ�ʹ��¼������ģʽ
#define User_CC1101_record_status 0  //1��ʾʹ��¼��ģʽ��0��ʾ��ʹ��¼��ģʽ




struct indicator_class 
{
    uint8_t duanlu_data;
    uint8_t jiedi_data;
    uint8_t DC_data_buf[14]; //1�¶ȣ�2��Դ��3�ο���ѹ��4�ɵ�أ�5���ϵ�ѹ��6̫���ܣ�7﮵��
    uint8_t AC_data_buf[6];  //ȫ���������糡���벨����
    uint8_t AC12T_ALL_Current_data_buf[24];
    uint8_t AC12T_ALL_dianchang_data_buf[24];
    uint8_t AC12T_HALF_Current_data_buf[24];
    uint8_t RTC_time_buf[7]; //RTC
    uint8_t data_type;  //01Ϊ���ڣ�02Ϊ����
    uint8_t count_time[2];  //ȫ��ͬ��ֵ
    uint8_t xinhao_db; //�ź�ǿ��
    uint8_t TX_status;  //���ݷ���״̬��������Ϊ0��û�з���Ϊ1

};


struct indicator_class_parameter
{
    uint16_t P1_300A_mul; //5031
    uint16_t P2_Add_value;  //5032
    uint16_t P3_E_mul;  //5033
    uint16_t P4_E_mul2;  //5034
    uint16_t P5_I_deta;  //5035
    uint16_t P6_I_max;  //5036
    uint16_t P7_I_0min;  //5037
    uint16_t P8_E_down_baifenbi;  //5038
    uint16_t P9_E_0min;  //5039
    uint16_t P10_E_down_min;  //503A
    uint16_t P11_V_Libat;  //503B
    uint16_t P12_CYC_time_MIN;  //503C
    uint16_t P13_CYC_time_MAX;  //503D
    uint16_t P14_sleep_time;  //503E
    uint16_t P15_awake_time;  //503F
    uint16_t P16;  //503F
    uint16_t P17_reset_LED_time;  //5041
    uint16_t P18_reset_sys_time;  //5042

};


struct indicator_alarm_class
{
    uint8_t duanlu_data;
    uint8_t jiedi_data;
    uint8_t DC_data_buf[14]; //1�¶ȣ�2��Դ��3�ο���ѹ��4�ɵ�أ�5���ϵ�ѹ��6̫���ܣ�7﮵��
    uint8_t AC_data_buf[6];  //ȫ���������糡���벨����
    uint8_t AC12T_ALL_Current_data_buf[24];
    uint8_t AC12T_ALL_dianchang_data_buf[24];
    uint8_t AC12T_HALF_Current_data_buf[24];
    uint8_t RTC_time_buf[7]; //RTC
    uint8_t data_type;  //01Ϊ���ڣ�02Ϊ����
    uint8_t count_time[2];  //ȫ��ͬ��ֵ
    uint8_t xinhao_db; //�ź�ǿ��
    uint8_t TX_status_duanlu;  //��·���ݷ���״̬��������Ϊ0��û�з���Ϊ1
		uint8_t TX_status_jiedi;   //�ӵ����ݷ���״̬��������Ϊ0��û�з���Ϊ1

};

#define my_record_count 1931
struct indicator_record_class
{
    uint8_t my_wave_record_I_buf[1931];
		uint8_t my_wave_record_E_buf[1931];
    //uint8_t my_wave_type; //1������2�ӵأ�0û��
    uint8_t my_wave_alam; //1���ڣ�2������0û��
		uint16_t my_count_write;
		uint16_t my_count_read;
		uint8_t my_wave_tx_status_I;  //¼�����ݷ���״̬��1��ʾ�������λ�û�з��ͳ�ȥ��0��ʾ�������
		uint8_t my_wave_tx_status_E;
};

struct my_ZSQ_change_vale  //ָʾ�����ò���ʹ��
{
    uint16_t my_inf_add;
    uint16_t zsq_add;
    uint8_t data_buf[8];
    uint8_t status;  //û�з���Ϊ1���������˱�Ϊ0
};


