#include "wdz_eeprom.h"
#include "my_usart.h"
#include "my_def_value.h"
#include "my_extrn_value.h"








/*
���ܣ����������������ݽ��д��������ٳɹ�������ݴ洢��EEPROM�У���ʶΪδ����--2
			������Դ��PW��4���������ݣ�ָʾ�����ٵ�����ң�š�ң������ ��3�����飬ʱ��������ң�������У�
			���ݴ洢��󣬰��������㣬ͬʱ�������������������ݱ�ʶ�������µ�GRPS�����ݱ�ʶ
��ʽ��״̬��1����ң�Ÿ���N1��1����ң�����N2��1������ص�ѹ��2����̫���ܵ�ѹ��2�����¶ȣ�2����ʪ�ȣ�2����
			ң����Ϣ��1ֵ��1����ң����Ϣ��2ֵ��1����....��ң����Ϣ��1ֵ��3����ң����Ϣ2ֵ��3����������ʱ��7
*/

uint16_t my_cyc_data_write_cn=0;
void my_cycle_data_to_eeprom(void)
{
    uint32_t startaddressPT=MY_Table2_Cycledata_StartAddress;
    uint32_t endaddressPT=MY_Table2_Cycledata_EndAddress;
    uint32_t writeaddressPT=MY_Table2_Cycledata_WriteAddress;
    uint32_t readaddressPT=MY_Table2_Cycledata_ReadAddress;
    uint16_t k=0;
    uint16_t number_single=0;
    uint16_t number_anlog=0;
    uint8_t status_cycle=0;
    uint8_t tempbuf[12]= {0};
    uint8_t *PT_buf;
    uint32_t PT_writeaddress=0;
    uint16_t PT_number=0;
    uint8_t temp=0;
    uint8_t Zero_buf[256]= {0};

    uint8_t rs_temp_buf[512]= {0}; //����ʹ��


//***���������µı�ָ���ַ
    SPI_EE_BufferRead2(tempbuf,EEPROM_Table2_Address,12);
    if(tempbuf[0]==tempbuf[1]&&tempbuf[1]==tempbuf[2]&&tempbuf[2]==tempbuf[3]&&tempbuf[3]==tempbuf[4]&&tempbuf[4]==tempbuf[5])
    {

        my_val_writeto_eeprom(tempbuf, startaddressPT, endaddressPT, writeaddressPT, readaddressPT,EEPROM_Table2_Address);  //�ѳ�ʼ���ַд�뵽EEPROM��

    }
    else
    {

        my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &writeaddressPT, &readaddressPT);  //��EEPROM�ж������±��ַ��������

        MY_Table2_Cycledata_StartAddress=startaddressPT;
        MY_Table2_Cycledata_EndAddress=endaddressPT;
        MY_Table2_Cycledata_WriteAddress=writeaddressPT;
        MY_Table2_Cycledata_ReadAddress=readaddressPT;

    }


