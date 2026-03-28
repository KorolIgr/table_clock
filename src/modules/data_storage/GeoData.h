#ifndef GEO_DATA_H
#define GEO_DATA_H

#include <Arduino.h>

// Geolocation data
struct GeoData {
    String continent = "";
    String country = "";
    String city = "";
    float latitude = 0.0f;
    float longitude = 0.0f;
    unsigned long last_update = 0;  // timestamp of last update
    String ip_address = "";         // IP address from geolocation service
};

#endif // GEO_DATA_H
