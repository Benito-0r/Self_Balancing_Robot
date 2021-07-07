/*
 * MPU6050.h
 *
 *  Created on: Jun 6, 2021
 *      Author: Alfonso
 */

#ifndef INC_GYRO_H_
#define INC_GYRO_H_

#include <stdint.h>

struct gyro_s{
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
};

typedef struct gyro_s gyro_t;


void set_gyro_axes(gyro_t* g, int16_t gx, int16_t gy, int16_t gz);

int16_t get_gx(gyro_t* g);
int16_t get_gy(gyro_t* g);
int16_t get_gz(gyro_t* g);


#endif /* INC_GYRO_H_ */
