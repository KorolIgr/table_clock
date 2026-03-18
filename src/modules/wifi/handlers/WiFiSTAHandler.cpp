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
        // Try to serve from LittleFS
        if (LittleFS.exists("/wifi_sta.html.gz")) {
            _server->sendHeader("Content-Encoding", "gzip");
            File file = LittleFS.open("/wifi_sta.html.gz", "r");
            size_t sent = _server->streamFile(file, "text/html");
            file.close();
        } else if (LittleFS.exists("/wifi_sta.html")) {
            File file = LittleFS.open("/wifi_sta.html", "r");
            size_t sent = _server->streamFile(file, "text/html");
            file.close();
        } else {
            // Fallback to basic form
            String html = "<!DOCTYPE html><html><head><title>WiFi STA Settings</title></head><body>";
            html += "<h1>WiFi STA Settings</h1>";
            html += "<form method=\"POST\">";
            html += "SSID: <input type=\"text\" name=\"ssid\" required><br>";
            html += "Password: <input type=\"password\" name=\"password\" required><br>";
            html += "<input type=\"submit\" value=\"Connect\">";
            html += "</form>";
            html += "<a href=\"/\">Back to Home</a>";
            html += "</body></html>";
            _server->send(200, "text/html", html);
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