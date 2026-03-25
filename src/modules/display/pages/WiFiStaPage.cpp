#include "WiFiStaPage.h"

WiFiStaPage::WiFiStaPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void WiFiStaPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    if (!_dataStorage) return;
    
    SharedData& data = _dataStorage->getData();
    
    display->drawStr(0, 10, "WiFi STA:");
    
    if (data.wifi_connected) {
        display->drawStr(0, 20, "Status: Connected");
        display->drawStr(0, 30, "IP:");
        display->drawStr(20, 30, data.ip_address.c_str());
    } else {
        display->drawStr(0, 20, "Status: Disconnected");
        display->drawStr(0, 30, "SSID:");
        display->drawStr(30, 30, data.sta_ssid.c_str());
    }
    
    // Show STA credentials if set
    if (!data.sta_ssid.isEmpty()) {
        display->drawStr(0, 40, "Target:");
        display->drawStr(40, 40, data.sta_ssid.c_str());
    }
}