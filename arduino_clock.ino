#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Display.h"
#include "Time.h"
#include "Button.h"
#include "Clock.h"

#define START_ANIM_POS_X 32
#define START_ANIM_POS_Y 0

void updateTime();

Display display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Time currentTime;
Time alarmTime;
Button buttonTimeOption(4);
Button buttonAlarmOption(3);
Button buttonHours(6);
Button buttonMinutes(5);
const int switchPin = 2;
const int ledPin = 12;

Clock clock(&display, &currentTime, &alarmTime, &buttonHours, &buttonMinutes, &buttonTimeOption, &buttonAlarmOption,
						switchPin, ledPin);
unsigned long currentMillis;

int start_frame = 0;

void setup()
{
	Serial.begin(9600);

	// Initialize screen
	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		Serial.println(F("SSD1306 allocation failed"));
		for (;;)
			;
	}
	display.clearDisplay();

	// Initialize clock
	clock.init();

	// Sched the interrupt (1 sec - updateTime func)
	Timer1.initialize(1000000);
	Timer1.attachInterrupt(updateTime);
}

void loop()
{
	currentMillis = millis();
	clock.update(currentMillis);
	display.clearDisplay();
	clock.draw();
	display.display();
	delay(100);
}

void updateTime()
{
	clock.schedIncrement();
}