//***********over****
    USART_printf(&huart3,"MCU_status=%d  433_CALL_Status=%d\r\n",MY_MCU_getdata_status,MY_433_Call_Status);  //����ʹ��@@@@
    if(MY_MCU_getdata_status==1 && MY_433_Call_Status==1 )
    {


        //�洢ң�š�ң�����Ϣ�����
        number_single=MY_433_Call_Single_data_number;
        number_anlog=MY_433_Call_Analog_data_number;

        //����д��ַָ��߽��⣬��������洢����ĩ��ַ������׵�ַ��ʼ���´洢��ʣ�µ��ֽھͲ�����
        if((endaddressPT-writeaddressPT+1)<144)
        {
            SPI_EE_BufferWrite2(Zero_buf,writeaddressPT,endaddressPT-writeaddressPT+1);//�����еĲ�����������
            writeaddressPT=startaddressPT;
            MY_Table2_Cycledata_WriteAddress=writeaddressPT;
            PT_writeaddress=writeaddressPT;
            USART_printf(&huart3,"CLear ear %d\r\n",endaddressPT-writeaddressPT+1);
        }


        //PT_writeaddress=writeaddressPT+3+8+number_single+number_anlog*3+7; //���д������ݺ�ĩ��ַ����
        //if(PT_writeaddress>endaddressPT)
        //{
        //	SPI_EE_BufferWrite2(Zero_buf,writeaddressPT,endaddressPT-writeaddressPT+1);//�����еĲ�����������

        //	writeaddressPT=startaddressPT;
        //	MY_Table2_Cycledata_WriteAddress=writeaddressPT;
        //}


        //��ʼ�������ݴ洢
        for(k=0; k<256; k++)
            Zero_buf[k]=k+1;
        SPI_EE_BufferWrite2(Zero_buf,writeaddressPT,144); //����������ݵ�Ԫ @@@@@@@@@

        //�洢GPRS����״̬��1Ϊ�����꣬2Ϊû�з���
        AT25_WriteByte(0X02,writeaddressPT);   //��ַ0�ֽڣ�Gprs״̬��1Ϊ�����꣬2Ϊû�з���

        //�洢ң�š�ң�����Ϣ�����
        number_single=MY_433_Call_Single_data_number;
        number_anlog=MY_433_Call_Analog_data_number;
        AT25_WriteByte(number_single,writeaddressPT+1);  //��ַ1�ֽڣ����ٻ�õ�ң����Ϣ�����
        AT25_WriteByte(number_anlog,writeaddressPT+2);   //��ַ2�ֽڣ����ٻ�õ�ң����Ϣ�����

        //����ʹ�ã����ж�ȡ���ղ�д�������
        //----------------


        //-------------
        //�洢PW��������
        PT_buf=MY_MCU_RsBuf;
        PT_writeaddress=writeaddressPT+3;
        PT_number=8;
        SPI_EE_BufferWrite2(PT_buf,PT_writeaddress,PT_number); //��ַ3-10�ֽڣ���ص�ѹ��̫���ܵ�ѹ���¶ȡ�ʪ��


        //�洢ң������
        PT_buf=MY_433_Call_Single_data_buf;
        PT_writeaddress=writeaddressPT+11;
        PT_number=number_single;
        SPI_EE_BufferWrite2(PT_buf,PT_writeaddress,PT_number);
        //SPI_EE_BufferWrite2(MY_433_Call_Single_data_buf,writeaddressPT+11,number_single);


        //�洢ң������
        PT_buf=MY_433_Call_Analog_data_buf;
        PT_writeaddress=writeaddressPT+11+number_single;
        PT_number=number_anlog*3;
        SPI_EE_BufferWrite2(PT_buf,PT_writeaddress,PT_number);
        //SPI_EE_BufferWrite2(MY_433_Call_Analog_data_buf,writeaddressPT+11+number_single,number_anlog*3);

        //�洢ʱ������
        PT_buf=MY_433_Call_Time_data_buf;
        PT_writeaddress=writeaddressPT+11+number_single+number_anlog*3;
        PT_number=7;
        SPI_EE_BufferWrite2(PT_buf,PT_writeaddress,PT_number);
        //SPI_EE_BufferWrite2(MY_433_Alarmdata_TimeBuf,);

        //------------------����ʹ��
        //����ʹ�ã���ʾд�뵽�ڴ��еĹ�������@@@@@
        status_cycle=AT25_ReadByte(writeaddressPT);
        MY_GPRS_Call_Single_data_number=AT25_ReadByte(writeaddressPT+1);
        MY_GPRS_Call_Analog_data_number=AT25_ReadByte(writeaddressPT+2);
        SPI_EE_BufferRead2(MY_GPRS_MCU_RsBuf,writeaddressPT+3,8);
        SPI_EE_BufferRead2(MY_GPRS_Call_Single_data_buf,writeaddressPT+11,18);
        SPI_EE_BufferRead2(MY_GPRS_Call_Analog_data_buf,writeaddressPT+29,108);
        SPI_EE_BufferRead2(MY_GPRS_Call_Time_data_buf,writeaddressPT+29+108,7);


        USART_printf(&huart3,"real data to eeprom start_writeaddress=%d%d%d%d%d%d\r\n",
                     (writeaddressPT-EEPROM_Chip_size)/100000,(writeaddressPT-EEPROM_Chip_size)%100000/10000,
                     (writeaddressPT-EEPROM_Chip_size)%10000/1000,(writeaddressPT-EEPROM_Chip_size)%1000/100,
                     (writeaddressPT-EEPROM_Chip_size)%100/10,(writeaddressPT-EEPROM_Chip_size)%10
                    );
        USART_printf(&huart3,"status cycle=%d  sing data=%d    analog data=%d\r\n",status_cycle,MY_GPRS_Call_Single_data_number,MY_GPRS_Call_Analog_data_number);
        HexToStr2(rs_temp_buf,MY_GPRS_MCU_RsBuf,8);
        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");
        HexToStr2(rs_temp_buf,MY_GPRS_Call_Single_data_buf,18);
        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");
        HexToStr2(rs_temp_buf,MY_GPRS_Call_Analog_data_buf,108);
        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");
        HexToStr2(rs_temp_buf,MY_GPRS_Call_Time_data_buf,7);
        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");


        //--------------------



        //--------???�����Ƿ�ʹ�� �Ѷ�ָ��ֱ���ƶ�����ָ����׵�ַ-
        /*
        readaddressPT=MY_Table2_Cycledata_WriteAddress;
        temp=readaddressPT&0x000000ff;
        AT25_WriteByte(temp,222);

        temp=(writeaddressPT>>8)&0x000000ff;
        AT25_WriteByte(temp,223);

        temp=(writeaddressPT>>16)&0x000000ff;
        AT25_WriteByte(temp,224);
        */

        //-----------

        //������ı�д��ַָ��
        //writeaddressPT=PT_writeaddress+7;
        //
        writeaddressPT=writeaddressPT+144; //��һ֡�������ռ�144���ֽڽ��д洢���
        if(writeaddressPT>=endaddressPT)
            writeaddressPT=startaddressPT; //��ַУ��

        MY_Table2_Cycledata_WriteAddress=writeaddressPT;

        //����д��ַָ��д�뵽EEPROM��
        //my_val_writeto_eeprom(tempbuf, startaddressPT, endaddressPT, writeaddressPT, readaddressPT,213);
        temp=writeaddressPT&0x000000ff;
        AT25_WriteByte(temp,219);

        temp=(writeaddressPT>>8)&0x000000ff;
        AT25_WriteByte(temp,220);

        temp=(writeaddressPT>>16)&0x000000ff;
        AT25_WriteByte(temp,221);


        //���������з���״̬����
        //MY_MCU_getdata_status=0;  //PW�����������ݴ������
        //MY_433_Call_Status=0;    //433ģ���������ݴ������
        MY_Table2_Newdata_status=1;


        //����ʹ�ã���������@@@@@@@
        USART_printf(&huart3,"real data to eeprom\r\n");


        HexToStr2(rs_temp_buf,MY_MCU_RsBuf,8);
        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");
        HexToStr2(rs_temp_buf,MY_433_Call_Single_data_buf,18);
        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");
        HexToStr2(rs_temp_buf,MY_433_Call_Analog_data_buf,108);
        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");
        HexToStr2(rs_temp_buf,MY_433_Call_Time_data_buf,7);
        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");


        //------------------------------------------
        //2016-05-28 ����
        my_cyc_data_write_cn++;
        USART_printf(&huart3,"*****cyc_data_write=%d\r\n",my_cyc_data_write_cn);
        //2016-05-28 ����

    }



}
//---------------------------

/*
���ܣ�ָ���ַ���뵽�����У���д�뵽EEPROM��
*/

void my_val_writeto_eeprom(uint8_t *tempbuf,uint32_t startaddressPT,uint32_t endaddressPT,uint32_t writeaddressPT,uint32_t readaddressPT,uint16_t table_startaddress)
{
    tempbuf[0]=startaddressPT&0x000000ff;
    tempbuf[1]=(startaddressPT>>8)&0x000000ff;
    tempbuf[2]=(startaddressPT>>16)&0x000000ff;

    tempbuf[3]=endaddressPT&0x000000ff;
    tempbuf[4]=(endaddressPT>>8)&0x000000ff;
    tempbuf[5]=(endaddressPT>>16)&0x000000ff;

    tempbuf[6]=writeaddressPT&0x000000ff;
    tempbuf[7]=(writeaddressPT>>8)&0x000000ff;
    tempbuf[8]=(writeaddressPT>>16)&0x000000ff;

    tempbuf[9]=readaddressPT&0x000000ff;
    tempbuf[10]=(readaddressPT>>8)&0x000000ff;
    tempbuf[11]=(readaddressPT>>16)&0x000000ff;

    SPI_EE_BufferWrite2(tempbuf,table_startaddress,12);

}
//------------------------------------------------
/*
���ܣ�����������д�뵽������

*/

