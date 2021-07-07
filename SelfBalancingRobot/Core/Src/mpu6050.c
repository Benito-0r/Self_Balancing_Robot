/*
 * mpu6050.c
 *
 *  Created on: Jun 6, 2021
 *      Author: Alfonso
 */

#include "mpu6050.h"
#include "i2c.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>

mpu6050_data_t mpu_t; // maintains data read from gyroscope and accelerometer
uint8_t in_buff[DATA_BUFFER_SIZE];//buffer stored data od size DATA_BUFFER_SIZE read from I2C

//mpu6050_data_t mpu_buffer; // buffer of mpu used in this software

//float pitch_accel_error,pitch_gyro_error;
//Functions
//Functions of MPU6050

/**
 * @brief Utils function used to read data in blocking mode from MPU6050
 * @param reg starting register from which read data
 * @param buffer where are stored read data
 * @param size amount of data to read
 * @retval MPU6050 read status
 */
int8_t __MPU6050_read(uint8_t reg, uint8_t *buffer, uint8_t size) {
	HAL_StatusTypeDef returnValue;
	returnValue = HAL_I2C_Mem_Read(&hi2c2, MPU6050_DEFAULT_ADDRESS, reg,
	ADDRESS_SIZE, buffer, size, 100);
	if (returnValue != HAL_OK) {
		return MPU6050_I2C_ERR;
	}
	return MPU6050_OK;
}

/**
 * @brief Utils function used to write data in blocking mode into MPU6050
 * @param reg starting register from which write data
 * @param buffer where are stored data to write
 * @param size amount of data to write
 * @retval MPU6050 write status
 */
int8_t __MPU6050_write(uint8_t reg, uint8_t *buffer, uint8_t size) {
	HAL_StatusTypeDef returnValue;
	returnValue = HAL_I2C_Mem_Write(&hi2c2, MPU6050_DEFAULT_ADDRESS, reg,
	ADDRESS_SIZE, buffer, size, 100);
	if (returnValue != HAL_OK) {
		return MPU6050_I2C_ERR;
	}
	return MPU6050_OK;
}

/**
 * @brief MPU6050 initialization function.
 * @retval MPU6050 initialization status
 */
int8_t MPU6050_init() {
	HAL_StatusTypeDef returnValue;
	returnValue = HAL_I2C_IsDeviceReady(&hi2c2, MPU6050_DEFAULT_ADDRESS, 3,
			1000);
	if (returnValue != HAL_OK) {
		return MPU6050_ERR;
	}

	if (MPU6050_configuration() == MPU6050_ERR ||
		MPU6050_set_rate_divider(SAMPLE_RATE_DIVIDER) == MPU6050_ERR ||
		MPU6050_gyro_set_full_scale_range(FS_SCALE) == MPU6050_ERR ||
		MPU6050_accel_set_full_scale_range(AFS_SCALE) == MPU6050_ERR ||
		MPU6050_DLPF_configuration() == MPU6050_ERR)
		return MPU6050_ERR;
	else
		return MPU6050_OK;

//	return_value = MPU6050_configuration();
//	return_value = MPU6050_set_rate_divider(SAMPLE_RATE_DIVIDER); //Set divider of gyroscope sammple rato to zero
//	return_value = MPU6050_gyro_set_full_scale_range(FS_SCALE);
//	return_value = MPU6050_accel_set_full_scale_range(AFS_SCALE);
//	return_value = MPU6050_DLPF_configuration();

//	return MPU6050_OK;
}

/**
 * @brief Configures gyroscope full scale range
 * @retval Status of gyroscope full_scale_range configuration
 */
int8_t MPU6050_gyro_set_full_scale_range(uint8_t FS_SEL) {
	if (FS_SEL > 3) {
		return MPU6050_ERR;
	}
	uint8_t buffer;
	__MPU6050_read(MPU6050_REG_GYRO_CONFIG, &buffer, sizeof(buffer)); // gyro_ref_conf_value must be like -> 000xx000 where xx=FS_SEL
	buffer = buffer & ((FS_SEL << 3) | 0x07);
	return __MPU6050_write(MPU6050_REG_GYRO_CONFIG, &buffer, sizeof(buffer));

}

