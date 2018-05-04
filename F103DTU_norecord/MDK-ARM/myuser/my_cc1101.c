#include "my_cc1101.h"
#include "cmsis_os.h"
#include "my_def_value.h"
#include "my_globle_extern.h"
#include "math.h"




#define RF_reg_count 47
uint16_t my_CC1101_sync_word = 0x0102; //CC1101ͬ���֣�Ĭ��Ϊ0x8799
#define my_CC1101_default_channer 0x02   //оƬĬ��CC1101�ŵ�
uint8_t my_CC1101_chip_address = 0xFE; //оƬ��ַ��0XFEλDTU��ַ��0XFDΪ��������ַ��01 02 03Ϊָʾ����ַ
//ָʾ����ַ���ɣ�ͬ���ֵ��ֽ�+ͬ���ָ��ֽ�+�ŵ��ֽ�+�ŵ��ڵ�ַ�ֽ�(01��02��03)
//���� 01-02-02-FE


uint8_t my_cc1101_dest_address = 0x01;
uint8_t my_CC1101_change_channer = my_CC1101_default_channer; //��̬�ŵ���



//10, 7, 5, 0, -5, -10, -15, -20, dbm output power, 0x12 == -30dbm
//INT8U PaTabel[] = { 0xc0, 0xC8, 0x84, 0x60, 0x68, 0x34, 0x1D, 0x0E};
INT8U PaTabel[] = { 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0};


extern uint8_t MY_CC1101_STATUS;
extern uint8_t Cmd_CC1101_RX[];
extern osMutexId myMutex01Handle;
extern EventGroupHandle_t xCreatedEventGroup ;

static const uint8_t CC1101InitData[RF_reg_count][2] =
{   /*
    {CC1101_IOCFG2,      0x0E},  //
    {CC1101_IOCFG1,      0x2E},
    {CC1101_IOCFG0,      0x07},  //
    {CC1101_FIFOTHR,     0x47},  //Ĭ��47���������
    {CC1101_PKTCTRL1,    0x06},  //06
    {CC1101_PKTCTRL0,    0x05},  //
    {CC1101_CHANNR,      0x01},  //
    {CC1101_FSCTRL1,     0x06},  //
    {CC1101_FREQ2,       0x0F},
    {CC1101_FREQ1,       0x62},
    {CC1101_FREQ0,       0x76},
    {CC1101_MDMCFG4,     0xF6},
    {CC1101_MDMCFG3,     0x43},
    {CC1101_MDMCFG2,     0x13},
    {CC1101_DEVIATN,     0x15},
    {CC1101_MCSM0,       0x18},  //
    {CC1101_FOCCFG,      0x16},
    {CC1101_WORCTRL,     0xFB},  //
    {CC1101_FSCAL3,      0xE9},
    {CC1101_FSCAL2,      0x2A},
    {CC1101_FSCAL1,      0x00},
    {CC1101_FSCAL0,      0x1F},
    {CC1101_TEST2,       0x81},
    {CC1101_TEST1,       0x35},
    {CC1101_MCSM1,       0x3B},//============
    */
    {CC1101_IOCFG2,      0x0E},
    {CC1101_IOCFG1,      0x2E},
    {CC1101_IOCFG0,      0x06},  //0X06��������ʱΪ0��������λ�ߵ�ƽ��0X46��������Ϊ�ߵ�ƽ��������λ�͵�ƽ
    {CC1101_FIFOTHR,     0x4E},
    {CC1101_SYNC1,       0x87},
    {CC1101_SYNC0,       0x99},
    {CC1101_PKTLEN,      0xFF},
    {CC1101_PKTCTRL1,    0x06},  //0x06������ַУ�飬04����������ַУ��
    {CC1101_PKTCTRL0,    0x05},  //05 0000 0101
    {CC1101_ADDR,        0x05},
    {CC1101_CHANNR,      my_CC1101_default_channer},//�ŵ�
    {CC1101_FSCTRL1,     0x06},
    {CC1101_FSCTRL0,     0x00},
    {CC1101_FREQ2,       0x0F},
    {CC1101_FREQ1,       0x62},
    {CC1101_FREQ0,       0x76},
    {CC1101_MDMCFG4,     0xFA},  //2.4K-0xF6 50K-0XFA 60k-FB
    {CC1101_MDMCFG3,     0x93},  //2.4K-0x83 50K-0XF8 60K-2E
    {CC1101_MDMCFG2,     0x13},  //Ĭ��Ϊ0X13
    {CC1101_MDMCFG1,     0x22},
    {CC1101_MDMCFG0,     0xF8},
    {CC1101_DEVIATN,     0x15},
    {CC1101_MCSM2,       0x07},
    {CC1101_MCSM1,       0x30},
    {CC1101_MCSM0,       0x18},
    {CC1101_FOCCFG,      0x16},
    {CC1101_BSCFG,       0x6C},
    {CC1101_AGCCTRL2,    0x03},
    {CC1101_AGCCTRL1,    0x40},
    {CC1101_AGCCTRL0,    0x91},
    {CC1101_WOREVT1,     0x87},
    {CC1101_WOREVT0,     0x6B},
    {CC1101_WORCTRL,     0xFB},
    {CC1101_FREND1,      0x56},
    {CC1101_FREND0,      0x10},
    {CC1101_FSCAL3,      0xE9},
    {CC1101_FSCAL2,      0x2A},
    {CC1101_FSCAL1,      0x00},
    {CC1101_FSCAL0,      0x1F},
    {CC1101_RCCTRL1,     0x41},
    {CC1101_RCCTRL0,     0x00},
    {CC1101_FSTEST,      0x59},
    {CC1101_PTEST,       0x7F},
    {CC1101_AGCTEST,     0x3F},
    {CC1101_TEST2,       0x81},
    {CC1101_TEST1,       0x35},
    {CC1101_TEST0,       0x0B},
};


// ������ر���
INT8U   COM_TxNeed = 0;
INT8U   COM_TimeOut = 0;
INT8U   COM_RxCounter = 0;
INT8U   COM_TxCounter = 0;
INT8U   COM_RxBuffer[65] = {0};
INT8U   COM_TxBuffer[65] = {0};

/*===========================================================================
* ���� ��RF_RecvHandler() => �������ݽ��մ���                               *
============================================================================*/
//void RF_RecvHandler(void)
//{
//    INT8U length = 0, recv_buffer[64] = {0};
//    uint8_t i = 0;

//    //__HAL_GPIO_EXTI_CLEAR_IT(PIN_CC_IRQ); //�����������ⲿ�жϣ�ÿ���жϵ������������һ������Ҫ�ֱ����
//    // HAL_NVIC_EnableIRQ(EXIT_CC_IRQ_GD0);
//    CC1101SetTRMode(RX_MODE);           // �������ģʽ



//    if (1 == CC_IRQ_READ())             // �������ģ���Ƿ���������ж�
//    {
//        while (CC_IRQ_READ() == 1);

//        // ��ȡ���յ������ݳ��Ⱥ���������
//        length = CC1101RecPacket(recv_buffer);

//        // �жϽ��������Ƿ���ȷ
//        if (length <= SEND_MAX && length > 0)
//        {
//            LED3_ON;               // LED��˸������ָʾ�յ�����
//            for(i = 0; i < length; i++)
//            {
//                printf("[%XH]--", recv_buffer[i]);
//            }
//            printf("\n");
//            //printf("%s\n",recv_buffer);
//            LED3_OFF;

//        }
//    }
//}

