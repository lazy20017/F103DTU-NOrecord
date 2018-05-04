#include "wdz_101.h"
//==========
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;

//==========
extern uint8_t rsbuf1[];	  //USART1������
extern uint8_t rsbuf2[];   //USART2������
extern uint8_t rsbuf3[];   //USART2������
extern uint8_t rsbuf4[];   //USART2������
extern uint8_t rsbuf5[];   //USART2������

extern uint8_t txbuf1[];
//extern uint8_t txbuf2[];
extern uint8_t txbuf3[];
extern uint8_t txbuf4[];
extern uint8_t txbuf5[];


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



extern  uint32_t TimingDelay;
extern  uint16_t DTU_ADDRESS;

extern struct rtc_time systmtime;  //RTCʵʱʱ��ʹ��
extern u8 rtcbuffer[];

extern uint16_t Transmint_to_GPRS_hearttime;
extern uint16_t Transmint_to_GPRS_Cycledata;

extern uint16_t my_tim6_count;

//ͨ�ò���

//����1
extern uint16_t USART1_address_first;
extern uint16_t USART1_address_second;
extern uint8_t USART1_length;
extern uint8_t USART1_FRAME_status;
extern uint8_t USART1_my_frame[256];
extern uint8_t USART1_TRANSMINT_STATUS;
extern uint8_t USART1_RESIVER_STATUS;

//����2
uint16_t USART2_address_first=0;
uint16_t USART2_address_second=0;
uint8_t USART2_length=0;
uint8_t USART2_FRAME_status=0;
uint8_t USART2_my_frame[256]= {0};
uint8_t USART2_TRANSMINT_STATUS=0;
uint8_t USART2_RESIVER_STATUS=0;
//����3
uint16_t USART3_address_first=0;
uint16_t USART3_address_second=0;
uint8_t USART3_length=0;
uint8_t USART3_FRAME_status=0;
uint8_t USART3_my_frame[100]= {0};
uint8_t USART3_TRANSMINT_STATUS=0;
uint8_t USART3_RESIVER_STATUS=0;

//����4
uint16_t USART4_address_first=0;
uint16_t USART4_address_second=0;
uint8_t USART4_length=0;
uint8_t USART4_FRAME_status=0;
uint8_t USART4_my_frame[100]= {0};
uint8_t USART4_TRANSMINT_STATUS=0;
uint8_t USART4_RESIVER_STATUS=0;

//-----433ģ����ת��
uint8_t my_433_anlag_buf[110]= {0}; //�洢433ģ��������������0���������洢��ֻҪ�б仯�����ñ�־λ����ת����
uint8_t my_433_anlag_flag=0;  //��־λ��Ϊ0��ʾ�Ѿ������������ݣ�Ϊ1��ʾ�������ݵ��ǻ�û�з���

//433ģ�� �������ݴ洢���黺����

extern uint8_t MY_433_Call_Single_data_buf[256];
extern uint8_t MY_433_Call_Analog_data_buf[256];
extern uint8_t MY_433_Call_Time_data_buf[7];
extern uint8_t MY_433_Call_Single_data_number;
extern uint8_t MY_433_Call_Analog_data_number;
extern uint8_t MY_433_Call_Status;  //433ģ����������״̬��Ϊ1��ʾ�յ����ݲ����뵽���黺�����У�0Ϊû������



/*
���ܣ����ݽ��շ������˲���Ϊ����101Э�����ݣ���֡ͷ����֡β����֡������������������Ҫ��ȡ������һ֡���ݡ�
������������ں�
��������� ȡ֡���ݳɹ�����1��ȡ֡����ʧ�ܷ���0
*/
uint8_t my_usart_101frame(uint8_t usart_port)
{
    uint8_t *rsbuf=0;
    uint16_t *rsbufpt_read=0;
    uint16_t *rsbufpt_COMM_read=0;
    uint16_t *rsbufpt_write=0;
    uint8_t *my_frame=0;
    uint8_t *USART_FRAME=0;


    uint16_t *address_first=0;
    uint16_t *address_second=0;
    uint8_t *length=0;

    uint8_t ch1=0;
    uint16_t pt=0;
    uint8_t tmp_status=0;
    uint8_t status=0;
    uint16_t ii=0;

    uint8_t my_temp_status=0;
    uint16_t temp_address_first=0;
    uint8_t temp_length=0;



//******���ڵ�ѡ��****************
    if(usart_port==1)
    {
        rsbuf=rsbuf1;
        rsbufpt_read=&rsbuf1pt_read;
        rsbufpt_COMM_read=&rsbuf1pt_COMM_read;
        rsbufpt_write=&rsbuf1pt_write;
        address_first=&USART1_address_first;
        address_second=&USART1_address_second;
        length=&USART1_length;
        my_frame=USART1_my_frame;
        USART_FRAME=&USART1_FRAME_status;


    }
    if(usart_port==2)
    {
        rsbuf=rsbuf2;
        rsbufpt_read=&rsbuf2pt_read;
        rsbufpt_COMM_read=&rsbuf2pt_COMM_read;
        rsbufpt_write=&rsbuf2pt_write;
        address_first=&USART2_address_first;
        address_second=&USART2_address_second;
        length=&USART2_length;
        my_frame=USART2_my_frame;
        USART_FRAME=&USART2_FRAME_status;


    }
    if(usart_port==3)
    {
        rsbuf=rsbuf3;
        rsbufpt_read=&rsbuf3pt_read;
        rsbufpt_COMM_read=&rsbuf3pt_COMM_read;
        rsbufpt_write=&rsbuf3pt_write;
        address_first=&USART3_address_first;
        address_second=&USART3_address_second;
        length=&USART3_length;
        my_frame=USART3_my_frame;
        USART_FRAME=&USART3_FRAME_status;


    }

    if(usart_port==4)
    {
        rsbuf=rsbuf4;
        rsbufpt_read=&rsbuf4pt_read;
        rsbufpt_COMM_read=&rsbuf4pt_COMM_read;
        rsbufpt_write=&rsbuf4pt_write;
        address_first=&USART4_address_first;
        address_second=&USART4_address_second;
        length=&USART4_length;
        my_frame=USART4_my_frame;
        USART_FRAME=&USART4_FRAME_status;


    }

////////****����ѡ�����***************



    while(*USART_FRAME==0 && *rsbufpt_COMM_read!=*rsbufpt_write)  //ǰһ��ָ�����ɣ�����δ�������ַ�������д�����������������������һ��ָ����������ַ��������
    {

        //ȡһ��δ�������ַ�
        if(*rsbufpt_COMM_read==rsbuf_max-1)
        {
            ch1=rsbuf[*rsbufpt_COMM_read];
            *rsbufpt_COMM_read=0;
            pt=*rsbufpt_COMM_read;
        }
        else
        {
            ch1=rsbuf[*rsbufpt_COMM_read];
            *rsbufpt_COMM_read=*rsbufpt_COMM_read+1;
            pt=*rsbufpt_COMM_read;
        }

        //����0X68֡ͷ��֡β���
        if(ch1==0x68)
        {
            tmp_status=1;
        }
        else if(ch1==0x16)
        {
            tmp_status=2;
        }
        else
        {
            *USART_FRAME=0;
            my_temp_status=0;
            tmp_status=0;
        }


        //����0X68֡ͷ����
        if(tmp_status==1)
        {

            //
            if((pt>0&&pt<4)&&ch1==rsbuf[rsbuf_max+pt-4]&&ch1==0x68)
            {

                *address_first=rsbuf_max+pt-4;
                if(pt-3==0)*length=rsbuf[pt-3];
                else *length=rsbuf[rsbuf_max+pt-3];

            }
            else if(pt>=4&&ch1==rsbuf[pt-4]&&ch1==0x68)
            {

                *address_first=pt-4;
                *length=rsbuf[pt-3];
            }
            else if(pt==0&&ch1==rsbuf[rsbuf_max-4]&&ch1==0x68)
            {

                *address_first=rsbuf_max-4;
                *length=rsbuf[rsbuf_max-3];
            }

            else
            {
                *USART_FRAME=0;
                my_temp_status=0;
                tmp_status=0;
            }

        }
        //������֡β��֡ͷ�ķ���
        if(tmp_status==2)
        {
            temp_address_first=*address_first;
            temp_length=*length;
            //�̶�����֡
            if(pt>0&&pt<6&&rsbuf[rsbuf_max+pt-6]==0x10)
            {

                *address_first=rsbuf_max+pt-6;
                *length=6;
                *address_second=pt-1;
                *USART_FRAME=1;
                my_temp_status=1;

            }
            else if(pt>=6&&rsbuf[pt-6]==0x10)
            {
                if(rsbuf[pt-2]!=(rsbuf[pt-3]+rsbuf[pt-4]+rsbuf[pt-5]))
                {
                    my_temp_status=0;
                    tmp_status=0;
                    return (0);
                }

                *address_first=pt-6;
                *length=6;
                *address_second=pt-1;
                *USART_FRAME=1;
                my_temp_status=1;


            }
            else if(pt==0&&rsbuf[rsbuf_max-6]==0x10)
            {
                if(rsbuf[rsbuf_max-2]!=(rsbuf[rsbuf_max-3]+rsbuf[rsbuf_max-4]+rsbuf[rsbuf_max-5]))
                {
                    my_temp_status=0;
                    tmp_status=0;
                    return (0);
                }

                *address_first=rsbuf_max-6;
                *length=6;
                *address_second=pt-1;
                *USART_FRAME=1;
                my_temp_status=1;


            }
            //�ǹ̶�����֡
            if(pt-6-temp_address_first==(temp_length)&&temp_address_first<pt)
            {
                *address_second=pt-1;
                *address_first=temp_address_first;
                *length=temp_length;
                *USART_FRAME=2;
                my_temp_status=2;
            }
            else if(temp_address_first>pt&&pt!=0)
            {
                if((pt+rsbuf_max-temp_address_first-6)==(temp_length))
                {   *address_second=pt-1;
                    *address_first=temp_address_first;
                    *length=temp_length;
                    *USART_FRAME=2;
                    my_temp_status=2;
                }
            }
            else if(pt==0)
            {
                if((rsbuf_max-temp_address_first-6)==(temp_length))
                {   *address_second=rsbuf_max-1;
                    *address_first=temp_address_first;
                    *length=temp_length;

                    *USART_FRAME=2;
                    my_temp_status=2;
                }
            }
            //


        }
    }

//ȡһ֡���ݴ��뵽ָ��������



    if(my_temp_status>0)	  //���������һ֡���ݣ��Ϳ�ʼ���������򷵻أ�������
    {
        //�������������
        for(ii=0; ii<256; ii++)
        {
            my_frame[ii]=0;
        }

        //ȡ�̶�����ָ��
        if(rsbuf[*address_first]==0x10)
        {
            for(ii=0; ii<6; ii++)
            {
                my_frame[ii]=rsbuf[*address_first];
                *address_first=*address_first+1;
                if(*address_first>=rsbuf_max) *address_first=0;
            }

            *rsbufpt_read=*address_second+1;
            if(*rsbufpt_read>=rsbuf_max)*rsbufpt_read=0;

            status=1;
        }
        //ȡ�ǹ̶�����ָ��
        else if(rsbuf[*address_first]==0x68)
        {
            for(ii=0; ii<6+*length; ii++)
            {
                my_frame[ii]=rsbuf[*address_first];
                *address_first=*address_first+1;
                if(*address_first>=rsbuf_max) *address_first=0;
            }

            *rsbufpt_read=*address_second+1;
            if(*rsbufpt_read>=rsbuf_max)*rsbufpt_read=0;
            status=1;
        }
        else
        {
            status=0;
        }
    }

    //���з��ش���
    if(status==1)
    {
        //*USART_FRAME=0;  //ȡ��һָ֡����б��
        //printf("\r\nOK=%s",my_frame);
        //USART_printf(USARTx,"\r\nOK=%s",my_frame);
        //USART_printf(USARTx,"%s",my_frame);
        return(1);
    }
    else
    {
        //USART_FRAME=0;  //ȡ��һָ֡����б��
        //printf("\r\nERROR");
        return(0);
    }

}