/**
 * @brief Configures accelerometer full scale range
 * @retval Status of accelerometer full_scale_range configuration
 */
int8_t MPU6050_accel_set_full_scale_range(uint8_t AF_SEL) {
	if (AF_SEL > 3) {
		return MPU6050_ERR;
	}

	uint8_t buffer;
	__MPU6050_read(MPU6050_REG_ACCEL_CONFIG, &buffer, sizeof(buffer)); // accel_ref_conf_value must be like -> 000xx000 where xx=AF_SEL
	buffer = buffer & ((AF_SEL << 3) | 0x07);
	return __MPU6050_write(MPU6050_REG_ACCEL_CONFIG, &buffer, sizeof(buffer));

}
/**
 * @brief Sets divider of gyroscope sample rate
 * @retval Status of this operation
 */
int8_t MPU6050_set_rate_divider(uint8_t sample_rate_divider) {
	return __MPU6050_write(MPU6050_SMPRT_DIV, &sample_rate_divider,
			sizeof(sample_rate_divider));
}


int8_t MPU6050_who_I_am() {
	HAL_StatusTypeDef returnValue;
	uint8_t value;
	returnValue = HAL_I2C_Mem_Read(&hi2c2, MPU6050_DEFAULT_ADDRESS,
	MPU6050_REG_WHO_AM_I,
	ADDRESS_SIZE, &value, sizeof(value), 100);
	if (returnValue != HAL_OK) {
		return MPU6050_I2C_ERR;
	}
	return MPU6050_OK;
}

/**
 * @brief Resets MPU6050, disables the temperature sensor and chooses internal 8MHz oscillator as clock source
 * @retval Status of MPU6050 configuration
 */
int8_t MPU6050_configuration() {
	uint8_t buffer;
	__MPU6050_read(MPU6050_REG_PWR_MGMT_1, &buffer, sizeof(buffer));
	//Such that default value of register is 0x08
	buffer = (buffer & 0x10) | 0x08;
	return __MPU6050_write(MPU6050_REG_PWR_MGMT_1, &buffer, sizeof(buffer));
}

/**
 * @brief Configures accelerometer and gyroscope filter to max bandwidth
 * @retval Status of DLPF configuration
 */
int8_t MPU6050_DLPF_configuration() {
	uint8_t buffer;
	__MPU6050_read(MPU6050_REG_CONFIG, &buffer, sizeof(buffer));
	buffer = buffer & 0xC0;
	return __MPU6050_write(MPU6050_REG_CONFIG, &buffer, sizeof(buffer));
}

/**
 * @brief Reads  accelerometer and gyroscope data from MPU6050
 * @retval Status reads operation
 */
int8_t MPU6050_retrieve_data() {
	return __MPU6050_read(MPU6050_REG_ACCEL_XOUT_H, in_buff, DATA_BUFFER_SIZE);
}

/**
 * @brief Concatenates two byte in a single half-word.
 * @param b1 Most significant byte
 * @param b2 Less significant byte
 * @retval b1 concatenated b2
 */
int16_t __concatenate_buffer(uint8_t b1, uint8_t b2) {
	return ((uint16_t) b1 << 8) + b2;
}

/**
 * @brief Write offset value in specified registered
 * @param reg register in which write value
 * @param value value to write in the register
 */
void __write_in_offsetAccel_reg(int16_t reg, int16_t value) {
	value = value << 1; //LSB of acceleromter offset register is reserved

	uint8_t buffer[2] = { value >> 8, value & 0xff };
	uint8_t in_buffer[2];

	// First read from this register in a way to not change reserved bit
	__MPU6050_read(reg, in_buffer, sizeof(in_buffer));

	in_buffer[0] = in_buffer[0] & 0x00;
	in_buffer[1] = in_buffer[1] & 0x01;

	buffer[0] = buffer[0] | in_buffer[0];
	buffer[1] = buffer[1] | in_buffer[1];
	__MPU6050_write(reg, buffer, sizeof(buffer));

}

