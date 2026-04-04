#ifndef BUILTIN_LED_H
#define BUILTIN_LED_H

#include <Arduino.h>
#include "../data_storage/DataStorage.h"

class BuiltInLED {
public:
    BuiltInLED();
    
    // Dependency injection for data storage
    void setDataStorage(DataStorage* dataStorage);
    
    void begin();
    void update();
    void setPattern(int pattern); // 0=off, 1=slow blink (5s), 2=fast blink (4Hz), 3=on
    
private:
    DataStorage* _dataStorage;
    unsigned long _lastToggle;
    bool _state;
    int _pattern;
};

#endif // BUILTIN_LED_H
