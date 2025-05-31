/*
 * 212 Steluta.c
 *
 * Created: 1/6/2025 8:53:49 PM
 * Author : stoic
 */ 

#define  F_CPU 3333333UL
#include <avr/io.h>

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "TCA.h"
#include "RTC.h"
#include "ADC.h"

// PA6 buton
// PA2 mosfet


uint16_t brightness = 0;    // how bright the LED is
uint16_t fadeAmount = 1;    // how many points to fade the LED by

   uint16_t durata_functionare = 60*60; // ore * min * sec
   
   float voltaj_bat,sample;  uint16_t bat;

//uint8_t cicluri_RTC= 6;



void disable();

int main(void)
{
   TCA0_init();
   RTC_init();
   SLPCTRL_init();
   ADC0_init_vcc();



   PORTA.DIRSET = PIN2_bm; // iesire mosfet
   sei();
   
   
   
    while (1) 
    {
		
			// o zi 1440 min -> 86.400 sec
			// 6 ore 360 min -> 21.600 sec

			// pentru a functiona doar in intervalul ales de timp
			// RTC_time se reseteaza dupa 24h
			if( (RTC_time > 0) && (RTC_time < durata_functionare) )
			{
				bat = read_vcc();
				voltaj_bat = calculate_vcc(bat);

				if (voltaj_bat>3.00)
				{
					sleep_disable();
					TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP2EN_bm;
					TCA0.SINGLE.CMP2 = brightness;
					// change the brightness for next time through the loop:
					brightness = brightness + fadeAmount;
      if(brightness==0){_delay_ms(1000);}
					// reverse the direction of the fading at the ends of the fade:
					if (brightness <= 0 || brightness >= PERIOD_VALUE) {
						fadeAmount = -fadeAmount;
						
					}
					// wait for 30 milliseconds to see the dimming effect
					_delay_ms(5);		
		
				} // Vbat > 3.0
		else{  disable();	}
			
			} // durata functionare
		else{	disable();	}		
			
			
    } // while(1)
} // main

void disable()
{
	TCA0.SINGLE.CTRLB &=~ TCA_SINGLE_CMP2EN_bm;
	PORTA.OUTCLR = PIN2_bm;
	  sleep_mode();
	  sleep_enable();
	  sleep_cpu();
}

