/****************************************************************************************************************************
  megaAVR_TimerInterrupt.h
  For Arduino megaAVR ATMEGA4809-based boards (UNO WiFi Rev2, NANO_EVERY, etc. )
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/megaAVR_TimerInterrupt
  Licensed under MIT license

  Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.7.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      01/04/2021 Initial coding to support Arduino megaAVR ATmega4809-based boards (UNO WiFi Rev2, etc.)
  1.1.0   K.Hoang      14/04/2021 Fix bug. Don't use v1.0.0
  1.2.0   K.Hoang      17/04/2021 Selectable TCB Clock 16MHz, 8MHz or 250KHz depending on necessary accuracy
  1.3.0   K.Hoang      17/04/2021 Fix TCB Clock bug. Don't use v1.2.0
  1.4.0   K.Hoang      19/11/2021 Fix TCB Clock bug in high frequencies
  1.5.0   K.Hoang      22/01/2022 Fix `multiple-definitions` linker error
  1.6.0   K.Hoang      05/02/2022 Add support to MegaCoreX core
  1.6.1   K.Hoang      25/04/2022 Suppress warnings when _TIMERINTERRUPT_LOGLEVEL_ < 2
  1.7.0   K.Hoang      11/11/2022 Fix bug disabling TCB0
****************************************************************************************************************************/

#pragma once

#ifndef MEGA_AVR_TIMERINTERRUPT_H
#define MEGA_AVR_TIMERINTERRUPT_H

#include "megaAVR_TimerInterrupt.hpp"
#include "megaAVR_TimerInterrupt-Impl.h"

#endif      //#ifndef MEGA_AVR_TIMERINTERRUPT_H
