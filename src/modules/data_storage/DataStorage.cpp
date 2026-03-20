#include "DataStorage.h"

DataStorage::DataStorage() : _locked(false) {
    // Initialize with default values
    _sharedData.wifi_connected = false;
    _sharedData.wifi_status = "Not connected";
    _sharedData.ip_address = "0.0.0.0";
    _sharedData.led_enabled = true;
    _sharedData.led_brightness = 100;
    _sharedData.temperature = 0.0f;
    _sharedData.uptime = 0;
    _sharedData.system_error = false;
    _sharedData.last_error = "";
}

SharedData& DataStorage::getData() {
    return _sharedData;
}

void DataStorage::lock() {
    _locked = true;
}

void DataStorage::unlock() {
    _locked = false;
}

void DataStorage::updateWifiStatus(bool connected, const String& status, const String& ip) {
    if (!_locked) {
        _sharedData.wifi_connected = connected;
        _sharedData.wifi_status = status;
        _sharedData.ip_address = ip;
    }
}

void DataStorage::updateLedStatus(bool enabled, uint8_t brightness) {
    if (!_locked) {
        _sharedData.led_enabled = enabled;
        _sharedData.led_brightness = brightness;
    }
}

void DataStorage::updateSystemStatus(float temp, uint32_t uptime, bool error, const String& lastError) {
    if (!_locked) {
        _sharedData.temperature = temp;
        _sharedData.uptime = uptime;
        _sharedData.system_error = error;
        _sharedData.last_error = lastError;
    }
}