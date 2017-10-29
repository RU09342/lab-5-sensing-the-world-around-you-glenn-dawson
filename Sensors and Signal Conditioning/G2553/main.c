#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	initGPIO();
	initADC();

	return 0;
}

void initGPIO()
{
    P6SEL |=0x01; // configure port to be used as ADC input
}

void initADC()
{
    REFCTL0 &=~REFMSTR;    // reset to hand over control internal reference voltages
                            // to ADC12_A control registers
    ADC12CTL0=ADC12SHT0_4
            +ADC12REFON
            +ADC12REFON2_5V
            +ADC12ON;    // initialize ADC12CTL0
    ADC12CTL1=ADC12SHP;                                         // initialize ADC12CTL1
    ADC12MCTL0=ADC12SREF_1+ADC12INCH0;  // set conversion memory register
}
