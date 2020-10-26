/****************************************************************************************************************************
   ESP32_ISR_Timer.h
   For ESP32 boards
   Written by Khoi Hoang

   Built by Khoi Hoang https://github.com/khoih-prog/ESP8266TimerInterrupt
   Licensed under MIT license
   Version: 1.0.3

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

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
    1.0.0   K Hoang      23/11/2019 Initial coding
    1.0.1   K Hoang      27/11/2019 No v1.0.1. Bump up to 1.0.2 to match ESP8266_ISR_TimerInterupt library
    1.0.2   K.Hoang      03/12/2019 Permit up to 16 super-long-time, super-accurate ISR-based timers to avoid being blocked
    1.0.3   K.Hoang      17/05/2020 Restructure code. Add examples. Enhance README.
*****************************************************************************************************************************/

#ifndef ESP32_ISR_TIMER_H
#define ESP32_ISR_TIMER_H

#ifndef ESP32
#error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif

#define CONFIG_ESP32_APPTRACE_ENABLE

#if 0
#ifndef configMINIMAL_STACK_SIZE
#define configMINIMAL_STACK_SIZE    2048
#else
#undef configMINIMAL_STACK_SIZE
#define configMINIMAL_STACK_SIZE    2048
#endif
#endif

#include <stddef.h>

#include <inttypes.h>

#if defined(ARDUINO)
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#endif

#define ESP32_ISR_Timer ESP32_ISRTimer

typedef void (*timer_callback)(void);
typedef void (*timer_callback_p)(void *);

class ESP32_ISR_Timer 
{

  public:
    // maximum number of timers
#define MAX_TIMERS        16
    //const static int MAX_TIMERS = 16;

    // setTimer() constants
#define RUN_FOREVER       0
#define RUN_ONCE          1
    //const static int RUN_FOREVER = 0;
    //const static int RUN_ONCE = 1;

    // constructor
    ESP32_ISR_Timer();

    void /*IRAM_ATTR*/ init();

    // this function must be called inside loop()
    void IRAM_ATTR run();

    // Timer will call function 'f' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int /*IRAM_ATTR*/ setInterval(unsigned long d, timer_callback f);

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int /*IRAM_ATTR*/ setInterval(unsigned long d, timer_callback_p f, void* p);

    // Timer will call function 'f' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int /*IRAM_ATTR*/ setTimeout(unsigned long d, timer_callback f);

    // Timer will call function 'f' with parameter 'p' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int /*IRAM_ATTR*/ setTimeout(unsigned long d, timer_callback_p f, void* p);

    // Timer will call function 'f' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int /*IRAM_ATTR*/ setTimer(unsigned long d, timer_callback f, unsigned n);

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int /*IRAM_ATTR*/ setTimer(unsigned long d, timer_callback_p f, void* p, unsigned n);

    // updates interval of the specified timer
    bool /*IRAM_ATTR*/ changeInterval(unsigned numTimer, unsigned long d);

    // destroy the specified timer
    void /*IRAM_ATTR*/ deleteTimer(unsigned numTimer);

    // restart the specified timer
    void /*IRAM_ATTR*/ restartTimer(unsigned numTimer);

    // returns true if the specified timer is enabled
    bool /*IRAM_ATTR*/ isEnabled(unsigned numTimer);

    // enables the specified timer
    void /*IRAM_ATTR*/ enable(unsigned numTimer);

    // disables the specified timer
    void /*IRAM_ATTR*/ disable(unsigned numTimer);

    // enables all timers
    void /*IRAM_ATTR*/ enableAll();

    // disables all timers
    void /*IRAM_ATTR*/ disableAll();

    // enables the specified timer if it's currently disabled,
    // and vice-versa
    void /*IRAM_ATTR*/ toggle(unsigned numTimer);

    // returns the number of used timers
    unsigned /*IRAM_ATTR*/ getNumTimers();

    // returns the number of available timers
    unsigned /*IRAM_ATTR*/ getNumAvailableTimers() 
    {
      return MAX_TIMERS - numTimers;
    };

  private:
    // deferred call constants
#define DEFCALL_DONTRUN   0       // don't call the callback function
#define DEFCALL_RUNONLY   1       // call the callback function but don't delete the timer
#define DEFCALL_RUNANDDEL 2     // call the callback function and delete the timer

    // low level function to initialize and enable a new timer
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int /*IRAM_ATTR*/ setupTimer(unsigned long d, void* f, void* p, bool h, unsigned n);

    // find the first available slot
    int /*IRAM_ATTR*/ findFirstFreeSlot();

    typedef struct 
    {
      unsigned long prev_millis;        // value returned by the millis() function in the previous run() call
      void* callback;                   // pointer to the callback function
      void* param;                      // function parameter
      bool hasParam;                 // true if callback takes a parameter
      unsigned long delay;              // delay value
      unsigned maxNumRuns;              // number of runs to be executed
      unsigned numRuns;                 // number of executed runs
      bool enabled;                  // true if enabled
      unsigned toBeCalled;              // deferred function call (sort of) - N.B.: only used in run()
    } timer_t;

    volatile timer_t timer[MAX_TIMERS];

    // actual number of timers in use (-1 means uninitialized)
    volatile int numTimers;

    // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

};


//#include "ESP32_ISR_Timer-Impl.h"


#endif