/*===========================================================================
* ���� : BSP_RF_SendPacket() => ���߷������ݺ��������ڽ������ݣ�ת����CC1101                        *
============================================================================*/
void RF_SendPacket(void)
{
    INT8U i = 0, j = 0, length = 0, buffer[65] = {0};

    if ((0 == COM_TimeOut) && (COM_RxCounter > 0) )
    {
        if(MY_CC1101_STATUS == 0x01 || MY_CC1101_STATUS == 0x02) return;
        else MY_CC1101_STATUS = 0X01;

        //HAL_NVIC_DisableIRQ(EXIT_CC_IRQ_GD0); //�ر��жϣ�����Ӧ�����ж�
        CC1101SetIdle();

        //LED3_ON;

        length = COM_RxCounter;

        COM_RxCounter = 0;

        for (i = 0; i < length; i++)   {
            buffer[i] = COM_RxBuffer[i];
        }

        //CC1101SendPacket(buffer, length, ADDRESS_CHECK);    // �������ݣ�Ĭ�ϵ���ַ���ͷ�ʽ
        for(j = 0; j <= 0x6; j++)			 //ע�⣬Ƶ�ʵļ��㷽������Ƶ+ͨ����*�����������Ҫ���ǣ��п��ܳ���CC1101��Ƶ�ʷ�Χ������CC1101����
        {
            my_CC1101_change_channer = j;
            CC1101WriteReg(CC1101_CHANNR, my_CC1101_change_channer); //�任�ŵ�
            buffer[0] = j + 0X30; //����ʹ�ã���Ϊ�ŵ��ŵı��


            for(i = 0xFA; i < 0XFF; i++) //���ַ���ͣ�ע��0��ַ�ǹ㲥��ַ,�任��ַ����
            {
                buffer[length] = '-';
                buffer[length + 1] = i - 0XFA + 0X30;
                buffer[length + 2] = '\n';
                CC1101SendPacket_add(buffer, length + 3, ADDRESS_CHECK, i);
            }
        }

        //	HAL_NVIC_ClearPendingIRQ(EXIT_CC_IRQ_GD0); //�����ʷ�ж�������Ϣ EXIT_CC_IRQ_GD0������������ã���֪��Ϊʲô


        // __HAL_GPIO_EXTI_CLEAR_IT(PIN_CC_IRQ); //�����������ⲿ�жϣ�ÿ���жϵ������������һ������Ҫ�ֱ����
        //	HAL_NVIC_EnableIRQ(EXIT_CC_IRQ_GD0);


        // CC1101SetTRMode(RX_MODE);                           // �������ģʽ

        printf("USART TO CC1101\n");

        MY_CC1101_STATUS = 0x00;
    }
}






/*===========================================================================
* ���� ��RF_Initial() => ��ʼ��RFоƬ                                       *
* ˵�� ��CC1101�Ĳ������Ѿ�������C�⣬��CC1101.c�ļ��� �ṩSPI��CSN������	*
         ���ɵ������ڲ����к����û������ٹ���CC1101�ļĴ����������⡣       *
============================================================================*/
void RF_Initial(void)
{
    CC1101Init();               // ��ʼ��CC1101�Ĵ���
    CC1101SetTRMode(RX_MODE);   // ����ģʽ
}

/*
================================================================================
Function : CC1101RecPacket( )
    Receive a packet
INPUT    : rxBuffer, A buffer store the received data
OUTPUT   : 1:received count, 0:no data
================================================================================
*/
INT8U RS_buf_status[2] = {0}; //�����洢���յ������ݵ�RSSI��crc���
uint8_t my_RSSI_dbm_all=0;  //�����洢���յ�CC1101���ź�ǿ�ȣ����յ����������

INT8U CC1101RecPacket( INT8U *rxBuffer )
{

    INT8U pktLen;
//    INT16U x;
    //j = 0;

    if ( CC1101GetRXCnt( ) != 0 )
    {
        pktLen = CC1101ReadReg(CC1101_RXFIFO);           // Read length byte

        if( pktLen == 0 || pktLen>60)
        {

            printf("CC1101 get length [%d] data!!error\r\n",pktLen);
            CC1101ClrRXBuff( );
            return 0;
        }
        else
        {
            ;
        } // { pktLen --; }
        CC1101ReadMultiReg(CC1101_RXFIFO, rxBuffer, pktLen); // Pull data
        CC1101ReadMultiReg(CC1101_RXFIFO, RS_buf_status, 2);   // Read  status bytes
        //my_fun_display_buf_16(rxBuffer,pktLen+2);//��ʾ���յ�������

        CC1101ClrRXBuff( );


        if( RS_buf_status[1] & CRC_OK )
        {
            int my_rssi_dbm = 0;
            my_rssi_dbm = RS_buf_status[0]; //RSSI
					
            if(my_rssi_dbm >= 128)
                my_rssi_dbm = (my_rssi_dbm - 256) / 2 - 75;
            else
                my_rssi_dbm = (my_rssi_dbm) / 2 - 75;
						//�洢�ź�ǿ��
						my_RSSI_dbm_all=(int)(fabs(my_rssi_dbm));

						if(my_rssi_dbm<=-70)
            printf("*** RSSI=[%d] ***\n", my_rssi_dbm); //@@@���ֵ���ߵ�ʱ���޸�Ϊ-70�����򲻶ϵ��ظ����
            return pktLen;
        }
        else
        {
            int my_rssi_dbm = 0;
            my_rssi_dbm = RS_buf_status[0]; //RSSI
            if(my_rssi_dbm >= 128)
                my_rssi_dbm = (my_rssi_dbm - 256) / 2 - 75;
            else
                my_rssi_dbm = (my_rssi_dbm) / 2 - 75;

            my_RSSI_dbm_all=(int)(fabs(my_rssi_dbm));
            printf("CC1101 CRC error\r\n");
            printf("RSSI=[%d]--CRC=[%XH]-length=[%d]\r\n",my_rssi_dbm,RS_buf_status[1],pktLen);
            return 0;
        }
    }
    else
    {
        printf("CC1101 get cnt 0 data!!\r\n");
        return 0;    // Error
    }
}


/*
================================================================================
Function : CC1101SendPacket( )
    Send a packet
INPUT    : txbuffer, The buffer stores data to be sent
           size, How many bytes should be sent
           mode, Broadcast or address check packet
OUTPUT   : None
================================================================================
*/
//void CC1101SendPacket( INT8U *txbuffer, INT8U size, TX_DATA_MODE mode )
//{
//    INT8U address;

//    HAL_NVIC_DisableIRQ(EXIT_CC_IRQ_GD0); //�ر��жϣ������������ж�

//    if( mode == BROADCAST )             {
//        address = 0;
//    }
//    else if( mode == ADDRESS_CHECK )    {
//        address = CC1101ReadReg( CC1101_ADDR );
//    }

//    CC1101ClrTXBuff( );

//    address = my_cc1101_dest_address; //����ʹ�ã�д����շ���ַ
//    if( ( CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03 ) != 0 )
//    {
//        CC1101WriteReg( CC1101_TXFIFO, size + 1 );
//        CC1101WriteReg( CC1101_TXFIFO, address );
//    }
//    else
//    {
//        CC1101WriteReg( CC1101_TXFIFO, size );
//    }

//    CC1101WriteMultiReg( CC1101_TXFIFO, txbuffer, size );
//    CC1101SetTRMode( TX_MODE );

//    while( HAL_GPIO_ReadPin(PORT_CC_IRQ, PIN_CC_IRQ) != 1 );
//    while( HAL_GPIO_ReadPin(PORT_CC_IRQ, PIN_CC_IRQ) == 1 );
//    //HAL_Delay(1000);



//    CC1101ClrTXBuff( );  //�������FIFO
//}