void my_buf_writeto_val(uint8_t *tempbuf,uint32_t *startaddressPT,uint32_t *endaddressPT,uint32_t *writeaddressPT,uint32_t *readaddressPT)

{
    *startaddressPT=0;
    *startaddressPT=tempbuf[2];
    *startaddressPT=(*startaddressPT<<8)+tempbuf[1];
    *startaddressPT=(*startaddressPT<<8)+tempbuf[0];

    *endaddressPT=0;
    *endaddressPT=tempbuf[5];
    *endaddressPT=(*endaddressPT<<8)+tempbuf[4];
    *endaddressPT=(*endaddressPT<<8)+tempbuf[3];

    *writeaddressPT=0;
    *writeaddressPT=tempbuf[8];
    *writeaddressPT=(*writeaddressPT<<8)+tempbuf[7];
    *writeaddressPT=(*writeaddressPT<<8)+tempbuf[6];

    *readaddressPT=0;
    *readaddressPT=tempbuf[11];
    *readaddressPT=(*readaddressPT<<8)+tempbuf[10];
    *readaddressPT=(*readaddressPT<<8)+tempbuf[9];

}

//----------------------------------------------------*******************-

/*
���ܣ����յ��ı������ݴ��뵽EEPROM��
��ʽ��״̬�ֽڣ�1������Ϣ�������1������Ϣ��1��ַ���ֽڣ�1������Ϣ��1��ַ���ֽڣ�1������Ϣ��1ֵ��1����ʱ�꣨7��
		��Ϣ��2��ַ���ֽڣ�1������Ϣ��2��ַ���ֽڣ�1������Ϣ��2ֵ��1����ʱ�꣨7��

*/
void my_alarm_data_to_eeprom(void)
{

    uint32_t startaddressPT=MY_Table1_Alarmdata_StartAddress;
    uint32_t endaddressPT=MY_Table1_Alarmdata_EndAddress;
    uint32_t writeaddressPT=MY_Table1_Alarmdata_WriteAddress;
    uint32_t readaddressPT=MY_Table1_Alarmdata_ReadAddress;
    //uint16_t k=0;
    uint16_t number_single=0;
    uint8_t tempbuf[12]= {0};
    uint8_t *PT_buf;
    uint32_t PT_writeaddress=0;
    uint16_t PT_number=0;
    uint8_t temp=0;
    uint8_t Zero_buf[256]= {0};

//***���������µı�ָ���ַ
    SPI_EE_BufferRead2(tempbuf,201,12);
    if(tempbuf[0]==tempbuf[1]&&tempbuf[1]==tempbuf[2]&&tempbuf[2]==tempbuf[3]&&tempbuf[3]==tempbuf[4]&&tempbuf[4]==tempbuf[5])
    {

        my_val_writeto_eeprom(tempbuf, startaddressPT, endaddressPT, writeaddressPT, readaddressPT,201);

    }
    else
    {

        my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &writeaddressPT, &readaddressPT);

        MY_Table1_Alarmdata_StartAddress=startaddressPT;
        MY_Table1_Alarmdata_EndAddress=endaddressPT;
        MY_Table1_Alarmdata_WriteAddress=writeaddressPT;
        MY_Table1_Alarmdata_ReadAddress=readaddressPT;

    }


//***********over****

    if(MY_433_ALarmdata_Time_status==1 )
    {


        //�洢 ���� ��ң�� ��Ϣ�����
        number_single=MY_433_ALarmdata_number;
        number_single=1; //ǿ����Ϣ�����Ϊ1������������
        //����д��ַָ��߽��⣬��������洢����ĩ��ַ������׵�ַ��ʼ���´洢��ʣ�µ��ֽھͲ�����

        PT_writeaddress=writeaddressPT+2+number_single*10; //���д������ݺ�ĩ��ַ����
        if(PT_writeaddress>endaddressPT)
        {
            SPI_EE_BufferWrite2(Zero_buf,writeaddressPT,endaddressPT-writeaddressPT+1);//�����еĲ�����������
            writeaddressPT=startaddressPT;
            MY_Table1_Alarmdata_WriteAddress=writeaddressPT;


        }


        //�洢GPRS����״̬��1Ϊ�����꣬2Ϊû�з���
        AT25_WriteByte(0X02,writeaddressPT);   //��ַ0�ֽڣ�Gprs״̬��1Ϊ�����꣬2Ϊû�з���

        //�洢 ���� ��ң�� ��Ϣ�����
        number_single=MY_433_ALarmdata_number;
        number_single=1; //ǿ����Ϣ�����Ϊ1��������������������ʱ����ģ�һ��һ����Ϣ�壬�洢��12�ֽ�
        AT25_WriteByte(number_single,writeaddressPT+1);  //��ַ1�ֽڣ�������õ�ң����Ϣ�����

        //�洢ң������
        PT_buf=MY_433_Alarmdata_TimeBuf;
        PT_writeaddress=writeaddressPT+2;
        PT_number=number_single*10;
        SPI_EE_BufferWrite2(PT_buf,PT_writeaddress,PT_number);
        //SPI_EE_BufferWrite2(MY_433_Call_Single_data_buf,writeaddressPT+2,number_single);

        //--------???�����Ƿ�ʹ�� �Ѷ�ָ��ֱ���ƶ�����ָ����׵�ַ-
        //readaddressPT=MY_Table1_Alarmdata_WriteAddress;
        //temp=readaddressPT&0x000000ff;
        //AT25_WriteByte(temp,210);

        //temp=(writeaddressPT>>8)&0x000000ff;
        //AT25_WriteByte(temp,211);

        //temp=(writeaddressPT>>16)&0x000000ff;
        //AT25_WriteByte(temp,212);


        //������ı�д��ַָ��
        writeaddressPT=PT_writeaddress+number_single*10;
        MY_Table1_Alarmdata_WriteAddress=writeaddressPT;


        //����д��ַָ��д�뵽EEPROM��
        //my_val_writeto_eeprom(tempbuf, startaddressPT, endaddressPT, writeaddressPT, readaddressPT,213);
        temp=writeaddressPT&0x000000ff;
        AT25_WriteByte(temp,207);

        temp=(writeaddressPT>>8)&0x000000ff;
        AT25_WriteByte(temp,208);

        temp=(writeaddressPT>>16)&0x000000ff;
        AT25_WriteByte(temp,209);


        //���������з���״̬����
        //MY_433_ALarmdata_Time_status=0;  //�������� ���� EEPROM �������
        //MY_433_ALarmdata_NOtime_status=0;
        MY_Table1_Newdata_status=1;


    }

}


