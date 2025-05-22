#ifndef RTC_H_
#define RTC_H_
#include <Arduino.h>

/* Real Time Counter Prescaler */
#define RTC_divide_by_1     (0x00 << 3)
#define RTC_divide_by_2     (0x01 << 3)
#define RTC_divide_by_4     (0x02 << 3)
#define RTC_divide_by_8     (0x03 << 3)
#define RTC_divide_by_16    (0x04 << 3)
#define RTC_divide_by_32    (0x05 << 3)
#define RTC_divide_by_64    (0x06 << 3)
#define RTC_divide_by_128   (0x07 << 3)
#define RTC_divide_by_256   (0x08 << 3)
#define RTC_divide_by_512   (0x09 << 3)
#define RTC_divide_by_1024  (0x0A << 3)
#define RTC_divide_by_2048  (0x0B << 3)
#define RTC_divide_by_4096  (0x0C << 3)
#define RTC_divide_by_8192  (0x0D << 3)
#define RTC_divide_by_16384 (0x0E << 3)
#define RTC_divide_by_32768 (0x0F << 3)

/* Real Time Counter clock select */
#define OSC32K  0x00
#define OSC1K   0x01
#define XOSC32K 0x02
#define EXTCLK  0x03

#define ENABLE 0x01
#define DISABLE 0x00

/* Type define for the callback function */
typedef void (*rtc_callback_t)(void);
rtc_callback_t rtc_callback_function = NULL;

namespace rtc {
/* Set the clock source and divider for the RTC */
void begin(uint16_t _divider, uint8_t _clock) {
  RTC_CLKSEL = _clock;
  RTC_CTRLA = _divider | RTC_RTCEN_bm;
}

/* Set the period of the timer */
void period(uint16_t _period) {
  RTC_PER = _period;
}

/* Enable or disable the interrupt */
void interrupt(bool _state) {
  if (_state == ENABLE) {
    RTC_INTCTRL = RTC_CMP_bm;
  } else {
    RTC_INTCTRL = 0x00;
  }
}

/* Disable the RTC */
void disable() {
  uint8_t t = RTC_CTRLA;
  RTC_CTRLA = t & 0xFE;
}

/* Enable the RTC */
void enable() {
  uint8_t t = RTC_CTRLA;
  RTC_CTRLA = t & 0xFF;
}

/* Attach a function to the interrupt */
void attach_interrupt(rtc_callback_t callback) {
  rtc_callback_function = callback;
}

}/* end rtc namespace */

/* The interrupt service routine for the RTC */
ISR (RTC_CNT_vect) {
  /* Call the interrupt handler */
  if (rtc_callback_function != NULL) {
    rtc_callback_function();
  }
  /* Clear the interrupt flag */
  RTC_INTFLAGS = RTC_CMP_bm;
}

#endif /* End RTC,h  */