/**
 * @brief Stores accelerometer and gyro raw data in mpu instance declared in this module
 */
void MPU6050_save_data() {
	int16_t ax = __concatenate_buffer(in_buff[0], in_buff[1]);
	int16_t ay = __concatenate_buffer(in_buff[2], in_buff[3]);
	int16_t az = __concatenate_buffer(in_buff[4], in_buff[5]);
	int16_t gx = __concatenate_buffer(in_buff[8], in_buff[9]);
	int16_t gy = __concatenate_buffer(in_buff[10], in_buff[11]);
	int16_t gz = __concatenate_buffer(in_buff[12], in_buff[13]);
	set_accel_axes(&(mpu_t.a), ax, ay, az);
	set_gyro_axes(&(mpu_t.g), gx, gy, gz);
}

/**
 * @brief Gets last gx raw data read from MPU6050
 * @retval last gx raw data
 */
int16_t MPU6050_get_gx(){
	return get_gx(&(mpu_t.g));
}

/**
 * @brief Gets last gy raw data read from MPU6050
 * @retval last gy raw data
 */
int16_t MPU6050_get_gy(){
	return get_gy(&(mpu_t.g));
}

/**
 * @brief Gets last gz raw data read from MPU6050
 * @retval last gz raw data
 */
int16_t MPU6050_get_gz(){
	return get_gz(&(mpu_t.g));
}

/**
 * @brief Gets last ax raw data read from MPU6050
 * @retval last ax raw data
 */
int16_t MPU6050_get_ax(){
	return get_ax(&(mpu_t.a));
}

/**
 * @brief Gets last ay raw data read from MPU6050
 * @retval last ay raw data
 */
int16_t MPU6050_get_ay(){
	return get_ay(&(mpu_t.a));
}

/**
 * @brief Gets last az raw data read from MPU6050
 * @retval last az raw data
 */
int16_t MPU6050_get_az(){
	return get_az(&(mpu_t.a));
}




/**
 * @brief Gets accelerometer x axis offset
 * @retval accelerometer x axis offset
 */
int16_t get_AccelX_offset() {
	uint8_t buffer[2];
	int16_t read_value;

	__MPU6050_read(MPU6050_REG_ACCEL_XOFFS_H, buffer, sizeof(buffer));
	read_value = __concatenate_buffer(buffer[0], buffer[1]);
	//offset value in accelerometer offset registers are shifted left
	read_value = read_value >> 1;
	return read_value;

}

/**
 * @brief Gets accelerometer y axis offset
 * @retval accelerometer y axis offset
 */
int16_t get_AccelY_offset() {
	uint8_t buffer[2];
	int16_t read_value;

	__MPU6050_read(MPU6050_REG_ACCEL_YOFFS_H, buffer, sizeof(buffer));
	read_value = __concatenate_buffer(buffer[0], buffer[1]);
	//offset value in accelerometer offset registers are shifted left
	read_value = read_value >> 1;
	return read_value;

}

/**
 * @brief Gets accelerometer y axis offset
 * @retval accelerometer y axis offset
 */
int16_t get_AccelZ_offset() {
	uint8_t buffer[2];
	int16_t read_value;
	__MPU6050_read(MPU6050_REG_ACCEL_ZOFFS_H, buffer, sizeof(buffer));
	read_value = __concatenate_buffer(buffer[0], buffer[1]);
	//offset value in accelerometer offset registers are shifted left
	read_value = read_value >> 1;
	return read_value;

}

/**
 * @brief Sets accelerometer x axis offset
 * @param offset_x accelerometer x axis offset
 */
void _set_AccelX_offset(uint16_t offset_x) {
	__write_in_offsetAccel_reg(MPU6050_REG_ACCEL_XOFFS_H, offset_x);

}

