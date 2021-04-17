/****************************************************************************************************************************
  TimerInterrupt_Generic_Debug.h
  For Arduino boards (UNO, Nano, Mega, etc. )
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt
  Licensed under MIT license

  Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.2.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      01/04/2021 Initial coding to support Arduino megaAVR ATmega4809-based boards (UNO WiFi Rev2, etc.)
  1.1.0   K.Hoang      14/04/2021 Fix bug. Don't use v1.0.0
  1.2.0   K.Hoang      17/04/2021 Selectable TCB Clock 16MHz, 8MHz or 250KHz depending on necessary accuracy
 *****************************************************************************************************************************/

#pragma once

#ifndef TIMERINTERRUPT_GENERIC_DEBUG_H
#define TIMERINTERRUPT_GENERIC_DEBUG_H

#ifdef TIMERINTERRUPT_DEBUG_PORT
  #define TISR_DBG_PORT      TIMERINTERRUPT_DEBUG_PORT
#else
  #define TISR_DBG_PORT      Serial
#endif

// Change _TIMERINTERRUPT_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _TIMERINTERRUPT_LOGLEVEL_
  #define _TIMERINTERRUPT_LOGLEVEL_       1
#endif

/////////////////////////////////////////////////////////

const char TISR_MARK[] = "[TISR] ";

#define TISR_PRINT_MARK   TISR_PRINT(TISR_MARK)
#define TISR_PRINT_SP     TISR_DBG_PORT.print(" ")

#define TISR_PRINT        TISR_DBG_PORT.print
#define TISR_PRINTLN      TISR_DBG_PORT.println

/////////////////////////////////////////////////////////

#define TISR_LOGERROR(x)         if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_PRINT_MARK; TISR_PRINTLN(x); }
#define TISR_LOGERROR0(x)        if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_PRINT(x); }
#define TISR_LOGERROR1(x,y)      if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINTLN(y); }
#define TISR_LOGERROR2(x,y,z)    if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINTLN(z); }
#define TISR_LOGERROR3(x,y,z,w)  if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINT(z); TISR_PRINT_SP; TISR_PRINTLN(w); }

/////////////////////////////////////////////////////////

#define TISR_LOGWARN(x)          if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_PRINT_MARK; TISR_PRINTLN(x); }
#define TISR_LOGWARN0(x)         if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_PRINT(x); }
#define TISR_LOGWARN1(x,y)       if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINTLN(y); }
#define TISR_LOGWARN2(x,y,z)     if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINTLN(z); }
#define TISR_LOGWARN3(x,y,z,w)   if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINT(z); TISR_PRINT_SP; TISR_PRINTLN(w); }

/////////////////////////////////////////////////////////

#define TISR_LOGINFO(x)          if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_PRINT_MARK; TISR_PRINTLN(x); }
#define TISR_LOGINFO0(x)         if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_PRINT(x); }
#define TISR_LOGINFO1(x,y)       if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINTLN(y); }
#define TISR_LOGINFO2(x,y,z)     if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINTLN(z); }
#define TISR_LOGINFO3(x,y,z,w)   if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINT(z); TISR_PRINT_SP; TISR_PRINTLN(w); }

/////////////////////////////////////////////////////////

#define TISR_LOGDEBUG(x)         if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_PRINT_MARK; TISR_PRINTLN(x); }
#define TISR_LOGDEBUG0(x)        if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_PRINT(x); }
#define TISR_LOGDEBUG1(x,y)      if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINTLN(y); }
#define TISR_LOGDEBUG2(x,y,z)    if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINTLN(z); }
#define TISR_LOGDEBUG3(x,y,z,w)  if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINT(z); TISR_PRINT_SP; TISR_PRINTLN(w); }

/////////////////////////////////////////////////////////

#endif    //TIMERINTERRUPT_GENERIC_DEBUG_H
