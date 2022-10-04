/*
                          Tiny85 I2C Rotary Encoder
                          Vernon Billingsley  c2022
                  
                  Decode a rotary encoder and send the data over
                  I2C as a master sender...
                  Sends 0b001 , 1 as Up
                        0b100 , 4 as Down
                        0b010 , 2 as Button pressed


            atTiny85                           
            ---U---
    Reset -| 1    8|- Vcc
    PB3   -| 2    7|- PB2
    PB4   -| 3    6|- PB1
    GND   -| 4    5|- PB0
            -------  

     Pin    Function
     ----------------------------
     1      NC
     2      Encoder CLK
     3      Encoder DATA
     4      GND
     5      I2C SDA
     6      Encoder Button
     7      IC2 SCL
     8      +5 vdc

           Encoder        
             ---                      
            |   |---- +5               Tiny85               Uno/Nano
          --|   |---- GND              ---U---            
         |  |   |---- CLK ------------| 2   5 |- SDA  ----|A4
          --|   |---- Data -----------| 3   7 |- SCL  ----|A5
            |   |---- Button ---------| 6   1 |-
             ---                 GND -| 4   8 |- +5
                                       -------  
             
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

    atTiny85 core:
    https://github.com/damellis/attiny  
    Internal 8 mHz
    
    I2C code:
    https://medium.com/@prateek.ideafires/programming-attiny85-and-i2c-communication-using-attiny85-a7dd41e4c907

    Encoder code:
    https://thewanderingengineer.com/2014/08/11/rotary-encoder-on-the-attiny85-part-2/

*/

/* https://github.com/adafruit/TinyWireM */
#include<TinyWireM.h>
#include "avr/interrupt.h";


/************************* Defines ********************************/
/* The bus address of the slave device */
#define address 42

#define EncoderA 3
#define EncoderB 4

/************************** Variables *****************************/
volatile byte lastEncoded = 0;
volatile byte data = 0;

volatile boolean new_data = false;

boolean button_pressed = false;
boolean button_released = true;
long current_millis;
long old_millis;

/**************************  Functions ****************************/
void send_byte(byte val) {
  TinyWireM.beginTransmission(address);
  TinyWireM.send(val);
  TinyWireM.endTransmission();
}

/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  /* Begin the wire */
  TinyWireM.begin();

  /************************* Setup Pins ***************************/
  /* Pin D1 to INPUT and pullup resistors */
  pinMode(1, INPUT_PULLUP);
  /* Pin D3 to INPUT and pullup resistors */
  pinMode(3, INPUT_PULLUP);
  /* Pin D4 to INPUT and pullup resistors */
  pinMode(4, INPUT_PULLUP);

  GIMSK = 0b00100000;       // Enable pin change interrupts
  PCMSK = 0b00011000;       // Enable pin change interrupt for PB3 and PD4
  sei();                    // Turn on interrupts

}/**************************  End Setup **************************/

/* This is the ISR that is called on each interrupt   */
ISR(PCINT0_vect)
{
  byte MSB = PINB >> EncoderA & 1;
  byte LSB = PINB >> EncoderB & 1;

  byte encoded = (MSB << 1) | LSB;
  byte sum = (lastEncoded << 2) | encoded;

  lastEncoded = encoded;

    /* Checking for one of the 4 positions allows sending one
     *  pulse per click.... 
     */
    if (sum == 0b1101 ){  //|| sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
      data = 0b001;
      new_data = true;
    }
    if (sum == 0b1110 ){  //|| sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
      data = 0b100;
      new_data = true;
    }
}

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
  current_millis = millis();

  if (new_data == true) {
    send_byte(data);
    new_data = false;
  }
  /* If the button is pressed, send the data */
  if (!digitalRead(1) && !button_pressed) {
    send_byte(0b010);
    button_pressed = true;
    button_released = false;
  }
  /*************** Debounce the button ******************/
  /* Wait for the button to be released */
  if (digitalRead(1) && !button_released) {
    /* Set the boolean */
    button_released = true;
    /* Get the current time */
    old_millis = current_millis;
  }
  /* If the button is released and the current time stored */
  /* Wait for 150 mS to clear the boolean */
  if (digitalRead(1) && button_released && current_millis > (old_millis + 150)) {
    button_pressed = false;
  }


}/*************************** End Loop *****************************/
