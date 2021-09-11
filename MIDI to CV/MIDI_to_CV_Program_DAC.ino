/*
                MIDI to CV Programer for the DAC
                Vernon Billingsley (c)2021

                A program to program the DAC values for the MIDI
             to CV program..By turning the encoder the DAC will
             increase in voltage for each step..When the desired
             voltage is reached, pressing the button will print
             the value to the Serial out...When all voltages have been
             set, copy the data and paste into the dac_values array...

             The voltage values I used came from
             https://www.allaboutcircuits.com/projects/diy-synth-series-vco/

             and are
             .0833, 0.1667, 0.2500, 0.3333, 0.4167, 0.5000, 0.5833, 0.6667, 0.7500, 0.8333, 0.9167, 1.0000
             1.0833, 1.1667, 1.2500, 1.3333, 1.4167, 1.5000, 1.5833, 1.6667, 1.7500, 1.8333, 1.9167, 2.0000
             2.0833, 2.1667, 2.2500, 2.3333, 2.4167, 2.5000, 2.5833, 2.6667, 2.7500, 2.8333, 2.9167, 3.0000
             3.0833, 3.1667, 3.2500, 3.3333, 3.4167, 3.5000, 3.5833, 3.6667, 3.7500, 3.8333, 3.9167, 4.0000
             4.0833, 4.1667, 4.2500, 4.3333, 4.4167, 4.5000, 4.5833, 4.6667, 4.7500, 4.8333, 4.9167, 5.0000

             My MCP4725 is powered by a regulator that puts out 5.05 volts
             so it will reach a full 5 volts, your top voltage may be
             lower depending on the supplied power...


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

*/
#include <Wire.h>
#include <RotaryEncoder.h>
#include <elapsedMillis.h>


/* Setup the encoder */
volatile int stepCount = 0;
/* Encoder connected to pins 2, DATA and 3, CLK */
RotaryEncoder encoder(2, 3);

/************************* Defines ********************************/
#define DEBUG 1

#if DEBUG == 1
#define dprint(expression) Serial.print("# "); Serial.print( #expression ); Serial.print( ": " ); Serial.println( expression )
#define dshow(expression) Serial.println( expression )
#else
#define dprint(expression)
#define dshow(expression)
#endif

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

/*** I2C address of the MCP4725  ***/
#define I2C_ADDRESS 0x61



/************************** Variables *****************************/

/*Hold the encoder position */
uint16_t current_pos;

byte row_count;

boolean button_pushed = false;

/*
    Example values from my setup...The values from the Serial out will
    need to be copied into this array in the EEprom programmer program..
    They will be read in on the startup of the main program and can then
    be adjusted as needed and restored in EEprom..Care should be taken
    in the number of times this is done to avoid burn out of the EEprom..
    But should only be needed for tweaking of the startup values or
    occasionally for componet drift due to age...
*/
uint16_t dac_values[] = {
  63, 129, 200, 267, 337, 405, 472, 538, 607, 675, 742, 812,
  878, 948, 1013, 1083, 1148, 1219, 1284, 1354, 1421, 1490, 1557, 1627,
  1695, 1762, 1831, 1898, 1967, 2033, 2101, 2168, 2238, 2303, 2374, 2440,
  2510, 2574, 2643, 2709, 2778, 2846, 2914, 2983, 3050, 3119, 3184, 3265,
  3328, 3399, 3468, 3536, 3599, 3671, 3736, 3804, 3875, 3941, 4009, 4079,
};

/**************************  Functions ****************************/
void dac_write(int cc) {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(64);             /* cmd to update the DAC */
  Wire.write(cc >> 4);        /* the 8 most significant bits... */
  Wire.write((cc & 15) << 4); /* the 4 least significant bits... */
  Wire.endTransmission();

}


/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  if (DEBUG) {
    Serial.begin(115200);
  }

  Wire.begin();
  Wire.setClock(800000L);


  /* Setup encoder */
  encoder.setPosition(0);
  current_pos = 0;

  /************************* Setup Pins ***************************/
  /* Pin D8 for encoder button INPUT normal HIGH */
  DDRB &= ~_BV (0); // pinMode (8, INPUT);
  /* Pullup resistors */
  PORTB |= _BV (0); // digitalWrite (8, HIGH);


}/**************************  End Setup **************************/

elapsedMillis debounce;


/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {

  //----------- Check the encoder for a change ----------------------
  encoder.tick();
  int16_t newPos = encoder.getPosition();
  if (current_pos != newPos) {
    if (newPos <= 0) {
      newPos = 0;
      encoder.setPosition(newPos);
    }
    if (newPos >= 4096) {
      newPos = 4096;
      encoder.setPosition(newPos);
    }
    dac_write(newPos);
    current_pos = newPos;
  }


  /* Check the button  */

  if (((PINB & 0x01) != 1) && !button_pushed && debounce > 200) {
    Serial.print(newPos);
    Serial.print(", ");
    row_count ++;
    if (row_count > 11) {
      Serial.println();
      row_count = 0;
    }
    debounce = 0;
    button_pushed = true;
  }

  /* Stop repeat prints */
  if (((PINB & 0x01) == 1) && button_pushed) {
    button_pushed = false;
    debounce = 0;
  }


}/*************************** End Loop *****************************/
