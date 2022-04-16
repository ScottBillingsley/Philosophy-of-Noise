/*
                    Drunken Walk
                    Vernon Billingsley 2022

      A random CV generator using  MCP4921 12 Bit DAC
      on the arduino 328 Uno or Nano...

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
                 2        Clock In  (Interrupt)
                 3
                 4        LED / GATE
                 5
                 6
                 7                          MCP4921
                 8        SPI Data Out         4
                 9        SPI Chip Select      2
                 10       SPI Clock            3
                 11
                 12
                 13
                 A0       Interval of internal timer
                 A1       Random step size
                 A2       DAC output step plus random
                 A3       Ramdom style
                 A4
                 A5
                 A6
                 A6


                              MCP4921
                                 ---u---
                        +5 Vdd -|1     8|- Vout
                           !CS -|2     7|- AVss   GND
                           SCK -|3     6|- Vref   +5
                           SDI -|4     5|- !LDAC  GND
                                 -------    * LDAC tied low to transfer
                                              data from the register on
                                              the rising edge of CS

*/

#include <Arduino.h>

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


/*** The defines for SPI   ****/
/*** Use pins D8, D9, D10 ******/
#define SPI_DIR DDRB
#define SPI_PORT PORTB
/* define the pin bits */
#define LATCH 1         /* Pin D9 */
#define SCLK  2         /* Clock Pin D10 */
#define SDIO  0         /* Data  Pin D8 */
#define ADC_MAX  4

/************************** Variables *****************************/
/* MCP4921 Control Word
   Bit 15 DACa                      0
   Bit 14 Bufferd Vref              0
   Bit 13 Output gain x1            1
   Bit 12 Power down control bit    1
*/
int control_word = 0x03 << 12;

volatile boolean clk_high = false;

/*Hold the adc values */
unsigned long adc_array[ADC_MAX];
/*The ADC channel to read */
byte adc_count = 0;
/* The ADC to serive */
byte adc_service;
/* Tell there's a new sample */
boolean new_adc = false;
/* The ADC interval value for self timing */
int interval;
/* The output int to the dac */
int dac_out;
/* The calculted step valut */
int out_val = 2048;
/* Hold the random step value */
int rand_val;
/* Run the main note step */
byte do_step;
/* Hold the state of the digital pins */
byte dig_state[2] = {LOW,LOW,};
/* Hold the timeing for the triggers */
int32_t dig_millis[2] = {0, 0,};
/* The current step millis */
int32_t current_millis;
/* The previous step millis */
int32_t prev_millis;
/* How long to hold the gate open */
int32_t trig_time = 10;
/* Set the random step style */
int rand_knob;
 
/*About the filter  */
/* .125 * 256 = 32 */
unsigned long alpha = 32;
/*Store the filtered sample */
unsigned long adc_filtered[ADC_MAX];
/*Store the previous filtered sample */
unsigned long f_v[ADC_MAX][3];



/**************************  Functions ****************************/
/* An integer math low pass filter for smooting the ADC reads */
void filter(byte ary_num) {
  /*Calculate the new value */
  //f_v[ary_num][1] = (float)alpha * adc_array[ary_num] + (1 - alpha) * f_v[ary_num][0];
  f_v[ary_num][1] = (((adc_array[ary_num] << 8) * alpha) + ((256 - alpha) * ( f_v[ary_num][0] << 8))) >> 16;
  /*Store the old value */
  f_v[ary_num][0] = f_v[ary_num][1];

  /*Store the filtered value */
  adc_filtered[ary_num] = f_v[ary_num][1];
}

void chage_adc(byte this_adc){
  ADMUX = 0x40 + this_adc;
  /*Start the conversion */
  sbi(ADCSRA, ADSC); 
}


void spi_begin() {
  /************* Setup Pins ****************/
  /* Pin D9 as OUTPUT for Latch */
  SPI_DIR |= _BV (LATCH);
  /* Set HIGH to begin */
  SPI_PORT |= _BV (LATCH);
  /* Pin D10 as OUTPUT for clock */
  SPI_DIR |= _BV (SCLK);
  /* Pin D8 as OUTPUT for data */
  SPI_DIR |= _BV (SDIO);

  /* Setup timer 2 to provide data clock */
  cli();                //stop interrupts

  TCCR2A = 0;// set entire TCCR1A register to 0
  TCCR2B = 0;// same for TCCR1B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register 1 mHz
  OCR2A = 16;  /* 1 mHz  */
  // Set to CTC
  cbi(TCCR2B, WGM22);
  sbi(TCCR2A, WGM21);
  cbi(TCCR2A, WGM20);
  /*Set prescaler to 1 */
  cbi(TCCR2B, CS22);
  cbi(TCCR2B, CS21);
  sbi(TCCR2B, CS20);

  sei();                //allow interrupts
}

