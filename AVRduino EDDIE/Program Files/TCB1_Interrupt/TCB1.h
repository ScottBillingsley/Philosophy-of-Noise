#ifndef TCB1_H_
#define TCB1_H_
#include <Arduino.h>

#define TCB_DIV1  (0x00 << 1)
#define TCB_DIV2  (0x01 << 1)

/* Type define for the callback function */
typedef void (*tcb1_callback_t)(void);
tcb1_callback_t tcb1_callback_function = NULL;

namespace tcb1 {

void begin(uint8_t _divider, uint16_t _period) {
  if (_divider == TCB_DIV1) {
    TCB1_CCMP = (F_CPU / _period) - 1;
  } else {
    TCB1_CCMP = ((F_CPU / 2) / _period) - 1;
  }
  /* No standby, No cascade, No syncupd, _divider, Enable */
  TCB1_CTRLA = _divider | TCB_ENABLE_bm;
  /* No async, No capture, No output, INT timer mode */
  TCB1_CTRLB = 0x00;
  /* No Events */
  TCB1_EVCTRL = 0x00;
  /* Capture Interrupt */
  TCB1_INTCTRL = TCB_CAPT_bm;
} /* End begin */
/* Attach a function to the interrupt */
void attach_interrupt(tcb1_callback_t callback) {
  tcb1_callback_function = callback;
}

} /* End tcb1 namespace */

/* The interrupt service routine for the TCB0 */
ISR(TCB1_INT_vect) {
  /* Call the interrupt handler */
  if (tcb1_callback_function != NULL) {
    tcb1_callback_function();
  }
  /* Clear the interrupt flag */
  TCB1_INTFLAGS = TCB_CAPT_bm;
}

#endif /* End TCB1.h  */
