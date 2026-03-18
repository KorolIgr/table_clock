#include "../WiFiManager.h"

void WiFiManager::handleConfig() {
    String html = "<!DOCTYPE html><html><head><title>Config</title></head><body>";
    html += "<h1>Configuration</h1>";
    html += "<p>Configuration page placeholder</p>";
    html += "<a href=\"/\">Back to Home</a>";
    html += "</body></html>";
    _server->send(200, "text/html", html);
}