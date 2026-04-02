#include "AirQualityPage.h"
#include "../DisplayUtils.h"

AirQualityPage::AirQualityPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void AirQualityPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    if (!_dataStorage) return;
    
    AirQualityData& data = _dataStorage->airQuality();
    
    // Display layout based on index:
    // 0: "AIR" heading
    // 1: Temperature
    // 2: Humidity
    // 3: TVOC
    // 4: eCO2
    // 5: AQI
    // 6-7: blank
    
    switch (displayIndex) {
        case 0: {
            displayTitle(display, "AIR QUALITY");
            break;
        }
        case 1: {
            displayTitle(display, "TEMPERATURE");
            if (data.valid || data.temperature != 0) {
                String value = String((int)round(data.temperature)) + "C";
                displayValue(display, value, 45, u8g2_font_10x20_tf, 20);
            } else {
                displayValue(display, "--", 45, u8g2_font_10x20_tf, 20);
            }
            break;
        }
        case 2: {
            displayTitle(display, "HUMIDITY");
            if (data.valid || data.humidity != 0) {
                String value = String((int)round(data.humidity)) + "%";
                displayValue(display, value, 45, u8g2_font_10x20_tf, 20);
            } else {
                displayValue(display, "--", 45, u8g2_font_10x20_tf, 20);
            }
            break;
        }
        case 3: {
            displayTitle(display, "TVOC");
            if (data.valid) {
                String value = String((int)round(data.tvoc)) + " ppb";
                displayValue(display, value, 45, u8g2_font_10x20_tf, 20);
            } else {
                displayValue(display, "--", 45, u8g2_font_10x20_tf, 20);
            }
            break;
        }
        case 4: {
            displayTitle(display, "eCO2");
            if (data.valid) {
                String value = String((int)round(data.eco2)) + " ppm";
                displayValue(display, value, 45, u8g2_font_10x20_tf, 20);
            } else {
                displayValue(display, "--", 45, u8g2_font_10x20_tf, 20);
            }
            break;
        }
        case 5: {
            displayTitle(display, "AQI");
            if (data.valid && data.aqi > 0) {
                String value = String(data.aqi);
                // Color code based on AQI
                display->setFont(u8g2_font_fub30_tf);
                display->setDrawColor(1);
                int xPos = (128 - display->getStrWidth(value.c_str())) / 2;
                display->drawStr(xPos, 45, value.c_str());
            } else {
                display->setFont(u8g2_font_fub30_tf);
                display->setDrawColor(1);
                String value = "--";
                int xPos = (128 - display->getStrWidth(value.c_str())) / 2;
                display->drawStr(xPos, 45, value.c_str());
            }
            break;
        }
        case 6: {
            display->clear();
            break;
        }
        case 7: {
            display->clear();
            break;
        }
    }
}
