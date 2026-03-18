#include "../../app_config.h"
#include <LittleFS.h>
#include "WiFiManager.h"
#include <DNSServer.h>
#include <ESP8266HTTPUpdateServer.h>

// Static variable to hold the instance for update handler
WiFiManager* wifiManagerInstance = nullptr;

WiFiManager::WiFiManager()
    : _server(nullptr), _ledController(nullptr), _credentialsLoaded(false), _hasCredentials(false) {
    _ssid = DEFAULT_AP_SSID;
    _password = DEFAULT_AP_PASSWORD;
    _localIP = "";
    wifiManagerInstance = this; // Store instance for update handler
}

void WiFiManager::begin(const char* ssid, const char* password, WiFiMode_t mode) {
    // Store the provided credentials
    _ssid = String(ssid);
    _password = String(password);
    
    // Set WiFi mode
    WiFi.mode(mode);
    
    // Setup AP and/or STA depending on mode
    if (mode == WIFI_AP || mode == WIFI_AP_STA) {
        setupAP();
    }
    
    if (mode == WIFI_STA || mode == WIFI_AP_STA) {
        setupSTA(ssid, password);
    }
    
    // Start web server
    _server = new ESP8266WebServer(80);
    
    // Define web server routes
    _server->on("/", std::bind(&WiFiManager::handleRoot, this));
    _server->on("/config", std::bind(&WiFiManager::handleConfig, this));
    _server->on("/led", std::bind(&WiFiManager::handleLED, this));
    _server->on("/led_pattern", std::bind(&WiFiManager::handleLEDPattern, this));
    _server->on("/wifi_ap", std::bind(&WiFiManager::handleWifiAP, this));
    _server->on("/wifi_sta", std::bind(&WiFiManager::handleWifiSTA, this));
    _server->on("/forget_wifi", std::bind(&WiFiManager::handleForgetWifi, this));
    _server->on("/scan_wifi", std::bind(&WiFiManager::handleScanWifi, this));
    
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
    
    // Initialize LED state tracking
    _ledState = false;
    _ledPattern = 1; // Start with slow blink
    _lastLEDToggle = 0;
    
    // Update LED to reflect current WiFi state
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

void WiFiManager::setupAP() {
    // Configure static IP for AP mode
    IPAddress ip(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP(_ssid.c_str(), _password.c_str());
    
    // Display AP connection information in console
    Serial.println("Access Point initialized:");
    Serial.print("SSID: ");
    Serial.println(_ssid);
    Serial.print("Password: ");
    Serial.println(_password);
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

void WiFiManager::handleConfig() {
    String html = "<!DOCTYPE html><html><head><title>Config</title></head><body>";
    html += "<h1>Configuration</h1>";
    html += "<p>Configuration page placeholder</p>";
    html += "<a href=\"/\">Back to Home</a>";
    html += "</body></html>";
    _server->send(200, "text/html", html);
}

void WiFiManager::handleLED() {
    String html = "<!DOCTYPE html><html><head><title>LED Control</title></head><body>";
    html += "<h1>LED Control</h1>";
    html += "<p>LED control page placeholder</p>";
    html += "<a href=\"/\">Back to Home</a>";
    html += "</body></html>";
    _server->send(200, "text/html", html);
}

void WiFiManager::handleLEDPattern() {
    String html = "<!DOCTYPE html><html><head><title>LED Pattern</title></head><body>";
    html += "<h1>LED Pattern</h1>";
    html += "<p>LED pattern page placeholder</p>";
    html += "<a href=\"/\">Back to Home</a>";
    html += "</body></html>";
    _server->send(200, "text/html", html);
}

void WiFiManager::handleWifiAP() {
    if (_server->method() == HTTP_POST) {
        String ssid = _server->arg("ssid");
        String password = _server->arg("password");
        
        if (ssid.length() > 0 && password.length() >= 8) { // Minimum 8 characters for WPA2
            // The actual saving will be handled by the main application
            _server->send(200, "text/plain", "Settings saved");
        } else {
            _server->send(400, "text/plain", "Invalid parameters");
        }
    } else {
        // Try to serve from LittleFS
        if (LittleFS.exists("/wifi_ap.html.gz")) {
            _server->sendHeader("Content-Encoding", "gzip");
            File file = LittleFS.open("/wifi_ap.html.gz", "r");
            size_t sent = _server->streamFile(file, "text/html");
            file.close();
        } else if (LittleFS.exists("/wifi_ap.html")) {
            File file = LittleFS.open("/wifi_ap.html", "r");
            size_t sent = _server->streamFile(file, "text/html");
            file.close();
        } else {
            // Fallback to basic form
            String html = "<!DOCTYPE html><html><head><title>WiFi AP Settings</title></head><body>";
            html += "<h1>WiFi AP Settings</h1>";
            html += "<form method=\"POST\">";
            html += "SSID: <input type=\"text\" name=\"ssid\" value=\"" + _ssid + "\" required><br>";
            html += "Password: <input type=\"password\" name=\"password\" value=\"" + _password + "\" required><br>";
            html += "<input type=\"submit\" value=\"Save\">";
            html += "</form>";
            html += "<a href=\"/\">Back to Home</a>";
            html += "</body></html>";
            _server->send(200, "text/html", html);
        }
    }
}

void WiFiManager::handleWifiSTA() {
    if (_server->method() == HTTP_POST) {
        String ssid = _server->arg("ssid");
        String password = _server->arg("password");
        
        if (ssid.length() > 0 && password.length() >= 8) { // Minimum 8 characters for WPA2
            // The actual saving will be handled by the main application
            _server->send(200, "text/plain", "Settings saved");
        } else {
            _server->send(400, "text/plain", "Invalid parameters");
        }
    } else {
        // Try to serve from LittleFS
        if (LittleFS.exists("/wifi_sta.html.gz")) {
            _server->sendHeader("Content-Encoding", "gzip");
            File file = LittleFS.open("/wifi_sta.html.gz", "r");
            size_t sent = _server->streamFile(file, "text/html");
            file.close();
        } else if (LittleFS.exists("/wifi_sta.html")) {
            File file = LittleFS.open("/wifi_sta.html", "r");
            size_t sent = _server->streamFile(file, "text/html");
            file.close();
        } else {
            // Fallback to basic form
            String html = "<!DOCTYPE html><html><head><title>WiFi STA Settings</title></head><body>";
            html += "<h1>WiFi STA Settings</h1>";
            html += "<form method=\"POST\">";
            html += "SSID: <input type=\"text\" name=\"ssid\" required><br>";
            html += "Password: <input type=\"password\" name=\"password\" required><br>";
            html += "<input type=\"submit\" value=\"Connect\">";
            html += "</form>";
            html += "<a href=\"/\">Back to Home</a>";
            html += "</body></html>";
            _server->send(200, "text/html", html);
        }
    }
}

void WiFiManager::handleForgetWifi() {
    // The actual forgetting will be handled by the main application
    _server->send(200, "text/plain", "WiFi credentials cleared");
}

void WiFiManager::handleScanWifi() {
    // Perform network scan
    int n = WiFi.scanNetworks();
    
    String json = "[";
    for (int i = 0; i < n; ++i) {
        if (i > 0) json += ",";
        json += "{";
        json += "\"ssid\":\"" + WiFi.SSID(i) + "\",";
        json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
        json += "\"secure\":" + String(WiFi.encryptionType(i) == ENC_TYPE_NONE ? "false" : "true");
        json += "}";
    }
    json += "]";
    
    _server->send(200, "application/json", json);
}
