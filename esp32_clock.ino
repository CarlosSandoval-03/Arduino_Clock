#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Display.h"
#include "Time.h"
#include "Button.h"
#include "Clock.h"

void updateTime();

hw_timer_t *timer = NULL;

Display display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Time currentTime;
Time alarmTime;
Button buttonTimeOption(25);
Button buttonAlarmOption(26);
Button buttonHours(33);
Button buttonMinutes(32);
const int switchPin = 5;
const int ledPin = 13;

Main_Clock main_clock(&display, &currentTime, &alarmTime, &buttonHours, &buttonMinutes, &buttonTimeOption,
											&buttonAlarmOption, switchPin, ledPin);
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

	// Initialize main_clock
	main_clock.init();

	// Sched the interrupt (1 sec - updateTime func)
	timer = timerBegin(0, 80, true); // Timer 0, divisor de reloj 80
	timerAttachInterrupt(timer, &updateTime, true); // Adjuntar la función de manejo de interrupción
	timerAlarmWrite(timer, 1000000, true); // Interrupción cada 1 segundo
	timerAlarmEnable(timer); // Habilitar la alarma
}

void loop()
{
	currentMillis = millis();
	main_clock.update(currentMillis);
	display.clearDisplay();
	main_clock.draw();
	display.display();
	delay(100);
}

void updateTime()
{
	main_clock.schedIncrement();
}
