#include "WebServer.h"
#include "../../app_config.h"

WebServer::WebServer()
    : _dataStorage(nullptr), _configManager(nullptr), _ledController(nullptr), _server(nullptr),
      _saveStaCallback(nullptr), _saveApCallback(nullptr), _forgetCallback(nullptr),
      _saveConfigCallback(nullptr), _scanCallback(nullptr),
      _getLEDPatternsCallback(nullptr), _getLEDCurrentCallback(nullptr), _applyLEDSettingsCallback(nullptr) {
}

void WebServer::setDataStorage(DataStorage* dataStorage) {
    _dataStorage = dataStorage;
}

void WebServer::setConfigManager(ConfigManager* configManager) {
    _configManager = configManager;
}

void WebServer::setLEDController(LEDController* ledController) {
    _ledController = ledController;
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

void WebServer::setGetLEDPatternsCallback(GetLEDPatternsCallback getPatternsCb) {
    _getLEDPatternsCallback = getPatternsCb;
}

void WebServer::setGetLEDCurrentCallback(GetLEDCurrentCallback getCurrentCb) {
    _getLEDCurrentCallback = getCurrentCb;
}

void WebServer::setApplyLEDSettingsCallback(ApplyLEDSettingsCallback applySettingsCb) {
    _applyLEDSettingsCallback = applySettingsCb;
}

void WebServer::begin() {

    LittleFS.begin();

    _server = new ESP8266WebServer(80);
    
    // API routes
    _server->on("/api/wifi_sta/scan", std::bind(&WebServer::handleWifiSTAScan, this));
    _server->on("/api/wifi_sta/forget", std::bind(&WebServer::handleWifiSTAForget, this));
    _server->on("/api/save_config", HTTP_POST, std::bind(&WebServer::handleSaveConfig, this));
    _server->on("/api/status", std::bind(&WebServer::handleStatus, this));
    _server->on("/api/led/patterns", std::bind(&WebServer::handleGetLEDPatterns, this));
    _server->on("/api/led/current", std::bind(&WebServer::handleGetLEDCurrent, this));
    _server->on("/api/weather/current", std::bind(&WebServer::handleGetCurrentWeather, this));
    
    // Form routes (used by HTML pages)
    _server->on("/wifi_sta", HTTP_POST, std::bind(&WebServer::handleWifiSTAApply, this));
    _server->on("/wifi_ap", HTTP_POST, std::bind(&WebServer::handleWifiApForm, this));
    _server->on("/led", HTTP_POST, std::bind(&WebServer::handleApplyLEDSettings, this));
    
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
        WiFiData& wifi = _dataStorage->wifi();
        LEDData& led = _dataStorage->led();
        response += "\"wifi_connected\":" + String(wifi.sta.connected ? "true" : "false") + ",";
        response += "\"wifi_status\":\"" + wifi.sta.status + "\",";
        response += "\"ip_address\":\"" + wifi.sta.ip_address + "\",";
        response += "\"led_enabled\":" + String(led.enabled ? "true" : "false") + ",";
        response += "\"led_brightness\":" + String(led.brightness);
        response += ",\"builtin_led_pattern\":" + String(led.builtin_led_pattern);
    }
    response += "}";
    
    _server->send(200, "application/json", response);
}

void WebServer::handleGetLEDPatterns() {
    if (!_getLEDPatternsCallback) {
        _server->send(500, "application/json", "{\"error\":\"Patterns callback not set\"}");
        return;
    }
    
    String response = _getLEDPatternsCallback();
    _server->send(200, "application/json", response);
}

void WebServer::handleGetLEDCurrent() {
    if (!_getLEDCurrentCallback) {
        _server->send(500, "application/json", "{\"error\":\"Current callback not set\"}");
        return;
    }
    
    String response = _getLEDCurrentCallback();
    _server->send(200, "application/json", response);
}

void WebServer::handleGetCurrentWeather() {
    if (!_dataStorage) {
        _server->send(500, "application/json", "{\"error\":\"Data storage not available\"}");
        return;
    }
    
    WeatherData& data = _dataStorage->weather();
    
    String response = "{";
    response += "\"valid\":" + String(data.current.valid ? "true" : "false") + ",";
    response += "\"temperature\":" + String(data.current.temperature, 1) + ",";
    response += "\"apparent_temperature\":" + String(data.current.apparent_temperature, 1) + ",";
    response += "\"wind_speed\":" + String(data.current.wind_speed, 1) + ",";
    response += "\"wind_direction\":" + String(data.current.wind_direction) + ",";
    response += "\"humidity\":" + String(data.current.humidity) + ",";
    response += "\"cloud_cover\":" + String(data.current.cloud_cover) + ",";
    response += "\"weather_code\":" + String(data.current.weather_code) + ",";
    response += "\"last_update\":" + String(data.current.last_update);
    if (!data.current.error.isEmpty()) {
        response += ",\"error\":\"" + data.current.error + "\"";
    }
    response += "}";
    
    _server->send(200, "application/json", response);
}

void WebServer::handleApplyLEDSettings() {
    if (!_server->hasArg("pattern") || !_server->hasArg("color") ||
        !_server->hasArg("speed") || !_server->hasArg("direction")) {
        _server->send(400, "application/json", "{\"error\":\"Missing required parameters\"}");
        return;
    }
    
    if (!_applyLEDSettingsCallback) {
        _server->send(500, "application/json", "{\"error\":\"Apply callback not set\"}");
        return;
    }
    
    String pattern = _server->arg("pattern");
    String color = _server->arg("color");
    uint16_t speed = _server->arg("speed").toInt();
    bool direction = _server->arg("direction") == "forward";
    
    _applyLEDSettingsCallback(pattern.c_str(), color.c_str(), speed, direction);
    
    _server->send(200, "application/json", "{\"status\":\"ok\"}");
}
