#include "GeoPage.h"

// Icon: 24x24 globe with cross, PROGMEM
static const unsigned char geo_icon_xbm[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 
	0x00, 0x1c, 0x38, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 
	0x80, 0x81, 0x81, 0x01, 0x80, 0xe1, 0x87, 0x01, 0x80, 0x20, 0x04, 0x01, 0x80, 0x30, 0x0c, 0x01, 
	0x80, 0x20, 0x04, 0x01, 0x80, 0x60, 0x06, 0x01, 0x80, 0xc1, 0x83, 0x01, 0x00, 0x01, 0x80, 0x00, 
	0x00, 0x03, 0xc0, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x0c, 0x30, 0x00, 
	0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x36, 0x6c, 0x00, 0x00, 0x63, 0xc6, 0x00, 
	0x80, 0xc1, 0x83, 0x01, 0x80, 0x80, 0x01, 0x01, 0x80, 0x01, 0x80, 0x01, 0x00, 0x0f, 0xf0, 0x00, 
	0x00, 0xfc, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

GeoPage::GeoPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void GeoPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    if (!_dataStorage) return;
    
    SharedData& data = _dataStorage->getData();

    display->setFont(u8g2_font_10x20_tf);

    switch (displayIndex) {
        case 0: {
            // Display 0: WiFi icon (XBMP)
            display->drawXBMP(0, 0, 32, 32, geo_icon_xbm);
            display->drawStr(0, 50, "GEO"); // 8px line spacing

            //display->drawFrame(5, 80, 32, 26); // вместо 32x32
            break;
        }
        case 1: {
            display->drawStr(0, 20, "IP");
            String ip = data.geo_ip_address;
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
        case 2: {
            display->drawStr(0, 20, "COUNTRY");
            String msg = data.country;
            int y = 40;
            for (size_t i = 0; i < msg.length(); i += 6) {
                String line = msg.substring(i, i + 6);
                display->drawStr(0, y, line.c_str());
                y += 20;
            }
            break;
        }
        case 3: {
            display->drawStr(0, 20, "CITY");
            String msg = data.city;
            int y = 40;
            for (size_t i = 0; i < msg.length(); i += 6) {
                String line = msg.substring(i, i + 6);
                display->drawStr(0, y, line.c_str());
                y += 20;
            }
            break;
        }
        case 4: {
            display->drawStr(0, 20, "LAT");
            String str = String(data.latitude, 4);
            display->drawStr(0, 40, str.c_str());
            break;
        }
        case 5: {
            display->drawStr(0, 20, "LON");
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
