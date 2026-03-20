#include "WebServer.h"
#include "../../app_config.h"

WebServer::WebServer()
    : _dataStorage(nullptr), _configManager(nullptr), _server(nullptr),
      _saveStaCallback(nullptr), _saveApCallback(nullptr), _forgetCallback(nullptr),
      _saveConfigCallback(nullptr), _scanCallback(nullptr) {
}

void WebServer::setDataStorage(DataStorage* dataStorage) {
    _dataStorage = dataStorage;
}

void WebServer::setConfigManager(ConfigManager* configManager) {
    _configManager = configManager;
}

void WebServer::setCallbacks(SaveWifiStaCallback saveStaCb, SaveWifiApCallback saveApCb, ForgetWifiCallback forgetCb) {
    _saveStaCallback = saveStaCb;
    _saveApCallback = saveApCb;
    _forgetCallback = forgetCb;
}

void WebServer::setSaveConfigCallback(SaveConfigCallback saveConfigCb) {
    _saveConfigCallback = saveConfigCb;
}

void WebServer::setScanCallback(ScanNetworksCallback scanCb) {
    _scanCallback = scanCb;
}

void WebServer::begin() {

    LittleFS.begin();

    _server = new ESP8266WebServer(80);
    
    // API routes
    _server->on("/api/wifi_sta/scan", std::bind(&WebServer::handleWifiSTAScan, this));
    _server->on("/api/wifi_sta/forget", std::bind(&WebServer::handleWifiSTAForget, this));
    _server->on("/api/save_config", HTTP_POST, std::bind(&WebServer::handleSaveConfig, this));
    _server->on("/api/status", std::bind(&WebServer::handleStatus, this));
    
    // Form routes (used by HTML pages)
    _server->on("/wifi_sta", HTTP_POST, std::bind(&WebServer::handleWifiSTAApply, this));
    _server->on("/wifi_ap", HTTP_POST, std::bind(&WebServer::handleWifiApForm, this));
    
    // Serve static files from LittleFS
    _server->serveStatic("/", LittleFS, "/");
    
    _server->onNotFound(std::bind(&WebServer::handleNotFound, this));
    
    _server->begin();
    
    Serial.println("Web server started on port 80");
}

void WebServer::update() {
    if (_server) {
        _server->handleClient();
    }
}

void WebServer::handleNotFound() {
    String path = _server->uri();
    
    if (path.endsWith("/")) {
        path += "index.html";
    } else if (!path.endsWith(".html")) {
        path += ".html";
    }
    
    // Check for gzipped version first
    if (LittleFS.exists(path + ".gz")) {
        File file = LittleFS.open(path + ".gz", "r");
        _server->streamFile(file, "text/html");
        file.close();
        return;
    }
    
    // Then regular file
    if (LittleFS.exists(path)) {
        File file = LittleFS.open(path, "r");
        _server->streamFile(file, "text/html");
        file.close();
        return;
    }
    
    _server->send(404, "text/plain", "Not found");
}

void WebServer::handleWifiSTAScan() {
    if (!_scanCallback) {
        _server->send(500, "text/plain", "Scan callback not set");
        return;
    }
    
    String response = _scanCallback();
    _server->send(200, "application/json", response);
}

void WebServer::handleWifiSTAApply() {
    if (!_server->hasArg("ssid") || !_server->hasArg("password")) {
        _server->send(400, "text/plain", "Missing ssid or password");
        return;
    }
    
    String ssid = _server->arg("ssid");
    String password = _server->arg("password");
    
    if (_saveStaCallback) {
        _saveStaCallback(ssid.c_str(), password.c_str());
        _server->send(200, "text/plain", "OK");
    } else {
        _server->send(500, "text/plain", "Callback not set");
    }
}

void WebServer::handleWifiSTAForget() {
    if (_forgetCallback) {
        _forgetCallback();
        _server->send(200, "text/plain", "OK");
    } else {
        _server->send(500, "text/plain", "Callback not set");
    }
}

void WebServer::handleWifiApForm() {
    if (!_server->hasArg("ssid") || !_server->hasArg("password")) {
        _server->send(400, "text/plain", "Missing ssid or password");
        return;
    }
    
    String ssid = _server->arg("ssid");
    String password = _server->arg("password");
    String ip = _server->hasArg("ip") ? _server->arg("ip") : "192.168.4.1";
    
    if (_saveApCallback) {
        _saveApCallback(ssid.c_str(), password.c_str(), ip.c_str());
        _server->send(200, "text/plain", "OK");
    } else {
        _server->send(500, "text/plain", "Callback not set");
    }
}

// Alias for API endpoint if needed
void WebServer::handleWifiApApply() {
    // For API consistency, use same handler but without IP? Actually API might also want IP.
    // We'll just call handleWifiApForm to keep logic centralized.
    handleWifiApForm();
}

void WebServer::handleSaveConfig() {
    if (_saveConfigCallback) {
        _saveConfigCallback();
        _server->send(200, "text/plain", "Configuration saved");
    } else {
        _server->send(500, "text/plain", "Save callback not set");
    }
}

void WebServer::handleStatus() {
    String response = "{";
    if (_dataStorage) {
        auto& data = _dataStorage->getData();
        response += "\"wifi_connected\":" + String(data.wifi_connected ? "true" : "false") + ",";
        response += "\"wifi_status\":\"" + data.wifi_status + "\",";
        response += "\"ip_address\":\"" + data.ip_address + "\",";
        response += "\"led_enabled\":" + String(data.led_enabled ? "true" : "false") + ",";
        response += "\"led_brightness\":" + String(data.led_brightness);
        // Note: trailing comma before last field is okay because we add builtin_led_pattern after
        response += ",\"builtin_led_pattern\":" + String(data.builtin_led_pattern);
    }
    response += "}";
    
    _server->send(200, "application/json", response);
}
