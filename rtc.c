				  
#include <LPC21xx.h>
#include "rtc.h"

#define CCR_CLKEN   (1U << 0)
#define CCR_CTCRST  (1U << 1)
#define CCR_CLKSRC  (1U << 4)

void RTC_Init(void)
{
    /* Stop RTC while configuring its clock source */
    CCR &= ~CCR_CLKEN;

    CCR |= CCR_CLKSRC;

    /* Do not reset a valid stored date/time.
       Initialize only if the stored calendar is invalid. */
    if (YEAR < 2000U || YEAR > 2099U ||
        MONTH < 1U || MONTH > 12U ||
        DOM < 1U || DOM > 31U ||
        HOUR > 23U || MIN > 59U || SEC > 59U)
    {
        CCR |= CCR_CTCRST;

        SEC  = 0;
        MIN  = 0;
        HOUR = 6;

        DOM   = 17;
        MONTH = 8;
        YEAR  = 2026;

        CCR &= ~CCR_CTCRST;
    }

    CCR |= CCR_CLKEN;
}

void RTC_GetTime(unsigned char *hour,
                 unsigned char *min,
                 unsigned char *sec)
{
    *hour = (unsigned char)HOUR;
    *min  = (unsigned char)MIN;
    *sec  = (unsigned char)SEC;
}

void RTC_GetDate(unsigned char *day,
                 unsigned char *month,
                 unsigned int *year)
{
    *day   = (unsigned char)DOM;
    *month = (unsigned char)MONTH;
    *year  = (unsigned int)YEAR;
}
