/****************************************************************************************************************************
  megaAVR_TimerInterrupt.cpp
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

#ifndef MEGA_AVR_TIMERINTERRUPT_IMPL_H
#define MEGA_AVR_TIMERINTERRUPT_IMPL_H

#ifndef TIMER_INTERRUPT_DEBUG
  #define TIMER_INTERRUPT_DEBUG      0
#endif

/*****************************************************************************************

  // From ~/.arduino15/packages/arduino/7.3.0-atmel3.6.1-arduino5/avr/include/avr/iom4809.h

  //#define TCB0                  (*(TCB_t *) 0x0A80) // 16-bit Timer Type B
  //#define TCB1                  (*(TCB_t *) 0x0A90) // 16-bit Timer Type B
  //#define TCB2                  (*(TCB_t *) 0x0AA0) // 16-bit Timer Type B
  //#define TCB3                  (*(TCB_t *) 0x0AB0) // 16-bit Timer Type B

  //
  typedef enum TCB_CLKSEL_enum
  {
    TCB_CLKSEL_CLKDIV1_gc = (0x00<<1),  // CLK_PER (No Prescaling)
    TCB_CLKSEL_CLKDIV2_gc = (0x01<<1),  // CLK_PER/2 (From Prescaler)
    TCB_CLKSEL_CLKTCA_gc = (0x02<<1),   // Use Clock from TCA
  } TCB_CLKSEL_t;

  //
  typedef enum TCB_CNTMODE_enum
  {
    TCB_CNTMODE_INT_gc = (0x00<<0),       // Periodic Interrupt
    TCB_CNTMODE_TIMEOUT_gc = (0x01<<0),   // Periodic Timeout
    TCB_CNTMODE_CAPT_gc = (0x02<<0),      // Input Capture Event
    TCB_CNTMODE_FRQ_gc = (0x03<<0),       // Input Capture Frequency measurement
    TCB_CNTMODE_PW_gc = (0x04<<0),        // Input Capture Pulse-Width measurement
    TCB_CNTMODE_FRQPW_gc = (0x05<<0),     // Input Capture Frequency and Pulse-Width measurement
    TCB_CNTMODE_SINGLE_gc = (0x06<<0),    // Single Shot
    TCB_CNTMODE_PWM8_gc = (0x07<<0),      // 8-bit PWM
  } TCB_CNTMODE_t;

*****************************************************************************************/

#if ( defined(__AVR_ATmega4809__) || defined(__AVR_ATmega3209__) || defined(__AVR_ATmega1609__) || defined(__AVR_ATmega809__) )
#if (_TIMERINTERRUPT_LOGLEVEL_ > 2)
  #warning Using __AVR_ATmegaXX09__ architecture
#endif

#define TIMER_INTERRUPT_USING_ATMEGA_XX09       true

TCB_t* TimerTCB[ NUM_HW_TIMERS ] = { &TCB0, &TCB1, &TCB2, &TCB3 };

#elif ( defined(__AVR_ATmega4808__) || defined(__AVR_ATmega3208__) || defined(__AVR_ATmega1608__) || defined(__AVR_ATmega808__) )
#if (_TIMERINTERRUPT_LOGLEVEL_ > 2)
  #warning Using __AVR_ATmegaXX08__ architecture
#endif

#define TIMER_INTERRUPT_USING_ATMEGA_XX08       true

TCB_t* TimerTCB[ NUM_HW_TIMERS ] = { &TCB0, &TCB1, &TCB2 };

#endif

#define CLK_TCA_FREQ      (250000L)

// Clock for UNO WiFi Rev2 and Nano Every is 16MHz
#if USING_16MHZ
  // Use no prescaler (prescaler 1) => 16MHz
  #if (_TIMERINTERRUPT_LOGLEVEL_ > 2)
    #warning Using no prescaler => 16MHz
  #endif

  #define TCB_CLKSEL_VALUE      TCB_CLKSEL_CLKDIV1_gc
  #define CLOCK_PRESCALER       1
#elif USING_8MHZ
  // Use prescaler 2 => 8MHz
  #if (_TIMERINTERRUPT_LOGLEVEL_ > 2)
    #warning Using prescaler 2 => 8MHz
  #endif

  #define TCB_CLKSEL_VALUE      TCB_CLKSEL_CLKDIV2_gc
  #define CLOCK_PRESCALER       2
#elif USING_250KHZ
  // Optional, but for clarity
  // Use Timer A as clock (prescaler 64) => 250KHz
  #define TCB_CLKSEL_VALUE      TCB_CLKSEL_CLKTCA_gc
  #define CLOCK_PRESCALER       64
