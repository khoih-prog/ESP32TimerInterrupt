/************************************************
 * SwitchDebounce.ino
 * For ESP32 boards
 * Written by Khoi Hoang
 * 
 * Built by Khoi Hoang https://github.com/khoih-prog/ESP32TimerInterrupt
 * Licensed under MIT license
 * Version: v1.0.2
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
/* Switch Debouncing uses high frequency hardware timer 50Hz == 20ms) to measure the time from the SW is pressed, 
 * debouncing time is 100ms => SW is considered pressed if timer count is > 5, then call / flag SW is pressed
 * When the SW is released, timer will count (debounce) until more than 50ms until consider SW is released.
 * We can set to flag or call a function whenever SW is pressed more than certain predetermined time, even before 
 * SW is released.
*/

//These define's must be placed at the beginning before #include "TimerInterrupt.h"
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG      1

#include "ESP32TimerInterrupt.h"

#define PIN_D23           23        // Pin D23 mapped to pin GPIO23/VSPI_MOSI of ESP32

unsigned int SWPin = PIN_D23;

#define TIMER1_INTERVAL_MS        20
#define DEBOUNCING_INTERVAL_MS    100
#define LONG_PRESS_INTERVAL_MS    5000

#define LOCAL_DEBUG      1

// Init ESP32 timer 0
ESP32Timer ITimer1(1);

volatile bool SWPressed     = false;
volatile bool SWLongPressed = false;

void IRAM_ATTR TimerHandler1()
{
  static unsigned int debounceCountSWPressed  = 0;
  static unsigned int debounceCountSWReleased = 0;

  static unsigned long SWPressedTime;
  static unsigned long SWReleasedTime;
  
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(SWPin, INPUT_PULLUP);
  }

  if ( (!digitalRead(SWPin)) )
  {
    // Start debouncing counting debounceCountSWPressed and clear debounceCountSWReleased     
    debounceCountSWReleased = 0;
    
    if (++debounceCountSWPressed >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS)
    {
      // Call and flag SWPressed
      if (!SWPressed)
      {
        SWPressedTime = millis();
        
        #if (LOCAL_DEBUG > 0)
        Serial.println("SW Press, from millis() = " + String(SWPressedTime - DEBOUNCING_INTERVAL_MS));
        #endif
        
        SWPressed = true;
        // Do something for SWPressed here in ISR
        // But it's better to use outside software timer to do your job instead of inside ISR
        //Your_Response_To_Press();
      }
      
      if (debounceCountSWPressed >= LONG_PRESS_INTERVAL_MS / TIMER1_INTERVAL_MS)
      {
        // Call and flag SWLongPressed
        if (!SWLongPressed)
        {         
          #if (LOCAL_DEBUG > 0)
          Serial.println("SW Long Pressed, total time ms = " + String(millis()) + " - " + String(SWPressedTime- DEBOUNCING_INTERVAL_MS) 
                          + " = " + String(millis() - SWPressedTime + DEBOUNCING_INTERVAL_MS) );
          #endif
                          
          SWLongPressed = true;
          // Do something for SWLongPressed here in ISR
          // But it's better to use outside software timer to do your job instead of inside ISR
          //Your_Response_To_Long_Press();
        }
      }     
    }   
  }
  else
  {
    // Start debouncing counting debounceCountSWReleased and clear debounceCountSWPressed
    if ( SWPressed && (++debounceCountSWReleased >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS))
    {
      SWReleasedTime = millis();
      
      // Call and flag SWPressed 
      #if (LOCAL_DEBUG > 0)
      Serial.println("SW Released, from millis() = " + String(SWReleasedTime));
      #endif
      
      SWPressed     = false;
      SWLongPressed = false;

      // Do something for !SWPressed here in ISR
      // But it's better to use outside software timer to do your job instead of inside ISR
      //Your_Response_To_Release();

      // Call and flag SWPressed 
      #if (LOCAL_DEBUG > 0)
      Serial.println("SW Pressed total time ms = " + String(SWReleasedTime - SWPressedTime));
      #endif
      
      debounceCountSWPressed = 0; 
    } 
  }
}


void setup()
{
  Serial.begin(115200);
  Serial.println("\nStarting");

  // Using ESP32  => 80 / 160 / 240MHz CPU clock , 
  // For 64-bit timer counter
  // For 16-bit timer prescaler up to 1024

  // Interval in microsecs
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
    Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer1. Select another freq., duration or timer");
}

void loop()
{
  
}
