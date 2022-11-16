# ESP32TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP32TimerInterrupt.svg)](https://github.com/khoih-prog/ESP32TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP32TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP32TimerInterrupt.svg)](http://github.com/khoih-prog/ESP32TimerInterrupt/issues)


<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>
<a href="https://profile-counter.glitch.me/khoih-prog/count.svg" title="Total khoih-prog Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog/count.svg" style="height: 30px;width: 200px;"></a>
<a href="https://profile-counter.glitch.me/khoih-prog-ESP32TimerInterrupt/count.svg" title="ESP32TimerInterrupt Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog-ESP32TimerInterrupt/count.svg" style="height: 30px;width: 200px;"></a>

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v2.3.0](#releases-v230)
  * [Releases v2.2.0](#releases-v220)
  * [Releases v2.1.0](#releases-v210)
  * [Releases v2.0.2](#releases-v202)
  * [Releases v2.0.1](#releases-v201)
  * [Releases v2.0.0](#releases-v200)
  * [Releases v1.5.0](#releases-v150)
  * [Releases v1.4.1](#releases-v141)
  * [Releases v1.4.0](#releases-v140)
  * [Releases v1.3.0](#releases-v130)
  * [Releases v1.2.0](#releases-v120)
  * [Releases v1.1.1](#releases-v111)
  * [Releases v1.1.0](#releases-v110)
  * [Releases v1.0.3](#releases-v103)
  * [Releases v1.0.2](#releases-v102)

---
---

## Changelog

### Releases v2.3.0

1. Fix doubled time for ESP32_C3, ESP32_S2 and ESP32_S3. Check [Error in the value defined by TIMER0_INTERVAL_MS #28](https://github.com/khoih-prog/ESP32TimerInterrupt/issues/28)
2. Modify examples to avoid using `LED_BUILTIN` / `GPIO2` as it cam cause crash in some boards, such as `ESP32_C3`
3. Use `allman astyle` and add `utils`

### Releases v2.2.0

1. Suppress warnings for ESP32_C3, ESP32_S2 and ESP32_S3
2. Add support to 
  - ESP32_S2 : ESP32S2 Native USB, UM FeatherS2 Neo, UM TinyS2, UM RMP, microS2, LOLIN_S2_MINI, LOLIN_S2_PICO, ADAFRUIT_FEATHER_ESP32S2, ADAFRUIT_FEATHER_ESP32S2_TFT, ATMegaZero ESP32-S2, Deneyap Mini, FRANZININHO_WIFI, FRANZININHO_WIFI_MSC
  - ESP32_S3 : UM TinyS3, UM PROS3, UM FeatherS3, ESP32_S3_USB_OTG, ESP32S3_CAM_LCD, DFROBOT_FIREBEETLE_2_ESP32S3, ADAFRUIT_FEATHER_ESP32S3_TFT
  - ESP32_C3 : LOLIN_C3_MINI, DFROBOT_BEETLE_ESP32_C3, ADAFRUIT_QTPY_ESP32C3, AirM2M_CORE_ESP32C3, XIAO_ESP32C3

### Releases v2.1.0

1. Suppress errors and warnings for new ESP32 core v2.0.4+

### Releases v2.0.2

1. Add support to new Adafruit boards such as QTPY_ESP32S2, FEATHER_ESP32S3_NOPSRAM and QTPY_ESP32S3_NOPSRAM

### Releases v2.0.1

1. Add example [ISR_16_Timers_Array_Complex_OneShot](examples/ISR_16_Timers_Array_Complex_OneShot) to demo how to use `one-shot ISR-based timer` in complex case
2. Add example [ISR_16_Timers_Array_OneShot](examples/ISR_16_Timers_Array_OneShot) to demo how to use `one-shot ISR-based timer`
3. Modify example [multiFileProject](examples/multiFileProject) to demo for more complex multiple-file project
4. Further optimize code by using passing by `reference` instead of by `value`

### Releases v2.0.0

1. Restructure library.
2. Add support to new ESP32_S3. Now supporting ESP32, ESP32_S2, ESP32_S3 and ESP32_C3
3. Optimize library code by using `reference-passing` instead of `value-passing`
4. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project

### Releases v1.5.0

1. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories.

### Releases v1.4.1

1. Avoid using D1 in examples due to issue with core v2.0.0 and v2.0.1. Check [ESP32 Core v2.0.1 / 2.0.1 RC1 crashes if using pinMode with GPIO1 #5868](https://github.com/espressif/arduino-esp32/issues/5868)
2. Add changelog.md

### Releases v1.4.0

1. Fix compiler errors due to conflict to some libraries.
2. Add complex examples.


### Releases v1.3.0

1. Add support to ESP32-S2 and ESP32-C3
2. Add support to new ESP32 core v1.0.6
3. Update examples

### Releases v1.2.0

1. Add better debug feature.
2. Optimize code and examples to reduce RAM usage

### Releases v1.1.1

1. Add [**Change_Interval**](examples/Change_Interval) example to show how to change TimerInterval on-the-fly
2. Add Version String and Change_Interval example to show how to change TimerInterval

### Releases v1.1.0

1. Restore cpp code besides Impl.h code to use if Multiple-Definition linker error.
2. Update examples.
3. Enhance README.


### Releases v1.0.3

1. Restructure code.
2. Add examples.
3. Enhance README.

### Releases v1.0.2

1. Permit up to 16 super-long-time, super-accurate ISR-based timers to avoid being blocked



