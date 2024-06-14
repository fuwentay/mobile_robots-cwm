/*
 * Copyright (c) 2016, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    MKL46Z256xxx4_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
unsigned char w; // Value of counter global for interrupt

int main(void) {

	unsigned int x; // The entire data in register
	unsigned int mask = 1 << 1; // The mask used to extract bit 1
	unsigned int c; // The value of PTC1

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();

	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	// Enable Port C
	SIM->SCGC5 |= SIM_SCGC5_PORTC(1);

	// Connect pin PTC1 to ALT1, the GPIO module.
	PORTC->PCR[1] = PORT_PCR_MUX(1)
			| PORT_PCR_PS(1)
			| PORT_PCR_PE(1) // Enable pull-up resistor
			| PORT_PCR_IRQC(0b1010); // Enable falling edge interrupt

	//Enable IRQ31 in the NVIC.
	NVIC->ISER[0] = 1 << 31;

	//Enable interrupts in the processor.
	__enable_irq();

	// Initialise the Liquid Crystal Display
	LCD_init();

	/* Enter an infinite loop. */
	while (1) {
		x = GPIOC->PDIR; // Read in Port C data

		// Mask off bit 1 of Port C
		c = x & mask;

		if (c == 0) // If PTC1 is low
				{
			if (w >= 100) // When get to 100
					{
				w = 0; // Reset to zero
			}
		}

		LCDNumberLeft(w); // Display w on LCD
	}
	return 0;
}

// The interrupt service routine (ISR) function:
void PORTC_PORTD_IRQHandler(void) {

	// Clear interrupt flag
	PORTC->PCR[1] |= PORT_PCR_ISF(1);

	w++; // increase counter by 1
}
