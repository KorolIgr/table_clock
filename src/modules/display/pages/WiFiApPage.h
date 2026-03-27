#ifndef WIFI_AP_PAGE_H
#define WIFI_AP_PAGE_H

#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include "../../data_storage/DataStorage.h"

class WiFiApPage {
public:
    WiFiApPage(DataStorage* dataStorage);
    void render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex);

private:
    DataStorage* _dataStorage;
};

#endif // WIFI_AP_PAGE_H