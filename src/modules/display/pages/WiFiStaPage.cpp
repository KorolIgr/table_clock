#include "WiFiStaPage.h"
#include <cstdio>
#include "../DisplayUtils.h"
#include "../Icons.h"

WiFiStaPage::WiFiStaPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void WiFiStaPage::onDisplay0(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    display->drawXBMP(0, 0, L_ICON_WIDTH, L_ICON_HEIGHT, wifi_icon);
    displayTitle(display, "STA", 50);
}

void WiFiStaPage::onDisplay1(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "STATE");
    String msg = data.sta.connected ? "Connected" : "Disconnected";
    displayMultilineText(display, msg, 40, 20, 6);
}

void WiFiStaPage::onDisplay2(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "SSID");
    String ssid = data.sta.ssid;
    displayMultilineText(display, ssid, 40, 20, 6);
}

void WiFiStaPage::onDisplay3(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "IP");
    String ip = data.sta.ip_address;
    displayIPAddress(display, ip, 40, 20);
}

void WiFiStaPage::onDisplay4(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "MASK");
    String mask = data.sta.subnet_mask;
    displayIPAddress(display, mask, 40, 20);
}

void WiFiStaPage::onDisplay5(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "GW");
    String gw = data.sta.gateway_ip;
    displayIPAddress(display, gw, 40, 20);
}

void WiFiStaPage::onDisplay6(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "RSSI");
    char rssiBuf[16];
    sprintf(rssiBuf, "%d dBm", data.sta.rssi);
    displayValue(display, rssiBuf, 40, u8g2_font_10x20_tf, 20);
}

void WiFiStaPage::onDisplay7(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    WiFiData& data = _dataStorage->wifi();

    displayTitle(display, "CH");
    char chBuf[16];
    sprintf(chBuf, "%d", data.sta.channel);
    displayValue(display, chBuf, 40, u8g2_font_10x20_tf, 20);
}