#else
  // Use Timer A as clock (prescaler 64) => 250KHz
  #if (_TIMERINTERRUPT_LOGLEVEL_ > 2)
    #warning Using prescaler 64 => 250KHz
  #endif

  #define TCB_CLKSEL_VALUE      TCB_CLKSEL_CLKTCA_gc
  #define CLOCK_PRESCALER       64
#endif

#define CLK_TCB_FREQ          ( F_CPU / CLOCK_PRESCALER )

void TimerInterrupt::init(const int8_t& timer)
{
  // Set timer specific stuff
  // All timers in CTC mode
  // 8 bit timers will require changing prescalar values,
  // whereas 16 bit timers are set to either ck/1 or ck/64 prescalar

  noInterrupts();

  // 16 bit timer
  TimerTCB[timer]->CTRLB    = TCB_CNTMODE_INT_gc;                         // Use timer compare mode
  TimerTCB[timer]->CCMP     = MAX_COUNT_16BIT;                            // Value to compare with.
  TimerTCB[timer]->INTCTRL  &= ~TCB_CAPT_bm;                              // Disable the interrupt
  TimerTCB[timer]->CTRLA    = TCB_CLKSEL_VALUE | TCB_ENABLE_bm;       // Use Timer A as clock, enable timer

  TISR_LOGWARN1(F("TCB"), timer);

  TISR_LOGINFO(F("=================="));
  TISR_LOGINFO1(F("Init, Timer = "), timer);
  TISR_LOGINFO1(F("CTRLB   = "), TimerTCB[timer]->CTRLB);
  TISR_LOGINFO1(F("CCMP    = "), TimerTCB[timer]->CCMP);
  TISR_LOGINFO1(F("INTCTRL = "), TimerTCB[timer]->INTCTRL);
  TISR_LOGINFO1(F("CTRLA   = "), TimerTCB[timer]->CTRLA);
  TISR_LOGINFO(F("=================="));

  _timer = timer;

  interrupts();

}

void TimerInterrupt::set_CCMP()
{
  // Run with noInterrupt()
  // Set the CCMP for the given timer,
  // set the toggle count,
  // then turn on the interrupts
  uint32_t _CCMPValueToUse;

  _CCMPValueToUse = min(MAX_COUNT_16BIT, _CCMPValueRemaining);
  _CCMPValueRemaining -= _CCMPValueToUse;

  TimerTCB[_timer]->CCMP    = _CCMPValueToUse;    // Value to compare with.

  TimerTCB[_timer]->INTCTRL = TCB_CAPT_bm; // Enable the interrupt

  TISR_LOGDEBUG(F("=================="));
  TISR_LOGDEBUG1(F("set_CCMP, Timer = "), _timer);
  TISR_LOGDEBUG1(F("CTRLB   = "), TimerTCB[_timer]->CTRLB);
  TISR_LOGDEBUG1(F("CCMP    = "), TimerTCB[_timer]->CCMP);
  TISR_LOGDEBUG1(F("INTCTRL = "), TimerTCB[_timer]->INTCTRL);
  TISR_LOGDEBUG1(F("CTRLA   = "), TimerTCB[_timer]->CTRLA);
  TISR_LOGDEBUG(F("=================="));

  // Flag _CCMPValue == 0 => end of long timer
  if (_CCMPValueRemaining == 0)
    _timerDone = true;

}

// frequency (in hertz) and duration (in milliseconds).
// Return true if frequency is OK with selected timer (CCMPValue is in range)
bool TimerInterrupt::setFrequency(const float& frequency, timer_callback_p callback, const uint32_t& params,
                                  const unsigned long& duration)
{
  //frequencyLimit must > 1
  float frequencyLimit = frequency * 17179.840;

  // Limit frequency to larger than (0.00372529 / 64) Hz or interval 17179.840s / 17179840 ms to avoid uint32_t overflow
  if ((_timer < 0) || (callback == NULL) || ((frequencyLimit) < 1) )
  {
    TISR_LOGDEBUG(F("setFrequency error"));

    return false;
  }
  else
  {
    // Calculate the toggle count. Duration must be at least longer then one cycle
    if (duration > 0)
    {
      _toggle_count = frequency * duration / 1000;

      TISR_LOGINFO1(F("setFrequency => _toggle_count = "), _toggle_count);
      TISR_LOGINFO3(F("Frequency ="), frequency, F(", duration = "), duration);

      if (_toggle_count < 1)
      {
        TISR_LOGDEBUG(F("setFrequency: _toggle_count < 1 error"));

        return false;
      }
    }
    else
    {
      _toggle_count = -1;
    }

    //Timer0-3 are 16 bit timers, meaning it can store a maximum counter value of 65535.

    noInterrupts();

    _frequency = frequency;
    _callback  = (void*) callback;
    _params    = reinterpret_cast<void*>(params);

    _timerDone = false;

    _CCMPValue = _CCMPValueRemaining = (uint32_t) (CLK_TCB_FREQ / frequency);

    TISR_LOGINFO3(F("Frequency = "), frequency, F(", CLK_TCB_FREQ = "), CLK_TCB_FREQ);
    TISR_LOGINFO1(F("setFrequency: _CCMPValueRemaining = "), _CCMPValueRemaining);

    // Set the CCMP for the given timer,
    // set the toggle count,
    // then turn on the interrupts
    set_CCMP();

    interrupts();

    return true;
  }
}

