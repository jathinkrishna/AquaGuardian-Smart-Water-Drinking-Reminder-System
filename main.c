
#include <LPC21xx.h>
#include "lcd.h"
#include "rtc.h"
#include "delay.h"
#include "keypad.h"


#define BUTTON_PIN         (1 << 16) 
#define BUZZER_PIN         (1 << 17)
#define YELLOW_LED         (1 << 18)
#define RED_LED            (1 << 19)
#define GREEN_LED          (1 << 20)
#define SCREEN_BUTTON_PIN  (1 << 21)
#define MENU_BUTTON_PIN    (1 << 14)


#define REMINDER_INTERVAL_SEC  60
#define RED_ALERT_SEC          130


volatile unsigned int menu_request = 0U;


unsigned int Menu(unsigned int DailyGoal,
                  unsigned int *WaterCnt);

void MenuButton_Init(void);

void DisplayDateTime(unsigned char hour,
                     unsigned char min,
                     unsigned char sec,
                     unsigned char day,
                     unsigned char month,
                     unsigned int year,
                     unsigned int WaterCnt);

void DisplayProgress(unsigned int DailyGoal,
                     unsigned int WaterCnt);

void Print2Digit(unsigned int value);
void Print4Digit(unsigned int value);
unsigned int IsLeapYear(unsigned int year);
unsigned int DaysInMonth(unsigned int month,
                         unsigned int year);


void __irq EINT1_ISR(void)
{
    menu_request = 1;

  
    EXTINT = (1U << 1);

   
    VICVectAddr = 0U;
}


void MenuButton_Init(void)
{
   
    PINSEL0 &= ~(3U << 28);
    PINSEL0 |=  (2U << 28);

    
    EXTMODE |= (1U << 1);
    EXTPOLAR &= ~(1U << 1);

    EXTINT = (1U << 1);

    
    VICIntEnClr = (1U << 15);
    VICIntSelect &= ~(1U << 15);

    VICVectAddr0 = (unsigned long)EINT1_ISR;
    VICVectCntl0 = 0x20U | 15U;

    VICIntEnable = (1U << 15);
}


void Print2Digit(unsigned int value)
{
    LCD_data(((value / 10U) % 10U) + '0');
    LCD_data((value % 10U) + '0');
}

void Print4Digit(unsigned int value)
{
    LCD_data(((value / 1000) % 10) + '0');
    LCD_data(((value / 100) % 10) + '0');
    LCD_data(((value / 10) % 10) + '0');
    LCD_data((value % 10) + '0');
}


unsigned int IsLeapYear(unsigned int year)
{
    if ((year % 400U) == 0U)
        return 1U;

    if ((year % 100U) == 0U)
        return 0U;

    if ((year % 4U) == 0U)
        return 1U;

    return 0U;
}


unsigned int DaysInMonth(unsigned int month,
                         unsigned int year)
{
    if (month == 2U)
    {
        if (IsLeapYear(year))
            return 29U;
        else
            return 28U;
    }

    if (month == 4U || month == 6U ||
        month == 9U || month == 11U)
        return 30U;

    if (month >= 1U && month <= 12U)
        return 31U;

    return 0U;
}


