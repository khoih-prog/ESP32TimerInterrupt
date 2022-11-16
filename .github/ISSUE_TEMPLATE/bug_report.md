---
name: Bug report
about: Create a report to help us improve
title: ''
labels: ''
assignees: ''

---

### Describe the bug

A clear and concise description of what the bug is.

### Steps to Reproduce

Steps to reproduce the behavior. Including the [MRE](https://stackoverflow.com/help/minimal-reproducible-example) sketches

### Expected behavior

A clear and concise description of what you expected to happen.

### Actual behavior

A clear and concise description of what you expected to happen.

### Debug and AT-command log (if applicable)

A clear and concise description of what you expected to happen.

### Screenshots

If applicable, add screenshots to help explain your problem.

### Information

Please ensure to specify the following:

* Arduino IDE version (e.g. 1.8.19) or Platform.io version
* `ESP32` Core Version (e.g. ESP32 core v2.0.5)
* `ESP32` Board type (e.g. ESP32_DEV Module, etc.)
* `ESP32-S2` Board type (e.g. ESP32S2_DEV Module, ESP32_S2_Saola, etc.)
* `ESP32_S3` Board type (e.g. ESP32S3_DEV, ESP32_S3_BOX, UM TINYS3, UM PROS3, UM FEATHERS3, etc.)
* `ESP32-C3` Board type (e.g. ESP32C3_DEV Module, LOLIN_C3_MINI, DFROBOT_BEETLE_ESP32_C3, ADAFRUIT_QTPY_ESP32C3, AirM2M_CORE_ESP32C3, XIAO_ESP32C3, etc.)
* Contextual information (e.g. what you were trying to achieve)
* Simplest possible steps to reproduce
* Anything that might be relevant in your opinion, such as:
  * Operating system (Windows, Ubuntu, etc.) and the output of `uname -a`
  * Network configuration


### Example

```
Arduino IDE version: 1.8.19
ESP32 core v2.0.5
ESP32S3_DEV Module
OS: Ubuntu 20.04 LTS
Linux xy-Inspiron-3593 5.15.0-52-generic #58~20.04.1-Ubuntu SMP Thu Oct 13 13:09:46 UTC 2022 x86_64 x86_64 x86_64 GNU/Linux

Context:
I encountered a crash while using this library
Steps to reproduce:
1. ...
2. ...
3. ...
4. ...
```

### Additional context

Add any other context about the problem here.

---

### Sending Feature Requests

Feel free to post feature requests. It's helpful if you can explain exactly why the feature would be useful.

There are usually some outstanding feature requests in the [existing issues list](https://github.com/khoih-prog/ESP32TimerInterrupt/issues?q=is%3Aopen+is%3Aissue+label%3Aenhancement), feel free to add comments to them.

---

### Sending Pull Requests

Pull Requests with changes and fixes are also welcome!

Please use the `astyle` to reformat the updated library code as follows (demo for Ubuntu Linux)

1. Change directory to the library GitHub

```
xy@xy-Inspiron-3593:~$ cd Arduino/xy/ESP32TimerInterrupt_GitHub/
xy@xy-Inspiron-3593:~/Arduino/xy/ESP32TimerInterrupt_GitHub$
```

2. Issue astyle command

```
xy@xy-Inspiron-3593:~/Arduino/xy/ESP32TimerInterrupt_GitHub$ bash utils/restyle.sh
```


