#include "eeprom.h"

EEprom::EEprom(uint8_t address)
{
  _address = address;
}

uint8_t EEprom::read(uint16_t memory_address)
{
  uint8_t _in_data;
  Wire.beginTransmission(_address);
  Wire.write(memory_address >> 8);   // MSB
  Wire.write(memory_address & 0xFF); // LSB
  Wire.endTransmission();
  Wire.requestFrom(_address, 1);
  if (Wire.available()) {
    _in_data = Wire.read();
  }
  return _in_data;
}

uint16_t EEprom::read_int(uint16_t memory_address)
{
  uint16_t _in_data;
  Wire.beginTransmission(_address);
  Wire.write(memory_address >> 8);   // MSB
  Wire.write(memory_address & 0xFF); // LSB
  Wire.endTransmission();
  Wire.requestFrom(_address, 2);
  if (Wire.available()) {
    _in_data = (Wire.read() << 8);   // MSB
    _in_data |= Wire.read();         // LSB
  }
    return _in_data;
}

void EEprom::write(uint16_t memory_address, uint8_t data)
{
  Wire.beginTransmission(_address);
  Wire.write(memory_address >> 8);   // MSB
  Wire.write(memory_address & 0xFF); // LSB
  Wire.write(data);
  Wire.endTransmission();
  delay(5); // writing can take up to 5m/s per address  
}

void EEprom::write_int(uint16_t memory_address, uint16_t data)
{
  Wire.beginTransmission(_address);
  Wire.write(memory_address >> 8);   // MSB
  Wire.write(memory_address & 0xFF); // LSB
  Wire.write(data >> 8);             // MSB
  Wire.write(data & 0xFF);           // LSB
  Wire.endTransmission();
  delay(5); // writing can take up to 5m/s per address    
}
