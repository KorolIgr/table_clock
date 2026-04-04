#include "CurrentWeatherPage.h"
#include "../Icons.h"
#include "../DisplayUtils.h"

CurrentWeatherPage::CurrentWeatherPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void CurrentWeatherPage::onDisplay0(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.current.valid) {
        display->setFont(u8g2_font_10x20_tf);
        display->drawStr(0, 20, "Current");
        return;
    }

    display->drawXBMP(0, 0, L_ICON_WIDTH, L_ICON_HEIGHT, weather_icon);
    displayTitle(display, "NOW", 25);
}

void CurrentWeatherPage::onDisplay1(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.current.valid) {
        display->setFont(u8g2_font_10x20_tf);
        display->drawStr(0, 20, "No data");
        return;
    }

    displayTitle(display, "TEMPERATURE");
    String value = formatTemp(data.current.temperature);
    displayValue(display, value, 45);
}

void CurrentWeatherPage::onDisplay2(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.current.valid) {
        display->setFont(u8g2_font_10x20_tf);
        return;
    }

    displayTitle(display, "FEELS LIKE");
    String value = formatTemp(data.current.apparent_temperature);
    displayValue(display, value, 45);
}

void CurrentWeatherPage::onDisplay3(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.current.valid) {
        display->setFont(u8g2_font_10x20_tf);
        return;
    }

    displayTitle(display, "HUMIDITY");
    String value = String(data.current.humidity) + "%";
    displayValue(display, value, 45);
}

void CurrentWeatherPage::onDisplay4(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.current.valid) {
        display->setFont(u8g2_font_10x20_tf);
        return;
    }

    displayTitle(display, "WIND");
    String speed = String(data.current.wind_speed, 1) + " m/s";
    String direction = formatWindDirection(data.current.wind_direction);
    displayValue(display, speed, 22);
    displayValue(display, direction, 32);
}

void CurrentWeatherPage::onDisplay5(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.current.valid) {
        display->setFont(u8g2_font_10x20_tf);
        return;
    }

    displayTitle(display, "CLOUDS");
    String value = String(data.current.cloud_cover) + "%";
    displayValue(display, value, 45);
}

void CurrentWeatherPage::onDisplay6(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.current.valid) {
        display->setFont(u8g2_font_10x20_tf);
        return;
    }

    displayTitle(display, "ICON");
    const unsigned char* symbol = getWeatherSymbol(data.current.weather_code);
    display->drawXBMP(0, 10, L_ICON_WIDTH, L_ICON_HEIGHT, symbol);
}

void CurrentWeatherPage::onDisplay7(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    display->clear();
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