void send_data_word(int value) {
  /* Add control word to the data packet */
  int val = control_word + (value & 0x0fff);

  /* Take the Latch pin Low to start the transfer */
  SPI_PORT &= ~_BV (LATCH);
  /* Step through the data word one bit at a time */
  for (byte x = 0; x < 16; x ++) {

    /* Set the data pin acording to the data word bit */
    /* MSB first */
    byte b = bitRead(val, 15 - x);
    if (b == 1) {
      /* Set data pin HIGH */
      SPI_PORT |= _BV (SDIO);
    } else {
      /* Set data pin LOW */
      SPI_PORT &= ~_BV (SDIO);
    }
    /* Reset the timer2 */
    bitSet(TIFR2, 1);
    /* Wait for timer to match */
    while (!(bitRead(TIFR2, 1))) {}
    /* Take clock pin HIGH */
    SPI_PORT |= _BV (SCLK);
    /* Reset the timer to 0 */
    bitSet(TIFR2, 1);
    /* Wait for the timer to match */
    while (!(bitRead(TIFR2, 1))) {}
    /* Take the clock pin LOW */
    SPI_PORT &= ~_BV (SCLK);
    /* Reset the timer to 0 */
    bitSet(TIFR2, 1);

  }
  /* Set Latch HIGH to end the transmission */
  SPI_PORT |= _BV (LATCH);
}



/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  if (DEBUG) {
    Serial.begin(115200);
  }

  /************************* Setup Pins ***************************/
  /* Pin D2 as INPUT for interrupt */
  /* 10k resistor to GND */
  DDRD &= ~_BV (2);

  /* Pin D4 as OUTPUT for LED / GATE  */
  DDRD |= _BV (4);

  DDRB |= _BV (4);

  /* Setup Pin 2 interrupt  CHANGE */
  cbi(EICRA, ISC01);
  sbi(EICRA, ISC00);
  sbi(EIMSK, INT0);
  sei(); // allow interrupts

  /*************************  Setup ADC ***************************/
  /*Clear to Right Adjust for 1024 precision */
  cbi(ADMUX, ADLAR);

  /*Set to VRef to AVCC */
  cbi(ADMUX, REFS1);
  sbi(ADMUX, REFS0);

  /*Set to ADC0 to start */
  cbi(ADMUX, MUX3);
  cbi(ADMUX, MUX2);
  cbi(ADMUX, MUX1);
  cbi(ADMUX, MUX0);

  /*Set prescaler to 128 */
  sbi(ADCSRA, ADPS2);
  sbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

  /*Turn off Auto Trigger */
  cbi(ADCSRA, ADATE);

  /*Turn the ADC ON  */
  sbi(ADCSRA, ADEN);

  /*Start the first conversion */
  sbi(ADCSRA, ADSC);  


 /* Start the custom SPI driver */
 spi_begin();

}/**************************  End Setup **************************/

/* Pin 2 interrupt ISR  */
ISR(INT0_vect, ISR_BLOCK) 
{
  clk_high = (PIND >> 2) & 0x01;
  
}


/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
  /* Get the loop time */
  current_millis = millis();

  if(clk_high){
    clk_high = false;
    do_step = true;
  }

   if((interval < 1200) && (current_millis - prev_millis) > interval){
     prev_millis = current_millis;
     do_step = true;
   } 


  if(do_step){
    /* Set the note step */
    send_data_word(dac_out);

    /* Turn on the gate pin */
    PORTD |= _BV (4);
    /* Set the State */
    dig_state[0] = HIGH;
    /* Get the step time */
    dig_millis[0] = current_millis;

    /* Calculate the next step */
    int this_val = out_val;
    /* Set the random style */
    if(rand_knob < 512){
      this_val = out_val + random(rand_val) - (rand_val >> 1);
    }else{
      this_val = out_val + random(rand_val) - (rand_val + random(rand_knob));
    }
    out_val = this_val;

    /* Check the out_val bit range */
    if(out_val > 4095){
      out_val = 4096 - (out_val - 4095);
    }
    if(out_val < 0){
      out_val = 0 - out_val;
    }


    do_step = false;
  }/***** End do_step  *******/

  /* Check for step time out */
  if(dig_state[0] == HIGH && (current_millis - dig_millis[0]) > trig_time){
    /* Turn off the gate */
    PORTD &= ~_BV (4);
    /* Reset the state */
    dig_state[0] = LOW;

  }


  /*Check to see if ADC has finished */
  if (!(bitRead(ADCSRA, ADSC))) {
    /*Read and store the results  */
    byte temp_adcl = ADCL;
    int temp_adc = (ADCH << 8) + temp_adcl;
    /*Keep a running average */
    adc_array[adc_count] = (adc_array[adc_count] + temp_adc) / 2;
    /*Filter the results using an integer math low pass filter */
    filter(adc_count);
    adc_service = adc_count;
    adc_count ++;
    if(adc_count == ADC_MAX){
      adc_count = 0;
    }
    /* Change the ADC */
    chage_adc(adc_count);

    new_adc = true;
  }

  if(new_adc){
    switch (adc_service)
    {
    case 0:
      interval = ((1023 - adc_filtered[0]) >> 4) * 20;
      break;
    case 1:
      rand_val = adc_filtered[1] + 33;
      break;
    case 2:
      dac_out = out_val + adc_filtered[2]; 
    /* Check the dac_out bit range */
    if(dac_out > 4095){
      dac_out = 4096 - (dac_out - 4095);
    }
    if(dac_out < 0){
      dac_out = 0 - dac_out;
    }      
      break;
    case 3:
      rand_knob = adc_filtered[3];
      break;                  
    
    default:
      break;
    }
    new_adc = false;
  }


}/*************************** End Loop *****************************/
