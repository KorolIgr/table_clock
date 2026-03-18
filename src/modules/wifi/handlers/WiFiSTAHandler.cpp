#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleWifiSTA() {
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
        if (LittleFS.exists("/wifi_sta.html.gz")) {
            _server->sendHeader("Content-Encoding", "gzip");
            File file = LittleFS.open("/wifi_sta.html.gz", "r");
            _server->streamFile(file, "text/html");
            file.close();
        } else {
            _server->send(200, "text/plain", "page not found.");
        }
    }
}

void WiFiManager::handleForgetWifi() {
    // The actual forgetting will be handled by the main application
    _server->send(200, "text/plain", "WiFi credentials cleared");
}

void WiFiManager::handleScanWifi() {
    // Perform network scan
    int n = WiFi.scanNetworks();
    
    String json = "[";
    for (int i = 0; i < n; ++i) {
        if (i > 0) json += ",";
        json += "{";
        json += "\"ssid\":\"" + WiFi.SSID(i) + "\",";
        json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
        json += "\"secure\":" + String(WiFi.encryptionType(i) == ENC_TYPE_NONE ? "false" : "true");
        json += "}";
    }
    json += "]";
    
    _server->send(200, "application/json", json);
}