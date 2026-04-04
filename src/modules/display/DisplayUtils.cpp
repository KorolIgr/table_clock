#include "DisplayUtils.h"
#include <cstdio>

void displayIPAddress(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display, const String& ip, uint8_t startY, uint8_t lineHeight) {
    int o1, o2, o3, o4;
    if (sscanf(ip.c_str(), "%d.%d.%d.%d", &o1, &o2, &o3, &o4) == 4) {
        char buf[16];
        sprintf(buf, "%d.", o1);
        display->drawStr(0, startY, buf);
        sprintf(buf, "%d.", o2);
        display->drawStr(0, startY + lineHeight, buf);
        sprintf(buf, "%d.", o3);
        display->drawStr(0, startY + 2 * lineHeight, buf);
        sprintf(buf, "%d", o4);
        display->drawStr(0, startY + 3 * lineHeight, buf);
    } else {
        display->drawStr(0, startY, "Invalid IP");
    }
}

void displayMultilineText(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display, const String& text, uint8_t startY, uint8_t lineHeight, uint8_t charsPerLine) {
    uint8_t y = startY;
    for (size_t i = 0; i < text.length(); i += charsPerLine) {
        String line = text.substring(i, i + charsPerLine);
        display->drawStr(0, y, line.c_str());
        y += lineHeight;
    }
}

void displayTitle(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display, const String& text, uint8_t startY, const uint8_t* font, uint8_t lineHeight) {

    String displayText = text;
    if (text.length() > 6) {
        displayText = text.substring(0, 5) + "*";
    }
    display->setFont(font);
    display->drawStr(0, startY, displayText.c_str());

}

void displayValue(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display, const String& text, uint8_t startY, const uint8_t* font, uint8_t lineHeight) {

    String displayText = text;
    if (text.length() > 6) {
        displayText = text.substring(0, 5) + "*";
    }
    display->setFont(font);
    display->drawStr(0, startY, displayText.c_str());

}
