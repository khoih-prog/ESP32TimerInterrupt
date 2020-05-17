# ESP32 TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP32TimerInterrupt.svg)](https://github.com/khoih-prog/ESP32TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP32TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP32TimerInterrupt.svg)](http://github.com/khoih-prog/ESP32TimerInterrupt/issues)

### Releases v1.0.3

1. Restructure code.
2. Add examples.
3. Enhance README.

### Releases v1.0.2

1. Permit up to 16 super-long-time, super-accurate ISR-based timers to avoid being blocked

## Features

This library enables you to use Interrupt from Hardware Timers on an ESP32-based board.

#### Why do we need this Hardware Timer Interrupt?

Imagine you have a system with a ***mission-critical*** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is ***blocking*** the loop() or setup().

So your function ***might not be executed, and the result would be disastrous.***

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with ***Interrupt*** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more ***precise*** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is ***your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.*** More to read on:

[Howto Attach Interrupt](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

#### Important Notes:

1. Inside the attached function, ***delay() won’t work and the value returned by millis() will not increment.*** Serial data received while in the function may be lost. You should declare as ***volatile any variables that you modify within the attached function.***

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

## Prerequisite
1. [`Arduino IDE 1.8.12 or later` for Arduino](https://www.arduino.cc/en/Main/Software)
2. [`ESP32 core 1.0.4 or later`](https://github.com/espressif/arduino-esp32/releases) for ESP32 boards

### Installation

The suggested way to install is to:

#### Use Arduino Library Manager

The best way is to use `Arduino Library Manager`. Search for `ESP32TimerInterrupt`, then select / install the latest version. You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32TimerInterrupt) for more detailed instructions.

#### Manual Install

1. Navigate to [ESP32TimerInterrupt](https://github.com/khoih-prog/ESP32TimerInterrupt) page.
2. Download the latest release `ESP32TimerInterrupt-master.zip`.
3. Extract the zip file to `ESP32TimerInterrupt-master` directory 
4. Copy whole 
  - `ESP32TimerInterrupt-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.


## More useful Information

The ESP32 has two timer groups, each one with two general purpose hardware timers. 
All the timers are based on 64 bits counters and 16 bit prescalers. 
The timer counters can be configured to count up or down and support automatic reload and software reload.
They can also generate alarms when they reach a specific value, defined by the software. The value of the counter can be read by 
the software program.

## New from v1.0.2

Now with these new `16 ISR-based timers` (while consuming only ***1 hardware timer***), the maximum interval is practically unlimited (limited only by unsigned long miliseconds). The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers Therefore, their executions are not blocked by bad-behaving functions / tasks.
This important feature is absolutely necessary for mission-critical tasks. 

The `ISR_Timer_Complex` example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.
Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.
This non-being-blocked important feature is absolutely necessary for mission-critical tasks. 
You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

### Also see examples: 

 1. [Argument_None](examples/Argument_None)
 2. [ISR_RPM_Measure](examples/ISR_RPM_Measure)
 3. [ISR_Switch](examples/ISR_Switch) 
 4. [ISR_Timer_4_Switches](examples/ISR_Timer_4_Switches) 
 5. [ISR_Timer_Complex](examples/ISR_Timer_Complex)
 6. [ISR_Timer_Switch](examples/ISR_Timer_Switch)
 7. [ISR_Timer_Switches](examples/ISR_Timer_Switches) 
 8. [RPM_Measure](examples/RPM_Measure)
 9. [SwitchDebounce](examples/SwitchDebounce)
10. [TimerInterruptTest](examples/TimerInterruptTest)


## Supported Boards

- ESP32

## Usage

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

`Timer0, Timer1, Timer2 and Timer3` are supported for ESP32.

### Example [ISR_Timer_Complex](examples/ISR_Timer_Complex)

```
#ifndef ESP32
#error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif

//These define's must be placed at the beginning before #include "ESP32TimerInterrupt.h"
#define TIMER_INTERRUPT_DEBUG      1

#define BLYNK_PRINT Serial

//#define BLYNK_DEBUG
#ifdef BLYNK_DEBUG
#undef BLYNK_DEBUG
#endif

//#include <ESP8266WiFi.h>
#include <esp_wifi.h>
#include <WiFi.h>

//#define USE_BLYNK_WM   true
#define USE_BLYNK_WM   false

#define USE_SSL     false

#if USE_SSL
#include <WiFiClientSecure.h>
#if USE_BLYNK_WM
#include <BlynkSimpleEsp32_SSL_WM.h>    //https://github.com/khoih-prog/Blynk_WM
#else
#include <BlynkSimpleEsp32_SSL.h>
#endif

#define BLYNK_HARDWARE_PORT     9443
#else
#include <WiFiClient.h>
#if USE_BLYNK_WM
#include <BlynkSimpleEsp32_WM.h>        //https://github.com/khoih-prog/Blynk_WM
#else
#include <BlynkSimpleEsp32.h>
#endif

#define BLYNK_HARDWARE_PORT     8080
#endif

#if !USE_BLYNK_WM
#define USE_LOCAL_SERVER    true

// If local server
#if USE_LOCAL_SERVER
char blynk_server[]   = "yourname.duckdns.org";
//char blynk_server[]   = "192.168.2.110";
#else
char blynk_server[]   = "";
#endif

char auth[]     = "****";
char ssid[]     = "****";
char pass[]     = "****";

#endif

#include "ESP32TimerInterrupt.h"
#include "ESP32_ISR_Timer.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN       2         // Pin D2 mapped to pin GPIO2/ADC12 of ESP32, control on-board LED
#endif

#define HW_TIMER_INTERVAL_MS        50

#define WIFI_TIMEOUT      20000L

volatile uint32_t lastMillis = 0;

// Init ESP32 timer 1
ESP32Timer ITimer(1);

// Init ESP32_ISR_Timer
ESP32_ISR_Timer ISR_Timer;

// Ibit Blynk Timer
BlynkTimer blynkTimer;

#define LED_TOGGLE_INTERVAL_MS        5000L

void IRAM_ATTR TimerHandler(void)
{
  static bool toggle  = false;
  static bool started = false;
  static int timeRun  = 0;

  ISR_Timer.run();

  // Toggle LED every LED_TOGGLE_INTERVAL_MS = 5000ms = 5s
  if (++timeRun == (LED_TOGGLE_INTERVAL_MS / HW_TIMER_INTERVAL_MS) )
  {
    timeRun = 0;

    if (!started)
    {
      started = true;
      pinMode(LED_BUILTIN, OUTPUT);
    }

    //timer interrupt toggles pin LED_BUILTIN
    digitalWrite(LED_BUILTIN, toggle);
    toggle = !toggle;
  }
}

// In ESP32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash : "Guru Meditation Error: Core 1 panic'ed (Cache disabled but cached memory region accessed)"
void IRAM_ATTR doingSomething2s()
{
  static unsigned long previousMillis = lastMillis;
  unsigned long deltaMillis = millis() - previousMillis;

#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.print("2s: D ms = ");
  Serial.println(deltaMillis);
#if (TIMER_INTERRUPT_DEBUG > 1)
  Serial.print("2s: core ");
  Serial.println(xPortGetCoreID());
#endif

#endif

  previousMillis = millis();
}

// In ESP32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash : "Guru Meditation Error: Core 1 panic'ed (Cache disabled but cached memory region accessed)"
void IRAM_ATTR doingSomething5s()
{
  static unsigned long previousMillis = lastMillis;
  unsigned long deltaMillis = millis() - previousMillis;

#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.print("5s: D ms = ");
  Serial.println(deltaMillis);
#if (TIMER_INTERRUPT_DEBUG > 1)
  Serial.print("5s: core ");
  Serial.println(xPortGetCoreID());
#endif

#endif

  previousMillis = millis();
}

// In ESP32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash : "Guru Meditation Error: Core 1 panic'ed (Cache disabled but cached memory region accessed)"
void IRAM_ATTR doingSomething11s()
{
  static unsigned long previousMillis = lastMillis;
  unsigned long deltaMillis = millis() - previousMillis;

#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.print("11s: D ms = ");
  Serial.println(deltaMillis);
#endif

  previousMillis = millis();
}

// In ESP32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash : "Guru Meditation Error: Core 1 panic'ed (Cache disabled but cached memory region accessed)"
void IRAM_ATTR doingSomething101s()
{
  static unsigned long previousMillis = lastMillis;
  unsigned long deltaMillis = millis() - previousMillis;

#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.print("101s: D ms = ");
  Serial.println(deltaMillis);
#endif

  previousMillis = millis();
}

#define BLYNK_TIMER_MS        2000L

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void blynkDoingSomething2s()
{
  static unsigned long previousMillis = lastMillis;
  Serial.println("blynkDoingSomething2s: Delta programmed ms = " + String(BLYNK_TIMER_MS) + ", actual = " + String(millis() - previousMillis));
  previousMillis = millis();
}

void setup()
{
  // Just a temporary hack
  // ESP32 WiFi is still buggy. By moving it up here, we can avoid interfering / interacting problem with other ISRs
  WiFi.begin(ssid, pass);

  delay(2000);

  Serial.begin(115200);
  Serial.println("\nStarting");

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  blynkTimer.setInterval(BLYNK_TIMER_MS, blynkDoingSomething2s);


  // Using ESP32  => 80 / 160 / 240MHz CPU clock ,
  // For 64-bit timer counter
  // For 16-bit timer prescaler up to 1024

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
  {
    lastMillis = millis();
    Serial.println("Starting  ITimer OK, millis() = " + String(lastMillis));
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  ISR_Timer.setInterval(2000L, doingSomething2s);
  ISR_Timer.setInterval(5000L, doingSomething5s);
  ISR_Timer.setInterval(11000L, doingSomething11s);
  ISR_Timer.setInterval(101000L, doingSomething101s);

#if USE_BLYNK_WM
  Blynk.begin();
#else

  unsigned long startWiFi = millis();

  do
  {
    delay(200);
    if ( (WiFi.status() == WL_CONNECTED) || (millis() > startWiFi + WIFI_TIMEOUT) )
      break;
  } while (WiFi.status() != WL_CONNECTED);

  Blynk.config(auth, blynk_server, BLYNK_HARDWARE_PORT);
  Blynk.connect();

  if (Blynk.connected())
    Serial.println("Blynk connected");
  else
    Serial.println("Blynk not connected yet");
#endif
}

#define BLOCKING_TIME_MS      3000L

void loop()
{
  static bool needWiFiBegin = true;

  Blynk.run();

  // Blynk.run() in ESP32 doesn't reconnect automatically without below code with WiFi.begin(ssid, pass);
  if (!Blynk.connected())
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      unsigned long startWiFi = millis();
      Serial.println("WiFi not connected. Reconnect");

      // Need to run again once per conn. lost
      if (needWiFiBegin)
      {
        Serial.println("WiFi begin again");
        WiFi.begin(ssid, pass);
        needWiFiBegin = false;
      }

      do
      {
        delay(200);
        if ( (WiFi.status() == WL_CONNECTED) || (millis() > startWiFi + WIFI_TIMEOUT) )
        {
          // There is ESP32 bug, if WiFi+Blynk connected, then lost WiFi+ Internet
          // ESP32 can't reconect, even if we place WiFi.begin() again in loop()
          // The _network_event_task(void * arg) in WiFiGeneric.cpp is just waiting forever for data
          ESP.restart();
        }
      } while (WiFi.status() != WL_CONNECTED);
    }
    else
    {
      // Ready for next conn. lost
      Serial.println("reset needWiFiBegin");
      needWiFiBegin = true;
      Blynk.config(auth, blynk_server, BLYNK_HARDWARE_PORT);
      Blynk.connect();
    }
  }
  else
  {
    // Ready for next conn. lost
    //needWiFiBegin = true;
  }


  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of ISR_Timer still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ISR_Timer is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
  blynkTimer.run();

}

```

The following is the sample terminal output when running example [ISR_Timer_Complex](examples/ISR_Timer_Complex) to demonstrate the accuracy of ISR Hardware Timer, ***especially when system is very busy***.  The ISR timer is ***programmed for 2s, is activated exactly after 2.000s !!!***

While software timer, ***programmed for 2s, is activated after 3.435s !!!***

```
Starting ISR_Timer_Complex
ESP32TimerInterrupt: _timerNo = 1, _fre = 1000000.00, _count = 0 - 50000
Starting  ITimer OK, millis() = 2140
[2341] 
    ___  __          __
   / _ )/ /_ _____  / /__
  / _  / / // / _ \/  '_/
 /____/_/\_, /_//_/_/\_\
        /___/ v0.6.1 on ESP32

[2342] Protocol connect: timeout =9000
[2346] BlynkArduinoClient.connect: Connecting to ****.duckdns.org:8080
[2506] Ready (ping: 35ms).
Blynk connected
2s: D ms = 2000   <=== ISR Timer still very accurate while system busy
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3435 <=== Software Timer not accurate when system busy
2s: D ms = 2000   <=== ISR Timer still very accurate no matter system busy or not
5s: D ms = 5000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
5s: D ms = 5000
11s: D ms = 11000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
5s: D ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3002
2s: D ms = 2000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
5s: D ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
11s: D ms = 11000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
5s: D ms = 5000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3002
2s: D ms = 2000
2s: D ms = 2000
5s: D ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
11s: D ms = 11000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: D ms = 2000
5s: D ms = 5000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
```

### Releases v1.0.3

1. Restructure code.
2. Add examples.
3. Enhance README.

### Releases v1.0.2

1. Permit up to 16 super-long-time, super-accurate ISR-based timers to avoid being blocked


### Releases v1.0.0

1. Initial coding


## TO DO

1. Search for bug and improvement.
2. Similar features for remaining Arduino boards suh as SAMD21, SAMD51, SAM-DUE, nRF52


## DONE

For current version v1.0.0

1. Basic hardware timers for ESP32.
2. More hardware-initiated software-enabled timers
3. Longer time interval


## Contributing
If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

## Copyright
Copyright 2019- Khoi Hoang
