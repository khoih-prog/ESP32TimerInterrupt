/****************************************************************************************************************************
  multiFileProject.cpp
  For ESP32, ESP32_S2, ESP32_S3, ESP32_C3 boards with ESP32 core v2.0.2+
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32TimerInterrupt
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#include "multiFileProject.h"

#define HW_TIMER_INTERVAL_US      1000000L

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

void doingSomething0()
{
	// Replace or comment out Serial.println() if crashed
	Serial.println("doingSomething0 triggered");
}

void setupISR()
{
	if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler))
	{
		Serial.println(F("Starting ITimer OK"));
	}
	else
		Serial.println(F("Can't set ITimer. Select another freq. or timer"));

	ISR_Timer.setInterval(3000, doingSomething0);
}
