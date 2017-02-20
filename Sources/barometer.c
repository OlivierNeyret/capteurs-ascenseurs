/*
 * barometer.c
 *
 *  Created on: 20 févr. 2017
 *      Author: Olivier Neyret
 */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "CI2C1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
/* User includes */
#include "barometer.h"

bool initBarometer()
{
	bool result = TRUE;
	CI2C1_SelectSlaveDevice(i2c_component,LDD_I2C_ADDRTYPE_7BITS,BAR_ADDRESS);
	result &= writeInSensorRegister(&BAR_CTRL_REG1, &INIT_ALTI);
	result &= writeInSensorRegister(&BAR_FLAG_REG, &INIT_FLAGS);
	result &= writeInSensorRegister(&BAR_CTRL_REG1, &ACTIVE_MODE);
	return result;
}

bool newBarometerDataAvailable()
{
	bool result;
	uint8_t value;
	CI2C1_SelectSlaveDevice(i2c_component,LDD_I2C_ADDRTYPE_7BITS,BAR_ADDRESS);
	result = readFromSensorRegister(&BAR_STATUS_REG,&value);
	if(value & BAR_MASK_STATUS) return result;
	return FALSE;
}

bool readAltitude(uint8_t* buffer)
{
	bool result = TRUE;
	while(!newBarometerDataAvailable()) {}
	result &= readFromSensorRegister(&BAR_DATA_MSB,buffer);
	result &= readFromSensorRegister(&BAR_DATA_CSB,buffer+1);
	result &= readFromSensorRegister(&BAR_DATA_LSB,buffer+2);
	return result;
}
