#include "DS3232_AVR.h"

int16_t ds3232 = 0x68;

/* Setup a struct to hold the data */
static internal_rtc _rtc;

/********************* Public functions ***************************/
void rtc_set_address(uint8_t address) {
  ds3232 = address;
}

uint8_t rtc_seconds(){
  return _rtc.seconds;
}
uint8_t rtc_minutes(){
  return _rtc.minutes;
}
uint8_t rtc_hours(){
  return _rtc.hours;
}
uint8_t rtc_day(){
  return _rtc.day;
}
uint8_t rtc_month(){
  return _rtc.month;
}
uint8_t rtc_year(){
  return _rtc.year;
}

void rtc_read() {
  /* Read and store the seconds */
  uint8_t s = read_rtc(0x00);
  _rtc.seconds = (((s >> 4) & 0x07) * 10) + (s & 0x0F);
  /* Read and store the minutes */
  uint8_t m = read_rtc(0x01);
  _rtc.minutes = (((m >> 4) & 0x07) * 10) + (m & 0x0F);
  /* Read and store the hours */
  uint8_t h = read_rtc(0x02);
  if (((h >> 6) & 0x01) == 1) {
    /* 12 Hour */
    _rtc.hours = (((h >> 4) & 0x01) * 10) +  (h & 0x0F);
  } else {
    /* 24 Hour */
    _rtc.hours = (((h >> 5) & 0x01) * 20) + (((h >> 4) & 0x01) * 10) + (h & 0x0F);
  }
  /* Read and store the date */
  uint8_t d = read_rtc(0x04);
  _rtc.day = (((d >> 4) & 0x03) * 10) + (d & 0x0F);
  /* Read and store the month */
  uint8_t x = read_rtc(0x05);
  _rtc.month = (((x >> 4) & 0x01) * 10) + (x & 0x0F);
  /* Read and store the year */
  uint8_t y = read_rtc(0x06);
  _rtc.year = ((y >> 4) * 10) + (y & 0x0F);
}

float rtc_temperature() {
  int8_t number;
  int8_t fraction;
  number = read_rtc(0x11);
  delay(10);
  fraction = read_rtc(0x12);
  fraction = (fraction >> 6);
  return (float)number + ((float) fraction * .25);
}


void rtc_write(uint8_t function, uint8_t value) {
  uint8_t temp = 0;
  uint8_t out_value = 0;
  uint8_t t = 0;
  switch (function) {
    case SECOND:
      temp = (value / 10);
      out_value = (temp << 4) | (value - (temp * 10));
      write_rtc(SECOND, out_value);
      break;
    case MINUTE:
      temp = (value / 10);
      out_value = (temp << 4) | (value - (temp * 10));
      write_rtc(MINUTE, out_value);
      break;
    case HOUR:
      temp = read_rtc(HOUR);
      if (((temp >> 6) & 0x01) == 1) {
        /* 12 Hour */
        out_value = (temp & 0xF0) | (value & 0x0F);
      } else {
        /* 24 Hour */
        out_value = (temp & 0xC0) | ((value / 20) << 5) | ((value / 10) << 4);
        temp = (value & 0x0F);
        if (temp >= 20) {
          temp = temp - 20;
        }
        if (temp >= 10) {
          temp = temp - 10;
        }
        out_value |= temp;
      }
      write_rtc(HOUR, out_value);
      break;
    case d_o_w:
      temp = value & 0x07;
      write_rtc(d_o_w, temp);
      break;
    case DAY:
      temp = (value / 10) & 0x03;
      out_value = (temp << 4) | (value - (temp * 10));
      write_rtc(DAY, out_value);
      break;
    case MONTH:
      temp = read_rtc(MONTH);
      out_value = (temp & 0x80);
      t = (value / 10);
      out_value |= (t << 4) | (value - (t * 10));
      write_rtc(MONTH, out_value);
      break;
    case YEAR:
      temp = (value / 10);
      out_value = (temp << 4) | (value - (temp * 10));
      write_rtc(YEAR, out_value);
      break;
    case SET_12:
      temp = read_rtc(HOUR);
      out_value = 0x40 | (temp & 0x3F);
      write_rtc(HOUR, out_value);
      break;
    case SET_24:
      temp = read_rtc(HOUR);
      out_value = 0x00 | (temp & 0x3F);
      write_rtc(HOUR, out_value);
      break;
    case SET_PM:
      temp = read_rtc(HOUR);
      if (((temp >> 6) & 0x01) == 1) {
        bitSet(temp, 5);
        write_rtc(HOUR, temp);
      }
      break;
    case SET_AM:
      temp = read_rtc(HOUR);
      if (((temp >> 6) & 0x01) == 1) {
        bitClear(temp, 5);
        write_rtc(HOUR, temp);
      }
      break;
  }
}


/********************** Private functions ************************/
void write_rtc(uint8_t function, uint8_t value) {
  Wire.beginTransmission(ds3232);
  Wire.write(function);
  Wire.write(value);
  Wire.endTransmission();
  delay(5);
}
uint8_t read_rtc(uint8_t function) {
  Wire.beginTransmission(ds3232);
  Wire.write(function);
  Wire.endTransmission();
  /* 10 uS delay at 40 mHz*/
  for (uint16_t d = 0; d < 400; d ++) {
    NOP;
  }
  Wire.requestFrom(ds3232, 0x01);
  return Wire.read();
}
