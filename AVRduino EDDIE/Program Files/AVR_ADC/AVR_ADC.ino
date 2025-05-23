/*
                 AVRduino AVR ADC
              Vernon Billingsley  c2025
              AVRduino AVR128DB48

      A non blocking ADC read..

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
        E0        Vin
        E1        Vin
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
#include "ADC0.h"
/* Needed for the external control example */
//#include "TCA0.h"


/************************** Variables *****************************/


/**************************  Functions ****************************/
/* Example for external read control */
//void update_adc(){
  /* Call to update the ADC and place the valuse in an array */
//  ADC::update();
//}


/******************************************************************/
/***************************  MAIN ********************************/
/******************************************************************/
int main() {
  /*
         Set the main system clock for Internal 32 mHz
         or External 40 mHz according to the build options..
  */
  if (F_CPU == 32000000ul) {
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

  /********** Serial Setup ***********/
  /* Swap the serial to the alt pins */
  Serial.swap(1);
  /* Start the serial */
  Serial.begin(115200);

  /* Example of using TCA0 to control the ADC update time */
  /* Start the control timer at 64 cycles per second */
  //tca0::begin(1024, 609);
  /* Attach a function to the TAC0 interrupt */
  //tca0::attach_interrupt(update_adc);

  /* Start the standard non_blocking ADC with internal interrupt */
  ADC::begin(true);

  /* Start the standard non_blocking ADC with external update */
  //ADC::begin(false);

  /* Add ADC pins to the MUX */
  ADC::add_pin(PIN_E0);
  ADC::add_pin(PIN_E1); 
  
  /* Enable global interrupts */
  sei();

  /******************************************************************/
  /**************************** WHILE *******************************/
  /******************************************************************/
  while (1) {

    /* Read the ADC by calling read ADC with the pin number */
    Serial.print(ADC::read(PIN_E0) >> 4);
    Serial.print("  ");
    Serial.println(ADC::read(PIN_E1));

    delay(250);

  } /*************************** End While Loop *****************************/
} /****************************End Main Loop ******************************/
