#include "WiFiManager.h"
#include "../../app_config.h"
#include <EEPROM.h>
#include <LittleFS.h>

WiFiManager::WiFiManager() : _server(nullptr), _credentialsLoaded(false), _hasCredentials(false), 
    _lastLEDToggle(0), _ledState(false), _ledPattern(0) {
    // Initialize the character arrays
    memset(_storedSsid, 0, sizeof(_storedSsid));
    memset(_storedPassword, 0, sizeof(_storedPassword));
    memset(_storedApSsid, 0, sizeof(_storedApSsid));
    memset(_storedApPassword, 0, sizeof(_storedApPassword));
}

void WiFiManager::begin(const char* ssid, const char* password, WiFiMode_t mode) {
    _mode = mode;
    _ssid = ssid;
    _password = password;
    
    // Initialize LittleFS
    if (!LittleFS.begin()) {
        // If LittleFS fails to mount, format and try again
        LittleFS.format();
        LittleFS.begin();
    }
    
    // Give some time for the system to settle before WiFi initialization
    delay(100);
    
    // Initialize WiFi with the requested mode
    WiFi.mode(_mode);
    delay(100); // Allow mode setting to take effect
    
    if (_mode == WIFI_AP || _mode == WIFI_AP_STA) {
        setupAP();
        if (_mode == WIFI_AP_STA) {
            delay(200); // Extra delay before setting up STA when in dual mode
        }
    }
    
    if (_mode == WIFI_STA || _mode == WIFI_AP_STA) {
        // Only attempt to connect if we have stored credentials
        String storedSsid = "";
        String storedPassword = "";
        if (loadStoredCredentials(storedSsid, storedPassword)) {
            // Only connect if both ssid and password are not empty
            if (storedSsid.length() > 0 && storedPassword.length() > 0) {
                setupSTA(storedSsid.c_str(), storedPassword.c_str());
            }
        } else {
            // No stored credentials for STA, don't try to connect to STA
            if (_mode == WIFI_STA) {
                // If we're only in STA mode and have no credentials, set to AP mode instead
                WiFi.mode(WIFI_AP);
                setupAP();
            }
        }
    }
    
    // Wait for WiFi to stabilize
    delay(500);
    
    // Setup web server
    _server = new ESP8266WebServer(80);
    if (_server) {
        _server->on("/", std::bind(&WiFiManager::handleRoot, this));
        _server->on("/config", std::bind(&WiFiManager::handleConfig, this));
        _server->on("/led", std::bind(&WiFiManager::handleLED, this));
        _server->on("/led_pattern", std::bind(&WiFiManager::handleLEDPattern, this));
        _server->on("/wifi_ap", std::bind(&WiFiManager::handleWifiAP, this));
        _server->on("/wifi_sta", std::bind(&WiFiManager::handleWifiSTA, this));
        _server->on("/forget_wifi", std::bind(&WiFiManager::handleForgetWifi, this));
        _server->on("/scan_wifi", std::bind(&WiFiManager::handleScanWifi, this));
        _server->begin();
    }
    
    // Additional delay after server setup
    delay(100);
    
    // Initialize built-in LED pin
    pinMode(BUILTIN_LED_PIN, OUTPUT);
    digitalWrite(BUILTIN_LED_PIN, HIGH); // LED off initially (inverted)
}

