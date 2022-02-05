## megaAVR_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/megaAVR_TimerInterrupt.svg?)](https://www.ardu-badge.com/megaAVR_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/megaAVR_TimerInterrupt.svg)](https://github.com/khoih-prog/megaAVR_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/megaAVR_TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/megaAVR_TimerInterrupt.svg)](http://github.com/khoih-prog/megaAVR_TimerInterrupt/issues)

## Table of Contents

* [Changelog](#changelog)
  * [Release v1.6.0](#release-v160)
  * [Release v1.5.0](#release-v150)
  * [Release v1.4.0](#release-v140)
  * [Release v1.3.0](#release-v130)
  * [Release v1.2.0](#release-v120)
  * [Release v1.1.0](#release-v110)
  * [Initial Release v1.0.0](#initial-release-v100)

---
---

## Changelog

### Release v1.6.0

1. Add support to `MegaCoreX` core, including ATmega4809, ATmega4808, ATmega3209, ATmega3208, ATmega1609, ATmega1608, ATmega809 and ATmega808
2. Fix compiler warning
3. Update all examples

### Release v1.5.0

1. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories
2. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project.
3. Optimize library code by using `reference-passing` instead of `value-passing`
4. Update all examples

### Release v1.4.0

1. Fix TCB Clock bug in high frequencies. Check [Interrupt interval 2X requested interval #1](https://github.com/khoih-prog/megaAVR_TimerInterrupt/issues/1)
2. Add example to demo High Frequency
3. Delete Blynk-related examples
4. Add changelog.md

### Release v1.3.0

1. Fix TCB Clock bug. Don't use v1.2.0


### Release v1.2.0

1. Selectable **TCB Clock 16MHz, 8MHz or 250KHz** depending on necessary accuracy
2. Add BOARD_NAME definition

### Release v1.1.0

1. Fix bug. Don't use v1.0.0

### Initial Release v1.0.0

1. Intial release to support to ATmega4809-based boards, such as **Arduino UNO WiFi Rev2, AVR_NANO_EVERY, etc.**
2. New examples to support Blynk using [**WiFiNINA_Generic**](https://github.com/khoih-prog/WiFiNINA_Generic) and [**Blynk_WiFiNINA_WM**](https://github.com/khoih-prog/Blynk_WiFiNINA_WM) libraries.


