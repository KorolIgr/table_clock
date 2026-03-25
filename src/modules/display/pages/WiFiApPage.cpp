#include "WiFiApPage.h"

WiFiApPage::WiFiApPage() {
}

void WiFiApPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    // Use larger fonts for better visibility
    display->setFont(u8g2_font_fub30_tf); // Large font for main info
    
    // Distribute content across 8 displays
    switch (displayIndex) {
        case 0:
            display->drawStr(0, 40, "Wi");
            display->drawStr(0, 80, "Fi");
            display->drawStr(0, 120, "AP");
            break;
        case 1:
            display->drawStr(0, 40, "IP");
            break;
        case 2:
            // Check if AP mode is active and show status
            if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA) {
                display->drawStr(0, 40, "ON");
            } else {
                display->drawStr(0, 40, "OFF");
            }
            break;
        case 3:
            // Show IP part if active
            if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA) {
                String apIp = WiFi.softAPIP().toString();
                int o1, o2, o3, o4;
                if (sscanf(apIp.c_str(), "%d.%d.%d.%d", &o1, &o2, &o3, &o4) == 4) {
                    char buf[4];
                    itoa(o1, buf, 10); 
                    display->drawStr(0, 40, buf);
                }
            } else {
                display->drawStr(0, 40, "-");
            }
            break;
        case 4:
            if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA) {
                String apIp = WiFi.softAPIP().toString();
                int o1, o2, o3, o4;
                if (sscanf(apIp.c_str(), "%d.%d.%d.%d", &o1, &o2, &o3, &o4) == 4) {
                    char buf[4];
                    itoa(o2, buf, 10);
                    display->drawStr(0, 40, buf);
                }
            } else {
                display->drawStr(0, 40, "-");
            }
            break;
        case 5:
            if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA) {
                String apIp = WiFi.softAPIP().toString();
                int o1, o2, o3, o4;
                if (sscanf(apIp.c_str(), "%d.%d.%d.%d", &o1, &o2, &o3, &o4) == 4) {
                    char buf[4];
                    itoa(o3, buf, 10);
                    display->drawStr(0, 40, buf);
                }
            } else {
                display->drawStr(0, 40, "-");
            }
            break;
        case 6:
            if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA) {
                String apIp = WiFi.softAPIP().toString();
                int o1, o2, o3, o4;
                if (sscanf(apIp.c_str(), "%d.%d.%d.%d", &o1, &o2, &o3, &o4) == 4) {
                    char buf[4];
                    itoa(o4, buf, 10);
                    display->drawStr(0, 40, buf);
                }
            } else {
                display->drawStr(0, 40, "-");
            }
            break;
        case 7:
            display->clear();
            break;
    }
}