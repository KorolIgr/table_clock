#include "WiFiManager.h"
#include "../../app_config.h"

WiFiManager::WiFiManager() : _server(nullptr) {
}

void WiFiManager::begin(const char* ssid, const char* password, WiFiMode_t mode) {
    _mode = mode;
    _ssid = ssid;
    _password = password;
    
    if (_mode == WIFI_AP || _mode == WIFI_AP_STA) {
        setupAP();
    }
    
    if (_mode == WIFI_STA || _mode == WIFI_AP_STA) {
        setupSTA(ssid, password);
    }
    
    // Setup web server
    _server = new ESP8266WebServer(80);
    _server->on("/", std::bind(&WiFiManager::handleRoot, this));
    _server->on("/config", std::bind(&WiFiManager::handleConfig, this));
    _server->begin();
}

void WiFiManager::update() {
    if (_server) {
        _server->handleClient();
    }
    
    if (_mode == WIFI_STA || _mode == WIFI_AP_STA) {
        if (WiFi.status() != WL_CONNECTED) {
            // Try to reconnect
            WiFi.begin(_ssid.c_str(), _password.c_str());
        }
    }
}

bool WiFiManager::isConnected() {
    if (_mode == WIFI_AP) {
        return true;  // AP always "connected"
    }
    return WiFi.status() == WL_CONNECTED;
}

WiFiMode_t WiFiManager::getMode() {
    return _mode;
}

const char* WiFiManager::getLocalIP() {
    if (WiFi.status() == WL_CONNECTED) {
        _localIP = WiFi.localIP().toString();
    } else if (_mode == WIFI_AP) {
        _localIP = WiFi.softAPIP().toString();
    }
    return _localIP.c_str();
}

ESP8266WebServer* WiFiManager::getServer() {
    return _server;
}

void WiFiManager::setupAP() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(DEFAULT_AP_SSID, DEFAULT_AP_PASSWORD);
}

void WiFiManager::setupSTA(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
}

void WiFiManager::handleRoot() {
    _server->send(200, "text/html", "<h1>Table Clock</h1><p>Home page</p>");
}

void WiFiManager::handleConfig() {
    _server->send(200, "text/html", "<h1>Configuration</h1><p>Config page</p>");
}
