/*
 * PID.c
 *
 *  Created on: 22 giu 2021
 *      Author: Ettore
 */

#include "PID.h"
#include <stdlib.h>


#define Kp 17
#define Kd 5
#define Ki 4

#define MAX_PWM 70
#define MIN_PWM -MAX_PWM


float lastError = 0;
float I = 0;

/**
 * @brief Computer PID control law
 * @param ref reference angle
 * @param pitch	pitch read from MPU6050
 * @param dt interval time between two MPU6050 read
 * @param pid_flag use a PID control law if this flag is 1 else use a PI law
 * @retval pwm compute from control law
 */
int PID(float ref, float pitch, float dt, uint8_t pid_flag) {
	float P, D, pid_pwm;

	//calculate error
	float error = ref - pitch;

	//calculate Proportional term
	P = Kp * error;

////	calculate Integral term. Account for wind-up
	if(pid_flag==1)
		I+=Ki* error ;
	else
		I+=0.5*error; // If the robot has to move the Ki term should be lower so there are less oscillation

	if (I > MAX_PWM)
		I = MAX_PWM;
	else if (I<MIN_PWM){
		I=MIN_PWM;
	}

	////calculate Derivative term
	D = -Kd * (error - lastError);

	// If the robot has to move the control low is PI so the movement is more fluid
	if(pid_flag == 0){
		D = 0;
	}

	//total PID value
	pid_pwm = P + I + D;

	//max sure pwm is bound between allowed min/max thresholds

	int out_pwm = (int) pid_pwm;
	if (pid_pwm > MAX_PWM)
		out_pwm = MAX_PWM;
	else if (pid_pwm < MIN_PWM)
		out_pwm = MIN_PWM;

	lastError = error;

	return out_pwm;

}