/*
================================================================================
Function : CC1101ClrTXBuff( )
    Flush the TX buffer of CC1101
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101ClrTXBuff( void )
{
    CC1101SetIdle();//MUST BE IDLE MODE
    CC1101WriteCmd( CC1101_SFTX );
}

/*
================================================================================
Function : CC1101SetIdle( )
    Set the CC1101 into IDLE mode
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101SetIdle( void )
{
    CC1101WriteCmd(CC1101_SIDLE);
}

/*
================================================================================
Function : CC1101ClrRXBuff( )
    Flush the RX buffer of CC1101
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101ClrRXBuff( void )
{
    CC1101SetIdle();//MUST BE IDLE MODE
    CC1101WriteCmd( CC1101_SFRX );
}

/*
================================================================================
Function : CC1101ReadMultiReg( )
    Read some bytes from the rigisters continously
INPUT    : addr, The address of the register
           buff, The buffer stores the data
           size, How many bytes should be read
OUTPUT   : None
================================================================================
*/
void CC1101ReadMultiReg( INT8U addr, INT8U *buff, INT8U size )
{
    INT8U i, j;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr | READ_BURST);
    for( i = 0; i < size; i ++ )
    {
        for( j = 0; j < 20; j ++ );
        *( buff + i ) = SPI_ExchangeByte( 0xFF );
    }
    CC_CSN_HIGH( );
}



/*
================================================================================
Function : CC1101GetRXCnt( )
    Get received count of CC1101
INPUT    : None
OUTPUT   : How many bytes hae been received
================================================================================
*/
INT8U CC1101GetRXCnt( void )
{
    return ( CC1101ReadStatus( CC1101_RXBYTES )  & BYTES_IN_RXFIFO );
}

/*
================================================================================
Function : CC1101SetTRMode( )
    Set the device as TX mode or RX mode
INPUT    : mode selection
OUTPUT   : None
================================================================================
*/
void CC1101SetTRMode( TRMODE mode )
{
    if( mode == TX_MODE )
    {
        CC1101WriteReg(CC1101_IOCFG0, 0x06);
        CC1101WriteCmd( CC1101_STX );
    }
    else if( mode == RX_MODE )
    {
        CC1101WriteReg(CC1101_IOCFG0, 0x06);
        CC1101WriteCmd( CC1101_SRX );
    }
}



/*
================================================================================
Function : CC1101Init( )
    Initialize the CC1101, User can modify it
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101Init( void )
{
    volatile uint8_t i, j;

    CC1101Reset( );

    for( i = 0; i < RF_reg_count; i++ )  //д��23���Ĵ��� RF_reg_count
    {
        CC1101WriteReg( CC1101InitData[i][0], CC1101InitData[i][1] );
    }


    CC1101SetAddress( my_CC1101_chip_address, BROAD_0AND255 );
    CC1101SetSYNC( my_CC1101_sync_word );//0X8799
    CC1101WriteReg(CC1101_MDMCFG1,   0x72); //Modem Configuration

    CC1101WriteMultiReg(CC1101_PATABLE, PaTabel, 8 );

    i = CC1101ReadStatus( CC1101_PARTNUM );//for test, must be 0x80
    printf("PARTNUM=[%XH]  ", i);
    i = CC1101ReadStatus( CC1101_VERSION );//for test, refer to the datasheet
    printf("VERSION=[%XH]  \n", i);
}

/*
================================================================================
Function : CC1101ReadStatus( )
    Read a status register
INPUT    : addr, The address of the register
OUTPUT   : the value read from the status register
================================================================================
*/
INT8U CC1101ReadStatus( INT8U addr )
{
    INT8U i;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr | READ_BURST);
    i = SPI_ExchangeByte( 0xFF );
    CC_CSN_HIGH( );
    return i;
}



/*
================================================================================
Function : CC1101WriteMultiReg( )
    Write some bytes to the specified register
INPUT    : addr, The address of the register
           buff, a buffer stores the values
           size, How many byte should be written
OUTPUT   : None
================================================================================
*/
void CC1101WriteMultiReg( INT8U addr, INT8U *buff, INT8U size )
{
    INT8U i;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr | WRITE_BURST );
    for( i = 0; i < size; i ++ )
    {
        SPI_ExchangeByte( *( buff + i ) );
    }
    CC_CSN_HIGH( );
}




/*
================================================================================
Function : CC1101SetSYNC( )
    Set the SYNC bytes of the CC1101
INPUT    : sync, 16bit sync
OUTPUT   : None
================================================================================
*/
void CC1101SetSYNC( INT16U sync )
{
    CC1101WriteReg(CC1101_SYNC1, 0xFF & ( sync >> 8 ) );
    CC1101WriteReg(CC1101_SYNC0, 0xFF & sync );
}


/*
================================================================================
Function : CC1101SetAddress( )
    Set the address and address mode of the CC1101
INPUT    : address, The address byte
           AddressMode, the address check mode
OUTPUT   : None
================================================================================
*/
void CC1101SetAddress( uint8_t address, ADDR_MODE AddressMode)
{
    uint8_t btmp = CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03;
    CC1101WriteReg(CC1101_ADDR, address);
    if     ( AddressMode == BROAD_ALL )     {}
    else if( AddressMode == BROAD_NO  )     {
        btmp |= 0x01;
    }
    else if( AddressMode == BROAD_0   )     {
        btmp |= 0x02;
    }
    else if( AddressMode == BROAD_0AND255 ) {
        btmp |= 0x03;
    }
}


/*
================================================================================
Function : CC1101ReadReg( )
    read a byte from the specified register
INPUT    : addr, The address of the register
OUTPUT   : the byte read from the rigister
================================================================================
*/
uint8_t CC1101ReadReg( uint8_t addr )
{
    uint8_t i;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr | READ_SINGLE);
    i = SPI_ExchangeByte( 0xFF );
    CC_CSN_HIGH( );
    return i;
}


/*
================================================================================
Function : CC1101Reset( )
    Reset the CC1101 device
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101Reset( void )
{
    uint8_t x;

    CC_CSN_HIGH( );
    HAL_Delay(50);
    CC_CSN_LOW( );
    HAL_Delay(50);
    CC_CSN_HIGH( );
    for( x = 0; x < 100; x ++ );
    CC1101WriteCmd( CC1101_SRES );
}

void CC_CSN_LOW(void)
{

    HAL_GPIO_WritePin(PORT_CC_CSN, PIN_CC_CSN, GPIO_PIN_RESET);
    while( HAL_GPIO_ReadPin(PORT_CC_CSN, PIN_CC_CSN) != GPIO_PIN_RESET);

}
void CC_CSN_HIGH( void)
{
    HAL_GPIO_WritePin(PORT_CC_CSN, PIN_CC_CSN, GPIO_PIN_SET);
}

/*
================================================================================
Function : CC1101WriteCmd( )
    Write a command byte to the device
INPUT    : command, the byte you want to write
OUTPUT   : None
================================================================================
*/
void CC1101WriteCmd( uint8_t command )
{   // uint8_t x1=0;
    CC_CSN_LOW( );
    // x1=SPI_ExchangeByte( command );
    SPI_ExchangeByte( command );
    CC_CSN_HIGH( );
    //printf("writeCmd -[%XH]-read:[%XH]\n",command,x1);
}

/*
================================================================================
Function : CC1101WriteReg( )
    Write a byte to the specified register
INPUT    : addr, The address of the register
           value, the byte you want to write
OUTPUT   : None
================================================================================
*/
void CC1101WriteReg( uint8_t addr, uint8_t value )
{
//		uint8_t x1=0,x2=0;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr );
    SPI_ExchangeByte( value );
    CC_CSN_HIGH( );
    //printf("write reg read data:[%XH]-[%XH]\n",x1,x2);

}

/*
================================================================================
Function : CC1101WORInit( )
    Initialize the WOR function of CC1101
INPUT    : None
OUTPUT   : None
================================================================================
*/
void  CC1101WORInit( void )
{
    CC1101WriteReg(CC1101_MCSM2, 0x00); //0x16
    CC1101WriteReg(CC1101_MCSM0, 0x18); //0X18
    CC1101WriteReg(CC1101_WOREVT1, 0x8C); //0x1E
    CC1101WriteReg(CC1101_WOREVT0, 0xA0); //0x1f
    CC1101WriteReg(CC1101_WORCTRL, 0x78); //0X20   Wake On Radio Control

    CC1101WriteCmd( CC1101_SWORRST );
}

