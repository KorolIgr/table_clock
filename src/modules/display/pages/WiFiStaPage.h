#ifndef WIFI_STA_PAGE_H
#define WIFI_STA_PAGE_H

#include <U8g2lib.h>
#include "../../data_storage/DataStorage.h"

class WiFiStaPage {
public:
    WiFiStaPage(DataStorage* dataStorage);
    void render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex);

private:
    DataStorage* _dataStorage;
};

#endif // WIFI_STA_PAGE_H