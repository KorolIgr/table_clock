LED Control Requirements

The system controls multiple RGB LEDs using configurable lighting patterns.

# General Rules
* The default pattern must be Running Light.
* Each lighting pattern must be implemented in a separate source file.
* Each pattern must support individual configuration parameters.
* If a parameter is not provided, the default value must be used.
* When loading the controller, the LEDs should work according to the selected pattern, if it is not selected, then run the default pattern

# Supported Patterns

## Running Light
Sequentially lights LEDs one by one.
### Configurable parameters:
* direction – LED movement direction
* values: forward, reverse
    * default: forward (from first LED to last LED)
* speed – switching interval in milliseconds
    * default: 500 ms
* color – LED color
    * default: white

## Ping Pong
A light moves from the first LED to the last and then back.
### Configurable parameters:
* speed – switching interval in milliseconds
    * default: 500 ms
* color – LED color
    * default: white