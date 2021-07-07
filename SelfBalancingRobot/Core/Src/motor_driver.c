/*
 * motor_driver.c
 *
 *  Created on: Jun 21, 2021
 *      Author: Alfonso
 */

#include "motor_driver.h"
#include "tim.h"
#include "stdlib.h"
#include "joystick.h"

//hysteresis bounds
#define UPPER_BOUND_H (200)
#define LOWER_BOUND_H (180)
#define UPPER_BOUND_L (74)
#define LOWER_BOUND_L (40)

int8_t prev_status = STOP ;
int8_t status =  STOP;
joystick_c j;


/**
 * @brief management of the phenomenon of hysteresis
 * @retval BACKWARD, STOP or FORWARD
 */
int8_t __handler(uint8_t value){

	if(value>=LOWER_BOUND_H && value<UPPER_BOUND_H){
		if (prev_status == STOP || prev_status == FORWARD){
			status = prev_status;
		}else if(prev_status == BACKWARD){
			status = STOP;
		}
	}else if (value>=LOWER_BOUND_L && value < UPPER_BOUND_L){
		if (prev_status == STOP || prev_status == BACKWARD){
			status = prev_status;
		}else if( prev_status == FORWARD){
			status = STOP;
		}
	}else if (value >= UPPER_BOUND_H)
		status = FORWARD;
	else if (value <= LOWER_BOUND_L)
		status = BACKWARD;
	else if (value < LOWER_BOUND_H && value > UPPER_BOUND_L)
		status = STOP;
	return status;
}

/**
 * @brief Get joystick direction
 * @retval BACKWARD, STOP or FORWARD
 */
int8_t get_direction(){
	request_data();
	read_data(&j);
	__handler(get_x(&j));
	prev_status = status;
	return status;
}




/**
 * @brief Handles motor power and direction
 * @param PWM PID controller output
 * @param pitch angle compute through  MPU6050
 * @param ref angle set point of balancing robot
 */
void motorMove(int PWM, float pitch, float ref) {
	int8_t direction;
	uint16_t speed = abs(PWM);

	// Direction is based on angle read from MPU6050
	if ( pitch > ref){
		direction = FORWARD;
	} else if (pitch < ref){
		direction = BACKWARD ;
	}


	if (direction == FORWARD) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, speed);

	} else if (direction == BACKWARD) {

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, speed);

	} else if (direction == STOP) {
		__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 0);

	}

}




