# Visualizing Data with MSP430 ADC
The included program is compatible with the MSP430FR6989. The program is designed to display the ADC contents of the microcontroller on its on-board LCD screen, particularly where the input to the ADC is from the circuits configured using a photoresistor, phototransistor and LM35 temperature sensor. A separate program is included for the LM35, in which the ADC contents are converted to the temperature in degrees Celcius.

## Dependencies
This program depends on having the 6989-specific header file, which is included in the main file. This file is the MSP430.h header file included at the beginning of the main file. Moreover, the program needs to use the 'LCDDriver.h' and its accompanying 'LCDDriver.c' file. These files are included in the repo.

## Usage
To use this program, configure the circuit shown in the picture included (OP0805SL). This circuit uses a phototransistor and its collector voltage as input to the ADC. The voltage can be increased by shielding the phototransistor from light and decreased by shining intense light on the phototransistor. These voltage changes are reflected by the values held in ADC10MEM0. The ADC contents can be seen on the LCD display. One can also use the circuit shown in photoresistor.JPG. The same process holds true for this circuit.

The LM35 uses its own program to display the temperature on the LCD screen. Using the transfer function provided by the manufacturer, the program converts the ADC contents to the temperature in Celcius. The conversion is as follows: 

	temp = ADC10MEM;
    IntDegC = (1.5 * temp) * 100 / 4096;
	
Again, this is derived from the transfer function: V = 10mV/C * T. The circuit simply wires the pins on the LM35 to the MCU: Vss to 3.3V, GND to the MCU's GND pin and V_out to P1.4, which is configured as the ADC input.

## ADC Configuration
Although the code included contains its own documentation, the following will provide an overview of how the ADC is configured.

	ADC12CTL0 &= ~ADC12ENC;                          // Disable Conversion
    ADC12CTL0 |= ADC12SHT0_5 | ADC12ON;              // 96 ADC12CLK cycles | ADC Module on
    ADC12CTL1 |= ADC12SHP | ADC12SSEL_3 | ADC12DIV2; // CLK Predivider = 1 | CLK Divider = 1/3 | SMCLK
    ADC12CTL2 |= ADC12RES_1;                         // Res = 10bit
    ADC12CTL3 |= ADC12CSTARTADD_0;                   // Result in ADC12MEM0
    ADC12MCTL0 |= ADC12INCH_3 | ADC12VRSEL_0;       // Input channel select A3
    ADC12CTL0 |= ADC12ENC;                           // Enable Conversion

This code configures the 12-bit ADC to use channel A3, located on P1.3. Moreover, the ADC is set to use VREF, which is configured as the default 1.2V. The voltage reference is important in converting the ADC contents to a temperature(displayed from the LM35).

## How do these circuits work?
![Circuits](https://github.com/RU09342/lab-5-sensing-the-world-around-you-glenn-dawson/blob/master/Sensors%20and%20Signal%20Conditioning/circuits.jpg)

The circuit which uses the phototransistor is commonly known as a common-emitter amplifier, meaning that the emitter of transistor is grounded. In this configuration, V_out is taken at the collector of the transistor.

The amount of base current is proportional to the amount of light that the phototransistor receives. By controlling the base current, the transistor's collector current also changes accordingly. The increase in current creates a greater voltage drop across the collector resistor, decreasing the output voltage.

Moreover, the photoresistor circuit uses a 10kohm resistor in a basic voltage divider. With the following relationship, 

V_out = V_cc * R2/(R1+R2)

where R1 is the photoresistor and R2 is the 10kohm resistor, the voltage is directly proportional to the amount of light on the photoresistor. As light increases, resistance decreases, which increases voltage and vice versa. R2 was chosen to approximately equal the impedance of the photoresistor when it is exposed to an average room's lighting environment.
Now that you are getting sensor data and acting on it, why don't you actually try to display the information the user in actual numbers. Using the MSP430FR6989, convert the information from all three of your sensors to a human readable value on the on-board LCD. Fair warning, *DO NOT TRY TO REINVENT THE WHEEL*. Make sure you give the resource explorer a good looking through to see what TI is going to provide you. You can utilize the provided LCDDriver.c and LCDDriver.h files in your code.
