#include "GeoPage.h"
#include "../Icons.h"
#include "../DisplayUtils.h"

GeoPage::GeoPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void GeoPage::onDisplay0(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    display->drawXBMP(0, 0, L_ICON_WIDTH, L_ICON_HEIGHT, geo_icon);
    displayTitle(display, "GEO", 50);
}

void GeoPage::onDisplay1(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    GeoData& data = _dataStorage->geo();

    displayTitle(display, "IP");
    String ip = data.ip_address;
    displayIPAddress(display, ip, 40, 20);
}

void GeoPage::onDisplay2(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    GeoData& data = _dataStorage->geo();

    displayTitle(display, "COUNTRY");
    String msg = data.country;
    displayMultilineText(display, msg, 40, 20, 6);
}

void GeoPage::onDisplay3(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    GeoData& data = _dataStorage->geo();

    displayTitle(display, "CITY");
    String msg = data.city;
    displayMultilineText(display, msg, 40, 20, 6);
}

void GeoPage::onDisplay4(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    GeoData& data = _dataStorage->geo();

    displayTitle(display, "LAT");
    String str = String(data.latitude, 4);
    display->drawStr(0, 40, str.c_str());
}

void GeoPage::onDisplay5(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    GeoData& data = _dataStorage->geo();

    displayTitle(display, "LON");
    String str = String(data.longitude, 4);
    display->drawStr(0, 40, str.c_str());
}

void GeoPage::onDisplay6(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    display->clear();
}

void GeoPage::onDisplay7(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    display->clear();
}
