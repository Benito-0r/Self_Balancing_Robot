/*
 * control_loop.c
 *
 *  Created on: 2 lug 2021
 *      Author: Alfonso
 */

#include "control_loop.h"
#include "mpu6050.h"
#include "angle.h"
#include "PID.h"
#include "motor_driver.h"
#include "stdlib.h"
#include "joystick.h"
#include "stdbool.h"


#define DT 0.005 // time interval of each computation
#define MOVEMENT_PID_FLAG 0
#define STOP_PID_FLAG 1


bool first_read= true; // flag necessary to set reference angle
float ref; // reference angle
float last_angle=0; // last angle read
int8_t prev_dir = STOP; //previous direction
float pitch =0;

/**
 * @brief Manages control law according  direction
 */
void __handle_pitch(){
	int8_t dir;
	int pwm;
	float inc=1.25;
	float inc_for= inc+0.25;
	float inc_stop=0.5;
	dir = get_direction();
	float angle=0;

	// Set reference angle as the first angle read from MPU6050
	if (first_read){
		ref = pitch;
		first_read = false;
	}


	if(dir == STOP){
		if(prev_dir == BACKWARD || pitch<(ref-0.75)){
			// define a lower angle helping to balance the robot when the angle read
			// from MPU is lower then reference angle or previous direction is BACKWARD
			angle = pitch-inc_stop;
			pwm = PID(ref, angle, DT,MOVEMENT_PID_FLAG);
			motorMove(pwm, angle,ref);

		}else if (prev_dir == FORWARD || pitch>(ref+0.75)){
			// define a greater angle helping to balance the robot when the angle read
			// from MPU is greater then reference angle or previous direction is FORWARD
			angle = pitch+inc_stop;
			pwm = PID(ref, angle, DT,MOVEMENT_PID_FLAG);
			motorMove(pwm, angle,ref);
		}else{
			pwm = PID(ref, pitch, DT,STOP_PID_FLAG);
			motorMove(pwm, pitch,ref);
		}

	}

	else if(dir==BACKWARD ){
		// define a lower angle helping to move the robot when the direction is BACKWARD
		angle=pitch-inc;
		pwm = PID(ref, angle, DT,MOVEMENT_PID_FLAG);
		motorMove(pwm, angle,ref);

	}
	else if(dir==FORWARD){
		// define a greater angle helping to balance the robot when the direction is FORWARD
		angle = pitch + inc_for;
		pwm = PID(ref, angle, DT,MOVEMENT_PID_FLAG);
		motorMove(pwm, angle,ref);

	}
	last_angle = angle;
	prev_dir=dir;

//	if(dir == BACKWARD)
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//	else if (dir == FORWARD)
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

}


/**
 * @brief Read data from MPU6050 and compute control law
 */
void execute_control(){
	float ax;
	float gy, ay;
	float az;

	float pitch_gyro = 0, pitch_accel = 0;

	if (MPU6050_retrieve_data() == MPU6050_OK) {
		MPU6050_save_data();
		gy = MPU6050_get_gy()/ SENSITIVITY_GYRO;
		ax = MPU6050_get_ax()/ SENSITIVITY_ACCEL;
		ay = MPU6050_get_ay()/ SENSITIVITY_ACCEL;
		az = MPU6050_get_az()/ SENSITIVITY_ACCEL;

		pitch_accel = get_Accel_pitch(ax, ay, az) - get_pitch_accel_error();
		pitch_gyro = pitch + (gy - get_pitch_gyro_error()) * DT;
		pitch = CF_OFFSET_GYRO * pitch_gyro + CF_OFFSET_ACCEL * pitch_accel;
		__handle_pitch(pitch);
	}
}
