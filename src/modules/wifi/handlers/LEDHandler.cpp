#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleLED() {
    File file = LittleFS.open(PAGE_LED, "r");
    if (file) {
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        sendPageNotFound();
    }
}