/*
*                   Cicada PCM Baby Eight
*                   Vernon Billingsley c2024
*
*               A basic scale PCM synth on the arduino nano
*               using a pin change interrupt to read and debounce 
*               the input buttons..
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
*                       5       CD4051 Ctrl A       
*                       6       CD4051 Ctrl B
*                       7       CD4051 Ctrl C
*                       8       Pin Interrupt In  10k to GND
*                       9       CD4051 Data In
*                       10
*                       11      PCM Wave OUT
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
*/


/************************ Includes ********************************/
#include "globals.h"
#include "timer1_things.h"
#include "timer2_things.h"



/************************** Variables *****************************/
/* The accu word */
volatile uint16_t mWord;
/* Select the waveform  */
volatile uint8_t wave = 0;
/* The envelope multiplier */
volatile int16_t envelope = 255;
/* The button pressed */
volatile uint8_t button;
volatile bool new_button = false;
volatile bool note_on = false;

/**************************  Functions ****************************/


ISR(TIMER1_COMPA_vect) {
  /* The word accumulator */
  static uint16_t mAccu;
  /* The accu increment */
  uint16_t mIcnt;
  /* Bit mask to form the triangle */
  uint8_t mask = 0;
  /* Hold the output sample */
  uint8_t out_sample;

  /* Increment the accumlator */
  asm(/* mAccu += mWord */
      "add %A1, %A2     \n"
      "adc %B1, %B2     \n"
      : "=r"(mAccu)
      : "r"(mAccu), "r"(mWord));

  /* Get the top 9 bits */
  asm(/* mIcnt = mAccu >> 7 */
      "mov %A0, %B1     \n"
      "clr %B0          \n"
      "lsl %A0          \n"
      "rol %B0          \n"
      : "+r"(mIcnt)
      : "r"(mAccu));
  /* Get the bit mask to reverse for triangle and form the square wave */
  asm(/* mask = mIcnt >> 8    */
      "mov %A0, %B1     \n"
      : "=r"(mask)
      : "r"(mIcnt));

  /* Get the sample according to the wave select */
  switch (wave) {
    case 0:
      if (mask) { /* Triangle */
        out_sample = ~mIcnt & 0x01ff;
      } else {
        out_sample = mIcnt;
      }
      break;
    case 1: /* Ramp */
      out_sample = mIcnt >> 1;
      break;
    case 2: /* Square */
      if (mask) {
        out_sample = 0;
      } else {
        out_sample = 255;
      }
      break;
  }

  /* Apply the envelope */
  /* Simple AR envelope, 15 mS Attack and Release */
  /*  (1 / Sample time) * 256 = 15 mS             */
  if (note_on == true) {
    envelope++;
    if (envelope >= 255) {
      envelope = 255;
    }
  } else {
    envelope--;
    if (envelope < 0) {
      envelope = 0;
    }
  }

  //out_sample = (out_sample * envelope) >> 8;
  asm(
    "mov r16, %A0       \n"
    "mov r17, %A1       \n"
    "mul r17, r16       \n"
    "mov %A0, r1       \n"
    : "+r"(out_sample)
    : "r"(envelope)
    : "r16", "r17");


  OCR2A = out_sample;
} /* End Timer1 ISR */

ISR(PCINT0_vect) {
  static bool state0 = LOW;
  uint8_t this_port = PINB & 0x01;
  uint8_t this_control = PIND & 0x1f;

  if (this_port == 1 && state0 == LOW) {
    state0 = HIGH;
    /* Active LED */
    PORTB |= _BV(5);  // digitalWrite (13, HIGH);
    /* Get the button pressed */
    for (uint8_t b = 0; b < 8; b++) {
      /* Set the control port */
      PORTD = this_control | (b << 5);
      /* Wait a little for the '4051 to settle */
      delayMicroseconds(5);
      /* Read the input bit */
      uint8_t this_bit = (PINB >> 1) & 001;
      /* Store the bit */
      bitWrite(button, (7 - b), this_bit);
    }
    note_on = true;
    new_button = true;
  }
  /* Reset the button */
  if (this_port == 0 and state0 == HIGH) {
    state0 = LOW;
    PORTB &= ~_BV(5);  // digitalWrite (13, LOW);
    note_on = false;
  }
}

/******************************************************************/
/***************************  MAIN ********************************/
/******************************************************************/
int main() {
  /* the bootloader connects pins 0 and 1 to the USART;  */
  /* disconnect them here so they can be used as normal digital i/o */
  /* wiring.c */
  cbi(UCSR0B, RXEN0);
  cbi(UCSR0B, TXEN0);
  /* If you need delay or micros comment out the above and */
  /* uncomment below                                       */
  //init();
  //Serial.begin(115200);


  /************************* Setup Pins ***************************/
  /*PCM pin 11 as OUTPUT */
  DDRB |= _BV(3);  // pinMode (11, OUTPUT);

  /* Pins D5, D6 and D7 as OUPUT for CD4051 Control */
  DDRD |= _BV(5);  // pinMode (5, OUTPUT);
  DDRD |= _BV(6);  // pinMode (6, OUTPUT);
  DDRD |= _BV(7);  // pinMode (7, OUTPUT);

  /* Pin D8 as INPUT for pin change interrupt */
  DDRB &= ~_BV(0);  // pinMode (8, INPUT);
  /* Pin D9 as INPUT for CD4051 data in */
  DDRB &= ~_BV(1);  // pinMode (9, INPUT);

  /* Pin D13 as OUTPUT for LED Blink */
  DDRB |= _BV(5);  // pinMode (13, OUTPUT);

  /* Disable Interrupts */
  cli();
  /* Setup Timer1 for the sample clock */
  timer1::begin();
  /* Setup Timer2 for PWM on pin D11 */
  timer2::begin();

  /* Setup the pin change interrupt */
  sbi(PCICR, PCIE0);
  sbi(PCMSK0, PCINT0); /* Interrupt on pin 8 */

  /* Enable Interrupts  */
  sei();


  mWord = (float)pow(2, 16) * 110.0 / SAMPLE_RATE;

  /******************************************************************/
  /**************************** WHILE *******************************/
  /******************************************************************/
  while (1) {

    if (new_button == true) {
      float freq;
      switch (button) {
        case 0b10000000:
          freq = 130.812;
          break;
        case 0b01000000:
          freq = 146.832;
          break;
        case 0b00100000:
          freq = 164.812;
          break;
        case 0b00010000:
          freq = 174.614;
          break;
        case 0b00001000:
          freq = 195.997;
          break;
        case 0b00000100:
          freq = 220.0;
          break;
        case 0b00000010:
          freq = 246.941;
          break;
        case 0b00000001:
          freq = 261.625;
          break;
      }

      mWord = (float)pow(2, 16) * freq / SAMPLE_RATE;

      new_button = false;
    }




  } /*************************** End While Loop *****************************/
} /****************************End Main Loop ******************************/