/*
���ܣ�����2 pwr��MCU  �������� ָ������򣬷����������յ�ָ���������
���룺��
�������
*/
void my_process_resive_101usart2()
{
    uint8_t status68=0;
    //uint8_t status10=0;
    uint16_t my_dtu_address=0;
    uint8_t *pt;
    uint16_t ii=0;
    uint16_t n=0;

    //���յ���ָ����з���������������������д����������㱨��,�����������������ϡ������趨�� ����վ���������͵ĳ���
    //������������ڴ����������ͣ��ȴ�Ӧ��ʱ���˳��������з���֡�������ɷ�����������Լ����в�ѯ����ֹ��֡�ĳ�ͻ����
    if(USART2_FRAME_status>0)
    {
        //����ʹ��
        //USART_printf(&huart1,"\r\n");
        //USART_printf(&huart1,USART1_my_frame);
        //USART_printf(&huart1,"\r\n");
        //if(USART1_my_frame[1]==0x31) USART_printf(&huart1,"\r\ndata is ok !!\r\n");
        //else USART_printf(&huart1,"\r\ndata is error !!\r\n");
        //------------------

        //֡ͷ���
        if(USART2_my_frame[0]==0x68)
        {   status68=1;
            //status10=0;
        }

        else if(USART2_my_frame[0]==0x10)
        {   status68=0;
            //status10=1;
        }
        //��վ��ַ���
        if(status68==1)
        {
            my_dtu_address=USART2_my_frame[6];
            my_dtu_address=(my_dtu_address<<8)+USART2_my_frame[5];
            if(my_dtu_address==0x0001)//DTU_ADDRESS)
                status68=1;
            else status68=0;
        }
        //У���ֽڼ��

        //֡���ͼ�顢����ԭ����
        if(status68==1)
        {
            if(USART2_my_frame[7]==0X67&&USART2_my_frame[9]==0X06)// ʱ��ͬ������
            {

                rtcbuffer[0]=USART2_my_frame[14];
                rtcbuffer[1]=USART2_my_frame[15];
                rtcbuffer[2]=USART2_my_frame[16]&0x3f;
                rtcbuffer[3]=USART2_my_frame[17]&0x1f;
                rtcbuffer[4]=USART2_my_frame[18]&0x1f;
                rtcbuffer[5]=USART2_my_frame[19]&0x0f;
                rtcbuffer[6]=USART2_my_frame[20]&0x7f;

                my_array_to_RTCtime(rtcbuffer); //д��ʱ���RTC

                my_process_transmit_101usart2(COMM_101_OK,1,100);  //����ȷ��ָ��

                //Delay_us(100); //����ʹ��

                my_RTCtime_to_array(rtcbuffer);
                pt=COMM_101_TIME_CONFIRM;
                n=*(pt+1);
                for(ii=0; ii<n+6; ii++)
                {
                    txbuf1[ii]=*pt++;
                }
                txbuf1[ii]=0x1A;
                txbuf1[ii+1]=0x00;

                txbuf1[14]=rtcbuffer[0];
                txbuf1[15]=rtcbuffer[1];
                txbuf1[16]=rtcbuffer[2];
                txbuf1[17]=rtcbuffer[3];
                txbuf1[18]=rtcbuffer[4];
                txbuf1[19]=rtcbuffer[5];
                txbuf1[20]=rtcbuffer[6];
                my_process_transmit_101usart2(txbuf1,1,100);  //����ʱ��ͬ��ȷ�ϼ���ָ��
                //
                status68=0;   //������
            }
            else status68=0;
        }


        USART2_FRAME_status=0;
    }
}


/*
���ܣ�����1����101��������,û��ʹ��
���������ָ���ַ��Ӧ��ָ���ַ���ظ����ʹ˴Σ��ظ����ͼ��ʱ��
������������ͳɹ�����1��ʧ�ܷ���0
*/
uint8_t my_process_transmit_101usart1(uint8_t *commpt,uint8_t N,uint16_t Time)
{
    uint8_t tempstatus=0;
    uint8_t n=0;
    uint16_t time_stop=Time/100;
    uint16_t my_start_time=0;

//�������յ�ָ���Ƿ������ˣ��������ˣ����У�û�����꣬��ʱ����������У��˳�������
    if(USART1_FRAME_status==1)
    {
        Delay_us(500);
    }
    if(USART1_FRAME_status==1)
    {
        USART_printf(&huart3,"USART1 IS BUSY,CAN NOT TRANSMINT DATA\r\n");
        tempstatus=0;
        return (tempstatus);
    }

//
    do
    {
        my_start_time=my_tim6_count;
        USART_printf(&huart1,commpt);
        n++;
        do
        {
            tempstatus=my_usart_101frame(1);
            if(tempstatus==1) break;
        } while(my_tim6_count-my_start_time<time_stop);
    } while(n!=N&&tempstatus!=1);

    /*
    if(tempstatus==1)
    {
    	USART_printf(USART_DEBUG,"TRANSMIT DATA IS OK ,AND RESIVE A ANSWER DATA=%s",USART1_my_frame);

    	USART1_FRAME_status=0; //����ǲ���ʹ�ã������������Ӧ��ɾ������Ҫ�ڷ���ָ���н��з�������

    }
    */
    return (tempstatus);  //����ָ����з���ָ�����һ�γɹ����ͣ�����1�����û�з���ָ����ʧ��


}


/*
����ʹ�ú�����û��ʹ��

*/
void my_test_usart1(uint8_t *my_COMM_TRANSMIT)  //����101���������������
{
    uint8_t *COMM_TRANSMIT;
    uint8_t tempstatus=0;
    //LED2_TOGGLE;  //����ʹ��
    //

    if(USART1_RESIVER_STATUS==0)
    {
        USART1_TRANSMINT_STATUS=1;
        COMM_TRANSMIT=my_COMM_TRANSMIT;  //���͵�����
        tempstatus=my_process_transmit_101usart1(COMM_TRANSMIT,3,50);//���յ������Ƿ�ɹ�

        if(tempstatus==1)       //��������ɹ�����
        {
            USART_printf(USART_DEBUG,"TRANSMIT DATA IS OK ,AND RESIVE A ANSWER DATA=");
            USART_printf(USART_DEBUG,USART1_my_frame);
            USART_printf(USART_DEBUG,"\r\n");
            USART1_FRAME_status=0; //����ǲ���ʹ�ã������������Ӧ��ɾ������Ҫ�ڷ���ָ���н��з�������
        }
        else if(tempstatus==0)
        {
            USART_printf(USART_DEBUG,"TRANSMIT DATA IS ERROR ,NO ANSWER COMM==%s\r\n",COMM_TRANSMIT);
            USART1_FRAME_status=0; //����ǲ���ʹ�ã������������Ӧ��ɾ������Ҫ�ڷ���ָ���н��з�������
        }

        USART1_TRANSMINT_STATUS=0;

    }
}
/*
���ܣ�����2����GPRS ����101��������
���������ָ���ַ��Ӧ��ָ���ַ���ظ����ʹ˴Σ��ظ����ͼ��ʱ��
������������ͳɹ�����1��ʧ�ܷ���0
*/
uint8_t my_process_transmit_101usart2(uint8_t *commpt,uint8_t N,uint16_t Time)
{
    uint8_t tempstatus=0;
    uint8_t n=0;
    uint16_t time_stop=Time/100;
    uint16_t my_start_time=0;

//�������յ�ָ���Ƿ������ˣ��������ˣ����У�û�����꣬��ʱ����������У��˳�������
    if(USART2_FRAME_status==1)
    {
        Delay_us(500);
    }
    if(USART2_FRAME_status==1)
    {
        USART_printf(USART_DEBUG," USART2 IS BUSY,CAN NOT TRANSMINT DATA\r\n");
        tempstatus=0;
        return (tempstatus);
    }

//
    do
    {

        //USART_printf(&huart2,commpt);
        my_at_senddata(commpt);
        my_start_time=my_tim6_count;
        n++;
        do
        {
            tempstatus=my_usart_101frame(2);
            if(tempstatus==1) break;
        } while(my_tim6_count-my_start_time<time_stop);
    } while(n!=N&&tempstatus!=1);

    /*
    if(tempstatus==1)
    {
    	USART_printf(USART_DEBUG,"TRANSMIT DATA IS OK ,AND RESIVE A ANSWER DATA=%s",USART1_my_frame);

    	USART1_FRAME_status=0; //����ǲ���ʹ�ã������������Ӧ��ɾ������Ҫ�ڷ���ָ���н��з�������

    }
    */
    return (tempstatus);  //����ָ����з���ָ�����һ�γɹ����ͣ�����1�����û�з���ָ����ʧ��


}



//*************USART3 WDZ ����ʹ��*************//

