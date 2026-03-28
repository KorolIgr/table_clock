#include "WiFiStaPage.h"
#include <cstdio>

static const unsigned char wifi_icon_bits[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xf0, 0x1f, 0x00, 0x00, 0xfe, 0xff, 0x00, 0x80, 0xff, 0xff, 0x03, 0xe0, 0xff, 0xff, 0x0f, 
	0xf0, 0x1f, 0xf8, 0x1f, 0xfc, 0x03, 0xc0, 0x3f, 0xfe, 0x00, 0x00, 0x7f, 0x7f, 0x00, 0x00, 0xfc, 
	0x1f, 0xf0, 0x0f, 0xf8, 0x0f, 0xfc, 0x3f, 0x70, 0x06, 0xfe, 0xff, 0x00, 0x00, 0xff, 0xff, 0x01, 
	0x80, 0xff, 0xfe, 0x03, 0xc0, 0x1f, 0xf0, 0x03, 0xc0, 0x07, 0xe0, 0x03, 0xc0, 0x03, 0x80, 0x03, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x0f, 0x00, 
	0x00, 0xf0, 0x0f, 0x00, 0x00, 0xe0, 0x0f, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xc0, 0x03, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

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
            display->drawXBMP(0, 0, 32, 32, wifi_icon_bits);
            display->drawStr(0, 50, "STA"); // 8px line spacing

            break;
        case 1: {
            display->drawStr(0, 20, "STATE");
            String msg = data.wifi_connected ? "Connected" : "Disconnected";
            int y = 40;
            for (size_t i = 0; i < msg.length(); i += 6) {
                String line = msg.substring(i, i + 6);
                display->drawStr(0, y, line.c_str());
                y += 20;
            }
            break;
        }
        case 2: {
            display->drawStr(0, 20, "IP");
            const char* ip = data.ip_address.c_str();
            int o1, o2, o3, o4;
            if (sscanf(ip, "%d.%d.%d.%d", &o1, &o2, &o3, &o4) == 4) {
                char buf[16];
                sprintf(buf, "%d", o1);
                display->drawStr(0, 2 * 20, buf);
                sprintf(buf, "%d", o2);
                display->drawStr(0, 3 * 20, buf);
                sprintf(buf, "%d", o3);
                display->drawStr(0, 4 * 20, buf);
                sprintf(buf, "%d", o4);
                display->drawStr(0, 5 * 20, buf);
            } else {
                display->drawStr(0, 0, "Invalid IP");
            }
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