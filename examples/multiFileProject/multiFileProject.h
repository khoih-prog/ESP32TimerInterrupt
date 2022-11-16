/****************************************************************************************************************************
  multiFileProject.h
  For ESP32, ESP32_S2, ESP32_S3, ESP32_C3 boards with ESP32 core v2.0.2+
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32TimerInterrupt
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#pragma once

// These define's must be placed at the beginning before #include "ESP8266TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         2
#define _TIMERINTERRUPT_LOGLEVEL_     0

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "ESP32TimerInterrupt.hpp"
#include "ESP32_ISR_Timer.hpp"

#ifndef LED_BUILTIN
	#define LED_BUILTIN       2
#endif

extern ESP32_ISR_Timer ISR_Timer;  // declaration of the global variable ISRTimer

void setupISR();
