///
/// @file RCWL_1601_i2c
///
/// I2C Driver for the RCWL-1601 ultrasonic distance sensor
///
/// NOTE: This is for the board in i2c mode (jumper on the back) instead
/// of in UART mode.
///
/// BSD license (see license.txt)
///

#ifndef RCWL_1601_I2C_H
#define RCWL_1601_I2C_H

#include <Arduino.h>
#include <Wire.h>

///
/// @brief Class for communicating with the RCWL-1601
///
class I2C_Sonar
{
  public:
	I2C_Sonar(uint16_t i2c_addr = 0x57, TwoWire *twi = &Wire);
	~I2C_Sonar();

	uint8_t startScanning();
	long readUm(unsigned long timeout = 100);
	void stopScanning();

  private:
    TwoWire *_wire = NULL;
	uint16_t _i2c_addr = 0x57;
};

#endif
