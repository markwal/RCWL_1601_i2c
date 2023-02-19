# RCWL_1601_i2c

An Arduino library for communicating with a RCWL-1601 distance sensor 
in i2c mode.  At the time of this writing, most searches on Google turn up
guides for using the RCWL-1601 in UART mode which makes it compatible with
other popular ultrasonic sensors.

In UART mode, you signal the "trigger" line which causes the ultrasonic send 
and then measure the time until the echo line signals that the bounce has 
been detected.

In I2C mode, the onboard chip does a continuous signal and sense, and an
i2c master asks it for the most recent measurements which is returned as 3 bytes
encoding the micrometers in big-endian order.

## I2C Citizenship Note

This board isn't a good i2c bus citizen as reported by Adafruit. You _can_ make
make it work, but basically, you have to call startScanning(), wait long enough
for the sonar ping to make it back (500ms maybe), readUm(), stopScanning() and
wait long enough for the RCWL-1601 to regather its wits and then you can use
the bus for something else.

It's often easier and works better for many applications to put the RCWL-1601 on

## Installing

Use the Arduino IDE Library manager to add the source archive zip file 
[main.zip](https://github.com/markwal/RCWL_1601_i2c/archive/refs/heads/main.zip)

## Usage

```
#include "RCWL_1601_i2c.h"

I2C_Sonar Sonar(0x57, &Wire);

void setup() {
  Serial.begin(115200);
  Serial.println("RCWL-1601 Sonar in i2c mode");

  Wire.begin();
  Sonar.startScanning();
}

void loop() {
  delay(100);
  long um = Sonar.readUm();
  float cm = um / 10000.0;
  Serial.print(cm);
  Serial.println("cm");
}
```

See also Examples/DistanceToSerial.ino
