#include "AHT21.h"

AHT21Wrapper::AHT21Wrapper() : _initialized(false) {
}

void AHT21Wrapper::begin(uint8_t sdaPin, uint8_t sclPin) {
    // Initialize I2C with the provided pins
    Wire.begin(sdaPin, sclPin);
    
    // Initialize the AHT20 sensor
    if (_sensor.begin(Wire)) {
        Serial.println("AHT21: Sensor initialized successfully");
        _initialized = true;
    } else {
        Serial.println("AHT21: Failed to initialize sensor");
        _initialized = false;
    }
}

bool AHT21Wrapper::isConnected() {
    return _sensor.isConnected();
}

bool AHT21Wrapper::read(float& temperature, float& humidity) {
    if (!_initialized) {
        return false;
    }
    
    // The AHT20 library's getTemperature() and getHumidity() handle measurement automatically
    temperature = _sensor.getTemperature();
    humidity = _sensor.getHumidity();
    
    return true;
}
