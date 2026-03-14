* The device must support hybrid WiFi mode (AP + STA).
* AP mode configuration must be stored in non-volatile memory to allow changing network parameters.
* If AP configuration is not stored in memory, the device must use default AP credentials defined in firmware constants.
* STA mode configuration (SSID and password) must be stored in non-volatile memory to allow updates via the web interface.
* If valid STA configuration exists, the device must automatically connect to the configured WiFi network during startup.
* The device must automatically reconnect to the STA network if the connection is lost.
* AP mode must remain available to allow device configuration and management through the web interface.
* If the STA does not have connection data, then there is no need to try to connect or reconnect.