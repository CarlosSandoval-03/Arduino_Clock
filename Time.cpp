#include "Time.h"

byte Time::getHours()
{
	return hours;
}

void Time::setHours(byte hours)
{
	this->hours = hours;
}

byte Time::getMinutes()
{
	return minutes;
}

void Time::setMinutes(byte minutes)
{
	this->minutes = minutes;
}

byte Time::getSeconds()
{
	return seconds;
}

void Time::setSeconds(byte seconds)
{
	this->seconds = seconds;
}

void Time::setTime(byte hours, byte minutes, byte seconds)
{
	this->hours = hours;
	this->minutes = minutes;
	this->seconds = seconds;
}

void Time::incrementSeconds()
{
	seconds++;
	if (seconds == 60) {
		seconds = 0;
		minutes++;
		if (minutes == 60) {
			minutes = 0;
			hours++;
			if (hours == 24) {
				hours = 0;
			}
		}
	}
}
