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
    
    // Define web server routes
    _server->on("/", std::bind(&WiFiManager::handleRoot, this));
    _server->on("/nav", std::bind(&WiFiManager::handleNav, this));
    _server->on("/led", std::bind(&WiFiManager::handleLED, this));
    _server->on("/wifi_ap", std::bind(&WiFiManager::handleWifiAP, this));
    _server->on("/wifi_sta", std::bind(&WiFiManager::handleWifiSTA, this));
    _server->on("/wifi_sta/forget", std::bind(&WiFiManager::handleForgetWifi, this));
    _server->on("/wifi_sta/scan", std::bind(&WiFiManager::handleScanWifi, this));
    
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
    
    _server->begin();
    
    // Update LED to reflect current WiFi state (after setupSTA may have changed pattern)
    updateBuiltinLED();
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

String WiFiManager::scanNetworks() {
    String networks = "[";
    
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            networks += ",";
        }
        networks += "{\"ssid\":\"" + WiFi.SSID(i) + "\",\"rssi\":" + WiFi.RSSI(i) + ",\"secure\":" + (WiFi.encryptionType(i) == ENC_TYPE_NONE ? "false" : "true") + "}";
    }
    networks += "]";
    
    return networks;
}

void WiFiManager::updateBuiltinLED() {
    if (!_ledController) return;
    
    unsigned long currentTime = millis();
    
    switch (_ledPattern) {
        case 0: // Off
            digitalWrite(BUILTIN_LED_PIN, HIGH); // Turn off built-in LED (active low)
            break;
        case 1: // Slow blink (5s interval)
            if (currentTime - _lastLEDToggle >= 5000) {
                _ledState = !_ledState;
                digitalWrite(BUILTIN_LED_PIN, _ledState ? LOW : HIGH); // Active low
                _lastLEDToggle = currentTime;
            }
            break;
        case 2: // Fast blink (4Hz)
            if (currentTime - _lastLEDToggle >= 250) { // 250ms = 4Hz
                _ledState = !_ledState;
                digitalWrite(BUILTIN_LED_PIN, _ledState ? LOW : HIGH); // Active low
                _lastLEDToggle = currentTime;
            }
            break;
        case 3: // On
            digitalWrite(BUILTIN_LED_PIN, LOW); // Turn on built-in LED (active low)
            break;
    }
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

void WiFiManager::setupSTA(const char* ssid, const char* password) {
    // Only connect if both ssid and password are provided
    if (ssid && password && strlen(ssid) > 0 && strlen(password) > 0) {
        WiFi.begin(ssid, password);

        // Set LED to fast blink while connecting
        _ledPattern = 2; // Fast blink

        Serial.print("Connecting to STA: ");
        Serial.println(ssid);

        // Wait for connection with timeout
        int attempts = 0;
        const int maxAttempts = 20; // 20 * 500ms = 10 seconds timeout
        while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
            delay(500);
            attempts++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("");
            Serial.print("STA Connected! IP address: ");
            Serial.println(WiFi.localIP());

            // Set LED to solid on when connected
            _ledPattern = 3; // On
        } else {
            Serial.println("");
            Serial.println("STA Connection failed!");

            // Set LED to slow blink when connection fails
            _ledPattern = 1; // Slow blink
        }
    } else {
        // No credentials provided, set LED to slow blink
        _ledPattern = 1; // Slow blink
    }
}

void WiFiManager::sendPageNotFound() {
    _server->send(404, "text/plain", "Page not found");
}

