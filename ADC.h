/*
 * ADC.h
 *
 * Created: 1/2/2025 8:05:27 PM
 *  Author: stoic
 */ 


#ifndef ADC_H_
#define ADC_H_

void ADC0_init_vcc(void);
uint16_t read_vcc(void);
float calculate_vcc(uint16_t adc_result);



#endif /* ADC_H_ */