/*
 * ADC.c
 *
 * Created: 1/2/2025 8:04:58 PM
 *  Author: stoica
 */ 


#include <avr/io.h>
#include "ADC.h"


void ADC0_init_vcc(void)
{
/* The App Note AN2447 uses Atmel Start to configure Vref but we'll do it explicitly in our code*/
    VREF.CTRLA = VREF_ADC0REFSEL_1V1_gc;  /* Set the Vref to 1.1V*/

/* The following section is directly taken from Microchip App Note AN2447 page 13*/        
    ADC0.MUXPOS = ADC_MUXPOS_INTREF_gc    /* ADC internal reference, the Vbg*/;
    
    ADC0.CTRLC = ADC_PRESC_DIV4_gc        /* CLK_PER divided by 4 */
    | ADC_REFSEL_VDDREF_gc                /* Vdd (Vcc) be ADC reference */
    | 0 << ADC_SAMPCAP_bp                 /* Sample Capacitance Selection: disabled */;
    

    ADC0.CTRLA = 1 << ADC_ENABLE_bp       /* ADC Enable: enabled */
    | 1 << ADC_FREERUN_bp                 /* ADC Free run mode: enabled */
    | ADC_RESSEL_10BIT_gc                 /* 10-bit mode */;
    ADC0.COMMAND |= 1;                    // start running ADC
}
 
// reads the internal Vcc using the ADC
uint16_t read_vcc(void)
{
	// start the ADC conversion
	ADC0.COMMAND = ADC_STCONV_bm;

	// wait for the conversion to complete
	while (!(ADC0.INTFLAGS & ADC_RESRDY_bm)) {
		;
	}

	// clear the conversion complete flag
	ADC0.INTFLAGS = ADC_RESRDY_bm;

	// return the ADC result
	return ADC0.RES;
}

// low-pass filter coefficient (value between 0 and 1)
#define ALPHA_vcc 0.0005f

// previous filtered Vcc value
float previous_vcc = 0;

// applies a low-pass filter to smooth the ADC readings
uint16_t applyLowPassFilter_vcc(uint16_t newValue) {
	// apply low-pass filter
	previous_vcc = (ALPHA_vcc * newValue) + ((1.0f - ALPHA_vcc) * previous_vcc);
	
	// convert the filtered value back to uint16_t for return
	return (uint16_t)previous_vcc;
}

// calculates the actual Vcc voltage based on the ADC result
float calculate_vcc(uint16_t adc_result)
{
	uint16_t filtered = applyLowPassFilter_vcc(adc_result);
	return (1.1 * 1024.0) / filtered;
}
