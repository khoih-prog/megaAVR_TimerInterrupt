## megaAVR_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/megaAVR_TimerInterrupt.svg?)](https://www.ardu-badge.com/megaAVR_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/megaAVR_TimerInterrupt.svg)](https://github.com/khoih-prog/megaAVR_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/megaAVR_TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/megaAVR_TimerInterrupt.svg)](http://github.com/khoih-prog/megaAVR_TimerInterrupt/issues)

## Table of Contents

* [Important Change from v1.5.0](#Important-Change-from-v150)
* [Why do we need this megaAVR_TimerInterrupt library](#why-do-we-need-this-megaavr_timerinterrupt-library)
  * [Features](#features)
  * [Why using ISR-based Hardware Timer Interrupt is better](#why-using-isr-based-hardware-timer-interrupt-is-better)
  * [Important Notes about ISR](#important-notes-about-isr)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [More useful Information](#more-useful-information)
  * [1. Documents](#1-documents)
  * [2. Timer TCB0-TCB3](#2-timer-tcb0-tcb3)
* [Usage](#usage)
  * [1. Using only Hardware Timer directly](#1-using-only-hardware-timer-directly)
    * [1.1 Init Hardware Timer](#11-init-hardware-timer)
    * [1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function](#12-set-hardware-timer-interval-and-attach-timer-interrupt-handler-function)
    * [1.3 Set Hardware Timer Frequency and attach Timer Interrupt Handler function](#13-set-hardware-timer-frequency-and-attach-timer-interrupt-handler-function)
  * [2. Using 16 ISR_based Timers from 1 Hardware Timer](#2-using-16-isr_based-timers-from-1-hardware-timer)
    * [2.1 Important Note](#21-important-note)
    * [2.2 Init Hardware Timer and ISR-based Timer](#22-init-hardware-timer-and-isr-based-timer)
    * [2.3 Set Hardware Timer Interval and attach Timer Interrupt Handler functions](#23-set-hardware-timer-interval-and-attach-timer-interrupt-handler-functions)
* [Examples](#examples)
  * [  1. Argument_Complex](examples/Argument_Complex)
  * [  2. Argument_None](examples/Argument_None)
  * [  3. Argument_Simple](examples/Argument_Simple)
  * [  4. Change_Interval](examples/Change_Interval)
  * [  5. FakeAnalogWrite](examples/FakeAnalogWrite)
  * [  6. **ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex).
  * [  7. ISR_RPM_Measure](examples/ISR_RPM_Measure)
  * [  8. **Change_Interval_HF**](examples/Change_Interval_HF)
  * [  9. **ISR_Timers_Array_Simple**](examples/ISR_Timers_Array_Simple).
  * [ 10. RPM_Measure](examples/RPM_Measure)
  * [ 11. SwitchDebounce](examples/SwitchDebounce)
  * [ 12. TimerDuration](examples/TimerDuration)
  * [ 13. TimerInterruptTest](examples/TimerInterruptTest)
  * [ 14. **multiFileProject**](examples/multiFileProject) **New**
* [Example ISR_16_Timers_Array_Complex](#example-isr_16_timers_array_complex)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ISR_16_Timers_Array_Complex on Arduino megaAVR Nano Every](#1-isr_16_timers_array_complex-on-arduino-megaavr-nano-every)
  * [2. Change_Interval on Arduino megaAVR Nano Every](#2-change_interval-on-arduino-megaavr-nano-every)
  * [3. ISR_16_Timers_Array_Complex on Arduino megaAVR Nano Every to show accuracy difference](#3-isr_16_timers_array_complex-on-arduino-megaavr-nano-every-to-show-accuracy-difference)
    * [3.1 TCB Clock Frequency 16MHz for highest accuracy](#31-tcb-clock-frequency-16mhz-for-highest-accuracy)
    * [3.2 TCB Clock Frequency 8MHz for very high accuracy](#32-tcb-clock-frequency-8mhz-for-very-high-accuracy)
    * [3.3 TCB Clock Frequency 250KHz for lower accuracy but longer time](#33-tcb-clock-frequency-250khz-for-lower-accuracy-but-longer-time)
  * [4. Change_Interval on Arduino megaAVR Nano Every](#4-change_interval_HF-on-arduino-megaavr-nano-every)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Important Change from v1.5.0

Please have a look at [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)

### Why do we need this [megaAVR_TimerInterrupt library](https://github.com/khoih-prog/megaAVR_TimerInterrupt)

### Features

This library enables you to use Interrupt from Hardware Timers on an ATmega4809-based boards, such as Arduino megaAVR : UNO WiFi Rev2, AVR_NANO_EVERY, etc.

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based Timers, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based timers**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds) while **the accuracy is nearly perfect** compared to software timers. 

The most important feature is they're ISR-based timers. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_Timer_Complex**](examples/ISR_Timer_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.

Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

### Why using ISR-based Hardware Timer Interrupt is better

Imagine you have a system with a **mission-critical function**, measuring water level and control the sump pump or doing something much more important. You normally use a **software timer to poll**, or even place the function in loop(). But what if another function is blocking the loop() or setup().

**So your function might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a **Hardware Timer with Interrupt** to call your function.

**These hardware timers, using interrupt**, still work even if other functions are blocking. Moreover, they are **much more precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules. More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

### Important Notes about ISR

1. Inside the attached function, delay() won’t work and the value returned by millis() will not increment. Serial data received while in the function may be lost. You should declare as volatile any variables that you modify within the attached function.

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

### Currently supported Boards

- **Arduino UNO WiFi Rev2, AVR_NANO_EVERY, etc.**
- ATmega4809-based boards.

---
---


## Prerequisites

1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
2. [`Arduino megaAVR core 1.8.7+`](https://github.com/arduino/ArduinoCore-megaavr/releases) for Arduino megaAVR boards. Use Arduino Board Manager to install.
3. To use with certain example
   - [`SimpleTimer library`](https://github.com/jfturcot/SimpleTimer) for [ISR_Timers_Array_Simple](examples/ISR_Timers_Array_Simple) and [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) examples.
   
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**megaAVR_TimerInterrupt**](https://github.com/khoih-prog/megaAVR_TimerInterrupt), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/megaAVR_TimerInterrupt.svg?)](https://www.ardu-badge.com/megaAVR_TimerInterrupt) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**megaAVR_TimerInterrupt**](https://github.com/khoih-prog/megaAVR_TimerInterrupt) page.
2. Download the latest release `megaAVR_TimerInterrupt-master.zip`.
3. Extract the zip file to `megaAVR_TimerInterrupt-master` directory 
4. Copy whole `megaAVR_TimerInterrupt-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**megaAVR_TimerInterrupt** library](https://platformio.org/lib/show/12025/megaAVR_TimerInterrupt) or [**megaAVR_TimerInterrupt** library](https://platformio.org/lib/show/12025/megaAVR_TimerInterrupt) by using [Library Manager](https://platformio.org/lib/show/xxxxx/megaAVR_TimerInterrupt/installation). Search for megaAVR_TimerInterrupt in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include these `.hpp` files

```
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "megaAVR_TimerInterrupt.hpp"   //https://github.com/khoih-prog/megaAVR_TimerInterrupt

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "megaAVR_ISR_Timer.hpp"        //https://github.com/khoih-prog/megaAVR_TimerInterrupt
```

in many files. But be sure to use the following `.h` files **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "megaAVR_TimerInterrupt.h"     //https://github.com/khoih-prog/megaAVR_TimerInterrupt

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "megaAVR_ISR_Timer.h"          //https://github.com/khoih-prog/megaAVR_TimerInterrupt
```

Check the new [**multiFileProject** example](examples/multiFileProject) for a `HOWTO` demo.

---
---

## More useful Information

### 1. Documents

1. [Arduino 101: Timers and Interrupts](https://www.robotshop.com/community/forum/t/arduino-101-timers-and-interrupts/13072)
2. [megaAVR0-series-Family-Data-Sheet](http://ww1.microchip.com/downloads/en/DeviceDoc/megaAVR0-series-Family-Data-Sheet-DS40002015B.pdf)

### 2. Timer TCB0-TCB3

TCB0-TCB3 are 16-bit timers.

---
---

## Usage

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

### 1.1 Init Hardware Timer

```
// Select USING_16MHZ     == true for  16MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_8MHZ      == true for   8MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_250KHZ    == true for 250KHz to Timer TCBx => shorter timer, but better accuracy
// Not select for default 250KHz to Timer TCBx => longer timer,  but worse accuracy
#define USING_16MHZ     true
#define USING_8MHZ      false
#define USING_250KHZ    false

// Select the timers you're using, here ITimer1
#define USE_TIMER_0     false
#define USE_TIMER_1     true
#define USE_TIMER_2     false
#define USE_TIMER_3     false

// Init timer ITimer1
ITimer1.init();
```

### 1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function

Use one of these functions with **interval in unsigned long milliseconds**

```
// interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
template<typename TArg> bool setInterval(unsigned long interval, void (*callback)(TArg), TArg params, unsigned long duration = 0);

// interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool setInterval(unsigned long interval, timer_callback callback, unsigned long duration = 0);

// Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
template<typename TArg> bool attachInterruptInterval(unsigned long interval, void (*callback)(TArg), TArg params, unsigned long duration = 0);

// Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool attachInterruptInterval(unsigned long interval, timer_callback callback, unsigned long duration = 0)
```

as follows

```
void TimerHandler1()
{
  // Doing something here inside ISR
}

#define TIMER1_INTERVAL_MS        50L

void setup()
{
  ....
  
  // Interval in unsigned long millisecs
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1))
    Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer1. Select another freq. or timer");
}  
```

### 1.3 Set Hardware Timer Frequency and attach Timer Interrupt Handler function

Use one of these functions with **frequency in float Hz**

```
// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool setFrequency(float frequency, timer_callback_p callback, /* void* */ uint32_t params, unsigned long duration = 0);

// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool setFrequency(float frequency, timer_callback callback, unsigned long duration = 0);

// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
template<typename TArg> bool attachInterrupt(float frequency, void (*callback)(TArg), TArg params, unsigned long duration = 0);

// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool attachInterrupt(float frequency, timer_callback callback, unsigned long duration = 0);
```

as follows

```
void TimerHandler1()
{
  // Doing something here inside ISR
}

#define TIMER1_FREQ_HZ        5555.555

void setup()
{
  ....
  
  // Frequency in float Hz
  if (ITimer1.attachInterrupt(TIMER1_FREQ_HZ, TimerHandler1))
    Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer1. Select another freq. or timer");
}  
```


### 2. Using 16 ISR_based Timers from 1 Hardware Timer

### 2.1 Important Note

The 16 ISR_based Timers, designed for long timer intervals, only support using **unsigned long millisec intervals**. If you have to use much higher frequency or sub-millisecond interval, you have to use the Hardware Timers directly as in [1.3 Set Hardware Timer Frequency and attach Timer Interrupt Handler function](#13-set-hardware-timer-frequency-and-attach-timer-interrupt-handler-function)

### 2.2 Init Hardware Timer and ISR-based Timer

```
// Select USING_16MHZ     == true for  16MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_8MHZ      == true for   8MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_250KHZ    == true for 250KHz to Timer TCBx => shorter timer, but better accuracy
// Not select for default 250KHz to Timer TCBx => longer timer,  but worse accuracy
#define USING_16MHZ     true
#define USING_8MHZ      false
#define USING_250KHZ    false

#define USE_TIMER_0     false
#define USE_TIMER_1     true
#define USE_TIMER_2     false
#define USE_TIMER_3     false

// Init ISR_Timer
// Each ISR_Timer can service 16 different ISR-based timers
ISR_Timer ISR_Timer1;
```

### 2.3 Set Hardware Timer Interval and attach Timer Interrupt Handler functions

```
void TimerHandler()
{
  ISR_Timer1.run();
}

#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

// In AVR, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething2s()
{
  // Doing something here inside ISR every 2 seconds
}
  
void doingSomething5s()
{
  // Doing something here inside ISR every 5 seconds
}

void doingSomething11s()
{
  // Doing something here inside ISR  every 11 seconds
}

void doingSomething101s()
{
  // Doing something here inside ISR every 101 seconds
}

void setup()
{
  ....
  
  // Interval in millisecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler))
  {
    lastMillis = millis();
    Serial.println("Starting ITimer OK, millis() = " + String(lastMillis));
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  ISR_Timer1.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  ISR_Timer1.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  ISR_Timer1.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
  ISR_Timer1.setInterval(TIMER_INTERVAL_101S, doingSomething101s);
}  
```

---
---

### Examples: 

 1. [Argument_Complex](examples/Argument_Complex)
 2. [Argument_None](examples/Argument_None)
 3. [Argument_Simple](examples/Argument_Simple)
 4. [Change_Interval](examples/Change_Interval).
 5. [FakeAnalogWrite](examples/FakeAnalogWrite).
 6. [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex).
 7. [ISR_RPM_Measure](examples/ISR_RPM_Measure)
 8. [**Change_Interval_HF**](examples/Change_Interval_HF)
 9. [**ISR_Timers_Array_Simple**](examples/ISR_Timers_Array_Simple).
10. [RPM_Measure](examples/RPM_Measure)
11. [SwitchDebounce](examples/SwitchDebounce)
12. [TimerDuration](examples/TimerDuration)
13. [TimerInterruptTest](examples/TimerInterruptTest)
14. [**multiFileProject**](examples/multiFileProject) **New**

---

### Example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)

```cpp
#if !( defined(__AVR_ATmega4809__) || defined(ARDUINO_AVR_UNO_WIFI_REV2) || defined(ARDUINO_AVR_NANO_EVERY) )
  #error This is designed only for Arduino megaAVR board! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "megaAVR_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     3

// Select USING_16MHZ     == true for  16MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_8MHZ      == true for   8MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_250KHZ    == true for 250KHz to Timer TCBx => shorter timer, but better accuracy
// Not select for default 250KHz to Timer TCBx => longer timer,  but worse accuracy
#define USING_16MHZ     true
#define USING_8MHZ      false
#define USING_250KHZ    false


#define USE_TIMER_0     false
#define USE_TIMER_1     true
#define USE_TIMER_2     true
#define USE_TIMER_3     false

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "megaAVR_TimerInterrupt.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "megaAVR_ISR_Timer.h"

#include <SimpleTimer.h>              // https://github.com/jfturcot/SimpleTimer

#ifndef LED_BUILTIN
  #define LED_BUILTIN       13
#endif

ISR_Timer ISR_Timer1;

#define LED_TOGGLE_INTERVAL_MS        1000L

// You have to use longer time here if having problem because Arduino AVR clock is low, 16MHz => lower accuracy.
// Tested OK with 1ms when not much load => higher accuracy.
#define TIMER1_INTERVAL_MS            5L

volatile uint32_t startMillis = 0;

void TimerHandler1()
{
  static bool toggle  = false;
  static int timeRun  = 0;

  ISR_Timer1.run();

  // Toggle LED every LED_TOGGLE_INTERVAL_MS = 2000ms = 2s
  if (++timeRun == ((LED_TOGGLE_INTERVAL_MS) / TIMER1_INTERVAL_MS) )
  {
    timeRun = 0;

    //timer interrupt toggles pin LED_BUILTIN
    digitalWrite(LED_BUILTIN, toggle);
    toggle = !toggle;
  }
}

/////////////////////////////////////////////////

#define NUMBER_ISR_TIMERS         16

typedef void (*irqCallback)  (void);

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
  
  // In NRF52, avoid doing something fancy in ISR, for example Serial.print()
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

////////////////////////////////////////////////


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

  Serial.print(F("SimpleTimer : "));Serial.print(SIMPLE_TIMER_MS / 1000);
  Serial.print(F(", ms : ")); Serial.print(currMillis);
  Serial.print(F(", Dms : ")); Serial.println(currMillis - previousMillis);

  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT    
    Serial.print(F("Timer : ")); Serial.print(i);
    Serial.print(F(", programmed : ")); Serial.print(curISRTimerData[i].TimerInterval);
    Serial.print(F(", actual : ")); Serial.println(curISRTimerData[i].deltaMillis);
#else
    Serial.print(F("Timer : ")); Serial.print(i);
    Serial.print(F(", programmed : ")); Serial.print(TimerInterval[i]);
    Serial.print(F(", actual : ")); Serial.println(deltaMillis[i]);
#endif    
  }

  previousMillis = currMillis;
}

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial);

  Serial.print(F("\nStarting ISR_16_Timers_Array_Complex on "));
  Serial.println(BOARD_NAME);
  Serial.println(MEGA_AVR_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  Serial.print(F("TCB Clock Frequency = ")); 

#if USING_16MHZ  
  Serial.println(F("16MHz for highest accuracy"));
#elif USING_8MHZ  
  Serial.println(F("8MHz for very high accuracy"));
#else
  Serial.println(F("250KHz for lower accuracy but longer time"));
#endif

  ITimer1.init();

  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

  //ISR_Timer1.setInterval(2000L, doingSomething2s);
  //ISR_Timer1.setInterval(5000L, doingSomething5s);

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT
    curISRTimerData[i].previousMillis = startMillis;
    ISR_Timer1.setInterval(curISRTimerData[i].TimerInterval, curISRTimerData[i].irqCallbackFunc);
#else
    previousMillis[i] = startMillis;
    ISR_Timer1.setInterval(TimerInterval[i], irqCallbackFunc[i]);
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
```

---
---

### Debug Terminal Output Samples

### 1. ISR_16_Timers_Array_Complex on Arduino megaAVR Nano Every

The following is the sample terminal output when running example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) on **Arduino megaAVR Nano Every** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after more than 10.000s in loop().

```
Starting ISR_16_Timers_Array_Complex on megaAVR Nano Every
megaAVR_TimerInterrupt v1.5.0
CPU Frequency = 16 MHz
TCB Clock Frequency = 250KHz for lower accuracy but longer time
Starting  ITimer1 OK, millis() = 6
SimpleTimer : 2, ms : 10006, Dms : 10006
Timer : 0, programmed : 5000, actual : 5006
Timer : 1, programmed : 10000, actual : 10010
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 20065, Dms : 10059
Timer : 0, programmed : 5000, actual : 4994
Timer : 1, programmed : 10000, actual : 9998
Timer : 2, programmed : 15000, actual : 15014
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 30125, Dms : 10060
Timer : 0, programmed : 5000, actual : 4994
Timer : 1, programmed : 10000, actual : 9998
Timer : 2, programmed : 15000, actual : 14992
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 25012
Timer : 5, programmed : 30000, actual : 30006
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 40184, Dms : 10059
Timer : 0, programmed : 5000, actual : 5004
Timer : 1, programmed : 10000, actual : 10008
Timer : 2, programmed : 15000, actual : 14992
Timer : 3, programmed : 20000, actual : 20006
Timer : 4, programmed : 25000, actual : 25012
Timer : 5, programmed : 30000, actual : 30006
Timer : 6, programmed : 35000, actual : 35010
Timer : 7, programmed : 40000, actual : 40014
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 50245, Dms : 10061
Timer : 0, programmed : 5000, actual : 5004
Timer : 1, programmed : 10000, actual : 9998
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 20006
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30006
Timer : 6, programmed : 35000, actual : 35010
Timer : 7, programmed : 40000, actual : 40014
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50012
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 60307, Dms : 10062
Timer : 0, programmed : 5000, actual : 5004
Timer : 1, programmed : 10000, actual : 9998
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 19996
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30004
Timer : 6, programmed : 35000, actual : 35010
Timer : 7, programmed : 40000, actual : 40014
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50012
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60010
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 70369, Dms : 10062
Timer : 0, programmed : 5000, actual : 4994
Timer : 1, programmed : 10000, actual : 9998
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 19996
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30004
Timer : 6, programmed : 35000, actual : 34998
Timer : 7, programmed : 40000, actual : 40014
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50012
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60010
Timer : 12, programmed : 65000, actual : 65014
Timer : 13, programmed : 70000, actual : 70008
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 80432, Dms : 10063
Timer : 0, programmed : 5000, actual : 4994
Timer : 1, programmed : 10000, actual : 9998
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 19996
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30004
Timer : 6, programmed : 35000, actual : 34998
Timer : 7, programmed : 40000, actual : 39992
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50012
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60010
Timer : 12, programmed : 65000, actual : 65014
Timer : 13, programmed : 70000, actual : 70008
Timer : 14, programmed : 75000, actual : 75012
Timer : 15, programmed : 80000, actual : 80016


```

---

### 2. Change_Interval on Arduino megaAVR Nano Every

The following is the sample terminal output when running example [Change_Interval](examples/Change_Interval) on **Arduino megaAVR Nano Every** to demonstrate how to change Timer Interval on-the-fly

```
Starting Change_Interval on megaAVR Nano Every
megaAVR_TimerInterrupt v1.5.0
CPU Frequency = 16 MHz
TCB Clock Frequency = 250KHz for lower accuracy but longer time
Starting  ITimer1 OK, millis() = 1
Starting  ITimer2 OK, millis() = 4
Time = 10001, Timer1Count = 97, Timer2Count = 49
Time = 20002, Timer1Count = 195, Timer2Count = 99
Changing Interval, Timer1 = 200,  Timer2 = 400
Time = 30003, Timer1Count = 244, Timer2Count = 123
Time = 40004, Timer1Count = 294, Timer2Count = 148
Changing Interval, Timer1 = 100,  Timer2 = 200
Time = 50006, Timer1Count = 391, Timer2Count = 197
Time = 60007, Timer1Count = 489, Timer2Count = 247
Changing Interval, Timer1 = 200,  Timer2 = 400
Time = 70008, Timer1Count = 538, Timer2Count = 271
Time = 80009, Timer1Count = 588, Timer2Count = 296
Changing Interval, Timer1 = 100,  Timer2 = 200
```

---

### 3. ISR_16_Timers_Array_Complex on Arduino megaAVR Nano Every to show accuracy difference.


### 3.1. TCB Clock Frequency 16MHz for highest accuracy


```
Starting ISR_16_Timers_Array_Complex on megaAVR Nano Every
megaAVR_TimerInterrupt v1.5.0
CPU Frequency = 16 MHz
TCB Clock Frequency = 16MHz for highest accuracy
Starting  ITimer1 OK, millis() = 6
SimpleTimer : 2, ms : 10007, Dms : 10007
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10006
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 20066, Dms : 10059
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15006
Timer : 3, programmed : 20000, actual : 20006
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0

...


SimpleTimer : 2, ms : 211269, Dms : 10064
Timer : 0, programmed : 5000, actual : 5000            <========== Very accurate @ clock 16MHz
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45000
Timer : 9, programmed : 50000, actual : 50000
Timer : 10, programmed : 55000, actual : 55000
Timer : 11, programmed : 60000, actual : 60000
Timer : 12, programmed : 65000, actual : 65000
Timer : 13, programmed : 70000, actual : 70000
Timer : 14, programmed : 75000, actual : 75000
Timer : 15, programmed : 80000, actual : 80000
SimpleTimer : 2, ms : 221333, Dms : 10064
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45000
Timer : 9, programmed : 50000, actual : 50000
Timer : 10, programmed : 55000, actual : 55000
Timer : 11, programmed : 60000, actual : 60000
Timer : 12, programmed : 65000, actual : 65000
Timer : 13, programmed : 70000, actual : 70000
Timer : 14, programmed : 75000, actual : 75000
Timer : 15, programmed : 80000, actual : 80000

```

---

### 3.2. TCB Clock Frequency 8MHz for very high accuracy

```

Starting ISR_16_Timers_Array_Complex on megaAVR Nano Every
megaAVR_TimerInterrupt v1.5.0
CPU Frequency = 16 MHz
TCB Clock Frequency = 8MHz for very high accuracy
Starting  ITimer1 OK, millis() = 10
SimpleTimer : 2, ms : 10011, Dms : 10011
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10011
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0

...


SimpleTimer : 2, ms : 160949, Dms : 10064
Timer : 0, programmed : 5000, actual : 5000            <========== Very accurate @ clock 8MHz
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45000
Timer : 9, programmed : 50000, actual : 50000
Timer : 10, programmed : 55000, actual : 55000
Timer : 11, programmed : 60000, actual : 60000
Timer : 12, programmed : 65000, actual : 65000
Timer : 13, programmed : 70000, actual : 70000
Timer : 14, programmed : 75000, actual : 75000
Timer : 15, programmed : 80000, actual : 80000
SimpleTimer : 2, ms : 171013, Dms : 10064
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45000
Timer : 9, programmed : 50000, actual : 50000
Timer : 10, programmed : 55000, actual : 55000
Timer : 11, programmed : 60000, actual : 60000
Timer : 12, programmed : 65000, actual : 65000
Timer : 13, programmed : 70000, actual : 70000
Timer : 14, programmed : 75000, actual : 75000
Timer : 15, programmed : 80000, actual : 80000

```

---

### 3.3. TCB Clock Frequency 250KHz for lower accuracy but longer time

```
Starting ISR_16_Timers_Array_Complex on megaAVR Nano Every
megaAVR_TimerInterrupt v1.5.0
CPU Frequency = 16 MHz
TCB Clock Frequency = 250KHz for lower accuracy but longer time
Starting  ITimer1 OK, millis() = 11
SimpleTimer : 2, ms : 10012, Dms : 10012
Timer : 0, programmed : 5000, actual : 5021
Timer : 1, programmed : 10000, actual : 10015
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 20071, Dms : 10059
Timer : 0, programmed : 5000, actual : 4994
Timer : 1, programmed : 10000, actual : 9999
Timer : 2, programmed : 15000, actual : 15020
Timer : 3, programmed : 20000, actual : 20014
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0

...


SimpleTimer : 2, ms : 845278, Dms : 10063
Timer : 0, programmed : 5000, actual : 4994            <========== Less accurate @ clock 250KHz
Timer : 1, programmed : 10000, actual : 9997
Timer : 2, programmed : 15000, actual : 15001
Timer : 3, programmed : 20000, actual : 20005
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30004
Timer : 6, programmed : 35000, actual : 34998
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 44995
Timer : 9, programmed : 50000, actual : 50000
Timer : 10, programmed : 55000, actual : 55004
Timer : 11, programmed : 60000, actual : 59998
Timer : 12, programmed : 65000, actual : 64992
Timer : 13, programmed : 70000, actual : 70005
Timer : 14, programmed : 75000, actual : 75000
Timer : 15, programmed : 80000, actual : 80004
SimpleTimer : 2, ms : 855342, Dms : 10064
Timer : 0, programmed : 5000, actual : 5004
Timer : 1, programmed : 10000, actual : 9999
Timer : 2, programmed : 15000, actual : 15003
Timer : 3, programmed : 20000, actual : 20005
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30004
Timer : 6, programmed : 35000, actual : 34998
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45007
Timer : 9, programmed : 50000, actual : 50000
Timer : 10, programmed : 55000, actual : 55004
Timer : 11, programmed : 60000, actual : 59998
Timer : 12, programmed : 65000, actual : 64992
Timer : 13, programmed : 70000, actual : 70005
Timer : 14, programmed : 75000, actual : 75000
Timer : 15, programmed : 80000, actual : 80004
```

---

### 4. Change_Interval_HF on Arduino megaAVR Nano Every

The following is the sample terminal output when running example [Change_Interval_HF](examples/Change_Interval_HF) on **Arduino megaAVR Nano Every** to demonstrate how to change High Frequency Timer Interval on-the-fly

```
Starting Change_Interval_HF on megaAVR Nano Every
megaAVR_TimerInterrupt v1.5.0
CPU Frequency = 16 MHz
TCB Clock Frequency = 16MHz for highest accuracy
[TISR] TCB 1
[TISR] ==================
[TISR] Init, Timer = 1
[TISR] CTRLB   = 0
[TISR] CCMP    = 65535
[TISR] INTCTRL = 0
[TISR] CTRLA   = 1
[TISR] ==================
[TISR] Frequency = 10000.00 , CLK_TCB_FREQ = 16000000
[TISR] setFrequency: _CCMPValueRemaining =  1600
Starting ITimer1 OK, millis() = 12
Frequency, Timer1 = 10000
Time = 10001, Timer1Count = 99856
Time = 20002, Timer1Count = 199807
[TISR] Frequency = 5000.00 , CLK_TCB_FREQ = 16000000
[TISR] setFrequency: _CCMPValueRemaining =  3200
Changing Frequency, Timer1 = 5000
Time = 30003, Timer1Count = 249792
Time = 40004, Timer1Count = 299784
[TISR] Frequency = 10000.00 , CLK_TCB_FREQ = 16000000
[TISR] setFrequency: _CCMPValueRemaining =  1600
Changing Frequency, Timer1 = 10000
```


---
---


### Debug

Debug is enabled by default on Serial.

You can also change the debugging level from 0 to 3

```cpp
// These define's must be placed at the beginning before #include "megaAVR_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [megaAVR_TimerInterrupt issues](https://github.com/khoih-prog/megaAVR_TimerInterrupt/issues)

---
---

### TO DO

1. Search for bug and improvement.


### DONE

 1. Longer Interval for timers.
 2. Reduce code size if use less timers. Eliminate compiler warnings.
 3. Now supporting complex object pointer-type argument.
 4. 16 hardware-initiated software-enabled timers while using only 1 hardware timer.
 5. Fix some bugs in v1.0.0
 6. Add more examples.
 7. Similar library for ESP32, ESP8266, SAMD21/SAMD51, nRF52, Mbed-OS Nano-33-BLE, STM32
 8. Add support to ATmega4809-based boards, such as **Arduino UNO WiFi Rev2, AVR_NANO_EVERY, etc.**
 9. Selectable **TCB Clock 16MHz, 8MHz or 250KHz** depending on necessary accuracy
10. Fix `multiple-definitions` linker error
11. Optimize library code by using `reference-passing` instead of `value-passing`

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library. Especially to these people who have directly or indirectly contributed to this [megaAVR_TimerInterrupt library](https://github.com/khoih-prog/megaAVR_TimerInterrupt)

1. Thanks to good work of [Miguel Wisintainer](https://github.com/tcpipchip) for initiating, inspriring, working with, developing, debugging, testing and maintaining.
2. Thanks to [Bill Wuttke](https://github.com/wcwuttke) to report the issue [Interrupt interval 2X requested interval #1](https://github.com/khoih-prog/megaAVR_TimerInterrupt/issues/1) leading to new release v1.4.0 to fix bug in using higher frequencies than 250Hz.

<table>
  <tr>
    <td align="center"><a href="https://github.com/tcpipchip"><img src="https://github.com/tcpipchip.png" width="100px;" alt="tcpipchip"/><br/><sub><b>⭐️ Miguel Wisintainer</b></sub></a><br/></td>
    <td align="center"><a href="https://github.com/wcwuttke"><img src="https://github.com/wcwuttke.png" width="100px;" alt="wcwuttke"/><br/><sub><b>Bill Wuttke</b></sub></a><br/></td>
  </tr> 
</table>

---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

## License

- The library is licensed under [MIT](https://github.com/khoih-prog/megaAVR_TimerInterrupt/blob/master/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang


