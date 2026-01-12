AirConditioning Floor Vent Booster Control - Low Power ESP32
============================================================

A low-power thermostat system running on ESP32 for controlling floor vent flaps. The system monitors room temperature and allows users to open/close vent flaps while controlling a relay for ventilation. Optimized for battery operation with a 10000mAh battery.

Features
--------
- **Real-time Temperature Monitoring**: Displays current room temperature from DHT11 sensor
- **Temperature Control**: Set desired temperature with up/down buttons (60-85°F range)
- **Servo-Controlled Flaps**: Opens/closes vent flaps to help regulate room temperature
- **Relay Control**: Relay activation synchronized with flap position for ventilation control
- **Low Power Optimization**: Reduced display refresh rate and optimized for extended battery life
- **10000mAh Battery Support**: Designed for long-term operation on battery power
- **Fun Startup Sequence**: Displays 100 random humorous messages during boot
- **16x2 LCD Display**: Shows flap status, set temperature, and actual temperature

Hardware
--------
- **Microcontroller**: ESP32 (ultra-low power WiFi/BLE capable microcontroller)
- **Power**: 10000mAh Li-ion battery with voltage regulator
- **Sensors**:
  - DHT11 Temperature/Humidity Sensor (GPIO 32)
- **Display**: 16x2 LCD Display (pins 8, 9, 4, 5, 6, 7)
- **Actuators**:
  - Servo Motor (GPIO 33) - Controls vent flaps
  - Relay Module (GPIO 25) - Ventilation control
- **Controls** - 4 Push Buttons:
  - Open Flaps/Cooling: GPIO 26
  - Close Flaps/Off: GPIO 27
  - Temperature Up: GPIO 14
  - Temperature Down: GPIO 12

Low Power Features
------------------
- Display refresh rate: 1 second interval (reduced from 100ms)
- No data logging (removed to save power)
- Minimal delay between loops (50ms)
- All peripherals sleep when not in use
- ESP32 can enter deep sleep modes for extended battery life

GPIO Pin Summary (ESP32)
------------------------
- GPIO 32: DHT11 Temperature Sensor
- GPIO 33: Servo Motor PWM
- GPIO 25: Relay Control
- GPIO 26: Open Flaps Button
- GPIO 27: Close Flaps Button
- GPIO 14: Temperature Up Button
- GPIO 12: Temperature Down Button

Future Enhancements
-------------------
- RTC module for real-world timestamps (optional logging)
- Deep sleep modes during idle periods
- WiFi connectivity for remote monitoring
- Automatic flap control based on temperature difference
- Battery level monitoring on display

