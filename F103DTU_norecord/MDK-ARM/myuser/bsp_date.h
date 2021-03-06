/******************** (C) COPYRIGHT 2009 www.armjishu.com ************************
* File Name          : date.h
* Author             : www.armjishu.com Team
* Version            : V1.0
* Date               : 12/1/2009
* Description        : 日期相关函数
*******************************************************************************/
#ifndef __DATE_H
#define __DATE_H

#include "stm32f1xx_hal.h"

struct rtc_time {
    uint16_t tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
};

void GregorianDay(struct rtc_time * tm);
uint32_t mktimev(struct rtc_time *tm);
void to_tm(uint32_t tim, struct rtc_time * tm);
#endif
