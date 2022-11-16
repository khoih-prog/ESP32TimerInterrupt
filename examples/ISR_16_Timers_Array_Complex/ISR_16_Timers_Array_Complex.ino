/****************************************************************************************************************************
  ISR_16_Timers_Array_Complex.ino
  For ESP32, ESP32_S2, ESP32_S3, ESP32_C3 boards with ESP32 core v2.0.2+
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32TimerInterrupt
  Licensed under MIT license

  The ESP32, ESP32_S2, ESP32_S3, ESP32_C3 have two timer groups, TIMER_GROUP_0 and TIMER_GROUP_1
  1) each group of ESP32, ESP32_S2, ESP32_S3 has two general purpose hardware timers, TIMER_0 and TIMER_1
  2) each group of ESP32_C3 has ony one general purpose hardware timer, TIMER_0

  All the timers are based on 64-bit counters (except 54-bit counter for ESP32_S3 counter) and 16 bit prescalers.
  The timer counters can be configured to count up or down and support automatic reload and software reload.
  They can also generate alarms when they reach a specific value, defined by the software.
  The value of the counter can be read by the software program.

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one ESP32-S2 timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.
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

   This example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs.
   Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet
   and Blynk services. You can also have many (up to 16) timers to use.
   This non-being-blocked important feature is absolutely necessary for mission-critical tasks.
   You'll see blynkTimer is blocked while connecting to WiFi / Internet / Blynk, and elapsed time is very unaccurate
   In this super simple example, you don't see much different after Blynk is connected, because of no competing task is
   written
*/

#if !defined( ESP32 )
	#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP32TimerInterrupt.h"
#define _TIMERINTERRUPT_LOGLEVEL_     4

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ESP32TimerInterrupt.h"

#include <SimpleTimer.h>              // https://github.com/jfturcot/SimpleTimer

// Don't use PIN_D1 in core v2.0.0 and v2.0.1. Check https://github.com/espressif/arduino-esp32/issues/5868

#ifndef LED_BLUE
	#define LED_BLUE          25
#endif

#ifndef LED_RED
	#define LED_RED           27
#endif

// Don't use PIN_D1 in core v2.0.0 and v2.0.1. Check https://github.com/espressif/arduino-esp32/issues/5868
// Don't use PIN_D2 with ESP32_C3 (crash)
#define PIN_D19             19        // Pin D19 mapped to pin GPIO9 of ESP32
#define PIN_D3               3        // Pin D3 mapped to pin GPIO3/RX0 of ESP32

#define HW_TIMER_INTERVAL_US      10000L

volatile uint32_t startMillis = 0;

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

		//timer interrupt toggles pin PIN_D19
		digitalWrite(PIN_D19, toggle);
		toggle = !toggle;
	}

	return true;
}

/////////////////////////////////////////////////

#define NUMBER_ISR_TIMERS         16

typedef void (*irqCallback)  ();

/////////////////////////////////////////////////

#define USE_COMPLEX_STRUCT      true

#if USE_COMPLEX_STRUCT

typedef struct
{
	irqCallback   irqCallbackFunc;
	uint32_t      TimerInterval;
	unsigned long deltaMillis;
	unsigned long previousMillis;
} ISRTimerData;

// In ESP32, avoid doing something fancy in ISR, for example Serial.print()
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash

void doingSomething(int index);

#else