//------------�Զ���---------
//void RF_RecvHandler_intrrupt(void)
//{
//    INT8U length = 0, recv_buffer[64] = {0};
//    uint8_t i = 0;
//    int RSSI_dBm = 0;

//    if(MY_CC1101_STATUS == 0x01 || MY_CC1101_STATUS == 0x02) return;
//    else MY_CC1101_STATUS = 0X02;

//    {
//        while (CC_IRQ_READ() == 1);

//        // ��ȡ���յ������ݳ��Ⱥ���������
//        length = CC1101RecPacket(recv_buffer);

//        // �жϽ��������Ƿ���ȷ
//        if (length <= SEND_MAX && length > 0)
//        {
//            //LED3_ON;               // LED��˸������ָʾ�յ�����

//            printf("RX-data:");
//            for(i = 0; i < length; i++)
//            {
//                Cmd_CC1101_RX[i] = recv_buffer[i];
//                printf("[%XH]-", recv_buffer[i]);
//            }


//            RSSI_dBm = RS_buf_status[0];
//            if(RSSI_dBm >= 128) RSSI_dBm = (RSSI_dBm - 256) / 2 - 75;
//            else RSSI_dBm = (RSSI_dBm) / 2 - 75;

//            printf("\nRX_RSSI:[%XH]-[%d]\n", RS_buf_status[0], RSSI_dBm);

//            //LED3_OFF;

//        }
//    }

//    CC1101SetTRMode(RX_MODE);           // �������ģʽ
//    MY_CC1101_STATUS = 0X00;
//    //CC1101WriteCmd(CC1101_SWOR); //����WORģʽ
//}

void my_read_all_reg(void)
{
    int i = 0;
    uint8_t read_buf[RF_reg_count] = {0};
    for(i = 0; i < RF_reg_count; i++)
    {
        read_buf[i] = CC1101ReadReg( CC1101InitData[i][0]);

        // CC1101WriteReg( CC1101InitData[i][0], CC1101InitData[i][1] );

    }

    for(i = 0; i < RF_reg_count; i++)
    {
        printf("address-[%2X] write-[%2X] read-[%2X]\n", CC1101InitData[i][0], CC1101InitData[i][1], read_buf[i]);

    }


}


/*
================================================================================
Function : CC1101SendPacket_add( )  ��Ŀ���ַ�ķ��ͺ���
    Send a packet
INPUT    : txbuffer, The buffer stores data to be sent
           size, How many bytes should be sent
           mode, Broadcast or address check packet
OUTPUT   : None

����address���Ŀ���ַ��һ���ֽ�
================================================================================
*/
void CC1101SendPacket_add( INT8U *txbuffer, INT8U size, TX_DATA_MODE mode, INT8U address)
{
    uint32_t kk = 0;
    //xSemaphoreTake(myMutex01Handle,3000);

    HAL_NVIC_DisableIRQ(EXIT_CC_IRQ_GD0); //�ر��жϣ������������ж�

    //�ز�����
    kk=0;
    while(my_fun_CC1101_re_CCA_status()==1)
    {
        osDelay(100);  //����ŵ��������ݣ�����ʱһ��ʱ��
        kk++;
        if(kk>=100)
            return ;
    }
    //


    if( mode == BROADCAST )             {
        address = 0;
    }
    //else if( mode == ADDRESS_CHECK )    { address = CC1101ReadReg( CC1101_ADDR ); }

    CC1101ClrTXBuff( );

    if( ( CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03 ) != 0 )
    {
        CC1101WriteReg( CC1101_TXFIFO, size + 1 ); //1���ֽڳ���+��1���ֽڵĵ�ַ+��Ч����,���ⳤ�ȷ���
        CC1101WriteReg( CC1101_TXFIFO, address );  //��ַ+��Ч���ݣ��̶����ȷ���
    }
    else
    {
        CC1101WriteReg( CC1101_TXFIFO, size ); //���ӵ�ַ
    }

    CC1101WriteMultiReg( CC1101_TXFIFO, txbuffer, size ); //д����Ч����
    CC1101SetTRMode( TX_MODE );  //����ģʽ
    // while( HAL_GPIO_ReadPin(PORT_CC_IRQ, PIN_CC_IRQ) != 1 ); //�������ݲ����ߵ�ƽ�ж�

    kk = 0;
    while( HAL_GPIO_ReadPin(PORT_CC_IRQ, PIN_CC_IRQ) != 1 ) //�������ݲ����ߵ�ƽ�ж�
    {
        kk++;
        if(kk >= 0X3FFFF)
        {
            break;
        }
    }
    //while( HAL_GPIO_ReadPin(PORT_CC_IRQ, PIN_CC_IRQ) == 1 ) ;//�ߵ�ƽ��Ϊ�͵�ƽ
    kk = 0;
    while( HAL_GPIO_ReadPin(PORT_CC_IRQ, PIN_CC_IRQ) == 1 ) //�ߵ�ƽ��Ϊ�͵�ƽ
    {
        kk++;
        if(kk >= 0X3FFFF)
        {
            break;
        }

    }

    CC1101ClrTXBuff( );  //�������FIFO
    kk = 0;
    while(CC1101ReadStatus(CC1101_MARCSTATE) != 0x01)
    {
        kk++;
        if(kk > 0X3FFFF)
        {
            break;
        }

    }

    //while(CC1101ReadStatus(CC1101_MARCSTATE) != 0x01);
#if Debug_uart_out_CC1101_TX_Status==1
    printf("1-before TX CC_status=[%XH] \n",CC1101ReadStatus(CC1101_MARCSTATE));
#endif
    CC1101SetTRMode(RX_MODE);  // �������ģʽ
    osDelay(100);
#if Debug_uart_out_CC1101_TX_Status==1

    printf("2-after TX CC_status=[%XH] \n",CC1101ReadStatus(CC1101_MARCSTATE));
#endif

    __HAL_GPIO_EXTI_CLEAR_IT(PIN_CC_IRQ); //�����������ⲿ�жϣ�ÿ���жϵ������������һ������Ҫ�ֱ����
#if Use_CC1101_receive_interrupt_status==1
    HAL_NVIC_EnableIRQ(EXIT_CC_IRQ_GD0); //����CC1101�жϣ�����/�������ݲ���
#endif

    //xSemaphoreGive(myMutex01Handle);


}

