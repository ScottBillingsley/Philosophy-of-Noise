#ifndef EEPROM_H_
#define EEPROM_H_
#include <Arduino.h>

#ifndef TWOWIRE_NEW_H_
#include <Wire.h>
#endif

class EEprom {

public:
  EEprom(uint8_t address);
  uint8_t read(uint16_t memory_address);
  uint16_t read_int(uint16_t memory_address);
  void write(uint16_t memory_address, uint8_t data);
  void write_int(uint16_t memory_address, uint16_t data);

private:
  uint8_t _address;
  
};  /* End  EEprom */ 

#endif  /* End eeprom.h */
