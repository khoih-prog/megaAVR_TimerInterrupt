/****************************************************************************************************************************
  TimerDuration.ino
  For Arduino megaAVR ATMEGA4809-based boards (UNO WiFi Rev2, NANO_EVERY, etc. )
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/megaAVR_TimerInterrupt
  Licensed under MIT license

  Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.0.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      01/04/2021 Initial coding to support Arduino megaAVR ATmega4809-based boards (UNO WiFi Rev2, etc.)
 *****************************************************************************************************************************/

// These define's must be placed at the beginning before #include "megaAVR_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         4
#define _TIMERINTERRUPT_LOGLEVEL_     4

#define USE_TIMER_0     false
#define USE_TIMER_1     true
#define USE_TIMER_2     true
#define USE_TIMER_3     false

#include "megaAVR_TimerInterrupt.h"

#if !defined(LED_BUILTIN)
  #define LED_BUILTIN     13
#endif

void TimerHandler1(unsigned int outputPin = LED_BUILTIN)
{
  static bool toggle1 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(outputPin, OUTPUT);
  }

#if (TIMER_INTERRUPT_DEBUG > 1)
  Serial.print("ITimer1 called, millis() = "); Serial.println(millis());
#endif

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(outputPin, toggle1);
  toggle1 = !toggle1;
}

#if !( TIMER_INTERRUPT_USING_ATMEGA_32U4 )

void TimerHandler2(unsigned int outputPin = LED_BUILTIN)
{
  static bool toggle2 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(outputPin, OUTPUT);
  }

#if (TIMER_INTERRUPT_DEBUG > 1)
  Serial.print("ITimer2 called, millis() = "); Serial.println(millis());
#endif

  //timer interrupt toggles outputPin
  digitalWrite(outputPin, toggle2);
  toggle2 = !toggle2;
}
#endif

unsigned int outputPin1 = LED_BUILTIN;
unsigned int outputPin2 = A0;

#define TIMER1_INTERVAL_MS    1000
#define TIMER1_FREQUENCY      (float) (1000.0f / TIMER1_INTERVAL_MS)
#define TIMER1_DURATION_MS    (10 * TIMER1_INTERVAL_MS)

#define TIMER2_INTERVAL_MS    1300
#define TIMER2_FREQUENCY      (float) (1000.0f / TIMER2_INTERVAL_MS)
#define TIMER2_DURATION_MS    (20 * TIMER2_INTERVAL_MS)

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println(F("\nStarting TimerDuration on megaAVR"));
  Serial.println(MEGA_AVR_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  //if (ITimer1.attachInterrupt(TIMER1_FREQUENCY, TimerHandler1, outputPin1, TIMER1_DURATION_MS))
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, outputPin1, TIMER1_DURATION_MS))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

#if !( TIMER_INTERRUPT_USING_ATMEGA_32U4 )

  ITimer2.init();

  //if (ITimer2.attachInterrupt(TIMER2_FREQUENCY, TimerHandler2, outputPin2, TIMER2_DURATION_MS))
  if (ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler2, outputPin2, TIMER2_DURATION_MS))
  {
    Serial.print(F("Starting  ITimer2 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer2. Select another freq. or timer"));

#endif    
}

void loop()
{
}