/**
 * @brief Sets accelerometer y axis offset
 * @param offset_y accelerometer y axis offset
 */
void _set_AccelY_offset(uint16_t offset_y) {
	__write_in_offsetAccel_reg(MPU6050_REG_ACCEL_YOFFS_H, offset_y);
}

/**
 * @brief Sets accelerometer z axis offset
 * @param offset_z accelerometer z axis offset
 */
void _set_AccelZ_offset(uint16_t offset_z) {
	__write_in_offsetAccel_reg(MPU6050_REG_ACCEL_ZOFFS_H, offset_z);
}

/**
 * @brief Sets accelerometer axes offset
 * @param offset_x accelerometer x axis offset
 * @param offset_y accelerometer y axis offset
 * @param offset_z accelerometer z axis offset
 */
void set_Accel_offset(uint16_t offset_x, uint16_t offset_y, uint16_t offset_z) {
	_set_AccelX_offset(offset_x);
	_set_AccelY_offset(offset_y);
	_set_AccelZ_offset(offset_z);
}



/**
 * @brief Gets gyroscope x axis offset
 * @retval gyroscope x axis offset
 */
int16_t get_GyroX_offset() {
	uint8_t buffer[2];
	__MPU6050_read(MPU6050_REG_GYRO_XOFFS_H, buffer, sizeof(buffer));
	return __concatenate_buffer(buffer[0], buffer[1]);

}

/**
 * @brief Gets gyroscope y axis offset
 * @retval gyroscope y axis offset
 */
int16_t get_GyroY_offset() {
	uint8_t buffer[2];
	__MPU6050_read(MPU6050_REG_GYRO_YOFFS_H, buffer, sizeof(buffer));
	return __concatenate_buffer(buffer[0], buffer[1]);

}

/**
 * @brief Gets gyroscope z axis offset
 * @retval gyroscope z axis offset
 */
int16_t get_GyroZ_offset() {
	uint8_t buffer[2];
	__MPU6050_read(MPU6050_REG_GYRO_ZOFFS_H, buffer, sizeof(buffer));
	return __concatenate_buffer(buffer[0], buffer[1]);

}

/**
 * @brief Sets gyroscope x axis offset
 * @param offset_x gyroscope x axis offset
 */
void _set_GyroX_offset(uint16_t offset_x) {
	uint8_t buffer[2] = { (offset_x >> 8), offset_x & 0xff };
	__MPU6050_write(MPU6050_REG_GYRO_XOFFS_H, buffer, sizeof(buffer));

}

/**
 * @brief Sets gyroscope y axis offset
 * @param offset_y gyroscope y axis offset
 */
void _set_GyroY_offset(uint16_t offset_y) {
	uint8_t buffer[2] = { (offset_y >> 8), offset_y & 0xff };
	__MPU6050_write(MPU6050_REG_GYRO_YOFFS_H, buffer, sizeof(buffer));
}

/**
 * @brief Sets gyroscope z axis offset
 * @param offset_z gyroscope z axis offset
 */
void _set_GyroZ_offset(uint16_t offset_z) {
	uint8_t buffer[2] = { (offset_z >> 8), offset_z & 0xff };
	__MPU6050_write(MPU6050_REG_GYRO_ZOFFS_H, buffer, sizeof(buffer));
}

/**
 * @brief Sets gyroscope axes offset
 * @param offset_x gyroscope x axis offset
 * @param offset_y gyroscope y axis offset
 * @param offset_z gyroscope z axis offset
 */
void set_Gyro_offset(uint16_t offset_x, uint16_t offset_y, uint16_t offset_z) {
	_set_GyroX_offset(offset_x);
	_set_GyroY_offset(offset_y);
	_set_GyroZ_offset(offset_z);
}




/**
 * @brief Computes 10000 gyroscope reads from MPU6050 and averages them
 *
 * @param mean_x average on gyroscope x axis
 * @param mean_y average on gyroscope y axis
 * @param mean_z average on gyroscope z axis
 *
 */
