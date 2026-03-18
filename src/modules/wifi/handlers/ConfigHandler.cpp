#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleConfig() {
    if (LittleFS.exists("/config.html.gz")) {
        _server->sendHeader("Content-Encoding", "gzip");
        File file = LittleFS.open("/config.html.gz", "r");
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        _server->send(200, "text/plain", "page not found.");
    }
}