#ifndef AIR_QUALITY_DATA_H
#define AIR_QUALITY_DATA_H

#include <Arduino.h>

// Air Quality Index (AQI) based on ENS160 standard
// Values: 1=Excellent, 2=Good, 3=Moderate, 4=Poor, 5=Unhealthy
struct AirQualityData {
    float tvoc = 0.0f;         // Total Volatile Organic Compounds in ppb
    float eco2 = 0.0f;         // Equivalent CO2 in ppm
    int aqi = 0;               // Air Quality Index (1-5)
    unsigned long last_update = 0;
    bool valid = false;
    String error = "";
    
    // Compensation data from AHT21
    float temperature = 0.0f;  // Temperature in Celsius for compensation
    float humidity = 0.0f;     // Relative humidity in % for compensation
};

#endif // AIR_QUALITY_DATA_H
