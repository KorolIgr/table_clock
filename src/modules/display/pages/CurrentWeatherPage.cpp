#include "CurrentWeatherPage.h"
#include "../Icons.h"
#include "../DisplayUtils.h"

CurrentWeatherPage::CurrentWeatherPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void CurrentWeatherPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    if (!_dataStorage) return;
    
    SharedData& data = _dataStorage->getData();
    
    if (!data.current_weather_valid) {
        // Show error or "no data" message
        display->setFont(u8g2_font_10x20_tf);
        if (displayIndex == 0) {
            display->drawStr(0, 20, "Current");
        } else if (displayIndex == 1) {
            display->drawStr(0, 20, "No data");
        }
        return;
    }
    
    // Display 0 shows "CURRENT" label
    if (displayIndex == 0) {
        display->drawXBMP(0,0,L_ICON_WIDTH,L_ICON_HEIGHT,weather_icon);
        displayTitle(display, "NOW", 50);

        return;
    }
    
    // Display 1: Temperature (current and apparent)
    if (displayIndex == 1) {
        String tempStr = formatTemp(data.current_temperature);
        String feelsStr = formatTemp(data.current_apparent_temperature);
        char symbol = getWeatherSymbol(data.current_weather_code);
        
        displayTitle(display, "TEMP");
        
        display->setFont(u8g2_font_fub20_tf);
        display->drawStr(0, 45, tempStr.c_str());
        
        display->setFont(u8g2_font_fub14_tf);
        char symStr[2] = { symbol, '\0' };
        display->drawStr(110, 45, symStr);
        
        display->setFont(u8g2_font_10x20_tf);
        display->drawStr(0, 60, ("Feels: " + feelsStr).c_str());
        return;
    }
    
    // Display 2: Wind speed and direction
    if (displayIndex == 2) {
        String windStr = String(data.current_wind_speed, 1) + " m/s";
        String dirStr = formatWindDirection(data.current_wind_direction);
        
        displayTitle(display, "WIND");
        
        display->setFont(u8g2_font_fub20_tf);
        display->drawStr(0, 45, windStr.c_str());
        
        display->setFont(u8g2_font_fub14_tf);
        display->drawStr(0, 65, dirStr.c_str());
        return;
    }
    
    // Display 3: Humidity
    if (displayIndex == 3) {
        String humStr = String(data.current_humidity) + "%";

        displayTitle(display, "HUMIDITY");
        
        display->setFont(u8g2_font_fub30_tf);
        display->drawStr(0, 55, humStr.c_str());
        return;
    }
    
    // Display 4: Cloud cover
    if (displayIndex == 4) {
        String cloudStr = String(data.current_cloud_cover) + "%";

        displayTitle(display, "CLOUDS");
        
        display->setFont(u8g2_font_fub30_tf);
        display->drawStr(0, 55, cloudStr.c_str());
        return;
    }
    
    // Display 5: Weather code / symbol
    if (displayIndex == 5) {
        char symbol = getWeatherSymbol(data.current_weather_code);

        displayTitle(display, "COND");
        
        display->setFont(u8g2_font_fub30_tf);
        char symStr[2] = { symbol, '\0' };
        display->drawStr(0, 55, symStr);
        return;
    }
}

String CurrentWeatherPage::formatTemp(float temp) const {
    int tempInt = (int)round(temp);
    return String(tempInt) + "C";
}

String CurrentWeatherPage::formatWindDirection(int degrees) const {
    const String directions[] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE",
                                "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};
    int index = (int)round(degrees / 22.5) % 16;
    return directions[index];
}
