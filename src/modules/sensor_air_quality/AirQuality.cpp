#include "AirQuality.h"

// ENS160 flag definitions (from SparkFun_ENS160 library)
// These should match the library's definitions
#ifndef SFE_ENS160_ERROR_I2C
#define SFE_ENS160_ERROR_I2C      0x01
#define SFE_ENS160_ERROR_ID       0x02
#define SFE_ENS160_ERROR_SENSOR   0x04
#define SFE_ENS160_DATA_VALID     0x10
#endif

AirQuality::AirQuality(DataStorage* dataStorage)
    : _dataStorage(dataStorage), _initialized(false), _aht21Connected(false), _ens160Connected(false), _lastError(""), _lastUpdateAttempt(0) {
}

void AirQuality::begin(uint8_t sdaPin, uint8_t sclPin) {
    // Initialize I2C with the provided pins
    Wire.begin(sdaPin, sclPin);
    
    // Initialize AHT21 sensor first
    if (_aht21.begin(Wire)) {
        Serial.println("AirQuality: AHT21 sensor initialized successfully");
        _aht21Connected = true;
    } else {
        Serial.println("AirQuality: Failed to initialize AHT21 sensor");
        _aht21Connected = false;
    }
    
    // Initialize ENS160 sensor
    if (_ens160.begin(Wire)) {
        Serial.println("AirQuality: ENS160 sensor initialized successfully");
        _ens160.setOperatingMode(SFE_ENS160_RESET);
        delay(100);
        _ens160.setOperatingMode(SFE_ENS160_STANDARD);
        _ens160Connected = true;
    } else {
        Serial.println("AirQuality: Failed to initialize ENS160 sensor");
        _ens160Connected = false;
    }
    
    // Overall initialized if both sensors are connected
    _initialized = _aht21Connected && _ens160Connected;
}

void AirQuality::update() {
    if (!_dataStorage) return;
    
    // Check if 10 seconds have passed since last update
    if (millis() - _lastUpdateAttempt >= UPDATE_INTERVAL) {
        _lastUpdateAttempt = millis();
        readAndUpdate();
    }
}

bool AirQuality::isConnected() {
    // Check both sensors
    bool aht21Ok = _aht21Connected && _aht21.isConnected();
    bool ens160Ok = _ens160Connected && _ens160.isConnected();
    return aht21Ok && ens160Ok;
}

void AirQuality::readAndUpdate() {
    if (!_initialized) {
        _lastError = "Not initialized";
        _dataStorage->updateAirQuality(0, 0, 0, 0, false, _lastError);
        return;
    }
    
    // Clear previous error
    _lastError = "";
    
    if(_aht21.available() == true){
        float temperature = _aht21.getTemperature();
        float humidity = _aht21.getHumidity();

        _dataStorage->updateAirQualityCompensation(temperature, humidity);

        
    }
    
    if (_ens160.checkDataStatus()) {
        uint16_t tvocRaw = _ens160.getTVOC();
        uint16_t eco2Raw = _ens160.getECO2();
        uint8_t aqiRaw = _ens160.getAQI();
        uint8_t flags = _ens160.getFlags();

        float tvoc = (float)tvocRaw;
        float eco2 = (float)eco2Raw;
        int aqi = (int)aqiRaw;

        _dataStorage->updateAirQuality(tvoc, eco2, aqi, flags, true, "");

        _ens160.setTempCompensationCelsius(_dataStorage->airQuality().temperature);
        _ens160.setRHCompensationFloat(_dataStorage->airQuality().humidity);
    }
    
}
