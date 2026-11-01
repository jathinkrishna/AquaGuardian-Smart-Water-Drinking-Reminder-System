//rtc.h
#ifndef RTC_H
#define RTC_H

void RTC_Init(void);
void RTC_GetTime(unsigned char *hour,unsigned char *min,unsigned char *sec);
void RTC_GetDate(unsigned char *day,unsigned char *month,unsigned int *year);
#endif