volatile unsigned long deltaMillis    [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile unsigned long previousMillis [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// You can assign any interval for any timer here, in milliseconds
uint32_t TimerInterval[NUMBER_ISR_TIMERS] =
{
	5000L,  10000L,  15000L,  20000L,  25000L,  30000L,  35000L,  40000L,
	45000L, 50000L,  55000L,  60000L,  65000L,  70000L,  75000L,  80000L
};

void doingSomething(int index)
{
	unsigned long currentMillis  = millis();

	deltaMillis[index]    = currentMillis - previousMillis[index];
	previousMillis[index] = currentMillis;
}

#endif

////////////////////////////////////
// Shared
////////////////////////////////////

void doingSomething0()
{
	doingSomething(0);
}

void doingSomething1()
{
	doingSomething(1);
}

void doingSomething2()
{
	doingSomething(2);
}

void doingSomething3()
{
	doingSomething(3);
}

void doingSomething4()
{
	doingSomething(4);
}

void doingSomething5()
{
	doingSomething(5);
}

void doingSomething6()
{
	doingSomething(6);
}

void doingSomething7()
{
	doingSomething(7);
}

void doingSomething8()
{
	doingSomething(8);
}

void doingSomething9()
{
	doingSomething(9);
}

void doingSomething10()
{
	doingSomething(10);
}

void doingSomething11()
{
	doingSomething(11);
}

void doingSomething12()
{
	doingSomething(12);
}

void doingSomething13()
{
	doingSomething(13);
}

void doingSomething14()
{
	doingSomething(14);
}

void doingSomething15()
{
	doingSomething(15);
}

#if USE_COMPLEX_STRUCT

ISRTimerData curISRTimerData[NUMBER_ISR_TIMERS] =
{
	//irqCallbackFunc, TimerInterval, deltaMillis, previousMillis
	{ doingSomething0,    5000L, 0, 0 },
	{ doingSomething1,   10000L, 0, 0 },
	{ doingSomething2,   15000L, 0, 0 },
	{ doingSomething3,   20000L, 0, 0 },
	{ doingSomething4,   25000L, 0, 0 },
	{ doingSomething5,   30000L, 0, 0 },
	{ doingSomething6,   35000L, 0, 0 },
	{ doingSomething7,   40000L, 0, 0 },
	{ doingSomething8,   45000L, 0, 0 },
	{ doingSomething9,   50000L, 0, 0 },
	{ doingSomething10,  55000L, 0, 0 },
	{ doingSomething11,  60000L, 0, 0 },
	{ doingSomething12,  65000L, 0, 0 },
	{ doingSomething13,  70000L, 0, 0 },
	{ doingSomething14,  75000L, 0, 0 },
	{ doingSomething15,  80000L, 0, 0 }
};

void doingSomething(int index)
{
	unsigned long currentMillis  = millis();

	curISRTimerData[index].deltaMillis    = currentMillis - curISRTimerData[index].previousMillis;
	curISRTimerData[index].previousMillis = currentMillis;
}

#else

irqCallback irqCallbackFunc[NUMBER_ISR_TIMERS] =
{
	doingSomething0,  doingSomething1,  doingSomething2,  doingSomething3,
	doingSomething4,  doingSomething5,  doingSomething6,  doingSomething7,
	doingSomething8,  doingSomething9,  doingSomething10, doingSomething11,
	doingSomething12, doingSomething13, doingSomething14, doingSomething15
};

#endif
///////////////////////////////////////////

#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerDoingSomething2s()
{
	static unsigned long previousMillis = startMillis;

	unsigned long currMillis = millis();

	Serial.print(F("SimpleTimer : "));
	Serial.print(SIMPLE_TIMER_MS / 1000);
	Serial.print(F(", ms : "));
	Serial.print(currMillis);
	Serial.print(F(", Dms : "));
	Serial.println(currMillis - previousMillis);

	for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
	{
#if USE_COMPLEX_STRUCT
		Serial.print(F("Timer : "));
		Serial.print(i);
		Serial.print(F(", programmed : "));
		Serial.print(curISRTimerData[i].TimerInterval);
		Serial.print(F(", actual : "));
		Serial.println(curISRTimerData[i].deltaMillis);
#else
		Serial.print(F("Timer : "));
		Serial.print(i);
		Serial.print(F(", programmed : "));
		Serial.print(TimerInterval[i]);
		Serial.print(F(", actual : "));
		Serial.println(deltaMillis[i]);
#endif
	}

	previousMillis = currMillis;
}

void setup()
{
	pinMode(PIN_D19, OUTPUT);

	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting ISR_16_Timers_Array_Complex on "));
	Serial.println(ARDUINO_BOARD);
	Serial.println(ESP32_TIMER_INTERRUPT_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	// Interval in microsecs
	if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler))
	{
		startMillis = millis();
		Serial.print(F("Starting ITimer OK, millis() = "));
		Serial.println(startMillis);
	}
	else
		Serial.println(F("Can't set ITimer. Select another freq. or timer"));

	startMillis = millis();

	// Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
	// You can use up to 16 timer for each ISR_Timer
	for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
	{
#if USE_COMPLEX_STRUCT
		curISRTimerData[i].previousMillis = startMillis;
		ISR_Timer.setInterval(curISRTimerData[i].TimerInterval, curISRTimerData[i].irqCallbackFunc);
#else
		previousMillis[i] = millis();
		ISR_Timer.setInterval(TimerInterval[i], irqCallbackFunc[i]);
#endif
	}

	// You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
	simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
	// This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
	// You see the time elapse of ISR_Timer still accurate, whereas very unaccurate for Software Timer
	// The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
	// While that of ISR_Timer is still prefect.
	delay(BLOCKING_TIME_MS);

	// You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
	// You don't need to and never call ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
	simpleTimer.run();
}
