#ifndef CURRENT_WEATHER_PAGE_H
#define CURRENT_WEATHER_PAGE_H

#include <Arduino.h>
#include "../../data_storage/DataStorage.h"
#include <U8g2lib.h>
#include <avr/pgmspace.h>
#include "BasePage.h"

class CurrentWeatherPage : public BasePage {
public:
    CurrentWeatherPage(DataStorage* dataStorage);
    ~CurrentWeatherPage() = default;

protected:
    void onDisplay0(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) override;
    void onDisplay1(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) override;
    void onDisplay2(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) override;
    void onDisplay3(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) override;
    void onDisplay4(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) override;
    void onDisplay5(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) override;
    void onDisplay6(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) override;
    void onDisplay7(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) override;

private:
    DataStorage* _dataStorage;

    // Helper to format temperature
    String formatTemp(float temp) const;
    // Helper to format wind direction from degrees to compass
    String formatWindDirection(int degrees) const;
};

#endif // CURRENT_WEATHER_PAGE_H