#define my_CC1101_RX_MAX  5000
uint8_t my_CC1101_re_buf[my_CC1101_RX_MAX]; //CC1101�Ļ�����,1������
uint16_t my_CC1101_re_buf_pt_write = 0;
uint16_t my_CC1101_re_buf_pt_read = 0;
uint8_t my_CC1101_COM_Fram_buf[my_CC1101_RX_MAX]; //ָ�����
uint8_t my_CC1101_Frame_status = 0;
extern uint8_t my_get_count;
/*
���ܣ���CC1101���յ����ݷŵ������ݻ�������
*/
uint8_t my_get_client_add=0; //CC1101���յ����ݵĿͻ��˵�ַ��
int  RF_RecvHandler_intrrupt_get_data_to_buf(void)
{
    INT8U length = 0, recv_buffer[64] = {0};
    uint8_t ii = 0;
    extern uint16_t my_tim6_count;
    uint32_t my_temp32=0;

    //xSemaphoreTake(myMutex01Handle,3000);
    if(MY_CC1101_STATUS == 0x01 || MY_CC1101_STATUS == 0x02)
        return 0;
    else
        MY_CC1101_STATUS = 0X02;


    while (CC_IRQ_READ() == 1)
    {
        my_temp32++;
        if(my_temp32>=0x3FFFFF)
        {
            printf("\n*****2 CC_IRQ_READ() == 1 ******\n");
            CC1101ClrRXBuff( );
            return 0;
        }
    }

    // ��ȡ���յ������ݳ��Ⱥ���������
    length = CC1101RecPacket(recv_buffer);  //��ǰ���յ���RX�����ֽ�����������1+��ַ1+��ЧN+CRCУ��2
    //printf("get length=%d\r\n",length);

    // �жϽ��������Ƿ���ȷ
    if (length <= SEND_MAX && length > 0)
    {
        //LED3_ON;               // LED��˸������ָʾ�յ�����
        my_get_client_add=recv_buffer[0];
        for(ii = 1; ii < length; ii++)
        {
            my_CC1101_re_buf[my_CC1101_re_buf_pt_write] = recv_buffer[ii];

            if(ii>1&& my_CC1101_re_buf_pt_write==my_CC1101_re_buf_pt_read)
            {
                my_CC1101_re_buf_pt_read++;
                if(my_CC1101_re_buf_pt_read>=my_CC1101_RX_MAX)
                    my_CC1101_re_buf_pt_read=0;

            }


            my_CC1101_re_buf_pt_write++;
            if(my_CC1101_re_buf_pt_write >= my_CC1101_RX_MAX)
                my_CC1101_re_buf_pt_write = 0;

        }
        //����ʹ�ã���ʾ��ǰ���յ�������
        //my_fun_display_buf_16(recv_buffer,length); //


        // printf("�������� %d\r\n",my_get_count);
        // my_get_count=0; //��ʾ����������
//          //����ʹ����ʾ����ֵ
//					printf("RX-data:");
//					for(i=0;i<length;i++)
//					{
//						Cmd_CC1101_RX[i]=recv_buffer[i];
//						printf("[%XH]-",recv_buffer[i]);
//					}
//
//					RSSI_dBm=RS_buf_status[0];
//					if(RSSI_dBm>=128) RSSI_dBm=(RSSI_dBm-256)/2-75;
//					else RSSI_dBm=(RSSI_dBm)/2-75;
//					printf("\nRX_RSSI:[%XH]-[%d]\n",RS_buf_status[0],RSSI_dBm);



    }

    CC1101SetTRMode(RX_MODE);           // �������ģʽ
    MY_CC1101_STATUS = 0X00;
    //CC1101WriteCmd(CC1101_SWOR); //����WORģʽ
    //xSemaphoreGive(myMutex01Handle);
    return length;
}
extern uint8_t rsbuf3[];
extern	uint8_t USART3_FRAME_status;
extern	uint8_t USART3_my_frame[];
extern uint16_t rsbuf3pt_write;
extern uint16_t rsbuf3pt_read;

extern uint8_t rsbuf1[];
extern	uint8_t USART1_FRAME_status;
extern	uint8_t USART1_my_frame[];
extern uint16_t rsbuf1pt_write;
extern uint16_t rsbuf1pt_read;

