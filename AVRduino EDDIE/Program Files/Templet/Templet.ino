/*
                 AVRduino Templet
              Vernon Billingsley  c2025
              AVRduino AVR128DB48


      arduino IDE 1.8.19
      Board:            DxCore AVR DB-series (no bootloader)
      Chip:             AVR128DB48
      Clock:            40 mHz external clock
      millis()/micro(): disabled, delay only

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
  SDA SCK     -| C2 |               D5 |- ADC5
  SCL SS      -| C3 |- MVIO         D4 |- ADC4
              -| C4 |               D3 |- ADC3
              -| C5 |               D2 |- ADC2
              -| C6 |               D1 |- ADC1
              -| C7 |               D0 |- ADC0
                -----------------------

        Pin:       Function
        A2
        A3
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

/************************** Variables *****************************/


/**************************  Functions ****************************/


/******************************************************************/
/***************************  MAIN ********************************/
/******************************************************************/
int main() {

  /* External 40 mHz clock */
  /* Set main clock to external osc */
  _PROTECTED_WRITE(CLKCTRL_MCLKCTRLA, 0X03);
  /* No main clock prescaler */
  _PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, 0X00);
  /* External 40 mHz clock */
  _PROTECTED_WRITE(CLKCTRL_XOSCHFCTRLA, 0XAF);


  /************************* Setup Pins ***************************/


  /******************************************************************/
  /**************************** WHILE *******************************/
  /******************************************************************/
  while (1) {




  } /*************************** End While Loop *****************************/
} /****************************End Main Loop ******************************/
