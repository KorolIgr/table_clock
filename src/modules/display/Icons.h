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

extern const unsigned char weather_icon_clear[] PROGMEM ;// Clear
extern const unsigned char weather_icon_cloudy[] PROGMEM ; // Cloudy
extern const unsigned char weather_icon_rain[] PROGMEM ; // Rain
extern const unsigned char weather_icon_drizzle[] PROGMEM ; // Drizzle
extern const unsigned char weather_icon_snow[] PROGMEM ; // Snow
extern const unsigned char weather_icon_fog[] PROGMEM ; // Fog
extern const unsigned char weather_icon_thunderstorm[] PROGMEM ; // Thunderstorm
extern const unsigned char weather_icon_unknown[] PROGMEM ; // Unknown

// Function to get weather symbol character based on WMO code
const unsigned char* getWeatherSymbol(int code);

#endif // ICONS_H
