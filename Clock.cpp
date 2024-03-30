#include "Clock.h"

Clock::Clock(Display *d, Time *currT, Time *alarmT, Button *btnH, Button *btnM, Button *btnT, Button *btnA, int swtPin, int lPin)
{
	display = d;
	currentTime = currT;
	alarmTime = alarmT;
	btnHours = btnH;
	btnMinutes = btnM;
	btnTimeOption = btnT;
	btnAlarmOption = btnA;
	switchAlarmPin = swtPin;
	ledPin = lPin;
}

// TODO: Documentar por que es necesario el isPressed
void Clock::handleHoursButton(Time *selectedTime, Button *btn, unsigned long currentMillis)
{
	if (btn->isPressed())
		selectedTime->setHours(selectedTime->getHours() + 1);

	if (btn->wasLongPressed()) {
		if (currentMillis - lastUpdateButtonTime >= LONGPRESS_UPDATE_DELAY) {
			selectedTime->setHours(selectedTime->getHours() + 1);
			lastUpdateButtonTime = currentMillis;
		}
	}

	if (selectedTime->getHours() > 23)
		selectedTime->setHours(0);
}

void Clock::handleMinutesButton(Time *selectedTime, Button *btn, unsigned long currentMillis)
{
	if (btn->isPressed())
		selectedTime->setMinutes(selectedTime->getMinutes() + 1);

	if (btn->wasLongPressed()) {
		if (currentMillis - lastUpdateButtonTime >= LONGPRESS_UPDATE_DELAY) {
			selectedTime->setMinutes(selectedTime->getMinutes() + 1);
			lastUpdateButtonTime = currentMillis;
		}
	}

	if (selectedTime->getMinutes() > 59)
		selectedTime->setMinutes(0);
}

void Clock::setTime(unsigned long currentMillis)
{
	currentTime->setSeconds(0);

	if (btnHours->wasPressed()) {
		handleHoursButton(currentTime, btnHours, currentMillis);
	} else if (btnMinutes->wasPressed()) {
		handleMinutesButton(currentTime, btnMinutes, currentMillis);
	}
}

void Clock::setAlarm(unsigned long currentMillis)
{
	alarmTime->setSeconds(0);

	if (btnHours->wasPressed()) {
		handleHoursButton(alarmTime, btnHours, currentMillis);
	} else if (btnMinutes->wasPressed()) {
		handleMinutesButton(alarmTime, btnMinutes, currentMillis);
	}
}

void Clock::checkButtons(unsigned long currentMillis)
{
	// Avoid collisions
	if (btnTimeOption->wasPressed() && btnAlarmOption->wasPressed()) {
		Serial.println("Error: Estan presionando ambas opciones al tiempo");
		return;
	}

	if (btnTimeOption->wasPressed()) {
		setTime(currentMillis);
	} else if (btnAlarmOption->wasPressed()) {
		setAlarm(currentMillis);
	}
}

void Clock::displayTime()
{
	display->setTextSize(2);
	display->setTextColor(WHITE);
	display->setCursor(0, 0);
	display->print("TIME:");
	display->setCursor(15, 20);
	display->print(currentTime->getHours() < 10 ? "0" : "");
	display->print(currentTime->getHours());
	display->print(":");
	display->print(currentTime->getMinutes() < 10 ? "0" : "");
	display->print(currentTime->getMinutes());
	display->print(":");
	display->print(currentTime->getSeconds() < 10 ? "0" : "");
	display->print(currentTime->getSeconds());
}

void Clock::displayAlarm()
{
	display->setTextSize(1);
	display->setTextColor(WHITE);
	display->setCursor(0, 40);
	display->print("ALARM:");
	display->setCursor(20, 52);
	display->print(alarmTime->getHours() < 10 ? "0" : "");
	display->print(alarmTime->getHours());
	display->print(":");
	display->print(alarmTime->getMinutes() < 10 ? "0" : "");
	display->print(alarmTime->getMinutes());
	display->print(":");
	display->print(alarmTime->getSeconds() < 10 ? "0" : "");
	display->print(alarmTime->getSeconds());

	display->setTextSize(2);
	display->setCursor(80, 48);
	if (digitalRead(switchAlarmPin))
		display->print("ON");
	else
		display->print("OFF");
}

