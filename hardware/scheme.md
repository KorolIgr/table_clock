ESP8266 (NodeMCU/Wemos D1 Mini)
├── VCC (3.3V) → TCA9548A VCC
├── GND → Все компоненты GND
├── SDA (D2, GPIO4) → TCA9548A SDA
├── SCL (D1, GPIO5) → TCA9548A SCL
├── DIN (D8, GPIO15) → APA106 LED Strip Data Input
└── 5V Power → APA106 VCC (отдельный источник)

TCA9548A (I2C Multiplexer)
├── VCC → 3.3V
├── GND → Ground
├── SDA → ESP8266 D2 (GPIO4)
├── SCL → ESP8266 D1 (GPIO5)
├── Address → GND (I2C address: 0x70)
├── Channel 0 → SSD1306 Display 1 (0x3C) SDA/SCL
├── Channel 1 → SSD1306 Display 2 (0x3C) SDA/SCL
├── Channel 2 → SSD1306 Display 3 (0x3C) SDA/SCL
├── Channel 3 → SSD1306 Display 4 (0x3C) SDA/SCL
├── Channel 4 → SSD1306 Display 5 (0x3C) SDA/SCL
├── Channel 5 → SSD1306 Display 6 (0x3C) SDA/SCL
├── Channel 6 → SSD1306 Display 7 (0x3C) SDA/SCL
└── Channel 7 → SSD1306 Display 8 (0x3C) SDA/SCL

APA106 LED Strip Connection
├── VCC → 5V Power Supply
├── GND → Ground
└── DIN → ESP8266 D8 (GPIO15)
    ├── LED 1 → LED 2 → LED 3 → LED 4 → LED 5 → LED 6 → LED 7 → LED 8
    └── DOUT of each LED connects to DIN of the next LED in sequence