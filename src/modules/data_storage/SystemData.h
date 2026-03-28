#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include <Arduino.h>

// System status data
struct SystemData {
    float temperature = 0.0f;
    uint32_t uptime = 0;  // in seconds
    bool system_error = false;
    String last_error = "";
};

#endif // SYSTEM_DATA_H
