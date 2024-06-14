#include "MKL46Z4.h"

#include "ADC.h"
unsigned int voltage_l;
unsigned int voltage_r;

// Initialise collision detection mechanism
void collision_init(void) {
	// Init ADC
	ADCinit();

	// Enable PTE
	SIM->SCGC5 |= SIM_SCGC5_PORTE(1);

	// Init PTE16
	PORTE->PCR[16] |= PORT_PCR_MUX(1);
	// Init PTE18
	PORTE->PCR[18] |= PORT_PCR_MUX(1);
}

// Set collision detection indicator to 1 when object detected
int collision_detect(void){

	// initialise channel 1
	ADCstart(1);
	voltage_l = ADCread();	// store voltage reading

	// initialise channel 2
	ADCstart(2);
	voltage_r = ADCread();

	// set flag to 1 if collision is detected
	if ((voltage_l > 70) || (voltage_r > 70)){
		return 1;
	}

	// otherwise set to 0
	return 0;
}
