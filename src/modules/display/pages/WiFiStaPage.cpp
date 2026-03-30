#include "WiFiStaPage.h"
#include <cstdio>
#include "../DisplayUtils.h"
#include "../Icons.h"

WiFiStaPage::WiFiStaPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void WiFiStaPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    if (!_dataStorage) return;
    
    WiFiData& data = _dataStorage->wifi();
    
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
            String msg = data.sta.connected ? "Connected" : "Disconnected";
            displayMultilineText(display, msg, 40, 20, 6);
            break;
        }
        case 2: {
            displayTitle(display, "SSID");
            String ssid = data.sta.ssid;
            displayMultilineText(display, ssid, 40, 20, 6);
            break;
        }
        case 3: {
            displayTitle(display, "IP");
            String ip = data.sta.ip_address;
            displayIPAddress(display, ip, 40, 20);
            break;
        }
        case 4: {
            displayTitle(display, "MASK");
            String mask = data.sta.subnet_mask;
            displayIPAddress(display, mask, 40, 20);
            break;
        }
        case 5: {
            displayTitle(display, "GW");
            String gw = data.sta.gateway_ip;
            displayIPAddress(display, gw, 40, 20);
            break;
        }
        case 6: {
            displayTitle(display, "RSSI");
            char rssiBuf[16];
            sprintf(rssiBuf, "%d dBm", data.sta.rssi);
            displayValue(display, rssiBuf, 40, u8g2_font_10x20_tf, 20);
            break;
        }
        case 7: {
            displayTitle(display, "CH");
            char chBuf[16];
            sprintf(chBuf, "%d", data.sta.channel);
            displayValue(display, chBuf, 40, u8g2_font_10x20_tf, 20);
            break;
        }
    }
}