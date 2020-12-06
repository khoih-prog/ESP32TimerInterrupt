/****************************************************************************************************************************
   Change_Interval.ino
   For ESP32 boards
   Written by Khoi Hoang

   Built by Khoi Hoang https://github.com/khoih-prog/ESP32TimerInterrupt
   Licensed under MIT license

   The ESP32 has two timer groups, each one with two general purpose hardware timers. All the timers are based on 64 bits
   counters and 16 bit prescalers. The timer counters can be configured to count up or down and support automatic reload
   and software reload. They can also generate alarms when they reach a specific value, defined by the software. The value
   of the counter can be read by the software program.

   Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
   unsigned long miliseconds), you just consume only one ESP32 timer and avoid conflicting with other cores' tasks.
   The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
   Therefore, their executions are not blocked by bad-behaving functions / tasks.
   This important feature is absolutely necessary for mission-critical tasks.

   Based on SimpleTimer - A timer library for Arduino.
   Author: mromani@ottotecnica.com
   Copyright (c) 2010 OTTOTECNICA Italy

   Based on BlynkTimer.h
   Author: Volodymyr Shymanskyy

   Version: 1.1.1

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
    1.0.0   K Hoang      23/11/2019 Initial coding
    1.0.1   K Hoang      27/11/2019 No v1.0.1. Bump up to 1.0.2 to match ESP8266_ISR_TimerInterupt library
    1.0.2   K.Hoang      03/12/2019 Permit up to 16 super-long-time, super-accurate ISR-based timers to avoid being blocked
    1.0.3   K.Hoang      17/05/2020 Restructure code. Add examples. Enhance README.
    1.1.0   K.Hoang      27/10/2020 Restore cpp code besides Impl.h code to use if Multiple-Definition linker error.
    1.1.1   K.Hoang      06/12/2020 Add Version String and Change_Interval example to show how to change TimerInterval
*****************************************************************************************************************************/

/*
   Notes:
   Special design is necessary to share data between interrupt code and the rest of your program.
   Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
   variable can not spontaneously change. Because your function may change variables while your program is using them,
   the compiler needs this hint. But volatile alone is often not enough.
   When accessing shared variables, usually interrupts must be disabled. Even with volatile,
   if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
   If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
   or the entire sequence of your code which accesses the data.
*/

#ifndef ESP32
  #error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP32TimerInterrupt.h"
// Don't define TIMER_INTERRUPT_DEBUG > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG      0

#include "ESP32TimerInterrupt.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN       2         // Pin D2 mapped to pin GPIO2/ADC12 of ESP32, control on-board LED
#endif

#define PIN_D23             23        // Pin D23 mapped to pin GPIO23/VSPI_MOSI of ESP32

volatile uint32_t Timer0Count = 0;
volatile uint32_t Timer1Count = 0;

void IRAM_ATTR TimerHandler0(void)
{
  static bool toggle0 = false;

  // Flag for checking to be sure ISR is working as Serial.print is not OK here in ISR
  Timer0Count++;

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

  // Flag for checking to be sure ISR is working as Serial.print is not OK here in ISR
  Timer1Count++;

#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.println("ITimer1: millis() = " + String(millis()));
#endif

  //timer interrupt toggles PIN_D23
  digitalWrite(PIN_D23, toggle1);
  toggle1 = !toggle1;
}

void printResult(uint32_t currTime)
{
  Serial.printf("Time = %ld, Timer0Count = %lu, , Timer1Count = %lu\n", currTime, Timer0Count, Timer1Count);
}

#define TIMER0_INTERVAL_MS        500

#define TIMER1_INTERVAL_MS        1000

// Init ESP32 timer 0
ESP32Timer ITimer0(0);
ESP32Timer ITimer1(1);

void setup()
{
  pinMode(LED_BUILTIN,  OUTPUT);
  pinMode(PIN_D23,      OUTPUT);
  
  Serial.begin(115200);
  while (!Serial);

  delay(100);

  Serial.println("\nStarting Change_Interval on " + String(ARDUINO_BOARD));
  Serial.println(ESP32_TIMER_INTERRUPT_VERSION);
  Serial.printf("CPU Frequency = %ld MHz\n", F_CPU / 1000000);

  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
  {
    Serial.printf("Starting  ITimer0 OK, millis() = %ld\n", millis());
  }
  else
    Serial.println("Can't set ITimer0. Select another freq. or timer");

  // Interval in microsecs
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
  {
    Serial.printf("Starting  ITimer1 OK, millis() = %ld\n", millis());
  }
  else
    Serial.println("Can't set ITimer1. Select another freq. or timer");
}

#define CHECK_INTERVAL_MS     10000L
#define CHANGE_INTERVAL_MS    20000L

void loop()
{
  static uint32_t lastTime = 0;
  static uint32_t lastChangeTime = 0;
  static uint32_t currTime;
  static uint32_t multFactor = 0;

  currTime = millis();

  if (currTime - lastTime > CHECK_INTERVAL_MS)
  {
    printResult(currTime);
    lastTime = currTime;

    if (currTime - lastChangeTime > CHANGE_INTERVAL_MS)
    {
      //setInterval(unsigned long interval, timerCallback callback)
      multFactor = (multFactor + 1) % 2;
      
      ITimer0.setInterval(TIMER0_INTERVAL_MS * 1000 * (multFactor + 1), TimerHandler0);
      ITimer1.setInterval(TIMER1_INTERVAL_MS * 1000 * (multFactor + 1), TimerHandler1);

      Serial.printf("Changing Interval, Timer0 = %lu,  Timer1 = %lu\n", TIMER0_INTERVAL_MS * (multFactor + 1), TIMER1_INTERVAL_MS * (multFactor + 1));
      
      lastChangeTime = currTime;
    }
  }
}
