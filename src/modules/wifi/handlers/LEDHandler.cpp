#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleLED() {
    if (LittleFS.exists(PAGE_LED)) {
        _server->sendHeader("Content-Encoding", "gzip");
        File file = LittleFS.open(PAGE_LED, "r");
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        sendPageNotFound();
    }
}