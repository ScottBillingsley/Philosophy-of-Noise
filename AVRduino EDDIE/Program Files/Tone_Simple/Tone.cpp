#include "Tone.h"

Tone::Tone(uint8_t _pin)
{
  this->counter = counter;
  init();
  _PIN = _pin;
  PORTB.DIRSET |= _PIN;
}

void Tone::init()
{
  counter = 0;
}

void Tone::update()
{
  counter ++;
  if (counter >= compare_count) {
    PORTB.OUTTGL = _PIN;
    counter = 0;
  }
}

void Tone::pwm_update()
{
  counter ++;
  if (counter < ((compare_count * _duty) >> 8)) {
    PORTB.OUTSET = _PIN;
  } else {
    PORTB.OUTCLR = _PIN;
  }
  if (counter >= compare_count){
    counter = 0;
  }
}

void Tone::set_duty(uint8_t duty)
{
  _duty = duty;
}

void Tone::set_compare(uint16_t compare_count)
{
  this->compare_count = compare_count;
}

void Tone::pwm_set_compare(uint16_t compare_count)
{
  this->compare_count = compare_count;
}

void Tone::on()
{
  PORTB.DIRSET |= _PIN;
}

void Tone::off()
{
  PORTB.DIRCLR |= _PIN;
}

uint16_t Tone::calculate_count(float freq, uint16_t rate)
{
  return (((float)rate / freq)/ 2);
}

uint16_t Tone::pwm_calculate_count(float freq, uint16_t rate)
{
  return ((float)rate / freq);
}

bool Tone::check()
{
  if (counter >= compare_count) {
    return true;
  } else {
    return false;
  }
}
