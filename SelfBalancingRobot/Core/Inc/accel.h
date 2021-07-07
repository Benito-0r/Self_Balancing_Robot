/*
 * accel.h
 *
 *  Created on: Jun 8, 2021
 *      Author: Alfonso
 */

#ifndef INC_ACCEL_H_
#define INC_ACCEL_H_

#include <stdint.h>

struct accel_s{
	int16_t ax;
	int16_t ay;
	int16_t az;
};

typedef struct accel_s accel_t;


void set_accel_axes(accel_t* a, int16_t ax, int16_t ay, int16_t az);

int16_t get_ax(accel_t* a);
int16_t get_ay(accel_t* a);
int16_t get_az(accel_t* a);

#endif /* INC_ACCEL_H_ */
