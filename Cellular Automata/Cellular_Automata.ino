/*
*                         Cellular Automata
*                       Vernon Billingsley c2024
*
*               Displays 30 generations of cellular automata to
*               the serial monitor..Rules are from 0 to 255..
*               A few interesting rules:
*                 30  54  62  90  94  110 126 150 158
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
*                             Arduino Nano
*                           -----------------
*              TXD PD1   --| D1              |-- Vin
*              RXD PD0   --| D0              |-- GND
*              Rest      --|                 |-- PC6 Reset
*              GND       --|                 |-- 5 volt
*              INT0 PD2  --| D2              |-- ADC7
*         OC2B INT1 PD3  --| D3              |-- ADC6
*                   PD4  --| D4         ADC5 |-- PC5 ADC5 SCL
*         OC0B      PD5  --| D5         ADC4 |-- PC4 ADC4 SDA
*         OC0A AIN0 PD6  --| D6         ADC3 |-- PC3 ADC3
*              AIN1 PD7  --| D7         ADC2 |-- PC2 ADC2
*              CLK0 PB0  --| D8         ADC1 |-- PC1 ADC1
*              OC1A PB1  --| D9         ADC0 |-- PC0 ADC0
*         SS   OC1B PB2  --| D10        AREF |--
*         MOSI OC2A PB3  --| D11             |-- 3.3 volt
*         MISO      PB4  --| D12         D13 |-- PB5 SCK LED
*                            ----------------
*
*                       Pins:
*                       Nano    Function
*                       0
*                       1
*                       2
*                       3
*                       4
*                       5
*                       6
*                       7
*                       8
*                       9
*                       10
*                       11
*                       12
*                       13
*                       A0
*                       A1
*                       A2
*                       A3
*                       A4
*                       A5
*                       A6
*                       A7
*
*   https://mathworld.wolfram.com/ElementaryCellularAutomaton.html
*   https://www.wolframscience.com/nks/
*
*/
/************************ Includes ********************************/
#include "globals.h"



/************************** Variables *****************************/
/* Define the rule to display */
uint8_t RULE = 90;

uint8_t display_reg[100];
uint8_t build_reg[100];
/* Hole the current rule */
uint8_t rule[8];


/**************************  Functions ****************************/
/* Setup the rule array and place center pixel in display array */
void startup(){
  for(int8_t x = 7; x >= 0; x --){
    rule[x] = bitRead(RULE, x);
  }
  for(int8_t q = 0; q < 49; q ++){
    display_reg[q] = 0;
  }
  display_reg[49] = 1;
  for(int8_t w = 50; w < 101; w ++){
    display_reg[w] = 0;
  }
}

/* Store the build register in the display register */
void store_reg(){
  for(uint8_t x = 0; x < 100; x ++){
    display_reg[x] = build_reg[x];
  }
}
/* Display the array to the serial monitor */
void display(){
  for(uint8_t d = 0; d < 100; d ++){
    uint8_t this_bit = display_reg[d];
    if(this_bit == 1){
      Serial.print("X");
    }else{
      Serial.print(" ");
    }
  }
  Serial.println();
}
/* Calculate the next generation */
void next_generation(){
  uint8_t index;
  /* We are skipping the first and last element to avoid worrying about */
  /* wrap around                                                        */
  for(uint8_t d = 1; d < 99; d ++){
    uint8_t l = display_reg[d - 1];
    uint8_t m = display_reg[d];
    uint8_t r = display_reg[d + 1];
    index = (l << 2) | (m << 1) | r;
    build_reg[d] = rule[index];
  }
}

/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  if (DEBUG == 1) {
    Serial.begin(115200);
  }


  /************************* Setup Pins ***************************/


} /**************************  End Setup **************************/



/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
 
  Serial.println("Enter number of Rule to view...");

  while(Serial.available() == 0){}

  RULE = Serial.parseInt();

  startup();

  /* Show 30 generations */
  for(uint8_t x = 0; x < 30; x ++){
    /* Show the current generation */
    display();
    /* Calculate the next generation */
    next_generation();
    /* Store the new generation */
    store_reg();
    /* Wait a little bit */
    delay(64);
  }

  while(Serial.available() > 0){
    uint8_t clear_buffer = Serial.read();
  }

  Serial.println();

} /*************************** End Loop *****************************/
