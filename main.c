/*
 * 212 Steluta.c
 *
 * Created: 1/6/2025 8:53:49 PM
 * Author : stoica
 */

#define F_CPU 3333333UL
#include <avr/io.h>

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "TCA.h"
#include "RTC.h"
#include "ADC.h"

// PA6 - button
// PA2 - mosfet control


uint16_t brightness = 0;    // current LED brightness
uint16_t fadeAmount = 1;    // step size for fading effect

uint16_t runtime_duration = 60 * 60; // operating time in seconds (hours * min * sec)

float battery_voltage, sample;
uint16_t bat_adc;

void disable();

int main(void)
{
    TCA0_init();
    RTC_init();
    SLPCTRL_init();
    ADC0_init_vcc();

    // configure PA2 as output (controls the MOSFET)
    PORTA.DIRSET = PIN2_bm;
    sei();

    while (1) 
    {
        // one day = 1440 minutes -> 86,400 seconds
        // 6 hours = 360 minutes -> 21,600 seconds

        // this block ensures operation only during the selected interval
        // RTC_time resets every 24h
        if ((RTC_time > 0) && (RTC_time < runtime_duration))
        {
            bat_adc = read_vcc();
            battery_voltage = calculate_vcc(bat_adc);

            if (battery_voltage > 3.00)
            {
                sleep_disable();
                TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP2EN_bm;
                TCA0.SINGLE.CMP2 = brightness;

                // change the brightness for next iteration
                brightness = brightness + fadeAmount;

                if (brightness == 0) {
                    _delay_ms(1000);
                }

                // reverse fading direction at min/max limits
                if (brightness <= 0 || brightness >= PERIOD_VALUE) {
                    fadeAmount = -fadeAmount;
                }

                // wait 5 ms to make the fading visible
                _delay_ms(5);		
            }
            else {
                disable();
            }
        }
        else {
            disable();
        }
    }
}

void disable()
{
    TCA0.SINGLE.CTRLB &= ~TCA_SINGLE_CMP2EN_bm;
    PORTA.OUTCLR = PIN2_bm;
    sleep_mode();
    sleep_enable();
    sleep_cpu();
}
