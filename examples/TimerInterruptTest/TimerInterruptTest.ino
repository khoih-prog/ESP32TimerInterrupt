/************************************************
 * TimerInterruptTest.ino
 * For ESP32 boards
 * Written by Khoi Hoang
 * 
 * Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt
 * Licensed under MIT license
 * Version: v1.0.0
 * 
 * Notes:
 * Special design is necessary to share data between interrupt code and the rest of your program.
 * Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume 
 * variable can not spontaneously change. Because your function may change variables while your program is using them, 
 * the compiler needs this hint. But volatile alone is often not enough.
 * When accessing shared variables, usually interrupts must be disabled. Even with volatile, 
 * if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly. 
 * If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled 
 * or the entire sequence of your code which accesses the data.
 *
 ************************************************/
//These define's must be placed at the beginning before #include "TimerInterrupt.h"
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
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
#define TIMER0_DURATION_MS        5000

#define TIMER1_INTERVAL_MS        3000
#define TIMER1_DURATION_MS        15000

// Init ESP32 timer 0
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
  static unsigned long lastTimer0 = 0;
  static unsigned long lastTimer1 = 0;
  
  static bool timer0Stopped         = false;
  static bool timer1Stopped         = false;
  
  if (millis() - lastTimer0 > TIMER0_DURATION_MS)  
  {
    lastTimer0 = millis();
   
    if (timer0Stopped)
    {      
      Serial.println("Start ITimer0, millis() = " + String(millis()));
      ITimer0.restartTimer();
    }
    else
    {
      Serial.println("Stop ITimer0, millis() = " + String(millis()));
      ITimer0.stopTimer();
    }
    timer0Stopped = !timer0Stopped;
  }

  if (millis() - lastTimer1 > TIMER1_DURATION_MS)  
  {
    lastTimer1 = millis();
   
    if (timer1Stopped)
    {      
      Serial.println("Start ITimer1, millis() = " + String(millis()));
      ITimer1.restartTimer();
    }
    else
    {
      Serial.println("Stop ITimer1, millis() = " + String(millis()));
      ITimer1.stopTimer();
    }
    timer1Stopped = !timer1Stopped;
  }
}
