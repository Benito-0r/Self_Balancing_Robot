/*
 * motor_driver.h
 *
 *  Created on: Jun 21, 2021
 *      Author: Alfonso
 */

#ifndef INC_MOTOR_DRIVER_H_
#define INC_MOTOR_DRIVER_H_

#include <stdint.h>

#define STOP (0)
#define FORWARD (1)
#define BACKWARD (-1)


int8_t get_direction();
void motorMove(int PidValue,float pitch, float ref);

#endif /* INC_MOTOR_DRIVER_H_ */
