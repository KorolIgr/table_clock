#include "GeoPage.h"
#include "../Icons.h"
#include "../DisplayUtils.h"

GeoPage::GeoPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void GeoPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    if (!_dataStorage) return;
    
    SharedData& data = _dataStorage->getData();

    display->setFont(u8g2_font_10x20_tf);

    switch (displayIndex) {
        case 0: {
            // Display 0: WiFi icon (XBMP)
            display->drawXBMP(0, 0, L_ICON_WIDTH, L_ICON_HEIGHT, geo_icon);
            displayTitle(display, "GEO", 50);

            //display->drawFrame(5, 80, 32, 26); // вместо 32x32
            break;
        }
        case 1: {
            displayTitle(display, "IP");
            String ip = data.geo_ip_address;
            displayIPAddress(display, ip, 40, 20);
            break;
        }
        case 2: {
            displayTitle(display, "COUNTRY");
            String msg = data.country;
            displayMultilineText(display, msg, 40, 20, 6);
            break;
        }
        case 3: {
            displayTitle(display, "CITY");
            String msg = data.city;
            displayMultilineText(display, msg, 40, 20, 6);
            break;
        }
        case 4: {
            displayTitle(display, "LAT");
            String str = String(data.latitude, 4);
            display->drawStr(0, 40, str.c_str());
            break;
        }
        case 5: {
            displayTitle(display, "LON");
            String str = String(data.longitude, 4);
            display->drawStr(0, 40, str.c_str());
            break;
        }
        case 6: {
            display->clear();
            break;
        }
        case 7: {
            display->clear();
            break;
        }
    }
    
}
