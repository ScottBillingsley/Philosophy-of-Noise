/*
                        Charge!!
                  Vernon Billingsley 2022

                  An adaptation of a circuit in an old
                  Popluar Electronics Magazine just for fun..
                        

  Timer 2 Tone Generator
  Tone Control  ADC0
  Tempo Control ADC1

  Pin D2, Interrupt Trigger    Vcc --- Switch --- .1 uf ----- D2
                                               |          | 
  Pin D7, Tempo                               10k         -
                                               |          ^ 1N4148 
  Pin D8, Tone Output                          |          | 
                                              GND        GND

  Source article :

  https://worldradiohistory.com/Archive-Poptronics/70s/1978/Poptronics-1978-01.pdf

*/

/************************* Defines ********************************/
#define DEBUG 0

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

/* The number of ADC channels to use */
#define ADC_MAX 3

/************************** Variables *****************************/
/* About the Tempo   */
volatile boolean tempo_clk = false;
volatile byte tempo_divide_by_256;


/* Divide counters */
volatile byte divide_by;

/* The note to play  */
volatile byte note;

boolean playing = false;

volatile boolean next_step = false;
byte song_step;

/* Bugle Charge  */
/*  note_duration = 5   */
/*  short_rest = 1      */
/*  long_rest = 5       */

//byte charge[] = {
byte my_array[] = {  
  3,3,3,3,3, 0, 3,3,3,3,3, 0,0,0,0,0,
  3,3,3,3,3, 0, 3,3,3,3,3, 0,0,0,0,0,
  3,3,3,3,3, 0, 3,3,3,3,3, 0,0,0,0,0,
  3,3,3,3,3, 0, 3,3,3,3,3, 0,0,0,0,0,
  2,2,2,2,2, 0, 1,1,1,1,1, 0,0,0,0,0,
  2,2,2,2,2, 0, 1,1,1,1,1, 0,0,0,0,0,
  2,2,2,2,2, 0, 1,1,1,1,1, 0,0,0,0,0,
  2,2,2,2,2, 0, 3,3,3,3,3, 3,3,3,3,3,
  0,0,0,0,0, 0, 0,0,0,0,0, 0,0,0,0,0,
};

/* Marry had a little lamb with 3 notes */
/* Note duration = 4                    */
/* Rest duration = 4                    */
byte mary[] = {
//byte my_array[] = {
  1,1,1,1, 0,0,0,0, 2,2,2,2, 0,0,0,0,
  3,3,3,3, 0,0,0,0, 2,2,2,2, 0,0,0,0,
  1,1,1,1, 0,0,0,0, 1,1,1,1, 0,0,0,0,
  1,1,1,1, 1,1,1,1, 0,0,0,0, 0,0,0,0,
  2,2,2,2, 0,0,0,0, 2,2,2,2, 0,0,0,0,
  2,2,2,2, 2,2,2,2, 0,0,0,0, 0,0,0,0,
  1,1,1,1, 0,0,0,0, 1,1,1,1, 0,0,0,0,
  1,1,1,1, 1,1,1,1, 0,0,0,0, 0,0,0,0,
  1,1,1,1, 0,0,0,0, 2,2,2,2, 0,0,0,0,
  3,3,3,3, 0,0,0,0, 2,2,2,2, 0,0,0,0,
  1,1,1,1, 0,0,0,0, 1,1,1,1, 0,0,0,0,
  1,1,1,1, 0,0,0,0, 1,1,1,1, 0,0,0,0, 
  2,2,2,2, 0,0,0,0, 2,2,2,2, 0,0,0,0,
  1,1,1,1, 0,0,0,0, 2,2,2,2, 0,0,0,0,
  3,3,3,3, 3,3,3,3, 0,0,0,0, 0,0,0,0, 
};


/*Hold the adc values */
unsigned long adc_array[5];
/*The ADC channel to read */
uint8_t adc_count = 0;

/*About the filter  */
/* .125 * 256 = 32 */
unsigned long alpha = 32;
/*Store the filtered sample */
unsigned long adc_filtered[5];
/*Store the previous filtered sample */
unsigned long f_v[5][3];

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

void change_adc(byte adc) {
  switch (adc) {
    case 0:
      ADMUX = 0x40;
      break;
    case 1:
      ADMUX = 0x41;
      break;
    case 2:
      ADMUX = 0x42;
      break;
    case 3:
      ADMUX = 0x43;
      break;
    case 4:
      ADMUX = 0x44;
      break;
    case 5:
      ADMUX = 0x45;
      break;
    case 6:
      ADMUX = 0x46;
      break;
    case 7:
      ADMUX = 0x47;
      break;
  }
  /*Start the conversion */
  sbi(ADCSRA, ADSC);
}

