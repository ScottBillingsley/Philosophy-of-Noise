/*
                 AVRduino Tone
              Vernon Billingsley  c2025
              AVRduino AVR128DB48

      Example of simple tone generation for the AVRduino EDDIE..
      Tone generation is limited to PORTB. The pins are
      PB0 to PB5.

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
        B0        -|
        B1         |
        B2         |  tone output
        B3         |
        B4         |
        B5        -|
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
#include "TCB1.h"
#include "Tone.h"

/* Declare an instance of Tone and attach an output pin */
Tone tone0(PB0);


/************************** Variables *****************************/
const uint16_t SAMPLE_RATE = 44100;

/**************************  Functions ****************************/
void audio_update() {

  /* Update the output pin state */
  tone0.update();
  
}

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

  /* Startup the serial monitor on alternate pins A4, A5 */
  //Serial.swap(1);
  //Serial.begin(115200);

  /************************* Setup Pins ***************************/


  /* Start TCB0 with a clock division of 1 and sample rate of 44100 Hz */
  tcb1::begin(TCB_DIV1, SAMPLE_RATE);
  /* Attach a function to the interrupt */
  tcb1::attach_interrupt(audio_update);
  /* Enable global interrupts */
  sei();

  /* Set the compare count, ie 261.63 Hz, middle C */
  tone0.set_compare(tone0.calculate_count(261.63, SAMPLE_RATE));

  /******************************************************************/
  /**************************** WHILE *******************************/
  /******************************************************************/
  while (1) {
    

    for (uint8_t x = 0; x < 16; x ++) {
      NOP;
    }

  } /*************************** End While Loop *****************************/
} /****************************End Main Loop ******************************/
