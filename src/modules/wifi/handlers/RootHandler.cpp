#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleRoot() {
    // Try to serve from LittleFS first
    if (_server->hasArg("plain")) {
        // Handle POST data if needed
    } else {
        // Try to serve index.html from LittleFS
        if (_server->hasArg("path")) {
            String path = _server->arg("path");
            if (LittleFS.exists(path + ".gz")) {
                _server->sendHeader("Content-Encoding", "gzip");
                File file = LittleFS.open(path + ".gz", "r");
                size_t sent = _server->streamFile(file, "text/html");
                file.close();
            } else if (LittleFS.exists(path)) {
                File file = LittleFS.open(path, "r");
                size_t sent = _server->streamFile(file, "text/html");
                file.close();
            } else {
                // Fallback to basic response
                String html = "<!DOCTYPE html><html><head><title>Table Clock</title></head><body>";
                html += "<h1>Table Clock</h1>";
                html += "<a href=\"/\">WiFi AP</a><br>";
                html += "<a href=\"/wifi_sta\">WiFi STA</a><br>";
                html += "<a href=\"/led\">LED</a>";
                html += "</body></html>";
                _server->send(200, "text/html", html);
            }
        } else {
            if (LittleFS.exists("/index.html.gz")) {
                _server->sendHeader("Content-Encoding", "gzip");
                File file = LittleFS.open("/index.html.gz", "r");
                size_t sent = _server->streamFile(file, "text/html");
                file.close();
            } else if (LittleFS.exists("/index.html")) {
                File file = LittleFS.open("/index.html", "r");
                size_t sent = _server->streamFile(file, "text/html");
                file.close();
            } else {
                // Fallback to basic response
                String html = "<!DOCTYPE html><html><head><title>Table Clock</title></head><body>";
                html += "<h1>Table Clock</h1>";
                html += "<a href=\"/\">WiFi AP</a><br>";
                html += "<a href=\"/wifi_sta\">WiFi STA</a><br>";
                html += "<a href=\"/led\">LED</a>";
                html += "</body></html>";
                _server->send(200, "text/html", html);
            }
        }
    }
}