/*
���ܣ�������1����ʼ��ַstartaddress1��ʼ��N�����ݣ����䵽����2��
*/
void my_buf1_to_buf2(uint8_t *source_buf,uint8_t startaddress1,uint8_t *direc_buf,uint8_t startaddress2,uint8_t number)
{
    uint8_t k=0;
    for(k=0; k<number; k++)
    {
        direc_buf[k+startaddress2]=source_buf[k+startaddress1];
    }



}

/*
���ܣ�EEPROM���������ݵ� �ض�������
��ʽ��

*/



uint8_t MY_GPRS_MCU_RsBuf[8];  //�洢�������Ե�ص�ѹ��̫���ܵ�ѹ���¶ȡ�ʪ�ȹ�4��8���ֽڵ�����

uint8_t MY_GPRS_Call_Single_data_buf[40]= {0};
uint8_t MY_GPRS_Call_Analog_data_buf[110]= {0};
uint8_t MY_GPRS_Call_Time_data_buf[7]= {0};
uint8_t MY_GPRS_Call_Single_data_number=0X12;  //18����Ϣ�壬6*3�����Դ�3��֧�ߣ�ÿ����Ϣ��1���ֽ�
uint8_t MY_GPRS_Call_Analog_data_number=0X24;  // 36  36/3=12  12/3=4  ÿ��ָʾ��4�����ݣ��������糡���¶ȡ��ڲ���ѹ����36����Ϣ�壬ÿ����Ϣ��3���ֽ�
uint8_t MY_GPRS_Call_Status=0;    //�洢�����������״̬��Ϊ1��ʾ���������ݣ�Ϊ0��ʾû����������

uint8_t MY_EEPROM_Buf[256]= {0}; //�����洢EEPROM�У�����������
uint8_t MY_GPRS_Cycle_Transmintdata_status=0;  //�����洢��ȡ���������ݷ���״̬��1Ϊ�ѷ��ͣ�2Ϊδ����

uint8_t my_eeprom_data_to_cycle_array(void) //EEPROM���������ݵ� �ض�����
{

    uint32_t startaddressPT=MY_Table2_Cycledata_StartAddress;
    uint32_t endaddressPT=MY_Table2_Cycledata_EndAddress;
    uint32_t writeaddressPT=MY_Table2_Cycledata_WriteAddress;
    uint32_t readaddressPT=MY_Table2_Cycledata_ReadAddress;
    uint16_t number_single=0;
    uint16_t number_anlog=0;
    uint8_t tempbuf[12]= {0};
    uint32_t my_int1=0;
    uint32_t my_int2=0;
    uint8_t rs_temp_buf[512]= {0}; //����ʹ�ã�@@@
    //uint8_t *PT_buf;
//	uint32_t PT_writeaddress=0;
//	uint16_t PT_number=0;
//	uint8_t temp=0;

//***���������µı�ָ���ַ
    SPI_EE_BufferRead2(tempbuf,213,12);

    my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &writeaddressPT, &readaddressPT);
    MY_Table2_Cycledata_StartAddress=startaddressPT;
    MY_Table2_Cycledata_EndAddress=endaddressPT;
    MY_Table2_Cycledata_WriteAddress=writeaddressPT;
    MY_Table2_Cycledata_ReadAddress=readaddressPT;
//USART_printf(&huart3,"write add=%d  read add=%d\r\n",writeaddressPT-EEPROM_Chip_size,readaddressPT-EEPROM_Chip_size); //����ʹ��@@@@@
    my_int1=(writeaddressPT-EEPROM_Chip_size);
    my_int2=readaddressPT-EEPROM_Chip_size;
    USART_printf(&huart3,"write add=%d%d%d%d%d%d  read add=%d%d%d%d%d%d   block=%d \r\n",my_int1/100000,my_int1%100000/10000,my_int1%10000/1000,my_int1%1000/100,my_int1%100/10,my_int1%10
                 ,my_int2/100000,my_int2%100000/10000,my_int2%10000/1000,my_int2%1000/100,my_int2%100/10,my_int2%10,(my_int1-my_int2)/144);

    if(readaddressPT!=writeaddressPT)
    {
        SPI_EE_BufferRead2(MY_EEPROM_Buf,readaddressPT,160); //���ö�ָ�룬�������µ�����

        MY_GPRS_Cycle_Transmintdata_status=MY_EEPROM_Buf[0];
        number_single=MY_EEPROM_Buf[1];
        number_anlog=MY_EEPROM_Buf[2];   //
        MY_GPRS_Call_Single_data_number=number_single;
        MY_GPRS_Call_Analog_data_number=number_anlog;

//��ָ��ǿ��У�黷�ڣ������ȡEEPROM�������Ǵ���ģ������Ϲ����򷵻�0.  2015-11.16
        if((MY_GPRS_Cycle_Transmintdata_status!=1 && MY_GPRS_Cycle_Transmintdata_status!=2)
                || (MY_GPRS_Call_Single_data_number!=18)
                || (MY_GPRS_Call_Analog_data_number!=36))
        {
            MY_GPRS_Call_Status=0;
            USART_printf(&huart3,"Read data from eeprom first 3 bytes is error!!\r\n");
            return MY_GPRS_Call_Status;
        }


        number_single=0x12; 	//ǿ��������18����Ϣ��
        number_anlog=0x24;   //ǿ��������36����Ϣ��


        MY_GPRS_Call_Single_data_number=number_single;
        MY_GPRS_Call_Analog_data_number=number_anlog;

        my_buf1_to_buf2(MY_EEPROM_Buf,3,MY_GPRS_MCU_RsBuf,0,8); //��������
        my_buf1_to_buf2(MY_EEPROM_Buf,11,MY_GPRS_Call_Single_data_buf,0,number_single);  //ң������
        my_buf1_to_buf2(MY_EEPROM_Buf,11+number_single,MY_GPRS_Call_Analog_data_buf,0,number_anlog*3); //ң������
        my_buf1_to_buf2(MY_EEPROM_Buf,11+number_single+number_anlog*3,MY_GPRS_Call_Time_data_buf,0,7);  //ʱ������

        //����ʹ�ã���ʾ��������@@@@@
        USART_printf(&huart3,"Read data address=%d%d%d%d%d%d\r\n",(readaddressPT-EEPROM_Chip_size)/100000,
                     (readaddressPT-EEPROM_Chip_size)%100000/10000,(readaddressPT-EEPROM_Chip_size)%10000/1000,
                     (readaddressPT-EEPROM_Chip_size)%1000/100,(readaddressPT-EEPROM_Chip_size)%100/10,
                     (readaddressPT-EEPROM_Chip_size)%10);
        HexToStr2(rs_temp_buf,MY_EEPROM_Buf,160);
        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");
        /*
        USART_printf(&huart3,"sing number=%d  analog number=%d\r\n",number_single,number_anlog);
        HexToStr2(rs_temp_buf,MY_GPRS_MCU_RsBuf,8);
        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");
        HexToStr2(rs_temp_buf,MY_GPRS_Call_Single_data_buf,18);
        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");
        HexToStr2(rs_temp_buf,MY_GPRS_Call_Analog_data_buf,108);
        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");
        HexToStr2(rs_temp_buf,MY_GPRS_Call_Time_data_buf,7);
        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");
        */

        //------------------------------------------

        //-------------------
        MY_GPRS_Call_Status=1;
    }
    else MY_GPRS_Call_Status=0;


    return MY_GPRS_Call_Status;

}



