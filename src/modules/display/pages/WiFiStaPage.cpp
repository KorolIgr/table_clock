#include "WiFiStaPage.h"
#include <cstdio>
#include "../DisplayUtils.h"
#include "../Icons.h"

WiFiStaPage::WiFiStaPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void WiFiStaPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    if (!_dataStorage) return;
    
    SharedData& data = _dataStorage->getData();
    
    // Use larger fonts for better visibility
    display->setFont(u8g2_font_10x20_tf); // Large font for main info
    
    // Distribute content across 8 displays
    switch (displayIndex) {
        case 0:
            // Display 0: WiFi icon (XBMP)
            display->drawXBMP(0, 0, L_ICON_WIDTH, L_ICON_HEIGHT, wifi_icon);
            displayTitle(display, "STA", 50);

            break;
        case 1: {
            displayTitle(display, "STATE");
            String msg = data.wifi_connected ? "Connected" : "Disconnected";
            displayMultilineText(display, msg, 40, 20, 6);
            break;
        }
        case 2: {
            displayTitle(display, "IP");
            String ip = data.ip_address;
            displayIPAddress(display, ip, 40, 20);
            break;
        }
        case 3:      
            display->clear();
            break; 
        case 4:
            display->clear();
            break;
        case 5:
            display->clear();
            break;
        case 6:
            display->clear();
            break;
        case 7:
            display->clear();
            break;
    }
}