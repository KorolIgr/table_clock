#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>
#include "../led/LEDController.h"
#include "../../app_config.h"

// WiFi configuration structure
struct WiFiConfig {
    char sta_ssid[33];      // Max SSID length is 32 + null terminator
    char sta_password[65];  // Max password length is 64 + null terminator
    char ap_ssid[33];       // Max SSID length is 32 + null terminator
    char ap_password[65];   // Max password length is 64 + null terminator
    char ap_ip[16];         // IP address string (e.g., "192.168.4.1")
    
    WiFiConfig() {
        // Initialize with default values
        strcpy(sta_ssid, "");
        strcpy(sta_password, "");
        strcpy(ap_ssid, DEFAULT_AP_SSID);
        strcpy(ap_password, DEFAULT_AP_PASSWORD);
        strcpy(ap_ip, DEFAULT_AP_IP);
    }
};

// Complete configuration structure
struct DeviceConfig {
    WiFiConfig wifi;
    PatternConfig led;      // From LEDController.h
    
    DeviceConfig() : led() {}  // Initialize LED config with defaults
};

class ConfigManager {
public:
    ConfigManager();
    
    // Load configuration from EEPROM
    bool loadConfig(DeviceConfig& config);
    
    // Save configuration to EEPROM
    bool saveConfig(const DeviceConfig& config);
    
    // Get default configuration
    DeviceConfig getDefaultConfig();
    
    // Validate configuration integrity
    bool validateConfig(const DeviceConfig& config);

private:
    static const int EEPROM_SIZE = 1024;  // Reserve 1KB for configuration
    static const int CONFIG_VERSION = 1;  // Version for future compatibility
    
    // Calculate checksum for validation
    uint32_t calculateChecksum(const DeviceConfig& config);
    
    // Write configuration to EEPROM with version and checksum
    bool writeConfigToEEPROM(const DeviceConfig& config);
    
    // Read configuration from EEPROM
    bool readConfigFromEEPROM(DeviceConfig& config);
};

#endif // CONFIG_MANAGER_H