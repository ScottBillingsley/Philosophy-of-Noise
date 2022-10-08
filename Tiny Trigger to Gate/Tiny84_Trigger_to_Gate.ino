/*
                AtTiny 84 Development
                  Trigger to Gate
                Vernon Billingsley 2022

            Take a trigger/sync/gate input on interrupt pin PB2
            and Output a 50% duty gate/pulse on PA0 and a 
            10 mS pulse on PA1

                      ----U---
                VCC -| 1    14|- GND
                PB0 -| 2    13|- PA0
                PB1 -| 3    12|- PA1
                PB3 -| 4    11|- PA2
                PB2 -| 5    10|- PA3
                PA7 -| 6     9|- PA4
                PA6 -| 7     8|- PA5
                      --------

        Chip  Arduino
        Pin     Pin       Function
        ----------------------------
        13    PA0         GATE
        12    PA1         10 mS Pulse on Rising edge
        5     PB2         Interrupt IN, 100k resistor to GND

*/

/************************* Defines ********************************/

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


/************************** Variables *****************************/
volatile uint16_t time_between_interrupt = 0;
volatile boolean new_clock = false;

/**************************  Functions ****************************/


/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {


  /************************* Setup Pins ***************************/
  /* Chip pin 5, PB2 as Input  */
  DDRB &= ~_BV (2);
  /* Chip pin 13, PA0 as Output */
  DDRA |= _BV (0);
  /* Chip pin 12, PA1 as Output */
  DDRA |= _BV (1);

  /************************* Setup Interrupt *********************/
  /*Interrupt 0 on Rising Edge   */
  sbi(MCUCR, ISC01);
  sbi(MCUCR, ISC00);
  sbi(GIMSK, INT0);
  sei();

  /*************************  Setup Timer1 ************************/
  /* Use Timer 1 to get the time between interrupts */

  cli();                /*  stop interrupts */
  /*set timer1  */
  TCCR1A = 0;/* set entire TCCR1A register to 0 */
  TCCR1B = 0;/* same for TCCR1B */
  TCNT1  = 0;/* initialize counter value to 0 */
  /* Each tick is .000128 second */
  OCR1A = 0;
  /* Set to Normal mode */
  cbi(TCCR1A, WGM10);
  cbi(TCCR1A, WGM11);
  cbi(TCCR1B, WGM12);
  cbi(TCCR1B, WGM13);
  /*  Set prescaler to 1024 */
  sbi(TCCR1B, CS12);
  cbi(TCCR1B, CS11);
  sbi(TCCR1B, CS10);
  /* disable timer compare interrupt */
  cbi(TIMSK1, OCIE1A);
  sei();                /*  allow interrupts  */

}/**************************  End Setup **************************/

/* The ISR for interrupt 0 */
ISR(INT0_vect) {
  /* Store the interrupt time */
  time_between_interrupt = TCNT1;
  /* Clear the count */
  TCNT1 = 0;
  /* Take GATE pin PA0 HIGH */
  PORTA |= _BV (0);
  /* Take Pulse pin PA1 HIGH */
  PORTA |= _BV (1);
  /* Set the boolean */
  new_clock = true;

}


/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {

  if (new_clock ) {

    /* Check the timer for the GATE pin */
    if (TCNT1 > (time_between_interrupt / 2)) {
      /* Set GATE pin PA0 LOW */
      PORTA &= ~_BV (0);
      /* Clear the boolean */
      new_clock = false;
    }
    /* Check the timer for the 10 mS Pulse pin */
    if (TCNT1 > 78) {
      /* Set Pulse pin PA1 LOW */
      PORTA &= ~_BV (1);
    }

  }


}/*************************** End Loop *****************************/
