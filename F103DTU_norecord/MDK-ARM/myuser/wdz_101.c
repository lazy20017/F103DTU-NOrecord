#include "wdz_101.h"
//==========
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;

//==========
extern uint8_t rsbuf1[];	  //USART1缓冲器
extern uint8_t rsbuf2[];   //USART2缓冲器
extern uint8_t rsbuf3[];   //USART2缓冲器
extern uint8_t rsbuf4[];   //USART2缓冲器
extern uint8_t rsbuf5[];   //USART2缓冲器

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

extern struct rtc_time systmtime;  //RTC实时时钟使用
extern u8 rtcbuffer[];

extern uint16_t Transmint_to_GPRS_hearttime;
extern uint16_t Transmint_to_GPRS_Cycledata;

extern uint16_t my_tim6_count;

//通用部分

//串口1
extern uint16_t USART1_address_first;
extern uint16_t USART1_address_second;
extern uint8_t USART1_length;
extern uint8_t USART1_FRAME_status;
extern uint8_t USART1_my_frame[256];
extern uint8_t USART1_TRANSMINT_STATUS;
extern uint8_t USART1_RESIVER_STATUS;

//串口2
uint16_t USART2_address_first=0;
uint16_t USART2_address_second=0;
uint8_t USART2_length=0;
uint8_t USART2_FRAME_status=0;
uint8_t USART2_my_frame[256]= {0};
uint8_t USART2_TRANSMINT_STATUS=0;
uint8_t USART2_RESIVER_STATUS=0;
//串口3
uint16_t USART3_address_first=0;
uint16_t USART3_address_second=0;
uint8_t USART3_length=0;
uint8_t USART3_FRAME_status=0;
uint8_t USART3_my_frame[100]= {0};
uint8_t USART3_TRANSMINT_STATUS=0;
uint8_t USART3_RESIVER_STATUS=0;

//串口4
uint16_t USART4_address_first=0;
uint16_t USART4_address_second=0;
uint8_t USART4_length=0;
uint8_t USART4_FRAME_status=0;
uint8_t USART4_my_frame[100]= {0};
uint8_t USART4_TRANSMINT_STATUS=0;
uint8_t USART4_RESIVER_STATUS=0;

//-----433模拟量转发
uint8_t my_433_anlag_buf[110]= {0}; //存储433模拟量，不进行清0处理，不存储，只要有变化，利用标志位进行转发。
uint8_t my_433_anlag_flag=0;  //标志位，为0表示已经发送了新数据，为1表示有新数据但是还没有发送

//433模块 总召数据存储数组缓冲区

extern uint8_t MY_433_Call_Single_data_buf[256];
extern uint8_t MY_433_Call_Analog_data_buf[256];
extern uint8_t MY_433_Call_Time_data_buf[7];
extern uint8_t MY_433_Call_Single_data_number;
extern uint8_t MY_433_Call_Analog_data_number;
extern uint8_t MY_433_Call_Status;  //433模块总召数据状态，为1表示收到数据并存入到数组缓冲区中，0为没有数据



/*
功能：数据接收分析，此部分为分析101协议数据，有帧头，有帧尾，有帧长，这三个参数很重要，取完整的一帧数据。
输入参数：串口号
输出参数： 取帧数据成功返回1，取帧数据失败返回0
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



//******串口的选择****************
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

////////****串口选择结束***************



    while(*USART_FRAME==0 && *rsbufpt_COMM_read!=*rsbufpt_write)  //前一个指令处理完成，还有未处理的字符，则进行处理。结束条件，有完整的一条指令或者所有字符处理完成
    {

        //取一个未处理的字符
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

        //进行0X68帧头和帧尾标记
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


        //进行0X68帧头分析
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
        //进行由帧尾到帧头的分析
        if(tmp_status==2)
        {
            temp_address_first=*address_first;
            temp_length=*length;
            //固定长度帧
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
            //非固定长度帧
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

//取一帧数据存入到指令数组中



    if(my_temp_status>0)	  //如果有完整一帧数据，就开始处理，否则返回，不处理
    {
        //清空命令数组区
        for(ii=0; ii<256; ii++)
        {
            my_frame[ii]=0;
        }

        //取固定长度指令
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
        //取非固定长度指令
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

    //进行返回处理
    if(status==1)
    {
        //*USART_FRAME=0;  //取完一帧指令进行标记
        //printf("\r\nOK=%s",my_frame);
        //USART_printf(USARTx,"\r\nOK=%s",my_frame);
        //USART_printf(USARTx,"%s",my_frame);
        return(1);
    }
    else
    {
        //USART_FRAME=0;  //取完一帧指令进行标记
        //printf("\r\nERROR");
        return(0);
    }

}



/*
功能：串口2 pwr从MCU  被动接收 指令处理程序，分析被动接收的指令，产生动作
输入：无
输出；无
*/
void my_process_resive_101usart2()
{
    uint8_t status68=0;
    //uint8_t status10=0;
    uint16_t my_dtu_address=0;
    uint8_t *pt;
    uint16_t ii=0;
    uint16_t n=0;

    //接收到的指令进行分析，如果满足条件，进行处理，不满足报错,用来分析心跳、故障、参数设定等 其它站点主动发送的程序
    //但如果程序正在处于主动发送，等待应答时，此程序不能运行分析帧，而是由发送命令程序，自己进行查询，防止对帧的冲突操作
    if(USART2_FRAME_status>0)
    {
        //测试使用
        //USART_printf(&huart1,"\r\n");
        //USART_printf(&huart1,USART1_my_frame);
        //USART_printf(&huart1,"\r\n");
        //if(USART1_my_frame[1]==0x31) USART_printf(&huart1,"\r\ndata is ok !!\r\n");
        //else USART_printf(&huart1,"\r\ndata is error !!\r\n");
        //------------------

        //帧头检查
        if(USART2_my_frame[0]==0x68)
        {   status68=1;
            //status10=0;
        }

        else if(USART2_my_frame[0]==0x10)
        {   status68=0;
            //status10=1;
        }
        //从站地址检查
        if(status68==1)
        {
            my_dtu_address=USART2_my_frame[6];
            my_dtu_address=(my_dtu_address<<8)+USART2_my_frame[5];
            if(my_dtu_address==0x0001)//DTU_ADDRESS)
                status68=1;
            else status68=0;
        }
        //校验字节检查

        //帧类型检查、传输原因检查
        if(status68==1)
        {
            if(USART2_my_frame[7]==0X67&&USART2_my_frame[9]==0X06)// 时钟同步处理
            {

                rtcbuffer[0]=USART2_my_frame[14];
                rtcbuffer[1]=USART2_my_frame[15];
                rtcbuffer[2]=USART2_my_frame[16]&0x3f;
                rtcbuffer[3]=USART2_my_frame[17]&0x1f;
                rtcbuffer[4]=USART2_my_frame[18]&0x1f;
                rtcbuffer[5]=USART2_my_frame[19]&0x0f;
                rtcbuffer[6]=USART2_my_frame[20]&0x7f;

                my_array_to_RTCtime(rtcbuffer); //写入时间进RTC

                my_process_transmit_101usart2(COMM_101_OK,1,100);  //发送确认指令

                //Delay_us(100); //测试使用

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
                my_process_transmit_101usart2(txbuf1,1,100);  //发送时间同步确认激活指令
                //
                status68=0;   //处理完
            }
            else status68=0;
        }


        USART2_FRAME_status=0;
    }
}


/*
功能：串口1主动101发送数据,没有使用
输入参数：指令地址，应答指令地址，重复发送此次，重复发送间隔时间
输出参数：发送成功返回1，失败返回0
*/
uint8_t my_process_transmit_101usart1(uint8_t *commpt,uint8_t N,uint16_t Time)
{
    uint8_t tempstatus=0;
    uint8_t n=0;
    uint16_t time_stop=Time/100;
    uint16_t my_start_time=0;

//被动接收的指令是否处理完了，处理完了，就行，没处理完，延时，如果还不行，退出报错。
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

    	USART1_FRAME_status=0; //这个是测试使用，如果正常运行应该删除，需要在返回指令中进行分析处理

    }
    */
    return (tempstatus);  //发送指令，并有返回指令，这算一次成功发送，返回1，如果没有返回指令算失败


}


