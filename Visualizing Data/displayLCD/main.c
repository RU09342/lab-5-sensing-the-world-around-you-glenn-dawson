#include <msp430.h> 
#include "LCDDriver.h"
#include "stdio.h"

void LCDInit();
void timerstart(int f);
int ADCdisplay;
int ADC_check;
int pauseUpdate;
int temp;

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;    // disable high-impedance

    timerstart(10);
    LCDInit();

    P1DIR &= ~BIT3;
    P1SEL0 |= BIT3;
    P1SEL1 |= BIT3;

    P1DIR &=~(BIT2); //set P1.2 input
    P1REN|=BIT2;//enable pull-up resistor
    P1OUT|=BIT2;
    P1IE |=BIT2;//enable the interrupt on P1.2
    P1IES |= BIT2;//set to look for falling edge
    P1IFG &=~(BIT2);//clear interrupt flag

    ADC12CTL0 &= ~ADC12ENC;                          // Disable Conversion
    ADC12CTL0 |= ADC12SHT0_5 | ADC12ON;              // 96 ADC12CLK cycles | ADC Module on
    ADC12CTL1 |= ADC12SHP | ADC12SSEL_3 | ADC12DIV2; // CLK Predivider = 1 | CLK Divider = 1/3 | SMCLK
    ADC12CTL2 |= ADC12RES_1;                         // Res = 10bit
    ADC12CTL3 |= ADC12CSTARTADD_0;                   // Result in ADC12MEM0
    ADC12MCTL0 |= ADC12INCH_3 | ADC12VRSEL_0;       // Input channel select A10
    //ADC12IER0 = ADC12IE0;                          // Interrupt when ready
    ADC12CTL0 |= ADC12ENC;                           // Enable Conversion

    __enable_interrupt();

    while(1){}
}

void timerstart(int f) // call function with desired frequency to initialize timer module
{
    int n;
    TA0CCTL0 = CCIE; //Enable interrupt in compare mode
    TA0CTL = TASSEL_2 + MC_1 + ID_2; //SMCLK, up mode
    n = 250000 / f; //250000 / 10 = 25000
    TA0CCR0 = n; // [( 10^6 / 4) / (25000) = 10Hz)]
}

void LCDInit()
{
    PJSEL0 = BIT4 | BIT5;                   // For LFXT

    // Initialize LCD segments 0 - 21; 26 - 43
    LCDCPCTL0 = 0xFFFF;
    LCDCPCTL1 = 0xFC3F;
    LCDCPCTL2 = 0x0FFF;

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    //PM5CTL0 &= ~LOCKLPM5;

    // Configure LFXT 32kHz crystal
    CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
    CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
    do
    {
      CSCTL5 &= ~LFXTOFFG;                  // Clear LFXT fault flag
      SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers

    // Initialize LCD_C
    // ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

    // VLCD generated internally,
    // V2-V4 generated internally, v5 to ground
    // Set VLCD voltage to 2.60v
    // Enable charge pump and select internal reference for it
    LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;

    LCDCCPCTL = LCDCPCLKSYNC;               // Clock synchronization enabled

    LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory
    //Turn LCD on
    LCDCCTL0 |= LCDON;
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A (void)
{
    if(pauseUpdate == 1) // IF the button was pressed to pause updating
    {
        ADC12CTL0 |= ADC12ENC | ADC12SC;    // Sampling and conversion start

        //reset screen
        showChar(' ', 1);
        showChar(' ', 2);
        showChar(' ', 3);
        showChar(' ', 4);

        ADC_check = ADC12MEM0;

        temp = (ADC_check*1.5*100)/4096;
        ADCdisplay = 4; // set starting index
        while (temp > 0)
        {
            int digit = temp % 10; //get one digit to display
            if(digit == 0) showChar('0', ADCdisplay);
            else if(digit == 1) showChar('1', ADCdisplay);
            else if(digit == 2) showChar('2', ADCdisplay);
            else if(digit == 3) showChar('3', ADCdisplay);
            else if(digit == 4) showChar('4', ADCdisplay);
            else if(digit == 5) showChar('5', ADCdisplay);
            else if(digit == 6) showChar('6', ADCdisplay);
            else if(digit == 7) showChar('7', ADCdisplay);
            else if(digit == 8) showChar('8', ADCdisplay);
            else if(digit == 9) showChar('9', ADCdisplay);
            ADCdisplay--;
            temp /= 10;
        }
        pauseUpdate = 0; //clear holder
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    pauseUpdate = 1; // set variable to allow update
    P1IFG &= ~BIT2; // clear flag
}
