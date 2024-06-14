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
 * @file    Robot.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"

/* TODO: insert other include files here. */
#include "delay.h"
#include "ADC.h"
#include "PWM.h"
#include "camera.h"
#include "LCD.h"
#include "LED.h"
#include "collision.h"

/* TODO: insert other definitions and declarations here. */
unsigned char left; // black pixels on left
unsigned char right; // black pixels on right
signed char dist; // difference between black pixels on left and right
unsigned int k = 12; // gain
unsigned int lturn = 0; // left turn indicator
unsigned int rturn = 0; // right turn indicator
unsigned int object_detected; // object detected indicator
void collision_avoidance(void);

/*
 * @brief   Application entry point.
 */
int main(void) {

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	// Initialise
	camera_init();
	motor_init();
	LCD_init();
	LED_init();
	collision_init();

	while (1) {
		collision_avoidance();

		//Read and process the information from the camera to get the first dist
		camera_process(&left, &right, &dist, &lturn, &rturn);
		// Display values on LCD
		LCDNumberLeft(left);
		LCDNumberRight(right);

		// stop and turn if (0,0)
		while ((left == 0) && (right == 0)) {
			// collision_avoidance();

			// Decide direction of turning
			if (lturn == 1) {
				motor(-255, 255); // turn left
//				camera_process(&left, &right, &dist, &lturn, &rturn);
//				// Display values on LCD
//				LCDNumberLeft(left);
//				LCDNumberRight(right);
						// turn on the green LED on the left
				LED(1, 0);
			}

			// else turn right
			if (rturn == 1) {
				motor(255, -255); // turn right
//				camera_process(&left, &right, &dist, &lturn, &rturn);
//				// Display values on LCD
//				LCDNumberLeft(left);
//				LCDNumberRight(right);
						// turn on the green LED on the right
				LED(0, 1);
			}
			// reset LED
			LED(0, 0);

			// read camera
			camera_process(&left, &right, &dist, &lturn, &rturn);
			// Display values on LCD
			LCDNumberLeft(left);
			LCDNumberRight(right);
		}

		// turning left
		while (dist > 5) {
			collision_avoidance();

			// line is entirely on the left, switch to bang bang
			if (right == 0) {
				motor(0, 255);
//				camera_process(&left, &right, &dist, &lturn, &rturn);
//				// Display values on LCD
//				LCDNumberLeft(left);
//				LCDNumberRight(right);
//				// turn on the green LED on the left
//				LED(1, 0);
			} else {
				// else use proportional control
				motor(255 - k * dist, 255);
				// get camera reading
//				camera_process(&left, &right, &dist, &lturn, &rturn);
//				// Display values on LCD
//				LCDNumberLeft(left);
//				LCDNumberRight(right);
//				// turn on the green LED on the left
//				LED(1, 0);
			}
			camera_process(&left, &right, &dist, &lturn, &rturn);
			// Display values on LCD
			LCDNumberLeft(left);
			LCDNumberRight(right);
			// turn on the green LED on the left
			LED(1, 0);
		}
		// reset LED
		LED(0, 0);

		// turning right
		while (dist < -5) {
			collision_avoidance();

			if (left == 0) {
				// line is entirely on the right, switch to bang bang
				motor(255, 0);
//				// get camera reading
//				camera_process(&left, &right, &dist, &lturn, &rturn);
//				// Display values on LCD
//				LCDNumberLeft(left);
//				LCDNumberRight(right);
//				// turn on the green LED on the right
//				LED(0, 1);
			} else {
				// else use proportional control
				motor(255, 255 + k * dist);
//				camera_process(&left, &right, &dist, &lturn, &rturn);
//				// Display values on LCD
//				LCDNumberLeft(left);
//				LCDNumberRight(right);
//				// turn on the green LED on the right
//				LED(0, 1);
			}
			// get camera reading
			camera_process(&left, &right, &dist, &lturn, &rturn);
			// Display values on LCD
			LCDNumberLeft(left);
			LCDNumberRight(right);
			// turn on the green LED on the right
			LED(0, 1);
		}
		// reset LED
		LED(0, 0);

		motor(255, 255);
	}

}

void collision_avoidance(void) {
	// initialise collision flags
	int object_detected = collision_detect(); // flag for detecting an object ahead
	int turn_flag; 								// flag for a 180 deg turn

	if (object_detected == 1) {
		for (int i = 0; i < 100000; i++) {	// Reverse when object detected
			motor(-255, -255);
		}
		turn_flag = 1; // set turn flag
	}

	// When object is detected
	while (object_detected == 1) {

		for (int i = 0; i < 300000; i++) {
			motor(-255, 255); // Turn robot until it no longer sees any black pixels
		}
		// update collision detection status
		object_detected = collision_detect();
	}

	// get left and right
	camera_process(&left, &right, &dist, &lturn, &rturn);
	// Display values on LCD
	LCDNumberLeft(left);
	LCDNumberRight(right);

	// When there are no black pixels, turn until it finds the line
	while ((left == 0) && (right == 0) && (turn_flag == 1)) {
		motor(-255, 255);
		camera_process(&left, &right, &dist, &lturn, &rturn);
		// Display values on LCD
		LCDNumberLeft(left);
		LCDNumberRight(right);
	}
	// clear the turn flag
	turn_flag = 0;

}
