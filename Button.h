#pragma once

#include "Arduino.h"

#define FIRST_CHANGE_STATE 0
#define DEBOUNCE_VALIDATION_STATE 1
#define LONGPRESS_VALIDATION_STATE 2

#define DEBOUNCE_DELAY_MS 50
#define LONGPRESS_TIME_MS 1000

class Button {
	private:
	int pin;
	bool isPressedFlag = false, wasPressedFlag = false, wasLongPressedFlag = false;
	int state = 0;
	int reading;
	int buttonState = HIGH;
	int lastButtonState = HIGH;
	unsigned long lastDebounceTime = 0;
	unsigned long lastPressTime = 0;

	public:
	Button(int pin);
	void update(unsigned long currentMillis);
	bool isPressed();
	bool wasPressed();
	bool wasLongPressed();
};
