#include "PageManager.h"
#include "DisplayManager.h"
#include <ESP8266WiFi.h>
#include <U8g2lib.h>
#include "pages/WiFiStaPage.h"
#include "pages/WiFiApPage.h"
#include "pages/GeoPage.h"

PageManager::PageManager(DataStorage* dataStorage) : _dataStorage(dataStorage), _wifiStaPage(nullptr), _wifiApPage(nullptr), _geoPage(nullptr) {
    if (_dataStorage) {
        _wifiStaPage = new WiFiStaPage(_dataStorage);
        _wifiApPage = new WiFiApPage();
        _geoPage = new GeoPage(_dataStorage);
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
            U8G2_SSD1306_128X64_NONAME_F_HW_I2C* u8g2 = displays[i]->getU8g2();
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
                }
            }
            
            displays[i]->display();
        }
    }
}

void PageManager::updatePageDisplay(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    if (!_dataStorage || !display) return;
    
    display->clearBuffer();
    // Use larger font for single display mode as well
    display->setFont(u8g2_font_fub30_tf);
    
    // Handle automatic page switching
    unsigned long currentTime = millis();
    if (currentTime - _lastPageChange >= _pageInterval) {
        nextPage();
        _lastPageChange = currentTime;
    }
    
    // Display full content on a single display (for backward compatibility)
    switch (_currentPage) {
        case DisplayPage::WIFI_STA:
            if (_wifiStaPage) {
                // Render full content on single display
                SharedData& data = _dataStorage->getData();
                display->drawStr(0, 20, "WiFi STA");
                if (data.wifi_connected) {
                    display->drawStr(0, 40, "IP:");
                    display->setFont(u8g2_font_fub20_tf);
                    display->drawStr(15, 55, data.ip_address.c_str());
                } else {
                    display->drawStr(0, 40, "SSID:");
                    display->setFont(u8g2_font_fub20_tf);
                    display->drawStr(25, 55, data.sta_ssid.c_str());
                }
            }
            break;
        case DisplayPage::WIFI_AP:
            if (_wifiApPage) {
                display->drawStr(0, 20, "WiFi AP");
                if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA) {
                    String apIp = WiFi.softAPIP().toString();
                    if (apIp != "0.0.0.0") {
                        display->drawStr(0, 40, "IP:");
                        display->setFont(u8g2_font_fub20_tf);
                        display->drawStr(15, 55, apIp.c_str());
                    } else {
                        display->drawStr(0, 40, "No IP");
                    }
                } else {
                    display->drawStr(0, 40, "Inactive");
                }
            }
            break;
        case DisplayPage::GEO_PAGE:
            if (_geoPage) {
                // For single display, show all geolocation info
                SharedData& data = _dataStorage->getData();
                display->setFont(u8g2_font_fub20_tf);
                display->drawStr(0, 20, "Location");
                display->setFont(u8g2_font_fub14_tf);
                String loc = data.city + ", " + data.country;
                if (loc.length() > 0) {
                    display->drawStr(0, 40, loc.c_str());
                } else {
                    display->drawStr(0, 40, "Fetching...");
                }
                // Show continent on third line if available
                if (data.continent.length() > 0) {
                    display->drawStr(0, 55, data.continent.c_str());
                }
            }
            break;
    }
    
    display->sendBuffer();
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
            _currentPage = DisplayPage::WIFI_STA;
            break;
    }
}

void PageManager::setPageInterval(unsigned long intervalMs) {
    _pageInterval = intervalMs;
}