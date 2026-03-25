#include "PageManager.h"
#include <ESP8266WiFi.h>
#include <U8g2lib.h>
#include "pages/WiFiStaPage.h"
#include "pages/WiFiApPage.h"

PageManager::PageManager(DataStorage* dataStorage) : _dataStorage(dataStorage), _wifiStaPage(nullptr), _wifiApPage(nullptr) {
    if (_dataStorage) {
        _wifiStaPage = new WiFiStaPage(_dataStorage);
        _wifiApPage = new WiFiApPage();
    }
}

PageManager::~PageManager() {
    if (_wifiStaPage) {
        delete _wifiStaPage;
    }
    if (_wifiApPage) {
        delete _wifiApPage;
    }
}

void PageManager::updatePageDisplay(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display) {
    if (!_dataStorage || !display) return;
    
    display->clearBuffer();
    display->setFont(u8g2_font_6x10_tf); // Smaller font to fit more text
    
    // Handle automatic page switching
    unsigned long currentTime = millis();
    if (currentTime - _lastPageChange >= _pageInterval) {
        nextPage();
        _lastPageChange = currentTime;
    }
    
    // Display content based on current page using separate page modules
    switch (_currentPage) {
        case DisplayPage::WIFI_STA:
            if (_wifiStaPage) {
                _wifiStaPage->render(display);
            }
            break;
        case DisplayPage::WIFI_AP:
            if (_wifiApPage) {
                _wifiApPage->render(display);
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
            _currentPage = DisplayPage::WIFI_STA;
            break;
    }
}

void PageManager::setPageInterval(unsigned long intervalMs) {
    _pageInterval = intervalMs;
}