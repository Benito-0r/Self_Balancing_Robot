/*
 * mpu6050.c
 *
 *  Created on: Jun 6, 2021
 *      Author: Alfonso
 */

#include "gyro.h"

/**
 * @brief Sets raw data read from gyroscope axes
 * @param g gyroscope data structure
 * @param gx raw data read from gyroscope x axis
 * @param gy raw data read from gyroscope y axis
 * @param gz raw data read from gyroscope z axis
 */
void set_gyro_axes(gyro_t* g, int16_t gx, int16_t gy, int16_t gz){
	g->gyro_x=gx;
	g->gyro_y=gy;
	g->gyro_z=gz;
}

/**
 * @brief Gets raw data read from gyroscope x axis
 * @param g gyroscope data structure
 * @retval raw data read from gyroscope x axis
 */
int16_t get_gx(gyro_t* g){
	return g->gyro_x;
}

/**
 * @brief Gets raw data read from gyroscope y axis
 * @param g gyroscope data structure
 * @retval raw data read from gyroscope y axis
 */
int16_t get_gy(gyro_t* g){
	return g->gyro_y;
}

/**
 * @brief Gets raw data read from gyroscope z axis
 * @param g gyroscope data structure
 * @retval raw data read from gyroscope z axis
 */
int16_t get_gz(gyro_t* g){
	return g->gyro_z;
}
