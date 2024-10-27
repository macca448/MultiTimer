#pragma once

#ifndef MULTITIMER_H
#define MULTITIMER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

extern bool global_blink;

#define ONE_SECOND 1000UL
#define MULTITIMER_MAX_DAYS 40
#define MULTITIMER_MAX_HOURS 23
#define MULTITIMER_MAX_MINUTES_SECONDS 59
#define USE_MILLIS true
#define USE_RTC false
#define FULL_DISPLAY 0
#define HMS_DISPLAY 1
#define AUTO_DISPLAY 2

typedef void(*timer_callback)(void);

class MultiTimer
{
public:
	MultiTimer();
	~MultiTimer();

	enum CountType
	{
		COUNT_NONE = 0,
		COUNT_UP = 1,
		COUNT_DOWN = 2
	};

	// Set up counter time(hours, minutes, seconds), count mode and function to execute if count is completed.
	void config(uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds, CountType countType, timer_callback callback, timer_callback onComplete);

	// Set up counter time(hours, minutes, seconds) for existing timer.
	void setCounter(uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);

	// Returns timer's current days.
	uint8_t getCurrentDays();

	// Returns timer's current hours.
	uint8_t getCurrentHours();

	// Returns timer's current minutes.
	uint8_t getCurrentMinutes();

	// Returns timer's current seconds.
	uint8_t getCurrentSeconds();

	// Returns current timer as formatted string HH:MM:SS
	char* getCurrentTime(uint8_t format);

	// Returns true if counter is completed, otherwise returns false.
	bool isCounterCompleted();

	// Returns true if counter is still running, otherwise returns false.
	bool isCounterRunning();

	// Returns true if timer is stopped, otherwise returns false.
	bool isStopped();

	// Run timer. This is main method.
	// If you want to start timer after run, you have to invoke start() method.
	void run(bool method, uint8_t sec);

	// Starting timer.
	void start();

	// Stopping timer.
	void stop();

	// Pausing timer.
	void pause();

	// Restart timer.
	void restart();

private:
	
	void updateClock();
	
	// Counting down timer.
	void countDown();
	
	void callback();
	void complete();

	// Counting up timer.
	void countUp();

	uint32_t _previousMillis;
	uint8_t _lastRTCsecond;

	// Stores current counter value in milliseconds.
	uint32_t _currentCountTime;
	uint32_t _startCountTime;

	// Stores cached user's time.
	uint32_t _countTime;

	// Function to execute.
	timer_callback _callback;

	// Function to execute when timer is complete.
	timer_callback _onComplete;
	bool _isCounterCompleted;
	bool _isStopped;
	char _formatted_time[12];
	CountType _countType;
};

#endif
