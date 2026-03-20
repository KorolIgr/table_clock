#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleLedApply() {
    // The actual forgetting will be handled by the main application
    _server->send(200, "text/plain", "WiFi credentials cleared");
}