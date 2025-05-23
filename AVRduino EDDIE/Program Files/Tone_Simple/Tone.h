#ifndef TONE_H_
#define TONE_H_
#include <Arduino.h>

/* Output pins defines */
#define PB0 PIN0_bm
#define PB1 PIN1_bm
#define PB2 PIN2_bm
#define PB3 PIN3_bm
#define PB4 PIN4_bm
#define PB5 PIN5_bm
#define PB6 PIN6_bm
#define PB7 PIN7_bm

class Tone {

  private:
    uint16_t counter;
    uint16_t compare_count;
    uint8_t _PIN;
    uint8_t _duty = 64;

  public:
    Tone(uint8_t _pin);
    void init();
    void update();
    void pwm_update();
    void set_duty(uint8_t duty);
    void set_compare(uint16_t compare_count);
    void pwm_set_compare(uint16_t compare_count);
    void on();
    void off();
    uint16_t calculate_count(float freq, uint16_t rate);
    uint16_t pwm_calculate_count(float freq, uint16_t rate);
    bool check();

}; /* End note timer */

#endif /* End tone.h */
