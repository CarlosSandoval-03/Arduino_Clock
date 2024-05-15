#pragma once

#include "Arduino.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Display.h"
#include "Time.h"
#include "Button.h"
#include "AlarmAnimation.h"

#define NORMAL_OPTION -1
#define CLOCK_OPTION 0
#define ALARM_OPTION 1
#define LONGPRESS_UPDATE_DELAY 200
#define LED_BLINK_DELAY 1000
#define BUZZER_BLINK_DELAY 1000
#define ANIMATION_BLINK_DELAY 3000

typedef struct {
	Display *display;
	Time *currentTime;
	Time *alarmTime;
	Button *btnHours;
	Button *btnMinutes;
	Button *btnTimeOption;
	Button *btnAlarmOption;
	int switchAlarmPin;
	int ledPin;
	int buzzerpin;
} ClockConfig;

class MainClock {
	private:
	/**
	 * The Clock class will only manipulate the Display memory, it will not be in charge of cleaning
	 * the screen or transferring the information in the memory to the screen
	 */
	Display *display;
	Time *currentTime;
	Time *alarmTime;
	Button *btnHours;
	Button *btnMinutes;
	Button *btnTimeOption;
	Button *btnAlarmOption;
	int switchAlarmPin;
	int ledPin;
	int buzzerpin;

	unsigned long lastUpdateButtonTime = 0;
	unsigned long lastUpdateLedTime = 0;
	unsigned long lastUpdateBuzzerTime = 0;
	boolean isAlarmActive = false;

	int currFrame = 0;
	boolean isAnimActive = false;
	unsigned long lastAnimationTime = 0;
	int clockState = NORMAL_OPTION;

	void handleHoursButton(Time *selectedTime, Button *btn, unsigned long currentMillis);
	void handleMinutesButton(Time *selectedTime, Button *btn, unsigned long currentMillis);
	void setTime(unsigned long currentMillis);
	void setAlarm(unsigned long currentMillis);
	void checkButtons(unsigned long currentMillis);
	void displayTime();
	void displayAlarm();
	void blinkLed(unsigned long currentMillis);
	void blinkBuzzer(unsigned long currentMillis);
	void blinkAnimation(unsigned long currentMillis);

	public:
	MainClock(ClockConfig *config);
	Display *getDisplay();
	Time *getTimeObj();
	Time *getAlarmObj();
	Button *getBtnHours();
	Button *getBtnMinutes();
	Button *getBtnTimeOption();
	Button *getBtnAlarmOption();
	void setDisplay(Display *display);
	void setTimeObj(Time *time);
	void setAlarmObj(Time *time);
	void setBtnHours(Button *btn);
	void setBtnMinutes(Button *btn);
	void setBtnTimeOption(Button *btn);
	void setBtnAlarmOption(Button *btn);

	void init();
	void checkAlarm();
	void update(unsigned long currentMillis);
	void draw();
	void schedIncrement();
};
