#include "MKL46Z4.h"

#include "delay.h"
#include "ADC.h"

#define CLOCK 1 	// The camera clock
#define SI 2     	// The camera sync pulse.
#define TRIG 4   	// Output to trigger the scope
#define Ulimit 160 	// Upper limit to max ADC reading
#define Blimit 150  // Bottom limit to max ADC reading

unsigned int n = 28; 		// arbitrary value required for delay of 8 microseconds
unsigned char CCD[64]; 		// Array for storing CCD output
unsigned int itime = 100;  	// integration time delay
int maxVal; 				// max in array

void camera_init(void) {
	// Init ADC
	ADCinit();

	// Enable PTB
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1);

	// Set PTB0 and PTB1 as GPIO digital outputs

	// Init PTB0
	PORTB->PCR[0] |= PORT_PCR_MUX(1); // set PTB0 to GPIOB
	// Init PTB1
	PORTB->PCR[1] |= PORT_PCR_MUX(1); // set PTB1 to GPIOB
	// Init PTB2
	PORTB->PCR[2] |= PORT_PCR_MUX(1); // set PTB2 to GPIOB

	// set PTB0, PTB1, PTB2 as output
	GPIOB->PDDR = CLOCK | SI | TRIG;
}

void camera_true(void) {
	GPIOB->PSOR = SI | TRIG;		// set SI and TRIG signal high
	delay(n);
	GPIOB->PSOR = CLOCK;	        // set clock high
	delay(n);
	delay(n);
	GPIOB->PCOR = SI | TRIG;		// set SI and TRIG signal low
	delay(n);
	GPIOB->PCOR = CLOCK;	        // set clock low
	delay(n);
	for (int i = 0; i < 64; i++) {
		/* Start the ADC conversion */
		ADCstart(0);
		/* Read in the data from the ADC */
		CCD[i] = ADCread();
		GPIOB->PSOR = CLOCK;	// set clock high
		delay(n);
		GPIOB->PCOR = CLOCK;	// set clock low
		delay(n);
	}
}

void camera_dummy(void) {
	unsigned char localarray[64];

	GPIOB->PSOR = SI;		// set SI signal high
	delay(n);
	GPIOB->PSOR = CLOCK;	// set clock high
	delay(n);
	delay(n);
	GPIOB->PCOR = SI;		// set SI signal low
	delay(n);
	GPIOB->PCOR = CLOCK;	// set clock low
	delay(n);
	for (int i = 0; i < 64; i++) {
		/* Start the ADC conversion */
		ADCstart(0);
		/* Read in the data from the ADC */
		localarray[i] = ADCread();
		GPIOB->PSOR = CLOCK;	// set clock high
		delay(n);
		GPIOB->PCOR = CLOCK;	// set clock low
		delay(n);
	}
}

int max(unsigned char arr[64]) {

	// initialise maxVal
	maxVal = arr[0];

	for (int i = 0; i < 64; i++) {
		if (arr[i] > maxVal) {
			maxVal = arr[i];
		}
	}
	return maxVal;
}

void camera_read(void) {

	camera_dummy(); 	// Clear accumulated output

	delay(itime); 		// Integration time

	camera_true(); 		// Read camera output into array.

	maxVal = max(CCD); 	// Find the maximum in the true read

//	/* Update itime using maxVal */
	if (maxVal > Ulimit) {
		itime *= 9;
		itime /= 10;
		if (itime == 0) {
			itime = 1;
		}
	}
	if (maxVal < Blimit) {
		itime *= 11;
		itime /= 10;
		if (itime < 10) {
			itime++;
		}
	}
}

void camera_process(unsigned char *l, unsigned char *r, signed char *dist, unsigned int *lturn, unsigned int *rturn) {
	// Initialise
	int threshold = 80;
	*r = 0;
	*l = 0;

	// get data
	camera_read();

	// if the far left pixels are black, turn left
	for (int x = 0; x < 5; x++) {
		if (CCD[x] < threshold) {
			*lturn = 1;
			*rturn = 0;
		}
	}

	// if the far right pixels are black, turn right
	for (int x = 59; x < 64; x++) {
		if (CCD[x] < threshold) {
			*rturn = 1;
			*lturn = 0;
		}
	}

	// count black pixels to the left
	for (int x = 0; x < 32; x++) {
		if (CCD[x] < threshold) {
			(*l)++;
		}
	}

	// count black pixels to the right
	for (int x = 32; x < 64; x++) {
		if (CCD[x] < threshold) {
			(*r)++;
		}
	}

	// calculate dist
	*dist = *l - *r;
}
