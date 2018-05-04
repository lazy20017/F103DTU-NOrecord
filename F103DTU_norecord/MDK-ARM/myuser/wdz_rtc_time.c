#include "wdz_rtc_time.h"
#include "rtc.h"


/*
����������ʽд��ʱ������ŵ�EEPROM�н��д洢
u8 rtcbuffer[]={00,00,30,8,20,9,14};  //2014-9-20 8��30��00
*/

void my_RTC_TO_EEPROM(uint8_t *_pWriteBuf,uint8_t _address)
{

    my_RTCtime_to_array(_pWriteBuf);
    SPI_EE_BufferWrite2(_pWriteBuf,_address,7);
}

/*
��ȡeeprom�е�ʱ������޸Ľ�����ǰʱ�䣬����RTC���м�ʱ
*/
void my_EEPROM_TO_RTC(uint8_t *_pWriteBuf,uint8_t _address)
{
    //��EEPROM�ж�ȡ7���ֽڵ�������
    SPI_EE_BufferRead2(_pWriteBuf,_address,7); //���ֽڷŵ����飬���롢���룺�֣�Сʱ����-��-��

    //������ֵд�뵽RTC��

    my_array_to_RTCtime(_pWriteBuf);



}


/*
���ܣ�����RTCʱ������ֵ����ϵͳУʱ������ֵ���޸��ɴ��ڻ��
���룺RTC����
�������
*/
void my_array_to_RTCtime(uint8_t *_pWriteBuf)
{   //struct rtc_time mytm;
    //u32 mycnt=0;
    uint16_t my_second=0;

    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef DateToUpdate;

    my_second=_pWriteBuf[1];
    my_second=((my_second<<8)+_pWriteBuf[0])/1000;
    //Time_Regulate(tm);  //���뵱ǰ��ʱ�䣬�꣬�£��գ�Сʱ�����ӣ��룬�ŵ��ṹ����
//	mytm.tm_year=2000+_pWriteBuf[6];
//	mytm.tm_mon=_pWriteBuf[5];
//	mytm.tm_mday=_pWriteBuf[4];
//	mytm.tm_hour=_pWriteBuf[3];
//	mytm.tm_min=_pWriteBuf[2];
//	mytm.tm_sec=my_second;

    /* ???? */
    /* ????:2015?10?4? ??? */
    DateToUpdate.WeekDay = RTC_WEEKDAY_SUNDAY;
    DateToUpdate.Month = _pWriteBuf[5];
    DateToUpdate.Date = _pWriteBuf[4];
    DateToUpdate.Year = _pWriteBuf[6];
    //HAL_RTC_SetDate(&hrtc,&DateToUpdate,RTC_FORMAT_BCD);
    HAL_RTC_SetDate(&hrtc,&DateToUpdate,RTC_FORMAT_BIN);

    sTime.Hours =_pWriteBuf[3];
    sTime.Minutes = _pWriteBuf[2];
    sTime.Seconds = my_second;
    //HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F1);
}

/*
���ܣ���ȡ��ǰʱ�䲢ת�������뵽������
���������RTC�����ַ
�������
*/
void my_RTCtime_to_array(uint8_t *_pWriteBuf)
{   //uint32_t TimeVar;
    //struct rtc_time mytm;
    uint16_t my_second=0;
    RTC_DateTypeDef sdatestructureget;
    RTC_TimeTypeDef stimestructureget;

    /* ���ʱ�� */
    HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
    /* ������� */
    HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);


    //TimeVar=RTC_GetCounter();
    //to_tm(TimeVar, &mytm);/*�Ѷ�ʱ����ֵת��Ϊunixʱ��,����tm��*/

//	    _pWriteBuf[6]=mytm.tm_year-2000; s
//			_pWriteBuf[5]=mytm.tm_mon;
//			_pWriteBuf[4]=mytm.tm_mday;
//			_pWriteBuf[3]=mytm.tm_hour;
//			_pWriteBuf[2]=mytm.tm_min;
//
//			my_second=mytm.tm_sec*1000;
//			_pWriteBuf[1]=my_second/0xff;
//			_pWriteBuf[0]=my_second%0xff;

    _pWriteBuf[6]=sdatestructureget.Year-2000;
    _pWriteBuf[5]=sdatestructureget.Month;
    _pWriteBuf[4]=sdatestructureget.Date;
    _pWriteBuf[3]=stimestructureget.Hours;
    _pWriteBuf[2]=stimestructureget.Minutes;

    my_second=stimestructureget.Seconds*1000;
    _pWriteBuf[1]=my_second/0xff;
    _pWriteBuf[0]=my_second%0xff;


}
