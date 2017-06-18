/// @file

#ifndef MPU6050_HPP
#define MPU6050_HPP

#include <math.h>
#include "hwlib.hpp"

/// \brief
/// library for mpu6050
/// \details
/// Library for the MPU 6050 sensor.
/// It contains all I2C-addresses necessary to communicate with the sensor, as well as the acceleration- and gyroscopic values of the sensor.
/// Futhermore, it contains functions to update said values, along with functions to calculate the pitch and roll based on said values.
/// Limitations:
/// -The MPU 6050 is installed 'sideways' on my breadboard, so the notions of 'pitch' and 'roll' are calculated differently than expected.
/// -The update rate is bound to that of the i2c_bus_bit_banged_scl_sda, so although the sensor could easily update at 400 kHz it will actually be somewhere below 100 kHz.
class mpu6050 {
private:
	const uint_fast8_t MPU_ADDR = 0x68;
	//Internal register addresses:
	const uint8_t PWR_MGMT = 0x6B;
	const uint8_t ACCEL_CONFIG = 0x1C;
	const uint8_t ACCEL_X_H = 0x3B;
	const uint8_t GYRO_CONFIG = 0x1B;
	const uint8_t GYRO_X_H = 0x43;
	//The following byte selects gyro-X to be our clock and wakes the device.
	const uint8_t clock_select[2] = {PWR_MGMT, 0x01};
	//The following byte will set the accelerometer to ±2g.
	const uint8_t accel_set_range[2] = {ACCEL_CONFIG, 0x00};
	//The following byte will set the gyroscope to 500 dps.
	const uint8_t gyro_set_range[2] = {GYRO_CONFIG, 0x08};

	hwlib::i2c_bus_bit_banged_scl_sda mpu;

	int16_t gX_offset = 0, gY_offset = 0, gZ_offset = 0;
	double roll_offset = 0.0, pitch_offset = 0.0; 
public:
/// \brief
/// constructor for mpu6050 class
/// \details
/// This constructor initialises our bit banged I2C-bus.
/// Furthermore, it writes some settings over that bus to select the internal clock and set various ranges.
	mpu6050 ( hwlib::i2c_bus_bit_banged_scl_sda mpu ):
		mpu (mpu)
	{
		mpu.write(MPU_ADDR, clock_select, 2);
		mpu.write(MPU_ADDR, accel_set_range, 2);
//		mpu.write(MPU_ADDR, gyro_set_range, 2);
	}
/// \brief
/// Sensor values
/// \details
/// All acceleration and gyroscopic variables are publicly accessible.
	int16_t accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z;
/// \brief
/// Raw gyroscopic values
/// \details
/// This function takes the X, Y and Z axis variables and places our gyroscopic values in them.
/// It is mainly used by the calibration function.
	void gyro_values (
		int16_t & gyro_x,
		int16_t & gyro_y,
		int16_t & gyro_z
	) {
		uint8_t values[6];
		mpu.write(MPU_ADDR, & GYRO_X_H, 1);
		mpu.read(MPU_ADDR, values, 6);
		gyro_x = (int16_t) (values[0]<<8|values[1]);
		gyro_y = (int16_t) (values[2]<<8|values[3]);
		gyro_z = (int16_t) (values[4]<<8|values[5]);
	}
/// \brief
/// Update public sensor values
/// \details
/// This function updates the public sensor values, subtracted by the calibration values.
/// The acceleration values are not offset by calibration values because they may differ according to the initial orientation of our sensor.
	void get_values () {
		uint8_t values[14];
		mpu.write(MPU_ADDR, & ACCEL_X_H, 1);
		mpu.read(MPU_ADDR, values, 14);

		accel_x = (int16_t) values[0]<<8|values[1];
		accel_y = (int16_t) values[2]<<8|values[3];
		accel_z = (int16_t) values[4]<<8|values[5];
		gyro_x = (int16_t) (values[8]<<8|values[9]) - gX_offset;
		gyro_y = (int16_t) (values[10]<<8|values[11]) - gY_offset;
		gyro_z = (int16_t) (values[12]<<8|values[13]) - gZ_offset;
	}
/// \brief
/// Calibrate gyro values
/// \details
/// This function calibrates the gyroscopic values, as well as the pitch- and roll doubles.
/// It takes a uint16_t (max. 65535) value as the amount of calibrations to do.
/// It doesn't calibrate the raw acceleration values because those are dependent on the orientation of the sensor.
	void calibrate (uint16_t amount_of_calibrations = 1000 ) {
		int32_t gX = 0, gY = 0, gZ = 0;
		for (int i = 0; i < amount_of_calibrations; i++) {
			gyro_values(gX_offset, gY_offset, gZ_offset);
			gX += gX_offset;
			gY += gY_offset;
			gZ += gZ_offset;
		}
		gX_offset = gX / amount_of_calibrations;
		gY_offset = gY / amount_of_calibrations;
		gZ_offset = gZ / amount_of_calibrations;
	}
/// \brief
/// Print all sensor values
/// \details
/// This function prints the acceleration- and gyroscopic values to the cout function included in hwlib.
/// I would have used setw from iomanip, were it not that it gives errors when trying to use it with hwlib.
/// It is for this reason that the alignment of the text may vary.
	void print() {
		hwlib::cout << "|acX: " << accel_x
			<< "|acY: " << accel_y
			<< "|acZ: " << accel_z
			<< "|gyX: " << gyro_x
			<< "|gyY: " << gyro_y
			<< "|gyZ: " << gyro_z 
		<< "\n";
	}
/// \brief
/// Calculate roll amount
/// \details
/// This function calculates the roll amount in degrees by dividing the Z-axis by the X-axis.
/// This is because the sensor is placed 'sideways' on my breadboard.
/// If the sensor was orientated to have its chip components facing skywards, you could calculate roll using atan(-accel_x / accel_z).
/// The range is [-90, 90] for both roll and pitch.
	double get_roll () {
		return atan(accel_z / (double) accel_x) * 57.3; //times 57.3 to convert from radians to degrees
	}
/// \brief
/// Calculate pitch amount
/// \details
/// This function calculates the pitch in (almost) the same way as our roll, only it divides the Y-axis by the length of the combined vector of X and Z.
	double get_pitch () {
		return -atan(accel_y / (double) sqrt(accel_x * accel_x + accel_z * accel_z)) * 57.3;
	}
};
#endif // MPU6050_HPP
