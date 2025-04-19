#ifndef TCA0_H
#define TCA0_H

#include <Arduino.h>

/*
 *    Functions to setup and use the TCA0 timer 
 *    
 *    Start the time by setting the divider and period 
 *    TCA0_begin(divider, period)
 *    
 *    Enable the timer
 *    TCA0_Enable()
 *    
 *    Disable the timer 
 *    TCA0_Disable()
 *    
 *    Change the period of the timer 
 *    TCA0_Change_Period(period)
 *    
 */

/******* Public functions ***************/
void TCA0_begin(uint16_t divider, uint16_t period);
void TCA0_Enable();
void TCA0_Disable();
void TCA0_Change_Period(uint16_t period);

/****** Private functions ***************/

#endif
