#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleWifiSTA() {
    File file = LittleFS.open(PAGE_WIFI_STA, "r");
    if (file) {
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        sendPageNotFound();
    }
    
}

void WiFiManager::handleForgetWifi() {
    // The actual forgetting will be handled by the main application
    _server->send(200, "text/plain", "WiFi credentials cleared");
}

void WiFiManager::handleScanWifi() {
     _server->send(200, "text/html", scanNetworks());
}