/*
 * barometer.h
 *
 *  Created on: 20 févr. 2017
 *      Author: Olivier Neyret
 */

#ifndef SOURCES_BAROMETER_H_
#define SOURCES_BAROMETER_H_

// Slave address for i2c
const LDD_I2C_TAddr BAR_ADDRESS = 0x60;

// Register addresses
const uint8_t BAR_CTRL_REG1 = 0x26;
const uint8_t BAR_FLAG_REG = 0x13; //PT_DATA_CFG in official documentation
const uint8_t BAR_STATUS_REG = 0x06;
const uint8_t BAR_DATA_MSB = 0x01;
const uint8_t BAR_DATA_CSB = 0x02;
const uint8_t BAR_DATA_LSB = 0x03;

// Values to put in registers during initialization
const uint8_t INIT_ALTI = 0x80; // activate the altimeter mode, sensor stays in standby
const uint8_t INIT_FLAGS = 0x06; // flag will be raised when a new altitude data is available
const uint8_t ACTIVE_MODE = 0x81; // altimeter mode and active mode

const uint8_t BAR_MASK_STATUS = 0x08;

bool initBarometer(LDD_DeviceData* i2c_component);
bool newBarometerDataAvailable(LDD_DeviceData* i2c_component);
bool readAltitude(LDD_DeviceData* i2c_component);

#endif /* SOURCES_BAROMETER_H_ */