void __meansensors(int64_t *mean_gx, int64_t *mean_gy, int64_t *mean_gz) {
	uint16_t i = 0;
	int64_t buff_gx = 0, buff_gy = 0, buff_gz = 0;
	int16_t  gx=0, gy=0, gz=0;
	uint8_t discardfirstmeas = 100;
	int16_t buffersize=10000;
	while (i < (buffersize + discardfirstmeas + 1)) {

		// read raw accel/gyro measurements from device
		if (MPU6050_retrieve_data() == HAL_OK) {
			MPU6050_save_data();


			gx = MPU6050_get_gx();
			gy = MPU6050_get_gy();
			gz = MPU6050_get_gz();

		}

		if (i > discardfirstmeas && i <= (buffersize + discardfirstmeas)) { //First 100 measures are discarded

			buff_gx = buff_gx + gx;
			buff_gy = buff_gy + gy;
			buff_gz = buff_gz + gz;

		}
		if (i == (buffersize + discardfirstmeas)) {

			*mean_gx = buff_gx / buffersize;
			*mean_gy = buff_gy / buffersize;
			*mean_gz = buff_gz / buffersize;
		}
		i++;
		HAL_Delay(2); //time delay shouldn't be too large or too small
	}
}




//
///**
// * @brief Computes calibration of gyroscope of MPU6050
// * @param gx_offset gyroscope x
// * @param gy_offset gyroscope y
// * @param gy_offset gyroscope z
// */
//int16_t calibration(int16_t *gx_offset, int16_t *gy_offset, int16_t *gz_offset) {
//
//	int64_t mean_gx, mean_gy, mean_gz;
//	__meansensors(&mean_gx, &mean_gy, &mean_gz,mpu);
//
//	int loop = 0;
//	char data3[1000];
//	//Gyro offset is 4 because the Offset register use 1000 dps, while we use 250 dps. In case of 1000 dps value of lsb/dps is 32.75,
//	//while with 250 dps is about 131. So the means is divided for 4.
//
//	int16_t gyro_offset_divisor = 4, gyro_deadzone = 1;
//
//	*gx_offset = -mean_gx / gyro_offset_divisor;
//	*gy_offset = -mean_gy / gyro_offset_divisor;
//	*gz_offset = -mean_gz / gyro_offset_divisor;
//
//	while (1) {
//		int ready = 0;
//		loop += 1;
//
//		set_Gyro_offset(*gx_offset, *gy_offset, *gz_offset);
//
//
//		__meansensors(&mean_gx, &mean_gy, &mean_gz);
//		// A specific offset is good if gyroscope averages value are lower than gyro_deadzone
//		if (abs(mean_gx) <= gyro_deadzone) {
//			ready++;
//			HAL_UART_Transmit(&huart2, (uint8_t*) data3,
//					sprintf(data3, "GX ready \t Loop: %d \r\n", loop), 1000);
//		} else {
//			*gx_offset = *gx_offset - mean_gx / (giro_deadzone + 1);
//		}
//
//		if (abs(mean_gy) <= gyro_deadzone) {
//			ready++;
//			HAL_UART_Transmit(&huart2, (uint8_t*) data3,
//					sprintf(data3, "GY ready \t Loop: %d \r\n", loop), 1000);
//		} else {
//			*gy_offset = *gy_offset - mean_gy / (giro_deadzone + 1);
//		}
//
//		if (abs(mean_gz) <= gyro_deadzone) {
//			ready++;
//			HAL_UART_Transmit(&huart2, (uint8_t*) data3,
//					sprintf(data3, "GZ ready \t Loop: %d \r\n", loop), 1000);
//		} else {
//			*gz_offset = *gz_offset - mean_gz / (giro_deadzone + 1);
//		}
//
//		if (ready == 3) {
//			return 0;
//		}
//
//	}
//	return -1;
//}
//
//
