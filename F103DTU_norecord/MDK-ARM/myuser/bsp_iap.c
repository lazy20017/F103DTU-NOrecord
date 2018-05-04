/**
  ******************************************************************************
  * �ļ�����: bsp_iap.c
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: IAP�ײ�����ʵ��
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  *
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp_iap.h"
#include "bsp_stm_flash.h"
#include "my_cc1101.h"
#include "my_usart.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/



static uint16_t ulBuf_Flash_App[1024];



/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/


__asm void MSR_MSP ( uint32_t ulAddr )
{
    MSR MSP, r0 			                   //set Main Stack value
    BX r14
}
//��ת��Ӧ�ó����
//ulAddr_App:�û�������ʼ��ַ.
void IAP_ExecuteApp ( uint32_t ulAddr_App )
{
    pIapFun_TypeDef pJump2App;
    uint32_t my_add=( * ( __IO uint32_t * ) ulAddr_App ) ;

    if ( ( my_add& 0x2FFE0000 ) == 0x20000000 )	  //���ջ����ַ�Ƿ�Ϸ�.
    {
        printf("ջ����ȷ  %XH\r\n",my_add);
        pJump2App = ( pIapFun_TypeDef ) * ( __IO uint32_t * ) ( ulAddr_App + 4 );	//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)
        MSR_MSP( * ( __IO uint32_t * ) ulAddr_App );					                            //��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
        pJump2App ();								                                    	//��ת��APP.
    }
    else
    {
        printf("ջ������  %XH\r\n",my_add);
    }
}




/*
�������ܣ���ָ�����ȵ����ݣ�д�뵽FLASH��
��������һ��Ϊ��FLASH�׵�ַ����2��Ϊ���ݻ��������ֽ�Ϊ��λ����3��д���ֽ���
*/

void IAP_Write_App_Bin ( uint32_t ulStartAddr, uint8_t * pBin_DataBuf, uint32_t ulBufLength )
{


    uint16_t us, usCtr=0, usTemp;
    uint32_t ulAdd_Write = ulStartAddr;                                //��ǰд��ĵ�ַ
    uint8_t * pData = pBin_DataBuf;

    for ( us = 0; us < ulBufLength; us += 2 )
    {
        usTemp =  ( uint16_t ) pData[1]<<8;
        usTemp += ( uint16_t ) pData[0];
        pData += 2;                                                      //ƫ��2���ֽ�
        ulBuf_Flash_App [ usCtr ++ ] = usTemp;
        if ( usCtr == 1024 )
        {
            usCtr = 0;
            STMFLASH_Write ( ulAdd_Write, ulBuf_Flash_App, 1024 );	//д�ĵ�λΪ2���ֽڣ����Ǹ��ڵ�ַ����1���ֽ�Ϊ��λ��һ��д1������2048���ֽ�
            ulAdd_Write += 2048;                                           //ƫ��2048  16=2*8.����Ҫ����2.
        }
    }
    if ( usCtr )
        STMFLASH_Write ( ulAdd_Write, ulBuf_Flash_App, usCtr );//������һЩ�����ֽ�д��ȥ.


}









/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/

