#pragma once

namespace timer2 {

void begin(){
  /*Select internal asynchronous clock */
  sbi(ASSR, EXCLK);
  sbi(ASSR, AS2);
  /* Fast PWM mode */
  sbi(TCCR2A, WGM21);
  sbi(TCCR2A, WGM20);
  cbi(TCCR2B, WGM22);
  /* Non-inverting PWM on pin 11 */
  sbi(TCCR2A, COM2A1);
  cbi(TCCR2A, COM2A0);
  sbi(TCCR2A, COM2B1);
  cbi(TCCR2A, COM2B0);
  /* No prescaler */
  cbi(TCCR2B, CS22);
  cbi(TCCR2B, CS21);
  sbi(TCCR2B, CS20); 
}


}/* End namespace */