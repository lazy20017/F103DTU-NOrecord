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
功能：发送命令，等待（特定）反馈命令
*/
uint8_t WDZ_MCUtransmint_commd_wait_commd(uint8_t type,uint8_t *txbuf,uint8_t address_type,uint8_t *rxbuf)  //type为类型，1为固定长度，2为非固定长度，txbuf为发送指令数组
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
//********************发送非固定长度帧，应答为OK帧
    if(type==2)
    {

        uint8_t linkcontrol=rxbuf[1];
        uint8_t type_identification=0;
        uint8_t  transmit_reason=0;

        //控制域码处理

        //目的地址处理
        my_txbuf[5]=0X01;
        my_txbuf[6]=0X00;


        //校验位产生
        wdz_101check_generate(my_txbuf);
        //发送指令
        while((n--)>0&&my_status==0)
        {
            //USART_printf(&huart2,my_txbuf);
            my_UART4_printf(&huart2,my_txbuf);
            //Delay_us(100);

            my_start_time=my_tim6_count;
            do {

                my_status=WDZ_MCUreceive_testactive(linkcontrol,type_identification,transmit_reason,300);  //等待0K确认反馈命令
                if(my_status==1)break;
                else my_status=0;
            } while(my_tim6_count-my_start_time<=3);

        }


    }


//**************发送固定长度帧，应答为固定长度帧
    else	if(type==1&&rxbuf[0]==0x10)
    {
        //产生接收的控制域码，类型，传输原因
        linkcontrol=rxbuf[1];
        type_identification=0;
        transmit_reason=0;

        //控制域码处理

        //目的地址处理
        my_txbuf[2]=0X01;
        my_txbuf[3]=0X00;

        //校验位产生
        wdz_101check_generate(my_txbuf);
        //发送指令
        while((n--)>0&&my_status==0)
        {
            //USART_printf(&huart2,my_txbuf);
            my_UART4_printf(&huart2,my_txbuf);

            //Delay_us(300); //测试

            my_status=WDZ_MCUreceive_testactive(linkcontrol,type_identification,transmit_reason,300);  //等待反馈命令
            if(my_status==1)break;
            else my_status=0;
        }
    }
