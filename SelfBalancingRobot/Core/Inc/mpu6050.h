/*
 * mpu6050.h
 *
 *  Created on: Jun 6, 2021
 *      Author: Alfonso
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_



#include <stdint.h>
#include "gyro.h"
#include "accel.h"

//Define of registers
#define ADDRESS_SIZE				(1)
#define DATA_BUFFER_SIZE			(14)

#define MPU6050_ADDRESS_AD0_LOW     (0x68) // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    (0x69) // address pin high (VCC)
#define MPU6050_DEFAULT_ADDRESS     (0xD0)//MPU6050_ADDRESS_AD0_LOW

#define MPU6050_REG_ACCEL_XOFFS_H     (0x06)
#define MPU6050_REG_ACCEL_XOFFS_L     (0x07)
#define MPU6050_REG_ACCEL_YOFFS_H     (0x08)
#define MPU6050_REG_ACCEL_YOFFS_L     (0x09)
#define MPU6050_REG_ACCEL_ZOFFS_H     (0x0A)
#define MPU6050_REG_ACCEL_ZOFFS_L     (0x0B)

#define MPU6050_REG_GYRO_XOFFS_H      (0x13)
#define MPU6050_REG_GYRO_XOFFS_L      (0x14)
#define MPU6050_REG_GYRO_YOFFS_H      (0x15)
#define MPU6050_REG_GYRO_YOFFS_L      (0x16)
#define MPU6050_REG_GYRO_ZOFFS_H      (0x17)
#define MPU6050_REG_GYRO_ZOFFS_L      (0x18)

#define MPU6050_SMPRT_DIV             (0x19)
#define MPU6050_REG_CONFIG            (0x1A)
#define MPU6050_REG_GYRO_CONFIG       (0x1B) // Gyroscope Configuration
#define MPU6050_REG_ACCEL_CONFIG      (0x1C) // Accelerometer Configuration
/*
#define MPU6050_REG_FF_THRESHOLD      (0x1D)
#define MPU6050_REG_FF_DURATION       (0x1E)
#define MPU6050_REG_MOT_THRESHOLD     (0x1F)
#define MPU6050_REG_MOT_DURATION      (0x20)
#define MPU6050_REG_ZMOT_THRESHOLD    (0x21)
#define MPU6050_REG_ZMOT_DURATION     (0x22)
*/
#define MPU6050_REG_INT_PIN_CFG       (0x37) // INT Pin. Bypass Enable Configuration
#define MPU6050_REG_INT_ENABLE        (0x38) // INT Enable
#define MPU6050_REG_INT_STATUS        (0x3A)
#define MPU6050_REG_ACCEL_XOUT_H      (0x3B)
#define MPU6050_REG_ACCEL_XOUT_L      (0x3C)
#define MPU6050_REG_ACCEL_YOUT_H      (0x3D)
#define MPU6050_REG_ACCEL_YOUT_L      (0x3E)
#define MPU6050_REG_ACCEL_ZOUT_H      (0x3F)
#define MPU6050_REG_ACCEL_ZOUT_L      (0x40)
/*
#define MPU6050_REG_TEMP_OUT_H        (0x41)
#define MPU6050_REG_TEMP_OUT_L        (0x42)
*/
#define MPU6050_REG_GYRO_XOUT_H       (0x43)
#define MPU6050_REG_GYRO_XOUT_L       (0x44)
#define MPU6050_REG_GYRO_YOUT_H       (0x45)
#define MPU6050_REG_GYRO_YOUT_L       (0x46)
#define MPU6050_REG_GYRO_ZOUT_H       (0x47)
#define MPU6050_REG_GYRO_ZOUT_L       (0x48)
/*
#define MPU6050_REG_MOT_DETECT_STATUS (0x61)
#define MPU6050_REG_MOT_DETECT_CTRL   (0x69)
*/
#define MPU6050_REG_USER_CTRL         (0x6A) // User Control
#define MPU6050_REG_PWR_MGMT_1        (0x6B) // Power Management 1
#define MPU6050_REG_WHO_AM_I          (0x75) // Who Am I

#define FS_SCALE (0)
#if FS_SCALE==0
#define SENSITIVITY_GYRO (131.0f)
#elif FS_SCALE==1
#define SENSITIVITY_GYRO (65.5f)
#elif FS_SCALE==2
#define SENSITIVITY_GYRO (32.8f)
#elif FS_SCALE==3
#define SENSITIVITY_GYRO (16.4f)
#endif

#define AFS_SCALE (0)
#if AFS_SCALE==0
#define SENSITIVITY_ACCEL (16384.0f)
#elif AFS_SCALE==1
#define SENSITIVITY_ACCEL (8192.0f)
#elif AFS_SCALE==2
#define SENSITIVITY_ACCEL (4096.0f)
#elif AFS_SCALE==3
#define SENSITIVITY_ACCEL (2048.0f)
#endif

//#define CF_OFFSET_GYRO (0.96)
#define CF_OFFSET_GYRO (0.98)
#define CF_OFFSET_ACCEL (1-CF_OFFSET_GYRO)

#define SAMPLE_RATE_DIVIDER (0)

#define MPU6050_OK 		(0)
#define MPU6050_ERR		(-1)
#define MPU6050_I2C_ERR	(-2)






struct mpu6050_data_s
{
	accel_t a;
	gyro_t g;
};

typedef struct mpu6050_data_s mpu6050_data_t;



//Functions
int8_t MPU6050_init();
int8_t MPU6050_gyro_set_full_scale_range(uint8_t fullscalerange);
int8_t MPU6050_accel_set_full_scale_range(uint8_t fullscalerange);
int8_t MPU6050_set_rate_divider(uint8_t sample_rate_divider);
int8_t MPU6050_retrieve_data();
void MPU6050_save_data();
int8_t MPU6050_read_data(mpu6050_data_t* mpu);
int8_t MPU6050_configuration();
int8_t MPU6050_DLPF_configuration() ;
int8_t MPU6050_who_I_am();


int16_t MPU6050_get_gx();
int16_t MPU6050_get_gy();
int16_t MPU6050_get_gz();
int16_t MPU6050_get_ax();
int16_t MPU6050_get_ay();
int16_t MPU6050_get_az();

int16_t get_AccelX_offset();
int16_t get_AccelY_offset();
int16_t get_AccelZ_offset();
void set_Accel_offset(uint16_t offset_x,uint16_t offset_y,uint16_t offset_z);

int16_t get_GyroX_offset();
int16_t get_GyroY_offset();
int16_t get_GyroZ_offset();
void set_Gyro_offset(uint16_t offset_x,uint16_t offset_y,uint16_t offset_z);


//int16_t calibration(int16_t *gx_offset, int16_t *gy_offset, int16_t *gz_offset);

//void insert_mpu_in_buffer(mpu6050_data_t mpu);
//mpu6050_data_t read_from_buffer();






#endif /* INC_MPU6050_H_ */
