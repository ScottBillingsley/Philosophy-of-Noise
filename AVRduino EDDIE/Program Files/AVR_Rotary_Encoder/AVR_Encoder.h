#ifndef AVR_ENCODER_H_
#define AVR_ENCODER_H_
#include <Arduino.h>

struct _encoder {
  uint8_t _data_pin;
  uint8_t _clock_pin;
  uint8_t _button_pin;
  int16_t r_count;
  int16_t count_step;
  bool _button_pressed;
  bool new_count;
};

class AVR_Encoder
{
  public:

    AVR_Encoder(PORT_struct &port, uint8_t data_pin, uint8_t clk_pin, uint8_t button_pin);
    bool check_button();
    bool check();
    int16_t read();
    void set_count(int16_t count);
    void count_by(int16_t value);

    void interrupt_handler(PORT_struct &port);

  private:
    struct _encoder this_encoder;

};

#endif  /* End AVR_Encoder.h */
