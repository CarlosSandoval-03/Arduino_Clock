#pragma once

#include "Arduino.h"

class Time {
	private:
	byte hours = 0;
	byte minutes = 0;
	byte seconds = 0;

	public:
	byte getHours();
	byte getMinutes();
	byte getSeconds();
	void setHours(byte hours);
	void setMinutes(byte minutes);
	void setSeconds(byte seconds);
	void setTime(byte hours, byte minutes, byte seconds);
	void incrementSeconds();
};
