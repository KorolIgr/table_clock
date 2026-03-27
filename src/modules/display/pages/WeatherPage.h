#ifndef WEATHER_PAGE_H
#define WEATHER_PAGE_H

#include <Arduino.h>
#include "../../data_storage/DataStorage.h"
#include <U8g2lib.h>
#include <avr/pgmspace.h>

class WeatherPage {
public:
    WeatherPage(DataStorage* dataStorage);
    ~WeatherPage() = default;
    
    void render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex);
    
private:
    DataStorage* _dataStorage;
    
    // Helper to format temperature
    String formatTemp(float temp) const;
    // Helper to format date (extract MM-DD from YYYY-MM-DD)
    String formatDate(const String& fullDate) const;
    // Helper to get weather symbol (simple character representation)
    char getWeatherSymbol(int code) const;

    static const unsigned char sun_icon[] PROGMEM;
};

#endif // WEATHER_PAGE_H