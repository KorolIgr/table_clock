#include "../../app_config.h"
#include <LittleFS.h>
#include "WiFiManager.h"
#include <DNSServer.h>
#include <ESP8266HTTPUpdateServer.h>

// Static variable to hold the instance for update handler
WiFiManager* wifiManagerInstance = nullptr;

WiFiManager::WiFiManager()
    : _server(nullptr), _ledController(nullptr), _credentialsLoaded(false), _hasCredentials(false) {
    // Initialize AP credentials with defaults


    _localIP = "";
    // Initialize LED state
    _ledState = false;
    _ledPattern = 1; // Slow blink by default (no STA connection)
    _lastLEDToggle = 0;
    wifiManagerInstance = this; // Store instance for update handler
}

void WiFiManager::begin(const char* ap_ssid, const char* ap_password,
                        const char* sta_ssid, const char* sta_password,
                        WiFiMode_t mode) {



    
    // Reset LED state before setup (will be updated by setupSTA)
    _ledState = false;
    _ledPattern = 1; // Default: slow blink (no STA connection)
    _lastLEDToggle = 0;
    
    // Set WiFi mode
    WiFi.mode(mode);
    
    // Setup AP (always, if mode includes AP)
    if (mode == WIFI_AP || mode == WIFI_AP_STA) {
        setupAP(ap_ssid, ap_password);
    }
    
    // Setup STA (only if mode includes STA and STA credentials are provided)
    if (mode == WIFI_STA || mode == WIFI_AP_STA) {
        setupSTA(sta_ssid, sta_password);
    }

    // Initialize LittleFS
    if (!LittleFS.begin()) {
        // If LittleFS fails to mount, format and try again
        LittleFS.format();
        LittleFS.begin();
    }
    
    // Give some time for the system to settle before WiFi initialization
    delay(100);
    
    // Start web server
    _server = new ESP8266WebServer(80);
    
    _server->on("/api/wifi_sta/scan", std::bind(&WiFiManager::handleWifiSTAScan, this));
    _server->on("/api/wifi_sta/forget", std::bind(&WiFiManager::handleWifiSTAForget, this));
    
    /*
    // Handle file system routes
    _server->on("/update", HTTP_POST, []() {
        if (wifiManagerInstance) {
            wifiManagerInstance->_server->send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        }
    }, []() {
        if (wifiManagerInstance) {
            HTTPUpload& upload = wifiManagerInstance->_server->upload();
            if (upload.status == UPLOAD_FILE_START) {
                WiFiUDP::stopAll();
                uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
                if (!Update.begin(maxSketchSpace)) {
                    Update.printError(Serial);
                }
            } else if (upload.status == UPLOAD_FILE_WRITE) {
                if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
                    Update.printError(Serial);
                }
            } else if (upload.status == UPLOAD_FILE_END) {
                if (Update.end(true)) {
                    Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
                } else {
                    Update.printError(Serial);
                }
            }
        }
    });
   */ 
    _server->serveStatic("/", LittleFS, "/");

    _server->onNotFound(std::bind(&WiFiManager::handleNotFound, this));

    _server->begin();
    
    // Update LED to reflect current WiFi state (after setupSTA may have changed pattern)
    updateBuiltinLED();
}

void WiFiManager::handleNotFound() {
    String path = _server->uri();

    if (path.endsWith("/")) {
        path += "index.html";
    } else if (!path.endsWith(".html")) {
        path += ".html";
    }

    // 🔥 1. Сначала проверяем gzip
    if (LittleFS.exists(path + ".gz")) {
        File file = LittleFS.open(path + ".gz", "r");
        _server->streamFile(file, "text/html");
        file.close();
        return;
    }

    // 2. Потом обычный файл
    if (LittleFS.exists(path)) {
        File file = LittleFS.open(path, "r");
        _server->streamFile(file, "text/html");
        file.close();
        return;
    }

    // 3. Если ничего не нашли
    _server->send(404, "text/plain", "Not found");
}

void WiFiManager::update() {
    if (_server) {
        _server->handleClient();
    }
    
    // Update LED based on current state
    updateBuiltinLED();
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

WiFiMode_t WiFiManager::getMode() {
    return WiFi.getMode();
}

const char* WiFiManager::getLocalIP() {
    _localIP = WiFi.localIP().toString();
    return _localIP.c_str();
}

ESP8266WebServer* WiFiManager::getServer() {
    return _server;
}

void WiFiManager::setLEDController(LEDController* ledController) {
    _ledController = ledController;
}

void WiFiManager::saveCredentials(const char* ssid, const char* password) {
    // Call the callback function to handle the saving
    if (_saveStaCallback) {
        _saveStaCallback(ssid, password);
    }
}

void WiFiManager::saveApCredentials(const char* ssid, const char* password) {
    // Call the callback function to handle the saving
    if (_saveApCallback) {
        _saveApCallback(ssid, password);
    }
}

void WiFiManager::forgetCredentials() {
    // Call the callback function to handle the clearing
    if (_forgetCallback) {
        _forgetCallback();
    }
}




void WiFiManager::sendPageNotFound() {
    _server->send(404, "text/plain", "Page not found");
}