unsigned int Menu(unsigned int DailyGoal,
                  unsigned int *WaterCnt)
{
    char key;

    unsigned int goal;
    unsigned int input_hour;
    unsigned int input_minute;
    unsigned int input_second;
    unsigned int input_day;
    unsigned int input_month;
    unsigned int input_year;

    while (1)
    {
        LCD_cmd(0x01);
        delay_ms(2);

        LCD_Goto(0, 0);
        LCD_string("1.Goal 2.Time");

        LCD_Goto(1, 0);
        LCD_string("3.Date 4.Exit");

        key = Keypad_GetKey();

        
        if (key == '1')
        {
            LCD_cmd(0x01);
            delay_ms(2);

            LCD_Goto(0, 0);
            LCD_string("Set Goal:");

            LCD_Goto(1, 0);
            goal = Keypad_GetNumber(2);

            if (goal > 0U)
            {
                DailyGoal = goal;

                LCD_cmd(0x01);
                delay_ms(2);
                LCD_Goto(0, 0);
                LCD_string("Goal Updated");
            }
            else
            {
                LCD_cmd(0x01);
                delay_ms(2);
                LCD_Goto(0, 0);
                LCD_string("Invalid Goal");
            }

            delay_ms(1000);
        }

     
        else if (key == '2')
        {
            LCD_cmd(0x01);
            delay_ms(2);
            LCD_Goto(0, 0);
            LCD_string("Hour (0-23):");
            LCD_Goto(1, 0);
            input_hour = Keypad_GetNumber(2);

            LCD_cmd(0x01);
            delay_ms(2);
            LCD_Goto(0, 0);
            LCD_string("Minute (0-59):");
            LCD_Goto(1, 0);
            input_minute = Keypad_GetNumber(2);

            LCD_cmd(0x01);
            delay_ms(2);
            LCD_Goto(0, 0);
            LCD_string("Second (0-59):");
            LCD_Goto(1, 0);
            input_second = Keypad_GetNumber(2);

            if (input_hour < 24U &&
                input_minute < 60U &&
                input_second < 60U)
            {
                HOUR = input_hour;
                MIN  = input_minute;
                SEC  = input_second;

                LCD_cmd(0x01);
                delay_ms(2);
                LCD_Goto(0, 0);
                LCD_string("Time Updated");
            }
            else
            {
                LCD_cmd(0x01);
                delay_ms(2);
                LCD_Goto(0, 0);
                LCD_string("Invalid Time");
            }

            delay_ms(1000);
        }

        
        else if (key == '3')
        {
            LCD_cmd(0x01);
            delay_ms(2);
            LCD_Goto(0, 0);
            LCD_string("Day (1-31):");
            LCD_Goto(1, 0);
            input_day = Keypad_GetNumber(2);

            LCD_cmd(0x01);
            delay_ms(2);
            LCD_Goto(0, 0);
            LCD_string("Month (1-12):");
            LCD_Goto(1, 0);
            input_month = Keypad_GetNumber(2);

            LCD_cmd(0x01);
            delay_ms(2);
            LCD_Goto(0, 0);
            LCD_string("Year (2000-2099)");
            LCD_Goto(1, 0);
            input_year = Keypad_GetNumber(4);

            if (input_year >= 2000U &&
                input_year <= 2099U &&
                input_month >= 1U &&
                input_month <= 12U &&
                input_day >= 1U &&
                input_day <= DaysInMonth(input_month,
                                         input_year))
            {
                DOM   = input_day;
                MONTH = input_month;
                YEAR  = input_year;

               
                *WaterCnt = 0;

                LCD_cmd(0x01);
                delay_ms(2);
                LCD_Goto(0, 0);
                LCD_string("Date Updated");
            }
            else
            {
                LCD_cmd(0x01);
                delay_ms(2);
                LCD_Goto(0, 0);
                LCD_string("Invalid Date");
            }

            delay_ms(1000);
        }

        
        else if (key == '4')
        {
            return DailyGoal;
        }
    }
}


void DisplayDateTime(unsigned char hour,
                     unsigned char min,
                     unsigned char sec,
                     unsigned char day,
                     unsigned char month,
                     unsigned int year,
                     unsigned int WaterCnt)
{
    LCD_cmd(0x01);
    delay_ms(2);

    LCD_Goto(0, 0);
    LCD_string("Time:");

    Print2Digit(hour);
    LCD_data(':');
    Print2Digit(min);
    LCD_data(':');
    Print2Digit(sec);

    LCD_Goto(1, 0);
    LCD_string("W:");
    LCD_Printnum(WaterCnt);

    LCD_string(" D:");
    Print2Digit(day);
    LCD_data('/');
    Print2Digit(month);
	LCD_data('/');
	Print4Digit(year);

    
}