/*
测试使用函数，没有使用

*/
void my_test_usart1(uint8_t *my_COMM_TRANSMIT)  //测试101主动发送命令程序
{
    uint8_t *COMM_TRANSMIT;
    uint8_t tempstatus=0;
    //LED2_TOGGLE;  //测试使用
    //

    if(USART1_RESIVER_STATUS==0)
    {
        USART1_TRANSMINT_STATUS=1;
        COMM_TRANSMIT=my_COMM_TRANSMIT;  //发送的命令
        tempstatus=my_process_transmit_101usart1(COMM_TRANSMIT,3,50);//接收的命令是否成功

        if(tempstatus==1)       //接收命令成功处理
        {
            USART_printf(USART_DEBUG,"TRANSMIT DATA IS OK ,AND RESIVE A ANSWER DATA=");
            USART_printf(USART_DEBUG,USART1_my_frame);
            USART_printf(USART_DEBUG,"\r\n");
            USART1_FRAME_status=0; //这个是测试使用，如果正常运行应该删除，需要在返回指令中进行分析处理
        }
        else if(tempstatus==0)
        {
            USART_printf(USART_DEBUG,"TRANSMIT DATA IS ERROR ,NO ANSWER COMM==%s\r\n",COMM_TRANSMIT);
            USART1_FRAME_status=0; //这个是测试使用，如果正常运行应该删除，需要在返回指令中进行分析处理
        }

        USART1_TRANSMINT_STATUS=0;

    }
}
/*
功能：串口2利用GPRS 主动101发送数据
输入参数：指令地址，应答指令地址，重复发送此次，重复发送间隔时间
输出参数：发送成功返回1，失败返回0
*/
uint8_t my_process_transmit_101usart2(uint8_t *commpt,uint8_t N,uint16_t Time)
{
    uint8_t tempstatus=0;
    uint8_t n=0;
    uint16_t time_stop=Time/100;
    uint16_t my_start_time=0;

//被动接收的指令是否处理完了，处理完了，就行，没处理完，延时，如果还不行，退出报错。
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

    	USART1_FRAME_status=0; //这个是测试使用，如果正常运行应该删除，需要在返回指令中进行分析处理

    }
    */
    return (tempstatus);  //发送指令，并有返回指令，这算一次成功发送，返回1，如果没有返回指令算失败


}



//*************USART3 WDZ 测试使用*************//

/*
功能：串口3被动接收 指令处理程序，分析被动接收的指令，产生动作
     (1)67-06测试进行RTC校时，利用101协议格式68 11 11 68 D3 01 00 67 01 06 01 00 00 00 /08 67 0F 07 02 07 0D/ 75 16
		 (2)67-07DTU地址设置EEPROM，             68 0C 0C 68 D3 01 00 67 01 07 01 00 00 00 /02 00/ 75 16
		 (3)67-08 IP地址
		 (4)67-09 端口
		 (5)67-0A  DTU地址，代码中的变量地址
		 (6)67-0B  重启MCU
		 (7)67-0C 心跳包时间间隔
		 (8)67-0D  周期包时间间隔

输入：无
输出；无
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




    //接收到的指令进行分析，如果满足条件，进行处理，不满足报错,用来分析心跳、故障、参数设定等 其它站点主动发送的程序
    //但如果程序正在处于主动发送，等待应答时，此程序不能运行分析帧，而是由发送命令程序，自己进行查询，防止对帧的冲突操作
    if(USART3_FRAME_status>0)
    {
        //关闭定时器4

        __HAL_RCC_TIM4_CLK_DISABLE();


        //帧头检查
        if(USART3_my_frame[0]==0x68)
        {
            status68=2;
        }

        else if(USART3_my_frame[0]==0x10)
        {
            status68=1;
        }
        //从站地址检查
        //if(status68==2)
        //{
        //	my_dtu_address=USART3_my_frame[6];
        //	my_dtu_address=(my_dtu_address<<8)+USART3_my_frame[5];
        //if(my_dtu_address==DTU_ADDRESS)
        //			status68=2;
        //else status68=0;
        //	}
        //校验字节检查
        //temp=wdz_101char_check(USART3_my_frame);  //测试时可以考虑先不使用
        //if(temp==1) status68=2;
        //else status68=0;

        //帧类型检查、传输原因检查
        if(status68==2)
        {

            //1 校时************
            if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X06)// 时钟同步处理 67-06校时
            {

                rtcbuffer[0]=USART3_my_frame[14];
                rtcbuffer[1]=USART3_my_frame[15];
                rtcbuffer[2]=USART3_my_frame[16]&0x3f;
                rtcbuffer[3]=USART3_my_frame[17]&0x1f;
                rtcbuffer[4]=USART3_my_frame[18]&0x1f;  //日
                rtcbuffer[5]=USART3_my_frame[19]&0x0f;  //月
                rtcbuffer[6]=USART3_my_frame[20]&0x7f; //年

                my_array_to_RTCtime(rtcbuffer); //写入时间进RTC


                //以下回复部分
                my_RTC_TO_EEPROM(rtcbuffer,0);

                USART_printf(&huart3,tx_buf); //发送确认数据
                my_RTCtime_to_array(time_buf);  //获得当前时间
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //发送校时包含确认帧


                //status68=0;   //处理完
            }
            //校时******完********

            //2  DTU地址修改*******
            //(2)67-07DTU地址设置，68 0C 0C 68 D3 01 00 67 01 07 01 00 00 00 /02 00/ 75 16
            else if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X07) //DTU地址修改 67-07
            {
                my_dtu_address=USART3_my_frame[15];
                my_dtu_address=(my_dtu_address<<8)+USART3_my_frame[14];
                DTU_ADDRESS=my_dtu_address;


                AT25_WriteByte(USART3_my_frame[14],EEPROM_DTU_Address); //把地址写到EEPROM中
                AT25_WriteByte(USART3_my_frame[15],EEPROM_DTU_Address+1);
                USART_printf(&huart3,tx_buf); //发送OK确认数据


                time_buf[0]=AT25_ReadByte(EEPROM_DTU_Address);
                time_buf[1]=AT25_ReadByte(EEPROM_DTU_Address+1);
                time_buf[2]	=0XFF;
                time_buf[3]	=0XFF;
                time_buf[4]	=0XFF;
                time_buf[5]	=0XFF;
                time_buf[6]	=0XFF;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x07;

                USART_printf(&huart3,tx_buf2); //发送新DTU地址到

                my_reset2_mcu(); //软重启MUC

            }
            //**********DTU地址修改***完****


            //3 *****IP地址*****
            if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X08)// 时钟同步处理 67-06校时
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


                //把新的Server的IP地址及端口号写入到EEPROM中

                SPI_EE_BufferWrite2(MY_IP,EEPROM_IP_Address,4); //把默认IP地址 写入到EEPROM中 222.222.118.3，都是十进制的


                USART_printf(&huart3,tx_buf); //发送确认数据


                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x08;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //发送校时包含确认帧

                my_reset2_mcu(); //软重启MUC
                //status68=0;   //处理完
            }
            // ***IP地址*完********

            //4 *****PORT地址*****
            if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X09)// 时钟同步处理 67-09校时
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
                MY_PORT=((temp_16<<8)&0XFF00)+time_buf[0];  //有问题，不能加上time buf【0】


                temp_16=MY_PORT;    //默认端口号，8080,都是十进制的
                AT25_WriteByte(time_buf[0],EEPROM_IPPort_Address);
                AT25_WriteByte(time_buf[1],EEPROM_IPPort_Address+1);


                USART_printf(&huart3,tx_buf); //发送OK确认数据


                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x09;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //发送PORT数据包

                my_reset2_mcu(); //软重启MUC
                //status68=0;   //处理完
            }
            // ***PORT地址*完********

            // byte[] xx=new byte[]{ 0x68, 0x0C, 0x0C, 0x68, 0xD3, 0x01, 0x00, 0x67, 0x01, 0x20, 0x01, 0x00, 0x00, 0x00, 0xAA, 0xBB,0x8F, 0x16 };  //67-20

            //5 *****重启指令*****
            if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X0B)// 时钟同步处理 67-0B重启
            {

                WDZ_MCUtransmit_restdata();

                //	Delay_ms(100);

                WDZ_MCUtransmit_restdata();
                //		Delay_ms(100);

                WDZ_MCUtransmit_restdata();
                //		Delay_ms(100);


                USART_printf(&huart3,tx_buf); //发送OK确认数据





                //status68=0;   //处理完
            }
            // ***重启完成*******

            //6 *****心跳包时间*****
            if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X0C)// 时钟同步处理 67-09校时
            {

                time_buf[0]=USART3_my_frame[14]; //time_low
                time_buf[1]=USART3_my_frame[15]; //time_high
                time_buf[2]=USART3_my_frame[16];
                time_buf[3]=USART3_my_frame[17];
                time_buf[4]=USART3_my_frame[18];
                time_buf[5]=USART3_my_frame[19];
                time_buf[6]=USART3_my_frame[20];

                temp_16=time_buf[1];
                Transmint_to_GPRS_hearttime=((temp_16<<8)&0XFF00)+time_buf[0];  //有问题，不能加上time buf【0】


                temp_16=Transmint_to_GPRS_hearttime;    //默认端口号，8080,都是十进制的
                AT25_WriteByte(time_buf[0],EEPROM_Hearttime_Address);
                AT25_WriteByte(time_buf[1],EEPROM_Hearttime_Address+1);



                USART_printf(&huart3,tx_buf); //发送OK确认数据


                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x0C;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //发送PORT数据包


                //status68=0;   //处理完
            }
            // ***心跳包时间*完********

            //7 *****周期包时间*****
            if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X0D)// 时钟同步处理 67-09校时
            {

                time_buf[0]=USART3_my_frame[14]; //time_low
                time_buf[1]=USART3_my_frame[15]; //time_high
                time_buf[2]=USART3_my_frame[16];
                time_buf[3]=USART3_my_frame[17];
                time_buf[4]=USART3_my_frame[18];
                time_buf[5]=USART3_my_frame[19];
                time_buf[6]=USART3_my_frame[20];

                temp_16=time_buf[1];
                Transmint_to_GPRS_Cycledata=((temp_16<<8)&0XFF00)+time_buf[0];  //有问题，不能加上time buf【0】


                temp_16=Transmint_to_GPRS_Cycledata;    //默认端口号，8080,都是十进制的
                AT25_WriteByte(time_buf[0],EEPROM_Cycetime_Address);
                AT25_WriteByte(time_buf[1],EEPROM_Cycetime_Address+1);



                USART_printf(&huart3,tx_buf); //发送OK确认数据


                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x0D;
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                USART_printf(&huart3,tx_buf2); //发送PORT数据包


                //status68=0;   //处理完
            }
            // ***心跳包时间*完********

            //***查询参数
            else if(USART3_my_frame[7]==0X67&&USART3_my_frame[9]==0X20) //DTU地址修改 67-20
            {

                USART_printf(&huart3,tx_buf); //发送OK确认数据

                //---------RTC----------
                my_RTCtime_to_array(time_buf);  //获得当前时间
                my_buf1_to_buf2(time_buf,0,tx_buf2,14,7);
                tx_buf2[9]=0x06;
                USART_printf(&huart3,tx_buf2); //发送校时包含确认帧
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

                USART_printf(&huart3,tx_buf2); //发送EEPROM的DTU地址到

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

                USART_printf(&huart3,tx_buf2); //发送新DTU地址到



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
                USART_printf(&huart3,tx_buf2); //发送校时包含确认帧

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
                USART_printf(&huart3,tx_buf2); //发送PORT

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
                USART_printf(&huart3,tx_buf2); //发送PORT
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
                USART_printf(&huart3,tx_buf2); //发送PORT
                //

            }
            //else status68=0;
        }






        USART3_FRAME_status=0;

        //打开定时器4
        //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
        __HAL_RCC_TIM4_CLK_ENABLE();
    }
}

/*
校验字检查
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

    if(status68==1) //固定长度校验位检查
    {
        check_char=buffer[1]+buffer[2]+buffer[3];
        if(check_char==buffer[4])
            temp=1;
        else temp=0;


    }
    else if(status68==2) //非固定长度校验位检查
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
生成校验字
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


    if(status68==1) //固定长度校验位检查
    {
        check_char=buffer[1]+buffer[2]+buffer[3];
        buffer[4]=check_char;
    }
    else if(status68==2) //非固定长度校验位检查
    {
        for(k=0; k<buffer[1]; k++)
        {
            check_char=check_char+buffer[k+4];
        }
        buffer[buffer[1]+4]=check_char;
    }


}

//***********WDZ-USART4 433模块接收数据处理部分
/*
命令字符串复制到数组中
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
功能：分析被动接收到的数据，主要是报警数据处理，非报警数据回复OK帧，丢弃数据包
分析：报警数据是先发无时标数据包，然后发有时标数据包，无时标数据包可能存在多条数据，都发送完会最后发送有时标数据包
*/

