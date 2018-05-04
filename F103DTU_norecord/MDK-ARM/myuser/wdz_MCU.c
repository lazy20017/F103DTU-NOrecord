#include "wdz_MCU.h"


extern	uint16_t USART2_address_first;
extern	uint16_t USART2_address_second;
extern	uint8_t USART2_length;
extern	uint8_t USART2_FRAME_status;
extern	uint8_t USART2_my_frame[256];
extern	uint8_t USART2_TRANSMINT_STATUS;
extern	uint8_t USART2_RESIVER_STATUS;


extern uint32_t TimingDelay;
extern uint16_t my_tim6_count;


/*
���ܣ���������ȴ����ض�����������
*/
uint8_t WDZ_MCUtransmint_commd_wait_commd(uint8_t type,uint8_t *txbuf,uint8_t address_type,uint8_t *rxbuf)  //typeΪ���ͣ�1Ϊ�̶����ȣ�2Ϊ�ǹ̶����ȣ�txbufΪ����ָ������
{
    //uint8_t FCB=0x20;
    //uint16_t my_dtu_address=0X0001;
    uint8_t my_status=0;
    uint8_t n=3;
    uint8_t *my_txbuf=txbuf;
//	uint8_t temp=0;

    uint8_t linkcontrol=0;
    uint8_t type_identification=0;
    uint8_t  transmit_reason=0;
    uint16_t my_start_time=0;
//********************���ͷǹ̶�����֡��Ӧ��ΪOK֡
    if(type==2)
    {

        uint8_t linkcontrol=rxbuf[1];
        uint8_t type_identification=0;
        uint8_t  transmit_reason=0;

        //�������봦��

        //Ŀ�ĵ�ַ����
        my_txbuf[5]=0X01;
        my_txbuf[6]=0X00;


        //У��λ����
        wdz_101check_generate(my_txbuf);
        //����ָ��
        while((n--)>0&&my_status==0)
        {
            //USART_printf(&huart2,my_txbuf);
            my_UART4_printf(&huart2,my_txbuf);
            //Delay_us(100);

            my_start_time=my_tim6_count;
            do {

                my_status=WDZ_MCUreceive_testactive(linkcontrol,type_identification,transmit_reason,300);  //�ȴ�0Kȷ�Ϸ�������
                if(my_status==1)break;
                else my_status=0;
            } while(my_tim6_count-my_start_time<=3);

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

        //Ŀ�ĵ�ַ����
        my_txbuf[2]=0X01;
        my_txbuf[3]=0X00;

        //У��λ����
        wdz_101check_generate(my_txbuf);
        //����ָ��
        while((n--)>0&&my_status==0)
        {
            //USART_printf(&huart2,my_txbuf);
            my_UART4_printf(&huart2,my_txbuf);

            //Delay_us(300); //����

            my_status=WDZ_MCUreceive_testactive(linkcontrol,type_identification,transmit_reason,300);  //�ȴ���������
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

        //Ŀ�ĵ�ַ����
        my_txbuf[2]=0x01;
        my_txbuf[3]=0x00;

        //У��λ����
        wdz_101check_generate(my_txbuf);
        //����ָ��
        while((n--)>0&&my_status==0)
        {
            //USART_printf(&huart2,my_txbuf);
            my_UART4_printf(&huart2,my_txbuf);

            //Delay_us(600); //����

            my_status=WDZ_MCUreceive_testactive(linkcontrol,type_identification,transmit_reason,300);  //�ȴ���������
            if(my_status==1)break;
            else my_status=0;
        }
    }

    if(my_status==1)
    {
        return 1;
    }
    else return 0;

}


/*
���ܣ�����Ӧ��֡
���������Link_control��������룬type_identification-֡���ͣ�transmit_reason-����ԭ��time-���ʱ��
��������Ӧ����ȷΪ1��Ӧ������û��Ӧ��Ϊ0
*/

uint8_t WDZ_MCUreceive_testactive(uint8_t Link_control,uint8_t type_identification,uint8_t transmit_reason,uint16_t time)
{
    uint8_t temp=0;
    uint8_t *rsbuf=USART2_my_frame;

    uint8_t status68=0;  //2�����ǹ̶�����֡��1�����̶�����6��֡��0��������������
    uint16_t my_dtu_address=0x0001;
    uint16_t my_start_time=0;


    my_start_time=my_tim6_count;
    while(my_tim6_count-my_start_time<=3)
    {   my_usart_101frame(2);
        if(USART2_FRAME_status>0)
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
                if(my_dtu_address==0x0001)
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



            //֡���ͼ�顢����ԭ����
            if(status68==1)
            {

                if((rsbuf[1]&0x0f)==(Link_control&0x0f))
                {
                    temp=1;
                    //֡�Ĵ������̽���
                    USART2_FRAME_status=0;
                    break;
                }
                else
                {   temp=0;
                    //���ԣ��ѽ��յ���֡ת������3
                    USART_printf(&huart3,"mm4444");
                    USART_printf(&huart3,USART2_my_frame);

                    //֡�Ĵ������̽���
                    USART2_FRAME_status=0;
                }

                //֡�Ĵ������̽���
                USART2_FRAME_status=0;
            }



//************0x68�ǹ̶�����֡����
            //��վ��ַ���
            if(status68==2)
            {
                my_dtu_address=rsbuf[6];
                my_dtu_address=(my_dtu_address<<8)+rsbuf[5];
                if(my_dtu_address==0x0001)
                    status68=2;
                else status68=0;
            }
            //У���ֽڼ��

            if(status68==2)
            {
                temp=wdz_101char_check(rsbuf);  //����ʱ���Կ����Ȳ�ʹ��
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
                    USART2_FRAME_status=0;
                    break;
                }
                else
                {   temp=0;
                    //���ԣ��ѽ��յ���֡ת������3
                    USART_printf(&huart3,"mm66666 MCU!\n");

                    //֡�Ĵ������̽���
                    USART2_FRAME_status=0;
                }
            }
            //********************�ǹ̶�����֡��������

            //֡�Ĵ������̽���
            USART2_FRAME_status=0;

        }


    }

    return temp;

}


