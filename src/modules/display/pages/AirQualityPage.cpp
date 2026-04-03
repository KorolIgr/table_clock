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
     // 6: blank
     // 7: Sensor status
    
    switch (displayIndex) {
        case 0: {
            displayTitle(display, "AIR");
            break;
        }
        case 1: {
            displayTitle(display, "T");
            String value = String((int)round(data.temperature)) + "C";
            displayValue(display, value, 45);
            break;
        }
        case 2: {
            displayTitle(display, "H");
            String value = String((int)round(data.humidity)) + "%";
            displayValue(display, value, 45);
            break;
        }
        case 3: {
            displayTitle(display, "TVOC");
            String value = String((int)round(data.tvoc)) + " ppb";
            displayValue(display, value, 45);
            break;
        }
        case 4: {
            displayTitle(display, "eCO2");
            String value = String((int)round(data.eco2)) + " ppm";
            displayValue(display, value, 45);
            break;
        }
        case 5: {
            displayTitle(display, "AQI");
            String value = String(data.aqi);
            displayValue(display, value, 45);
            break;
        }
        case 6: {
            displayTitle(display, "STATUS");
            String value = String(data.flags);
            displayValue(display, value, 45);
            break;
        }
        case 7: {
            display->clear();
            break;
        }
    }
}