void TimerInterrupt::detachInterrupt()
{
  noInterrupts();

  // Clear interrupt flag
  TimerTCB[_timer]->INTFLAGS = TCB_CAPT_bm;
  TimerTCB[_timer]->INTCTRL  &= ~TCB_CAPT_bm;    // Disable the interrupt
  TimerTCB[_timer]->CTRLA    &= ~TCB_ENABLE_bm;  // Disable timer

  interrupts();
}

// Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
void TimerInterrupt::reattachInterrupt(const unsigned long& duration)
{
  noInterrupts();

  // Calculate the toggle count
  if (duration > 0)
  {
    _toggle_count = _frequency * duration / 1000;
  }
  else
  {
    _toggle_count = -1;
  }

  // Set interrupt flag
  TimerTCB[_timer]->INTCTRL  |= TCB_CAPT_bm;    // Enable the interrupt
  TimerTCB[_timer]->CTRLA    |= TCB_ENABLE_bm;  // Enable timer

  interrupts();
}

// Just stop clock source, still keep the count
// To fix this.
void TimerInterrupt::pauseTimer()
{
  detachInterrupt();
}

// Just reconnect clock source, continue from the current count
void TimerInterrupt::resumeTimer()
{
  reattachInterrupt();
}

////////////////////////////////////////////////////////

// To be sure not used Timers are disabled

// TCB0
#if !defined(USE_TIMER_0)
  #define USE_TIMER_0     false
#endif

// TCB1
#if !defined(USE_TIMER_1)
  #define USE_TIMER_1     false
#endif

// TCB2
#if !defined(USE_TIMER_2)
  #define USE_TIMER_2     false
#endif

// TCB3
#if !defined(USE_TIMER_3)
  #define USE_TIMER_3     false
#endif


//////////////////////////////////////////////

#if USE_TIMER_0
#ifndef TIMER0_INSTANTIATED
// To force pre-instatiate only once
#define TIMER0_INSTANTIATED
TimerInterrupt ITimer0(HW_TIMER_0);

ISR(TCB0_INT_vect)
{
  long countLocal = ITimer0.getCount();

  if (ITimer0.getTimer() == 0)
  {
    if (countLocal != 0)
    {
      if (ITimer0.checkTimerDone())
      {
        TISR_LOGDEBUG3(("T0 callback, _CCMPValueRemaining = "), ITimer0.get_CCMPValueRemaining(), (", millis = "), millis());

        ITimer0.callback();

        if (ITimer0.get_CCMPValue() > MAX_COUNT_16BIT)
        {
          // To reload _CCMPValueRemaining as well as _CCMP register to MAX_COUNT_16BIT
          ITimer0.reload_CCMPValue();
        }

        if (countLocal > 0)
          ITimer0.setCount(countLocal - 1);
      }
      else
      {
        //Deduct _CCMPValue by min(MAX_COUNT_16BIT, _CCMPValue)
        // If _CCMPValue == 0, flag _timerDone for next cycle
        // If last one (_CCMPValueRemaining < MAX_COUNT_16BIT) => load _CCMP register _CCMPValueRemaining
        ITimer0.adjust_CCMPValue();
      }
    }
    else
    {
      TISR_LOGWARN(("T0 done"));

      ITimer0.detachInterrupt();
    }
  }

  // Clear interrupt flag
  TCB0.INTFLAGS = TCB_CAPT_bm;
}
#endif  //#ifndef TIMER0_INSTANTIATED
#endif    //#if USE_TIMER_0

#if USE_TIMER_1
#ifndef TIMER1_INSTANTIATED
// To force pre-instatiate only once
#define TIMER1_INSTANTIATED
TimerInterrupt ITimer1(HW_TIMER_1);

// Timer0 is used for micros(), millis(), delay(), etc and can't be used
// Pre-instatiate

