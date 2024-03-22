
#define I2C_SDA 5
#define I2C_SCL 4

#define ZERO_ERROR_V 0
#define ZERO_ERROR_C 0.20

// Voltage Divider - Voltage 
#define R1 97.4
#define R2 6.75

// Voltage Divider - Current
#define R3 22000
#define R4 22000

// Assuming a fixed battery capacity for SoC estimation (e.g., 100Wh for demonstration)
#define batteryCapacityWh  40.4 // Replace with actual battery capacity

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C

