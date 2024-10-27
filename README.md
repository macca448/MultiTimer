# MultiTimer
Based on the work of [Inflop](https://github.com/inflop) and the [Countimer](https://github.com/inflop/Countimer) library 

This is simple timer and counter Arduino library that counts in seconds up to 41 days
Offers two work modes:

 * Count-up timer with call specified method when count is complete.
 * Count-down timer with call specified method when count is complete.


The timer's clock source can be the controller's **`millis()`** or an **`RTC`**

 * **`run(USE_MILLIS, 0);`**
 * **`run(USE_RTC, rtc.second());`**


A global **`heartbeat`** is available to any or all timer's and is intended to give a visual indication that a timer is running

The setup **`config()`** for a timer has changed and both callback's are specified here. Times can be zero or a default period
 * void config(uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds, CountType countType, timer_callback refreshClock, timer_callback onComplete);

It allows you to start/pause, stop or restart timer.  
The following are public methods for actions:

 * void setCounter(uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);
 * void start()
 * void stop()
 * void pause()
 * void restart()


 Other methods:

 * byte getCurrentDays()
 * byte getCurrentHours()
 * byte getCurrentMinutes()
 * byte getCurrentSeconds()
 * String getCurrentTime(uint8_t format)
 * bool isCounterCompleted()
 * bool isCounterRunning()
 * bool isStopped()

Three formatted **`getCurrentTime()`** displays are available

 * FULL_DISPLAY - **`00 00:00:00`**
 * HMS_DISPLAY  - **`00:00:00`**
 * AUTO_DISPLAY - **`a00:00`** where 'a' is D, H or blank depending on current count


And here's some sample code!

```c
#include "MultiTimer.h"

MultiTimer timer;

void setup() {
	Serial.begin(9600);

    // Set up count down timer with 10s and call method onComplete() when timer is complete.
    // 00h:00m:10s
	timer.config(0, 0, 0, 10, timer.COUNT_DOWN, refreshClock, onComplete);
}

void refreshClock() {
	Serial.print("Current count time is: ");
    	Serial.println(timer.getCurrentTime(AUTO_DISPLAY));
}

void onComplete() {
	Serial.println("Complete!!!");
}

void loop() {
	// Run timer
	timer.run(USE_MILLIS, 0);

    // Now timer is running and listening for actions.
    // If you want to start the timer, you have to call start() method.
    if(!timer.isCounterCompleted()) {
      timer.start();
    }
}
```
