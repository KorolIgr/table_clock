#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleLED() {
    if (LittleFS.exists("/led.html.gz")) {
        _server->sendHeader("Content-Encoding", "gzip");
        File file = LittleFS.open("/led.html.gz", "r");
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        _server->send(200, "text/plain", "page not found.");
    }
}

void WiFiManager::handleLEDPattern() {
    if (LittleFS.exists("/led_pattern.html.gz")) {
        _server->sendHeader("Content-Encoding", "gzip");
        File file = LittleFS.open("/led_pattern.html.gz", "r");
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        _server->send(200, "text/plain", "page not found.");
    }
}