#ifndef PIN_ISR_H_
#define PIN_ISR_H_
#include <Arduino.h>

/* Type define for the callback function */
typedef void (*isr_callback_t)(void);

namespace ISR_PORTA {

isr_callback_t isr_callback_function = NULL;

/* Attach a function to the interrupt */
void attach_interrupt(isr_callback_t callback) {
  isr_callback_function = callback;
}

ISR(PORTA_PORT_vect) {
  /* Read the flags */
  uint8_t flags = PORTA.INTFLAGS;
  /* Call the interrupt handler */
  if (isr_callback_function != NULL) {
    isr_callback_function();
  }
  /* Clear the flags */
  PORTA.INTFLAGS = flags;
}

} /* End ISR_PORTA */

namespace ISR_PORTB {

isr_callback_t isr_callback_function = NULL;

/* Attach a function to the interrupt */
void attach_interrupt(isr_callback_t callback) {
  isr_callback_function = callback;
}

ISR(PORTB_PORT_vect) {
  /* Read the flags */
  uint8_t flags = PORTB.INTFLAGS;
  /* Call the interrupt handler */
  if (isr_callback_function != NULL) {
    isr_callback_function();
  }
  /* Clear the flags */
  PORTB.INTFLAGS = flags;
}

} /* End ISR_PORTB */

namespace ISR_PORTC {

isr_callback_t isr_callback_function = NULL;

/* Attach a function to the interrupt */
void attach_interrupt(isr_callback_t callback) {
  isr_callback_function = callback;
}

ISR(PORTC_PORT_vect) {
  /* Read the flags */
  uint8_t flags = PORTC.INTFLAGS;
  /* Call the interrupt handler */
  if (isr_callback_function != NULL) {
    isr_callback_function();
  }
  /* Clear the flags */
  PORTC.INTFLAGS = flags;
}

} /* End ISR_PORTC */

namespace ISR_PORTD {

isr_callback_t isr_callback_function = NULL;

/* Attach a function to the interrupt */
void attach_interrupt(isr_callback_t callback) {
  isr_callback_function = callback;
}

ISR(PORTD_PORT_vect) {
  /* Read the flags */
  uint8_t flags = PORTD.INTFLAGS;
  /* Call the interrupt handler */
  if (isr_callback_function != NULL) {
    isr_callback_function();
  }
  /* Clear the flags */
  PORTD.INTFLAGS = flags;
}

} /* End ISR_PORTD */

namespace ISR_PORTE {

isr_callback_t isr_callback_function = NULL;

/* Attach a function to the interrupt */
void attach_interrupt(isr_callback_t callback) {
  isr_callback_function = callback;
}

ISR(PORTE_PORT_vect) {
  /* Read the flags */
  uint8_t flags = PORTE.INTFLAGS;
  /* Call the interrupt handler */
  if (isr_callback_function != NULL) {
    isr_callback_function();
  }
  /* Clear the flags */
  PORTE.INTFLAGS = flags;
}

} /* End ISR_PORTE */

namespace ISR_PORTF {

isr_callback_t isr_callback_function = NULL;

/* Attach a function to the interrupt */
void attach_interrupt(isr_callback_t callback) {
  isr_callback_function = callback;
}

ISR(PORTF_PORT_vect) {
  /* Read the flags */
  uint8_t flags = PORTF.INTFLAGS;
  /* Call the interrupt handler */
  if (isr_callback_function != NULL) {
    isr_callback_function();
  }
  /* Clear the flags */
  PORTF.INTFLAGS = flags;
}

} /* End ISR_PORTF */

#endif /* End Pin_ISR.h */
