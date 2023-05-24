#include "RCWL_1601_i2c.h"

///
/// @brief Instantiate the I2C_Sonar object
/// @param i2c_addr 
///        I2C bus address of the RCWL_1601, defaults to 0x57
/// @param wire 
///        A pointer to the TwoWire object from the Wire arduino library
///        that will handle the i2c bus and protocol
///
I2C_Sonar::I2C_Sonar(uint16_t i2c_addr, TwoWire *wire) :
	_i2c_addr(i2c_addr), _wire(wire)
{
}

I2C_Sonar::~I2C_Sonar()
{
}

///
/// @brief Tell the RCWL-9600 ranging chip to start transmitting ultrasonic
/// @return Wire library status values (TwoWire.endTransmission return value)
///         0: success.
///         1: data too long to fit in transmit buffer.
///         2: received NACK on transmit of address.
///         3: received NACK on transmit of data.
///         4: other error.
///         5: timeout
///
uint8_t I2C_Sonar::startScanning()
{
  _wire->beginTransmission(_i2c_addr);
  _wire->write((byte)1);

  return _wire->endTransmission();
}

///
/// @brief Ask the RCWL-9600 for the most recent distance measurement
/// @param timeout 
///        time in msec to wait for a response
/// @return The distance in micrometers
///         -1 on failure
///
long I2C_Sonar::readUm(unsigned long timeout)
{
  byte bytes[3];
  long um = -1;

  _wire->requestFrom(_i2c_addr, sizeof(bytes));
  unsigned long start = millis();
  while(!_wire->available() && millis() - start < timeout) {
	delay(10);
  }
  if (_wire->available()) {
	size_t byte_count = _wire->readBytes(bytes, sizeof(bytes));
	if (byte_count != 3)
		return -1;

	// the micrometers are transmitted in big-endian order
	um = bytes[0] * 0x10000 + bytes[1] * 0x100 + bytes[2];
  }

  // REVIEW I believe this is incorrect.  endTransmission is for ending a master's
  // write sequence after a matching beginTransmission, but it appears that the 
  // RCWL-1601 expects a stop from the master or the next read will fail.
  _wire->beginTransmission(_i2c_addr);
  _wire->write((byte)1);
  _wire->endTransmission();

  return um;
}

///
/// @brief Tell the RCWL-9600 ranging chip to stop
///
void I2C_Sonar::stopScanning()
{
  _wire->beginTransmission(_i2c_addr);
  _wire->write((byte)0);
  _wire->endTransmission();
}