#include "CurrentWeatherPage.h"
#include "../Icons.h"
#include "../DisplayUtils.h"

CurrentWeatherPage::CurrentWeatherPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void CurrentWeatherPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    if (!_dataStorage) return;
    
    WeatherData& data = _dataStorage->weather();
    
    if (!data.current.valid) {
        // Show error or "no data" message
        display->setFont(u8g2_font_10x20_tf);
        if (displayIndex == 0) {
            display->drawStr(0, 20, "Current");
        } else if (displayIndex == 1) {
            display->drawStr(0, 20, "No data");
        }
        return;
    }

     switch (displayIndex) {
       case 0: {
           display->drawXBMP(0,0,L_ICON_WIDTH,L_ICON_HEIGHT,weather_icon);
           displayTitle(display, "NOW", 50);
           break;
       }
       case 1: {
           displayTitle(display, "TEMPERATURE");
           String value = formatTemp(data.current.temperature);
           displayValue(display, value, 45);
           break;
       }
       case 2: {
           displayTitle(display, "FEELS LIKE");
           String value = formatTemp(data.current.apparent_temperature);
           displayValue(display, value, 45);
           break;
       }
       case 3: {
           displayTitle(display, "HUMIDITY");
           String value = String(data.current.humidity) + "%";
           displayValue(display, value, 45);
           break;
       }
       case 4: {
           displayTitle(display, "WIND");
           String speed = String(data.current.wind_speed, 1) + " m/s";
           String direction = formatWindDirection(data.current.wind_direction);
           displayValue(display, speed, 45);
           displayValue(display, direction, 65);
           break;
       }
       case 5: {
           displayTitle(display, "CLOUDS");
           String value = String(data.current.cloud_cover) + "%";
           displayValue(display, value, 45);
           break;
       }
       case 6: {
            displayTitle(display, "ICON");
            const unsigned char* symbol = getWeatherSymbol(data.current.weather_code);
            display->drawXBMP(0, 20, L_ICON_WIDTH, L_ICON_HEIGHT, symbol);
            break;
       }
       case 7: {
            display->clear();
           break;
       }
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
