#ifndef AVR_MIDI_H_
#define AVR_MIDI_H_
#include <Arduino.h>

/* Declare the MIDI functions */
void MIDI_Note_On(uint8_t channel, uint8_t note, uint8_t velocity);
void MIDI_Note_Off(uint8_t channel, uint8_t note, uint8_t velocity);
void MIDI_Pitch_Bend(uint8_t channel, uint8_t lsb, uint8_t msb);
void MIDI_Control_Change(uint8_t channel, uint8_t number, uint8_t value);

namespace MIDI {

/* 32 byte ring buffer */
uint8_t _serial_buffer[32] = {0};
uint8_t _buffer_write_count = 0;
uint8_t _buffer_read_count = 31;

/*
       This will start a MIDI port on Serial2..
    Serial2 was chosen because Serial0 is the default serial port
    for the serial monitor and Serial1 is on PORTC which is the MVIO port..

*/
void begin(uint32_t f_baud) {
  /* Stop all interrupts */
  cli();
  /* Set Tx pin as OUTPUT */
  PORTF.DIRSET |= PIN0_bm;
  /* Set Rx pin as INPUT */
  PORTF.DIRCLR |= PIN1_bm;
  /* Enable pullup resistor */
  PORTF.PIN1CTRL |= PORT_PINCTRLSET_3_bm;
  /* Enable the receive complete interrupt */
  USART2.CTRLA = USART_RXCIE_bm;
  /* Enable the receive and transmit modes */
  USART2.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
  /* Set port for Async, 8N1 */
  USART2.CTRLC = USART_CHSIZE_1_bm | USART_CHSIZE_0_bm;
  /* Set the baud rate */
  USART2.BAUD = ((uint32_t)64 * F_CPU) / ((uint32_t)16 * f_baud);
  /* Enable global interrupts */
  sei();
}

/* The usart interrupt handler */
inline void handle_interrupt()
{
  _serial_buffer[(_buffer_write_count & 0x1F)] = USART2.RXDATAL;
  _buffer_write_count ++;
}

/* Return the number of unread bytes in the ring buffer */
uint8_t available()
{
  if (((_buffer_write_count & 0x1F) - 1) >= (_buffer_read_count & 0x1F)) {
    return (((_buffer_write_count & 0x1F) - 1) - (_buffer_read_count & 0x1F));
  } else {
    return (32 - (_buffer_read_count & 0x1F)) + ((_buffer_write_count & 0x1F) - 1);
  }
}

/* Return the next byte in the ring buffer */
uint8_t read() {
  _buffer_read_count ++;
  return _serial_buffer[(_buffer_read_count & 0x1F)];
}

/* Call this from the main to update the midi functions */
void hook() {
  uint8_t midi_byte;

  while (MIDI::available() > 2)
  {
    midi_byte = MIDI::read();
    if ((midi_byte >> 4) == 0x09)
    {
      uint8_t channel = midi_byte - 0x90;
      uint8_t note = MIDI::read();
      uint8_t velocity = MIDI::read();
      MIDI_Note_On(channel, note, velocity);
    }
    if ((midi_byte >> 4) == 0x08)
    {
      uint8_t channel = midi_byte - 0x80;
      uint8_t note = MIDI::read();
      uint8_t velocity = MIDI::read();
      MIDI_Note_Off(channel, note, velocity);
    }
    if ((midi_byte >> 4) == 0x0E)
    {
      uint8_t channel = midi_byte - 0xE0;
      uint8_t lsb = MIDI::read();
      uint8_t msb = MIDI::read();
      MIDI_Pitch_Bend(channel, lsb, msb);
    }
    if ((midi_byte >> 4) == 0x0B)
    {
      uint8_t channel = midi_byte - 0xB0;
      uint8_t number = MIDI::read();
      uint8_t value = MIDI::read();
      MIDI_Control_Change(channel, number, value);
    }
  }/* End while */
}

}/* end namespace */

/* The receive finished interrupt service routine */
ISR(USART2_RXC_vect) {
  MIDI::handle_interrupt();
}

#endif