//Э����������һ֡����,
//��1������Ϊ�˿ںţ�
//��2��Ϊ��ַ2�ֽ�,1Ϊ�����ֽڣ�0λ1���ֽ�
//��3������Ϊ�Ƿ���CRCУ�飬1Ϊ����
int my_101frame_analyse(uint8_t port_number, uint8_t length_long_status,uint8_t CRC_check_status)
{

    int ii = 0;
    uint16_t my_temp = 0;
    uint16_t my_start_add = 0;
    uint16_t my_end_add = 0;
    uint16_t my_temp_length1 = 0;
    uint16_t my_temp_length2 = 0;
    uint8_t my_status1 = 0;
    uint8_t my_status2 = 0;
    uint16_t my_chazhi = 0;
    int x = 0, x11 = 0, x12 = 0, x21 = 0, x22 = 0;
    uint16_t y = 0;
    volatile	  uint8_t my_point = 0;

    uint8_t *my_pro1_status = NULL;
    uint16_t *my_buf_read_count = NULL;
    uint16_t *my_buf_write_count = NULL;
    uint16_t re_max = 0;
    uint8_t *my_re_buf = NULL;
    uint8_t *my_com_buf1 = NULL; //������ָ��
    uint8_t my_length_max = 0;
    //===CC1101
    if(port_number == 4 )
    {
        my_pro1_status = &my_CC1101_Frame_status;
        my_buf_read_count = &my_CC1101_re_buf_pt_read;
        my_buf_write_count = &my_CC1101_re_buf_pt_write;
        re_max = my_CC1101_RX_MAX;
        my_re_buf = my_CC1101_re_buf;
        my_com_buf1 = my_CC1101_COM_Fram_buf; //������ָ��
    }
    //===USART3
    else if(port_number == 3)
    {
        my_pro1_status = &USART3_FRAME_status;
        my_buf_read_count = &rsbuf3pt_read;
        my_buf_write_count = &rsbuf3pt_write;
        re_max =rsbuf3_max ;
        my_re_buf = rsbuf3;
        my_com_buf1 = USART3_my_frame; //������ָ��
    }
    else if(port_number == 1)
    {
        my_pro1_status = &USART1_FRAME_status;
        my_buf_read_count = &rsbuf1pt_read;
        my_buf_write_count = &rsbuf1pt_write;
        re_max =rsbuf_max ;
        my_re_buf = rsbuf1;
        my_com_buf1 = USART1_my_frame; //������ָ��
    }
    //===========
    if(length_long_status == 1)
    {
        my_length_max = 5;
    }
    else
        my_length_max = 3;

    //=====

    if(*my_pro1_status == 1)
    {
        my_point = 1;
        return 1;
    }

    //-----------1�ж� ֡ͷ��λ��--------



xx0:
    while(*my_buf_read_count != *my_buf_write_count)
    {
        if(my_re_buf[*my_buf_read_count] == 0X68) //��֡ 68H-xx-xx-68H-ID-XX-XX... CRC-16H
        {   my_status2 = 1;
            break;
        }
        else if(my_re_buf[*my_buf_read_count] == 0x10) //�̶����ȶ�֡ 10H-ID-XX-XX-CRC-16H
        {   my_status2 = 2;
            break;
        }
        else
        {   (*my_buf_read_count)++;
            if(*my_buf_read_count >= re_max)
                *my_buf_read_count = 0;
        }
    }


    //--------2 �ж϶�ָ���λ��
    if(*my_buf_read_count == *my_buf_write_count)
    {   //my_point=2;
        return 0;
    }
    else if(*my_buf_read_count < *my_buf_write_count)
    {
        my_status1 = 1;  //���ڵ͵�ַ
    }
    else if(*my_buf_read_count > *my_buf_write_count)
    {
        my_status1 = 2;  //���ڸߵ�ַ
    }

    //-----3 ȡ��ǰʱ�̵�ָ���ַ

    my_start_add = *my_buf_read_count;
    my_end_add = *my_buf_write_count;


    //--------4 ��֡�����ж� //����ǰ��д�ں�
    if(my_status2 == 1 && my_status1 == 1) //���ڵ͵�ַ��д�ڸߵ�ַ
    {
        my_chazhi = my_end_add - my_start_add;
        if(my_chazhi < 10) //��ֵС����С����
        {   my_point = 3;
            return 0;
        }
        //ȡ������һ��֡ͷ

        if(my_re_buf[my_start_add + my_length_max] != 0X68)
        {
            (*my_buf_read_count)++;
            my_point = 4;
            goto xx0;
            //return 0;

        }


        if(length_long_status == 1)
        {
            //ȡ��֡�ĳ��ȣ���ID��ʼ�㣬һֱ��CRC������CRC��֡���ܳ���Ϊlength+6+2   68 xx xx yy yy 68 (....length..) crc 16
            my_temp_length1 = my_re_buf[my_start_add + 1];
            my_temp_length1 = my_temp_length1 + ((uint16_t)(my_re_buf[my_start_add + 2]) << 8);

            my_temp_length2 = my_re_buf[my_start_add + 3];
            my_temp_length2 = my_temp_length2 + ((uint16_t)(my_re_buf[my_start_add + 4]) << 8);

            if(my_temp_length1 != my_temp_length2)
            {
                (*my_buf_read_count)++;
                return 0;
            }
        }
        else
        {
            //ȡ��֡�ĳ��ȣ���ID��ʼ�㣬һֱ��CRC������CRC��֡���ܳ���Ϊlength+6+2   68 xx xx yy yy 68 (....length..) crc 16
            my_temp_length1 = my_re_buf[my_start_add + 1];
            my_temp_length2 = my_re_buf[my_start_add + 2];
            if(my_temp_length1 != my_temp_length2)
            {
                (*my_buf_read_count)++;
                return 0;
            }
        }

        if(my_start_add + my_temp_length1 + my_length_max + 2 >= my_end_add)
        {
            my_point = 31;
            return 0;
        }


        if((my_start_add + my_temp_length1 + my_length_max + 2) >= re_max)
        {

            my_point = 5;
            return 0;
        }

        if(my_chazhi < my_temp_length1) //��ֵ����С��֡����
        {
            my_point = 6;
            return 0;
        }
        //ȡ֡β
        x = my_re_buf[my_start_add + my_length_max + 2 + my_temp_length1];
        if(x != 0X16)
        {
            (*my_buf_read_count)++;
            my_point = 7;
            goto xx0;
            //return 0;

        }
        else
        {
            for(ii = 0; ii < (my_temp_length1 + my_length_max + 2 + 1); ii++)
            {

                my_com_buf1[ii] = my_re_buf[my_start_add + ii]; //ȡһ֡����������

            }
            *my_buf_read_count = my_start_add + my_temp_length1 + my_length_max + 2 + 1;

            if(CRC_check_status==1)
            {
                *my_pro1_status=my_fun_101check_verify(my_com_buf1,length_long_status);
                if(*my_pro1_status==1)
                {

                    if(*my_buf_read_count<*my_buf_write_count && port_number==4)
                        xEventGroupSetBits(xCreatedEventGroup,0X08);
                    else if(*my_buf_read_count<*my_buf_write_count && port_number==1)
                        xEventGroupSetBits(xCreatedEventGroup,0X01);
                    else if(*my_buf_read_count<*my_buf_write_count && port_number==2)
                        xEventGroupSetBits(xCreatedEventGroup,0X02);

                }

                return *my_pro1_status;
            }
            else
            {

                *my_pro1_status = 1; //��ʾȡ��һ֡����������
                if(*my_buf_read_count<*my_buf_write_count && port_number==4)
                    xEventGroupSetBits(xCreatedEventGroup,0X08);
                else if(*my_buf_read_count<*my_buf_write_count && port_number==1)
                    xEventGroupSetBits(xCreatedEventGroup,0X01);
                else if(*my_buf_read_count<*my_buf_write_count && port_number==2)
                    xEventGroupSetBits(xCreatedEventGroup,0X02);
                my_point = 22;
                return 1;
            }
        }


    }
    //--------5 ��֡�����ж� //д��ǰ�����ں�
    if(my_status2 == 1 && my_status1 == 2) //д�ڵ͵�ַ�����ڸߵ�ַ
    {
        my_temp = re_max - my_start_add;
        my_chazhi = (my_temp + my_end_add);
        if(my_chazhi < 8) //��ֵС����С����
        {   my_point = 9;
            return 0;
        }
        if(my_temp >= (my_length_max + 1))
            x = my_start_add + my_length_max;  //xΪ��2��֡ͷ�ĵ�ַ
        else
            x = my_length_max - my_temp;

        //ȡ������һ��֡ͷ
        if(my_re_buf[x] != 0X68)
        {
            (*my_buf_read_count)++;
            if(*my_buf_read_count >= re_max)
                *my_buf_read_count = 0;

            my_point = 10;
            goto xx0;
            //return 0;
        }
        //ȡ��֡�ĳ���

        if(length_long_status == 1) //˫�ֽڵ�ַ
        {
            if(x > 5)
            {
                x11 = my_start_add + 1;
                x12 = my_start_add + 2;
                x21 = my_start_add + 3;
                x22 = my_start_add + 4;
            }
            else
            {
                x11 = my_start_add + 1;
                if(x11 >= re_max)
                    x11 = x11 - re_max;

                x12 = my_start_add + 2;
                if(x12 >= re_max)
                    x12 = x12 - re_max;

                x21 = my_start_add + 3;
                if(x21 >= re_max)
                    x21 = x21 - re_max;

                x22 = my_start_add + 4;
                if(x22 >= re_max)
                    x22 = x22 - re_max;

            }

            my_temp_length1 = my_re_buf[x11];
            my_temp_length1 = my_temp_length1 + ((uint16_t)(my_re_buf[x12]) << 8);

            my_temp_length2 = my_re_buf[x21];
            my_temp_length2 = my_temp_length2 + ((uint16_t)(my_re_buf[x22]) << 8);
        }
        else //���ֽڵ�ַ
        {
            if(x > 3)
            {
                x11 = my_start_add + 1;
                x21 = my_start_add + 2;

            }
            else
            {
                x11 = my_start_add + 1;
                if(x11 >= re_max)
                    x11 = x11 - re_max;

                x21 = my_start_add + 2;
                if(x21 >= re_max)
                    x21 = x21 - re_max;

            }

            my_temp_length1 = my_re_buf[x11];
            my_temp_length2 = my_re_buf[x21];

        }


        if(my_temp_length1 != my_temp_length2)
        {
            (*my_buf_read_count)++;
            goto xx0;
            // return 0;

        }

        if(my_chazhi < my_temp_length1)
        {   my_point = 11;
            return 0;
        }


        //=======

        //ȡ��֡β
        y = my_start_add + my_temp_length1 + my_length_max+2; //;
        if(y >= re_max) //֡β���������ĩβ
        {
            y = y - re_max;

            if(y >= my_end_add)
            {
                my_point = 32;
                return 0;
            }
        }


        x = my_re_buf[y]; //ȡ��֡β����

        //ȡһ֡����������
        if(x != 0X16)
        {
            (*my_buf_read_count)++;
            if(*my_buf_read_count >= re_max)
                *my_buf_read_count = 0;
            my_point = 18;
            goto xx0;
            // return 0;

        }
        else
        {
            for(ii = 0; ii < my_temp_length1 + 6; ii++)
            {
                y = my_start_add + ii;
                if(y >= re_max)
                {
                    y = y - re_max;
                }
                my_com_buf1[ii] = my_re_buf[y]; //ȡһ֡����������

            }
            *my_buf_read_count = *my_buf_read_count + my_temp_length1 + my_length_max + 2 + 1;
            if(*my_buf_read_count >= re_max)
            {
                *my_buf_read_count = *my_buf_read_count - re_max;
            }


            if(CRC_check_status==1)
            {
                *my_pro1_status=my_fun_101check_verify(my_com_buf1,length_long_status);

                if(*my_pro1_status==1)
                {

                    if(*my_buf_read_count<*my_buf_write_count && port_number==4)
                        xEventGroupSetBits(xCreatedEventGroup,0X08);
                    else if(*my_buf_read_count<*my_buf_write_count && port_number==1)
                        xEventGroupSetBits(xCreatedEventGroup,0X01);
                    else if(*my_buf_read_count<*my_buf_write_count && port_number==2)
                        xEventGroupSetBits(xCreatedEventGroup,0X02);

                }

                return *my_pro1_status;
            }
            else
            {

                *my_pro1_status = 1; //��ʾȡ��һ֡����������
                if(*my_buf_read_count<*my_buf_write_count && port_number==4)
                    xEventGroupSetBits(xCreatedEventGroup,0X08);
                else if(*my_buf_read_count<*my_buf_write_count && port_number==1)
                    xEventGroupSetBits(xCreatedEventGroup,0X01);
                else if(*my_buf_read_count<*my_buf_write_count && port_number==2)
                    xEventGroupSetBits(xCreatedEventGroup,0X02);
                my_point = 22;
                return 1;
            }
        }

    }

    //-------6����֡�ж�  ����ǰ��д�ں�

    if(my_status2 == 2 && my_status1 == 1) //����ǰ��д�ں�
    {
        my_chazhi = my_end_add - my_start_add;
        if(my_chazhi < 5) //��ֵС����С����
        {   my_point = 20;
            return 0;
        }

        //ȡ��֡�ĳ���
        if(length_long_status == 1)
            my_temp_length1 = 7;
        else
            my_temp_length1 = 5;
        if(my_start_add + my_temp_length1 >= my_end_add)
        {
            my_point = 33;
            return 0;
        }

        //ȡ֡β
        x = my_re_buf[my_start_add + my_temp_length1];
        if(x != 0X16)
        {
            (*my_buf_read_count)++;
            my_point = 21;
            goto xx0;
            //return 0;

        }
        else
        {
            for(ii = 0; ii < my_temp_length1 + 1; ii++)
            {

                my_com_buf1[ii] = my_re_buf[my_start_add + ii]; //ȡһ֡����������

            }
            *my_buf_read_count = *my_buf_read_count + my_temp_length1 + 1;


            if(CRC_check_status==1)
            {
                *my_pro1_status=my_fun_101check_verify(my_com_buf1,length_long_status);
                if(*my_pro1_status==1)
                {

                    if(*my_buf_read_count<*my_buf_write_count && port_number==4)
                        xEventGroupSetBits(xCreatedEventGroup,0X08);
                    else if(*my_buf_read_count<*my_buf_write_count && port_number==1)
                        xEventGroupSetBits(xCreatedEventGroup,0X01);
                    else if(*my_buf_read_count<*my_buf_write_count && port_number==2)
                        xEventGroupSetBits(xCreatedEventGroup,0X02);

                }


                return *my_pro1_status;
            }
            else
            {

                *my_pro1_status = 1; //��ʾȡ��һ֡����������
                if(*my_buf_read_count<*my_buf_write_count && port_number==4)
                    xEventGroupSetBits(xCreatedEventGroup,0X08);
                else if(*my_buf_read_count<*my_buf_write_count && port_number==1)
                    xEventGroupSetBits(xCreatedEventGroup,0X01);
                else if(*my_buf_read_count<*my_buf_write_count && port_number==2)
                    xEventGroupSetBits(xCreatedEventGroup,0X02);
                my_point = 22;
                return 1;
            }

        }


    }


//--------7 �̶�֡�ж�  д��ǰ�����ں�

    if(my_status2 == 2 && my_status1 == 2) //д��ǰ�����ں�
    {
        my_temp = re_max - my_start_add;
        my_chazhi = (my_temp + my_end_add);
        if(my_chazhi < 5) //��ֵС����С����
            return 0;

        //ȡ��֡�ĳ���
        if(length_long_status == 1)
            my_temp_length1 = 7;
        else
            my_temp_length1 = 5;

        //ȡ��֡β
        y = my_start_add + my_temp_length1; //;
        if(y >= re_max) //֡β���������ĩβ
        {
            y = y - re_max;
            if(y > my_end_add)
            {
                my_point = 35;
                return 0;
            }

        }

        x = my_re_buf[y]; //ȡ��֡β����

        //ȡһ֡����������
        if(x != 0X16)
        {
            (*my_buf_read_count)++;
            if(*my_buf_read_count >= re_max)
                *my_buf_read_count = 0;
            my_point = 36;
            goto xx0;
            //return 0;

        }
        else
        {
            for(ii = 0; ii < my_temp_length1 + 1; ii++)
            {
                y = my_start_add + ii;
                if(y >= re_max)
                {
                    y = y - re_max;
                }
                my_com_buf1[ii] = my_re_buf[y]; //ȡһ֡����������

            }
            *my_buf_read_count = *my_buf_read_count + my_temp_length1 + 1;
            if(*my_buf_read_count >= re_max)
                *my_buf_read_count = *my_buf_read_count - re_max;
            //CRCУ��
            if(CRC_check_status==1)
            {
                *my_pro1_status=my_fun_101check_verify(my_com_buf1,length_long_status);

                if(*my_pro1_status==1)
                {

                    if(*my_buf_read_count<*my_buf_write_count && port_number==4)
                        xEventGroupSetBits(xCreatedEventGroup,0X08);
                    else if(*my_buf_read_count<*my_buf_write_count && port_number==1)
                        xEventGroupSetBits(xCreatedEventGroup,0X01);
                    else if(*my_buf_read_count<*my_buf_write_count && port_number==2)
                        xEventGroupSetBits(xCreatedEventGroup,0X02);

                }

                return *my_pro1_status;
            }
            else
            {

                *my_pro1_status = 1; //��ʾȡ��һ֡����������


                if(*my_buf_read_count<*my_buf_write_count && port_number==4)
                    xEventGroupSetBits(xCreatedEventGroup,0X08);
                else if(*my_buf_read_count<*my_buf_write_count && port_number==1)
                    xEventGroupSetBits(xCreatedEventGroup,0X01);
                else if(*my_buf_read_count<*my_buf_write_count && port_number==2)
                    xEventGroupSetBits(xCreatedEventGroup,0X02);

                my_point = 22;
                return 1;
            }
        }

    }

    //===========
    return 0;
}

