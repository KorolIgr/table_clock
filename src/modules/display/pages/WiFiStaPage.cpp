#include "WiFiStaPage.h"

WiFiStaPage::WiFiStaPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void WiFiStaPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    if (!_dataStorage) return;
    
    SharedData& data = _dataStorage->getData();
    
    // Use larger fonts for better visibility
    display->setFont(u8g2_font_fub30_tf); // Large font for main info
    
    // Distribute content across 8 displays
    switch (displayIndex) {
        case 0:
            display->drawStr(0, 40, "Wi");
            display->drawStr(0, 80, "Fi");
            display->drawStr(0, 120, "STA");
            break;
        case 1:
            if (data.wifi_connected) {
                display->drawStr(0, 40, "C");
            } else {
                display->drawStr(0, 40, "D");
            }
            break;
        case 2:
            display->drawStr(0, 40, "IP");
            break;
        case 3:
             if (data.wifi_connected) {
                String ip = data.ip_address.c_str();
                int o1, o2, o3, o4;
                if (sscanf(ip.c_str(), "%d.%d.%d.%d", &o1, &o2, &o3, &o4) == 4) {
                    char buf[4];
                    itoa(o1, buf, 10); 
                    display->drawStr(0, 40, buf);
                }
             } else {
                display->drawStr(0, 40, "-");
             }
            break;
        case 4:
            if (data.wifi_connected) {
                String ip = data.ip_address.c_str();
                int o1, o2, o3, o4;
                if (sscanf(ip.c_str(), "%d.%d.%d.%d", &o1, &o2, &o3, &o4) == 4) {
                    char buf[4];
                    itoa(o2, buf, 10); 
                    display->drawStr(0, 40, buf);
                }
             } else {
                display->drawStr(0, 40, "-");
             }
            break;
        case 5:
            if (data.wifi_connected) {
                String ip = data.ip_address.c_str();
                int o1, o2, o3, o4;
                if (sscanf(ip.c_str(), "%d.%d.%d.%d", &o1, &o2, &o3, &o4) == 4) {
                    char buf[4];
                    itoa(o3, buf, 10); 
                    display->drawStr(0, 40, buf);
                }
             } else {
                display->drawStr(0, 40, "-");
             }
            break;
        case 6:
            if (data.wifi_connected) {
                String ip = data.ip_address.c_str();
                int o1, o2, o3, o4;
                if (sscanf(ip.c_str(), "%d.%d.%d.%d", &o1, &o2, &o3, &o4) == 4) {
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