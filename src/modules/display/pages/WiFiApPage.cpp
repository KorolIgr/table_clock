#include "WiFiApPage.h"
#include <U8g2lib.h>
#include <cstdio>

// XBMP icon for WiFi AP: 32x32 pixel bitmap (signal bars icon)
// Designed as three horizontal bars of increasing width and a dot below
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

WiFiApPage::WiFiApPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void WiFiApPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    if (!_dataStorage) return;
    
    SharedData& data = _dataStorage->getData();
    
    // Distribute content across 8 displays according to requirements
    switch (displayIndex) {
        case 0: {
            // Display 0: WiFi icon (XBMP)
            display->drawXBMP(0, 0, 32, 32, wifi_icon_bits);
            display->setFont(u8g2_font_fub14_tf);
            display->drawStr(0, 50, "AP"); // 8px line spacing

            //display->drawFrame(5, 80, 32, 26); // вместо 32x32
            break;
        }
        case 1: {
            // Display 1: Network name (SSID) - each letter on a new line
            display->setFont(u8g2_font_10x20_tf);
            display->drawStr(0, 20, "SSID");
            String ssid = data.ap_ssid;
            int y = 40;

            for (int i = 0; i < ssid.length(); i += 6) {
                String line = ssid.substring(i, i + 6);
                display->drawStr(0, y, line.c_str());
                y += 20;
            }
            break;
        }
        case 2: {
            // Display 2: Network password - each letter on a new line
            display->setFont(u8g2_font_10x20_tf);
            display->drawStr(0, 20, "PWD");
            String pwd = data.ap_password;
            int y = 40;
            for (int i = 0; i < pwd.length(); i += 6) {
                String line = pwd.substring(i, i + 6);
                display->drawStr(0, y, line.c_str());
                y += 20;
            }
            break;
        }
        case 3: {
            // Display 3: Network IP - each octet on a new line
            display->setFont(u8g2_font_10x20_tf);
            display->drawStr(0, 20, "IP");
            String ip = data.ap_ip;
            int o1, o2, o3, o4;
            if (sscanf(ip.c_str(), "%d.%d.%d.%d", &o1, &o2, &o3, &o4) == 4) {
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
        case 4: {
            // Display 4: Network mask - each octet on a new line
            display->setFont(u8g2_font_10x20_tf);
            display->drawStr(0, 20, "MASK");
            String mask = data.ap_mask;
            int o1, o2, o3, o4;
            if (sscanf(mask.c_str(), "%d.%d.%d.%d", &o1, &o2, &o3, &o4) == 4) {
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
                display->drawStr(0, 0, "Invalid mask");
            }
            break;
        }
        case 5: {
            // Display 5: MAC address
            display->setFont(u8g2_font_10x20_tf);
            display->drawStr(0, 20, "MAC");

            String mac = data.ap_mac;
            int y = 40;
            for (int i = 0; i < 3; i++) {
                String line = mac.substring(i * 6, i * 6 + 5); // "AA:BB"
                display->drawStr(0, y, line.c_str());
                y += 22;
            }
            break;
        }
        case 6: {
            // Display 6: Max clients / connected clients (e.g., "4/2")
            display->setFont(u8g2_font_fub20_tf);
            char buf[16];
            sprintf(buf, "%d/%d", data.ap_max_clients, data.ap_connected_clients);
            display->drawStr(0, 40, buf);
            break;
        }
        case 7: {
            // Display 7: Channel
            display->setFont(u8g2_font_fub14_tf);
            char buf[8];
            sprintf(buf, "%d", data.ap_channel);
            display->drawStr(0, 40, buf);
            break;
        }
    }
}