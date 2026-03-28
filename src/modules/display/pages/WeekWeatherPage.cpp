#include "WeekWeatherPage.h"
#include "../../data_storage/DataStorage.h"
#include "WeatherIcons.h"

WeekWeatherPage::WeekWeatherPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void WeekWeatherPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    if (!_dataStorage) return;
    
    SharedData& data = _dataStorage->getData();
    
    if (!data.weather_valid) {
        // Show error or "no data" message
        display->setFont(u8g2_font_10x20_tf);
        if (displayIndex == 0) {
            display->drawStr(0, 20, "Weather");
        } else if (displayIndex == 1) {
            display->drawStr(0, 20, "No data");
        }
        return;
    }
    
    // Display 0 shows "Weather" label
    if (displayIndex == 0) {
        //display->setFont(u8g2_font_fub30_tf);
        //display->drawStr(0, 40, "WTH");
        display->drawXBMP(0,0,WEATHER_ICON_WIDTH,WEATHER_ICON_HEIGHT,sun_icon);
        display->drawStr(0, 50, "WEEK"); // 8px line spacing

        return;
    }
    
    // Displays 1-7 show each day's forecast (displayIndex 1 = day 0, displayIndex 7 = day 6)
    int dayIndex = displayIndex - 1;
    if (dayIndex >= 0 && dayIndex < 7) {
        WeatherDay day = data.weather_forecast[dayIndex];
        
        // Format date to MM-DD
        String dateStr = formatDate(day.date);
        String tempMaxStr = formatTemp(day.temp_max);
        String tempMinStr = formatTemp(day.temp_min);
        char symbol = ::getWeatherSymbol(day.weather_code);
        
        // Layout with proper vertical spacing for 128x64 display
        // Line 1: date (y=20)
        display->setFont(u8g2_font_fub14_tf);
        display->drawStr(0, 20, dateStr.c_str());
        
        // Line 2: temperatures (y=45)
        display->setFont(u8g2_font_fub20_tf);
        display->drawStr(0, 45, tempMaxStr.c_str());
        display->drawStr(0, 70, tempMinStr.c_str());
        
        // Line 3: weather symbol (y=60) in medium font
        display->setFont(u8g2_font_fub14_tf);
        char symStr[2] = { symbol, '\0' };
        display->drawStr(0, 100, symStr);
    }
}

String WeekWeatherPage::formatTemp(float temp) const {
    // Format as integer degrees Celsius
    int tempInt = (int)round(temp);
    return String(tempInt) + "C";
}

String WeekWeatherPage::formatDate(const String& fullDate) const {
    // fullDate format: YYYY-MM-DD
    if (fullDate.length() >= 10) {
        // Extract MM-DD
        return fullDate.substring(5, 10); // MM-DD
    }
    return fullDate;
}