void DisplayProgress(unsigned int DailyGoal,
                     unsigned int WaterCnt)
{
    unsigned int Remaining;
    unsigned int Percentage;

    if (DailyGoal == 0U)
    {
        Remaining = 0U;
        Percentage = 0U;
    }
    else if (WaterCnt >= DailyGoal)
    {
        Remaining = 0U;
        Percentage = 100U;
    }
    else
    {
        Remaining = DailyGoal - WaterCnt;
        Percentage = (WaterCnt * 100U) / DailyGoal;
    }

    LCD_cmd(0x01);
    delay_ms(2);

    LCD_Goto(0, 0);
    LCD_string("Goal:");
    LCD_Printnum(DailyGoal);

    LCD_string(" Rem:");
    LCD_Printnum(Remaining);

    LCD_Goto(1, 0);
    LCD_string("Progress:");
    LCD_Printnum(Percentage);
    LCD_string("%");
}


int main(void)
{
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
    unsigned char day;
    unsigned char month;
    unsigned int year;

    unsigned char previous_day;
    unsigned char previous_month;
    unsigned int previous_year;

    unsigned int DailyGoal;
    unsigned int WaterCnt = 0U;
    unsigned int tick;
    unsigned int screen = 0U;

    unsigned int last_water_sec;
    unsigned int current_total_sec;
    unsigned int elapsed_sec;

    unsigned char drink_now;
    unsigned char drink_previous = 1U;

    unsigned char screen_now;
    unsigned char screen_previous = 1U;

  
    LCD_Init();
    Keypad_Init();
    RTC_Init();

    RTC_GetTime(&hour, &min, &sec);
    RTC_GetDate(&day, &month, &year);

    previous_day = day;
    previous_month = month;
    previous_year = year;

    last_water_sec =
        ((unsigned int)hour * 3600U) +
        ((unsigned int)min * 60U) +
        (unsigned int)sec;

    
    IODIR0 &= ~(BUTTON_PIN | SCREEN_BUTTON_PIN);

    IODIR0 |= BUZZER_PIN |
              YELLOW_LED |
              RED_LED |
              GREEN_LED;

    IOCLR0 = BUZZER_PIN |
             YELLOW_LED |
             RED_LED |
             GREEN_LED;

    
    MenuButton_Init();

    
    LCD_cmd(0x01);
    delay_ms(2);
	LCD_string("AQUAGUARDIAN");
	delay_ms(2000);
	
	LCD_cmd(0x01);
    delay_ms(2);

    do
    {
        LCD_cmd(0x01);
        delay_ms(2);

        LCD_Goto(0, 0);
        LCD_string("Set Goal:");

        LCD_Goto(1, 0);
        DailyGoal = Keypad_GetNumber(2);

        if (DailyGoal == 0U)
        {
            LCD_cmd(0x01);
            delay_ms(2);

            LCD_Goto(0, 0);
            LCD_string("Invalid Goal");

            LCD_Goto(1, 0);
            LCD_string("Enter Again");

            delay_ms(1000);
        }
    } while (DailyGoal == 0U);

   
    while (1)
    {
       
        if (menu_request != 0U)
        {
            menu_request = 0U;

            DailyGoal = Menu(DailyGoal, &WaterCnt);

            EXTINT = (1U << 1);
            menu_request = 0U;

            RTC_GetTime(&hour, &min, &sec);
            RTC_GetDate(&day, &month, &year);

            last_water_sec =
                ((unsigned int)hour * 3600U) +
                ((unsigned int)min * 60U) +
                (unsigned int)sec;

            previous_day = day;
            previous_month = month;
            previous_year = year;

            IOCLR0 = BUZZER_PIN |
                     YELLOW_LED |
                     RED_LED;

            if (WaterCnt >= DailyGoal)
                IOSET0 = GREEN_LED;
            else
                IOCLR0 = GREEN_LED;

            screen = 0U;

            LCD_cmd(0x01);
            delay_ms(2);
        }

       
        RTC_GetTime(&hour, &min, &sec);
        RTC_GetDate(&day, &month, &year);

      
        if (day != previous_day ||
            month != previous_month ||
            year != previous_year)
        {
            WaterCnt = 0U;

            previous_day = day;
            previous_month = month;
            previous_year = year;

            last_water_sec =
                ((unsigned int)hour * 3600U) +
                ((unsigned int)min * 60U) +
                (unsigned int)sec;

            IOCLR0 = BUZZER_PIN |
                     YELLOW_LED |
                     RED_LED |
                     GREEN_LED;
        }

       
        if (screen == 0U)
        {
            DisplayDateTime(hour, min, sec,
                            day, month, year, WaterCnt);
        }
        else
        {
            DisplayProgress(DailyGoal, WaterCnt);
        }

     
        for (tick = 0U; tick < 200U; tick++)
        {
            
            if (menu_request != 0U)
            {
                menu_request = 0U;

                DailyGoal = Menu(DailyGoal, &WaterCnt);

                EXTINT = (1U << 1);
                menu_request = 0U;

                RTC_GetTime(&hour, &min, &sec);
                RTC_GetDate(&day, &month, &year);

                last_water_sec =
                    ((unsigned int)hour * 3600U) +
                    ((unsigned int)min * 60U) +
                    (unsigned int)sec;

                previous_day = day;
                previous_month = month;
                previous_year = year;

                IOCLR0 = BUZZER_PIN |
                         YELLOW_LED |
                         RED_LED;

                if (WaterCnt >= DailyGoal)
                    IOSET0 = GREEN_LED;
                else
                    IOCLR0 = GREEN_LED;

                screen = 0U;

                LCD_cmd(0x01);
                delay_ms(2);

                break;
            }

            
            screen_now =
                ((IOPIN0 & SCREEN_BUTTON_PIN) != 0U);

            if (screen_now == 0U &&
                screen_previous == 1U)
            {
                delay_ms(20);

                if ((IOPIN0 & SCREEN_BUTTON_PIN) == 0U)
                {
                    screen = (screen == 0U) ? 1U : 0U;

                    LCD_cmd(0x01);
                    delay_ms(2);
                }
            }

            screen_previous = screen_now;

          
            drink_now = ((IOPIN0 & BUTTON_PIN) != 0U);

            if (drink_now == 0U &&
                drink_previous == 1U)
            {
                delay_ms(20);

                if ((IOPIN0 & BUTTON_PIN) == 0U)
                {
                    if (WaterCnt < DailyGoal)
                        WaterCnt++;

                    RTC_GetTime(&hour, &min, &sec);

                    last_water_sec =
                        ((unsigned int)hour * 3600U) +
                        ((unsigned int)min * 60U) +
                        (unsigned int)sec;

                    IOCLR0 = BUZZER_PIN |
                             YELLOW_LED |
                             RED_LED;

                    if (WaterCnt >= DailyGoal)
                        IOSET0 = GREEN_LED;
                    else
                        IOCLR0 = GREEN_LED;

                    if (screen == 0U)
                    {
                        RTC_GetDate(&day, &month, &year);

                        DisplayDateTime(hour, min, sec,
                                        day, month, year,
                                        WaterCnt);
                    }
                    else
                    {
                        DisplayProgress(DailyGoal, WaterCnt);
                    }
                }
            }

            drink_previous = drink_now;

            delay_ms(10);
        }

        
        RTC_GetTime(&hour, &min, &sec);

        current_total_sec =
            ((unsigned int)hour * 3600U) +
            ((unsigned int)min * 60U) +
            (unsigned int)sec;

        
        if (current_total_sec >= last_water_sec)
        {
            elapsed_sec = current_total_sec - last_water_sec;
        }
        else
        {
            elapsed_sec =
                (86400U - last_water_sec) + current_total_sec;
        }

       
        if (WaterCnt >= DailyGoal)
        {
           
            IOSET0 = GREEN_LED;

            IOCLR0 = YELLOW_LED |
                     RED_LED |
                     BUZZER_PIN;
        }
        else
        {
            IOCLR0 = GREEN_LED;

            if (elapsed_sec >= REMINDER_INTERVAL_SEC)
            {
                
                if ((sec % 2) == 0)
                    IOSET0 = YELLOW_LED | BUZZER_PIN;
                else
                    IOCLR0 = YELLOW_LED | BUZZER_PIN;

               
                if (elapsed_sec >= RED_ALERT_SEC)
                    IOSET0 = RED_LED;
                else
                    IOCLR0 = RED_LED;												    
            }
            else
            {
                IOCLR0 = YELLOW_LED |
                         RED_LED |
                         BUZZER_PIN;
            }
        }
    }
}
