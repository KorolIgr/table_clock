#include "BasePage.h"

//https://lopaka.app/

void BasePage::render(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* display, uint8_t displayIndex) {
    switch (displayIndex) {
        case 0: onDisplay0(display); break;
        case 1: onDisplay1(display); break;
        case 2: onDisplay2(display); break;
        case 3: onDisplay3(display); break;
        case 4: onDisplay4(display); break;
        case 5: onDisplay5(display); break;
        case 6: onDisplay6(display); break;
        case 7: onDisplay7(display); break;
    }
}
