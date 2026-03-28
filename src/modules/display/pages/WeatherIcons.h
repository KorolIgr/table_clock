#ifndef WEATHER_ICONS_H
#define WEATHER_ICONS_H

#include <Arduino.h>
#include <avr/pgmspace.h>

// Weather icon dimensions
const uint8_t WEATHER_ICON_WIDTH = 24;
const uint8_t WEATHER_ICON_HEIGHT = 24;

// Sun icon data (XBMP format)
extern const unsigned char sun_icon[] PROGMEM;

// Function to get weather symbol character based on WMO code
char getWeatherSymbol(int code);

#endif // WEATHER_ICONS_H