/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  if (DEBUG) {
    Serial.begin(115200);
  }
  /* Setup a trigger */
  attachInterrupt(digitalPinToInterrupt(2), play, RISING);

  /************************* Setup Pins ***************************/
  /* Tempo Pin D7 */
  DDRD |= _BV (7);

  /* Tone Output */
  DDRB |= _BV (0); // pinMode (8, OUTPUT);


  /*************************  Setup ADC ***************************/
  /*Set to Right Adjust for 1024 precision */
  cbi(ADMUX, ADLAR);

  /*Set to VRef to AVCC */
  cbi(ADMUX, REFS1);
  sbi(ADMUX, REFS0);

  /*Set to ADC0 to start */
  cbi(ADMUX, MUX3);
  cbi(ADMUX, MUX2);
  cbi(ADMUX, MUX1);
  cbi(ADMUX, MUX0);

  /*Set prescaler to 64 */
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

  /*Turn off Auto Trigger */
  cbi(ADCSRA, ADATE);

  /*Turn the ADC ON  */
  sbi(ADCSRA, ADEN);

  /*Start the first conversion */
  sbi(ADCSRA, ADSC);

  /*************************  Setup Timer1 ************************/
  cli();                //stop interrupts
  //set timer1
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register to 8 kHz
  OCR1A = 1999;
  // turn on CTC mode
  sbi(TCCR1B, WGM12);
  /*Set prescaler to 1 */
  cbi(TCCR1B, CS12);
  cbi(TCCR1B, CS11);
  sbi(TCCR1B, CS10);
  // enable timer compare interrupt
  sbi(TIMSK1, OCIE1A);
  sei();


  /*************************  Setup Timer2 ************************/
  cli();                //stop interrupts

  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register 8 kHz
  OCR2A = 124;
  // Set to CTC
  cbi(TCCR2B, WGM22);
  sbi(TCCR2A, WGM21);
  cbi(TCCR2A, WGM20);
  /*Set prescaler to 32 */
  cbi(TCCR2B, CS22);
  sbi(TCCR2B, CS21);
  sbi(TCCR2B, CS20);
  // enable timer compare interrupt
  sbi(TIMSK2, OCIE2A);
  sei();                //allow interrupts

}/**************************  End Setup **************************/

ISR(TIMER1_COMPA_vect) {
  tempo_divide_by_256 ++;
  if (tempo_divide_by_256 == 255) {
    tempo_clk = !tempo_clk;
    if (tempo_clk) {
      PORTD |= _BV (7); // digitalWrite (2, HIGH);
    } else {
      PORTD &= ~_BV (7); // digitalWrite (2, LOW);
    }
    tempo_divide_by_256 = 0;
    next_step = true;
  }
}

ISR(TIMER2_COMPA_vect) {
  /* Increment the counter */
  divide_by ++;

  switch (note) {
    case 0:   /* Rest */
      /* Clear the counter */
      divide_by = 0;
      /* Lower output pin */
      PORTB &= ~_BV (0);
      break;
    case 1:   /* divide by 10 */
      if (divide_by == 9) {
        divide_by = 0;
        PINB = PINB | 0b00000001;
      }
      break;
    case 2:   /* divide by 12 */
      if (divide_by == 11) {
        divide_by = 0;
        PINB = PINB | 0b00000001;
      }
      break;
    case 3:   /* divide by 15 */
      if (divide_by == 14) {
        divide_by = 0;
        PINB = PINB | 0b00000001;
      }
      break;
  }
}/* End ISR */

void play(){
  playing = true;
}



/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {

  if (next_step && playing) {
    if (tempo_clk) {
      note = my_array[song_step];
      song_step ++;
      if (song_step > sizeof(my_array) - 1) {
        song_step = 0;
        playing = false;
      }
    }
    next_step = false;
  }

  /*Check to see if ADC has finished */
  if (!(bitRead(ADCSRA, ADSC))) {
    /*Read and store the results  */
    uint8_t temp_adcl = ADCL;
    uint16_t temp_adc = (ADCH << 8) + temp_adcl;
    /*Keep a running average */
    adc_array[adc_count] = (adc_array[adc_count] + temp_adc) / 2;
    /*Filter the results using an integer math low pass filter */
    filter(adc_count);
    if (DEBUG) {
      /*Send the results to the Plotter to test */
      Serial.println(adc_filtered[adc_count]);
    }
    if (adc_count == 0) {
      OCR2A = map(adc_filtered[0], 0, 1024, 255, 25);
    }
    if (adc_count == 1) {
      OCR1A = map(adc_filtered[2], 0, 1024, 1200, 300);
    }

    adc_count ++;
    if (adc_count >= ADC_MAX) {
      adc_count = 0;
    }
    /* Change the ADC */
    change_adc(adc_count);

  }

}/*************************** End Loop *****************************/
