/*
                AtTiny 84 Development
                Vernon Billingsley c2022


    A pseudorandom noise generator on the AtTiny 84 using the
  Galois LFSR formula from https://en.wikipedia.org/wiki/Linear-feedback_shift_register#Galois_LFSRs.
  Both OCR1A, pin PA6 and OCRaB, pin PA5 are used in the Fast PWM mode..
  Configure the AtTiny for 8 mhz internal clock..

  Update..This version uses a 32 bit shift register instead of 
  the original 16 bit...

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission
    notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

                      ----U---
                VCC -| 1    14|- GND
                PB0 -| 2    13|- PA0
                PB1 -| 3    12|- PA1
                PB3 -| 4    11|- PA2
                PB2 -| 5    10|- PA3
                PA7 -| 6     9|- PA4
                PA6 -| 7     8|- PA5
                      --------

                      Output circuit
                _____
               |     |-- PA5 ---- 500K ---|
               |     |                    |--- OUT
               |     |-- PA6 ---- 3k9  ---|
               |     |                    |
                -----                    4n7
                                          |
                                         GND

  Based on
  http://wiki.openmusiclabs.com/wiki/PWMDAC
  https://elfquake.wordpress.com/2018/08/08/arduino-white-noise-generator/


*/

/************************* Defines ********************************/

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define PWM_FREQ 0x00FF
#define PWM_MODE 1 // Fast (1) or Phase Correct (0)
#define PWM_QTY 2 // number of pwms, either 1 or 2

/************************** Variables *****************************/

uint32_t start_state = 0xBAD00ACEu;
uint32_t lfsr = start_state;

/**************************  Functions ****************************/


/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {


  /************************* Setup Pins ***************************/
  /* Set pins PA5 and PA6 to output */
  DDRA |= _BV (5);
  DDRA |= _BV (6);

  /*************************  Setup Timer1 ************************/
  cli();                //stop interrupts

  // Set PWM mode dual output
  sbi(TCCR1A, COM1A1);
  sbi(TCCR1A, COM1A0);
  sbi(TCCR1A, COM1B1);
  sbi(TCCR1A, COM1B0);

  // turn on PWM mode
  sbi(TCCR1B, WGM13);
  cbi(TCCR1B, WGM12);
  cbi(TCCR1A, WGM11);
  cbi(TCCR1A, WGM10);

  /*Set prescaler to 1 */
  cbi(TCCR1B, CS12);
  cbi(TCCR1B, CS11);
  sbi(TCCR1B, CS10);
  // enable timer compare interrupt
  sbi(TIMSK1, ICIE1);

  // Set counter freq to 125 kHz
  ICR1H = (PWM_FREQ >> 8);
  ICR1L = (PWM_FREQ & 0xff);

  sei();

}/**************************  End Setup **************************/

ISR(TIMER1_CAPT_vect) {

  uint8_t lsb = lfsr & 0x01;   /* Get LSB (i.e., the output bit). */
  lfsr >>= 1;                  /* Shift register */
  /* XOR the bits if lsb = 1 */
  if (lsb) {
    lfsr ^= 0xB4000000;
  }

  uint16_t temp1 = (lfsr & 0x0000FF00u) >> 8; /* Get the HIGH byte */
  uint16_t temp2 = lfsr & 0x000000FFu; /* Get the LOW byte */

  OCR1A = temp1;    /*Load the counters */
  OCR1B = temp2;

}

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {

  while (1); // gets rid of jitter

}/*************************** End Loop *****************************/
