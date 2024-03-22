# AmpVolt: Precision Measurement Module - 240141 📊

## Overview 🌐
AmpVolt, designed for the any Arduino or ESP32 project, is a  module for accurately measuring current and voltage with ease. Featuring a voltage divider, INA169 for current sensing, and an ADS1015 ADC, it's equipped with an OLED display for immediate feedback. Suitable for electrical projects requiring up to 50V and 5A measurements, it offers modular connectivity through Grove, Qwiic, and 2.54mm pitch connectors.

## Key Features 🚀
- Precise current and voltage measurement up to 50V and 5A.
- Modular design with Grove, Qwiic, and standard connectors.
- Real-time data display on an OLED screen.
- Tested on ESP32-C3 Mini.

## Hardware Design 🛠️
AmpVolt's PCB is designed for flexibility, featuring various connectors to suit diverse project needs. The inclusion of Grove and Qwiic connectors alongside traditional 2.54mm pitch options ensures compatibility with a wide range of devices and systems.

## Function Descriptions 📖

### `setup()`
Initializes the system: sets up serial communication, I2C parameters, ADC gain, and data rate for quick measurements, and checks the OLED display's status.

### `loop()`
Main program loop: periodically reads commands, updates measurements, calculates energy consumed, and refreshes the OLED display with current data.

### `readVoltage()`
Reads and calculates the voltage from ADS1115 based on the voltage divider's configuration. It applies corrections for zero error and scales the reading according to the resistive divider ratio.

### `readCurrent()`
Measures the current using the INA169 and ADS1115, adjusting the value with a calibration factor to account for zero error and component variations.

### `calculateAndSendAdditionalData()`
Compiles voltage, current, and calculated power into a comprehensive dataset, including energy consumed and battery state of charge (SoC), then outputs this information through the serial port in JSON format.

### `readCommand()`
Listens for incoming serial commands, assembling them character-by-character until a newline character is encountered, indicating a complete command ready for processing.

### `sendData(String type)`
Responds to specific command types by reading sensor values, performing calculations, and sending the requested data back through the serial port.

### `updateDisplay()`
Refreshes the OLED display with the latest measurement data, including voltage, current, power, elapsed time, battery capacity, energy consumed, and remaining battery percentage.

## Installation & Usage 💡
Follow detailed steps to clone the repo, prepare your microcontroller, assemble the module, and start measuring with the provided Arduino sketch.

## Acknowledgements 🙏
Credits to the creators of the ADS1X15 and Adafruit libraries, enabling the AmpVolt project to utilize advanced ADC and display functionalities.
