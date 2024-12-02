/*
  PrecDueTimer.h - PrecDueTimer header file, definition of methods and attributes...

  Original DueTimer library:
  Created by Ivan Seidel Gomes, March, 2013.
  Modified by Philipp Klaus, June 2013.
  
  PrecDueTimer library:
  Copyright (C) 2024 Krzysztof Bieliński
  
  Licensed under MIT license. For instructions and additional information go to
  https://github.com/KriBielinski/PrecDueTimer
*/

#include "Arduino.h"

#if defined(_SAM3XA_)

#ifndef PrecDueTimer_h
#define PrecDueTimer_h

#include <inttypes.h>

/*
	This fixes compatibility for Arduono Servo Library.
	Uncomment to make it compatible.

	Note that:
		+ Timers: 0,2,3,4,5 WILL NOT WORK, and will
				  neither be accessible by Timer0,...
*/
// #define USING_SERVO_LIB	true

#ifdef USING_SERVO_LIB
	#warning "HEY! You have set flag USING_SERVO_LIB. Timer0, 2,3,4 and 5 are not available"
#endif


#if defined TC2
#define NUM_TIMERS  9
#else
#define NUM_TIMERS  6
#endif

class PrecDueTimer
{
protected:

	// Represents the timer id (index for the array of Timer structs)
	const unsigned short timer;

	// Stores the object timer period
	// (allows to access current timer period and frequency):
	static uint32_t _period[NUM_TIMERS];

	// Picks the best clock to lower the error
	static uint8_t bestClock(double frequency, uint32_t& retRC);

  // Make Interrupt handlers friends, so they can use callbacks
  friend void TC0_Handler(void);
  friend void TC1_Handler(void);
  friend void TC2_Handler(void);
  friend void TC3_Handler(void);
  friend void TC4_Handler(void);
  friend void TC5_Handler(void);
#if NUM_TIMERS > 6
  friend void TC6_Handler(void);
  friend void TC7_Handler(void);
  friend void TC8_Handler(void);
#endif

	static void (*callbacks[NUM_TIMERS])();

	struct Timer
	{
		Tc *tc;
		uint32_t channel;
		IRQn_Type irq;
	};

	// Store timer configuration (static, as it's fixed for every object)
	static const Timer Timers[NUM_TIMERS];

public:

	static PrecDueTimer getAvailable(void);

	PrecDueTimer(unsigned short _timer);
	PrecDueTimer& attachInterrupt(void (*isr)());
	PrecDueTimer& detachInterrupt(void);
	PrecDueTimer& start(uint32_t microseconds = 0);
	PrecDueTimer& stop(void);
	PrecDueTimer& setFrequency(double frequency);
	PrecDueTimer& setPeriod(uint32_t microseconds);

	double getFrequency(void) const;
	uint32_t getPeriod(void) const;

  inline __attribute__((always_inline)) bool operator== (const PrecDueTimer& rhs) const
    {return timer == rhs.timer; };
  inline __attribute__((always_inline)) bool operator!= (const PrecDueTimer& rhs) const
    {return timer != rhs.timer; };
};

// Just to call Timer.getAvailable instead of Timer::getAvailable() :
extern PrecDueTimer Timer;

extern PrecDueTimer Timer1;
// Fix for compatibility with Servo library
#ifndef USING_SERVO_LIB
	extern PrecDueTimer Timer0;
	extern PrecDueTimer Timer2;
	extern PrecDueTimer Timer3;
	extern PrecDueTimer Timer4;
	extern PrecDueTimer Timer5;
#endif
#if NUM_TIMERS > 6
extern PrecDueTimer Timer6;
extern PrecDueTimer Timer7;
extern PrecDueTimer Timer8;
#endif

#endif

#else
	#error Oops! Trying to include DueTimer on another device?
#endif
