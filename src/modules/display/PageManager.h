#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include <Arduino.h>
#include "../data_storage/DataStorage.h"
#include "pages/WiFiStaPage.h"
#include "pages/WiFiApPage.h"
#include "pages/GeoPage.h"
#include "pages/WeatherPage.h"

// Forward declaration to avoid circular dependency
class DisplayManager;

// Enum for different display pages
enum class DisplayPage {
    WIFI_STA,
    WIFI_AP,
    GEO_PAGE,
    WEATHER_PAGE
};

class PageManager {
public:
    PageManager(DataStorage* dataStorage);
    ~PageManager();
    
    void updateAllDisplays(DisplayManager** displays, uint8_t count);
    void updatePageDisplay(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display);
    void setCurrentPage(DisplayPage page);
    void nextPage();
    void setPageInterval(unsigned long intervalMs);
    DisplayPage getCurrentPage() const { return _currentPage; }

private:
    DataStorage* _dataStorage;
    DisplayPage _currentPage = DisplayPage::WIFI_STA;
    unsigned long _pageInterval = 10000; // Default 10 seconds
    unsigned long _lastPageChange = 0;
    
    WiFiStaPage* _wifiStaPage;
    WiFiApPage* _wifiApPage;
    GeoPage* _geoPage;
    WeatherPage* _weatherPage;
};

#endif // PAGE_MANAGER_H