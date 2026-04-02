#ifndef AHT21_H
#define AHT21_H

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_Qwiic_Humidity_AHT20.h"

class AHT21Wrapper {
public:
    AHT21Wrapper();
    
    void begin(uint8_t sdaPin, uint8_t sclPin);
    bool isInitialized() const { return _initialized; }
    
    // Read temperature and humidity
    // Returns true on success, false on failure
    bool read(float& temperature, float& humidity);
    
    // Sensor status check
    bool isConnected();
    
private:
    bool _initialized;
    AHT20 _sensor;
};

#endif // AHT21_H
