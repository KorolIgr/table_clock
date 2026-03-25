#include "WeatherPage.h"
#include "../../data_storage/DataStorage.h"

WeatherPage::WeatherPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void WeatherPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    if (!_dataStorage) return;
    
    SharedData& data = _dataStorage->getData();
    
    if (!data.weather_valid) {
        // Show error or "no data" message
        display->setFont(u8g2_font_6x10_tf);
        if (displayIndex == 0) {
            display->drawStr(0, 20, "WTH");
        } else if (displayIndex == 1) {
            display->drawStr(0, 20, "No weather");
            display->drawStr(0, 35, "data");
        }
        return;
    }
    
    // Display 0 shows "WTH" label
    if (displayIndex == 0) {
        display->setFont(u8g2_font_fub30_tf);
        display->drawStr(0, 40, "WTH");
        return;
    }
    
    // Displays 1-7 show each day's forecast (displayIndex 1 = day 0, displayIndex 7 = day 6)
    int dayIndex = displayIndex - 1;
    if (dayIndex >= 0 && dayIndex < 7) {
        WeatherDay day = data.weather_forecast[dayIndex];
        
        // Format date to MM-DD
        String dateStr = formatDate(day.date);
        String tempStr = formatTemp(day.temp_max) + "/" + formatTemp(day.temp_min);
        char weatherCodeStr[8];
        snprintf(weatherCodeStr, sizeof(weatherCodeStr), "%d", day.weather_code);
        
        // Use smaller font for multi-line info
        display->setFont(u8g2_font_6x10_tf);
        
        // Line 1: date
        display->drawStr(0, 15, dateStr.c_str());
        
        // Line 2: temperatures
        display->drawStr(0, 30, tempStr.c_str());
        
        // Line 3: weather code (or symbol)
        display->drawStr(0, 45, weatherCodeStr);
    }
}

String WeatherPage::formatTemp(float temp) const {
    // Format as integer degrees Celsius
    int tempInt = (int)round(temp);
    return String(tempInt) + "C";
}

String WeatherPage::formatDate(const String& fullDate) const {
    // fullDate format: YYYY-MM-DD
    if (fullDate.length() >= 10) {
        // Extract MM-DD
        return fullDate.substring(5, 10); // MM-DD
    }
    return fullDate;
}

char WeatherPage::getWeatherSymbol(int code) const {
    // WMO Weather interpretation codes (WW)
    // Return a simple ASCII/Unicode symbol based on code
    if (code == 0) return 'O'; // Clear
    if (code <= 3) return 'C'; // Cloudy
    if (code <= 9) return 'R'; // Rain
    if (code <= 19) return 'D'; // Drizzle
    if (code <= 39) return 'S'; // Snow
    if (code <= 49) return 'F'; // Fog
    if (code <= 59) return 'R'; // Rain
    if (code <= 69) return 'S'; // Snow
    if (code <= 79) return 'R'; // Rain
    if (code <= 99) return 'T'; // Thunderstorm
    return '?';
}