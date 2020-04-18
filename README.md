## ESP32 TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP32TimerInterrupt.svg)](https://github.com/khoih-prog/ESP32TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP32TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP32TimerInterrupt.svg)](http://github.com/khoih-prog/ESP32TimerInterrupt/issues)

This library enables you to use Interrupt from Hardware Timers on an ESP32-based board.

***Why do we need this Hardware Timer Interrupt?***

Imagine you have a system with a `mission-critical` function, measuring water level and control the sump pump or doing something much more important. You normally use a `software timer` to poll, or even place the function in loop(). But what if another function is blocking the loop() or setup().

So your function might not be executed, and the result would be disastrous.

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a `Hardware Timer with Interrupt` to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more precise (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is your function is now part of an `ISR (Interrupt Service Routine)`, and must be `lean / mean`, and follow certain rules. More to read on:

[Attach Interrupt](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

**Important Notes:**
1. Inside the attached function, `delay()` won’t work and the value returned by `millis()` will not increment. Serial data received while in the function may be lost. You should declare as volatile any variables that you modify within the attached function.

2. Typically global variables are used to pass data between an ISR and the main program. To make sure `variables shared between an ISR and the main program` are updated correctly, declare them as `volatile`.

### Installation

#### Use Arduino Library Manager

The suggested and easiest way is to use `Arduino Library Manager`. Search for `ESP32TimerInterrupt`, then select / install the latest version.

#### Manual Install

The suggested way to install manually is to:

1. Navigate to [ESP32TimerInterrupt](https://github.com/khoih-prog/ESP32TimerInterrupt).
2. Download the latest release `ESP32TimerInterrupt-master.zip`.
3. Extract the zip file to `ESP32TimerInterrupt-master` directory 
4. Copy whole folder to Arduino libraries' directory such as `.Arduino/libraries/ESP32TimerInterrupt-master`.

### More useful Information

The ESP32 has two timer groups, each one with two general purpose hardware timers. 
All the timers are based on 64 bits counters and 16 bit prescalers. 
The timer counters can be configured to count up or down and support automatic reload and software reload.
They can also generate alarms when they reach a specific value, defined by the software. The value of the counter can be read by 
the software program.

### New from v1.0.2

Now with these new `16 ISR-based timers` (while consuming only ***1 hardware timer***), the maximum interval is practically unlimited (limited only by unsigned long miliseconds). The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers Therefore, their executions are not blocked by bad-behaving functions / tasks.
This important feature is absolutely necessary for mission-critical tasks. 

The `ISR_Timer_Complex` example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.
Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.
This non-being-blocked important feature is absolutely necessary for mission-critical tasks. 
You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

### Supported Boards

- ESP32

### Usage

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

`Timer0, Timer1, Timer2 and Timer3` are supported for ESP32.

How to use:

```
//These define's must be placed at the beginning before #include "TimerInterrupt.h"
#define TIMER_INTERRUPT_DEBUG      1

#include "ESP32TimerInterrupt.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN       2         // Pin D2 mapped to pin GPIO2/ADC12 of ESP32, control on-board LED
#endif

#define PIN_D23           23        // Pin D23 mapped to pin GPIO23/VSPI_MOSI of ESP32

void IRAM_ATTR TimerHandler0(void)
{
  static bool toggle0 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(LED_BUILTIN, OUTPUT);
  }

  #if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.println("ITimer0: millis() = " + String(millis()));
  #endif
  
  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(LED_BUILTIN, toggle0);
  toggle0 = !toggle0;
}

void IRAM_ATTR TimerHandler1(void)
{
  static bool toggle1 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(PIN_D23, OUTPUT);
  }

  #if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.println("ITimer1: millis() = " + String(millis()));
  #endif
  
  //timer interrupt toggles outputPin
  digitalWrite(PIN_D23, toggle1);
  toggle1 = !toggle1;
}

#define TIMER0_INTERVAL_MS        1000 

#define TIMER1_INTERVAL_MS        5000 

// Instantiate ESP32 timer0 and 1
ESP32Timer ITimer0(0);
ESP32Timer ITimer1(1);

void setup()
{
  Serial.begin(115200);
  Serial.println("\nStarting");
  
  // Using ESP32  => 80 / 160 / 240MHz CPU clock , 
  // For 64-bit timer counter
  // For 16-bit timer prescaler up to 1024

  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
    Serial.println("Starting  ITimer0 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer0. Select another freq. or timer");

  // Interval in microsecs    
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
    Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer1. Select another freq. or timer");    
}

void loop()
{
  
}

```
### TO DO

1. Search for bug and improvement.


### DONE

For current version v1.0.0

1. Basic hardware timers for ESP32.
2. More hardware-initiated software-enabled timers
3. Longer time interval


### Contributing
If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

### Copyright
Copyright 2019- Khoi Hoang