/*
���ܣ�EEPROM�б������ݴ��뵽�ض�������

*/

uint8_t MY_GPRS_Alarmdata_TimeBuf[256]= {0}; //�洢����ʱ�꣬��������
uint8_t MY_GPRS_ALarmdata_NOtime_status=0; //Ϊ1����ʾ�յ���ʱ�걨������

uint8_t MY_GPRS_ALarmdata_Time_status=0;   //Ϊ1����ʾ�յ���ʱ�걨������
uint8_t MY_GPRS_ALarmdata_number=0;  // �洢��������Ϣ�����
uint8_t	MY_GPRS_ALarm_Transmintdata_status=0;

uint8_t my_eeprom_data_to_alarm_array(void)//EEPROM�б������ݵ� �ض�������
{
    uint32_t startaddressPT=MY_Table1_Alarmdata_StartAddress;
    uint32_t endaddressPT=MY_Table1_Alarmdata_EndAddress;
    uint32_t writeaddressPT=MY_Table1_Alarmdata_WriteAddress;
    uint32_t readaddressPT=MY_Table1_Alarmdata_ReadAddress;
    uint16_t number_alarm=0;
    uint8_t tempbuf[12]= {0};
//	uint8_t *PT_buf;
//	uint32_t PT_writeaddress=0;
//	uint16_t PT_number=0;
//	uint8_t temp=0;

    //***���������µı�ָ���ַ
    SPI_EE_BufferRead2(tempbuf,201,12);

    my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &writeaddressPT, &readaddressPT);
    MY_Table1_Alarmdata_StartAddress=startaddressPT;
    MY_Table1_Alarmdata_EndAddress=endaddressPT;
    MY_Table1_Alarmdata_WriteAddress=writeaddressPT;
    MY_Table1_Alarmdata_ReadAddress=readaddressPT;

    if(writeaddressPT!=readaddressPT)
    {
        SPI_EE_BufferRead2(MY_EEPROM_Buf,readaddressPT,256); //���ö�ָ�룬�������µ�����
        MY_GPRS_ALarm_Transmintdata_status=MY_EEPROM_Buf[0];  //��GPRS����״̬
        number_alarm=MY_EEPROM_Buf[1];
        MY_GPRS_ALarmdata_number=number_alarm;
        my_buf1_to_buf2(MY_EEPROM_Buf,2,MY_GPRS_Alarmdata_TimeBuf,0,number_alarm*10); //��ʱ��ı������ݣ�ÿ������10���ֽ�

        //--------------------
        MY_GPRS_ALarmdata_Time_status=1;
    }
    else MY_GPRS_ALarmdata_Time_status=0;




    return MY_GPRS_ALarmdata_Time_status;

}

