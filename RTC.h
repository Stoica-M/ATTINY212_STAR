/*
 * RTC.h
 *
 * Created: 1/2/2025 1:30:08 PM
 *  Author: stoic
 */ 


#ifndef RTC_H_
#define RTC_H_


void RTC_init(void);
void LED0_init(void); 
void SLPCTRL_init(void);

extern uint8_t RTC_count;
extern uint32_t RTC_time;


#endif /* RTC_H_ */