extern uint8_t MY_433_Alarmdata_NOtimeBuf[]; //存储 无时标 报警数据
extern uint8_t MY_433_Alarmdata_TimeBuf[];  //存储，有时标，报警数据
extern uint8_t MY_433_ALarmdata_number;  // 存储，报警信息体个数


extern	uint8_t MY_433_ALarmdata_NOtime_status; //为1，表示收到无时标报警数据
extern	uint8_t MY_433_ALarmdata_Time_status;   //为1，表示收到有时标报警数据



extern uint8_t MY_433_Alarmdata_NOtime_WriteAddress;

void my_process_resive_101usart4(void)
{

    uint8_t status68=0;  //2代表非固定长度帧，1代表固定长度6的帧，0代表问题或处理完成
    uint16_t my_dtu_address=0;
    uint8_t temp=0;
    uint8_t *rsbuf=USART4_my_frame;
    uint8_t mess_number=0;
    uint8_t k=0;
    uint8_t first_address=0;
    uint8_t rs_temp_buf[512]= {0};
    uint8_t my_status=0;
    int my_index=0;

    //my_test_alarm_data();  //测试使用，产生人工报警数据，@@@@@

    //接收到的指令进行分析，如果满足条件，进行处理，不满足报错,用来分析心跳、故障、参数设定等 其它站点主动发送的程序
    //但如果程序正在处于主动发送，等待应答时，此程序不能运行分析帧，而是由发送命令程序，自己进行查询，防止对帧的冲突操作
    if(USART4_FRAME_status>0)
    {
        //---调试，16进制转字符串，输出字符串 68-0D-0D-68-73-01-00-09-01-03-01-00-08-40-00-00-00-CA-16
        HexToStr(rs_temp_buf,rsbuf,0);

        USART_printf(&huart3,rs_temp_buf);
        USART_printf(&huart3,"\r\n");
        //------------------------------------------

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
            if(my_dtu_address==0x0001)  //DTU_ADDRESS)
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



        //帧类型检查、传输原因检查，固定长度报警帧检查
        if(status68==1)
        {
            WDZ_101Transmit_OKdata();//对收到的非报警数据进行丢包，只回复一个OK帧
            //测试，把接收到的帧转发串口3
            //USART_printf(&huart3,"1111");
            //USART_printf(&huart3,rsbuf);
            //USART_printf(&huart3,"1111");
            USART_printf(&huart3,"433-1 No Alarmdata_10h \r\n"); //调试使用
        }



        //************0x68非固定长度帧处理 68-0B-0B-68-73-01-00-01-01-03-01-00-05-00-01-80-16
        //从站地址检查
        my_status=0;
        if(status68==2)
        {
            my_dtu_address=rsbuf[6];
            my_dtu_address=(my_dtu_address<<8)+rsbuf[5];
            if(my_dtu_address==0x0001)//DTU_ADDRESS)
            {
                my_status=2;
                //USART_printf(&huart3,"address OK\r\n"); //测试使用

            }
            else
            {
                my_status=0;
                USART_printf(&huart3,"433 address error\r\n"); //测试使用
            }
        }
        //校验字节检查

        if(my_status==2)
        {
            temp=wdz_101char_check(rsbuf);
            if(temp==1)
            {
                //USART_printf(&huart3,"char check OK\r\n"); //测试使用
                my_status=2;
            }
            else
            {
                my_status=0;
                USART_printf(&huart3,"433 char check error\r\n");
            }

        }


//*******************重要****************
        //域控制码53H/73H、帧类型检查01H（无时标）/2EH（有时标）、传输原因检查03H，非固定长度报警帧数量
        if(my_status==2)
        {
            //*********遥信数据处理  68-0D-0D-68-53-01-00-09-01-03-01-00-01-40-11-00-00-B4-16
            if((rsbuf[4]&0x0f)==0x03 &&rsbuf[7]==0x01&&rsbuf[9]==0x03)  //遥信，突发、无时标，数据  单点信息(突发)68 0B 0B 68 73 01 00 01 01 03 00 01 08 00 01 83 16
            {
                mess_number=rsbuf[8]&0x7f; //信息体数量


                first_address=MY_433_Alarmdata_NOtime_WriteAddress;  //取数组中空数据单元首地址

                for(k=0; k<mess_number*3; k++) //每个信息体占3个字节，地址2个，值一个
                {
                    MY_433_Alarmdata_NOtimeBuf[k+first_address]=rsbuf[12+k];
                }
                first_address=first_address+k;
                MY_433_Alarmdata_NOtime_WriteAddress=first_address;  //移动地址计数器到空数据单元首地址，目的是利用数组把本次报警的多个信息体数据存在一起

                WDZ_101Transmit_OKdata();  //发送确认指令

                //不存入EEPROM中，只进行标识， 有 无时标报警数据产生
                MY_433_ALarmdata_NOtime_status=1;

                USART_printf(&huart3," 433 Alarmdata NO_time\r\n"); //调试使用


            }



            else if((rsbuf[4]&0x0f)==0x03 &&rsbuf[7]==0x1E&&rsbuf[9]==0x03)  //遥信，突发、长时标，数据 带长时标的单点信息(突发) 68 1C 1C 68 73 01 00 1E 02 03 01 00 08 00 01 20 BA 16 0F 05 07 0D 05 00 01 49 BC 16 0F 05 07 0D 02 16
            {
                mess_number=rsbuf[8]&0x7f; //信息体数量
                MY_433_ALarmdata_number=mess_number;
                //MY_433_Alarmdata_TimeBuf[0]=mess_number;

                for(k=0; k<mess_number*(3+7); k++) //每个信息体占10个字节，地址2个，值一个，时标7个
                {
                    MY_433_Alarmdata_TimeBuf[k]=rsbuf[12+k];
                }
                WDZ_101Transmit_OKdata();   //发送确认指令




                //标识为有 有带时标报警数据 产生，等待GPS进行数据处理
                MY_433_ALarmdata_Time_status=1;
                MY_433_Alarmdata_NOtime_WriteAddress=0;

                //存入到EEPROM中，标识为未进行GPRS发送----重要--------
                my_alarm_data_to_eeprom();

                USART_printf(&huart3," 433 Alarmdata with time\r\n"); //调试使用

            }
            //***********遥测数据处理，根据原理，遥测数据应该没有报警68-0D-0D-68-53-01-00-09-01-03-01-00-01-40-11-00-00-B4-16

            else if((rsbuf[4]&0x0f)==0x03 &&rsbuf[7]==0x09&&rsbuf[9]==0x03)  //遥测，突发、无时标，数据  单点信息(突发)68 0B 0B 68 73 01 00 01 01 03 00 01 08 00 01 83 16
            {



                WDZ_101Transmit_OKdata();   //发送确认指令,非报警数据，只回复OK帧，丢掉数据；2016-4-6方案修改，把此数据利用GPRS进行转发
                USART_printf(&huart3," 433 No Alarmdata_68h-1 analog data1\r\n"); //调试使用

                my_index=rsbuf[12];
                my_index=(my_index-1)*3;
                //判断原则，只要本次采集的数据，与已经存储的数据不一样，就进行转发，但是转发的时间采用周期控制的方式，在TIM4中处理发送
                if(my_433_anlag_buf[my_index+0]!=rsbuf[14]||my_433_anlag_buf[my_index+1]!=rsbuf[15]||my_433_anlag_buf[my_index+2]!=rsbuf[16] )
                {
                    my_433_anlag_buf[my_index+0]=rsbuf[14];
                    my_433_anlag_buf[my_index+1]=rsbuf[15];
                    my_433_anlag_buf[my_index+2]=rsbuf[16];
                    my_433_anlag_flag=1;   //1表示有新数据产生，发送完后清零。
                }




            }

            else if((rsbuf[4]&0x0f)==0x03 &&rsbuf[7]==0x22&&rsbuf[9]==0x03)  //遥测，突发、长时标，数据 带长时标的单点信息(突发) 68 1C 1C 68 73 01 00 1E 02 03 01 00 08 00 01 20 BA 16 0F 05 07 0D 05 00 01 49 BC 16 0F 05 07 0D 02 16
            {

                WDZ_101Transmit_OKdata();   //发送确认指令,非报警数据，只回复OK帧，丢掉数据
                USART_printf(&huart3,"433  No Alarmdata_68h-2 analog data2\r\n"); //调试使用
            }

            else
            {
                WDZ_101Transmit_OKdata();   //发送确认指令,非报警数据，只回复OK帧，丢掉数据
                USART_printf(&huart3,"433 No Alarmdata_68h-3\r\n"); //调试使用

                //---调试，16进制转字符串，输出字符串
                //HexToStr(rs_temp_buf,USART4_my_frame,0);
                //USART_printf(&huart3,rs_temp_buf);
                //USART_printf(&huart3,"\r\n");
                //----
            }


            //测试，把接收到的帧转发串口3
            //USART_printf(&huart3,"2222");
            //USART_printf(&huart3,rsbuf);
            //USART_printf(&huart3,"2222");
        }
        //********************非固定长度帧处理结束

        //帧的处理过程结束
        USART4_FRAME_status=0;
        //测试，把非报警帧显示出来

        //USART_printf(&huart3,"7777");
        //USART_printf(&huart3,rsbuf);
        //USART_printf(&huart3,"2227");

    }

}