/*
���ܣ�����3�������� ָ������򣬷����������յ�ָ���������
     (1)67-06���Խ���RTCУʱ������101Э���ʽ68 11 11 68 D3 01 00 67 01 06 01 00 00 00 /08 67 0F 07 02 07 0D/ 75 16
		 (2)67-07DTU��ַ����EEPROM��             68 0C 0C 68 D3 01 00 67 01 07 01 00 00 00 /02 00/ 75 16
		 (3)67-08 IP��ַ
		 (4)67-09 �˿�
		 (5)67-0A  DTU��ַ�������еı�����ַ
		 (6)67-0B  ����MCU
		 (7)67-0C ������ʱ����
		 (8)67-0D  ���ڰ�ʱ����

���룺��
�������
*/
extern uint8_t MY_IP[4];
extern uint16_t MY_PORT;
void my_process_resive_101usart3(void)
{
    uint8_t status68=0;
    uint16_t my_dtu_address=0;
//	uint8_t temp=0;
    uint8_t tx_buf[150]= {"\x10\x00\x01\x00\x01\x16"};
    uint8_t tx_buf2[250]= {"\x68\x11\x11\x68\xD3\x01\x00\x67\x01\x06\x01\x00\x00\x00\x08\x67\x0F\x07\x02\x07\x0D\x75\x16"};
    uint8_t time_buf[7]= {0};
    uint16_t temp_16=0;




    //���յ���ָ����з���������������������д����������㱨��,�����������������ϡ������趨�� ����վ���������͵ĳ���
    //������������ڴ����������ͣ��ȴ�Ӧ��ʱ���˳��������з���֡�������ɷ�����������Լ����в�ѯ����ֹ��֡�ĳ�ͻ����
    if(USART3_FRAME_status>0)
    {
        //�رն�ʱ��4

        __HAL_RCC_TIM4_CLK_DISABLE();


        //֡ͷ���
        if(USART3_my_frame[0]==0x68)
        {
            status68=2;
        }

        else if(USART3_my_frame[0]==0x10)
        {
            status68=1;
        }
        //��վ��ַ���
        //if(status68==2)
        //{
        //	my_dtu_address=USART3_my_frame[6];
        //	my_dtu_address=(my_dtu_address<<8)+USART3_my_frame[5];
        //if(my_dtu_address==DTU_ADDRESS)
        //			status68=2;
        //else status68=0;
        //	}
        //У���ֽڼ��
        //temp=wdz_101char_check(USART3_my_frame);  //����ʱ���Կ����Ȳ�ʹ��
        //if(temp==1) status68=2;
        //else status68=0;

        //֡���ͼ�顢����ԭ����
        if(status68==2)
        {

            //1 Уʱ************
            if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X06)// ʱ��ͬ������ 67-06Уʱ
            {

                rtcbuffer[0]=USART3_my_frame[14];
                rtcbuffer[1]=USART3_my_frame[15];
                rtcbuffer[2]=USART3_my_frame[16]&0x3f;
                rtcbuffer[3]=USART3_my_frame[17]&0x1f;
                rtcbuffer[4]=USART3_my_frame[18]&0x1f;  //��
                rtcbuffer[5]=USART3_my_frame[19]&0x0f;  //��
                rtcbuffer[6]=USART3_my_frame[20]&0x7f; //��

                my_array_to_RTCtime(rtcbuffer); //д��ʱ���RTC


                //���»ظ�����
                my_RTC_TO_EEPROM(rtcbuffer,0);

                USART_printf(&huart3,tx_buf); //����ȷ������
                my_RTCtime_to_array(time_buf);  //��õ�ǰʱ��
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //����Уʱ����ȷ��֡


                //status68=0;   //������
            }
            //Уʱ******��********

            //2  DTU��ַ�޸�*******
            //(2)67-07DTU��ַ���ã�68 0C 0C 68 D3 01 00 67 01 07 01 00 00 00 /02 00/ 75 16
            else if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X07) //DTU��ַ�޸� 67-07
            {
                my_dtu_address=USART3_my_frame[15];
                my_dtu_address=(my_dtu_address<<8)+USART3_my_frame[14];
                DTU_ADDRESS=my_dtu_address;


                AT25_WriteByte(USART3_my_frame[14],EEPROM_DTU_Address); //�ѵ�ַд��EEPROM��
                AT25_WriteByte(USART3_my_frame[15],EEPROM_DTU_Address+1);
                USART_printf(&huart3,tx_buf); //����OKȷ������


                time_buf[0]=AT25_ReadByte(EEPROM_DTU_Address);
                time_buf[1]=AT25_ReadByte(EEPROM_DTU_Address+1);
                time_buf[2]	=0XFF;
                time_buf[3]	=0XFF;
                time_buf[4]	=0XFF;
                time_buf[5]	=0XFF;
                time_buf[6]	=0XFF;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x07;

                USART_printf(&huart3,tx_buf2); //������DTU��ַ��

                my_reset2_mcu(); //������MUC

            }
            //**********DTU��ַ�޸�***��****


            //3 *****IP��ַ*****
            if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X08)// ʱ��ͬ������ 67-06Уʱ
            {

                time_buf[0]=USART3_my_frame[14];  //IP0
                time_buf[1]=USART3_my_frame[15];  //IP1
                time_buf[2]=USART3_my_frame[16];  //IP2
                time_buf[3]=USART3_my_frame[17];  //IP3
                time_buf[4]=USART3_my_frame[18];
                time_buf[5]=USART3_my_frame[19];
                time_buf[6]=USART3_my_frame[20];
                //uint8_t MY_IP[4]={222,222,118,3};
                //uint16_t MY_PORT=8080;
                MY_IP[0]=time_buf[0];
                MY_IP[1]=time_buf[1];
                MY_IP[2]=time_buf[2];
                MY_IP[3]=time_buf[3];


                //���µ�Server��IP��ַ���˿ں�д�뵽EEPROM��

                SPI_EE_BufferWrite2(MY_IP,EEPROM_IP_Address,4); //��Ĭ��IP��ַ д�뵽EEPROM�� 222.222.118.3������ʮ���Ƶ�


                USART_printf(&huart3,tx_buf); //����ȷ������


                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x08;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //����Уʱ����ȷ��֡

                my_reset2_mcu(); //������MUC
                //status68=0;   //������
            }
            // ***IP��ַ*��********

            //4 *****PORT��ַ*****
            if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X09)// ʱ��ͬ������ 67-09Уʱ
            {

                time_buf[0]=USART3_my_frame[14]; //PORT0
                time_buf[1]=USART3_my_frame[15]; //PORT1
                time_buf[2]=USART3_my_frame[16];
                time_buf[3]=USART3_my_frame[17];
                time_buf[4]=USART3_my_frame[18];
                time_buf[5]=USART3_my_frame[19];
                time_buf[6]=USART3_my_frame[20];

                temp_16=time_buf[1];
                //temp_16=(temp_16<<8)&0XFF00;
                //MY_PORT=temp_16;
                //temp_16=time_buf[0];
                //MY_PORT=MY_PORT+temp_16;
                MY_PORT=((temp_16<<8)&0XFF00)+time_buf[0];  //�����⣬���ܼ���time buf��0��


                temp_16=MY_PORT;    //Ĭ�϶˿ںţ�8080,����ʮ���Ƶ�
                AT25_WriteByte(time_buf[0],EEPROM_IPPort_Address);
                AT25_WriteByte(time_buf[1],EEPROM_IPPort_Address+1);


                USART_printf(&huart3,tx_buf); //����OKȷ������


                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x09;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //����PORT���ݰ�

                my_reset2_mcu(); //������MUC
                //status68=0;   //������
            }
            // ***PORT��ַ*��********

            // byte[] xx=new byte[]{ 0x68, 0x0C, 0x0C, 0x68, 0xD3, 0x01, 0x00, 0x67, 0x01, 0x20, 0x01, 0x00, 0x00, 0x00, 0xAA, 0xBB,0x8F, 0x16 };  //67-20

            //5 *****����ָ��*****
            if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X0B)// ʱ��ͬ������ 67-0B����
            {

                WDZ_MCUtransmit_restdata();

                //	Delay_ms(100);

                WDZ_MCUtransmit_restdata();
                //		Delay_ms(100);

                WDZ_MCUtransmit_restdata();
                //		Delay_ms(100);


                USART_printf(&huart3,tx_buf); //����OKȷ������





                //status68=0;   //������
            }
            // ***�������*******

            //6 *****������ʱ��*****
            if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X0C)// ʱ��ͬ������ 67-09Уʱ
            {

                time_buf[0]=USART3_my_frame[14]; //time_low
                time_buf[1]=USART3_my_frame[15]; //time_high
                time_buf[2]=USART3_my_frame[16];
                time_buf[3]=USART3_my_frame[17];
                time_buf[4]=USART3_my_frame[18];
                time_buf[5]=USART3_my_frame[19];
                time_buf[6]=USART3_my_frame[20];

                temp_16=time_buf[1];
                Transmint_to_GPRS_hearttime=((temp_16<<8)&0XFF00)+time_buf[0];  //�����⣬���ܼ���time buf��0��


                temp_16=Transmint_to_GPRS_hearttime;    //Ĭ�϶˿ںţ�8080,����ʮ���Ƶ�
                AT25_WriteByte(time_buf[0],EEPROM_Hearttime_Address);
                AT25_WriteByte(time_buf[1],EEPROM_Hearttime_Address+1);



                USART_printf(&huart3,tx_buf); //����OKȷ������


                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x0C;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //����PORT���ݰ�


                //status68=0;   //������
            }
            // ***������ʱ��*��********

            //7 *****���ڰ�ʱ��*****
            if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X0D)// ʱ��ͬ������ 67-09Уʱ
            {

                time_buf[0]=USART3_my_frame[14]; //time_low
                time_buf[1]=USART3_my_frame[15]; //time_high
                time_buf[2]=USART3_my_frame[16];
                time_buf[3]=USART3_my_frame[17];
                time_buf[4]=USART3_my_frame[18];
                time_buf[5]=USART3_my_frame[19];
                time_buf[6]=USART3_my_frame[20];

                temp_16=time_buf[1];
                Transmint_to_GPRS_Cycledata=((temp_16<<8)&0XFF00)+time_buf[0];  //�����⣬���ܼ���time buf��0��


                temp_16=Transmint_to_GPRS_Cycledata;    //Ĭ�϶˿ںţ�8080,����ʮ���Ƶ�
                AT25_WriteByte(time_buf[0],EEPROM_Cycetime_Address);
                AT25_WriteByte(time_buf[1],EEPROM_Cycetime_Address+1);



                USART_printf(&huart3,tx_buf); //����OKȷ������


                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x0D;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //����PORT���ݰ�


                //status68=0;   //������
            }
            // ***������ʱ��*��********

            //***��ѯ����
            else if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X20) //DTU��ַ�޸� 67-20
            {

                USART_printf(&huart3,tx_buf); //����OKȷ������

                //---------RTC----------
                my_RTCtime_to_array(time_buf);  //��õ�ǰʱ��
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x06;
                USART_printf(&huart3,tx_buf2); //����Уʱ����ȷ��֡
                //----DTU--EEPROM--

                time_buf[0]=AT25_ReadByte(EEPROM_DTU_Address);
                time_buf[1]=AT25_ReadByte(EEPROM_DTU_Address+1);
                time_buf[2]	=0XFF;
                time_buf[3]	=0XFF;
                time_buf[4]	=0XFF;
                time_buf[5]	=0XFF;
                time_buf[6]	=0XFF;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x07;

                USART_printf(&huart3,tx_buf2); //����EEPROM��DTU��ַ��

                //-----DTU--CODE------------
                time_buf[0]=DTU_ADDRESS&0xff;
                time_buf[1]=(DTU_ADDRESS>>8)&0xff;
                time_buf[2]	=0XEE;
                time_buf[3]	=0XEE;
                time_buf[4]	=0XEE;
                time_buf[5]	=0XEE;
                time_buf[6]	=0XEE;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x0A;

                USART_printf(&huart3,tx_buf2); //������DTU��ַ��



                //-------IP--------


                time_buf[0]=MY_IP[0];
                time_buf[1]=MY_IP[1];
                time_buf[2]=MY_IP[2];
                time_buf[3]=MY_IP[3];
                time_buf[4]=0xAA;
                time_buf[5]=0xAA;
                time_buf[6]=0xAA;

                tx_buf2[9]=0x08;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //����Уʱ����ȷ��֡

                //-----port-------
                time_buf[0]=MY_PORT&0x00ff;
                time_buf[1]=(MY_PORT>>8)&0x00ff;
                time_buf[2]=0xBB;
                time_buf[3]=0xBB;
                time_buf[4]=0xBB;
                time_buf[5]=0xBB;
                time_buf[6]=0xBB;

                tx_buf2[9]=0x09;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //����PORT

                //-----Hearttime-------
                time_buf[0]=Transmint_to_GPRS_hearttime&0x00ff;
                time_buf[1]=(Transmint_to_GPRS_hearttime>>8)&0x00ff;
                time_buf[2]=0xCC;
                time_buf[3]=0xCC;
                time_buf[4]=0xCC;
                time_buf[5]=0xCC;
                time_buf[6]=0xCC;

                tx_buf2[9]=0x0C;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //����PORT
                //
                //-----Cyce time-------
                time_buf[0]=Transmint_to_GPRS_Cycledata&0x00ff;
                time_buf[1]=(Transmint_to_GPRS_Cycledata>>8)&0x00ff;
                time_buf[2]=0xDD;
                time_buf[3]=0xDD;
                time_buf[4]=0xDD;
                time_buf[5]=0xDD;
                time_buf[6]=0xDD;

                tx_buf2[9]=0x0D;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //����PORT
                //

            }
            //else status68=0;
        }






        USART3_FRAME_status=0;

        //�򿪶�ʱ��4
        //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
        __HAL_RCC_TIM4_CLK_ENABLE();
    }
}

/*
У���ּ��
*/

uint8_t wdz_101char_check(uint8_t *buffer)
{
    uint16_t k=0;
    uint8_t status68=0;
    uint8_t temp=0;
    uint8_t check_char=0x00;

    if(buffer[0]==0x10)
    {
        status68=1;
    }
    else if(buffer[0]==0x68)
    {
        status68=2;
    }

    if(status68==1) //�̶�����У��λ���
    {
        check_char=buffer[1]+buffer[2]+buffer[3];
        if(check_char==buffer[4])
            temp=1;
        else temp=0;


    }
    else if(status68==2) //�ǹ̶�����У��λ���
    {
        for(k=0; k<buffer[1]; k++)
        {
            check_char=check_char+buffer[k+4];
        }

        if(check_char==buffer[buffer[1]+4])
            temp=1;
        else temp=0;
    }
    temp=1;
    return temp;


}

/*
����У����
*/

void wdz_101check_generate(uint8_t *buffer)
{
    uint16_t k=0;
    uint8_t status68=0;

    uint8_t check_char=0x00;

    if(buffer[0]==0x10)
    {
        status68=1;
    }
    else if(buffer[0]==0x68)
    {
        status68=2;
    }


    if(status68==1) //�̶�����У��λ���
    {
        check_char=buffer[1]+buffer[2]+buffer[3];
        buffer[4]=check_char;
    }
    else if(status68==2) //�ǹ̶�����У��λ���
    {
        for(k=0; k<buffer[1]; k++)
        {
            check_char=check_char+buffer[k+4];
        }
        buffer[buffer[1]+4]=check_char;
    }


}

//***********WDZ-USART4 433ģ��������ݴ�������
/*
�����ַ������Ƶ�������
*/

void wdz_string_to_array(uint8_t *my_string,uint8_t *txbuf)
{
    uint32_t k=0;
    uint32_t length=0;
    if(my_string[0]==0x10)
    {
        length=6;
    }
    else if(my_string[0]==0x68)
    {
        length=my_string[1]+6;
    }


    for(k=0; k<length; k++)
    {
        txbuf[k]=my_string[k];
    }
}

uint8_t  WDZ_101FCB=0x00; //0x20