ISR(TCB1_INT_vect)
{
  long countLocal = ITimer1.getCount();

  if (ITimer1.getTimer() == 1)
  {
    if (countLocal != 0)
    {
      if (ITimer1.checkTimerDone())
      {
        TISR_LOGDEBUG3(("T1 callback, _CCMPValueRemaining = "), ITimer1.get_CCMPValueRemaining(), (", millis = "), millis());

        ITimer1.callback();

        if (ITimer1.get_CCMPValue() > MAX_COUNT_16BIT)
        {
          // To reload _CCMPValueRemaining as well as _CCMP register to MAX_COUNT_16BIT if _CCMPValueRemaining > MAX_COUNT_16BIT
          ITimer1.reload_CCMPValue();
        }

        if (countLocal > 0)
          ITimer1.setCount(countLocal - 1);
      }
      else
      {
        //Deduct _CCMPValue by min(MAX_COUNT_16BIT, _CCMPValue)
        // If _CCMPValue == 0, flag _timerDone for next cycle
        // If last one (_CCMPValueRemaining < MAX_COUNT_16BIT) => load _CCMP register _CCMPValueRemaining
        ITimer1.adjust_CCMPValue();
      }
    }
    else
    {
      TISR_LOGWARN(("T1 done"));

      ITimer1.detachInterrupt();
    }
  }

  // Clear interrupt flag
  TCB1.INTFLAGS = TCB_CAPT_bm;
}

#endif  //#ifndef TIMER1_INSTANTIATED
#endif    //#if USE_TIMER_1

#if USE_TIMER_2
#ifndef TIMER2_INSTANTIATED
#define TIMER2_INSTANTIATED
TimerInterrupt ITimer2(HW_TIMER_2);

ISR(TCB2_INT_vect)
{
  long countLocal = ITimer2.getCount();

  if (ITimer2.getTimer() == 2)
  {
    if (countLocal != 0)
    {
      if (ITimer2.checkTimerDone())
      {
        TISR_LOGDEBUG3(("T2 callback, _CCMPValueRemaining = "), ITimer2.get_CCMPValueRemaining(), (", millis = "), millis());

        ITimer2.callback();

        if (ITimer2.get_CCMPValue() > MAX_COUNT_16BIT)
        {
          // To reload _CCMPValueRemaining as well as _CCMP register to MAX_COUNT_16BIT if _CCMPValueRemaining > MAX_COUNT_16BIT
          ITimer2.reload_CCMPValue();
        }

        if (countLocal > 0)
          ITimer2.setCount(countLocal - 1);

      }
      else
      {
        //Deduct _CCMPValue by min(MAX_COUNT_8BIT, _CCMPValue)
        // If _CCMPValue == 0, flag _timerDone for next cycle
        ITimer2.adjust_CCMPValue();
      }
    }
    else
    {
      TISR_LOGWARN(("T2 done"));

      ITimer2.detachInterrupt();
    }
  }

  // Clear interrupt flag
  TCB2.INTFLAGS = TCB_CAPT_bm;
}
#endif  //#ifndef TIMER2_INSTANTIATED
#endif    //#if USE_TIMER_2

// Pre-instatiate
#if USE_TIMER_3
#ifndef TIMER3_INSTANTIATED
// To force pre-instatiate only once
#define TIMER3_INSTANTIATED
TimerInterrupt ITimer3(HW_TIMER_3);

ISR(TCB3_INT_vect)
{
  long countLocal = ITimer3.getCount();

  if (ITimer3.getTimer() == 3)
  {
    if (countLocal != 0)
    {
      if (ITimer3.checkTimerDone())
      {
        TISR_LOGDEBUG3(("T3 callback, _CCMPValueRemaining = "), ITimer3.get_CCMPValueRemaining(), (", millis = "), millis());

        ITimer3.callback();

        if (ITimer3.get_CCMPValue() > MAX_COUNT_16BIT)
        {
          // To reload _CCMPValueRemaining as well as _CCMP register to MAX_COUNT_16BIT if _CCMPValueRemaining > MAX_COUNT_16BIT
          ITimer3.reload_CCMPValue();
        }

        if (countLocal > 0)
          ITimer3.setCount(countLocal - 1);
      }
      else
      {
        //Deduct _CCMPValue by min(MAX_COUNT_16BIT, _CCMPValue)
        // If _CCMPValue == 0, flag _timerDone for next cycle
        // If last one (_CCMPValueRemaining < MAX_COUNT_16BIT) => load _CCMP register _CCMPValueRemaining
        ITimer3.adjust_CCMPValue();
      }
    }
    else
    {
      TISR_LOGWARN(("T3 done"));

      ITimer3.detachInterrupt();
    }
  }

  // Clear interrupt flag
  TCB3.INTFLAGS = TCB_CAPT_bm;
}

#endif  //#ifndef TIMER3_INSTANTIATED
#endif    //#if USE_TIMER_3

#endif // MEGA_AVR_TIMERINTERRUPT_IMPL_H
