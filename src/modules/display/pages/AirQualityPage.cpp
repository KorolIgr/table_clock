#include "AirQualityPage.h"
#include "../DisplayUtils.h"

AirQualityPage::AirQualityPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void AirQualityPage::onDisplay0(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    displayTitle(display, "AIR");
}

void AirQualityPage::onDisplay1(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    AirQualityData& data = _dataStorage->airQuality();

    displayTitle(display, "T");
    String value = String((int)round(data.temperature)) + "C";
    displayValue(display, value, 45);
}

void AirQualityPage::onDisplay2(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    AirQualityData& data = _dataStorage->airQuality();

    displayTitle(display, "H");
    String value = String((int)round(data.humidity)) + "%";
    displayValue(display, value, 45);
}

void AirQualityPage::onDisplay3(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    AirQualityData& data = _dataStorage->airQuality();

    displayTitle(display, "TVOC");
    String value = String((int)round(data.tvoc)) + " ppb";
    displayValue(display, value, 45);
}

void AirQualityPage::onDisplay4(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    AirQualityData& data = _dataStorage->airQuality();

    displayTitle(display, "eCO2");
    String value = String((int)round(data.eco2)) + " ppm";
    displayValue(display, value, 45);
}

void AirQualityPage::onDisplay5(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    AirQualityData& data = _dataStorage->airQuality();

    displayTitle(display, "AQI");
    String value = String(data.aqi);
    displayValue(display, value, 45);
}

void AirQualityPage::onDisplay6(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    AirQualityData& data = _dataStorage->airQuality();

    displayTitle(display, "STATUS");
    String value = String(data.flags);
    displayValue(display, value, 45);
}

void AirQualityPage::onDisplay7(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    display->clear();
}
