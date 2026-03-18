#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleWifiAP() {
    if (_server->method() == HTTP_POST) {
        String ssid = _server->arg("ssid");
        String password = _server->arg("password");

        if (ssid.length() > 0 && password.length() >= 8) { // Minimum 8 characters for WPA2
            // The actual saving will be handled by the main application
            _server->send(200, "text/plain", "Settings saved");
        } else {
            _server->send(400, "text/plain", "Invalid parameters");
        }
    } else {
        if (LittleFS.exists("/wifi_ap.html.gz")) {
            _server->sendHeader("Content-Encoding", "gzip");
            File file = LittleFS.open("/wifi_ap.html.gz", "r");
            _server->streamFile(file, "text/html");
            file.close();
        } else {
            _server->send(200, "text/plain", "page not found.");
        }
    }
}