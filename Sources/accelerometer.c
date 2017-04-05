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


bool initAccelerometer(LDD_TDeviceData* i2c_component)
{
	bool result = TRUE;
	result &= writeInSensorRegister(i2c_component, &ACC_CTRL_REG1, &STANDBY_MODE);
	result &= writeInSensorRegister(i2c_component, &ACC_RANGE_REG, &RANGE_4G);
	//parametrage du data rate ?
	result &= writeInSensorRegister(i2c_component, &ACC_CTRL_REG1, &ACTIVE_MODE);
	return result;
}

bool newAccelerationAvailable(LDD_TDeviceData* i2c_component)
{
	bool result;
	uint8_t value;
	result = readFromSensorRegister(i2c_component, &ACC_STATUS_REG,&value);
	if(value & ACC_MASK_STATUS) return result;
	return FALSE;
}

bool readAcceleration(LDD_TDeviceData* i2c_component, int8_t* buffer)
{
	bool result = TRUE;
	while(!newAccelerationAvailable(i2c_component)) {}
	//TODO: verifier comment fonctionne la lecture multi registre
	return result;
}

float convert(uint8_t* tab)
{
    float gPerLSB = MODE / 2048.0;
    int16_t concatData;
    if(tab[0]>=8)
    {
        int8_t tabI[2];
        tabI[0] = (int8_t) tab[0]+0xF0;
        tabI[1] = (int8_t) tab[1];
        concatData = (tabI[0] << 8) | tabI[1];
    }
    else
        concatData = tab[0] * 256 + tab[1];
    return concatData * gPerLSB;
}