//����ʹ�ã�16������ʾ�յ��ĵ�����
void my_fun_display_fram_16(uint8_t portnumber,uint8_t dis_length)
{
    uint8_t *pt_buf = NULL;
    uint16_t my_length = 0;
    uint16_t ii = 0;
    char *ptchar = NULL;
    if(portnumber == 4)
    {
        pt_buf = my_CC1101_COM_Fram_buf;
        ptchar = "CC1101 re:";

    }
    else if(portnumber==3)
    {   if(USART3_FRAME_status==0)
            return;

        pt_buf = USART3_my_frame;
        ptchar = "USART3 re:";
    }
    else if(portnumber==1)
    {   if(USART1_FRAME_status==0)
            return;

        pt_buf = USART1_my_frame;
        ptchar = "USART1 re:";
    }

    if(pt_buf[0] == 0X10 && pt_buf[5] == 0X16)
    {
        my_length = 6;

    }
    else if(pt_buf[0] == 0X10 && pt_buf[7] == 0X16)
    {

        my_length = 8;
    }
    else if(pt_buf[0] == 0X68 && pt_buf[3] == 0X68)
    {
        my_length = pt_buf[1] + 6;

    }

    else if(pt_buf[0] == 0X68 && pt_buf[5] == 0X68)
    {
        my_length = pt_buf[2];
        my_length = my_length << 8;
        my_length = my_length + pt_buf[1] + 8;

    }
    printf("%s", ptchar);

    if(my_length>8)
        my_length=dis_length;   //������ʾ�ַ����ĳ���

    for(ii = 0; ii < my_length; ii++)
    {
        printf("[%XH]-", pt_buf[ii]);

    }
    printf("\r\n");

}

//����ʹ�ã���ʾ���͵�����
//���һ������������Ϊ1������Ϊ0
void my_fun_display_buf_16(uint8_t *pt, uint16_t size,uint8_t TX_status)
{
    uint16_t ii = 0;
    for(ii = 0; ii < size; ii++)
    {
        printf ("[%XH]-", pt[ii]);

    }
    if(TX_status==1)
        printf("TX-GRPS\r\n");
    else
        printf("RX-GPRS\r\n");

}

