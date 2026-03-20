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

void WiFiManager::handleNav() {
    File file = LittleFS.open("/nav.html.gz", "r");
    if (file) {
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        sendPageNotFound();
    }
}