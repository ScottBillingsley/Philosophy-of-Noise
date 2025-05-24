/*
                 AVRduino MIDI
              Vernon Billingsley  c2025
              AVRduino AVR128DB48

      A basic MIDI receive port for the AVRduino EDDIE..

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
        E0
        E1
        E2
        E3
        F0      MIDI Tx
        F1      MIDI Rx
        F2
        F3
        F4
        F5

                                                6N138     Vcc
                                                ---u---    |    470
                1 4         220               -| 1    8 |--|---\/\/\--|
                | |---------\/\/\---|----------| 2    7 |-----|       |
        MIDI    o o                 _       |--| 3    6 |-------------|-----> RX Out
               >    o-2           1N4148    | -| 4    5 |--|  | pin 7 
        DIN     o o                 |       |   --------   | 1K
                | |-----------------|-------|              |--|
                3 5                                       GND
*/


/************************ Includes ********************************/
#include "globals.h"
#include "AVR_MIDI.h"

/************************** Variables *****************************/


/**************************  Functions ****************************/
/********** MIDI Functions ************/
void MIDI_Note_On(uint8_t channel, uint8_t note, uint8_t velocity) {

  Serial.print("NOTE ON : ");
  Serial.println(note);


}
void MIDI_Note_Off(uint8_t channel, uint8_t note, uint8_t velocity) {

  Serial.print("NOTE OFF : ");
  Serial.println(note);

}
void MIDI_Pitch_Bend(uint8_t channel, uint8_t lsb, uint8_t msb) {
  Serial.print("Pitch Bend: ");
  Serial.println((msb << 6) | lsb);
}

void MIDI_Control_Change(uint8_t channel, uint8_t number, uint8_t value) {
  Serial.print("Control: ");
  Serial.print(number);
  Serial.print("  ");
  Serial.println(value);
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

  /* Swap the serial to the alt pins */
  Serial.swap(1);
  /* Start the serial */
  Serial.begin(115200);
  /* Start a basic midi port on Serial2 */
  MIDI::begin(31250);

  /* Enable global interrupts */
  sei();


  /******************************************************************/
  /**************************** WHILE *******************************/
  /******************************************************************/
  while (1) {
    /* Call to update the MIDI */
    MIDI::hook();

    /* 
     *  Short delay to slow main down a little.
     *  May not be needed if more functions are  
     *  added to main..
     */
    for (uint8_t x = 0; x < 4; x ++) {
      NOP;
    }

  } /*************************** End While Loop *****************************/
} /****************************End Main Loop ******************************/
