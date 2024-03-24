#pragma once

#include <Arduino.h>

class Time {
	private:
	byte hours = 0;
	byte minutes = 0;
	byte seconds = 0;

	public:
	byte getHours();
	void setHours(byte hours);
	byte getMinutes();
	void setMinutes(byte minutes);
	byte getSeconds();
	void setSeconds(byte seconds);
	void setTime(byte hours, byte minutes, byte seconds);
	void incrementSeconds();
};
