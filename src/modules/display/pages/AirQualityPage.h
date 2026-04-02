#ifndef AIR_QUALITY_PAGE_H
#define AIR_QUALITY_PAGE_H

#include <Arduino.h>
#include "../../data_storage/DataStorage.h"
#include <U8g2lib.h>

class AirQualityPage {
public:
    AirQualityPage(DataStorage* dataStorage);
    ~AirQualityPage() = default;
    
    void render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex);
    
private:
    DataStorage* _dataStorage;
};

#endif // AIR_QUALITY_PAGE_H
