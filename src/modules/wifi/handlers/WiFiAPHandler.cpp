#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleWifiAP() {
    File file = LittleFS.open(PAGE_WIFI_AP, "r");
    if (file) {
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        sendPageNotFound();
    }
}