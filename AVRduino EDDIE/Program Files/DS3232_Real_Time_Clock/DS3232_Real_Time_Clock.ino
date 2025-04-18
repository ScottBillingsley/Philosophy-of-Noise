/*
               DS3232 Real Time Clock
              Vernon Billingsley  c2025
              AVRduino AVR128DB48

      Routines to read and write the DS3232 Real Time Clock over I2C

      arduino IDE 1.8.19
      Board:            DxCore AVR DB-series (no bootloader)
      Chip:             AVR128DB48
      Clock:            40 mHz external clock
      millis()/micro(): disabled, delay only

      Bootloader burn required...
      Reset pin         Hardware reset
      MVIO              Enabled ( Jumper on 3.3 volt )
      
      programmer:       SerialUPDI - 230400 Baud


                   PWR   UPDI   Serial
                -----------------------
         SDA  -| A2                 F5 |-
         SCL  -| A3                 F4 |-
    MOSI TXD0 -| A4        NC 3.3   F3 |-
    MISO RXD0 -| A5   A1   +5  +5   F2 |-
    SCK       -| A6   LED           F1 |-
    SS        -| A7                 F0 |-
              -| B0   TX RX         E3 |-
              -| B1   NC NC         E2 |-
              -| B2                 E1 |-
              -| B3                 E0 |-
              -| B4                GND |-
              -| B5                 +5 |-
    MOSI TXD1 -| C0 |               D7 |- ADC7
    MISO RXD1 -| C1 |               D6 |- ADC6  DAC Out
    SCK  SDA  -| C2 |               D5 |- ADC5
    SS   SCL  -| C3 |- MVIO         D4 |- ADC4
              -| C4 |               D3 |- ADC3
              -| C5 |               D2 |- ADC2
              -| C6 |               D1 |- ADC1
              -| C7 |               D0 |- ADC0
                -----------------------

        Pin:       Function
        A2         SDA
        A3         SCL
        A4
        A5
        A6
        A7
        B0
        B1
        B2
        B3
        B4
        B5
        C0
        C1
        C2
        C3
        C4
        C5
        C6
        C7
        D0
        D1
        D2
        D3
        D4
        D5
        D6
        D7
        E0
        E1
        E2
        E3
        F0
        F1
        F2
        F3
        F4
        F5

*/


/************************ Includes ********************************/
#include "globals.h"
#include <Wire.h>
#include "DS3232_AVR.h"


/* Define the main clock source as either _INTERNAL or _EXTERNAL */
/* Remember to set the build options to the correct clock speed  */
#define _EXTERNAL 0
#define _INTERNAL 1
#define MAIN_CLOCK _EXTERNAL

/************************** Variables *****************************/



/**************************  Functions ****************************/


/******************************************************************/
/***************************  MAIN ********************************/
/******************************************************************/
int main() {
  /* Set the main system clock  */
  if (MAIN_CLOCK == _INTERNAL) {
    /* Set main clock to internal osc */
    _PROTECTED_WRITE(CLKCTRL_MCLKCTRLA, 0X00);
    /* No main clock prescaler */
    _PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, 0X00);
    /* Internal 32 mHz */
    _PROTECTED_WRITE(CLKCTRL_OSCHFCTRLA, 0XAC);
  } else {
    /* External 40 mHz clock */
    /* Set main clock to external osc */
    _PROTECTED_WRITE(CLKCTRL_MCLKCTRLA, 0X03);
    /* No main clock prescaler */
    _PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, 0X00);
    /* External 40 mHz clock */
    _PROTECTED_WRITE(CLKCTRL_XOSCHFCTRLA, 0XAF);
  }

  /* Start the I2C library on default pins PA2, PA3 */
  Wire.begin();
  /* Set the clock to 400 kHz */
  Wire.setClock(400000UL);  

  /* Startup the serial monitor on alternate pins A4, A5 */
  Serial.swap(1);
  Serial.begin(115200);

  /* Set the address of the RTC */
  rtc_set_address(0x68);


  /************************* Setup Pins ***************************/


/*
  rtc_write(SET_12, 0x00);
  delay(10);
  rtc_write(HOUR, 6);
  delay(10);
  rtc_write(MINUTE,42);
  delay(10);
  rtc_write(SECOND,0);  
  delay(10);
  rtc_write(DAY,6);
  delay(10);
  rtc_write(MONTH,4);
    delay(10);
  rtc_write(YEAR,25); 
    delay(10);
  rtc_write(SET_PM,0); 
*/

  Serial.println("DS3232 Real Time Clock ");


  /******************************************************************/
  /**************************** WHILE *******************************/
  /******************************************************************/
  while (1) {

    /* Read the DS3232 */
    rtc_read();

    Serial.print(rtc_month());
    Serial.print("/");
    Serial.print(rtc_day());
    Serial.print("/");
    Serial.print(rtc_year());
    Serial.print("  ");
    Serial.print(rtc_hours());
    Serial.print(":");
    uint8_t m = rtc_minutes();
    if (m < 10) {
      Serial.print("0");
    }
    Serial.print(m);
    Serial.print(":");
    uint8_t s = rtc_seconds();
    if (s < 10) {
      Serial.print("0");
    }
    Serial.print(s);
    Serial.println(" ");

    if(rtc_seconds() == 0x00){
    Serial.println(rtc_temperature(), 2);
    }

    delay(1000);

  } /*************************** End While Loop *****************************/
} /****************************End Main Loop ******************************/
