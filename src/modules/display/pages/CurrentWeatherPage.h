#ifndef CURRENT_WEATHER_PAGE_H
#define CURRENT_WEATHER_PAGE_H

#include <Arduino.h>
#include "../../data_storage/DataStorage.h"
#include <U8g2lib.h>
#include <avr/pgmspace.h>

class CurrentWeatherPage {
public:
    CurrentWeatherPage(DataStorage* dataStorage);
    ~CurrentWeatherPage() = default;
    
    void render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex);
    
private:
    DataStorage* _dataStorage;
    
    // Helper to format temperature
    String formatTemp(float temp) const;
    // Helper to format wind direction from degrees to compass
    String formatWindDirection(int degrees) const;
    // Helper to get weather symbol (simple character representation)
    char getWeatherSymbol(int code) const;
};

#endif // CURRENT_WEATHER_PAGE_H
