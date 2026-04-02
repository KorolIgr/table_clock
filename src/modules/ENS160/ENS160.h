#ifndef ENS160_H
#define ENS160_H

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_ENS160.h"

class ENS160Wrapper {
public:
    ENS160Wrapper();
    
    void begin(uint8_t sdaPin, uint8_t sclPin);
    bool isInitialized() const { return _initialized; }
    
    // Set temperature and humidity for compensation (in Celsius and %)
    void setCompensation(float temperature, float humidity);
    
    // Read air quality data
    // Returns true on success, false on failure
    bool read(float& tvoc, float& eco2, int& aqi);
    
    // Sensor status check
    bool isConnected();
    
private:
    bool _initialized;
    SparkFun_ENS160 _sensor;
};

#endif // ENS160_H
