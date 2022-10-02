/*
              I2C to Serial
              Vernon Billingsley c2022

           A simple comm program to receive data from 
           the I2C and send it to the serial port...Used
           to develop the I2C rotary encoder on the 
           atTiny 85..
               
 
 */
#include <Wire.h>

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

/**************************  Functions ****************************/
/* Receive the data and print it to the screen 
    It expects to receive a single byte per encoder click or 
   button push 
 */
void receiveEvent(int howMany) {
  while(Wire.available()){
    byte c = Wire.read();
    Serial.println(c);
  }
}

/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  if (DEBUG) {
    Serial.begin(115200);
  }
  /* Start the wire as slave */
  Wire.begin(42); 
  /* Asign a receive event */
  Wire.onReceive(receiveEvent); 
  
  /************************* Setup Pins ***************************/


}/**************************  End Setup **************************/


/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {

delay(100);

}/*************************** End Loop *****************************/
