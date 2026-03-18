#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleLED() {
    // Try to serve from LittleFS first
    if (LittleFS.exists("/led.html.gz")) {
        _server->sendHeader("Content-Encoding", "gzip");
        File file = LittleFS.open("/led.html.gz", "r");
        size_t sent = _server->streamFile(file, "text/html");
        file.close();
    } else if (LittleFS.exists("/led.html")) {
        File file = LittleFS.open("/led.html", "r");
        size_t sent = _server->streamFile(file, "text/html");
        file.close();
    } else {
        // Fallback to basic response
        String html = "<!DOCTYPE html><html><head><title>LED Control</title></head><body>";
        html += "<h1>LED Control</h1>";
        html += "<p>LED control page placeholder</p>";
        html += "<a href=\"/\">Back to Home</a>";
        html += "</body></html>";
        _server->send(200, "text/html", html);
    }
}

void WiFiManager::handleLEDPattern() {
    String html = "<!DOCTYPE html><html><head><title>LED Pattern</title></head><body>";
    html += "<h1>LED Pattern</h1>";
    html += "<p>LED pattern page placeholder</p>";
    html += "<a href=\"/\">Back to Home</a>";
    html += "</body></html>";
    _server->send(200, "text/html", html);
}