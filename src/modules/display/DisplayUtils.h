#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

#include <Arduino.h>
#include <U8g2lib.h>

// Display an IP address as four separate lines (one octet per line)
// startY: Y coordinate for the first octet
// lineHeight: vertical spacing between lines (default 20)
void displayIPAddress(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, const String& ip, uint8_t startY, uint8_t lineHeight = 20);

// Display text in multiple lines with a maximum number of characters per line
// startY: Y coordinate for the first line
// lineHeight: vertical spacing between lines (default 20)
// charsPerLine: maximum characters per line (default 6)
void displayMultilineText(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, const String& text, uint8_t startY, uint8_t lineHeight = 20, uint8_t charsPerLine = 6);

void displayTitle(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, const String& text, uint8_t startY = 20, const uint8_t* font = u8g2_font_10x20_tf, uint8_t lineHeight = 20);

void displayValue(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, const String& text, uint8_t startY, const uint8_t* font = u8g2_font_10x20_tf, uint8_t lineHeight = 20);

#endif // DISPLAY_UTILS_H
