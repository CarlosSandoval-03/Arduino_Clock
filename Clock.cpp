#include "Clock.h"

MainClock::MainClock(ClockConfig *config)
{
	display = config->display;
	currentTime = config->currentTime;
	alarmTime = config->alarmTime;
	btnHours = config->btnHours;
	btnMinutes = config->btnMinutes;
	btnTimeOption = config->btnTimeOption;
	btnAlarmOption = config->btnAlarmOption;
	switchAlarmPin = config->switchAlarmPin;
	ledPin = config->ledPin;
	buzzerpin = config->buzzerpin;
}

// TODO: Documentar por que es necesario el isPressed
void MainClock::handleHoursButton(Time *selectedTime, Button *btn, unsigned long currentMillis)
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

void MainClock::handleMinutesButton(Time *selectedTime, Button *btn, unsigned long currentMillis)
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

void MainClock::setTime(unsigned long currentMillis)
{
	currentTime->setSeconds(0);

	if (btnHours->wasPressed()) {
		handleHoursButton(currentTime, btnHours, currentMillis);
	} else if (btnMinutes->wasPressed()) {
		handleMinutesButton(currentTime, btnMinutes, currentMillis);
	}
}

void MainClock::setAlarm(unsigned long currentMillis)
{
	alarmTime->setSeconds(0);

	if (btnHours->wasPressed()) {
		handleHoursButton(alarmTime, btnHours, currentMillis);
	} else if (btnMinutes->wasPressed()) {
		handleMinutesButton(alarmTime, btnMinutes, currentMillis);
	}
}

void MainClock::checkButtons(unsigned long currentMillis)
{
	// Avoid collisions
	if (btnTimeOption->wasPressed() && btnAlarmOption->wasPressed()) {
		Serial.println("Error: Estan presionando ambas opciones al tiempo");
		return;
	}

	clockState = NORMAL_OPTION;
	if (btnTimeOption->wasPressed()) {
		clockState = (clockState != CLOCK_OPTION) ? CLOCK_OPTION : NORMAL_OPTION;
		setTime(currentMillis);
	} else if (btnAlarmOption->wasPressed()) {
		clockState = (clockState != ALARM_OPTION) ? ALARM_OPTION : NORMAL_OPTION;
		setAlarm(currentMillis);
	}
}

void MainClock::displayTime()
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

	// Change indicator
	if (clockState == CLOCK_OPTION) {
		display->setTextSize(2);
		display->setTextColor(WHITE);
		display->setCursor(110, 2);
		display->print("T");
	}
}

void MainClock::displayAlarm()
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

	// Change indicator
	if (clockState == ALARM_OPTION) {
		display->setTextSize(2);
		display->setTextColor(WHITE);
		display->setCursor(110, 2);
		display->print("A");
	}
}

void MainClock::blinkLed(unsigned long currentMillis)
{
	static bool ledState = LOW;
	if (!digitalRead(switchAlarmPin)) {
		digitalWrite(ledPin, LOW);
		Serial.println("S-OFF");
		return;
	}

	if (currentMillis - lastUpdateLedTime >= LED_BLINK_DELAY) {
		lastUpdateLedTime = currentMillis;
		ledState = !ledState;
		digitalWrite(ledPin, ledState);
		Serial.println(ledState ? "ON" : "OFF");
	}
}

void MainClock::blinkBuzzer(unsigned long currentMillis)
{
	static bool buzzerStatus = LOW;
	if (!digitalRead(switchAlarmPin)) {
		noTone(buzzerpin);
		return;
	}

	if (currentMillis - lastUpdateBuzzerTime >= BUZZER_BLINK_DELAY) {
		lastUpdateBuzzerTime = currentMillis;
		buzzerStatus = !buzzerStatus;
		if (buzzerStatus)
			tone(buzzerpin, 440);
		else
			noTone(buzzerpin);
	}
}

void MainClock::blinkAnimation(unsigned long currentMillis)
{
	if (!isAlarmActive)
		return;

	if (currentMillis - lastAnimationTime >= ANIMATION_BLINK_DELAY && !isAnimActive) {
		// The last animation time is updated in draw function
		isAnimActive = true;
	}
}

Display *MainClock::getDisplay()
{
	return display;
}

Time *MainClock::getTimeObj()
{
	return currentTime;
}

Time *MainClock::getAlarmObj()
{
	return alarmTime;
}

Button *MainClock::getBtnHours()
{
	return btnHours;
}

Button *MainClock::getBtnMinutes()
{
	return btnMinutes;
}

Button *MainClock::getBtnTimeOption()
{
	return btnTimeOption;
}

Button *MainClock::getBtnAlarmOption()
{
	return btnAlarmOption;
}

void MainClock::setDisplay(Display *display)
{
	this->display = display;
}

void MainClock::setTimeObj(Time *time)
{
	currentTime = time;
}

void MainClock::setAlarmObj(Time *time)
{
	alarmTime = time;
}

void MainClock::setBtnHours(Button *btn)
{
	btnHours = btn;
}

void MainClock::setBtnMinutes(Button *btn)
{
	btnMinutes = btn;
}

void MainClock::setBtnTimeOption(Button *btn)
{
	btnTimeOption = btn;
}

void MainClock::setBtnAlarmOption(Button *btn)
{
	btnAlarmOption = btn;
}

void MainClock::init()
{
	pinMode(ledPin, OUTPUT);

	currentTime->setTime(12, 1, 0);
	alarmTime->setTime(12, 11, 0);
}

void MainClock::checkAlarm()
{
	if (currentTime->getHours() != alarmTime->getHours() || currentTime->getMinutes() != alarmTime->getMinutes()) {
		isAlarmActive = false;
		return;
	}
	isAlarmActive = true;
}

void MainClock::update(unsigned long currentMillis)
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
	if (isAlarmActive) {
		blinkLed(currentMillis);
		blinkBuzzer(buzzerpin);
	}

	// Animation setup
	blinkAnimation(currentMillis);
}

void MainClock::draw()
{
	// Setup animation
	if (isAlarmActive && isAnimActive && digitalRead(switchAlarmPin)) {
		display->drawBitmap(32, 0, clarr[currFrame], FRAME_WIDTH, FRAME_HEIGHT, 1);

		// 1 iteration of animation
		if (currFrame + 1 == FRAME_COUNT) {
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

void MainClock::schedIncrement()
{
	currentTime->incrementSeconds();
}