/*
功能：接收数据肯定确认 判断，收到确认包，返回1，没收到返回0

10 8B 01 00 8C 16(旧的，错的) 10 80 01 00 81 16
10 00 01 00 01 16（新的）
*/
uint8_t WDZ_101recive_OKdata(void)
{
    uint8_t temp=0;
    uint8_t status68=0;  //2代表非固定长度帧，1代表固定长度6的帧，0代表问题或处理完成
    uint16_t my_dtu_address=0;
    uint8_t *my_rsbuf=USART4_my_frame;

    my_usart_101frame(4);
    if(USART4_FRAME_status>0)
    {
        //帧头检查
        if(USART4_my_frame[0]==0x68)
        {
            status68=2;
        }

        else if(USART4_my_frame[0]==0x10)
        {
            status68=1;
        }


//************0x10固定长度帧处理
        //站地址检查
        if(status68==1)
        {
            my_dtu_address=USART4_my_frame[3];
            my_dtu_address=(my_dtu_address<<8)+USART4_my_frame[2];
            if(my_dtu_address==0x0001)//DTU_ADDRESS)
                status68=1;
            else status68=0;
        }
        //校验字节检查
        if(status68==1)
        {
            temp=wdz_101char_check(USART4_my_frame);  //
            if(temp==1) status68=1;
            else status68=0;
        }



        //帧类型检查、传输原因检查
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

//************0x68非固定长度帧处理
        //从站地址检查
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
        //校验字节检查
        /*
        	if(status68==2)
        	{
        	temp=wdz_101char_check(USART4_my_frame);  //测试时可以考虑先不使用
        	if(temp==1) status68=2;
        	else status68=0;
        	}



        		//帧类型检查、传输原因检查
        		if(status68==2)
        		{
        			//测试，把接收到的帧转发串口3
        			USART_printf(&huart3,"33333");
        			USART_printf(&huart3,USART4_my_frame);
        			USART_printf(&huart3,"3333");
        		}
        		*/
//********************非固定长度帧处理结束

        //帧的处理过程结束
        USART4_FRAME_status=0;

    }



    return temp;
}



/*

功能：通用型发送命令，等待肯定确认函数，主动发送指令，收到肯定确认指令返回1，没收到确认返回0
入口参数：type为类型，1为固定长度，2为非固定长度，txbuf为发送指令数组
					address_type为1，则应用地址，先发低字节，后发高字节，属于默认规则
									 如果为2，则先发高字节，后发低字节，有些指令只认这个，没办法

*/



