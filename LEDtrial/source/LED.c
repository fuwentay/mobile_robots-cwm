#include "MKL46Z4.h"

//Function LED_init
void LED_init(void) {
//This function configures pin 1 and 2 of Port A as digital outputs.
//Each of these pins will be connected to an LED.
//There are no inputs or outputs.

//1 Enable Port A (without disabling any other Port)
	SIM->SCGC5 |= SIM_SCGC5_PORTA(1);

//2 Connect PTA1 to ALT1 the GPIO module
	PORTA->PCR[1] = PORT_PCR_MUX(1);

//3 Connect PTA2 to ALT1 the GPIO module
	PORTA->PCR[2] = PORT_PCR_MUX(1);

//4 Set PTA1 and PTA2 to be outputs
	GPIOA->PDDR = 1 << 1 |  // pin 1
			      1 << 2;   // pin 2

}

//Function LED
void LED(unsigned char a, unsigned char b) {
//This function is used turn the LEDs on and off.
//There are no outputs and 2 inputs called a and b.

//1 If a is 1
	if (a == 1) {
//2 the LED connected to pin 1 is turned on.
		GPIOA->PCOR = 1 << 1;
	}
//3 else
	else {
//4 the LED connected to pin 1 is turned off.
		GPIOA->PSOR = 1 << 1;
//5 end if
	}
//6 If b is 1
	if (b == 1) {
//7 the LED connected to pin 2 is turned on.
		GPIOA->PCOR = 1 << 2;
	}
//8 else
	else {
//9 the LED connected to pin 2 is turned off.
		GPIOA->PSOR = 1 << 2;
//10 end if
	}
}
