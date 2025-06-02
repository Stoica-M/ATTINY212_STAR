/*
 * TCA.h
 *
 * Created: 07/04/2024 15:11:32
 *  Author: stoicaa
 */ 


#ifndef TCA_H_
#define TCA_H_

//#define PERIOD_VALUE        (0x05FF)



#define PERIOD_VALUE     0x03FF   //  
#define DUTY_CYCLE_VALUE 0x00FF   // 



void TCA0_init(void);
void PORT_init(void);


#endif /* TCA_H_ */
