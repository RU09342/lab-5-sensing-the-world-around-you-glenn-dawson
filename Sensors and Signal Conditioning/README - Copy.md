# Signal Conditioning for MSP430 ADC
The included program is compatible with the MSP430G2553. The program is designed to take input from analog devices, namely the OP0805SL, the LM35 and a photoresistor, and convert it to a digital representation. This is stored in the ADC register that the program configures and communicates this data to a local UART terminal.

## Dependencies
This program depends only on having the G2553-specific header file, which is included in the main file. This file is the MSP430.h file included at the beginning of the main file.

## Usage
To use this program, configure the circuit shown in the picture included (OP0805SL.JPG). This circuit uses a phototransistor and its collector voltage as input to the ADC. The voltage can be increased by shielding the phototransistor from light and decreased by shining intense light on the phototransistor. These voltage changes are reflected by the values held in ADC10MEM0. One can also use the circuit shown in photoresistor.JPG. The same process holds true for this circuit.

The LM35 uses its own program to display the temperature over UART. Using the transfer function provided by the manufacturer, the program converts the ADC contents to the temperature in Celcius. The conversion is as follows: 

	temp = ADC10MEM;
    IntDegC = (1.5 * temp) * 100 / 1024;
	
Again, this is derived from the transfer function: V = 10mV/C * T. The circuit simply wires the pins on the LM35 to the MCU: Vss to 3.3V, GND to the MCU's GND pin and V_out to P1.4, which is configured as the ADC input.

## ADC Configuration
Although the code included contains its own documentation, the following will provide an overview of how the ADC is configured.

  ADC10CTL1 = INCH_4 + ADC10DIV_6;         // VREF, ADC10CLK/4
  ADC10CTL0 = SREF_1 + ADC10SHT_2 + ADC10ON + REFON + REF2_5V + ADC10IE;          

This code configures the ADC to use channel A4, located on P1.4, with a clock divider = 6. Moreover, the ADC is set to use VREF, which is configured as 2.5V. Lastly, the ADC interrupt is enabled to allow the MCU to transmit updated ADC data.

## How do these circuits work?
The circuit which uses the phototransistor is commonly known as a common-emitter amplifier, meaning that the emitter of transistor is grounded. In this configuration, V_out is taken at the collector of the transistor.

The amount of base current is proportional to the amount of light that the phototransistor receives. By controlling the base current, the transistor's collector current also changes accordingly. The increase in current creates a greater voltage drop across the collector resistor, decreasing the output voltage.

Moreover, the photoresistor circuit uses a 10kohm resistor in a basic voltage divider. With the following relationship, 

V_out = V_cc * R2/(R1+R2)

where R1 is the photoresistor and R2 is the 10kohm resistor, the voltage is directly proportional to the amount of light on the photoresistor. As light increases, resistance decreases, which increases voltage and vice versa. R2 was chosen to approximately equal the impedance of the photoresistor when it is exposed to an average room's lighting environment.