/*

���ܣ��ƶ���Ķ�ָ�룬ͬʱ�޸��ѷ���֡��GPRS����״̬�ֽ�Ϊ1��
			��ʾ��������,transmin_OKΪ1����ʾ���ͳɹ���Ϊ0��ʾ����ʧ��
*/
void my_GPRS_chang_tablereadpt(uint8_t table_number,uint8_t transmint_OK)  //table_numberΪ1��ʾ������Ϊ2��ʾ�������ݱ�
{
    uint32_t startaddressPT=0;      		//MY_Table2_Cycledata_StartAddress;
    uint32_t endaddressPT=0;        	 //MY_Table2_Cycledata_EndAddress;
    uint32_t temp_writeaddressPT=0;      	 //MY_Table2_Cycledata_WriteAddress;
    uint32_t temp_readaddressPT=0;  			//MY_Table2_Cycledata_ReadAddress;
    //uint16_t number_single=0;
    //uint16_t number_anlog=0;
    uint8_t tempbuf[12]= {0};
    uint32_t tableaddress=0;
    int length=0;

    if(table_number==1)
    {
        SPI_EE_BufferRead2(tempbuf,201,12);
        my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &temp_writeaddressPT, &temp_readaddressPT);
        tableaddress=210;

    }
    else if(table_number==2)
    {
        SPI_EE_BufferRead2(tempbuf,213,12);
        my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &temp_writeaddressPT, &temp_readaddressPT);
        tableaddress=222;

    }
    if(transmint_OK==1)
        AT25_WriteByte(0X01,temp_readaddressPT); //�޸�GPRS����״̬�ֽ�Ϊ1����ʾ��������,2��ʾû�з��ͳ�ȥ
    else if(transmint_OK==0 && table_number==1)
    {
        AT25_WriteByte(0X02,temp_readaddressPT);
        AT25_WriteByte(0X01,200);  //ROM��200��ַ�ĵ�Ԫ��ʶ���������ݷ��͵�״̬��Ϊ1��ʾ�б���������ΪGPRS����û�з��ͳ�ȥ��Ϊ0��ʾ���б������ݶ����ͳ�ȥ��
        RE_ALarmData_Status=01;
    }
    else if(transmint_OK==0 && table_number==2)
    {
        AT25_WriteByte(0X02,temp_readaddressPT);
        AT25_WriteByte(0X01,199);  //ROM��200��ַ�ĵ�Ԫ��ʶ���������ݷ��͵�״̬��Ϊ1��ʾ�б���������ΪGPRS����û�з��ͳ�ȥ��Ϊ0��ʾ���б������ݶ����ͳ�ȥ��
        RE_CycleData_Status=01;
    }

    //���������ݳɹ��˾ͽ��ж�ָ���ƶ������û�ɹ����ƶ�����

    if(transmint_OK==1 && table_number==1)
    {

        length=endaddressPT-temp_readaddressPT+1;  //�ж��Ƿ��˴洢����ĩβ
        if(length>12)    //�������12��ʾ��û��ĩβ  //ÿ����Ϣ��ռ10���ֽڣ���ַ2����ֵһ����ʱ��7������һ��״̬��һ������
        {
            SPI_EE_BufferRead2(tempbuf,temp_readaddressPT,12);  //��ȡһ֡������
            if(tempbuf[0]==0x00&&tempbuf[1]==00)
                temp_readaddressPT=startaddressPT;  //�����Ϊ00��˵��ĩβ��û�������ˣ����Ӧ��û�ã������⣬ʵ�������ڿ���һ�£�����
            else   //��Ч���ݣ����ж�ָ���ƶ�
            {
                if(table_number==1)
                {
                    temp_readaddressPT=temp_readaddressPT+2+tempbuf[1]*10;  //�������
                }
                else if(table_number==2)
                {
                    temp_readaddressPT=temp_readaddressPT+11+tempbuf[1]+tempbuf[2]*3+7;
                }
            }
        }
        else  //���С��12��ʾ����ĩβ
        {
            temp_readaddressPT=startaddressPT;
        }
        MY_Table1_Alarmdata_ReadAddress=temp_readaddressPT;
        tempbuf[0]=temp_readaddressPT&0x0000ff;
        tempbuf[1]=(temp_readaddressPT>>8)&0x0000ff;
        tempbuf[2]=(temp_readaddressPT>>16)&0x0000ff;
        SPI_EE_BufferWrite2(tempbuf,tableaddress,3);


    }
    else if(transmint_OK==1 && table_number==2)
    {

        //SPI_EE_BufferRead2(tempbuf,temp_readaddressPT,20);  //��ȡһ֡������
        //temp_readaddressPT=temp_readaddressPT+11+tempbuf[1]+tempbuf[2]*3+7;
        temp_readaddressPT=temp_readaddressPT+144;

        length=endaddressPT-temp_readaddressPT+1;  //�ж��Ƿ��˴洢����ĩβ

        if(temp_readaddressPT>temp_writeaddressPT)
        {
            if(length<144)//�������144��ʾ��û��ĩβ3+8+18+3*9*4+7=144,С��144����β��		3��,8��DTU��4�������� ,18�Ǳ���(2��������9��ָʾ��)
                temp_readaddressPT=startaddressPT;  //û������
        }

        MY_Table2_Cycledata_ReadAddress=temp_readaddressPT;
        tempbuf[0]=temp_readaddressPT&0x0000ff;
        tempbuf[1]=(temp_readaddressPT>>8)&0x0000ff;
        tempbuf[2]=(temp_readaddressPT>>16)&0x0000ff;
        SPI_EE_BufferWrite2(tempbuf,tableaddress,3);

    }


}


/*
���ܣ��޸�֡���͵�Ԫ״̬���������ˣ���2��Ϊ1��������
*/
void my_GPRS_chang_Transmitword_status(uint8_t table_number)  //table_numberΪ1��ʾ������Ϊ2��ʾ�������ݱ�
{
    uint32_t startaddressPT=0;      		//MY_Table2_Cycledata_StartAddress;
    uint32_t endaddressPT=0;        	 //MY_Table2_Cycledata_EndAddress;
    uint32_t writeaddressPT=0;      	 //MY_Table2_Cycledata_WriteAddress;
    uint32_t readaddressPT=0;  				//MY_Table2_Cycledata_ReadAddress;
    //uint16_t number_single=0;
    //uint16_t number_anlog=0;
    uint8_t tempbuf[12]= {0};
//	uint32_t tableaddress=0;
//	uint8_t length=0;

    if(table_number==1)
    {
        SPI_EE_BufferRead2(tempbuf,201,12);
        my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &writeaddressPT, &readaddressPT);
        //tableaddress=210;

    }
    else if(table_number==2)
    {
        SPI_EE_BufferRead2(tempbuf,213,12);

        my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &writeaddressPT, &readaddressPT);
// tableaddress=222;

    }
    AT25_WriteByte(0X01,readaddressPT); //�޸�GPRS����״̬�ֽ�Ϊ1����ʾ��������,��������⣬���û�з��ͳ�ȥ��������Ҫ�ƶ���ָ��
}






/*
���ܣ�MCU��������ʼ����Ҫ�̶�������DTU��ַ��Server��IP��ַ�Ͷ˿ںţ� �绰�š�Server����ʱ�䡢Server��������ʱ��
*/

extern uint16_t Transmint_to_GPRS_hearttime;
extern uint16_t Transmint_to_GPRS_Cycledata;

