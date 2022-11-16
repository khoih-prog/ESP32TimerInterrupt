/****************************************************************************************************************************
  multiFileProject.ino
  For ESP32, ESP32_S2, ESP32_S3, ESP32_C3 boards with ESP32 core v2.0.2+
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32TimerInterrupt
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#if !defined( ESP32 )
	#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#define ESP32_TIMER_INTERRUPT_VERSION_MIN_TARGET      "ESP32TimerInterrupt v2.3.0"
#define ESP32_TIMER_INTERRUPT_VERSION_MIN             2003000

#include "multiFileProject.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ESP32TimerInterrupt.h"
#include "ESP32_ISR_Timer.h"

void doingSomething1()
{
	// Replace or comment out Serial.println() if crashed
	Serial.println("doingSomething1 triggered");
}

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

	delay(500);

	Serial.println("\nStart multiFileProject");
	Serial.println(ESP32_TIMER_INTERRUPT_VERSION);

#if defined(ESP32_TIMER_INTERRUPT_VERSION_MIN)

	if (ESP32_TIMER_INTERRUPT_VERSION_INT < ESP32_TIMER_INTERRUPT_VERSION_MIN)
	{
		Serial.print("Warning. Must use this example on Version equal or later than : ");
		Serial.println(ESP32_TIMER_INTERRUPT_VERSION_MIN_TARGET);
	}

#endif

	setupISR();  // in multifileProject.cpp

	ISR_Timer.setTimeout(5000, doingSomething1);
}

void loop()
{
	// put your main code here, to run repeatedly:
	delay(1);
}
