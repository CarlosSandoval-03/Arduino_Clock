#include "Clock.h"

Clock::Clock(Display *d, Time *currT, Time *alarmT, Button *btnH, Button *btnM, Button *btnT, Button *btnA)
{
	display = d;
	currentTime = currT;
	alarmTime = alarmT;
	btnHours = btnH;
	btnMinutes = btnM;
	btnTimeOption = btnT;
	btnAlarmOption = btnA;
}

// TODO: Documentar por que es necesario el isPressed
void Clock::handleHoursButton(Time *selectedTime, Button *btn, unsigned long currentMillis)
{
	if (btn->isPressed())
		selectedTime->setHours(selectedTime->getHours() + 1);

	if (btn->wasLongPressed()) {
		if (currentMillis - lastUpdateTime >= LONGPRESS_UPDATE_DELAY) {
			selectedTime->setHours(selectedTime->getHours() + 1);
			lastUpdateTime = currentMillis;
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
		if (currentMillis - lastUpdateTime >= LONGPRESS_UPDATE_DELAY) {
			selectedTime->setMinutes(selectedTime->getMinutes() + 1);
			lastUpdateTime = currentMillis;
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
	if (true)
		display->print("ON"); // TODO: Add Switch support
	else
		display->print("OFF");
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
	currentTime->setTime(12, 1, 0);
	alarmTime->setTime(12, 11, 0);
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
}

void Clock::draw()
{
	// Update memory display
	displayTime();
	displayAlarm();
}

void Clock::schedIncrement()
{
	currentTime->incrementSeconds();
}
