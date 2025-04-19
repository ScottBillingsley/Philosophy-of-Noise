#include "TCA0.h"


/********************* Public functions ***************************/
void TCA0_begin(uint16_t divider, uint16_t period) {
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

void TCA0_Enable(){
  TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void TCA0_Disable(){
  TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
}

void TCA0_Change_Period(uint16_t period){
  /* Stop global interrupts */
  cli();
  /* Disable the timer */
  TCA0_Disable();
  /* Set new period */
  TCA0.SINGLE.PER = period;
  /* Enable the timer */
  TCA0_Enable();
  /* Enable global interrupts */
  sei();
}
