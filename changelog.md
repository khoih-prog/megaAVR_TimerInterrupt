## megaAVR_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/megaAVR_TimerInterrupt.svg?)](https://www.ardu-badge.com/megaAVR_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/megaAVR_TimerInterrupt.svg)](https://github.com/khoih-prog/megaAVR_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/megaAVR_TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/megaAVR_TimerInterrupt.svg)](http://github.com/khoih-prog/megaAVR_TimerInterrupt/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>
<a href="https://profile-counter.glitch.me/khoih-prog/count.svg" title="Total Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog/count.svg" style="height: 30px;width: 200px;"></a>
<a href="https://profile-counter.glitch.me/khoih-prog-megaAVR_TimerInterrupt/count.svg" title="Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog-megaAVR_TimerInterrupt/count.svg" style="height: 30px;width: 200px;"></a>

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Release v1.7.0](#release-v170)
  * [Release v1.6.1](#release-v161)
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

### Release v1.7.0

1. Fix bug disabling `TCB0`. Check [TimerB0 does not trigger interrupt #4](https://github.com/khoih-prog/megaAVR_TimerInterrupt/issues/4)
2. Use `allman astyle` and add `utils`

### Release v1.6.1

1. Suppress warnings when `_TIMERINTERRUPT_LOGLEVEL_` < 2
2. Remove unused `_prescalerIndex`

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

1. Initial release to support to ATmega4809-based boards, such as **Arduino UNO WiFi Rev2, AVR_NANO_EVERY, etc.**
2. New examples to support Blynk using [**WiFiNINA_Generic**](https://github.com/khoih-prog/WiFiNINA_Generic) and [**Blynk_WiFiNINA_WM**](https://github.com/khoih-prog/Blynk_WiFiNINA_WM) libraries.


