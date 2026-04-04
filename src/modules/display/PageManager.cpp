#include "PageManager.h"
#include "DisplayManager.h"
#include <ESP8266WiFi.h>
#include <U8g2lib.h>
#include "pages/WiFiStaPage.h"
#include "pages/WiFiApPage.h"
#include "pages/GeoPage.h"
#include "pages/WeekWeatherPage.h"
#include "pages/CurrentWeatherPage.h"
#include "pages/AirQualityPage.h"

PageManager::PageManager(DataStorage* dataStorage) : _dataStorage(dataStorage), _wifiStaPage(nullptr), _wifiApPage(nullptr), _geoPage(nullptr), _weatherPage(nullptr), _currentWeatherPage(nullptr), _airQualityPage(nullptr) {
    if (_dataStorage) {
        _wifiStaPage = new WiFiStaPage(_dataStorage);
        _wifiApPage = new WiFiApPage(_dataStorage);
        _geoPage = new GeoPage(_dataStorage);
        _weatherPage = new WeekWeatherPage(_dataStorage);
        _currentWeatherPage = new CurrentWeatherPage(_dataStorage);
        _airQualityPage = new AirQualityPage(_dataStorage);
    }
}

PageManager::~PageManager() {
    if (_wifiStaPage) {
        delete _wifiStaPage;
    }
    if (_wifiApPage) {
        delete _wifiApPage;
    }
    if (_geoPage) {
        delete _geoPage;
    }
    if (_weatherPage) {
        delete _weatherPage;
    }
    if (_currentWeatherPage) {
        delete _currentWeatherPage;
    }
    if (_airQualityPage) {
        delete _airQualityPage;
    }
}

void PageManager::updateAllDisplays(DisplayManager** displays, uint8_t count) {
    if (!_dataStorage || count == 0) return;
    
    // Handle automatic page switching
    unsigned long currentTime = millis();
    if (currentTime - _lastPageChange >= _pageInterval) {
        nextPage();
        _lastPageChange = currentTime;
    }
    
    // Update each display with distributed content
    for (uint8_t i = 0; i < count; i++) {
        if (displays[i]) {
            displays[i]->clear();
            
            // Get the U8G2 display object
            U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* u8g2 = displays[i]->getU8g2();
            if (u8g2) {
            // Render content based on current page for this display index
            switch (_currentPage) {
                case DisplayPage::WIFI_STA:
                    if (_wifiStaPage) {
                        _wifiStaPage->render(u8g2, i);
                    }
                    break;
                case DisplayPage::WIFI_AP:
                    if (_wifiApPage) {
                        _wifiApPage->render(u8g2, i);
                    }
                    break;
                case DisplayPage::GEO_PAGE:
                    if (_geoPage) {
                        _geoPage->render(u8g2, i);
                    }
                    break;
                case DisplayPage::WEATHER_PAGE:
                    if (_weatherPage) {
                        _weatherPage->render(u8g2, i);
                    }
                    break;
                case DisplayPage::CURRENT_WEATHER_PAGE:
                    if (_currentWeatherPage) {
                        _currentWeatherPage->render(u8g2, i);
                    }
                    break;
                case DisplayPage::AIR_QUALITY_PAGE:
                    if (_airQualityPage) {
                        _airQualityPage->render(u8g2, i);
                    }
                    break;
            }
            }
            
            displays[i]->display();
        }
    }
}

    void PageManager::updateSingleDisplay(DisplayManager* display, uint8_t displayIndex) {
        if (!_dataStorage || !display) return;
        
        // Handle automatic page switching (only for the first display in the cycle to avoid multiple switches)
        if (displayIndex == 0) {
            unsigned long currentTime = millis();
            if (currentTime - _lastPageChange >= _pageInterval) {
                nextPage();
                _lastPageChange = currentTime;
            }
        }
        
        display->clear();
        
        // Get the U8G2 display object
        U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* u8g2 = display->getU8g2();
        if (u8g2) {
            // Render content based on current page for this display index
            switch (_currentPage) {
                case DisplayPage::WIFI_STA:
                    if (_wifiStaPage) {
                        _wifiStaPage->render(u8g2, displayIndex);
                    }
                    break;
                case DisplayPage::WIFI_AP:
                    if (_wifiApPage) {
                        _wifiApPage->render(u8g2, displayIndex);
                    }
                    break;
                case DisplayPage::GEO_PAGE:
                    if (_geoPage) {
                        _geoPage->render(u8g2, displayIndex);
                    }
                    break;
                case DisplayPage::WEATHER_PAGE:
                    if (_weatherPage) {
                        _weatherPage->render(u8g2, displayIndex);
                    }
                    break;
                case DisplayPage::CURRENT_WEATHER_PAGE:
                    if (_currentWeatherPage) {
                        _currentWeatherPage->render(u8g2, displayIndex);
                    }
                    break;
                case DisplayPage::AIR_QUALITY_PAGE:
                    if (_airQualityPage) {
                        _airQualityPage->render(u8g2, displayIndex);
                    }
                    break;
            }
        }
        
         display->display();
     }

 void PageManager::setCurrentPage(DisplayPage page) {
    _currentPage = page;
    _lastPageChange = millis(); // Reset timer when manually changing page
}

void PageManager::nextPage() {
    switch (_currentPage) {
        case DisplayPage::WIFI_STA:
            _currentPage = DisplayPage::WIFI_AP;
            break;
        case DisplayPage::WIFI_AP:
            _currentPage = DisplayPage::GEO_PAGE;
            break;
        case DisplayPage::GEO_PAGE:
            _currentPage = DisplayPage::WEATHER_PAGE;
            break;
        case DisplayPage::WEATHER_PAGE:
            _currentPage = DisplayPage::CURRENT_WEATHER_PAGE;
            break;
        case DisplayPage::CURRENT_WEATHER_PAGE:
            _currentPage = DisplayPage::AIR_QUALITY_PAGE;
            break;
        case DisplayPage::AIR_QUALITY_PAGE:
            _currentPage = DisplayPage::WIFI_STA;
            break;
    }
}

void PageManager::setPageInterval(unsigned long intervalMs) {
    _pageInterval = intervalMs;
}