/**********************************

*/

/*
���ܣ�����������
*/

uint8_t WDZ_MCUtransmit_heartdata(void)
{

    uint8_t my_status=0;
    uint8_t my_rxbuf[7]="\x10\x80\x01\x00\x81\x16";
    uint8_t my_txbuf[7]=TX101_heartdata;


    my_status=WDZ_MCUtransmint_commd_wait_commd(1,my_txbuf,1,my_rxbuf);

    if(my_status==0)
    {
        USART_printf(&huart3," MUC--heart-\r\n");
    }
    else if(my_status==1)
    {
        USART_printf(&huart3," MUC--heart*\r\n");
    }

    return my_status;

}


/*
68 0B 0B 68 53 01 00 64 01 06 01 00 00 00 14 D4 16�����ٻ����� ���
		Ӧ��ΪOK֡ "\x10\x80\x01\x00\x81\x16"
		ң�⣬��Ʒ�ʣ�����ʱ�� 68 53 53 68 73 01 00 09 98 14 00 01 00 01 40 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 6B 16 ң�⣬��Ʒ�ʣ�����ʱ��
10 00 01 00 01 16 �϶�ȷ��



���ܣ���������
*/
extern uint8_t MY_MCU_RsBuf[8];
extern uint8_t MY_MCU_getdata_status;
uint16_t MY_Bat_value=0;  //2016-06-12  �޸� ��ȡ��ص�ѹֵ
uint16_t MY_Sun_value=0;  //2016-08-30  �޸� ��ȡSUN��ѹֵ
uint16_t MY_Temperature_value=0; //2016-08-30
uint16_t MY_Humidity_value=0; //2016-08-30

uint8_t WDZ_MCUtransmit_Calldata(void)
{
    uint8_t my_status=0;
    uint8_t k=0;
    uint8_t my_rxbuf[256]="\x10\x80\x01\x00\x81\x16";
    uint8_t my_txbuf[18]=TX101_calldata;

    //����--����
    //�ӷ�--�϶�ȷ��

    my_status=WDZ_MCUtransmint_commd_wait_commd(2,my_txbuf,1,my_rxbuf);

    //Delay_us(300); 	//����ʹ��

    //�ӷ�--ң��
    if(my_status==1)
    {
        my_status=WDZ_MCUreceive_testactive(0x03,0x09,0x14,300);
        //�յ�����֡�����д���������������

        if(my_status==1)
        {
            MY_MCU_getdata_status=1;  //��ʾ�յ�����������
            for(k=0; k<8; k++)
            {
                MY_MCU_RsBuf[k]=USART2_my_frame[k+14];
            }
            //2016-06-12 ȡ��ص�ѹ
            MY_Bat_value=MY_MCU_RsBuf[1];
            MY_Bat_value=(MY_Bat_value<<8)+MY_MCU_RsBuf[0];
            //2016-08-30 ȡ̫���ܣ��¶ȡ�ʪ��
            MY_Sun_value=MY_MCU_RsBuf[3];
            MY_Sun_value=(MY_Sun_value<<8)+MY_MCU_RsBuf[2];

            MY_Temperature_value=MY_MCU_RsBuf[5];
            MY_Temperature_value=(MY_Temperature_value<<8)+MY_MCU_RsBuf[4];

            MY_Humidity_value=MY_MCU_RsBuf[7];
            MY_Humidity_value=(MY_Humidity_value<<8)+MY_MCU_RsBuf[6];

        }
        else
        {
            MY_MCU_getdata_status=1;  //��ʾ�յ�����������
            for(k=0; k<8; k++)
            {
                MY_MCU_RsBuf[k]=0XFF;
            }
            MY_Bat_value=0XFFFF;  //AD�豸�������⣬ȡ0xffff���������ٶȷ�������

        }

    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3," MCU--call1!\r\n ");
        //LED3_TOGGLE;
    }

    //��DTU--���϶�ȷ��
    if(my_status==1) //���Ϳ϶�ȷ������
    {

        //USART_printf(&huart2,TX101_OKdata);
        my_UART4_printf(&huart2,TX101_OKdata);

        //Delay_us(100);//����ʹ��
        USART_printf(&huart3," MUC--call*\r\n "); //����ʹ��

        //LED2_TOGGLE;
    }
    else if(my_status==0)  //ʧ�ܴ���
    {
        //Delay_us(100);
        USART_printf(&huart3," MUC--call-\r\n ");
        //LED3_TOGGLE;
    }

    return my_status;
}

