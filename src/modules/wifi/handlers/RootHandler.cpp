#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleRoot() {
    File file = LittleFS.open(PAGE_INDEX, "r");
    if (file) {
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        sendPageNotFound();
    }
}