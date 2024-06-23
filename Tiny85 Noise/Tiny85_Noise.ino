/*
*                  Tiny85 Noise
*              Vernon Billingsley  2024
*
*           A triple random noise souce for DIY Synth
*
*           Arduino IDE 2.3.0
*           ATtiny Microcontrollers 1.0.2
*           Internal 16 mHz clock
*
*     Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission
*    notice shall be included in all copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*    THE SOFTWARE.
*
*                        
*                     ----U---
*             Reset -| 1     8|- Vcc
*               PB3 -| 2     7|- PB2   SCL SCK
*               PB4 -| 3     6|- PB1       MISO  OC0B
*               GND -| 4     5|- PB0   SDA MOSI  OC0A
*                     --------
*
*         Pin       Function
*         1
*         2
*         3         Low Speed Noise Source 3
*         4         GND
*         5         High Speed Noise Source 2  
*         6         Medium Speed Noise Source 1
*         7
*         8         Vcc +5 vdc 
*
* Noise 1:  Pin 6 ---- 470R -- Out   Noise 2:  Pin 5 ---- 470R ---- Out
*                       |                                  |
*                      4n7                                10n
*                       |                                  |
*                      GND                                GND
*
* Noise 3:  Pin 3 ---- 470R -- Out
*                       |
*                      33n
*                       |
*                      GND
*
*/

/************************ Includes ********************************/
#include "globals.h"

/* Uncomment the noise source you want to use... */
/* You may use all three but the SLOW speed will be slower... */

//#define HIGH
//#define MEDIUM
#define SLOW


/************************** Variables *****************************/
volatile uint32_t lfsr_1 = 0xBAD00ACEu;
volatile uint32_t lfsr_2 = 0x98A8A38Du;
volatile uint32_t lfsr_3 = 0x7FAAB6E9u;

uint8_t count = 0;
uint8_t top = 64;

/**************************  Functions ****************************/
ISR(TIMER0_COMPA_vect) {
  uint8_t lsb = lfsr_1 & 0x01; /* Get LSB (i.e., the output bit). */
  lfsr_1 >>= 1;                /* Shift register */
  /* XOR the bits if lsb = 1 */
  if (lsb) {
    lfsr_1 ^= 0xB4000000;
  }
  OCR0A = lfsr_1 & 0x000000FFu;
}
ISR(TIMER1_COMPA_vect) {
  uint8_t lsb = lfsr_2 & 0x01; /* Get LSB (i.e., the output bit). */
  lfsr_2 >>= 1;                /* Shift register */
  /* XOR the bits if lsb = 1 */
  if (lsb) {
    lfsr_2 ^= 0xB4000000;
  }
  OCR1A = lfsr_2 & 0x000000FFu;
}

/******************************************************************/
/***************************  MAIN ********************************/
/******************************************************************/
int main() {


  /************************* Setup Pins ***************************/
  /* Set PORT B0 , chip pin 5 as OUTPUT for OC0A */
  DDRB |= _BV(0);
  /* Set PORT B1 , chip pin 6 as OUTPUT for OC1A */
  DDRB |= _BV(1);
  /* Set PORT B4 , chip pin 4 as OUTPUT for slow PWM wave */
  DDRB |= _BV(4);

#ifdef MEDIUM
  /* Set pin to clear on Timer 0 match */
  sbi(TCCR0A, COM0A1);
  cbi(TCCR0A, COM0A0);
  /* Set Fast PWM */
  cbi(TCCR0B, WGM02);
  sbi(TCCR0A, WGM01);
  sbi(TCCR0A, WGM00);
  /* Set prescale to 1 */
  cbi(TCCR0B, CS02);
  cbi(TCCR0B, CS01);
  sbi(TCCR0B, CS00);
  /* Set timer compare interrupt */
  sbi(TIMSK, OCIE0A);
  OCR0A = 64;
#endif

#ifdef HIGH
  /* Timer 1 PWM , pin 6 */
  /* Enable PWM Mode */
  sbi(TCCR1, PWM1A);
  /* Enable the OC1A pin, Clear on match */
  sbi(TCCR1, COM1A1);
  cbi(TCCR1, COM1A0);
  /* Prescale to 500 khz */
  cbi(TCCR1, CS13);
  cbi(TCCR1, CS12);
  cbi(TCCR1, CS11);
  sbi(TCCR1, CS10);
  /* Enable the interrupt vector */
  sbi(TIMSK, OCIE1A);
  /* Preload counter */
  OCR1A = 64;
#endif

  /* Turn on interrupts */
  sei();


  /******************************************************************/
  /**************************** WHILE *******************************/
  /******************************************************************/
  while (1) {

    #ifdef SLOW
    count++;
    if (count >= top) {
      static bool flip = false;
      flip = !flip;

      uint8_t lsb = lfsr_3 & 0x01; /* Get LSB (i.e., the output bit). */
      lfsr_3 >>= 1;                /* Shift register */
      /* XOR the bits if lsb = 1 */
      if (lsb) {
        lfsr_3 ^= 0xB4000000;
      }

      top = lfsr_3 & 0x000000FFu;

      if (flip == true) {
        PORTB |= _BV(4);  // digitalWrite (4, HIGH);
      } else {
        PORTB &= ~_BV(4);  // digitalWrite (4, LOW);
      }
    }
    #endif

  } /*************************** End While Loop *****************************/
} /****************************End Main Loop ******************************/
