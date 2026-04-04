#include <Arduino.h>
#include "MainApplication.h"

// Global application instance (defined here, declared extern in MainApplication.h)
MainApplication app;
MainApplication* appInstance = &app;

void setup() {
    app.begin();
}

void loop() {
    app.appLoop();
}
