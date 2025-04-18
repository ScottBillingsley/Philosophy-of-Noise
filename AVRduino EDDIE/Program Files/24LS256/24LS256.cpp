#include "24LS256.h"

/******* Public functions ***************/

void EEPROM_write(uint16_t eeaddress, uint8_t data, uint8_t eeprom_address) {
  Wire.beginTransmission(eeprom_address);
  Wire.write(eeaddress >> 8);    // MSB
  Wire.write(eeaddress & 0xFF); // LSB
  Wire.write(data);
  Wire.endTransmission();
  delay(5); // writing can take up to 5m/s per address
}

uint8_t EEPROM_read(uint16_t eeaddress, uint8_t eeprom_address) {
  byte rdata;
  Wire.beginTransmission(eeprom_address);
  Wire.write(eeaddress >> 8);    // MSB
  Wire.write(eeaddress & 0xFF); // LSB
  Wire.endTransmission();
  Wire.requestFrom(eeprom_address, 1);
  if (Wire.available()) {
    rdata = Wire.read();
  }
  return rdata;
}

void EEPROM_page_write(uint16_t eeaddress, uint8_t in_buffer[], uint8_t num_bytes, uint8_t eeprom_address) {
  Wire.beginTransmission(eeprom_address);
  Wire.write(eeaddress >> 8);    // MSB
  Wire.write(eeaddress & 0xFF); // LSB
  Wire.write(in_buffer[0]);
  for (uint8_t e = 1; e < num_bytes; e ++) {
    Wire.write(in_buffer[e]);
  }
  Wire.endTransmission();
  delay(5); // writing can take up to 5m/s per address
}

void EEPROM_page_read(uint16_t eeaddress, uint8_t in_buffer[], uint16_t num_bytes, uint8_t eeprom_address) {
  uint8_t byte_count;
  Wire.beginTransmission(eeprom_address);
  Wire.write(eeaddress >> 8);    // MSB
  Wire.write(eeaddress & 0xFF); // LSB
  Wire.endTransmission();
  while (byte_count < num_bytes) {
    Wire.requestFrom(eeprom_address, 1);
    if (Wire.available()) {
      in_buffer[byte_count] = Wire.read();
      byte_count ++;
    }
  }
}

void EEPROM_erase_chip(uint16_t start_address, uint16_t end_address, uint8_t eeprom_address){
  for(uint16_t address = start_address; address <= end_address; address ++){
     EEPROM_write(address, 0xFF, eeprom_address);
  }
}

/****** Private functions ***************/
