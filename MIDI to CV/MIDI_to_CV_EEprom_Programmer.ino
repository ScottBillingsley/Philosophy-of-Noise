/*
                MIDI to CV EEprm Programer 
                Vernon Billingsley (c)2021

                A program to program the DAC values for the MIDI
             to CV program into EEprom..The value will be read
             the dac_values array and written to the EEprom..They
             will then be read back to the Serial for comparision..


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

#include <EEPROM.h>


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


/************************** Variables *****************************/
/*!!! Copy and paste your DAC readings here !!!*/
uint16_t dac_values[] = {
  63, 129, 200, 267, 337, 405, 472, 538, 607, 675, 742, 812,
  878, 948, 1013, 1083, 1148, 1219, 1284, 1354, 1421, 1490, 1557, 1627,
  1695, 1762, 1831, 1898, 1967, 2033, 2101, 2168, 2238, 2303, 2374, 2440,
  2510, 2574, 2643, 2709, 2778, 2846, 2914, 2983, 3050, 3119, 3184, 3265,
  3328, 3399, 3468, 3536, 3599, 3671, 3736, 3804, 3875, 3941, 4009, 4079,
};

/*Start the storage on address 0 and 1 */
/*Each value takes 2 bytes of storage */
uint16_t eeprom_address = 0;

/**************************  Functions ****************************/
void eeprom_write(uint16_t dac_val, uint16_t address){
  /* Each dac value will require 2 byte of EEprom */
  /* Split the MSB from the LSB  */
  byte MSB = dac_val >> 8;
  byte LSB = dac_val & 0x00ff;
  /* Store MSB in lower byte */
  EEPROM.write(address, MSB);
  /* Store LSB in upper byte */
  EEPROM.write(address + 1, LSB);
}

uint16_t eeprom_read(uint16_t address){
  /* Read and return a stored dac value */
  /* MIDI read will be (inNote - 36) * 2 and ((inNote - 36) * 2) + 1 */
  /* Read the MSB */
  byte MSB = EEPROM.read(address);
  /* Read the LSB */
  byte LSB = EEPROM.read(address + 1);
  /* Return the integer */
  return (MSB << 8) + LSB;
}

/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  if (DEBUG) {
    Serial.begin(115200);
  }


  /************************* Setup Pins ***************************/


  for(int t = 0; t < (sizeof(dac_values) / sizeof(dac_values[0])); t++){
    
    eeprom_write(dac_values[t], eeprom_address);
    
    /* Print to Serial to check values */
    Serial.print(dac_values[t]);
    Serial.print(", ");
    Serial.println(eeprom_read(eeprom_address));
    /* Increment the address */
    /*!! Remember, each value take 2 bytes */
    eeprom_address += 2;
    
    delay(200);
  }

}/**************************  End Setup **************************/


/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {


}/*************************** End Loop *****************************/
