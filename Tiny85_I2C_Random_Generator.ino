/*
               AtTiny 85 Development
              Vernon Billingsley c2023

            A random number genrator from 0 to 255..
            Get value by request over I2C..
            atTiny85 running 8 mHz...

                                                        Nano
                                                        I2C   Program
                     ----U---
  D10         Reset -| 1     8|- Vcc
               PB3 -| 2     7|- PB2   SCL SCK           ADC5  D13
               PB4 -| 3     6|- PB1       MISO  OC1A          D12
               GND -| 4     5|- PB0   SDA MOSI          ADC4  D11
                     --------

  https://thewanderingengineer.com/2014/02/17/attiny-i2c-slave/
  https://github.com/ScottBillingsley/Philosophy-of-Noise/tree/main/Tiny%20Noise
  
*/

/* https://github.com/svoisen/TinyWire */
#include <TinyWireS.h>


/************************* Defines ********************************/
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

/* Device I2C address */
#define I2C_ADDRESS 0x42

/************************** Variables *****************************/
uint32_t start_state = 0xBAD00ACEu;
uint32_t lfsr = start_state;

/* Hold the random number */
volatile uint8_t random_number;

/**************************  Functions ****************************/
void requestEvent() {
  /* Send a random number upon request */
  TinyWireS.write(random_number);
}

void generate_random_number() {
  uint8_t lsb = lfsr & 0x01;   /* Get LSB (i.e., the output bit). */
  lfsr >>= 1;                  /* Shift register */
  /* XOR the bits if lsb = 1 */
  if (lsb) {
    lfsr ^= 0xB4000000;
  }
  /* Get the LOW byte */
  random_number = lfsr & 0x000000FFu;
}

/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  /* Start tiny wire */
  TinyWireS.begin(I2C_ADDRESS);
  /* Attach interrupt */
  TinyWireS.onRequest(requestEvent);

  /************************* Setup Pins ***************************/


}/**************************  End Setup **************************/



/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
  /* Pause to check the I2C */
  TinyWireS_stop_check();
  /* Get a new random number */
  generate_random_number();

}/*************************** End Loop *****************************/
