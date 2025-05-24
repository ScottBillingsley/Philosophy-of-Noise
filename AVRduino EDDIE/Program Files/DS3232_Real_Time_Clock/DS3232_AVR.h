#ifndef DS3232_AVR_H
#define DS3232_AVR_H
#include <Arduino.h>

#ifndef TWOWIRE_NEW_H_
#include <Wire.h>
#endif

#ifndef NOP
#define NOP __asm__ __volatile__("nop\n\t")
#endif

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

class DS3232 {

    uint8_t _address;

  public:
    DS3232(uint8_t address);
    void update();
    /* Return the values from the struct */
    uint8_t seconds();
    uint8_t minutes();
    uint8_t hours();
    uint8_t day();
    uint8_t month();
    uint8_t year();
    float temperature();
    void write(uint8_t function, uint8_t value);

  private:
    void write_rtc(uint8_t function, uint8_t value);
    uint8_t read_rtc(uint8_t function);

};

#endif