uint8_t WDZ_101transmint_commd(uint8_t type,uint8_t *txbuf,uint8_t address_type)  //type为类型，1为固定长度，2为非固定长度，txbuf为发送指令数组
{
    uint8_t FCB=0x20;
    uint16_t my_dtu_address=0x0001; //  DTU_ADDRESS;
    uint8_t my_status=0;
    uint8_t n=3;
    uint8_t *my_txbuf=txbuf;
    uint16_t my_start_time=0;

    if(type==2)
    {
        //控制域码处理
        if(WDZ_101FCB==0x00)
            my_txbuf[4]=my_txbuf[4]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[4]=my_txbuf[4]|(FCB);
        //目的地址处理
        my_txbuf[5]=my_dtu_address&0xFF;
        my_txbuf[6]=my_dtu_address>>8;


        if(address_type==1)
        {
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }
        else if(address_type==2)
        {
            //my_txbuf[10]=my_txbuf[6]; //发送指令，不等待回复
            //my_txbuf[11]=my_txbuf[5];
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }

    }
    else if(type==1)
    {
        //控制域码处理
        if(WDZ_101FCB==0x00)
            my_txbuf[1]=my_txbuf[1]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[1]=my_txbuf[1]|(FCB);
        //目的地址处理

        my_txbuf[5]=my_dtu_address&0xFF;
        my_txbuf[6]=my_dtu_address>>8;



    }


    //校验位处理
    wdz_101check_generate(my_txbuf);
    //发送指令

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
功能：发送命令，等待（特定）反馈命令
*/
uint8_t WDZ_101transmint_commd_wait_commd(uint8_t type,uint8_t *txbuf,uint8_t address_type,uint8_t *rxbuf)  //type为类型，1为固定长度，2为非固定长度，txbuf为发送指令数组
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
//********************发送非固定长度帧，应答为OK帧
    if(type==2 && address_type==1)
    {
        //控制域码处理
        if(WDZ_101FCB==0x00)
            my_txbuf[4]=my_txbuf[4]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[4]=my_txbuf[4]|(FCB);
        //目的地址处理
        my_txbuf[5]=my_dtu_address&0xFF;
        my_txbuf[6]=my_dtu_address>>8;



        if(address_type==1)
        {
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }
        else if(address_type==2)
        {
            //my_txbuf[10]=my_txbuf[6];  //发送指令，等待回复
            // my_txbuf[11]=my_txbuf[5];
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }

        //校验位产生
        wdz_101check_generate(my_txbuf);
        //发送指令
        while((n--)>0&&my_status==0)
        {
            my_UART4_printf(&huart4,my_txbuf);

            //Delay_us(300); //测试使用

            my_start_time=my_tim6_count;
            do {

                my_status=WDZ_101recive_OKdata();  //等待0K确认反馈命令
                if(my_status==1)break;
                else my_status=0;
            } while(my_tim6_count-my_start_time<=4);

        }



    }
//********************发送非固定长度帧，应答为OK帧,针对翻牌指令设计，发送后等待1秒
    if(type==2 && address_type==2)
    {
        //控制域码处理
        if(WDZ_101FCB==0x00)
            my_txbuf[4]=my_txbuf[4]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[4]=my_txbuf[4]|(FCB);
        //目的地址处理
        my_txbuf[5]=my_dtu_address&0xFF;
        my_txbuf[6]=my_dtu_address>>8;



        if(address_type==1)
        {
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }
        else if(address_type==2)
        {
            //my_txbuf[10]=my_txbuf[6];  //发送指令，等待回复
            // my_txbuf[11]=my_txbuf[5];
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }

        //校验位产生
        wdz_101check_generate(my_txbuf);
        //产生接收的控制域码，类型，传输原因
        linkcontrol=rxbuf[4];
        type_identification=rxbuf[7];
        transmit_reason=rxbuf[9];
        //发送指令
        n=1;
        while(my_status==0 && (n--)>0)
        {
            Delay_ms(3);
            my_UART4_printf(&huart4,my_txbuf);  //关键语句

            Delay_ms(15); //测试，等待1秒

            //my_status=WDZ_101receive_testactive(linkcontrol,type_identification,transmit_reason,600);  //等待反馈命令

            my_status=1;
            if(my_status==1)
                break;
            else
                my_status=0;


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
        if(WDZ_101FCB==0x00)
            my_txbuf[1]=my_txbuf[1]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[1]=my_txbuf[1]|(FCB);
        //目的地址处理
        my_txbuf[2]=my_dtu_address&0xFF;
        my_txbuf[3]=my_dtu_address>>8;

        //校验位产生
        wdz_101check_generate(my_txbuf);
        //发送指令
        while((n--)>0&&my_status==0)
        {
            my_UART4_printf(&huart4,my_txbuf);

            //Delay_us(300); //测试

            my_status=WDZ_101receive_testactive(linkcontrol,type_identification,transmit_reason,600);  //等待反馈命令
            if(my_status==1)break;
            else my_status=0;
        }
    }
//**********发送固定长度帧，应该为非固定长度帧
    else if(type==1&&rxbuf[0]==0x68)
    {
        //产生接收的控制域码，类型，传输原因
        linkcontrol=rxbuf[4];
        type_identification=rxbuf[7];
        transmit_reason=rxbuf[9];

        //控制域码处理
        if(WDZ_101FCB==0x00)
            my_txbuf[1]=my_txbuf[1]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[1]=my_txbuf[1]|(FCB);
        //目的地址处理
        my_txbuf[2]=my_dtu_address&0xFF;
        my_txbuf[3]=my_dtu_address>>8;

        //校验位产生
        wdz_101check_generate(my_txbuf);
        //发送指令
        while((n--)>0&&my_status==0)
        {
            my_UART4_printf(&huart4,my_txbuf);

            //Delay_us(600); //测试

            my_status=WDZ_101receive_testactive(linkcontrol,type_identification,transmit_reason,600);  //等待反馈命令
            if(my_status==1)break;
            else my_status=0;
        }
    }
//发送数据

    /*
    if(type==1)
    {

    	//校验位产生
    	wdz_101check_generate(my_txbuf);
    	//发送指令
    	while((n--)>0&&my_status==0)
    	{
    		my_UART4_printf(UART4,my_txbuf);

    		Delay_us(300); //测试

    			my_status=WDZ_101receive_testactive(linkcontrol,type_identification,transmit_reason,300);  //等待反馈命令
    			if(my_status==1)break;
    			else my_status=0;
    	}
    }
    */



    //记录发送变化帧
    WDZ_101FCB=WDZ_101FCB^0x20;
    if(my_status==1)
    {
        return 1;
    }
    else return 0;

}

/*
发送OK肯定确认指令

*/

void WDZ_101Transmit_OKdata(void)
{
    uint8_t *commd=TX101_OKdata;
    my_UART4_printf(&huart4,commd);
}




/*
功能：发送心跳包，等待应答程序

发送心跳包，10 D2 01 00 D3 16 或者 10 F2 01 00 F3 16
接收确认包  10 80 01 00 81 16 或者 10 82 01 00 83 16
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



    //----处理433指令堵塞问题，先发送OK，清除堵塞，然后在发心跳包
    if(my_status==0)
    {
        //WDZ_101Transmit_OKdata(); //发送OK帧
        //my_status=WDZ_101transmint_commd(1,my_txbuf,1); //第2次发送心跳包

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

    if(my_status==0) wdz_433_heart_count++; //无心跳回复，计数一次

    if(wdz_433_heart_count>=3)
    {
        link_status_433=0; //记录3次心跳包无回复，就断开链接，重新进行链接
        wdz_433_heart_count=0; //心跳故障计数清零
    }

    return my_status;

}
//*************发送测试命令*************

/*
68 0C 0C 68 73 01 00 68 01 06 00 01 00 00 AA 55 E3 16测试命令(激活)
68 0C 0C 68 F3 01 00 68 01 06 01 00 00 00 AA 55 63 16
（肯定确认）10 80 01 00 81 16
						10 00 01 00 01 16
测试命令(激活)	68 0D 0D 68 53 01 00 68 01 2E 00 01 00 00 00 55 E3 24 16—
（传输原因2E，表示应用服务地址未知，引起的原因是发送命令是01 00）

							68 0C 0C 68 73 01 00 68 01 87 01 00 00 00 AA 55 64 16
10 00 01 00 01 16肯定确认
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
功能：测试应答帧
输入参数：Link_control，域控制码，type_identification-帧类型，transmit_reason-传输原因，time-检测时间
输入结果：应答正确为1，应答错误或没有应答为0
*/

uint8_t WDZ_101receive_testactive(uint8_t Link_control,uint8_t type_identification,uint8_t transmit_reason,uint16_t time)
{   uint8_t temp=0;
    uint8_t *rsbuf=USART4_my_frame;

    uint8_t status68=0;  //2代表非固定长度帧，1代表固定长度6的帧，0代表问题或处理完成
    uint16_t my_dtu_address=0;
    uint16_t my_start_time=0;


    my_start_time=my_tim6_count;
    while(my_tim6_count-my_start_time<=3)
    {   my_usart_101frame(4);
        if(USART4_FRAME_status>0)
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
                if(my_dtu_address==0x0001) //DTU_ADDRESS
                    status68=1;
                else status68=0;
            }
            //校验字节检查
            if(status68==1)
            {
                temp=wdz_101char_check(rsbuf);  //测试时可以考虑先不使用
                temp=1;  //屏蔽校验字@@@@@
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
                    USART4_FRAME_status=0;
                    break;
                }
                else
                {   temp=0;
                    //测试，把接收到的帧转发串口3
                    //USART_printf(&huart3,"4444");
                    //USART_printf(&huart3,USART4_my_frame);
                    //USART_printf(&huart3,"444444");
                    //帧的处理过程结束
                    USART4_FRAME_status=0;
                }

                //帧的处理过程结束
                USART4_FRAME_status=0;
            }



//************0x68非固定长度帧处理
            //从站地址检查
            if(status68==2)
            {
                my_dtu_address=rsbuf[6];
                my_dtu_address=(my_dtu_address<<8)+rsbuf[5];
                if(my_dtu_address==0x0001) //DTU_ADDRESS)
                    status68=2;
                else status68=0;
            }
            //校验字节检查

            if(status68==2)
            {
                temp=wdz_101char_check(rsbuf);  //测试时可以考虑先不使用
                temp=1;  //屏蔽校验字@@@@@
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
                    USART4_FRAME_status=0;
                    break;
                }
                else
                {   temp=0;
                    //测试，把接收到的帧转发串口3
                    //USART_printf(&huart3,"666666");
                    //USART_printf(&huart3,USART4_my_frame);
                    //USART_printf(&huart3,"6666");
                    //帧的处理过程结束
                    USART4_FRAME_status=0;
                }
            }
            //********************非固定长度帧处理结束

            //帧的处理过程结束
            USART4_FRAME_status=0;

        }


    }

    return temp;

}

