#ifndef ADC0_H_
#define ADC0_H_

#include <Arduino.h>

/* ADC MUX Pin defines */
#define PIN_D0  0x00
#define PIN_D1  0x01
#define PIN_D2  0x02
#define PIN_D3  0x03
#define PIN_D4  0x04
#define PIN_D5  0x05
#define PIN_D6  0x06
#define PIN_D7  0x07
#define PIN_E0  0x08
#define PIN_E1  0x09
#define PIN_E2  0x0A
#define PIN_E3  0x0B
#define PIN_F0  0x10
#define PIN_F1  0x11
#define PIN_F2  0x12
#define PIN_F3  0x13
#define PIN_F4  0x14
#define PIN_F5  0x15

uint8_t adc_scan[22] = {0};
uint8_t adc_scan_count = 0;
uint8_t adc_count = 0;
uint16_t adc_array[22] = {0};

namespace ADC {

/*
   Starts a standard non blocking ADC at a prescale of 32..
   Setting interrupt to true starts the update interrupt and the
   results are automaticly read and stored..
   If set to false, update must be called externaly to read and store
   the results..
*/
void begin(bool _interrupt)
{
  /* Vref always on, Vref to VDD */
  VREF_ADC0REF = VREF_ALWAYSON_bm | VREF_REFSEL_2_bm | VREF_REFSEL_0_bm; //0X85;
  /* ADC0 in Singleended, Not Leftadjusted, 12 bit, One Shot */
  ADC0_CTRLA = ADC_ENABLE_bm; //0x01;
  /* ADC0 0 accumulated samples */
  ADC0_CTRLB = 0x00;
  /* ADC0 Prescale of 32 */
  ADC0_CTRLC = ADC_PRESC_3_bm;  //0x08;
  /* ADC0 Initdelay of 16, No sample delay */
  ADC0_CTRLD = ADC_INITDLY_0_bm;  //0x10;
  /* ADC0 neg pin to GND */
  ADC0_MUXNEG = ADC_MUXNEG_6_bm;  //0x40;

  if (_interrupt == true)
  {
    ADC0_INTCTRL = ADC_RESRDY_bm;
    /* Start the first conversion */
    ADC0_COMMAND = ADC_STCONV_bm;
    /* Enable global interrupts */
    sei();
  }
}

/* Checks if the current read is finished */
uint8_t isFinished()
{
  return (ADC0_COMMAND & ADC_STCONV_bm);
}

/* Start the next read */
void start()
{
  ADC0_COMMAND = ADC_STCONV_bm;
}

/* Add a pin to the ADC MUX */
void add_pin(uint8_t _pin)
{
  /* Set the first adc */
  if (adc_scan_count == 0) {
    ADC0_MUXPOS = _pin;
  }
  adc_scan[adc_scan_count] = _pin;
  adc_scan_count ++;
}

/*    If the ADC is finished, store the value in
    the array, increment the adc mux and start
    the next read..
*/
void update()
{
  if (ADC::isFinished() == 0) {
    adc_array[adc_scan[adc_count]] = ADC0_RES;
    adc_count ++;
    if (adc_count == adc_scan_count) {
      adc_count = 0;
    }
    ADC0_MUXPOS = adc_scan[adc_count];
    ADC::start();
  }
}

/* Read the adc pin */
uint16_t read(uint8_t _pin)
{
  return adc_array[_pin];
}

}/* end ADC namespace */

#endif

/* ISR for internal interrupt read */
ISR(ADC0_RESRDY_vect) {
  /* Read and store the result */
  adc_array[adc_scan[adc_count]] = ADC0_RES;
  /* Increment the MUX count */
  adc_count ++;
  if (adc_count == adc_scan_count) {
    adc_count = 0;
  }
  /* Set the new MUX */
  ADC0_MUXPOS = adc_scan[adc_count];
  /* Start the next conversion */
  ADC0_COMMAND = ADC_STCONV_bm;
  /* Clear the interrupt flag */
  ADC0_INTFLAGS = ADC_RESRDY_bm;
}
