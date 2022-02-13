# ESP32TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP32TimerInterrupt.svg)](https://github.com/khoih-prog/ESP32TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP32TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP32TimerInterrupt.svg)](http://github.com/khoih-prog/ESP32TimerInterrupt/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Buy Me A Coffee" style="height: 50px !important;width: 181px !important;" ></a>

---
---

## Table of Contents

* [Important Breaking Change from v2.0.0](#Important-Breaking-Change-from-v200)
* [Why do we need this ESP32TimerInterrupt library](#why-do-we-need-this-esp32timerinterrupt-library)
  * [Features](#features)
  * [Why using ISR-based Hardware Timer Interrupt is better](#why-using-isr-based-hardware-timer-interrupt-is-better)
  * [Currently supported Boards](#currently-supported-boards)
  * [Important Notes about ISR](#important-notes-about-isr)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Note for Platform IO using ESP32 LittleFS](#note-for-platform-io-using-esp32-littlefs)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)](#howto-use-analogread-with-esp32-running-wifi-andor-bluetooth-btble)
  * [1. ESP32 has 2 ADCs, named ADC1 and ADC2](#1--esp32-has-2-adcs-named-adc1-and-adc2)
  * [2. ESP32 ADCs functions](#2-esp32-adcs-functions)
  * [3. ESP32 WiFi uses ADC2 for WiFi functions](#3-esp32-wifi-uses-adc2-for-wifi-functions)
* [More useful Information](#more-useful-information)
* [How to use](#how-to-use)
* [Examples](#examples)
  * [ 1. Argument_None](examples/Argument_None)
  * [ 2. Change_Interval](examples/Change_Interval)
  * [ 3. ISR_16_Timers_Array](examples/ISR_16_Timers_Array)
  * [ 4. ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)
  * [ 5. RPM_Measure](examples/RPM_Measure)
  * [ 6. SwitchDebounce](examples/SwitchDebounce)
  * [ 7. TimerInterruptTest](examples/TimerInterruptTest)
  * [ 8. multiFileProject](examples/multiFileProject) **New**
* [Example ISR_16_Timers_Array_Complex](#example-ISR_16_Timers_Array_Complex)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. TimerInterruptTest on ESP32C3_DEV](#1-TimerInterruptTest-on-ESP32C3_DEV)
  * [2. Change_Interval on ESP32C3_DEV](#2-Change_Interval-on-ESP32C3_DEV)
  * [3. Argument_None on ESP32S3_DEV](#3-Argument_None-on-ESP32S3_DEV)
  * [4. ISR_16_Timers_Array_Complex on ESP32S3_DEV](#4-ISR_16_Timers_Array_Complex-on-ESP32S3_DEV)
  * [5. ISR_16_Timers_Array on ESP32S3_DEV](#5-ISR_16_Timers_Array-on-ESP32S3_DEV)
  * [6. ISR_16_Timers_Array_Complex on ESP32C3_DEV](#6-ISR_16_Timers_Array_Complex-on-ESP32C3_DEV)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---


### Important Breaking Change from v2.0.0

Please have a look at [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)

---
---


### Why do we need this [ESP32TimerInterrupt library](https://github.com/khoih-prog/ESP32TimerInterrupt)

### Features

This library enables you to use Interrupt from Hardware Timers on an ESP32-based board.

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based Timers, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based timers**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds) while **the accuracy is nearly perfect** compared to software timers. 

The most important feature is they're ISR-based timers. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_Timer_Complex**](examples/ISR_Timer_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.

Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

### Why using ISR-based Hardware Timer Interrupt is better

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with **Interrupt** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

### Currently supported Boards

1. ESP32 boards, such as `ESP32_DEV`, etc.
2. ESP32_S2-based boards, such as `ESP32S2_DEV`, `ESP32_S2 Saola`, etc.
3. ESP32_C3-based boards, such as `ESP32C3_DEV`, etc. **New**
4. ESP32_S3 (ESP32S3_DEV, ESP32_S3_BOX, UM TINYS3, UM PROS3, UM FEATHERS3, etc.) **New**

---

### Important Notes about ISR

1. Inside the attached function, **delay() won’t work and the value returned by millis() will not increment.** Serial data received while in the function may be lost. You should declare as **volatile any variables that you modify within the attached function.**

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.


---
---

## Prerequisites

1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
2. [`ESP32 Core 2.0.2+`](https://github.com/espressif/arduino-esp32) for ESP32-based boards. [![Latest release](https://img.shields.io/github/release/espressif/arduino-esp32.svg)](https://github.com/espressif/arduino-esp32/releases/latest/).
3. [`SimpleTimer library`](https://github.com/jfturcot/SimpleTimer) to use with some examples.

---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**ESP32TimerInterrupt**](https://github.com/khoih-prog/ESP32TimerInterrupt), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32TimerInterrupt) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**ESP32TimerInterrupt**](https://github.com/khoih-prog/ESP32TimerInterrupt) page.
2. Download the latest release `ESP32TimerInterrupt-master.zip`.
3. Extract the zip file to `ESP32TimerInterrupt-master` directory 
4. Copy whole `ESP32TimerInterrupt-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**ESP32TimerInterrupt** library](https://registry.platformio.org/libraries/khoih-prog/ESP32TimerInterrupt) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/ESP32TimerInterrupt/installation). Search for **ESP32TimerInterrupt** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### Note for Platform IO using ESP32 LittleFS

#### Necessary only for esp32 core v1.0.6-

From esp32 core v1.0.6+, [`LittleFS_esp32 v1.0.6`](https://github.com/lorol/LITTLEFS) has been included and this step is not necessary anymore.

In Platform IO, to fix the error when using [`LittleFS_esp32 v1.0`](https://github.com/lorol/LITTLEFS) for ESP32-based boards with ESP32 core v1.0.4- (ESP-IDF v3.2-), uncomment the following line

from

```
//#define CONFIG_LITTLEFS_FOR_IDF_3_2   /* For old IDF - like in release 1.0.4 */
```

to

```
#define CONFIG_LITTLEFS_FOR_IDF_3_2   /* For old IDF - like in release 1.0.4 */
```

It's advisable to use the latest [`LittleFS_esp32 v1.0.5+`](https://github.com/lorol/LITTLEFS) to avoid the issue.

Thanks to [Roshan](https://github.com/solroshan) to report the issue in [Error esp_littlefs.c 'utime_p'](https://github.com/khoih-prog/ESPAsync_WiFiManager/issues/28) 

---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can use

```
#include <ESP32TimerInterrupt.hpp>               //https://github.com/khoih-prog/ESP32TimerInterrupt
```

in many files. But be sure to use the following `#include <ESP32TimerInterrupt.h>` **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include <ESP32TimerInterrupt.h>                //https://github.com/khoih-prog/ESP32TimerInterrupt
```

You now don't need to include `ESP32_ISR_Timer.h` anymore.


---
---

### HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)

Please have a look at [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to have more detailed description and solution of the issue.

#### 1.  ESP32 has 2 ADCs, named ADC1 and ADC2

#### 2. ESP32 ADCs functions

- ADC1 controls ADC function for pins **GPIO32-GPIO39**
- ADC2 controls ADC function for pins **GPIO0, 2, 4, 12-15, 25-27**

#### 3.. ESP32 WiFi uses ADC2 for WiFi functions

Look in file [**adc_common.c**](https://github.com/espressif/esp-idf/blob/master/components/driver/adc_common.c)

> In ADC2, there're two locks used for different cases:
> 1. lock shared with app and Wi-Fi:
>    ESP32:
>         When Wi-Fi using the ADC2, we assume it will never stop, so app checks the lock and returns immediately if failed.
>    ESP32S2:
>         The controller's control over the ADC is determined by the arbiter. There is no need to control by lock.
> 
> 2. lock shared between tasks:
>    when several tasks sharing the ADC2, we want to guarantee
>    all the requests will be handled.
>    Since conversions are short (about 31us), app returns the lock very soon,
>    we use a spinlock to stand there waiting to do conversions one by one.
> 
> adc2_spinlock should be acquired first, then adc2_wifi_lock or rtc_spinlock.


- In order to use ADC2 for other functions, we have to **acquire complicated firmware locks and very difficult to do**
- So, it's not advisable to use ADC2 with WiFi/BlueTooth (BT/BLE).
- Use ADC1, and pins GPIO32-GPIO39
- If somehow it's a must to use those pins serviced by ADC2 (**GPIO0, 2, 4, 12, 13, 14, 15, 25, 26 and 27**), use the **fix mentioned at the end** of [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to work with ESP32 WiFi/BlueTooth (BT/BLE).

---
---

## More useful Information

### ESP32 Hardware Timers

  - **The ESP32, ESP32_S2 and ESP32_S3 has two timer groups, each one with two general purpose hardware timers.**
  - **The ESP32_C3 has two timer groups, each one with only one general purpose hardware timer.**
  - All the timers are based on **64-bit counters (except 54-bit counter for ESP32_S3 counter) and 16-bit prescalers.**
  - The timer counters can be configured to count up or down and support automatic reload and software reload.
  - They can also generate alarms when they reach a specific value, defined by the software. 
  - The value of the counter can be read by the software program.

---


Now with these new `16 ISR-based timers` (while consuming only **1 hardware timer**), the maximum interval is practically unlimited (limited only by unsigned long miliseconds). The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers Therefore, their executions are not blocked by bad-behaving functions / tasks.
This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.
Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks. 

You'll see software-based `SimpleTimer` is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---
---

## How to use

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

`Timer0, Timer1, Timer2 and Timer3` are supported for ESP32, ESP32_S2 and ESP32_S3.
`Timer0, Timer1` are supported for ESP32_C3.

---
---

### Examples: 

 1. [Argument_None](examples/Argument_None)
 2. [Change_Interval](examples/Change_Interval)
 3. [**ISR_16_Timers_Array**](examples/ISR_16_Timers_Array)
 4. [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex)
 5. [RPM_Measure](examples/RPM_Measure)
 6. [SwitchDebounce](examples/SwitchDebounce)
 7. [TimerInterruptTest](examples/TimerInterruptTest)
 8. [multiFileProject](examples/multiFileProject) **New**




---
---

### Example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)

```
#if !defined( ESP32 )
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP32TimerInterrupt.h"
#define _TIMERINTERRUPT_LOGLEVEL_     4

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ESP32TimerInterrupt.h"

#include <SimpleTimer.h>              // https://github.com/jfturcot/SimpleTimer

// Don't use PIN_D1 in core v2.0.0 and v2.0.1. Check https://github.com/espressif/arduino-esp32/issues/5868

#ifndef LED_BUILTIN
  #define LED_BUILTIN       2
#endif

#ifndef LED_BLUE
  #define LED_BLUE          25
#endif

#ifndef LED_RED
  #define LED_RED           27
#endif

#define HW_TIMER_INTERVAL_US      10000L

volatile uint32_t startMillis = 0;

// Init ESP32 timer 1
ESP32Timer ITimer(1);

// Init ESP32_ISR_Timer
ESP32_ISR_Timer ISR_Timer;

#define LED_TOGGLE_INTERVAL_MS        2000L

// With core v2.0.0+, you can't use Serial.print/println in ISR or crash.
// and you can't use float calculation inside ISR
// Only OK in core v1.0.6-
bool IRAM_ATTR TimerHandler(void * timerNo)
{ 
  static bool toggle  = false;
  static int timeRun  = 0;

  ISR_Timer.run();

  // Toggle LED every LED_TOGGLE_INTERVAL_MS = 2000ms = 2s
  if (++timeRun == ((LED_TOGGLE_INTERVAL_MS * 1000) / HW_TIMER_INTERVAL_US) )
  {
    timeRun = 0;

    //timer interrupt toggles pin LED_BUILTIN
    digitalWrite(LED_BUILTIN, toggle);
    toggle = !toggle;
  }

  return true;
}

/////////////////////////////////////////////////

#define NUMBER_ISR_TIMERS         16

typedef void (*irqCallback)  ();

/////////////////////////////////////////////////

#define USE_COMPLEX_STRUCT      true

#if USE_COMPLEX_STRUCT

typedef struct
{
  irqCallback   irqCallbackFunc;
  uint32_t      TimerInterval;
  unsigned long deltaMillis;
  unsigned long previousMillis;
} ISRTimerData;

// In NRF52, avoid doing something fancy in ISR, for example Serial.print()
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash

void doingSomething(int index);

#else

volatile unsigned long deltaMillis    [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile unsigned long previousMillis [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// You can assign any interval for any timer here, in milliseconds
uint32_t TimerInterval[NUMBER_ISR_TIMERS] =
{
  5000L,  10000L,  15000L,  20000L,  25000L,  30000L,  35000L,  40000L,
  45000L, 50000L,  55000L,  60000L,  65000L,  70000L,  75000L,  80000L
};

void doingSomething(int index)
{
  unsigned long currentMillis  = millis();

  deltaMillis[index]    = currentMillis - previousMillis[index];
  previousMillis[index] = currentMillis;
}

#endif

////////////////////////////////////
// Shared
////////////////////////////////////

void doingSomething0()
{
  doingSomething(0);
}

void doingSomething1()
{
  doingSomething(1);
}

void doingSomething2()
{
  doingSomething(2);
}

void doingSomething3()
{
  doingSomething(3);
}

void doingSomething4()
{
  doingSomething(4);
}

void doingSomething5()
{
  doingSomething(5);
}

void doingSomething6()
{
  doingSomething(6);
}

void doingSomething7()
{
  doingSomething(7);
}

void doingSomething8()
{
  doingSomething(8);
}

void doingSomething9()
{
  doingSomething(9);
}

void doingSomething10()
{
  doingSomething(10);
}

void doingSomething11()
{
  doingSomething(11);
}

void doingSomething12()
{
  doingSomething(12);
}

void doingSomething13()
{
  doingSomething(13);
}

void doingSomething14()
{
  doingSomething(14);
}

void doingSomething15()
{
  doingSomething(15);
}

#if USE_COMPLEX_STRUCT

ISRTimerData curISRTimerData[NUMBER_ISR_TIMERS] =
{
  //irqCallbackFunc, TimerInterval, deltaMillis, previousMillis
  { doingSomething0,    5000L, 0, 0 },
  { doingSomething1,   10000L, 0, 0 },
  { doingSomething2,   15000L, 0, 0 },
  { doingSomething3,   20000L, 0, 0 },
  { doingSomething4,   25000L, 0, 0 },
  { doingSomething5,   30000L, 0, 0 },
  { doingSomething6,   35000L, 0, 0 },
  { doingSomething7,   40000L, 0, 0 },
  { doingSomething8,   45000L, 0, 0 },
  { doingSomething9,   50000L, 0, 0 },
  { doingSomething10,  55000L, 0, 0 },
  { doingSomething11,  60000L, 0, 0 },
  { doingSomething12,  65000L, 0, 0 },
  { doingSomething13,  70000L, 0, 0 },
  { doingSomething14,  75000L, 0, 0 },
  { doingSomething15,  80000L, 0, 0 }
};

void doingSomething(int index)
{
  unsigned long currentMillis  = millis();

  curISRTimerData[index].deltaMillis    = currentMillis - curISRTimerData[index].previousMillis;
  curISRTimerData[index].previousMillis = currentMillis;
}

#else

irqCallback irqCallbackFunc[NUMBER_ISR_TIMERS] =
{
  doingSomething0,  doingSomething1,  doingSomething2,  doingSomething3,
  doingSomething4,  doingSomething5,  doingSomething6,  doingSomething7,
  doingSomething8,  doingSomething9,  doingSomething10, doingSomething11,
  doingSomething12, doingSomething13, doingSomething14, doingSomething15
};

#endif
///////////////////////////////////////////

#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerDoingSomething2s()
{
  static unsigned long previousMillis = startMillis;

  unsigned long currMillis = millis();

  Serial.print(F("SimpleTimer : ")); Serial.print(SIMPLE_TIMER_MS / 1000);
  Serial.print(F(", ms : ")); Serial.print(currMillis);
  Serial.print(F(", Dms : ")); Serial.println(currMillis - previousMillis);

  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT
    Serial.print(F("Timer : ")); Serial.print(i);
    Serial.print(F(", programmed : ")); Serial.print(curISRTimerData[i].TimerInterval);
    Serial.print(F(", actual : ")); Serial.println(curISRTimerData[i].deltaMillis);
#else
    Serial.print(F("Timer : ")); Serial.print(i);
    Serial.print(F(", programmed : ")); Serial.print(TimerInterval[i]);
    Serial.print(F(", actual : ")); Serial.println(deltaMillis[i]);
#endif
  }

  previousMillis = currMillis;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print(F("\nStarting ISR_16_Timers_Array_Complex on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP32_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler))
  {
    startMillis = millis();
    Serial.print(F("Starting ITimer OK, millis() = ")); Serial.println(startMillis);
  }
  else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));

  startMillis = millis();

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT
    curISRTimerData[i].previousMillis = startMillis;
    ISR_Timer.setInterval(curISRTimerData[i].TimerInterval, curISRTimerData[i].irqCallbackFunc);
#else
    previousMillis[i] = millis();
    ISR_Timer.setInterval(TimerInterval[i], irqCallbackFunc[i]);
#endif
  }

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of ISR_Timer still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ISR_Timer is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
  simpleTimer.run();
}
```
---
---

### Debug Terminal Output Samples

### 1. TimerInterruptTest on ESP32C3_DEV

The following is the sample terminal output when running example [TimerInterruptTest](examples/TimerInterruptTest) on **ESP32C3_DEV** to demonstrate how to start/stop Hardware Timers.


```
Starting TimerInterruptTest on ESP32C3_DEV
ESP32TimerInterrupt v2.0.0
CPU Frequency = 160 MHz
[TISR] ESP32_TimerInterrupt: _timerNo = 0 , _fre = 1000000
[TISR] TIMER_BASE_CLK = 80000000 , TIMER_DIVIDER = 80
[TISR] _timerIndex = 0 , _timerGroup = 0
[TISR] _count = 0 - 1000000
[TISR] timer_set_alarm_value = 1000000.00
Starting  ITimer0 OK, millis() = 314
[TISR] ESP32_TimerInterrupt: _timerNo = 1 , _fre = 1000000
[TISR] TIMER_BASE_CLK = 80000000 , TIMER_DIVIDER = 80
[TISR] _timerIndex = 0 , _timerGroup = 1
[TISR] _count = 0 - 3000000
[TISR] timer_set_alarm_value = 3000000.00
Starting  ITimer1 OK, millis() = 346
Stop ITimer0, millis() = 5001
Start ITimer0, millis() = 10002
Stop ITimer1, millis() = 15001
Stop ITimer0, millis() = 15003
Start ITimer0, millis() = 20004
Stop ITimer0, millis() = 25005
Start ITimer1, millis() = 30002
Start ITimer0, millis() = 30006
Stop ITimer0, millis() = 35007
```


---


### 2. Change_Interval on ESP32C3_DEV

The following is the sample terminal output when running example [Change_Interval](examples/Change_Interval) on **ESP32C3_DEV** to demonstrate how to change Timer Interval on-the-fly

```
Starting Change_Interval on ESP32C3_DEV
ESP32TimerInterrupt v2.0.0
CPU Frequency = 160 MHz
Starting  ITimer0 OK, millis() = 293
Starting  ITimer1 OK, millis() = 303
Time = 10001, Timer0Count = 5, Timer1Count = 2
Time = 20002, Timer0Count = 10, Timer1Count = 4
Changing Interval, Timer0 = 4000,  Timer1 = 10000
Time = 30003, Timer0Count = 12, Timer1Count = 5
Time = 40004, Timer0Count = 15, Timer1Count = 6
Changing Interval, Timer0 = 2000,  Timer1 = 5000
Time = 50005, Timer0Count = 20, Timer1Count = 8
Time = 60006, Timer0Count = 25, Timer1Count = 10
Changing Interval, Timer0 = 4000,  Timer1 = 10000
Time = 70007, Timer0Count = 27, Timer1Count = 11
Time = 80008, Timer0Count = 30, Timer1Count = 12
Changing Interval, Timer0 = 2000,  Timer1 = 5000
Time = 90009, Timer0Count = 35, Timer1Count = 14
Time = 100010, Timer0Count = 40, Timer1Count = 16
Changing Interval, Timer0 = 4000,  Timer1 = 10000
Time = 110011, Timer0Count = 42, Timer1Count = 17
Time = 120012, Timer0Count = 45, Timer1Count = 18
Changing Interval, Timer0 = 2000,  Timer1 = 5000
Time = 130013, Timer0Count = 50, Timer1Count = 20
Time = 140014, Timer0Count = 55, Timer1Count = 22
Changing Interval, Timer0 = 4000,  Timer1 = 10000
```

---

### 3. Argument_None on ESP32S3_DEV

The following is the sample terminal output when running example [Argument_None](examples/Argument_None) on **ESP32S3_DEV**

```
Starting Argument_None on ESP32S3_DEV
ESP32TimerInterrupt v2.0.0
CPU Frequency = 240 MHz
[TISR] ESP32_S3_TimerInterrupt: _timerNo = 0 , _fre = 1000000
[TISR] TIMER_BASE_CLK = 80000000 , TIMER_DIVIDER = 80
[TISR] _timerIndex = 0 , _timerGroup = 0
[TISR] _count = 0 - 1000000
[TISR] timer_set_alarm_value = 1000000.00
Starting  ITimer0 OK, millis() = 317
[TISR] ESP32_S3_TimerInterrupt: _timerNo = 1 , _fre = 1000000
[TISR] TIMER_BASE_CLK = 80000000 , TIMER_DIVIDER = 80
[TISR] _timerIndex = 1 , _timerGroup = 0
[TISR] _count = 0 - 5000000
[TISR] timer_set_alarm_value = 5000000.00
Starting  ITimer1 OK, millis() = 348
```

---

### 4. ISR_16_Timers_Array_Complex on ESP32S3_DEV

The following is the sample terminal output when running example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) on **ESP32S3_DEV** to demonstrate of ISR Hardware Timer, especially when system is very busy or blocked. The 16 independent ISR timers are programmed to be activated repetitively after certain intervals, is activated exactly after that programmed interval !!!

```
Starting ISR_16_Timers_Array_Complex on ESP32S3_DEV
ESP32TimerInterrupt v2.0.0
CPU Frequency = 240 MHz
[TISR] ESP32_S3_TimerInterrupt: _timerNo = 1 , _fre = 1000000
[TISR] TIMER_BASE_CLK = 80000000 , TIMER_DIVIDER = 80
[TISR] _timerIndex = 1 , _timerGroup = 0
[TISR] _count = 0 - 10000
[TISR] timer_set_alarm_value = 10000.00
Starting ITimer OK, millis() = 318
SimpleTimer : 2, ms : 10317, Dms : 9999
Timer : 0, programmed : 5000, actual : 5009
Timer : 1, programmed : 10000, actual : 0
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 20380, Dms : 10063
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15009
Timer : 3, programmed : 20000, actual : 20009
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 30443, Dms : 10063
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20009
Timer : 4, programmed : 25000, actual : 25009
Timer : 5, programmed : 30000, actual : 30009
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 40506, Dms : 10063
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25009
Timer : 5, programmed : 30000, actual : 30009
Timer : 6, programmed : 35000, actual : 35009
Timer : 7, programmed : 40000, actual : 40009
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 50569, Dms : 10063
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30009
Timer : 6, programmed : 35000, actual : 35009
Timer : 7, programmed : 40000, actual : 40009
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50009
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 60632, Dms : 10063
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35009
Timer : 7, programmed : 40000, actual : 40009
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50009
Timer : 10, programmed : 55000, actual : 55009
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 70695, Dms : 10063
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40009
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50009
Timer : 10, programmed : 55000, actual : 55009
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 65009
Timer : 13, programmed : 70000, actual : 70009
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 80758, Dms : 10063
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50009
Timer : 10, programmed : 55000, actual : 55009
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 65009
Timer : 13, programmed : 70000, actual : 70009
Timer : 14, programmed : 75000, actual : 75009
Timer : 15, programmed : 80000, actual : 80009
...
SimpleTimer : 2, ms : 161262, Dms : 10063
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45000
Timer : 9, programmed : 50000, actual : 50000
Timer : 10, programmed : 55000, actual : 55000
Timer : 11, programmed : 60000, actual : 60000
Timer : 12, programmed : 65000, actual : 65000
Timer : 13, programmed : 70000, actual : 70000
Timer : 14, programmed : 75000, actual : 75000
Timer : 15, programmed : 80000, actual : 80000

```

---

### 5. ISR_16_Timers_Array on ESP32S3_DEV

The following is the sample terminal output when running example [ISR_16_Timers_Array](examples/ISR_16_Timers_Array) on **ESP32S3_DEV** to demonstrate of ISR Hardware Timer, especially when system is very busy or blocked. The 16 independent ISR timers are programmed to be activated repetitively after certain intervals, is activated exactly after that programmed interval !!!


```
Starting ISR_16_Timers_Array on ESP32S3_DEV
ESP32TimerInterrupt v2.0.0
CPU Frequency = 240 MHz
[TISR] ESP32_S3_TimerInterrupt: _timerNo = 1 , _fre = 1000000
[TISR] TIMER_BASE_CLK = 80000000 , TIMER_DIVIDER = 80
[TISR] _timerIndex = 1 , _timerGroup = 0
[TISR] _count = 0 - 1000
[TISR] timer_set_alarm_value = 1000.00
Starting ITimer OK, millis() = 318
simpleTimerDoingSomething2s: Delta programmed ms = 2000, actual = 10001
simpleTimerDoingSomething2s: Delta programmed ms = 2000, actual = 10000
simpleTimerDoingSomething2s: Delta programmed ms = 2000, actual = 10000
simpleTimerDoingSomething2s: Delta programmed ms = 2000, actual = 10000
```

---

### 6. ISR_16_Timers_Array_Complex on ESP32C3_DEV

The following is the sample terminal output when running example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) on **ESP32C3_DEV** to demonstrate of ISR Hardware Timer, especially when system is very busy or blocked. The 16 independent ISR timers are programmed to be activated repetitively after certain intervals, is activated exactly after that programmed interval !!!


```
Starting ISR_16_Timers_Array_Complex on ESP32C3_DEV
ESP32TimerInterrupt v2.0.0
CPU Frequency = 160 MHz
[TISR] ESP32_TimerInterrupt: _timerNo = 1 , _fre = 1000000
[TISR] TIMER_BASE_CLK = 80000000 , TIMER_DIVIDER = 80
[TISR] _timerIndex = 0 , _timerGroup = 1
[TISR] _count = 0 - 10000
[TISR] timer_set_alarm_value = 10000.00
Starting ITimer OK, millis() = 314
SimpleTimer : 2, ms : 10314, Dms : 9999
Timer : 0, programmed : 5000, actual : 5009
Timer : 1, programmed : 10000, actual : 0
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 20381, Dms : 10067
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15009
Timer : 3, programmed : 20000, actual : 20009
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 30448, Dms : 10067
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20009
Timer : 4, programmed : 25000, actual : 25009
Timer : 5, programmed : 30000, actual : 30009
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 40515, Dms : 10067
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25009
Timer : 5, programmed : 30000, actual : 30009
Timer : 6, programmed : 35000, actual : 35009
Timer : 7, programmed : 40000, actual : 40009
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 50582, Dms : 10067
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30009
Timer : 6, programmed : 35000, actual : 35009
Timer : 7, programmed : 40000, actual : 40009
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50009
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 60649, Dms : 10067
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35009
Timer : 7, programmed : 40000, actual : 40009
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50009
Timer : 10, programmed : 55000, actual : 55009
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 70716, Dms : 10067
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40009
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50009
Timer : 10, programmed : 55000, actual : 55009
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 65009
Timer : 13, programmed : 70000, actual : 70009
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 80783, Dms : 10067
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50009
Timer : 10, programmed : 55000, actual : 55009
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 65009
Timer : 13, programmed : 70000, actual : 70009
Timer : 14, programmed : 75000, actual : 75009
Timer : 15, programmed : 80000, actual : 80009
...
SimpleTimer : 2, ms : 161319, Dms : 10067
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45000
Timer : 9, programmed : 50000, actual : 50000
Timer : 10, programmed : 55000, actual : 55000
Timer : 11, programmed : 60000, actual : 60000
Timer : 12, programmed : 65000, actual : 65000
Timer : 13, programmed : 70000, actual : 70000
Timer : 14, programmed : 75000, actual : 75000
Timer : 15, programmed : 80000, actual : 80000
```


---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level (_TIMERINTERRUPT_LOGLEVEL_) from 0 to 4

```cpp
// These define's must be placed at the beginning before #include "ESP32TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [ESP32TimerInterrupt issues](https://github.com/khoih-prog/ESP32TimerInterrupt/issues)

---

## TO DO

1. Search for bug and improvement.



## DONE

 1. Basic hardware timers for ESP32.
 2. More hardware-initiated software-enabled timers
 3. Longer time interval
 4. Similar features for remaining Arduino boards such as SAMD21, SAMD51, SAM-DUE, nRF52, ESP8266, STM32, etc.
 5. Add support to new ESP32-S2
 6. Add support to new ESP32 core v1.0.6
 7. Fix compiler errors due to conflict to some libraries.
 8. Add complex examples.
 9. Avoid using D1 in examples due to issue with core v2.0.0 and v2.0.1.
10. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories
11. Restructure library.
12. Add support to new ESP32_S3. Now supporting ESP32, ESP32_S2, ESP32_S3 and ESP32_C3
13. Optimize library code by using `reference-passing` instead of `value-passing`
14. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.

1. Thanks to [Jelmer](https://github.com/jjwbruijn) to report and make PR in [Moved the implementation header file to a separate .cpp file](https://github.com/khoih-prog/ESP32TimerInterrupt/pull/6) leading to new Version v1.1.0. 

<table>
  <tr>
    <td align="center"><a href="https://github.com/jjwbruijn"><img src="https://github.com/jjwbruijn.png" width="100px;" alt="jjwbruijn"/><br /><sub><b>Jelmer</b></sub></a><br /></td>
  </tr> 
</table>

---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/ESP32TimerInterrupt/blob/master/LICENSE)

---

## Copyright

Copyright 2019- Khoi Hoang


