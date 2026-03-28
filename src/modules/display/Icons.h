#ifndef ICONS_H
#define ICONS_H

#include <Arduino.h>
#include <avr/pgmspace.h>

// Icon dimensions
const uint8_t L_ICON_WIDTH = 32;
const uint8_t L_ICON_HEIGHT = 32;

// Sun icon (24x24 XBMP)
extern const unsigned char weather_icon[] PROGMEM;

// WiFi icon (32x32 XBMP)
extern const unsigned char wifi_icon[] PROGMEM;

// Geo/globe icon (32x32 XBMP)
extern const unsigned char geo_icon[] PROGMEM;

// Function to get weather symbol character based on WMO code
char getWeatherSymbol(int code);

#endif // ICONS_H
