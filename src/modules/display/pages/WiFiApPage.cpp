#include "WiFiApPage.h"
#include <U8g2lib.h>
#include <cstdio>
#include "../DisplayUtils.h"
#include "../Icons.h"

WiFiApPage::WiFiApPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void WiFiApPage::onDisplay0(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    display->drawXBMP(0, 0, L_ICON_WIDTH, L_ICON_HEIGHT, wifi_icon);
    displayTitle(display, "AP", 50);
}

void WiFiApPage::onDisplay1(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "SSID");
    String ssid = data.ap.ssid;
    displayMultilineText(display, ssid, 40, 20);
}

void WiFiApPage::onDisplay2(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "PWD");
    String pwd = data.ap.password;
    displayMultilineText(display, pwd, 40, 20);
}

void WiFiApPage::onDisplay3(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "IP");
    String ip = data.ap.ip;
    displayIPAddress(display, ip, 40, 20);
}

void WiFiApPage::onDisplay4(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "MASK");
    String mask = data.ap.mask;
    displayIPAddress(display, mask, 40, 20);
}

void WiFiApPage::onDisplay5(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "MAC");

    String mac = data.ap.mac;
    int y = 40;
    for (int i = 0; i < 3; i++) {
        String line = mac.substring(i * 6, i * 6 + 5);
        display->drawStr(0, y, line.c_str());
        y += 22;
    }
}

void WiFiApPage::onDisplay6(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "CLIENT");
    char buf[16];
    sprintf(buf, "max:%d", data.ap.max_clients);
    display->drawStr(0, 40, buf);
    sprintf(buf, "now:%d", data.ap.connected_clients);
    display->drawStr(0, 60, buf);
}

void WiFiApPage::onDisplay7(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "CH");
    char buf[8];
    sprintf(buf, "%d", data.ap.channel);
    display->drawStr(0, 40, buf);
}
