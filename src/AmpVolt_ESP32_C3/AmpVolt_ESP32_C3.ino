/*
  ADS1X15 Libaray :  https://github.com/RobTillaart/ADS1X15
*/


#include "ADS1X15.h"
#include "Arduino.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "config.h"
#include <Wire.h>
 #include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


ADS1115 ADS(0x48);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// Function Prototypes
float readVoltage();
float readCurrent();
void sendData(char type[2]);
void calculateAndSendAdditionalData();

// Global Variables for Energy Calculation
unsigned long previousMillis = 0; // Stores the last time the energy was updated
float energyConsumed = 0; // Total energy consumed over time in Watt-hours (Wh)
unsigned long startTime;


void setup() 
{
  Serial.begin(115200);
  startTime = millis(); // Save the start time
  Wire.setPins(I2C_SDA, I2C_SCL);
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);
  ADS.begin();
   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    //for(;;); // Don't proceed, loop forever
}

  ADS.setGain(0);
  ADS.setDataRate(7); // Fast Date Rate Mode 
  
  display.display();
  delay(2000); 
  display.clearDisplay();
}

void loop() 
{ 
    String command = readCommand(); // Read a complete command from serial
  if (command != "") {
    sendData(command);
  }

  // Periodic energy update (example: every 1 second)
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    float current = readCurrent(); // Read current in Amps
    float voltage = readVoltage(); // Read voltage in Volts
    float power = voltage * current; // Calculate power in Watts
    energyConsumed += power / 3600; // Convert power to Wh and accumulate
  }

  updateDisplay();
}


float readVoltage(){
  int16_t val_0 = ADS.readADC(1);  
  float f = ADS.toVoltage(1); 
  float voltage = fabs((val_0 *f) * ((R1+R2)/R2) + ZERO_ERROR_V);

  return voltage;
}

float readCurrent(){
    int16_t val_1 = ADS.readADC(0); 
    float f = ADS.toVoltage(1); 
    float current = (val_1 *f) * ((R3+R4)/R4) + ZERO_ERROR_C;

  return current;
}

void calculateAndSendAdditionalData() {
  float voltage = readVoltage();
  float current = readCurrent();
  float power = voltage * current; // Power in Watts


  // enter the batteryCapacityWh in config.h
  float soc = (energyConsumed / batteryCapacityWh) * 100; // State of Charge in percentage

  // Send all data in JSON format
  Serial.print("{");
  Serial.print("\"voltage\": "); Serial.print(voltage, 3);
  Serial.print(", \"current\": "); Serial.print(current, 3);
  Serial.print(", \"power\": "); Serial.print(power, 3);
  Serial.print(", \"energyConsumed\": "); Serial.print(energyConsumed, 3);
  Serial.print(", \"stateOfCharge\": "); Serial.print(soc, 3);
  Serial.println("}");
}


String readCommand() {
  static String command = ""; // Hold the incoming serial command
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == '\n') {
      String completedCommand = command; // Copy the completed command
      command = ""; // Reset command buffer
      return completedCommand; // Return the completed command
    } else {
      command += receivedChar; // Add received character to the command string
    }
  }
  return ""; // Return an empty string if the command is not yet complete
}

void sendData(String type) {
  float voltage = readVoltage();
  float current = readCurrent();
  float power = voltage * current; // Calculating power
  
  if (type == "#i") { // Send current value
      Serial.println(current);
  } else if (type == "#v") { // Send voltage value
      Serial.println(voltage);
  } else if (type == "#p") { // send power 
      Serial.println(power);
  } else if (type == "#a") { // Send all values in JSON format
      calculateAndSendAdditionalData();
  }
}


void printValues(){
  Serial.print("V:");
  Serial.print(readVoltage());
  Serial.print(",");
  Serial.print("A:");
  Serial.println(readCurrent());
}

String elapsedTimeAsString() {
  unsigned long currentMillis = millis();
  unsigned long elapsedTime = currentMillis - startTime;

  int hours = (int)(elapsedTime / 3600000); // 3,600,000 milliseconds in an hour
  int mins = (int)((elapsedTime / 60000) % 60); // 60,000 milliseconds in a minute, % 60 to get remainder
  int secs = (int)((elapsedTime / 1000) % 60); // 1,000 milliseconds in a second, % 60 to get remainder

  char timeStr[9]; // Buffer to hold the time string, HH:MM:SS format requires 8 chars + 1 for null terminator
  sprintf(timeStr, "%02d:%02d:%02d", hours, mins, secs);

  return String(timeStr); // Convert char array to String and return
}

void updateDisplay(){
  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(5, 10);     // Start at top-left corner
  display.print(readVoltage());
  display.write("V");
  display.setCursor(5, 30);   
  display.print(readCurrent());
  display.write("A");
  float power = readCurrent() * readVoltage();
  display.setCursor(5, 50);   
  display.print(power);
  display.write("W");

  display.setTextSize(1);
  display.setCursor(80, 10);
  display.print(elapsedTimeAsString());

  display.setCursor(80, 20);
  display.print(batteryCapacityWh);
  display.write("Wh");

  display.setCursor(80, 30);
  display.print(energyConsumed);
  display.write("Wh");

  // enter the batteryCapacityWh in config.h
  float remainingBattery = 100 - ((energyConsumed / batteryCapacityWh) * 100) ; // State of Charge in percentage

  display.setCursor(80, 40);
  display.print(remainingBattery);
  display.write("%");

  display.display();
}