//**********发送固定长度帧，应答为非固定长度帧
    else if(type==1&&rxbuf[0]==0x68)
    {
        //产生接收的控制域码，类型，传输原因
        linkcontrol=rxbuf[4];
        type_identification=rxbuf[7];
        transmit_reason=rxbuf[9];

        //控制域码处理

        //目的地址处理
        my_txbuf[2]=0x01;
        my_txbuf[3]=0x00;

        //校验位产生
        wdz_101check_generate(my_txbuf);
        //发送指令
        while((n--)>0&&my_status==0)
        {
            //USART_printf(&huart2,my_txbuf);
            my_UART4_printf(&huart2,my_txbuf);

            //Delay_us(600); //测试

            my_status=WDZ_MCUreceive_testactive(linkcontrol,type_identification,transmit_reason,300);  //等待反馈命令
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
功能：测试应答帧
输入参数：Link_control，域控制码，type_identification-帧类型，transmit_reason-传输原因，time-检测时间
输入结果：应答正确为1，应答错误或没有应答为0
*/

uint8_t WDZ_MCUreceive_testactive(uint8_t Link_control,uint8_t type_identification,uint8_t transmit_reason,uint16_t time)
{
    uint8_t temp=0;
    uint8_t *rsbuf=USART2_my_frame;

    uint8_t status68=0;  //2代表非固定长度帧，1代表固定长度6的帧，0代表问题或处理完成
    uint16_t my_dtu_address=0x0001;
    uint16_t my_start_time=0;


    my_start_time=my_tim6_count;
    while(my_tim6_count-my_start_time<=3)
    {   my_usart_101frame(2);
        if(USART2_FRAME_status>0)
        {
            //帧头检查
            if(rsbuf[0]==0x68)
            {
                status68=2;
            }

            else if(rsbuf[0]==0x10)
            {
                status68=1;
            }

//************0x10固定长度帧处理
            //站地址检查
            if(status68==1)
            {
                my_dtu_address=rsbuf[3];
                my_dtu_address=(my_dtu_address<<8)+rsbuf[2];
                if(my_dtu_address==0x0001)
                    status68=1;
                else status68=0;
            }
            //校验字节检查
            if(status68==1)
            {
                temp=wdz_101char_check(rsbuf);  //测试时可以考虑先不使用
                if(temp==1) status68=1;
                else status68=0;
            }



            //帧类型检查、传输原因检查
            if(status68==1)
            {

                if((rsbuf[1]&0x0f)==(Link_control&0x0f))
                {
                    temp=1;
                    //帧的处理过程结束
                    USART2_FRAME_status=0;
                    break;
                }
                else
                {   temp=0;
                    //测试，把接收到的帧转发串口3
                    USART_printf(&huart3,"mm4444");
                    USART_printf(&huart3,USART2_my_frame);

                    //帧的处理过程结束
                    USART2_FRAME_status=0;
                }

                //帧的处理过程结束
                USART2_FRAME_status=0;
            }



//************0x68非固定长度帧处理
            //从站地址检查
            if(status68==2)
            {
                my_dtu_address=rsbuf[6];
                my_dtu_address=(my_dtu_address<<8)+rsbuf[5];
                if(my_dtu_address==0x0001)
                    status68=2;
                else status68=0;
            }
            //校验字节检查

            if(status68==2)
            {
                temp=wdz_101char_check(rsbuf);  //测试时可以考虑先不使用
                if(temp==1) status68=2;
                else status68=0;
            }



            //帧类型检查、传输原因检查 68 0D 0D 68 53 01 00 68 01 07 00 01 00 00 00 55 E3 FD 16
            if(status68==2)
            {
                if(((rsbuf[4]&0x0f)==(Link_control&0x0f))&&(rsbuf[7]==type_identification)&&(rsbuf[9]==transmit_reason))
                {
                    temp=1;
                    //帧的处理过程结束
                    USART2_FRAME_status=0;
                    break;
                }
                else
                {   temp=0;
                    //测试，把接收到的帧转发串口3
                    USART_printf(&huart3,"mm66666 MCU!\n");

                    //帧的处理过程结束
                    USART2_FRAME_status=0;
                }
            }
            //********************非固定长度帧处理结束

            //帧的处理过程结束
            USART2_FRAME_status=0;

        }


    }

    return temp;

}


/**********************************

*/

/*
功能：发送心跳包
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
68 0B 0B 68 53 01 00 64 01 06 01 00 00 00 14 D4 16（总召唤命令 激活）
		应答为OK帧 "\x10\x80\x01\x00\x81\x16"
		遥测，带品质，不带时标 68 53 53 68 73 01 00 09 98 14 00 01 00 01 40 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 6B 16 遥测，带品质，不带时标
10 00 01 00 01 16 肯定确认



功能：总召命令
*/
extern uint8_t MY_MCU_RsBuf[8];
extern uint8_t MY_MCU_getdata_status;
uint16_t MY_Bat_value=0;  //2016-06-12  修改 读取电池电压值
uint16_t MY_Sun_value=0;  //2016-08-30  修改 读取SUN电压值
uint16_t MY_Temperature_value=0; //2016-08-30
uint16_t MY_Humidity_value=0; //2016-08-30

uint8_t WDZ_MCUtransmit_Calldata(void)
{
    uint8_t my_status=0;
    uint8_t k=0;
    uint8_t my_rxbuf[256]="\x10\x80\x01\x00\x81\x16";
    uint8_t my_txbuf[18]=TX101_calldata;

    //主发--命令
    //从发--肯定确认

    my_status=WDZ_MCUtransmint_commd_wait_commd(2,my_txbuf,1,my_rxbuf);

    //Delay_us(300); 	//测试使用

    //从发--遥测
    if(my_status==1)
    {
        my_status=WDZ_MCUreceive_testactive(0x03,0x09,0x14,300);
        //收到数据帧，进行处理，存入数组中

        if(my_status==1)
        {
            MY_MCU_getdata_status=1;  //表示收到总召数据了
            for(k=0; k<8; k++)
            {
                MY_MCU_RsBuf[k]=USART2_my_frame[k+14];
            }
            //2016-06-12 取电池电压
            MY_Bat_value=MY_MCU_RsBuf[1];
            MY_Bat_value=(MY_Bat_value<<8)+MY_MCU_RsBuf[0];
            //2016-08-30 取太阳能，温度、湿度
            MY_Sun_value=MY_MCU_RsBuf[3];
            MY_Sun_value=(MY_Sun_value<<8)+MY_MCU_RsBuf[2];

            MY_Temperature_value=MY_MCU_RsBuf[5];
            MY_Temperature_value=(MY_Temperature_value<<8)+MY_MCU_RsBuf[4];

            MY_Humidity_value=MY_MCU_RsBuf[7];
            MY_Humidity_value=(MY_Humidity_value<<8)+MY_MCU_RsBuf[6];

        }
        else
        {
            MY_MCU_getdata_status=1;  //表示收到总召数据了
            for(k=0; k<8; k++)
            {
                MY_MCU_RsBuf[k]=0XFF;
            }
            MY_Bat_value=0XFFFF;  //AD设备出现问题，取0xffff，用最慢速度发送数据

        }

    }
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3," MCU--call1!\r\n ");
        //LED3_TOGGLE;
    }

    //主DTU--发肯定确认
    if(my_status==1) //发送肯定确认命令
    {

        //USART_printf(&huart2,TX101_OKdata);
        my_UART4_printf(&huart2,TX101_OKdata);

        //Delay_us(100);//测试使用
        USART_printf(&huart3," MUC--call*\r\n "); //测试使用

        //LED2_TOGGLE;
    }
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3," MUC--call-\r\n ");
        //LED3_TOGGLE;
    }

    return my_status;
}


