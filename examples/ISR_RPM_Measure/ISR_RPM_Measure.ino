/************************************************
 * RPM_Measure.ino
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
/* RPM Measuring uses high frequency hardware timer 1Hz == 1ms) to measure the time from of one rotation, in ms
 * then convert to RPM. One rotation is detected by reading the state of a magnetic REED SW or IR LED Sensor
 * Asssuming LOW is active. 
 * For example: Max speed is 600RPM => 10 RPS => minimum 100ms a rotation. We'll use 80ms for debouncing
 * If the time between active state is less than 8ms => consider noise.
 * RPM = 60000 / (rotation time in ms)
 * 
 * We use interrupt to detect whenever the SW is active, set a flag 
 * then use timer to count the time between active state
 */

//These define's must be placed at the beginning before #include "TimerInterrupt.h"
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG      1

#include "ESP32TimerInterrupt.h"

#define PIN_D23           23        // Pin D23 mapped to pin GPIO23/VSPI_MOSI of ESP32

unsigned int interruptPin = PIN_D23;

#define TIMER1_INTERVAL_MS        1
#define DEBOUNCING_INTERVAL_MS    80

#define LOCAL_DEBUG      1

// Init ESP32 timer 1
ESP32Timer ITimer1(1);

volatile unsigned long rotationTime = 0;
float RPM       = 0.00;
float avgRPM    = 0.00;

volatile int debounceCounter;

volatile bool activeState = false;

void IRAM_ATTR detectRotation(void)
{
  activeState = true;
}

void IRAM_ATTR TimerHandler1()
{  
  static bool started = false;

  if ( activeState )
  {
    // Reset to prepare for next round of interrupt
    activeState = false;

    if (debounceCounter >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS )
    {
    
      //min time between pulses has passed
      RPM = (float) ( 60000.0f / ( rotationTime * TIMER1_INTERVAL_MS ) );
  
      avgRPM = ( 2 * avgRPM + RPM) / 3,
      
      Serial.println("RPM = " + String(avgRPM) + ", rotationTime ms = " + String(rotationTime * TIMER1_INTERVAL_MS) );
      
      rotationTime = 0;
      debounceCounter = 0;
    }
    else
      debounceCounter++;
  }
  else
  {
    debounceCounter++;
  }
  
  if (rotationTime >= 5000)
  {
    // If idle, set RPM to 0, don't increase rotationTime
    RPM = 0;
    Serial.println("RPM = " + String(RPM) + ", rotationTime = " + String(rotationTime) );
    rotationTime = 0;
  }
  else
  {
    rotationTime++;
  } 
}


void setup()
{
  Serial.begin(115200);
  Serial.println("\nStarting");

  pinMode(interruptPin, INPUT_PULLUP);  

  // Using ESP32  => 80 / 160 / 240MHz CPU clock , 
  // For 64-bit timer counter
  // For 16-bit timer prescaler up to 1024

  // Interval in microsecs
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1))
    Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer1. Select another freq., duration or timer");

  // Assumming the interruptPin will go LOW
  attachInterrupt(digitalPinToInterrupt(interruptPin), detectRotation, FALLING);  
}

void loop()
{
  
}
