#pragma once

/* Programed sample rate */
#define SAMPLE_RATE 16384.0

namespace timer1 {

void begin() {
  /* Setup Timer1 for sample clock */
  //set timer1
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register
  OCR1A = (F_CPU / SAMPLE_RATE) - 1;
  // turn on CTC mode
  sbi(TCCR1B, WGM12);
  /*Set prescaler to 1 */
  cbi(TCCR1B, CS12);
  cbi(TCCR1B, CS11);
  sbi(TCCR1B, CS10);
  // enable timer compare interrupt
  sbi(TIMSK1, OCIE1A);
}

} /* end namespace */