/*
68 0B 0B 68 73 01 00 69 01 06 01 00 00 00 01 E6 16 复位进程命令(激活)
			肯定确认10 80 01 00 81 16
			复位进程命令(激活确认) 	68 0B 0B 68 53 01 00 69 01 07 01 00 00 00 01 C7 16
														68 0B 0B 68 73 01 00 69 01 2E 01 00 00 00 02 0F 16
														68 0B 0B 68 73 01 00 69 01 07 01 00 00 00 02 E8 16
10 00 01 00 01 16肯定确认


功能：复位进程命令
*/

int MY_433_Link_count=0;

uint8_t WDZ_101transmit_Resetdata(void)
{
    uint8_t my_status=0;
    uint8_t my_txbuf[18]=TX101_restdata;
    //---------断电判断-----
    MY_433_Link_count++;
    if(MY_433_Link_count%5==0 && MY_433_Link_count!=0)  //硬重启，让POWER断电重启433模块
    {
        WDZ_MCUtransmit_433restdata();
    }
    else if(MY_433_Link_count>60)  //硬重启，系统断电重启,5个小时后
    {
        WDZ_MCUtransmit_restdata();
    }
    //发送3个ok帧，防止拥塞
    wdz_sendOKfrma(3);

    my_status=WDZ_101transmint_commd(2,my_txbuf,1);
    if(my_status==1)
        my_status=WDZ_101receive_testactive(0x03,0x69,0x07,300);
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 rest error1\r\n");
        //LED3_TOGGLE;
    }


    if(my_status==1) //发送肯定确认命令
    {   my_UART4_printf(&huart4,TX101_OKdata);

        //Delay_us(100);
        USART_printf(&huart3,"433 rest ok*\r\n");

        // LED2_TOGGLE;
    }
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 rest error end-\r\n");
        //LED3_TOGGLE;
    }

    //发送3个ok帧，防止拥塞
    wdz_sendOKfrma(2);
    return my_status;
}



/*功能：(1)---链路初始化建立
10 C9 01 00 CA 16(请求链路FCB不能变，为0)
				（响应链路状态）10 0B 01 00 0C 16  （指示器DIR为1）
10 C0 01 00 C1 16 （复位远方链路FCB不能变）（）
				（肯定确认）10 80 01 00 81 16
				（请求链路）10 C9 01 00 CA 16
10 8B 01 00 8C 16（响应链路状态FCB不能变）
			  （复位远方链路）10 40 01 00 41 16
10 80 01 00 81 16（肯定确认FCB不能变）
			（初始化结束） 68 0B 0B 68 73 01 00 46 01 04 01 00 00 00 00 C0 16
10 80 01 00 81 16（肯定确认FCB不能变）
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




    //主发--请求链路
    //从发--响应链路状态


    WDZ_101FCB=0X00;
    my_status=WDZ_101transmint_commd_wait_commd(1,my_txbuf,1,my_rxbuf);




    //主发--复位远方链路
    //从发--肯定确认


    if(my_status==1)
    {
        my_commd=TX101_Linkrest_data;  //"\x10\xC0\x01\x00\xC1\x16";
        wdz_string_to_array(my_commd,my_txbuf);

        my_commd="\x10\x00\x01\x00\x01\x16";
        wdz_string_to_array(my_commd,my_rxbuf);
        WDZ_101FCB=0X00;
        my_status=WDZ_101transmint_commd_wait_commd(1,my_txbuf,1,my_rxbuf);
    }
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Link1!\r\n ");
        //LED3_TOGGLE;
    }
    //从发--请求链路
    if(my_status==1)
    {
        linkcontrol=0x49;
        WDZ_101FCB=0X00;
        my_status=WDZ_101receive_testactive(linkcontrol,type_identification,transmit_reason,600);
    }
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Link2!\r\n ");
        //LED3_TOGGLE;
    }
    //主发--响应链路状态
    //从发--复位远方链路
    if(my_status==1)
    {

        my_commd=TX101_Linkconfirm_data; //"\x10\x8B\x01\x00\x8C\x16";
        wdz_string_to_array(my_commd,my_txbuf);

        my_commd="\x10\x40\x01\x00\x41\x16";
        wdz_string_to_array(my_commd,my_rxbuf);
        WDZ_101FCB=0X00;
        my_status=WDZ_101transmint_commd_wait_commd(1,my_txbuf,1,my_rxbuf);
    }
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Link3!\r\n ");
        //LED3_TOGGLE;
    }
    //主发--肯定确认
    //从发--初始化结束
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
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Link4!\r\n ");
        //LED3_TOGGLE;
    }

    //主DTU--发肯定确认
    if(my_status==1) //发送肯定确认命令
    {
        WDZ_101Transmit_OKdata();
        USART_printf(&huart3,"433 Link*\r\n ");

        //
        Delay_us(300);           //测试使用，清除无效指令
        WDZ_101Transmit_OKdata(); //测试使用，清除无效指令

        //LED2_TOGGLE;
    }
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Link-\r\n ");
        //LED3_TOGGLE;
    }


    //发送2个ok帧，防止拥塞
    wdz_sendOKfrma(2);
    return my_status;

}

/*功能：(2)----总召命令
68 0B 0B 68 F3 01 00 64 01 06 01 00 00 00 14 74 16（总召唤命令 激活）
		肯定确认10 00 01 00 01 16
		确认激活 68 0B 0B 68 53 01 00 64 01 07 01 00 00 00 14 D5 16
10 80 01 00 81 16肯定确认
		单遥信，带品质，不带时标 68 1C 1C 68 73 01 00 01 92 14 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 1D 16(共18个数据，从地址0001开始，到18号)
10 80 01 00 81 16肯定确认
		遥测，带品质，不带时标  68 25 25 68 53 01 00 09 89 14 01 00 01 40 10 00 00 11 00 00 32 00 00 15 00 00 10 00 00 08 00 00 0E 00 00 00 00 00 12 00 00 DC 16 遥测，带品质，不带时标
10 80 01 00 81 16肯定确认
		激活终止 68 0B 0B 68 53 01 00 64 01 0A 01 00 00 00 14 D8 16
10 80 01 00 81 16 肯定确认
参数：Linktype标识总召数据类型，1为初始化，不保存数据，2为发送周期数据总召命令，需要保存数据
*/


