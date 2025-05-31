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
 

uint16_t read_vcc(void)
{
    /* Porne?te conversia */
    ADC0.COMMAND = ADC_STCONV_bm;

    /* A?teapt? finalizarea conversiei */
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm)) {
        ;
    }

    /* Cur??? flag-ul de conversie complet? */
    ADC0.INTFLAGS = ADC_RESRDY_bm;

    /* Returneaz? rezultatul */
    return ADC0.RES;
}


#define ALPHA_vcc 0.0005f // Factor de atenuare (valoare între 0 ?i 1)
float previous_vcc = 0; // Valoarea filtrat? anterioar?

uint16_t applyLowPassFilter_vcc(uint16_t newValue) {
	// Aplic?m filtrul pas-bas
	previous_vcc = (ALPHA_vcc* newValue) + ((1.0f - ALPHA_vcc) * previous_vcc);
	
	// Convertim rezultatul în uint16_t pentru a-l returna
	return (uint16_t)previous_vcc;
}

float calculate_vcc(uint16_t adc_result)
{
	uint16_t filtru = applyLowPassFilter_vcc(adc_result);
    return (1.1 * 1024.0) / filtru;
}