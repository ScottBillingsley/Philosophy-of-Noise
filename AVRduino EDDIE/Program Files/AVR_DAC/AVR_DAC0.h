#ifndef AVR_DAC0_H_
#define AVR_DAC0_H_
#include <Arduino.h>

namespace DAC {
void begin() {
  /*********** Setup DAC ********************/
  /* Vref always on, Vref to VDD */
  VREF_DAC0REF = VREF_ALWAYSON_bm | VREF_REFSEL_2_bm | VREF_REFSEL_0_bm;;
  /* DAC always runs, Output Buffer Enabled, Enable DAC */
  DAC0_CTRLA = DAC_RUNSTDBY_bm | DAC_OUTEN_bm | DAC_ENABLE_bm;
}

/*  Output a sample to the dac..
 *  Value must be left shifted 5..
 */
void write(uint16_t data) {
  DAC0.DATA = (data << 5);
}

}/* end namespace */

#endif /* end dac0.h */