uint8_t WDZ_101transmit_Calldata(uint8_t linktype)   //参数：Linktype标识总召数据类型，1为初始化，不保存数据，2为发送周期数据总召命令，需要保存数据
{
    uint8_t my_status=0;
    uint8_t k=0;
    uint8_t length=0;
    uint8_t my_txbuf[18]=TX101_calldata;

    if(linktype==1) WDZ_101Transmit_OKdata();//主发OK帧 ，清除前面无用帧

    //主发--命令
    //从发--肯定确认

    my_status=WDZ_101transmint_commd(2,my_txbuf,1);

    //Delay_us(500);


    //从发--确认激活
    if(my_status==1)
    {
        my_status=WDZ_101receive_testactive(0x03,0x64,0x07,300);
    }
    else if(my_status==0)  //失败处理
    {

        //Delay_us(100);
        USART_printf(&huart3,"433 call1!\r\n ");
        //LED3_TOGGLE;
    }
    //主发-OK
    //从发--遥信
    if(my_status==1)
    {
        WDZ_101Transmit_OKdata();//主发OK帧
        my_status=WDZ_101receive_testactive(0x03,0x01,0x14,1000);
//************重要****************把数据存储到数组缓冲区中

        if(linktype==2)
        {
            // for(k=0;k<256;k++)
            //	MY_433_Call_Single_data_buf[k]=0xff;  //测试使用

            length=USART4_my_frame[8]&0x7f;  //信息体数量
            length=0x12;  //18个信息体，强制值@@@@

            MY_433_Call_Single_data_number=length;

            //MY_433_Call_Single_data_buf[0]=length;

            for(k=0; k<length; k++)
            {
                MY_433_Call_Single_data_buf[k]=USART4_my_frame[k+14];  //存入遥信数据
            }
            MY_433_Call_Status=1;// 收到遥信数据
        }


    }
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 call2!\r\n ");
        //LED3_TOGGLE;
        MY_433_Call_Status=0;
    }
    //主发-OK帧
    //从发--遥测
    if(my_status==1)
    {
        WDZ_101Transmit_OKdata();//主发OK帧
        my_status=WDZ_101receive_testactive(0x03,0x09,0x14,1000);

//*************重要******************把数据存储到数据缓冲区中

        if(linktype==2)
        {
            //for(k=0;k<256;k++)
            //MY_433_Call_Analog_data_buf[k]=0xff;  //测试使用


            length=USART4_my_frame[8]&0x7f;  //信息体数量
            length=0x24;  //36个信息体，强制值@@@@


            MY_433_Call_Analog_data_number=length;
            //MY_433_Call_Analog_data_buf[0]=length;

            for(k=0; k<length*3; k++)
            {
                MY_433_Call_Analog_data_buf[k]=USART4_my_frame[k+14];  //存入遥测数据，带品质
            }

            my_RTCtime_to_array(MY_433_Call_Time_data_buf); //存入时标

            MY_433_Call_Status=1;
        }



    }
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 call3!\r\n ");
        //LED3_TOGGLE;
        MY_433_Call_Status=0;
    }
    //主发-OK帧
    //从发--激活终止
    if(my_status==1)
    {   WDZ_101Transmit_OKdata();//主发OK帧
        my_status=WDZ_101receive_testactive(0x03,0x64,0x0A,1000);
    }
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 call4!\r\n ");
        //LED3_TOGGLE;
    }


    //主DTU--发肯定确认
    if(my_status==1) //发送肯定确认命令
    {
        WDZ_101Transmit_OKdata();//主发OK帧
        //my_UART4_printf(UART4,TX101_OKdata);
        // Delay_us(100);
        USART_printf(&huart3,"433 call*\r\n");
        if(length==1)
        {
            Delay_us(300);
            WDZ_101Transmit_OKdata();//主发OK帧,消除无用帧
        }

        //LED2_TOGGLE;
    }
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        link_status_433=0;
        USART_printf(&huart3,"433 call-\r\n ");
        //LED3_TOGGLE;
    }
    //发送2个ok帧，防止拥塞
    wdz_sendOKfrma(2);
    return my_status;
}

/*功能：(3)---发送 延时获得命令,时间校正命令
68 0C 0C 68 53 01 00 6A 01 06 01 00 00 00 3D C3 C6 16延时获得命令(激活FCB可变)
68 0C 0C 68 D3 01 00 6A 01 06 01 00 00 00 3D C3 46 16
			肯定确认10 80 01 00 81 16
						  10 00 01 00 01 16
			延时获得命令(激活确认) 68 0C 0C 68 73 01 00 6A 01 07 01 00 00 00 5B C3 05 16
													  68 0C 0C 68 73 01 00 6A 01 07 01 00 00 00 43 C3 ED 16
10 00 01 00 01 16肯定确认
10 80 01 00 81 16
68 0C 0C 68 73 01 00 6A 01 03 01 00 00 00 C4 01 A8 16延时获得命令(突发 FCB可变)
68 0C 0C 68 F3 01 00 6A 01 03 01 00 00 00 C4 01 28 16
			肯定确认 10 80 01 00 81 16
							 10 00 01 00 01 16
68 12 12 68 53 01 00 67 01 06 00 01 00 00 008B D4 0B 0F 05 07 0E 56 16时钟同步命令(激活 FCB可变) 时标为8个字节，
68 11 11 68 D3 01 00 67 01 06 01 00 00 00   8B D4 0B 0F A5 07 0D 75 16
			肯定确认 10 80 01 00 81 16
							10 00 01 00 01 16
			时钟同步命令(激活确认) 68 12 12 68 73 01 00 67 01 07 00 01 00 00 00 B2 9D 0E 0F 05 07 0E 6A 16
													 68 11 11 68 53 01 00 67 01 07 01 00 00 00 AD D3 0B 0F 05 07 0D 77 16
10 00 01 00 01 16肯定确认FCB不可变
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

    //********第一步*********
    //主发--延时获得命令
    //从发--肯定确认
//WDZ_101FCB=0X00;
    my_status=WDZ_101transmint_commd_wait_commd(2,my_txbuf,1,my_rxbuf);

//从发延时获得命令(激活确认)
    //if(my_status==1)
    //{
    //	my_status=WDZ_101receive_testactive(0x73,0x6A,0x07,600);
    //}


    //********第二步*********
    //主发--肯定确认OK帧
    //主发--延时获得命令(突发 FCB可变)
    //从发--肯定确认
    if(my_status==1)
    {
        WDZ_101Transmit_OKdata(); //主发0K帧

        my_commd=TX101_delaytime_burst_data;//主发送帧
        wdz_string_to_array(my_commd,my_txbuf);

        my_commd="\x10\x00\x01\x00\x01\x16";  //从发送的OK帧
        wdz_string_to_array(my_commd,my_rxbuf);//接收帧

        //WDZ_101FCB=0X00;
        my_status=WDZ_101transmint_commd_wait_commd(2,my_txbuf,1,my_rxbuf);
    }
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Time1!\r\n ");
        //LED3_TOGGLE;
    }

    //*********第三步**********
    //主发---时钟同步命令
    //从发--肯定确认
    if(my_status==1)
    {
        my_commd=TX101_time_synchronization_data;//发送帧
        wdz_string_to_array(my_commd,my_txbuf);

        my_commd="\x68\x11\x11\x68\x53\x01\x00\x67\x01\x07\x01\x00\x00\x00\xAD\xD3\x0B\x0F\x05\x07\x0D\x77\x16";  //送发的OK帧
        wdz_string_to_array(my_commd,my_rxbuf);//接收帧

        //对校时时间进行修订，发送给433

        my_RTCtime_to_array(rtcbuffer);
        my_txbuf[14]=rtcbuffer[0];
        my_txbuf[15]=rtcbuffer[1];
        my_txbuf[16]=rtcbuffer[2];
        my_txbuf[17]=rtcbuffer[3];
        my_txbuf[18]=rtcbuffer[4];
        my_txbuf[19]=rtcbuffer[5];
        my_txbuf[20]=rtcbuffer[6];
        //WDZ_101FCB=0X00;
        if(my_txbuf[17]==0x10) my_txbuf[17]=my_txbuf[17]+1; //处理错误数据

        my_status=WDZ_101transmint_commd_wait_commd(2,my_txbuf,1,my_rxbuf);
    }
    else if(my_status==0)  //失败处理
    {
        //Delay_us(100);
        USART_printf(&huart3,"433 Time2!\r\n ");
        //LED3_TOGGLE;
    }

    //从发--时钟同步命令(激活确认) 68 11 11 68 53 01 00 67 01 07 01 00 00 00 AD D3 0B 0F 05 07 0D 77 16
    /*
    if(my_status==1)
    {
    	linkcontrol=0x73;
    	type_identification=0x67;
    	transmit_reason=0x07;
    	//WDZ_101FCB=0X00;
    	my_status=WDZ_101receive_testactive(linkcontrol,type_identification,transmit_reason,1000);


    }
    else if(my_status==0)  //失败处理
    {
    	//Delay_us(100);
    	USART_printf(&huart3,"Time3!\r\n ");
    	//LED3_TOGGLE;
    }
    */

    //主DTU--发肯定确认
    if(my_status==1) //发送肯定确认命令
    {
        WDZ_101Transmit_OKdata();

        USART_printf(&huart3,"433 Time*\r\n ");
        //Delay_us(400);
        //WDZ_101Transmit_OKdata(); //每条指令重复发送4次，每次间隔1秒，因此最后发送一个OK帧，结束建立链路过程，否则指令堵塞了。
    }
    else if(my_status==0)  //失败处理
    {

        USART_printf(&huart3,"433 Time-\r\n ");
        link_status_433=0; //链路堵塞了,从新启动

    }

    //发送2个ok帧，防止拥塞
    wdz_sendOKfrma(2);
    return my_status;

}


