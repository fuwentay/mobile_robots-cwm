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
 * @file    PWMtest.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"

/* TODO: insert other include files here. */
#include "PWM.h"
#include "delay.h"

/* TODO: insert other definitions and declarations here. */
unsigned int dutyL = 128; // set the width of duty cycle of the left motor
unsigned int dutyR = 64; // set the width of duty cycle of the right motor
signed int lspeed;
signed int rspeed;
unsigned long d;

/*
 * @brief   Application entry point.
 */
int main(void) {

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	//Initialise the PWM module
	PWM_init();
	//Initialise the motor
	motor_init();

//	//loop forever
//	while (1) {
//		//Set the duty cycle
//		motor(lspeed, rspeed);
//	}

	//loop forever
	while (1) {
		//Set the duty cycle
		motor(lspeed, rspeed);
		delay(1E7);				// 2s pause
		motor(lspeed, 0);
		delay(1E7);				// 2s pause
		motor(0, rspeed);
		delay(1E7);				// 2s pause
		motor(-lspeed, -rspeed);
		delay(1E7);				// 2s pause
		motor(0, 0);
		delay(2E7);				// 4s pause
	}

	return 0;
}