/*
���ܣ������������յ������ݣ���Ҫ�Ǳ������ݴ������Ǳ������ݻظ�OK֡���������ݰ�
�����������������ȷ���ʱ�����ݰ���Ȼ����ʱ�����ݰ�����ʱ�����ݰ����ܴ��ڶ������ݣ�����������������ʱ�����ݰ�
*/

extern uint8_t MY_433_Alarmdata_NOtimeBuf[]; //�洢 ��ʱ�� ��������
extern uint8_t MY_433_Alarmdata_TimeBuf[];  //�洢����ʱ�꣬��������
extern uint8_t MY_433_ALarmdata_number;  // �洢��������Ϣ�����


extern	uint8_t MY_433_ALarmdata_NOtime_status; //Ϊ1����ʾ�յ���ʱ�걨������
extern	uint8_t MY_433_ALarmdata_Time_status;   //Ϊ1����ʾ�յ���ʱ�걨������



extern uint8_t MY_433_Alarmdata_NOtime_WriteAddress;

void my_process_resive_101usart4(void)
{

    uint8_t status68=0;  //2�����ǹ̶�����֡��1�����̶�����6��֡��0��������������
    uint16_t my_dtu_address=0;
    uint8_t temp=0;
    uint8_t *rsbuf=USART4_my_frame;
    uint8_t mess_number=0;
    uint8_t k=0;
    uint8_t first_address=0;
    uint8_t rs_temp_buf[512]= {0};
    uint8_t my_status=0;
    int my_index=0;

    //my_test_alarm_data();  //����ʹ�ã������˹��������ݣ�@@@@@

    //���յ���ָ����з���������������������д����������㱨��,�����������������ϡ������趨�� ����վ���������͵ĳ���
    //������������ڴ����������ͣ��ȴ�Ӧ��ʱ���˳��������з���֡�������ɷ�����������Լ����в�ѯ����ֹ��֡�ĳ�ͻ����
    if(USART4_FRAME_status>0)
    {
        //---���ԣ�16����ת�ַ���������ַ��� 68-0D-0D-68-73-01-00-09-01-03-01-00-08-40-00-00-00-CA-16
        HexToStr(rs_temp_buf,rsbuf,0);

        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");
        //------------------------------------------

        //֡ͷ���
        if(rsbuf[0]==0x68)
        {
            status68=2;
        }

        else if(rsbuf[0]==0x10)
        {
            status68=1;
        }

        //************0x10�̶�����֡����
        //վ��ַ���
        if(status68==1)
        {
            my_dtu_address=rsbuf[3];
            my_dtu_address=(my_dtu_address<<8)+rsbuf[2];
            if(my_dtu_address==0x0001)  //DTU_ADDRESS)
                status68=1;
            else status68=0;
        }
        //У���ֽڼ��
        if(status68==1)
        {
            temp=wdz_101char_check(rsbuf);  //����ʱ���Կ����Ȳ�ʹ��
            if(temp==1) status68=1;
            else status68=0;
        }



        //֡���ͼ�顢����ԭ���飬�̶����ȱ���֡���
        if(status68==1)
        {
            WDZ_101Transmit_OKdata();//���յ��ķǱ������ݽ��ж�����ֻ�ظ�һ��OK֡
            //���ԣ��ѽ��յ���֡ת������3
            //USART_printf(&huart3,"1111");
            //USART_printf(&huart3,rsbuf);
            //USART_printf(&huart3,"1111");
            USART_printf(&huart3,"433-1 No Alarmdata_10h \r\n"); //����ʹ��
        }



        //************0x68�ǹ̶�����֡���� 68-0B-0B-68-73-01-00-01-01-03-01-00-05-00-01-80-16
        //��վ��ַ���
        my_status=0;
        if(status68==2)
        {
            my_dtu_address=rsbuf[6];
            my_dtu_address=(my_dtu_address<<8)+rsbuf[5];
            if(my_dtu_address==0x0001)//DTU_ADDRESS)
            {
                my_status=2;
                //USART_printf(&huart3,"address OK\r\n"); //����ʹ��

            }
            else
            {
                my_status=0;
                USART_printf(&huart3,"433 address error\r\n"); //����ʹ��
            }
        }
        //У���ֽڼ��

        if(my_status==2)
        {
            temp=wdz_101char_check(rsbuf);
            if(temp==1)
            {
                //USART_printf(&huart3,"char check OK\r\n"); //����ʹ��
                my_status=2;
            }
            else
            {
                my_status=0;
                USART_printf(&huart3,"433 char check error\r\n");
            }

        }


//*******************��Ҫ****************
        //�������53H/73H��֡���ͼ��01H����ʱ�꣩/2EH����ʱ�꣩������ԭ����03H���ǹ̶����ȱ���֡����
        if(my_status==2)
        {
            //*********ң�����ݴ���  68-0D-0D-68-53-01-00-09-01-03-01-00-01-40-11-00-00-B4-16
            if((rsbuf[4]&0x0f)==0x03 &&rsbuf[7]==0x01&&rsbuf[9]==0x03)  //ң�ţ�ͻ������ʱ�꣬����  ������Ϣ(ͻ��)68 0B 0B 68 73 01 00 01 01 03 00 01 08 00 01 83 16
            {
                mess_number=rsbuf[8]&0x7f; //��Ϣ������


                first_address=MY_433_Alarmdata_NOtime_WriteAddress;  //ȡ�����п����ݵ�Ԫ�׵�ַ

                for(k=0; k<mess_number*3; k++) //ÿ����Ϣ��ռ3���ֽڣ���ַ2����ֵһ��
                {
                    MY_433_Alarmdata_NOtimeBuf[k+first_address]=rsbuf[12+k];
                }
                first_address=first_address+k;
                MY_433_Alarmdata_NOtime_WriteAddress=first_address;  //�ƶ���ַ�������������ݵ�Ԫ�׵�ַ��Ŀ������������ѱ��α����Ķ����Ϣ�����ݴ���һ��

                WDZ_101Transmit_OKdata();  //����ȷ��ָ��

                //������EEPROM�У�ֻ���б�ʶ�� �� ��ʱ�걨�����ݲ���
                MY_433_ALarmdata_NOtime_status=1;

                USART_printf(&huart3," 433 Alarmdata NO_time\r\n"); //����ʹ��


            }



            else if((rsbuf[4]&0x0f)==0x03 &&rsbuf[7]==0x1E&&rsbuf[9]==0x03)  //ң�ţ�ͻ������ʱ�꣬���� ����ʱ��ĵ�����Ϣ(ͻ��) 68 1C 1C 68 73 01 00 1E 02 03 01 00 08 00 01 20 BA 16 0F 05 07 0D 05 00 01 49 BC 16 0F 05 07 0D 02 16
            {
                mess_number=rsbuf[8]&0x7f; //��Ϣ������
                MY_433_ALarmdata_number=mess_number;
                //MY_433_Alarmdata_TimeBuf[0]=mess_number;

                for(k=0; k<mess_number*(3+7); k++) //ÿ����Ϣ��ռ10���ֽڣ���ַ2����ֵһ����ʱ��7��
                {
                    MY_433_Alarmdata_TimeBuf[k]=rsbuf[12+k];
                }
                WDZ_101Transmit_OKdata();   //����ȷ��ָ��




                //��ʶΪ�� �д�ʱ�걨������ �������ȴ�GPS�������ݴ���
                MY_433_ALarmdata_Time_status=1;
                MY_433_Alarmdata_NOtime_WriteAddress=0;

                //���뵽EEPROM�У���ʶΪδ����GPRS����----��Ҫ--------
                my_alarm_data_to_eeprom();

                USART_printf(&huart3," 433 Alarmdata with time\r\n"); //����ʹ��

            }
            //***********ң�����ݴ���������ԭ����ң������Ӧ��û�б���68-0D-0D-68-53-01-00-09-01-03-01-00-01-40-11-00-00-B4-16

            else if((rsbuf[4]&0x0f)==0x03 &&rsbuf[7]==0x09&&rsbuf[9]==0x03)  //ң�⣬ͻ������ʱ�꣬����  ������Ϣ(ͻ��)68 0B 0B 68 73 01 00 01 01 03 00 01 08 00 01 83 16
            {



                WDZ_101Transmit_OKdata();   //����ȷ��ָ��,�Ǳ������ݣ�ֻ�ظ�OK֡���������ݣ�2016-4-6�����޸ģ��Ѵ���������GPRS����ת��
                USART_printf(&huart3," 433 No Alarmdata_68h-1 analog data1\r\n"); //����ʹ��

                my_index=rsbuf[12];
                my_index=(my_index-1)*3;
                //�ж�ԭ��ֻҪ���βɼ������ݣ����Ѿ��洢�����ݲ�һ�����ͽ���ת��������ת����ʱ��������ڿ��Ƶķ�ʽ����TIM4�д�������
                if(my_433_anlag_buf[my_index+0]!=rsbuf[14]||my_433_anlag_buf[my_index+1]!=rsbuf[15]||my_433_anlag_buf[my_index+2]!=rsbuf[16] )
                {
                    my_433_anlag_buf[my_index+0]=rsbuf[14];
                    my_433_anlag_buf[my_index+1]=rsbuf[15];
                    my_433_anlag_buf[my_index+2]=rsbuf[16];
                    my_433_anlag_flag=1;   //1��ʾ�������ݲ���������������㡣
                }




            }

            else if((rsbuf[4]&0x0f)==0x03 &&rsbuf[7]==0x22&&rsbuf[9]==0x03)  //ң�⣬ͻ������ʱ�꣬���� ����ʱ��ĵ�����Ϣ(ͻ��) 68 1C 1C 68 73 01 00 1E 02 03 01 00 08 00 01 20 BA 16 0F 05 07 0D 05 00 01 49 BC 16 0F 05 07 0D 02 16
            {

                WDZ_101Transmit_OKdata();   //����ȷ��ָ��,�Ǳ������ݣ�ֻ�ظ�OK֡����������
                USART_printf(&huart3,"433  No Alarmdata_68h-2 analog data2\r\n"); //����ʹ��
            }

            else
            {
                WDZ_101Transmit_OKdata();   //����ȷ��ָ��,�Ǳ������ݣ�ֻ�ظ�OK֡����������
                USART_printf(&huart3,"433 No Alarmdata_68h-3\r\n"); //����ʹ��

                //---���ԣ�16����ת�ַ���������ַ���
                //HexToStr(rs_temp_buf,USART4_my_frame,0);
                //USART_printf(&huart3,rs_temp_buf);
                //USART_printf(&huart3,"\r\n");
                //----
            }


            //���ԣ��ѽ��յ���֡ת������3
            //USART_printf(&huart3,"2222");
            //USART_printf(&huart3,rsbuf);
            //USART_printf(&huart3,"2222");
        }
        //********************�ǹ̶�����֡��������

        //֡�Ĵ������̽���
        USART4_FRAME_status=0;
        //���ԣ��ѷǱ���֡��ʾ����

        //USART_printf(&huart3,"7777");
        //USART_printf(&huart3,rsbuf);
        //USART_printf(&huart3,"2227");

    }

}



