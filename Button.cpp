#include "Button.h"

Button::Button(int pin)
{
	this->pin = pin;
	pinMode(pin, INPUT_PULLUP);
}

void Button::update(unsigned long currentMillis)
{
	isPressedFlag = wasPressedFlag = wasLongPressedFlag = false;
	reading = digitalRead(pin);
	switch (state) {
	case FIRST_CHANGE_STATE:
		if (reading != lastButtonState) {
			lastDebounceTime = currentMillis;
			state = DEBOUNCE_VALIDATION_STATE;
		}
		break;
	case DEBOUNCE_VALIDATION_STATE:
		if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY_MS) {
			if (reading == buttonState) {
				state = FIRST_CHANGE_STATE;
				break;
			}
			buttonState = reading;
			lastButtonState = buttonState;
			if (buttonState != LOW)
				break;
			isPressedFlag = wasPressedFlag = true;
			lastPressTime = currentMillis;
			state = LONGPRESS_VALIDATION_STATE;
		}
		break;
	case LONGPRESS_VALIDATION_STATE:
		if (reading != LOW) {
			state = FIRST_CHANGE_STATE;
			break;
		}
		wasPressedFlag = true;
		if (currentMillis - lastPressTime >= LONGPRESS_TIME_MS)
			wasLongPressedFlag = true;
		break;
	}
}

bool Button::isPressed()
{
	return isPressedFlag;
}

bool Button::wasLongPressed()
{
	return wasLongPressedFlag;
}

bool Button::wasPressed()
{
	return wasPressedFlag;
}