//��������CC1101����
/*
CC1101��󻺳�����64���ֽڣ������� ����1+��ַ1+....N������+2��crc
�������һ�η���60���ֽ�
*/

void my_fun_CC1101_send_long_data(INT8U *txbuffer, uint16_t size, TX_DATA_MODE mode, INT8U address)
{


    uint8_t ii = 0;

    uint8_t my_block_size = 0;
    uint8_t my_block_last_data = 0;
    uint8_t *pt = txbuffer;

    my_block_size = size / My_CC1101_send_data_size_MAX;
    my_block_last_data = size % My_CC1101_send_data_size_MAX;

    for(ii = 0; ii < my_block_size; ii++)
    {
        CC1101SendPacket_add(pt + ii * My_CC1101_send_data_size_MAX, My_CC1101_send_data_size_MAX, mode, address);
        //====
        if(ii==0)
        {
            printf("start DB=%d\r\n",ii);
            //my_fun_display_buf_16(pt + ii * My_CC1101_send_data_size_MAX,My_CC1101_send_data_size_MAX); //����ʹ�ã���ʾ���͵�����
        }
        //====
        //HAL_Delay(1);
        if(my_block_size>0)
            osDelay(20);
    }
    if(my_block_last_data!=0)
    {
        //====
        printf("last DB=%d\r\n",ii);
        //my_fun_display_buf_16(pt + ii * My_CC1101_send_data_size_MAX,my_block_last_data); //����ʹ�ã���ʾ���͵�����
        //=====
        CC1101SendPacket_add(pt + ii * My_CC1101_send_data_size_MAX, my_block_last_data, mode, address);

    }
}

//��ʼ��CC1101

void my_fun_CC1101_init_reum(void)
{
    //xSemaphoreTake(myMutex01Handle,3000);
    HAL_NVIC_DisableIRQ(EXIT_CC_IRQ_GD0);
    CC1101Init();
    HAL_Delay(1000);
    CC1101WriteCmd(CC1101_SIDLE); //�������״̬
    CC1101WriteReg(CC1101_MCSM2, 0X00); //д0x16 ,RX_TIME��д��0X00�����ý���ʱ�����ƣ�ռ�ձ�����00���12%,���û���յ����ݣ��ͽ���SLEEP��
    CC1101WriteReg(CC1101_MCSM0, 0x18); //0X18 	//
    CC1101WriteReg(CC1101_WOREVT1, 0x87); //0X1E,event0 ���ֽ�
    CC1101WriteReg(CC1101_WOREVT0, 0x6A);  //0X1F event0 ���ֽ�
    CC1101WriteReg(CC1101_WORCTRL, 0X78); //д0X20,0111 100,WOR_RES
    //CC1101WriteCmd(CC1101_SWOR); //����WORģʽ

    //---------�������ģʽ

    CC1101SetIdle();
    CC1101WriteReg(CC1101_MCSM2, 0X07); //д0x16 ,RX_TIME��д��0X07,����û��ʱ�����ƣ�һ�½�������
    CC1101SetTRMode(RX_MODE);           // �������ģʽ

    __HAL_GPIO_EXTI_CLEAR_IT(PIN_CC_IRQ);
    HAL_NVIC_ClearPendingIRQ(EXIT_CC_IRQ_GD0);
#if Use_CC1101_receive_interrupt_status==1
    HAL_NVIC_EnableIRQ(EXIT_CC_IRQ_GD0); //�����жϣ�����/�������ݲ���
#endif
    //xSemaphoreGive(myMutex01Handle);
}

/*
����У����
��������һ��Ϊָ�����飬�ڶ���Ϊ��֡��飬1��ʾ˫�ֽڳ��ȣ�0Ϊ���ֳ���
*/
uint8_t my_fun_101check_generate(uint8_t *buffer,uint8_t longstatus)
{
    uint16_t k=0;
    uint8_t status68=0;
    uint16_t length=0;

    uint8_t check_char=0x00;
    if(longstatus==0)  //���ֽڵ�ַ���
    {
        if(buffer[0]==0x10)
        {
            status68=1;
        }
        else if(buffer[0]==0x68)
        {
            status68=2;
        }


        if(status68==1) //�̶�����У��λ���
        {   check_char=0;
            check_char=buffer[1]+buffer[2]+buffer[3];
            buffer[4]=check_char;
						buffer[5]=0x16;
        }
				
        else if(status68==2) //�ǹ̶�����У��λ���
        {   check_char=0;
            for(k=0; k<buffer[1]; k++)
            {
                check_char=check_char+buffer[k+4];
            }
            buffer[buffer[1]+4]=check_char;
						buffer[buffer[1]+5]=0x16;
        }
    }


    else if(longstatus==1)  //˫�ֽڵ�ַ���
    {

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
            check_char=buffer[1]+buffer[2]+buffer[3]+buffer[4]+buffer[5];;
            buffer[6]=check_char;
					  buffer[7]=0X16;
        }
        else if(status68==2) //�ǹ̶�����У��λ���
        {
            length=buffer[2];
            length=buffer [1]+(length<<8);
            for(k=0; k<length; k++)
            {
                check_char=check_char+buffer[k+6];
            }
            buffer[length+6]=check_char;
						buffer[length+7]=0X16;
        }


    }
    return check_char;

}

/*
У���ּ��
����ֵ��ͨ��Ϊ1����ͨ��Ϊ0
*/

uint8_t my_fun_101check_verify(uint8_t *buffer,uint8_t longstatus)
{
    uint16_t k=0;
    uint8_t status68=0;
    uint8_t temp=0;
    uint8_t check_char=0x00;
    uint16_t my_length=0;
    if(longstatus==0)
    {
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

    }
    else if(longstatus==1)
    {
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
            check_char=buffer[1]+buffer[2]+buffer[3]+buffer[4]+buffer[5];
            if(check_char==buffer[6])
                temp=1;
            else temp=0;
        }
        else if(status68==2) //�ǹ̶�����У��λ���
        {
            my_length=buffer[2];
            my_length=buffer[1]+(my_length<<8);
            for(k=0; k<my_length; k++)
            {
                check_char=check_char+buffer[k+6];
            }

            if(check_char==buffer[my_length+6])
                temp=1;
            else temp=0;
        }
        temp=1;



    }
    printf("CRC verify is  %d==%d\r\n",temp,status68);
    return temp;

}


void  CC1101CCA( void )//����Ϊ�ز���������
{
    CC1101WriteReg(CC1101_IOCFG2,0x0E);//GDO2��������ز���Ӧ��ƽ,���RSSI����������֮��Ϊ�ߵ�ƽ,�ŵ�ռ��Ϊ�ߵ�ƽ
    CC1101WriteReg(CC1101_MCSM1,0x3F);//�����ź�ǿ�ȵ�������ֵ�ҵ�ǰδ���ձ������ŵ����У���TX��RX������оƬ��������RX״̬��
    CC1101WriteReg(CC1101_AGCCTRL1,0x40);//�������Ŵ��������ȼ�С���ز����������ֵ����,�ز�����������ֵ��MAGN_TARGET����
    CC1101WriteReg(CC1101_AGCCTRL2,0x07);//42dB
}

/*
���ܣ����ص�ǰ�ز������ŵ����ź�״̬�����źŷ���1��û���źţ����У�����0
����ֵ�����źŷ���1��û���źţ����У�����0
*/
uint8_t my_fun_CC1101_re_CCA_status(void)
{
    //if(HAL_GPIO_ReadPin(GPIO2_INPUT_GPIO_Port,GPIO2_INPUT_Pin)==1)//��⵽���������ز��ź�

    CC1101SetTRMode( RX_MODE );
    HAL_Delay(10);
    return (uint8_t)(HAL_GPIO_ReadPin(CC1101_GDO1_GPIO_Port,CC1101_GDO1_Pin));

}