extern uint8_t MY_IP[4]; //222.222.118.3
extern uint16_t MY_PORT;  //8080  16λ����
extern uint16_t DTU_ADDRESS;
//extern uint8_t AT_MESS_telphonenumber[100];
//extern uint8_t AT_MESS_telphonenumber2[50];
uint8_t FLASH_DTU=1;  //FLASH_DTU���״̬��Ϊ1��ʾ���DTU����EEPROM��ַ��Ϊ0��ʾ�����EEPROM��ַ
extern uint8_t my_sys_init_flash;
void MY_Value_init(void)
{

    //uint8_t k=0;
    uint16_t my_temp_val16=0;
    uint8_t my_buf[8]= {0};
    uint8_t my_status=0;


    uint32_t startaddressPT=MY_Table2_Cycledata_StartAddress;
    uint32_t endaddressPT=MY_Table2_Cycledata_EndAddress;
    uint32_t writeaddressPT=MY_Table2_Cycledata_WriteAddress;
    uint32_t readaddressPT=MY_Table2_Cycledata_ReadAddress;
    uint8_t tempbuf[12]= {0};
    float temp_t1=0;//temp_t2=0;

    //=============

//    my_buf[0]=AT25_ReadByte(EEPROM_sys_start_status);
//    my_buf[1]=AT25_ReadByte(EEPROM_sys_start_status+1);
//    my_temp_val16=my_buf[1];
//    my_temp_val16=(my_temp_val16<<8)+ my_buf[0];
//    if(my_temp_val16==0xCDAB) //�ǵ�һ���ϵ�
//    {
//        //FLASH_DTU=0;  //�ǵ�һ���ϵ�
//        //my_sys_init_flash=0;
//			  //my_sys_init_flash=1; //��һ���ϵ�
//        FLASH_DTU=1;
//    }
//    else
//    {
//        //my_sys_init_flash=1; //��һ���ϵ�
//        FLASH_DTU=1;
//    }
    //=================
//����ڴ����ݲ��֣�����DTUΪ0X0000
    

    if(my_sys_init_flash==1)
    {
        my_status=2;  //��һ���ϵ磬д��Ĭ��ֵ
    }
    else my_status=1;  //�ǵ�һ���ϵ磬��ȡEEPROMֵ��������

    if(my_status==1)
    {

        //(1)�Ѷ�������DTU��ַд�뵽������
        my_buf[0]=AT25_ReadByte(EEPROM_DTU_Address);
        my_buf[1]=AT25_ReadByte(EEPROM_DTU_Address+1);
        my_temp_val16=my_buf[1];
        my_temp_val16=((my_temp_val16<<8)&0xff00)+my_buf[0];
        DTU_ADDRESS=my_temp_val16;

        //(2)Server������ʱ��
        my_buf[0]=AT25_ReadByte(EEPROM_Hearttime_Address);
        my_buf[1]=AT25_ReadByte(EEPROM_Hearttime_Address+1);
        my_temp_val16=my_buf[1];
        my_temp_val16=((my_temp_val16<<8)&0xff00)+my_buf[0];

        //Transmint_to_GPRS_hearttime=my_temp_val16;
        if(my_temp_val16>60 && my_temp_val16<60*10)
            MY_M_speed_heart=my_temp_val16;


        //��3��Server��������ʱ��

        my_buf[0]=AT25_ReadByte(EEPROM_Cycetime_Address);
        my_buf[1]=AT25_ReadByte(EEPROM_Cycetime_Address+1);
        my_temp_val16=my_buf[1];
        my_temp_val16=((my_temp_val16<<8)&0xff00)+my_buf[0];
        //Transmint_to_GPRS_Cycledata=my_temp_val16;
        if(my_temp_val16>60*3 && my_temp_val16<60*60)
            MY_M_speed_cyc=my_temp_val16;

        //��ȡ��ֵ������

        my_buf[0]=AT25_ReadByte(EEPROM_SPEED_Gate_H_Address);
        my_buf[1]=AT25_ReadByte(EEPROM_SPEED_Gate_H_Address+1);
        my_temp_val16=my_buf[1];
        my_temp_val16=((my_temp_val16<<8)&0xff00)+my_buf[0];
        //Transmint_to_GPRS_hearttime=my_temp_val16;
        temp_t1=my_temp_val16/10.0;
        MY_Speed_H_Gate=temp_t1;

        //��ȡ���� ����
        my_buf[0]=AT25_ReadByte(EEPROM_SPEED_H_Cyc_Address);
        my_buf[1]=AT25_ReadByte(EEPROM_SPEED_H_Cyc_Address+1);
        my_temp_val16=my_buf[1];
        my_temp_val16=((my_temp_val16<<8)&0xff00)+my_buf[0];
        if(my_temp_val16>60*1 && my_temp_val16<60*60)
            MY_H_speed_cyc=my_temp_val16;


        //��ȡ���� ����
        my_buf[0]=AT25_ReadByte(EEPROM_SPEED_L_Cyc_Address);
        my_buf[1]=AT25_ReadByte(EEPROM_SPEED_L_Cyc_Address+1);
        my_temp_val16=my_buf[1];
        my_temp_val16=((my_temp_val16<<8)&0xff00)+my_buf[0];
        if(my_temp_val16>60*1 && my_temp_val16<60*60)
            MY_L_speed_cyc=my_temp_val16;




        //��4��Server��IP��ַ���˿ں�
        SPI_EE_BufferRead2(MY_IP,EEPROM_IP_Address,4);  //��ȡEEPROM�еĵ�ַ��������
        my_buf[0]=AT25_ReadByte(EEPROM_IPPort_Address);
        my_buf[1]=AT25_ReadByte(EEPROM_IPPort_Address+1);
        my_temp_val16=my_buf[1];
        my_temp_val16=((my_temp_val16<<8)&0xff00)+my_buf[0];
        MY_PORT=my_temp_val16;   //�˿ں�

        //(7)��ȡ�������ݲ���״̬
        RE_ALarmData_Status=AT25_ReadByte(EEPROM_RE_Alarmdata_t_status);


    }
    else if(my_status==2)  //��һ������
    {
        //��Ĭ��ֵд�뵽EEPROM��
        //(1) DTU
        my_temp_val16=DTU_ADDRESS;      //Ĭ��ֵΪ0001
        my_buf[0]=my_temp_val16&0x00ff;
        my_buf[1]=(my_temp_val16>>8)&0x00ff;
        if(my_temp_val16!=0x0000)
        {
            AT25_WriteByte(my_buf[0],EEPROM_DTU_Address);
            AT25_WriteByte(my_buf[1],EEPROM_DTU_Address+1);
        }
        //(2)������ʱ��
        //my_temp_val16=Transmint_to_GPRS_hearttime;  //Ĭ��ֵΪ10��ʱ��Ϊ10*5S=50��
        my_temp_val16=MY_M_speed_heart;
        my_buf[0]=my_temp_val16&0x00ff;
        my_buf[1]=(my_temp_val16>>8)&0x00ff;
        AT25_WriteByte(my_buf[0],EEPROM_Hearttime_Address);
        AT25_WriteByte(my_buf[1],EEPROM_Hearttime_Address+1);


        //��3��Server��������ʱ��

        //my_temp_val16=Transmint_to_GPRS_Cycledata;  //Ĭ��ֵΪ24��ʱ��Ϊ10*5S=120��
        my_temp_val16=MY_M_speed_cyc;
        my_buf[0]=my_temp_val16&0x00ff;
        my_buf[1]=(my_temp_val16>>8)&0x00ff;
        AT25_WriteByte(my_buf[0],EEPROM_Cycetime_Address);
        AT25_WriteByte(my_buf[1],EEPROM_Cycetime_Address+1);

        //���޷�ֵ������
        my_temp_val16=(uint16_t)(MY_Speed_H_Gate*10);  //����
        my_buf[0]=my_temp_val16&0x00ff;
        my_buf[1]=(my_temp_val16>>8)&0x00ff;
        AT25_WriteByte(my_buf[0],EEPROM_SPEED_Gate_H_Address);
        AT25_WriteByte(my_buf[1],EEPROM_SPEED_Gate_H_Address+1);

        //��������
        my_temp_val16=MY_H_speed_cyc;
        my_buf[0]=my_temp_val16&0x00ff;
        my_buf[1]=(my_temp_val16>>8)&0x00ff;
        AT25_WriteByte(my_buf[0],EEPROM_SPEED_H_Cyc_Address);
        AT25_WriteByte(my_buf[1],EEPROM_SPEED_H_Cyc_Address+1);



        //��������
        my_temp_val16=MY_L_speed_cyc;
        my_buf[0]=my_temp_val16&0x00ff;
        my_buf[1]=(my_temp_val16>>8)&0x00ff;
        AT25_WriteByte(my_buf[0],EEPROM_SPEED_L_Cyc_Address);
        AT25_WriteByte(my_buf[1],EEPROM_SPEED_L_Cyc_Address+1);

        //��4��Server��IP��ַ���˿ں�

        SPI_EE_BufferWrite2(MY_IP,EEPROM_IP_Address,4); //��Ĭ��IP��ַ д�뵽EEPROM�� 222.222.118.3������ʮ���Ƶ�

        my_temp_val16=MY_PORT;    //Ĭ�϶˿ںţ�8080,����ʮ���Ƶ�
        my_buf[0]=my_temp_val16&0x00ff;
        my_buf[1]=(my_temp_val16>>8)&0x00ff;
        AT25_WriteByte(my_buf[0],EEPROM_IPPort_Address);
        AT25_WriteByte(my_buf[1],EEPROM_IPPort_Address+1);

        //(7) ������������״̬
        RE_ALarmData_Status=0;
        AT25_WriteByte(RE_ALarmData_Status,200);
        //(8)�������ݱ�
        startaddressPT=MY_Table1_Alarmdata_StartAddress;
        endaddressPT=MY_Table1_Alarmdata_EndAddress;
        writeaddressPT=MY_Table1_Alarmdata_WriteAddress;
        readaddressPT=MY_Table1_Alarmdata_ReadAddress;
        my_val_writeto_eeprom(tempbuf, startaddressPT, endaddressPT, writeaddressPT, readaddressPT,EEPROM_Table1_Address);

        //(9)�������ݱ�
        startaddressPT=MY_Table2_Cycledata_StartAddress;
        endaddressPT=MY_Table2_Cycledata_EndAddress;
        writeaddressPT=MY_Table2_Cycledata_WriteAddress;
        readaddressPT=MY_Table2_Cycledata_ReadAddress;
        my_val_writeto_eeprom(tempbuf, startaddressPT, endaddressPT, writeaddressPT, readaddressPT,EEPROM_Table2_Address);  //�ѳ�ʼ���ַд�뵽EEPROM��



    }


}