//-----------调试使用程序------------
//参数：第一个 输出目标字符串，第2个16进制字符串，第3个16进制字节数据
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
连续发送N个OK帧，防止433模块拥塞
*/

void wdz_sendOKfrma(int n)
{
    int i=0;
    for(i=0; i<n; i++)
    {
        //发送Ok帧，防止拥塞
        Delay_us(100);
        WDZ_101Transmit_OKdata();

    }
}

//发送给主MCU重启命令
/*
功能：发送重启命令
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
功能：进行翻牌操作，在接收到GPRS发送的翻牌指令后，进行433模块翻牌操作
*/
uint8_t WDZ_101Transmit_Control_monitor(uint8_t address,uint8_t status_value)
{

    uint8_t my_status=0;
    uint8_t my_txbuf[18]=TX101_controlMonitor;
    //uint8_t my_rsbuf[18]="\x68\x0B\x0B\x68\x53\x01\x00\x2D\x01\x07\x01\x00\x01\x60\x80\x6B\x16";
    // 68 0B 0B 68 D3 01 00 2D 01 06 01 00 01 60 80 EA 16
    my_txbuf[12]=address; //01代表1支路A，02，03代表B、C  04代表2支路A,07代表3支路A
    my_txbuf[14]=status_value;  //0X81代表翻牌，0X80代表复位
    //主发--命令
    //从发--肯定确认


    //Delay_ms(3);  //翻牌指令的延时，为什么？？？
    //my_status=WDZ_101transmint_commd_wait_commd(2,my_txbuf,2,my_rsbuf);//type为类型，1为固定长度，2为非固定长度，txbuf为发送指令数组
    //Delay_ms(15);  //翻牌指令的延时，为什么？？？
    //直接发送433翻牌指针
    wdz_101check_generate(my_txbuf);
    my_UART4_printf(&huart4,my_txbuf);
    my_status=1;


    //主发-OK
    if(my_status==1)
    {
        //WDZ_101Transmit_OKdata();//主发OK帧
        //USART_printf(&huart3,"433 ControlMonitor OK--%d!\r\n ",address);
    }
    else
    {
        USART_printf(&huart3,"433 ControlMonitor fail-2-%d!\r\n ",address);
    }

    return my_status;

}

/*
测试使用
*/
void test_controlmonitor(int status)
{
    if(status==1)
    {
        //  WDZ_101Transmit_Control_monitor(0X01,0X80); //测试使用
        //	WDZ_101Transmit_Control_monitor(02,0X81); //测试使用
        //	WDZ_101Transmit_Control_monitor(0X03,0X80); //测试使用
        //	WDZ_101Transmit_Control_monitor(0X05,0X80); //2aê?ê1ó?
        WDZ_101Transmit_Control_monitor(0X06,0X80); //2aê?ê1ó?
        //	WDZ_101Transmit_Control_monitor(0X07,0X80); //2aê?ê1ó?

    }
}


void my_reset2_mcu()  //重启MCU通过软命令
{
    __disable_fault_irq();
    NVIC_SystemReset();
}

//功能：测试使用，产生报警数据

int my_alarm_count=0;
uint8_t  my_temp_address=0;
void my_test_alarm_data()
{
    uint8_t *rsbuf=USART4_my_frame;
    uint8_t my_rtc_temp_time[7]= {0};

    my_alarm_count++;

    if(my_alarm_count%(2*57)==0)  //0.5计数加一次
    {
        my_temp_address++;
        USART4_FRAME_status=1;  //人工产生报警数据
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
        rsbuf[12]=my_temp_address; //地址低位
        rsbuf[13]=0x00;
        rsbuf[14]=0x01; //值
        rsbuf[15]=0xA1;  //校验字
        rsbuf[16]=0x16;

        wdz_101check_generate(rsbuf);
    }

    if(my_alarm_count%(2*58)==0) //带时标  //0.5计数加1
    {
        USART4_FRAME_status=1;  //人工产生报警数据
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
        rsbuf[12]=my_temp_address; //地址低位
        rsbuf[13]=0x00;
        rsbuf[14]=0x01; //值

        my_RTCtime_to_array(my_rtc_temp_time);

        rsbuf[15]=my_rtc_temp_time[0];
        rsbuf[16]=my_rtc_temp_time[1];
        rsbuf[17]=my_rtc_temp_time[2];
        rsbuf[18]=my_rtc_temp_time[3];
        rsbuf[19]=my_rtc_temp_time[4];
        rsbuf[20]=my_rtc_temp_time[5];
        rsbuf[21]=my_rtc_temp_time[6];
        rsbuf[22]=0xA1;  //校验字
        rsbuf[23]=0x16;

        wdz_101check_generate(rsbuf);
    }


}



/*

功能：通用型发送命令，等待肯定确认函数，主动发送指令，收到肯定确认指令返回1，没收到确认返回0
入口参数：type为类型，1为固定长度，2为非固定长度，txbuf为发送指令数组
					address_type为1，则应用地址，先发低字节，后发高字节，属于默认规则
									 如果为2，则先发高字节，后发低字节，有些指令只认这个，没办法

*/



uint8_t WDZ_101transmint_commd2(uint8_t type,uint8_t *txbuf,uint8_t address_type,uint8_t n_count)  //type为类型，1为固定长度，2为非固定长度，txbuf为发送指令数组
{
    uint8_t FCB=0x20;
    uint16_t my_dtu_address=0x0001; //  DTU_ADDRESS;
    uint8_t my_status=0;
    uint8_t n=n_count;
    uint8_t *my_txbuf=txbuf;
    uint16_t my_start_time=0;

    if(type==2)
    {
        //控制域码处理
        if(WDZ_101FCB==0x00)
            my_txbuf[4]=my_txbuf[4]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[4]=my_txbuf[4]|(FCB);
        //目的地址处理
        my_txbuf[5]=my_dtu_address&0xFF;
        my_txbuf[6]=my_dtu_address>>8;


        if(address_type==1)
        {
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }
        else if(address_type==2)
        {
            //my_txbuf[10]=my_txbuf[6]; //发送指令，不等待回复
            //my_txbuf[11]=my_txbuf[5];
            my_txbuf[10]=my_txbuf[5];
            my_txbuf[11]=my_txbuf[6];
        }

    }
    else if(type==1)
    {
        //控制域码处理
        if(WDZ_101FCB==0x00)
            my_txbuf[1]=my_txbuf[1]&(~FCB);
        else if(WDZ_101FCB==0x20)
            my_txbuf[1]=my_txbuf[1]|(FCB);
        //目的地址处理

        my_txbuf[5]=my_dtu_address&0xFF;
        my_txbuf[6]=my_dtu_address>>8;



    }


    //校验位处理
    wdz_101check_generate(my_txbuf);
    //发送指令

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
功能：子函数，调整指示器周期时间
*/
uint8_t WDZ_101Transmit_Cycle_analog_time(uint8_t address,uint8_t status_value)
{

    uint8_t my_status=0;
    uint8_t my_txbuf[18]=TX101_Cycleanalogtime;
    //uint8_t my_rsbuf[18]="\x68\x0B\x0B\x68\x53\x01\x00\x2D\x01\x07\x01\x00\x01\x60\x80\x6B\x16";
    //68 0B 0B 68 F3 01 00 3D 01 06 01 00 01 70 01 AB 16
    my_txbuf[12]=address; //01代表1支路A，02，03代表B、C  04代表2支路A,07代表3支路A
    my_txbuf[14]=status_value;  //0X81代表翻牌，0X80代表复位
    //主发--命令
    //从发--肯定确认


    //Delay_ms(3);  //翻牌指令的延时，为什么？？？
    //my_status=WDZ_101transmint_commd_wait_commd(2,my_txbuf,2,my_rsbuf);//type为类型，1为固定长度，2为非固定长度，txbuf为发送指令数组
    //Delay_ms(15);  //翻牌指令的延时，为什么？？？
    //直接发送433翻牌指针
    wdz_101check_generate(my_txbuf);
    my_UART4_printf(&huart4,my_txbuf);
    my_status=1;


    //主发-OK
    if(my_status==1)
    {
        //WDZ_101Transmit_OKdata();//主发OK帧
        //USART_printf(&huart3,"433 ControlMonitor OK--%d!\r\n ",address);
    }
    else
    {
        USART_printf(&huart3,"433 CycleAnalogTime fail-2-%d!\r\n ",address);
    }

    return my_status;

}

//发送给电压控制MCU,执行433重启命令
/*
功能：发送重启命令
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
