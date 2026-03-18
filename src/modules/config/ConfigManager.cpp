#include "ConfigManager.h"
#include <cstring>

// Define static members
const int ConfigManager::EEPROM_SIZE;
const int ConfigManager::CONFIG_VERSION;

ConfigManager::ConfigManager() {
    // Initialize EEPROM
    EEPROM.begin(EEPROM_SIZE);
}

bool ConfigManager::loadConfig(DeviceConfig& config) {
    // Read configuration from EEPROM
    if (!readConfigFromEEPROM(config)) {
        // If reading fails, use default configuration
        config = getDefaultConfig();
        return false;
    }
    
    // Validate the loaded configuration
    if (!validateConfig(config)) {
        // If validation fails, use default configuration
        config = getDefaultConfig();
        return false;
    }
    
    return true;
}

bool ConfigManager::saveConfig(const DeviceConfig& config) {
    // Validate the configuration before saving
    if (!validateConfig(config)) {
        return false;
    }
    
    return writeConfigToEEPROM(config);
}

DeviceConfig ConfigManager::getDefaultConfig() {
    DeviceConfig config;
    // Default values are already initialized in the constructor
    return config;
}

bool ConfigManager::validateConfig(const DeviceConfig& config) {
    // Basic validation of configuration values
    
    // Check if SSIDs are reasonable lengths
    if (strlen(config.wifi.sta_ssid) > 32 || strlen(config.wifi.ap_ssid) > 32) {
        return false;
    }
    
    // Check if passwords are reasonable lengths
    if (strlen(config.wifi.sta_password) > 64 || strlen(config.wifi.ap_password) > 64) {
        return false;
    }
    
    // Check if IP address format is reasonable
    if (strlen(config.wifi.ap_ip) > 15) {
        return false;
    }
    
    // Validate LED pattern is within valid range
    if (static_cast<int>(config.led.pattern) < 0 || 
        static_cast<int>(config.led.pattern) > 1) {  // Assuming 0-1 are valid patterns
        return false;
    }
    
    // Validate speed is within reasonable range
    if (config.led.speed < 50 || config.led.speed > 5000) {
        return false;
    }
    
    return true;
}

uint32_t ConfigManager::calculateChecksum(const DeviceConfig& config) {
    // Simple checksum calculation by summing all bytes
    uint32_t checksum = 0;
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&config);
    size_t size = sizeof(DeviceConfig);
    
    for (size_t i = 0; i < size; ++i) {
        checksum += ptr[i];
    }
    
    return checksum;
}

bool ConfigManager::writeConfigToEEPROM(const DeviceConfig& config) {
    // Start from address 0
    int addr = 0;
    
    // Write version
    EEPROM.put(addr, CONFIG_VERSION);
    addr += sizeof(CONFIG_VERSION);
    
    // Write configuration data
    EEPROM.put(addr, config);
    addr += sizeof(DeviceConfig);
    
    // Write checksum
    uint32_t checksum = calculateChecksum(config);
    EEPROM.put(addr, checksum);
    addr += sizeof(checksum);
    
    // Commit to EEPROM
    bool success = EEPROM.commit();
    
    return success;
}

bool ConfigManager::readConfigFromEEPROM(DeviceConfig& config) {
    int addr = 0;
    
    // Read version
    int version;
    EEPROM.get(addr, version);
    addr += sizeof(version);
    
    // Check if version matches
    if (version != CONFIG_VERSION) {
        return false;
    }
    
    // Read configuration data
    EEPROM.get(addr, config);
    addr += sizeof(DeviceConfig);
    
    // Read checksum
    uint32_t storedChecksum;
    EEPROM.get(addr, storedChecksum);
    
    // Verify checksum
    uint32_t calculatedChecksum = calculateChecksum(config);
    if (calculatedChecksum != storedChecksum) {
        return false;
    }
    
    return true;
}