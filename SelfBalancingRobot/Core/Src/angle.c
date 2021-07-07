/*
 * angle.c
 *
 *  Created on: Jun 22, 2021
 *      Author: Benito
 */

#include "angle.h"
#include "mpu6050.h"
#include "math.h"
#include <stdlib.h>
#include "stm32f4xx_hal.h"


static float pitch_accel_error = 0, pitch_gyro_error = 0;

/**
 * @brief Gets accelerometer pitch computes from ax,ay,az
 * @param ax acceleration computes from x axis
 * @param ay acceleration computes from y axis
 * @param az acceleration computes from z axis
 * @retval pitch angle computer from accelerometer values
 *
 */
float get_Accel_pitch(float ax, float ay, float az) {
	return atan2(ax, sqrt(ay*ay + az*az)) * (180 / M_PI);
}



/**
 * @brief Computes averages error on pitch angle and gyroscope pitch angle
 * @param buffersize number of read on which to average
 */
void means_pitch_error( int16_t buffersize) {
	int i = 0;
	float ax = 0, ay = 0, az = 0, gy = 0;
	int discardfirstmeas = 100;
	//float pitch = 0;
	while (i < (buffersize + discardfirstmeas + 1)) {

		// read raw accel/gyro measurements from device
		if (MPU6050_retrieve_data() == MPU6050_OK) {
			MPU6050_save_data();

			gy = MPU6050_get_gy()/ SENSITIVITY_GYRO;

			ax = MPU6050_get_ax()/ SENSITIVITY_ACCEL;
			ay = MPU6050_get_ay()/ SENSITIVITY_ACCEL;
			az = MPU6050_get_az()/ SENSITIVITY_ACCEL;


		}

		if (i > discardfirstmeas && i <= (buffersize + discardfirstmeas)) { //First 100 measures are discarded

			pitch_accel_error += get_Accel_pitch(ax, ay, az);
			pitch_gyro_error += gy;

		}
		if (i == (buffersize + discardfirstmeas)) {

			pitch_gyro_error /= buffersize;
			pitch_accel_error /= buffersize;
		}
		i++;
		HAL_Delay(2); //time delay shouldn't be too large or too small
	}

}

/**
 * @brief Gets pitch accelerometer error
 * @retval pitch accelerometer error
 */
float get_pitch_accel_error(){
	return pitch_accel_error;
}

/**
 * @brief Gets pitch gyroscope error
 * @retval pitch gyroscope error
 */
float get_pitch_gyro_error(){
	return pitch_gyro_error;
}



