/*
 * joystick.c
 *
 *  Created on: Jun 21, 2021
 *      Author: Alfonso
 */

#include "joystick.h"
#include "adc.h"
#include <stdbool.h>

#define NUMBER_OF_AXIS (1)
#define INDEX_X (0)





joystick_c joystick;

static uint16_t buffer;	//[NUMBER_OF_AXIS];

/**
 * @brief Computes DMA request necessary to read data from joystick
 * @retval Status of the request
 */
int8_t request_data(){
	HAL_StatusTypeDef return_value;
	return_value=HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&buffer, NUMBER_OF_AXIS);
	if (return_value != HAL_OK) {
		return JOYSTICK_ADC_ERR;
	}
	return JOYSTICK_OK;
}



/**
 * @brief Stores data read from ADC in joystick
 */
void save_data(){
	set_x(&joystick,buffer);
}

/**
 * @brief Stores data in specified joystick
 * @param j joystick data structure where store data
 */
void read_data(joystick_c* j){
	set_x(j, get_x(&joystick));
}


/**
 * @brief Sets x value of specified joystick in input
 * @param x x value
 * @param external joystick data structure
 */
void set_x(joystick_c * j, uint16_t x){
	j->x=x;
}

/**
 * @brief Gets x value of specified joystick in input
 * @param external joystick data structure
 */
uint16_t get_x(joystick_c* j){
	return j->x;
}





