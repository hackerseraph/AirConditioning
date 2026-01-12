AirConditioning Floor Vent Booster Control
===========================================

A thermostat system for controlling a floor vent booster fan servo. The system monitors room temperature and allows users to set desired temperatures while controlling the vent servo with on/off buttons.

Features
--------
- **Real-time Temperature Monitoring**: Displays current room temperature from DHT11 sensor
- **Temperature Control**: Set desired temperature with up/down buttons (60-85°F range)
- **Servo Control**: On/Off buttons to control the floor vent booster fan servo
- **Fun Startup Sequence**: Displays 100 random humorous messages during boot
- **16x2 LCD Display**: Shows servo status, set temperature, and actual temperature
- **Data Logging Ready**: Framework for logging temperature/humidity data over time

Hardware
--------
- Arduino microcontroller
- DHT11 Temperature/Humidity Sensor (Pin 2)
- 16x2 LCD Display (pins 8, 9, 4, 5, 6, 7)
- 4 Push Buttons:
  - Servo On: GPIO Pin 10
  - Servo Off: GPIO Pin 11
  - Temperature Up: GPIO Pin 12
  - Temperature Down: GPIO Pin 13
- 1 Servo Motor (for floor vent control)

Future Enhancements
-------------------
- SD card data logging for temperature/humidity history
- Automatic servo control based on temperature difference
- Web/mobile dashboard for remote monitoring
- Multiple vent/fan support

