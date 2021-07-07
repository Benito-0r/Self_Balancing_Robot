/*
 * joystick.h
 *
 *  Created on: Jun 21, 2021
 *      Author: Alfonso
 */

#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_

#include <stdint.h>

#define JOYSTICK_ADC_ERR (-2)
#define JOYSTICK_OK (0)


struct joystick_s {
	uint16_t x;
};

typedef struct joystick_s joystick_c;



int8_t request_data();
void save_data();
void read_data();
uint16_t get_x(joystick_c* j);

void set_x(joystick_c * j, uint16_t x);



#endif /* INC_JOYSTICK_H_ */
