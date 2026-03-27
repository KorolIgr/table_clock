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
    
    // Display 0: show XBMP icon
    if (displayIndex == 0) {
        // Center the 24x24 icon on the 128x64 display
        display->drawXBMP(0, 0, 32, 32, geo_icon_xbm);
        return;
    }
    
    // Displays 1-5: show geolocation data
    if (displayIndex >= 1 && displayIndex <= 5) {
        // Use a readable font for text
        display->setFont(u8g2_font_fub14_tf);
        String text;
        
        switch (displayIndex) {
            case 1: // Continent
                text = data.continent;
                // Truncate to fit (approx 12 chars for fub14)
                if (text.length() > 12) {
                    text = text.substring(0, 12);
                }
                break;
            case 2: // Country
                text = data.country;
                if (text.length() > 12) {
                    text = text.substring(0, 12);
                }
                break;
            case 3: // City
                text = data.city;
                if (text.length() > 12) {
                    text = text.substring(0, 12);
                }
                break;
            case 4: // Latitude (world data)
                // Format latitude with 4 decimal places
                text = "Lat: " + String(data.latitude, 4);
                // Might be long, truncate if needed
                if (text.length() > 12) {
                    text = text.substring(0, 12);
                }
                break;
            case 5: // Longitude
                text = "Lon: " + String(data.longitude, 4);
                if (text.length() > 12) {
                    text = text.substring(0, 12);
                }
                break;
        }
        
        // Center text horizontally
        int textWidth = display->getStrWidth(text.c_str());
        int x = (128 - textWidth) / 2;
        display->drawStr(x, 40, text.c_str());
        return;
    }
    
    // For other displays (6,7, etc.), leave blank
}