/*

//extern uint8_t USART3_FRAME_status;
//extern uint8_t USART3_my_frame[];
//extern uint8_t rsbuf3[];	 //���ڽ��ջ�����
//extern uint8_t txbuf3[];
//extern uint16_t rsbuf3pt_write;
//extern uint16_t rsbuf3pt_read;


//uint16_t my_update_get_data_count=0;
//uint8_t my_update_status=0;  //0Ϊû��������1Ϊ�����ɹ���2Ϊ��������

//void my_fun_update_get_data_to_flash(void)
//{
//	uint16_t my_status1=0,my_status2=0;
//	uint32_t my_length1=0,my_length2=0,my_length0=0;
//  //1  ���տ�ʼָ��
//	//10 F0 01 02 03 16
//	if(USART3_FRAME_status==1&&USART3_my_frame[1]==0XF0) //��ʼ����
//	{
//		my_update_get_data_count=0;
//		rsbuf3pt_write=0;
//		rsbuf3pt_read=0;
//		printf("update get data started  block=%d\r\n",my_update_get_data_count);
//    my_update_status=2; //��������
//		USART3_FRAME_status=0;
//		return;
//	}
//	//2  �������һ���ļ�ָ��
//	//10 F1 01 02 03 16
//	if(USART3_FRAME_status==1&&USART3_my_frame[1]==0XF1) //�����������һ����
//	{
//
//		rsbuf3pt_write=0;
//		rsbuf3pt_read=0;
//		printf("update get data  lasted block=%d  !!\r\n",my_update_get_data_count);


//    my_update_status=3; //�������һ���ļ���
//		USART3_FRAME_status=0;
//		return;
//	}
//
//	 //3  �м���յĹ������ݿ�
//		if(my_update_status==2 && rsbuf3pt_write==APP_FLASH_LEN) //�м��
//	{
//		HAL_Delay(500);
//		if(rsbuf3pt_write>APP_FLASH_LEN)
//		{
//				printf("get data is error=%d!!  \r\n",my_update_get_data_count);
//			  return;
//		}

//		printf("block=%d   write_add=%d\r\n",my_update_get_data_count,rsbuf3pt_write);
//		IAP_Write_App_Bin(APP_START_ADDR2+(my_update_get_data_count*APP_FLASH_LEN),rsbuf3,APP_FLASH_LEN);
//		my_update_get_data_count++;
//		printf("write data to flash block=%d!!\r\n",my_update_get_data_count-1);
//
//		rsbuf3pt_write=0;
//		rsbuf3pt_read=0;
//
//    my_update_status=2; //������
//
//		return;
//	}
//	//4  ���յ����һ����β��
//		if(my_update_status==3 && rsbuf3pt_write<=APP_FLASH_LEN && rsbuf3pt_write>0) //��������
//	{
//		uint16_t my_temp=rsbuf3pt_write;
//		HAL_Delay(1000);
//		if(my_temp!=rsbuf3pt_write)
//			return;
//
//		if(rsbuf3pt_write>APP_FLASH_LEN)
//		{
//				printf("get data is error=%d!!  \r\n",my_update_get_data_count);
//			  return;
//		}
//
//		printf("block=%d   write_add=%d\r\n",my_update_get_data_count,rsbuf3pt_write);
//		IAP_Write_App_Bin(APP_START_ADDR2+(my_update_get_data_count*APP_FLASH_LEN),rsbuf3,rsbuf3pt_write);
//		my_update_get_data_count++;
//		printf("write data to flash block=%d!!\r\n",my_update_get_data_count-1);
//		printf("===write data to flash finish!!===\r\n");
//
//		if((rsbuf3pt_write)%2==0)
//		my_length0=(my_update_get_data_count-1)*APP_FLASH_LEN+rsbuf3pt_write;  //����ܵ�����������ֽ�����
//		else
//		my_length0=(my_update_get_data_count-1)*APP_FLASH_LEN+rsbuf3pt_write+1;  //����ܵ�����������ֽ�����
//
//		rsbuf3pt_write=0;
//		rsbuf3pt_read=0;
//		my_update_get_data_count=0;
//    my_update_status=1; //��������
//
//		//5  д���룬�ͳ��ȣ��ֽ�Ϊ��λ��
//		//����
//		my_status1=STMFLASH_ReadHalfWord(APP_update_status1_add);
//		my_status2=STMFLASH_ReadHalfWord(APP_update_status2_add);
//		printf("\r\n my_status1=%XH,  my_status2=%XH\r\n",my_status1,my_status2);
//		my_status1=APP_update_password; //д������
//		STMFLASH_Write(APP_update_status1_add,&my_status1,1);
//		STMFLASH_Write(APP_update_status2_add,&my_status1,1);
//		my_status1=STMFLASH_ReadHalfWord(APP_update_status1_add);
//		my_status2=STMFLASH_ReadHalfWord(APP_update_status2_add);
//		printf("my_status1=%XH,   my_status2=%XH\r\n",my_status1,my_status2);
//		//����
//		my_length1=STMFLASH_ReadHalfWord(APP_update_length1_add+2);
//		my_length1=(my_length1<<16)+STMFLASH_ReadHalfWord(APP_update_length1_add);
//		my_length2=STMFLASH_ReadHalfWord(APP_update_length2_add+2);
//		my_length2=(my_length2<<16)+STMFLASH_ReadHalfWord(APP_update_length2_add);
//		printf("my_length1=%XH,my_length2=%XH\r\n",my_length1,my_length2);
//
//		my_status1=my_length0;  //д�ĳ���
//		my_status2=my_length0>>16;
//		STMFLASH_Write(APP_update_length1_add,&my_status1,1);
//		STMFLASH_Write(APP_update_length1_add+2,&my_status2,1);
//		STMFLASH_Write(APP_update_length2_add,&my_status1,1);
//		STMFLASH_Write(APP_update_length2_add+2,&my_status2,1);
//
//		my_length1=STMFLASH_ReadHalfWord(APP_update_length1_add+2);
//		my_length1=(my_length1<<16)+STMFLASH_ReadHalfWord(APP_update_length1_add);
//		my_length2=STMFLASH_ReadHalfWord(APP_update_length2_add+2);
//		my_length2=(my_length2<<16)+STMFLASH_ReadHalfWord(APP_update_length2_add);
//		printf("my_length1=%XH,my_length2=%XH\r\n",my_length1,my_length2);
//
//
//		return;
//	}
//
//	if((my_update_status==2||my_update_status==3)&&rsbuf3pt_write>APP_FLASH_LEN)
//	{
//		printf("blok error=%d      write_add=%d\r\n",my_update_get_data_count,rsbuf3pt_write);
//		rsbuf3pt_write=0;
//		rsbuf3pt_read=0;
//
//
//	}
//
//}



//���ܣ����յ����������������ݵķ��ͣ�����CC1101���з��͡�
//���͵Ĺ��̣���2K�ֽ�Ϊһ��֡������CC1101���г�֡�ķ��ͣ�CC1101���յ�1֡�󣬰Ѵ�֡����д�뵽FLASH�У�ÿ�յ�1֡��ȷ��1֡��
//���öϵ������ķ�����ÿ1֡����1��֡�š�


//uint32_t my_update_send_count=0;
//uint32_t my_update_send_length=0;
//	uint16_t my_send_buf[1024];
//	uint8_t my_send_buf2[2048+12];
//extern uint8_t my_cc1101_dest_address;

//extern uint8_t my_CC1101_Frame_status;
//extern uint8_t my_CC1101_COM_Fram_buf[];
//#define my_time_max 10         //��Ϊ��λ

//void my_fun_update_send_data(uint8_t my_port)
//{
//
//	uint16_t my_status1=0,my_status2=0;
//	uint32_t my_length1=0,my_length2=0;
//	uint16_t my_block=0;
//	uint16_t my_remain_data=0;
//	uint16_t ii=0,jj=0;
//	uint8_t my_count=0;
//	uint8_t my_CRC_check=0;
//	//===
//	uint8_t *my_fram_status=NULL;
//	uint8_t *pt_my_frame_buf=NULL;

//
//	if(my_port==3)
//	{
//		my_fram_status=&USART3_FRAME_status;
//		pt_my_frame_buf=USART3_my_frame;
//		my_CRC_check=my_UART3_CRC_check;
//	}
//	else if(my_port==4)
//	{
//		my_fram_status=&my_CC1101_Frame_status;
//		pt_my_frame_buf=my_CC1101_COM_Fram_buf;
//		my_CRC_check=CC1101_CRC_check;
//	}
//
//
//	//========0X00=====
//  //1  ���տ�ʼָ��
//	//10 F2 01 02 03 16
//			if(USART3_FRAME_status==1&&USART3_my_frame[1]==0XF2) //����CC1101������������
//			{
//					USART3_FRAME_status=0;
//					 //	if(my_update_status==1)
//				my_update_get_data_count=0;
//				my_update_status=0X10;   //���뷢������״̬
//       printf("���յ�����ָ��,����0X10״̬\r\n");
//			}
//
//	//======0X10==========
//		if(my_update_status==0X10)  //��鷢�ͣ����״̬
//		{
//			my_status1=STMFLASH_ReadHalfWord(APP_update_status1_add);
//			my_status2=STMFLASH_ReadHalfWord(APP_update_status2_add);
//			printf("\r\nmy_status1=%XH,  my_status2=%XH\r\n",my_status1,my_status2);
//
//			my_length1=STMFLASH_ReadHalfWord(APP_update_length1_add+2);
//			my_length1=(my_length1<<16)+STMFLASH_ReadHalfWord(APP_update_length1_add);
//			my_length2=STMFLASH_ReadHalfWord(APP_update_length2_add+2);
//			my_length2=(my_length2<<16)+STMFLASH_ReadHalfWord(APP_update_length2_add);
//			printf("my_length1=%XH,my_length2=%XH\r\n",my_length1,my_length2);
//
//			if(my_status1==my_status2&& my_status1==APP_update_password && my_length1==my_length2 &&my_length1!=0XFFFFFFFF )
//			{
//				my_update_status=0x20;  //���ɹ������뷢��״̬
//				my_update_send_length=my_length1;   //���͵��ܵ����ݳ��ȣ��ֽ�Ϊ��λ
//				printf("����0X20 ״̬,�������ݰ�\r\n");
//
//			}
//			else
//			{
//				my_update_status=0x00;
//				printf("FALSH��APP���������⣬��Ҫ�����ϴ�\r\n");
//			}
//		}
//		//========0X20==========
//			if(my_update_status==0X20)
//			{
//				//===
////				for(ii=1;ii<=58;ii++)
////				{
////					my_send_buf2[ii-1]=ii;
////				}
////					CC1101SendPacket_add(my_send_buf2,58,ADDRESS_CHECK,my_cc1101_dest_address);
//				//===
//				//���Ϳ�ʼ����
//				my_send_buf2[0]=0x10;
//				my_send_buf2[1]=0xF0;  //��ʼָ��
//				my_send_buf2[2]=0xFD; //��������ַ
//				my_send_buf2[3]=0x00;
//				my_send_buf2[4]=0x01;//����
//				my_send_buf2[5]=0x08;
//				my_send_buf2[6]=my_fun_101check_generate(my_send_buf2,1);
//				my_send_buf2[7]=0x16;
//				CC1101SendPacket_add(my_send_buf2,8,ADDRESS_CHECK,my_cc1101_dest_address);
//				printf("CC1101���Ϳ�ʼָ��=1\r\n");
//
//
//				my_count=0;
//				while(1)
//				{
//										HAL_Delay(1000);
//										my_101frame_analyse(my_port,1,my_CRC_check);  //���ָ��
//
//										if(*my_fram_status==1 && pt_my_frame_buf[1]==0X20) //���յ�ȷ��֡
//										{
//
//												*my_fram_status=0; //���յ�ok֡
//												my_update_status=0X30;
//												break;
//
//										}
//										else if(*my_fram_status==1 && pt_my_frame_buf[1]!=0X20)
//										{
//											*my_fram_status=0;
//										}
//
//										my_count++;
//										if(my_count>=15)
//										{
//											my_update_status=0x00;
//											my_update_send_count=0;
//											*my_fram_status=0;
//											printf("��ʱ����������=0X20\r\n");
//											break;
//										}
//										if(my_count%5==0)
//										{
//											printf("CC1101���Ϳ�ʼָ��--�ظ�=%d\r\n",my_count/5+1);
//											CC1101SendPacket_add(my_send_buf2,8,ADDRESS_CHECK,my_cc1101_dest_address); //�ظ�����
//										}
//
//
//				}
//			}
//				//==========0X30=====
//				if(my_update_status==0x30)
//				{
//
//									//���͹������ݰ�
//									my_block=my_update_send_length/STM_SECTOR_SIZE;  //2KΪ��λ���з���
//									my_remain_data=my_update_send_length%STM_SECTOR_SIZE;
//									ii=my_update_send_count/STM_SECTOR_SIZE;
//
//									my_send_buf2[0]=0X68;
//									my_send_buf2[1]=0x02;
//									my_send_buf2[2]=0X08;
//									my_send_buf2[3]=0x02;
//									my_send_buf2[4]=0X08;
//									my_send_buf2[5]=0x68;
//
//
//									for(;ii<my_block;ii++)
//									{
//										STMFLASH_Read(APP_START_ADDR2+ii*STM_SECTOR_SIZE,my_send_buf,STM_SECTOR_SIZE/2);
//										my_send_buf2[6]=0XF2;
//										my_send_buf2[7]=ii;
//										for(jj=0;jj<STM_SECTOR_SIZE/2;jj++)
//										{
//											my_send_buf2[jj*2+8]=my_send_buf[jj];
//											my_send_buf2[jj*2+1+8]=(my_send_buf[jj]>>8);
//										}
//										my_send_buf2[jj*2+8]=my_fun_101check_generate(my_send_buf2,1);
//										my_send_buf2[jj*2+8+1]=0X16;
//										printf("�������ݰ�%d --��ʼ--1��\r\n",ii);
//										my_fun_CC1101_send_long_data(my_send_buf2,STM_SECTOR_SIZE+10,ADDRESS_CHECK,my_cc1101_dest_address); //���Ϳ�����
//
//										//�ظ�����2��
//										my_count=0;
//										while(1)
//										{
//															HAL_Delay(1000);
//															my_101frame_analyse(my_port,1,my_CRC_check);  //���ָ��
//															if(*my_fram_status==1 && pt_my_frame_buf[1]==0X20) //���յ�ȷ��֡
//															{
//																//if(USART3_my_frame[2]==ii)  //�Խ���֡����У��
//																{
//																	my_update_send_count=my_update_send_count+ii*STM_SECTOR_SIZE; //�ѽᷢ�͵��ֽ�������
//																	*my_fram_status=0;
//																	my_update_status=0X40;
//																	break;
//
//																}
//															}
//															else if(*my_fram_status==1 && pt_my_frame_buf[1]!=0X20)
//															{
//																*my_fram_status=0;
//															}
//
//															my_count++;
//															if(my_count>=my_time_max*3)
//															{
//															my_update_status=0x00;
//															my_update_send_count=0;
//															*my_fram_status=0;
//															printf("��ʱ����������=0X30\r\n");
//
//															break;
//
//															}
//															if(my_count%my_time_max==0)
//															{
//																 	printf("�ظ��������ݰ�%d--%d�� \r\n",ii,my_count/my_time_max+1);
//																my_fun_CC1101_send_long_data(my_send_buf2,STM_SECTOR_SIZE+10,ADDRESS_CHECK,my_cc1101_dest_address); //���Ϳ�����

//															}
//
//										}
//										//====while end ====
//										if(my_update_status==0)
//											break;
//									}
//									//====for end=====
//				}
//				//=======0X40========
//				if(my_update_status==0X40)
//				{
//											//�������һ�����ݰ�ָ��
//										my_send_buf2[0]=0x10;
//										my_send_buf2[1]=0xF4; //���һ�����ݰ�
//										my_send_buf2[2]=0xFD; //��������ַ
//										my_send_buf2[3]=0x00;
//										my_send_buf2[4]=0x01;//����
//										my_send_buf2[5]=0x08;
//										my_send_buf2[6]=my_fun_101check_generate(my_send_buf2,1);
//										my_send_buf2[7]=0x16;
//										CC1101SendPacket_add(my_send_buf2,8,ADDRESS_CHECK,my_cc1101_dest_address);
//										printf("CC1101����-----������ݰ�ָ��----1��-\r\n");
//										my_count=0;
//										while(1)
//										{
//																HAL_Delay(1000);
//																my_101frame_analyse(4,1,my_CRC_check);  //���ָ��
//
//																if(*my_fram_status==1 && pt_my_frame_buf[1]==0X20) //���յ�ȷ��֡
//																{
//
//																		*my_fram_status=0; //���յ�ok֡
//																		my_update_status=0X50;
//																		break;
//
//																}
//																else if(*my_fram_status==1 && pt_my_frame_buf[1]!=0X20)
//																{
//																	*my_fram_status=0;
//																}
//
//																my_count++;
//																if(my_count>=15)
//																{
//																my_update_status=0x00;
//																my_update_send_count=0;
//																*my_fram_status=0;
//																printf("��ʱ����������=0X40\r\n");
//																	break;
//																}
//																if(my_count%5==0)
//																{
//																	printf("CC1101����---������ݰ�ָ��--�ظ�%d��\r\n",my_count/5+1);
//																	CC1101SendPacket_add(my_send_buf2,8,ADDRESS_CHECK,my_cc1101_dest_address); //�ظ�����
//																}
//
//
//										}
//			}
//
//
//			//========0X50=============
//

//      	if(my_update_status==0X50)
//				{

//					if(my_remain_data==0)
//							{
//								my_update_status=0x00;
//								my_update_send_count=0;
//								printf ("���ͽ�����û��ʣ��� \r\n");
//								return;
//
//							}
//
//							STMFLASH_Read(APP_START_ADDR2+ii*STM_SECTOR_SIZE,my_send_buf,my_remain_data/2);			//����ʣ�������
//							my_send_buf2[0]=0X68;
//							my_send_buf2[1]=my_remain_data+2;
//							my_send_buf2[2]=((my_remain_data+2)>>8);
//							my_send_buf2[3]=my_send_buf2[1];
//							my_send_buf2[4]=my_send_buf2[2];
//							my_send_buf2[5]=0x68;
//							my_send_buf2[6]=0XF6;
//							my_send_buf2[7]=ii;
//
//							for(jj=0;jj<my_remain_data/2;jj++)
//							{
//								my_send_buf2[jj*2+8]=my_send_buf[jj];
//								my_send_buf2[jj*2+1+8]=(my_send_buf[jj]>>8);
//							}
//							my_send_buf2[jj*2+8]=my_fun_101check_generate(my_send_buf2,1);
//							my_send_buf2[jj*2+8+1]=0X16;
//							printf("�����������ݰ�--1��\r\n");
//							my_fun_CC1101_send_long_data(my_send_buf2,my_remain_data+10,ADDRESS_CHECK,my_cc1101_dest_address); //���Ϳ�����

//             	//�ظ�����2��
//					my_count=0;
//					while(1)
//					{
//						HAL_Delay(1000);
//						my_101frame_analyse(my_port,1,my_CRC_check);
//						if(*my_fram_status==1 && pt_my_frame_buf[1]==0X20) //���յ�ȷ��֡
//						{
//							//if(USART3_my_frame[2]==ii) //�Խ���֡����У��
//							{
//								my_update_send_count=my_update_send_count+my_remain_data; //�ѽᷢ�͵��ֽ�������
//
//								if(my_update_send_count==my_update_send_length)
//								{
//
//									my_update_status=0X60;
//									*my_fram_status=0;
//									printf("lasted data  is finish=[%XH]\r\n",my_update_send_count);
//									my_update_send_count=0;
//								}
//								else
//								{
//									*my_fram_status=0;
//									my_update_send_count=0;
//									my_update_status=0;
//									printf("lasted data  is error=[%XH]\r\n",my_update_send_count);
//								}
//								break;
//
//							}
//						}
//						else if(*my_fram_status==1 && pt_my_frame_buf[1]!=0X20)
//						{
//							*my_fram_status=0;
//
//						}
//					  my_count++;
//						if(my_count>=3*my_time_max)
//						{
//							my_update_status=0x00;
//							my_update_send_count=0;
//							*my_fram_status=0;
//							printf("��ʱ����������=0X50\r\n");
//							break;
//						}
//						if(my_count%my_time_max==0)
//						{
//								printf("�����������ݰ�--�ظ�����-%d��\r\n",my_count/my_time_max+1);
//								my_fun_CC1101_send_long_data(my_send_buf2,my_remain_data+10,ADDRESS_CHECK,my_cc1101_dest_address); //���Ϳ�����
//						}
//
//					}
//

//
//				}
//    //========0X60==========
//					if(my_update_status==0X60)
//				{
//											//�������һ������ָ��
//										my_send_buf2[0]=0x10;
//										my_send_buf2[1]=0xF8; //����ָ��
//										my_send_buf2[2]=0xFD; //��������ַ
//										my_send_buf2[3]=0x00;
//										my_send_buf2[4]=0x01;//����
//										my_send_buf2[5]=0x08;
//										my_send_buf2[6]=my_fun_101check_generate(my_send_buf2,1);
//										my_send_buf2[7]=0x16;
//										CC1101SendPacket_add(my_send_buf2,8,ADDRESS_CHECK,my_cc1101_dest_address);
//										printf("CC1101����-----����ָ��----1��-\r\n");
//										my_count=0;
//										while(1)
//										{
//																HAL_Delay(1000);
//																my_101frame_analyse(my_port,1,my_CRC_check);  //���ָ��
//
//																if(*my_fram_status==1 && pt_my_frame_buf [1]==0X20) //���յ�ȷ��֡
//																{
//
//																		my_update_status=0x00;
//																		my_update_send_count=0;
//																		*my_fram_status=0;
//																	  printf("�����������ݽ���--��ȷ\r\n");
//																		break;
//
//																}
//																else if(*my_fram_status==1 && pt_my_frame_buf [1]!=0X20)
//																{
//																	*my_fram_status=0;
//																}
//
//																my_count++;
//																if(my_count>=15)
//																{
//																my_update_status=0x00;
//																my_update_send_count=0;
//																*my_fram_status=0;
//																printf("��ʱ����������=0X40\r\n");
//																	break;
//																}
//																if(my_count%5==0)
//																{
//																	printf("CC1101����---����ָ��--�ظ�%d��\r\n",my_count/5+1);
//																	CC1101SendPacket_add(my_send_buf2,8,ADDRESS_CHECK,my_cc1101_dest_address); //�ظ�����
//																}
//
//
//										}
//			}
//
//		//=====
//}

*/

