/*
                      MIDI to CV
                Vernon Billingsley (c)2021

        !!! Requires DAC values from MIDI_to_CV_Program_DAC.ino to
            be stored in EEprom by MIDI_to_CC_EEprom_Programmer.ino
            Files at
            https://github.com/ScottBillingsley/Philosophy-of-Noise

            This is a program to generate a CV from incoming MIDI..
            It uses an arduino nano and MCP4725 DAC...The dac values
            are stored in EEprom..They are read into the memory array
            on startup..They can be adjusted and restored to correct
            note errors....

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
#include <MIDI.h>
#include <EEPROM.h>

//create an instance of MIDI receive on hardware serial
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, myMidi);

/* Setup the encoder */
volatile int stepCount = 0;
RotaryEncoder encoder(2, 3);

/************************* Defines ********************************/
/* Turn Off Serial... */
#define DEBUG 0

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

/* Size of the keyboard buffer */
#define KYB_BUF 5

#define GATE_ON PORTB |= _BV (4); // digitalWrite (12, HIGH);
#define GATE_OFF PORTB &= ~_BV (4); // digitalWrite (12, LOW);   


/************************** Variables *****************************/

/*Hold the encoder position */
uint16_t current_pos;
/* Blink the program LED */
boolean flip = false;

/*** About the keyboard ***/
/* Store the incoming note */
byte buff[KYB_BUF];

uint16_t dac_values[60];

/*Start the storage on address 0 and 1 */
/*Each value takes 2 bytes of storage */
uint16_t eeprom_address = 0;

boolean PROGRAM_MODE = false;
boolean NEW_NOTE = false;
boolean button_pushed = false;

byte dac_value_cell = 0;

/**************************  Functions ****************************/
void dac_write(int cc) {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(64);                     // cmd to update the DAC
  Wire.write(cc >> 4);        // the 8 most significant bits...
  Wire.write((cc & 15) << 4); // the 4 least significant bits...
  Wire.endTransmission();

}

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity) {
  /* If in the program mode skip the buffer */
  if (!PROGRAM_MODE) {
    /* Check for the first unused cell */
    for (byte cell = 0; cell < KYB_BUF; cell ++) {
      if (buff[cell] == 0) {
        buff[cell] = inNote;
        break;
      }
    }
  }

  dac_write(dac_values[inNote - 36]);

  if (PROGRAM_MODE) {
    dac_value_cell = (inNote - 36);
    NEW_NOTE = true;
  }
  GATE_ON;
}

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity) {
  /* Remove the note from the buffer */
  for (byte cell = 0; cell < KYB_BUF; cell ++) {
    if (buff[cell] == inNote) {
      buff[cell] = 0;
      break;
    }
  }
  /* Check for remaining notes on */
  byte remaining = 0;
  for (byte remain = 0; remain < KYB_BUF; remain ++) {
    remaining = remaining + buff[remain];
  }
  /* If all notes are off, turn off GATE */
  if (remaining == 0) {
    GATE_OFF;
  }
}

uint16_t eeprom_read(uint16_t address) {
  /* Read and return a stored dac value */
  /* Read the MSB */
  byte MSB = EEPROM.read(address);
  /* Read the LSB */
  byte LSB = EEPROM.read(address + 1);
  /* Return the integer */
  return (MSB << 8) + LSB;
}

void eeprom_write(uint16_t dac_val, uint16_t address) {
  /* Each dac value will require 2 bytes of EEprom */
  /* Split the MSB from the LSB  */
  byte MSB = dac_val >> 8;
  byte LSB = dac_val & 0x00ff;
  /* Store MSB in lower byte */
  EEPROM.write(address, MSB);
  /* Store LSB in upper byte */
  EEPROM.write(address + 1, LSB);
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

  //Setup the handlers for the  midi
  myMidi.setHandleNoteOn(handleNoteOn);
  myMidi.setHandleNoteOff(handleNoteOff);
  //Start the midi
  myMidi.begin(MIDI_CHANNEL_OMNI);


  /************************* Setup Pins ***************************/
  /* Pin D8 for button INPUT normal HIGH */
  DDRB &= ~_BV (0); // pinMode (8, INPUT);
  /* Pullup resistors */
  PORTB |= _BV (0); // digitalWrite (8, HIGH);

  /* Pin D9 for program switch INPUT normal HIGH */
  DDRB &= ~_BV (1); // pinMode (8, INPUT);
  /* Pullup resistors */
  PORTB |= _BV (1); // digitalWrite (8, HIGH);

  /* Pin D10 as OUTPUT for run, Green LED */
  DDRB |= _BV (2); // pinMode (10, OUTPUT);

  /* Pin D11 as OUTPUT for program, Red LED */
  DDRB |= _BV (3); // pinMode (11, OUTPUT);

  /* Pin D12 as OUTPUT for GATE */
  DDRB |= _BV (4); // pinMode (12, OUTPUT);

  DDRB |= _BV (5); // pinMode (13, OUTPUT);

  /* Read the stored DAC values from the EEprom and store them in
      the dac_values array...
  */
  for (int t = 0; t < (sizeof(dac_values) / sizeof(dac_values[0])); t++) {
    dac_values[t] = eeprom_read(eeprom_address);
    /* Increment the address */
    /*!! Remember, each value take 2 bytes */
    eeprom_address += 2;
  }


}/**************************  End Setup **************************/

elapsedMillis debounce;
elapsedMillis blink_led;

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
  /*** Check the MIDI every loop */
  myMidi.read();

  if (((PINB & 0x02) >> 1) == 1) {
    /* !!! Run Mode  */
    PORTB |= _BV (2); // digitalWrite (10, HIGH);
    PORTB &= ~_BV (3); // digitalWrite (11, LOW);
    PROGRAM_MODE = false;
  } else {
    /* !!! Program Mode */
    if (blink_led >= 250) {
      flip = !flip;
      if (flip) {
        PORTB |= _BV (3); // digitalWrite (11, HIGH);
        PORTB &= ~_BV (2); // digitalWrite (10, LOW);
      } else {
        PORTB &= ~_BV (3); // digitalWrite (11, LOW);
        PORTB &= ~_BV (2); // digitalWrite (10, LOW);
      }
      blink_led = 0;
    }
    PROGRAM_MODE = true;
  }

  /*************** Program Mode ************************/
  if (PROGRAM_MODE) {
    if (NEW_NOTE) {
      encoder.setPosition(dac_values[dac_value_cell]);
      NEW_NOTE = false;
    }


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
      /* Write the new value to the dac */
      dac_write(newPos);
      current_pos = newPos;
    }


    /* Check the button  */

    if (((PINB & 0x01) != 1) && !button_pushed && debounce > 200) {
      /* Store the new value */
      eeprom_write(newPos, (dac_value_cell * 2));
      /* Update the dac array */
      dac_values[dac_value_cell] = newPos;
      debounce = 0;
      button_pushed = true;
      PORTB |= _BV (5); // digitalWrite (13, HIGH);
    }

    /* Stop repeat prints */
    if (((PINB & 0x01) == 1) && button_pushed) {
      button_pushed = false;
      debounce = 0;
      PORTB &= ~_BV (5); // digitalWrite (13, LOW);
    }


  }/* End program mode */

}/*************************** End Loop *****************************/
