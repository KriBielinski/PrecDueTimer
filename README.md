# PrecDueTimer

Timer Library that is optimized for when frequent changes to the timer(s) period/frequency are required.

Based on the original DueTimer library (https://github.com/ivanseidel/DueTimer).

## Installation

PrecDueTimer library is available for download in the Arduino Library Manager. 

Alternatively you can download this library manually by following this tutorial:

https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/#importing-a-zip-library

## Information about the library

PrecDueTimer has nearly identical API as the original DueTimer library and can be used interchangably with it in most cases. For information about available functions see [Library Reference](#library-reference).

This library provides improvements in performance over the original library by:
1. Removing all the excessive floating-point calculations
2. Moving the calculations from being based on timer frequencies to being based on timer periods.
3. Using only a single clock (*TIMER_CLOCK1*) for timing purposes (see [TimerCounter](TimerCounter.md) file for more details).

The result is that the time it takes to change the timer period (either by calling `setPeriod` or `start` methods) has been reduced from around *90~120μs* to less than *1μs*.
This means that frequent changes to the timer's period can be performed while still being able to accurately measure time with precision down to single microseconds.

This library only supports periods up to around 102 seconds while the original DueTimer library supports more than 1 hour periods.
If long period times are required in your application, then it is recommended to use DueTimer library instead.

### Compatibility with Servo.h

Because Servo Library uses the same callbacks of DueTimer, we provided a custom solution for working with both of them.

However, Timers 0,2,3,4 and 5 will not Work anymore.

To enable compatibility you will need uncomment the following line in `PrecDueTimer.h`:

```
#define USING_SERVO_LIB	true
```

## Using the library

To call a function `handler` every `1000` microseconds:

```c++
Timer3.attachInterrupt(handler).start(1000);
// or:
Timer3.attachInterrupt(handler).setPeriod(1000).start();
// or, to select whichever available timer:
Timer.getAvailable().attachInterrupt(handler).start(1000);
```

In case you need to stop a timer, just do like this:

```c++
Timer3.stop();
```

And to continue running:

```c++
Timer3.start();
```

There are `9` Timer objects already instantiated for you:
`Timer0`, `Timer1`, `Timer2`, `Timer3`, `Timer4`, `Timer5`, `Timer6`, `Timer7` and `Timer8`.

### TIPs and Warnings

```c++
Timer4.attachInterrupt(handler).setPeriod(10).start();
// Is the same as:
Timer4.attachInterrupt(handler);
Timer4.setPeriod(10);
Timer4.start();

// To create a custom timer, refer to:
PrecDueTimer myTimer = DueTimer(0); // Creates a Timer 0 object.
PrecDueTimer myTimer = DueTimer(3); // Creates a Timer 3 object.
PrecDueTimer myTimer = DueTimer(t); // Creates a Timer t object.
// Note: Maximum t allowed is 8, as there is only 9 timers [0..8];

Timer1.attachInterrupt(handler1).start(10);
Timer1.attachInterrupt(handler2).start(10);
PrecDueTimer myTimer = PrecDueTimer(1);
myTimer.attachInterrupt(handler3).start(20);
// Will run only handle3, on Timer 1 (You are just overriding the callback)

Timer.getAvailable().attachInterrupt(callback1).start(10);
// Start timer on first available timer
PrecDueTimer::getAvailable().attachInterrupt(callback2).start(10);
// Start timer on second available timer
// And so on...

PrecDueTimer myTimer = Timer.getAvailable();
if (myTimer != PrecDueTimer(0))
// Now we know that the timer returned is actually available
// Can compare timers using == or !=

```

## Library Reference

### You should know:

- `getAvailable()` - Get the first available Timer.

- `attachInterrupt(void (*isr)())` - Attach a interrupt (callback function) for the timer of the object.

- `detachInterrupt()` - Detach current callback of timer.
  
- `setPeriod(unsigned int microseconds)` - Set the timer period (in microseconds)*

- `unsigned int getPeriod()` - Get the timer period (in microseconds)

- `start(unsigned int microseconds = 0)` - Start the timer with an optional period parameter*

- `stop()` - Stop the timer

- `setFrequency(double frequency)` - Set the timer frequency**

- `double getFrequency()` - Get the timer frequency**

  *\* Minimum supported period is 5μs while the maximum is 102261126μs (around 102 seconds).*
  
  *\*\* Although methods related to frequency have been provided to preserve compatibility with the DueTimer library it is not recommended to use them as they perform computationally expensive floating-point calculations.*

### Hardware Information

More information on the Timer Counter module of the µC on the Arduino Due
can be found in the documentation file [TimerCounter](TimerCounter.md).
