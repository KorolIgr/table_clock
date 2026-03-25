#ifndef GEO_PAGE_H
#define GEO_PAGE_H

#include <U8g2lib.h>
#include "../../data_storage/DataStorage.h"

class GeoPage {
public:
    GeoPage(DataStorage* dataStorage);
    void render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex);
    
private:
    DataStorage* _dataStorage;
};

#endif // GEO_PAGE_H
