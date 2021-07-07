/*
 * PID.h
 *
 *  Created on: 22 giu 2021
 *      Author: Ettore
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

int PID(float ref, float pitch, float dt, uint8_t flag);

#endif /* INC_PID_H_ */
