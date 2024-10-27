#include "MultiTimer.h"
#include "Arduino.h"

bool heartbeat = 0;

MultiTimer::MultiTimer()
{
	_previousMillis = 0;
	_currentCountTime = 0;
	_countTime = 0;
	_isCounterCompleted = false;
	_isStopped = true;
	_countType = COUNT_DOWN;
	_startCountTime = 0;
	_lastRTCsecond = 0;
}

MultiTimer::~MultiTimer()
{
}

void MultiTimer::config(uint8_t dd_t, uint8_t hh_t, uint8_t mm_t, uint8_t ss_t, CountType countType, timer_callback callback, timer_callback onComplete)
{
	_callback = callback;
	_onComplete = onComplete;
	_countType = countType;
	setCounter(dd_t, hh_t, mm_t, ss_t);
}

void MultiTimer::setCounter(uint8_t dd_t, uint8_t hh_t, uint8_t mm_t, uint8_t ss_t)
{
	if (dd_t > MULTITIMER_MAX_DAYS) {
		dd_t = MULTITIMER_MAX_DAYS;
	}
	
	if (hh_t > MULTITIMER_MAX_HOURS) {
		hh_t = MULTITIMER_MAX_HOURS;
	}

	if (mm_t > MULTITIMER_MAX_MINUTES_SECONDS) {
		mm_t = MULTITIMER_MAX_MINUTES_SECONDS;
	}

	if (ss_t > MULTITIMER_MAX_MINUTES_SECONDS) {
		ss_t = MULTITIMER_MAX_MINUTES_SECONDS;
	}

	_currentCountTime = (dd_t * 86400L) + (hh_t * 3600L) + (mm_t * 60L) + ss_t;
	_countTime = _currentCountTime;

	if (_countType == COUNT_UP)
	{
		// if is count up mode, we have to start from 00:00:00;
		_currentCountTime = 0;
	}

	_startCountTime = _currentCountTime;

}

uint8_t MultiTimer::getCurrentDays()
{
	return _currentCountTime / 86400;
}

uint8_t MultiTimer::getCurrentHours()
{
	return _currentCountTime % 86400 / 3600;
}

uint8_t MultiTimer::getCurrentMinutes()
{
	return _currentCountTime  % 3600 / 60;
}

uint8_t MultiTimer::getCurrentSeconds()
{
	return _currentCountTime % 3600 % 60 % 60;
}

char* MultiTimer::getCurrentTime(uint8_t format)
{
	if(format == FULL_DISPLAY)
	{
		sprintf(_formatted_time, "%02d %02d:%02d:%02d", getCurrentDays(), getCurrentHours(), getCurrentMinutes(), getCurrentSeconds());
	}
	else if(format == HMS_DISPLAY)
	{
		sprintf(_formatted_time, "%02d:%02d:%02d", getCurrentHours(), getCurrentMinutes(), getCurrentSeconds());
	}
	else if(format == AUTO_DISPLAY)
	{
		if(getCurrentDays() > 0)
		{
			sprintf(_formatted_time, "D%02d:%02d", getCurrentDays(), getCurrentHours());
		}
		else if(getCurrentHours() > 0)
		{
			sprintf(_formatted_time, "H%02d:%02d", getCurrentHours(), getCurrentMinutes());
		}
		else
		{
			sprintf(_formatted_time, " %02d:%02d", getCurrentMinutes(), getCurrentSeconds());
		}
	}

	return _formatted_time;
}

bool MultiTimer::isCounterCompleted()
{
	return _isCounterCompleted;
}

bool MultiTimer::isStopped()
{
	return _isStopped;
}

bool MultiTimer::isCounterRunning()
{
	return !_isStopped;
}

void MultiTimer::start()
{
	_isStopped = false;
	if(_isCounterCompleted)
		_isCounterCompleted = false;
}

void MultiTimer::pause()
{
	_isStopped = true;
}

void MultiTimer::stop()
{
	_isStopped = true;
	_isCounterCompleted = true;
	_currentCountTime = _countTime;

	if(_countType == COUNT_UP)
	{
		_currentCountTime = 0;		
	}
}

void MultiTimer::restart()
{
	_currentCountTime = _startCountTime;
	_isCounterCompleted = false;
	_isStopped = false;

	start();
}

void MultiTimer::run(bool method, uint8_t sec)
{
	// timer is running only if is not completed or not stopped.
	if (_isCounterCompleted || _isStopped)
		return;
	if(method){
		if (millis() - _previousMillis >= ONE_SECOND)
		{
			_previousMillis = millis();
			updateClock();
		}
	}
	else
	{
		if(sec != _lastRTCsecond)
		{
			_lastRTCsecond = sec;
			updateClock();
		}
	}
}

void MultiTimer::updateClock()
{
	heartbeat = !heartbeat;

	if (_countType == COUNT_DOWN)
	{
		countDown();
	}
	else if (_countType == COUNT_UP)
	{
		countUp();
	}
	else
	{
		callback();
	}
}

void MultiTimer::countDown()
{
	if (_currentCountTime > 0)
	{
		callback();
		_currentCountTime -= 1;
	}
	else
	{
		callback();
		stop();
		complete();
	}
}

void MultiTimer::countUp()
{
	if (_currentCountTime < _countTime)
	{
		callback();
		_currentCountTime += 1;
	}
	else
	{
		callback();
		stop();
		complete();
	}
}

void MultiTimer::callback()
{
	if(_callback != NULL)
		_callback();
}

void MultiTimer::complete()
{
	if(_onComplete != NULL)
		_onComplete();
}
