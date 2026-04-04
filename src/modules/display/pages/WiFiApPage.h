#ifndef WIFI_AP_PAGE_H
#define WIFI_AP_PAGE_H

#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include "../../data_storage/DataStorage.h"
#include "BasePage.h"

class WiFiApPage : public BasePage {
public:
    WiFiApPage(DataStorage* dataStorage);

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
};

#endif // WIFI_AP_PAGE_H