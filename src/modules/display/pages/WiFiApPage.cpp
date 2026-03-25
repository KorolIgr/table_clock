#include "WiFiApPage.h"

WiFiApPage::WiFiApPage() {
}

void WiFiApPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    display->drawStr(0, 10, "WiFi AP:");
    display->drawStr(0, 20, "Mode: Access Point");
    
    // Check if AP mode is active
    if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA) {
        String apIp = WiFi.softAPIP().toString();
        if (apIp != "0.0.0.0") {
            display->drawStr(0, 30, "IP:");
            display->drawStr(20, 30, apIp.c_str());
        } else {
            display->drawStr(0, 30, "No IP");
        }
        display->drawStr(0, 40, "Active");
    } else {
        display->drawStr(0, 30, "Inactive");
        display->drawStr(0, 40, "SSID: ESP_TableClock");
    }
}