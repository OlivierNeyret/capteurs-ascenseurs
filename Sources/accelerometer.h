/*
 * accelerometer.h
 *
 *  Created on: 3 mars 2017
 *      Author: olivier
 */

#ifndef SOURCES_ACCELEROMETER_H_
#define SOURCES_ACCELEROMETER_H_

// Slave address for i2c
static const LDD_I2C_TAddr ACC_ADDRESS = 0x1D;

// Register addresses
static const uint8_t ACC_CTRL_REG1 = 0x2A;
static const uint8_t ACC_RANGE_REG = 0x0E;
static const uint8_t ACC_STATUS_REG = 0x00;

// Values to put in registers during initialization of accelerometer
static const uint8_t STANDBY_MODE = 0x00; // put the sensor in standby mode
static const uint8_t ACTIVE_MODE = 0x01; // put the sensor in active mode
static const uint8_t RANGE_2G = 0x00;
static const uint8_t RANGE_4G = 0x01;
static const uint8_t RANGE_8G = 0x02;
static const uint8_t ACC_MASK_STATUS = 0x08;

bool initAccelerometer();

#endif /* SOURCES_ACCELEROMETER_H_ */
