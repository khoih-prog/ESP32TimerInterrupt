/****************************************************************************************************************************
  multiFileProject.ino
  
  For ESP32, ESP32_S2, ESP32_S3, ESP32_C3 boards with ESP32 core v2.0.2+
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32TimerInterrupt
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#if !defined( ESP32 )
  #error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting. 
#endif

#define ESP32_TIMER_INTERRUPT_VERSION_MIN_TARGET      "ESP32_PWM v1.2.2"
#define ESP32_TIMER_INTERRUPT_VERSION_MIN             1002002

// These define's must be placed at the beginning before #include "ESP32_PWM.h"
// _PWM_LOGLEVEL_ from 0 to 4
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_                4

#include "multiFileProject.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ESP32TimerInterrupt.h"

void setup() 
{
  Serial.begin(115200);
  while (!Serial);

  delay(300);
  
  Serial.println("\nStart multiFileProject");
  Serial.println(ESP32_TIMER_INTERRUPT_VERSION);

#if defined(ESP32_TIMER_INTERRUPT_VERSION_MIN)
  if (ESP32_TIMER_INTERRUPT_VERSION_INT < ESP32_TIMER_INTERRUPT_VERSION_MIN)
  {
    Serial.print("Warning. Must use this example on Version equal or later than : ");
    Serial.println(ESP32_TIMER_INTERRUPT_VERSION_MIN_TARGET);
  }
#endif
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
