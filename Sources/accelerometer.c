/*
 * accelerometer.c
 *
 *  Created on: 3 mars 2017
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
#include "accelerometer.h"


//-> choix de la sensiblite : p13 : 5.2


bool initAccelerometer()
{
	bool result = TRUE;
	result &= writeInSensorRegister(&ACC_CTRL_REG1, &STANDBY_MODE);
	result &= writeInSensorRegister(&ACC_RANGE_REG, &RANGE_4G);
	//parametrage du data rate ?
	result &= writeInSensorRegister(&ACC_CTRL_REG1, &ACTIVE_MODE);
	return result;
}

bool newAccelerationAvailable()
{
	bool result;
	uint8_t value;
	result = readFromSensorRegister(&ACC_STATUS_REG,&value);
	if(value & ACC_MASK_STATUS) return result;
	return FALSE;
}

bool readAcceleration(int8_t* buffer)
{
	bool result = TRUE;
	while(!newAccelerationAvailable()) {}
	//TODO: verifier comment fonctionne la lecture multi registre
	return result;
}
