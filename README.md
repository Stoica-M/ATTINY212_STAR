# ATTINY212_STAR
To this little star ornament, I added a small circuit based on an ATtiny212 microcontroller, along with a 18650 rechargeable battery and a TP4056 charging module. Since I didn’t want the star to stay lit continuously, I incorporated a real-time clock (RTC) to control the lighting schedule. The light turns on only for a specific period each day, after which the system enters sleep mode until the next cycle. This approach greatly extends battery life.

However, because the ATtiny doesn’t use an external crystal for the RTC, I noticed that over time, there’s a drift in the activation schedule. To improve long-term accuracy, I plan to add a 32.768 kHz external crystal in a future version.