//--------------����ʹ�ó���-----------
/*
���ܣ���EEPRom�е�����������
*/
void my_test_Eepromtable_zero(void)
{
    uint8_t tempbuf[24]= {0};

    SPI_EE_BufferWrite2(tempbuf,201,24);
}




/*
���ܣ����Գ��򣬶�ң�š�ң�⡢�����洢������и�ֵ
*/
uint8_t temp=0x00; //��������������ģ������
void my_test_analoydata_array_to_eeprom(void)
{
    uint8_t k=0;



    //ң��
    MY_433_Call_Single_data_number=2;
    for(k=0; k<MY_433_Call_Single_data_number; k++)
    {
        temp=temp+1;
        MY_433_Call_Single_data_buf[k]=temp;
    }

    //ң��
    MY_433_Call_Analog_data_number=4;
    for(k=0; k<MY_433_Call_Analog_data_number*3; k++)
    {
        temp=temp+1;
        MY_433_Call_Analog_data_buf[k]=temp;
    }


    //����
    for(k=0; k<4*2; k++)
    {

        temp=temp+1;
        MY_MCU_RsBuf[k]=temp;
    }

    //ʱ��
    for(k=0; k<7; k++)
    {
        temp=temp+1;
        MY_433_Call_Time_data_buf[k]=temp;
    }

    MY_MCU_getdata_status=1;
    MY_433_Call_Status=1;

}

//���ܣ�����ʹ�ã���table2�������ݣ��Ķ�дָ����г�ʼ��Ϊ0x2000
void my_test_TABLE2_init()
{
    uint8_t tempbuf[12]= {0};
    uint32_t startaddressPT=MY_Table2_Cycledata_StartAddress;
    uint32_t endaddressPT=MY_Table2_Cycledata_EndAddress;
    uint32_t writeaddressPT=MY_Table2_Cycledata_WriteAddress;
    uint32_t readaddressPT=MY_Table2_Cycledata_ReadAddress;
    //uint32_t tableaddress=0;


    my_val_writeto_eeprom(tempbuf, startaddressPT, endaddressPT, writeaddressPT, readaddressPT,EEPROM_Table2_Address);  //�ѳ�ʼ���ַд�뵽EEPROM��

}

//-----------����ʹ�ó���------------
//��������һ�� ���Ŀ���ַ�������2��16�����ַ�������3��16�����ֽ�����
void HexToStr2(uint8_t *pbDest, uint8_t *pbSrc, int nLen)
{
    uint8_t	ddl,ddh;
    int i;

//if(pbSrc[0]==0x10) nLen=6;
//else if(pbSrc[0]==0x68)nLen=6+pbSrc[1];

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

