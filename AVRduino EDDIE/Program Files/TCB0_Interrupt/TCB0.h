#ifndef TCB0_H_
#define TCB0_H_
#include <Arduino.h>

#define TCB_DIV1  (0x00 << 1)
#define TCB_DIV2  (0x01 << 1)

/* Type define for the callback function */
typedef void (*tcb0_callback_t)(void);
tcb0_callback_t tcb0_callback_function = NULL;

namespace tcb0 {

void begin(uint8_t _divider, uint16_t _period) {
  if (_divider == TCB_DIV1) {
    TCB0_CCMP = (F_CPU / _period) - 1;
  } else {
    TCB0_CCMP = ((F_CPU / 2) / _period) - 1;
  }
  /* No standby, No cascade, No syncupd, _divider, Enable */
  TCB0_CTRLA = _divider | TCB_ENABLE_bm;
  /* No async, No capture, No output, INT timer mode */
  TCB0_CTRLB = 0x00;
  /* No Events */
  TCB0_EVCTRL = 0x00;
  /* Capture Interrupt */
  TCB0_INTCTRL = TCB_CAPT_bm;
} /* End begin */
/* Attach a function to the interrupt */
void attach_interrupt(tcb0_callback_t callback) {
  tcb0_callback_function = callback;
}

} /* End tcb0 namespace */

/* The interrupt service routine for the TCB0 */
ISR(TCB0_INT_vect) {
  /* Call the interrupt handler */
  if (tcb0_callback_function != NULL) {
    tcb0_callback_function();
  }
  /* Clear the interrupt flag */
  TCB0_INTFLAGS = TCB_CAPT_bm;
}


#endif /* End TCB0.h  */
