#ifndef AIR_QUALITY_PAGE_H
#define AIR_QUALITY_PAGE_H

#include <Arduino.h>
#include "../../data_storage/DataStorage.h"
#include <U8g2lib.h>
#include "BasePage.h"

class AirQualityPage : public BasePage {
public:
    AirQualityPage(DataStorage* dataStorage);
    ~AirQualityPage() = default;

protected:
    void onDisplay0(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) override;
    void onDisplay1(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) override;
    void onDisplay2(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) override;
    void onDisplay3(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) override;
    void onDisplay4(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) override;
    void onDisplay5(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) override;
    void onDisplay6(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) override;
    void onDisplay7(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) override;

private:
    DataStorage* _dataStorage;
};

#endif // AIR_QUALITY_PAGE_H
