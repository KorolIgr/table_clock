#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>

void WiFiManager::handleWifiApAplly() {
    // The actual forgetting will be handled by the main application
    _server->send(200, "text/plain", "WiFi credentials cleared");
}

void WiFiManager::setupAP(const char* ssid, const char* password) {
    // Configure static IP for AP mode
    IPAddress ip(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP(ssid, password);
    
    // Display AP connection information in console
    Serial.println("Access Point initialized:");
    Serial.print("SSID: ");
    Serial.println(String(ssid));
    Serial.print("Password: ");
    Serial.println(String(password));
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP().toString());
}
