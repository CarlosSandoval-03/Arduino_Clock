#pragma once

/* If the used library is Adafruit_SSD1306 */
#ifdef _Adafruit_SSD1306_H_

// Generic Typo for display
typedef Adafruit_SSD1306 Display;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// All colors (except black) is taken as WHITE
#define TFT_WHITE WHITE
#define TFT_BLACK BLACK
#define TFT_NAVY WHITE
#define TFT_DARKGREEN WHITE
#define TFT_DARKCYAN WHITE
#define TFT_MAROON WHITE
#define TFT_PURPLE WHITE
#define TFT_OLIVE WHITE
#define TFT_LIGHTGREY WHITE
#define TFT_DARKGREY WHITE
#define TFT_BLUE WHITE
#define TFT_GREEN WHITE
#define TFT_CYAN WHITE
#define TFT_RED WHITE
#define TFT_MAGENTA WHITE
#define TFT_YELLOW WHITE
#define TFT_ORANGE WHITE
#define TFT_GREENYELLOW WHITE
#define TFT_PINK WHITE
#define TFT_BROWN WHITE
#define TFT_GOLD WHITE
#define TFT_SILVER WHITE
#define TFT_SKYBLUE WHITE
#define TFT_VIOLET WHITE

/* If used the library _TFT_eSPIH_ */
#elif defined _TFT_eSPIH_
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 135

// Define BLACK and WHITE
#define WHITE TFT_WHITE
#define BLACK TFT_BLACK

// Generic Typo for display
typedef TFT_eSPI Display;

#endif

// General Definitions
#define OFF 0
#define ON 1