/*
���ܣ��������ݿ϶�ȷ�� �жϣ��յ�ȷ�ϰ�������1��û�յ�����0

10 8B 01 00 8C 16(�ɵģ�����) 10 80 01 00 81 16
10 00 01 00 01 16���µģ�
*/
uint8_t WDZ_101recive_OKdata(void)
{
    uint8_t temp=0;
    uint8_t status68=0;  //2�����ǹ̶�����֡��1�����̶�����6��֡��0��������������
    uint16_t my_dtu_address=0;
    uint8_t *my_rsbuf=USART4_my_frame;

    my_usart_101frame(4);
    if(USART4_FRAME_status>0)
    {
        //֡ͷ���
        if(USART4_my_frame[0]==0x68)
        {
            status68=2;
        }

        else if(USART4_my_frame[0]==0x10)
        {
            status68=1;
        }


//************0x10�̶�����֡����
        //վ��ַ���
        if(status68==1)
        {
            my_dtu_address=USART4_my_frame[3];
            my_dtu_address=(my_dtu_address<<8)+USART4_my_frame[2];
            if(my_dtu_address==0x0001)//DTU_ADDRESS)
                status68=1;
            else status68=0;
        }
        //У���ֽڼ��
        if(status68==1)
        {
            temp=wdz_101char_check(USART4_my_frame);  //
            if(temp==1) status68=1;
            else status68=0;
        }



        //֡���ͼ�顢����ԭ����
        if(status68==1)
        {
            //if((my_rsbuf[1]&0x0F)==0x00)
            if((my_rsbuf[1])==0x00)
            {
                temp=1;
                status68=0;

            }
            else
            {   temp=0;
                status68=0;

            }
        }

//************0x68�ǹ̶�����֡����
        //��վ��ַ���
        if(status68==2)
        {
            /*
            my_dtu_address=USART4_my_frame[6];
            my_dtu_address=(my_dtu_address<<8)+USART4_my_frame[5];
            if(my_dtu_address==DTU_ADDRESS)
            			status68=2;
            else status68=0;
            */
            USART4_FRAME_status=0;
        }
        //У���ֽڼ��
        /*
        	if(status68==2)
        	{
        	temp=wdz_101char_check(USART4_my_frame);  //����ʱ���Կ����Ȳ�ʹ��
        	if(temp==1) status68=2;
        	else status68=0;
        	}



        		//֡���ͼ�顢����ԭ����
        		if(status68==2)
        		{
        			//���ԣ��ѽ��յ���֡ת������3
        			USART_printf(&huart3,"33333");
        			USART_printf(&huart3,USART4_my_frame);
        			USART_printf(&huart3,"3333");
        		}
        		*/
//********************�ǹ̶�����֡��������

        //֡�Ĵ������̽���
        USART4_FRAME_status=0;

    }



    return temp;
}



/*

���ܣ�ͨ���ͷ�������ȴ��϶�ȷ�Ϻ�������������ָ��յ��϶�ȷ��ָ���1��û�յ�ȷ�Ϸ���0
��ڲ�����typeΪ���ͣ�1Ϊ�̶����ȣ�2Ϊ�ǹ̶����ȣ�txbufΪ����ָ������
					address_typeΪ1����Ӧ�õ�ַ���ȷ����ֽڣ��󷢸��ֽڣ�����Ĭ�Ϲ���
									 ���Ϊ2�����ȷ����ֽڣ��󷢵��ֽڣ���Щָ��ֻ�������û�취

*/



uint8_t WDZ_101transmint_commd(uint8_t type,uint8_t *txbuf,uint8_t address_type)  //typeΪ���ͣ�1Ϊ�̶����ȣ�2Ϊ�ǹ̶����ȣ�txbufΪ����ָ������
{
    uint8_t FCB=0x20;
    uint16_t my_dtu_address=0x0001; //  DTU_ADDRESS;
    uint8_t my_status=0;
    uint8_t n=3;
    uint8_t *my_txbuf=txbuf;
    uint16_t my_start_time=0;

    if(type==2)
    {
        //�������봦��
        if(WDZ_101FCB==0x00)
            my_txbuf[4]=my_txbuf[4]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[4]=my_txbuf[4]|(FCB);
        //Ŀ�ĵ�ַ����
        my_txbuf[5]=my_dtu_address&0xFF;
        my_txbuf[6]=my_dtu_address>>8;


        if(address_type==1)
        {
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }
        else if(address_type==2)
        {
            //my_txbuf[10]=my_txbuf[6]; //����ָ����ȴ��ظ�
            //my_txbuf[11]=my_txbuf[5];
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }

    }
    else if(type==1)
    {
        //�������봦��
        if(WDZ_101FCB==0x00)
            my_txbuf[1]=my_txbuf[1]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[1]=my_txbuf[1]|(FCB);
        //Ŀ�ĵ�ַ����

        my_txbuf[5]=my_dtu_address&0xFF;
        my_txbuf[6]=my_dtu_address>>8;



    }


    //У��λ����
    wdz_101check_generate(my_txbuf);
    //����ָ��

    while((n--)>0&&my_status==0)
    {
        my_UART4_printf(&huart4,my_txbuf);

        my_start_time=my_tim6_count;
        do {

            my_status=WDZ_101recive_OKdata();
            if(my_status==1)break;
            else my_status=0;
        } while(my_tim6_count-my_start_time<=3);

    }
    WDZ_101FCB=WDZ_101FCB^0x20;
    if(my_status==1)
    {
        return 1;
    }
    else return 0;

}

/*
���ܣ���������ȴ����ض�����������
*/
uint8_t WDZ_101transmint_commd_wait_commd(uint8_t type,uint8_t *txbuf,uint8_t address_type,uint8_t *rxbuf)  //typeΪ���ͣ�1Ϊ�̶����ȣ�2Ϊ�ǹ̶����ȣ�txbufΪ����ָ������
{
    uint8_t FCB=0x20;
    uint16_t my_dtu_address=0x0001;  //DTU_ADDRESS;
    uint8_t my_status=0;
    uint8_t n=3;
    uint8_t *my_txbuf=txbuf;
//	uint8_t temp=0;

    uint8_t linkcontrol=0;
    uint8_t type_identification=0;
    uint8_t  transmit_reason=0;
    uint16_t my_start_time=0;
//********************���ͷǹ̶�����֡��Ӧ��ΪOK֡
    if(type==2 && address_type==1)
    {
        //�������봦��
        if(WDZ_101FCB==0x00)
            my_txbuf[4]=my_txbuf[4]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[4]=my_txbuf[4]|(FCB);
        //Ŀ�ĵ�ַ����
        my_txbuf[5]=my_dtu_address&0xFF;
        my_txbuf[6]=my_dtu_address>>8;



        if(address_type==1)
        {
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }
        else if(address_type==2)
        {
            //my_txbuf[10]=my_txbuf[6];  //����ָ��ȴ��ظ�
            // my_txbuf[11]=my_txbuf[5];
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }

        //У��λ����
        wdz_101check_generate(my_txbuf);
        //����ָ��
        while((n--)>0&&my_status==0)
        {
            my_UART4_printf(&huart4,my_txbuf);

            //Delay_us(300); //����ʹ��

            my_start_time=my_tim6_count;
            do {

                my_status=WDZ_101recive_OKdata();  //�ȴ�0Kȷ�Ϸ�������
                if(my_status==1)break;
                else my_status=0;
            } while(my_tim6_count-my_start_time<=4);

        }



    }
//********************���ͷǹ̶�����֡��Ӧ��ΪOK֡,��Է���ָ����ƣ����ͺ�ȴ�1��
    if(type==2 && address_type==2)
    {
        //�������봦��
        if(WDZ_101FCB==0x00)
            my_txbuf[4]=my_txbuf[4]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[4]=my_txbuf[4]|(FCB);
        //Ŀ�ĵ�ַ����
        my_txbuf[5]=my_dtu_address&0xFF;
        my_txbuf[6]=my_dtu_address>>8;



        if(address_type==1)
        {
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }
        else if(address_type==2)
        {
            //my_txbuf[10]=my_txbuf[6];  //����ָ��ȴ��ظ�
            // my_txbuf[11]=my_txbuf[5];
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }

        //У��λ����
        wdz_101check_generate(my_txbuf);
        //�������յĿ������룬���ͣ�����ԭ��
        linkcontrol=rxbuf[4];
        type_identification=rxbuf[7];
        transmit_reason=rxbuf[9];
        //����ָ��
        n=1;
        while(my_status==0 && (n--)>0)
        {
            Delay_ms(3);
            my_UART4_printf(&huart4,my_txbuf);  //�ؼ����

            Delay_ms(15); //���ԣ��ȴ�1��

            //my_status=WDZ_101receive_testactive(linkcontrol,type_identification,transmit_reason,600);  //�ȴ���������

            my_status=1;
            if(my_status==1)
                break;
            else
                my_status=0;


        }



    }

//**************���͹̶�����֡��Ӧ��Ϊ�̶�����֡
    else	if(type==1&&rxbuf[0]==0x10)
    {
        //�������յĿ������룬���ͣ�����ԭ��
        linkcontrol=rxbuf[1];
        type_identification=0;
        transmit_reason=0;

        //�������봦��
        if(WDZ_101FCB==0x00)
            my_txbuf[1]=my_txbuf[1]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[1]=my_txbuf[1]|(FCB);
        //Ŀ�ĵ�ַ����
        my_txbuf[2]=my_dtu_address&0xFF;
        my_txbuf[3]=my_dtu_address>>8;

        //У��λ����
        wdz_101check_generate(my_txbuf);
        //����ָ��
        while((n--)>0&&my_status==0)
        {
            my_UART4_printf(&huart4,my_txbuf);

            //Delay_us(300); //����

            my_status=WDZ_101receive_testactive(linkcontrol,type_identification,transmit_reason,600);  //�ȴ���������
            if(my_status==1)break;
            else my_status=0;
        }
    }
//**********���͹̶�����֡��Ӧ��Ϊ�ǹ̶�����֡
    else if(type==1&&rxbuf[0]==0x68)
    {
        //�������յĿ������룬���ͣ�����ԭ��
        linkcontrol=rxbuf[4];
        type_identification=rxbuf[7];
        transmit_reason=rxbuf[9];

        //�������봦��
        if(WDZ_101FCB==0x00)
            my_txbuf[1]=my_txbuf[1]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[1]=my_txbuf[1]|(FCB);
        //Ŀ�ĵ�ַ����
        my_txbuf[2]=my_dtu_address&0xFF;
        my_txbuf[3]=my_dtu_address>>8;

        //У��λ����
        wdz_101check_generate(my_txbuf);
        //����ָ��
        while((n--)>0&&my_status==0)
        {
            my_UART4_printf(&huart4,my_txbuf);

            //Delay_us(600); //����

            my_status=WDZ_101receive_testactive(linkcontrol,type_identification,transmit_reason,600);  //�ȴ���������
            if(my_status==1)break;
            else my_status=0;
        }
    }
//��������

    /*
    if(type==1)
    {

    	//У��λ����
    	wdz_101check_generate(my_txbuf);
    	//����ָ��
    	while((n--)>0&&my_status==0)
    	{
    		my_UART4_printf(UART4,my_txbuf);

    		Delay_us(300); //����

    			my_status=WDZ_101receive_testactive(linkcontrol,type_identification,transmit_reason,300);  //�ȴ���������
    			if(my_status==1)break;
    			else my_status=0;
    	}
    }
    */



    //��¼���ͱ仯֡
    WDZ_101FCB=WDZ_101FCB^0x20;
    if(my_status==1)
    {
        return 1;
    }
    else return 0;

}

/*
����OK�϶�ȷ��ָ��

*/

void WDZ_101Transmit_OKdata(void)
{
    uint8_t *commd=TX101_OKdata;
    my_UART4_printf(&huart4,commd);
}




/*
���ܣ��������������ȴ�Ӧ�����

������������10 D2 01 00 D3 16 ���� 10 F2 01 00 F3 16
����ȷ�ϰ�  10 80 01 00 81 16 ���� 10 82 01 00 83 16
*/


extern uint8_t wdz_433_heart_count;
extern uint8_t link_status_433;
uint8_t WDZ_101transmit_heartdata(void)
{

//	uint8_t FCB=0x20;
//	uint16_t my_dtu_address=DTU_ADDRESS;
    uint8_t my_status=0;
    uint8_t *my_commd;
    uint8_t my_rxbuf[256]="\x10\x00\x01\x00\x01\x16";
    //uint8_t n=3;
    uint8_t my_txbuf[7]=TX101_heartdata;

    //my_status=WDZ_101transmint_commd2(1,my_txbuf,1,1);

    my_commd=TX101_heartdata;  //"\x10\xC0\x01\x00\xC1\x16";
    wdz_string_to_array(my_commd,my_txbuf);

    my_commd="\x10\x00\x01\x00\x01\x16";
    wdz_string_to_array(my_commd,my_rxbuf);
    WDZ_101FCB=0X00;
    my_status=WDZ_101transmint_commd_wait_commd(1,my_txbuf,1,my_rxbuf);



    //----����433ָ��������⣬�ȷ���OK�����������Ȼ���ڷ�������
    if(my_status==0)
    {
        //WDZ_101Transmit_OKdata(); //����OK֡
        //my_status=WDZ_101transmint_commd(1,my_txbuf,1); //��2�η���������

    }

    //----

    if(my_status==0)
    {
        USART_printf(&huart3,"433 heart-\r\n");
    }
    else if(my_status==1)
    {
        USART_printf(&huart3," 433 heart*\r\n");
        wdz_433_heart_count=0;
    }

    if(my_status==0) wdz_433_heart_count++; //�������ظ�������һ��

    if(wdz_433_heart_count>=3)
    {
        link_status_433=0; //��¼3���������޻ظ����ͶϿ����ӣ����½�������
        wdz_433_heart_count=0; //�������ϼ�������
    }

    return my_status;

}
//*************���Ͳ�������*************

