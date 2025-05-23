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

/*
 *    Tone(uint8_t _pin)            
 *              Decalres an instance of Tone with pin 
 *    void init()                   
 *              Resets the Tone counter to zero
 *    void update()                 
 *              Updates the Tone counter and pin state
 *    void pwm_update()             
 *              Updates the Tone pwm counter and pin state
 *    void set_duty(uint8_t duty)   
 *              Sets the duty cycle of the PWM counter
 *    void set_compare(uint16_t compare_count)  
 *              Sets the compare count of the wave
 *    void pwm_set_compare(uint16_t compare_count)
 *              Sets the compare count of the PWM wave
 *    void on()
 *              Sets the output pin as OUTPUT
 *    void off()
 *              Sets the output pin as INPUT
 *    uint16_t calculate_count(float freq, uint16_t rate)
 *              Returns the compare count for a given frequency
 *    uint16_t pwm_calculate_count(float freq, uint16_t rate)          
 *              Return the pwm compare count for a given frequency
 *    bool check()          
 *              Checks if the counter equals the compare count
 */

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
