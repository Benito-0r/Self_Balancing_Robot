/*
 * accel.c
 *
 *  Created on: Jun 8, 2021
 *      Author: Alfonso
 */


#include "accel.h"


/**
 * @brief Sets raw data read from accelerometer axes
 * @param a accelerometer data structure
 * @param ax raw data read from accelerometer x axis
 * @param ay raw data read from accelerometer y axis
 * @param az raw data read from accelerometer z axis
 */
void set_accel_axes(accel_t* a, int16_t ax, int16_t ay, int16_t az){
	a->ax=ax;
	a->ay=ay;
	a->az=az;
}

/**
 * @brief Gets raw data read from  accelerometer x axis
 * @param a accelerometer data structure
 * @retval raw data read from accelerometer x axis
 */
int16_t get_ax(accel_t* a){
	return a->ax;
}

/**
 * @brief Gets raw data read from  accelerometer y axis
 * @param a accelerometer data structure
 * @retval raw data read from accelerometer y axis
 */
int16_t get_ay(accel_t* a){
	return a->ay;
}

/**
 * @brief Gets raw data read from  accelerometer z axis
 * @param a accelerometer data structure
 * @retval raw data read from accelerometer z axis
 */
int16_t get_az(accel_t* a){
	return a->az;
}

