# Arduino Reflex
Small reflex game for the Arduino Uno.

## Hardware
- Arduino UNO R3
- Piezo speaker
- LCD Display
- I2C Module connected with the LCD
- Button
- LED + Resistor

# Wiring
| Arduino | I2C | Button | Speaker | LED + Resistor |
|---------|-----|--------|---------|----------------|
| A0      |     |        | +       |                |
| A4      | SDA |        |         |                |
| A5      | SCL |        |         |                |
| 2       |     | Any A  |         |                |
| 3       |     |        |         | +              |
| 5V      |     |        |         |                |
| GND     |     | Any B  | -       | -              |
