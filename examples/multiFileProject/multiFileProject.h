/****************************************************************************************************************************
  multiFileProject.h
  
  For ESP32, ESP32_S2, ESP32_S3, ESP32_C3 boards with ESP32 core v2.0.2+
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32TimerInterrupt
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#pragma once

#define USING_MICROS_RESOLUTION       true    //false

// Default is true, uncomment to false
//#define CHANGING_PWM_END_OF_CYCLE     false 

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "ESP32TimerInterrupt.hpp"
