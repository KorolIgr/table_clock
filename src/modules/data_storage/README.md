# Data Storage Module

## Overview
The DataStorage module provides a centralized shared data exchange buffer that allows different modules in the system to share common data safely. This implements a shared memory pattern that enables loose coupling between modules while maintaining access to common system state information.

## Features
- Thread-safe access to shared data (with lock/unlock mechanisms)
- Centralized storage for system-wide information
- Easy integration with dependency injection
- Support for various data types (bool, String, numeric)

## Data Structure
The `SharedData` structure contains common system information:
- WiFi status information (connection status, IP address, status message)
- LED status information (enabled state, brightness level)
- System status information (temperature, uptime, error states)

## Usage
The DataStorage module is instantiated in the main application and injected into other modules that need access to shared data:

```cpp
// In main application initialization
_dataStorage = new DataStorage();
_configManager->setDataStorage(_dataStorage);

// Accessing shared data
SharedData& data = _dataStorage->getData();
data.wifi_connected = true;
data.ip_address = "192.168.1.100";

// Thread-safe access
_dataStorage->lock();
// Modify data safely
_dataStorage->updateWifiStatus(true, "Connected", "192.168.1.100");
_dataStorage->unlock();
```

## Integration
The DataStorage module is integrated with the ConfigManager through dependency injection, allowing the configuration system to access shared data when needed. Other modules can similarly be extended to use the DataStorage for sharing information.

## Benefits
- Reduces tight coupling between modules
- Provides a single source of truth for shared data
- Enables safer concurrent access to shared information
- Simplifies data synchronization between modules