void Clock::blinkLed(unsigned long currentMillis)
{
	static bool ledState = LOW;
	if (!digitalRead(switchAlarmPin))
	{
		digitalWrite(ledPin, LOW);
		return;
	}
	
	if (currentMillis - lastUpdateLedTime >= LED_BLINK_DELAY)
	{
		lastUpdateLedTime = currentMillis;
		ledState = !ledState;
		digitalWrite(ledPin, ledState);
	}
}

void Clock::blinkAnimation(unsigned long currentMillis)
{
	if (!isAlarmActive)
		return;

	if (currentMillis - lastAnimationTime >= ANIMATION_BLINK_DELAY && !isAnimActive)
	{
		// The last animation time is updated in draw function
		isAnimActive = true;
	}
}

Display *Clock::getDisplay()
{
	return display;
}

Time *Clock::getTimeObj()
{
	return currentTime;
}

Time *Clock::getAlarmObj()
{
	return alarmTime;
}

Button *Clock::getBtnHours()
{
	return btnHours;
}

Button *Clock::getBtnMinutes()
{
	return btnMinutes;
}

Button *Clock::getBtnTimeOption()
{
	return btnTimeOption;
}

Button *Clock::getBtnAlarmOption()
{
	return btnAlarmOption;
}

void Clock::setDisplay(Display *display)
{
	this->display = display;
}

void Clock::setTimeObj(Time *time)
{
	currentTime = time;
}

void Clock::setAlarmObj(Time *time)
{
	alarmTime = time;
}

void Clock::setBtnHours(Button *btn)
{
	btnHours = btn;
}

void Clock::setBtnMinutes(Button *btn)
{
	btnMinutes = btn;
}

void Clock::setBtnTimeOption(Button *btn)
{
	btnTimeOption = btn;
}

void Clock::setBtnAlarmOption(Button *btn)
{
	btnAlarmOption = btn;
}

void Clock::init()
{
	pinMode(ledPin, OUTPUT);

	currentTime->setTime(12, 1, 0);
	alarmTime->setTime(12, 11, 0);
}

void Clock::checkAlarm()
{
	if (currentTime->getHours() != alarmTime->getHours() || currentTime->getMinutes() != alarmTime->getMinutes())
	{
		isAlarmActive = false;
		return;
	}
	isAlarmActive = true;
}

void Clock::update(unsigned long currentMillis)
{
	// Update Buttons State
	btnHours->update(currentMillis);
	btnMinutes->update(currentMillis);
	btnTimeOption->update(currentMillis);
	btnAlarmOption->update(currentMillis);

	// Check changes
	checkButtons(currentMillis);
	checkAlarm();
	
	// Blink led
	if (isAlarmActive)
		blinkLed(currentMillis);
	
	// Animation setup
	blinkAnimation(currentMillis);
}

void Clock::draw()
{
	// Setup animation
	if (isAlarmActive && isAnimActive)
	{
		display->drawBitmap(32, 0, clarr[currFrame], FRAME_WIDTH, FRAME_HEIGHT, 1);
		
		// 1 iteration of animation
		if (currFrame + 1 == FRAME_COUNT)
		{
			isAnimActive = false;
			lastAnimationTime = millis();
		}

		// Update frame value
		currFrame = (currFrame + 1) % FRAME_COUNT;

		// Avoid print clock and alarm when the animation is active
		return;
	}

	// Update memory display
	displayTime();
	displayAlarm();
}

void Clock::schedIncrement()
{
	currentTime->incrementSeconds();
}