/*
68 0C 0C 68 73 01 00 68 01 06 00 01 00 00 AA 55 E3 16��������(����)
68 0C 0C 68 F3 01 00 68 01 06 01 00 00 00 AA 55 63 16
���϶�ȷ�ϣ�10 80 01 00 81 16
						10 00 01 00 01 16
��������(����)	68 0D 0D 68 53 01 00 68 01 2E 00 01 00 00 00 55 E3 24 16��
������ԭ��2E����ʾӦ�÷����ַδ֪�������ԭ���Ƿ���������01 00��

							68 0C 0C 68 73 01 00 68 01 87 01 00 00 00 AA 55 64 16
10 00 01 00 01 16�϶�ȷ��
10 80 01 00 81 16
*/



uint8_t WDZ_101transmit_testdata(void)
{
    uint8_t my_status=0;
    uint8_t my_txbuf[19]=TX101_testdata;

    my_status=WDZ_101transmint_commd(2,my_txbuf,2);
    if(my_status==1)
    {
        my_status=WDZ_101receive_testactive(0x03,0x68,0x07,300);

    }
    else if(my_status==0)
    {   //Delay_us(100);
        USART_printf(&huart3,"433 test!\r\n");
        //LED3_TOGGLE;
    }


    if(my_status==1)
    {
        WDZ_101Transmit_OKdata();

        //Delay_us(100);
        USART_printf(&huart3,"433 test*\r\n");

        // LED2_TOGGLE;
    }
    else if(my_status==0)
    {   //Delay_us(100);
        USART_printf(&huart3,"433 test-\r\n");
        //LED3_TOGGLE;
    }
    return my_status;

}

/*
���ܣ�����Ӧ��֡
���������Link_control��������룬type_identification-֡���ͣ�transmit_reason-����ԭ��time-���ʱ��
��������Ӧ����ȷΪ1��Ӧ������û��Ӧ��Ϊ0
*/

uint8_t WDZ_101receive_testactive(uint8_t Link_control,uint8_t type_identification,uint8_t transmit_reason,uint16_t time)
{   uint8_t temp=0;
    uint8_t *rsbuf=USART4_my_frame;

    uint8_t status68=0;  //2�����ǹ̶�����֡��1�����̶�����6��֡��0��������������
    uint16_t my_dtu_address=0;
    uint16_t my_start_time=0;


    my_start_time=my_tim6_count;
    while(my_tim6_count-my_start_time<=3)
    {   my_usart_101frame(4);
        if(USART4_FRAME_status>0)
        {
            //֡ͷ���
            if(rsbuf[0]==0x68)
            {
                status68=2;
            }

            else if(rsbuf[0]==0x10)
            {
                status68=1;
            }

//************0x10�̶�����֡����
            //վ��ַ���
            if(status68==1)
            {
                my_dtu_address=rsbuf[3];
                my_dtu_address=(my_dtu_address<<8)+rsbuf[2];
                if(my_dtu_address==0x0001) //DTU_ADDRESS
                    status68=1;
                else status68=0;
            }
            //У���ֽڼ��
            if(status68==1)
            {
                temp=wdz_101char_check(rsbuf);  //����ʱ���Կ����Ȳ�ʹ��
                temp=1;  //����У����@@@@@
                if(temp==1) status68=1;
                else status68=0;
            }



            //֡���ͼ�顢����ԭ����
            if(status68==1)
            {

                if((rsbuf[1]&0x0f)==(Link_control&0x0f))
                {
                    temp=1;
                    //֡�Ĵ������̽���
                    USART4_FRAME_status=0;
                    break;
                }
                else
                {   temp=0;
                    //���ԣ��ѽ��յ���֡ת������3
                    //USART_printf(&huart3,"4444");
                    //USART_printf(&huart3,USART4_my_frame);
                    //USART_printf(&huart3,"444444");
                    //֡�Ĵ������̽���
                    USART4_FRAME_status=0;
                }

                //֡�Ĵ������̽���
                USART4_FRAME_status=0;
            }



//************0x68�ǹ̶�����֡����
            //��վ��ַ���
            if(status68==2)
            {
                my_dtu_address=rsbuf[6];
                my_dtu_address=(my_dtu_address<<8)+rsbuf[5];
                if(my_dtu_address==0x0001) //DTU_ADDRESS)
                    status68=2;
                else status68=0;
            }
            //У���ֽڼ��

            if(status68==2)
            {
                temp=wdz_101char_check(rsbuf);  //����ʱ���Կ����Ȳ�ʹ��
                temp=1;  //����У����@@@@@
                if(temp==1) status68=2;
                else status68=0;
            }



            //֡���ͼ�顢����ԭ���� 68 0D 0D 68 53 01 00 68 01 07 00 01 00 00 00 55 E3 FD 16
            if(status68==2)
            {
                if(((rsbuf[4]&0x0f)==(Link_control&0x0f))&&(rsbuf[7]==type_identification)&&(rsbuf[9]==transmit_reason))
                {
                    temp=1;
                    //֡�Ĵ������̽���
                    USART4_FRAME_status=0;
                    break;
                }
                else
                {   temp=0;
                    //���ԣ��ѽ��յ���֡ת������3
                    //USART_printf(&huart3,"666666");
                    //USART_printf(&huart3,USART4_my_frame);
                    //USART_printf(&huart3,"6666");
                    //֡�Ĵ������̽���
                    USART4_FRAME_status=0;
                }
            }
            //********************�ǹ̶�����֡��������

            //֡�Ĵ������̽���
            USART4_FRAME_status=0;

        }


    }

    return temp;

}

/*
68 0B 0B 68 73 01 00 69 01 06 01 00 00 00 01 E6 16 ��λ��������(����)
			�϶�ȷ��10 80 01 00 81 16
			��λ��������(����ȷ��) 	68 0B 0B 68 53 01 00 69 01 07 01 00 00 00 01 C7 16
														68 0B 0B 68 73 01 00 69 01 2E 01 00 00 00 02 0F 16
														68 0B 0B 68 73 01 00 69 01 07 01 00 00 00 02 E8 16
10 00 01 00 01 16�϶�ȷ��


���ܣ���λ��������
*/

int MY_433_Link_count=0;

uint8_t WDZ_101transmit_Resetdata(void)
{
    uint8_t my_status=0;
    uint8_t my_txbuf[18]=TX101_restdata;
    //---------�ϵ��ж�-----
    MY_433_Link_count++;
    if(MY_433_Link_count%5==0 && MY_433_Link_count!=0)  //Ӳ��������POWER�ϵ�����433ģ��
    {
        WDZ_MCUtransmit_433restdata();
    }
    else if(MY_433_Link_count>60)  //Ӳ������ϵͳ�ϵ�����,5��Сʱ��
    {
        WDZ_MCUtransmit_restdata();
    }
    //����3��ok֡����ֹӵ��
    wdz_sendOKfrma(3);

    my_status=WDZ_101transmint_commd(2,my_txbuf,1);
    if(my_status==1)
        my_status=WDZ_101receive_testactive(0x03,0x69,0x07,300);
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 rest error1\r\n");
        //LED3_TOGGLE;
    }


    if(my_status==1) //���Ϳ϶�ȷ������
    {   my_UART4_printf(&huart4,TX101_OKdata);

        //Delay_us(100);
        USART_printf(&huart3,"433 rest ok*\r\n");

        // LED2_TOGGLE;
    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 rest error end-\r\n");
        //LED3_TOGGLE;
    }

    //����3��ok֡����ֹӵ��
    wdz_sendOKfrma(2);
    return my_status;
}



/*���ܣ�(1)---��·��ʼ������
10 C9 01 00 CA 16(������·FCB���ܱ䣬Ϊ0)
				����Ӧ��·״̬��10 0B 01 00 0C 16  ��ָʾ��DIRΪ1��
10 C0 01 00 C1 16 ����λԶ����·FCB���ܱ䣩����
				���϶�ȷ�ϣ�10 80 01 00 81 16
				��������·��10 C9 01 00 CA 16
10 8B 01 00 8C 16����Ӧ��·״̬FCB���ܱ䣩
			  ����λԶ����·��10 40 01 00 41 16
10 80 01 00 81 16���϶�ȷ��FCB���ܱ䣩
			����ʼ�������� 68 0B 0B 68 73 01 00 46 01 04 01 00 00 00 00 C0 16
10 80 01 00 81 16���϶�ȷ��FCB���ܱ䣩
*/


uint8_t WDZ_101Transmit_LinkInit(void)
{

    uint8_t my_status=0;
    uint8_t my_rxbuf[256]="\x10\x0B\x01\x00\x0C\x16";
    uint8_t linkcontrol=0;
    uint8_t type_identification=0;
    uint8_t transmit_reason=0;
    uint8_t *my_commd;
    //uint8_t temp=0;
    uint8_t my_txbuf[256]=TX101_Linkquire_data;




    //����--������·
    //�ӷ�--��Ӧ��·״̬


    WDZ_101FCB=0X00;
    my_status=WDZ_101transmint_commd_wait_commd(1,my_txbuf,1,my_rxbuf);




    //����--��λԶ����·
    //�ӷ�--�϶�ȷ��


    if(my_status==1)
    {
        my_commd=TX101_Linkrest_data;  //"\x10\xC0\x01\x00\xC1\x16";
        wdz_string_to_array(my_commd,my_txbuf);

        my_commd="\x10\x00\x01\x00\x01\x16";
        wdz_string_to_array(my_commd,my_rxbuf);
        WDZ_101FCB=0X00;
        my_status=WDZ_101transmint_commd_wait_commd(1,my_txbuf,1,my_rxbuf);
    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Link1!\r\n ");
        //LED3_TOGGLE;
    }
    //�ӷ�--������·
    if(my_status==1)
    {
        linkcontrol=0x49;
        WDZ_101FCB=0X00;
        my_status=WDZ_101receive_testactive(linkcontrol,type_identification,transmit_reason,600);
    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Link2!\r\n ");
        //LED3_TOGGLE;
    }
    //����--��Ӧ��·״̬
    //�ӷ�--��λԶ����·
    if(my_status==1)
    {

        my_commd=TX101_Linkconfirm_data; //"\x10\x8B\x01\x00\x8C\x16";
        wdz_string_to_array(my_commd,my_txbuf);

        my_commd="\x10\x40\x01\x00\x41\x16";
        wdz_string_to_array(my_commd,my_rxbuf);
        WDZ_101FCB=0X00;
        my_status=WDZ_101transmint_commd_wait_commd(1,my_txbuf,1,my_rxbuf);
    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Link3!\r\n ");
        //LED3_TOGGLE;
    }
    //����--�϶�ȷ��
    //�ӷ�--��ʼ������
    if(my_status==1)
    {
        WDZ_101FCB=0X00;

        my_commd=TX101_OKdata;//"\x10\x80\x01\x00\x81\x16";
        wdz_string_to_array(my_commd,my_txbuf);

        my_commd="\x68\x0B\x0B\x68\x73\x01\x00\x46\x01\x04\x01\x00\x00\x00\x00\xC0\x16";
        wdz_string_to_array(my_commd,my_rxbuf);
        WDZ_101FCB=0X00;
        my_status=WDZ_101transmint_commd_wait_commd(1,my_txbuf,1,my_rxbuf);


    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Link4!\r\n ");
        //LED3_TOGGLE;
    }

    //��DTU--���϶�ȷ��
    if(my_status==1) //���Ϳ϶�ȷ������
    {
        WDZ_101Transmit_OKdata();
        USART_printf(&huart3,"433 Link*\r\n ");

        //
        Delay_us(300);           //����ʹ�ã������Чָ��
        WDZ_101Transmit_OKdata(); //����ʹ�ã������Чָ��

        //LED2_TOGGLE;
    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Link-\r\n ");
        //LED3_TOGGLE;
    }


    //����2��ok֡����ֹӵ��
    wdz_sendOKfrma(2);
    return my_status;

}

