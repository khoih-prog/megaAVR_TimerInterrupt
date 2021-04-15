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

  Version: 1.1.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      01/04/2021 Initial coding to support Arduino megaAVR ATmega4809-based boards (UNO WiFi Rev2, etc.)
  1.1.0   K.Hoang      14/04/2021 Fix bug. Don't use v1.0.0
****************************************************************************************************************************/

#include "megaAVR_TimerInterrupt.h"

#ifndef TIMER_INTERRUPT_DEBUG
  #define TIMER_INTERRUPT_DEBUG      0
#endif

// From iom4809.h
//#define TCB0                  (*(TCB_t *) 0x0A80) /* 16-bit Timer Type B */
//#define TCB1                  (*(TCB_t *) 0x0A90) /* 16-bit Timer Type B */
//#define TCB2                  (*(TCB_t *) 0x0AA0) /* 16-bit Timer Type B */
//#define TCB3                  (*(TCB_t *) 0x0AB0) /* 16-bit Timer Type B */

TCB_t* TimerTCB[ NUM_HW_TIMERS ] = { &TCB0, &TCB1, &TCB2, &TCB3 };

void TimerInterrupt::init(int8_t timer)
{    
  // Set timer specific stuff
  // All timers in CTC mode
  // 8 bit timers will require changing prescalar values,
  // whereas 16 bit timers are set to either ck/1 or ck/64 prescalar
  
  //cli();//stop interrupts
  noInterrupts();
   
  // 16 bit timer
  TimerTCB[timer]->CTRLB    = TCB_CNTMODE_INT_gc;                         // Use timer compare mode
  TimerTCB[timer]->CCMP     = MAX_COUNT_16BIT;                            // Value to compare with.
  TimerTCB[timer]->INTCTRL  &= ~TCB_CAPT_bm;                              // Disable the interrupt
  TimerTCB[timer]->CTRLA    = TCB_CLKSEL_CLKTCA_gc | TCB_ENABLE_bm;       // Use Timer A as clock, enable timer

  TISR_LOGWARN1(F("TCB"), timer);
  
  TISR_LOGDEBUG(F("=================="));
  TISR_LOGDEBUG1(F("Init, Timer ="), timer);
  TISR_LOGDEBUG1(F("CTRLB   ="), TimerTCB[timer]->CTRLB);
  TISR_LOGDEBUG1(F("CCMP    ="), TimerTCB[timer]->CCMP);
  TISR_LOGDEBUG1(F("INTCTRL ="), TimerTCB[timer]->INTCTRL);
  TISR_LOGDEBUG1(F("CTRLA   ="), TimerTCB[timer]->CTRLA);
  TISR_LOGDEBUG(F("=================="));
   
  _timer = timer;

  //sei();//enable interrupts
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
   
  TimerTCB[_timer]->CCMP     = _CCMPValueToUse;    // Value to compare with.
  
  TimerTCB[_timer]->INTCTRL = TCB_CAPT_bm; // Enable the interrupt
  
  TISR_LOGDEBUG(F("=================="));
  TISR_LOGDEBUG1(F("set_CCMP, Timer ="), _timer);
  TISR_LOGDEBUG1(F("CTRLB   ="), TimerTCB[_timer]->CTRLB);
  TISR_LOGDEBUG1(F("CCMP    ="), TimerTCB[_timer]->CCMP);
  TISR_LOGDEBUG1(F("INTCTRL ="), TimerTCB[_timer]->INTCTRL);
  TISR_LOGDEBUG1(F("CTRLA   ="), TimerTCB[_timer]->CTRLA);
  TISR_LOGDEBUG(F("=================="));

  // Flag _CCMPValue == 0 => end of long timer
  if (_CCMPValueRemaining == 0)
    _timerDone = true;

}

// frequency (in hertz) and duration (in milliseconds).
// Return true if frequency is OK with selected timer (CCMPValue is in range)
bool TimerInterrupt::setFrequency(float frequency, timer_callback_p callback, uint32_t params, unsigned long duration)
{

#define CLK_TCA_FREQ      (250000L)

  //uint8_t       andMask = 0b11111000;
  unsigned long CCMPValue;
  bool isSuccess = false;
  
  // Currently using CLK_TCA from TCA0 == 250KHz

  //frequencyLimit must > 1
  float frequencyLimit = frequency * 17179.840;

  // Limit frequency to larger than (0.00372529 / 64) Hz or interval 17179.840s / 17179840 ms to avoid uint32_t overflow
  if ((_timer <= 0) || (callback == NULL) || ((frequencyLimit) < 1) )
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

      TISR_LOGWARN1(F("setFrequency => _toggle_count ="), _toggle_count);
      TISR_LOGWARN3(F("Frequency ="), frequency, F(", duration ="), duration);
           
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

    //cli();//stop interrupts
    noInterrupts();

    _frequency = frequency;
    _callback  = (void*) callback;
    _params    = reinterpret_cast<void*>(params);

    _timerDone = false;
    
    
    _CCMPValue = _CCMPValueRemaining = (uint32_t) (CLK_TCA_FREQ / frequency);
    
    TISR_LOGDEBUG3(F("Frequency ="), frequency, F(", CLK_TCA_FREQ ="), CLK_TCA_FREQ);
    TISR_LOGDEBUG1(F("setFrequency: _CCMPValueRemaining = "), _CCMPValueRemaining);
                
    // Set the CCMP for the given timer,
    // set the toggle count,
    // then turn on the interrupts     
    set_CCMP();
    
    //sei();//allow interrupts
    interrupts();

    return true;
  }
}

void TimerInterrupt::detachInterrupt(void)
{
  //cli();//stop interrupts
  noInterrupts();
     
  // Clear interrupt flag
  TimerTCB[_timer]->INTFLAGS = TCB_CAPT_bm;
  TimerTCB[_timer]->INTCTRL  &= ~TCB_CAPT_bm;    // Disable the interrupt
  TimerTCB[_timer]->CTRLA    &= ~TCB_ENABLE_bm;  // Disable timer
  
  //sei();//allow interrupts
  interrupts();
}

// Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
void TimerInterrupt::reattachInterrupt(unsigned long duration)
{
  //cli();//stop interrupts
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
  //TimerTCB[_timer]->INTFLAGS = TCB_CAPT_bm;
  TimerTCB[_timer]->INTCTRL  |= TCB_CAPT_bm;    // Enable the interrupt
  TimerTCB[_timer]->CTRLA    |= TCB_ENABLE_bm;  // Enable timer
  
  //sei();//allow interrupts
  interrupts();
}

// Just stop clock source, still keep the count
// To fix this.
void TimerInterrupt::pauseTimer(void)
{ 
  detachInterrupt();
}

// Just reconnect clock source, continue from the current count
void TimerInterrupt::resumeTimer(void)
{ 
  reattachInterrupt();
}

