#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleRoot() {
    // Try to serve gzipped page from LittleFS
    if (_server->hasArg("plain")) {
        // Handle POST data if needed
    } else {
        String path;
        if (_server->hasArg("path")) {
            path = _server->arg("path");
        } else {
            path = "/index.html";
        }
        String gzPath = path + ".gz";
        if (LittleFS.exists(gzPath)) {
            _server->sendHeader("Content-Encoding", "gzip");
            File file = LittleFS.open(gzPath, "r");
            _server->streamFile(file, "text/html");
            file.close();
        } else {
            _server->send(200, "text/plain", "page not found.");
        }
    }
}