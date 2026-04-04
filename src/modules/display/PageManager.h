#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include <Arduino.h>
#include "../data_storage/DataStorage.h"
#include "pages/WiFiStaPage.h"
#include "pages/WiFiApPage.h"
#include "pages/GeoPage.h"
#include "pages/WeekWeatherPage.h"
#include "pages/CurrentWeatherPage.h"
#include "pages/AirQualityPage.h"

// Forward declaration to avoid circular dependency
class DisplayManager;

// Enum for different display pages
enum class DisplayPage {
    WIFI_STA,
    WIFI_AP,
    GEO_PAGE,
    WEATHER_PAGE,
    CURRENT_WEATHER_PAGE,
    AIR_QUALITY_PAGE
};

class PageManager {
public:
    PageManager(DataStorage* dataStorage);
    ~PageManager();
    
     void updateAllDisplays(DisplayManager** displays, uint8_t count);
     void updateSingleDisplay(DisplayManager* display, uint8_t displayIndex);
     void setCurrentPage(DisplayPage page);
    void nextPage();
    void setPageInterval(unsigned long intervalMs);
    DisplayPage getCurrentPage() const { return _currentPage; }
    uint8_t getCurrentDisplayIndex() const { return _currentDisplayIndex; }
    void resetDisplayCycle() { _currentDisplayIndex = 0; }

private:
    DataStorage* _dataStorage;
    DisplayPage _currentPage = DisplayPage::WIFI_STA;
    unsigned long _pageInterval = 10000; // Default 2 seconds
    unsigned long _lastPageChange = 0;
    uint8_t _currentDisplayIndex = 0; // For single display cycling
    
    WiFiStaPage* _wifiStaPage;
    WiFiApPage* _wifiApPage;
    GeoPage* _geoPage;
    WeekWeatherPage* _weatherPage;
    CurrentWeatherPage* _currentWeatherPage;
    AirQualityPage* _airQualityPage;
};

#endif // PAGE_MANAGER_H