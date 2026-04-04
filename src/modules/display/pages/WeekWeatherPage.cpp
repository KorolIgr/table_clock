#include "WeekWeatherPage.h"
#include "../../data_storage/DataStorage.h"
#include "../Icons.h"
#include "../DisplayUtils.h"

WeekWeatherPage::WeekWeatherPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void WeekWeatherPage::onDisplay0(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.forecast.valid) {
        display->setFont(u8g2_font_10x20_tf);
        display->drawStr(0, 20, "Weather");
        return;
    }

    display->drawXBMP(0, 0, L_ICON_WIDTH, L_ICON_HEIGHT, weather_icon);
    displayTitle(display, "WEEK", 50);
}

void WeekWeatherPage::onDisplay1(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.forecast.valid) {
        display->setFont(u8g2_font_10x20_tf);
        return;
    }

    renderDay(display, 0);
}

void WeekWeatherPage::onDisplay2(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.forecast.valid) {
        display->setFont(u8g2_font_10x20_tf);
        return;
    }

    renderDay(display, 1);
}

void WeekWeatherPage::onDisplay3(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.forecast.valid) {
        display->setFont(u8g2_font_10x20_tf);
        return;
    }

    renderDay(display, 2);
}

void WeekWeatherPage::onDisplay4(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.forecast.valid) {
        display->setFont(u8g2_font_10x20_tf);
        return;
    }

    renderDay(display, 3);
}

void WeekWeatherPage::onDisplay5(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.forecast.valid) {
        display->setFont(u8g2_font_10x20_tf);
        return;
    }

    renderDay(display, 4);
}

void WeekWeatherPage::onDisplay6(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.forecast.valid) {
        display->setFont(u8g2_font_10x20_tf);
        return;
    }

    renderDay(display, 5);
}

void WeekWeatherPage::onDisplay7(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WeatherData& data = _dataStorage->weather();

    if (!data.forecast.valid) {
        display->setFont(u8g2_font_10x20_tf);
        return;
    }

    renderDay(display, 6);
}

void WeekWeatherPage::renderDay(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, int dayIndex) {
    if (dayIndex < 0 || dayIndex >= 7) return;

    WeatherDay day = _dataStorage->weather().forecast.forecast[dayIndex];

    // Format date to MM-DD
    String dateStr = formatDate(day.date);
    String tempMaxStr = formatTemp(day.temp_max);
    String tempMinStr = formatTemp(day.temp_min);
    const unsigned char* symbol = ::getWeatherSymbol(day.weather_code);

    String title = "DAY ";
    title += (dayIndex + 1);
    displayTitle(display, title, 20);

    displayValue(display, dateStr, 45);

    displayValue(display, tempMaxStr.c_str(), 70);
    display->drawLine(0, 73, 64, 73);
    displayValue(display, tempMinStr.c_str(), 90);

    display->drawXBMP(0, 100, L_ICON_WIDTH, L_ICON_HEIGHT, symbol);
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
