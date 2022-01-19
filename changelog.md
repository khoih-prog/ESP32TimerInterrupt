# ESP32TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP32TimerInterrupt.svg)](https://github.com/khoih-prog/ESP32TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP32TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP32TimerInterrupt.svg)](http://github.com/khoih-prog/ESP32TimerInterrupt/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
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



