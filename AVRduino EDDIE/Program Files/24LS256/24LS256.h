#ifndef A24LS256_H
#define A24LS256_H

#include <Wire.h>

#include <Arduino.h>

/*
 *    Routines to read and write to the 24LS256 or 24LS512 EEPorm..
 *    
 *    Write a single byte to the eeprom..
 *    EEPROM_write(store address, data to store, eeprom address)
 *    
 *    Read a single byte from the eeprom..
 *    data = EEPROM_read(stored address, eeprom address)
 *    
 *    Write upto 64 bytes at a time..
 *    EEPROM_page_write(start address, input buffer, number of bytes to write, eeprom address)
 *    
 *    Read mulitple bytes and store them in buffer 
 *    EEPROM_page_read(start address, input buffer, number of bytes to read, eeprom address)
 *    
 *    Erase the eeprom by filling each cell with 0xFF
 *    EEPROM_erase_chip(start address, end address, eeprom address)
 *    
 */

/******* Public functions ***************/
void EEPROM_write(uint16_t eeaddress, uint8_t data, uint8_t eeprom_address);
uint8_t EEPROM_read(uint16_t eeaddress, uint8_t eeprom_address);
void EEPROM_page_write(uint16_t eeaddress, uint8_t in_buffer[], uint8_t num_bytes, uint8_t eeprom_address);
void EEPROM_page_read(uint16_t eeaddress, uint8_t in_buffer[], uint16_t num_bytes, uint8_t eeprom_address);
void EEPROM_erase_chip(uint16_t start_address, uint16_t end_address, uint8_t eeprom_address);

/****** Private functions ***************/

#endif
