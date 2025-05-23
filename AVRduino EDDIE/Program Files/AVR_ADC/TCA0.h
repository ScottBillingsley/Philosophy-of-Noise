#ifndef TCA0_H
#define TCA0_H

#include <Arduino.h>

/*
 *          Setup the TCA0 on the AVR128DB48
 */

/* Type define for the callback function */
typedef void (*tca0_callback_t)(void);
tca0_callback_t tca0_callback_function = NULL;

namespace tca0 {
/* Setup the timer */
void begin(uint16_t divider, uint16_t period) {
  uint16_t clksel;
  /* Set the overflow interrupt bit in the Interrupt Control register */
  TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
  /* No waveform generation */
  TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
  /* No events */
  TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm);
  /* Set the Period register */
  TCA0.SINGLE.PER = period;  
  /* Select the divider */
  switch (divider) {
    case 1:
      clksel = (0x00<<1);
      break;
    case 2:
      clksel = (0x01<<1);
      break;
    case 4:
      clksel = (0x02<<1);
      break;
    case 8:
      clksel = (0x03<<1);
      break;
    case 16:
      clksel = (0x04<<1);
      break;
    case 64:
      clksel = (0x05<<1);
      break;
    case 256:
      clksel = (0x06<<1);
      break;
    case 1024:
      clksel = (0x07<<1);
      break;
  }
  /* Set the divider and set the Enable bit */
  TCA0.SINGLE.CTRLA = clksel | TCA_SINGLE_ENABLE_bm;
}

/* Enable the timer */
void enable(){
  TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

/* Disable the timer */
void disable(){
  TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
}

/* Set a new period */
void set_period(uint16_t period){
  /* Stop global interrupts */
  cli();
  /* Disable the timer */
  tca0::disable();
  /* Set new period */
  TCA0.SINGLE.PER = period;
  /* Enable the timer */
  tca0::enable();
  /* Enable global interrupts */
  sei();
}

/* Attach a function to the interrupt */
void attach_interrupt(tca0_callback_t callback) {
  tca0_callback_function = callback;
}  
  
}/* End namespace */

#endif /* end TCA0.h */

/* The interrupt service routine for the TCA0 */
ISR(TCA0_OVF_vect) {
  /* Call the interrupt handler */
  if (tca0_callback_function != NULL) {
    tca0_callback_function();
  }
  /* Clear the interrupt flag */
  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}
