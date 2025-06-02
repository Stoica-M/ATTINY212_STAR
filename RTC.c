/*
 * RTC.c
 *
 * Created: 1/2/2025 1:29:07 PM
 *  Author: stoica
 */ 


#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "RTC.h"

void RTC_init(void)
{
	uint8_t temp;
	
	/* Initialize 32.768kHz Oscillator: */
	/* Disable oscillator: */
	temp = CLKCTRL.XOSC32KCTRLA;
	temp &= ~CLKCTRL_ENABLE_bm;
	/* Writing to protected register */
	ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);
	
	while(CLKCTRL.MCLKSTATUS & CLKCTRL_XOSC32KS_bm)
	{
		; /* Wait until XOSC32KS becomes 0 */
	}
	
	/* SEL = 0 (Use External Crystal): */
	temp = CLKCTRL.XOSC32KCTRLA;
	temp &= ~CLKCTRL_SEL_bm;
	/* Writing to protected register */
	ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);
	
	/* Enable oscillator: */
	temp = CLKCTRL.XOSC32KCTRLA;
	temp |= CLKCTRL_ENABLE_bm;
	/* Writing to protected register */
	ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);
	
	/* Initialize RTC: */
	while (RTC.STATUS > 0)
	{
		; /* Wait for all register to be synchronized */
	}

	/* 32.768kHz External Crystal Oscillator (XOSC32K) */
	RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;

	/* Run in debug: enabled */
	RTC.DBGCTRL = RTC_DBGRUN_bm;
	
	RTC.PITINTCTRL = RTC_PI_bm; /* Periodic Interrupt: enabled */
	
	RTC.PITCTRLA = RTC_PERIOD_CYC32768_gc /* RTC Clock Cycles 32768 */
	| RTC_PITEN_bm; /* Enable: enabled */
}



//uint8_t RTC_count;
uint32_t RTC_time;


ISR(RTC_PIT_vect)
{
	/* Clear flag by writing '1': */
	RTC.PITINTFLAGS = RTC_PI_bm;
	//RTC_count++;
	RTC_time++;
	//if(RTC_time > 86400){RTC_time=0;}  // reset at one day;
	if(RTC_time > 86400){RTC_time=0;}  // reset at 12 hour
}

void SLPCTRL_init(void)
{
	SLPCTRL.CTRLA |= SLPCTRL_SMODE_PDOWN_gc;
	SLPCTRL.CTRLA |= SLPCTRL_SEN_bm;
	for (uint8_t pin=0; pin < 8; pin++)
	{
		(&PORTA.PIN0CTRL)[pin] = PORT_ISC_INPUT_DISABLE_gc; //Disable on PAx pin
	}
	//set_sleep_mode(SLEEP_MODE_IDLE);
	
}
