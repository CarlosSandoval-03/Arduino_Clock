#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Display.h"
#include "Time.h"
#include "Button.h"
#include "Clock.h"

void updateTime();

Display display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Time currentTime;
Time alarmTime;
Button buttonTimeOption(6);
Button buttonAlarmOption(5);
Button buttonHours(4);
Button buttonMinutes(3);

Clock clock(&display, &currentTime, &alarmTime, &buttonTimeOption, &buttonAlarmOption, &buttonHours, &buttonMinutes);
unsigned long currentMillis;

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
