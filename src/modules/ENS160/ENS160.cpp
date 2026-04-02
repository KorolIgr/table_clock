#include "ENS160.h"

ENS160Wrapper::ENS160Wrapper() : _initialized(false) {
}

void ENS160Wrapper::begin(uint8_t sdaPin, uint8_t sclPin) {
    // Initialize I2C with the provided pins
    Wire.begin(sdaPin, sclPin);
    
    // Initialize the ENS160 sensor
    if (_sensor.begin(Wire)) {
        Serial.println("ENS160: Sensor initialized successfully");
        _initialized = true;
    } else {
        Serial.println("ENS160: Failed to initialize sensor");
        _initialized = false;
    }
}

bool ENS160Wrapper::isConnected() {
    return _sensor.isConnected();
}

void ENS160Wrapper::setCompensation(float temperature, float humidity) {
    if (!_initialized) return;
    _sensor.setTempCompensationCelsius(temperature);
    _sensor.setRHCompensationFloat(humidity);
}

bool ENS160Wrapper::read(float& tvoc, float& eco2, int& aqi) {
    if (!_initialized) {
        return false;
    }
    
    // Read data from sensor
    uint16_t tvocRaw = _sensor.getTVOC();
    uint16_t eco2Raw = _sensor.getECO2();
    uint8_t aqiRaw = _sensor.getAQI();
    
    // Check if data is valid? The library doesn't provide a direct status check for each read.
    // We assume if sensor is initialized, we can read values.
    if (tvocRaw == 0 && eco2Raw == 0 && aqiRaw == 0) {
        // Might be invalid, but we'll still return true as the read succeeded.
        // Could also check other status registers if needed.
    }
    
    tvoc = (float)tvocRaw;
    eco2 = (float)eco2Raw;
    aqi = (int)aqiRaw;
    
    return true;
}
