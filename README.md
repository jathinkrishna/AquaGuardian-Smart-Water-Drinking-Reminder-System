# AquaGuardian -- Smart Water Drinking Reminder System

AquaGuardian is an embedded system project built around the **NXP
LPC2148 ARM7 microcontroller**. It reminds users to drink water, records
daily water intake, and displays hydration progress using an LCD,
keypad, LEDs, and a buzzer.

## Project Overview

The system uses the LPC2148's internal Real-Time Clock (RTC) to track
time and date. Users enter a daily water-intake goal, record each glass
with a push button, and receive visual and audible reminders when the
configured interval has elapsed.

## Features

-   Displays the current time and date on a 16×2 LCD.
-   Allows the user to configure a daily water-intake goal.
-   Records water consumption using a push button.
-   Displays the daily goal, remaining glasses, and hydration progress
    percentage.
-   Provides a keypad-based menu to update the goal, time, and date.
-   Supports keypad editing:
    -   Numeric keys (`0–9`) enter digits.
    -   `B` deletes the last entered digit (Backspace).
    -   `*` clears the complete input.
    -   `#` confirms the input.
-   Validates time values (hours, minutes, and seconds).
-   Validates dates, including month lengths and leap years.
-   Uses LEDs and a buzzer to indicate reminder and goal status.
-   Resets the daily water count when the RTC date changes.

## Hardware Requirements

-   LPC2148 development board
-   16×2 LCD
-   4×4 matrix keypad
-   Push buttons / switches
-   Yellow LED
-   Red LED
-   Green LED
-   Buzzer
-   USB-to-UART converter or compatible programming/debug connection
-   Connecting wires and suitable current-limiting resistors for LEDs

## Software Requirements

-   Embedded C
-   ARM7-compatible toolchain / IDE (for example, Keil µVision)
-   Flash Magic (if used for programming the LPC2148)
-   LPC2148 device header and supporting peripheral driver files

## System Operation

1.  On startup, the LCD displays the AquaGuardian welcome message.
2.  The user enters a daily water goal. A zero value is rejected.
3.  The main screen displays the RTC time/date and the number of glasses
    consumed.
4.  Pressing the drink button records one glass, up to the configured
    goal.
5.  The screen button switches between the time/date screen and the
    hydration-progress screen.
6.  Pressing the menu switch opens the configuration menu:
    -   `1` -- Set daily goal
    -   `2` -- Set time
    -   `3` -- Set date
    -   `4` -- Exit
7.  When the reminder interval is reached, the buzzer and yellow LED
    provide an alert. The red LED indicates that the longer alert
    threshold has been reached.
8.  When the daily goal is achieved, the green LED turns on.
9.  When the RTC date changes, the daily water count is reset while the
    configured goal is retained.

## Keypad Layout

The keypad mapping used by the driver is:

  1    2   3    A
  ---- --- ---- ---
  4    5   6    B
  7    8   9    C
  \*   0   \#   D

For numeric entry, `B` is Backspace, `*` clears the input, and `#`
confirms the entered value.

## Source Files

The project uses modular source and header files:

  -----------------------------------------------------------------------
  File                                Purpose
  ----------------------------------- -----------------------------------
  `main.c`                            Main application, menu, RTC
                                      handling, water tracking, and alert
                                      logic

  `keypad.c`                          4×4 keypad scanning and numeric
                                      input handling

  `keypad.h`                          Keypad function declarations

  `lcd.c`                             LCD driver implementation

  `lcd.h`                             LCD function declarations

  `rtc.c`                             RTC driver implementation

  `rtc.h`                             RTC function declarations

  `delay.c`                           Delay routines

  `delay.h`                           Delay function declarations
  -----------------------------------------------------------------------

Keep the required driver files in the same project and add their source
files to your IDE project before building.

## Build and Programming

1.  Create or open an LPC2148 project in your ARM7-compatible IDE.
2.  Add `main.c` and the required driver source files (`keypad.c`,
    `lcd.c`, `rtc.c`, and `delay.c`).
3.  Add the corresponding header files to the project include path.
4.  Select the correct LPC2148 device and configure the compiler and
    linker for your development board.
5.  Build the project and resolve any toolchain or driver configuration
    errors.
6.  Program the generated HEX file to the LPC2148 using the supported
    programming method (for example, Flash Magic).
7.  Connect the LCD, keypad, buttons, LEDs, and buzzer according to the
    pin definitions and verified board wiring.
8.  Power on the board and test each input and output.

**Hardware integration note:** Verify that the pin assignments in
`main.c`, `keypad.c`, and `lcd.c` match your actual board wiring and do
not overlap. The LCD and other peripherals must not share GPIO pins
unintentionally.
