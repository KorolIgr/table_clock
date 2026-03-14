#include "WiFiManager.h"
#include "../../app_config.h"
#include <EEPROM.h>

WiFiManager::WiFiManager() : _server(nullptr), _credentialsLoaded(false), _hasCredentials(false) {
    // Initialize the character arrays
    memset(_storedSsid, 0, sizeof(_storedSsid));
    memset(_storedPassword, 0, sizeof(_storedPassword));
}

void WiFiManager::begin(const char* ssid, const char* password, WiFiMode_t mode) {
    _mode = mode;
    _ssid = ssid;
    _password = password;
    
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
        // Only attempt to connect if we have stored credentials or if explicit credentials were provided
        if (strlen(ssid) > 0 && strlen(password) > 0) {
            // Save these credentials for future use
            saveCredentials(ssid, password);
            setupSTA(ssid, password);
        } else {
            // Check if we have stored credentials
            String storedSsid, storedPassword;
            if (loadStoredCredentials(storedSsid, storedPassword)) {
                setupSTA(storedSsid.c_str(), storedPassword.c_str());
            } else {
                // No connection data available, don't try to connect in STA mode
                if (_mode == WIFI_STA) {
                    // If we're only in STA mode and have no credentials, set to AP mode instead
                    WiFi.mode(WIFI_AP);
                    setupAP();
                }
            }
        }
    }
    
    // Wait for WiFi to stabilize
    delay(500);
    
    // Setup web server
    _server = new ESP8266WebServer(80);
    _server->on("/", std::bind(&WiFiManager::handleRoot, this));
    _server->on("/config", std::bind(&WiFiManager::handleConfig, this));
    _server->on("/led", std::bind(&WiFiManager::handleLED, this));
    _server->on("/led_pattern", std::bind(&WiFiManager::handleLEDPattern, this));
    _server->begin();
    
    // Additional delay after server setup
    delay(100);
}

void WiFiManager::update() {
    if (_server) {
        _server->handleClient();
    }
    
    if (_mode == WIFI_STA || _mode == WIFI_AP_STA) {
        if (WiFi.status() != WL_CONNECTED) {
            // Only try to reconnect if we have stored credentials
            String storedSsid, storedPassword;
            if (loadStoredCredentials(storedSsid, storedPassword)) {
                // Try to reconnect
                WiFi.begin(storedSsid.c_str(), storedPassword.c_str());
            }
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

void WiFiManager::setLEDController(LEDController* ledController) {
    _ledController = ledController;
}

void WiFiManager::handleLED() {
    if (_server->method() == HTTP_POST) {
        // Handle LED color update
        uint8_t red = _server->arg("red").toInt();
        uint8_t green = _server->arg("green").toInt();
        uint8_t blue = _server->arg("blue").toInt();
        
        if (_ledController) {
            _ledController->setAllLEDs(red, green, blue);
            _ledController->show();
        }
    }
    
    // Send LED control page
    String html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head><title>Table Clock - LED Control</title></head>";
    html += "<body>";
    html += "<h1>LED Control</h1>";
    html += "<form action='/led' method='post'>";
    html += "<label for='red'>Red:</label>";
    html += "<input type='range' id='red' name='red' min='0' max='255' value='0'><br><br>";
    
    html += "<label for='green'>Green:</label>";
    html += "<input type='range' id='green' name='green' min='0' max='255' value='0'><br><br>";
    
    html += "<label for='blue'>Blue:</label>";
    html += "<input type='range' id='blue' name='blue' min='0' max='255' value='0'><br><br>";
    
    html += "<input type='submit' value='Update'>";
    html += "</form>";
    
    // Add link to pattern selection page
    html += "<br><a href='/led_pattern'>LED Pattern Selection</a>";
    html += "</body>";
    html += "</html>";
    
    _server->send(200, "text/html", html);
}

void WiFiManager::handleLEDPattern() {
    if (_server->method() == HTTP_POST) {
        // Handle LED pattern update
        String patternStr = _server->arg("pattern");
        String directionStr = _server->arg("direction");
        uint16_t speed = _server->arg("speed").toInt();
        
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
            
            // Parse direction
            config.direction = (directionStr == "forward");
            
            // Parse speed
            if (speed > 0 && speed <= 10000) { // Limit speed to reasonable values
                config.speed = speed;
            } else {
                config.speed = 500; // Default speed
            }
            
            // Set default color
            config.color = RgbColor(255, 255, 255);
            
            // Apply the new pattern configuration
            _ledController->setPattern(config);
            
            // Save the configuration to persistent storage
            _ledController->savePatternConfig();
        }
    }
    
    // Get current configuration to show defaults
    PatternConfig currentConfig;
    if (_ledController) {
        currentConfig = _ledController->getCurrentConfig();
    } else {
        currentConfig.pattern = LEDPattern::RUNNING_LIGHT;
        currentConfig.direction = true;
        currentConfig.speed = 500;
        currentConfig.color = RgbColor(255, 255, 255);
    }
    
    // Send LED pattern selection page
    String html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head><title>Table Clock - LED Pattern Selection</title></head>";
    html += "<body>";
    html += "<h1>LED Pattern Selection</h1>";
    html += "<form action='/led_pattern' method='post'>";
    
    html += "<label for='pattern'>Pattern:</label><br>";
    html += "<input type='radio' id='running_light' name='pattern' value='running_light' checked>";
    html += "<label for='running_light'>Running Light</label><br>";
    html += "<input type='radio' id='ping_pong' name='pattern' value='ping_pong'>";
    html += "<label for='ping_pong'>Ping Pong</label><br><br>";
    
    html += "<label for='direction'>Direction:</label><br>";
    html += "<input type='radio' id='forward' name='direction' value='forward' checked>";
    html += "<label for='forward'>Forward</label><br>";
    html += "<input type='radio' id='reverse' name='direction' value='reverse'>";
    html += "<label for='reverse'>Reverse</label><br><br>";
    
    html += "<label for='speed'>Speed (ms):</label>";
    html += "<input type='number' id='speed' name='speed' min='50' max='2000' value='500'><br><br>";
    
    html += "<input type='submit' value='Apply Pattern'>";
    html += "</form>";
    html += "<br><a href='/led'>Back to LED Control</a>";
    html += "</body>";
    html += "</html>";
    
    _server->send(200, "text/html", html);
}

void WiFiManager::setupAP() {
    // Set mode to AP only temporarily
    WiFi.mode(WIFI_AP);
    delay(100);
    WiFi.softAP(DEFAULT_AP_SSID, DEFAULT_AP_PASSWORD);
    delay(200); // Allow AP to start
}

void WiFiManager::setupSTA(const char* ssid, const char* password) {
    // Set mode to STA only temporarily
    WiFi.mode(WIFI_STA);
    delay(100);
    
    // If provided credentials are empty, try to use stored ones
    if (strlen(ssid) == 0 || strlen(password) == 0) {
        String storedSsid, storedPassword;
        if (loadStoredCredentials(storedSsid, storedPassword)) {
            WiFi.begin(storedSsid.c_str(), storedPassword.c_str());
        }
    } else {
        WiFi.begin(ssid, password);
    }
    
    delay(200); // Allow connection attempt to start
}

void WiFiManager::handleRoot() {
    _server->send(200, "text/html", "<h1>Table Clock</h1><p>Home page</p>");
}

void WiFiManager::handleConfig() {
    _server->send(200, "text/html", "<h1>Configuration</h1><p>Config page</p>");
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
            ssid = String(_storedSsid);
            password = String(_storedPassword);
            return true;
        } else {
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
