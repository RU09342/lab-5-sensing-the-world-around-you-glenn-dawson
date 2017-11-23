# Sensors and Signal Conditioning

## Code

### Files
* MSP430F55xx_adc_01.c
* msp430fr231x_adc10_01.c
* msp430fr599x_adc12_01.c
* msp430fr69xx_adc12_01.c
* msp430g2x33_adc10_01.c

### Dependencies
None

### Compatability
All boards listed.

### Implementation
This code will turn an on-board LED on if the voltage sampled by the ADC is greater than half the reference voltage. The ADC will start sampling and converting while the processor waits in LPM0. The ADC ISR will force the program out of the main loop when conversion finishes.



## Hardware


