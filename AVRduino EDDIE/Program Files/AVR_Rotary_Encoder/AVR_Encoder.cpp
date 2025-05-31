#include "AVR_Encoder.h"

/*
    Constructor..
    Requires port the encoder is connected to, the data pin, clock pin and button
    pin of the encoder..All pins must be on the same port...
*/
AVR_Encoder::AVR_Encoder(PORT_struct &port, uint8_t data_pin, uint8_t clk_pin, uint8_t button_pin)
{

  /* Define the start state of the variables */
  this_encoder._button_pressed = false;
  this_encoder.new_count = false;
  this_encoder.count_step = 1;
  this_encoder.r_count = 0;

  /* Set the button pin to INPUT */
  port.DIRCLR = button_pin;
  /* Config the pin */
  /* Non invert, Schmitt trigger, pullups, FALLING */
  switch (button_pin)
  {
    case PIN0_bm:
      port.PIN0CTRL = PORT_PULLUPEN_bm | PORT_ISC_0_bm | PORT_ISC_1_bm;
      this_encoder._button_pin = 0;
      break;
    case PIN1_bm:
      port.PIN1CTRL = PORT_PULLUPEN_bm | PORT_ISC_0_bm | PORT_ISC_1_bm;
      this_encoder._button_pin = 1;
      break;
    case PIN2_bm:
      port.PIN2CTRL = PORT_PULLUPEN_bm | PORT_ISC_0_bm | PORT_ISC_1_bm;
      this_encoder._button_pin = 2;
      break;
    case PIN3_bm:
      port.PIN3CTRL = PORT_PULLUPEN_bm | PORT_ISC_0_bm | PORT_ISC_1_bm;
      this_encoder._button_pin = 3;
      break;
    case PIN4_bm:
      port.PIN4CTRL = PORT_PULLUPEN_bm | PORT_ISC_0_bm | PORT_ISC_1_bm;
      this_encoder._button_pin = 4;
      break;
    case PIN5_bm:
      port.PIN5CTRL = PORT_PULLUPEN_bm | PORT_ISC_0_bm | PORT_ISC_1_bm;
      this_encoder._button_pin = 5;
      break;
    case PIN6_bm:
      port.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_0_bm | PORT_ISC_1_bm;
      this_encoder._button_pin = 6;
      break;
    case PIN7_bm:
      port.PIN7CTRL = PORT_PULLUPEN_bm | PORT_ISC_0_bm | PORT_ISC_1_bm;
      this_encoder._button_pin = 7;
      break;
  } /* end switch */
  /* Set the data pin to INPUT */
  port.DIRCLR = data_pin;
  /* Config the pin */
  /* Non invert, Schmitt trigger, no pullups, Both edge */
  switch (data_pin)
  {
    case PIN0_bm:
      port.PIN0CTRL = PORT_ISC_0_bm;
      this_encoder._data_pin = 0;
      break;
    case PIN1_bm:
      port.PIN1CTRL = PORT_ISC_0_bm;
      this_encoder._data_pin = 1;
      break;
    case PIN2_bm:
      port.PIN2CTRL = PORT_ISC_0_bm;
      this_encoder._data_pin = 2;
      break;
    case PIN3_bm:
      port.PIN3CTRL = PORT_ISC_0_bm;
      this_encoder._data_pin = 3;
      break;
    case PIN4_bm:
      port.PIN4CTRL = PORT_ISC_0_bm;
      this_encoder._data_pin = 4;
      break;
    case PIN5_bm:
      port.PIN5CTRL = PORT_ISC_0_bm;
      this_encoder._data_pin = 5;
      break;
    case PIN6_bm:
      port.PIN6CTRL = PORT_ISC_0_bm;
      this_encoder._data_pin = 6;
      break;
    case PIN7_bm:
      port.PIN7CTRL = PORT_ISC_0_bm;
      this_encoder._data_pin = 7;
      break;
  } /* end switch */

  /* Set the data pin to INPUT */
  port.DIRCLR = clk_pin;
  /* Config the pin */
  /* Non invert, Schmitt trigger, no pullups, Both edge */
  switch (clk_pin)
  {
    case PIN0_bm:
      port.PIN0CTRL = PORT_ISC_0_bm;
      this_encoder._clock_pin = 0;
      break;
    case PIN1_bm:
      port.PIN1CTRL = PORT_ISC_0_bm;
      this_encoder._clock_pin = 1;
      break;
    case PIN2_bm:
      port.PIN2CTRL = PORT_ISC_0_bm;
      this_encoder._clock_pin = 2;
      break;
    case PIN3_bm:
      port.PIN3CTRL = PORT_ISC_0_bm;
      this_encoder._clock_pin = 3;
      break;
    case PIN4_bm:
      port.PIN4CTRL = PORT_ISC_0_bm;
      this_encoder._clock_pin = 4;
      break;
    case PIN5_bm:
      port.PIN5CTRL = PORT_ISC_0_bm;
      this_encoder._clock_pin = 5;
      break;
    case PIN6_bm:
      port.PIN6CTRL = PORT_ISC_0_bm;
      this_encoder._clock_pin = 6;
      break;
    case PIN7_bm:
      port.PIN7CTRL = PORT_ISC_0_bm;
      this_encoder._clock_pin = 6;
      break;
  } /* end switch */

}/* End AVR_Encoder */

/*
    This is the main calculation function called from
    the pin interrupt ISR..
*/
void  AVR_Encoder::interrupt_handler(PORT_struct &port)
{
  static uint8_t clk_state;
  static uint8_t dat_state;
  static bool clk_read = false;

  if (((port.IN >> this_encoder._button_pin) & 0x01) == 0 && this_encoder._button_pressed == false) {
    this_encoder._button_pressed = true;
  }

  clk_state = ((port.IN >> this_encoder._clock_pin) & 0x01);
  dat_state = ((port.IN >> this_encoder._data_pin) & 0x01);
  /* Get direction */
  if (clk_state == 0 && clk_read == false) {
    clk_read = true;
    if (dat_state == 0) {
      this_encoder.r_count -= this_encoder.count_step;
    } else {
      this_encoder.r_count += this_encoder.count_step;
    }
    this_encoder.new_count = true;
  }

  if (clk_state == 1 && clk_read == true) {
    clk_read = false;
  }

}/* end interrupt handler */

/*
   Check if the encoder has be turned..
*/
bool AVR_Encoder::check()
{
  if (this_encoder.new_count == true)
  {
    this_encoder.new_count = false;
    return true;
  } else {
    return false;
  }
}

/*
    Sets the count of the encoder, default is 0..
*/
void AVR_Encoder::set_count(int16_t count)
{
  this_encoder.r_count = count;
}

/*
    Set the counts per tick, default is 1..
*/
void AVR_Encoder::count_by(int16_t value)
{
  this_encoder.count_step = value;
}

/*
    Return the current count.
*/
int16_t AVR_Encoder::read()
{
  return this_encoder.r_count;
}

/*
    Check if the button was pressed..
*/
bool AVR_Encoder::check_button()
{
  if (this_encoder._button_pressed == true)
  {
    this_encoder._button_pressed = false;
    return true;
  } else {
    return false;
  }
}
