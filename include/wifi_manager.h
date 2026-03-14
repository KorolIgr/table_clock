#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include "config.h"

// =============================================================================
// WiFi Manager for Table Clock
// Stage 4: WiFi and Network Functionality
//
// Features:
//  - Hybrid AP+STA mode (always runs AP for configuration)
//  - Web server for WiFi configuration
//  - Saves WiFi credentials to EEPROM
//  - Auto-connect on startup and reconnect on connection loss
//  - "Forget WiFi" functionality
//  - Captive portal in AP mode
// =============================================================================

// WiFi manager connection states
enum class WifiMgrState {
    IDLE,           // Not started
    AP_ONLY,        // Only AP mode active (no saved credentials or connection failed)
    CONNECTING,     // Attempting to connect to STA
    CONNECTED,      // Connected to STA + AP active
    RECONNECTING,   // Lost connection, attempting to reconnect
    DISCONNECTED    // STA disconnected, AP still active
};

// Structure for storing WiFi credentials in EEPROM
struct WiFiCredentials {
    uint32_t magic;             // Magic number to validate data
    char ssid[33];              // WiFi SSID (max 32 chars + null)
    char password[65];          // WiFi password (max 64 chars + null)
    bool hasCredentials;        // Flag: credentials are saved
    uint8_t checksum;           // Simple XOR checksum
};

class WiFiManager {
public:
    // Constructor
    WiFiManager();

    // Destructor
    ~WiFiManager();

    // Initialize WiFi manager
    // Loads saved credentials, starts AP, attempts STA connection if credentials exist
    void begin();

    // Must be called in main loop() to handle web server requests and reconnection
    void loop();

    // --- State ---

    // Get current WiFi manager state
    WifiMgrState getState() const { return _state; }

    // Check if connected to STA (external WiFi network)
    bool isConnected() const;

    // Check if AP is active
    bool isAPActive() const { return _apActive; }

    // Get current IP address (STA IP if connected, AP IP otherwise)
    IPAddress getIP() const;

    // Get STA IP address (0.0.0.0 if not connected)
    IPAddress getSTAIP() const;

    // Get AP IP address
    IPAddress getAPIP() const;

    // Get connected SSID (empty string if not connected)
    String getConnectedSSID() const;

    // Get WiFi signal strength (RSSI), 0 if not connected
    int32_t getRSSI() const;

    // Get state as human-readable string
    String getStateString() const;

    // --- Credentials Management ---

    // Save WiFi credentials to EEPROM
    bool saveCredentials(const String& ssid, const String& password);

    // Load WiFi credentials from EEPROM
    bool loadCredentials(String& ssid, String& password);

    // Check if credentials are saved
    bool hasCredentials() const;

    // Forget saved WiFi credentials and disconnect from STA
    void forgetCredentials();

    // --- Connection Control ---

    // Connect to WiFi with given credentials
    // Returns true if connection started (not necessarily successful)
    bool connectToWiFi(const String& ssid, const String& password);

    // Disconnect from STA WiFi
    void disconnectSTA();

    // Scan for available WiFi networks
    // Returns number of networks found
    int scanNetworks();

    // Get scan results as JSON string
    String getScanResultsJSON() const;

    // --- Callbacks ---

    // Set callback for when STA connection is established
    void onConnected(std::function<void(const String& ssid, IPAddress ip)> callback);

    // Set callback for when STA connection is lost
    void onDisconnected(std::function<void()> callback);

    // Set callback for when new credentials are saved via web interface
    void onCredentialsSaved(std::function<void(const String& ssid)> callback);

private:
    // Web server instance
    ESP8266WebServer _server;

    // DNS server for captive portal
    DNSServer _dnsServer;

    // Current state
    WifiMgrState _state;

    // AP active flag
    bool _apActive;

    // Reconnection tracking
    uint32_t _lastReconnectAttempt;
    uint8_t _reconnectRetries;

    // Connection attempt tracking
    uint32_t _connectStartTime;
    bool _connectingToSTA;

    // Scan results cache
    int _scanCount;

    // Callbacks
    std::function<void(const String& ssid, IPAddress ip)> _onConnectedCb;
    std::function<void()> _onDisconnectedCb;
    std::function<void(const String& ssid)> _onCredentialsSavedCb;

    // --- Internal Methods ---

    // Start Access Point
    void startAP();

    // Stop Access Point
    void stopAP();

    // Setup web server routes
    void setupWebServer();

    // Handle reconnection logic
    void handleReconnection();

    // Check STA connection status
    void checkSTAConnection();

    // Calculate checksum for credentials
    uint8_t calculateChecksum(const WiFiCredentials& creds) const;

    // Validate loaded credentials
    bool validateCredentials(const WiFiCredentials& creds) const;

    // --- Web Server Handlers ---

    // GET / - Main configuration page
    void handleRoot();

    // GET /time - Time settings page
    void handleTimePage();

    // GET /leds - LED settings page
    void handleLEDsPage();

    // GET /settings - General settings page
    void handleSettingsPage();

    // GET /scan - Scan for WiFi networks, return JSON
    void handleScan();

    // POST /connect - Connect to WiFi with provided credentials
    void handleConnect();

    // POST /forget - Forget saved WiFi credentials
    void handleForget();

    // GET /status - Get current WiFi status as JSON
    void handleStatus();

    // POST /save_time_settings - Save time settings
    void handleSaveTimeSettings();

    // POST /sync_time - Sync time from NTP
    void handleSyncTime();

    // GET /current_time - Get current time
    void handleCurrentTime();

    // POST /save_led_settings - Save LED settings
    void handleSaveLEDSettings();

    // POST /test_leds - Test LEDs
    void handleTestLEDs();

    // POST /turn_off_leds - Turn off all LEDs
    void handleTurnOffLEDs();

    // POST /set_led_pattern - Set LED pattern temporarily
    void handleSetLEDPattern();

    // POST /run_led_pattern - Run LED pattern once
    void handleRunLEDPattern();

    // POST /save_general_settings - Save general settings
    void handleSaveGeneralSettings();

    // GET /device_info - Get device information
    void handleDeviceInfo();

    // POST /reboot - Reboot device
    void handleReboot();

    // POST /factory_reset - Factory reset
    void handleFactoryReset();

    // Handle 404 / captive portal redirect
    void handleNotFound();

    // Generate main HTML page
    String generateMainPage() const;

    // Generate time settings page
    String generateTimePage() const;

    // Generate LED settings page
    String generateLEDsPage() const;

    // Generate general settings page
    String generateSettingsPage() const;

    // Generate status JSON
    String generateStatusJSON() const;
};