/*���ܣ�(2)----��������
68 0B 0B 68 F3 01 00 64 01 06 01 00 00 00 14 74 16�����ٻ����� ���
		�϶�ȷ��10 00 01 00 01 16
		ȷ�ϼ��� 68 0B 0B 68 53 01 00 64 01 07 01 00 00 00 14 D5 16
10 80 01 00 81 16�϶�ȷ��
		��ң�ţ���Ʒ�ʣ�����ʱ�� 68 1C 1C 68 73 01 00 01 92 14 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 1D 16(��18�����ݣ��ӵ�ַ0001��ʼ����18��)
10 80 01 00 81 16�϶�ȷ��
		ң�⣬��Ʒ�ʣ�����ʱ��  68 25 25 68 53 01 00 09 89 14 01 00 01 40 10 00 00 11 00 00 32 00 00 15 00 00 10 00 00 08 00 00 0E 00 00 00 00 00 12 00 00 DC 16 ң�⣬��Ʒ�ʣ�����ʱ��
10 80 01 00 81 16�϶�ȷ��
		������ֹ 68 0B 0B 68 53 01 00 64 01 0A 01 00 00 00 14 D8 16
10 80 01 00 81 16 �϶�ȷ��
������Linktype��ʶ�����������ͣ�1Ϊ��ʼ�������������ݣ�2Ϊ���������������������Ҫ��������
*/


uint8_t WDZ_101transmit_Calldata(uint8_t linktype)   //������Linktype��ʶ�����������ͣ�1Ϊ��ʼ�������������ݣ�2Ϊ���������������������Ҫ��������
{
    uint8_t my_status=0;
    uint8_t k=0;
    uint8_t length=0;
    uint8_t my_txbuf[18]=TX101_calldata;

    if(linktype==1) WDZ_101Transmit_OKdata();//����OK֡ �����ǰ������֡

    //����--����
    //�ӷ�--�϶�ȷ��

    my_status=WDZ_101transmint_commd(2,my_txbuf,1);

    //Delay_us(500);


    //�ӷ�--ȷ�ϼ���
    if(my_status==1)
    {
        my_status=WDZ_101receive_testactive(0x03,0x64,0x07,300);
    }
    else if(my_status==0)  //ʧ�ܴ���
    {

        //Delay_us(100);
        USART_printf(&huart3,"433 call1!\r\n ");
        //LED3_TOGGLE;
    }
    //����-OK
    //�ӷ�--ң��
    if(my_status==1)
    {
        WDZ_101Transmit_OKdata();//����OK֡
        my_status=WDZ_101receive_testactive(0x03,0x01,0x14,1000);
//************��Ҫ****************�����ݴ洢�����黺������

        if(linktype==2)
        {
            // for(k=0;k<256;k++)
            //	MY_433_Call_Single_data_buf[k]=0xff;  //����ʹ��

            length=USART4_my_frame[8]&0x7f;  //��Ϣ������
            length=0x12;  //18����Ϣ�壬ǿ��ֵ@@@@

            MY_433_Call_Single_data_number=length;

            //MY_433_Call_Single_data_buf[0]=length;

            for(k=0; k<length; k++)
            {
                MY_433_Call_Single_data_buf[k]=USART4_my_frame[k+14];  //����ң������
            }
            MY_433_Call_Status=1;// �յ�ң������
        }


    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 call2!\r\n ");
        //LED3_TOGGLE;
        MY_433_Call_Status=0;
    }
    //����-OK֡
    //�ӷ�--ң��
    if(my_status==1)
    {
        WDZ_101Transmit_OKdata();//����OK֡
        my_status=WDZ_101receive_testactive(0x03,0x09,0x14,1000);

//*************��Ҫ******************�����ݴ洢�����ݻ�������

        if(linktype==2)
        {
            //for(k=0;k<256;k++)
            //MY_433_Call_Analog_data_buf[k]=0xff;  //����ʹ��


            length=USART4_my_frame[8]&0x7f;  //��Ϣ������
            length=0x24;  //36����Ϣ�壬ǿ��ֵ@@@@


            MY_433_Call_Analog_data_number=length;
            //MY_433_Call_Analog_data_buf[0]=length;

            for(k=0; k<length*3; k++)
            {
                MY_433_Call_Analog_data_buf[k]=USART4_my_frame[k+14];  //����ң�����ݣ���Ʒ��
            }

            my_RTCtime_to_array(MY_433_Call_Time_data_buf); //����ʱ��

            MY_433_Call_Status=1;
        }



    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 call3!\r\n ");
        //LED3_TOGGLE;
        MY_433_Call_Status=0;
    }
    //����-OK֡
    //�ӷ�--������ֹ
    if(my_status==1)
    {   WDZ_101Transmit_OKdata();//����OK֡
        my_status=WDZ_101receive_testactive(0x03,0x64,0x0A,1000);
    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 call4!\r\n ");
        //LED3_TOGGLE;
    }


    //��DTU--���϶�ȷ��
    if(my_status==1) //���Ϳ϶�ȷ������
    {
        WDZ_101Transmit_OKdata();//����OK֡
        //my_UART4_printf(UART4,TX101_OKdata);
        // Delay_us(100);
        USART_printf(&huart3,"433 call*\r\n");
        if(length==1)
        {
            Delay_us(300);
            WDZ_101Transmit_OKdata();//����OK֡,��������֡
        }

        //LED2_TOGGLE;
    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        link_status_433=0;
        USART_printf(&huart3,"433 call-\r\n ");
        //LED3_TOGGLE;
    }
    //����2��ok֡����ֹӵ��
    wdz_sendOKfrma(2);
    return my_status;
}

/*���ܣ�(3)---���� ��ʱ�������,ʱ��У������
68 0C 0C 68 53 01 00 6A 01 06 01 00 00 00 3D C3 C6 16��ʱ�������(����FCB�ɱ�)
68 0C 0C 68 D3 01 00 6A 01 06 01 00 00 00 3D C3 46 16
			�϶�ȷ��10 80 01 00 81 16
						  10 00 01 00 01 16
			��ʱ�������(����ȷ��) 68 0C 0C 68 73 01 00 6A 01 07 01 00 00 00 5B C3 05 16
													  68 0C 0C 68 73 01 00 6A 01 07 01 00 00 00 43 C3 ED 16
10 00 01 00 01 16�϶�ȷ��
10 80 01 00 81 16
68 0C 0C 68 73 01 00 6A 01 03 01 00 00 00 C4 01 A8 16��ʱ�������(ͻ�� FCB�ɱ�)
68 0C 0C 68 F3 01 00 6A 01 03 01 00 00 00 C4 01 28 16
			�϶�ȷ�� 10 80 01 00 81 16
							 10 00 01 00 01 16
68 12 12 68 53 01 00 67 01 06 00 01 00 00 008B D4 0B 0F 05 07 0E 56 16ʱ��ͬ������(���� FCB�ɱ�) ʱ��Ϊ8���ֽڣ�
68 11 11 68 D3 01 00 67 01 06 01 00 00 00   8B D4 0B 0F A5 07 0D 75 16
			�϶�ȷ�� 10 80 01 00 81 16
							10 00 01 00 01 16
			ʱ��ͬ������(����ȷ��) 68 12 12 68 73 01 00 67 01 07 00 01 00 00 00 B2 9D 0E 0F 05 07 0E 6A 16
													 68 11 11 68 53 01 00 67 01 07 01 00 00 00 AD D3 0B 0F 05 07 0D 77 16
10 00 01 00 01 16�϶�ȷ��FCB���ɱ�
10 80 01 00 81 16


*/

uint8_t WDZ_101Transmit_Delaytimeadjust(void)
{

    uint8_t my_status=0;
    uint8_t my_rxbuf[256]="\x68\x0C\x0C\x68\x73\x01\x00\x6A\x01\x07\x01\x00\x00\x00\x43\xC3\xED\x16";
//	uint8_t linkcontrol=0;
    //uint8_t type_identification=0;
    //uint8_t transmit_reason=0;
    uint8_t *my_commd;
    //uint8_t temp=0;
    uint8_t my_txbuf[256]=TX101_delaytime_data;

    //********��һ��*********
    //����--��ʱ�������
    //�ӷ�--�϶�ȷ��
//WDZ_101FCB=0X00;
    my_status=WDZ_101transmint_commd_wait_commd(2,my_txbuf,1,my_rxbuf);

//�ӷ���ʱ�������(����ȷ��)
    //if(my_status==1)
    //{
    //	my_status=WDZ_101receive_testactive(0x73,0x6A,0x07,600);
    //}


    //********�ڶ���*********
    //����--�϶�ȷ��OK֡
    //����--��ʱ�������(ͻ�� FCB�ɱ�)
    //�ӷ�--�϶�ȷ��
    if(my_status==1)
    {
        WDZ_101Transmit_OKdata(); //����0K֡

        my_commd=TX101_delaytime_burst_data;//������֡
        wdz_string_to_array(my_commd,my_txbuf);

        my_commd="\x10\x00\x01\x00\x01\x16";  //�ӷ��͵�OK֡
        wdz_string_to_array(my_commd,my_rxbuf);//����֡

        //WDZ_101FCB=0X00;
        my_status=WDZ_101transmint_commd_wait_commd(2,my_txbuf,1,my_rxbuf);
    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Time1!\r\n ");
        //LED3_TOGGLE;
    }

    //*********������**********
    //����---ʱ��ͬ������
    //�ӷ�--�϶�ȷ��
    if(my_status==1)
    {
        my_commd=TX101_time_synchronization_data;//����֡
        wdz_string_to_array(my_commd,my_txbuf);

        my_commd="\x68\x11\x11\x68\x53\x01\x00\x67\x01\x07\x01\x00\x00\x00\xAD\xD3\x0B\x0F\x05\x07\x0D\x77\x16";  //�ͷ���OK֡
        wdz_string_to_array(my_commd,my_rxbuf);//����֡

        //��Уʱʱ������޶������͸�433

        my_RTCtime_to_array(rtcbuffer);
        my_txbuf[14]=rtcbuffer[0];
        my_txbuf[15]=rtcbuffer[1];
        my_txbuf[16]=rtcbuffer[2];
        my_txbuf[17]=rtcbuffer[3];
        my_txbuf[18]=rtcbuffer[4];
        my_txbuf[19]=rtcbuffer[5];
        my_txbuf[20]=rtcbuffer[6];
        //WDZ_101FCB=0X00;
        if(my_txbuf[17]==0x10) my_txbuf[17]=my_txbuf[17]+1; //������������

        my_status=WDZ_101transmint_commd_wait_commd(2,my_txbuf,1,my_rxbuf);
    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Time2!\r\n ");
        //LED3_TOGGLE;
    }

    //�ӷ�--ʱ��ͬ������(����ȷ��) 68 11 11 68 53 01 00 67 01 07 01 00 00 00 AD D3 0B 0F 05 07 0D 77 16
    /*
    if(my_status==1)
    {
    	linkcontrol=0x73;
    	type_identification=0x67;
    	transmit_reason=0x07;
    	//WDZ_101FCB=0X00;
    	my_status=WDZ_101receive_testactive(linkcontrol,type_identification,transmit_reason,1000);


    }
    else if(my_status==0)  //ʧ�ܴ���
    {
    	//Delay_us(100);
    	USART_printf(&huart3,"Time3!\r\n ");
    	//LED3_TOGGLE;
    }
    */

    //��DTU--���϶�ȷ��
    if(my_status==1) //���Ϳ϶�ȷ������
    {
        WDZ_101Transmit_OKdata();

        USART_printf(&huart3,"433 Time*\r\n ");
        //Delay_us(400);
        //WDZ_101Transmit_OKdata(); //ÿ��ָ���ظ�����4�Σ�ÿ�μ��1�룬��������һ��OK֡������������·���̣�����ָ������ˡ�
    }
    else if(my_status==0)  //ʧ�ܴ���
    {

        USART_printf(&huart3,"433 Time-\r\n ");
        link_status_433=0; //��·������,��������

    }

    //����2��ok֡����ֹӵ��
    wdz_sendOKfrma(2);
    return my_status;

}