void WiFiManager::update() {
    // Add null check for server
    if (_server) {
        _server->handleClient();
    }
    
    if (_mode == WIFI_STA || _mode == WIFI_AP_STA) {
        if (WiFi.status() != WL_CONNECTED) {
            // Only try to reconnect if we have stored credentials
            String storedSsid = "";
            String storedPassword = "";
            if (loadStoredCredentials(storedSsid, storedPassword)) {
                // Only attempt to connect if both ssid and password are not empty
                if (storedSsid.length() > 0 && storedPassword.length() > 0) {
                    // Add a small delay before attempting to reconnect
                    delay(100);
                    // Try to reconnect
                    WiFi.begin(storedSsid.c_str(), storedPassword.c_str());
                }
            }
        } else {
            // Connected - display IP in console
            static bool ipDisplayed = false;
            if (!ipDisplayed) {
                Serial.print("STA Connected! IP Address: ");
                Serial.println(WiFi.localIP().toString());
                ipDisplayed = true;
            }
        }
        
        // Update built-in LED based on connection status
        updateBuiltinLED();
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

void WiFiManager::setLEDController(LEDController* ledController) {
    _ledController = ledController;
}

void WiFiManager::handleLED() {
    if (_server->method() == HTTP_POST) {
        // Handle LED pattern update
        String patternStr = _server->arg("pattern");
        String colorStr = _server->arg("color");
        uint16_t speed = _server->arg("speed").toInt();
        String directionStr = _server->arg("direction");
        
        if (_ledController) {
            PatternConfig config;
            
            // Parse pattern
            if (patternStr == "running_light") {
                config.pattern = LEDPattern::RUNNING_LIGHT;
            } else if (patternStr == "ping_pong") {
                config.pattern = LEDPattern::PING_PONG;
            } else {
                config.pattern = LEDPattern::RUNNING_LIGHT; // Default
            }
            
            // Parse color
            if (colorStr == "red") {
                config.color = RgbColor(255, 0, 0);
            } else if (colorStr == "green") {
                config.color = RgbColor(0, 255, 0);
            } else if (colorStr == "blue") {
                config.color = RgbColor(0, 0, 255);
            } else if (colorStr == "yellow") {
                config.color = RgbColor(255, 255, 0);
            } else if (colorStr == "purple") {
                config.color = RgbColor(255, 0, 255);
            } else if (colorStr == "cyan") {
                config.color = RgbColor(0, 255, 255);
            } else {
                config.color = RgbColor(255, 255, 255); // White default
            }
            
            // Parse direction
            config.direction = (directionStr == "forward");
            
            // Parse speed
            if (speed > 0 && speed <= 10000) { // Limit speed to reasonable values
                config.speed = speed;
            } else {
                config.speed = 500; // Default speed
            }
            
            // Apply the new pattern configuration
            _ledController->setPattern(config);
            
            // Save the configuration to persistent storage
            _ledController->savePatternConfig();
        }
    }
    
    // Serve led.html from LittleFS
    File file = LittleFS.open("/led.html.gz", "r");
    if (file) {
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        _server->send(404, "text/plain", "File not found");
    }
}

void WiFiManager::handleLEDPattern() {
    // Redirect to the main LED page since we now have a unified LED page
    _server->sendHeader("Location", "/led");
    _server->send(302, "text/plain", "");
}

void WiFiManager::setupAP() {
    String apSsid = "";
    String apPassword = "";
    
    // Check if we have stored AP credentials in EEPROM
    if (hasStoredApCredentials()) {
        if (loadStoredApCredentials(apSsid, apPassword)) {
            // Use stored AP credentials
            WiFi.softAP(apSsid.c_str(), apPassword.c_str());
        } else {
            // Use default credentials for AP mode if loading failed
            // Configure static IP for AP mode
            IPAddress ip(192, 168, 4, 1);
            IPAddress gateway(192, 168, 4, 1);
            IPAddress subnet(255, 255, 255, 0);
            
            WiFi.softAPConfig(ip, gateway, subnet);
            WiFi.softAP(DEFAULT_AP_SSID, DEFAULT_AP_PASSWORD);
            
            // Display AP connection information in console
            Serial.println("Access Point initialized with default settings:");
            Serial.print("SSID: ");
            Serial.println(DEFAULT_AP_SSID);
            Serial.print("Password: ");
            Serial.println(DEFAULT_AP_PASSWORD);
            Serial.print("IP Address: ");
            Serial.println(WiFi.softAPIP().toString());
        }
    } else {
        // Use default credentials for AP mode
        // Configure static IP for AP mode
        IPAddress ip(192, 168, 4, 1);
        IPAddress gateway(192, 168, 4, 1);
        IPAddress subnet(255, 255, 255, 0);
        
        WiFi.softAPConfig(ip, gateway, subnet);
        WiFi.softAP(DEFAULT_AP_SSID, DEFAULT_AP_PASSWORD);
        
        // Display AP connection information in console
        Serial.println("Access Point initialized with default settings:");
        Serial.print("SSID: ");
        Serial.println(DEFAULT_AP_SSID);
        Serial.print("Password: ");
        Serial.println(DEFAULT_AP_PASSWORD);
        Serial.print("IP Address: ");
        Serial.println(WiFi.softAPIP().toString());
    }
    delay(200); // Allow AP to start
}

void WiFiManager::setupSTA(const char* ssid, const char* password) {
    // If provided credentials are empty, try to use stored ones
    if (ssid == nullptr || password == nullptr || strlen(ssid) == 0 || strlen(password) == 0) {
        String storedSsid = "";
        String storedPassword = "";
        if (hasStoredCredentials()) {
            if (loadStoredCredentials(storedSsid, storedPassword)) {
                if (storedSsid.length() > 0 && storedPassword.length() > 0) {
                    WiFi.begin(storedSsid.c_str(), storedPassword.c_str());
                }
            }
        }
    } else {
        WiFi.begin(ssid, password);
    }
    
    delay(200); // Allow connection attempt to start
    
    // Wait for connection or timeout
    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 30) {
        delay(100);
        timeout++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("STA Connected! IP Address: ");
        Serial.println(WiFi.localIP().toString());
    }
}

void WiFiManager::handleRoot() {
    // Serve index.html from LittleFS
    File file = LittleFS.open("/index.html.gz", "r");
    if (file) {
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        _server->send(404, "text/plain", "File not found");
    }
}

void WiFiManager::handleConfig() {
    // Serve wifi_config.html from LittleFS
    File file = LittleFS.open("/wifi_config.html.gz", "r");
    if (file) {
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        _server->send(404, "text/plain", "File not found");
    }
}

bool WiFiManager::hasStoredCredentials() {
    // Check if we've already loaded credentials
    if (_credentialsLoaded) {
        return _hasCredentials;
    }
    
    // Initialize EEPROM
    EEPROM.begin(512);
    
    // Read the first few bytes to check if we have stored credentials
    // We'll use a simple approach: if the first byte is not 255 (uninitialized), we assume credentials exist
    char firstByte = EEPROM.read(0);
    EEPROM.end();
    
    bool hasCreds = firstByte != 255;
    _credentialsLoaded = true;
    _hasCredentials = hasCreds;
    
    if (!hasCreds) {
        memset(_storedSsid, 0, sizeof(_storedSsid));
        memset(_storedPassword, 0, sizeof(_storedPassword));
    }
    return hasCreds;
}

bool WiFiManager::loadStoredCredentials(String& ssid, String& password) {
    // Check if we've already loaded credentials
    if (_credentialsLoaded) {
        if (_hasCredentials) {
            // Make sure strings are cleared first
            ssid = "";
            password = "";
            ssid = String(_storedSsid);
            password = String(_storedPassword);
            return true;
        } else {
            // Make sure strings are cleared when no credentials found
            ssid = "";
            password = "";
            return false;
        }
    }
    
    // Initialize EEPROM
    EEPROM.begin(512);
    
    // Read the first byte to check if we have stored credentials
    char firstByte = EEPROM.read(0);
    if (firstByte == 255) {
        // No stored credentials
        EEPROM.end();
        _credentialsLoaded = true;
        _hasCredentials = false;
        memset(_storedSsid, 0, sizeof(_storedSsid));
        memset(_storedPassword, 0, sizeof(_storedPassword));
        return false;
    }
    
    // Read stored SSID length
    int ssidLen = EEPROM.read(1);
    if (ssidLen <= 0 || ssidLen > 32) {
        // Invalid length
        EEPROM.end();
        _credentialsLoaded = true;
        _hasCredentials = false;
        memset(_storedSsid, 0, sizeof(_storedSsid));
        memset(_storedPassword, 0, sizeof(_storedPassword));
        return false;
    }
    
    // Read SSID
    for (int i = 0; i < ssidLen; i++) {
        _storedSsid[i] = EEPROM.read(2 + i);
    }
    _storedSsid[ssidLen] = '\0';  // Null terminate
    
    // Read password length
    int pwdLen = EEPROM.read(2 + ssidLen);
    if (pwdLen < 0 || pwdLen > 64) {
        // Invalid length
        EEPROM.end();
        _credentialsLoaded = true;
        _hasCredentials = false;
        memset(_storedSsid, 0, sizeof(_storedSsid));
        memset(_storedPassword, 0, sizeof(_storedPassword));
        return false;
    }
    
    // Read password
    for (int i = 0; i < pwdLen; i++) {
        _storedPassword[i] = EEPROM.read(2 + ssidLen + 1 + i);
    }
    _storedPassword[pwdLen] = '\0';  // Null terminate
    
    // Cache the loaded credentials
    _credentialsLoaded = true;
    _hasCredentials = true;
    
    ssid = String(_storedSsid);
    password = String(_storedPassword);
    
    EEPROM.end();
    return true;
}

void WiFiManager::saveCredentials(const char* ssid, const char* password) {
    // Initialize EEPROM
    EEPROM.begin(512);
    
    int addr = 0;
    
    // Mark that we have stored credentials (use 1 to indicate presence)
    EEPROM.write(addr++, 1);
    
    // Store SSID length
    int ssidLen = strlen(ssid);
    EEPROM.write(addr++, ssidLen);
    
    // Store SSID
    for (int i = 0; i < ssidLen; i++) {
        EEPROM.write(addr++, ssid[i]);
    }
    
    // Store password length
    int pwdLen = strlen(password);
    EEPROM.write(addr++, pwdLen);
    
    // Store password
    for (int i = 0; i < pwdLen; i++) {
        EEPROM.write(addr++, password[i]);
    }
    
    // Commit to EEPROM
    EEPROM.commit();
    EEPROM.end();
    
    // Update cached values
    _credentialsLoaded = true;
    _hasCredentials = true;
    strncpy(_storedSsid, ssid, sizeof(_storedSsid) - 1);
    _storedSsid[sizeof(_storedSsid) - 1] = '\0';
    strncpy(_storedPassword, password, sizeof(_storedPassword) - 1);
    _storedPassword[sizeof(_storedPassword) - 1] = '\0';
}

bool WiFiManager::hasStoredApCredentials() {
    // Initialize EEPROM
    EEPROM.begin(512);
    
    // Check a different location in EEPROM for AP credentials
    // Use address 256 onwards for AP credentials to separate from STA credentials
    char firstByte = EEPROM.read(256);
    EEPROM.end();
    
    return firstByte != 255;
}

bool WiFiManager::loadStoredApCredentials(String& ssid, String& password) {
    // Initialize EEPROM
    EEPROM.begin(512);
    
    // Check if AP credentials exist (marked at address 256)
    char firstByte = EEPROM.read(256);
    if (firstByte == 255) {
        // No stored AP credentials
        EEPROM.end();
        return false;
    }
    
    // Read stored AP SSID length (at address 257)
    int ssidLen = EEPROM.read(257);
    if (ssidLen <= 0 || ssidLen > 32) {
        // Invalid length
        EEPROM.end();
        return false;
    }
    
    // Read AP SSID (starting at address 258)
    for (int i = 0; i < ssidLen; i++) {
        _storedApSsid[i] = EEPROM.read(258 + i);
    }
    _storedApSsid[ssidLen] = '\0';  // Null terminate
    
    // Read AP password length (after SSID data)
    int pwdLen = EEPROM.read(258 + ssidLen);
    if (pwdLen < 0 || pwdLen > 64) {
        // Invalid length
        EEPROM.end();
        return false;
    }
    
    // Read AP password (after length byte)
    for (int i = 0; i < pwdLen; i++) {
        _storedApPassword[i] = EEPROM.read(258 + ssidLen + 1 + i);
    }
    _storedApPassword[pwdLen] = '\0';  // Null terminate
    
    // Set return values
    ssid = String(_storedApSsid);
    password = String(_storedApPassword);
    
    EEPROM.end();
    return true;
}

void WiFiManager::saveApCredentials(const char* ssid, const char* password) {
    // Initialize EEPROM
    EEPROM.begin(512);
    
    int addr = 256; // Start at address 256 for AP credentials
    
    // Mark that we have stored AP credentials (use 1 to indicate presence)
    EEPROM.write(addr++, 1);
    
    // Store AP SSID length
    int ssidLen = strlen(ssid);
    EEPROM.write(addr++, ssidLen);
    
    // Store AP SSID
    for (int i = 0; i < ssidLen; i++) {
        EEPROM.write(addr++, ssid[i]);
    }
    
    // Store AP password length
    int pwdLen = strlen(password);
    EEPROM.write(addr++, pwdLen);
    
    // Store AP password
    for (int i = 0; i < pwdLen; i++) {
        EEPROM.write(addr++, password[i]);
    }
    
    // Commit to EEPROM
    EEPROM.commit();
    EEPROM.end();
    
    // Update cached values
    strncpy(_storedApSsid, ssid, sizeof(_storedApSsid) - 1);
    _storedApSsid[sizeof(_storedApSsid) - 1] = '\0';
    strncpy(_storedApPassword, password, sizeof(_storedApPassword) - 1);
    _storedApPassword[sizeof(_storedApPassword) - 1] = '\0';
}

void WiFiManager::handleWifiAP() {
    if (_server->method() == HTTP_POST) {
        // Handle AP configuration update
        String ssid = _server->arg("ssid");
        String password = _server->arg("password");
        
        if (ssid.length() > 0 && password.length() >= 8) { // Minimum 8 characters for WPA2
            // Save AP credentials to EEPROM
            saveApCredentials(ssid.c_str(), password.c_str());
            
            // Restart AP with new credentials
            WiFi.softAPdisconnect(true); // Disconnect current AP
            delay(100);
            WiFi.softAP(ssid.c_str(), password.c_str());
        }
    }
    
    // Load current AP settings
    String currentSsid, currentPassword;
    if (hasStoredApCredentials()) {
        if (loadStoredApCredentials(currentSsid, currentPassword)) {
            // Use stored AP credentials
        } else {
            // Fallback to defaults if loading failed
            currentSsid = DEFAULT_AP_SSID;
            currentPassword = DEFAULT_AP_PASSWORD;
        }
    } else {
        // Use default credentials
        currentSsid = DEFAULT_AP_SSID;
        currentPassword = DEFAULT_AP_PASSWORD;
    }
    
    // Serve wifi_ap.html from LittleFS
    File file = LittleFS.open("/wifi_ap.html.gz", "r");
    if (file) {
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        _server->send(404, "text/plain", "File not found");
    }
}

void WiFiManager::handleWifiSTA() {
    if (_server->method() == HTTP_POST) {
        // Handle STA configuration update
        String ssid = _server->arg("ssid");
        String password = _server->arg("password");
        
        if (ssid.length() > 0 && password.length() >= 8) { // Minimum 8 characters for WPA2
            // Save credentials to EEPROM
            saveCredentials(ssid.c_str(), password.c_str());
            
            // Attempt to connect to the new network
            WiFi.begin(ssid.c_str(), password.c_str());
        }
    }
    
    // Load current STA settings
    String currentSsid, currentPassword;
    if (hasStoredCredentials()) {
        if (loadStoredCredentials(currentSsid, currentPassword)) {
            // Use stored credentials
        } else {
            // No stored credentials
            currentSsid = "";
            currentPassword = "";
        }
    } else {
        // No stored credentials
        currentSsid = "";
        currentPassword = "";
    }
    
    // Serve wifi_sta.html from LittleFS
    File file = LittleFS.open("/wifi_sta.html.gz", "r");
    if (file) {
        _server->streamFile(file, "text/html");
        file.close();
    } else {
        _server->send(404, "text/plain", "File not found");
    }
}

void WiFiManager::handleForgetWifi() {
    if (_server->method() == HTTP_POST) {
        forgetCredentials();
        WiFi.disconnect(true); // Disconnect from network and clear stored credentials
        _server->send(200, "text/plain", "Credentials forgotten");
    } else {
        _server->send(405, "text/plain", "Method Not Allowed");
    }
}

void WiFiManager::forgetCredentials() {
    // Initialize EEPROM
    EEPROM.begin(512);
    
    // Clear STA credentials by writing 255 to the first byte
    EEPROM.write(0, 255);
    
    // Also clear the stored SSID and password length bytes
    for (int i = 1; i < 100; i++) {
        EEPROM.write(i, 0);
    }
    
    // Commit to EEPROM
    EEPROM.commit();
    EEPROM.end();
    
    // Update cached values
    _credentialsLoaded = true;
    _hasCredentials = false;
    memset(_storedSsid, 0, sizeof(_storedSsid));
    memset(_storedPassword, 0, sizeof(_storedPassword));
}

String WiFiManager::scanNetworks() {
    String result = "";
    
    // Scan for available networks
    int numNetworks = WiFi.scanNetworks();
    
    if (numNetworks == 0) {
        result = "<option value=\"\">No networks found</option>";
    } else {
        for (int i = 0; i < numNetworks; i++) {
            result += "<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + " dBm)</option>";
        }
    }
    
    WiFi.scanDelete(); // Clean up scan results
    return result;
}

