#include "MKL46Z4.h"

void ADCinit(void) {
	/*initialise the ADC at the start of the program */

	/* 1. Enable the clock to the ADC0 module */
	SIM->SCGC6 |= SIM_SCGC6_ADC0(1);

	/* 2. Set the clock speed */
	ADC0->CFG1 = ADC_CFG1_ADIV(0b10);
}

void ADCstart(unsigned int ch) {
	/*Start an ADC conversion
	 ch is the ADC input channel to use */
	/* Write to ADC0_SC1A to start conversion */
	ADC0->SC1[0] = ADC_SC1_ADCH(ch);
}

unsigned char ADCread(void) {
	/* Wait for the ADC conversion to complete
	 * then return the output of the ADC
	 */
	unsigned int coco;
	unsigned char output;

	// #define ADC_SC1_COCO_MASK1 1<<7
	/* Wait for the conversion complete flag to go high */
	do {
		// Mask off the COCO bit and shift it to digit 0 */
		coco = (ADC0->SC1[0] & ADC_SC1_COCO_MASK) >> ADC_SC1_COCO_SHIFT;

	} while (coco == 0);

	// Reads the result of the ADC conversion */
	output = (unsigned char) ADC0->R[0];	// result register ADC0_RA

	//
	return output;
}
