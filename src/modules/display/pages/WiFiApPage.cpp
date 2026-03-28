#include "WiFiApPage.h"
#include <U8g2lib.h>
#include <cstdio>
#include "../DisplayUtils.h"
#include "../Icons.h"

WiFiApPage::WiFiApPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void WiFiApPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    if (!_dataStorage) return;
    
    WiFiData& data = _dataStorage->wifi();

    // Use larger fonts for better visibility
    display->setFont(u8g2_font_10x20_tf); // Large font for main info
    
    // Distribute content across 8 displays according to requirements
    switch (displayIndex) {
        case 0: {
            // Display 0: WiFi icon (XBMP)
            display->drawXBMP(0, 0, L_ICON_WIDTH, L_ICON_HEIGHT, wifi_icon);
            displayTitle(display, "AP", 50);

            //display->drawFrame(5, 80, 32, 26); // вместо 32x32
            break;
        }
        case 1: {
            // Display 1: Network name (SSID) - each letter on a new line
            displayTitle(display, "SSID");
            String ssid = data.ap.ssid;
            displayMultilineText(display, ssid, 40, 20, 6);
            break;
        }
        case 2: {
            // Display 2: Network password - each letter on a new line
            displayTitle(display, "PWD");
            String pwd = data.ap.password;
            displayMultilineText(display, pwd, 40, 20, 6);
            break;
        }
        case 3: {
            // Display 3: Network IP - each octet on a new line
            displayTitle(display, "IP");
            String ip = data.ap.ip;
            displayIPAddress(display, ip, 40, 20);
            break;
        }
        case 4: {
            // Display 4: Network mask - each octet on a new line
            displayTitle(display, "MASK");
            String mask = data.ap.mask;
            displayIPAddress(display, mask, 40, 20);
            break;
        }
        case 5: {
            // Display 5: MAC address
            displayTitle(display, "MAC");

            String mac = data.ap.mac;
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
            displayTitle(display, "CLIENT");
            char buf[16];
            sprintf(buf, "max:%d", data.ap.max_clients);
            display->drawStr(0, 40, buf);
            sprintf(buf, "now:%d", data.ap.connected_clients);
            display->drawStr(0, 60, buf);
            break;
        }
        case 7: {
            // Display 7: Channel
            displayTitle(display, "CH");
            char buf[8];
            sprintf(buf, "%d", data.ap.channel);
            display->drawStr(0, 40, buf);
            break;
        }
    }
}