#ifndef DS3232_AVR_H
#define DS3232_AVR_H

#ifndef TWOWIRE_NEW_H_
#include <Wire.h>
#endif

#ifndef NOP
#define NOP __asm__ __volatile__("nop\n\t")
#endif

#include <Arduino.h>

/*
 *      Routines to read and write to the DS3232 Real Time Clock over I2C
 *      
 *      Sets the bus address for the DS3232
 *      rtc_set_address(DS3232 I2C address)
 *      
 *      Reads the DS322 and stores the valuse in struct.. 
 *      rtc_read()
 *      
 *      Reads the temperture sensor and return in float variable
 *      rtc_temperature()
 *      
 *      Used to set the RTC 
 *      rtc_write(function, value)
 *      
 */

enum rtc_arg {
  SECOND,
  MINUTE,
  HOUR,
  d_o_w,
  DAY,
  MONTH,
  YEAR,
  SET_12,
  SET_24,
  SET_PM,
  SET_AM,
};

typedef struct {
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t day;
  uint8_t month;
  uint8_t year;
} internal_rtc;

/******* Public functions ***************/
void rtc_set_address(uint8_t address);
void rtc_read();
float rtc_temperature();
void rtc_write(uint8_t function, uint8_t value);

/* Return the values from the struct */
uint8_t rtc_seconds();
uint8_t rtc_minutes();
uint8_t rtc_hours();
uint8_t rtc_day();
uint8_t rtc_month();
uint8_t rtc_year();

/****** Private functions ***************/
void write_rtc(uint8_t function, uint8_t value);
uint8_t read_rtc(uint8_t function);


#endif
