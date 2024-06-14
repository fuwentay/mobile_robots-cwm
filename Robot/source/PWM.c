#include "MKL46Z4.h"

// PWM functions
void PWM_init(void) {

	// Enable PTB
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1);

	// Enable TPM2
	SIM->SCGC6 |= SIM_SCGC6_TPM2(1);

	// Connect TPM2CH0 to PTB18
	PORTB->PCR[18] = PORT_PCR_MUX(3);
	// Connect TPM2CH1 to PTB19
	PORTB->PCR[19] = PORT_PCR_MUX(3);

	// Set the TPM2 Clock Source
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(0b10);

	// Set TPM_SC (TPM status and control)
	TPM2->SC = TPM_SC_CMOD(1)    // use internal clock source
			| TPM_SC_PS(0b110);  // prescale divide by 64

	// Set the period
	TPM2->MOD = 254; // counter counts from 0 to 254, 500 times per second

	//Channel 0 Status and Control
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB(1) // PWM mode
								| TPM_CnSC_MSA(0)
								| TPM_CnSC_ELSB(1)
								| TPM_CnSC_ELSA(0);
	//Channel 1 Status and Control
	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB(1) // PWM mode
								| TPM_CnSC_MSA(0)
								| TPM_CnSC_ELSB(1)
								| TPM_CnSC_ELSA(0);
}

void PWMduty0(unsigned int pw) {
	// Set the duty cycle width
	TPM2->CONTROLS[0].CnV = pw;
}

void PWMduty1(unsigned int pw) {
	// Set the duty cycle width
	TPM2->CONTROLS[1].CnV = pw;
}



/* Motor functions */
void motor_init(void){
	// Initialise PWM
	PWM_init();

	// Enable PTC
	SIM->SCGC5 |= SIM_SCGC5_PORTC(1);

	// Init PTC10
	PORTC->PCR[10] |= PORT_PCR_MUX(1); // set PTC10 to GPIOC
	// Init PTC11
	PORTC->PCR[11] |= PORT_PCR_MUX(1); // set PTC11 to GPIOC

	// set PTC10 and PTC11 as output
	GPIOC->PDDR = 1<<10 | 1<<11;
}

void motor(signed int ls, signed int rs) {
	/* Set the direction of the motor
	 At the same time, make ls positive if needed */
	if (ls >= 0) {
		GPIOC->PSOR = 1 << 10;
	}
	else {
		GPIOC->PCOR = 1 << 10;
		ls = -ls;
	}

	//If ls larger than 255, reduce to 255
	if (ls > 255) {
		ls = 255;
	}

	//Set the PWM duty cycle pulse width
	PWMduty0(ls);

	/* Set the direction of the right motor
	At the same time, make rs positive if needed */
	if (rs >= 0) {
		GPIOC->PSOR = 1 << 11;
	}
	else {
		GPIOC->PCOR = 1 << 11;
		rs = -rs;
	}

	//If rs larger than 255, reduce to 255
	if (rs > 255) {
		rs = 255;
	}

	//Set the PWM duty cycle pulse width
	PWMduty1(rs);
}