//-----------����ʹ�ó���------------
//��������һ�� ���Ŀ���ַ�������2��16�����ַ�������3��16�����ֽ�����
void HexToStr(uint8_t *pbDest, uint8_t *pbSrc, int nLen)
{
    uint8_t	ddl,ddh;
    int i;

    if(pbSrc[0]==0x10) nLen=6;
    else if(pbSrc[0]==0x68)nLen=6+pbSrc[1];

    for (i=0; i<nLen; i++)
    {
        ddh = 48 + pbSrc[i] / 16;
        ddl = 48 + pbSrc[i] % 16;
        if (ddh > 57) ddh = ddh + 7;
        if (ddl > 57) ddl = ddl + 7;
        pbDest[i*3] = ddh;
        pbDest[i*3+1] = ddl;
        pbDest[i*3+2] ='-';

    }

    pbDest[nLen*3] = '\0';
}

/*
��������N��OK֡����ֹ433ģ��ӵ��
*/

void wdz_sendOKfrma(int n)
{
    int i=0;
    for(i=0; i<n; i++)
    {
        //����Ok֡����ֹӵ��
        Delay_us(100);
        WDZ_101Transmit_OKdata();

    }
}

//���͸���MCU��������
/*
���ܣ�������������
*/

uint8_t WDZ_MCUtransmit_restdata(void)
{

    uint8_t my_status=0;
    uint8_t my_rxbuf[7]="\x10\x80\x01\x00\x81\x16";
    uint8_t my_txbuf[7]=TXMCU_restdata;




    my_status=WDZ_MCUtransmint_commd_wait_commd(1,my_txbuf,1,my_rxbuf);

    if(my_status==0)
    {
        USART_printf(&huart3," MUC--rest-\r\n");
    }
    else if(my_status==1)
    {
        USART_printf(&huart3," MUC--rest*\r\n");
    }

    return my_status;


}

/*
���ܣ����з��Ʋ������ڽ��յ�GPRS���͵ķ���ָ��󣬽���433ģ�鷭�Ʋ���
*/
uint8_t WDZ_101Transmit_Control_monitor(uint8_t address,uint8_t status_value)
{

    uint8_t my_status=0;
    uint8_t my_txbuf[18]=TX101_controlMonitor;
    //uint8_t my_rsbuf[18]="\x68\x0B\x0B\x68\x53\x01\x00\x2D\x01\x07\x01\x00\x01\x60\x80\x6B\x16";
    // 68 0B 0B 68 D3 01 00 2D 01 06 01 00 01 60 80 EA 16
    my_txbuf[12]=address; //01����1֧·A��02��03����B��C  04����2֧·A,07����3֧·A
    my_txbuf[14]=status_value;  //0X81�������ƣ�0X80������λ
    //����--����
    //�ӷ�--�϶�ȷ��


    //Delay_ms(3);  //����ָ�����ʱ��Ϊʲô������
    //my_status=WDZ_101transmint_commd_wait_commd(2,my_txbuf,2,my_rsbuf);//typeΪ���ͣ�1Ϊ�̶����ȣ�2Ϊ�ǹ̶����ȣ�txbufΪ����ָ������
    //Delay_ms(15);  //����ָ�����ʱ��Ϊʲô������
    //ֱ�ӷ���433����ָ��
    wdz_101check_generate(my_txbuf);
    my_UART4_printf(&huart4,my_txbuf);
    my_status=1;


    //����-OK
    if(my_status==1)
    {
        //WDZ_101Transmit_OKdata();//����OK֡
        //USART_printf(&huart3,"433 ControlMonitor OK--%d!\r\n ",address);
    }
    else
    {
        USART_printf(&huart3,"433 ControlMonitor fail-2-%d!\r\n ",address);
    }

    return my_status;

}

/*
����ʹ��
*/
void test_controlmonitor(int status)
{
    if(status==1)
    {
        //  WDZ_101Transmit_Control_monitor(0X01,0X80); //����ʹ��
        //	WDZ_101Transmit_Control_monitor(02,0X81); //����ʹ��
        //	WDZ_101Transmit_Control_monitor(0X03,0X80); //����ʹ��
        //	WDZ_101Transmit_Control_monitor(0X05,0X80); //2a��?��1��?
        WDZ_101Transmit_Control_monitor(0X06,0X80); //2a��?��1��?
        //	WDZ_101Transmit_Control_monitor(0X07,0X80); //2a��?��1��?

    }
}


void my_reset2_mcu()  //����MCUͨ��������
{
    __disable_fault_irq();
    NVIC_SystemReset();
}

//���ܣ�����ʹ�ã�������������

int my_alarm_count=0;
uint8_t  my_temp_address=0;
void my_test_alarm_data()
{
    uint8_t *rsbuf=USART4_my_frame;
    uint8_t my_rtc_temp_time[7]= {0};

    my_alarm_count++;

    if(my_alarm_count%(2*57)==0)  //0.5������һ��
    {
        my_temp_address++;
        USART4_FRAME_status=1;  //�˹�������������
        // 68 0B 0B 68 73 01 00 01 01 03 01 00 26 00 01 A1 16
        rsbuf[0]=0x68;
        rsbuf[1]=0x0B;
        rsbuf[2]=0x0B;
        rsbuf[3]=0x68;
        rsbuf[4]=0x73;
        rsbuf[5]=0x01;
        rsbuf[6]=0x00;
        rsbuf[7]=0x01;
        rsbuf[8]=0x01;
        rsbuf[9]=0x03;
        rsbuf[10]=0x01;
        rsbuf[11]=0x00;
        rsbuf[12]=my_temp_address; //��ַ��λ
        rsbuf[13]=0x00;
        rsbuf[14]=0x01; //ֵ
        rsbuf[15]=0xA1;  //У����
        rsbuf[16]=0x16;

        wdz_101check_generate(rsbuf);
    }

    if(my_alarm_count%(2*58)==0) //��ʱ��  //0.5������1
    {
        USART4_FRAME_status=1;  //�˹�������������
        // 68 12 12 68 53 01 00 1E 01 03 01 00 26 00 01 DB 6C 12 0B 1E 08 0B 33 16
        rsbuf[0]=0x68;
        rsbuf[1]=0x12;
        rsbuf[2]=0x12;
        rsbuf[3]=0x68;
        rsbuf[4]=0x53;
        rsbuf[5]=0x01;
        rsbuf[6]=0x00;
        rsbuf[7]=0x1E;
        rsbuf[8]=0x01;
        rsbuf[9]=0x03;
        rsbuf[10]=0x01;
        rsbuf[11]=0x00;
        rsbuf[12]=my_temp_address; //��ַ��λ
        rsbuf[13]=0x00;
        rsbuf[14]=0x01; //ֵ

        my_RTCtime_to_array(my_rtc_temp_time);

        rsbuf[15]=my_rtc_temp_time[0];
        rsbuf[16]=my_rtc_temp_time[1];
        rsbuf[17]=my_rtc_temp_time[2];
        rsbuf[18]=my_rtc_temp_time[3];
        rsbuf[19]=my_rtc_temp_time[4];
        rsbuf[20]=my_rtc_temp_time[5];
        rsbuf[21]=my_rtc_temp_time[6];
        rsbuf[22]=0xA1;  //У����
        rsbuf[23]=0x16;

        wdz_101check_generate(rsbuf);
    }


}



/*

���ܣ�ͨ���ͷ�������ȴ��϶�ȷ�Ϻ�������������ָ��յ��϶�ȷ��ָ���1��û�յ�ȷ�Ϸ���0
��ڲ�����typeΪ���ͣ�1Ϊ�̶����ȣ�2Ϊ�ǹ̶����ȣ�txbufΪ����ָ������
					address_typeΪ1����Ӧ�õ�ַ���ȷ����ֽڣ��󷢸��ֽڣ�����Ĭ�Ϲ���
									 ���Ϊ2�����ȷ����ֽڣ��󷢵��ֽڣ���Щָ��ֻ�������û�취

*/



uint8_t WDZ_101transmint_commd2(uint8_t type,uint8_t *txbuf,uint8_t address_type,uint8_t n_count)  //typeΪ���ͣ�1Ϊ�̶����ȣ�2Ϊ�ǹ̶����ȣ�txbufΪ����ָ������
{
    uint8_t FCB=0x20;
    uint16_t my_dtu_address=0x0001; //  DTU_ADDRESS;
    uint8_t my_status=0;
    uint8_t n=n_count;
    uint8_t *my_txbuf=txbuf;
    uint16_t my_start_time=0;

    if(type==2)
    {
        //�������봦��
        if(WDZ_101FCB==0x00)
            my_txbuf[4]=my_txbuf[4]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[4]=my_txbuf[4]|(FCB);
        //Ŀ�ĵ�ַ����
        my_txbuf[5]=my_dtu_address&0xFF;
        my_txbuf[6]=my_dtu_address>>8;


        if(address_type==1)
        {
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }
        else if(address_type==2)
        {
            //my_txbuf[10]=my_txbuf[6]; //����ָ����ȴ��ظ�
            //my_txbuf[11]=my_txbuf[5];
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }

    }
    else if(type==1)
    {
        //�������봦��
        if(WDZ_101FCB==0x00)
            my_txbuf[1]=my_txbuf[1]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[1]=my_txbuf[1]|(FCB);
        //Ŀ�ĵ�ַ����

        my_txbuf[5]=my_dtu_address&0xFF;
        my_txbuf[6]=my_dtu_address>>8;



    }


    //У��λ����
    wdz_101check_generate(my_txbuf);
    //����ָ��

    while((n--)>0&&my_status==0)
    {
        //my_UART4_printf(UART4,my_txbuf);
        my_UART4_printf(&huart4,my_txbuf);
        Delay_ms(1);

        my_start_time=my_tim6_count;
        do {

            my_status=WDZ_101recive_OKdata();
            if(my_status==1)break;
            else my_status=0;
        } while(my_tim6_count-my_start_time<=3);

    }
    WDZ_101FCB=WDZ_101FCB^0x20;
    if(my_status==1)
    {
        return 1;
    }
    else return 0;

}
/*
���ܣ��Ӻ���������ָʾ������ʱ��
*/
uint8_t WDZ_101Transmit_Cycle_analog_time(uint8_t address,uint8_t status_value)
{

    uint8_t my_status=0;
    uint8_t my_txbuf[18]=TX101_Cycleanalogtime;
    //uint8_t my_rsbuf[18]="\x68\x0B\x0B\x68\x53\x01\x00\x2D\x01\x07\x01\x00\x01\x60\x80\x6B\x16";
    //68 0B 0B 68 F3 01 00 3D 01 06 01 00 01 70 01 AB 16
    my_txbuf[12]=address; //01����1֧·A��02��03����B��C  04����2֧·A,07����3֧·A
    my_txbuf[14]=status_value;  //0X81�������ƣ�0X80������λ
    //����--����
    //�ӷ�--�϶�ȷ��


    //Delay_ms(3);  //����ָ�����ʱ��Ϊʲô������
    //my_status=WDZ_101transmint_commd_wait_commd(2,my_txbuf,2,my_rsbuf);//typeΪ���ͣ�1Ϊ�̶����ȣ�2Ϊ�ǹ̶����ȣ�txbufΪ����ָ������
    //Delay_ms(15);  //����ָ�����ʱ��Ϊʲô������
    //ֱ�ӷ���433����ָ��
    wdz_101check_generate(my_txbuf);
    my_UART4_printf(&huart4,my_txbuf);
    my_status=1;


    //����-OK
    if(my_status==1)
    {
        //WDZ_101Transmit_OKdata();//����OK֡
        //USART_printf(&huart3,"433 ControlMonitor OK--%d!\r\n ",address);
    }
    else
    {
        USART_printf(&huart3,"433 CycleAnalogTime fail-2-%d!\r\n ",address);
    }

    return my_status;

}

//���͸���ѹ����MCU,ִ��433��������
/*
���ܣ�������������
*/

uint8_t WDZ_MCUtransmit_433restdata(void)
{

    uint8_t my_status=0;
    uint8_t my_rxbuf[7]="\x10\x80\x01\x00\x81\x16";
    uint8_t my_txbuf[7]=TXMCU_433restdata;




    my_status=WDZ_MCUtransmint_commd_wait_commd(1,my_txbuf,1,my_rxbuf);

    if(my_status==0)
    {
        USART_printf(&huart3," MUC--433rest-\r\n");
    }
    else if(my_status==1)
    {
        USART_printf(&huart3," MUC--433rest*\r\n");
    }

    return my_status;


}