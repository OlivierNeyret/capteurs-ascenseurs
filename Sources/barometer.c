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
	result &= writeInSensorRegister(&BAR_CTRL_REG1, &INIT_ALTI);
	result &= writeInSensorRegister(&BAR_FLAG_REG, &INIT_FLAGS);
	result &= writeInSensorRegister(&BAR_CTRL_REG1, &ACTIVE_MODE);
	return result;
}

bool newBarometerDataAvailable()
{
	bool result;
	uint8_t value;
	result = readFromSensorRegister(&BAR_STATUS_REG,&value);
	if(value & BAR_MASK_STATUS) return result;
	return FALSE;
}

bool readAltitude(int8_t* buffer)
{
	bool result = TRUE;
	while(!newBarometerDataAvailable()) {}
	result &= readFromSensorRegister(&BAR_DATA_MSB,buffer);
	result &= readFromSensorRegister(&BAR_DATA_CSB,buffer+1);
	result &= readFromSensorRegister(&BAR_DATA_LSB,buffer+2);
	buffer[2] = buffer[2] >> 4;
	return result;
}

float convertQ164toFloat(int8_t* q164_tab)
{
	float result;
	int32_t q164Concat = q164_tab[0] * 4096 + q164_tab[1] * 16 + q164_tab[2]; //les multiplications permettent de caler correctement les valeurs du tableau
	result = (float) q164Concat;
	result *= 0.0625; //Because 0.0625 = 2^-4 and 4 is the number of fractional bits
	return result;
}
