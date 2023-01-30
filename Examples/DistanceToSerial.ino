#include <Wire.h>
#include "RCWL_1601_i2c.h"

// Adafruit reports that the RCWL-1601 is not a good i2c citizen
// it may be that quirk I noticed that it holds onto the bus after a read until
// it sees a stop from the master

// In any case, this demo for the ESP32 feather uses Wire1 and an alternate gpio
// for the i2c bus, to keep it isolated from other i2c devices
#define SDA_PIN 33
#define SCL_PIN 27

#define I2C_ADDR 0x57

I2C_Sonar Sonar(I2C_ADDR, &Wire1);

void setup() {
  Serial.begin(115200);
  Serial.println("RCWL-1601 Sonar in i2c mode");

  Wire1.begin(SDA_PIN, SCL_PIN);
  uint8_t result = 0;
  for(;;) {
    result = Sonar.startScanning();
    if (result == 0)
      break;

    switch (result) {
      case 2:
      case 3:
        Serial.println("[Sonar] RCW-1601 responded with NACK");
        break;
      case 5:
        // didn't respond within timeout
        Serial.println("[Sonar] Unable to find the RCW-1601 on the i2c bus.");
        break;
      default:
        // includes 1 - data too long which shouldn't happen
        Serial.println("[Sonar] Unknown error: Can't communicate with RCW-1601");
        break;
    }
    delay(1000);
  }
}

void loop() {
  delay(100);
  long um = Sonar.readUm();
  if (um < 0) {
  	Serial.println("Failed to read.");
  	delay(500);
  	return;
  }
  float cm = um / 10000.0;
  Serial.print(cm);
  Serial.println("cm");
}