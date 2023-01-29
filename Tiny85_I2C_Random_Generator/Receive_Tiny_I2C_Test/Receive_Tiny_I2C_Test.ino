/*
                    Receive Random Number Over I2C
                      Vernon Billingsley c2023

              Send a request to Tiny85 I2C Random Generator
              for a random number and send to serial out..  

                       Arduino Nano
                     -----------------
        TXD PD1   --| D1              |-- Vin
        RXD PD0   --| D0              |-- GND
        Rest      --|                 |-- PC6 Reset
        GND       --|                 |-- 5 volt
        INT0 PD2  --| D2              |-- ADC7
   OC2B INT1 PD3  --| D3              |-- ADC6
             PD4  --| D4         ADC5 |-- PC5 ADC5 SCL
   OC0B      PD5  --| D5         ADC4 |-- PC4 ADC4 SDA
   OC0A AIN0 PD6  --| D6         ADC3 |-- PC3 ADC3
        AIN1 PD7  --| D7         ADC2 |-- PC2 ADC2
        CLK0 PB0  --| D8         ADC1 |-- PC1 ADC1
        OC1A PB1  --| D9         ADC0 |-- PC0 ADC0
   SS   OC1B PB2  --| D10        AREF |--
   MOSI OC2A PB3  --| D11             |-- 3.3 volt
   MISO      PB4  --| D12         D13 |-- PB5 SCK LED
                      ----------------

                 Pins:
                 Nano    Function
                 0
                 1
                 2
                 3
                 4
                 5
                 6
                 7
                 8       
                 9
                 10
                 11
                 12
                 13
                 A0
                 A1
                 A2
                 A3
                 A4
                 A5
                 A6
                 A7

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

#define GENERATOR_I2C_ADDRESS 0x42

/************************** Variables *****************************/


/**************************  Functions ****************************/


/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  if (DEBUG) {
    Serial.begin(115200);
  }
  Wire.begin();
  

  /************************* Setup Pins ***************************/


}/**************************  End Setup **************************/



/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
 Wire.requestFrom(GENERATOR_I2C_ADDRESS, 1); // request 1 byte from slave device address 4
 
while(Wire.available()) // slave may send less than requested
 {
 int i = Wire.read(); // receive a byte as character
 Serial.println(i); // print the character
 }
 
delay(10);


}/*************************** End Loop *****************************/
