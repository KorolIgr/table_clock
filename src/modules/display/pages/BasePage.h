#ifndef BASE_PAGE_H
#define BASE_PAGE_H

#include <Arduino.h>
#include <U8g2lib.h>

class BasePage {
public:
    BasePage() = default;
    virtual ~BasePage() = default;

    void render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex);

protected:
    virtual void onDisplay0(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {}
    virtual void onDisplay1(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {}
    virtual void onDisplay2(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {}
    virtual void onDisplay3(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {}
    virtual void onDisplay4(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {}
    virtual void onDisplay5(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {}
    virtual void onDisplay6(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {}
    virtual void onDisplay7(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {}
};

#endif // BASE_PAGE_H
