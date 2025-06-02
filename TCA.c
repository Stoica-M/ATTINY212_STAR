/*
 * TCA.c
 *
 * Created: 07/04/2024 15:11:15
 *  Author: stoica
 */ 
#define  F_CPU 3000000UL

#include <avr/io.h>
//#include <avr/interrupt.h>
#include "TCA.h"

void TCA0_init(void) {
	// stop the timer before configuration
	TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
	
	// set PWM mode to single slope, enable channel 2 (CMP2)
	TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP2EN_bm;
	
	// set the PWM period and duty cycle
	TCA0.SINGLE.PER = PERIOD_VALUE;       // PWM period
	TCA0.SINGLE.CMP2 = DUTY_CYCLE_VALUE;  // PWM duty cycle
	
	// start the timer with no prescaler (CLK_PER directly)
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
}


void PORT_init(void)
{
	  PORTA.DIRSET = PIN2_bm;
}

/*
void TCA0_init(void)
{
	// Configure TCA0 for single slope PWM
	
	
	; // Select clock source with no prescaler
	TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc |  TCA_SINGLE_CMP2EN_bm; // Single slope PWM mode, enable compare 0
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; // Enable TCA0
	TCA0.SINGLE.INTCTRL |= TCA_SINGLE_OVF_bm; //enable overflow interrupt
	// Set period and duty cycle
	TCA0.SINGLE.PER =PERIOD_VALUE; // Set period
	//TCA0.SINGLE.CMP1 = 0x7FF; // Set duty cycle (50%)
}

void PORT_init(void)
{
	// Set PA2 as output for PWM
	PORTA.DIRSET |=  PIN2_bm;
}



ISR(TCA0_OVF_vect)  //on overflow, we will increment TCA0.CMP0, this will happen after every full cycle - a little over 7 minutes.
{
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; //Always remember to clear the interrupt flags, otherwise the interrupt will fire continually!
}
*/
