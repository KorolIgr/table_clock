# AGENTS.md

## Basic Rules

- Always respond **briefly and to the point**.
- Always respond **in English only**.
- Do **not create commits** in the repository unless explicitly requested.
- If I ask you to commit changes, after committing you must **push the changes to the repository**.
- Do **not run builds automatically**.
- Do **not upload firmware automatically**.
- Try to keep files **no longer than 200 lines whenever possible**.
- Use the **indexed code base** to navigate and understand the project.
- Development tasks are located in the `tasks/new` directory.
- After completing a task, move the task file to the `tasks/done` directory.
- Hardware configuration is located in the `hardware` directory.
- The list of hardware components is stored in `hardware/components.md`.
- The hardware wiring diagram is stored in `hardware/scheme.md`.
- If something **non-standard** needs to be implemented, ask me first.
- If you **do not know how to implement something**, ask me first.
- After making changes to the project, save a **short context of the current project state** in the `AGENTS.md` file.

---

## Project Name

**Table Clock**

---

## Purpose

Firmware for a **table clock based on the ESP8266 microcontroller**.

The device controls:

- **8 OLED displays SSD1306** (via I2C multiplexer **TCA9548A**)
- **8 addressable LEDs APA106**

The device supports:

- WiFi connectivity
- Web configuration interface
- Access Point (AP) mode for initial setup

---

## Coding Rules

- Development language: **C++ Arduino (PlatformIO)**
- Code must be compatible with **ESP8266 (ESP-12E)**
- Avoid excessive use of **STL**
- Avoid **dynamic memory allocation**
- Code must be **modular**
- HTML pages must be placed in **separate `.html` files**

---

## Web Interface Rules

The web interface runs on an ESP8266 and must be **simple, lightweight, and compact**.

### General

- Prefer **native HTML elements** instead of custom UI components.
- Do **not use frontend frameworks** (React, Vue, Angular, etc.).
- Avoid complex layouts and UI abstractions.
- Functionality is more important than visual design.

### HTML

Use standard elements whenever possible:

- `form`
- `button`
- `input`
- `select`
- `label`
- `table`
- `fieldset`

Avoid complex `div`-based component structures.

### Styling

- Prefer **default browser styles**.
- Avoid custom CSS.
- If CSS is needed, keep it **minimal and inline**.
- Do not use external CSS libraries.

### JavaScript

- JavaScript must be **minimal**.
- Prefer **form submission or simple fetch requests**.
- Do not use external JS libraries.

### Layout

- Prefer **simple vertical layout**.
- Avoid complex grids and positioning.

### Code Compactness

Since the firmware runs on **ESP8266**, keep web resources small:

- minimize HTML size
- minimize CSS and JS
- avoid deep DOM nesting
- avoid unnecessary whitespace
- reuse markup where possible

## Commands

```bash
# Build firmware
pio run

# Upload firmware to device
pio run --target upload

# Serial port monitor
pio device monitor

# Generate web_pages.h from HTML files (REQUIRED before build after HTML changes)
python build_html.py
# or
make build-web