//======================
/*

*/
extern uint8_t my_CC1101_COM_Fram_buf[];
uint32_t my_update_length=0;
uint8_t my_fun_write_update_data_to_FLASH(void)
{
    uint8_t temp_status=0;
    uint8_t my_block=0;
    uint16_t my_length=0;

    if(my_CC1101_COM_Fram_buf[0]==0x10)
    {
        temp_status=my_CC1101_COM_Fram_buf[1];
#if Debug_uart_out_cc1101_rx_data_status==1
        my_fun_display_buf_16(my_CC1101_COM_Fram_buf,8,0);  //����ʹ�ã���ʾ���յ�������8���ֽ�
#endif
    }
    else if (my_CC1101_COM_Fram_buf[0]==0x68)
    {
        temp_status=my_CC1101_COM_Fram_buf[6];
        my_block=my_CC1101_COM_Fram_buf[7];  //��ÿ��

        my_length=my_CC1101_COM_Fram_buf[2];
        my_length=(my_length<<8)+my_CC1101_COM_Fram_buf[1]-2; //��ó���
#if Debug_uart_out_cc1101_rx_data_status==1
        my_fun_display_buf_16(my_CC1101_COM_Fram_buf,8,0);   //����ʹ�ã���ʾ���յ�������8���ֽ�
#endif
    }

    //------
    if(temp_status==0xF0)
    {
        printf("�����������ݿ�ʼ  F0\r\n");
        my_update_length=0;
        return 1;

    }
    else if(temp_status==0xF2)
    {
        if(my_length==STM_SECTOR_SIZE)
        {
            printf("���յ����ݰ�������д��FLASH  block=%d   F2\r\n",my_block);
            IAP_Write_App_Bin(APP_START_ADDR2+my_block*STM_SECTOR_SIZE,(my_CC1101_COM_Fram_buf+8),my_length);

            printf("FLASH д��ɹ�-----block=%d   length=%d    F2\r\n",my_block,my_length);
            my_update_length=my_update_length+my_length;
            return 1;
        }
        else
        {
            printf("���յ������ݿⳤ�ȴ���  block=%d  length=%d\r\n   F4",my_block,my_length);
            return 0;
        }

    }
    else if(temp_status==0xF4)
    {
        printf("�����������ݰ�   F4\r\n");
        return 1;
    }
    else if(temp_status==0xF6)
    {

        if(my_length<=STM_SECTOR_SIZE)
        {
            printf("���յ�--���---���ݰ�������д��FLASH  block=%d   F6\r\n",my_block);
            IAP_Write_App_Bin(APP_START_ADDR2+my_block*STM_SECTOR_SIZE,(my_CC1101_COM_Fram_buf+8),my_length);
            printf("FLASH --������ݰ�---д��ɹ�-----block=%d   length=%d     F6\r\n",my_block,my_length);
            my_update_length=my_update_length+my_length;
            if(my_update_length==(my_block*STM_SECTOR_SIZE+my_length))
            {
                printf("==д��FLASH����   OK   data=[%XH]\r\n",my_update_length);
                return 1;
            }
            else
            {
                printf("==д��FLASH����  ERROR!!   right_data=[%XH], error_data=[%XH\r\n",(my_block*STM_SECTOR_SIZE+my_length),my_update_length);
                return 0;
            }

        }
        else
        {
            printf("���յ����������ݳ��ȴ���  block=%d  length=%d\r\n   F6",my_block,my_length);

            return 0;
        }



    }
    else if(temp_status==0xF8)
    {
        uint16_t passsword=APP_update_password;
        uint16_t length_L=my_update_length;
        uint16_t length_H=(my_update_length>>16);

        STMFLASH_Write(APP_update_status1_add,&passsword,1);
        STMFLASH_Write(APP_update_status2_add,&passsword,1);

        STMFLASH_Write(APP_update_length1_add,&length_L,1);
        STMFLASH_Write(APP_update_length1_add+2,&length_H,1);
        STMFLASH_Write(APP_update_length2_add,&length_L,1);
        STMFLASH_Write(APP_update_length2_add+2,&length_H,1);

        printf("�������ݴ������   F8\r\n");
        return 1;
    }

    return 0;

}