void WiFiManager::updateBuiltinLED() {
    // Only control LED if we're in STA mode or AP+STA mode
    if (_mode != WIFI_STA && _mode != WIFI_AP_STA) {
        return;
    }
    
    unsigned long currentMillis = millis();
    bool isConnected = (WiFi.status() == WL_CONNECTED);
    
    // Determine the LED pattern based on connection state
    if (isConnected) {
        // Connected - LED constantly on
        _ledPattern = 3;
    } else if (hasStoredCredentials()) {
        // Has credentials but not connected - connecting (fast blink 4Hz)
        _ledPattern = 2;
    } else {
        // No credentials - slow blink once every 5 seconds
        _ledPattern = 1;
    }
    
    // Handle LED blinking based on pattern
    unsigned long interval;
    switch (_ledPattern) {
        case 1: // Slow blink - 5 seconds (2500ms on, 2500ms off)
            interval = 2500;
            break;
        case 2: // Fast blink - 4Hz (125ms on, 125ms off)
            interval = 125;
            break;
        case 3: // Constant on
            digitalWrite(BUILTIN_LED_PIN, LOW); // LED on (inverted)
            return;
        default: // Off
            digitalWrite(BUILTIN_LED_PIN, HIGH); // LED off (inverted)
            return;
    }
    
    // Toggle LED based on interval
    if (currentMillis - _lastLEDToggle >= interval) {
        _lastLEDToggle = currentMillis;
        _ledState = !_ledState;
        digitalWrite(BUILTIN_LED_PIN, _ledState ? HIGH : LOW);
    }
}

void WiFiManager::handleScanWifi() {
    _server->send(200, "text/html", scanNetworks());
}
