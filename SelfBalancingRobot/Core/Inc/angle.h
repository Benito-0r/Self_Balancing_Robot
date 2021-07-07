/*
 * angle.h
 *
 *  Created on: Jun 22, 2021
 *      Author: Benito
 */

#ifndef INC_ANGLE_H_
#define INC_ANGLE_H_

#include "stdint.h"
float get_Accel_pitch(float ax, float ay, float az);


void means_pitch_error(int16_t buffersize) ;
float get_pitch_accel_error();

float get_pitch_gyro_error();


#endif /* INC_ANGLE_H_ */
