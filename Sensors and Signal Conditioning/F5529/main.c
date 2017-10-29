#include <msp430f5529.h>


/**
 * main.c
 * Author: Damon Boorstein and Brendan Nugent
 *          Rowan University
 * Date Created: 10/21/17
 *      Updated: 10/21/17
 */

void initGPIO();
void initADC();

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	initGPIO();
	initADC();

	while (1)
	{
        ADC12CTL0 |= ADC12SC;   // Start conversion
        __bis_SR_register(LPM0 + GIE);
        __no_operation();       // SET BREAKPOINT HERE
	}

	return 0;
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    if(ADC12MEM0 <= 0x0F) ;
}

void initGPIO()
{
    P6SEL |=0x01; // configure port to be used as ADC input

    // initialize unused ports to eliminate current consumption on unused pins
    PADIR = 0xFF;
    PAOUT = 0x00;
    PBDIR = 0xFF;
    PBOUT = 0x00;
    PCDIR = 0xFF;
    PCOUT = 0x00;
    PDDIR = 0xFF;
    PDOUT = 0x00;
}

void initADC()
{
    REFCTL0 &=~REFMSTR;                 // reset to hand over control internal reference voltages
                                        //  to ADC12_A control registers
    ADC12CTL0=ADC12SHT0_4               // initialize ADC12CTL0
            +ADC12REFON
            +ADC12REF2_5V
            +ADC12ON;
    ADC12CTL1=ADC12SHP;                 // initialize ADC12CTL1
    ADC12MCTL0=ADC12SREF_1+ADC12INCH0;  // set conversion memory register

    ADC12IE = 0x01;         //enable interrupt
    ADC12CTL0 |= ADC12ENC;  //enable conversion
}
